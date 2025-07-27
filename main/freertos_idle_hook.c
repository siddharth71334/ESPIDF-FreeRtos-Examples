/*
 * FreeRTOS Idle Hook Demo
 * -----------------------
 * Demonstrates using the idle task hook for background/low-priority work.
 *
 * WHAT: The idle hook is a function called by the FreeRTOS idle task when no other tasks are runnable.
 * WHY: Useful for background cleanup, power management, or statistics.
 * WHEN: Use for lowest-priority, non-time-critical work.
 *
 * NOTE: You must enable configUSE_IDLE_HOOK in FreeRTOSConfig.h or menuconfig.
 * The idle hook runs in the context of the idle task, which has the lowest priority.
 * This is ideal for background processing that shouldn't interfere with normal task execution.
 */
#include "freertos_idle_hook.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Idle hook function. Must be named vApplicationIdleHook.
// NOTE: This function is called automatically by FreeRTOS when the system is idle
void vApplicationIdleHook(void) {
    static int count = 0;
    if (++count % 100000 == 0) {
        printf("Idle hook: running in background (count=%d)\n", count);
    }
}

void freertos_idle_hook_demo(void) {
    printf("Idle hook demo: Enable configUSE_IDLE_HOOK and watch for idle messages.\n");
    // No tasks needed; idle hook runs automatically when system is idle.
    // NOTE: The idle hook will run whenever no other tasks are ready to execute
} 