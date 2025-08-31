/**
 * @file sht3xd_reader.cpp
 * @brief Implementation of SHT31/SHT3x temperature and humidity sensor reader
 * 
 * This file implements the SHT3xReader class which provides a C++ wrapper
 * around Zephyr's sensor driver framework for SHT31 sensors. It handles
 * device initialization, sensor communication, and data conversion.
 */

#include "sht3xd_reader.h"

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(SHT3xReader);

/**
 * @brief Constructor - Initialize SHT31 sensor device
 * 
 * Automatically discovers the SHT31 sensor through Zephyr's device tree
 * and checks if the device is ready for operation. The sensor must be
 * properly configured in the device tree overlay with compatible string
 * "sensirion,sht3xd".
 * 
 * Initializes cached temperature and humidity values to zero.
 */
SHT3xReader::SHT3xReader() : dev_(DEVICE_DT_GET_ONE(sensirion_sht3xd)), temp_(0), hum_(0) {
    // Check if the SHT31 sensor device is ready for operation
    if (!device_is_ready(dev_)) {
        LOG_ERR("SHT31 sensor device %s is not ready", dev_->name);
    } else {
        LOG_INF("SHT31 sensor device %s initialized successfully", dev_->name);
    }
}

/**
 * @brief Fetch fresh sensor readings from SHT31 hardware
 * 
 * This method performs a complete sensor reading cycle:
 * 1. Triggers sensor measurement via I2C
 * 2. Reads temperature channel data
 * 3. Reads humidity channel data
 * 4. Converts sensor values to double precision
 * 5. Updates internal cache
 * 
 * @return true if all sensor operations completed successfully
 * @return false if any sensor operation failed (I2C error, device not ready, etc.)
 * 
 * @note This method performs I2C communication and may block briefly
 * @note On failure, cached values remain unchanged from previous reading
 */
bool SHT3xReader::fetch() {
    // Trigger sensor measurement - initiates I2C communication
    int rc = sensor_sample_fetch(dev_);
    if (rc != 0) {
        LOG_ERR("Sensor sample fetch failed with error code: %d", rc);
        return false;
    }

    // Declare sensor value structures for temperature and humidity
    struct sensor_value temperature_raw, humidity_raw;
    
    // Read temperature channel from sensor
    rc = sensor_channel_get(dev_, SENSOR_CHAN_AMBIENT_TEMP, &temperature_raw);
    if (rc != 0) {
        LOG_ERR("Temperature channel read failed with error code: %d", rc);
        return false;
    }
    
    // Read humidity channel from sensor
    rc = sensor_channel_get(dev_, SENSOR_CHAN_HUMIDITY, &humidity_raw);
    if (rc != 0) {
        LOG_ERR("Humidity channel read failed with error code: %d", rc);
        return false;
    }

    // Convert raw sensor values to double precision and update cache
    temp_ = sensor_value_to_double(&temperature_raw);  // Convert to degrees Celsius
    hum_ = sensor_value_to_double(&humidity_raw);      // Convert to percentage (0-100%)
    
    LOG_DBG("Sensor reading successful: %.2f°C, %.2f%%", temp_, hum_);
    return true;
}
