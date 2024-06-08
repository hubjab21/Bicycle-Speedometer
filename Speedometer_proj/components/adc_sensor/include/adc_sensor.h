#include "esp_adc/adc_continuous.h"

void threshold_task(void *arg);
void continuous_adc_init(adc_continuous_handle_t *handle);
void sensor_func(void);
extern uint32_t sensor_data;
extern char sensor_speed_data[8];