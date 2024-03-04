/*
 * temp.c
 *
 *  Created on: Mar 3, 2024
 *      Author: Mateusz Korzeniewski <mateusz.korzeniewski@student.put.poznan.pl>
 */

#include "temp.h"
#include "bmp280.h"
#include "i2c.h"

float temperature = 0;
BMP280_HandleTypedef bmp;

/**
  * @brief  Read BMP280 measurements
  * @param  None
  * @retval None
  */
void grab_bmp_measurement() {
	bmp280_read_float(&bmp, &temperature, NULL, NULL);
}

/**
  * @brief  Initialize BMP280 sensor for temperature measurement
  * @param  None
  * @retval None
  */
void bmp_init() {
	bmp280_init_default_params(&bmp.params);
	bmp.addr = BMP280_I2C_ADDRESS_0;
	bmp.i2c = &hi2c1;
	bmp280_init(&bmp, &bmp.params);
}
