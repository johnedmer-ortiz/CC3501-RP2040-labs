#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "boards.h"
#include <stdio.h>

#define MIC_PIN 26
#define N_SAMPLES 1024


void configure_adc_continuous() {
    adc_init();

    adc_gpio_init(MIC_PIN); 
    adc_select_input(0);  // ADC input 0 corresponds to GPIO26 (MIC_PIN)

    adc_fifo_setup(
        true,   // Enable FIFO
        false,  // Do not shift to 8 bits, keep full 12-bit resolution
        0,      // Number of samples to trigger an interrupt (1 sample)
        false,  // Disable overwriting when FIFO is full
        false   // No DMA data request
    );

    adc_set_clkdiv(1087);  // Set clock divider for desired sample rate (~1 kHz)

    adc_run(true);  // Start ADC
}

int main() {
    stdio_init_all();
    configure_adc_continuous();
    uint16_t sample_buf[N_SAMPLES];

    for (size_t i = 0; i < N_SAMPLES; i = i + 1){
        sample_buf[i] = adc_fifo_get_blocking();
        printf("Reading: %d\n", sample_buf[i]);
    }
}
