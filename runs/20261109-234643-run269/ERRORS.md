# Sixfaced CI - FAILED run #269

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42721469385
- ref     : refs/heads/bakasu
- commit  : 921b6580f805c910923cb6ae40fb219a6f546c1c
- date    : 2026-11-09 23:46:43 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
2800:./usr/include/linux/types.h:5:10: fatal error: 'asm/types.h' file not found
2815:../drivers/kernelsu/preinit/blob.S:10:10: error: Could not find incbin file '/home/runner/work/Kernel/Kernel/out/drivers/kernelsu/preinit/runner.elf'
2818:make[3]: *** [../scripts/Makefile.build:465: drivers/kernelsu/preinit/blob.o] Error 1
2819:make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
2820:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
2936:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      net/sched/act_gact.o
  CC      drivers/leds/leds-qti-tri-led.o
-- BakaSU/compat: seccomp_filter_count found
  CC      net/netfilter/nf_nat_irc.o
-- BakaSU/compat: security_inode_init_security_anon found
  CC      net/netfilter/nf_nat_tftp.o
  CC      drivers/gpu/drm/msm/msm_iommu.o
  CC      net/sched/act_mirred.o
  CC      drivers/leds/trigger/ledtrig-timer.o
-- BakaSU/compat: file_inode() found
  AR      drivers/leds/trigger/built-in.a
  CC      net/netfilter/nf_tables_core.o
  CC      drivers/gpu/drm/msm/msm_smmu.o
-- BakaSU/compat: ns_get_path found
  AR      drivers/leds/built-in.a
  CC      net/sched/act_skbedit.o
-- BakaSU/compat: modern dentry_open found
-- BakaSU/compat: ext4_unregister_sysfs found
-- BakaSU/compat: f_op->read_iter found
-- BakaSU/compat: iterate_dir found
  CC      net/netfilter/nf_tables_api.o
  CC      drivers/gpu/drm/msm/msm_perf.o
  CC      net/sched/act_bpf.o
-- BakaSU/compat: modern static_key_interface found
-- BakaSU/compat: d_inode found
-- BakaSU/compat: inode_lock found
-- BakaSU/compat: modern errno file found
-- BakaSU/compat: overflow.h found
-- BakaSU/compat: modern proc ns header file found
  CC      drivers/gpu/drm/msm/msm_rd.o
-- BakaSU/compat: android spec POLICYDB_CONFIG_ANDROID_NETLINK_ROUTE found!!
-- BakaSU/compat: android spec POLICYDB_CONFIG_ANDROID_NETLINK_GETNEIGH found!!
  CC      net/netfilter/nf_tables_trace.o
  CC      net/sched/sch_fifo.o
-- BakaSU/compat: found sidtab as reference
  CC      drivers/gpu/drm/msm/msm_ringbuffer.o
-- BakaSU/compat: found hlist in security_hook_list
-- BakaSU/compat: found required provide lsm name
  CC      net/netfilter/nft_immediate.o
  CC      drivers/gpu/drm/msm/msm_prop.o
  CC      net/sched/sch_htb.o
  CC      net/sctp/sm_statetable.o
-- BakaSU/compat: found security_add_hooks
  CC      drivers/gpu/drm/msm/msm_fence.o
  CC      net/netfilter/nft_cmp.o
  CC      net/sctp/sm_statefuns.o
-- BakaSU/compat: modern dentry_open found
-- BakaSU/compat: extended_perms found
-- BakaSU/compat: stricter mls_context_to_sid found
  CC      net/sched/sch_ingress.o
-- BakaSU/compat: found hash indexed sidtab
-- BakaSU/compat: found KERNEL_PARAM_OPS_FL_NOARG
  CC      drivers/gpu/drm/msm/msm_debugfs.o
  CC      net/netfilter/nft_range.o
  CC      net/sctp/sm_sideeffect.o
  CC      net/sched/sch_tbf.o
  AR      drivers/gpu/drm/msm/msm_drm.o
  CC      net/netfilter/nft_bitwise.o
  CC      net/sched/sch_prio.o
  CC      net/sctp/protocol.o
  CC      net/netfilter/nft_byteorder.o
  CC      net/sched/sch_multiq.o
  CC      net/sctp/endpointola.o
  CC      net/netfilter/nft_payload.o
  AR      drivers/gpu/drm/msm/built-in.a
  CC      net/sched/sch_fq_codel.o
  AR      drivers/gpu/drm/built-in.a
  CC      net/netfilter/nft_lookup.o
  CC      net/sctp/associola.o
  CC      net/sched/cls_u32.o
  CC      net/netfilter/nft_dynset.o
  CC      net/sctp/transport.o
  HOSTCC  drivers/kernelsu/tools/code_generation/syscall_no/syscall_no
  AR      drivers/gpu/built-in.a
  CC      net/sched/cls_fw.o
  CC      net/netfilter/x_tables.o
  CC      net/sctp/chunk.o
  AR      drivers/macintosh/built-in.a
  CC      net/netfilter/xt_tcpudp.o
  CC      net/sctp/sm_make_chunk.o
  CC      net/sched/cls_flow.o
  CC      drivers/mailbox/mailbox.o
  CC      net/sctp/ulpevent.o
  CC      net/netfilter/xt_mark.o
  CC      net/sched/cls_bpf.o
  CC      net/sctp/inqueue.o
  CC      drivers/mailbox/qcom-apcs-ipc-mailbox.o
  CC      net/netfilter/xt_connmark.o
  CC      drivers/kernelsu/preinit/preinit.o
  CC      net/sched/cls_matchall.o
  CC      drivers/mailbox/qcom-rpmh-mailbox.o
  CC      net/sctp/outqueue.o
  CC      drivers/kernelsu/preinit/runner.elf
  CC      net/netfilter/xt_nat.o
  CC      net/sched/ematch.o
  CC      drivers/mailbox/msm_qmp.o
  CC      net/sctp/ulpqueue.o
  CC      net/netfilter/xt_CLASSIFY.o
In file included from /home/runner/work/Kernel/Kernel/KernelSU/kernel/preinit/runner/runner.c:15:
In file included from /home/runner/work/Kernel/Kernel/KernelSU/kernel/preinit/runner/nolibc/nolibc.h:95:
In file included from /home/runner/work/Kernel/Kernel/KernelSU/kernel/preinit/runner/nolibc/std.h:19:
./usr/include/linux/types.h:5:10: fatal error: 'asm/types.h' file not found
    5 | #include <asm/types.h>
      |          ^~~~~~~~~~~~~
  CC      net/sched/em_cmp.o
  AR      drivers/mailbox/built-in.a
1 error generated.
  CC      net/netfilter/xt_CONNSECMARK.o
  CC      net/sctp/tsnmap.o
  GEN     drivers/kernelsu/compat/syscall_no.h
  AS      drivers/kernelsu/preinit/blob.o
  CC      net/sched/em_nbyte.o
  CC      net/strparser/strparser.o
  CC      net/netfilter/xt_CT.o
  CC      net/sctp/bind_addr.o
  CC      net/sched/em_u32.o
../drivers/kernelsu/preinit/blob.S:10:10: error: Could not find incbin file '/home/runner/work/Kernel/Kernel/out/drivers/kernelsu/preinit/runner.elf'
 .incbin "/home/runner/work/Kernel/Kernel/out/drivers/kernelsu/preinit/runner.elf"
         ^
make[3]: *** [../scripts/Makefile.build:465: drivers/kernelsu/preinit/blob.o] Error 1
make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
make[1]: *** Waiting for unfinished jobs....
  CC      net/netfilter/xt_DSCP.o
  AR      net/strparser/built-in.a
  CC      net/sched/em_meta.o
  CC      net/sctp/socket.o
  CC      net/netfilter/xt_HL.o
  CC      net/unix/af_unix.o
  CC      net/unix/garbage.o
  CC      net/sched/em_text.o
  CC      net/netfilter/xt_HMARK.o
  CC      net/sctp/primitive.o
  CC      net/unix/sysctl_net_unix.o
  CC      net/netfilter/xt_LOG.o
  AR      net/sched/built-in.a
  CC      net/sctp/output.o
  CC      net/unix/scm.o
  CC      net/wireless/core.o
  CC      net/netfilter/xt_NETMAP.o
  CC      net/sctp/input.o
  AR      net/unix/unix.o
  CC      net/wireless/sysfs.o
  CC      net/netfilter/xt_NFLOG.o
  CC      net/xfrm/xfrm_policy.o
  CC      net/sctp/debug.o
  AR      net/unix/built-in.a
  CC      net/wireless/radiotap.o
  CC      net/netfilter/xt_NFQUEUE.o
  CC      net/xfrm/xfrm_state.o
  CC      net/sctp/stream.o
  CC      net/compat.o
  CC      net/xfrm/xfrm_hash.o
  CC      net/wireless/util.o
  CC      net/netfilter/xt_REDIRECT.o
  CC      net/sctp/auth.o
  CC      net/xfrm/xfrm_input.o
  CC      net/sysctl_net.o
  CC      net/wireless/reg.o
  CC      net/netfilter/xt_SECMARK.o
  CC      net/sctp/offload.o
  CC      net/xfrm/xfrm_output.o
  CC      net/sctp/proc.o
  CC      net/wireless/scan.o
  CC      net/netfilter/xt_TPROXY.o
  CC      net/sctp/sysctl.o
  CC      net/xfrm/xfrm_sysctl.o
  CC      net/netfilter/xt_TCPMSS.o
  CC      net/xfrm/xfrm_replay.o
  CC      net/wireless/nl80211.o
  CC      net/netfilter/xt_TEE.o
  CC      net/wireless/mlme.o
  CC      net/sctp/ipv6.o
  CC      net/xfrm/xfrm_device.o
  CC      net/sctp/sctp_diag.o
  CC      net/netfilter/xt_TRACE.o
  CC      net/wireless/ibss.o
  AR      net/sctp/sctp.o
  CC      net/xfrm/xfrm_proc.o
  CC      net/netfilter/xt_IDLETIMER.o
  CC      net/wireless/sme.o
  CC      net/wireless/chan.o
  CC      net/netfilter/xt_HARDIDLETIMER.o
  CC      net/xfrm/xfrm_algo.o
  AR      net/sctp/built-in.a
  CC      net/wireless/ethtool.o
  CC      net/wireless/mesh.o
  CC      net/netfilter/xt_addrtype.o
  CC      net/xfrm/xfrm_user.o
  CC      net/xfrm/xfrm_ipcomp.o
  CC      net/netfilter/xt_bpf.o
  CC      net/netfilter/xt_comment.o
  CC      net/wireless/ap.o
  CC      net/wireless/trace.o
  CC      net/netfilter/xt_connlimit.o
  CC      net/xfrm/xfrm_interface.o
  CC      net/wireless/ocb.o
  CC      net/netfilter/xt_conntrack.o
  CC      net/wireless/of.o
  CC      net/netfilter/xt_dscp.o
  AR      net/xfrm/built-in.a
  CC      net/wireless/wext-core.o
  CC      net/wireless/wext-proc.o
  CC      net/wireless/wext-spy.o
  CC      net/netfilter/xt_ecn.o
  CC      net/wireless/wext-priv.o
  CC      net/wireless/regdb.o
  CC      net/netfilter/xt_esp.o
  CC      net/netfilter/xt_hashlimit.o
  CC      net/netfilter/xt_helper.o
  CC      net/netfilter/xt_hl.o
  AR      net/wireless/cfg80211.o
  CC      net/netfilter/xt_iprange.o
  CC      net/netfilter/xt_length.o
  CC      net/netfilter/xt_limit.o
  CC      net/netfilter/xt_mac.o
  CC      net/netfilter/xt_multiport.o
  CC      net/netfilter/xt_owner.o
  CC      net/netfilter/xt_pkttype.o
  CC      net/netfilter/xt_policy.o
  AR      net/wireless/built-in.a
  CC      net/netfilter/xt_quota.o
  CC      net/netfilter/xt_quota2.o
  CC      net/netfilter/xt_socket.o
  CC      net/netfilter/xt_state.o
  CC      net/netfilter/xt_statistic.o
  CC      net/netfilter/xt_string.o
  CC      net/netfilter/xt_time.o
  CC      net/netfilter/xt_u32.o
  AR      net/netfilter/netfilter.o
  AR      net/netfilter/nf_conntrack_h323.o
  AR      net/netfilter/nf_nat.o
  AR      net/netfilter/nf_conntrack.o
  AR      net/netfilter/nf_tables.o
  AR      net/netfilter/built-in.a
  AR      net/built-in.a
make[1]: Leaving directory '/home/runner/work/Kernel/Kernel/out'
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   98212 / 115292 (85.19%)
  Hits:            82237 /  98212 (83.73%)
    Direct:        64850 /  82237 (78.86%)
    Preprocessed:  17387 /  82237 (21.14%)
  Misses:          15975 /  98212 (16.27%)
Uncacheable calls: 17062 / 115292 (14.80%)
Errors:               18 / 115292 ( 0.02%)
Local storage:
  Cache size (GB):   0.7 /    4.0 (18.64%)
  Hits:            82237 /  98212 (83.73%)
  Misses:          15975 /  98212 (16.27%)
 Kernel build failed (make exit code 2)
```
