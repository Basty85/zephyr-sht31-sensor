#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "sensor_handler.h"
#include "udp_client.h"

LOG_MODULE_REGISTER(main);

/**
 * @brief Main application entry point
 *
 * This function initializes the SensorHandler and UDP client, then continuously
 * processes sensor data and transmits it via UDP at 1Hz to the configured remote
 * server. The application uses a clean architecture with SensorHandler managing
 * all sensor operations and data caching.
 *
 * Architecture flow:
 * SensorHandler.update() -> SensorData (with timestamp) -> UDP transmission
 *
 * @return int Return code (never reached due to infinite loop)
 */
int main(void) {
    // Initialize high-level sensor handler for SHT31 temperature/humidity sensor
    // SensorHandler encapsulates SHT3xReader and manages SensorData internally
    SensorHandler my_sensor;

    // Initialize UDP client with target server IP and port
    // Target: 192.168.1.37:8888
    UdpClient udp_client("192.168.1.37", 8888);

    LOG_INF("=== SHT31 Sensor UDP Transmitter ===");
    LOG_INF("Using SensorHandler with integrated SensorData management");
    LOG_INF("Target server: 192.168.1.37:8888");

    // Wait for network initialization to complete
    // This ensures Ethernet interface is up and IP is configured
    LOG_INF("Waiting for network initialization...");
    k_sleep(K_SECONDS(3));
    LOG_INF("Starting sensor data transmission loop");

    // Main application loop - runs continuously
    while (true) {
        // Update sensor readings using SensorHandler
        // This calls SHT3xReader internally and updates SensorData with timestamp
        if (my_sensor.update()) {
            // Get reference to SensorData structure (contains temp, humidity, timestamp)
            const SensorData& sensor_data = my_sensor.getData();
            
            // Log sensor readings locally via UART
            // Explicit cast to double to avoid float-to-double promotion warnings
            LOG_INF("Sensor readings: %.2f deg, %.2f %%", 
                    (double)sensor_data.temperature, (double)sensor_data.humidity);

            // Transmit complete SensorData structure via UDP
            // SensorData is packed and optimized for network transmission
            if (udp_client.send(&sensor_data, sizeof(sensor_data))) {
                // Log successful transmission with timestamp
                // Explicit casts to avoid float-to-double promotion warnings
                LOG_INF("UDP transmitted: %.2f deg, %.2f %% [%u ms]", 
                        (double)sensor_data.temperature,
                        (double)sensor_data.humidity, 
                        sensor_data.timestamp);
            } else {
                // Log transmission failure
                LOG_ERR("UDP transmission failed");
            }
        } else {
            // Log sensor read failure
            LOG_ERR("Sensor reading failed");
        }

        // Wait 1 second before next sensor reading
        // This creates a 1Hz sampling rate
        k_sleep(K_SECONDS(1));
    }

    // This return is never reached due to infinite loop
    return 0;
}
