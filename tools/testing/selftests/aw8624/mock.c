// SPDX-License-Identifier: GPL-2.0
/*
 * Userspace harness: run.py inserts the real driver functions below.
 * Hardware, input, and synchronization APIs are mocks; this is not a phone test.
 */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int16_t s16;
typedef int64_t s64;
typedef int64_t ktime_t;
typedef int atomic_t;
#define READ_ONCE(x) __atomic_load_n(&(x), __ATOMIC_SEQ_CST)
#define WRITE_ONCE(x, v) __atomic_store_n(&(x), (v), __ATOMIC_SEQ_CST)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define min_t(type, a, b) min((type)(a), (type)(b))
#define clamp(x, l, h) ((x) < (l) ? (l) : (x) > (h) ? (h) : (x))
#define DIV_ROUND_CLOSEST(x, y) (((x) + ((y) / 2)) / (y))
#define DIV_ROUND_UP_ULL(x, y) (((x) + (y) - 1) / (y))
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
#define spin_lock_irq(l) assert(pthread_mutex_lock(l) == 0)
#define spin_unlock_irq(l) assert(pthread_mutex_unlock(l) == 0)
#define spin_lock_irqsave(l, flags)                                                                \
	do {                                                                                       \
		(flags) = 0;                                                                       \
		assert(pthread_mutex_lock(l) == 0);                                                \
	} while (0)
#define spin_unlock_irqrestore(l, flags)                                                           \
	do {                                                                                       \
		(void)(flags);                                                                     \
		assert(pthread_mutex_unlock(l) == 0);                                              \
	} while (0)
#define mutex_lock(l) assert(pthread_mutex_lock(l) == 0)
#define mutex_unlock(l) assert(pthread_mutex_unlock(l) == 0)
#define atomic_read(a) (*(a))
#define atomic_set(a, v) (*(a) = (v))
#define wait_event_interruptible(q, condition) (mock_wait(q, !!(condition)))
#define wake_up_interruptible(q) ((void)(q))
#define VIB_FUNC_ENTER()                                                                           \
	do {                                                                                       \
	} while (0)
#define VIB_DEBUG(...)                                                                             \
	do {                                                                                       \
	} while (0)
#define pr_debug(...)                                                                              \
	do {                                                                                       \
	} while (0)
#define pr_err(...)                                                                                \
	do {                                                                                       \
	} while (0)
#define MSEC_PER_SEC 1000
#define AW8624_GAIN_FULL 0x80
#define AW8624_LIGHT_MAGNITUDE 0x3fff
#define AW8624_MEDIUM_MAGNITUDE 0x5fff
#define AW8624_STRONG_MAGNITUDE 0x7fff
#define AW8624_MAX_EFFECT_WAIT_US 20000
#define AW8624_RTP_EFFECT_BASE 20
#define AW8624_RTP_FIRST_EFFECT_ID 21
#define AW8624_FALLBACK_MAX_DELAY_MS 250
#define AW8624_RTP_MIN_SAMPLE_RATE 12000
#define AW8624_RTP_WAKE_MARGIN_MS 1000
#define AW8624_HAPTIC_RAM_MODE 1
#define AW8624_HAPTIC_RTP_MODE 2
#define AW8624_HAPTIC_ACTIVATE_RAM_MODE 0
#define AW8624_HAPTIC_ACTIVATE_RTP_MODE 2
#define AW8624_HAPTIC_ACTIVATE_RAM_LOOP_MODE 3
#define CUSTOM_DATA_LEN 3
#define OSC_CALI 1

struct work_struct {
	int queued;
};
struct hrtimer {
	bool active;
	s64 remaining_us;
};
struct ff_device {
	int max_effects;
};
struct input_dev {
	struct ff_device *ff;
	pthread_mutex_t event_lock;
	void *drvdata;
};
struct predefined {
	int play_rate_us;
	u8 level;
};
struct aw8624_ff_effect {
	int effect_type, effect_id, duration;
	u8 activate_mode;
	u16 vmax_mv;
};
struct aw8624_container {
	int len;
	u8 data[];
};
struct firmware {
	size_t size;
	const u8 *data;
};
struct aw8624 {
	pthread_mutex_t lock, rtp_lock;
	struct input_dev *input_dev;
	struct aw8624_ff_effect *ff_effects;
	struct predefined *predefined;
	struct {
		int effect_id_boundary, effect_max, gain_flag;
		unsigned int rtp_time[175];
	} info;
	struct {
		u16 vmax_mv;
	} play;
	struct {
		int base_addr;
	} ram;
	struct hrtimer timer, stop_timer, hap_disable_timer;
	struct { struct hrtimer timer; struct work_struct work; } haptic_audio;
	struct work_struct vibrator_work, rtp_work, gain_work;
	void *work_queue, *dev;
	int effects_count, osc_cali_run, effect_type, effect_id, duration;
	int activate_mode, state, play_mode, ulevel;
	unsigned int rtp_file_num, rtp_cnt;
	u32 play_generation;
	ktime_t play_requested_at;
	u16 ff_gain;
	u8 level, applied_gain, rtp_init, ram_init;
	bool suspended;
	atomic_t exit_in_rtp_loop, is_in_irq;
	int wait_q, stop_wait_q;
};
#define AW8624_RTP_NAME_MAX 64
/* EXTRACT_RTP_TABLE */
static struct aw8624_container *aw8624_rtp;
static int mock_copy_from_error, mock_copy_to_error, mock_wait_error;
static int slept_us, start_count, stop_count, wavseq, written_gain;
static bool timer_fires_on_cancel, rearm_timer_on_cancel_work;
static int wake_active, relaxed, rtp_result, preload_streaming;
static int fw_error, fw_delay_ms, fw_action, fail_vmalloc, fw_calls;
static bool replace_before_request;
static u8 fw_data[200] = {1, 2, 3};
static struct firmware valid_fw = {.size = sizeof(fw_data), .data = fw_data};
static ktime_t fake_clock;
static struct aw8624 *fw_device;

enum { FW_NONE, FW_STOP, FW_REPLACE, FW_SUSPEND };
static void *input_get_drvdata(struct input_dev *d)
{
	return d->drvdata;
}
static int copy_from_user(void *dst, const void *src, size_t n)
{
	if (mock_copy_from_error || !src)
		return 1;
	memcpy(dst, src, n);
	return 0;
}
static int copy_to_user(void *dst, const void *src, size_t n)
{
	if (mock_copy_to_error || !dst)
		return 1;
	memcpy(dst, src, n);
	return 0;
}
static int hrtimer_active(struct hrtimer *t)
{
	return t->active;
}
static s64 hrtimer_get_remaining(struct hrtimer *t)
{
	return t->remaining_us;
}
static s64 ktime_to_us(s64 k)
{
	return k;
}
static void usleep_range(s64 min, s64 max)
{
	assert(max == min + 100);
	slept_us = min;
}
static int hrtimer_cancel(struct hrtimer *t)
{
	int was = t->active;

	if (timer_fires_on_cancel) {
		timer_fires_on_cancel = false;
		WRITE_ONCE(fw_device->state, 0);
	}
	t->active = false;
	return was;
}
static void cancel_work_sync(struct work_struct *work)
{
	work->queued = 0;
	if (rearm_timer_on_cancel_work) {
		rearm_timer_on_cancel_work = false;
		fw_device->timer.active = true;
	}
}
static int mock_wait(int q, int condition)
{
	(void)q;
	return condition ? 0 : mock_wait_error;
}
static bool queue_work(void *q, struct work_struct *w)
{
	(void)q;
	w->queued++;
	return true;
}
static void *vmalloc(size_t n)
{
	return fail_vmalloc ? NULL : malloc(n);
}
static void vfree(void *p)
{
	free(p);
}
static ktime_t ktime_get(void)
{
	return fake_clock;
}
static s64 ktime_ms_delta(ktime_t end, ktime_t start)
{
	return end - start;
}
static void pm_stay_awake(void *dev)
{
	(void)dev;
	wake_active = 1;
}
static void pm_wakeup_event(void *dev, u64 ms)
{
	(void)dev;
	assert(ms >= 1000);
	wake_active = 1;
}
static void pm_relax(void *dev)
{
	(void)dev;
	relaxed++;
	wake_active = 0;
}
static void release_firmware(const struct firmware *fw)
{
	(void)fw;
}
static int request_firmware(const struct firmware **fw, const char *name, void *dev)
{
	(void)name;
	(void)dev;
	fw_calls++;
	fake_clock += fw_delay_ms;
	if (fw_action == FW_STOP) {
		WRITE_ONCE(fw_device->state, 0);
		fw_device->play_generation++;
	}
	if (fw_action == FW_REPLACE) {
		WRITE_ONCE(fw_device->state, 1);
		fw_device->play_generation++;
	}
	if (fw_action == FW_SUSPEND)
		WRITE_ONCE(fw_device->suspended, true);
	if (fw_error)
		return fw_error;
	*fw = &valid_fw;
	return 0;
}
static void aw8624_haptic_set_wav_seq(struct aw8624 *a, int which, int id)
{
	(void)a;
	if (!which)
		wavseq = id;
}
static void aw8624_haptic_set_wav_loop(struct aw8624 *a, int n, int v)
{
	(void)a;
	(void)n;
	(void)v;
}
static void aw8624_haptic_play_mode(struct aw8624 *a, int mode)
{
	a->play_mode = mode;
}
static void aw8624_haptic_set_repeat_wav_seq(struct aw8624 *a, int seq)
{
	(void)a;
	wavseq = seq;
}
static void aw8624_haptic_play_repeat_seq(struct aw8624 *a, bool on)
{
	(void)a;
	if (on)
		start_count++;
}
static void aw8624_haptic_start(struct aw8624 *a)
{
	(void)a;
	start_count++;
}
static void aw8624_haptic_upload_lra(struct aw8624 *a, int mode)
{
	(void)mode;
	if (replace_before_request) {
		replace_before_request = false;
		a->play_generation++;
	}
}
static void aw8624_haptic_set_rtp_aei(struct aw8624 *a, bool on)
{
	(void)a;
	(void)on;
}
static void aw8624_interrupt_clear(struct aw8624 *a)
{
	(void)a;
}
static void aw8624_haptic_stop(struct aw8624 *a)
{
	(void)a;
	stop_count++;
}
static int aw8624_haptic_rtp_init(struct aw8624 *a)
{
	a->rtp_init = preload_streaming;
	return rtp_result;
}

/* EXTRACT aw8624_haptic_set_level */
static void aw8624_haptic_set_gain(struct aw8624 *a, u8 gain)
{
	a->applied_gain = gain;
	written_gain = aw8624_haptic_set_level(a, gain);
}
/* EXTRACT aw8624_haptic_effect_strength */
/* EXTRACT aw8624_haptic_play_ram */
/* EXTRACT aw8624_haptic_play_effect_seq */
/* EXTRACT aw8624_haptics_upload_effect */
/* EXTRACT aw8624_haptics_playback */
/* EXTRACT aw8624_haptics_erase */
/* EXTRACT aw8624_haptics_set_gain */
/* EXTRACT aw8624_block_new_effects */
/* EXTRACT aw8624_quiesce */
/* EXTRACT aw8624_gain_work_routine */
/* EXTRACT aw8624_rtp_work_routine */

static struct ff_device ff = {.max_effects = 32};
static struct input_dev input;
static struct aw8624 a;
static struct predefined pre[11];
static struct aw8624_ff_effect slots[32];
static void setup(void)
{
	memset(&a, 0, sizeof(a));
	memset(&input, 0, sizeof(input));
	memset(slots, 0, sizeof(slots));
	memset(pre, 0, sizeof(pre));
	assert(!pthread_mutex_init(&a.lock, NULL));
	assert(!pthread_mutex_init(&a.rtp_lock, NULL));
	assert(!pthread_mutex_init(&input.event_lock, NULL));
	a.input_dev = &input;
	input.drvdata = &a;
	input.ff = &ff;
	a.ff_effects = slots;
	a.predefined = pre;
	a.effects_count = 11;
	a.info.effect_id_boundary = 10;
	a.info.effect_max = 170;
	a.info.gain_flag = 1;
	a.ulevel = 128;
	a.ff_gain = 0xffff;
	a.ram_init = 1;
	a.ram.base_addr = 64;
	fw_device = &a;
	for (int i = 0; i < 11; i++) {
		pre[i].play_rate_us = (i + 1) * 1000;
		pre[i].level = 0x80;
	}
	for (int i = 0; i < 175; i++)
		a.info.rtp_time[i] = 1234 + i;
	mock_copy_from_error = mock_copy_to_error = mock_wait_error = 0;
	slept_us = start_count = stop_count = written_gain = wake_active = relaxed = 0;
	fw_error = fw_delay_ms = fw_action = fake_clock = rtp_result = preload_streaming =
	    fail_vmalloc = fw_calls = 0;
	replace_before_request = false;
	valid_fw.size = sizeof(fw_data);
	wavseq = -1;
	timer_fires_on_cancel = false;
	rearm_timer_on_cancel_work = false;
}
static void cleanup(void)
{
	free(aw8624_rtp);
	aw8624_rtp = NULL;
	assert(!pthread_mutex_destroy(&a.lock));
	assert(!pthread_mutex_destroy(&a.rtp_lock));
	assert(!pthread_mutex_destroy(&input.event_lock));
}
static int upload(int slot, int wave, u16 mag, s16 out[3])
{
	struct ff_effect e = {.type = FF_PERIODIC, .id = slot};

	out[0] = wave;
	out[1] = out[2] = -1;
	e.u.periodic.custom_data = out;
	e.u.periodic.magnitude = mag;
	return aw8624_haptics_upload_effect(&input, &e, NULL);
}
static void play(int slot, int value)
{
	pthread_mutex_lock(&input.event_lock);
	assert(aw8624_haptics_playback(&input, slot, value) == 0);
	pthread_mutex_unlock(&input.event_lock);
}
static void test_routing(void)
{
	s16 data[3];

	setup();
	assert(ARRAY_SIZE(aw8624_rtp_name) == 123);
	assert(!strcmp(aw8624_rtp_name[0], "osc_rtp_24K_5s.bin"));
	assert(strstr(aw8624_rtp_name[65], "screenshot"));
	for (int id = 0; id <= 170; id++) {
		assert(upload(4, id, AW8624_STRONG_MAGNITUDE, data) == 0);
		struct aw8624_ff_effect *slot = &slots[4];

		if (id <= 10) {
			assert(slot->effect_id == id &&
			       slot->activate_mode == AW8624_HAPTIC_ACTIVATE_RAM_MODE);
			assert(data[1] == 0 && data[2] == id + 1);
		} else if (id <= 20 || id >= 143) {
			assert(slot->effect_id == 0 &&
			       slot->activate_mode == AW8624_HAPTIC_ACTIVATE_RAM_MODE);
			assert(data[1] == 0 && data[2] == 1);
		} else {
			assert(slot->effect_id == id &&
			       slot->activate_mode == AW8624_HAPTIC_ACTIVATE_RTP_MODE);
			assert(data[1] == (1234 + id) / 1000 && data[2] == (1234 + id) % 1000);
		}
	}
	assert(upload(4, -1, 0x7fff, data) == -EINVAL);
	assert(upload(4, 171, 0x7fff, data) == -EINVAL);
	assert(upload(32, 0, 0x7fff, data) == -EINVAL);
	a.effects_count = 3;
	assert(upload(4, 10, 0x7fff, data) == 0 && slots[4].effect_id == 2 && data[2] == 3);
	a.effects_count = 0;
	assert(upload(4, 0, 0x7fff, data) == -EINVAL);
	a.effects_count = 11;
	mock_copy_from_error = 1;
	assert(upload(4, 1, 0x7fff, data) == -EFAULT);
	mock_copy_from_error = 0;
	struct aw8624_ff_effect before = slots[4];

	mock_copy_to_error = 1;
	assert(upload(4, 1, 0x7fff, data) == -EFAULT);
	mock_copy_to_error = 0;
	assert(!memcmp(&before, &slots[4], sizeof(before)));
	a.timer.active = true;
	a.timer.remaining_us = 5000000;
	assert(upload(4, 0, 0x7fff, data) == 0 && slept_us == 20000);
	a.timer.remaining_us = 1234;
	assert(upload(4, 0, 0x7fff, data) == 0 && slept_us == 1234);
	a.exit_in_rtp_loop = 1;
	mock_wait_error = -EINTR;
	assert(upload(4, 0, 0x7fff, data) == -EINTR);
	a.exit_in_rtp_loop = 0;
	a.suspended = true;
	assert(upload(4, 0, 0x7fff, data) == -EBUSY);
	cleanup();
}
static void test_gain_and_slots(void)
{
	s16 data[3];

	setup();
	unsigned int previous = 0;

	for (int m = 0; m <= AW8624_STRONG_MAGNITUDE; m++) {
		aw8624_haptic_effect_strength(&a, m);
		assert(a.level >= previous && a.level <= AW8624_GAIN_FULL);
		previous = a.level;
	}
	aw8624_haptic_effect_strength(&a, AW8624_LIGHT_MAGNITUDE);
	assert(a.level == 0x50);
	aw8624_haptic_effect_strength(&a, AW8624_MEDIUM_MAGNITUDE);
	assert(a.level == 0x68);
	aw8624_haptic_effect_strength(&a, AW8624_STRONG_MAGNITUDE);
	assert(a.level == 0x80);
	for (int u = 0; u <= 128; u++)
		for (int g = 0; g <= 255; g++)
			for (int i = 0; i < 4; i++) {
				unsigned int gains[] = {0, 0x3fff, 0x7fff, 0xffff};

				a.ulevel = u;
				a.ff_gain = gains[i];
				unsigned int expect = ((u * g / 128) * gains[i]) / 65535;

				assert(aw8624_haptic_set_level(&a, g) == expect);
			}
	a.ulevel = 128;
	a.ff_gain = 0xffff;
	assert(upload(1, 10, AW8624_STRONG_MAGNITUDE, data) == 0);
	assert(upload(7, 85, AW8624_LIGHT_MAGNITUDE, data) == 0);
	play(7, 1);
	assert(a.effect_id == 85 && a.play.vmax_mv == AW8624_LIGHT_MAGNITUDE);
	assert(a.rtp_work.queued == 1 && a.state == 1);
	timer_fires_on_cancel = true;
	play(1, 1);
	assert(a.state == 1 && a.effect_id == 10 && a.play.vmax_mv == AW8624_STRONG_MAGNITUDE);
	assert(a.vibrator_work.queued == 1);
	pre[10].level = 64;
	aw8624_haptic_play_effect_seq(&a, true);
	assert(wavseq == 11 && written_gain == 64 && start_count == 1);
	play(7, 0);
	assert(a.state == 0 && a.exit_in_rtp_loop == 1);
	a.exit_in_rtp_loop = 0;
	assert(aw8624_haptics_erase(&input, 7) == 0 && slots[7].effect_type == 0);
	aw8624_haptics_set_gain(&input, 0x7fff);
	assert(a.ff_gain == 0x7fff && a.gain_work.queued == 1);
	a.applied_gain = 0x80;
	aw8624_gain_work_routine(&a.gain_work);
	assert(written_gain == 63);
	aw8624_block_new_effects(&a);
	assert(a.suspended && a.state == 0);
	aw8624_haptics_set_gain(&input, 0);
	aw8624_gain_work_routine(&a.gain_work);
	assert(written_gain == 63);
	assert(a.ff_gain == 0 && a.gain_work.queued == 1);
	cleanup();
}
static void test_firmware_worker(void)
{
	s16 data[3];

	setup();
	assert(upload(2, 85, AW8624_STRONG_MAGNITUDE, data) == 0);
	play(2, 1);
	replace_before_request = true;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(fw_calls == 0 && start_count == 0);
	play(2, 1);
	fw_error = -ENOENT;
	fw_delay_ms = 10;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(wavseq == 1 && start_count == 1 && a.effect_id == 85 &&
	       a.activate_mode == AW8624_HAPTIC_ACTIVATE_RTP_MODE);
	assert(!wake_active && a.rtp_file_num == 65);
	start_count = 0;
	wavseq = -1;
	fw_action = FW_STOP;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	play(2, 1);
	fw_action = FW_REPLACE;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	fw_action = FW_NONE;
	fw_delay_ms = 251;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	play(2, 1);
	fake_clock += 300;
	fw_delay_ms = 0;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	fw_error = 0;
	fw_delay_ms = 5;
	fw_action = FW_REPLACE;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && aw8624_rtp == NULL);
	fw_action = FW_NONE;
	preload_streaming = 0;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 1 && aw8624_rtp && aw8624_rtp->len == 200 && !wake_active);
	assert(a.rtp_file_num == 65 && a.play_mode == AW8624_HAPTIC_RTP_MODE);
	start_count = 0;
	preload_streaming = 1;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 1 && wake_active);
	pm_relax(a.dev);
	preload_streaming = 0;
	start_count = 0;
	valid_fw.size = 0;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	valid_fw.size = (size_t)INT_MAX;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	valid_fw.size = sizeof(fw_data);
	fail_vmalloc = 1;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	fail_vmalloc = 0;
	fw_action = FW_SUSPEND;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	a.suspended = false;
	fw_action = FW_NONE;
	a.effect_id = 20;
	aw8624_rtp_work_routine(&a.rtp_work);
	assert(start_count == 0 && !wake_active);
	cleanup();
}
static void test_quiesce(void)
{
	s16 data[3];

	setup();
	assert(upload(3, 85, AW8624_STRONG_MAGNITUDE, data) == 0);
	play(3, 1);
	a.timer.active = true;
	a.stop_timer.active = true;
	a.hap_disable_timer.active = true;
	a.haptic_audio.timer.active = true;
	a.haptic_audio.work.queued = 1;
	a.gain_work.queued = 1;
	a.rtp_init = 1;
	a.rtp_cnt = 10;
	wake_active = 1;
	rearm_timer_on_cancel_work = true;
	aw8624_quiesce(&a);
	assert(a.suspended && !a.state && !a.rtp_init && !a.rtp_cnt);
	assert(!a.exit_in_rtp_loop && !wake_active);
	assert(!a.timer.active && !a.stop_timer.active && !a.hap_disable_timer.active);
	assert(!a.haptic_audio.timer.active && !a.haptic_audio.work.queued);
	assert(!a.rtp_work.queued && !a.gain_work.queued);
	assert(upload(3, 85, AW8624_STRONG_MAGNITUDE, data) == -EBUSY);
	play(3, 1);
	assert(!a.state && !a.rtp_work.queued);
	cleanup();
}
int main(void)
{
	test_routing();
	test_gain_and_slots();
	test_firmware_worker();
	test_quiesce();
	puts("AW8624 extracted-code tests: routing, slots, gain, timer and RTP fallback passed");
	return 0;
}
