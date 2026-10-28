// SPDX-License-Identifier: GPL-2.0
/*
 * TEE driver for goodix fingerprint sensor
 * Copyright (C) 2016 Goodix
 * Copyright (C) 2022 Jebaitedneko <Jebaitedneko@gmail.com>
 */

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/irq.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeup.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <net/netlink.h>
#include <net/sock.h>

static struct gf_dev {
	dev_t devt;
	struct list_head device_entry;
	struct platform_device *spi;
	struct input_dev *input;
	struct wakeup_source *fp_wakelock;
	signed irq_gpio, rst_gpio;
	int irq;
	bool irq_requested;
	bool wake_enabled;
	bool present;
	unsigned int users;
} gf;

/* Protects device presence, users, GPIO/IRQ ownership, and wake state. */
static DEFINE_MUTEX(gf_lock);

#define MAX_MSGSIZE 2
#define NETLINK_TEST 25
static struct sock *nl_sk;
static u32 portid;

static void sendnlmsg(void)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	u32 dst = READ_ONCE(portid);
	char msg[MAX_MSGSIZE] = { 1, 0 };

	/* No daemon has registered yet, or the socket failed to initialize. */
	if (!dst || !nl_sk)
		return;

	skb = alloc_skb(NLMSG_SPACE(MAX_MSGSIZE), GFP_KERNEL);
	if (!skb)
		return;
	nlh = nlmsg_put(skb, 0, 0, 0, MAX_MSGSIZE, 0);
	if (!nlh) {
		kfree_skb(skb);
		return;
	}
	memcpy(NLMSG_DATA(nlh), msg, MAX_MSGSIZE);
	NETLINK_CB(skb).portid = NETLINK_CB(skb).dst_group = 0;
	netlink_unicast(nl_sk, skb, dst, MSG_DONTWAIT);
}

static void nl_data_ready(struct sk_buff *skb)
{
	if (skb->len < sizeof(struct nlmsghdr))
		return;

	/* Use the socket's actual port ID, not the caller-supplied header. */
	WRITE_ONCE(portid, NETLINK_CB(skb).portid);
}

static int netlink_init(void)
{
	struct netlink_kernel_cfg netlink_cfg = {
		.input = nl_data_ready,
	};

	nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &netlink_cfg);
	if (!nl_sk) {
		pr_err("gf: failed to create netlink socket\n");
		return -ENOMEM;
	}
	return 0;
}

static void netlink_exit(void)
{
	netlink_kernel_release(nl_sk);
	nl_sk = NULL;
	WRITE_ONCE(portid, 0);
}

/* All callers hold gf_lock. Wake state is not the same as IRQ ownership. */
static int gf_set_irq_wake(struct gf_dev *gf_dev, bool enable)
{
	int ret;

	if (!gf_dev->irq_requested)
		return -ENODEV;
	if (gf_dev->wake_enabled == enable)
		return 0;

	if (enable)
		ret = enable_irq_wake(gf_dev->irq);
	else
		ret = disable_irq_wake(gf_dev->irq);
	if (!ret)
		WRITE_ONCE(gf_dev->wake_enabled, enable);
	return ret;
}

#define WAKELOCK_HOLD_TIME 400
static inline irqreturn_t gf_irq(int irq, void *handle) {
	struct gf_dev *gf_dev = handle;

	if (!gf_dev)
		return IRQ_HANDLED;

	/*
	 * Only keep the AP awake when userspace armed the wake source.  The
	 * IRQ stays unmasked while the HAL has it disabled, so a stray trigger
	 * used to hold the system out of suspend for 400 ms for nothing.
	 * gf_lock must not be taken here: gf_cleanup() holds it across
	 * free_irq(), which waits for this thread to return.
	 */
	if (READ_ONCE(gf_dev->wake_enabled) && gf_dev->fp_wakelock)
		__pm_wakeup_event(gf_dev->fp_wakelock, WAKELOCK_HOLD_TIME);

	sendnlmsg();
	return IRQ_HANDLED;
}

static int gf_setup(struct gf_dev *gf_dev)
{
	struct device *dev = &gf_dev->spi->dev;
	int rst_gpio, irq_gpio, irq, ret;

	rst_gpio = of_get_named_gpio(dev->of_node, "fp-gpio-reset", 0);
	if (rst_gpio < 0)
		return rst_gpio;
	ret = gpio_request(rst_gpio, "goodix_reset");
	if (ret)
		return ret;
	ret = gpio_direction_output(rst_gpio, 1);
	if (ret)
		goto err_reset;

	irq_gpio = of_get_named_gpio(dev->of_node, "fp-gpio-irq", 0);
	if (irq_gpio < 0) {
		ret = irq_gpio;
		goto err_reset;
	}
	ret = gpio_request(irq_gpio, "goodix_irq");
	if (ret)
		goto err_reset;
	ret = gpio_direction_input(irq_gpio);
	if (ret)
		goto err_irq_gpio;
	irq = gpio_to_irq(irq_gpio);
	if (irq < 0) {
		ret = irq;
		goto err_irq_gpio;
	}
	ret = request_threaded_irq(irq, NULL, gf_irq,
			IRQF_TRIGGER_RISING | IRQF_ONESHOT, "gf", gf_dev);
	if (ret)
		goto err_irq_gpio;

	gf_dev->rst_gpio = rst_gpio;
	gf_dev->irq_gpio = irq_gpio;
	gf_dev->irq = irq;
	gf_dev->irq_requested = true;
	/*
	 * Leave the wake source disarmed until userspace asks for it with
	 * GF_IOC_ENABLE_IRQ.  Arming it here kept the fingerprint IRQ able to
	 * resume the AP, and to take a 400 ms wakelock per trigger, for as
	 * long as the HAL held the fd - even while the sensor was idle.
	 */
	return 0;

err_irq_gpio:
	gpio_free(irq_gpio);
err_reset:
	gpio_free(rst_gpio);
	return ret;
}

static void gf_cleanup(struct gf_dev *gf_dev)
{
	if (gf_dev->irq_requested) {
		int ret = gf_set_irq_wake(gf_dev, false);

		if (ret)
			dev_warn(&gf_dev->spi->dev,
				 "failed to disable IRQ wake (%d); wake may remain enabled\n",
				 ret);
		free_irq(gf_dev->irq, gf_dev);
		gf_dev->irq_requested = false;
		gf_dev->wake_enabled = false;
		gf_dev->irq = -EINVAL;
	}
	if (gpio_is_valid(gf_dev->irq_gpio)) {
		gpio_free(gf_dev->irq_gpio);
		gf_dev->irq_gpio = -EINVAL;
	}
	if (gpio_is_valid(gf_dev->rst_gpio)) {
		gpio_free(gf_dev->rst_gpio);
		gf_dev->rst_gpio = -EINVAL;
	}
}

static int gpio_reset(struct gf_dev *gf_dev)
{
	int ret = gpio_direction_output(gf_dev->rst_gpio, 1);

	if (ret)
		return ret;
	gpio_set_value(gf_dev->rst_gpio, 0);
	mdelay(3);
	gpio_set_value(gf_dev->rst_gpio, 1);
	mdelay(3);
	return 0;
}

#define GF_IOC_MAGIC 'g'
#define GF_IOC_INIT _IOR(GF_IOC_MAGIC, 0, uint8_t)
#define GF_IOC_RESET _IO(GF_IOC_MAGIC, 2)
#define GF_IOC_ENABLE_IRQ _IO(GF_IOC_MAGIC, 3)
#define GF_IOC_DISABLE_IRQ _IO(GF_IOC_MAGIC, 4)

static long gf_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct gf_dev *gf_dev = filp->private_data;
	u8 netlink_route = NETLINK_TEST;
	int ret = 0;

	mutex_lock(&gf_lock);
	if (!gf_dev || !gf_dev->present) {
		ret = -ENODEV;
		goto out;
	}
	switch (cmd) {
	case GF_IOC_INIT:
		if (copy_to_user((void __user *)arg, &netlink_route, sizeof(netlink_route)))
			ret = -EFAULT;
		break;
	case GF_IOC_DISABLE_IRQ:
		ret = gf_set_irq_wake(gf_dev, false);
		break;
	case GF_IOC_ENABLE_IRQ:
		ret = gf_set_irq_wake(gf_dev, true);
		break;
	case GF_IOC_RESET:
		ret = gpio_reset(gf_dev);
		break;
	default:
		break;
	}
out:
	mutex_unlock(&gf_lock);
	return ret;
}

static int gf_open(struct inode *inode, struct file *filp)
{
	struct gf_dev *gf_dev = &gf;
	int ret = nonseekable_open(inode, filp);

	if (ret)
		return ret;
	mutex_lock(&gf_lock);
	if (!gf_dev->present) {
		ret = -ENODEV;
		goto out;
	}
	if (!gf_dev->users) {
		ret = gf_setup(gf_dev);
		if (ret) {
			dev_err(&gf_dev->spi->dev,
				"failed to set up fingerprint GPIO/IRQ (%d)\n", ret);
			goto out;
		}
		ret = gpio_reset(gf_dev);
		if (ret) {
			gf_cleanup(gf_dev);
			goto out;
		}
	}
	gf_dev->users++;
	filp->private_data = gf_dev;
out:
	mutex_unlock(&gf_lock);
	return ret;
}

static int gf_release(struct inode *inode, struct file *filp)
{
	struct gf_dev *gf_dev = filp->private_data;

	mutex_lock(&gf_lock);
	filp->private_data = NULL;
	if (gf_dev && gf_dev->users && !--gf_dev->users)
		gf_cleanup(gf_dev);
	mutex_unlock(&gf_lock);
	return 0;
}

static const struct file_operations gf_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = gf_ioctl,
	.open = gf_open,
	.release = gf_release,
};

static struct class *gf_class;
#define N_SPI_MINORS 256
static DECLARE_BITMAP(minors, N_SPI_MINORS);
static LIST_HEAD(device_list);
static int SPIDEV_MAJOR;
#define GF_DEV_NAME "goodix_fp"
#define GF_INPUT_NAME "uinput-goodix"
static int gf_probe(struct platform_device *pdev)
{
	struct gf_dev *gf_dev = &gf;
	struct device *class_dev;
	unsigned long minor;
	int ret;

	mutex_lock(&gf_lock);
	/* This driver has one static device; don't reuse it with old fds open. */
	if (gf_dev->present || gf_dev->users) {
		ret = -EBUSY;
		goto out;
	}
	minor = find_first_zero_bit(minors, N_SPI_MINORS);
	if (minor >= N_SPI_MINORS) {
		ret = -ENODEV;
		goto out;
	}
	gf_dev->spi = pdev;
	gf_dev->irq = gf_dev->irq_gpio = gf_dev->rst_gpio = -EINVAL;
	gf_dev->input = input_allocate_device();
	if (!gf_dev->input) {
		ret = -ENOMEM;
		goto err_device;
	}
	gf_dev->input->name = GF_INPUT_NAME;
	ret = input_register_device(gf_dev->input);
	if (ret) {
		input_free_device(gf_dev->input);
		gf_dev->input = NULL;
		goto err_device;
	}
	gf_dev->fp_wakelock = wakeup_source_register(&pdev->dev, "fp_wakelock");
	if (!gf_dev->fp_wakelock)
		dev_warn(&pdev->dev, "Failed to register wakelock\n");

	gf_dev->devt = MKDEV(SPIDEV_MAJOR, minor);
	class_dev = device_create(gf_class, &pdev->dev, gf_dev->devt, gf_dev,
				  GF_DEV_NAME);
	if (IS_ERR(class_dev)) {
		ret = PTR_ERR(class_dev);
		if (gf_dev->fp_wakelock)
			wakeup_source_unregister(gf_dev->fp_wakelock);
		gf_dev->fp_wakelock = NULL;
		input_unregister_device(gf_dev->input);
		gf_dev->input = NULL;
		goto err_device;
	}
	INIT_LIST_HEAD(&gf_dev->device_entry);
	set_bit(minor, minors);
	list_add(&gf_dev->device_entry, &device_list);
	gf_dev->present = true;
	mutex_unlock(&gf_lock);
	return 0;

err_device:
	gf_dev->spi = NULL;
out:
	mutex_unlock(&gf_lock);
	return ret;
}

static int gf_remove(struct platform_device *pdev)
{
	struct gf_dev *gf_dev = &gf;
	struct wakeup_source *fp_wakelock;
	struct input_dev *input;

	mutex_lock(&gf_lock);
	gf_dev->present = false;
	/* free_irq waits for the IRQ thread before its wakelock is destroyed. */
	gf_cleanup(gf_dev);
	list_del(&gf_dev->device_entry);
	clear_bit(MINOR(gf_dev->devt), minors);
	device_destroy(gf_class, gf_dev->devt);
	fp_wakelock = gf_dev->fp_wakelock;
	input = gf_dev->input;
	gf_dev->fp_wakelock = NULL;
	gf_dev->input = NULL;
	gf_dev->spi = NULL;
	mutex_unlock(&gf_lock);

	if (fp_wakelock)
		wakeup_source_unregister(fp_wakelock);
	if (input)
		input_unregister_device(input);
	return 0;
}

static const struct of_device_id gx_match_table[] = {
	{.compatible = "goodix,fingerprint"},
	{},
};

static struct platform_driver gf_driver = {
	.driver = {
		.name = GF_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = gx_match_table,
	},
	.probe = gf_probe,
	.remove = gf_remove,
};

#define CHRD_DRIVER_NAME "goodix_fp_spi"
#define CLASS_NAME "goodix_fp"
static int __init gf_init(void)
{
	int ret;

	ret = register_chrdev(0, CHRD_DRIVER_NAME, &gf_fops);
	if (ret < 0)
		return ret;
	SPIDEV_MAJOR = ret;
	gf_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(gf_class)) {
		ret = PTR_ERR(gf_class);
		goto err_chrdev;
	}
	ret = netlink_init();
	if (ret)
		goto err_class;
	ret = platform_driver_register(&gf_driver);
	if (ret)
		goto err_netlink;
	return 0;

err_netlink:
	netlink_exit();
err_class:
	class_destroy(gf_class);
err_chrdev:
	unregister_chrdev(SPIDEV_MAJOR, CHRD_DRIVER_NAME);
	return ret;
}
module_init(gf_init);

static void __exit gf_exit(void)
{
	/* Removing the platform device quiesces IRQs before releasing netlink. */
	platform_driver_unregister(&gf_driver);
	netlink_exit();
	class_destroy(gf_class);
	unregister_chrdev(SPIDEV_MAJOR, CHRD_DRIVER_NAME);
}
module_exit(gf_exit);

MODULE_AUTHOR("Jiangtao Yi, <yijiangtao@goodix.com>");
MODULE_AUTHOR("Jandy Gou, <gouqingsong@goodix.com>");
MODULE_AUTHOR("Jebaitedneko, <Jebaitedneko@gmail.com>");
MODULE_DESCRIPTION("goodix fingerprint sensor device driver");
MODULE_LICENSE("GPL");
