// SPDX-License-Identifier: GPL-2.0
/* Runtime ABI checks: only a child created by this test is killed. */
#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#if defined(__aarch64__) || defined(__arm__)
#ifndef __NR_pidfd_open
#define __NR_pidfd_open 434
#endif
#ifndef __NR_process_mrelease
#define __NR_process_mrelease 448
#endif
#endif

static void check(int ok, const char *what)
{
	if (!ok) {
		fprintf(stderr, "FAIL: %s (errno=%d)\n", what, errno);
		exit(1);
	}
}

int main(void)
{
#if defined(__NR_pidfd_open) && defined(__NR_process_mrelease)
	int ready[2], command[2], fd, status;
	pid_t child;
	long ret;
	char token;

	errno = 0;
	ret = syscall(__NR_process_mrelease, -1, 0);
	if (ret == -1 && errno == ENOSYS) {
		puts("SKIP: running kernel has no process_mrelease");
		return 4;
	}
	check(ret == -1 && errno == EBADF, "invalid pidfd");
	errno = 0;
	check(syscall(__NR_process_mrelease, -1, 1) == -1 && errno == EINVAL,
	      "nonzero flags");
	check(pipe(ready) == 0 && pipe(command) == 0, "pipe");
	child = fork();
	check(child >= 0, "fork");
	if (!child) {
		size_t len = 1024 * 1024;
		unsigned char *p = mmap(NULL, len, PROT_READ | PROT_WRITE,
				       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		close(ready[0]);
		close(command[1]);
		if (p == MAP_FAILED)
			_exit(1);
		memset(p, 0x5a, len);
		if (write(ready[1], "x", 1) != 1 || read(command[0], &token, 1) != 1)
			_exit(1);
		for (size_t i = 0; i < len; i++)
			if (p[i] != 0x5a)
				_exit(1);
		if (write(ready[1], "x", 1) != 1)
			_exit(1);
		/* EOF also exits the child if the parent's checks fail. */
		if (read(command[0], &token, 1) != 1)
			_exit(0);
		_exit(1);
	}
	close(ready[1]);
	close(command[0]);
	check(read(ready[0], &token, 1) == 1, "child ready");
	fd = syscall(__NR_pidfd_open, child, 0);
	check(fd >= 0, "pidfd_open");
	errno = 0;
	check(syscall(__NR_process_mrelease, fd, 0) == -1 && errno == EINVAL,
	      "live child rejected");
	check(write(command[1], "x", 1) == 1 && read(ready[0], &token, 1) == 1,
	      "live child's data preserved");
	check(kill(child, SIGKILL) == 0, "kill test child");
	errno = 0;
	ret = syscall(__NR_process_mrelease, fd, 0);
	check(ret == 0 || (ret == -1 && (errno == ESRCH || errno == EAGAIN)),
	      "dying child reaped, already exited or temporarily unreapable");
	check(waitpid(child, &status, 0) == child && WIFSIGNALED(status) &&
	      WTERMSIG(status) == SIGKILL, "child terminated");
	close(fd);
	close(ready[0]);
	close(command[1]);
	puts("PASS: process_mrelease ABI, live-target safety and exit race");
	return 0;
#else
	puts("SKIP: syscall numbers unavailable for this architecture");
	return 4;
#endif
}
