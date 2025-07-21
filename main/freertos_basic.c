/*
 * Basic FreeRTOS Demo
 * -------------------
 * This demo shows how to create and run multiple FreeRTOS tasks.
 * Two tasks blink the same LED at different rates, demonstrating task scheduling and preemption.
 *
 * Concepts covered:
 * - Task creation (xTaskCreate)
 * - Task scheduling
 * - GPIO output
 * - Print/log statements
 */
#include "freertos_basic.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

// Use the GPIO defined in menuconfig or sdkconfig
#define BASIC_BLINK_GPIO CONFIG_BLINK_GPIO

static const char *TAG_BASIC = "freertos_basic";

// Task 1: Blinks the LED every 500ms
static void basic_blink_task1(void *pvParameter) {
    while (1) {
        ESP_LOGI(TAG_BASIC, "Task1: LED ON");
        gpio_set_level(BASIC_BLINK_GPIO, 1); // Turn LED ON
        vTaskDelay(500 / portTICK_PERIOD_MS); // Wait 500ms
        ESP_LOGI(TAG_BASIC, "Task1: LED OFF");
        gpio_set_level(BASIC_BLINK_GPIO, 0); // Turn LED OFF
        vTaskDelay(500 / portTICK_PERIOD_MS); // Wait 500ms
    }
}

// Task 2: Blinks the LED every 200ms
static void basic_blink_task2(void *pvParameter) {
    while (1) {
        ESP_LOGI(TAG_BASIC, "Task2: LED ON");
        gpio_set_level(BASIC_BLINK_GPIO, 1); // Turn LED ON
        vTaskDelay(200 / portTICK_PERIOD_MS); // Wait 200ms
        ESP_LOGI(TAG_BASIC, "Task2: LED OFF");
        gpio_set_level(BASIC_BLINK_GPIO, 0); // Turn LED OFF
        vTaskDelay(200 / portTICK_PERIOD_MS); // Wait 200ms
    }
}

// Entry point for the basic FreeRTOS demo
void freertos_basic_demo(void) {
    ESP_LOGI(TAG_BASIC, "Configuring GPIO for basic demo");
    gpio_reset_pin(BASIC_BLINK_GPIO); // Reset the LED GPIO
    gpio_set_direction(BASIC_BLINK_GPIO, GPIO_MODE_OUTPUT); // Set as output
    // Create two tasks with the same priority
    xTaskCreate(basic_blink_task1, "basic_blink_task1", 2048, NULL, 5, NULL);
    xTaskCreate(basic_blink_task2, "basic_blink_task2", 2048, NULL, 5, NULL);
    // Both tasks will run forever, blinking the LED at different rates
} 