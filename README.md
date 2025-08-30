# Zephyr Temperature (SHT31) & UDP Application for Nucleo H755ZI-Q

This repository contains a Zephyr example application that reads temperature and humidity data from an SHT31 sensor and transmits it via UDP over Ethernet. The main purpose of this repository is to serve as a reference on how to structure Zephyr-based applications with sensor integration and network communication.

## Features

- **SHT31 Temperature & Humidity Sensor** support via I2C
- **UDP networking** for data transmission over Ethernet
- **Timer-based sensor readings** with configurable intervals
- **Work queue implementation** for non-blocking sensor operations
- **Structured Zephyr application** with proper device tree overlays

## Hardware Requirements

- **STM32 Nucleo-H755ZI-Q** development board
- **SHT31 Temperature & Humidity Sensor**
- **Ethernet connection** for UDP communication
- Jumper wires for sensor connections

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
the ``zephyr-temp-udp-app-repo`` and all Zephyr modules will be cloned. Run the following
commands:

```shell
cd ~
mkdir my-zephyr-workspace
cd my-zephyr-workspace
git clone https://github.com/your-username/zephyr-temp-udp-app-repo my-manifest-repo

python3 -m venv .venv
source .venv/bin/activate
pip install west

west init -l my-manifest-repo
west update
west packages pip --install
```

### Building and running

To build the application, run the following command:

```shell
west build -b nucleo_h755zi_q/stm32h755xx/m7 my-manifest-repo/app -d build_sht31 -- -DDTC_OVERLAY_FILE="boards/nucleo_h755zi_q.overlay"
```

Once you have built the application, run the following command to flash it:

```shell
west flash --runner openocd --build-dir build_sht31
```

### Configuration

The application can be configured through the following files:

- **`prj.conf`** - Zephyr configuration options (I2C, networking, logging)
- **`boards/nucleo_h755zi_q.overlay`** - Device tree overlay for I2C and sensor configuration
- **`src/main.cpp`** - Application logic and sensor reading intervals

### Monitoring Output

Connect to the board via serial console to see sensor readings and debug output:

```shell
# Find the correct serial device (usually /dev/ttyACM0 or /dev/ttyUSB0)
minicom -D /dev/ttyACM0 -b 115200
```

Expected output:
```
*** Booting Zephyr OS build v4.2.0 ***
[00:00:00.021,000] <inf> main: Starting SHT3x reader...
[00:00:01.027,000] <inf> main: Temp: 22.94 C, Hum: 50.98 %
[00:00:03.033,000] <inf> main: Temp: 22.95 C, Hum: 50.91 %
```

## Troubleshooting

### SHT31 Not Detected
- Check wiring connections (VCC, GND, SDA, SCL)
- Verify 3.3V power supply
- Ensure I2C pull-up resistors are enabled in device tree

### Build Errors
- Make sure all dependencies are installed:
