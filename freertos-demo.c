#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_PIN     2
#define QUEUE_DEPTH 5
#define TAG_PROD    "PRODUCER"
#define TAG_CONS    "CONSUMER"

static QueueHandle_t data_queue;

void producer_task(void *pvParameters) {
    uint32_t counter = 0;
    while (1) {
        counter++;
        BaseType_t sent = xQueueSend(data_queue, &counter, 0);
        if (sent != pdTRUE) {
            ESP_LOGW(TAG_PROD, "Queue full — dropped counter: %lu", counter);
        } else {
            gpio_set_level(LED_PIN, counter % 2);
            ESP_LOGI(TAG_PROD, "Enqueued: %lu", counter);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void consumer_task(void *pvParameters) {
    uint32_t received;
    while (1) {
        if (xQueueReceive(data_queue, &received, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG_CONS, "Received: %lu", received);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void) {
    // Configure LED GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // Create queue
    data_queue = xQueueCreate(QUEUE_DEPTH, sizeof(uint32_t));

    // Create tasks
    xTaskCreate(producer_task, "producer", 2048, NULL, 5, NULL);
    xTaskCreate(consumer_task, "consumer", 2048, NULL, 10, NULL);
}