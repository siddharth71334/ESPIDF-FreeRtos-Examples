/*
 * FreeRTOS Task Notification Demo
 * ------------------------------
 * Demonstrates direct-to-task notifications for fast, lightweight signaling.
 *
 * WHAT: Task notifications are like lightweight binary/counting semaphores or event flags, built into every task.
 * WHY: Fastest way to signal a task from another task or ISR.
 * WHEN: Use for simple signaling or counting events between tasks/ISRs.
 */
#include "freertos_task_notify.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static TaskHandle_t notify_task_handle = NULL;

static void notifier_task(void *pvParameter) {
    while (1) {
        vTaskDelay(800 / portTICK_PERIOD_MS);
        xTaskNotifyGive(notify_task_handle);
        printf("notifier_task: sent notification\n");
    }
}

static void notified_task(void *pvParameter) {
    uint32_t count = 0;
    notify_task_handle = xTaskGetCurrentTaskHandle();
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        count++;
        printf("notified_task: got notification #%lu\n", count);
    }
}

void freertos_task_notify_demo(void) {
    xTaskCreate(notified_task, "notified_task", 2048, NULL, 5, NULL);
    xTaskCreate(notifier_task, "notifier_task", 2048, NULL, 4, NULL);
} 