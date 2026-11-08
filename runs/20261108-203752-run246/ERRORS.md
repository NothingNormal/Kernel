# Sixfaced CI - FAILED run #246

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42520919915
- ref     : refs/heads/OriginSU
- commit  : 9f71c2f5f96aa57404ef500b61dfc259f39561e0
- date    : 2026-11-08 20:37:52 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
5:error: patch failed: kernel/Kbuild:57
6:error: kernel/Kbuild: patch does not apply
7:error: patch failed: kernel/feature/selinux_hide.c:172
8:error: kernel/feature/selinux_hide.c: patch does not apply
9:error: patch failed: kernel/include/klog.h:11
10:error: kernel/include/klog.h: patch does not apply
11:error: patch failed: kernel/runtime/ksud_integration.c:43
12:error: kernel/runtime/ksud_integration.c: patch does not apply
13:error: patch failed: kernel/kpm/super_access.c:148
14:error: kernel/kpm/super_access.c: patch does not apply
15:error: patch failed: kernel/policy/su_request.c:8
16:error: kernel/policy/su_request.c: patch does not apply
```

## Tail of build log (last 250 lines)
```
Using Neutron Clang toolchain: /home/runner/neutron-clang/bin
Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)
Target: x86_64-unknown-linux-gnu
ccache enabled: ccache version 4.9.1
error: patch failed: kernel/Kbuild:57
error: kernel/Kbuild: patch does not apply
error: patch failed: kernel/feature/selinux_hide.c:172
error: kernel/feature/selinux_hide.c: patch does not apply
error: patch failed: kernel/include/klog.h:11
error: kernel/include/klog.h: patch does not apply
error: patch failed: kernel/runtime/ksud_integration.c:43
error: kernel/runtime/ksud_integration.c: patch does not apply
error: patch failed: kernel/kpm/super_access.c:148
error: kernel/kpm/super_access.c: patch does not apply
error: patch failed: kernel/policy/su_request.c:8
error: kernel/policy/su_request.c: patch does not apply
```
