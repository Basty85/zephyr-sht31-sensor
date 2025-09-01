/**
 * @file sensor_handler.h
 * @brief High-level sensor data handler for SHT31 temperature/humidity sensor
 *
 * This header provides a wrapper class around the low-level SHT3xReader
 * to simplify sensor data management and provide a clean interface for
 * accessing the latest sensor readings.
 */

#pragma once

#include <zephyr/kernel.h>

#include "sht3xd_reader.h"

/**
 * @brief Sensor data structure containing temperature and humidity readings
 *
 * This structure stores the latest sensor readings in float precision
 * format optimized for network transmission. It includes automatic
 * timestamping for data correlation and is packed for efficient UDP transmission.
 */
struct SensorData {
    float temperature;   ///< Temperature in degrees Celsius
    float humidity;      ///< Relative humidity in percentage (0-100%)
    uint32_t timestamp;  ///< System uptime in milliseconds when reading was taken
} __attribute__((packed));

/**
 * @brief High-level sensor handler class for SHT31 temperature/humidity sensor
 *
 * This class provides a simplified interface for managing SHT31 sensor readings.
 * It encapsulates the low-level sensor operations and maintains the latest
 * sensor data in an easily accessible format with automatic timestamping.
 *
 * Usage example:
 * @code
 * SensorHandler sensor;
 * if (sensor.update()) {                  // Read latest sensor values with error checking
 *     const auto& data = sensor.getData(); // Get cached readings with timestamp
 *     printf("Temp: %.2f°C at %u ms\n", data.temperature, data.timestamp);
 * }
 * @endcode
 */
class SensorHandler {
   public:
    /**
     * @brief Default constructor
     *
     * Initializes the sensor handler with default values.
     * The internal SHT3xReader will be initialized automatically.
     * SensorData is zero-initialized for safe initial state.
     */
    SensorHandler() {}

    /**
     * @brief Read the sensor and update internal data cache
     *
     * This method performs a sensor reading operation using the underlying
     * SHT3xReader. If the reading is successful, the internal data cache
     * is updated with the new temperature, humidity values, and current timestamp.
     *
     * @return true if sensor reading was successful and data was updated
     * @return false if sensor reading failed (I2C error, device not ready, etc.)
     *
     * @note This method should be called periodically to keep sensor data current
     * @note Failed sensor readings will leave the previous data unchanged
     * @note Timestamp is automatically updated on successful readings only
     */
    bool update() {
        // Attempt to fetch new sensor readings
        if (reader.fetch()) {
            // Update cached data with fresh sensor readings
            data.temperature = static_cast<float>(reader.getTemperature());
            data.humidity = static_cast<float>(reader.getHumidity());
            data.timestamp = k_uptime_get_32();  // Capture current system uptime
            return true;  // Indicate successful update
        }
        // Note: If fetch() fails, previous data remains unchanged
        return false;  // Indicate update failure
    }

    /**
     * @brief Get the latest sensor data
     *
     * Returns a const reference to the internal sensor data structure
     * containing the most recent temperature, humidity readings and timestamp
     * from the last successful update() call.
     *
     * @return const SensorData& Reference to the cached sensor data
     *
     * @note The returned data reflects the last successful sensor reading
     * @note Call update() first and check return value to ensure fresh data
     * @note Timestamp indicates when the data was last successfully updated
     */
    const SensorData& getData() const { return data; }

   private:
    SHT3xReader reader;  ///< Low-level sensor reader instance
    SensorData data{};   ///< Cached sensor data with timestamp (zero-initialized)
};
