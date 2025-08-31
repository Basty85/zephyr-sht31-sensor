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
 * This structure stores the latest sensor readings in double precision
 * format for maximum accuracy in calculations and processing.
 */
struct SensorData {
    float temperature;   // Temperature in degrees Celsius
    float humidity;      // Relative humidity in percentage
    uint32_t timestamp;  // System uptime in milliseconds
} __attribute__((packed));

/**
 * @brief High-level sensor handler class for SHT31 temperature/humidity sensor
 *
 * This class provides a simplified interface for managing SHT31 sensor readings.
 * It encapsulates the low-level sensor operations and maintains the latest
 * sensor data in an easily accessible format.
 *
 * Usage example:
 * @code
 * SensorHandler sensor;
 * sensor.update();                    // Read latest sensor values
 * const auto& data = sensor.getData(); // Get cached readings
 * printf("Temp: %.2f°C\n", data.temperature);
 * @endcode
 */
class SensorHandler {
   public:
    /**
     * @brief Default constructor
     *
     * Initializes the sensor handler with default values.
     * The internal SHT3xReader will be initialized automatically.
     */
    SensorHandler() {}

    /**
     * @brief Read the sensor and update internal data cache
     *
     * This method performs a sensor reading operation using the underlying
     * SHT3xReader. If the reading is successful, the internal data cache
     * is updated with the new temperature and humidity values.
     *
     * @note This method should be called periodically to keep sensor data current.
     *       Failed sensor readings will leave the previous data unchanged.
     */
    void update() {
        // Attempt to fetch new sensor readings
        if (reader.fetch()) {
            // Update cached data with fresh sensor readings
            data.temperature = reader.getTemperature();
            data.humidity = reader.getHumidity();
            data.timestamp = k_uptime_get_32();
        }
        // Note: If fetch() fails, previous data remains unchanged
    }

    /**
     * @brief Get the latest sensor data
     *
     * Returns a const reference to the internal sensor data structure
     * containing the most recent temperature and humidity readings.
     *
     * @return const SensorData& Reference to the cached sensor data
     *
     * @note The returned data reflects the last successful sensor reading.
     *       Call update() first to ensure fresh data.
     */
    const SensorData& getData() const { return data; }

   private:
    SHT3xReader reader;  ///< Low-level sensor reader instance
    SensorData data{};   ///< Cached sensor data (zero-initialized)
};
