#ifndef BOARDS_H
#define BOARDS_H

// GPIO pin connected to the WS2812 LEDs for the specific board
#ifdef BOARD_1
    #define LED_PIN 14  // Pin for board 1
#elif defined(BOARD_2)
    #define LED_PIN 15  // Pin for board 2
#else
    #define LED_PIN 14  // Default pin
#endif

#endif 
