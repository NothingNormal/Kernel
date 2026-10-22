# Sixfaced CI - FAILED run #1

- run URL : https://github.com/rkmandilwar-rgb/Kernel/actions/runs/39809880860
- ref     : refs/heads/arena/0fc62de9-kernel
- commit  : 4fb1436c75748743eef64ed3eba77480becbcb76
- date    : 2026-10-22 16:00:44 UTC
- clang   : Ubuntu clang version 21.1.8 (6ubuntu1)

## Extracted errors (last 300 matches)
```
57:make[3]: *** No rule to make target '../techpack/data/Makefile'.  Stop.
58:make[2]: *** [../scripts/Makefile.modbuiltin:55: techpack/data] Error 2
67:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1543: techpack/modules.builtin] Error 2
69:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
Using LLVM toolchain at: /usr/bin
Ubuntu clang version 21.1.8 (6ubuntu1)
Target: x86_64-pc-linux-gnu
Cloning into 'gcc64'...
Cloning into 'gcc32'...
  HOSTCC  scripts/basic/fixdep
  GEN     ./Makefile
  HOSTCC  scripts/kconfig/conf.o
  SHIPPED scripts/kconfig/zconf.tab.c
  SHIPPED scripts/kconfig/zconf.lex.c
  HOSTCC  scripts/kconfig/zconf.tab.o
  HOSTLD  scripts/kconfig/conf
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:165:warning: override: reassigning to symbol TCP_CONG_CUBIC
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:166:warning: override: reassigning to symbol TCP_CONG_BBR
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:759:warning: override: reassigning to symbol UTS_NS
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:761:warning: override: reassigning to symbol PSTORE
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:762:warning: override: reassigning to symbol PSTORE_RAM
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:763:warning: override: reassigning to symbol PSTORE_CONSOLE
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:764:warning: override: reassigning to symbol PSTORE_PMSG
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:767:warning: override: reassigning to symbol PSTORE
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:768:warning: override: reassigning to symbol PSTORE_RAM
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:769:warning: override: reassigning to symbol PSTORE_CONSOLE
arch/arm64/configs/vendor/xiaomi/miatoll_defconfig:770:warning: override: reassigning to symbol PSTORE_PMSG
#
# configuration written to .config
#
  GEN     ./Makefile
scripts/kconfig/conf  --silentoldconfig Kconfig
  CHK     include/config/kernel.release
  UPD     include/config/kernel.release
  GEN     ./Makefile
  WRAP    arch/arm64/include/generated/uapi/asm/errno.h
  WRAP    arch/arm64/include/generated/uapi/asm/ioctl.h
  WRAP    arch/arm64/include/generated/uapi/asm/ipcbuf.h
  WRAP    arch/arm64/include/generated/uapi/asm/ioctls.h
  WRAP    arch/arm64/include/generated/uapi/asm/kvm_para.h
  WRAP    arch/arm64/include/generated/uapi/asm/mman.h
  WRAP    arch/arm64/include/generated/uapi/asm/poll.h
  WRAP    arch/arm64/include/generated/uapi/asm/msgbuf.h
  WRAP    arch/arm64/include/generated/uapi/asm/sembuf.h
  WRAP    arch/arm64/include/generated/uapi/asm/resource.h
  WRAP    arch/arm64/include/generated/uapi/asm/shmbuf.h
  WRAP    arch/arm64/include/generated/uapi/asm/socket.h
  WRAP    arch/arm64/include/generated/uapi/asm/sockios.h
  WRAP    arch/arm64/include/generated/uapi/asm/swab.h
  WRAP    arch/arm64/include/generated/uapi/asm/termbits.h
  WRAP    arch/arm64/include/generated/uapi/asm/termios.h
  WRAP    arch/arm64/include/generated/uapi/asm/types.h
  CHK     include/generated/uapi/linux/version.h
  UPD     include/generated/uapi/linux/version.h
  CHK     include/generated/utsrelease.h
  UPD     include/generated/utsrelease.h
  HOSTCC  scripts/basic/fixdep
  HOSTCC  scripts/basic/bin2c
../net/Makefile:24: CC cannot link executables. Skipping bpfilter.
../scripts/Makefile.modbuiltin:26: ../techpack/data/Makefile: No such file or directory
make[3]: *** No rule to make target '../techpack/data/Makefile'.  Stop.
make[2]: *** [../scripts/Makefile.modbuiltin:55: techpack/data] Error 2
make[2]: *** Waiting for unfinished jobs....
-- KDIR: 
-- MDIR: /home/runner/work/Kernel/Kernel/KernelSU/kernel
-- KernelSU Manager package name: me.weishu.kernelsu
-- KernelSU Manager signature size: 0x033b
-- KernelSU Manager signature hash: c371061b19d8c7d7d6133c6a9bafe198fa944e50c1b31c9d8daa8d7f1fc2d2d6
-- KernelSU/compat: iterate_dir found!
-- KernelSU/compat: f_op->read_iter found!
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1543: techpack/modules.builtin] Error 2
make[1]: *** Waiting for unfinished jobs....
make: *** [Makefile:151: sub-make] Error 2
 Kernel build failed (make exit code 2)
```
