#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

// Function declarations
void init_leds();
void change_led(uint8_t led_num, uint32_t r, uint32_t g, uint32_t b);
void commit_led_changes();
void turnoff_led();

#endif 

//Sample Usage

    // init_leds();  // Initialize the LEDs using the function from leds.cpp

    // bool led_off = true;  // Flag to determine if LEDs should be turned off

    // for (;;) {
    //     if (led_off) {
    //         turnoff_led();  // Turn off all LEDs if flag is set
    //     }

    //     // Set specific LEDs to desired colors (do not commit changes yet)
    //     change_led(1, 255, 255, 255);  // Set LED 1 to white
    //     change_led(3, 255, 0, 0);      // Set LED 3 to red
    //     change_led(12, 0, 255, 0);     // Set LED 12 to green
    //     change_led(11, 0, 255, 0);     // Set LED 11 to green
    //     change_led(2, 255, 255, 0);    // Set LED 2 to yellow
    //     change_led(4, 255, 255, 255);  // Set LED 4 to white
    //     commit_led_changes();

    //     sleep_ms(500);  // Wait for 500 milliseconds
    // }