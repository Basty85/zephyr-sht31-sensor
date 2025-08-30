#include "sht3xd_reader.h"

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(SHT3xReader);

SHT3xReader::SHT3xReader() : dev_(DEVICE_DT_GET_ONE(sensirion_sht3xd)), temp_(0), hum_(0) {
    if (!device_is_ready(dev_)) {
        LOG_ERR("Device %s is not ready", dev_->name);
    }
}

bool SHT3xReader::fetch() {
    int rc = sensor_sample_fetch(dev_);
    if (rc != 0) {
        LOG_ERR("Fetch failed: %d", rc);
        return false;
    }

    struct sensor_value t, h;
    rc = sensor_channel_get(dev_, SENSOR_CHAN_AMBIENT_TEMP, &t);
    if (rc != 0) return false;
    rc = sensor_channel_get(dev_, SENSOR_CHAN_HUMIDITY, &h);
    if (rc != 0) return false;

    temp_ = sensor_value_to_double(&t);
    hum_ = sensor_value_to_double(&h);
    return true;
}
