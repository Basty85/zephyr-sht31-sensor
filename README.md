# Zephyr SHT31 Sensor UDP Transmitter for STM32 Nucleo-H755ZI-Q

This repository contains a Zephyr RTOS application that reads temperature and humidity data from an SHT31 sensor and transmits it via UDP over Ethernet. The project demonstrates C++ design patterns, modular architecture, and embedded networking best practices.

## 🌟 Features

- **SHT31 Temperature & Humidity Sensor** integration via I2C interface
- **UDP networking** for real-time sensor data transmission over Ethernet
- **Modern C++ modular design** with clean separation of concerns
- **Professional documentation** with Doxygen-style comments
- **Remote Telnet console** for system monitoring and diagnostics
- **Network statistics** and debugging capabilities
- **High-precision sensor readings** with efficient network transmission
- **Static IP configuration** for reliable network deployment
- **Integrated data management** with timestamp and packed transmission format
- **Docker development environment** for consistent cross-platform builds

## 🔧 Hardware Requirements

- **STM32 Nucleo-H755ZI-Q** development board (Cortex-M7 @ 480MHz)
- **SHT31 Temperature & Humidity Sensor** (I2C interface)
- **Ethernet connection** for UDP communication and Telnet access
- Jumper wires for sensor connections

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
│       └── main.cpp                # Application orchestration using SensorHandler
├── docker/
│   ├── Dockerfile                  # Docker development environment
│   ├── entrypoint.sh               # Container initialization script
│   └── west-stm32.yml              # West workspace configuration
├── scripts/
│   ├── build.sh                    # Automated build script
│   └── flash.sh                    # Automated flash script
├── python_receiver/
│   └── simple_receiver.py          # Python UDP receiver for testing
├── docker-compose.yml              # Docker Compose configuration
├── west.yml                        # West manifest for dependencies
├── CMakeLists.txt
├── LICENSE
└── README.md
```

### 🎯 Key Design Principles

- **Separation of Concerns**: Each module has a single, well-defined responsibility
- **Clean Interfaces**: Header/implementation separation for maintainability
- **Integrated Data Management**: `SensorHandler` manages `SensorData` with automatic timestamping
- **Professional Documentation**: Doxygen-style comments throughout
- **Error Handling**: Comprehensive error checking and logging
- **Resource Management**: Proper RAII and cleanup patterns

### 📊 Data Flow Architecture

```
SHT3xReader (low-level I2C)
    ↓
SensorHandler (high-level management)
    ↓
SensorData (temp, humidity, timestamp)
    ↓
UdpClient (network transmission)
    ↓
Remote Server (192.168.1.37:8888)
```

### 🔄 SensorData Structure

The application uses a packed data structure for efficient network transmission:

```cpp
struct SensorData {
    float temperature;   // Temperature in degrees Celsius
    float humidity;      // Relative humidity in percentage (0-100%)
    uint32_t timestamp;  // System uptime in milliseconds
} __attribute__((packed));
```

**Benefits:**
- **Automatic timestamping** by SensorHandler
- **Packed format** for efficient UDP transmission (12 bytes total)
- **Float precision** optimized for network bandwidth
- **Integrated management** - no separate data conversion needed

## 🚀 Getting Started

You can set up the development environment either using Docker (recommended) or manually.

### Option 1: Docker Development Environment (Recommended)

The easiest way to get started is using the provided Docker development environment, which provides a consistent build environment across all platforms.

#### Quick Start with Docker

1. **Prerequisites**
   - Install Docker and Docker Compose on your system
   - Clone this repository

2. **Start Development Environment**
   ```shell
   cd zephyr-sht31-sensor
   docker compose run --rm zephyr
   ```

3. **Build and Flash** (inside the container)
   ```shell
   # Build the application
   ./scripts/build.sh
   
   # Flash to hardware (requires USB device access)
   ./scripts/flash.sh
   ```

#### Docker Development Benefits

- **No local toolchain installation required**
- **Consistent build environment** across Windows, macOS, and Linux
- **Pre-configured Zephyr SDK** and dependencies
- **Reproducible builds** for CI/CD integration
- **Isolated environment** that doesn't affect your host system

#### USB Device Access for Flashing

To flash the firmware from within Docker, you need to provide access to the USB device:

**Linux:**
```shell
docker compose run --rm --device=/dev/ttyACM0 zephyr ./scripts/flash.sh
```

**Alternative: Manual Docker Run**
```shell
docker build -t zephyr-sht31-dev -f docker/Dockerfile .
docker run --rm -it \
  -v $PWD:/workspace \
  --device=/dev/ttyACM0 \
  zephyr-sht31-dev ./scripts/flash.sh
```

### Option 2: Manual Environment Setup

If you prefer to set up the environment manually:

#### Step 1: Environment Setup

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

# Install Zephyr SDK
cd zephyr
west sdk install
```

#### Step 2: Build Application

Build the application with the following command:

```shell
west build -b nucleo_h755zi_q/stm32h755xx/m7 zephyr-sht31-sensor/temp_udp_app -d build_sht31 -- -DDTC_OVERLAY_FILE="boards/nucleo_h755zi_q.overlay"
```

#### Step 3: Flash to Hardware

Flash the compiled firmware to the Nucleo board:

```shell
west flash --runner openocd --build-dir build_sht31
```

## 🛳️ Docker Environment Details

### Docker Configuration

The Docker environment includes:
- **Zephyr SDK** with ARM Cortex-M toolchain
- **West tool** and Python dependencies
- **OpenOCD** for flashing and debugging
- **Build scripts** for easy compilation and deployment

### Available Scripts

Inside the Docker container, use these convenience scripts:

```shell
# Build the application
./scripts/build.sh

# Flash to connected hardware
./scripts/flash.sh

# Interactive shell for development
docker compose run --rm zephyr bash
```

### Docker Compose Services

- **zephyr**: Main development service with full toolchain
- **Volume mounting**: Your project directory is mounted at `/workspace`
- **Environment variables**: Pre-configured for Zephyr development

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
[00:00:03.027,000] <inf> main: Using SensorHandler with integrated SensorData management
[00:00:03.034,000] <inf> main: Target server: 192.168.1.37:8888
[00:00:06.042,000] <inf> main: Starting sensor data transmission loop
[00:00:06.049,000] <inf> main: Sensor readings: 23.45 deg, 51.20 %
[00:00:06.056,000] <inf> main: UDP transmitted: 23.45 deg, 51.20 % [6056 ms]
```

### UDP Data Format

The transmitted UDP packets contain the packed SensorData structure:

```
Bytes 0-3:   Temperature (float, IEEE 754)
Bytes 4-7:   Humidity (float, IEEE 754)  
Bytes 8-11:  Timestamp (uint32_t, little-endian)
Total:       12 bytes per packet
```

### UDP Data Reception

Use the included Python receiver to monitor sensor data:

```shell
cd python_receiver
python3 simple_receiver.py
```

Expected receiver output:
```
Listening for sensor data on port 8888...
Received from 192.168.1.38: Temp=23.45°C, Humidity=51.20%, Timestamp=6056ms
Received from 192.168.1.38: Temp=23.46°C, Humidity=51.18%, Timestamp=7056ms
```

## 🔧 Core Components

### SensorHandler Class

The `SensorHandler` provides high-level sensor management:

```cpp
SensorHandler sensor;

// Read sensor and update internal SensorData
if (sensor.update()) {
    const SensorData& data = sensor.getData();
    printf("Temp: %.2f°C, Hum: %.2f%%\n", data.temperature, data.humidity);
}
```

**Key Features:**
- Encapsulates low-level `SHT3xReader`
- Automatic timestamp generation
- Error handling and data caching
- Clean, simple interface

### UdpClient Class

The `UdpClient` handles network transmission:

```cpp
UdpClient client("192.168.1.37", 8888);

// Send SensorData directly
if (client.send(&sensor_data, sizeof(sensor_data))) {
    printf("Data transmitted successfully\n");
}
```

**Key Features:**
- BSD socket API wrapper
- Automatic server address configuration
- Error handling and logging
- Efficient binary data transmission

## 🧪 Testing and Validation

### 1. Sensor Data Validation

```shell
# Via Telnet console - check device status
nucleo-eth:~$ device list
# Should show SHT31 sensor as ready

# Monitor logs for sensor readings
# Temperature should be reasonable (15-40°C)
# Humidity should be 0-100%
```

### 2. Network Data Validation

```shell
# Start Python receiver
python3 python_receiver/simple_receiver.py

# Verify data format and timing
# Packets should arrive every ~1 second
# Data should be consistent and reasonable
```

### 3. Timestamp Validation

```shell
# Check timestamp progression in logs
# Timestamp should increase by ~1000ms each reading
# Timestamp = system uptime in milliseconds
```

## 📈 Performance Metrics

- **Sampling Rate**: 1 Hz (1 second intervals)
- **Data Packet Size**: 12 bytes (temperature + humidity + timestamp)
- **Network Latency**: < 10ms (local network)
- **Memory Usage**: ~50KB RAM, ~200KB Flash
- **Power Consumption**: ~200mA @ 3.3V (with Ethernet active)
- **Temperature Accuracy**: ±0.3°C (sensor specification)
- **Humidity Accuracy**: ±2% RH (sensor specification)
- **Timestamp Resolution**: 1ms (system tick dependent)

## 🔮 Future Enhancements

- **Multiple sensor support** with sensor array management
- **JSON data format** for better interoperability
- **MQTT protocol** for IoT cloud integration
- **Data buffering** for unreliable network conditions
- **Configuration via Telnet** for runtime parameter changes
- **SD card logging** for local data storage
- **OTA firmware updates** via Ethernet

## 📜 License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.
