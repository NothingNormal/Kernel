#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Test the 4.14 verifier allocation shim using its extracted source.

Run with python3 tools/testing/selftests/bpf/verifier_copy_array_host_test.py.
This mocks allocation failures; it does not run the BPF verifier in a kernel.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
source = (root / "kernel/bpf/verifier.c").read_text()
start = source.index("static void *copy_array(")
end = source.index("/* resize an array", start)
function = source[start:end]

prefix = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int gfp_t;
#define ZERO_SIZE_PTR ((void *)16)
#define ZERO_OR_NULL_PTR(p) ((uintptr_t)(p) <= (uintptr_t)ZERO_SIZE_PTR)
#define unlikely(x) (x)
#define check_mul_overflow(a, b, result) __builtin_mul_overflow(a, b, result)
struct allocation { size_t size; unsigned char data[]; };
static unsigned int allocs, frees;
static int fail_allocation;
static void *kmalloc_track_caller(size_t size, gfp_t flags)
{
    struct allocation *a;
    (void)flags;
    if (fail_allocation) { fail_allocation = 0; return NULL; }
    if (!size) return ZERO_SIZE_PTR;
    a = malloc(sizeof(*a) + size);
    assert(a);
    a->size = size;
    allocs++;
    return a->data;
}
static struct allocation *allocation(const void *p)
{
    return (struct allocation *)((unsigned char *)p - offsetof(struct allocation, data));
}
static size_t ksize(const void *p)
{
    /* The target 4.14 ksize(NULL) warns and may panic if panic_on_warn is set. */
    assert(p);
    return p == ZERO_SIZE_PTR ? 0 : allocation(p)->size;
}
static void kfree(void *p)
{
    if (ZERO_OR_NULL_PTR(p)) return;
    frees++;
    free(allocation(p));
}
'''
# offsetof is needed by the mocked allocation header.
prefix = "#include <stddef.h>\n" + prefix

suffix = r'''
int main(void)
{
    unsigned char src[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    void *dst, *old;
    unsigned int before;

    /* A fresh destination must never be passed to the old ksize(). */
    dst = copy_array(NULL, src, sizeof(src), 1, 0);
    assert(dst && memcmp(dst, src, sizeof(src)) == 0);

    /* An existing buffer is reused when it is large enough. */
    old = dst;
    dst = copy_array(dst, src + 1, 2, 1, 0);
    assert(dst == old && memcmp(dst, src + 1, 2) == 0);

    /* Empty inputs leave the destination untouched, including NULL. */
    assert(copy_array(NULL, src, 0, 1, 0) == ZERO_SIZE_PTR);
    assert(copy_array(NULL, src, 1, 0, 0) == ZERO_SIZE_PTR);
    assert(copy_array(dst, src, 0, 1, 0) == dst);
    assert(copy_array(dst, NULL, 5, 1, 0) == dst);
    assert(copy_array(dst, ZERO_SIZE_PTR, 5, 1, 0) == dst);

    /* Overflow is rejected without freeing the existing allocation. */
    before = frees;
    assert(copy_array(dst, src, SIZE_MAX, 2, 0) == NULL);
    assert(frees == before);
    kfree(dst);

    /* The zero-size sentinel is resized into a real allocation. */
    dst = copy_array(ZERO_SIZE_PTR, src, sizeof(src), 1, 0);
    assert(dst && dst != ZERO_SIZE_PTR && memcmp(dst, src, sizeof(src)) == 0);
    kfree(dst);

    /* Growth frees the old buffer; a failed allocation returns NULL. */
    dst = copy_array(NULL, src, 2, 1, 0);
    before = frees;
    fail_allocation = 1;
    assert(copy_array(dst, src, sizeof(src), 1, 0) == NULL);
    assert(frees == before + 1);
    assert(allocs == frees);
    puts("PASS: verifier copy_array NULL, empty, reuse, overflow and failure cases");
    return 0;
}
'''

with tempfile.TemporaryDirectory(prefix="bpf-copy-array-test-") as tmp:
    test = Path(tmp) / "test.c"
    exe = Path(tmp) / "test"
    test.write_text(prefix + function + suffix)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O1", "-g",
        "-Wall", "-Wextra", "-Werror", "-fsanitize=undefined",
        "-fno-sanitize-recover=all", str(test), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
