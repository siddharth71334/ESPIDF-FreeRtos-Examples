/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos_basic.h"
#include "freertos_intermediate.h"
#include "freertos_advanced.h"

// Uncomment ONE of the following lines to select the demo to run:
#define RUN_FREERTOS_BASIC_DEMO
// #define RUN_FREERTOS_INTERMEDIATE_DEMO
// #define RUN_FREERTOS_ADVANCED_DEMO

void app_main(void)
{
#if defined(RUN_FREERTOS_BASIC_DEMO)
    freertos_basic_demo();
#elif defined(RUN_FREERTOS_INTERMEDIATE_DEMO)
    freertos_intermediate_demo();
#elif defined(RUN_FREERTOS_ADVANCED_DEMO)
    freertos_advanced_demo();
#else
    #error "Please select a FreeRTOS demo to run."
#endif
}
