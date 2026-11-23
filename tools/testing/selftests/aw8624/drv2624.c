// SPDX-License-Identifier: GPL-2.0
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <linux/input.h>
#include <string.h>

#define DRV2624_REG_GO 0x0c
#define DRV2624_GO_BIT_MASK 0x01
#define GO_BIT_CHECK_INTERVAL 5
#define GO_BIT_MAX_RETRY_CNT 20
#define GO 1
#define STOP 0
#define OVERCURRENT_MASK 0x01
#define OVERTEMPRATURE_MASK 0x02
#define ULVO_MASK 0x04
#define PRG_ERR_MASK 0x10
#define dev_dbg(...) do {} while (0)
#define dev_err(...) do {} while (0)
typedef uint16_t u16;
#define EFFECT_MAX_NUM 32
#define CUSTOM_DATA_LEN 3
enum { CUSTOM_DATA_EFFECT_IDX, CUSTOM_DATA_TIMEOUT_SEC_IDX, CUSTOM_DATA_TIMEOUT_MSEC_IDX };
enum { DRV2624_RTP_MODE, DRV2624_RAM_MODE, DRV2624_NEW_RTP_MODE };
#define MSEC_PER_SEC 1000
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define mutex_lock(p) ((void)0)
#define mutex_unlock(p) ((void)0)
#define spin_lock_irq(p) ((void)0)
#define spin_unlock_irq(p) ((void)0)
#define WRITE_ONCE(x,v) ((x) = (v))
#define wake_up(p) ((void)0)
#define schedule_work(p) (++*(p))
#define copy_from_user(dst,src,len) (memcpy(dst,src,len), 0)
#define copy_to_user(dst,src,len) (memcpy(dst,src,len), 0)
struct drv2624_ff_effect {
	struct { int effect_id, length, magnitude, rtp_input; } play;
	int mode;
};
struct drv2624_data {
	void *dev;
	struct { int fw_effCount; } fw_header;
	unsigned int mnEffectTimems[EFFECT_MAX_NUM];
	struct drv2624_ff_effect ff_effects[EFFECT_MAX_NUM], requested_effect;
	int requested_slot, request_generation, haptics_playback_work;
};
struct input_dev { struct drv2624_data *chip; };
#define input_get_drvdata(dev) ((dev)->chip)
static int rtp_get_effect_duration(int id) { return id == 12 ? 1100 : -EINVAL; }
/* EXTRACT drv2624_haptics_upload_effect */
/* EXTRACT drv2624_haptics_playback */
/* EXTRACT drv2624_haptics_erase */

static void test_slots(void)
{
	struct drv2624_data chip = { .requested_slot = -1, .fw_header = { 5 } };
	struct input_dev dev = { &chip };
	short data[3] = { 2 };
	struct ff_effect effect = { .type = FF_PERIODIC, .id = 1 };
	struct ff_effect constant = { .type = FF_CONSTANT, .id = 2 };

	effect.u.periodic.waveform = FF_CUSTOM;
	effect.u.periodic.custom_data = data;
	chip.mnEffectTimems[2] = 40;
	assert(!drv2624_haptics_upload_effect(&dev, &effect, NULL));
	assert(data[1] == 0 && data[2] == 40);
	constant.replay.length = 120;
	assert(!drv2624_haptics_upload_effect(&dev, &constant, NULL));
	assert(!drv2624_haptics_playback(&dev, 2, 1));
	assert(chip.requested_effect.play.length == 120);
	assert(!drv2624_haptics_playback(&dev, 1, 1));
	assert(chip.requested_effect.mode == DRV2624_RAM_MODE);
	assert(chip.requested_effect.play.effect_id == 3 && !chip.requested_effect.play.length);
	assert(!drv2624_haptics_playback(&dev, 2, 0));
	assert(!drv2624_haptics_erase(&dev, 2));
	assert(chip.requested_slot == 1 && chip.haptics_playback_work == 2);
	data[0] = 12;
	assert(!drv2624_haptics_upload_effect(&dev, &effect, NULL));
	assert(data[1] == 1 && data[2] == 100);
	assert(chip.requested_effect.mode == DRV2624_RAM_MODE);
	assert(!drv2624_haptics_playback(&dev, 1, 0));
	assert(chip.requested_slot == -1);
	assert(!drv2624_haptics_playback(&dev, 1, 1));
	assert(chip.requested_effect.mode == DRV2624_NEW_RTP_MODE);
	data[0] = 15;
	assert(!drv2624_haptics_upload_effect(&dev, &effect, NULL));
	assert(chip.ff_effects[1].play.length == 25);
	data[0] = 150;
	assert(drv2624_haptics_upload_effect(&dev, &effect, NULL) == -EINVAL);
	assert(chip.ff_effects[1].play.length == 25);
}

static int write_error, read_error_at, mismatch_reads, reads, delays;
static unsigned char requested;
static int drv2624_reg_write(struct drv2624_data *d, unsigned char reg, unsigned char val)
{
	(void)d; assert(reg == DRV2624_REG_GO); requested = val; return write_error;
}
static int drv2624_reg_read(struct drv2624_data *d, unsigned char reg)
{
	(void)d; assert(reg == DRV2624_REG_GO); reads++;
	if (reads == read_error_at) return -EIO;
	return reads <= mismatch_reads ? !requested : requested;
}
static void mdelay(unsigned int ms) { assert(ms == GO_BIT_CHECK_INTERVAL); delays++; }
/* EXTRACT drv2624_set_go_bit */
/* EXTRACT drv2624_fault_status */
static void reset(void)
{
	write_error = read_error_at = mismatch_reads = reads = delays = requested = 0;
}
int main(void)
{
	struct drv2624_data d = {0};
	test_slots();
	reset(); assert(drv2624_set_go_bit(&d, GO) == 0 && reads == 1 && delays == 1);
	reset(); mismatch_reads = 3; assert(drv2624_set_go_bit(&d, GO) == 0 && reads == 4);
	reset(); mismatch_reads = 1; read_error_at = 2;
	assert(drv2624_set_go_bit(&d, GO) == -EIO && reads == 2);
	reset(); write_error = -EIO; assert(drv2624_set_go_bit(&d, STOP) == -EIO && !reads);
	reset(); mismatch_reads = 100; assert(drv2624_set_go_bit(&d, GO) == -ETIMEDOUT);
	assert(reads == GO_BIT_MAX_RETRY_CNT && delays == GO_BIT_MAX_RETRY_CNT);
	assert(!drv2624_fault_status(0));
	assert(drv2624_fault_status(OVERCURRENT_MASK));
	assert(drv2624_fault_status(OVERTEMPRATURE_MASK));
	assert(drv2624_fault_status(ULVO_MASK));
	assert(drv2624_fault_status(PRG_ERR_MASK));
	puts("DRV2624 extracted-code tests: GO timeout and fault classification passed");
	return 0;
}
