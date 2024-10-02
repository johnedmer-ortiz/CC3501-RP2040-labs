#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "boards.h"
#include "arm_math.h"
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

void window_samples(int16_t *processed_buf, int16_t *window, int16_t *windowed_buf, int n_samples, int shift_amount)
{
    printf("\n***Windowed samples***\n");
    for (int i = 0; i < n_samples; i++)
    {
        windowed_buf[i] = (window[i] * processed_buf[i]) << shift_amount;
        printf("%d, ", windowed_buf[i]);
    }
}

void perform_fft(int16_t *fft_output, int16_t *windowed_buf, int n_samples)
{
    int fft_dir = 0; // 0 for forward transform, 1 for inverse transform
    arm_rfft_instance_q15 fft_instance;
    arm_rfft_init_q15(&fft_instance, n_samples, fft_dir, 1);
    arm_rfft_q15(&fft_instance, windowed_buf, fft_output);
    printf("\n\n***FFT output***\n");
    for (int i = 0; i < 1024; i++)
    {
        printf("%d, ", windowed_buf[i]);
    }
}

void calc_mag_squared(int16_t *magnitude_squared, int16_t *fft_output, int n_samples)
{
    arm_cmplx_mag_squared_q15(fft_output, magnitude_squared, n_samples / 2);
    printf("\n\n***Magnitude squared***\n");
    for (int i = 0; i < n_samples / 2; i++)
    {
        printf("%d, ", magnitude_squared[i]);
    }
}