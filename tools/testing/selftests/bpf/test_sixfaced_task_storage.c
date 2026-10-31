// SPDX-License-Identifier: GPL-2.0
/*
 * On-device BPF task-storage smoke test using this tree's pidfd backport.
 * Build against tools/include/uapi and run as a privileged user. This does
 * not replace upstream selftests or test concurrent task exit.
 */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <linux/bpf.h>
#include <linux/btf.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>

#if !defined(__NR_pidfd_open) && (defined(__aarch64__) || defined(__x86_64__))
#define __NR_pidfd_open 434
#endif
#define KSFT_SKIP 4
#define PTR_TO_U64(p) ((__u64)(uintptr_t)(p))

static int bpf_call(enum bpf_cmd cmd, union bpf_attr *attr)
{
	return syscall(__NR_bpf, cmd, attr, sizeof(*attr));
}

static int load_u32_btf(void)
{
	struct btf_blob {
		struct btf_header header;
		struct btf_type type;
		__u32 int_data;
		char strings[5];
	} blob = {
		.header = {
			.magic = BTF_MAGIC,
			.version = BTF_VERSION,
			.hdr_len = sizeof(struct btf_header),
			.type_len = sizeof(struct btf_type) + sizeof(__u32),
			.str_off = sizeof(struct btf_type) + sizeof(__u32),
			.str_len = 5,
		},
		.type = {
			.name_off = 1,
			.info = BTF_KIND_INT << 24,
			.size = sizeof(__u32),
		},
		.int_data = 32,
		.strings = "\0u32",
	};
	char log[8192] = { 0 };
	union bpf_attr attr = { 0 };
	int fd;

	attr.btf = PTR_TO_U64(&blob);
	/* Exclude any trailing structure padding after the string section. */
	attr.btf_size = offsetof(struct btf_blob, strings) + sizeof(blob.strings);
	attr.btf_log_buf = PTR_TO_U64(log);
	attr.btf_log_size = sizeof(log);
	attr.btf_log_level = 1;
	fd = bpf_call(BPF_BTF_LOAD, &attr);
	if (fd < 0 && errno != EPERM && errno != EACCES)
		fprintf(stderr, "FAIL: BTF load: %s\n%s\n", strerror(errno), log);
	return fd;
}

int main(void)
{
#ifndef __NR_pidfd_open
	puts("SKIP: pidfd_open syscall number not known on this architecture");
	return KSFT_SKIP;
#else
	struct rlimit memlock = { RLIM_INFINITY, RLIM_INFINITY };
	uint32_t input = 0x1234abcd, output = 0;
	int pidfd = -1, btf_fd = -1, map_fd = -1, regular_fd = -1;
	int result = 1, key;
	union bpf_attr attr;

	if (setrlimit(RLIMIT_MEMLOCK, &memlock))
		fprintf(stderr, "WARN: could not raise memlock limit: %s\n", strerror(errno));
	pidfd = syscall(__NR_pidfd_open, getpid(), 0);
	if (pidfd < 0) {
		fprintf(stderr, "FAIL: pidfd_open: %s\n", strerror(errno));
		goto out;
	}
	btf_fd = load_u32_btf();
	if (btf_fd < 0) {
		if (errno == EPERM || errno == EACCES) {
			fprintf(stderr, "SKIP: BTF load denied: %s\n", strerror(errno));
			result = KSFT_SKIP;
		}
		goto out;
	}

	memset(&attr, 0, sizeof(attr));
	attr.map_type = BPF_MAP_TYPE_TASK_STORAGE;
	attr.key_size = sizeof(key);
	attr.value_size = sizeof(input);
	attr.map_flags = BPF_F_NO_PREALLOC;
	attr.btf_fd = btf_fd;
	attr.btf_key_type_id = 1;
	attr.btf_value_type_id = 1;
	map_fd = bpf_call(BPF_MAP_CREATE, &attr);
	if (map_fd < 0) {
		fprintf(stderr, "FAIL: task storage map creation: %s\n", strerror(errno));
		goto out;
	}

	memset(&attr, 0, sizeof(attr));
	attr.map_fd = map_fd;
	attr.key = PTR_TO_U64(&key);
	attr.value = PTR_TO_U64(&input);
	key = -1;
	if (bpf_call(BPF_MAP_UPDATE_ELEM, &attr) != -1 || errno != EBADF) {
		fprintf(stderr, "FAIL: invalid pidfd was not rejected with EBADF\n");
		goto out;
	}
	regular_fd = open("/dev/null", O_RDONLY | O_CLOEXEC);
	if (regular_fd < 0) {
		fprintf(stderr, "FAIL: open /dev/null: %s\n", strerror(errno));
		goto out;
	}
	key = regular_fd;
	if (bpf_call(BPF_MAP_UPDATE_ELEM, &attr) != -1 || errno != EBADF) {
		fprintf(stderr, "FAIL: non-pidfd was not rejected with EBADF\n");
		goto out;
	}

	key = pidfd;
	if (bpf_call(BPF_MAP_UPDATE_ELEM, &attr)) {
		fprintf(stderr, "FAIL: task storage update: %s\n", strerror(errno));
		goto out;
	}
	attr.value = PTR_TO_U64(&output);
	if (bpf_call(BPF_MAP_LOOKUP_ELEM, &attr) || output != input) {
		fprintf(stderr, "FAIL: task storage lookup: %s, value=%#x\n",
			strerror(errno), output);
		goto out;
	}
	attr.value = 0;
	if (bpf_call(BPF_MAP_DELETE_ELEM, &attr)) {
		fprintf(stderr, "FAIL: task storage delete: %s\n", strerror(errno));
		goto out;
	}
	attr.value = PTR_TO_U64(&output);
	if (bpf_call(BPF_MAP_LOOKUP_ELEM, &attr) != -1 || errno != ENOENT) {
		fprintf(stderr, "FAIL: deleted task storage still present\n");
		goto out;
	}
	puts("PASS: pidfd task storage update, lookup, delete and invalid fds");
	result = 0;
out:
	if (regular_fd >= 0)
		close(regular_fd);
	if (map_fd >= 0)
		close(map_fd);
	if (btf_fd >= 0)
		close(btf_fd);
	if (pidfd >= 0)
		close(pidfd);
	return result;
#endif
}
