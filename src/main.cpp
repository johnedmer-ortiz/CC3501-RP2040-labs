#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "accel.h"
#include "boards.h"

#define I2C_PORT i2c0
#define SCL_PIN 17
#define SDA_PIN 16

int main() {
    stdio_init_all();
    // Initialize I2C and accelerometer
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN);
    gpio_pull_up(SDA_PIN);
    accel_init(I2C_PORT);

    while (1) {
        accel_data_t accel_data;
        accel_read(&accel_data);
        float x_mps2, y_mps2, z_mps2;
        convert_to_mps2(&accel_data, &x_mps2, &y_mps2, &z_mps2);
        printf("X: %.2f m/s^2, Y: %.2f m/s^2, Z: %.2f m/s^2\n", x_mps2, y_mps2, z_mps2);
        sleep_ms(50);
    }

    return 0;
}


/*WHO_AM_I register reading test */
//#define WHO_AM_I_REG 0x0F

// void i2c_read_register(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t nbytes) {
//     i2c_write_blocking(I2C_PORT, addr, &reg, 1, true); // Send register address
//     i2c_read_blocking(I2C_PORT, addr, buf, nbytes, false); // Read nbytes into buffer
// }

// int main() {
//     // Initialize chosen serial port
//     stdio_init_all();

//     // Initialize I2C
//     i2c_init(I2C_PORT, 100 * 1000); // 100kHz I2C frequency
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(SCL_PIN);
//     gpio_pull_up(SDA_PIN);

//     uint8_t who_am_i = 0;
    
//     // Read WHO_AM_I register
//     i2c_read_register(LIS3DH_ADDRESS, WHO_AM_I_REG, &who_am_i, 1);

//     // Print WHO_AM_I value
//     printf("WHO_AM_I register: 0x%02X\n", who_am_i);

//     return 0;
// }
///////////////////////////////////////////////////////////////////