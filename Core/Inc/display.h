#ifndef DISPLAY_H
#define DISPLAY_H

#include "display_conf.h"
#include "i2c.h"

void display_write(I2C_HandleTypeDef* hi2c, uint8_t data, uint8_t control);
void display_init(I2C_HandleTypeDef *hi2c);
void display_print(I2C_HandleTypeDef* hi2c, const char* str);
void display_setCursor(I2C_HandleTypeDef* hi2c, uint8_t col, uint8_t row);

#endif //DISPLAY_H
