// SPDX-License-Identifier: GPL-2.0
/* Exact-source misc file operation tests, including live FD across rebind. */
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>

typedef uint64_t u64;
#define __user
#define GFP_KERNEL 0
#define THIS_MODULE NULL
#define AW8624_HAPTIC_IOCTL_MAGIC 'h'
#define AW8624_HAPTIC_CMD_READ_REG 0
#define AW8624_HAPTIC_CMD_WRITE_REG 1
#define MAX_I2C_BUFFER_SIZE 65536
#define IS_ERR(p) ((uintptr_t)(p) >= (uintptr_t)(-(intptr_t)4095))
#define PTR_ERR(p) ((long)(intptr_t)(p))
#define mutex_lock(m) assert(!pthread_mutex_lock(m))
#define mutex_unlock(m) assert(!pthread_mutex_unlock(m))
#define dev_err(...)                                                                               \
	do {                                                                                       \
	} while (0)
struct inode {
	int unused;
};
struct file {
	void *private_data;
};
struct aw8624_file_context {
	u64 generation;
};
struct aw8624 {
	pthread_mutex_t lock;
	u64 bind_generation;
	void *dev;
	struct {
		unsigned char cmd, reg;
	} fileops;
};
static pthread_mutex_t aw8624_file_lock = PTHREAD_MUTEX_INITIALIZER;
static struct aw8624 *g_aw8624;
static int module_refs, fail_user_copy, i2c_error;
static pthread_mutex_t gate_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gate_cond = PTHREAD_COND_INITIALIZER;
static bool hold_read, read_started, finish_read, unbind_started, unbind_finished;
static void *kzalloc(size_t n, int flags)
{
	(void)flags;
	return calloc(1, n ? n : 1);
}
static void kfree(void *p)
{
	free(p);
}
static void *memdup_user(const void *p, size_t n)
{
	if (fail_user_copy || !p)
		return (void *)(intptr_t)-EFAULT;
	void *v = malloc(n);

	if (!v)
		return (void *)(intptr_t)-ENOMEM;
	memcpy(v, p, n);
	return v;
}
static int copy_to_user(void *dst, const void *src, size_t n)
{
	if (fail_user_copy || !dst)
		return 1;
	memcpy(dst, src, n);
	return 0;
}
static bool try_module_get(void *m)
{
	(void)m;
	module_refs++;
	return true;
}
static void module_put(void *m)
{
	(void)m;
	module_refs--;
}
static int aw8624_i2c_read(struct aw8624 *a, unsigned char reg, unsigned char *out)
{
	(void)a;
	pthread_mutex_lock(&gate_lock);
	if (hold_read) {
		hold_read = false;
		read_started = true;
		pthread_cond_broadcast(&gate_cond);
		while (!finish_read)
			pthread_cond_wait(&gate_cond, &gate_lock);
	}
	pthread_mutex_unlock(&gate_lock);
	if (i2c_error)
		return -EIO;
	*out = reg;
	return reg;
}
static int aw8624_i2c_write(struct aw8624 *a, unsigned char reg, unsigned char val)
{
	(void)a;
	(void)reg;
	(void)val;
	return i2c_error ? -EIO : 0;
}

/* EXTRACT aw8624_file_open */
/* EXTRACT aw8624_file_get */
/* EXTRACT aw8624_file_release */
/* EXTRACT aw8624_file_unlocked_ioctl */
/* EXTRACT aw8624_file_read */
/* EXTRACT aw8624_file_write */

static void *reader(void *arg)
{
	char data[2];

	assert(aw8624_file_read(arg, data, sizeof(data), NULL) == 2);
	return NULL;
}
static void *unbind(void *arg)
{
	struct aw8624 *a = arg;

	pthread_mutex_lock(&gate_lock);
	unbind_started = true;
	pthread_cond_broadcast(&gate_cond);
	pthread_mutex_unlock(&gate_lock);
	pthread_mutex_lock(&aw8624_file_lock);
	if (g_aw8624 == a)
		g_aw8624 = NULL;
	pthread_mutex_unlock(&aw8624_file_lock);
	pthread_mutex_destroy(&a->lock);
	free(a);
	pthread_mutex_lock(&gate_lock);
	unbind_finished = true;
	pthread_cond_broadcast(&gate_cond);
	pthread_mutex_unlock(&gate_lock);
	return NULL;
}
int main(void)
{
	struct file old = {0}, newer = {0};
	struct inode inode = {0};
	struct aw8624 *a = calloc(1, sizeof(*a));

	assert(a);
	pthread_mutex_init(&a->lock, NULL);
	a->bind_generation = 1;
	g_aw8624 = a;
	assert(aw8624_file_open(&inode, &old) == 0 && module_refs == 1);
	assert(aw8624_file_unlocked_ioctl(&old, _IO('x', 1), 0) == -EINVAL);
	assert(aw8624_file_unlocked_ioctl(&old, _IO('h', 1), 0) == 0);
	char readcmd[] = {0, 4}, writecmd[] = {1, 4, 7, 8}, data[2] = {0};

	assert(aw8624_file_write(&old, readcmd, 2, NULL) == 2);
	assert(aw8624_file_read(&old, data, 2, NULL) == 2 && data[0] == 4 && data[1] == 5);
	fail_user_copy = 1;
	assert(aw8624_file_read(&old, data, 2, NULL) == -EFAULT);
	fail_user_copy = 0;
	assert(aw8624_file_write(&old, writecmd, 4, NULL) == 4);
	assert(aw8624_file_write(&old, readcmd, 0, NULL) == -EINVAL);
	assert(aw8624_file_write(&old, readcmd, 65537, NULL) == -EINVAL);
	fail_user_copy = 1;
	assert(aw8624_file_write(&old, readcmd, 2, NULL) == -EFAULT);
	fail_user_copy = 0;
	i2c_error = 1;
	assert(aw8624_file_write(&old, writecmd, 4, NULL) == -EIO);
	i2c_error = 0;
	assert(aw8624_file_write(&old, readcmd, 2, NULL) == 2);
	pthread_t read_thread, unbind_thread;

	hold_read = true;
	assert(!pthread_create(&read_thread, NULL, reader, &old));
	pthread_mutex_lock(&gate_lock);
	while (!read_started)
		pthread_cond_wait(&gate_cond, &gate_lock);
	pthread_mutex_unlock(&gate_lock);
	assert(!pthread_create(&unbind_thread, NULL, unbind, a));
	pthread_mutex_lock(&gate_lock);
	while (!unbind_started)
		pthread_cond_wait(&gate_cond, &gate_lock);
	assert(!unbind_finished);
	finish_read = true;
	pthread_cond_broadcast(&gate_cond);
	pthread_mutex_unlock(&gate_lock);
	assert(!pthread_join(read_thread, NULL));
	assert(!pthread_join(unbind_thread, NULL));
	assert(unbind_finished);
	assert(aw8624_file_read(&old, data, 2, NULL) == -ENODEV);
	a = calloc(1, sizeof(*a));
	assert(a);
	pthread_mutex_init(&a->lock, NULL);
	a->bind_generation = 2;
	g_aw8624 = a;
	assert(aw8624_file_write(&old, readcmd, 2, NULL) == -ENODEV);
	assert(aw8624_file_unlocked_ioctl(&old, _IO('h', 1), 0) == -ENODEV);
	assert(aw8624_file_open(&inode, &newer) == 0 && module_refs == 2);
	assert(aw8624_file_write(&newer, readcmd, 2, NULL) == 2);
	assert(aw8624_file_release(&inode, &old) == 0);
	assert(aw8624_file_release(&inode, &newer) == 0 && module_refs == 0);
	g_aw8624 = NULL;
	pthread_mutex_destroy(&a->lock);
	free(a);
	puts("AW8624 extracted-code tests: misc operation/unbind and stale FD rebind races passed");
	return 0;
}
