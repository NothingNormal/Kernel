// SPDX-License-Identifier: GPL-2.0
/* Device smoke test for 32-bit conditional-jump support in the ARM64 BPF JIT. */
#define _GNU_SOURCE
#include <errno.h>
#include <linux/bpf.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>

#define KSFT_SKIP 4
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define PTR_TO_U64(p) ((__u64)(uintptr_t)(p))
#define INSN(c, d, s, o, i) ((struct bpf_insn) { \
	.code = (c), .dst_reg = (d), .src_reg = (s), .off = (o), .imm = (i) })

struct jump_case {
	int op;
	const char *name;
	int taken;
};

static const struct jump_case cases[] = {
	{ BPF_JEQ, "JEQ", 0 }, { BPF_JNE, "JNE", 1 },
	{ BPF_JGT, "JGT", 1 }, { BPF_JLT, "JLT", 0 },
	{ BPF_JGE, "JGE", 1 }, { BPF_JLE, "JLE", 0 },
	{ BPF_JSGT, "JSGT", 0 }, { BPF_JSLT, "JSLT", 1 },
	{ BPF_JSGE, "JSGE", 0 }, { BPF_JSLE, "JSLE", 1 },
	{ BPF_JSET, "JSET", 1 },
};

static int bpf_call(enum bpf_cmd cmd, union bpf_attr *attr)
{
	return syscall(__NR_bpf, cmd, attr, sizeof(*attr));
}

static int run_case(const struct jump_case *test, int use_reg)
{
	const uint32_t lhs = 0x80000005U;
	const int32_t rhs = 0x7fffffff;
	struct bpf_insn insns[8];
	unsigned char packet[64] = { 0 }, output[64] = { 0 };
	char log[8192] = { 0 };
	union bpf_attr attr;
	int n = 0, fd, ret;

	insns[n++] = INSN(BPF_ALU64 | BPF_MOV | BPF_K, BPF_REG_0, 0, 0, 1);
	insns[n++] = INSN(BPF_ALU | BPF_MOV | BPF_K, BPF_REG_2, 0, 0,
			  (int32_t)lhs);
	if (use_reg)
		insns[n++] = INSN(BPF_ALU | BPF_MOV | BPF_K, BPF_REG_3, 0, 0, rhs);
	insns[n++] = INSN(BPF_JMP32 | test->op | (use_reg ? BPF_X : BPF_K),
			  BPF_REG_2, use_reg ? BPF_REG_3 : 0, 1, rhs);
	insns[n++] = INSN(BPF_ALU64 | BPF_MOV | BPF_K, BPF_REG_0, 0, 0, 0);
	insns[n++] = INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0);

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
		if ((errno == EPERM || errno == EACCES) && !log[0]) {
			printf("SKIP: BPF_PROG_LOAD requires privilege (%s)\n", strerror(errno));
			return KSFT_SKIP;
		}
		fprintf(stderr, "FAIL: load JMP32 %s/%s: %s\n%s", test->name,
			use_reg ? "reg" : "imm", strerror(errno), log);
		return 1;
	}

	memset(&attr, 0, sizeof(attr));
	attr.test.prog_fd = fd;
	attr.test.data_size_in = sizeof(packet);
	attr.test.data_in = PTR_TO_U64(packet);
	attr.test.data_size_out = sizeof(output);
	attr.test.data_out = PTR_TO_U64(output);
	attr.test.repeat = 1;
	ret = bpf_call(BPF_PROG_TEST_RUN, &attr);
	if (ret || attr.test.retval != (uint32_t)test->taken) {
		fprintf(stderr, "FAIL: run JMP32 %s/%s: ret=%d errno=%s retval=%u expected=%u\n",
			test->name, use_reg ? "reg" : "imm", ret, strerror(errno),
			attr.test.retval, test->taken);
		close(fd);
		return 1;
	}
	close(fd);
	return 0;
}

int main(void)
{
	struct rlimit memlock = { RLIM_INFINITY, RLIM_INFINITY };
	size_t i;
	int source, ret, total = 0;

	if (setrlimit(RLIMIT_MEMLOCK, &memlock))
		fprintf(stderr, "WARN: could not raise memlock limit: %s\n", strerror(errno));
	for (source = 0; source <= 1; ++source) {
		for (i = 0; i < ARRAY_SIZE(cases); ++i) {
			ret = run_case(&cases[i], source);
			if (ret)
				return ret;
			++total;
		}
	}
	printf("PASS: %d JMP32 immediate/register programs loaded and executed\n", total);
	return 0;
}
