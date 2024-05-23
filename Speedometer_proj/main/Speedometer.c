#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "sdkconfig.h"
#include "nimBLE.h"
 
void ble_app_advertise(void);

void app_main(void)
{
    Init_nimBLE_Sequence();
}
    