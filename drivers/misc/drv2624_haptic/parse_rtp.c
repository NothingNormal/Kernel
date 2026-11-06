#include <linux/init.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include "drv2624.h"
#include "parse_rtp.h"

static struct rtp_head head = { 0 };

static struct idx_element *index_table;
static unsigned char *instruction_data;
static unsigned int instruction_size;

unsigned char *instruction_begin;;
unsigned char *instruction_current;
unsigned char *instruction_end;
bool open_rtp;
int pop_running_effect_command(char *command, int *data)
{
	if (!open_rtp) {
		return -INVALID;
	}
	if (instruction_end == instruction_current)
		return -INVALID;
	if ((instruction_current - instruction_begin) % 2 == 0)
		*command = 'w';
	else
		*command = 'd';
	*data = *instruction_current;
	instruction_current++;
	return SUCCESS;
}

int rtp_get_effect_duration(int id)
{
	unsigned int offset, length;

	/* The header count is populated entries, not the largest sparse ID. */
	if (!open_rtp || id < 0 || id >= INDEX_NUM)
		return -INVALID;

	offset = index_table[id].offset;
	length = index_table[id].length;
	if (!length || offset > instruction_size ||
	    length > instruction_size - offset) {
		pr_info("%s: invalid RTP entry %d\n", __func__, id);
		return -INVALID;
	}
	return index_table[id].duration;
}

int set_running_effect_id(int id)
{
	int duration = rtp_get_effect_duration(id);

	if (duration < 0)
		return duration;
	instruction_begin = instruction_data + index_table[id].offset;
	instruction_current = instruction_begin;
	instruction_end =
	    instruction_data + index_table[id].offset + index_table[id].length;

	return duration;
}

static int rtp_parse_header(unsigned char *pData, unsigned int nSize)
{
	unsigned char *pDataStart = pData;
	unsigned char pMagicNumber[] = { 0xcf, 0xcf, 0x00, 0x00 };
	if (nSize < 16) {
		pr_info("rtp: Header too short\n");
		return -INVALID;
	}

	if (memcmp(pData, pMagicNumber, 4)) {
		pr_info("rtp: Magic number doesn't match\n");
		return -INVALID;
	}
	pData += 4;

	memcpy(&head.ver, pData, 4);
	pr_info("rtp version : %d\n", head.ver);
	pData += 4;

	memcpy(&head.effect_num, pData, sizeof(head.effect_num));
	pr_info("rtp effect_num : %d\n", head.effect_num);
	pData += 8;
	return pData - pDataStart;
}

int rtp_parse(unsigned char *pData, unsigned int nSize)
{
	int head_size = 0;
	int i;
	pr_info("%s begin\n", __func__);
	open_rtp = false;
	if (!pData || nSize < RTP_HEAD_SIZE + INDEX_NUM * sizeof(*index_table))
		return -INVALID;
	head_size = rtp_parse_header(pData, nSize);
	pr_info("header size: %d\n", head_size);
	if (head_size != RTP_HEAD_SIZE) {
		pr_info("rtp file: Wrong Header\n");
		return -INVALID;
	}

	pData += RTP_HEAD_SIZE;
	index_table = (struct idx_element *)pData;
	for (i = 0; i < 24; i++) {
		pr_info("%s: idx_tabel[%d] = %d, %d, %d\n",
		       __func__, i, index_table[i].offset,
		       index_table[i].length, index_table[i].duration);
	}
	instruction_data = pData + INDEX_NUM * sizeof(*index_table);
	instruction_size = nSize - RTP_HEAD_SIZE - INDEX_NUM * sizeof(*index_table);
	open_rtp = true;
	pr_info("%s: idx_element size = %d\n", __func__,
	       sizeof(struct idx_element));
	pr_info("%s end\n", __func__);
	return SUCCESS;
}
