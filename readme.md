# nRF9160 Vibration Logger

Zephyr (nRF Connect SDK) firmware for a Thingy91 device for it to function as a vibration logger.

Based on ncs\v2.4.0\nrf\samples\nrf9160\nrf_cloud_mqtt_multi_service . See readme.rst for more details.

## Building

```
west build -b thingy91_nrf9160_ns --build-dir build_thingy91_nrf9160_ns
west build -b nrf9160dk_nrf9160_ns --build-dir build_nrf9160dk_nrf9160_ns
west build -p -b nrf9160dk_nrf9160_ns -- -DSHIELD=nrf7002ek -DDTC_OVERLAY_FILE="nrf9160dk_with_nrf7002ek.overlay" -DOVERLAY_CONFIG="overlay-nrf7002ek-wifi-scan-only.conf"
west build -t menuconfig
west flash --erase
```
