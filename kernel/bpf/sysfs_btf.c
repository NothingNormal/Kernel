// SPDX-License-Identifier: GPL-2.0
/*
 * Provide kernel BTF information for introspection and use by eBPF tools.
 *
 * TODO: miatoll builds without DEBUG_INFO, so there is no vmlinux BTF and
 * CO-RE programs fail to load (Android timeInState, BPF_CORE_READ on
 * task_struct->pid in raw_tp/sched_process_free). Full DEBUG_INFO_BTF needs
 * pahole on the whole LTO vmlinux DWARF, which takes too much RAM. Cheaper
 * way: build one non-LTO object with -g ($(DISABLE_LTO)) that uses the
 * needed types (task_struct, ...), run pahole -J on just that object,
 * llvm-objcopy --dump-section .BTF, .incbin the blob and serve it here as
 * /sys/kernel/btf/vmlinux. Use own symbols, not __start_BTF, so the
 * verifier (btf_vmlinux, DEBUG_INFO_BTF only) never parses the partial BTF.
 * CI then needs pahole (dwarves); pahole >= 1.24 emits ENUM64, fine for
 * libbpf since the kernel does not parse this blob.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/init.h>
#include <linux/sysfs.h>

/* See scripts/link-vmlinux.sh, gen_btf() func for details */
extern char __weak __start_BTF[];
extern char __weak __stop_BTF[];

static ssize_t
btf_vmlinux_read(struct file *file, struct kobject *kobj,
		 struct bin_attribute *bin_attr,
		 char *buf, loff_t off, size_t len)
{
	memcpy(buf, __start_BTF + off, len);
	return len;
}

static struct bin_attribute bin_attr_btf_vmlinux __ro_after_init = {
	.attr = { .name = "vmlinux", .mode = 0444, },
	.read = btf_vmlinux_read,
};

struct kobject *btf_kobj;

static int __init btf_vmlinux_init(void)
{
	bin_attr_btf_vmlinux.size = __stop_BTF - __start_BTF;

	if (!__start_BTF || bin_attr_btf_vmlinux.size == 0)
		return 0;

	btf_kobj = kobject_create_and_add("btf", kernel_kobj);
	if (!btf_kobj)
		return -ENOMEM;

	return sysfs_create_bin_file(btf_kobj, &bin_attr_btf_vmlinux);
}

subsys_initcall(btf_vmlinux_init);
