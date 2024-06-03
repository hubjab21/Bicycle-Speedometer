idf_component_register(SRCS "adc_sensor.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES "esp_event"
                                  "nvs_flash"
                                  "bt"
                                  "driver"
                                  "esp_adc"
                                  "nimBLE")