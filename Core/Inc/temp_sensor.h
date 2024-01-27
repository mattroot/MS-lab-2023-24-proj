#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "temp_sensor_conf.h"
#include "i2c.h"

void temp_sensor_init(I2C_HandleTypeDef *hi2c);
float temp_sensor_read(I2C_HandleTypeDef *hi2c);

#endif
