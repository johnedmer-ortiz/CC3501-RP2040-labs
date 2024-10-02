void configure_adc_continuous(int mic_pin);
void mic_read(uint16_t *sample_buf, int n_samples);
void process_samples(uint16_t *sample_buf, int16_t *processed_buf, int n_samples, int shift_amount);