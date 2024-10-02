#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "boards.h"
#include <stdio.h>

void configure_adc_continuous(int mic_pin)
{
    adc_init();

    adc_gpio_init(mic_pin);
    adc_select_input(0); // ADC input 0 corresponds to GPIO26 (MIC_PIN)

    adc_fifo_setup(
        true,  // Enable FIFO
        false, // Do not shift to 8 bits, keep full 12-bit resolution
        0,     // Number of samples to trigger an interrupt (1 sample)
        false, // Disable overwriting when FIFO is full
        false  // No DMA data request
    );

    adc_set_clkdiv(1087);

    // safety
    adc_run(false);
    adc_fifo_drain();
}

void mic_read(uint16_t *sample_buf, int n_samples)
{
    adc_run(true);
    printf("***RAW samples***\n");
    for (size_t i = 0; i < n_samples; i++)
    {
        sample_buf[i] = adc_fifo_get_blocking();
        printf("%d, ", sample_buf[i]);
    }
    adc_run(false);
    adc_fifo_drain();
}

void process_samples(uint16_t *sample_buf, int16_t *processed_buf, int n_samples, int shift_amount)
{
    int32_t sum;

    for (int i = 0; i < n_samples; i++)
    {
        sum = sum + sample_buf[i];
    }
    int32_t dc_bias = sum / n_samples; // sample average

    printf("\n\n***Processed samples - bias: %d shift factor: 2^%d***\n", dc_bias, shift_amount);
    for (int i = 0; i < n_samples; i++)
    {
        processed_buf[i] = (int16_t)((sample_buf[i] - dc_bias) << shift_amount); // Moves graph down to 0 line. Data precision increased by multiplying by 2^SHIFT_AMOUNT
        printf("%d, ", processed_buf[i]);
    }
    printf("\n");
}