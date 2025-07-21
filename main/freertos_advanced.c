/*
 * Advanced FreeRTOS Demo
 * ----------------------
 * This demo shows advanced FreeRTOS features: software timers and event groups.
 * A software timer toggles the LED every second. Two tasks wait for the timer event using an event group.
 *
 * ---
 * WHAT are Software Timers and Event Groups?
 *
 * - Software Timer: A FreeRTOS object that calls a function (callback) at a set interval or after a delay, managed by the RTOS (not hardware).
 *   - Example: Toggle an LED every 1 second, or trigger a periodic task.
 *
 * - Event Group: A FreeRTOS object for synchronizing tasks using bits (flags). Tasks can wait for one or more bits to be set by other tasks or ISRs.
 *   - Example: Multiple tasks wait for a "ready" signal, or for several events to occur before proceeding.
 *
 * ---
 * WHY use Software Timers and Event Groups?
 *
 * - Software Timers:
 *   - Avoids using hardware timers for simple periodic actions.
 *   - Lets you schedule actions without blocking a task (no vTaskDelay needed).
 *   - Useful for timeouts, periodic checks, or delayed actions.
 *
 * - Event Groups:
 *   - Allows multiple tasks to synchronize on events (e.g., all must wait for a resource to be ready).
 *   - Efficiently signals many tasks at once (broadcast events).
 *   - Can wait for multiple conditions to be true before proceeding.
 *
 * ---
 * WHEN to use them?
 *
 * - Use a Software Timer when:
 *   - You need to run code at regular intervals, but don't want a dedicated task just for timing.
 *   - You want to trigger an action after a delay (e.g., timeout, retry).
 *   - You want to avoid blocking tasks with vTaskDelay for periodic actions.
 *
 * - Use an Event Group when:
 *   - You need to synchronize two or more tasks on one or more events.
 *   - You want to signal multiple tasks at once (e.g., "start now!").
 *   - You need to wait for several conditions to be true before proceeding.
 *
 * ---
 * ADVANTAGES:
 * - Software timers save CPU and memory by sharing a timer task for many timers.
 * - Event groups are more efficient than polling or using many semaphores for multi-task sync.
 *
 * ---
 * Concepts covered:
 * - Software timers
 * - Event groups for task synchronization
 * - Task creation
 * - GPIO output
 * - Print/log statements
 */
#include "freertos_advanced.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

// Use the GPIO defined in menuconfig or sdkconfig for LED
#define ADVANCED_BLINK_GPIO CONFIG_BLINK_GPIO
// Event bit used for synchronization
#define ADVANCED_EVENT_BIT (1 << 0)

static const char *TAG_ADVANCED = "freertos_advanced";
static EventGroupHandle_t sync_event_group;
static TimerHandle_t blink_timer;

// Timer callback: toggles the LED and sets the event bit
static void advanced_timer_callback(TimerHandle_t xTimer) {
    static int led_state = 0;
    led_state = !led_state;
    gpio_set_level(ADVANCED_BLINK_GPIO, led_state); // Toggle LED
    ESP_LOGI(TAG_ADVANCED, "Timer: LED %s", led_state ? "ON" : "OFF");
    // Set event bit to notify tasks
    xEventGroupSetBits(sync_event_group, ADVANCED_EVENT_BIT);
}

// Task 1: Waits for the event bit and logs when LED toggles
static void advanced_task1(void *pvParameter) {
    while (1) {
        // Wait for the event bit to be set by the timer
        xEventGroupWaitBits(sync_event_group, ADVANCED_EVENT_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
        ESP_LOGI(TAG_ADVANCED, "Task1: Detected LED toggle event");
    }
}

// Task 2: Waits for the event bit and logs when LED toggles
static void advanced_task2(void *pvParameter) {
    while (1) {
        // Wait for the event bit to be set by the timer
        xEventGroupWaitBits(sync_event_group, ADVANCED_EVENT_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
        ESP_LOGI(TAG_ADVANCED, "Task2: Detected LED toggle event");
    }
}

// Entry point for the advanced FreeRTOS demo
void freertos_advanced_demo(void) {
    ESP_LOGI(TAG_ADVANCED, "Configuring GPIO for advanced demo");
    // Configure LED GPIO as output
    gpio_reset_pin(ADVANCED_BLINK_GPIO);
    gpio_set_direction(ADVANCED_BLINK_GPIO, GPIO_MODE_OUTPUT);
    // Create event group for task synchronization
    sync_event_group = xEventGroupCreate();
    // Create a periodic software timer (1 second)
    blink_timer = xTimerCreate("blink_timer", pdMS_TO_TICKS(1000), pdTRUE, NULL, advanced_timer_callback);
    // Create two tasks that wait for the event bit
    xTaskCreate(advanced_task1, "advanced_task1", 2048, NULL, 5, NULL);
    xTaskCreate(advanced_task2, "advanced_task2", 2048, NULL, 5, NULL);
    // Start the timer
    xTimerStart(blink_timer, 0);
} 