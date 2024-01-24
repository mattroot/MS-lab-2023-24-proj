#ifndef DISPLAY_H
#define DISPLAY_H

#include "display_conf.h"
#include "i2c.h"

void display_init(I2C_HandleTypeDef *hi2c);

#endif //DISPLAY_H