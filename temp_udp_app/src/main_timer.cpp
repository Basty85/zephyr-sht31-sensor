#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "sht3xd_reader.h"

LOG_MODULE_REGISTER(main);

// Global pointer to sensor instance (not the object itself to avoid early construction)
static SHT3xReader *my_sensor = nullptr;

// Work queue handler for sensor readings - runs in thread context
static void sensor_work_handler(struct k_work *work) {
    // Only read sensor if it's properly initialized
    if (my_sensor && my_sensor->fetch()) {
        LOG_INF("Temp: %.2f C, Hum: %.2f %%", my_sensor->getTemperature(), my_sensor->getHumidity());
    }
}

// Define work item for sensor readings
K_WORK_DEFINE(sensor_work, sensor_work_handler);

// Timer callback - runs in interrupt context, schedules work for thread context
void walltimer_handler(struct k_timer *timer_id) {
    // Submit work to system work queue for thread context execution
    k_work_submit(&sensor_work);
}

// Define timer with callback
K_TIMER_DEFINE(my_timer, walltimer_handler, nullptr);

int main(void) {
    // Create sensor instance on stack (after system initialization)
    SHT3xReader sensor_instance;
    my_sensor = &sensor_instance;

    LOG_INF("Starting callback-based SHT3x reader...");

    // Start timer - first trigger after 1 second, then every 2 seconds
    k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(2));

    // Keep main thread alive forever - all work is done via timer callbacks
    k_sleep(K_FOREVER);

    // legacy solution busy wait
    /*
    while (true) {
        if (my_sensor.fetch()) {
            LOG_INF("Temp: %.2f C, Hum: %.2f %%", my_sensor.getTemperature(), my_sensor.getHumidity());
        }
        k_sleep(K_SECONDS(2));
    }
    */

    return 0;
}
