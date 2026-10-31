#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Compile the JIT cap expression from core.c and check its intended quarter-limit."""
import os
import re
import shlex
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
source = (ROOT / "kernel/bpf/core.c").read_text()
match = re.search(
    r"bpf_jit_limit\s*=\s*min_t\(u64,\s*round_up\(bpf_jit_limit_max\s*>>\s*(\d+),"
    r"\s*PAGE_SIZE\),\s*LONG_MAX\)", source)
assert match, "production BPF JIT limit expression changed; review this test"
shift = int(match.group(1))

program = f"""
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
typedef uint64_t u64;
#define PAGE_SIZE 4096ULL
#define round_up(x, y) (((x) + ((y) - 1)) / (y) * (y))
#define min_t(type, x, y) ((type)(x) < (type)(y) ? (type)(x) : (type)(y))
static u64 limit_for(u64 bpf_jit_limit_max)
{{
    u64 bpf_jit_limit;
    bpf_jit_limit = min_t(u64, round_up(bpf_jit_limit_max >> {shift}, PAGE_SIZE), LONG_MAX);
    return bpf_jit_limit;
}}
int main(void)
{{
    assert(limit_for(128ULL * 1024 * 1024) == 32ULL * 1024 * 1024);
    assert(limit_for(4 * PAGE_SIZE) == PAGE_SIZE);
    assert(limit_for(4 * PAGE_SIZE + 4) == PAGE_SIZE + PAGE_SIZE);
    assert(limit_for(0) == 0);
    puts("PASS: BPF JIT accounting cap matches the 5.15 quarter-space heuristic");
    return 0;
}}
"""
with tempfile.TemporaryDirectory(prefix="bpf-jit-limit-test-") as tmp:
    c_file = Path(tmp) / "jit_limit.c"
    exe = Path(tmp) / "jit_limit"
    c_file.write_text(program)
    cc = shlex.split(os.environ.get("CC", "cc"))
    subprocess.run(cc + ["-std=gnu11", "-O2", "-Wall", "-Wextra", "-Werror",
                         "-fsanitize=undefined", "-fno-sanitize-recover=all",
                         str(c_file), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
