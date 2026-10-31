#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Exercise the source-extracted task-storage pidfd adapter with host mocks.

Run with python3 tools/testing/selftests/bpf/task_storage_pidfd_host_test.py.
The mocks test validation and refcounts, not live kernel task-storage races.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
source = (root / "kernel/bpf/bpf_task_storage.c").read_text()
start = source.index("static struct pid *pidfd_get_pid(")
end = source.index("DEFINE_BPF_STORAGE_CACHE(task_cache);", start)
function = source[start:end]

prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_PTR(e) ((void *)(intptr_t)(e))
#define PTR_ERR(p) ((intptr_t)(p))
struct pid { int refs; };
struct file_operations { int kind; };
struct file {
    const struct file_operations *f_op;
    struct pid *private_data;
    unsigned int f_flags;
    int refs;
};
struct fd { struct file *file; unsigned int flags; };
static const struct file_operations pidfd_fops = { 1 };
static const struct file_operations other_fops = { 2 };
static struct pid task_pid = { 1 };
static struct file real_pidfd = { &pidfd_fops, &task_pid, 0x81, 1 };
static struct file regular_file = { &other_fops, &task_pid, 0x42, 1 };
static unsigned int acquired, released;

static struct fd fdget(unsigned int fd)
{
    struct file *f = fd == 4 ? &real_pidfd : fd == 5 ? &regular_file : NULL;
    if (f) { f->refs++; acquired++; }
    return (struct fd) { f, 0 };
}
static void fdput(struct fd f)
{
    assert(f.file && f.file->refs > 1);
    f.file->refs--;
    released++;
}
static struct pid *get_pid(struct pid *pid) { pid->refs++; return pid; }
static void put_pid(struct pid *pid) { assert(pid->refs > 1); pid->refs--; }
'''
suffix = r'''
int main(void)
{
    struct pid *pid;
    unsigned int flags = 0xdead;

    pid = pidfd_get_pid(4, &flags);
    assert(pid == &task_pid && flags == 0x81);
    assert(pid->refs == 2 && real_pidfd.refs == 1);
    put_pid(pid);

    flags = 0xdead;
    pid = pidfd_get_pid(5, &flags);
    assert(PTR_ERR(pid) == -EBADF && flags == 0xdead);
    assert(task_pid.refs == 1 && regular_file.refs == 1);

    pid = pidfd_get_pid(1000, &flags);
    assert(PTR_ERR(pid) == -EBADF && flags == 0xdead);
    pid = pidfd_get_pid((unsigned int)-1, &flags);
    assert(PTR_ERR(pid) == -EBADF && flags == 0xdead);

    assert(acquired == 2 && released == 2);
    puts("PASS: pidfd validation, flags and balanced file/pid references");
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix="bpf-pidfd-test-") as tmp:
    test = Path(tmp) / "test.c"
    exe = Path(tmp) / "test"
    test.write_text(prefix + function + suffix)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O1", "-g",
        "-Wall", "-Wextra", "-Werror", "-fsanitize=undefined",
        "-fno-sanitize-recover=all", str(test), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
