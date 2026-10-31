#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Run source-extracted BPF hash-map regressions with host-side mocks.

Covers recursion masks, LRU failed-lock cleanup, reuse and callback traversal.
This is not a test of real kernel locking, RCU, or concurrent execution.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[4]
source = (root / "kernel/bpf/hashtab.c").read_text()


def extract(start, end):
    first = source.index(start)
    return source[first:source.index(end, first)]


lock_functions = extract("static inline int htab_lock_bucket(",
                         "static bool htab_lru_map_delete_node(")
lru_update = extract("static int htab_lru_map_update_elem(",
                     "static int __htab_percpu_map_update_elem(")
lru_percpu_update = extract("static int __htab_lru_percpu_map_update_elem(",
                            "static int htab_percpu_map_update_elem(")
lru_pop = extract("static struct htab_elem *prealloc_lru_pop(",
                  "static int prealloc_init(")
iteration = extract("static int bpf_for_each_hash_elem(",
                    "static int htab_map_btf_id;")
batch_body = extract("\t/* do not grab the lock unless need it (bucket_cnt > 0). */",
                     "\n\tbucket_cnt = 0;")

lock_prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
typedef uint32_t u32;
#define HASHTAB_MAP_LOCK_MASK 7
#define min_t(type, a, b) ((type)(a) < (type)(b) ? (type)(a) : (type)(b))
#define unlikely(x) (x)
#define __this_cpu_inc_return(x) (++(x))
#define __this_cpu_dec(x) (--(x))
struct bpf_htab { u32 n_buckets; bool raw; int *map_locked[8]; };
struct bucket { int raw_lock, lock; };
static int depth;
#define preempt_disable() (++depth)
#define preempt_enable() (--depth)
#define migrate_disable() (++depth)
#define migrate_enable() (--depth)
#define htab_use_raw_lock(h) ((h)->raw)
#define raw_spin_lock_irqsave(p, flags) do { assert(!*(p)); *(p) = 1; (flags) = 0; } while (0)
#define spin_lock_irqsave(p, flags) raw_spin_lock_irqsave(p, flags)
#define raw_spin_unlock_irqrestore(p, flags) do { (void)(flags); assert(*(p)); *(p) = 0; } while (0)
#define spin_unlock_irqrestore(p, flags) raw_spin_unlock_irqrestore(p, flags)
'''
lock_suffix = r'''
int main(void) {
    for (int raw = 0; raw <= 1; raw++) {
        for (u32 n = 1; n <= 16; n *= 2) {
            int counters[8] = {0}; struct bpf_htab h = {.n_buckets=n, .raw=raw};
            struct bucket b = {0}; unsigned long first, second;
            for (int i = 0; i < 8; i++) h.map_locked[i] = &counters[i];
            assert(htab_lock_bucket(&h, &b, 0, &first) == 0);
            assert(depth == 1);
            /* Simulated nested interrupt: same bucket, potentially different hash. */
            assert(htab_lock_bucket(&h, &b, n, &second) == -EBUSY);
            assert(depth == 1);
            htab_unlock_bucket(&h, &b, 0, first);
            assert(depth == 0);
            for (int i = 0; i < 8; i++) assert(counters[i] == 0);
        }
    }
    puts("PASS: hash bucket recursion uses the same guard for the same bucket");
    return 0;
}
'''

lru_prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
typedef uint32_t u32;
typedef uint64_t u64;
#define unlikely(x) (x)
#define BPF_EXIST 2
#define round_up(x, a) (((x) + (a) - 1) & ~((a) - 1))
#define container_of(p, type, member) ((type *)((char *)(p) - offsetof(type, member)))
#define WARN_ON_ONCE(x) ((void)(x))
#define rcu_read_lock_held() 1
#define rcu_read_lock_trace_held() 1
#define rcu_read_lock_bh_held() 1
struct bpf_map { u32 key_size; };
struct bpf_lru_node { int dummy; };
struct hlist_nulls_node { int dummy; };
struct hlist_nulls_head { int dummy; };
struct htab_elem { struct bpf_lru_node lru_node; struct hlist_nulls_node hash_node; char key[32]; };
struct bucket { struct hlist_nulls_head head; };
struct bpf_htab { struct bpf_map map; int hashrnd, lru; struct bucket bucket; };
static struct htab_elem new_elem, old_elem;
static int busy, old_exists, frees, unlocks, added;
static u32 htab_map_hash(void *k, u32 n, int rnd) { (void)k; (void)n; (void)rnd; return 0; }
static struct bucket *__select_bucket(struct bpf_htab *h, u32 hash) { (void)hash; return &h->bucket; }
static struct htab_elem *prealloc_lru_pop(struct bpf_htab *h, void *k, u32 hash) {
    (void)h; (void)k; (void)hash; return &new_elem;
}
static void copy_map_value(struct bpf_map *m, void *d, void *s) { (void)m; (void)d; (void)s; }
static int htab_lock_bucket(struct bpf_htab *h, struct bucket *b, u32 hash, unsigned long *f) {
    (void)h; (void)b; (void)hash; *f = 0; return busy ? -EBUSY : 0;
}
static void htab_unlock_bucket(struct bpf_htab *h, struct bucket *b, u32 hash, unsigned long f) {
    (void)h; (void)b; (void)hash; (void)f; unlocks++;
}
static struct htab_elem *lookup_elem_raw(struct hlist_nulls_head *h, u32 a, void *b, u32 c) {
    (void)h; (void)a; (void)b; (void)c; return old_exists ? &old_elem : NULL;
}
static int check_flags(struct bpf_htab *h, struct htab_elem *e, u64 f) { (void)h; (void)e; (void)f; return 0; }
static void hlist_nulls_add_head_rcu(struct hlist_nulls_node *n, struct hlist_nulls_head *h) {
    (void)n; (void)h; added++;
}
static void hlist_nulls_del_rcu(struct hlist_nulls_node *n) { (void)n; }
static void bpf_lru_node_set_ref(struct bpf_lru_node *n) { (void)n; }
static void htab_lru_push_free(struct bpf_htab *h, struct htab_elem *e) { (void)h; assert(e); frees++; }
static void bpf_lru_push_free(int *lru, struct bpf_lru_node *n) { (void)lru; assert(n); frees++; }
static void *htab_elem_get_ptr(struct htab_elem *e, u32 n) { (void)n; return e->key; }
static void pcpu_copy_value(struct bpf_htab *h, void *d, void *s, bool all) { (void)h; (void)d; (void)s; (void)all; }
static void pcpu_init_value(struct bpf_htab *h, void *d, void *s, bool all) { (void)h; (void)d; (void)s; (void)all; }
'''
lru_suffix = r'''
int main(void) {
    struct bpf_htab h = {.map.key_size=4}; int key = 1, val = 2;
    busy = 1;
    assert(htab_lru_map_update_elem(&h.map, &key, &val, 0) == -EBUSY);
    assert(frees == 1 && unlocks == 0 && added == 0);
    frees = 0;
    assert(__htab_lru_percpu_map_update_elem(&h.map, &key, &val, 0, false) == -EBUSY);
    assert(frees == 1 && unlocks == 0 && added == 0);
    frees = 0;
    assert(__htab_lru_percpu_map_update_elem(&h.map, &key, &val, BPF_EXIST, false) == -EBUSY);
    assert(frees == 0 && unlocks == 0);
    busy = 0;
    assert(htab_lru_map_update_elem(&h.map, &key, &val, 0) == 0);
    assert(frees == 0 && unlocks == 1 && added == 1);
    frees = unlocks = added = 0;
    assert(__htab_lru_percpu_map_update_elem(&h.map, &key, &val, 0, false) == 0);
    assert(frees == 0 && unlocks == 1 && added == 1);
    frees = unlocks = added = 0; old_exists = 1;
    assert(htab_lru_map_update_elem(&h.map, &key, &val, 0) == 0);
    assert(frees == 1 && unlocks == 1 && added == 1);
    frees = unlocks = added = 0;
    assert(__htab_lru_percpu_map_update_elem(&h.map, &key, &val, 0, false) == 0);
    assert(frees == 1 && unlocks == 1 && added == 0);
    puts("PASS: LRU and per-CPU LRU return preallocated entries after lock failure");
    return 0;
}
'''

pop_prefix = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint32_t u32;
#define round_up(x, a) (((x) + (a) - 1) & ~((a) - 1))
#define container_of(p, type, member) ((type *)((char *)(p) - offsetof(type, member)))
struct bpf_lru_node { int dummy; };
struct htab_elem { struct bpf_lru_node lru_node; unsigned char key[32]; };
struct bpf_map { u32 key_size; };
struct bpf_htab { struct bpf_map map; int lru; };
static struct htab_elem elem;
static struct bpf_lru_node *bpf_lru_pop_free(int *lru, u32 hash) { (void)lru; (void)hash; return &elem.lru_node; }
/* Used by a deliberately reverted mutant. */
static void __attribute__((unused)) check_and_init_map_value(struct bpf_map *map, void *value) {
    (void)map; memset(value, 0, 16);
}
'''
pop_suffix = r'''
int main(void) {
    struct bpf_htab h = {.map.key_size=4}; unsigned char key[4] = {1,2,3,4};
    memset(elem.key, 0x5a, sizeof(elem.key));
    assert(prealloc_lru_pop(&h, key, 0) == &elem);
    assert(memcmp(elem.key, key, sizeof(key)) == 0);
    for (int i = 4; i < 32; i++) assert(elem.key[i] == 0x5a);
    puts("PASS: reused LRU values do not reinitialize live special fields");
    return 0;
}
'''

batch_prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
static int instrumentation, rcu, busy;
static int htab_lock_bucket(void *htab, void *b, int batch, unsigned long *flags) {
    (void)htab; (void)b; (void)batch; *flags = 0; return busy ? -EBUSY : 0;
}
#define rcu_read_unlock() (--rcu)
#define bpf_enable_instrumentation() (--instrumentation)
static int batch_check(bool locked) {
    void *htab = 0, *b = 0; int batch = 0, ret = 0; unsigned long flags = 0;
    instrumentation = rcu = 1;
    if (batch == -1) goto next_batch; /* label retained for the reverted mutant */
'''
batch_suffix = r'''
    rcu_read_unlock(); bpf_enable_instrumentation();
    goto after_loop;
next_batch:
    rcu_read_unlock(); bpf_enable_instrumentation();
    return 99; /* incorrect skip instead of propagating the lock error */
after_loop:
    assert(rcu == 0 && instrumentation == 0);
    return ret;
}
int main(void) {
    busy = 1; assert(batch_check(true) == -EBUSY);
    busy = 0; assert(batch_check(true) == 0);
    busy = 1; assert(batch_check(false) == 0);
    puts("PASS: batch lock failures propagate after releasing RCU/instrumentation");
    return 0;
}
'''

iteration_prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
typedef uint32_t u32;
typedef uint64_t u64;
#define __percpu
#define round_up(x, a) (((x) + (a) - 1) & ~((a) - 1))
#define container_of(p, type, member) ((type *)((char *)(p) - offsetof(type, member)))
#define BPF_CAST_CALL(p) ((u64 (*)(u64, u64, u64, u64, u64))(p))
struct hlist_nulls_node { struct hlist_nulls_node *next; };
struct hlist_nulls_head { struct hlist_nulls_node *first; };
struct htab_elem { struct hlist_nulls_node hash_node; unsigned char key[16]; };
struct bucket { struct hlist_nulls_head head; };
struct bpf_map { u32 key_size; };
struct bpf_htab { struct bpf_map map; int n_buckets; struct bucket *buckets; };
#define hlist_nulls_for_each_entry_safe(elem, n, head, member) \
    for ((n) = (head)->first; (n) && ((elem) = container_of((n), struct htab_elem, member), \
         (n) = (n)->next, 1); )
#define hlist_nulls_for_each_entry_rcu(elem, n, head, member) \
    for ((n) = (head)->first; (n) && ((elem) = container_of((n), struct htab_elem, member), 1); \
         (n) = (n)->next)
#define rcu_read_lock() do { } while (0)
#define rcu_read_unlock() do { } while (0)
#define migrate_disable() do { } while (0)
#define migrate_enable() do { } while (0)
#define this_cpu_ptr(p) (p)
static bool htab_is_percpu(struct bpf_htab *h) { (void)h; return false; }
static void *htab_elem_get_ptr(struct htab_elem *e, u32 n) { (void)n; return e->key; }
'''
iteration_suffix = r'''
struct context { struct htab_elem *first; int calls; };
static u64 callback(u64 map, u64 key, u64 value, u64 arg, u64 unused) {
    struct context *c = (void *)(uintptr_t)arg;
    (void)map; (void)value; (void)unused;
    c->calls++;
    if ((void *)(uintptr_t)key == c->first->key)
        c->first->hash_node.next = &c->first->hash_node; /* removed/reused node */
    return c->calls > 2; /* bound a deliberately broken iteration mutant */
}
int main(void) {
    struct htab_elem a = {0}, b = {0}; struct bucket bucket = {0};
    struct bpf_htab h = {.map.key_size=4, .n_buckets=1, .buckets=&bucket};
    struct context c = {.first=&a};
    a.hash_node.next = &b.hash_node; bucket.head.first = &a.hash_node;
    assert(bpf_for_each_hash_elem(&h.map, callback, &c, 0) == 2);
    assert(c.calls == 2);
    assert(bpf_for_each_hash_elem(&h.map, callback, &c, 1) == -EINVAL);
    puts("PASS: hash iteration retains next node across callback removal/reuse");
    return 0;
}
'''


def run_case(tmp, name, program):
    test_file = Path(tmp) / (name + ".c")
    exe = Path(tmp) / name
    test_file.write_text(program)
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=gnu11", "-O2", "-g",
        "-Wall", "-Wextra", "-Werror", "-fsanitize=undefined",
        "-fno-sanitize-recover=all", str(test_file), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)


with tempfile.TemporaryDirectory(prefix="bpf-hash-safety-") as tmp:
    run_case(tmp, "lock", lock_prefix + lock_functions + lock_suffix)
    run_case(tmp, "lru", lru_prefix + lru_update + lru_percpu_update + lru_suffix)
    run_case(tmp, "reuse", pop_prefix + lru_pop + pop_suffix)
    run_case(tmp, "batch", batch_prefix + batch_body + batch_suffix)
    run_case(tmp, "iteration", iteration_prefix + iteration + iteration_suffix)
