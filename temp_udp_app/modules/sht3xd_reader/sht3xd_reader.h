/**
 * @file sht3xd_reader.h
 * @brief SHT31/SHT3x temperature and humidity sensor interface for Zephyr
 * 
 * This header provides a C++ wrapper class for the SHT31 sensor driver
 * in Zephyr RTOS. It simplifies sensor access and provides cached readings
 * for efficient data access.
 */

#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/**
 * @brief SHT31/SHT3x temperature and humidity sensor reader class
 * 
 * This class provides a high-level interface to the SHT31 sensor hardware
 * through Zephyr's sensor driver framework. It handles sensor initialization,
 * data fetching, and maintains cached readings for efficient access.
 * 
 * The sensor communicates via I2C and provides high-precision temperature
 * and humidity measurements suitable for environmental monitoring applications.
 * 
 * Usage example:
 * @code
 * SHT3xReader sensor;
 * if (sensor.fetch()) {
 *     double temp = sensor.getTemperature();  // Get temperature in Celsius
 *     double hum = sensor.getHumidity();      // Get humidity in percentage
 * }
 * @endcode
 */
class SHT3xReader {
public:
    /**
     * @brief Constructor - initializes the SHT31 sensor
     * 
     * Automatically finds and initializes the SHT31 sensor device
     * through Zephyr's device tree configuration. The sensor must be
     * properly configured in the device tree overlay.
     */
    SHT3xReader();

    /**
     * @brief Fetch fresh sensor readings from hardware
     * 
     * Triggers a new measurement cycle and reads both temperature and
     * humidity values from the SHT31 sensor. The readings are cached
     * internally and can be accessed via getTemperature() and getHumidity().
     * 
     * @return true if sensor reading was successful
     * @return false if sensor communication failed or sensor not available
     * 
     * @note This method performs I2C communication and may block briefly
     * @note Previous cached values remain unchanged if reading fails
     */
    bool fetch();

    /**
     * @brief Get the latest temperature reading
     * 
     * Returns the cached temperature value from the most recent successful
     * fetch() operation. The value is in degrees Celsius with high precision.
     * 
     * @return double Temperature in degrees Celsius
     * 
     * @note Call fetch() first to ensure fresh data
     * @note Returns cached value from last successful reading
     */
    inline double getTemperature() const { 
        return temp_; 
    }

    /**
     * @brief Get the latest humidity reading
     * 
     * Returns the cached humidity value from the most recent successful
     * fetch() operation. The value is relative humidity as a percentage (0-100%).
     * 
     * @return double Relative humidity in percentage (0-100%)
     * 
     * @note Call fetch() first to ensure fresh data
     * @note Returns cached value from last successful reading
     */
    inline double getHumidity() const { 
        return hum_; 
    }

private:
    const struct device *dev_;  ///< Pointer to Zephyr sensor device instance
    double temp_;               ///< Cached temperature reading in degrees Celsius
    double hum_;                ///< Cached humidity reading in percentage
};
