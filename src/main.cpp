#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#define LED_PIN 14

int change_led(uint8_t led_number, uint8_t color) {
    
}

int main()
{
    stdio_init_all();

    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
    static uint32_t led_data [1];

    for (;;) {
        // Test the log system
        // log(LogLevel::INFORMATION, "Hello world");

        uint8_t red;
        uint8_t green;
        uint8_t blue;
        
        for(int i = 0; i < 12; i++) {
            if(i % 2 == 0) {
                red = 255;
                green = 0;
                blue = 0;
                led_data[0] = (red << 24) | (green << 16) | (blue << 8);
                pio_sm_put_blocking(pio0, 0, led_data[0]);
            }
            else{
                red = 0;
                green = 0;
                blue = 255;
                led_data[0] = (red << 24) | (green << 16) | (blue << 8);
                pio_sm_put_blocking(pio0, 0, led_data[0]);
            }
        }
        sleep_ms(500);


        // // Turn on the first LED to be a certain colour
        // uint8_t red = 255;
        // uint8_t green = 0;
        // uint8_t blue = 0;
        // led_data[0] = (red << 24) | (green << 16) | (blue << 8);
        // pio_sm_put_blocking(pio0, 0, led_data[0]);
        // //sleep_ms(500);

        // red = 0;
        // green = 255;
        // blue = 0;
        // led_data[1] = (red << 24) | (green << 16) | (blue << 8);
        // pio_sm_put_blocking(pio0, 0, led_data[1]);

        // red = 0;
        // green = 0;
        // blue = 255;
        // led_data[2] = (red << 24) | (green << 16) | (blue << 8);
        // pio_sm_put_blocking(pio0, 0, led_data[2]);

        // sleep_ms(500); //LEDs reset if it pauses for more than 200us

        // // Set the first LED off 
        // led_data[0] = 0;
        // pio_sm_put_blocking(pio0, 0, led_data[0]);
        // sleep_ms(500);
    }

    return 0;
}
