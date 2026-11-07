# Sixfaced CI - FAILED run #216

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42372522208
- ref     : refs/heads/OriginSU
- commit  : 2893186dcdd47c30d99412728191f2a067de87fd
- date    : 2026-11-07 12:34:05 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
2848:../drivers/kernelsu/policy/su_request.c:201:9: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
2857:../drivers/kernelsu/policy/su_request.c:201:9: error: a parameter list without types is only allowed in a function definition
2864:../drivers/kernelsu/policy/su_request.c:201:9: error: function declared in block scope cannot have 'static' storage class
2871:../drivers/kernelsu/policy/su_request.c:201:9: error: use of undeclared identifier '_rs'
2881:make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/policy/su_request.o] Error 1
2882:make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
2981:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1341: drivers] Error 2
2982:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_cmn.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_1_4.o
  CC      drivers/iommu/io-pgtable-arm.o
  CC      net/netfilter/xt_hashlimit.o
  AR      drivers/input/touchscreen/nt36xxx_spi/built-in.a
  CC      drivers/irqchip/irq-gic-common.o
  CC      drivers/input/touchscreen/tp_common.o
../drivers/kernelsu/Kbuild:116: -- KernelSU: ./usr/include not found, preinit disabled
  AR      drivers/input/touchscreen/built-in.a
  AR      drivers/input/built-in.a
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_2_0.o
  CC      drivers/irqchip/irq-gic-v3.o
  CC      drivers/iommu/iova.o
-- OriginSU version code: 35456
-- OriginSU version name: v4.2.7-4245c03c@OriginSU
-- Primary Manager: OriginSU (also supported: MKSU, RKSU, KOWSU, SukiSU-Ultra, ReSukiSU) 
-- KERNEL_VERSION: 4.14
-- KERNEL_TYPE: Non-GKI
-- OriginSU: using SuSFS Inline hook
-- You are using SUSFS, if you face any issues, please MAKE SURE you are using LATEST SUSFS KERNEL SIDE before submit issue to ReSukiSU.
-- We keep tracking simonpunk's latest changes, and don't maintain ANY backward compatibility for old version of susfs.
  CC      drivers/iommu/io-pgtable-fast.o
-- OriginSU/susfs_inline: ksu_handle_setresuid found
-- OriginSU/susfs_inline: ksu_handle_execveat found
-- OriginSU/susfs_inline: ksu_handle_faccessat found
-- OriginSU/susfs_inline: ksu_handle_sys_read found
-- OriginSU/susfs_inline: ksu_handle_stat found
-- OriginSU/susfs_inline: ksu_handle_sys_reboot found
-- OriginSU/susfs_inline: ksu_handle_input_handle_event found
-- OriginSU/susfs_feature_check: selinux_hide manual hook found

-- SUSFS_VERSION: v2.3.0
-- KPM is disabled
-- OriginSU/compat: current_sid found
-- OriginSU/compat: selinux_state found
-- OriginSU/compat: strncpy found
-- OriginSU/compat: kernel_read found
-- OriginSU/compat: kernel_write found
-- OriginSU/compat: selinux_inode found
-- OriginSU/compat: selinux_cred found
-- OriginSU/compat: seccomp_filter_count found
-- OriginSU/compat: security_inode_init_security_anon found
-- OriginSU/compat: file_inode() found
-- OriginSU/compat: ns_get_path found
-- OriginSU/compat: modern dentry_open found
-- OriginSU/compat: ext4_unregister_sysfs found
-- OriginSU/compat: f_op->read_iter found
-- OriginSU/compat: iterate_dir found
-- OriginSU/compat: modern static_key_interface found
-- OriginSU/compat: d_inode found
-- OriginSU/compat: inode_lock found
-- OriginSU/compat: modern errno file found
-- OriginSU/compat: overflow.h found
-- OriginSU/compat: modern proc ns header file found
-- OriginSU/compat: android spec POLICYDB_CONFIG_ANDROID_NETLINK_ROUTE found!!
-- OriginSU/compat: android spec POLICYDB_CONFIG_ANDROID_NETLINK_GETNEIGH found!!
-- OriginSU/compat: found sidtab as reference
-- OriginSU/compat: found hlist in security_hook_list
-- OriginSU/compat: found required provide lsm name
  CC      net/netfilter/xt_helper.o
-- OriginSU/compat: found security_add_hooks
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_2_2.o
-- OriginSU/compat: modern dentry_open found
-- OriginSU/compat: extended_perms found
-- OriginSU/compat: stricter mls_context_to_sid found
-- OriginSU/compat: found hash indexed sidtab
  CC      drivers/irqchip/irq-partition-percpu.o
  CC      drivers/leds/led-core.o
  CC      drivers/iommu/dma-mapping-fast.o
  CC      drivers/kernelsu/core/init.o
  CC      drivers/iommu/of_iommu.o
  CC      drivers/leds/led-class.o
  CC      drivers/irqchip/msm_show_resume_irq.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl.o
  CC      net/netfilter/xt_hl.o
  CC      drivers/irqchip/qcom/pdc.o
  CC      drivers/leds/led-triggers.o
  CC      drivers/kernelsu/policy/allowlist.o
  CC      drivers/iommu/arm-smmu.o
  CC      drivers/leds/leds-qpnp-flash-v2.o
  CC      drivers/irqchip/qcom/pdc-sdmmagpie.o
  CC      net/netfilter/xt_iprange.o
  CC      drivers/irqchip/qcom/pdc-atoll.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_catalog.o
  CC      drivers/iommu/arm-smmu-errata.o
  CC      drivers/leds/leds-qti-tri-led.o
  CC      drivers/kernelsu/policy/app_profile.o
  AR      drivers/irqchip/qcom/built-in.a
  AR      drivers/irqchip/built-in.a
  CC      drivers/leds/trigger/ledtrig-timer.o
  CC      drivers/iommu/arm-smmu-debug.o
  CC      net/netfilter/xt_length.o
  AR      drivers/macintosh/built-in.a
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_drm.o
  AR      drivers/leds/trigger/built-in.a
  AR      drivers/leds/built-in.a
  CC      drivers/mailbox/mailbox.o
  CC      drivers/kernelsu/policy/feature.o
  CC      net/netfilter/xt_limit.o
  CC      drivers/kernelsu/policy/su_request.o
  AR      drivers/iommu/built-in.a
  CC      drivers/md/dm-uevent.o
  CC      drivers/mailbox/qcom-apcs-ipc-mailbox.o
  CC      net/netfilter/xt_mac.o
../drivers/kernelsu/policy/su_request.c:201:9: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
  201 |         pr_warn_ratelimited("su_request: queue full, deny uid=%u\n", uid);
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
../include/linux/printk.h:443:2: note: expanded from macro 'pr_warn_ratelimited'
  443 |         printk_ratelimited(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
../include/linux/printk.h:422:9: note: expanded from macro 'printk_ratelimited'
  422 |         static DEFINE_RATELIMIT_STATE(_rs,                              \
      |         ~~~~~~ ^
../drivers/kernelsu/policy/su_request.c:201:9: error: a parameter list without types is only allowed in a function definition
../include/linux/printk.h:443:2: note: expanded from macro 'pr_warn_ratelimited'
  443 |         printk_ratelimited(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
      |         ^
../include/linux/printk.h:422:32: note: expanded from macro 'printk_ratelimited'
  422 |         static DEFINE_RATELIMIT_STATE(_rs,                              \
      |                                       ^
../drivers/kernelsu/policy/su_request.c:201:9: error: function declared in block scope cannot have 'static' storage class
../include/linux/printk.h:443:2: note: expanded from macro 'pr_warn_ratelimited'
  443 |         printk_ratelimited(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
      |         ^
../include/linux/printk.h:422:2: note: expanded from macro 'printk_ratelimited'
  422 |         static DEFINE_RATELIMIT_STATE(_rs,                              \
      |         ^
../drivers/kernelsu/policy/su_request.c:201:9: error: use of undeclared identifier '_rs'
  201 |         pr_warn_ratelimited("su_request: queue full, deny uid=%u\n", uid);
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
../include/linux/printk.h:443:2: note: expanded from macro 'pr_warn_ratelimited'
  443 |         printk_ratelimited(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
../include/linux/printk.h:426:19: note: expanded from macro 'printk_ratelimited'
  426 |         if (__ratelimit(&_rs))                                          \
      |                          ^~~
4 errors generated.
make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/policy/su_request.o] Error 1
make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
make[2]: *** Waiting for unfinished jobs....
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_display.o
  CC      drivers/mailbox/qcom-rpmh-mailbox.o
  CC      net/netfilter/xt_multiport.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_panel.o
  CC      drivers/md/dm.o
  CC      net/netfilter/xt_owner.o
  CC      drivers/mailbox/msm_qmp.o
  CC      drivers/md/dm-table.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_clk_manager.o
  CC      net/netfilter/xt_pkttype.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_display_test.o
  CC      net/netfilter/xt_policy.o
  AR      drivers/mailbox/built-in.a
  CC      drivers/md/dm-target.o
  CC      drivers/md/dm-linear.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_catalog.o
  CC      net/netfilter/xt_quota.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_cdm.o
  CC      drivers/md/dm-stripe.o
  CC      drivers/md/dm-ioctl.o
  CC      net/netfilter/xt_quota2.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_dspp.o
  CC      net/netfilter/xt_socket.o
  CC      drivers/md/dm-io.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_intf.o
  CC      net/netfilter/xt_state.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_lm.o
  CC      drivers/md/dm-kcopyd.o
  CC      net/netfilter/xt_statistic.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_ctl.o
  CC      drivers/md/dm-sysfs.o
  CC      drivers/md/dm-stats.o
  CC      net/netfilter/xt_string.o
  CC      drivers/md/dm-rq.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_util.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_sspp.o
  CC      drivers/md/dm-builtin.o
  CC      net/netfilter/xt_time.o
  CC      drivers/md/dm-bufio.o
  CC      drivers/md/dm-crypt.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_wb.o
  CC      net/netfilter/xt_u32.o
  AR      net/netfilter/netfilter.o
  AR      net/netfilter/nf_conntrack.o
  CC      drivers/md/dm-default-key.o
  CC      drivers/md/dm-snap.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_rot.o
  AR      net/netfilter/nf_conntrack_h323.o
  AR      net/netfilter/nf_nat.o
  CC      drivers/md/dm-exception-store.o
  AR      net/netfilter/nf_tables.o
  CC      drivers/md/dm-snap-transient.o
  CC      drivers/md/dm-snap-persistent.o
  AR      net/netfilter/built-in.a
  AR      net/built-in.a
  CC      drivers/gpu/drm/msm/sde/sde_hw_pingpong.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_top.o
  CC      drivers/md/dm-verity-fec.o
  CC      drivers/md/dm-verity-target.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_interrupts.o
  AR      drivers/md/dm-mod.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_vbif.o
  AR      drivers/md/dm-snapshot.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_blk.o
  CC      drivers/gpu/drm/msm/sde/sde_formats.o
  CC      drivers/gpu/drm/msm/sde_power_handle.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_color_processing_v1_7.o
  AR      drivers/md/dm-verity.o
  AR      drivers/md/built-in.a
  CC      drivers/gpu/drm/msm/sde/sde_reg_dma.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_reg_dma_v1.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_dsc.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_ds.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_qdss.o
  CC      drivers/gpu/drm/msm/sde/sde_wb.o
  CC      drivers/gpu/drm/msm/sde/sde_encoder_phys_wb.o
  CC      drivers/gpu/drm/msm/msm_atomic.o
  CC      drivers/gpu/drm/msm/msm_drv.o
  CC      drivers/gpu/drm/msm/msm_fb.o
  CC      drivers/gpu/drm/msm/msm_gem.o
  CC      drivers/gpu/drm/msm/msm_gem_prime.o
  CC      drivers/gpu/drm/msm/msm_gem_submit.o
  CC      drivers/gpu/drm/msm/msm_gem_shrinker.o
  CC      drivers/gpu/drm/msm/msm_gem_vma.o
  CC      drivers/gpu/drm/msm/msm_gpu.o
  CC      drivers/gpu/drm/msm/msm_iommu.o
  CC      drivers/gpu/drm/msm/msm_smmu.o
  CC      drivers/gpu/drm/msm/msm_perf.o
  CC      drivers/gpu/drm/msm/msm_rd.o
  CC      drivers/gpu/drm/msm/msm_ringbuffer.o
  CC      drivers/gpu/drm/msm/msm_prop.o
  CC      drivers/gpu/drm/msm/msm_fence.o
  CC      drivers/gpu/drm/msm/msm_debugfs.o
  AR      drivers/gpu/drm/msm/msm_drm.o
  AR      drivers/gpu/drm/msm/built-in.a
  AR      drivers/gpu/drm/built-in.a
  AR      drivers/gpu/built-in.a
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1341: drivers] Error 2
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   2355 / 2927 (80.46%)
  Hits:              32 / 2355 ( 1.36%)
    Direct:          30 /   32 (93.75%)
    Preprocessed:     2 /   32 ( 6.25%)
  Misses:          2323 / 2355 (98.64%)
Uncacheable calls:  572 / 2927 (19.54%)
Local storage:
  Cache size (GB):  0.1 /  4.0 ( 2.74%)
  Hits:              32 / 2355 ( 1.36%)
  Misses:          2323 / 2355 (98.64%)
 Kernel build failed (make exit code 2)
```
