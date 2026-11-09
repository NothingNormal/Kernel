// SPDX-License-Identifier: GPL-2.0
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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
struct drv2624_data { void *dev; };
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
	reset(); assert(drv2624_set_go_bit(&d, GO) == 0 && reads == 1 && delays == 1);
	reset(); mismatch_reads = 3; assert(drv2624_set_go_bit(&d, GO) == 0 && reads == 4);
	reset(); read_error_at = 2; assert(drv2624_set_go_bit(&d, GO) == -EIO && reads == 2);
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
