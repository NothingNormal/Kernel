// SPDX-License-Identifier: GPL-2.0
/* Exact-source async firmware lifecycle tests using pthread-backed mocks. */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef uint8_t u8;
typedef uint32_t u32;
#define READ_ONCE(x) __atomic_load_n(&(x), __ATOMIC_SEQ_CST)
#define WRITE_ONCE(x, v) __atomic_store_n(&(x), (v), __ATOMIC_SEQ_CST)
#define mutex_lock(m) assert(!pthread_mutex_lock(m))
#define mutex_unlock(m) assert(!pthread_mutex_unlock(m))
#define pr_debug(...)                                                                              \
	do {                                                                                       \
	} while (0)
#define pr_err(...)                                                                                \
	do {                                                                                       \
	} while (0)
#define THIS_MODULE NULL
#define GFP_KERNEL 0
#define FW_ACTION_HOTPLUG 0
#define AWINIC_RAM_UPDATE_DELAY 1
#define AW8624_RTP_NAME_MAX 64
#define AW8624_HAPTIC_RTP_MODE 2
#define AW8624_REG_SYSCTRL 1
#define AW8624_BIT_SYSCTRL_RAMINIT_MASK 2
#define AW8624_BIT_SYSCTRL_RAMINIT_EN 3
#define AW8624_BIT_SYSCTRL_RAMINIT_OFF 4
#define AW8624_REG_BASE_ADDRH 5
#define AW8624_REG_BASE_ADDRL 6
#define AW8624_REG_FIFO_AEH 7
#define AW8624_REG_FIFO_AEL 8
#define AW8624_REG_FIFO_AFH 9
#define AW8624_REG_FIFO_AFL 10
#define AW8624_REG_RAMADDRH 11
#define AW8624_REG_RAMADDRL 12
#define AW8624_REG_RAMDATA 13
struct wait_queue {
	pthread_mutex_t lock;
	pthread_cond_t cond;
};
#define wake_up_all(q)                                                                             \
	do {                                                                                       \
		pthread_mutex_lock(&(q)->lock);                                                    \
		pthread_cond_broadcast(&(q)->cond);                                                \
		pthread_mutex_unlock(&(q)->lock);                                                  \
	} while (0)
#define wait_event(q, condition)                                                                   \
	do {                                                                                       \
		pthread_mutex_lock(&(q).lock);                                                     \
		while (!(condition))                                                               \
			pthread_cond_wait(&(q).cond, &(q).lock);                                   \
		pthread_mutex_unlock(&(q).lock);                                                   \
	} while (0)
struct firmware {
	size_t size;
	const u8 *data;
};
struct aw8624_container {
	int len;
	u8 data[];
};
struct aw8624 {
	pthread_mutex_t lock, rtp_lock, firmware_lock;
	struct wait_queue firmware_wait;
	struct {
		unsigned int baseaddr_shift, ram_shift, base_addr, len;
		unsigned short check_sum;
	} ram;
	unsigned int firmware_pending, rtp_file_num;
	int ram_work, play_mode, ram_init, rtp_init;
	bool shutting_down, suspended;
	void *dev;
};
/* EXTRACT_RTP_TABLE */
static char *aw8624_ram_name = "aw8624_haptic.bin";
static struct aw8624_container *aw8624_rtp;
static pthread_mutex_t gate_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gate_cond = PTHREAD_COND_INITIALIZER;
static bool block_write, write_started, allow_write, shutdown_returned;
static int writes, released, schedule_failure, request_count, cancel_count, late_request;
static void (*callback[8])(const struct firmware *, void *);
static void *callback_context[8];
static u8 ram_data[] = {0, 10, 0, 4, 1, 2, 3};
static struct firmware ram_fw = {sizeof(ram_data), ram_data};
static u8 rtp_data[] = {1, 2, 3};
static struct firmware rtp_fw = {sizeof(rtp_data), rtp_data};
static void *kzalloc(size_t n, int flags)
{
	(void)flags;
	return calloc(1, n);
}
static void kfree(void *p)
{
	free(p);
}
static void *vmalloc(size_t n)
{
	return malloc(n);
}
static void vfree(void *p)
{
	free(p);
}
static void release_firmware(const struct firmware *fw)
{
	assert(fw);
	released++;
}
static int request_firmware_nowait(void *module, int action, const char *name, void *dev, int flags,
				   void *context, void (*cb)(const struct firmware *, void *))
{
	(void)module;
	(void)action;
	(void)name;
	(void)dev;
	(void)flags;
	if (READ_ONCE(((struct aw8624 *)context)->shutting_down)) {
		late_request++;
		return -ENODEV;
	}
	if (schedule_failure)
		return -ENOMEM;
	assert(request_count < 8);
	callback[request_count] = cb;
	callback_context[request_count] = context;
	request_count++;
	return 0;
}
static void cancel_delayed_work_sync(int *work)
{
	(void)work;
	cancel_count++;
}
static void write_maybe_wait(void)
{
	writes++;
	pthread_mutex_lock(&gate_lock);
	if (block_write) {
		block_write = false;
		write_started = true;
		pthread_cond_broadcast(&gate_cond);
		while (!allow_write)
			pthread_cond_wait(&gate_cond, &gate_lock);
	}
	pthread_mutex_unlock(&gate_lock);
}
static int aw8624_i2c_write_bits(struct aw8624 *a, int r, int m, int v)
{
	(void)a;
	(void)r;
	(void)m;
	(void)v;
	write_maybe_wait();
	return 0;
}
static int aw8624_i2c_write(struct aw8624 *a, int r, int v)
{
	(void)a;
	(void)r;
	(void)v;
	write_maybe_wait();
	return 0;
}

/* EXTRACT aw8624_firmware_done */
/* EXTRACT aw8624_request_firmware */
/* EXTRACT aw8624_rtp_loaded */
/* EXTRACT aw8624_rtp_update */
/* EXTRACT aw8624_container_update */
/* EXTRACT aw8624_ram_loaded */
/* EXTRACT aw8624_ram_update */
/* EXTRACT aw8624_shutdown_firmware */

static void setup(struct aw8624 *a)
{
	memset(a, 0, sizeof(*a));
	assert(!pthread_mutex_init(&a->lock, NULL));
	assert(!pthread_mutex_init(&a->rtp_lock, NULL));
	assert(!pthread_mutex_init(&a->firmware_lock, NULL));
	assert(!pthread_mutex_init(&a->firmware_wait.lock, NULL));
	assert(!pthread_cond_init(&a->firmware_wait.cond, NULL));
	writes = released = request_count = schedule_failure = cancel_count = late_request = 0;
	block_write = write_started = allow_write = shutdown_returned = false;
}
static void cleanup(struct aw8624 *a)
{
	assert(a->firmware_pending == 0);
	free(aw8624_rtp);
	aw8624_rtp = NULL;
	assert(!pthread_cond_destroy(&a->firmware_wait.cond));
	assert(!pthread_mutex_destroy(&a->firmware_wait.lock));
	assert(!pthread_mutex_destroy(&a->firmware_lock));
	assert(!pthread_mutex_destroy(&a->rtp_lock));
	assert(!pthread_mutex_destroy(&a->lock));
}
static void run_callback(int i, const struct firmware *fw)
{
	callback[i](fw, callback_context[i]);
}
static void *run_parent(void *arg)
{
	(void)arg;
	run_callback(0, &ram_fw);
	return NULL;
}
static void *run_shutdown(void *arg)
{
	aw8624_shutdown_firmware(arg);
	pthread_mutex_lock(&gate_lock);
	shutdown_returned = true;
	pthread_cond_broadcast(&gate_cond);
	pthread_mutex_unlock(&gate_lock);
	return NULL;
}
static void wait_for_shutdown_flag(struct aw8624 *a)
{
	struct timespec tick = {.tv_nsec = 1000000};

	for (int i = 0; i < 2000 && !READ_ONCE(a->shutting_down); i++)
		nanosleep(&tick, NULL);
	assert(READ_ONCE(a->shutting_down));
}
static void test_firmware_paths(void)
{
	struct aw8624 a;

	setup(&a);
	schedule_failure = 1;
	assert(aw8624_ram_update(&a) == -ENOMEM && a.firmware_pending == 0);
	schedule_failure = 0;
	assert(aw8624_ram_update(&a) == 0 && a.firmware_pending == 1);
	run_callback(0, &ram_fw);
	assert(a.ram_init == 1 && a.ram.base_addr == 4 && a.ram.len == sizeof(ram_data));
	assert(request_count == 2 && a.firmware_pending == 1 && writes > 0 && released == 1);
	run_callback(1, &rtp_fw);
	assert(a.firmware_pending == 0 && a.rtp_init && aw8624_rtp->len == 3 && released == 2);
	assert(aw8624_ram_update(&a) == 0);
	run_callback(2, NULL);
	assert(a.firmware_pending == 0 && released == 2);
	u8 short_data[] = {0, 4, 4};
	struct firmware short_fw = {sizeof(short_data), short_data};

	assert(aw8624_ram_update(&a) == 0);
	run_callback(3, &short_fw);
	assert(a.firmware_pending == 0 && released == 3);
	u8 bad[] = {0, 0, 0, 4, 1};
	struct firmware bad_fw = {sizeof(bad), bad};

	assert(aw8624_ram_update(&a) == 0);
	run_callback(4, &bad_fw);
	assert(a.firmware_pending == 0 && released == 4);
	struct firmware huge_fw = {(size_t)INT_MAX, ram_data};

	assert(aw8624_ram_update(&a) == 0);
	run_callback(5, &huge_fw);
	assert(a.firmware_pending == 0 && released == 5);
	a.suspended = true;
	writes = 0;
	assert(aw8624_ram_update(&a) == 0);
	run_callback(6, &ram_fw);
	assert(writes == 0 && a.ram_init == 0 && a.firmware_pending == 0 && request_count == 7);
	cleanup(&a);
}
static void test_shutdown_waits_for_child(void)
{
	struct aw8624 a;
	pthread_t thread;

	setup(&a);
	assert(aw8624_ram_update(&a) == 0);
	run_callback(0, &ram_fw);
	assert(request_count == 2 && a.firmware_pending == 1);
	assert(!pthread_create(&thread, NULL, run_shutdown, &a));
	wait_for_shutdown_flag(&a);
	pthread_mutex_lock(&gate_lock);
	assert(!shutdown_returned);
	pthread_mutex_unlock(&gate_lock);
	run_callback(1, &rtp_fw);
	assert(!pthread_join(thread, NULL));
	assert(shutdown_returned && a.firmware_pending == 0 && !a.rtp_init && !aw8624_rtp);
	assert(cancel_count == 1 && aw8624_ram_update(&a) == -ENODEV);
	cleanup(&a);
}
static void test_shutdown_blocks_new_child(void)
{
	struct aw8624 a;
	pthread_t parent, shutdown;

	setup(&a);
	assert(aw8624_ram_update(&a) == 0);
	block_write = true;
	assert(!pthread_create(&parent, NULL, run_parent, NULL));
	pthread_mutex_lock(&gate_lock);
	while (!write_started)
		pthread_cond_wait(&gate_cond, &gate_lock);
	pthread_mutex_unlock(&gate_lock);
	assert(!pthread_create(&shutdown, NULL, run_shutdown, &a));
	wait_for_shutdown_flag(&a);
	pthread_mutex_lock(&gate_lock);
	assert(!shutdown_returned);
	allow_write = true;
	pthread_cond_broadcast(&gate_cond);
	pthread_mutex_unlock(&gate_lock);
	assert(!pthread_join(parent, NULL));
	assert(!pthread_join(shutdown, NULL));
	assert(shutdown_returned && a.firmware_pending == 0 && request_count == 1 &&
	       late_request == 0);
	cleanup(&a);
}
int main(void)
{
	test_firmware_paths();
	test_shutdown_waits_for_child();
	test_shutdown_blocks_new_child();
	puts("AW8624 tests: firmware lifetime and suspend races passed");
	return 0;
}
