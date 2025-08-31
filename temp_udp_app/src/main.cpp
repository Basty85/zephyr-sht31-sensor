#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "sht3xd_reader.h"
#include "udp_client.h"

LOG_MODULE_REGISTER(main);

/**
 * @brief Sensor data structure for UDP transmission
 * 
 * This structure contains temperature, humidity, and timestamp data
 * that will be transmitted via UDP to the remote server.
 * The __attribute__((packed)) ensures no padding between fields.
 */
struct SensorData {
    float temperature;      // Temperature in degrees Celsius
    float humidity;         // Relative humidity in percentage
    uint32_t timestamp;     // System uptime in milliseconds
} __attribute__((packed));

/**
 * @brief Main application entry point
 * 
 * This function initializes the SHT31 sensor and UDP client,
 * then continuously reads sensor data and transmits it via UDP
 * every second to the configured remote server.
 * 
 * @return int Return code (never reached due to infinite loop)
 */
int main(void) {
    // Initialize sensor reader for SHT31 temperature/humidity sensor
    SHT3xReader my_sensor;
    
    // Initialize UDP client with target server IP and port
    // Target: 192.168.1.37:8888
    UdpClient udp_client("192.168.1.37", 8888);

    LOG_INF("Starting SHT3x sensor with UDP transmission...");
    LOG_INF("Target server: 192.168.1.37:8888");

    // Wait for network initialization to complete
    // This ensures Ethernet interface is up and IP is configured
    k_sleep(K_SECONDS(3));

    // Main application loop - runs continuously
    while (true) {
        // Attempt to read sensor data
        if (my_sensor.fetch()) {
            // Extract temperature and humidity values
            double temp = my_sensor.getTemperature();
            double hum = my_sensor.getHumidity();

            // Log sensor readings locally via UART
            LOG_INF("Temp: %.2f deg, Hum: %.2f %%", temp, hum);

            // Prepare sensor data structure for UDP transmission
            SensorData data;
            data.temperature = (float)temp;         // Convert to float for network efficiency
            data.humidity = (float)hum;             // Convert to float for network efficiency
            data.timestamp = k_uptime_get_32();     // System uptime in milliseconds

            // Transmit sensor data via UDP
            if (udp_client.send(&data, sizeof(data))) {
                // Log successful transmission with explicit casts to avoid warnings
                LOG_INF("UDP sent: %.2f deg, %.2f %% [%u ms]", 
                        (double)data.temperature, (double)data.humidity, data.timestamp);
            } else {
                // Log transmission failure
                LOG_ERR("Failed to send UDP data");
            }
        } else {
            // Log sensor read failure
            LOG_ERR("Failed to read sensor data");
        }

        // Wait 1 second before next sensor reading
        // This creates a 1Hz sampling rate
        k_sleep(K_SECONDS(1));
    }

    // This return is never reached due to infinite loop
    return 0;
}
