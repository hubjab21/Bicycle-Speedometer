#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "sdkconfig.h"
#include "nimBLE.h"
#include "driver/gpio.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

static const char *TAG = "ADC EXAMPLE";

static esp_adc_cal_characteristics_t adc1_chars;

void app_main(void)
{
    uint32_t voltage;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11));
    Init_nimBLE_Sequence();
    while (1) 
    {
        voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_4), &adc1_chars);
        ESP_LOGI(TAG, "ADC1_CHANNEL_6: %lu mV", voltage);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}