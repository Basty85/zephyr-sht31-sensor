# Zephyr Temperature (SHT31) & UDP Application for Nucleo H755ZI-Q

This repository contains a Zephyr example application that reads temperature and humidity data from an SHT31 sensor and transmits it via UDP over Ethernet. The main purpose of this repository is to serve as a reference on how to structure Zephyr-based applications with sensor integration and network communication.

## Features

- **SHT31 Temperature & Humidity Sensor** support via I2C
- **UDP networking** for data transmission over Ethernet
- **Modern C++ modular design** with custom sensor modules
- **Structured Zephyr application** with proper device tree overlays
- **Simple polling-based approach** for reliable sensor readings

## Hardware Requirements

- **STM32 Nucleo-H755ZI-Q** development board
- **SHT31 Temperature & Humidity Sensor**
- **Ethernet connection** for UDP communication
- Jumper wires for sensor connections

## Development Environment

This project is developed and tested on **Linux**. The setup has been verified on:

- **Ubuntu 22.04 LTS** (recommended)
- Other Linux distributions should work but may require additional package installations

**Note:** While Zephyr supports Windows and macOS, this guide assumes a Linux development environment. For other operating systems, please refer to the [official Zephyr documentation](https://docs.zephyrproject.org/latest/getting_started/index.html).

### Prerequisites

Before getting started, ensure your Linux system has the required packages:

```shell
# Ubuntu/Debian
sudo apt update
sudo apt install git cmake ninja-build gperf ccache dfu-util device-tree-compiler wget python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev

# For flashing via OpenOCD
sudo apt install openocd
```

## Project Structure

This project demonstrates modern C++ modular design in Zephyr:

```
zephyr-sht31-sensor/
├── app/
│   ├── CMakeLists.txt
│   ├── prj.conf                    # Zephyr configuration
│   ├── boards/
│   │   └── nucleo_h755zi_q.overlay # Device tree overlay
│   ├── modules/
│   │   └── sht3xd_reader/          # Custom C++ sensor module
│   │       ├── CMakeLists.txt
│   │       ├── sht3xd_reader.h     # Module interface
│   │       └── sht3xd_reader.cpp   # Module implementation
│   └── src/
│       └── main.cpp                # Application entry point
├── west.yml                        # West manifest for dependencies
└── README.md
```

**Key Design Features:**
- **Modular Architecture**: Sensor functionality encapsulated in reusable C++ modules
- **Clean Interfaces**: Header/implementation separation for maintainability
- **Zephyr Integration**: Proper use of Zephyr APIs and device tree
- **C++ Best Practices**: Modern C++ object-oriented design

## SHT31 Sensor Connection

Connect the SHT31 sensor to the Nucleo board using the following pin mapping:

| SHT31 Pin | Nucleo Pin | Description |
|-----------|------------|-------------|
| VCC       | 3.3V       | Power supply (3.3V) |
| GND       | GND        | Ground |
| SDA       | PB9        | I2C Data Line |
| SCL       | PB8        | I2C Clock Line |

### Wiring Diagram

```
SHT31 Sensor          Nucleo H755ZI-Q
┌─────────────┐       ┌─────────────────┐
│ VCC    ●────┼───────┼─● 3.3V          │
│ GND    ●────┼───────┼─● GND           │
│ SDA    ●────┼───────┼─● PB9 (I2C1_SDA)│
│ SCL    ●────┼───────┼─● PB8 (I2C1_SCL)│
└─────────────┘       └─────────────────┘
```

**Note:** The device tree overlay (`boards/nucleo_h755zi_q.overlay`) configures PB8/PB9 as I2C1 pins with proper pull-up resistors. No external pull-up resistors are required.

## Getting Started

Before getting started, make sure you have a proper Zephyr development environment. Follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).

### Initialization

The first step is to create a workspace folder (e.g. ``my-zephyr-workspace``) where
the ``zephyr-sht31-sensor`` and all Zephyr modules will be cloned. Run the following
commands:

```shell
cd ~
mkdir my-zephyr-workspace
cd my-zephyr-workspace
git clone https://github.com/Basty85/zephyr-sht31-sensor

python3 -m venv .venv
source .venv/bin/activate
pip install west

west init -l zephyr-sht31-sensor
west update
west packages pip --install
```

### Building and running

To build the application, run the following command:

```shell
west build -b nucleo_h755zi_q/stm32h755xx/m7 zephyr-sht31-sensor/temp_udp_app -d build_temp_udp_app -- -DDTC_OVERLAY_FILE="boards/nucleo_h755zi_q.overlay"
```

Once you have built the application, run the following command to flash it:

```shell
west flash --runner openocd --build-dir build_temp_udp_app
```

### Configuration

The application can be configured through the following files:

- **`app/prj.conf`** - Zephyr configuration options (I2C, C++, logging)
- **`app/boards/nucleo_h755zi_q.overlay`** - Device tree overlay for I2C and sensor configuration
- **`app/src/main.cpp`** - Application logic and sensor reading intervals
- **`app/modules/sht3xd_reader/`** - Custom sensor module implementation

### Monitoring Output

Connect to the board via serial console to see sensor readings and debug output:

```shell
# Find the correct serial device (usually /dev/ttyACM0 or /dev/ttyUSB0)
minicom -D /dev/ttyACM0 -b 115200
```

Expected output:
```
*** Booting Zephyr OS build v4.2.0 ***
[00:00:00.021,000] <inf> main: Temp: 22.94 C, Hum: 50.98 %
[00:00:02.033,000] <inf> main: Temp: 22.95 C, Hum: 50.91 %
[00:00:04.045,000] <inf> main: Temp: 22.97 C, Hum: 50.88 %
```

## Troubleshooting

### SHT31 Not Detected
- Check wiring connections (VCC, GND, SDA, SCL)
- Verify 3.3V power supply
- Ensure I2C pull-up resistors are enabled in device tree

### Build Errors
- Make sure all dependencies are installed: `west packages pip --install`
- Clean build directory: `rm -rf build_sht31`
- Check Zephyr version compatibility
- Ensure C++ support is enabled: `CONFIG_CPP=y` in `prj.conf`

### Network Issues
- Verify Ethernet cable connection
- Check network configuration in `prj.conf`
- Ensure DHCP is working or configure static IP

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.
