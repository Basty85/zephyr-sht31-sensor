#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

class SHT3xReader {
   public:
    SHT3xReader();

    bool fetch();                                            // read sensor value
    inline double getTemperature() const { return temp_; };  // latest temperature value
    inline double getHumidity() const { return hum_; };      // latest humidity value

   private:
    const struct device *dev_;
    double temp_;
    double hum_;
};
