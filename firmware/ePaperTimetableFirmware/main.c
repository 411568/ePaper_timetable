/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#define DEBUG 
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"   //Examples

int main() {
    stdio_init_all();
    gpio_set_function(1, GPIO_FUNC_UART);
    gpio_set_function(2, GPIO_FUNC_UART);
    printf("Starting\n");
	DEV_Delay_ms(500);
    printf("After delay...\n"); 
    EPD_7in5b_V2_test(); 
    printf("Test finished!\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
   
}
