// SPDX-License-Identifier: GPL-2.0
/*
 * On-device BPF verifier smoke test for the temporary timer-helper gate.
 * Run with sufficient BPF privileges; no timer or callback is started.
 */
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
#define PTR_TO_U64(p) ((__u64)(uintptr_t)(p))
#define INSN(c, d, s, o, i) ((struct bpf_insn) { \
	.code = (c), .dst_reg = (d), .src_reg = (s), .off = (o), .imm = (i) })

static int load_program(int helper, char *log, size_t log_size)
{
	struct bpf_insn insns[3];
	union bpf_attr attr;
	int n = 0;

	if (helper >= 0)
		insns[n++] = INSN(BPF_JMP | BPF_CALL, 0, 0, 0, helper);
	insns[n++] = INSN(BPF_ALU64 | BPF_MOV | BPF_K, BPF_REG_0, 0, 0, 0);
	insns[n++] = INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0);
	memset(&attr, 0, sizeof(attr));
	attr.prog_type = BPF_PROG_TYPE_SOCKET_FILTER;
	attr.insn_cnt = n;
	attr.insns = PTR_TO_U64(insns);
	attr.license = PTR_TO_U64("GPL");
	attr.log_buf = PTR_TO_U64(log);
	attr.log_size = log_size;
	attr.log_level = 1;
	log[0] = '\0';
	return syscall(__NR_bpf, BPF_PROG_LOAD, &attr, sizeof(attr));
}

int main(void)
{
	static const int helpers[] = {
		BPF_FUNC_timer_init, BPF_FUNC_timer_set_callback,
		BPF_FUNC_timer_start, BPF_FUNC_timer_cancel,
	};
	struct rlimit limit = { RLIM_INFINITY, RLIM_INFINITY };
	char log[16384];
	int fd, saved;
	size_t i;

	/* Some kernels still charge BPF allocations to RLIMIT_MEMLOCK. */
	(void)setrlimit(RLIMIT_MEMLOCK, &limit);
	fd = load_program(-1, log, sizeof(log));
	if (fd < 0) {
		saved = errno;
		if ((saved == EPERM || saved == EACCES) && !log[0]) {
			fprintf(stderr, "SKIP: BPF program load denied: %s\n",
				strerror(saved));
			return KSFT_SKIP;
		}
		fprintf(stderr, "FAIL: control program: %s\n%s\n", strerror(saved), log);
		return 1;
	}
	close(fd);

	for (i = 0; i < sizeof(helpers) / sizeof(helpers[0]); i++) {
		fd = load_program(helpers[i], log, sizeof(log));
		saved = errno;
		if (fd >= 0) {
			close(fd);
			fprintf(stderr, "FAIL: timer helper %d unexpectedly loaded\n", helpers[i]);
			return 1;
		}
		if (saved != EOPNOTSUPP ||
		    !strstr(log, "bpf_timer requires softirq hrtimers")) {
			fprintf(stderr, "FAIL: timer helper %d: %s\n%s\n",
				helpers[i], strerror(saved), log);
			return 1;
		}
	}
	puts("PASS: BPF timer helpers are rejected; ordinary programs still load");
	return 0;
}
