#include "accel.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include "pico/stdlib.h"

#define LIS3DH_ADDRESS 0x19  // Assuming SA0 is high
#define CTRL_REG1 0x20
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define CTRL_REG1_CONFIG 0x77 // 400Hz, all axes enabled, normal mode

#define GRAVITY 9.81 // m/s²
#define SENSITIVITY_2G 16384.0 // LSB per g for ±2g range

// Helper function to write a single register
static int write_register(i2c_inst_t *i2c, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    return i2c_write_blocking(i2c, LIS3DH_ADDRESS, data, 2, false);
}

// Helper function to read multiple bytes
static int read_registers(i2c_inst_t *i2c, uint8_t reg, uint8_t *buf, uint8_t len) {
    i2c_write_blocking(i2c, LIS3DH_ADDRESS, &reg, 1, true);
    return i2c_read_blocking(i2c, LIS3DH_ADDRESS, buf, len, false);
}

// Initialize the accelerometer
int accel_init(i2c_inst_t *i2c) {
    int result = write_register(i2c, CTRL_REG1, CTRL_REG1_CONFIG);
    return 0;
}


// Read acceleration data (LSB units)
int accel_read(accel_data_t *data) {
    uint8_t buffer[6];
    int result = read_registers(i2c0, OUT_X_L | 0x80, buffer, 6); // 0x80 for auto-increment

    // Combine low and high bytes to get 16-bit signed values
    data->x = (int16_t)(buffer[1] << 8 | buffer[0]); //creates 16bit integer, shifts high byte by 8 and then adds low byte using OR operation
    data->y = (int16_t)(buffer[3] << 8 | buffer[2]);
    data->z = (int16_t)(buffer[5] << 8 | buffer[4]);

    return 0; // Success
}

// Convert raw accelerometer data to acceleration in m/s²
void convert_to_mps2(accel_data_t *data, float *x_mps2, float *y_mps2, float *z_mps2) {
    *x_mps2 = (data->x / SENSITIVITY_2G) * GRAVITY;
    *y_mps2 = (data->y / SENSITIVITY_2G) * GRAVITY;
    *z_mps2 = (data->z / SENSITIVITY_2G) * GRAVITY;
}


