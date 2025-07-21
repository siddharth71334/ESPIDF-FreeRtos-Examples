/*
 * Advanced FreeRTOS Demo
 * ----------------------
 * This demo shows advanced FreeRTOS features: software timers and event groups.
 * A software timer toggles the LED every second. Two tasks wait for the timer event using an event group.
 *
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