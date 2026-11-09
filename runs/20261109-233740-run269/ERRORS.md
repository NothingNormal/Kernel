# Sixfaced CI - FAILED run #269

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42721469385
- ref     : refs/heads/bakasu
- commit  : 921b6580f805c910923cb6ae40fb219a6f546c1c
- date    : 2026-11-09 23:37:40 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
2761:./usr/include/linux/types.h:5:10: fatal error: 'asm/types.h' file not found
2778:../drivers/kernelsu/preinit/blob.S:10:10: error: Could not find incbin file '/home/runner/work/Kernel/Kernel/out/drivers/kernelsu/preinit/runner.elf'
2781:make[3]: *** [../scripts/Makefile.build:465: drivers/kernelsu/preinit/blob.o] Error 1
2782:make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
3409:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
3411:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      drivers/media/rc/keymaps/rc-dib0700-nec.o
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/built-in.a
  CC      net/netfilter/xt_quota.o
  CC      drivers/media/platform/msm/camera/cam_isp/cam_isp_context.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_workq.o
  CC      drivers/media/rc/keymaps/rc-dib0700-rc5.o
  CC      net/netfilter/xt_quota2.o
  CC      drivers/media/v4l2-core/v4l2-event.o
  CC      drivers/media/usb/uvc/uvc_driver.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_mem_mgr.o
  AR      drivers/media/platform/msm/camera/cam_isp/built-in.a
  CC      drivers/media/v4l2-core/v4l2-ctrls.o
  CC      drivers/media/rc/keymaps/rc-digitalnow-tinytwin.o
  CC      net/netfilter/xt_socket.o
  CC      drivers/media/usb/uvc/uvc_queue.o
  CC      drivers/media/rc/keymaps/rc-digittrade.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_timer.o
  CC      drivers/media/v4l2-core/v4l2-subdev.o
  CC      drivers/media/usb/uvc/uvc_v4l2.o
  CC      net/netfilter/xt_state.o
  CC      drivers/media/rc/keymaps/rc-dm1105-nec.o
  AR      drivers/media/usb/zr364xx/built-in.a
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_debug.o
  CC      drivers/media/v4l2-core/v4l2-clk.o
  CC      drivers/media/rc/keymaps/rc-dntv-live-dvb-t.o
  CC      drivers/media/usb/uvc/uvc_ctrl.o
  CC      drivers/media/usb/uvc/uvc_video.o
  CC      net/netfilter/xt_statistic.o
  CC      drivers/media/v4l2-core/v4l2-async.o
  AR      drivers/media/platform/msm/camera/cam_req_mgr/built-in.a
  CC      drivers/media/rc/keymaps/rc-dntv-live-dvbt-pro.o
  AR      drivers/media/rc/rc-core.o
  CC      drivers/media/usb/uvc/uvc_status.o
  CC      drivers/media/v4l2-core/v4l2-compat-ioctl32.o
  CC      drivers/media/rc/keymaps/rc-dtt200u.o
  CC      net/netfilter/xt_string.o
  CC      drivers/media/usb/uvc/uvc_isight.o
  CC      drivers/media/rc/keymaps/rc-dvbsky.o
  CC      drivers/media/v4l2-core/vb2-trace.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_dev.o
  CC      net/netfilter/xt_time.o
  CC      drivers/media/usb/uvc/uvc_entity.o
  CC      drivers/media/rc/keymaps/rc-dvico-mce.o
  CC      drivers/media/v4l2-core/v4l2-trace.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_core.o
  CC      drivers/media/platform/msm/vidc/msm_v4l2_vidc.o
  CC      net/netfilter/xt_u32.o
  CC      drivers/media/rc/keymaps/rc-dvico-portable.o
  AR      drivers/media/usb/uvc/uvcvideo.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_soc.o
  AR      net/netfilter/netfilter.o
  CC      drivers/media/v4l2-core/v4l2-mc.o
  CC      drivers/media/platform/msm/vidc/msm_v4l2_private.o
  CC      drivers/media/rc/keymaps/rc-em-terratec.o
  AR      drivers/media/usb/uvc/built-in.a
  AR      drivers/media/usb/built-in.a
  CC      drivers/media/platform/msm/vidc/msm_vidc_platform.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv2.o
  AR      net/netfilter/nf_conntrack.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/built-in.a
  CC      drivers/media/v4l2-core/v4l2-common.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_dev.o
  CC      drivers/media/platform/msm/vidc/msm_vidc_common.o
  CC      drivers/media/v4l2-core/v4l2-dv-timings.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv-fm53.o
  AR      net/netfilter/nf_conntrack_h323.o
  CC      drivers/media/v4l2-core/v4l2-mem2mem.o
  CC      drivers/media/platform/msm/vidc/msm_vidc.o
  CC      drivers/media/rc/keymaps/rc-evga-indtube.o
  AR      net/netfilter/nf_nat.o
  CC      drivers/media/platform/msm/camera/cam_smmu/cam_smmu_api.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_core.o
  CC      drivers/media/v4l2-core/videobuf2-core.o
  CC      drivers/media/platform/msm/vidc/msm_vdec.o
  AR      net/netfilter/nf_tables.o
  CC      drivers/media/rc/keymaps/rc-eztv.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_soc.o
  AR      drivers/media/platform/msm/camera/cam_smmu/built-in.a
  CC      drivers/media/v4l2-core/videobuf2-v4l2.o
  CC      drivers/media/platform/msm/vidc/msm_venc.o
  AR      net/netfilter/built-in.a
  CC      drivers/media/rc/keymaps/rc-flydvb.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_soc.o
  AR      net/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/built-in.a
  CC      drivers/media/v4l2-core/videobuf2-memops.o
  CC      drivers/media/platform/msm/vidc/msm_cvp.o
  CC      drivers/media/rc/keymaps/rc-flyvideo.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_dev.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_dev.o
  CC      drivers/media/v4l2-core/videobuf2-vmalloc.o
  CC      drivers/media/platform/msm/vidc/msm_smem.o
  CC      drivers/media/rc/keymaps/rc-fusionhdtv-mce.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_core.o
  CC      drivers/media/v4l2-core/videobuf2-dma-sg.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_core.o
  CC      drivers/media/rc/keymaps/rc-gadmei-rm008z.o
  CC      drivers/media/platform/msm/vidc/msm_vidc_res_parse.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_soc.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/built-in.a
  AR      drivers/media/v4l2-core/videodev.o
  CC      drivers/media/rc/keymaps/rc-geekbox.o
  CC      drivers/media/platform/msm/vidc/venus_hfi.o
  CC      drivers/media/rc/keymaps/rc-genius-tvgo-a11mce.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_dev.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/built-in.a
  AR      drivers/media/v4l2-core/built-in.a
  CC      drivers/media/platform/msm/vidc/hfi_response_handler.o
  CC      drivers/media/rc/keymaps/rc-gotview7135.o
  AR      drivers/media/v4l2loopback-master/built-in.a
  CC      drivers/media/media-device.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_core.o
  CC      drivers/media/platform/msm/vidc/hfi_packetization.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_dev.o
  CC      drivers/media/rc/keymaps/rc-hisi-poplar.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_soc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_soc.o
  CC      drivers/media/media-devnode.o
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync.o
  CC      drivers/media/rc/keymaps/rc-hisi-tv-demo.o
  CC      drivers/media/platform/msm/vidc/vidc_hfi.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/built-in.a
  CC      drivers/media/platform/msm/vidc/venus_boot.o
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_core.o
  CC      drivers/media/rc/keymaps/rc-imon-mce.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_soc_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_dev.o
  CC      drivers/media/platform/msm/vidc/msm_vidc_clocks.o
  CC      drivers/media/rc/keymaps/rc-imon-pad.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sync/built-in.a
  CC      drivers/media/rc/keymaps/rc-imon-rsc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/cam_res_mgr.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_io_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_core.o
  CC      drivers/media/rc/keymaps/rc-iodata-bctv7e.o
  CC      drivers/media/platform/msm/vidc/governors/msm_vidc_dyn_gov.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_packet_util.o
  CC      drivers/media/media-entity.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/built-in.a
  CC      drivers/media/rc/keymaps/rc-it913x-v1.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_soc.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_trace.o
  CC      drivers/media/platform/msm/vidc/governors/msm_vidc_ar50_dyn_gov.o
  CC      drivers/media/rc/keymaps/rc-it913x-v2.o
  CC      drivers/media/rc/keymaps/rc-kaiomy.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/built-in.a
  CC      drivers/media/platform/msm/camera/cam_utils/cam_common_util.o
  AR      drivers/media/platform/msm/vidc/governors/msm-vidc-dyn-gov.o
  AR      drivers/media/media.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_dev.o
  CC      drivers/media/rc/keymaps/rc-kworld-315u.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_core.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_cx_ipeak.o
  AR      drivers/media/platform/msm/vidc/governors/msm-vidc-ar50-dyn-gov.o
  CC      drivers/media/rc/keymaps/rc-kworld-pc150u.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_soc.o
  AR      drivers/media/platform/msm/camera/cam_utils/built-in.a
  AR      drivers/media/platform/msm/vidc/governors/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_io.o
  CC      drivers/media/rc/keymaps/rc-kworld-plus-tv-analog.o
  AR      drivers/media/platform/msm/vidc/msm-vidc.o
  CC      drivers/media/rc/keymaps/rc-leadtek-y04g0051.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/cam_sensor_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_cci_i2c.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_qup_i2c.o
  CC      drivers/media/rc/keymaps/rc-lme2510.o
  AR      drivers/media/platform/msm/vidc/built-in.a
  CC      drivers/media/rc/keymaps/rc-manli.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_spi.o
  CC      drivers/media/rc/keymaps/rc-medion-x10.o
  CC      drivers/media/rc/keymaps/rc-medion-x10-digitainer.o
  CC      drivers/media/rc/keymaps/rc-medion-x10-or2x.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/built-in.a
  CC      drivers/media/rc/keymaps/rc-msi-digivox-ii.o
  CC      drivers/media/rc/keymaps/rc-msi-digivox-iii.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/built-in.a
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere.o
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere-plus.o
  CC      drivers/media/rc/keymaps/rc-nebula.o
  CC      drivers/media/rc/keymaps/rc-nec-terratec-cinergy-xs.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/built-in.a
  CC      drivers/media/rc/keymaps/rc-norwood.o
  CC      drivers/media/rc/keymaps/rc-npgtech.o
  CC      drivers/media/rc/keymaps/rc-pctv-sedna.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-color.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-grey.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-pctv-hd.o
  CC      drivers/media/rc/keymaps/rc-pixelview.o
  AR      drivers/media/platform/msm/camera/built-in.a
  CC      drivers/media/rc/keymaps/rc-pixelview-mk12.o
  CC      drivers/media/rc/keymaps/rc-pixelview-002t.o
  CC      drivers/media/rc/keymaps/rc-pixelview-new.o
  CC      drivers/media/rc/keymaps/rc-powercolor-real-angel.o
  AR      drivers/media/platform/msm/built-in.a
  CC      drivers/media/rc/keymaps/rc-proteus-2309.o
  CC      drivers/media/rc/keymaps/rc-purpletv.o
  CC      drivers/media/rc/keymaps/rc-pv951.o
  CC      drivers/media/rc/keymaps/rc-hauppauge.o
  AR      drivers/media/platform/built-in.a
  CC      drivers/media/rc/keymaps/rc-rc6-mce.o
  CC      drivers/media/rc/keymaps/rc-real-audio-220-32-keys.o
  CC      drivers/media/rc/keymaps/rc-reddo.o
  CC      drivers/media/rc/keymaps/rc-snapstream-firefly.o
  CC      drivers/media/rc/keymaps/rc-streamzap.o
  CC      drivers/media/rc/keymaps/rc-tango.o
  CC      drivers/media/rc/keymaps/rc-tbs-nec.o
  CC      drivers/media/rc/keymaps/rc-technisat-ts35.o
  CC      drivers/media/rc/keymaps/rc-technisat-usb2.o
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-c-pci.o
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-s2-hd.o
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-xs.o
  CC      drivers/media/rc/keymaps/rc-terratec-slim.o
  CC      drivers/media/rc/keymaps/rc-terratec-slim-2.o
  CC      drivers/media/rc/keymaps/rc-tevii-nec.o
  CC      drivers/media/rc/keymaps/rc-tivo.o
  CC      drivers/media/rc/keymaps/rc-total-media-in-hand.o
  CC      drivers/media/rc/keymaps/rc-total-media-in-hand-02.o
  CC      drivers/media/rc/keymaps/rc-trekstor.o
  CC      drivers/media/rc/keymaps/rc-tt-1500.o
  CC      drivers/media/rc/keymaps/rc-twinhan-dtv-cab-ci.o
  CC      drivers/media/rc/keymaps/rc-twinhan1027.o
  CC      drivers/media/rc/keymaps/rc-videomate-m1f.o
  CC      drivers/media/rc/keymaps/rc-videomate-s350.o
  CC      drivers/media/rc/keymaps/rc-videomate-tv-pvr.o
  CC      drivers/media/rc/keymaps/rc-winfast.o
  CC      drivers/media/rc/keymaps/rc-winfast-usbii-deluxe.o
  CC      drivers/media/rc/keymaps/rc-zx-irdec.o
  CC      drivers/media/rc/keymaps/rc-su3000.o
  AR      drivers/media/rc/keymaps/built-in.a
  AR      drivers/media/rc/built-in.a
  AR      drivers/media/built-in.a
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
make[1]: Leaving directory '/home/runner/work/Kernel/Kernel/out'
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   95838 / 112651 (85.08%)
  Hits:            79864 /  95838 (83.33%)
    Direct:        62477 /  79864 (78.23%)
    Preprocessed:  17387 /  79864 (21.77%)
  Misses:          15974 /  95838 (16.67%)
Uncacheable calls: 16798 / 112651 (14.91%)
Errors:               15 / 112651 ( 0.01%)
Local storage:
  Cache size (GB):   0.7 /    4.0 (18.64%)
  Hits:            79864 /  95838 (83.33%)
  Misses:          15974 /  95838 (16.67%)
 Kernel build failed (make exit code 2)
```
