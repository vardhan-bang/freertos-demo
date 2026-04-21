# freertos-demo
Demonstration of two concurrent FreeRTOS tasks on ESP32 with logging over UART

### Usage
- Replace the `main` of an ESP-EDF example folder with the files in this repository. Then build, flash and monitor your ESP32 device.
- To demonstrate handling of queue overflow, change the delay of the consumer task to 3000ms or more.