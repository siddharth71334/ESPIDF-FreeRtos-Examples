/*
 * FreeRTOS Message Buffer Demo
 * ----------------------------
 * Demonstrates message buffers for sending discrete messages between tasks.
 *
 * WHAT: Message buffers allow sending/receiving variable-length messages (each with its own length).
 * WHY: Useful for command passing, packets, or any message-based protocol.
 * WHEN: Use when you need to send discrete messages (not just streams) between tasks or ISRs.
 *
 * NOTE: Message buffers preserve message boundaries, making them ideal for protocol
 * implementations where each message has a specific structure and length.
 */
#include "freertos_message_buffer.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"

static MessageBufferHandle_t msg_buf;

// Task that sends discrete messages to the message buffer
// NOTE: This task demonstrates sending variable-length messages
static void msg_sender_task(void *pvParameter) {
    const char *msgs[] = {"Msg1", "Msg2: Hello", "Msg3: FreeRTOS"};
    int idx = 0;
    while (1) {
        size_t len = strlen(msgs[idx]);
        
        // FreeRTOS API: xMessageBufferSend - Sends a discrete message to the buffer
        // Parameters: message buffer handle, message data, message length, timeout
        // NOTE: The entire message is sent as a unit, preserving message boundaries
        xMessageBufferSend(msg_buf, msgs[idx], len, portMAX_DELAY);
        printf("msg_sender: sent '%s'\n", msgs[idx]);
        idx = (idx + 1) % 3;
        vTaskDelay(1200 / portTICK_PERIOD_MS);
    }
}

// Task that receives discrete messages from the message buffer
// NOTE: This task demonstrates receiving complete messages with their original length
static void msg_receiver_task(void *pvParameter) {
    char buf[32];
    while (1) {
        // FreeRTOS API: xMessageBufferReceive - Receives a complete message from the buffer
        // Parameters: message buffer handle, receive buffer, buffer size, timeout
        // NOTE: Returns the actual length of the received message
        size_t rcvd = xMessageBufferReceive(msg_buf, buf, sizeof(buf)-1, portMAX_DELAY);
        buf[rcvd] = '\0'; // Null-terminate the received string
        printf("msg_receiver: got '%s'\n", buf);
    }
}

void freertos_message_buffer_demo(void) {
    // FreeRTOS API: xMessageBufferCreate - Creates a message buffer
    // Parameters: buffer size in bytes
    // NOTE: Message buffers store both message data and length information
    msg_buf = xMessageBufferCreate(64); // 64 bytes total capacity
    
    // FreeRTOS API: xTaskCreate - Creates tasks that use the message buffer
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Message buffers are ideal for protocol implementations
    xTaskCreate(msg_sender_task, "msg_sender", 2048, NULL, 4, NULL);
    xTaskCreate(msg_receiver_task, "msg_receiver", 2048, NULL, 5, NULL);
} 