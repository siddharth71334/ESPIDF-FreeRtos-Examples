/*
 * FreeRTOS Recursive Mutex Demo
 * ----------------------------
 * Demonstrates recursive mutexes, which allow the same task to lock a mutex multiple times.
 *
 * WHAT: Recursive mutexes are like normal mutexes, but allow re-locking by the same task.
 * WHY: Useful for nested function calls that need the same lock.
 * WHEN: Use when a task may need to lock the same resource multiple times in a call chain.
 */
#include "freertos_recursive_mutex.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static SemaphoreHandle_t rec_mutex;

void deep_function(void) {
    xSemaphoreTakeRecursive(rec_mutex, portMAX_DELAY);
    printf("deep_function: locked recursively\n");
    xSemaphoreGiveRecursive(rec_mutex);
}

void nested_function(void) {
    xSemaphoreTakeRecursive(rec_mutex, portMAX_DELAY);
    printf("nested_function: locked recursively\n");
    deep_function();
    xSemaphoreGiveRecursive(rec_mutex);
}

static void rec_mutex_task(void *pvParameter) {
    while (1) {
        xSemaphoreTakeRecursive(rec_mutex, portMAX_DELAY);
        printf("rec_mutex_task: locked recursively\n");
        nested_function();
        xSemaphoreGiveRecursive(rec_mutex);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void rec_mutex_blocked_task(void *pvParameter) {
    while (1) {
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
    rec_mutex = xSemaphoreCreateRecursiveMutex();
    xTaskCreate(rec_mutex_task, "rec_mutex_task", 2048, NULL, 5, NULL);
    xTaskCreate(rec_mutex_blocked_task, "rec_mutex_blocked_task", 2048, NULL, 4, NULL);
} 