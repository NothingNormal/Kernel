#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host-test source-extracted BPF tracing map compatibility rules.

Run with python3 tools/testing/selftests/bpf/tracing_safety_host_test.py.
This does not run verifier bytecode or kernel tracepoints.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
verifier = (root / "kernel/bpf/verifier.c").read_text()
start = verifier.index("static bool is_tracing_prog_type(")
end = verifier.index("static bool bpf_map_is_cgroup_storage(", start)
functions = verifier[start:end]
trace = (root / "kernel/trace/bpf_trace.c").read_text()
trace = trace[trace.index("bpf_tracing_func_proto("):trace.index("kprobe_prog_func_proto(")]
base = (root / "kernel/bpf/helpers.c").read_text()
base = base[base.index("bpf_base_func_proto("):]
assert "case BPF_FUNC_ktime_get_coarse_ns:" not in trace
assert "case BPF_FUNC_ktime_get_coarse_ns:" not in base
net = (root / "net/core/filter.c").read_text()
assert net.count("case BPF_FUNC_ktime_get_coarse_ns:") >= 3

prefix = r'''
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#define CONFIG_PREEMPT_RT 0
#define IS_ENABLED(x) (x)
#define WARN_ONCE(...) ((void)0)
#define verbose(...) ((void)0)
enum bpf_prog_type {
    BPF_PROG_TYPE_SOCKET_FILTER = 1, BPF_PROG_TYPE_KPROBE,
    BPF_PROG_TYPE_TRACEPOINT, BPF_PROG_TYPE_PERF_EVENT,
    BPF_PROG_TYPE_RAW_TRACEPOINT, BPF_PROG_TYPE_XDP
};
enum map_type { BPF_MAP_TYPE_HASH, BPF_MAP_TYPE_LRU_HASH, BPF_MAP_TYPE_ARRAY,
    BPF_MAP_TYPE_PERCPU_HASH, BPF_MAP_TYPE_PERCPU_ARRAY, BPF_MAP_TYPE_LRU_PERCPU_HASH,
    BPF_MAP_TYPE_ARRAY_OF_MAPS, BPF_MAP_TYPE_HASH_OF_MAPS, BPF_MAP_TYPE_RINGBUF,
    BPF_MAP_TYPE_STRUCT_OPS };
struct bpf_map { enum map_type map_type; bool prealloc, timer, spin;
                 struct bpf_map *inner_map_meta; };
struct bpf_prog_aux { bool sleepable; };
struct bpf_prog { enum bpf_prog_type type; struct bpf_prog_aux *aux; };
struct bpf_verifier_env { int ignored; };
static enum bpf_prog_type resolve_prog_type(struct bpf_prog *p) { return p->type; }
static bool check_map_prealloc(struct bpf_map *m) { return m->prealloc; }
static bool map_value_has_spin_lock(struct bpf_map *m) { return m->spin; }
static bool map_value_has_timer(struct bpf_map *m) { return m->timer; }
static bool bpf_prog_is_dev_bound(struct bpf_prog_aux *a) { (void)a; return false; }
static bool bpf_map_is_dev_bound(struct bpf_map *m) { (void)m; return false; }
static bool bpf_offload_prog_map_match(struct bpf_prog *p, struct bpf_map *m)
{ (void)p; (void)m; return true; }
'''
suffix = r'''
int main(void)
{
    enum bpf_prog_type trace_types[] = { BPF_PROG_TYPE_KPROBE, BPF_PROG_TYPE_TRACEPOINT,
        BPF_PROG_TYPE_PERF_EVENT, BPF_PROG_TYPE_RAW_TRACEPOINT };
    struct bpf_verifier_env env = { 0 };
    struct bpf_prog_aux aux = { 0 };
    struct bpf_prog prog = { .aux = &aux };
    struct bpf_map map = { .map_type = BPF_MAP_TYPE_HASH, .prealloc = true, .timer = true };
    for (unsigned int i = 0; i < sizeof(trace_types)/sizeof(trace_types[0]); i++) {
        prog.type = trace_types[i];
        assert(check_map_prog_compatibility(&env, &map, &prog) == -EINVAL);
        map.timer = false;
        assert(check_map_prog_compatibility(&env, &map, &prog) == 0);
        map.timer = true;
    }
    prog.type = BPF_PROG_TYPE_XDP;
    assert(check_map_prog_compatibility(&env, &map, &prog) == 0);
    puts("PASS: tracing timer maps rejected; ordinary/network maps accepted");
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix="bpf-tracing-safety-") as tmp:
    test = Path(tmp) / "test.c"
    exe = Path(tmp) / "test"
    test.write_text(prefix + functions + suffix)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O1", "-g",
        "-Wall", "-Wextra", "-Werror", "-Wno-unused-parameter",
        "-fsanitize=undefined", "-fno-sanitize-recover=all", str(test), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
