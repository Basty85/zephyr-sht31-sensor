# Zephyr SHT31 Sensor UDP Transmitter for STM32 Nucleo-H755ZI-Q

This repository contains a professional Zephyr RTOS application that reads temperature and humidity data from an SHT31 sensor and transmits it via UDP over Ethernet. The project demonstrates modern C++ design patterns, modular architecture, and embedded networking best practices.

## 🌟 Features

- **SHT31 Temperature & Humidity Sensor** integration via I2C interface
- **UDP networking** for real-time sensor data transmission over Ethernet
- **Modern C++ modular design** with clean separation of concerns
- **Professional documentation** with Doxygen-style comments
- **Remote Telnet console** for system monitoring and diagnostics
- **Network statistics** and debugging capabilities
- **High-precision sensor readings** with efficient network transmission
- **Static IP configuration** for reliable network deployment

## 🔧 Hardware Requirements

- **STM32 Nucleo-H755ZI-Q** development board (Cortex-M7 @ 480MHz)
- **SHT31 Temperature & Humidity Sensor** (I2C interface)
- **Ethernet connection** for UDP communication and Telnet access
- Jumper wires for sensor connections

## 💻 Development Environment

This project is developed and tested on **Linux**. The setup has been verified on:

- **Ubuntu 22.04 LTS** (recommended)
- Other Linux distributions should work but may require additional package installations

**Note:** While Zephyr supports Windows and macOS, this guide assumes a Linux development environment. For other operating systems, please refer to the [official Zephyr documentation](https://docs.zephyrproject.org/latest/getting_started/index.html).

### Prerequisites

Before getting started, ensure your Linux system has the required packages:

```shell
# Ubuntu/Debian
sudo apt update
sudo apt install git cmake ninja-build gperf ccache dfu-util device-tree-compiler wget \
    python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
    make gcc gcc-multilib g++-multilib libsdl2-dev

# For flashing via OpenOCD
sudo apt install openocd

# For network debugging (optional)
sudo apt install wireshark telnet minicom
```

## 🏗️ Project Architecture

This project demonstrates professional embedded software architecture with clean modularity:

```
zephyr-sht31-sensor/
├── temp_udp_app/
│   ├── CMakeLists.txt
│   ├── prj.conf                    # Comprehensive Zephyr configuration
│   ├── VERSION
│   ├── boards/
│   │   └── nucleo_h755zi_q.overlay # Device tree overlay for I2C and Ethernet
│   ├── modules/                    # Modular C++ components
│   │   ├── sht3xd_reader/          # SHT31 sensor abstraction layer
│   │   │   ├── sht3xd_reader.h     # Low-level sensor interface
│   │   │   ├── sht3xd_reader.cpp   # Sensor driver implementation
│   │   │   └── sensor_handler.h    # High-level sensor data management
│   │   └── udp_client/             # Network communication layer
│   │       ├── udp_client.h        # UDP client interface
│   │       └── udp_client.cpp      # Network transmission implementation
│   └── src/
│       └── main.cpp                # Application orchestration and main loop
├── python_receiver/
│   └── simple_receiver.py          # Python UDP receiver for testing
├── west.yml                        # West manifest for dependencies
├── CMakeLists.txt
├── LICENSE
└── README.md
```

### 🎯 Key Design Principles

- **Separation of Concerns**: Each module has a single, well-defined responsibility
- **Clean Interfaces**: Header/implementation separation for maintainability
- **Data Flow Architecture**: `SensorHandler` → `SensorData` → `NetworkPacket` → `UdpClient`
- **Professional Documentation**: Doxygen-style comments throughout
- **Error Handling**: Comprehensive error checking and logging
- **Resource Management**: Proper RAII and cleanup patterns

## 🔌 Hardware Connection

Connect the SHT31 sensor to the Nucleo board using the following pin mapping:

| SHT31 Pin | Nucleo Pin | Function | Description |
|-----------|------------|----------|-------------|
| VCC       | 3.3V       | Power    | Power supply (3.3V) |
| GND       | GND        | Ground   | Ground connection |
| SDA       | PB9        | I2C Data | I2C Data Line (with internal pull-up) |
| SCL       | PB8        | I2C Clock| I2C Clock Line (with internal pull-up) |

### Wiring Diagram

```
SHT31 Sensor          STM32 Nucleo-H755ZI-Q
┌─────────────┐       ┌─────────────────────────┐
│ VCC    ●────┼───────┼─● 3.3V                  │
│ GND    ●────┼───────┼─● GND                   │
│ SDA    ●────┼───────┼─● PB9 (I2C1_SDA)       │
│ SCL    ●────┼───────┼─● PB8 (I2C1_SCL)       │
└─────────────┘       └─────────────────────────┘
```

**Note:** The device tree overlay configures internal pull-up resistors, so no external pull-ups are required.

## 🌐 Network Configuration

The application uses static IP configuration for reliable deployment:

| Parameter | Value | Description |
|-----------|-------|-------------|
| Board IP | 192.168.1.38 | Static IP address of the Nucleo board |
| Netmask | 255.255.255.0 | Subnet mask |
| Gateway | 192.168.1.1 | Default gateway |
| UDP Target | 192.168.1.37:8888 | Target server for sensor data |
| Telnet Port | 23 | Remote console access |

## 🚀 Getting Started

### Step 1: Environment Setup

Create a workspace and clone the repository:

```shell
cd ~
mkdir my-zephyr-workspace
cd my-zephyr-workspace
git clone https://github.com/Basty85/zephyr-sht31-sensor

# Create Python virtual environment
python3 -m venv .venv
source .venv/bin/activate
pip install west

# Initialize West workspace
west init -l zephyr-sht31-sensor
west update
west packages pip --install
```

### Step 2: Build Application

Build the application with the following command:

```shell
west build -b nucleo_h755zi_q/stm32h755xx/m7 zephyr-sht31-sensor/temp_udp_app -d build_temp_udp_app -- -DDTC_OVERLAY_FILE="boards/nucleo_h755zi_q.overlay"
```

### Step 3: Flash to Hardware

Flash the compiled firmware to the Nucleo board:

```shell
west flash --runner openocd --build-dir build_temp_udp_app
```

## 📊 Monitoring and Debugging

### UART Console (Local)

Connect via serial to monitor sensor readings and system logs:

```shell
# Find the correct device (usually /dev/ttyACM0)
minicom -D /dev/ttyACM0 -b 115200
```

Expected output:
```
*** Booting Zephyr OS build v4.2.0 ***
[00:00:03.021,000] <inf> main: === SHT31 Sensor UDP Transmitter ===
[00:00:03.027,000] <inf> main: Using SensorHandler with SensorData structure
[00:00:03.034,000] <inf> main: Target server: 192.168.1.37:8888
[00:00:06.042,000] <inf> main: Starting sensor data transmission loop
[00:00:06.049,000] <inf> main: Sensor readings: 23.45 deg, 51.20 %
[00:00:06.056,000] <inf> main: UDP transmitted: 23.45 deg, 51.20 % [6056 ms]
```

### Remote Telnet Console

Access the system remotely via Telnet for network diagnostics:

```shell
# Connect to Telnet console
telnet 192.168.1.38
```

Available commands in Telnet console:
```shell
nucleo-eth:~$ help               # Show all available commands
nucleo-eth:~$ net stats          # Network statistics
nucleo-eth:~$ net conn           # Active connections
nucleo-eth:~$ net iface          # Network interface info
nucleo-eth:~$ net ping 192.168.1.37  # Test connectivity
nucleo-eth:~$ kernel uptime      # System uptime
nucleo-eth:~$ device list        # Hardware devices
```

To exit Telnet:
```shell
# Press Ctrl+] then type:
quit
```

### UDP Data Reception

Use the included Python receiver to monitor sensor data:

```shell
cd zephyr-sht31-sensor/python_receiver
python3 simple_receiver.py
```

## 🔧 Configuration Options

### Application Configuration (`prj.conf`)

Key configuration sections:

```ini
# Core system
CONFIG_LOG_DEFAULT_LEVEL=3           # INFO level logging
CONFIG_CPP=y                         # C++ support
CONFIG_SENSOR=y                      # Sensor subsystem
CONFIG_I2C=y                         # I2C bus support

# Networking
CONFIG_NETWORKING=y                  # Network stack
CONFIG_NET_UDP=y                     # UDP protocol
CONFIG_NET_CONFIG_MY_IPV4_ADDR="192.168.1.38"  # Static IP

# Shell and monitoring
CONFIG_SHELL_BACKEND_TELNET=y        # Telnet console
CONFIG_NET_STATISTICS=y              # Network statistics
```

### Device Tree Overlay (`boards/nucleo_h755zi_q.overlay`)

Hardware configuration for I2C and sensor:

```dts
&i2c1 {
    status = "okay";
    sht3xd@44 {
        compatible = "sensirion,sht3xd";
        reg = <0x44>;
    };
};
```

## 🧪 Testing and Validation

### 1. Sensor Functionality Test

```shell
# Via Telnet console
nucleo-eth:~$ device list
# Should show SHT31 sensor as ready
```

### 2. Network Connectivity Test

```shell
# From development machine
ping 192.168.1.38

# Via Telnet console
nucleo-eth:~$ net ping 192.168.1.37
```

### 3. UDP Data Transmission Test

```shell
# Start Python receiver
python3 python_receiver/simple_receiver.py

# Should receive sensor data every second
```

## 🐛 Troubleshooting

### Sensor Issues

**SHT31 not detected:**
- Verify wiring connections (VCC=3.3V, GND, SDA=PB9, SCL=PB8)
- Check I2C bus status: `nucleo-eth:~$ device list`
- Ensure sensor has proper power supply

**Sensor read failures:**
- Check I2C timing in device tree overlay
- Verify sensor is not damaged (try different sensor)
- Monitor logs for specific I2C error codes

### Network Issues

**No network connectivity:**
- Verify Ethernet cable connection
- Check DHCP/static IP configuration in `prj.conf`
- Test with: `nucleo-eth:~$ net iface`

**UDP transmission failures:**
- Verify target server is reachable: `nucleo-eth:~$ net ping 192.168.1.37`
- Check firewall settings on target machine
- Monitor network with Wireshark for UDP packets

**Telnet console not accessible:**
- Verify IP address configuration
- Check if port 23 is blocked by firewall
- Test with: `telnet 192.168.1.38`

### Build Issues

**Compilation errors:**
```shell
# Clean rebuild
rm -rf build_sht31
west build -b nucleo_h755zi_q/stm32h755xx/m7 zephyr-sht31-sensor/temp_udp_app -d build_sht31
```

**Dependencies missing:**
```shell
# Reinstall dependencies
west packages pip --install
```

## 📈 Performance Metrics

- **Sampling Rate**: 1 Hz (configurable)
- **Network Latency**: < 10ms (local network)
- **Memory Usage**: ~50KB RAM, ~200KB Flash
- **Power Consumption**: ~200mA @ 3.3V (with Ethernet active)
- **Temperature Accuracy**: ±0.3°C (sensor specification)
- **Humidity Accuracy**: ±2% RH (sensor specification)

## 🔮 Future Enhancements

- **Multiple sensor support** (SHT30, SHT35)
- **JSON data format** for better interoperability
- **MQTT protocol** for IoT cloud integration
- **Web server** for browser-based monitoring
- **Data logging** to SD card
- **OTA firmware updates** via Ethernet

## 📜 License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📞 Support

For questions and support, please open an issue in the GitHub repository.
