#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "boards.h"
#include "microphone.h"
#include <stdio.h>

#define MIC_PIN 26
#define N_SAMPLES 1024

int main() {
    stdio_init_all();
    configure_adc_continuous(MIC_PIN);
    uint16_t sample_buf[N_SAMPLES];
    mic_read(sample_buf, N_SAMPLES);
}