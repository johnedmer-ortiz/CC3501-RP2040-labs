#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "accel.h"
#include "accel_task.h"
#include "leds.h"
#include "boards.h"

#define I2C_PORT i2c0
#define SCL_PIN 17
#define SDA_PIN 16

// Maximum and minimum expected acceleration (adjust based on real data)
#define MAX_ACCEL 9.81 // 1G (gravity) in m/s^2
#define MIN_ACCEL 0.0  // Zero acceleration

#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 8
#define UART_RX_PIN 9


int accel_task_run() {
    stdio_init_all();
    
    // Initialize UART for RN4871 communication
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Send AT commands to configure RN4871 (you may need to adjust these)

    // Initialize I2C and accelerometer
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN);
    gpio_pull_up(SDA_PIN);
    accel_init(I2C_PORT);

    // Initialize the LEDs
    init_leds();

    while (!stop_task) {
        accel_data_t accel_data;
        accel_read(&accel_data);
        float x_mps2, y_mps2, z_mps2;
        convert_to_mps2(&accel_data, &x_mps2, &y_mps2, &z_mps2);

        char bluetooth_message[100];
        snprintf(bluetooth_message, sizeof(bluetooth_message), "X: %.2f m/s^2, Y: %.2f m/s^2, Z: %.2f m/s^2\n", x_mps2, y_mps2, z_mps2);
        uart_puts(UART_ID, bluetooth_message);
        
        // Print acceleration data to the terminal
        printf("%s", bluetooth_message);

        // Normalize acceleration values to a range of 0-255 for brightness
        uint32_t x_brightness = (uint32_t)((x_mps2 / MAX_ACCEL) * 255);
        uint32_t y_brightness = (uint32_t)((y_mps2 / MAX_ACCEL) * 255);
        uint32_t z_brightness = (uint32_t)((z_mps2 / MAX_ACCEL) * 255);

        // Light up all 4 LEDs for each axis with varying brightness
        for (int i = 1; i <= 4; i++) {
            change_led(i, x_brightness, 0, 0); // X-axis, red
        }

        for (int i = 5; i <= 8; i++) {
            change_led(i, 0, y_brightness, 0); // Y-axis, green
        }

        for (int i = 9; i <= 12; i++) {
            change_led(i, 0, 0, z_brightness); // Z-axis, blue
        }

        // Commit the LED changes
        commit_led_changes();

        sleep_ms(500);  // Delay before the next reading
    }

    return 0;
}



/*WHO_AM_I register reading test */
// #define WHO_AM_I_REG 0x0F

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