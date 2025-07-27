/*
 * FreeRTOS Recursive Mutex Demo
 * ----------------------------
 * Demonstrates recursive mutexes, which allow the same task to lock a mutex multiple times.
 *
 * WHAT: Recursive mutexes are like normal mutexes, but allow re-locking by the same task.
 * WHY: Useful for nested function calls that need the same lock.
 * WHEN: Use when a task may need to lock the same resource multiple times in a call chain.
 *
 * NOTE: Recursive mutexes maintain a count of how many times they've been taken by the same task.
 * The mutex is only released when the count reaches zero (same number of gives as takes).
 */
#include "freertos_recursive_mutex.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static SemaphoreHandle_t rec_mutex;

// Deep nested function that also needs the mutex
// NOTE: This function demonstrates nested locking - it takes the mutex again
void deep_function(void) {
    // FreeRTOS API: xSemaphoreTakeRecursive - Takes a recursive mutex (can be called multiple times by same task)
    // Parameters: recursive mutex handle, timeout
    // NOTE: This increases the lock count for the current task
    xSemaphoreTakeRecursive(rec_mutex, portMAX_DELAY);
    printf("deep_function: locked recursively\n");
    
    // FreeRTOS API: xSemaphoreGiveRecursive - Releases a recursive mutex
    // Parameters: recursive mutex handle
    // NOTE: This decreases the lock count for the current task
    xSemaphoreGiveRecursive(rec_mutex);
}

// Nested function that calls deep_function
// NOTE: This function also takes the mutex, demonstrating the recursive capability
void nested_function(void) {
    // FreeRTOS API: xSemaphoreTakeRecursive - Takes a recursive mutex
    // Parameters: recursive mutex handle, timeout
    // NOTE: This task can take the same mutex multiple times
    xSemaphoreTakeRecursive(rec_mutex, portMAX_DELAY);
    printf("nested_function: locked recursively\n");
    deep_function(); // This function will also take the same mutex
    xSemaphoreGiveRecursive(rec_mutex);
}

// Main task that demonstrates recursive mutex usage
// NOTE: This task shows the complete recursive locking pattern
static void rec_mutex_task(void *pvParameter) {
    while (1) {
        // FreeRTOS API: xSemaphoreTakeRecursive - Takes a recursive mutex
        // Parameters: recursive mutex handle, timeout
        // NOTE: This is the first take - other tasks will be blocked
        xSemaphoreTakeRecursive(rec_mutex, portMAX_DELAY);
        printf("rec_mutex_task: locked recursively\n");
        nested_function(); // This will take the mutex again (recursive)
        xSemaphoreGiveRecursive(rec_mutex);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// Task that tries to access the same mutex
// NOTE: This task will be blocked when the recursive mutex is held by another task
static void rec_mutex_blocked_task(void *pvParameter) {
    while (1) {
        // FreeRTOS API: xSemaphoreTakeRecursive - Attempts to take the recursive mutex
        // Parameters: recursive mutex handle, timeout (100ms = short timeout)
        // NOTE: This will fail if the mutex is held by another task
        if (xSemaphoreTakeRecursive(rec_mutex, 100 / portTICK_PERIOD_MS) == pdTRUE) {
            printf("blocked_task: got recursive mutex\n");
            xSemaphoreGiveRecursive(rec_mutex);
        } else {
            printf("blocked_task: waiting for recursive mutex\n");
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void freertos_recursive_mutex_demo(void) {
    // FreeRTOS API: xSemaphoreCreateRecursiveMutex - Creates a recursive mutex
    // Parameters: none (recursive mutexes are always created in 'taken' state)
    // NOTE: Recursive mutexes allow the same task to take them multiple times
    rec_mutex = xSemaphoreCreateRecursiveMutex();
    
    // FreeRTOS API: xTaskCreate - Creates tasks that demonstrate recursive mutex usage
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Different priorities show how recursive mutexes work with task scheduling
    xTaskCreate(rec_mutex_task, "rec_mutex_task", 2048, NULL, 5, NULL);
    xTaskCreate(rec_mutex_blocked_task, "rec_mutex_blocked_task", 2048, NULL, 4, NULL);
} 