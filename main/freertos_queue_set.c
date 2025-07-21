/*
 * FreeRTOS Queue Set Demo
 * -----------------------
 * Demonstrates queue sets: waiting on multiple queues/semaphores at once.
 *
 * WHAT: Queue sets let a task block on multiple queues/semaphores and handle whichever is ready first.
 * WHY: Useful for event-driven tasks that need to handle multiple sources.
 * WHEN: Use when a task must wait for messages from several sources.
 */
#include "freertos_queue_set.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

static QueueHandle_t queue1, queue2;
static QueueSetHandle_t queue_set;

static void sender_task1(void *pvParameter) {
    int val = 1;
    while (1) {
        vTaskDelay(700 / portTICK_PERIOD_MS);
        xQueueSend(queue1, &val, 0);
        printf("sender_task1: sent to queue1\n");
    }
}

static void sender_task2(void *pvParameter) {
    int val = 2;
    while (1) {
        vTaskDelay(1200 / portTICK_PERIOD_MS);
        xQueueSend(queue2, &val, 0);
        printf("sender_task2: sent to queue2\n");
    }
}

static void queue_set_receiver_task(void *pvParameter) {
    QueueSetMemberHandle_t activated;
    int val;
    while (1) {
        activated = xQueueSelectFromSet(queue_set, portMAX_DELAY);
        if (activated == queue1) {
            xQueueReceive(queue1, &val, 0);
            printf("queue_set_receiver: got %d from queue1\n", val);
        } else if (activated == queue2) {
            xQueueReceive(queue2, &val, 0);
            printf("queue_set_receiver: got %d from queue2\n", val);
        }
    }
}

void freertos_queue_set_demo(void) {
    queue1 = xQueueCreate(5, sizeof(int));
    queue2 = xQueueCreate(5, sizeof(int));
    queue_set = xQueueCreateSet(10);
    xQueueAddToSet(queue1, queue_set);
    xQueueAddToSet(queue2, queue_set);
    xTaskCreate(sender_task1, "sender_task1", 2048, NULL, 4, NULL);
    xTaskCreate(sender_task2, "sender_task2", 2048, NULL, 4, NULL);
    xTaskCreate(queue_set_receiver_task, "queue_set_receiver", 2048, NULL, 5, NULL);
} 