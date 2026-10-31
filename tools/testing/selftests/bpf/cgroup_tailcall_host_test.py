#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Test source-extracted BPF tail-call cgroup-storage ownership rules.

Run with python3 tools/testing/selftests/bpf/cgroup_tailcall_host_test.py.
Mocks do not exercise kernel RCU, JIT, or an actual cgroup.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
source = (root / "kernel/bpf/core.c").read_text()
start = source.index("bool bpf_prog_array_compatible(")
end = source.index("static int bpf_check_tail_call(", start)
function = source[start:end]
syscall_source = (root / "kernel/bpf/syscall.c").read_text()
assert "map->cookie = gen_cookie_next(&bpf_map_cookie);" in syscall_source
assert syscall_source.index("map->cookie = gen_cookie_next") < syscall_source.index("err = bpf_map_alloc_id(map)")

prefix = r'''
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint64_t u64;
enum bpf_cgroup_storage_type { SHARED, PERCPU, MAX_BPF_CGROUP_STORAGE_TYPE };
struct bpf_map { u64 cookie; };
struct bpf_prog_aux { struct bpf_map *cgroup_storage[MAX_BPF_CGROUP_STORAGE_TYPE]; };
struct bpf_prog { int type; bool jited, kprobe_override; struct bpf_prog_aux *aux; };
struct bpf_array_aux {
    struct { pthread_mutex_t lock; int type; bool jited;
             u64 storage_cookie[MAX_BPF_CGROUP_STORAGE_TYPE]; } owner;
};
struct bpf_array { struct bpf_array_aux *aux; };
#define spin_lock(l) assert(pthread_mutex_lock(l) == 0)
#define spin_unlock(l) assert(pthread_mutex_unlock(l) == 0)
'''
suffix = r'''
static void init_array(struct bpf_array *a, struct bpf_array_aux *aux)
{
    memset(aux, 0, sizeof(*aux));
    assert(!pthread_mutex_init(&aux->owner.lock, NULL));
    a->aux = aux;
}
struct race_arg { struct bpf_array *array; struct bpf_prog *prog;
                  pthread_barrier_t *barrier; bool result; };
static void *race(void *arg)
{
    struct race_arg *a = arg;
    pthread_barrier_wait(a->barrier);
    a->result = bpf_prog_array_compatible(a->array, a->prog);
    return NULL;
}
int main(void)
{
    struct bpf_map a = { 111 }, b = { 222 }, p = { 333 }, q = { 444 };
    struct bpf_prog_aux aux_a = { { &a, NULL } }, aux_b = { { &b, NULL } };
    struct bpf_prog_aux aux_none = { { NULL, NULL } }, aux_ap = { { &a, &p } };
    struct bpf_prog_aux aux_aq = { { &a, &q } };
    struct bpf_prog pa = { 1, true, false, &aux_a };
    struct bpf_prog pb = { 1, true, false, &aux_b };
    struct bpf_prog pn = { 1, true, false, &aux_none };
    struct bpf_prog pap = { 1, true, false, &aux_ap };
    struct bpf_prog paq = { 1, true, false, &aux_aq };
    struct bpf_array array; struct bpf_array_aux owner;
    pthread_t threads[16]; struct race_arg args[16]; pthread_barrier_t barrier;

    init_array(&array, &owner);
    assert(bpf_prog_array_compatible(&array, &pa));
    assert(bpf_prog_array_compatible(&array, &pa));
    assert(bpf_prog_array_compatible(&array, &pn));
    assert(!bpf_prog_array_compatible(&array, &pb));
    pb.jited = false; assert(!bpf_prog_array_compatible(&array, &pb)); pb.jited = true;
    pb.type = 2; assert(!bpf_prog_array_compatible(&array, &pb)); pb.type = 1;
    pn.kprobe_override = true; assert(!bpf_prog_array_compatible(&array, &pn));
    pn.kprobe_override = false;
    a.cookie = 555; /* same address reused for a different map */
    assert(!bpf_prog_array_compatible(&array, &pa)); a.cookie = 111;
    assert(!pthread_mutex_destroy(&owner.owner.lock));

    init_array(&array, &owner);
    assert(bpf_prog_array_compatible(&array, &pn));
    assert(!bpf_prog_array_compatible(&array, &pa));
    assert(!pthread_mutex_destroy(&owner.owner.lock));

    init_array(&array, &owner);
    assert(bpf_prog_array_compatible(&array, &pap));
    assert(bpf_prog_array_compatible(&array, &pa));
    assert(!bpf_prog_array_compatible(&array, &paq));
    assert(!pthread_mutex_destroy(&owner.owner.lock));

    init_array(&array, &owner);
    assert(!pthread_barrier_init(&barrier, NULL, 16));
    for (int i = 0; i < 16; i++) {
        args[i] = (struct race_arg){ &array, i % 2 ? &pa : &pb, &barrier, false };
        assert(!pthread_create(&threads[i], NULL, race, &args[i]));
    }
    for (int i = 0; i < 16; i++) assert(!pthread_join(threads[i], NULL));
    assert(owner.owner.storage_cookie[SHARED] == a.cookie ||
           owner.owner.storage_cookie[SHARED] == b.cookie);
    for (int i = 0; i < 16; i++)
        assert(args[i].result == (args[i].prog->aux->cgroup_storage[SHARED]->cookie ==
                                  owner.owner.storage_cookie[SHARED]));
    assert(!pthread_barrier_destroy(&barrier));
    assert(!pthread_mutex_destroy(&owner.owner.lock));
    puts("PASS: tail-call storage identity, pointer reuse and concurrent claims");
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix="bpf-cgroup-tailcall-") as tmp:
    test = Path(tmp) / "test.c"
    exe = Path(tmp) / "test"
    test.write_text(prefix + function + suffix)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O1", "-g", "-pthread",
        "-Wall", "-Wextra", "-Werror", "-fsanitize=undefined",
        "-fno-sanitize-recover=all", str(test), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
