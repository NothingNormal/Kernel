# Sixfaced CI - FAILED run #171

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/41801990080
- ref     : refs/heads/bakasu
- commit  : 5735e2b6c4fa577e242f451a9e9dea8ca54cf056
- date    : 2026-11-04 08:49:42 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
326:../fs/namei.c:1748:65: error: use of undeclared identifier 'found_sus_path'
329:../fs/namei.c:1763:3: error: use of undeclared identifier 'found_sus_path'
332:../fs/namei.c:1764:8: error: use of undeclared label 'retry'
336:make[2]: *** [../scripts/Makefile.build:364: fs/namei.o] Error 1
337:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1341: fs] Error 2
637:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  AR      arch/arm64/kernel/probes/built-in.a
  LDS     arch/arm64/kernel/vmlinux.lds
  AR      arch/arm64/kernel/built-in.a
  CC      kernel/bpf/inode.o
  CC      kernel/cgroup/cgroup-v1.o
  CC      kernel/events/core.o
  CC      mm/debug.o
  CC      kernel/irq/irqdesc.o
  CC      kernel/bpf/helpers.o
  CC      mm/gup.o
  CC      kernel/cgroup/freezer.o
  CC      kernel/bpf/tnum.o
  CC      kernel/irq/handle.o
  AR      kernel/livepatch/built-in.a
  CC      kernel/locking/mutex.o
  CC      mm/highmem.o
  CC      kernel/cgroup/legacy_freezer.o
  CC      kernel/bpf/bpf_iter.o
  CC      kernel/irq/manage.o
  CC      mm/memory.o
  CC      kernel/locking/semaphore.o
  CC      kernel/cgroup/pids.o
  CC      kernel/events/ring_buffer.o
  CC      kernel/bpf/map_iter.o
  CC      kernel/irq/spurious.o
  CC      kernel/locking/rwsem.o
  CC      kernel/cgroup/cpuset.o
  CC      mm/mincore.o
  CC      kernel/locking/percpu-rwsem.o
  CC      kernel/irq/resend.o
  CC      kernel/locking/spinlock.o
  CC      kernel/locking/osq_lock.o
  CC      kernel/events/callchain.o
  CC      kernel/irq/chip.o
  CC      mm/mlock.o
  CC      kernel/bpf/task_iter.o
  CC      kernel/locking/rtmutex.o
  CC      kernel/events/hw_breakpoint.o
  CC      kernel/locking/rwsem-xadd.o
  CC      kernel/irq/dummychip.o
  AR      kernel/cgroup/built-in.a
  CC      mm/mmap.o
  CC      kernel/power/qos.o
  CC      kernel/bpf/prog_iter.o
  CC      kernel/events/uprobes.o
  CC      kernel/locking/qrwlock.o
  AR      kernel/locking/built-in.a
  CC      kernel/printk/printk.o
  CC      kernel/irq/devres.o
  CC      kernel/bpf/hashtab.o
  CC      kernel/power/main.o
  AR      kernel/events/built-in.a
  CC      kernel/irq/autoprobe.o
  CC      kernel/rcu/update.o
  CC      mm/mmu_gather.o
  CC      kernel/power/console.o
  CC      kernel/rcu/sync.o
  CC      kernel/printk/printk_safe.o
  CC      kernel/irq/irqdomain.o
  AR      kernel/printk/built-in.a
  CC      kernel/sched/core.o
  CC      mm/mprotect.o
  CC      kernel/bpf/arraymap.o
  CC      kernel/rcu/srcutree.o
  CC      kernel/irq/proc.o
  CC      kernel/power/process.o
  CC      kernel/bpf/percpu_freelist.o
  CC      mm/mremap.o
  CC      kernel/bpf/bpf_lru_list.o
  CC      kernel/bpf/lpm_trie.o
  CC      kernel/rcu/tree.o
  CC      kernel/irq/cpuhotplug.o
  CC      mm/msync.o
  CC      kernel/power/suspend.o
  CC      kernel/irq/pm.o
  CC      kernel/bpf/map_in_map.o
  CC      kernel/sched/loadavg.o
  CC      mm/page_vma_mapped.o
  CC      kernel/power/wakelock.o
  CC      kernel/bpf/local_storage.o
  CC      kernel/irq/affinity.o
  CC      kernel/rcu/rcu_segcblist.o
  CC      kernel/sched/clock.o
  AR      kernel/rcu/built-in.a
  CC      kernel/power/poweroff.o
  CC      mm/pagewalk.o
  AR      kernel/irq/built-in.a
  CC      mm/pgtable-generic.o
  CC      kernel/time/time.o
  CC      kernel/sched/cputime.o
  CC      kernel/bpf/queue_stack_maps.o
  CC      kernel/power/wakeup_reason.o
  CC      mm/rmap.o
  CC      kernel/time/timer.o
  CC      kernel/bpf/ringbuf.o
  AR      kernel/power/built-in.a
  CC      kernel/sched/idle_task.o
  CC      kernel/trace/trace_clock.o
  CC      kernel/sched/fair.o
  CC      mm/vmalloc.o
  CC      kernel/trace/ring_buffer.o
  CC      kernel/bpf/bpf_local_storage.o
  CC      kernel/time/hrtimer.o
  CC      kernel/time/timekeeping.o
  CC      kernel/time/ntp.o
  CC      mm/process_vm_access.o
  CC      kernel/trace/trace.o
  CC      kernel/freezer.o
  CC      kernel/bpf/bpf_task_storage.o
  CC      kernel/time/clocksource.o
  CC      kernel/sched/rt.o
  CC      mm/showmem.o
  CC      kernel/profile.o
  CC      kernel/time/jiffies.o
  CC      mm/vmpressure.o
  CC      kernel/bpf/disasm.o
  CC      kernel/trace/trace_output.o
  CC      kernel/sched/deadline.o
  CC      kernel/time/timer_list.o
  CC      kernel/stacktrace.o
  CC      mm/init-mm.o
  CC      kernel/bpf/trampoline.o
  CC      kernel/time/timeconv.o
  CC      kernel/futex.o
  CC      mm/nobootmem.o
  CC      kernel/trace/trace_seq.o
  CC      kernel/sched/wait.o
  CC      kernel/time/timecounter.o
  CC      kernel/time/alarmtimer.o
  CC      kernel/bpf/btf.o
  CC      mm/fadvise.o
  CC      kernel/trace/trace_stat.o
  CC      kernel/sched/wait_bit.o
  CC      kernel/time/posix-timers.o
  CC      kernel/trace/trace_printk.o
  CC      mm/madvise.o
  CC      kernel/sched/swait.o
  CC      kernel/time/posix-cpu-timers.o
  CC      kernel/trace/trace_sched_switch.o
  CC      kernel/sched/completion.o
  CC      kernel/bpf/dispatcher.o
  CC      kernel/time/posix-clock.o
  CC      kernel/time/itimer.o
  CC      kernel/sched/idle.o
  CC      mm/memblock.o
  CC      kernel/smp.o
  CC      kernel/trace/trace_nop.o
  CC      kernel/time/clockevents.o
  CC      kernel/bpf/devmap.o
  CC      mm/page_io.o
  CC      kernel/sched/cpupri.o
  CC      kernel/module.o
  CC      kernel/time/tick-common.o
  CC      kernel/trace/blktrace.o
  CC      kernel/sched/cpudeadline.o
  CC      kernel/bpf/cpumap.o
  CC      mm/swap_state.o
  CC      kernel/sched/topology.o
  CC      kernel/time/tick-broadcast.o
  CC      kernel/trace/trace_events.o
  CC      mm/swapfile.o
  CC      kernel/uid16.o
  CC      kernel/sched/stop_task.o
  CC      kernel/time/tick-broadcast-hrtimer.o
  CC      kernel/bpf/offload.o
  CC      kernel/time/sched_clock.o
  CC      kernel/sched/sched_avg.o
  CC      kernel/kallsyms.o
  CC      kernel/trace/trace_export.o
  CC      mm/swap_ratio.o
  CC      kernel/time/tick-oneshot.o
  CC      kernel/bpf/net_namespace.o
  CC      kernel/time/tick-sched.o
  CC      kernel/sched/energy.o
  CC      kernel/sched/tune.o
  CC      kernel/trace/trace_event_perf.o
  CC      mm/dmapool.o
  CC      kernel/compat.o
  CC      kernel/sched/cpuacct.o
  AR      kernel/time/built-in.a
  CC      kernel/bpf/stackmap.o
  CC      mm/sparse.o
  CC      kernel/bpf/cgroup.o
  CC      kernel/trace/trace_events_filter.o
  CC      mm/sparse-vmemmap.o
  CC      kernel/utsname.o
  CC      kernel/sched/cpufreq.o
  CC      kernel/bpf/reuseport_array.o
  CC      kernel/bpf/bpf_struct_ops.o
  CC      kernel/trace/trace_events_trigger.o
  CC      mm/slub.o
  CC      kernel/sched/cpufreq_schedutil.o
  CC      kernel/user_namespace.o
  CC      mm/migrate.o
  CC      kernel/trace/bpf_trace.o
  CC      kernel/sched/membarrier.o
  AR      kernel/bpf/built-in.a
  CC      kernel/trace/error_report-traces.o
  CC      kernel/pid_namespace.o
  CC      kernel/trace/power-traces.o
  CC      mm/page_counter.o
  CC      mm/memcontrol.o
  CC      kernel/sched/psi.o
  GZIP    kernel/config_data.gz
  CC      kernel/trace/rpm-traces.o
  CC      mm/page_isolation.o
  CC      kernel/trace/trace_probe.o
  CC      kernel/stop_machine.o
  AR      kernel/sched/built-in.a
  CC      kernel/audit.o
  CC      mm/zsmalloc.o
  CC      kernel/trace/trace_uprobe.o
  CC      mm/early_ioremap.o
  CC      kernel/auditfilter.o
  CC      mm/cma.o
  CC      mm/userfaultfd.o
  CC      kernel/auditsc.o
  CC      kernel/audit_watch.o
  AR      kernel/trace/built-in.a
  CC      mm/frame_vector.o
  CC      kernel/audit_fsnotify.o
  CC      mm/usercopy.o
  CC      kernel/audit_tree.o
  CC      kernel/seccomp.o
  CC      kernel/utsname_sysctl.o
  CC      kernel/taskstats.o
  CC      kernel/tsacct.o
  CC      kernel/tracepoint.o
  AR      mm/built-in.a
  CC      kernel/irq_work.o
  CC      kernel/cpu_pm.o
  CC      kernel/scs.o
  CC      kernel/jump_label.o
  CC      kernel/memremap.o
  CHK     kernel/config_data.h
  UPD     kernel/config_data.h
  CC      kernel/configs.o
  AR      kernel/built-in.a
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   5207 / 7172 (72.60%)
  Hits:            1057 / 5207 (20.30%)
    Direct:        1032 / 1057 (97.63%)
    Preprocessed:    25 / 1057 ( 2.37%)
  Misses:          4150 / 5207 (79.70%)
Uncacheable calls: 1965 / 7172 (27.40%)
Local storage:
  Cache size (GB):  0.2 /  4.0 ( 4.99%)
  Hits:            1057 / 5207 (20.30%)
  Misses:          4150 / 5207 (79.70%)
 Kernel build failed (make exit code 2)
```
