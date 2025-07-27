/*
 * FreeRTOS Semaphore Demo
 * -----------------------
 * Demonstrates binary and counting semaphores for signaling and resource management.
 *
 * WHAT: Semaphores are used for signaling (binary) and managing resource pools (counting).
 * WHY: Useful for task synchronization and limiting access to multiple resources.
 * WHEN: Use binary semaphores for signaling (e.g., ISR to task), counting for resource pools.
 *
 * NOTE: Binary semaphores are like mutexes but without ownership - any task can give/take them.
 * Counting semaphores can have multiple tokens, useful for managing resource pools.
 */
#include "freertos_semaphore.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static SemaphoreHandle_t bin_sem;
static SemaphoreHandle_t count_sem;

// Simulate an ISR that gives a binary semaphore
// NOTE: This task simulates an ISR that would signal a task using a binary semaphore
static void isr_simulator_task(void *pvParameter) {
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        // FreeRTOS API: xSemaphoreGive - Gives (signals) a binary semaphore
        // Parameters: semaphore handle
        // NOTE: This simulates an ISR signaling that an event has occurred
        xSemaphoreGive(bin_sem);
        printf("ISR Simulator: Gave binary semaphore\n");
    }
}

// Task waits for binary semaphore (simulates waiting for ISR)
// NOTE: This task demonstrates waiting for events using binary semaphores
static void bin_sem_task(void *pvParameter) {
    while (1) {
        // FreeRTOS API: xSemaphoreTake - Takes (waits for) a binary semaphore
        // Parameters: semaphore handle, timeout (portMAX_DELAY = wait forever)
        // NOTE: This task blocks until the "ISR" signals an event
        if (xSemaphoreTake(bin_sem, portMAX_DELAY) == pdTRUE) {
            printf("bin_sem_task: Got binary semaphore!\n");
        }
    }
}

// Task tries to take from a pool of 3 resources (counting semaphore)
// NOTE: This task demonstrates resource pool management using counting semaphores
static void count_sem_task(void *pvParameter) {
    while (1) {
        // FreeRTOS API: xSemaphoreTake - Takes a token from counting semaphore
        // Parameters: semaphore handle, timeout (500ms = wait up to 500ms)
        // NOTE: Returns pdTRUE if token was available, pdFALSE if timeout occurred
        if (xSemaphoreTake(count_sem, 500 / portTICK_PERIOD_MS) == pdTRUE) {
            printf("count_sem_task: Got resource from pool\n");
            vTaskDelay(700 / portTICK_PERIOD_MS); // Simulate using the resource
            
            // FreeRTOS API: xSemaphoreGive - Returns a token to the counting semaphore
            // Parameters: semaphore handle
            // NOTE: This returns the resource to the pool for other tasks to use
            xSemaphoreGive(count_sem);
            printf("count_sem_task: Released resource\n");
        } else {
            printf("count_sem_task: No resource available\n");
        }
    }
}

void freertos_semaphore_demo(void) {
    // FreeRTOS API: xSemaphoreCreateBinary - Creates a binary semaphore
    // Parameters: none (binary semaphores are always created in 'empty' state)
    // NOTE: Binary semaphores can only have 0 or 1 tokens
    bin_sem = xSemaphoreCreateBinary();
    
    // FreeRTOS API: xSemaphoreCreateCounting - Creates a counting semaphore
    // Parameters: maximum count, initial count
    // NOTE: Pool of 3 resources, all initially available
    count_sem = xSemaphoreCreateCounting(3, 3); // Pool of 3 resources
    
    // FreeRTOS API: xTaskCreate - Creates tasks that use the semaphores
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Different priorities demonstrate different scheduling behaviors
    xTaskCreate(isr_simulator_task, "isr_simulator_task", 2048, NULL, 5, NULL);
    xTaskCreate(bin_sem_task, "bin_sem_task", 2048, NULL, 5, NULL);
    xTaskCreate(count_sem_task, "count_sem_task", 2048, NULL, 4, NULL);
} 