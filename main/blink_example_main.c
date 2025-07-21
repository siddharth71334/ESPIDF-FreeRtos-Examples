/*
 * FreeRTOS Educational Demo Main
 * -----------------------------
 * Select which FreeRTOS demo to run by uncommenting ONE macro below.
 * Each demo demonstrates a different FreeRTOS API or concept.
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
#include "freertos_mutex.h"
#include "freertos_recursive_mutex.h"
#include "freertos_semaphore.h"
#include "freertos_queue_set.h"
#include "freertos_stream_buffer.h"
#include "freertos_message_buffer.h"
#include "freertos_task_notify.h"
#include "freertos_priority_inheritance.h"
#include "freertos_dynamic_task.h"
#include "freertos_idle_hook.h"

// Uncomment ONE of the following lines to select the demo to run:
#define RUN_FREERTOS_BASIC_DEMO
// #define RUN_FREERTOS_INTERMEDIATE_DEMO
// #define RUN_FREERTOS_ADVANCED_DEMO
// #define RUN_FREERTOS_MUTEX_DEMO
// #define RUN_FREERTOS_RECURSIVE_MUTEX_DEMO
// #define RUN_FREERTOS_SEMAPHORE_DEMO
// #define RUN_FREERTOS_QUEUE_SET_DEMO
// #define RUN_FREERTOS_STREAM_BUFFER_DEMO
// #define RUN_FREERTOS_MESSAGE_BUFFER_DEMO
// #define RUN_FREERTOS_TASK_NOTIFY_DEMO
// #define RUN_FREERTOS_PRIORITY_INHERITANCE_DEMO
// #define RUN_FREERTOS_DYNAMIC_TASK_DEMO
// #define RUN_FREERTOS_IDLE_HOOK_DEMO

void app_main(void)
{
#if defined(RUN_FREERTOS_BASIC_DEMO)
    freertos_basic_demo();
#elif defined(RUN_FREERTOS_INTERMEDIATE_DEMO)
    freertos_intermediate_demo();
#elif defined(RUN_FREERTOS_ADVANCED_DEMO)
    freertos_advanced_demo();
#elif defined(RUN_FREERTOS_MUTEX_DEMO)
    freertos_mutex_demo();
#elif defined(RUN_FREERTOS_RECURSIVE_MUTEX_DEMO)
    freertos_recursive_mutex_demo();
#elif defined(RUN_FREERTOS_SEMAPHORE_DEMO)
    freertos_semaphore_demo();
#elif defined(RUN_FREERTOS_QUEUE_SET_DEMO)
    freertos_queue_set_demo();
#elif defined(RUN_FREERTOS_STREAM_BUFFER_DEMO)
    freertos_stream_buffer_demo();
#elif defined(RUN_FREERTOS_MESSAGE_BUFFER_DEMO)
    freertos_message_buffer_demo();
#elif defined(RUN_FREERTOS_TASK_NOTIFY_DEMO)
    freertos_task_notify_demo();
#elif defined(RUN_FREERTOS_PRIORITY_INHERITANCE_DEMO)
    freertos_priority_inheritance_demo();
#elif defined(RUN_FREERTOS_DYNAMIC_TASK_DEMO)
    freertos_dynamic_task_demo();
#elif defined(RUN_FREERTOS_IDLE_HOOK_DEMO)
    freertos_idle_hook_demo();
#else
    #error "Please select a FreeRTOS demo to run."
#endif
}
