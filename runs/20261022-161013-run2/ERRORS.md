# Sixfaced CI - FAILED run #2

- run URL : https://github.com/rkmandilwar-rgb/Kernel/actions/runs/39810900051
- ref     : refs/heads/arena/0fc62de9-kernel
- commit  : d330c18f618cc7f846bdf1827b3d5ff321386529
- date    : 2026-10-22 16:10:13 UTC
- clang   : Ubuntu clang version 21.1.8 (6ubuntu1)

## Extracted errors (last 300 matches)
```
218:../kernel/fork.c:2260:2: error: must use 'struct' tag to refer to type 'pid'
222:../kernel/fork.c:2260:6: error: expected identifier or '('
225:../kernel/fork.c:2261:15: error: use of undeclared identifier 'pid'
228:../kernel/fork.c:2270:21: error: implicitly declaring library function 'vfork' with type 'int (void)' [-Werror,-Wimplicit-function-declaration]
232:../kernel/fork.c:2270:18: error: incompatible pointer types assigning to 'struct completion *' from 'int (*)(void)' [-Werror,-Wincompatible-pointer-types]
235:../kernel/fork.c:2271:20: error: incompatible pointer types passing 'int (*)(void)' to parameter of type 'struct completion *' [-Werror,-Wincompatible-pointer-types]
244:../kernel/fork.c:2279:28: error: use of undeclared identifier 'pid'
247:../kernel/fork.c:2282:32: error: incompatible pointer types passing 'int (*)(void)' to parameter of type 'struct completion *' [-Werror,-Wincompatible-pointer-types]
253:../kernel/fork.c:2283:47: error: use of undeclared identifier 'pid'
256:../kernel/fork.c:2286:11: error: use of undeclared identifier 'pid'
259:../kernel/fork.c:2287:4: error: expected identifier or '('
262:../kernel/fork.c:2291:2: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
266:../kernel/fork.c:2291:19: error: a parameter list without types is only allowed in a function definition
269:../kernel/fork.c:2294:2: error: expected identifier or '('
272:../kernel/fork.c:2297:2: error: expected identifier or '('
275:../kernel/fork.c:2302:2: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
279:../kernel/fork.c:2302:10: error: a parameter list without types is only allowed in a function definition
282:../kernel/fork.c:2303:2: error: expected identifier or '('
285:../kernel/fork.c:2304:1: error: extraneous closing brace ('}')
290:make[2]: *** [../scripts/Makefile.build:364: kernel/fork.o] Error 1
291:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: kernel] Error 2
392:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  SHIPPED scripts/dtc/dtc-parser.tab.c
  HOSTCC  scripts/pnmtologo
  HOSTCC  scripts/dtc/dtc-lexer.lex.o
  HOSTCC  scripts/conmakehash
  HOSTCC  scripts/dtc/dtc-parser.tab.o
  HOSTCC  scripts/sortextable
  VDSOSYM include/generated/vdso-offsets.h
  HOSTCC  scripts/asn1_compiler
  HOSTCC  scripts/extract-cert
  HOSTLD  scripts/dtc/dtc
  CC      init/main.o
  CHK     include/generated/compile.h
  CC      init/do_mounts.o
  UPD     include/generated/compile.h
  CC      init/do_mounts_rd.o
  HOSTCC  usr/gen_init_cpio
  GEN     usr/initramfs_data.cpio
  AS      usr/initramfs_data.o
In file included from ../usr/initramfs_data.S:26:
../include/asm-generic/vmlinux.lds.h:78:9: warning: 'SDATA_MAIN' macro redefined [-Wmacro-redefined]
   78 | #define SDATA_MAIN .sdata .sdata.[0-9a-zA-Z_]* .sdata..compoundliteral* .sdata..L* .sdata..L_MergedGlobals*
      |         ^
../include/asm-generic/vmlinux.lds.h:74:9: note: previous definition is here
   74 | #define SDATA_MAIN .sdata .sdata.[0-9a-zA-Z_]*
      |         ^
../include/asm-generic/vmlinux.lds.h:79:9: warning: 'RODATA_MAIN' macro redefined [-Wmacro-redefined]
   79 | #define RODATA_MAIN .rodata .rodata.[0-9a-zA-Z_]* .rodata..compoundliteral* .rodata..L* .rodata..L_MergedGlobals*
      |         ^
../include/asm-generic/vmlinux.lds.h:75:9: note: previous definition is here
   75 | #define RODATA_MAIN .rodata .rodata.[0-9a-zA-Z_]*
      |         ^
../include/asm-generic/vmlinux.lds.h:81:9: warning: 'SBSS_MAIN' macro redefined [-Wmacro-redefined]
   81 | #define SBSS_MAIN .sbss .sbss.[0-9a-zA-Z_]* .sbss..compoundliteral* .sbss..L* .sbss..L_MergedGlobals*
      |         ^
../include/asm-generic/vmlinux.lds.h:76:9: note: previous definition is here
   76 | #define SBSS_MAIN .sbss .sbss.[0-9a-zA-Z_]*
      |         ^
3 warnings generated.
  AR      usr/built-in.a
  CC      init/do_mounts_initrd.o
  CC      init/do_mounts_dm.o
  CC      arch/arm64/kernel/debug-monitors.o
  AS      arch/arm64/kernel/entry.o
  CC      arch/arm64/mm/dma-mapping.o
  CC      init/noinitramfs.o
  CC      arch/arm64/kernel/irq.o
  CC      arch/arm64/mm/extable.o
  CC      arch/arm64/kernel/fpsimd.o
  CC      arch/arm64/net/bpf_jit_comp.o
  CC      init/initramfs.o
  AS      arch/arm64/kernel/entry-fpsimd.o
  CC      init/calibrate.o
  CC      arch/arm64/mm/fault.o
  CC      arch/arm64/kernel/process.o
  AR      arch/arm64/net/built-in.a
  CC      init/init_task.o
  CC      arch/arm64/crypto/sha1-ce-glue.o
  CC      arch/arm64/mm/init.o
  AS      arch/arm64/crypto/sha1-ce-core.o
  CC      init/version.o
  CC      arch/arm64/crypto/sha2-ce-glue.o
  CC      arch/arm64/kernel/ptrace.o
  AR      init/mounts.o
  AR      init/built-in.a
  AS      arch/arm64/mm/cache.o
  AS      arch/arm64/crypto/sha2-ce-core.o
  CC      arch/arm64/mm/copypage.o
  CC      arch/arm64/crypto/ghash-ce-glue.o
  CC      kernel/fork.o
  CC      arch/arm64/kernel/setup.o
  CC      arch/arm64/mm/flush.o
  AS      arch/arm64/crypto/ghash-ce-core.o
  AS      arch/arm64/crypto/aes-ce-core.o
  CC      arch/arm64/crypto/aes-ce-glue.o
../kernel/fork.c:2260:2: error: must use 'struct' tag to refer to type 'pid'
 2260 |         pid = get_task_pid(p, PIDTYPE_PID);
      |         ^
      |         struct 
../kernel/fork.c:2260:6: error: expected identifier or '('
 2260 |         pid = get_task_pid(p, PIDTYPE_PID);
      |             ^
../kernel/fork.c:2261:15: error: use of undeclared identifier 'pid'
 2261 |         nr = pid_vnr(pid);
      |                      ^~~
../kernel/fork.c:2270:21: error: implicitly declaring library function 'vfork' with type 'int (void)' [-Werror,-Wimplicit-function-declaration]
 2270 |                         p->vfork_done = &vfork;
      |                                          ^
../kernel/fork.c:2270:21: note: include the header <unistd.h> or explicitly provide a declaration for 'vfork'
../kernel/fork.c:2270:18: error: incompatible pointer types assigning to 'struct completion *' from 'int (*)(void)' [-Werror,-Wincompatible-pointer-types]
 2270 |                         p->vfork_done = &vfork;
      |                                       ^ ~~~~~~
../kernel/fork.c:2271:20: error: incompatible pointer types passing 'int (*)(void)' to parameter of type 'struct completion *' [-Werror,-Wincompatible-pointer-types]
 2271 |                         init_completion(&vfork);
      |                                         ^~~~~~
../include/linux/completion.h:62:46: note: expanded from macro 'init_completion'
   62 | #define init_completion(x) __init_completion(x)
      |                                              ^
../include/linux/completion.h:117:57: note: passing argument to parameter 'x' here
  117 | static inline void __init_completion(struct completion *x)
      |                                                         ^
../kernel/fork.c:2279:28: error: use of undeclared identifier 'pid'
 2279 |                         ptrace_event_pid(trace, pid);
      |                                                 ^~~
../kernel/fork.c:2282:32: error: incompatible pointer types passing 'int (*)(void)' to parameter of type 'struct completion *' [-Werror,-Wincompatible-pointer-types]
 2282 |                         if (!wait_for_vfork_done(p, &vfork))
      |                                                     ^~~~~~
../kernel/fork.c:1129:24: note: passing argument to parameter 'vfork' here
 1129 |                                 struct completion *vfork)
      |                                                    ^
../kernel/fork.c:2283:47: error: use of undeclared identifier 'pid'
 2283 |                                 ptrace_event_pid(PTRACE_EVENT_VFORK_DONE, pid);
      |                                                                           ^~~
../kernel/fork.c:2286:11: error: use of undeclared identifier 'pid'
 2286 |                 put_pid(pid);
      |                         ^~~
../kernel/fork.c:2287:4: error: expected identifier or '('
 2287 |         } else {
      |           ^
../kernel/fork.c:2291:2: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
 2291 |         wake_up_new_task(p);
      |         ^
      |         int
../kernel/fork.c:2291:19: error: a parameter list without types is only allowed in a function definition
 2291 |         wake_up_new_task(p);
      |                          ^
../kernel/fork.c:2294:2: error: expected identifier or '('
 2294 |         if (unlikely(trace))
      |         ^
../kernel/fork.c:2297:2: error: expected identifier or '('
 2297 |         if (clone_flags & CLONE_VFORK) {
      |         ^
../kernel/fork.c:2302:2: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
 2302 |         put_pid(pid);
      |         ^
      |         int
../kernel/fork.c:2302:10: error: a parameter list without types is only allowed in a function definition
 2302 |         put_pid(pid);
      |                 ^
../kernel/fork.c:2303:2: error: expected identifier or '('
 2303 |         return nr;
      |         ^
../kernel/fork.c:2304:1: error: extraneous closing brace ('}')
 2304 | }
      | ^
19 errors generated.
  CC      arch/arm64/mm/ioremap.o
make[2]: *** [../scripts/Makefile.build:364: kernel/fork.o] Error 1
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: kernel] Error 2
make[1]: *** Waiting for unfinished jobs....
  CC      arch/arm64/crypto/aes-ce-ccm-glue.o
  CC      arch/arm64/kernel/signal.o
  CC      arch/arm64/mm/mmap.o
  AS      arch/arm64/crypto/aes-ce-ccm-core.o
  CC      arch/arm64/kernel/sys.o
  CC      arch/arm64/mm/pgd.o
  CC      arch/arm64/crypto/aes-glue-ce.o
  AS      arch/arm64/crypto/aes-ce.o
  CC      arch/arm64/mm/mmu.o
  CC      arch/arm64/kernel/stacktrace.o
  CC      arch/arm64/crypto/aes-glue-neon.o
  CC      arch/arm64/mm/context.o
  CC      arch/arm64/kernel/time.o
  AS      arch/arm64/crypto/aes-neon.o
  AS      arch/arm64/mm/proc.o
  CC      arch/arm64/mm/pageattr.o
  CC      arch/arm64/crypto/sha256-glue.o
  CC      arch/arm64/kernel/traps.o
  SHIPPED arch/arm64/crypto/sha256-core.S
  CC      arch/arm64/kernel/io.o
  AR      arch/arm64/mm/built-in.a
  AS      arch/arm64/crypto/aes-cipher-core.o
  CC      arch/arm64/kernel/vdso.o
  CC      arch/arm64/crypto/aes-cipher-glue.o
  AS      arch/arm64/kernel/hyp-stub.o
  CC      arch/arm64/kernel/psci.o
  CC      arch/arm64/kernel/cpu_ops.o
  AR      arch/arm64/crypto/sha1-ce.o
  CC      arch/arm64/kernel/insn.o
  AR      arch/arm64/crypto/sha2-ce.o
  CC      arch/arm64/kernel/return_address.o
  AR      arch/arm64/crypto/ghash-ce.o
  AR      arch/arm64/crypto/aes-ce-cipher.o
  AR      arch/arm64/crypto/aes-ce-ccm.o
  AR      arch/arm64/crypto/aes-ce-blk.o
  AR      arch/arm64/crypto/aes-neon-blk.o
  AS      arch/arm64/crypto/sha256-core.o
  AR      arch/arm64/crypto/aes-arm64.o
  CC      arch/arm64/kernel/cpuinfo.o
  AR      arch/arm64/crypto/sha256-arm64.o
  AR      arch/arm64/crypto/built-in.a
  CC      arch/arm64/kernel/cpu_errata.o
  CC      arch/arm64/kernel/cpufeature.o
  CC      arch/arm64/kernel/alternative.o
  CC      arch/arm64/kernel/cacheinfo.o
  CC      arch/arm64/kernel/smp.o
  CC      arch/arm64/kernel/smp_spin_table.o
  CC      arch/arm64/kernel/topology.o
  AS      arch/arm64/kernel/smccc-call.o
  CC      arch/arm64/kernel/sys32.o
  AS      arch/arm64/kernel/kuser32.o
  CC      arch/arm64/kernel/signal32.o
  CC      arch/arm64/kernel/sys_compat.o
  AS      arch/arm64/kernel/entry32.o
  CC      arch/arm64/kernel/arm64ksyms.o
  CC      arch/arm64/kernel/module.o
  CC      arch/arm64/kernel/module-plts.o
  CC      arch/arm64/kernel/perf_regs.o
  CC      arch/arm64/kernel/perf_callchain.o
  CC      arch/arm64/kernel/perf_event.o
  CC      arch/arm64/kernel/hw_breakpoint.o
  AS      arch/arm64/kernel/sleep.o
  CC      arch/arm64/kernel/suspend.o
  CC      arch/arm64/kernel/cpuidle.o
  CC      arch/arm64/kernel/jump_label.o
  CC      arch/arm64/kernel/armv8_deprecated.o
  CC      arch/arm64/kernel/kaslr.o
  CC      arch/arm64/kernel/ssbd.o
  CC      arch/arm64/kernel/scs.o
  CC      arch/arm64/kernel/probes/uprobes.o
  OBJCOPY arch/arm64/kernel/vdso/vdso.so
  AS      arch/arm64/kernel/head.o
  AS      arch/arm64/kernel/vdso/vdso.o
  LDS     arch/arm64/kernel/vmlinux.lds
In file included from ../arch/arm64/kernel/vmlinux.lds.S:8:
../include/asm-generic/vmlinux.lds.h:78:9: warning: 'SDATA_MAIN' macro redefined [-Wmacro-redefined]
   78 | #define SDATA_MAIN .sdata .sdata.[0-9a-zA-Z_]* .sdata..compoundliteral* .sdata..L* .sdata..L_MergedGlobals*
      |         ^
../include/asm-generic/vmlinux.lds.h:74:9: note: previous definition is here
   74 | #define SDATA_MAIN .sdata .sdata.[0-9a-zA-Z_]*
      |         ^
../include/asm-generic/vmlinux.lds.h:79:9: warning: 'RODATA_MAIN' macro redefined [-Wmacro-redefined]
   79 | #define RODATA_MAIN .rodata .rodata.[0-9a-zA-Z_]* .rodata..compoundliteral* .rodata..L* .rodata..L_MergedGlobals*
      |         ^
../include/asm-generic/vmlinux.lds.h:75:9: note: previous definition is here
   75 | #define RODATA_MAIN .rodata .rodata.[0-9a-zA-Z_]*
      |         ^
../include/asm-generic/vmlinux.lds.h:81:9: warning: 'SBSS_MAIN' macro redefined [-Wmacro-redefined]
   81 | #define SBSS_MAIN .sbss .sbss.[0-9a-zA-Z_]* .sbss..compoundliteral* .sbss..L* .sbss..L_MergedGlobals*
      |         ^
../include/asm-generic/vmlinux.lds.h:76:9: note: previous definition is here
   76 | #define SBSS_MAIN .sbss .sbss.[0-9a-zA-Z_]*
      |         ^
3 warnings generated.
  CC      arch/arm64/kernel/probes/decode-insn.o
  CC      arch/arm64/kernel/probes/simulate-insn.o
  AR      arch/arm64/kernel/vdso/built-in.a
  AR      arch/arm64/kernel/probes/built-in.a
  AR      arch/arm64/kernel/built-in.a
make: *** [Makefile:151: sub-make] Error 2
 Kernel build failed (make exit code 2)
```
