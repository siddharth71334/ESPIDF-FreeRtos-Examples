/*
 * FreeRTOS Stream Buffer Demo
 * ---------------------------
 * Demonstrates stream buffers for variable-length data transfer between tasks.
 *
 * WHAT: Stream buffers allow sending/receiving streams of bytes (not discrete messages).
 * WHY: Useful for UART, audio, or any streaming data.
 * WHEN: Use when you need to send variable-length data between tasks or ISRs.
 *
 * NOTE: Stream buffers treat data as a continuous stream of bytes, unlike message buffers
 * which preserve message boundaries. Ideal for continuous data flow applications.
 */
#include "freertos_stream_buffer.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"
#include "esp_log.h"

static StreamBufferHandle_t stream_buf;

// Task that sends data to the stream buffer
// NOTE: This task demonstrates sending data as a continuous stream
static void stream_sender_task(void *pvParameter) {
    const char *msgs[] = {"Hello", "FreeRTOS", "StreamBuffer!"};
    int idx = 0;
    while (1) {
        size_t len = strlen(msgs[idx]);
        
        // FreeRTOS API: xStreamBufferSend - Sends data to the stream buffer
        // Parameters: stream buffer handle, data pointer, data length, timeout
        // NOTE: Data is sent as a stream of bytes without preserving message boundaries
        xStreamBufferSend(stream_buf, msgs[idx], len, portMAX_DELAY);
        printf("stream_sender: sent '%s'\n", msgs[idx]);
        idx = (idx + 1) % 3;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Task that receives data from the stream buffer
// NOTE: This task demonstrates receiving data from a continuous stream
static void stream_receiver_task(void *pvParameter) {
    char buf[32];
    while (1) {
        // FreeRTOS API: xStreamBufferReceive - Receives data from the stream buffer
        // Parameters: stream buffer handle, receive buffer, buffer size, timeout
        // NOTE: Returns the number of bytes actually received (may be less than requested)
        size_t rcvd = xStreamBufferReceive(stream_buf, buf, sizeof(buf)-1, portMAX_DELAY);
        buf[rcvd] = '\0'; // Null-terminate the received data
        printf("stream_receiver: got '%s'\n", buf);
    }
}

void freertos_stream_buffer_demo(void) {
    // FreeRTOS API: xStreamBufferCreate - Creates a stream buffer
    // Parameters: buffer size in bytes, trigger level (bytes that trigger receive)
    // NOTE: Trigger level determines when a waiting receiver is woken up
    stream_buf = xStreamBufferCreate(64, 4); // 64 bytes, trigger level 4
    
    // FreeRTOS API: xTaskCreate - Creates tasks that use the stream buffer
    // Parameters: task function, task name, stack size, parameters, priority, task handle
    // NOTE: Stream buffers are ideal for continuous data flow applications
    xTaskCreate(stream_sender_task, "stream_sender", 2048, NULL, 4, NULL);
    xTaskCreate(stream_receiver_task, "stream_receiver", 2048, NULL, 5, NULL);
} 