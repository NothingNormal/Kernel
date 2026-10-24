## Sixfaced KSUN — miatoll (4.14)

Kernel image for the Xiaomi **miatoll** family — Redmi Note 9S, Redmi Note 9 Pro,
Redmi Note 9 Pro Max, POCO M2 Pro.

Flash the AnyKernel3 zip from recovery or from the KernelSU-Next manager.

### Kernel

- Linux 4.14.357 (SM6250)
- Built with Neutron Clang, Polly enabled, LTO, `-O3`
- Built into the kernel — no loadable module, works on locked-bootloader setups

### Root

- KernelSU-Next **v3.4.2** (legacy branch)
- Built-in (GKI2) integration, recognised by the official manager
- Manager ABI 5 — pairs with the released v3.4.2 manager
- Manual hook mode (no kprobes, no tracepoints)

### SUSFS v2.3.0 (NON-GKI)

- `SUS_PATH`
- `SUS_MOUNT`
- `SUS_KSTAT`
- `SPOOF_UNAME`
- `ENABLE_LOG`
- `HIDE_KSU_SUSFS_SYMBOLS`
- `SPOOF_CMDLINE_OR_BOOTCONFIG`
- `OPEN_REDIRECT`
- `SUS_MAP`

### NoMount

- VFS path redirection subsystem integrated in-kernel
- Injected files are served without creating any mount, so nothing appears in
  `/proc/mounts` and read-only partitions are never touched
- Whiteouts (hiding existing files) and per-UID isolation
- Works with the NoMount metamodule, driven from its WebUI

### Filesystem support

- OverlayFS, FUSE, tmpfs ACL and xattr — module mounting works natively and
  needs no additional metamodule for that step
- SELinux enforcing
- Namespaces, UTS namespace

### Notes

- Kernel version string: `v3.4.2-legacy`
- SUSFS is reported as `Supported | v2.3.0 (NON-GKI)` in the manager
- Keep a backup of your current boot partition before flashing
