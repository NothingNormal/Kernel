# Sixfaced CI - FAILED run #244

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/42519971545
- ref     : refs/heads/OriginSU
- commit  : 1d17165cde64930b39e233fb069f7584a18e29a6
- date    : 2026-11-08 20:28:18 UTC
- clang   : Neutron clang version 24.0.0git (https://github.com/llvm/llvm-project.git f42c87ba2c70219b1f4ad1f031a5b6a1ea6603fb)

## Extracted errors (last 300 matches)
```
3102:../drivers/kernelsu/policy/su_request.c:201:9: error: type specifier missing, defaults to 'int' [-Werror,-Wimplicit-int]
3111:../drivers/kernelsu/policy/su_request.c:201:9: error: a parameter list without types is only allowed in a function definition
3118:../drivers/kernelsu/policy/su_request.c:201:9: error: function declared in block scope cannot have 'static' storage class
3125:../drivers/kernelsu/policy/su_request.c:201:9: error: use of undeclared identifier '_rs'
3138:make[3]: *** [../scripts/Makefile.build:364: drivers/kernelsu/policy/su_request.o] Error 1
3139:make[2]: *** [../scripts/Makefile.build:669: drivers/kernelsu] Error 2
3640:make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
3642:make: *** [Makefile:151: sub-make] Error 2
```

## Tail of build log (last 250 lines)
```
  CC      drivers/media/rc/rc-main.o
  CC      drivers/media/v4l2-core/v4l2-event.o
  CC      drivers/media/usb/uvc/uvc_entity.o
  CC      drivers/media/rc/keymaps/rc-genius-tvgo-a11mce.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_dev.o
  CC      drivers/media/v4l2-core/v4l2-ctrls.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_dev.o
  AR      drivers/media/usb/uvc/uvcvideo.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_core.o
  AR      drivers/media/usb/uvc/built-in.a
  CC      drivers/media/rc/keymaps/rc-gotview7135.o
  CC      drivers/media/v4l2-core/v4l2-subdev.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_core.o
  AR      drivers/media/usb/zr364xx/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/cam_actuator_soc.o
  CC      drivers/media/rc/keymaps/rc-hisi-poplar.o
  AR      drivers/media/usb/built-in.a
  CC      drivers/media/v4l2-core/v4l2-clk.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/cam_cci_soc.o
  CC      drivers/media/rc/keymaps/rc-hisi-tv-demo.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_actuator/built-in.a
  CC      drivers/media/v4l2-core/v4l2-async.o
  CC      drivers/net/ppp/pppopns.o
  CC      drivers/media/platform/msm/camera/cam_smmu/cam_smmu_api.o
  CC      drivers/media/rc/keymaps/rc-imon-mce.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_cci/built-in.a
  CC      drivers/media/v4l2-core/v4l2-compat-ioctl32.o
  AR      drivers/media/platform/msm/camera/cam_smmu/built-in.a
  CC      drivers/media/rc/keymaps/rc-imon-pad.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_soc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_dev.o
  CC      drivers/media/rc/keymaps/rc-imon-rsc.o
  CC      drivers/media/v4l2-core/vb2-trace.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_core.o
  CC      drivers/net/slip/slhc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_dev.o
  CC      drivers/media/v4l2-core/v4l2-trace.o
  CC      drivers/media/rc/keymaps/rc-iodata-bctv7e.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/cam_eeprom_soc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/cam_csiphy_core.o
  CC      drivers/media/v4l2-core/v4l2-mc.o
  CC      drivers/media/rc/keymaps/rc-it913x-v1.o
  CC      drivers/media/rc/keymaps/rc-it913x-v2.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_csiphy/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_eeprom/built-in.a
  CC      drivers/media/rc/rc-ir-raw.o
  CC      drivers/media/v4l2-core/v4l2-common.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_dev.o
  AR      drivers/media/rc/rc-core.o
  CC      drivers/media/rc/keymaps/rc-kaiomy.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_core.o
  CC      drivers/media/v4l2-core/v4l2-dv-timings.o
  CC      drivers/media/rc/keymaps/rc-kworld-315u.o
  CC      drivers/media/v4l2-core/v4l2-mem2mem.o
  CC      drivers/media/rc/keymaps/rc-kworld-pc150u.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/cam_flash_soc.o
  CC      drivers/media/v4l2-core/videobuf2-core.o
  CC      drivers/net/usb/r8152.o
  CC      drivers/media/rc/keymaps/rc-kworld-plus-tv-analog.o
  AR      drivers/net/ppp/built-in.a
  CC      drivers/media/v4l2-core/videobuf2-v4l2.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_flash/built-in.a
  AR      drivers/media/v4l2loopback-master/built-in.a
  CC      drivers/media/rc/keymaps/rc-leadtek-y04g0051.o
  CC      drivers/media/v4l2-core/videobuf2-memops.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_dev.o
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync.o
  CC      drivers/media/rc/keymaps/rc-lme2510.o
  CC      drivers/media/v4l2-core/videobuf2-vmalloc.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_soc.o
  CC      drivers/media/platform/msm/camera/cam_sync/cam_sync_util.o
  CC      drivers/media/v4l2-core/videobuf2-dma-sg.o
  CC      drivers/media/rc/keymaps/rc-manli.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/cam_ir_led_core.o
  AR      drivers/media/platform/msm/camera/cam_sync/built-in.a
  CC      drivers/media/rc/keymaps/rc-medion-x10.o
  AR      drivers/net/slip/built-in.a
  AR      drivers/media/v4l2-core/videodev.o
  CC      drivers/net/usb/asix_devices.o
  CC      drivers/media/rc/keymaps/rc-medion-x10-digitainer.o
  AR      drivers/media/v4l2-core/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ir_led/built-in.a
  CC      drivers/media/rc/keymaps/rc-medion-x10-or2x.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_dev.o
  CC      drivers/net/wireguard/main.o
  CC      drivers/media/rc/keymaps/rc-msi-digivox-ii.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_core.o
  CC      drivers/net/usb/asix_common.o
  CC      drivers/media/rc/keymaps/rc-msi-digivox-iii.o
  AR      drivers/net/wireless/admtek/built-in.a
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/cam_ois_soc.o
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere.o
  AR      drivers/net/wireless/ath/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_ois/built-in.a
  CC      drivers/media/rc/keymaps/rc-msi-tvanywhere-plus.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/cam_res_mgr.o
  AR      drivers/net/wireless/atmel/built-in.a
  AR      drivers/net/wireless/broadcom/built-in.a
  CC      drivers/media/rc/keymaps/rc-nebula.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_res_mgr/built-in.a
  AR      drivers/net/wireless/cisco/built-in.a
  CC      drivers/net/usb/ax88172a.o
  CC      drivers/media/rc/keymaps/rc-nec-terratec-cinergy-xs.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_dev.o
  CC      drivers/media/rc/keymaps/rc-norwood.o
  CC      drivers/net/wireless/cnss_genl/cnss_nl.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_core.o
  CC      drivers/media/rc/keymaps/rc-npgtech.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/cam_sensor_soc.o
  CC      drivers/media/rc/keymaps/rc-pctv-sedna.o
  CC      drivers/media/media-device.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-color.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor/built-in.a
  CC      drivers/media/rc/keymaps/rc-pinnacle-grey.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_io.o
  CC      drivers/net/wireguard/noise.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_soc_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_cci_i2c.o
  CC      drivers/media/rc/keymaps/rc-pinnacle-pctv-hd.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_io_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_qup_i2c.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_packet_util.o
  CC      drivers/media/rc/keymaps/rc-pixelview.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/cam_sensor_spi.o
  AR      drivers/net/wireless/cnss_genl/built-in.a
  CC      drivers/net/usb/ax88179_178a.o
  CC      drivers/media/rc/keymaps/rc-pixelview-mk12.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_trace.o
  CC      drivers/net/wireless/cnss_prealloc/cnss_prealloc.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_io/built-in.a
  CC      drivers/media/platform/msm/camera/cam_utils/cam_common_util.o
  CC      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/cam_sensor_util.o
  CC      drivers/media/rc/keymaps/rc-pixelview-002t.o
  CC      drivers/media/platform/msm/camera/cam_utils/cam_cx_ipeak.o
  AR      drivers/media/platform/msm/camera/cam_sensor_module/cam_sensor_utils/built-in.a
  AR      drivers/media/platform/msm/camera/cam_sensor_module/built-in.a
  CC      drivers/media/rc/keymaps/rc-pixelview-new.o
  AR      drivers/media/platform/msm/camera/cam_utils/built-in.a
  CC      drivers/net/dummy.o
  AR      drivers/media/platform/msm/camera/built-in.a
  CC      drivers/media/rc/keymaps/rc-powercolor-real-angel.o
  CC      drivers/media/rc/keymaps/rc-proteus-2309.o
  AR      drivers/media/platform/msm/built-in.a
  CC      drivers/media/rc/keymaps/rc-purpletv.o
  CC      drivers/net/wireguard/device.o
  CC      drivers/media/rc/keymaps/rc-pv951.o
  AR      drivers/media/platform/built-in.a
  CC      drivers/net/usb/cdc_ether.o
  CC      drivers/media/rc/keymaps/rc-hauppauge.o
  AR      drivers/net/wireless/cnss_prealloc/built-in.a
  CC      drivers/net/wireless/cnss_utils/cnss_utils.o
  CC      drivers/net/macvlan.o
  CC      drivers/media/rc/keymaps/rc-rc6-mce.o
  CC      drivers/media/rc/keymaps/rc-real-audio-220-32-keys.o
  CC      drivers/media/rc/keymaps/rc-reddo.o
  CC      drivers/media/rc/keymaps/rc-snapstream-firefly.o
  CC      drivers/net/wireguard/peer.o
  CC      drivers/media/rc/keymaps/rc-streamzap.o
  CC      drivers/media/rc/keymaps/rc-tango.o
  CC      drivers/media/rc/keymaps/rc-tbs-nec.o
  CC      drivers/media/media-devnode.o
  CC      drivers/net/usb/net1080.o
  CC      drivers/media/rc/keymaps/rc-technisat-ts35.o
  CC      drivers/net/mii.o
  CC      drivers/media/media-entity.o
  CC      drivers/media/rc/keymaps/rc-technisat-usb2.o
  AR      drivers/net/wireless/cnss_utils/built-in.a
  AR      drivers/media/media.o
  AR      drivers/net/wireless/intel/built-in.a
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-c-pci.o
  CC      drivers/net/wireguard/timers.o
  AR      drivers/net/wireless/intersil/built-in.a
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-s2-hd.o
  AR      drivers/net/wireless/marvell/built-in.a
  CC      drivers/media/rc/keymaps/rc-terratec-cinergy-xs.o
  AR      drivers/net/wireless/mediatek/built-in.a
  AR      drivers/net/wireless/quantenna/built-in.a
  CC      drivers/media/rc/keymaps/rc-terratec-slim.o
  AR      drivers/net/wireless/ralink/built-in.a
  CC      drivers/media/rc/keymaps/rc-terratec-slim-2.o
  CC      drivers/net/wireguard/queueing.o
  AR      drivers/net/wireless/realtek/built-in.a
  CC      drivers/media/rc/keymaps/rc-tevii-nec.o
  AR      drivers/net/wireless/rsi/built-in.a
  CC      drivers/net/Space.o
  CC      drivers/net/usb/cdc_subset.o
  CC      drivers/media/rc/keymaps/rc-tivo.o
  AR      drivers/net/wireless/st/built-in.a
  AR      drivers/net/wireless/ti/built-in.a
  CC      drivers/media/rc/keymaps/rc-total-media-in-hand.o
  AR      drivers/net/wireless/zydas/built-in.a
  AR      drivers/net/wireless/built-in.a
  CC      drivers/media/rc/keymaps/rc-total-media-in-hand-02.o
  CC      drivers/media/rc/keymaps/rc-trekstor.o
  CC      drivers/media/rc/keymaps/rc-tt-1500.o
  CC      drivers/net/wireguard/send.o
  CC      drivers/net/wireguard/receive.o
  CC      drivers/media/rc/keymaps/rc-twinhan-dtv-cab-ci.o
  CC      drivers/net/usb/zaurus.o
  CC      drivers/media/rc/keymaps/rc-twinhan1027.o
  CC      drivers/net/wireguard/socket.o
  CC      drivers/media/rc/keymaps/rc-videomate-m1f.o
  CC      drivers/media/rc/keymaps/rc-videomate-s350.o
  CC      drivers/net/loopback.o
  CC      drivers/media/rc/keymaps/rc-videomate-tv-pvr.o
  CC      drivers/media/rc/keymaps/rc-winfast.o
  CC      drivers/media/rc/keymaps/rc-winfast-usbii-deluxe.o
  CC      drivers/media/rc/keymaps/rc-su3000.o
  CC      drivers/net/wireguard/peerlookup.o
  CC      drivers/net/usb/usbnet.o
  CC      drivers/net/wireguard/allowedips.o
  CC      drivers/media/rc/keymaps/rc-zx-irdec.o
  CC      drivers/net/usb/cdc_ncm.o
  CC      drivers/net/tun.o
  AR      drivers/media/rc/keymaps/built-in.a
  AR      drivers/media/rc/built-in.a
  AR      drivers/media/built-in.a
  AR      drivers/net/usb/asix.o
  CC      drivers/net/veth.o
  CC      drivers/net/wireguard/ratelimiter.o
  CC      drivers/net/wireguard/cookie.o
  CC      drivers/net/wireguard/netlink.o
  AR      drivers/net/usb/built-in.a
  CC      drivers/net/wireguard/crypto/zinc/chacha20/chacha20.o
  PERLASM drivers/net/wireguard/crypto/zinc/chacha20/chacha20-arm64.S
  CC      drivers/net/wireguard/crypto/zinc/poly1305/poly1305.o
  PERLASM drivers/net/wireguard/crypto/zinc/poly1305/poly1305-arm64.S
  CC      drivers/net/wireguard/crypto/zinc/chacha20poly1305.o
  CC      drivers/net/wireguard/crypto/zinc/blake2s/blake2s.o
  CC      drivers/net/wireguard/crypto/zinc/curve25519/curve25519.o
  AS      drivers/net/wireguard/crypto/zinc/chacha20/chacha20-arm64.o
  AS      drivers/net/wireguard/crypto/zinc/poly1305/poly1305-arm64.o
  AR      drivers/net/wireguard/wireguard.o
  AR      drivers/net/wireguard/built-in.a
  AR      drivers/net/built-in.a
make[1]: *** [/home/runner/work/Kernel/Kernel/Makefile:1330: drivers] Error 2
make[1]: Leaving directory '/home/runner/work/Kernel/Kernel/out'
make: *** [Makefile:151: sub-make] Error 2
Cacheable calls:   41148 / 47079 (87.40%)
  Hits:            32408 / 41148 (78.76%)
    Direct:        23350 / 32408 (72.05%)
    Preprocessed:   9058 / 32408 (27.95%)
  Misses:           8740 / 41148 (21.24%)
Uncacheable calls:  5922 / 47079 (12.58%)
Errors:                9 / 47079 ( 0.02%)
Local storage:
  Cache size (GB):   0.5 /   4.0 (12.46%)
  Hits:            32408 / 41148 (78.76%)
  Misses:           8740 / 41148 (21.24%)
 Kernel build failed (make exit code 2)
```
