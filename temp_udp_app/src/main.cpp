#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "sht3xd_reader.h"

LOG_MODULE_REGISTER(main);

int main(void) {
    SHT3xReader my_sensor;
    
    while (true) {
        if (my_sensor.fetch()) {
            LOG_INF("Temp: %.2f C, Hum: %.2f %%", my_sensor.getTemperature(), my_sensor.getHumidity());
        }
        k_sleep(K_SECONDS(2));
    }
    
    return 0;
}
