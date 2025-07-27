/*
 * FreeRTOS Task Notification Demo
 * ------------------------------
 * Demonstrates direct-to-task notifications for fast, lightweight signaling.
 *
 * WHAT: Task notifications are like lightweight binary/counting semaphores or event flags, built into every task.
 * WHY: Fastest way to signal a task from another task or ISR.
 * WHEN: Use for simple signaling or counting events between tasks/ISRs.
 *
 * NOTE: Task notifications are the most efficient signaling mechanism in FreeRTOS.
 * Each task has a built-in notification value that can be used for signaling without
 * creating separate semaphore objects.
 */
#include "freertos_task_notify.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static TaskHandle_t notify_task_handle = NULL;

// Task that sends notifications to another task
// NOTE: This task demonstrates how to send notifications to a specific task
static void notifier_task(void *pvParameter) {
    while (1) {
        vTaskDelay(800 / portTICK_PERIOD_MS);
        
        // FreeRTOS API: xTaskNotifyGive - Sends a notification to a specific task
        // Parameters: task handle to notify
        // NOTE: This increments the notification value of the target task
        xTaskNotifyGive(notify_task_handle);
        printf("notifier_task: sent notification\n");
    }
}

// Task that receives notifications
// NOTE: This task demonstrates how to wait for and process notifications
static void notified_task(void *pvParameter) {
    uint32_t count = 0;
    
    // FreeRTOS API: xTaskGetCurrentTaskHandle - Gets the handle of the current task
    // Parameters: none
    // NOTE: This allows the task to pass its handle to other tasks for notifications
    notify_task_handle = xTaskGetCurrentTaskHandle();
    
    while (1) {
        // FreeRTOS API: ulTaskNotifyTake - Waits for and takes a notification
        // Parameters: clear on exit flag, timeout (portMAX_DELAY = wait forever)
        // NOTE: pdTRUE means clear the notification value after taking it
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        count++;
        printf("notified_task: got notification #%lu\n", count);
    }
}

void freertos_task_notify_demo(void) {
    // FreeRTOS API: xTaskCreate - Creates tasks that use task notifications
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Task notifications are built into every task - no additional objects needed
    xTaskCreate(notified_task, "notified_task", 2048, NULL, 5, NULL);
    xTaskCreate(notifier_task, "notifier_task", 2048, NULL, 4, NULL);
} 