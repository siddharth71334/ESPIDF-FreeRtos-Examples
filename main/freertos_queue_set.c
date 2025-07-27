/*
 * FreeRTOS Queue Set Demo
 * -----------------------
 * Demonstrates queue sets: waiting on multiple queues/semaphores at once.
 *
 * WHAT: Queue sets let a task block on multiple queues/semaphores and handle whichever is ready first.
 * WHY: Useful for event-driven tasks that need to handle multiple sources.
 * WHEN: Use when a task must wait for messages from several sources.
 *
 * NOTE: Queue sets are powerful for event-driven architectures where a task needs to
 * respond to events from multiple sources without polling or using multiple tasks.
 */
#include "freertos_queue_set.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

static QueueHandle_t queue1, queue2;
static QueueSetHandle_t queue_set;

// Task that sends data to queue1
// NOTE: This task simulates one event source
static void sender_task1(void *pvParameter) {
    int val = 1;
    while (1) {
        vTaskDelay(700 / portTICK_PERIOD_MS);
        
        // FreeRTOS API: xQueueSend - Sends data to a queue
        // Parameters: queue handle, data pointer, timeout (0 = non-blocking)
        // NOTE: This simulates an event from source 1
        xQueueSend(queue1, &val, 0);
        printf("sender_task1: sent to queue1\n");
    }
}

// Task that sends data to queue2
// NOTE: This task simulates another event source
static void sender_task2(void *pvParameter) {
    int val = 2;
    while (1) {
        vTaskDelay(1200 / portTICK_PERIOD_MS);
        
        // FreeRTOS API: xQueueSend - Sends data to a queue
        // Parameters: queue handle, data pointer, timeout (0 = non-blocking)
        // NOTE: This simulates an event from source 2
        xQueueSend(queue2, &val, 0);
        printf("sender_task2: sent to queue2\n");
    }
}

// Task that waits for events from multiple sources using queue set
// NOTE: This task demonstrates the main advantage of queue sets - waiting on multiple sources
static void queue_set_receiver_task(void *pvParameter) {
    QueueSetMemberHandle_t activated;
    int val;
    while (1) {
        // FreeRTOS API: xQueueSelectFromSet - Waits for any queue in the set to have data
        // Parameters: queue set handle, timeout (portMAX_DELAY = wait forever)
        // NOTE: This blocks until ANY queue in the set has data available
        activated = xQueueSelectFromSet(queue_set, portMAX_DELAY);
        
        if (activated == queue1) {
            // FreeRTOS API: xQueueReceive - Receives data from queue1
            // Parameters: queue handle, data buffer, timeout (0 = non-blocking)
            // NOTE: Since we know queue1 has data, we use non-blocking receive
            xQueueReceive(queue1, &val, 0);
            printf("queue_set_receiver: got %d from queue1\n", val);
        } else if (activated == queue2) {
            // FreeRTOS API: xQueueReceive - Receives data from queue2
            // Parameters: queue handle, data buffer, timeout (0 = non-blocking)
            // NOTE: Since we know queue2 has data, we use non-blocking receive
            xQueueReceive(queue2, &val, 0);
            printf("queue_set_receiver: got %d from queue2\n", val);
        }
    }
}

void freertos_queue_set_demo(void) {
    // FreeRTOS API: xQueueCreate - Creates individual queues
    // Parameters: queue length, item size in bytes
    // NOTE: Each queue can hold up to 5 items
    queue1 = xQueueCreate(5, sizeof(int));
    queue2 = xQueueCreate(5, sizeof(int));
    
    // FreeRTOS API: xQueueCreateSet - Creates a queue set
    // Parameters: total number of queue positions across all queues
    // NOTE: Queue set can monitor multiple queues simultaneously
    queue_set = xQueueCreateSet(10);
    
    // FreeRTOS API: xQueueAddToSet - Adds a queue to the queue set
    // Parameters: queue handle, queue set handle
    // NOTE: Only queues (not semaphores) can be added to queue sets
    xQueueAddToSet(queue1, queue_set);
    xQueueAddToSet(queue2, queue_set);
    
    // FreeRTOS API: xTaskCreate - Creates tasks that use the queue set
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Different priorities ensure proper event handling
    xTaskCreate(sender_task1, "sender_task1", 2048, NULL, 4, NULL);
    xTaskCreate(sender_task2, "sender_task2", 2048, NULL, 4, NULL);
    xTaskCreate(queue_set_receiver_task, "queue_set_receiver", 2048, NULL, 5, NULL);
} 