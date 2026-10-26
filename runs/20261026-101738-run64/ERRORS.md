# Sixfaced CI - FAILED run #64

- run URL : https://github.com/rkmandilwar-rgb/Kernel/actions/runs/40305231434
- ref     : refs/heads/arena/cd1940a2-kernel
- commit  : 2fd5f025e32969c529da11e64ab8e92f490267a6
- date    : 2026-10-26 10:17:38 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
1411:../kernel/sched/cpufreq_schedutil.c:323:30: error: use of undeclared identifier 'rq'
1415:make[3]: *** [../scripts/Makefile.build:364: kernel/sched/cpufreq_schedutil.o] Error 1
1416:make[2]: *** [../scripts/Makefile.build:667: kernel/sched] Error 2
1417:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: kernel] Error 2
2279:../net/netfilter/nf_tables_api.c:3534:8: error: no member named 'dead' in 'struct nft_set'
2283:make[3]: *** [../scripts/Makefile.build:364: net/netfilter/nf_tables_api.o] Error 1
2284:make[2]: *** [../scripts/Makefile.build:667: net/netfilter] Error 2
2505:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1286: net] Error 2
4489:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/init_deinit/src/init_cmd_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/init_deinit/src/init_deinit_lmac.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/init_deinit/src/init_event_handler.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/init_deinit/src/service_ready_util.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/mlme/vdev_mgr/src/target_if_vdev_mgr_tx_ops.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/mlme/vdev_mgr/src/target_if_vdev_mgr_rx_ops.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/mlme/psoc/src/target_if_psoc_timer_tx_ops.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/mlme/psoc/src/target_if_psoc_wake_lock.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/crypto/src/target_if_crypto.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_arp.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_gtk.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_hw_filter.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_lphb.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_main.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_mc_addr_filtering.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_static_config.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_suspend_resume.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_wow.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_ns.o
  CC      drivers/staging/qcacld-3.0/components/target_if/pmo/src/target_if_pmo_pkt_filter.o
  CC      drivers/staging/qcacld-3.0/components/target_if/disa/src/target_if_disa.o
  CC      drivers/staging/qcacld-3.0/components/target_if/blacklist_mgr/src/target_if_blm.o
  CC      drivers/staging/qcacld-3.0/components/target_if/ipa/src/target_if_ipa.o
  CC      drivers/staging/qcacld-3.0/components/target_if/action_oui/src/target_if_action_oui.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/global_lmac_if/src/wlan_global_lmac_if.o
  CC      drivers/staging/qcacld-3.0/core/dp/htt/htt_tx.o
  CC      drivers/staging/qcacld-3.0/core/dp/htt/htt.o
  CC      drivers/staging/qcacld-3.0/core/dp/htt/htt_t2h.o
  CC      drivers/staging/qcacld-3.0/core/dp/htt/htt_h2t.o
  CC      drivers/staging/qcacld-3.0/core/dp/htt/htt_fw_stats.o
  CC      drivers/staging/qcacld-3.0/core/dp/htt/htt_rx.o
  CC      drivers/staging/qcacld-3.0/core/dp/htt/htt_rx_ll.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/misc/dfs.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/misc/dfs_cac.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/misc/dfs_nol.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/misc/dfs_random_chan_sel.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/misc/dfs_process_radar_found_ind.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/dispatcher/src/wlan_dfs_init_deinit_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/dispatcher/src/wlan_dfs_lmac_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/dispatcher/src/wlan_dfs_mlme_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/dispatcher/src/wlan_dfs_tgt_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/dispatcher/src/wlan_dfs_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/dispatcher/src/wlan_dfs_utils_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/dfs/src/target_if_dfs.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/dfs/src/target_if_dfs_partial_offload.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_fcc_bin5.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_bindetects.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_init.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_misc.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_phyerr_tlv.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_process_phyerr.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_process_radarevent.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_staggered.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_radar.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/filtering/dfs_partial_offload_radar.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/dfs/core/src/misc/dfs_filter_init.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/obj_mgr/src/wlan_objmgr_global_obj.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/obj_mgr/src/wlan_objmgr_pdev_obj.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/obj_mgr/src/wlan_objmgr_peer_obj.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/obj_mgr/src/wlan_objmgr_psoc_obj.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/obj_mgr/src/wlan_objmgr_vdev_obj.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/wifi_pos/src/wifi_pos_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/wifi_pos/src/wifi_pos_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/wifi_pos/src/wifi_pos_ucfg.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/wifi_pos/src/wifi_pos_utils.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/os_if/linux/wifi_pos/src/os_if_wifi_pos.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/wifi_pos/src/target_if_wifi_pos.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/cp_stats/src/target_if_mc_cp_stats.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cp_stats/core/src/wlan_cp_stats_comp_handler.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cp_stats/core/src/wlan_cp_stats_obj_mgr_handler.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cp_stats/core/src/wlan_cp_stats_ol_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/os_if/linux/cp_stats/src/wlan_cfg80211_mc_cp_stats.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cp_stats/dispatcher/src/wlan_cp_stats_utils_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cp_stats/dispatcher/src/wlan_cp_stats_mc_tgt_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cp_stats/dispatcher/src/wlan_cp_stats_mc_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/target_if/interop_issues_ap/src/target_if_interop_issues_ap.o
  CC      drivers/staging/qcacld-3.0/components/interop_issues_ap/core/src/wlan_interop_issues_ap_api.o
  CC      drivers/staging/qcacld-3.0/os_if/interop_issues_ap/src/wlan_cfg80211_interop_issues_ap.o
  CC      drivers/staging/qcacld-3.0/components/interop_issues_ap/dispatcher/src/wlan_interop_issues_ap_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/interop_issues_ap/dispatcher/src/wlan_interop_issues_ap_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/nan/core/src/nan_main.o
  CC      drivers/staging/qcacld-3.0/components/nan/core/src/nan_api.o
  CC      drivers/staging/qcacld-3.0/components/nan/dispatcher/src/nan_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/nan/dispatcher/src/cfg_nan.o
  CC      drivers/staging/qcacld-3.0/components/target_if/nan/src/target_if_nan.o
  CC      drivers/staging/qcacld-3.0/os_if/nan/src/os_if_nan.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/mgmt_txrx/core/src/wlan_mgmt_txrx_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/mgmt_txrx/dispatcher/src/wlan_mgmt_txrx_utils_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/mgmt_txrx/dispatcher/src/wlan_mgmt_txrx_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/tdls/core/src/wlan_tdls_main.o
  CC      drivers/staging/qcacld-3.0/components/tdls/core/src/wlan_tdls_cmds_process.o
  CC      drivers/staging/qcacld-3.0/components/tdls/core/src/wlan_tdls_peer.o
  CC      drivers/staging/qcacld-3.0/components/tdls/core/src/wlan_tdls_mgmt.o
  CC      drivers/staging/qcacld-3.0/components/tdls/core/src/wlan_tdls_ct.o
  CC      drivers/staging/qcacld-3.0/components/tdls/dispatcher/src/wlan_tdls_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/tdls/dispatcher/src/wlan_tdls_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/tdls/dispatcher/src/wlan_tdls_utils_api.o
  CC      drivers/staging/qcacld-3.0/components/tdls/dispatcher/src/wlan_tdls_cfg.o
  CC      drivers/staging/qcacld-3.0/os_if/tdls/src/wlan_cfg80211_tdls.o
  CC      drivers/staging/qcacld-3.0/components/target_if/tdls/src/target_if_tdls.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_main.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_apf.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_arp.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_gtk.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_mc_addr_filtering.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_static_config.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_wow.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_lphb.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_suspend_resume.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_hw_filter.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_obj_mgmt_api.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_arp.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_gtk.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_wow.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_static_config.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_mc_addr_filtering.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_lphb.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_suspend_resume.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_hw_filter.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_pkt_filter.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_pkt_filter.o
  CC      drivers/staging/qcacld-3.0/components/pmo/core/src/wlan_pmo_ns.o
  CC      drivers/staging/qcacld-3.0/components/pmo/dispatcher/src/wlan_pmo_tgt_ns.o
  CC      drivers/staging/qcacld-3.0/components/p2p/dispatcher/src/wlan_p2p_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/p2p/dispatcher/src/wlan_p2p_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/p2p/dispatcher/src/wlan_p2p_cfg.o
  CC      drivers/staging/qcacld-3.0/components/p2p/dispatcher/src/wlan_p2p_api.o
  CC      drivers/staging/qcacld-3.0/components/p2p/core/src/wlan_p2p_main.o
  CC      drivers/staging/qcacld-3.0/components/p2p/core/src/wlan_p2p_roc.o
  CC      drivers/staging/qcacld-3.0/components/p2p/core/src/wlan_p2p_off_chan_tx.o
  CC      drivers/staging/qcacld-3.0/os_if/p2p/src/wlan_cfg80211_p2p.o
  CC      drivers/staging/qcacld-3.0/components/target_if/p2p/src/target_if_p2p.o
  CC      drivers/staging/qcacld-3.0/components/cmn_services/policy_mgr/src/wlan_policy_mgr_action.o
  CC      drivers/staging/qcacld-3.0/components/cmn_services/policy_mgr/src/wlan_policy_mgr_core.o
  CC      drivers/staging/qcacld-3.0/components/cmn_services/policy_mgr/src/wlan_policy_mgr_get_set_utils.o
  CC      drivers/staging/qcacld-3.0/components/cmn_services/policy_mgr/src/wlan_policy_mgr_init_deinit.o
  CC      drivers/staging/qcacld-3.0/components/cmn_services/policy_mgr/src/wlan_policy_mgr_ucfg.o
  CC      drivers/staging/qcacld-3.0/components/cmn_services/policy_mgr/src/wlan_policy_mgr_pcl.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/utils/nlink/src/wlan_nlink_srv.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/utils/ptt/src/wlan_ptt_sock_svc.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_utils.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_legacy_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_rules.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_internal.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_non_scan.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_queue.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/serialization/src/wlan_serialization_scan.o
  CC      drivers/staging/qcacld-3.0/core/pld/src/pld_common.o
  CC      drivers/staging/qcacld-3.0/core/pld/src/pld_snoc.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/sm_engine/src/wlan_sm_engine.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/mlme_objmgr/dispatcher/src/wlan_vdev_mlme_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/vdev_mgr/core/src/vdev_mlme_sm.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/vdev_mgr/dispatcher/src/wlan_vdev_mlme_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/vdev_mgr/core/src/vdev_mgr_ops.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/vdev_mgr/dispatcher/src/wlan_vdev_mgr_tgt_if_rx_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/vdev_mgr/dispatcher/src/wlan_vdev_mgr_tgt_if_tx_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/vdev_mgr/dispatcher/src/wlan_vdev_mgr_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/vdev_mgr/dispatcher/src/wlan_vdev_mgr_utils_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/mlme_objmgr/dispatcher/src/wlan_cmn_mlme_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/mlme_objmgr/dispatcher/src/wlan_pdev_mlme_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/pdev_mgr/dispatcher/src/wlan_pdev_mlme_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/mlme_objmgr/dispatcher/src/wlan_psoc_mlme_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/mlme/psoc_mgr/dispatcher/src/wlan_psoc_mlme_api.o
  CC      drivers/staging/qcacld-3.0/components/mlme/core/src/wlan_mlme_main.o
  CC      drivers/staging/qcacld-3.0/components/mlme/dispatcher/src/wlan_mlme_api.o
  CC      drivers/staging/qcacld-3.0/components/mlme/dispatcher/src/wlan_mlme_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/mlme/core/src/wlan_mlme_vdev_mgr_interface.o
  CC      drivers/staging/qcacld-3.0/components/fw_offload/core/src/wlan_fw_offload_main.o
  CC      drivers/staging/qcacld-3.0/components/fw_offload/dispatcher/src/wlan_fwol_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/fw_offload/dispatcher/src/wlan_fwol_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/target_if/fw_offload/src/target_if_fwol.o
  CC      drivers/staging/qcacld-3.0/os_if/fw_offload/src/os_if_fwol.o
  CC      drivers/staging/qcacld-3.0/components/blacklist_mgr/core/src/wlan_blm_main.o
  CC      drivers/staging/qcacld-3.0/components/blacklist_mgr/core/src/wlan_blm_core.o
  CC      drivers/staging/qcacld-3.0/components/blacklist_mgr/dispatcher/src/wlan_blm_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/blacklist_mgr/dispatcher/src/wlan_blm_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/ipa/dispatcher/src/wlan_ipa_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/ipa/dispatcher/src/wlan_ipa_obj_mgmt_api.o
  CC      drivers/staging/qcacld-3.0/components/ipa/dispatcher/src/wlan_ipa_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/ipa/core/src/wlan_ipa_main.o
  CC      drivers/staging/qcacld-3.0/components/ipa/core/src/wlan_ipa_core.o
  CC      drivers/staging/qcacld-3.0/components/ipa/core/src/wlan_ipa_stats.o
  CC      drivers/staging/qcacld-3.0/components/ipa/core/src/wlan_ipa_rm.o
  CC      drivers/staging/qcacld-3.0/components/disa/core/src/wlan_disa_main.o
  CC      drivers/staging/qcacld-3.0/components/disa/dispatcher/src/wlan_disa_obj_mgmt_api.o
  CC      drivers/staging/qcacld-3.0/components/disa/dispatcher/src/wlan_disa_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/disa/dispatcher/src/wlan_disa_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/components/action_oui/core/src/wlan_action_oui_main.o
  CC      drivers/staging/qcacld-3.0/components/action_oui/core/src/wlan_action_oui_parse.o
  CC      drivers/staging/qcacld-3.0/components/action_oui/dispatcher/src/wlan_action_oui_tgt_api.o
  CC      drivers/staging/qcacld-3.0/components/action_oui/dispatcher/src/wlan_action_oui_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/global_umac_dispatcher/lmac_if/src/wlan_lmac_if.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/core/src/wlan_scan_cache_db.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/core/src/wlan_scan_11d.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/core/src/wlan_scan_bss_score.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/core/src/wlan_scan_filter.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/core/src/wlan_scan_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/core/src/wlan_scan_manager.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/dispatcher/src/wlan_scan_tgt_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/dispatcher/src/wlan_scan_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/dispatcher/src/wlan_scan_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/scan/dispatcher/src/wlan_scan_utils_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/os_if/linux/scan/src/wlan_cfg80211_scan.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/os_if/linux/wlan_cfg80211.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/scan/src/target_if_scan.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/utils/src/wlan_utility.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/spectral/core/spectral_offload.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/spectral/core/spectral_common.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/spectral/dispatcher/src/wlan_spectral_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/spectral/dispatcher/src/wlan_spectral_utils_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/spectral/dispatcher/src/wlan_spectral_tgt_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/os_if/linux/spectral/src/wlan_cfg80211_spectral.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/os_if/linux/spectral/src/os_if_spectral_netlink.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/spectral/target_if_spectral_netlink.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/spectral/target_if_spectral_phyerr.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/spectral/target_if_spectral.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/spectral/target_if_spectral_sim.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/gpio/dispatcher/src/wlan_gpio_tgt_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/gpio/dispatcher/src/wlan_gpio_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/gpio/core/src/wlan_gpio_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/os_if/linux/gpio/src/wlan_cfg80211_gpio.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/gpio/target_if_gpio.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/green_ap/core/src/wlan_green_ap_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/green_ap/dispatcher/src/wlan_green_ap_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/green_ap/dispatcher/src/wlan_green_ap_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/target_if/green_ap/src/target_if_green_ap.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/crypto/src/wlan_crypto_global_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/crypto/src/wlan_crypto_ucfg_api.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/crypto/src/wlan_crypto_main.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/crypto/src/wlan_crypto_obj_mgr.o
  CC      drivers/staging/qcacld-3.0/../qca-wifi-host-cmn/umac/cmn_services/crypto/src/wlan_crypto_param_handling.o
  AR      drivers/staging/qcacld-3.0/wlan.o
  AR      drivers/staging/qcacld-3.0/built-in.a
  AR      drivers/staging/built-in.a
  AR      drivers/built-in.a
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   91926 / 106783 (86.09%)
  Hits:            70851 /  91926 (77.07%)
    Direct:        66532 /  70851 (93.90%)
    Preprocessed:   4319 /  70851 ( 6.10%)
  Misses:          21075 /  91926 (22.93%)
Uncacheable calls: 14857 / 106783 (13.91%)
Local storage:
  Cache size (GB):   0.8 /    4.0 (19.94%)
  Hits:            70851 /  91926 (77.07%)
  Misses:          21075 /  91926 (22.93%)
 Kernel build failed (make exit code 2)
```
