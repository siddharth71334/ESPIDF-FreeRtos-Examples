# ESP-IDF FreeRTOS Educational Demo Project

This project demonstrates FreeRTOS concepts on ESP32/ESP-IDF, progressing from beginner to advanced. Each level is implemented in its own source files, with detailed comments to help students understand how FreeRTOS works in real embedded applications.

## Project Structure

- `main/freertos_basic.c` / `.h`: Basic FreeRTOS demo (task creation, scheduling, LED blink)
- `main/freertos_intermediate.c` / `.h`: Intermediate demo (queues, button input, ISR, inter-task communication)
- `main/freertos_advanced.c` / `.h`: Advanced demo (software timers, event groups, task synchronization)
- `main/blink_example_main.c`: Main entry point. Select which demo to run by uncommenting a line.

## How to Use

1. **Select a Demo**
   - Open `main/blink_example_main.c`.
   - Uncomment **one** of the following lines to choose which demo to run:
     ```c
     #define RUN_FREERTOS_BASIC_DEMO
     // #define RUN_FREERTOS_INTERMEDIATE_DEMO
     // #define RUN_FREERTOS_ADVANCED_DEMO
     ```
   - Only one demo should be active at a time.

2. **Build and Flash**
   - Use ESP-IDF commands:
     ```sh
     idf.py build
     idf.py -p (PORT) flash monitor
     ```
   - Replace `(PORT)` with your serial port (e.g., `/dev/ttyUSB0` or `COM3`).

3. **Observe Output**
   - Open the serial monitor to see log messages and interact with the board (e.g., press the BOOT button for the intermediate demo).

## Demo Details

### 1. Basic FreeRTOS Demo
- **Files:** `freertos_basic.c/h`
- **Concepts:** Task creation, scheduling, GPIO output
- **What it does:**
  - Creates two tasks that blink the same LED at different rates (500ms and 200ms).
  - Shows how FreeRTOS schedules multiple tasks.

### 2. Intermediate FreeRTOS Demo
- **Files:** `freertos_intermediate.c/h`
- **Concepts:** Queues, ISR, inter-task communication, GPIO input/output
- **What it does:**
  - Uses the BOOT button (GPIO0) as input.
  - When the button is pressed, an interrupt sends an event to a queue.
  - A task receives the event and toggles the LED.
  - Demonstrates safe communication between an ISR and a task.

### 3. Advanced FreeRTOS Demo
- **Files:** `freertos_advanced.c/h`
- **Concepts:** Software timers, event groups, task synchronization
- **What it does:**
  - A software timer toggles the LED every second.
  - Two tasks wait for the timer event using an event group and log when the LED toggles.
  - Shows advanced synchronization and timer usage in FreeRTOS.

## Customization
- You can change the LED GPIO in `menuconfig` or by editing `sdkconfig`/`CONFIG_BLINK_GPIO`.
- For more experiments, try modifying task priorities, stack sizes, or add your own tasks!

## Requirements
- ESP-IDF (latest recommended version)
- ESP32 or compatible board

## Learning Resources
- [ESP-IDF FreeRTOS Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
- [FreeRTOS Official Site](https://www.freertos.org/)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)

---

**Happy Learning!**

This project is designed for students and educators to explore and understand FreeRTOS on real hardware. All code is heavily commented for clarity. If you have questions or want to extend the demos, feel free to experiment!
