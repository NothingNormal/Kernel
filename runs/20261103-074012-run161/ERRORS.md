# Sixfaced CI - FAILED run #161

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/41597446350
- ref     : refs/heads/arena/0fc62de9-kernel
- commit  : 25839d56a3b01be4d8c626da2f0868c13f054a5c
- date    : 2026-11-03 07:40:12 UTC
- clang   : Ubuntu clang version 21.1.8 (6ubuntu1)

## Extracted errors (last 300 matches)
```
179:../kernel/fork.c:2309:3: error: expected identifier or '('
183:make[2]: *** [../scripts/Makefile.build:364: kernel/fork.o] Error 1
184:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: kernel] Error 2
344:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  SHIPPED scripts/dtc/dtc-parser.tab.h
  SHIPPED scripts/dtc/dtc-parser.tab.c
  HOSTCC  scripts/dtc/dtc-lexer.lex.o
  HOSTCC  scripts/dtc/dtc-parser.tab.o
  MKELF   scripts/mod/elfconfig.h
  CHK     scripts/mod/devicetable-offsets.h
  UPD     scripts/mod/devicetable-offsets.h
  HOSTCC  scripts/mod/sumversion.o
  HOSTLD  scripts/dtc/dtc
  HOSTCC  scripts/mod/modpost.o
  HOSTCC  scripts/selinux/genheaders/genheaders
  HOSTCC  scripts/mod/file2alias.o
  HOSTLD  scripts/mod/modpost
  CC      kernel/bounds.s
  HOSTCC  scripts/selinux/mdp/mdp
  HOSTCC  scripts/kallsyms
  CHK     include/generated/timeconst.h
  UPD     include/generated/timeconst.h
  CHK     include/generated/bounds.h
  UPD     include/generated/bounds.h
  CC      arch/arm64/kernel/asm-offsets.s
  HOSTCC  scripts/conmakehash
  HOSTCC  scripts/sortextable
  HOSTCC  scripts/asn1_compiler
  HOSTCC  scripts/extract-cert
  CHK     include/generated/asm-offsets.h
  UPD     include/generated/asm-offsets.h
  CALL    ../scripts/checksyscalls.sh
  LDS     arch/arm64/kernel/vdso/vdso.lds
  VDSOA   arch/arm64/kernel/vdso/gettimeofday.o
  VDSOA   arch/arm64/kernel/vdso/note.o
  VDSOA   arch/arm64/kernel/vdso/sigreturn.o
  LD      arch/arm64/kernel/vdso/vdso.so.dbg
  VDSOSYM include/generated/vdso-offsets.h
  CC      init/main.o
  CHK     include/generated/compile.h
  CC      init/do_mounts.o
  CC      init/do_mounts_rd.o
  CC      init/do_mounts_initrd.o
  UPD     include/generated/compile.h
  CC      init/do_mounts_dm.o
  HOSTCC  usr/gen_init_cpio
  GEN     usr/initramfs_data.cpio
  AS      usr/initramfs_data.o
  AR      usr/built-in.a
  CC      init/noinitramfs.o
  CC      arch/arm64/kernel/debug-monitors.o
  CC      init/initramfs.o
  CC      init/calibrate.o
  AS      arch/arm64/kernel/entry.o
  CC      arch/arm64/kernel/irq.o
  CC      arch/arm64/mm/dma-mapping.o
  CC      arch/arm64/net/bpf_jit_comp.o
  CC      arch/arm64/kernel/fpsimd.o
  CC      arch/arm64/mm/extable.o
  AS      arch/arm64/kernel/entry-fpsimd.o
  CC      init/init_task.o
  CC      arch/arm64/kernel/process.o
  AR      arch/arm64/net/built-in.a
  CC      arch/arm64/crypto/sha1-ce-glue.o
  CC      arch/arm64/kernel/ptrace.o
  CC      arch/arm64/mm/fault.o
  CC      kernel/fork.o
  CC      init/version.o
  CC      arch/arm64/mm/init.o
  AR      init/mounts.o
  AS      arch/arm64/crypto/sha1-ce-core.o
  CC      arch/arm64/crypto/sha2-ce-glue.o
  AR      init/built-in.a
  AS      arch/arm64/mm/cache.o
  CC      arch/arm64/kernel/setup.o
  CC      certs/system_keyring.o
  CC      mm/filemap.o
../kernel/fork.c:2309:3: error: expected identifier or '('
 2309 |                 if (IS_ENABLED(CONFIG_LRU_GEN) && !(clone_flags & CLONE_VM)) {
      |                 ^
1 error generated.
make[2]: *** [../scripts/Makefile.build:364: kernel/fork.o] Error 1
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: kernel] Error 2
make[1]: *** Waiting for unfinished jobs....
  AS      arch/arm64/crypto/sha2-ce-core.o
  CC      mm/mempool.o
  EXTRACT_CERTS   
  AS      certs/system_certificates.o
  CC      arch/arm64/crypto/ghash-ce-glue.o
  AR      certs/built-in.a
  CC      arch/arm64/kernel/signal.o
  CC      arch/arm64/mm/copypage.o
  CC      arch/arm64/kernel/sys.o
  CC      mm/oom_kill.o
  AS      arch/arm64/crypto/ghash-ce-core.o
  AS      arch/arm64/crypto/aes-ce-core.o
  CC      arch/arm64/crypto/aes-ce-glue.o
  CC      arch/arm64/mm/flush.o
  CC      mm/maccess.o
  CC      arch/arm64/mm/ioremap.o
  CC      mm/page_alloc.o
  CC      arch/arm64/crypto/aes-ce-ccm-glue.o
  CC      arch/arm64/kernel/stacktrace.o
  CC      mm/page-writeback.o
  CC      mm/readahead.o
  CC      arch/arm64/kernel/time.o
  CC      arch/arm64/mm/mmap.o
  AS      arch/arm64/crypto/aes-ce-ccm-core.o
  CC      arch/arm64/crypto/aes-glue-ce.o
  CC      mm/swap.o
  CC      arch/arm64/mm/pgd.o
  CC      arch/arm64/kernel/traps.o
  AS      arch/arm64/crypto/aes-ce.o
  CC      arch/arm64/crypto/aes-glue-neon.o
  AS      arch/arm64/crypto/aes-neon.o
  CC      arch/arm64/mm/mmu.o
  CC      arch/arm64/crypto/sha256-glue.o
  CC      arch/arm64/kernel/io.o
  SHIPPED arch/arm64/crypto/sha256-core.S
  AS      arch/arm64/crypto/aes-cipher-core.o
  CC      arch/arm64/kernel/vdso.o
  CC      arch/arm64/crypto/aes-cipher-glue.o
  CC      arch/arm64/mm/context.o
  AS      arch/arm64/kernel/hyp-stub.o
  CC      mm/truncate.o
  CC      arch/arm64/kernel/psci.o
  CC      arch/arm64/kernel/cpu_ops.o
  AR      arch/arm64/crypto/sha1-ce.o
  AR      arch/arm64/crypto/sha2-ce.o
  AR      arch/arm64/crypto/ghash-ce.o
  AR      arch/arm64/crypto/aes-ce-cipher.o
  CC      arch/arm64/kernel/insn.o
  AR      arch/arm64/crypto/aes-ce-ccm.o
  AS      arch/arm64/mm/proc.o
  AR      arch/arm64/crypto/aes-ce-blk.o
  AR      arch/arm64/crypto/aes-neon-blk.o
  CC      mm/vmscan.o
  AS      arch/arm64/crypto/sha256-core.o
  CC      arch/arm64/mm/pageattr.o
  AR      arch/arm64/crypto/aes-arm64.o
  AR      arch/arm64/crypto/sha256-arm64.o
  AR      arch/arm64/crypto/built-in.a
  CC      arch/arm64/kernel/return_address.o
  CC      mm/shmem.o
  CC      arch/arm64/kernel/cpuinfo.o
  CC      mm/util.o
  CC      arch/arm64/kernel/cpu_errata.o
  CC      arch/arm64/kernel/cpufeature.o
  AR      arch/arm64/mm/built-in.a
  CC      mm/mmzone.o
  CC      mm/vmstat.o
  CC      mm/backing-dev.o
  CC      arch/arm64/kernel/alternative.o
  CC      mm/mm_init.o
  CC      arch/arm64/kernel/cacheinfo.o
  CC      arch/arm64/kernel/smp.o
  CC      mm/mmu_context.o
  CC      arch/arm64/kernel/smp_spin_table.o
  CC      arch/arm64/kernel/topology.o
  CC      mm/percpu.o
  CC      mm/slab_common.o
  CC      mm/compaction.o
  AS      arch/arm64/kernel/smccc-call.o
  CC      mm/vmacache.o
  CC      arch/arm64/kernel/sys32.o
  AS      arch/arm64/kernel/kuser32.o
  CC      mm/swap_slots.o
  CC      mm/interval_tree.o
  CC      arch/arm64/kernel/signal32.o
  CC      arch/arm64/kernel/sys_compat.o
  CC      mm/list_lru.o
  CC      mm/workingset.o
  CC      mm/debug.o
  CC      mm/gup.o
  CC      mm/highmem.o
  AS      arch/arm64/kernel/entry32.o
  CC      arch/arm64/kernel/arm64ksyms.o
  CC      mm/memory.o
  CC      arch/arm64/kernel/module.o
  CC      arch/arm64/kernel/module-plts.o
  CC      mm/mincore.o
  CC      mm/mlock.o
  CC      arch/arm64/kernel/perf_regs.o
  CC      mm/mmap.o
  CC      arch/arm64/kernel/perf_callchain.o
  CC      mm/mmu_gather.o
  CC      arch/arm64/kernel/perf_event.o
  CC      arch/arm64/kernel/hw_breakpoint.o
  CC      mm/mprotect.o
  AS      arch/arm64/kernel/sleep.o
  CC      arch/arm64/kernel/suspend.o
  CC      mm/mremap.o
  CC      mm/msync.o
  CC      arch/arm64/kernel/cpuidle.o
  CC      arch/arm64/kernel/jump_label.o
  CC      arch/arm64/kernel/armv8_deprecated.o
  CC      mm/page_vma_mapped.o
  CC      mm/pagewalk.o
  CC      arch/arm64/kernel/kaslr.o
  CC      mm/pgtable-generic.o
  CC      mm/rmap.o
  CC      arch/arm64/kernel/ssbd.o
  CC      arch/arm64/kernel/scs.o
  CC      mm/vmalloc.o
  CC      mm/process_vm_access.o
  CC      arch/arm64/kernel/probes/uprobes.o
  CC      mm/showmem.o
  CC      mm/vmpressure.o
  CC      mm/init-mm.o
  CC      mm/nobootmem.o
  CC      mm/fadvise.o
  CC      arch/arm64/kernel/probes/decode-insn.o
  CC      mm/madvise.o
  CC      mm/memblock.o
  CC      mm/page_io.o
  OBJCOPY arch/arm64/kernel/vdso/vdso.so
  AS      arch/arm64/kernel/vdso/vdso.o
  CC      arch/arm64/kernel/probes/simulate-insn.o
  AS      arch/arm64/kernel/head.o
  AR      arch/arm64/kernel/vdso/built-in.a
  LDS     arch/arm64/kernel/vmlinux.lds
  CC      mm/swap_state.o
  CC      mm/swapfile.o
  CC      mm/swap_ratio.o
  AR      arch/arm64/kernel/probes/built-in.a
  AR      arch/arm64/kernel/built-in.a
  CC      mm/dmapool.o
  CC      mm/sparse.o
  CC      mm/sparse-vmemmap.o
  CC      mm/slub.o
  CC      mm/migrate.o
  CC      mm/page_counter.o
  CC      mm/memcontrol.o
  CC      mm/swap_cgroup.o
  CC      mm/page_isolation.o
  CC      mm/zsmalloc.o
  CC      mm/early_ioremap.o
  CC      mm/cma.o
  CC      mm/userfaultfd.o
  CC      mm/frame_vector.o
  CC      mm/usercopy.o
  AR      mm/built-in.a
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   300 / 774 (38.76%)
  Hits:            154 / 300 (51.33%)
    Direct:        124 / 154 (80.52%)
    Preprocessed:   30 / 154 (19.48%)
  Misses:          146 / 300 (48.67%)
Uncacheable calls: 474 / 774 (61.24%)
Local storage:
  Cache size (GB): 0.7 / 8.0 ( 9.34%)
  Hits:            154 / 300 (51.33%)
  Misses:          146 / 300 (48.67%)
 Kernel build failed (make exit code 2)
```
