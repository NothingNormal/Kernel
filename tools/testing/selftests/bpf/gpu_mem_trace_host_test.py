#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host mock of KGSL's source-extracted BPF GPU-memory tracing.

Run with python3 tools/testing/selftests/bpf/gpu_mem_trace_host_test.py.
This tests ordering and accounting, not real GPU hardware or kernel locking.
"""
from pathlib import Path
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
s=(ROOT / 'drivers/gpu/msm/kgsl_device.h').read_text()
start=s.index('static inline void kgsl_trace_gpu_mem_total(')
end=s.index('static inline bool kgsl_is_register_offset(',start)
functions=s[start:end]
assert functions.count('spin_lock_irqsave(&kgsl_gpu_mem_lock, flags);')==2
assert functions.count('spin_unlock_irqrestore(&kgsl_gpu_mem_lock, flags);')==2
c=(ROOT / 'drivers/gpu/msm/kgsl.c').read_text()
start2=c.index('long kgsl_ioctl_map_user_mem('); end2=c.index('static int _kgsl_gpumem_sync_cache',start2)
body=c[start2:end2]
assert 'KGSL_STATS_ADD(entry->memdesc.size, &kgsl_driver.stats.mapped' in body
assert 'kgsl_memdesc_usermem_type(&entry->memdesc), entry->memdesc.size)' in body
prefix=r'''
#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <assert.h>
#ifndef CONFIG_TRACE_GPU_MEM
#define CONFIG_TRACE_GPU_MEM 1
#endif
#define IS_ENABLED(x) (x)
#define KGSL_MEM_ENTRY_MAX 3
#define PAGE_SHIFT 12
#define MM_UNRECLAIMABLE 1
typedef uint64_t u64; typedef int64_t s64; typedef _Atomic int64_t atomic64_t;
struct pid { int nr; }; struct mm_struct { int x; };
struct task_struct { struct mm_struct *mm; };
struct kgsl_process_private { struct pid *pid; struct { atomic64_t cur,max; } stats[KGSL_MEM_ENTRY_MAX]; };
static struct task_struct test_task; static struct task_struct *current=&test_task;
static pthread_mutex_t kgsl_gpu_mem_lock=PTHREAD_MUTEX_INITIALIZER;
static atomic64_t kgsl_gpu_mem_total=0;
static int trace_enabled=1; static uint64_t pending=0; static unsigned long pair_count=0;
#define spin_lock_irqsave(lock, flags) do { (flags)=0; assert(!pthread_mutex_lock(lock)); } while(0)
#define spin_unlock_irqrestore(lock, flags) do { (void)(flags); assert(!pthread_mutex_unlock(lock)); } while(0)
static s64 atomic64_add_return(s64 n,atomic64_t *v) { return atomic_fetch_add(v,n)+n; }
static void atomic64_sub(s64 n,atomic64_t *v) { atomic_fetch_sub(v,n); }
static s64 atomic64_read(atomic64_t *v) { return atomic_load(v); }
static void atomic64_set(atomic64_t *v,s64 n) { atomic_store(v,n); }
static int pid_nr(struct pid *p) { return p->nr; }
static int trace_gpu_mem_total_enabled(void) { return trace_enabled; }
static void trace_gpu_mem_total(unsigned int gpu,unsigned int pid,uint64_t size) {
    (void)gpu;
    assert(size < 100000000);
    if (pid) { assert(pid==123 && !pending); pending=size+1; }
    else { assert(pending && pending-1==size); pending=0; pair_count++; }
    sched_yield();
}
static void add_mm_counter(struct mm_struct *mm,int key,s64 delta) { (void)mm;(void)key;(void)delta; }
static struct pid *find_get_pid(int pid) { (void)pid;return NULL; }
static struct task_struct *get_pid_task(struct pid *p,int type) { (void)p;(void)type;return NULL; }
static struct mm_struct *get_task_mm(struct task_struct *t) { return t->mm; }
static void mmput(struct mm_struct *mm) { (void)mm; }
static void put_task_struct(struct task_struct *t) { (void)t; }
static void put_pid(struct pid *p) { (void)p; }
#define PIDTYPE_PID 0
'''
suffix=r'''
static struct pid pid={123}; static struct kgsl_process_private process={.pid=&pid};
static void *worker(void *arg) {
    uintptr_t i=(uintptr_t)arg; int type=i%KGSL_MEM_ENTRY_MAX; uint64_t size=(i+1)*4096;
    for (int n=0;n<2000;n++) { kgsl_process_add_stats(&process,type,size); sched_yield(); kgsl_process_sub_stats(&process,type,size); }
    return NULL;
}
int main(void) {
#if CONFIG_TRACE_GPU_MEM
    pthread_t threads[8];
    trace_enabled=0; kgsl_process_add_stats(&process,0,8192);
    assert(atomic64_read(&kgsl_gpu_mem_total)==8192 && pair_count==0);
    trace_enabled=1; kgsl_process_add_stats(&process,1,4096);
    assert(atomic64_read(&kgsl_gpu_mem_total)==12288 && pair_count==1);
    kgsl_process_sub_stats(&process,1,4096); kgsl_process_sub_stats(&process,0,8192);
    for (uintptr_t i=0;i<8;i++) assert(!pthread_create(&threads[i],NULL,worker,(void *)i));
    for (int i=0;i<8;i++) assert(!pthread_join(threads[i],NULL));
    assert(atomic64_read(&kgsl_gpu_mem_total)==0 && pending==0);
    for (int i=0;i<KGSL_MEM_ENTRY_MAX;i++) assert(atomic64_read(&process.stats[i].cur)==0);
    assert(pair_count==8*2000*2+3);
    printf("PASS: %lu serialized GPU trace pairs, disabled tracing, and balanced totals\n",pair_count);
#else
    kgsl_process_add_stats(&process,0,8192); kgsl_process_sub_stats(&process,0,8192);
    assert(atomic64_read(&process.stats[0].cur)==0); puts("PASS: GPU trace disabled config");
#endif
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix="kgsl-bpf-test-") as tmp:
    t = Path(tmp)
    (t / "gpu_stats_host.c").write_text(prefix + functions + suffix)
    for enabled in (0, 1):
        exe = t / f"gpu-stats-{enabled}"
        subprocess.run([
            os.environ.get("CC", "cc"), "-std=gnu11", "-O1", "-g",
            "-Wall", "-Wextra", "-Werror", "-Wno-sign-compare",
            "-Wno-unused-parameter", "-Wno-unused-function", "-Wno-unused-variable",
            "-pthread", "-fsanitize=undefined", f"-DCONFIG_TRACE_GPU_MEM={enabled}",
            str(t / "gpu_stats_host.c"), "-o", str(exe),
        ], check=True)
        subprocess.run([str(exe)], check=True)
