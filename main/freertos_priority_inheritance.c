/*
 * FreeRTOS Priority Inheritance Demo
 * ----------------------------------
 * Demonstrates priority inheritance with mutexes to avoid priority inversion.
 *
 * WHAT: Priority inheritance boosts the priority of a low-priority task holding a mutex needed by a higher-priority task.
 * WHY: Prevents 'priority inversion' where a high-priority task is blocked by a low-priority one.
 * WHEN: Use when sharing resources between tasks of different priorities.
 */
#include "freertos_priority_inheritance.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static SemaphoreHandle_t pi_mutex;

static void low_task(void *pvParameter) {
    while (1) {
        xSemaphoreTake(pi_mutex, portMAX_DELAY);
        printf("low_task: holding mutex (low priority)\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Hold mutex for a while
        xSemaphoreGive(pi_mutex);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void medium_task(void *pvParameter) {
    while (1) {
        printf("medium_task: running (medium priority)\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void high_task(void *pvParameter) {
    while (1) {
        vTaskDelay(200 / portTICK_PERIOD_MS); // Let low_task take mutex first
        printf("high_task: trying to take mutex (high priority)\n");
        xSemaphoreTake(pi_mutex, portMAX_DELAY);
        printf("high_task: got mutex!\n");
        xSemaphoreGive(pi_mutex);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void freertos_priority_inheritance_demo(void) {
    pi_mutex = xSemaphoreCreateMutex();
    xTaskCreate(low_task, "low_task", 2048, NULL, 2, NULL);    // Low priority
    xTaskCreate(medium_task, "medium_task", 2048, NULL, 3, NULL); // Medium priority
    xTaskCreate(high_task, "high_task", 2048, NULL, 4, NULL);  // High priority
} 