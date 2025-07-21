/*
 * FreeRTOS Stream Buffer Demo
 * ---------------------------
 * Demonstrates stream buffers for variable-length data transfer between tasks.
 *
 * WHAT: Stream buffers allow sending/receiving streams of bytes (not discrete messages).
 * WHY: Useful for UART, audio, or any streaming data.
 * WHEN: Use when you need to send variable-length data between tasks or ISRs.
 */
#include "freertos_stream_buffer.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"
#include "esp_log.h"

static StreamBufferHandle_t stream_buf;

static void stream_sender_task(void *pvParameter) {
    const char *msgs[] = {"Hello", "FreeRTOS", "StreamBuffer!"};
    int idx = 0;
    while (1) {
        size_t len = strlen(msgs[idx]);
        xStreamBufferSend(stream_buf, msgs[idx], len, portMAX_DELAY);
        printf("stream_sender: sent '%s'\n", msgs[idx]);
        idx = (idx + 1) % 3;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void stream_receiver_task(void *pvParameter) {
    char buf[32];
    while (1) {
        size_t rcvd = xStreamBufferReceive(stream_buf, buf, sizeof(buf)-1, portMAX_DELAY);
        buf[rcvd] = '\0';
        printf("stream_receiver: got '%s'\n", buf);
    }
}

void freertos_stream_buffer_demo(void) {
    stream_buf = xStreamBufferCreate(64, 4); // 64 bytes, trigger level 4
    xTaskCreate(stream_sender_task, "stream_sender", 2048, NULL, 4, NULL);
    xTaskCreate(stream_receiver_task, "stream_receiver", 2048, NULL, 5, NULL);
} 