// SPDX-License-Identifier: GPL-2.0
/*
 * Basic on-device smoke tests for the Sixfaced 5.15 BPF backport.
 * Build against this tree's tools/include/uapi and run as root on the device.
 * This does not replace the upstream BPF selftest suite or prove ROM boot.
 */
#define _GNU_SOURCE
#include <errno.h>
#include <inttypes.h>
#include <linux/bpf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>

#define KSFT_SKIP 4
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define PTR_TO_U64(p) ((__u64)(uintptr_t)(p))
#define INSN(c, d, s, o, i) ((struct bpf_insn) { \
	.code = (c), .dst_reg = (d), .src_reg = (s), .off = (o), .imm = (i) })

static int bpf_call(enum bpf_cmd cmd, union bpf_attr *attr)
{
	return syscall(__NR_bpf, cmd, attr, sizeof(*attr));
}

static void load_imm64(struct bpf_insn *insns, int *n, int reg, uint64_t val)
{
	insns[(*n)++] = INSN(BPF_LD | BPF_DW | BPF_IMM, reg, 0, 0, (__u32)val);
	insns[(*n)++] = INSN(0, 0, 0, 0, (__u32)(val >> 32));
}

static uint64_t expected_result(int op, uint64_t old, uint64_t val,
				uint64_t cmp, uint64_t mask)
{
	switch (op) {
	case BPF_ADD:
	case BPF_ADD | BPF_FETCH:
		return (old + val) & mask;
	case BPF_AND:
	case BPF_AND | BPF_FETCH:
		return old & val;
	case BPF_OR:
	case BPF_OR | BPF_FETCH:
		return old | val;
	case BPF_XOR:
	case BPF_XOR | BPF_FETCH:
		return old ^ val;
	case BPF_XCHG:
		return val;
	case BPF_CMPXCHG:
		return old == cmp ? val : old;
	default:
		abort();
	}
}

static int test_atomic(int width, int op, int cmp_matches)
{
	const uint64_t original = UINT64_C(0xfedcba9876543210);
	const uint64_t operand = UINT64_C(0x0123456789abcdef);
	const uint64_t mask = width == 32 ? UINT32_MAX : UINT64_MAX;
	uint64_t old = original & mask, val = operand & mask;
	uint64_t cmp = cmp_matches ? old : old ^ 1;
	uint64_t result = expected_result(op, old, val, cmp, mask);
	uint64_t expected_src = (op & BPF_FETCH) && op != BPF_CMPXCHG ?
		old : operand;
	struct bpf_insn insns[40];
	unsigned char packet[64] = { 0 };
	int jumps[3], nr_jumps = 0, fail, n = 0, fd, ret;
	char log[16384] = { 0 };
	union bpf_attr attr;

	/* Initialize all 8 bytes of the stack slot for both widths. */
	load_imm64(insns, &n, BPF_REG_1, original);
	insns[n++] = INSN(BPF_STX | BPF_MEM | BPF_DW, BPF_REG_10, BPF_REG_1, -8, 0);
	load_imm64(insns, &n, BPF_REG_2, operand);
	load_imm64(insns, &n, BPF_REG_0, cmp);
	insns[n++] = INSN(BPF_STX | BPF_ATOMIC | (width == 32 ? BPF_W : BPF_DW),
			  BPF_REG_10, BPF_REG_2, -8, op);
	insns[n++] = INSN(BPF_LDX | BPF_MEM | (width == 32 ? BPF_W : BPF_DW),
			  BPF_REG_3, BPF_REG_10, -8, 0);
	load_imm64(insns, &n, BPF_REG_4, result);
	jumps[nr_jumps++] = n;
	insns[n++] = INSN(BPF_JMP | BPF_JNE | BPF_X, BPF_REG_3, BPF_REG_4, 0, 0);
	load_imm64(insns, &n, BPF_REG_4, expected_src);
	jumps[nr_jumps++] = n;
	insns[n++] = INSN(BPF_JMP | BPF_JNE | BPF_X, BPF_REG_2, BPF_REG_4, 0, 0);
	if (op == BPF_CMPXCHG) {
		load_imm64(insns, &n, BPF_REG_4, old);
		jumps[nr_jumps++] = n;
		insns[n++] = INSN(BPF_JMP | BPF_JNE | BPF_X, BPF_REG_0, BPF_REG_4, 0, 0);
	}
	insns[n++] = INSN(BPF_ALU64 | BPF_MOV | BPF_K, BPF_REG_0, 0, 0, 1);
	insns[n++] = INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0);
	fail = n;
	insns[n++] = INSN(BPF_ALU64 | BPF_MOV | BPF_K, BPF_REG_0, 0, 0, 0);
	insns[n++] = INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0);
	while (nr_jumps--) {
		int at = jumps[nr_jumps];

		insns[at].off = fail - at - 1;
	}

	memset(&attr, 0, sizeof(attr));
	attr.prog_type = BPF_PROG_TYPE_SOCKET_FILTER;
	attr.insn_cnt = n;
	attr.insns = PTR_TO_U64(insns);
	attr.license = PTR_TO_U64("GPL");
	attr.log_buf = PTR_TO_U64(log);
	attr.log_size = sizeof(log);
	attr.log_level = 1;
	fd = bpf_call(BPF_PROG_LOAD, &attr);
	if (fd < 0) {
		int saved = errno;

		if ((saved == EPERM || saved == EACCES) && !log[0]) {
			fprintf(stderr, "SKIP: BPF program load denied: %s\n", strerror(saved));
			return KSFT_SKIP;
		}
		fprintf(stderr, "FAIL: %d-bit atomic %#x load: %s\n%s\n",
			width, op, strerror(saved), log);
		return 1;
	}

	memset(&attr, 0, sizeof(attr));
	attr.test.prog_fd = fd;
	attr.test.data_size_in = sizeof(packet);
	attr.test.data_in = PTR_TO_U64(packet);
	attr.test.repeat = 3;
	ret = bpf_call(BPF_PROG_TEST_RUN, &attr);
	if (ret || attr.test.retval != 1) {
		fprintf(stderr, "FAIL: %d-bit atomic %#x run: ret=%d errno=%d value=%u\n",
			width, op, ret, errno, attr.test.retval);
		close(fd);
		return 1;
	}
	close(fd);
	return 0;
}

static int test_large_key(void)
{
	enum { KEY_SIZE = 4112 };
	unsigned char key[KEY_SIZE];
	uint32_t input = 0x1234abcd, output = 0;
	union bpf_attr attr;
	int fd, i;

	for (i = 0; i < KEY_SIZE; i++)
		key[i] = (unsigned char)(i * 29);
	memset(&attr, 0, sizeof(attr));
	attr.map_type = BPF_MAP_TYPE_HASH;
	attr.key_size = sizeof(key);
	attr.value_size = sizeof(input);
	attr.max_entries = 2;
	fd = bpf_call(BPF_MAP_CREATE, &attr);
	if (fd < 0) {
		fprintf(stderr, "FAIL: create large-key hash: %s\n", strerror(errno));
		return 1;
	}
	memset(&attr, 0, sizeof(attr));
	attr.map_fd = fd;
	attr.key = PTR_TO_U64(key);
	attr.value = PTR_TO_U64(&input);
	attr.flags = BPF_ANY;
	if (bpf_call(BPF_MAP_UPDATE_ELEM, &attr))
		goto error;
	attr.value = PTR_TO_U64(&output);
	attr.flags = 0;
	if (bpf_call(BPF_MAP_LOOKUP_ELEM, &attr))
		goto error;
	if (output != input) {
		fprintf(stderr, "FAIL: large-key lookup returned %#x instead of %#x\n",
			output, input);
		close(fd);
		return 1;
	}
	/* DELETE only accepts map_fd and key, unlike LOOKUP. */
	attr.value = 0;
	if (bpf_call(BPF_MAP_DELETE_ELEM, &attr))
		goto error;
	close(fd);
	return 0;
error:
	fprintf(stderr, "FAIL: large-key hash operation: %s\n", strerror(errno));
	close(fd);
	return 1;
}

static int test_stack_overflow_guard(void)
{
	union bpf_attr attr;
	int fd;

	memset(&attr, 0, sizeof(attr));
	attr.map_type = BPF_MAP_TYPE_STACK_TRACE;
	attr.key_size = 4;
	attr.value_size = 8;
	attr.max_entries = UINT32_MAX;
	fd = bpf_call(BPF_MAP_CREATE, &attr);
	if (fd >= 0) {
		close(fd);
		fprintf(stderr, "FAIL: overflow-sized stack map was accepted\n");
		return 1;
	}
	if (errno != E2BIG) {
		fprintf(stderr, "FAIL: overflow-sized stack map returned %s, not E2BIG\n",
			strerror(errno));
		return 1;
	}
	return 0;
}

int main(void)
{
	static const int ops[] = {
		BPF_ADD, BPF_AND, BPF_OR, BPF_XOR,
		BPF_ADD | BPF_FETCH, BPF_AND | BPF_FETCH,
		BPF_OR | BPF_FETCH, BPF_XOR | BPF_FETCH,
		BPF_XCHG, BPF_CMPXCHG,
	};
	struct rlimit memlock = { RLIM_INFINITY, RLIM_INFINITY };
	size_t i;
	int width, result, total = 0;

	if (setrlimit(RLIMIT_MEMLOCK, &memlock))
		fprintf(stderr, "WARN: could not raise memlock limit: %s\n", strerror(errno));
	for (width = 32; width <= 64; width *= 2) {
		for (i = 0; i < ARRAY_SIZE(ops); i++) {
			result = test_atomic(width, ops[i], 1);
			if (result)
				return result;
			total++;
			if (ops[i] == BPF_CMPXCHG) {
				result = test_atomic(width, ops[i], 0);
				if (result)
					return result;
				total++;
			}
		}
	}
	if (test_large_key() || test_stack_overflow_guard())
		return 1;
	printf("PASS: %d atomic programs, large hash key and stack map overflow guard\n", total);
	return 0;
}
