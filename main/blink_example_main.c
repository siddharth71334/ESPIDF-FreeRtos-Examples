/*
 * FreeRTOS Educational Demo Main
 * -----------------------------
 * Select which FreeRTOS demo to run by uncommenting ONE macro below.
 * Each demo demonstrates a different FreeRTOS API or concept.
 *
 * This main file serves as a demo selector for various FreeRTOS features:
 * - Basic task creation and scheduling
 * - Intermediate queue usage and ISR handling
 * - Advanced software timers and event groups
 * - Mutex and semaphore synchronization
 * - Queue sets for multi-source event handling
 * - Stream and message buffers for data transfer
 * - Task notifications for lightweight signaling
 * - Priority inheritance and dynamic task management
 * - Idle hooks for background processing
 *
 * NOTE: Only one demo should be active at a time to avoid resource conflicts.
 * Each demo is self-contained and demonstrates specific FreeRTOS concepts.
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

// Demo Selection Macros
// Uncomment ONE of the following lines to select the demo to run:
// #define RUN_FREERTOS_BASIC_DEMO          // Basic task creation and scheduling
#define RUN_FREERTOS_INTERMEDIATE_DEMO     // Queue usage and ISR handling
// #define RUN_FREERTOS_ADVANCED_DEMO       // Software timers and event groups
// #define RUN_FREERTOS_MUTEX_DEMO          // Mutex synchronization
// #define RUN_FREERTOS_RECURSIVE_MUTEX_DEMO // Recursive mutex usage
// #define RUN_FREERTOS_SEMAPHORE_DEMO      // Binary and counting semaphores
// #define RUN_FREERTOS_QUEUE_SET_DEMO      // Multi-source event handling
// #define RUN_FREERTOS_STREAM_BUFFER_DEMO  // Continuous data streaming
// #define RUN_FREERTOS_MESSAGE_BUFFER_DEMO // Discrete message handling
// #define RUN_FREERTOS_TASK_NOTIFY_DEMO    // Lightweight task signaling
// #define RUN_FREERTOS_PRIORITY_INHERITANCE_DEMO // Priority inheritance
// #define RUN_FREERTOS_DYNAMIC_TASK_DEMO   // Dynamic task creation/deletion
// #define RUN_FREERTOS_IDLE_HOOK_DEMO      // Background processing

// Main application entry point
// NOTE: This function is called by the ESP-IDF framework after system initialization
void app_main(void)
{
    // Demo selection using preprocessor directives
    // NOTE: Only one demo will be compiled and executed based on the defined macro
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
    // Compile-time error if no demo is selected
    #error "Please select a FreeRTOS demo to run by uncommenting one of the demo macros above."
#endif
}
