#include "display.h"

void display_write(I2C_HandleTypeDef* hi2c, uint8_t data, uint8_t control) {
	uint8_t data_u, data_l;
	uint8_t control_u, control_l;
	data_u = data & 0xF0;
	data_l = (data << 4) & 0xF0;
	control_u = control & 0xF0;
	control_l = (control << 4) & 0xF0;

	// Send the data
	// Set the LCD address and send the upper 4 bits
	uint8_t buf[2];
	buf[0] = PCF8574_ADDRESS;
	buf[1] = data_u | control_u | (1 << LCD_EN) | (1 << LCD_RW) | (1 << LCD_RS);
	HAL_I2C_Master_Transmit(hi2c, buf[0], &buf[1], 1, 100);

	// Pulse the En pin
	buf[1] = data_u | control_u | (1 << LCD_RW) | (1 << LCD_RS);
	HAL_I2C_Master_Transmit(hi2c, buf[0], &buf[1], 1, 100);

	// Send the lower 4 bits
	buf[1] = data_l | control_l | (1 << LCD_EN) | (1 << LCD_RW) | (1 << LCD_RS);
	HAL_I2C_Master_Transmit(hi2c, buf[0], &buf[1], 1, 100);

	// Pulse the En pin
	buf[1] = data_l | control_l | (1 << LCD_RW) | (1 << LCD_RS);
	HAL_I2C_Master_Transmit(hi2c, buf[0], &buf[1], 1, 100);

	// Wait for the command to finish
	HAL_Delay(2);
}

void display_init(I2C_HandleTypeDef* hi2c) {
    HAL_Delay(50);
    display_write(hi2c, 0x30, 0);
    HAL_Delay(5);
    display_write(hi2c, 0x30, 0);
    HAL_Delay(1);
    display_write(hi2c, 0x30, 0);
    HAL_Delay(1);
    display_write(hi2c, 0x20, 0);
    HAL_Delay(1);

    display_write(hi2c, 0x28, 0);  // 4-bit mode, 2 lines, 5x7 format
    HAL_Delay(1);
    display_write(hi2c, 0x08, 0);  // Display off
    HAL_Delay(1);
    display_write(hi2c, 0x01, 0);  // Clear display
    HAL_Delay(2);
    display_write(hi2c, 0x06, 0);  // Increment cursor
    HAL_Delay(1);
    display_write(hi2c, 0x0C, 0);  // Display on
    HAL_Delay(1);
}

void display_print(I2C_HandleTypeDef* hi2c, const char* str) {
    while (*str) {
        display_write(hi2c, *str, 1);
        str++;
    }
}

void display_setCursor(I2C_HandleTypeDef* hi2c, uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    display_write(hi2c, 0x80 | (col + row_offsets[row]), 0);
}
