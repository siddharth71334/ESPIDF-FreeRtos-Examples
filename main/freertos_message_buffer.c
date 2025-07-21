/*
 * FreeRTOS Message Buffer Demo
 * ----------------------------
 * Demonstrates message buffers for sending discrete messages between tasks.
 *
 * WHAT: Message buffers allow sending/receiving variable-length messages (each with its own length).
 * WHY: Useful for command passing, packets, or any message-based protocol.
 * WHEN: Use when you need to send discrete messages (not just streams) between tasks or ISRs.
 */
#include "freertos_message_buffer.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"

static MessageBufferHandle_t msg_buf;

static void msg_sender_task(void *pvParameter) {
    const char *msgs[] = {"Msg1", "Msg2: Hello", "Msg3: FreeRTOS"};
    int idx = 0;
    while (1) {
        size_t len = strlen(msgs[idx]);
        xMessageBufferSend(msg_buf, msgs[idx], len, portMAX_DELAY);
        printf("msg_sender: sent '%s'\n", msgs[idx]);
        idx = (idx + 1) % 3;
        vTaskDelay(1200 / portTICK_PERIOD_MS);
    }
}

static void msg_receiver_task(void *pvParameter) {
    char buf[32];
    while (1) {
        size_t rcvd = xMessageBufferReceive(msg_buf, buf, sizeof(buf)-1, portMAX_DELAY);
        buf[rcvd] = '\0';
        printf("msg_receiver: got '%s'\n", buf);
    }
}

void freertos_message_buffer_demo(void) {
    msg_buf = xMessageBufferCreate(64); // 64 bytes
    xTaskCreate(msg_sender_task, "msg_sender", 2048, NULL, 4, NULL);
    xTaskCreate(msg_receiver_task, "msg_receiver", 2048, NULL, 5, NULL);
} 