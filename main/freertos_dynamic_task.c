/*
 * FreeRTOS Dynamic Task Demo
 * --------------------------
 * Demonstrates creating and deleting tasks at runtime.
 *
 * WHAT: Tasks can be created and deleted dynamically as needed.
 * WHY: Useful for temporary/background work, or to save memory.
 * WHEN: Use when a task is only needed for a short time or on demand.
 */
#include "freertos_dynamic_task.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static void temporary_task(void *pvParameter) {
    printf("temporary_task: running, will self-delete\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("temporary_task: deleting itself\n");
    vTaskDelete(NULL);
}

static void creator_task(void *pvParameter) {
    while (1) {
        printf("creator_task: creating temporary task\n");
        xTaskCreate(temporary_task, "temporary_task", 2048, NULL, 5, NULL);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void freertos_dynamic_task_demo(void) {
    xTaskCreate(creator_task, "creator_task", 2048, NULL, 4, NULL);
} 