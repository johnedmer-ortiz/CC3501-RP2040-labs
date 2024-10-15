// microphone.h

#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <stdint.h>

void configure_adc_continuous(int mic_pin);
void mic_read(uint16_t *sample_buf, int n_samples);
void process_samples(uint16_t *sample_buf, int16_t *processed_buf, int n_samples, int shift_amount);
void window_samples(int16_t *processed_buf, int16_t *window, int16_t *windowed_buf, int n_samples, int shift_amount);
void perform_fft(int16_t *fft_output, int16_t *windowed_buf, int n_samples);
void calc_rfft_mag_squared(int16_t *fft_output, uint32_t *magnitude_squared, int n_samples);

#endif // MICROPHONE_H
