/*
 * FreeRTOS Priority Inheritance Demo
 * ----------------------------------
 * Demonstrates priority inheritance with mutexes to avoid priority inversion.
 *
 * WHAT: Priority inheritance boosts the priority of a low-priority task holding a mutex needed by a higher-priority task.
 * WHY: Prevents 'priority inversion' where a high-priority task is blocked by a low-priority one.
 * WHEN: Use when sharing resources between tasks of different priorities.
 *
 * NOTE: Priority inheritance is a key feature of mutexes that prevents priority inversion problems.
 * When a high-priority task waits for a mutex held by a low-priority task, the low-priority task
 * temporarily inherits the high priority until it releases the mutex.
 */
#include "freertos_priority_inheritance.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static SemaphoreHandle_t pi_mutex;

// Low priority task that holds the mutex
// NOTE: This task demonstrates how priority inheritance works
static void low_task(void *pvParameter) {
    while (1) {
        // FreeRTOS API: xSemaphoreTake - Takes a mutex with priority inheritance
        // Parameters: mutex handle, timeout (portMAX_DELAY = wait forever)
        // NOTE: When high_task tries to take this mutex, low_task will inherit high priority
        xSemaphoreTake(pi_mutex, portMAX_DELAY);
        printf("low_task: holding mutex (low priority)\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Hold mutex for a while
        xSemaphoreGive(pi_mutex);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Medium priority task that runs in the background
// NOTE: This task demonstrates normal priority scheduling
static void medium_task(void *pvParameter) {
    while (1) {
        printf("medium_task: running (medium priority)\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// High priority task that needs the mutex
// NOTE: This task demonstrates priority inheritance in action
static void high_task(void *pvParameter) {
    while (1) {
        vTaskDelay(200 / portTICK_PERIOD_MS); // Let low_task take mutex first
        printf("high_task: trying to take mutex (high priority)\n");
        
        // FreeRTOS API: xSemaphoreTake - Attempts to take mutex held by low-priority task
        // Parameters: mutex handle, timeout (portMAX_DELAY = wait forever)
        // NOTE: When this blocks, low_task will inherit high priority to complete faster
        xSemaphoreTake(pi_mutex, portMAX_DELAY);
        printf("high_task: got mutex!\n");
        xSemaphoreGive(pi_mutex);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void freertos_priority_inheritance_demo(void) {
    // FreeRTOS API: xSemaphoreCreateMutex - Creates a mutex with priority inheritance
    // Parameters: none (mutexes are always created in 'taken' state)
    // NOTE: Mutexes automatically implement priority inheritance to prevent priority inversion
    pi_mutex = xSemaphoreCreateMutex();
    
    // FreeRTOS API: xTaskCreate - Creates tasks with different priorities
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Different priorities demonstrate priority inheritance behavior
    xTaskCreate(low_task, "low_task", 2048, NULL, 2, NULL);    // Low priority
    xTaskCreate(medium_task, "medium_task", 2048, NULL, 3, NULL); // Medium priority
    xTaskCreate(high_task, "high_task", 2048, NULL, 4, NULL);  // High priority
} 