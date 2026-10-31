#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Exercise extracted verifier reference tracking using mock function frames.

This is a host regression test, not a replacement for real BPF verifier tests.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
source = (root / "kernel/bpf/verifier.c").read_text()
start = source.index("static int acquire_reference_state(")
end = source.index("static void free_func_state(", start)
reference_functions = source[start:end]
start = source.index("static int check_reference_leak(")
end = source.index("static int check_bpf_snprintf_call(", start)
leak_function = source[start:end]
func_start = source.index("static int __check_func_call(")
condition_start = source.index("\tif (insn->code == (BPF_JMP | BPF_CALL) &&", func_start)
condition_end = source.index(" {", condition_start)
async_condition = source[condition_start:condition_end]
# The exit path must check a callback's references before returning to its caller.
exit_start = source.index("static int do_check(")
exit_end = source.index("static int do_check_common(", exit_start)
exit_body = source[exit_start:exit_end]
assert exit_body.index("err = check_reference_leak(env);") < exit_body.index(
    "err = prepare_func_exit(env, &env->insn_idx);"
)

prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct bpf_reference_state { int id, insn_idx, callback_ref; };
struct bpf_func_state {
    struct bpf_reference_state *refs;
    int acquired_refs, frameno;
    bool in_callback_fn;
};
struct bpf_verifier_env { struct bpf_func_state *current; int id_gen; };
static struct bpf_func_state *cur_func(struct bpf_verifier_env *env) { return env->current; }
static int resize_reference_state(struct bpf_func_state *state, size_t n) {
    void *p = realloc(state->refs, n * sizeof(*state->refs));
    if (!p) return -ENOMEM;
    state->refs = p; state->acquired_refs = n; return 0;
}
#define verbose(...) do { } while (0)
#define BPF_JMP 0x05
#define BPF_CALL 0x80
#define BPF_FUNC_timer_set_callback 170
#define BPF_PSEUDO_CALL 1
struct bpf_insn { int code, src_reg, imm; };
'''
async_prefix = r'''
static int is_async_callback(struct bpf_insn *insn) {
'''
async_suffix = r'''
        return 1;
    return 0;
}
'''
suffix = r'''
static void copy_refs(struct bpf_func_state *dst, struct bpf_func_state *src) {
    dst->acquired_refs = src->acquired_refs;
    dst->refs = malloc(dst->acquired_refs * sizeof(*dst->refs));
    assert(dst->refs);
    memcpy(dst->refs, src->refs, dst->acquired_refs * sizeof(*dst->refs));
}
int main(void) {
    struct bpf_func_state main_frame = {0}, cb = {.frameno=1, .in_callback_fn=true};
    struct bpf_func_state nested = {.frameno=3, .in_callback_fn=true};
    struct bpf_verifier_env env = {.current=&main_frame};
    struct bpf_insn call = {.code=BPF_JMP | BPF_CALL, .imm=BPF_FUNC_timer_set_callback};
    int parent = acquire_reference_state(&env, 10), child, inner;
    assert(is_async_callback(&call));
    call.src_reg = BPF_PSEUDO_CALL; /* matching delta is an ordinary subprog call */
    assert(!is_async_callback(&call));
    call.src_reg = 0; call.imm--;
    assert(!is_async_callback(&call));
    assert(parent > 0 && check_reference_leak(&env) == -EINVAL);
    copy_refs(&cb, &main_frame); env.current = &cb;
    assert(check_reference_leak(&env) == 0);
    assert(release_reference_state(&cb, parent) == -EINVAL);
    child = acquire_reference_state(&env, 20);
    assert(child > 0 && cb.refs[1].callback_ref == 1);
    assert(check_reference_leak(&env) == -EINVAL);
    copy_refs(&nested, &cb); env.current = &nested;
    assert(check_reference_leak(&env) == 0);
    assert(release_reference_state(&nested, parent) == -EINVAL);
    assert(release_reference_state(&nested, child) == -EINVAL);
    inner = acquire_reference_state(&env, 30);
    assert(inner > 0 && nested.refs[2].callback_ref == 3);
    assert(check_reference_leak(&env) == -EINVAL);
    assert(release_reference_state(&nested, inner) == 0);
    assert(check_reference_leak(&env) == 0);
    env.current = &cb;
    assert(release_reference_state(&cb, child) == 0);
    assert(check_reference_leak(&env) == 0);
    env.current = &main_frame;
    assert(release_reference_state(&main_frame, parent) == 0);
    assert(check_reference_leak(&env) == 0);
    free(main_frame.refs); free(cb.refs); free(nested.refs);
    puts("PASS: callback frames cannot consume caller refs or leak their own");
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix="bpf-callback-ref-") as tmp:
    test_file = Path(tmp) / "test.c"
    exe = Path(tmp) / "test"
    test_file.write_text(prefix + reference_functions + leak_function +
                         async_prefix + async_condition + async_suffix + suffix)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O2", "-g",
        "-Wall", "-Wextra", "-Werror", "-fsanitize=undefined",
        "-fno-sanitize-recover=all", str(test_file), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
