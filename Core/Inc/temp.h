/*
 * temp.h
 *
 *  Created on: Mar 3, 2024
 *      Author: why
 */

#ifndef INC_TEMP_H_
#define INC_TEMP_H_

#include "bmp280.h"

extern float temperature;
extern BMP280_HandleTypedef bmp;
void grab_bmp_measurement();
void bmp_init();

#endif /* INC_TEMP_H_ */
