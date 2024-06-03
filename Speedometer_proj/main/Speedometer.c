#include <stdio.h>
#include <string.h>
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
#include "esp_system.h"
#include "esp_task_wdt.h"  

#define ADC_UNIT ADC_UNIT_1
#define ADC_CHANNEL ADC_CHANNEL_4  // GPIO32
#define ADC_ATTEN ADC_ATTEN_DB_12
#define ADC_BIT_WIDTH ADC_BITWIDTH_12
#define ADC_READ_LEN 16
#define TAG "ADC_CONTINUOUS"

static void continuous_adc_init(adc_continuous_handle_t *handle) {
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 16,
        .conv_frame_size = ADC_READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
    };

    adc_digi_pattern_config_t adc_pattern = {
        .atten = ADC_ATTEN,
        .channel = ADC_CHANNEL,
        .unit = ADC_UNIT,
        .bit_width = ADC_BIT_WIDTH
    };
    
    dig_cfg.pattern_num = 1;
    dig_cfg.adc_pattern = &adc_pattern;

    ESP_ERROR_CHECK(adc_continuous_config(*handle, &dig_cfg));
}

static bool IRAM_ATTR adc_conversion_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data) {
    BaseType_t mustYield = pdFALSE;
    vTaskNotifyGiveFromISR((TaskHandle_t)user_data, &mustYield);
    return (mustYield == pdTRUE);
}

void app_main(void) {
    adc_continuous_handle_t adc_handle;
    continuous_adc_init(&adc_handle);

    uint8_t result[ADC_READ_LEN] = {0};
    uint32_t ret_num = 0;

    TaskHandle_t task_handle = xTaskGetCurrentTaskHandle();
    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = adc_conversion_done_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(adc_handle, &cbs, task_handle));
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));

    ESP_LOGI(TAG, "ADC continuous mode started");

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        while (1) {
            esp_err_t ret = adc_continuous_read(adc_handle, result, ADC_READ_LEN, &ret_num, 0);
            if (ret == ESP_OK) {
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
                    adc_digi_output_data_t *p = (adc_digi_output_data_t*)&result[i];
                    uint32_t chan_num = (p->type1.channel);
                    uint32_t data = p->type1.data;
                    if (chan_num == ADC_CHANNEL) {
                        ESP_LOGI(TAG, "Channel: %"PRIu32", Value: %"PRIu32, chan_num, data);
                    } else {
                        ESP_LOGW(TAG, "Invalid data [Channel: %"PRIu32", Value: %"PRIu32"]", chan_num, data);
                    }
                }
                vTaskDelay(pdMS_TO_TICKS(100)); // Delay to avoid flooding the log
            } else if (ret == ESP_ERR_TIMEOUT) {
                break; // Exit the inner loop if no data is available
            }
        }

        // Feed the watchdog
        esp_task_wdt_reset();
    }

    ESP_ERROR_CHECK(adc_continuous_stop(adc_handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(adc_handle));
}