#!/bin/bash
source /opt/venv/bin/activate

cd /workspace

west build -b nucleo_h755zi_q/stm32h755xx/m7 zephyr-sht31-sensor/temp_udp_app -d build_sht31 -- -DDTC_OVERLAY_FILE="boards/nucleo_h755zi_q.overlay"

if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Build failed!"
    exit 1
fi
