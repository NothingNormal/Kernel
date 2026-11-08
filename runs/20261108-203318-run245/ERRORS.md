# Sixfaced CI - FAILED run #245

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42520468068
- ref     : refs/heads/OriginSU
- commit  : 48c0c9e7cf12dbcf1a632cf5a6ce24c2578d4f42
- date    : 2026-11-08 20:33:18 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
2819:../drivers/kernelsu/policy/su_request.c:201:9: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
2828:../drivers/kernelsu/policy/su_request.c:201:9: error: a parameter list without types is only allowed in a function definition
2835:../drivers/kernelsu/policy/su_request.c:201:9: error: function declared in block scope cannot have 'static' storage class
2842:../drivers/kernelsu/policy/su_request.c:201:9: error: use of undeclared identifier '_rs'
2853:make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/policy/su_request.o] Error 1
2854:make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
2984:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
2986:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      net/sctp/offload.o
  CC      net/netfilter/xt_NETMAP.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_hw_v3_0.o
  CC      net/sctp/proc.o
  CC      net/sched/cls_bpf.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_hw_v4_0.o
  CC      net/sched/cls_matchall.o
  CC      net/netfilter/xt_NFLOG.o
  CC      net/sctp/sysctl.o
  CC      net/wireless/core.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_calc.o
  CC      net/netfilter/xt_NFQUEUE.o
  CC      net/wireless/sysfs.o
  CC      net/sctp/ipv6.o
  CC      net/sched/ematch.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_v2_0.o
  CC      net/netfilter/xt_REDIRECT.o
  CC      net/sctp/sctp_diag.o
  CC      net/sched/em_cmp.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_v3_0.o
  CC      drivers/kernelsu/policy/allowlist.o
  CC      net/wireless/radiotap.o
  CC      net/netfilter/xt_SECMARK.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_v4_0.o
  AR      net/sctp/sctp.o
  CC      net/sched/em_nbyte.o
  CC      drivers/kernelsu/policy/app_profile.o
  CC      net/netfilter/xt_TPROXY.o
  CC      net/wireless/util.o
  CC      net/sched/em_u32.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_cmn.o
  CC      net/netfilter/xt_TCPMSS.o
  CC      drivers/kernelsu/policy/feature.o
  AR      net/sctp/built-in.a
  CC      net/wireless/reg.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_1_4.o
  CC      net/sched/em_meta.o
  CC      net/netfilter/xt_TEE.o
  CC      drivers/kernelsu/policy/su_request.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_2_0.o
  CC      drivers/mailbox/mailbox.o
  CC      net/wireless/scan.o
  CC      net/sched/em_text.o
  CC      net/netfilter/xt_TRACE.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_2_2.o
  CC      drivers/mailbox/qcom-apcs-ipc-mailbox.o
  CC      net/wireless/nl80211.o
  CC      net/netfilter/xt_IDLETIMER.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl.o
  AR      net/sched/built-in.a
  CC      drivers/mailbox/qcom-rpmh-mailbox.o
  CC      net/wireless/mlme.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_catalog.o
  CC      net/netfilter/xt_HARDIDLETIMER.o
  CC      drivers/mailbox/msm_qmp.o
  CC      net/netfilter/xt_addrtype.o
  CC      net/xfrm/xfrm_policy.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_drm.o
  CC      net/wireless/ibss.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_display.o
  AR      drivers/mailbox/built-in.a
  CC      net/wireless/sme.o
  CC      net/netfilter/xt_bpf.o
  CC      net/xfrm/xfrm_state.o
  CC      net/netfilter/xt_comment.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_panel.o
  CC      net/wireless/chan.o
  CC      net/xfrm/xfrm_hash.o
  CC      net/netfilter/xt_connlimit.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_clk_manager.o
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
  CC      net/wireless/ethtool.o
make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/policy/su_request.o] Error 1
make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
make[2]: *** Waiting for unfinished jobs....
  CC      net/compat.o
  CC      net/xfrm/xfrm_input.o
  CC      net/netfilter/xt_conntrack.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_display_test.o
  CC      net/wireless/mesh.o
  CC      net/wireless/ap.o
  CC      drivers/md/dm-uevent.o
  CC      net/xfrm/xfrm_output.o
  CC      net/netfilter/xt_dscp.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_catalog.o
  CC      drivers/md/dm.o
  CC      net/wireless/trace.o
  CC      drivers/md/dm-table.o
  CC      net/xfrm/xfrm_sysctl.o
  CC      net/netfilter/xt_ecn.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_cdm.o
  CC      net/wireless/ocb.o
  CC      drivers/md/dm-target.o
  CC      net/sysctl_net.o
  CC      net/xfrm/xfrm_replay.o
  CC      drivers/md/dm-linear.o
  CC      net/wireless/of.o
  CC      net/netfilter/xt_esp.o
  CC      net/xfrm/xfrm_device.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_dspp.o
  CC      drivers/md/dm-stripe.o
  CC      net/wireless/wext-core.o
  CC      net/netfilter/xt_hashlimit.o
  CC      net/xfrm/xfrm_proc.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_intf.o
  CC      drivers/md/dm-ioctl.o
  CC      net/wireless/wext-proc.o
  CC      net/wireless/wext-spy.o
  CC      net/netfilter/xt_helper.o
  CC      net/xfrm/xfrm_algo.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_lm.o
  CC      drivers/md/dm-io.o
  CC      net/xfrm/xfrm_user.o
  CC      net/wireless/wext-priv.o
  CC      net/xfrm/xfrm_ipcomp.o
  CC      net/netfilter/xt_hl.o
  CC      drivers/md/dm-kcopyd.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_ctl.o
  CC      net/xfrm/xfrm_interface.o
  CC      net/wireless/regdb.o
  CC      net/netfilter/xt_iprange.o
  CC      drivers/md/dm-sysfs.o
  CC      drivers/md/dm-stats.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_util.o
  AR      net/wireless/cfg80211.o
  CC      drivers/md/dm-rq.o
  AR      net/xfrm/built-in.a
  CC      drivers/gpu/drm/msm/sde/sde_hw_sspp.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_wb.o
  CC      drivers/md/dm-builtin.o
  CC      net/netfilter/xt_length.o
  CC      drivers/md/dm-bufio.o
  AR      net/wireless/built-in.a
  CC      drivers/gpu/drm/msm/sde/sde_hw_rot.o
  CC      net/netfilter/xt_limit.o
  CC      drivers/md/dm-crypt.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_pingpong.o
  CC      drivers/md/dm-default-key.o
  CC      drivers/md/dm-snap.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_top.o
  CC      net/netfilter/xt_mac.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_interrupts.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_vbif.o
  CC      drivers/md/dm-exception-store.o
  CC      drivers/md/dm-snap-transient.o
  CC      drivers/md/dm-snap-persistent.o
  CC      net/netfilter/xt_multiport.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_blk.o
  CC      drivers/gpu/drm/msm/sde/sde_formats.o
  CC      drivers/gpu/drm/msm/sde_power_handle.o
  CC      drivers/md/dm-verity-fec.o
  CC      net/netfilter/xt_owner.o
  CC      drivers/md/dm-verity-target.o
  CC      net/netfilter/xt_pkttype.o
  CC      net/netfilter/xt_policy.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_color_processing_v1_7.o
  CC      drivers/gpu/drm/msm/sde/sde_reg_dma.o
  CC      net/netfilter/xt_quota.o
  AR      drivers/md/dm-mod.o
  CC      net/netfilter/xt_quota2.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_reg_dma_v1.o
  CC      net/netfilter/xt_socket.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_dsc.o
  CC      net/netfilter/xt_state.o
  AR      drivers/md/dm-snapshot.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_ds.o
  AR      drivers/md/dm-verity.o
  CC      net/netfilter/xt_statistic.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_qdss.o
  CC      net/netfilter/xt_string.o
  CC      net/netfilter/xt_time.o
  AR      drivers/md/built-in.a
  CC      drivers/gpu/drm/msm/sde/sde_wb.o
  CC      net/netfilter/xt_u32.o
  AR      net/netfilter/netfilter.o
  AR      net/netfilter/nf_conntrack.o
  CC      drivers/gpu/drm/msm/sde/sde_encoder_phys_wb.o
  AR      net/netfilter/nf_conntrack_h323.o
  CC      drivers/gpu/drm/msm/msm_atomic.o
  AR      net/netfilter/nf_nat.o
  CC      drivers/gpu/drm/msm/msm_drv.o
  AR      net/netfilter/nf_tables.o
  CC      drivers/gpu/drm/msm/msm_fb.o
  CC      drivers/gpu/drm/msm/msm_gem.o
  CC      drivers/gpu/drm/msm/msm_gem_prime.o
  CC      drivers/gpu/drm/msm/msm_gem_submit.o
  CC      drivers/gpu/drm/msm/msm_gem_shrinker.o
  CC      drivers/gpu/drm/msm/msm_gem_vma.o
  CC      drivers/gpu/drm/msm/msm_gpu.o
  AR      net/netfilter/built-in.a
  CC      drivers/gpu/drm/msm/msm_iommu.o
  CC      drivers/gpu/drm/msm/msm_smmu.o
  CC      drivers/gpu/drm/msm/msm_perf.o
  AR      net/built-in.a
  CC      drivers/gpu/drm/msm/msm_rd.o
  CC      drivers/gpu/drm/msm/msm_ringbuffer.o
  CC      drivers/gpu/drm/msm/msm_prop.o
  CC      drivers/gpu/drm/msm/msm_fence.o
  CC      drivers/gpu/drm/msm/msm_debugfs.o
  AR      drivers/gpu/drm/msm/msm_drm.o
  AR      drivers/gpu/drm/msm/built-in.a
  AR      drivers/gpu/drm/built-in.a
  AR      drivers/gpu/built-in.a
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
make[1]: Leaving directory '/home/runner/work/Kernel/Kernel/out'
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   43500 / 49652 (87.61%)
  Hits:            34759 / 43500 (79.91%)
    Direct:        25700 / 34759 (73.94%)
    Preprocessed:   9059 / 34759 (26.06%)
  Misses:           8741 / 43500 (20.09%)
Uncacheable calls:  6140 / 49652 (12.37%)
Errors:               12 / 49652 ( 0.02%)
Local storage:
  Cache size (GB):   0.5 /   4.0 (12.46%)
  Hits:            34759 / 43500 (79.91%)
  Misses:           8741 / 43500 (20.09%)
 Kernel build failed (make exit code 2)
```
