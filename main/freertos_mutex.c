/*
 * FreeRTOS Mutex Demo
 * -------------------
 * Demonstrates the use of a mutex to protect a shared resource (UART/printf).
 *
 * WHAT: Mutexes are used to ensure only one task accesses a resource at a time.
 * WHY: Prevents data corruption and race conditions when multiple tasks share resources.
 * WHEN: Use when multiple tasks share a resource (e.g., UART, I2C, SPI, shared variable).
 *
 * NOTE: Mutexes provide mutual exclusion - only one task can hold the mutex at a time.
 * Other tasks trying to take the mutex will block until it's released.
 */
#include "freertos_mutex.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static SemaphoreHandle_t print_mutex;

// Task 1: Uses mutex to safely access shared resource (printf)
// NOTE: This task demonstrates proper mutex usage pattern: take -> use -> give
static void mutex_task1(void *pvParameter) {
    while (1) {
        // FreeRTOS API: xSemaphoreTake - Takes (locks) a mutex, blocks if already taken
        // Parameters: mutex handle, timeout (portMAX_DELAY = wait forever)
        // NOTE: This ensures exclusive access to the shared resource (printf)
        xSemaphoreTake(print_mutex, portMAX_DELAY);
        
        printf("Task 1: Printing safely with mutex!\n");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        
        // FreeRTOS API: xSemaphoreGive - Releases (unlocks) a mutex
        // Parameters: mutex handle
        // NOTE: Always release the mutex after using the shared resource
        xSemaphoreGive(print_mutex);
        
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

// Task 2: Also uses mutex to safely access shared resource (printf)
// NOTE: This task will block when Task 1 has the mutex, preventing resource conflicts
static void mutex_task2(void *pvParameter) {
    while (1) {
        // FreeRTOS API: xSemaphoreTake - Takes (locks) a mutex, blocks if already taken
        // Parameters: mutex handle, timeout (portMAX_DELAY = wait forever)
        // NOTE: This task will wait here if Task 1 is currently printing
        xSemaphoreTake(print_mutex, portMAX_DELAY);
        
        printf("Task 2: Printing safely with mutex!\n");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        
        // FreeRTOS API: xSemaphoreGive - Releases (unlocks) a mutex
        // Parameters: mutex handle
        // NOTE: Releasing allows other tasks to access the shared resource
        xSemaphoreGive(print_mutex);
        
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

void freertos_mutex_demo(void) {
    // FreeRTOS API: xSemaphoreCreateMutex - Creates a new mutex
    // Parameters: none (mutexes are always created in the 'taken' state)
    // NOTE: Mutexes are binary semaphores with additional safety features
    print_mutex = xSemaphoreCreateMutex();
    
    // FreeRTOS API: xTaskCreate - Creates tasks that will compete for the mutex
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Both tasks have same priority, so they will compete fairly for the mutex
    xTaskCreate(mutex_task1, "mutex_task1", 2048, NULL, 5, NULL);
    xTaskCreate(mutex_task2, "mutex_task2", 2048, NULL, 5, NULL);
} 