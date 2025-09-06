#!/bin/sh

# Navigate to the application directory
cd "$(dirname "$0")/../.."

# Build the application using West
west build -b nucleo_h755zi_q/stm32h755xx/m7 temp_udp_app -d build_sht31 -- -DDTC_OVERLAY_FILE="boards/nucleo_h755zi_q.overlay"

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Build failed!"
    exit 1
fi
