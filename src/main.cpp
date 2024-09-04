#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h"
#include "drivers/logging/logging.h"

#define LED_PIN 14  // GPIO pin connected to the WS2812 LEDs

// Changes the color of a specific LED in the WS2812 LED strip.
void change_led(uint8_t led_num, uint32_t r, uint32_t g, uint32_t b) {
    static uint32_t led_data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // Array to store color data for 12 LEDs

    led_data[led_num - 1] = (r << 24) | (g << 16) | (b << 8);  // Update color for the specified LED

    for(int i = 0; i < 12; i++) {
        pio_sm_put_blocking(pio0, 0, led_data[i]);  // Send color data to the PIO state machine
    }
}

// Turns off all LEDs in the WS2812 LED strip.
void turnoff_led() {
    for(int j = 0; j < 12; j++) {
        pio_sm_put_blocking(pio0, 0, 0);  // Send zero value to turn off each LED
    }
}

int main() {
    stdio_init_all();  // Initialize standard I/O

    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);  // Load WS2812 PIO program into PIO0
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);  // Initialize the WS2812 program

    bool led_off = true;  // Flag to determine if LEDs should be turned off

    for (;;) {
        if (led_off) {
            turnoff_led();  // Turn off all LEDs if flag is set
        }

        // Set specific LEDs to desired colors
        change_led(1, 255, 255, 255);  // Set LED 1 to white
        change_led(3, 255, 0, 0);      // Set LED 3 to red
        change_led(12, 0, 255, 0);     // Set LED 12 to green
        change_led(11, 0, 255, 0);     // Set LED 11 to green

        sleep_ms(500);  // Wait for 500 milliseconds
    }

    return 0;  // This line will never be reached due to the infinite loop
}
