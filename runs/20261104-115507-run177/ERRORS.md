# Sixfaced CI - FAILED run #177

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/41823830399
- ref     : refs/heads/miui-experiment
- commit  : f9d50067d77f69a59db5b9b273a07e1cbaed21c9
- date    : 2026-11-04 11:55:07 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
2719:../drivers/gpu/drm/msm/sde/sde_connector.c:1693:31: error: incompatible pointer types passing 'struct device **' to parameter of type 'struct device *'; remove & [-Wincompatible-pointer-types]
2725:../drivers/gpu/drm/msm/sde/sde_connector.c:1704:22: error: incompatible pointer types passing 'struct device **' to parameter of type 'struct device *'; remove & [-Wincompatible-pointer-types]
2732:make[5]: *** [../scripts/Makefile.build:364: drivers/gpu/drm/msm/sde/sde_connector.o] Error 1
2733:make[4]: *** [../scripts/Makefile.build:669: drivers/gpu/drm/msm] Error 2
2734:make[3]: *** [../scripts/Makefile.build:669: drivers/gpu/drm] Error 2
2735:make[2]: *** [../scripts/Makefile.build:669: drivers/gpu] Error 2
2879:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1341: drivers] Error 2
2880:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      drivers/input/serio/libps2.o
  CC      net/wireless/nl80211.o
  CC      net/xfrm/xfrm_sysctl.o
  CC      net/netfilter/xt_comment.o
  AR      drivers/input/keyboard/built-in.a
  CC      drivers/input/misc/qpnp-power-on.o
  AR      drivers/input/serio/built-in.a
  CC      drivers/iommu/iommu.o
  CC      net/netfilter/xt_connlimit.o
  CC      net/xfrm/xfrm_replay.o
  CC      drivers/iommu/iommu-traces.o
  CC      drivers/gpu/drm/msm/sde/sde_encoder_phys_vid.o
  CC      drivers/input/misc/uinput.o
  CC      net/netfilter/xt_conntrack.o
  AR      drivers/input/misc/built-in.a
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_core.o
  CC      drivers/iommu/iommu-sysfs.o
  CC      net/xfrm/xfrm_device.o
  CC      drivers/gpu/drm/msm/sde/sde_encoder_phys_cmd.o
  CC      net/wireless/mlme.o
  CC      drivers/iommu/dma-iommu.o
  CC      net/netfilter/xt_dscp.o
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_ex_fun.o
  CC      net/xfrm/xfrm_proc.o
  CC      drivers/gpu/drm/msm/sde/sde_irq.o
  CC      drivers/iommu/msm_dma_iommu_mapping.o
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_ex_mode.o
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_gesture.o
  CC      drivers/iommu/io-pgtable.o
  CC      net/netfilter/xt_ecn.o
  CC      drivers/gpu/drm/msm/sde/sde_core_irq.o
  CC      net/xfrm/xfrm_algo.o
  CC      drivers/iommu/io-pgtable-arm.o
  CC      net/wireless/ibss.o
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_esdcheck.o
  CC      drivers/iommu/iova.o
  CC      drivers/gpu/drm/msm/sde/sde_core_perf.o
  CC      net/xfrm/xfrm_user.o
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_point_report_check.o
  CC      net/netfilter/xt_esp.o
  CC      drivers/iommu/io-pgtable-fast.o
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_spi.o
  CC      drivers/gpu/drm/msm/sde/sde_rm.o
  CC      net/wireless/sme.o
  CC      net/xfrm/xfrm_ipcomp.o
  CC      net/netfilter/xt_hashlimit.o
  CC      drivers/iommu/dma-mapping-fast.o
  CC      drivers/input/touchscreen/ft8756_spi/focaltech_flash.o
  CC      drivers/gpu/drm/msm/sde/sde_kms_utils.o
  CC      net/xfrm/xfrm_interface.o
  CC      drivers/iommu/of_iommu.o
  CC      drivers/gpu/drm/msm/sde/sde_kms.o
  CC      net/netfilter/xt_helper.o
  CC      drivers/iommu/arm-smmu.o
  AR      drivers/input/touchscreen/ft8756_spi/built-in.a
  CC      drivers/input/touchscreen/nt36xxx_spi/nt36xxx.o
  CC      net/wireless/chan.o
  AR      net/xfrm/built-in.a
  AR      drivers/gpu/drm/tilcdc/built-in.a
  AR      drivers/gpu/drm/drm_kms_helper.o
  CC      drivers/irqchip/irqchip.o
  CC      net/netfilter/xt_hl.o
  CC      drivers/irqchip/irq-gic.o
  CC      drivers/gpu/drm/msm/sde/sde_plane.o
  CC      drivers/input/touchscreen/nt36xxx_spi/nt36xxx_fw_update.o
  CC      drivers/irqchip/irq-gic-common.o
  CC      drivers/input/touchscreen/nt36xxx_spi/nt36xxx_ext_proc.o
  CC      drivers/iommu/arm-smmu-errata.o
  CC      net/netfilter/xt_iprange.o
  CC      drivers/irqchip/irq-gic-v3.o
  CC      drivers/gpu/drm/msm/sde/sde_connector.o
  CC      net/wireless/ethtool.o
  CC      drivers/iommu/arm-smmu-debug.o
  AR      drivers/input/touchscreen/nt36xxx_spi/built-in.a
  CC      drivers/input/touchscreen/xiaomi/xiaomi_touch.o
  CC      net/netfilter/xt_length.o
  AR      drivers/iommu/built-in.a
../drivers/gpu/drm/msm/sde/sde_connector.c:1693:31: error: incompatible pointer types passing 'struct device **' to parameter of type 'struct device *'; remove & [-Wincompatible-pointer-types]
 1693 |                 int rc = device_create_file(&connector->kdev, &dev_attr_disp_param);
      |                                             ^~~~~~~~~~~~~~~~
../include/linux/device.h:602:46: note: passing argument to parameter 'device' here
  602 | extern int device_create_file(struct device *device,
      |                                              ^
../drivers/gpu/drm/msm/sde/sde_connector.c:1704:22: error: incompatible pointer types passing 'struct device **' to parameter of type 'struct device *'; remove & [-Wincompatible-pointer-types]
 1704 |                 device_remove_file(&connector->kdev, &dev_attr_disp_param);
      |                                    ^~~~~~~~~~~~~~~~
../include/linux/device.h:604:47: note: passing argument to parameter 'dev' here
  604 | extern void device_remove_file(struct device *dev,
      |                                               ^
2 errors generated.
make[5]: *** [../scripts/Makefile.build:364: drivers/gpu/drm/msm/sde/sde_connector.o] Error 1
make[4]: *** [../scripts/Makefile.build:669: drivers/gpu/drm/msm] Error 2
make[3]: *** [../scripts/Makefile.build:669: drivers/gpu/drm] Error 2
make[2]: *** [../scripts/Makefile.build:669: drivers/gpu] Error 2
make[2]: *** Waiting for unfinished jobs....
  AR      drivers/input/input-core.o
  CC      drivers/irqchip/irq-partition-percpu.o
-- BakaSU version code: 35257
-- BakaSU version name: v4.2.0-rc3-516d3e87
-- Supported Managers: KernelSU Official, KOWSU, BakaSU 
  AR      drivers/input/touchscreen/xiaomi/built-in.a
-- KERNEL_VERSION: 4.14
-- KERNEL_TYPE: Non-GKI
-- BakaSU: using SuSFS Inline hook
-- You are using SUSFS, if you face any issues, please MAKE SURE you are using LATEST SUSFS KERNEL SIDE before submit issue to BakaSU.
-- We keep tracking simonpunk's latest changes, and don't maintain ANY backward compatibility for old version of susfs.
  CC      drivers/input/touchscreen/of_touchscreen.o
  CC      net/compat.o
-- BakaSU/susfs_inline: ksu_handle_setresuid found
-- BakaSU/susfs_inline: ksu_handle_execveat found
-- BakaSU/susfs_inline: ksu_handle_faccessat found
-- BakaSU/susfs_inline: ksu_handle_sys_read found
-- BakaSU/susfs_inline: ksu_handle_stat found
-- BakaSU/susfs_inline: ksu_handle_sys_reboot found
-- BakaSU/susfs_inline: ksu_handle_input_handle_event found
-- BakaSU/susfs_feature_check: selinux_hide manual hook found

-- SUSFS_VERSION: v2.3.0
-- BakaSU/compat: current_sid found
-- BakaSU/compat: selinux_state found
-- BakaSU/compat: strncpy found
-- BakaSU/compat: kernel_read found
-- BakaSU/compat: kernel_write found
-- BakaSU/compat: selinux_inode found
-- BakaSU/compat: selinux_cred found
-- BakaSU/compat: seccomp_filter_count found
-- BakaSU/compat: security_inode_init_security_anon found
-- BakaSU/compat: file_inode() found
-- BakaSU/compat: ns_get_path found
-- BakaSU/compat: modern dentry_open found
-- BakaSU/compat: ext4_unregister_sysfs found
-- BakaSU/compat: f_op->read_iter found
-- BakaSU/compat: iterate_dir found
-- BakaSU/compat: modern static_key_interface found
-- BakaSU/compat: d_inode found
-- BakaSU/compat: inode_lock found
-- BakaSU/compat: modern errno file found
-- BakaSU/compat: overflow.h found
-- BakaSU/compat: modern proc ns header file found
-- BakaSU/compat: android spec POLICYDB_CONFIG_ANDROID_NETLINK_ROUTE found!!
-- BakaSU/compat: android spec POLICYDB_CONFIG_ANDROID_NETLINK_GETNEIGH found!!
-- BakaSU/compat: found sidtab as reference
-- BakaSU/compat: found hlist in security_hook_list
-- BakaSU/compat: found required provide lsm name
-- BakaSU/compat: found security_add_hooks
-- BakaSU/compat: modern dentry_open found
-- BakaSU/compat: extended_perms found
-- BakaSU/compat: stricter mls_context_to_sid found
-- BakaSU/compat: found hash indexed sidtab
-- BakaSU/compat: found KERNEL_PARAM_OPS_FL_NOARG
  CC      net/netfilter/xt_limit.o
  CC      drivers/input/touchscreen/tp_common.o
  CC      net/wireless/mesh.o
  HOSTCC  drivers/kernelsu/tools/code_generation/syscall_no/syscall_no
  CC      drivers/irqchip/msm_show_resume_irq.o
  CC      drivers/kernelsu/preinit/preinit.o
  CC      net/sysctl_net.o
  AR      drivers/input/touchscreen/built-in.a
  AR      drivers/input/built-in.a
  CC      drivers/irqchip/qcom/pdc.o
  CC      net/wireless/ap.o
  CC      net/netfilter/xt_mac.o
  CC      drivers/irqchip/qcom/pdc-sdmmagpie.o
  CC      net/wireless/trace.o
  CC      drivers/kernelsu/preinit/runner.elf
  CC      net/netfilter/xt_multiport.o
  CC      net/wireless/ocb.o
  CC      drivers/irqchip/qcom/pdc-atoll.o
  GEN     drivers/kernelsu/compat/syscall_no.h
  CC      net/wireless/of.o
  AS      drivers/kernelsu/preinit/blob.o
  CC      drivers/kernelsu/core/init.o
  AR      drivers/irqchip/qcom/built-in.a
  AR      drivers/irqchip/built-in.a
  CC      net/netfilter/xt_owner.o
  CC      net/netfilter/xt_pkttype.o
  CC      drivers/kernelsu/policy/allowlist.o
  CC      net/netfilter/xt_policy.o
  CC      drivers/kernelsu/policy/app_profile.o
  CC      net/netfilter/xt_quota.o
  CC      net/netfilter/xt_quota2.o
  CC      net/wireless/wext-core.o
  CC      net/netfilter/xt_socket.o
  CC      net/netfilter/xt_state.o
  CC      drivers/kernelsu/policy/feature.o
  CC      net/wireless/wext-proc.o
  CC      net/netfilter/xt_statistic.o
  CC      net/wireless/wext-spy.o
  CC      net/netfilter/xt_string.o
  CC      drivers/kernelsu/feature/sucompat.o
  CC      net/wireless/wext-priv.o
  CC      drivers/kernelsu/feature/kernel_umount.o
  CC      drivers/kernelsu/feature/sulog.o
  CC      net/wireless/regdb.o
  CC      net/netfilter/xt_time.o
  CC      drivers/kernelsu/feature/adb_root.o
  CC      drivers/kernelsu/feature/selinux_hide.o
  AR      net/wireless/cfg80211.o
  CC      drivers/kernelsu/feature/shell_su_adb_only.o
  CC      net/netfilter/xt_u32.o
  AR      net/wireless/built-in.a
  AR      net/netfilter/netfilter.o
  AR      net/netfilter/nf_conntrack.o
  AR      net/netfilter/nf_conntrack_h323.o
  CC      drivers/kernelsu/feature/module_blacklist.o
  AR      net/netfilter/nf_nat.o
  AR      net/netfilter/nf_tables.o
  CC      drivers/kernelsu/hook/setuid_hook.o
  CC      drivers/kernelsu/feature/dynamic_manager.o
  CC      drivers/kernelsu/manager/apk_sign.o
  CC      drivers/kernelsu/manager/throne_tracker.o
  AR      net/netfilter/built-in.a
  AR      net/built-in.a
  CC      drivers/kernelsu/manager/manager.o
  CC      drivers/kernelsu/compat/kernel_compat.o
  CC      drivers/kernelsu/infra/su_mount_ns.o
  CC      drivers/kernelsu/infra/seccomp_cache.o
  CC      drivers/kernelsu/infra/file_wrapper.o
  CC      drivers/kernelsu/infra/event_queue.o
  CC      drivers/kernelsu/runtime/boot_event.o
  CC      drivers/kernelsu/runtime/ksud_integration.o
  CC      drivers/kernelsu/infra/btf_parser.o
  CC      drivers/kernelsu/selinux/inode.o
  CC      drivers/kernelsu/selinux/selinux.o
  CC      drivers/kernelsu/selinux/sepolicy.o
  CC      drivers/kernelsu/selinux/rules.o
  CC      drivers/kernelsu/sulog/event.o
  CC      drivers/kernelsu/sulog/fd.o
  CC      drivers/kernelsu/supercall/dispatch.o
  CC      drivers/kernelsu/supercall/perm.o
  CC      drivers/kernelsu/supercall/supercall.o
  CC      drivers/kernelsu/compat/apatch_conflict.o
  CC      drivers/kernelsu/hook/arm64/patch_memory.o
  CC      drivers/kernelsu/infra/symbol_resolver.o
  CC      drivers/kernelsu/hook/lsm_hooks.o
  AR      drivers/kernelsu/kernelsu.o
  AR      drivers/kernelsu/built-in.a
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1341: drivers] Error 2
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   2327 / 3026 (76.90%)
  Hits:              79 / 2327 ( 3.39%)
    Direct:          78 /   79 (98.73%)
    Preprocessed:     1 /   79 ( 1.27%)
  Misses:          2248 / 2327 (96.61%)
Uncacheable calls:  699 / 3026 (23.10%)
Local storage:
  Cache size (GB):  0.1 /  4.0 ( 2.64%)
  Hits:              79 / 2327 ( 3.39%)
  Misses:          2248 / 2327 (96.61%)
 Kernel build failed (make exit code 2)
```
