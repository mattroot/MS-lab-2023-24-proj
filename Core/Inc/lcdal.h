/*
 * lcdal.h - LCD Abstraction Layer
 *
 *  Created on: Mar 3, 2024
 *      Author: why
 */

#ifndef INC_LCDAL_H_
#define INC_LCDAL_H_

#include "pwm_fan.h"

void prepare_display();
void update_display();
void generate_fan_display_line(char *str, PWM_Fan_HandleTypeDef *fan);

#endif /* INC_LCDAL_H_ */
