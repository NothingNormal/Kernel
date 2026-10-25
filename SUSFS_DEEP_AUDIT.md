# SUSFS deep audit — v2.3.0, 4.14.357, NON-GKI

Scope: every SUSFS command in the ABI, traced config → dispatch → handler →
state → **every reader** → whether that reader runs for the process that needs
to be affected. Plus flag-bit hygiene, reference parity against the approved
v2.3.0 trees, and end-to-end verification of the app-spawn marking path.

Tree: `f5d2edc81`. ABI source: `include/linux/susfs_def.h`. Handlers:
`fs/susfs.c`. Dispatch: `drivers/kernelsu/supercall/supercall.c`.

---

## Headline: one earlier finding was WRONG

**`ENABLE_AVC_LOG_SPOOFING` is implemented and works.** My previous audit
reported it as routed-but-inert. That was a grep error: I searched for the
handler name `susfs_avc_log_spoofing` and concluded it had no consumer, having
missed that the consumer reads a differently-named flag.

```
security/selinux/avc.c:50   bool susfs_is_avc_log_spoofing_enabled = false;
security/selinux/avc.c:197  if (unlikely(tsid == susfs_ksu_sid &&
                              susfs_is_avc_log_spoofing_enabled)) {
security/selinux/avc.c:199      audit_log_format(ab, " tsid=%d", susfs_priv_app_sid);
```

So an AVC denial whose target SID is the KernelSU domain is rewritten in the
audit log to report the priv-app SID. Both SIDs come from `cache_sid()`, which
runs at `/system/bin/init second_stage`. Fully functional; not advertised
because it has no `CONFIG_KSU_SUSFS_*` symbol gates — it is always compiled.

## The app-spawn marking path — verified end to end

This is what SUS_PATH, SUS_MAP and the loop all depend on. Every link checked:

| Link | Evidence |
|---|---|
| `susfs_zygote_sid` is resolved before zygote spawns apps | `cache_sid()` runs on `/system/bin/init second_stage` (`ksud_integration.c:314-320` → task_work → `cache_sid()` at :274). This is init's second stage, before zygote is exec'd. **Closes the residual risk flagged earlier.** |
| app spawn reaches the hook | zygote child calls `setresuid` → `ksu_handle_setresuid` (manual hook, `fs/exec.c` + syscall path) |
| the guard is satisfiable | `is_zygote(current_cred())` — true at setresuid (cred is still zygote's); `is_appuid(new_uid)` — 10000..19999 per user range; `ksu_uid_should_umount(new_uid)` — with no app profile it returns `default_non_root_profile.umount_modules`, set to **true** at `allowlist.c:60` ("we will umount modules by default!") |
| flag is set on the right task | `susfs_set_current_proc_umounted()` → TIF bit 33 on `current` — the child doing setresuid |
| consumers see it | `susfs_is_current_proc_umounted_app()` = TIF set **and** `current_uid() >= 10000` — true right after setresuid to an app uid |
| `susfs_init()` armed the workqueue | called unconditionally under `CONFIG_KSU_SUSFS` at end of `kernelsu_init()` (`core/init.c:207`), so `INIT_WORK(&susfs_extra_works, …)` did run — a `schedule_work()` on it is safe |

Before `f5d2edc81` the marking was only done inside `ksu_handle_umount()`, which
returns early on `!ksu_module_mounted`, `!ksu_kernel_umount_enabled`, uid not on
the umount list, or non-zygote caller — only the last being a real precondition.
Any of the others left apps unmarked, and every hiding feature silently no-oped.

## The nine enabled features

| # | Feature | Setter | Readers | Verdict |
|---|---|---|---|---|
| 1 | `SUS_PATH` | `susfs_add_sus_path` → `i_state` bit 33 | `fs/namei.c` ×8 (`lookup_dcache`, `__lookup_hash`, `lookup_fast` ×2, `lookup_slow`, `link_path_walk`, `lookup_open`, `may_delete`); `fs/readdir.c` ×5 — **all five** filldir variants | working |
| 2 | loop variant | `susfs_add_sus_path_loop` → `LH_SUS_PATH_LOOP` | `susfs_run_sus_path_loop()` via `susfs_extra_works` | **was dead**, fixed `ae0297c13` + `f5d2edc81` |
| 3 | `SUS_MOUNT` | static key via `susfs_set_hide_sus_mnts_for_non_su_procs` | `fs/proc_namespace.c:471` + `susfs_show_*`; `fs/namespace.c:879` `__lookup_mnt` skips sus mounts for umounted procs; private id ranges at :162/:323 | working |
| 4 | `SUS_KSTAT` | `susfs_add_sus_kstat` → `SUS_KSTAT_HLIST` + `i_state` bit 35 | `fs/stat.c` (tag → spoof after `->getattr` → strip marks), `fs/statfs.c`, `fs/proc/fd.c`, `fs/notify/fdinfo.c` | working (incl. `0x55572`, fixed `ae0297c13`) |
| 5 | `SPOOF_UNAME` | `susfs_set_uname` | `kernel/sys.c` `SYSCALL_DEFINE1(newuname)` before `copy_to_user` | working |
| 6 | `ENABLE_LOG` | `susfs_enable_log` | `SUSFS_LOG*` macros throughout `fs/susfs.c` | working |
| 7 | `HIDE_KSU_SUSFS_SYMBOLS` | always on | `kernel/kallsyms.c:621` — 14 prefixes | working (module branch unfiltered; correct, KSU is `=y` not `=m`) |
| 8 | `SPOOF_CMDLINE_OR_BOOTCONFIG` | `susfs_set_cmdline_or_bootconfig` | `fs/proc/cmdline.c:55` early-return | working (no `/proc/bootconfig` in 4.14, correctly absent) |
| 9 | `OPEN_REDIRECT` | `susfs_add_open_redirect` | `fs/namei.c` ×3, `fs/open.c` ×2, `fs/proc/base.c` ×2, `fs/proc/task_mmu.c` ×2 | working — **fully v2.3.0**: `uid_scheme` field present, validated to the 5-scheme range, stored per entry, and evaluated per lookup at `susfs.c:1084-1107`; stores `spoofed_mnt_id` + `spoofed_kstatfs` so stat after redirect stays consistent |
| 10 | `SUS_MAP` | `susfs_add_sus_map` → `i_state` bit 39 | `fs/proc/task_mmu.c`: `show_map_vma` drops VMA, `show_smap` returns early (covers smaps **and** smaps_rollup) | working |

(Both approved references advertise 9; the tenth row above is the loop variant
of SUS_PATH, which `show enabled_features` reports under `CONFIG_KSU_SUSFS_SUS_PATH`,
not as a separate line.)

## Strings userspace parses

| Reported | Value | Note |
|---|---|---|
| `show version` | `v2.3.0` | BRENE tests `== "v2"*` → matches |
| `show variant` | `NON-GKI` | matches manager display |
| `show enabled_features` | **9 lines** | 11 emit blocks, minus `TRY_UMOUNT` and `SUS_MEMFD` compiled out. BRENE hardcodes `/9`. |

## Flag-bit hygiene — no collisions

| Flag | Bit | 4.14 usage | Safe? |
|---|---|---|---|
| `TIF_PROC_UMOUNTED` | 33 | arm64 uses TIF 0–25; `_TIF_*` masks stop at 25 | yes |
| `AS_FLAGS_SUS_PATH` | 33 | `i_state` in use up to ~12 (`I_LINKABLE`); `I_OVL_INUSE` = 9 | yes |
| `AS_FLAGS_SUS_MOUNT` | 34 | **unused** — vestigial define; SUS_MOUNT uses `VFSMOUNT_MNT_FLAGS_KSU_UNSHARED_MNT` | harmless |
| `AS_FLAGS_SUS_KSTAT` | 35 | free | yes |
| `AS_FLAGS_SUS_MAP` | 39 | free | yes |

Every `set_bit`/`test_bit` pair confirmed to target the same field:
- `SUS_PATH` → `i_state` (both sides, plus the fuse_inode mirror)
- `SUS_KSTAT` → `i_state`
- `SUS_MAP` → `i_state` — note the violet reference stores this on
  `i_mapping->flags` instead; ours is internally consistent, which is what
  matters. Not a bug.

## Commands that are not features

**Deprecated, deliberately unrouted (8):** `SET_ANDROID_DATA_ROOT_PATH` 0x55551,
`SET_SDCARD_ROOT_PATH` 0x55552, `ADD_SUS_MOUNT` 0x55560,
`UMOUNT_FOR_ZYGOTE_ISO_SERVICE` 0x55562, `ADD_TRY_UMOUNT` 0x55580,
`SHOW_SUS_SU_WORKING_MODE` 0x555e4, `IS_SUS_SU_READY` 0x555f0, `SUS_SU` 0x60000.

**Info-only (3):** `SHOW_VERSION`, `SHOW_ENABLED_FEATURES`, `SHOW_VARIANT`.

**Compiled out (2):** `TRY_UMOUNT`, `SUS_MEMFD` — deliberate, handlers `#ifdef`-guarded.

**Was unrouted, now fixed:** `ADD_SUS_KSTAT_STATICALLY` 0x55572 → `susfs_add_sus_kstat`
(`ae0297c13`).

## Reference parity

File-level diff against `hestudio-community/android_kernel_xiaomi_violet`
(lineageos-23.2-ksunext, also 4.14 + v2.3.0), comparing every file containing
`susfs`:

- **In violet, not ours:** `fs/exec.c`. Content is
  `susfs_clear_current_proc_no_su()` on `/system/bin/su` — part of the
  **deprecated `SUS_SU`** feature (`TIF_PROC_NO_SU`), which the official v2.3.0
  legacy driver dropped. Correctly absent.
- **In ours, not violet:** the whole `drivers/kernelsu/*` glue set (expected —
  different driver vintage), plus `fs/open.c`, `include/linux/mount.h`,
  `include/linux/sched.h`, `mm/shmem.c`.

Systematic sweep for entry points with **no caller anywhere**: only
`susfs_variant` / `susfs_version` (strings) and `susfs_mnt_id_backup` (a struct
field name, not a function). No orphaned hooks.

`mm/memory.c` in our tree contains only a SUSFS include and no hook. The
canonical 4.14 patch does not touch `mm/memory.c` either, so this is dead
weight rather than a missing hook.

## Known non-defects, for the record

- `DEFINE_STATIC_KEY_FALSE(susfs_is_hide_sus_mnts_for_non_su_procs_enabled)` —
  mount hiding is **off** until userspace enables it, despite the code comment
  saying "by default". All three approved references use FALSE too; the comment
  is inherited and misleading, the behaviour is upstream-correct. BRENE sets it
  from its own toggle.
- `susfs_init()`'s comment in `core/init.c` claims it "seeds hide_sus_mnts and
  spawns the sdcard monitor". It does neither — the monitor is started from
  `supercall/dispatch.c:144` on `EVENT_BOOT_COMPLETED`. Functionality present,
  comment wrong.
- Module-symbol branch of `kallsyms.c` is unfiltered. Correct here since
  `CONFIG_KSU=y`.

## Residual caveats (honest)

1. **First-spawn race.** The loop work is queued on the workqueue and runs
   asynchronously. The very first app to spawn after boot could run detection
   before the walk completes. Detector apps normally launch much later, so the
   practical exposure is small, but it is not zero.
2. **Scope is app processes only.** `susfs_is_inode_sus_path()` requires
   `susfs_is_current_proc_umounted_app()` — TIF flag **and** uid ≥ 10000. Root,
   `shell` (2000) and `system_server` are never marked, so they still see every
   "hidden" path. By design.
3. **`/sdcard` caveat** is upstream's: a path under `/sdcard` is owned by the
   MediaProvider uid, so `is_i_uid_not_allowed()` can return false for it.
4. **Still static analysis.** Everything above is proven from source — setters
   have readers, readers are reachable, flags are consistent, ordering holds.
   That is stronger than "a reader exists", but it is still not a device test.
   Only on-device verification proves a specific detector is defeated.

---

# Addendum: file-by-file comparison against `Experiment-kernel` (reference)

Reference: `rkmandilwar-rgb/Experiment-kernel`, branch
`arena/de8f5ae9-experiment-kernel` — 4.14 NON-GKI, SUSFS v2.3.0, tiann-fork KSU,
reported as verified on hardware (curtana, Infinity X, Android 16).
Method: whole-tree extraction (193 MB tarball), then for every file that
mentions SUSFS in either tree: reference-count diff, then a hunk-level diff
filtered to hunks that contain SUSFS code, then a manual read of each.

Direction convention below: "reference" = their branch, "ours" = this branch.

## A. Confirmed defects found and fixed in this branch

Each of these was read in both trees and traced to a caller before being
changed. Commit ids are on `arena/cd1940a2-kernel`.

### A1. `__lookup_hash()` SUS_PATH test was dead code — `1c8ed7bc2`
`dentry` came straight from `d_alloc()`, so `d_inode` was always NULL and
`susfs_is_inode_sus_path()` was never reached. Every create/unlink/rename path
(`mknodat`, `mkdirat`, `rmdir`, `unlinkat`, `symlinkat`, `linkat`, `renameat2`)
reaches this with `LOOKUP_CREATE` and saw the hidden name as existing. Our
`goto retry` then handed a *negative* fake-qstr dentry to `lookup_real()`, so a
create could instantiate a real `..5.u.S` object on disk. Now: test after
`lookup_real()`, `-EACCES` for `LOOKUP_CREATE|LOOKUP_EXCL`, `-ENOENT`
otherwise, and the reserved name is rejected outright.

### A2. `lookup_open()` — same class, `open()` side — `1c8ed7bc2`
Two separate gaps, both present in ours and fixed in the reference:
- cached-dentry hit: the fake-qstr retry was still reachable for
  `O_CREAT`/`O_EXCL`, so `open(hidden, O_CREAT)` created `..5.u.S` on disk;
- cache miss: after the fs `->lookup()` instantiated the real hidden dentry
  nothing re-checked it, so `open()` of a hidden path succeeded.

### A3. `do_sys_open()` OPEN_REDIRECT ran the real file first — `1c8ed7bc2`
Ours opened the target, then closed it and reopened the replacement. An
`O_TRUNC|O_CREAT` request therefore **truncated the hidden file**, and a
FIFO/device target could block in `->open()` before the close. The reference
resolves with `filename_lookup()` first and swaps the name before opening
anything. Adopting that required `filename_lookup()` to be non-static (it is
dfd-relative; `kern_path()` is not), so it is now declared in
`include/linux/namei.h`.
Trade-off, stated plainly: one extra lookup for every `open()` that misses in
the dcache.

### A4. `susfs_open_redirect_spoof_vfs_readlink()` returned 0, not the length — `283589983`
`vfs_readlink()` returns this value straight to `readlink()`, so **every
redirected symlink reported an empty target**. The reference returns `len`
(and truncates to `buflen`, as `readlink()` does, instead of failing).

### A5. `show_map_vma()` spoof was not app-uid gated — `d44cf4645`
`susfs_show_map_vma_spoofer()` does not self-gate. Ours called it
unconditionally, so root/ksu readers got spoofed dev/ino in `/proc/pid/maps`
while `stat()` on the same inode (which *is* app-uid gated) returned the truth
— an internal inconsistency that is itself a signal.

### A6. `fanotify_fdinfo()` had no SUSFS code at all — `d44cf4645`
Raw `ino:`/`sdev:` and a raw `mnt->mnt_id`, i.e. exact identity of a hidden
file and of a KSU mount, readable by an app. Now spoofed like the inotify path.

### A7. `/proc/pid/numa_maps` ignored OPEN_REDIRECT — `d44cf4645`
It prints `file->f_path` directly, so a redirected file leaked its real path
even though `/proc/pid/maps` was spoofed.

### A8. `map_files` direct lookup bypassed SUS_MAP — `d44cf4645`
`/proc/<pid>/map_files/<start>-<end>` by address instantiated the hidden file;
only `readdir` was guarded.

### A9. `vfs_statfs()` overwrote the spoofed `f_flags` — `d44cf4645`
The SUS_KSTAT branch fell through to the shared exit that recomputes `f_flags`
from `path->mnt`, silently undoing the `susfs_calculate_f_flags()` capture added
in `a6a3cb925` and exposing the sus mount's own mount flags. The reference
returns the spoofed `kstatfs` untouched (upstream `769e31fbe`).

### A10. `susfs_update_sus_kstat()` leaked `new_entry` — `283589983`
`kzalloc()`ed, and when no matching entry exists the function returned
`-ENOENT` without freeing it. The reference frees it.

### A11. Userspace string fields were not validated — `283589983`
Reference has `susfs_path_is_valid()` (non-empty **and** NUL-terminated inside
the field) on every add/update entry point; ours only checked for an empty
first byte. An unterminated field makes `strlen()`/`strcmp()`/`kern_path()` walk
past the copied struct. Restored on `add_sus_path`, `add_sus_path_loop`,
`add/update_sus_kstat`, `add_sus_map`, `add_open_redirect` (both names),
`set_uname` (release + version), `set_cmdline_or_bootconfig`.
Reachable only by a KSU-authorized caller, so this is robustness, not a
privilege boundary.

### A12. `susfs_spoof_uname()` skipped the spoof while the mutex was held — `283589983`
`mutex_is_locked()` early-returned, so a concurrent `SET_UNAME` made one
`uname()` call return the real release/version, and `my_uname` was read
unlocked (torn string). The newuname syscall is the only caller, so blocking on
the mutex is safe.

### A13. `susfs_spoof_cmdline_or_bootconfig()` printed inside the seqlock — `283589983`
`seq_puts()` writes into the seq_file buffer and can `kmalloc(GFP_KERNEL)`
there; worse, a failed `read_seqretry()` repeats the body and appended the
string a second time. Snapshot first, print after.

### A14. `get_anon_bdev()` lost the KSU minor on an ida race — `dac5b2cf1`
`goto retry` jumped to the common `retry:` label, which allocates a normal
minor — the mount silently stops being a KSU mount. Reference retries with a
private label.

### A15. `clone_mnt()` used `CL_COPY_UNBINDABLE` as the unshare marker — `dac5b2cf1`
4.14 has no `CL_COPY_MNT_NS`, and `clone_private_mount()` passes
`CL_COPY_ALL`, which contains the unbindable bit. A private clone by a ksu
process was therefore treated as a namespace copy and kept a non-KSU `mnt_id`.
Ours now sets a private `CL_COPY_MNT_NS` bit only in `copy_mnt_ns()`, like the
reference.

### A16. `kallsyms` hidden-prefix list was short — `dac5b2cf1`
Reference adds `is_task_ksu_domain` (it exists at
`drivers/kernelsu/selinux/selinux.c:255`, global, `CONFIG_KSU=y`, so it is
listed by name). Went further than the reference and added the other KSU
globals that are built in and listed: `allowed_for_su`, `always_allow`,
`only_manager`, `only_root`, `manager_or_root`, `cache_sid`, `is_adbd`,
`is_init`, `setup_ksu_cred`, `setup_groups`, `setup_mount_ns`,
`search_manager`, `scan_call_to`, `get_pkg_from_apk_dir_path`,
`on_boot_completed`, `on_module_mounted`, `stop_execve_hook`,
`stop_init_rc_hook`, `nuke_ext4_sysfs`. Each was grepped tree-wide: no
non-KSU symbol shares these prefixes.

### A17. Smaller items — `283589983`
- `susfs_open_redirect_spoof_do_proc_readlink()`: `strncpy()` pads only when
  the source is shorter; the buffer is `strlen()`ed by the caller, so terminate
  it explicitly.
- `susfs_add_open_redirect()`: the mirror-entry match was name-only inside the
  redirected inode's bucket, so replacing a redirect could also free an
  unrelated forward entry. Now requires `reversed_lookup_only` + matching dev.
- `watch_one_dir()`: clear `kpath` after `path_put()`; the deferred cleanup
  puts it again when `.mnt` is non-NULL.

## B. Differences examined and deliberately **not** changed

- `fs/proc/base.c` `do_proc_readlink()`: reference passes `buflen` to the
  spoof helper, ours passes `PAGE_SIZE` and clamps the result to `buflen`.
  Ours is strictly safer for the callers that use it (the buffer is a page).
- `susfs_add_open_redirect()`: reference fails the whole command if the
  `vfs_statfs()` capture fails, ours ignores it. Ours keeps the redirect
  working with a zeroed spoofed kstatfs; neither is a leak.
- `susfs_sdcard_monitor_fn()`: on failure the reference forces
  `susfs_is_sdcard_android_data_decrypted = true`; ours leaves it. Ours keeps
  marking ksu mounts (more hiding), the reference stops marking them.
- `mm/memory.c` `GUP` loop: the reference hoists `find_vma()` out of the loop;
  ours keeps it per-iteration, which is correct as `addr` advances.
- `fs/proc/base.c` `proc_map_files_readdir()` counting loop: differs only in
  shape (`if (vm_file && ++pos > ctx->pos)`); same behaviour, same bug fixed.
- `fs/namei.c` fake-qstr comments, `fs/super.c` include placement,
  `fs/namespace.c` `ida_pre_get`/`ida_get_new_above` vs `ida_alloc()` (4.14 has
  no `ida_alloc()`, so ours is the compilable form): formatting or
  version-legitimate.
- `fs/proc/cmdline.c`: ours carries the base kernel's
  `INITRAMFS_IGNORE_SKIP_FLAG` / `CONFIG_PROC_CMDLINE_APPEND_ANDROID_FORCE_NORMAL_BOOT`
  copy; the reference branch has no such base code. Not SUSFS-related.

## C. Checked and found to be non-issues (with the reason)

- `copy_mnt_ns()` has zero SUSFS references in **both** trees, so upstream
  `273ae364` (clone_mnt unshare race) is not applicable to 4.14.
- `show_numa_map()` open-redirect spoofing in the reference is *addition*; ours
  had none, so this was a gap (fixed, A7), not a defect in ours.
- `show_mark_fhandle()` prints only `fhandle-bytes`, `fhandle-type` and
  `f_handle` — no dev/ino — and `CONFIG_EXPORTFS` is **not** set in
  `miatoll_defconfig`, so the whole call compiles to `{}`. The reference's
  `metadata_spoofed` guard is therefore belt-and-braces; ours now does the same.
- `susfs_get_non_sus_vfsmnt_from_vfsmnt()` cannot return NULL (it returns
  either `vfsmnt` or `&mnt->mnt` with both refs taken), so the reference's
  extra `if (!no_sus_vfsmnt)` guard is defensive only.
- `vfs_statfs()` callers (`user_statfs`, `fd_statfs`, cachefiles, nfsd,
  overlayfs, sdcardfs, acct) all pass a resolved `path`, so `path->mnt` cannot
  be NULL; the added guard is still free.
- `susfs_run_sus_path_loop()` being `static` in ours: no caller outside
  `fs/susfs.c` (only `susfs_run_extra_works()`), so this compiles and links.

## D. Status of the comparison

Files that mention SUSFS: 30 in ours, 25 in the reference (ours carries
supercall/setuid/SELinux-side code theirs does not; theirs has `ksu.c`). All 18
shared VFS/proc/kernel files were diffed hunk-by-hunk, plus `fs/susfs.c` (32
hunks, 28 with real code changes — the remainder of those are the feature-set
differences: ours compiles `TRY_UMOUNT` and `SUS_MEMFD` code that is disabled
in the defconfig, theirs does not have it at all).

Not yet compared: `drivers/kernelsu/**` in depth. The KSU side is a different
fork (ours = official legacy adapted, theirs = backslashxx tiann fork), so a
line diff there is not meaningful; only SUSFS-touching call sites matter, and
the known ones (`core/init.c`, `feature/kernel_umount.c`, `supercall/*`,
`selinux/*`) were already checked in the main audit.

## E. Cross-check against the upstream fix list

The reference's `fs/SUSFS_UPSTREAM.md` claims six upstream fixes are "already
present" in its tree: `b56b5ac0`, `6c2b5042`, `273ae364`, `ad8e498d`, `6d574ebc`,
`44a893a6`. Three of those were never verified for **our** tree, so I pulled
their messages from `ShirkNeko/susfs4ksu` via the GitHub API:

| commit | what it fixes | in our tree before this audit? |
|---|---|---|
| `ad8e498d` | "free new_entry when susfs_update_sus_kstat() finds no match" | **No** — fixed here (A10) |
| `6d574ebc` | "emit spoofed cmdline once outside seqlock retry" | **No** — we printed inside the window (A13) |
| `44a893a6` | "Fix NULL deref for susfs_get_enabled_features()" | Yes, byte-for-byte |

So the reference's "already present" list was **wrong for two of the three** for
this branch, and both gaps were found independently by diffing `fs/susfs.c`
before the upstream messages were consulted. That is the argument for doing the
file-by-file comparison instead of trusting the reference's changelog.

Also relevant from upstream history: `d4e35532` ("change to spoof kstat for all
process") was later reverted by `0cca4f58` ("add back uid check for sus_kstat"),
which is the same conclusion reached in A5 — the app-uid gate is the current
upstream position, not a local invention.

## F. Verification state

- Four commits on `arena/cd1940a2-kernel`: `1c8ed7bc2`, `283589983`,
  `d44cf4645`, `dac5b2cf1`.
- CI run `40121900509` — **success**, no warnings in any touched file.
- Release asset rebuilt from that commit:
  `Sixfaced-V5-KSUN-miatoll-20261024-18.zip` (21,618,979 B), tag `KSUN`.
- Still static analysis. Nothing here has been exercised on a device; the
  behavioural claims are proven by reading both trees and every caller, not by
  running them.

---

# Addendum 2: re-verified against upstream SUSFS v2.3.0 itself

The first addendum used the sibling branch as the yardstick. That was a
reasonable first pass, but it is not authority: a sibling can be wrong, and
"it differs from the reference" is not the same as "it is a bug". So this
pass compares every change against the **actual upstream source**.

## Where upstream v2.3.0 lives

Not where I first looked. `ShirkNeko/susfs4ksu` branch `kernel-4.14` is
**v1.5.9**, much older than the code we ship, and has no `open_redirect`,
`SUS_MEMFD` or the v2 hook set. The v2.3.0 tree is on
`ShirkNeko/susfs4ksu` branch **`gki-android14-6.1`** (also `-6.1-dev`,
`gki-android15-6.6`): `kernel_patches/fs/susfs.c`,
`kernel_patches/include/linux/susfs.h` (`SUSFS_VERSION "v2.3.0"`),
`kernel_patches/include/linux/susfs_def.h`, and
`kernel_patches/50_add_susfs_in_gki-android14-6.1.patch`.

It is a 6.1 GKI tree, so function names differ (`lookup_one_qstr_excl` /
`__lookup_slow` / `open_last_lookups` / `do_open` vs our 4.14 `__lookup_hash` /
`lookup_slow` / `do_last`), and `ida_alloc_range()` exists there but not in
4.14 — the *logic* is what gets compared, not the text.

## Provenance of every fix in this branch

| # | Fix | Upstream v2.3.0 says |
|---|---|---|
| A1 | SUS_PATH test after `lookup_real()`, error instead of fake dentry | upstream has the same dead pre-lookup test in `lookup_one_qstr_excl`; its own commit `6cbc44c6` ("Prevent dentry creation for fake_qstr") admits the fake-dentry problem and mitigates it only for FUSE + `LOOKUP_CREATE\|EXCL` + full-file-access. Our fix is the general form. **Beyond upstream, confirmed real** |
| A2 | `lookup_open()` cached-hit `O_CREAT` guard + post-`->lookup()` re-check | absent upstream; upstream's `skip_orig_flow` falls straight into `i_op->create()` on a negative dentry named `..5.u.S`. **Upstream bug** |
| A3 | `do_sys_open()` resolves before opening | upstream hooks `path_openat()` instead, which is better placed (covers every `filp_open`) but needs 6.1's split of `open_last_lookups` from `do_open`; 4.14's `do_last` opens inside the same call. Same semantics, one extra lookup per `open()`. **Same intent, 4.14-adapted** |
| A4 | readlink spoof returns the length | upstream returns `0` and its callers test `if (!res) return res` — so `readlink()` on a redirected inode reports an **empty** link. `vfs_readlink()`'s contract is the byte count. **Upstream bug** |
| A5 | app-uid gate on `show_map_vma` | **upstream does not gate it** — my gate was wrong and has been reverted; ungated spoofs strictly more readers, gating would hand root/system the real dev/ino |
| A6 | `fanotify_fdinfo()` spoofing | upstream has **no fanotify code at all** (inotify only, app-uid gated). Fanotify prints the same `ino:`/`sdev:`/`mnt_id` fields, so the leak is real. **Beyond upstream**; used the same spoofer as our inotify path so the two files cannot disagree |
| A7 | `numa_maps` open-redirect spoof | absent upstream. `show_numa_map()` prints `file->f_path` raw. **Beyond upstream** |
| A8 | `map_files` direct lookup hides SUS_MAP | absent upstream (only `proc_map_files_readdir` is hooked). Lookup by address bypasses readdir. **Beyond upstream** |
| A9 | `vfs_statfs` must not recompute spoofed `f_flags` | **identical to upstream**, including the `(dentry, mnt, buf, is_fuse)` signature and the early `return` |
| A10 | `kfree(new_entry)` in `update_sus_kstat` | **identical to upstream** |
| A11 | NUL-terminated-field validation | upstream only checks `*str == '\0'` (empty). Cannot crash anything real (KSU-authorized caller), so this is **beyond upstream**; harmless |
| A12 | uname spoof must not skip | upstream uses a **seqlock** and never skips. My first version used the mutex and blocked; replaced with upstream's seqlock shape (`strscpy`, read_seqbegin/retry). **Now identical in effect to upstream** |
| A13 | cmdline spoof prints outside the seqlock | **identical to upstream** (snapshot under seqlock, `seq_puts` after) |
| A14 | `get_anon_bdev` retries inside the KSU branch | upstream uses `ida_alloc_range()`; 4.14's `ida_get_new_above()` needs the retry, and the shared `retry:` label drops the KSU minor. **4.14-adapted, same intent** |
| A15 | private `CL_COPY_MNT_NS BIT(25)` | **identical to upstream**, including the bit number and the `copy_flags \|= CL_COPY_MNT_NS` in `copy_mnt_ns()` |
| A16 | extra kallsyms prefixes | upstream's list is exactly what we already had (no `is_task_ksu_domain`). Our KSU fork has different symbol names, so upstream's list cannot cover them. **Ours only** — justified by the symbols existing as globals, with a tree-wide collision scan |
| A17 | proc_readlink NUL, mirror-entry guards, kpath clear | mirror-guard differs from both upstream and the reference is stricter; only ever removes an entry that is genuinely the mirror of the redirect being replaced |

## What this changed in our tree (second pass)

- **Reverted A5** — `show_map_vma()` is ungated again, with a comment saying why.
- **A12 rewritten** to upstream's seqlock, replacing my mutex version.
- **A6 rewritten** to use `susfs_is_inode_sus_kstat()` +
  `susfs_sus_kstat_spoof_inotify_fdinfo()` (the inotify path's pair) instead of
  `susfs_show_map_vma_spoofer()`, so inotify and fanotify fdinfo agree on the
  same inode.

## The honest generalisation

Two of the reference branch's "fixes" were wrong or overreaching (the
`show_map_vma` gate, and gating first then claiming upstream consistency), and
three of the things I fixed are bugs that still exist in upstream v2.3.0 today
(A1/A2 fake-qstr creation, A4 readlink length, A6/A7/A8 unhooked readers). The
reference was right about the *shape* of the namei problem and wrong about the
maps gate. Neither source is authority; the VFS contract is.

## Second-pass follow-ups

### The `-EACCES` → `-ENOENT` change (own reasoning, not copied)

The first pass returned `-EACCES` for `LOOKUP_CREATE|LOOKUP_EXCL` and
`-ENOENT` otherwise, copying the reference branch. That is an **existence
oracle**: an app that runs `open("/data/local/tmp/some_hidden_thing", O_CREAT)`
in a world-writable directory and gets `EACCES` learns that *something is there
and the kernel is protecting it*. `ENOENT` is what a name that does not exist
answers, so it reveals nothing. Now every caller — create family included —
gets `-ENOENT`.

### Deliberately absent: upstream's getpriority/ioprio uid filter

Upstream v2.3.0 carries a second feature we have no equivalent of at all:
`st_susfs_priority_uid_filter` / `susfs_set_priority_uid_filter()` /
`susfs_is_priority_uid_filtered()`, hooked into `block/ioprio.c`'s
`ioprio_set`/`ioprio_get` so a filtered uid cannot observe or set another
process's ioprio. It is unconditional upstream (no CONFIG guard), so it never
shows up in upstream's enabled-features output either.

Not ported, on purpose:

1. It is a whole new supercall command + struct + two syscall hooks, not a fix.
2. It adds a line to `CMD_SUSFS_SHOW_ENABLED_FEATURES`, which is exactly the
   string the SUSFS module parses. Our contract is 9 feature lines; adding a
   tenth changes what userspace sees for a feature the module does not use.
3. `kernel/reboot.c` (also new upstream) *is* covered: `ksu_handle_sys_reboot`
   already exists at `kernel/reboot.c:281/298` in our tree.

If it is ever wanted, it is a feature decision, not a bug fix.

---

# Addendum 3: upstream GitLab HEAD (simonpunk/susfs4ksu, gki-android12-5.10)

Checked against the canonical repo on GitLab, branch `gki-android12-5.10`
(`SUSFS_VERSION "v2.3.0"` — same version we ship). Two commits, both ~1 day old:

## `5419f4ee` — "kernel & KernelSU: Sync with upstream"

- **kernel:** in `sel_open_handle_status()`, drop the
  `mutex_lock(&selinux_state.status_lock)` around reading `fake_status` and
  read the pointer directly.
- **KernelSU:** revert its own `selinux_hide.c` edits to match tiann
  `528bafc5`, i.e. keep the hook only in the kernel patch.

**Not present here, and does not need to be.** We never edit `selinuxfs.c`:
`security/selinux/selinuxfs.c:220 sel_open_handle_status()` is stock in our
tree, and the hiding is a runtime fops-slot patch —
`drivers/kernelsu/feature/selinux_hide.c:473 ksu_hook_status_open()` swaps
`fops->open` for `ksu_status_open()` (line 461). That hook already does the
thing the commit is after, without any lock: the fake page is published once
with `smp_store_release(&ksu_fake_status_page, ...)` (line 456) and read with
`smp_load_acquire()` (line 464). So the post-commit property already holds; the
pre-commit bug being fixed (status_lock taken to read a page that is published
lock-free anyway) never existed in our shape. The KernelSU half is a revert
inside a different fork — not applicable.

## `35cbfe11` — "susfs: Add priority uid filter"

New feature: `CMD_SUSFS_SET_PRIORITY_UID_FILTER 0x60030`, hides selected app
uids from `setpriority`/`getpriority`/`ioprio_get`/`ioprio_set` (an app-uid
caller asking about a filtered uid gets `-ESRCH`, like a uid with no
processes), backed by an RCU bitmap of the 10000–19999 range and
`susfs_ends_with()`.

**Not present, deliberately** (same reasoning as Addendum 2): it is a feature,
not a fix, it adds a 10th line to `CMD_SUSFS_SHOW_ENABLED_FEATURES` which is
exactly the string the module parses, and it needs new `kernel/sys.c` and
`block/ioprio.c` hooks.

### Collision this exposed

`0x60030` **is** the new upstream command id, and upstream's header has **no**
`CMD_SUSFS_ADD_SUS_MEMFD` at all (`... ADD_SUS_MAP 0x60020` →
`SET_PRIORITY_UID_FILTER 0x60030`). Our tree uses `0x60030` for
`ADD_SUS_MEMFD`.

Currently harmless and verified so: `case CMD_SUSFS_ADD_SUS_MEMFD` is inside
`#ifdef CONFIG_KSU_SUSFS_SUS_MEMFD`, and `miatoll_defconfig` has that **off**,
so a `0x60030` command reaches `default:` → `pr_info("sys_reboot: unknown
susfs cmd: 0x%x")` and returns 0. No misinterpretation, no memory unsafety.

It becomes a real defect only if `CONFIG_KSU_SUSFS_SUS_MEMFD` is ever enabled:
the upstream priority-filter payload (a 65535-byte `app_strings` blob) would be
handed to `susfs_add_sus_memfd()` and interpreted as a memfd struct. Documented
at the define in `include/linux/susfs_def.h`; renumber to `0x60040` if memfd is
ever turned on.
