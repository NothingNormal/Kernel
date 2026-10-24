# Sixfaced CI - FAILED run #31

- run URL : https://github.com/rkmandilwar-rgb/Kernel/actions/runs/40084816419
- ref     : refs/heads/arena/cd1940a2-kernel
- commit  : 9ebeb43c530f7bee836e5ee8d678011ee76780c0
- date    : 2026-10-24 10:17:51 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
2580:../drivers/kernelsu/policy/allowlist.h:6:10: fatal error: 'uapi/app_profile.h' file not found
2584:make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/core/init.o] Error 1
2585:make[2]: *** [../scripts/Makefile.build:667: drivers/kernelsu] Error 2
2895:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: drivers] Error 2
2896:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      net/sctp/inqueue.o
  CC      net/sched/sch_fq_codel.o
  CC      net/netfilter/xt_tcpudp.o
  CC      net/ipv4/netfilter/iptable_raw.o
  CC      drivers/gpu/drm/msm/sde/sde_plane.o
  CC      net/sched/cls_u32.o
  CC      net/sctp/outqueue.o
  CC      net/netfilter/xt_mark.o
  CC      net/ipv4/netfilter/iptable_security.o
  CC      net/wireless/scan.o
  CC      net/sched/cls_fw.o
  CC      net/netfilter/xt_connmark.o
  CC      drivers/gpu/drm/msm/sde/sde_connector.o
  CC      net/sctp/ulpqueue.o
  CC      net/ipv4/netfilter/ipt_ah.o
  CC      net/sched/cls_flow.o
  CC      drivers/gpu/drm/msm/sde/sde_color_processing.o
  CC      net/sctp/tsnmap.o
  CC      net/netfilter/xt_nat.o
  CC      net/wireless/nl80211.o
  CC      net/ipv4/netfilter/ipt_rpfilter.o
  CC      net/sched/cls_bpf.o
  CC      drivers/gpu/drm/msm/sde/sde_vbif.o
  CC      net/sctp/bind_addr.o
  CC      net/netfilter/xt_CLASSIFY.o
  CC      net/ipv4/netfilter/ipt_MASQUERADE.o
  CC      net/sched/cls_matchall.o
  CC      drivers/gpu/drm/msm/sde_io_util.o
  CC      net/netfilter/xt_CONNSECMARK.o
  CC      net/sctp/socket.o
  CC      net/ipv4/netfilter/ipt_REJECT.o
  CC      net/sched/ematch.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_reg_dma_v1_color_proc.o
  CC      net/wireless/mlme.o
  CC      net/netfilter/xt_CT.o
  CC      net/ipv4/netfilter/arp_tables.o
  CC      net/sched/em_cmp.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_color_proc_v4.o
  CC      net/sctp/primitive.o
  CC      net/netfilter/xt_DSCP.o
  CC      net/ipv4/netfilter/arpt_mangle.o
  CC      net/sched/em_nbyte.o
  CC      net/wireless/ibss.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_ad4.o
  CC      net/sctp/output.o
  CC      net/sched/em_u32.o
  CC      net/netfilter/xt_HL.o
  CC      net/ipv4/netfilter/arptable_filter.o
  CC      drivers/gpu/drm/msm/sde_edid_parser.o
  CC      net/wireless/sme.o
  CC      net/sched/em_meta.o
  CC      net/ipv4/netfilter/nf_dup_ipv4.o
  CC      drivers/gpu/drm/msm/sde_hdcp_1x.o
  CC      net/sctp/input.o
  CC      net/netfilter/xt_HMARK.o
  CC      net/sched/em_text.o
  AR      net/ipv4/netfilter/nf_conntrack_ipv4.o
  AR      net/ipv4/netfilter/nf_nat_ipv4.o
  CC      drivers/gpu/drm/msm/sde_hdcp_2x.o
  AR      net/ipv4/netfilter/built-in.a
  CC      net/ipv4/inet_diag.o
  CC      net/wireless/chan.o
  AR      net/sched/built-in.a
  CC      net/netfilter/xt_LOG.o
  CC      net/xfrm/xfrm_policy.o
  CC      net/sctp/debug.o
  CC      net/ipv4/tcp_diag.o
  CC      drivers/gpu/drm/msm/sde_dbg.o
  CC      net/ipv4/udp_diag.o
  CC      net/netfilter/xt_NETMAP.o
  CC      net/wireless/ethtool.o
  CC      drivers/gpu/drm/msm/sde_dbg_evtlog.o
  CC      net/sctp/stream.o
  CC      net/xfrm/xfrm_state.o
  CC      net/ipv4/tcp_bbr.o
  CC      net/netfilter/xt_NFLOG.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_kcal_ctrl.o
  CC      net/wireless/mesh.o
  CC      net/netfilter/xt_NFQUEUE.o
  CC      net/sctp/auth.o
  CC      drivers/gpu/drm/msm/sde_rsc.o
  CC      net/xfrm/xfrm_hash.o
  CC      net/ipv4/tcp_westwood.o
  CC      net/netfilter/xt_REDIRECT.o
  CC      net/xfrm/xfrm_input.o
  CC      net/sctp/offload.o
  CC      drivers/gpu/drm/msm/sde_rsc_hw.o
  CC      net/ipv4/xfrm4_policy.o
  CC      net/wireless/ap.o
  CC      drivers/gpu/drm/msm/sde_rsc_hw_v3.o
  CC      net/netfilter/xt_SECMARK.o
  CC      drivers/gpu/drm/msm/sde/sde_fence.o
  CC      net/sctp/proc.o
  CC      net/xfrm/xfrm_output.o
  CC      net/netfilter/xt_TPROXY.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy.o
  CC      net/ipv4/xfrm4_state.o
  CC      net/wireless/trace.o
  CC      net/sctp/sysctl.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_pwr.o
  CC      net/xfrm/xfrm_sysctl.o
  CC      net/ipv4/xfrm4_input.o
  CC      net/netfilter/xt_TCPMSS.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_hw_v2_0.o
  CC      net/sctp/ipv6.o
  CC      net/xfrm/xfrm_replay.o
  CC      net/netfilter/xt_TEE.o
  CC      net/ipv4/xfrm4_output.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_hw_v3_0.o
  CC      net/wireless/ocb.o
  CC      net/sctp/sctp_diag.o
  CC      net/xfrm/xfrm_device.o
  CC      net/netfilter/xt_TRACE.o
  CC      net/ipv4/xfrm4_protocol.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_hw_v4_0.o
  AR      net/sctp/sctp.o
  AR      net/sctp/built-in.a
  AR      drivers/gpu/drm/tilcdc/built-in.a
  AR      drivers/gpu/drm/drm_kms_helper.o
  CC      net/wireless/of.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_calc.o
  CC      net/netfilter/xt_IDLETIMER.o
  CC      net/xfrm/xfrm_proc.o
  CC      net/ipv4/bpf_tcp_ca.o
  CC      net/compat.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_v2_0.o
  CC      net/wireless/wext-core.o
  CC      net/netfilter/xt_HARDIDLETIMER.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_v3_0.o
  CC [M]  net/ipv4/tcp_bic.o
  CC      net/xfrm/xfrm_algo.o
  CC      net/netfilter/xt_bpf.o
  CC      net/sysctl_net.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_phy_timing_v4_0.o
  CC [M]  net/ipv4/tcp_htcp.o
  CC      net/wireless/wext-proc.o
  CC      net/xfrm/xfrm_user.o
  CC      net/netfilter/xt_comment.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_cmn.o
  AR      drivers/gpu/drm/drm.o
  AR      net/ipv4/gre.o
  CC      net/wireless/wext-spy.o
  AR      net/ipv4/udp_tunnel.o
  CC      net/xfrm/xfrm_ipcomp.o
  AR      net/ipv4/built-in.a
  CC      net/netfilter/xt_connlimit.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_1_4.o
  CC      net/xfrm/xfrm_interface.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_2_0.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl_hw_2_2.o
  CC      net/wireless/wext-priv.o
  CC      net/netfilter/xt_conntrack.o
  CC      net/netfilter/xt_dscp.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_ctrl.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_catalog.o
  AR      net/xfrm/built-in.a
  CC      net/netfilter/xt_ecn.o
  CC      net/wireless/regdb.o
  CC      net/netfilter/xt_esp.o
  CC      net/netfilter/xt_hashlimit.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_drm.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_display.o
  CC      net/netfilter/xt_helper.o
  AR      net/wireless/cfg80211.o
  AR      net/wireless/built-in.a
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_panel.o
  CC      net/netfilter/xt_hl.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_clk_manager.o
  CC      net/netfilter/xt_iprange.o
  CC      net/netfilter/xt_length.o
  CC      drivers/gpu/drm/msm/dsi-staging/dsi_display_test.o
  CC      net/netfilter/xt_limit.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_catalog.o
  CC      net/netfilter/xt_mac.o
  CC      net/netfilter/xt_multiport.o
  CC      net/netfilter/xt_owner.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_cdm.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_dspp.o
  CC      net/netfilter/xt_pkttype.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_intf.o
  CC      net/netfilter/xt_policy.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_lm.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_ctl.o
  CC      net/netfilter/xt_quota.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_util.o
  CC      net/netfilter/xt_quota2.o
  CC      net/netfilter/xt_socket.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_sspp.o
  CC      net/netfilter/xt_state.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_wb.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_rot.o
  CC      net/netfilter/xt_statistic.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_pingpong.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_top.o
  CC      net/netfilter/xt_string.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_interrupts.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_vbif.o
  CC      net/netfilter/xt_time.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_blk.o
  CC      drivers/gpu/drm/msm/sde/sde_formats.o
  CC      net/netfilter/xt_u32.o
  CC      drivers/gpu/drm/msm/sde_power_handle.o
  AR      net/netfilter/netfilter.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_color_processing_v1_7.o
  AR      net/netfilter/nf_conntrack.o
  AR      net/netfilter/nf_conntrack_h323.o
  AR      net/netfilter/nf_nat.o
  CC      drivers/gpu/drm/msm/sde/sde_reg_dma.o
  CC      drivers/gpu/drm/msm/sde/sde_hw_reg_dma_v1.o
  AR      net/netfilter/built-in.a
  AR      net/built-in.a
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
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: drivers] Error 2
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   5994 / 7144 (83.90%)
  Hits:             318 / 5994 ( 5.31%)
    Direct:         172 /  318 (54.09%)
    Preprocessed:   146 /  318 (45.91%)
  Misses:          5676 / 5994 (94.69%)
Uncacheable calls: 1150 / 7144 (16.10%)
Local storage:
  Cache size (GB):  0.2 /  4.0 ( 6.04%)
  Hits:             318 / 5994 ( 5.31%)
  Misses:          5676 / 5994 (94.69%)
 Kernel build failed (make exit code 2)
```
