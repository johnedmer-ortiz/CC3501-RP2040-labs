#ifndef ACCEL_H
#define ACCEL_H

#include "hardware/i2c.h"

// Structure to hold acceleration data
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} accel_data_t;

// Function declarations
int accel_init(i2c_inst_t *i2c);
int accel_read(accel_data_t *data);
void convert_to_mps2(accel_data_t *data, float *x_mps2, float *y_mps2, float *z_mps2);

#endif // ACCEL_H
