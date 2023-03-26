#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "string.h"

#define LED 2
#define DELAY 1000

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};

// Set UART parameters
void init_uart() {
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

    const char *msg = "Hello world!\n";
    uart_write_bytes(UART_NUM_0, msg, strlen(msg));
}

void app_main() {
    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    init_uart();

    uint32_t start = xthal_get_ccount();
    gpio_set_level(LED, 1);
    vTaskDelay(DELAY / portTICK_PERIOD_MS);
    gpio_set_level(LED, 0);
    vTaskDelay(DELAY / portTICK_PERIOD_MS);
    uint32_t end = xthal_get_ccount();
    printf("Time: %ld\n", end - start);

    for (;;) {
        gpio_set_level(LED, 1);
        printf("Turn on\n");
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
        gpio_set_level(LED, 0);
        printf("Turn off\n");
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
    }
}