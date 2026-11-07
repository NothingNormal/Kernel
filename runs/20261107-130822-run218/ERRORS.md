# Sixfaced CI - FAILED run #218

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42375312182
- ref     : refs/heads/OriginSU
- commit  : 2447214c116b7af1008189337463b3443f7ba208
- date    : 2026-11-07 13:08:22 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
2983:../drivers/kernelsu/policy/su_request.c:201:9: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
2992:../drivers/kernelsu/policy/su_request.c:201:9: error: a parameter list without types is only allowed in a function definition
2999:../drivers/kernelsu/policy/su_request.c:201:9: error: function declared in block scope cannot have 'static' storage class
3006:../drivers/kernelsu/policy/su_request.c:201:9: error: use of undeclared identifier '_rs'
3018:make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/policy/su_request.o] Error 1
3019:make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
3483:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1341: drivers] Error 2
3484:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      drivers/media/platform/msm/camera/cam_icp/cam_icp_subdev.o
  CC      drivers/media/platform/msm/camera/cam_icp/icp_hw/icp_hw_mgr/cam_icp_hw_mgr.o
  CC      drivers/media/rc/keymaps/rc-dm1105-nec.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/ife_csid_hw/cam_ife_csid_dev.o
  CC      drivers/media/v4l2-core/v4l2-mem2mem.o
  CC      drivers/media/rc/keymaps/rc-dntv-live-dvb-t.o
  CC      drivers/media/platform/msm/vidc/governors/msm_vidc_dyn_gov.o
  CC      drivers/media/v4l2-core/videobuf2-core.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/ife_csid_hw/cam_ife_csid_soc.o
  AR      drivers/media/platform/msm/camera/cam_icp/icp_hw/icp_hw_mgr/built-in.a
  CC      drivers/media/rc/keymaps/rc-dntv-live-dvbt-pro.o
  CC      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_dma_hw/jpeg_dma_dev.o
  CC      drivers/media/platform/msm/vidc/governors/msm_vidc_ar50_dyn_gov.o
  CC      drivers/media/v4l2-core/videobuf2-v4l2.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/ife_csid_hw/cam_ife_csid_core.o
  CC      drivers/media/platform/msm/camera/cam_icp/icp_hw/ipe_hw/ipe_dev.o
  CC      drivers/media/rc/keymaps/rc-dtt200u.o
  CC      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_dma_hw/jpeg_dma_core.o
  CC      drivers/media/v4l2-core/videobuf2-memops.o
  AR      drivers/media/platform/msm/vidc/governors/msm-vidc-dyn-gov.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/ife_csid_hw/cam_ife_csid17x.o
  CC      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_dma_hw/jpeg_dma_soc.o
  CC      drivers/media/platform/msm/camera/cam_icp/icp_hw/ipe_hw/ipe_core.o
  AR      drivers/media/platform/msm/vidc/governors/msm-vidc-ar50-dyn-gov.o
  CC      drivers/media/v4l2-core/videobuf2-vmalloc.o
  CC      drivers/media/rc/keymaps/rc-dvbsky.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/ife_csid_hw/cam_ife_csid_lite17x.o
  AR      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_dma_hw/built-in.a
  CC      drivers/media/platform/msm/camera/cam_icp/icp_hw/ipe_hw/ipe_soc.o
  AR      drivers/media/platform/msm/vidc/governors/built-in.a
  CC      drivers/media/rc/keymaps/rc-dvico-mce.o
  CC      drivers/media/v4l2-core/videobuf2-dma-sg.o
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/ife_csid_hw/built-in.a
  AR      drivers/media/platform/msm/vidc/msm-vidc.o
  CC      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_enc_hw/jpeg_enc_dev.o
  CC      drivers/media/rc/keymaps/rc-dvico-portable.o
  AR      drivers/media/platform/msm/camera/cam_icp/icp_hw/ipe_hw/built-in.a
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/cam_vfe_soc.o
  AR      drivers/media/v4l2-core/videodev.o
  AR      drivers/media/platform/msm/camera/cam_icp/icp_hw/built-in.a
  CC      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_enc_hw/jpeg_enc_core.o
  CC      drivers/media/rc/keymaps/rc-em-terratec.o
  AR      drivers/media/platform/msm/vidc/built-in.a
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/cam_vfe_dev.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/cam_isp_hw_mgr.o
  AR      drivers/media/v4l2-core/built-in.a
  CC      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_enc_hw/jpeg_enc_soc.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv2.o
  CC      drivers/media/platform/msm/camera/cam_isp/cam_isp_context.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/cam_vfe_core.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv.o
  CC      drivers/media/platform/msm/camera/cam_icp/cam_icp_context.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv-fm53.o
  CC      drivers/media/platform/msm/camera/cam_jpeg/cam_jpeg_dev.o
  AR      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/jpeg_enc_hw/built-in.a
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe17x/cam_vfe17x.o
  CC      drivers/media/platform/msm/camera/cam_icp/hfi.o
  CC      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/cam_jpeg_hw_mgr.o
  CC      drivers/media/rc/keymaps/rc-evga-indtube.o
  CC      drivers/media/media-device.o
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe17x/built-in.a
  AR      drivers/media/platform/msm/camera/cam_icp/built-in.a
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/cam_ife_hw_mgr.o
  CC      drivers/media/rc/keymaps/rc-eztv.o
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/cam_lrme_hw_dev.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_bus/cam_vfe_bus.o
  AR      drivers/media/platform/msm/camera/cam_jpeg/jpeg_hw/built-in.a
  CC      drivers/media/platform/msm/camera/cam_jpeg/cam_jpeg_context.o
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/cam_lrme_hw_core.o
  CC      drivers/media/rc/keymaps/rc-flydvb.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_bus/cam_vfe_bus_ver2.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_core.o
  CC      drivers/media/platform/msm/camera/cam_lrme/cam_lrme_dev.o
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/cam_lrme_hw_soc.o
  CC      drivers/media/rc/keymaps/rc-flyvideo.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_dev.o
  AR      drivers/media/platform/msm/camera/cam_jpeg/built-in.a
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_bus/cam_vfe_bus_rd_ver1.o
  CC      drivers/media/platform/msm/camera/cam_lrme/cam_lrme_context.o
  CC      drivers/media/rc/keymaps/rc-fusionhdtv-mce.o
  AR      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/built-in.a
  CC      drivers/media/media-devnode.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_util.o
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/cam_lrme_hw_mgr.o
  CC      drivers/media/rc/keymaps/rc-gadmei-rm008z.o
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_bus/built-in.a
  CC      drivers/media/media-entity.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_workq.o
  CC      drivers/media/rc/keymaps/rc-geekbox.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_dev.o
  AR      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/built-in.a
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_mem_mgr.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_top/cam_vfe_camif_lite_ver2.o
  AR      drivers/media/platform/msm/camera/cam_lrme/built-in.a
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_timer.o
  CC      drivers/media/rc/keymaps/rc-genius-tvgo-a11mce.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_core.o
  AR      drivers/media/media.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_debug.o
  CC      drivers/media/rc/keymaps/rc-gotview7135.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_top/cam_vfe_top.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_dev.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_soc.o
  CC      drivers/media/rc/keymaps/rc-hisi-poplar.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_core.o
  AR      drivers/media/platform/msm/camera/cam_req_mgr/built-in.a
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_top/cam_vfe_top_ver2.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_top/cam_vfe_camif_ver2.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_soc.o
  CC      drivers/media/rc/keymaps/rc-hisi-tv-demo.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_soc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_dev.o
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_top/cam_vfe_rdi.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_dev.o
  CC      drivers/media/rc/keymaps/rc-imon-mce.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_core.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/built-in.a
  CC      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_top/cam_vfe_fe_ver1.o
  CC      drivers/media/rc/keymaps/rc-imon-pad.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_core.o
  CC      drivers/media/platform/msm/camera/cam_smmu/cam_smmu_api.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_dev.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_soc.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/built-in.a
  CC      drivers/media/rc/keymaps/rc-imon-rsc.o
  AR      drivers/media/platform/msm/camera/cam_smmu/built-in.a
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/vfe_top/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_core.o
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/vfe_hw/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/built-in.a
  CC      drivers/media/rc/keymaps/rc-iodata-bctv7e.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_soc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_dev.o
  CC      drivers/media/rc/keymaps/rc-it913x-v1.o
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync.o
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/isp_hw/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_dev.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_soc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/cam_res_mgr.o
  CC      drivers/media/rc/keymaps/rc-it913x-v2.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_core.o
  AR      drivers/media/platform/msm/camera/cam_isp/isp_hw_mgr/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_core.o
  AR      drivers/media/platform/msm/camera/cam_sync/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/built-in.a
  CC      drivers/media/rc/keymaps/rc-kaiomy.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_soc.o
  CC      drivers/media/rc/keymaps/rc-kworld-315u.o
  AR      drivers/media/platform/msm/camera/cam_isp/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_dev.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_io.o
  CC      drivers/media/rc/keymaps/rc-kworld-pc150u.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_cci_i2c.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/cam_sensor_util.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_core.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_qup_i2c.o
  CC      drivers/media/rc/keymaps/rc-kworld-plus-tv-analog.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_spi.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_soc.o
  CC      drivers/media/rc/keymaps/rc-leadtek-y04g0051.o
  CC      drivers/media/rc/keymaps/rc-lme2510.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/built-in.a
  CC      drivers/media/rc/keymaps/rc-manli.o
  CC      drivers/media/rc/keymaps/rc-medion-x10.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_soc_util.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/built-in.a
  CC      drivers/media/rc/keymaps/rc-medion-x10-digitainer.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/built-in.a
  CC      drivers/media/rc/keymaps/rc-medion-x10-or2x.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_io_util.o
  CC      drivers/media/rc/keymaps/rc-msi-digivox-ii.o
  CC      drivers/media/rc/keymaps/rc-msi-digivox-iii.o
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere.o
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere-plus.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_packet_util.o
  CC      drivers/media/rc/keymaps/rc-nebula.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_trace.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_common_util.o
  CC      drivers/media/rc/keymaps/rc-nec-terratec-cinergy-xs.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_cx_ipeak.o
  CC      drivers/media/rc/keymaps/rc-norwood.o
  CC      drivers/media/rc/keymaps/rc-npgtech.o
  AR      drivers/media/platform/msm/camera/cam_utils/built-in.a
  CC      drivers/media/rc/keymaps/rc-pctv-sedna.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-color.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-grey.o
  AR      drivers/media/platform/msm/camera/built-in.a
  CC      drivers/media/rc/keymaps/rc-pinnacle-pctv-hd.o
  CC      drivers/media/rc/keymaps/rc-pixelview.o
  CC      drivers/media/rc/keymaps/rc-pixelview-mk12.o
  CC      drivers/media/rc/keymaps/rc-pixelview-002t.o
  CC      drivers/media/rc/keymaps/rc-pixelview-new.o
  CC      drivers/media/rc/keymaps/rc-powercolor-real-angel.o
  CC      drivers/media/rc/keymaps/rc-proteus-2309.o
  CC      drivers/media/rc/keymaps/rc-purpletv.o
  CC      drivers/media/rc/keymaps/rc-pv951.o
  CC      drivers/media/rc/keymaps/rc-hauppauge.o
  CC      drivers/media/rc/keymaps/rc-rc6-mce.o
  CC      drivers/media/rc/keymaps/rc-real-audio-220-32-keys.o
  CC      drivers/media/rc/keymaps/rc-reddo.o
  CC      drivers/media/rc/keymaps/rc-snapstream-firefly.o
  CC      drivers/media/rc/keymaps/rc-streamzap.o
  CC      drivers/media/rc/keymaps/rc-tango.o
  CC      drivers/media/rc/keymaps/rc-tbs-nec.o
  CC      drivers/media/rc/keymaps/rc-technisat-ts35.o
  AR      drivers/media/platform/msm/built-in.a
  CC      drivers/media/rc/keymaps/rc-technisat-usb2.o
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-c-pci.o
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-s2-hd.o
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-xs.o
  CC      drivers/media/rc/keymaps/rc-terratec-slim.o
  CC      drivers/media/rc/keymaps/rc-terratec-slim-2.o
  CC      drivers/media/rc/keymaps/rc-tevii-nec.o
  CC      drivers/media/rc/keymaps/rc-tivo.o
  AR      drivers/media/platform/built-in.a
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
  CC      drivers/media/rc/keymaps/rc-su3000.o
  CC      drivers/media/rc/keymaps/rc-zx-irdec.o
  AR      drivers/media/rc/keymaps/built-in.a
  AR      drivers/media/rc/built-in.a
  AR      drivers/media/built-in.a
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1341: drivers] Error 2
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   8892 / 10614 (83.78%)
  Hits:            5109 /  8892 (57.46%)
    Direct:        5099 /  5109 (99.80%)
    Preprocessed:    10 /  5109 ( 0.20%)
  Misses:          3783 /  8892 (42.54%)
Uncacheable calls: 1722 / 10614 (16.22%)
Local storage:
  Cache size (GB):  0.2 /   4.0 ( 4.61%)
  Hits:            5109 /  8892 (57.46%)
  Misses:          3783 /  8892 (42.54%)
 Kernel build failed (make exit code 2)
```
