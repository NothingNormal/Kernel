#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Check extracted BPF map-value copying and initialization on the host.

Run with python3 tools/testing/selftests/bpf/map_value_copy_host_test.py.
Pass CC=clang (or CC=gcc) to test the desired compiler. This mocks map layout.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
source = (root / "include/linux/bpf.h").read_text()
start = source.index("static inline bool map_value_has_spin_lock(")
end = source.index("void copy_map_value_locked(", start)
functions = source[start:end]
prefix = r'''
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint32_t u32;
struct bpf_spin_lock { uint32_t val; };
struct bpf_timer { uint64_t :64; uint64_t :64; } __attribute__((aligned(8)));
struct bpf_map { int spin_lock_off, timer_off; u32 value_size; };
#define unlikely(x) (x)
#define swap(a, b) do { __typeof__(a) tmp = (a); (a) = (b); (b) = tmp; } while (0)
'''
suffix = r'''
static void test(int spin, int timer)
{
    unsigned char src[64], dst[64];
    struct bpf_map map = { spin, timer, sizeof(src) };
    memset(src, 0xa5, sizeof(src)); memset(dst, 0x5a, sizeof(dst));
    copy_map_value(&map, dst, src);
    for (int i = 0; i < (int)sizeof(src); i++) {
        bool protected = (spin >= 0 && i >= spin && i < spin + (int)sizeof(struct bpf_spin_lock)) ||
                         (timer >= 0 && i >= timer && i < timer + (int)sizeof(struct bpf_timer));
        assert(dst[i] == (protected ? 0x5a : 0xa5));
    }
    memset(dst, 0x5a, sizeof(dst));
    check_and_init_map_value(&map, dst);
    for (int i = 0; i < (int)sizeof(dst); i++) {
        bool initialized = (spin >= 0 && i >= spin && i < spin + (int)sizeof(struct bpf_spin_lock)) ||
                           (timer >= 0 && i >= timer && i < timer + (int)sizeof(struct bpf_timer));
        assert(dst[i] == (initialized ? 0 : 0x5a));
    }
}
int main(void)
{
    test(-1, -1); test(0, -1); test(-1, 0); test(0, 16); test(24, 0);
    test(8, 16); test(16, 0);
    puts("PASS: BPF values preserve and zero spin-lock/timer fields in either order");
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix="bpf-map-value-") as tmp:
    test_file = Path(tmp) / "test.c"
    exe = Path(tmp) / "test"
    test_file.write_text(prefix + functions + suffix)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O2", "-g",
        "-Wall", "-Wextra", "-Werror", "-fsanitize=undefined",
        "-fno-sanitize-recover=all", str(test_file), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
