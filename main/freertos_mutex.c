/*
 * FreeRTOS Mutex Demo
 * -------------------
 * Demonstrates the use of a mutex to protect a shared resource (UART/printf).
 *
 * WHAT: Mutexes are used to ensure only one task accesses a resource at a time.
 * WHY: Prevents data corruption and race conditions.
 * WHEN: Use when multiple tasks share a resource (e.g., UART, I2C, SPI, shared variable).
 */
#include "freertos_mutex.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static SemaphoreHandle_t print_mutex;

static void mutex_task1(void *pvParameter) {
    while (1) {
        xSemaphoreTake(print_mutex, portMAX_DELAY);
        printf("Task 1: Printing safely with mutex!\n");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        xSemaphoreGive(print_mutex);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

static void mutex_task2(void *pvParameter) {
    while (1) {
        xSemaphoreTake(print_mutex, portMAX_DELAY);
        printf("Task 2: Printing safely with mutex!\n");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        xSemaphoreGive(print_mutex);
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

void freertos_mutex_demo(void) {
    print_mutex = xSemaphoreCreateMutex();
    xTaskCreate(mutex_task1, "mutex_task1", 2048, NULL, 5, NULL);
    xTaskCreate(mutex_task2, "mutex_task2", 2048, NULL, 5, NULL);
} 