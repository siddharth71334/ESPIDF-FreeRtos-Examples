/*
 * Intermediate FreeRTOS Demo
 * --------------------------
 * This demo shows how to use a FreeRTOS queue to communicate between an interrupt (button press) and a task.
 * Pressing the button toggles the LED. Demonstrates inter-task communication and ISR handling.
 *
 * Concepts covered:
 * - Task creation and management
 * - Queue creation and use for inter-task communication
 * - GPIO input (button) and output (LED) configuration
 * - Interrupt Service Routine (ISR) implementation
 * - ISR to task communication using queues
 * - Print/log statements
 *
 * NOTE: This demonstrates a key FreeRTOS concept - safe communication between
 * interrupt context and task context using queues.
 */
#include "freertos_intermediate.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

// Use the GPIO defined in menuconfig or sdkconfig for LED
#define INTERMEDIATE_BLINK_GPIO CONFIG_BLINK_GPIO
// Use GPIO0 (BOOT button) for button input
#define INTERMEDIATE_BUTTON_GPIO 0

static const char *TAG_INTERMEDIATE = "freertos_intermediate";
static QueueHandle_t button_evt_queue = NULL;

typedef enum {
    BUTTON_PRESSED,
    BUTTON_RELEASED
} button_event_t;

// ISR: Called when button is pressed (falling edge)
// NOTE: ISR functions must be fast and cannot use most FreeRTOS APIs
// Only xQueueSendFromISR, xSemaphoreGiveFromISR, and xTaskNotifyFromISR are safe
static void IRAM_ATTR button_isr_handler(void* arg) {
    button_event_t evt = BUTTON_PRESSED;
    // FreeRTOS API: xQueueSendFromISR - Sends data to queue from ISR context
    // Parameters: queue handle, data pointer, higher priority task woken flag
    // NOTE: This is the ONLY safe way to communicate from ISR to task
    xQueueSendFromISR(button_evt_queue, &evt, NULL);
}

// Task: Waits for button press events and toggles the LED
// NOTE: This task demonstrates blocking on a queue to wait for events
static void button_task(void *pvParameter) {
    button_event_t evt;
    while (1) {
        // FreeRTOS API: xQueueReceive - Receives data from queue, blocks if empty
        // Parameters: queue handle, data buffer, timeout (portMAX_DELAY = wait forever)
        // NOTE: This task will block here until the ISR sends data to the queue
        if (xQueueReceive(button_evt_queue, &evt, portMAX_DELAY)) {
            ESP_LOGI(TAG_INTERMEDIATE, "Button pressed! Toggling LED.");
            int level = gpio_get_level(INTERMEDIATE_BLINK_GPIO);
            gpio_set_level(INTERMEDIATE_BLINK_GPIO, !level); // Toggle LED
        }
    }
}

// Entry point for the intermediate FreeRTOS demo
void freertos_intermediate_demo(void) {
    ESP_LOGI(TAG_INTERMEDIATE, "Configuring GPIO for intermediate demo");
    // Configure LED GPIO as output
    gpio_reset_pin(INTERMEDIATE_BLINK_GPIO);
    gpio_set_direction(INTERMEDIATE_BLINK_GPIO, GPIO_MODE_OUTPUT);
    // Configure button GPIO as input
    gpio_reset_pin(INTERMEDIATE_BUTTON_GPIO);
    gpio_set_direction(INTERMEDIATE_BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_intr_type(INTERMEDIATE_BUTTON_GPIO, GPIO_INTR_NEGEDGE); // Interrupt on falling edge
    
    // FreeRTOS API: xQueueCreate - Creates a new queue
    // Parameters: queue length, item size in bytes
    // NOTE: Queue length of 10 means it can hold 10 button events before blocking
    button_evt_queue = xQueueCreate(10, sizeof(button_event_t));
    
    // FreeRTOS API: xTaskCreate - Creates the button handling task
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Higher priority (10) ensures button events are handled promptly
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
    
    // Install ISR service and add handler for button GPIO
    gpio_install_isr_service(0);
    gpio_isr_handler_add(INTERMEDIATE_BUTTON_GPIO, button_isr_handler, NULL);
} 