#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Check the extracted BPF timer-helper gate on a host (mock verifier)."""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
source = (root / "kernel/bpf/verifier.c").read_text()
check = source.index("static int check_helper_call(")
start = source.index("\t/* 4.14 hrtimers execute", check)
end = source.index("\n\tif (env->ops->get_func_proto)", start)
gate = source[start:end]
prefix = r'''
#include <assert.h>
#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
struct bpf_verifier_env { int messages; };
#define verbose(env, ...) (++(env)->messages)
static int timer_gate(struct bpf_verifier_env *env, int func_id) {
'''
suffix = r'''
    return 0;
}
int main(void) {
    struct bpf_verifier_env env = {0};
    int rejected = 0;
    for (int i = 0; i < __BPF_FUNC_MAX_ID; i++) {
        int timer = i == BPF_FUNC_timer_init || i == BPF_FUNC_timer_set_callback ||
                    i == BPF_FUNC_timer_start || i == BPF_FUNC_timer_cancel;
        int ret = timer_gate(&env, i);
        assert(ret == (timer ? -EOPNOTSUPP : 0));
        rejected += timer;
    }
    assert(rejected == 4 && env.messages == 4);
    puts("PASS: only the four unsafe BPF timer helpers are gated");
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix="bpf-timer-gate-") as tmp:
    test_file = Path(tmp) / "test.c"
    exe = Path(tmp) / "test"
    test_file.write_text(prefix + gate + suffix)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O2", "-g",
        "-Wall", "-Wextra", "-Werror", "-fsanitize=undefined",
        "-fno-sanitize-recover=all", "-I", str(root / "tools/include/uapi"),
        str(test_file), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
