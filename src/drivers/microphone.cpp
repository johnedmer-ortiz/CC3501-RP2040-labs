// microphone.c

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "boards.h"
#include "arm_math.h"

// Manually define INT16_MAX and INT16_MIN since limits.h is not used
#define INT16_MAX 32767
#define INT16_MIN (-32768)

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

    // Safety measures
    adc_run(false);
    adc_fifo_drain();
}

void mic_read(uint16_t *sample_buf, int n_samples)
{
    adc_run(true);
    for (size_t i = 0; i < n_samples; i++)
    {
        sample_buf[i] = adc_fifo_get_blocking();
    }
    adc_run(false);
    adc_fifo_drain();
}

void process_samples(uint16_t *sample_buf, int16_t *processed_buf, int n_samples, int shift_amount)
{
    int32_t sum = 0;

    for (int i = 0; i < n_samples; i++)
    {
        sum += sample_buf[i];
    }
    int32_t dc_bias = sum / n_samples; // Sample average

    for (int i = 0; i < n_samples; i++)
    {
        int32_t temp = (int32_t)(sample_buf[i] - dc_bias) << shift_amount;
        // checks if calculated value goes over or goes under maximum and minimum number the variable can hold
        if (temp > INT16_MAX)
            temp = INT16_MAX;
        else if (temp < INT16_MIN)
            temp = INT16_MIN;
        processed_buf[i] = (int16_t)temp;
    }
}

void window_samples(int16_t *processed_buf, int16_t *window, int16_t *windowed_buf, int n_samples, int shift_amount)
{
    for (int i = 0; i < n_samples; i++)
    {
        int32_t temp = ((int32_t)processed_buf[i] * (int32_t)window[i]) << shift_amount;
        // checks if calculated value goes over or goes under maximum and minimum number the variable can hold
        if (temp > INT16_MAX)
            temp = INT16_MAX;
        else if (temp < INT16_MIN)
            temp = INT16_MIN;
        windowed_buf[i] = (int16_t)temp;
    }
}

void perform_fft(int16_t *fft_output, int16_t *windowed_buf, int n_samples)
{
    int fft_dir = 0; // 0 for forward transform, 1 for inverse transform
    arm_rfft_instance_q15 fft_instance;
    arm_status status = arm_rfft_init_q15(&fft_instance, n_samples, fft_dir, 1);
    // if (status != ARM_MATH_SUCCESS)
    // {
    //     return;
    // }
    arm_rfft_q15(&fft_instance, windowed_buf, fft_output);
}

void calc_rfft_mag_squared(int16_t *fft_output, uint32_t *magnitude_squared, int n_samples)
{
    int k;
    int32_t real, imag;
    uint32_t mag_sq;

    // k = 0
    real = fft_output[0];
    magnitude_squared[0] = (uint32_t)(((int32_t)real * (int32_t)real) >> 15);

    // k = N/2
    real = fft_output[1];
    magnitude_squared[n_samples / 2] = (uint32_t)(((int32_t)real * (int32_t)real) >> 15);

    // For k = 1 to N/2 -1
    for (k = 1; k < n_samples / 2; k++)
    {
        real = fft_output[2 * k];
        imag = fft_output[2 * k + 1];
        mag_sq = (uint32_t)((((int32_t)real * (int32_t)real) + ((int32_t)imag * (int32_t)imag)) >> 15);
        magnitude_squared[k] = mag_sq;
    }
}
