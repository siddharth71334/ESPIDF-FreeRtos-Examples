/*
 * FreeRTOS Semaphore Demo
 * -----------------------
 * Demonstrates binary and counting semaphores for signaling and resource management.
 *
 * WHAT: Semaphores are used for signaling (binary) and managing resource pools (counting).
 * WHY: Useful for task synchronization and limiting access to multiple resources.
 * WHEN: Use binary semaphores for signaling (e.g., ISR to task), counting for resource pools.
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
static void isr_simulator_task(void *pvParameter) {
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        xSemaphoreGive(bin_sem);
        printf("ISR Simulator: Gave binary semaphore\n");
    }
}

// Task waits for binary semaphore (simulates waiting for ISR)
static void bin_sem_task(void *pvParameter) {
    while (1) {
        if (xSemaphoreTake(bin_sem, portMAX_DELAY) == pdTRUE) {
            printf("bin_sem_task: Got binary semaphore!\n");
        }
    }
}

// Task tries to take from a pool of 3 resources (counting semaphore)
static void count_sem_task(void *pvParameter) {
    while (1) {
        if (xSemaphoreTake(count_sem, 500 / portTICK_PERIOD_MS) == pdTRUE) {
            printf("count_sem_task: Got resource from pool\n");
            vTaskDelay(700 / portTICK_PERIOD_MS);
            xSemaphoreGive(count_sem);
            printf("count_sem_task: Released resource\n");
        } else {
            printf("count_sem_task: No resource available\n");
        }
    }
}

void freertos_semaphore_demo(void) {
    bin_sem = xSemaphoreCreateBinary();
    count_sem = xSemaphoreCreateCounting(3, 3); // Pool of 3 resources
    xTaskCreate(isr_simulator_task, "isr_simulator_task", 2048, NULL, 5, NULL);
    xTaskCreate(bin_sem_task, "bin_sem_task", 2048, NULL, 5, NULL);
    xTaskCreate(count_sem_task, "count_sem_task", 2048, NULL, 4, NULL);
} 