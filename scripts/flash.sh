#!/bin/sh

# Flash the compiled firmware to the STM32 Nucleo-H755ZI-Q board

# Set the build directory
BUILD_DIR="build_sht31"

# Check if the build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory '$BUILD_DIR' does not exist. Please build the application first."
    exit 1
fi

# Flash the firmware using west
west flash --runner openocd --build-dir "$BUILD_DIR"
