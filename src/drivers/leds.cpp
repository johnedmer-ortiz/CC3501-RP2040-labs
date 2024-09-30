#include "leds.h"
#include "hardware/pio.h"
#include "WS2812.pio.h"
#include "boards.h" 
#include "pico/stdlib.h"


// Array to store color data for all LEDs
static uint32_t led_data[12] = {0};  // Initialize with all LEDs off

// Initializes LED strip.
void init_leds() {
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program); 
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
}

// Changes the color of a specific LED - data is not sent.
void change_led(uint8_t led_num, uint32_t r, uint32_t g, uint32_t b) {
        led_data[led_num - 1] = (r << 24) | (g << 16) | (b << 8);
}

// Commits the changes by sending the LED data 
void commit_led_changes() {
    for (int i = 0; i < 12; i++) {
        pio_sm_put_blocking(pio0, 0, led_data[i]);  
    }
    sleep_us(500);
}

void turnoff_led() {
    for (int j = 0; j < 12; j++) {
        led_data[j] = 0; 
    }
    commit_led_changes(); 
}


