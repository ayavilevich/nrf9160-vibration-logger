# nRF9160 Vibration Logger

Zephyr (nRF Connect SDK) firmware for a Thingy91 device for it to function as a vibration logger.

Based on ncs\v2.4.0\nrf\samples\nrf9160\nrf_cloud_mqtt_multi_service . See readme.rst for more details.

Additional references: asset_tracker_v2, mqtt (mqtt helper), mqtt_simple (older), coap_client.

## Reference

ADXL362 - low power
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/kconfig/index.html#!CONFIG_ADXL362
https://docs.zephyrproject.org/latest/build/dts/api/bindings/sensor/adi,adxl362.html

ADXL372 - high G, impact sensor
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/kconfig/index.html#!CONFIG_ADXL372
https://docs.zephyrproject.org/latest/build/dts/api/bindings/sensor/adi,adxl372-spi.html

MQTT and MQTT helper
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/kconfig/index.html#!CONFIG_MQTT
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/zephyr/connectivity/networking/api/mqtt.html
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/nrf/libraries/networking/mqtt_helper.html

CoAP and CoAP utils
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/kconfig/index.html#!CONFIG_COAP
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/zephyr/connectivity/networking/api/coap.html
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.0/nrf/libraries/networking/coap_utils.html

## Building

```
west build -b thingy91_nrf9160_ns --build-dir build_thingy91_nrf9160_ns
west build -b nrf9160dk_nrf9160_ns --build-dir build_nrf9160dk_nrf9160_ns
west build -p -b nrf9160dk_nrf9160_ns -- -DSHIELD=nrf7002ek -DDTC_OVERLAY_FILE="nrf9160dk_with_nrf7002ek.overlay" -DOVERLAY_CONFIG="overlay-nrf7002ek-wifi-scan-only.conf"
west build -t menuconfig
west flash --erase
```
