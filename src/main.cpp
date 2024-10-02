#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "boards.h"
#include "microphone.h"
#include <stdio.h>
#include "arm_math.h"

#define MIC_PIN 26
#define N_SAMPLES 1024
#define SHIFT_AMOUNT 3

int main()
{
    stdio_init_all();
    configure_adc_continuous(MIC_PIN);

    int fft_dir = 0; // 0 for forward transform, 1 for inverse transform
    arm_rfft_instance_q15 fft_instance;
    arm_rfft_init_q15(&fft_instance, N_SAMPLES, fft_dir, 1);

    uint16_t sample_buf[N_SAMPLES];
    int16_t processed_buf[N_SAMPLES];
    mic_read(sample_buf, N_SAMPLES); // get mic samples and stores it in sample_buf
    process_samples(sample_buf, processed_buf, N_SAMPLES, SHIFT_AMOUNT); // process sample_buf and store it in processed_buf
}