/*
 * FreeRTOS Dynamic Task Demo
 * --------------------------
 * Demonstrates creating and deleting tasks at runtime.
 *
 * WHAT: Tasks can be created and deleted dynamically as needed.
 * WHY: Useful for temporary/background work, or to save memory.
 * WHEN: Use when a task is only needed for a short time or on demand.
 *
 * NOTE: Dynamic task creation allows for flexible resource management and
 * on-demand task execution. However, task creation has overhead, so it's
 * not suitable for frequently created/deleted tasks.
 */
#include "freertos_dynamic_task.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Temporary task that will self-delete after completion
// NOTE: This task demonstrates how tasks can delete themselves
static void temporary_task(void *pvParameter) {
    printf("temporary_task: running, will self-delete\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("temporary_task: deleting itself\n");
    
    // FreeRTOS API: vTaskDelete - Deletes the current task
    // Parameters: task handle (NULL = delete current task)
    // NOTE: This task will be removed from the scheduler and its stack freed
    vTaskDelete(NULL);
}

// Task that creates temporary tasks periodically
// NOTE: This task demonstrates dynamic task creation
static void creator_task(void *pvParameter) {
    while (1) {
        printf("creator_task: creating temporary task\n");
        
        // FreeRTOS API: xTaskCreate - Creates a new task dynamically
        // Parameters: task function, task name, stack size, parameters, priority, task handle
        // NOTE: This creates a task that will run once and then delete itself
        xTaskCreate(temporary_task, "temporary_task", 2048, NULL, 5, NULL);
        
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void freertos_dynamic_task_demo(void) {
    // FreeRTOS API: xTaskCreate - Creates the task creator
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: This task will continuously create and manage temporary tasks
    xTaskCreate(creator_task, "creator_task", 2048, NULL, 4, NULL);
} 