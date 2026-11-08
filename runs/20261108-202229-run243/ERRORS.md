# Sixfaced CI - FAILED run #243

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42519372040
- ref     : refs/heads/OriginSU
- commit  : 0125e0b73f7af7c56dc52bba57f734917eaf4a6d
- date    : 2026-11-08 20:22:29 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
3127:../drivers/kernelsu/kpm/super_access.c:152:28: error: no member named 'pids' in 'struct task_struct'
3133:../drivers/kernelsu/kpm/super_access.c:152:1: error: no member named 'pids' in 'task_struct'
3145:../drivers/kernelsu/kpm/super_access.c:170:1: error: invalid application of 'sizeof' to an incomplete type 'struct DynamicStructMember[]'
3153:make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/kpm/super_access.o] Error 1
3154:make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
3511:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
3513:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      drivers/media/platform/msm/camera/cam_jpeg/cam_jpeg_dev.o
  CC      drivers/media/platform/msm/vidc/venus_hfi.o
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/cam_lrme_hw_dev.o
  AR      drivers/gpu/drm/msm/built-in.a
  AR      drivers/gpu/drm/built-in.a
  CC      drivers/media/rc/keymaps/rc-dntv-live-dvbt-pro.o
  AR      drivers/gpu/built-in.a
  CC      drivers/media/tuners/mc44s803.o
  CC      drivers/media/platform/msm/camera/cam_jpeg/cam_jpeg_context.o
  AR      drivers/media/usb/b2c2/built-in.a
  CC      drivers/media/rc/keymaps/rc-dtt200u.o
  AR      drivers/media/usb/dvb-usb/built-in.a
  CC      drivers/media/platform/msm/vidc/hfi_response_handler.o
  AR      drivers/media/usb/dvb-usb-v2/built-in.a
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/cam_lrme_hw_core.o
  AR      drivers/media/usb/s2255/built-in.a
  CC      drivers/media/rc/keymaps/rc-dvbsky.o
  AR      drivers/media/tuners/tda18271.o
  AR      drivers/media/usb/siano/built-in.a
  AR      drivers/media/usb/stkwebcam/built-in.a
  AR      drivers/media/tuners/built-in.a
  AR      drivers/media/platform/msm/camera/cam_jpeg/built-in.a
  CC      drivers/media/platform/msm/camera/cam_lrme/cam_lrme_dev.o
  AR      drivers/media/usb/ttusb-budget/built-in.a
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_core.o
  AR      drivers/media/usb/ttusb-dec/built-in.a
  CC      drivers/media/platform/msm/vidc/hfi_packetization.o
  CC      drivers/media/usb/uvc/uvc_driver.o
  CC      drivers/media/rc/keymaps/rc-dvico-mce.o
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/cam_lrme_hw_soc.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_dev.o
  CC      drivers/media/rc/rc-ir-raw.o
  CC      drivers/media/usb/uvc/uvc_queue.o
  CC      drivers/media/platform/msm/vidc/vidc_hfi.o
  CC      drivers/media/rc/keymaps/rc-dvico-portable.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_util.o
  CC      drivers/media/usb/uvc/uvc_v4l2.o
  AR      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/lrme_hw/built-in.a
  AR      drivers/media/usb/zr364xx/built-in.a
  CC      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/cam_lrme_hw_mgr.o
  CC      drivers/media/v4l2-core/v4l2-dev.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_workq.o
  CC      drivers/media/rc/keymaps/rc-em-terratec.o
  CC      drivers/media/platform/msm/vidc/venus_boot.o
  CC      drivers/media/usb/uvc/uvc_video.o
  CC      drivers/media/v4l2-core/v4l2-ioctl.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv2.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_mem_mgr.o
  AR      drivers/media/platform/msm/camera/cam_lrme/lrme_hw_mgr/built-in.a
  CC      drivers/media/platform/msm/camera/cam_lrme/cam_lrme_context.o
  CC      drivers/media/usb/uvc/uvc_ctrl.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv.o
  CC      drivers/media/platform/msm/vidc/msm_vidc_clocks.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_timer.o
  AR      drivers/media/platform/msm/camera/cam_lrme/built-in.a
  CC      drivers/media/v4l2-core/v4l2-device.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_dev.o
  CC      drivers/media/usb/uvc/uvc_status.o
  CC      drivers/media/platform/msm/camera/cam_req_mgr/cam_req_mgr_debug.o
  CC      drivers/media/v4l2-core/v4l2-fh.o
  CC      drivers/media/rc/keymaps/rc-encore-enltv-fm53.o
  CC      drivers/media/platform/msm/vidc/governors/msm_vidc_dyn_gov.o
  CC      drivers/media/usb/uvc/uvc_isight.o
  AR      drivers/media/platform/msm/camera/cam_req_mgr/built-in.a
  AR      drivers/media/rc/rc-core.o
  CC      drivers/media/rc/keymaps/rc-evga-indtube.o
  AR      drivers/media/v4l2loopback-master/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_core.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_dev.o
  CC      drivers/media/platform/msm/vidc/governors/msm_vidc_ar50_dyn_gov.o
  CC      drivers/media/v4l2-core/v4l2-event.o
  CC      drivers/media/rc/keymaps/rc-eztv.o
  CC      drivers/media/usb/uvc/uvc_entity.o
  CC      drivers/media/v4l2-core/v4l2-ctrls.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_core.o
  AR      drivers/media/usb/uvc/uvcvideo.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_soc.o
  AR      drivers/media/usb/uvc/built-in.a
  AR      drivers/media/usb/built-in.a
  CC      drivers/media/rc/keymaps/rc-flydvb.o
  CC      drivers/media/v4l2-core/v4l2-subdev.o
  AR      drivers/media/platform/msm/vidc/governors/msm-vidc-dyn-gov.o
  AR      drivers/media/platform/msm/vidc/governors/msm-vidc-ar50-dyn-gov.o
  CC      drivers/media/media-device.o
  AR      drivers/media/platform/msm/vidc/governors/built-in.a
  AR      drivers/media/platform/msm/vidc/msm-vidc.o
  AR      drivers/media/platform/msm/vidc/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_soc.o
  CC      drivers/media/platform/msm/camera/cam_smmu/cam_smmu_api.o
  CC      drivers/media/rc/keymaps/rc-flyvideo.o
  CC      drivers/media/v4l2-core/v4l2-clk.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/built-in.a
  CC      drivers/media/media-devnode.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_soc.o
  CC      drivers/media/rc/keymaps/rc-fusionhdtv-mce.o
  AR      drivers/media/platform/msm/camera/cam_smmu/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync_util.o
  CC      drivers/media/v4l2-core/v4l2-async.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_soc_util.o
  CC      drivers/media/rc/keymaps/rc-gadmei-rm008z.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_dev.o
  CC      drivers/media/rc/keymaps/rc-geekbox.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_dev.o
  AR      drivers/media/platform/msm/camera/cam_sync/built-in.a
  CC      drivers/media/platform/msm/camera/cam_utils/cam_io_util.o
  CC      drivers/media/v4l2-core/v4l2-compat-ioctl32.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_core.o
  CC      drivers/media/rc/keymaps/rc-genius-tvgo-a11mce.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_packet_util.o
  CC      drivers/media/v4l2-core/vb2-trace.o
  CC      drivers/media/media-entity.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_core.o
  CC      drivers/media/rc/keymaps/rc-gotview7135.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_dev.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_trace.o
  AR      drivers/media/media.o
  CC      drivers/media/rc/keymaps/rc-hisi-poplar.o
  CC      drivers/media/v4l2-core/v4l2-trace.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_common_util.o
  CC      drivers/media/rc/keymaps/rc-hisi-tv-demo.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_soc.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_cx_ipeak.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_core.o
  CC      drivers/media/rc/keymaps/rc-imon-mce.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_dev.o
  AR      drivers/media/platform/msm/camera/cam_utils/built-in.a
  CC      drivers/media/v4l2-core/v4l2-mc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_soc.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_dev.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_soc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_core.o
  CC      drivers/media/rc/keymaps/rc-imon-pad.o
  CC      drivers/media/v4l2-core/v4l2-common.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/cam_res_mgr.o
  CC      drivers/media/rc/keymaps/rc-imon-rsc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_core.o
  CC      drivers/media/rc/keymaps/rc-iodata-bctv7e.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/built-in.a
  CC      drivers/media/v4l2-core/v4l2-dv-timings.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_soc.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/built-in.a
  CC      drivers/media/rc/keymaps/rc-it913x-v1.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_dev.o
  CC      drivers/media/v4l2-core/v4l2-mem2mem.o
  CC      drivers/media/rc/keymaps/rc-it913x-v2.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_core.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/built-in.a
  CC      drivers/media/rc/keymaps/rc-kaiomy.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_io.o
  CC      drivers/media/v4l2-core/videobuf2-core.o
  CC      drivers/media/v4l2-core/videobuf2-v4l2.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_soc.o
  CC      drivers/media/rc/keymaps/rc-kworld-315u.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_cci_i2c.o
  CC      drivers/media/v4l2-core/videobuf2-memops.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/cam_sensor_util.o
  CC      drivers/media/rc/keymaps/rc-kworld-pc150u.o
  CC      drivers/media/rc/keymaps/rc-kworld-plus-tv-analog.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/built-in.a
  CC      drivers/media/v4l2-core/videobuf2-vmalloc.o
  CC      drivers/media/rc/keymaps/rc-leadtek-y04g0051.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/built-in.a
  CC      drivers/media/v4l2-core/videobuf2-dma-sg.o
  CC      drivers/media/rc/keymaps/rc-lme2510.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_qup_i2c.o
  AR      drivers/media/v4l2-core/videodev.o
  CC      drivers/media/rc/keymaps/rc-manli.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_spi.o
  CC      drivers/media/rc/keymaps/rc-medion-x10.o
  AR      drivers/media/v4l2-core/built-in.a
  CC      drivers/media/rc/keymaps/rc-medion-x10-digitainer.o
  CC      drivers/media/rc/keymaps/rc-medion-x10-or2x.o
  CC      drivers/media/rc/keymaps/rc-msi-digivox-ii.o
  CC      drivers/media/rc/keymaps/rc-msi-digivox-iii.o
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere.o
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere-plus.o
  CC      drivers/media/rc/keymaps/rc-nebula.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/built-in.a
  CC      drivers/media/rc/keymaps/rc-nec-terratec-cinergy-xs.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/built-in.a
  CC      drivers/media/rc/keymaps/rc-norwood.o
  CC      drivers/media/rc/keymaps/rc-npgtech.o
  AR      drivers/media/platform/msm/camera/built-in.a
  AR      drivers/media/platform/msm/built-in.a
  CC      drivers/media/rc/keymaps/rc-pctv-sedna.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-color.o
  AR      drivers/media/platform/built-in.a
  CC      drivers/media/rc/keymaps/rc-pinnacle-grey.o
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
  CC      drivers/media/rc/keymaps/rc-su3000.o
  CC      drivers/media/rc/keymaps/rc-zx-irdec.o
  AR      drivers/media/rc/keymaps/built-in.a
  AR      drivers/media/rc/built-in.a
  AR      drivers/media/built-in.a
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
make[1]: Leaving directory '/home/runner/work/Kernel/Kernel/out'
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   38342 / 44052 (87.04%)
  Hits:            29607 / 38342 (77.22%)
    Direct:        20631 / 29607 (69.68%)
    Preprocessed:   8976 / 29607 (30.32%)
  Misses:           8735 / 38342 (22.78%)
Uncacheable calls:  5704 / 44052 (12.95%)
Errors:                6 / 44052 ( 0.01%)
Local storage:
  Cache size (GB):   0.5 /   4.0 (12.45%)
  Hits:            29607 / 38342 (77.22%)
  Misses:           8735 / 38342 (22.78%)
 Kernel build failed (make exit code 2)
```
