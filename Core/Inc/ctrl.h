/*
 * ctrl.h
 *
 *  Created on: Feb 26, 2024
 *      Author: mk
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

typedef struct {
	// Floating point parameters
	float 		ctrl_gain;
	float		i_time;
	float 		integral = 0;
	float		closed_loop_threshold = 0.3; // 0 < CLT < 1
	float 		target_speed;

	// Raw unsigned integer parameters
	uint16_t 	brickwall;
	uint16_t    value;
} Ctrl_HandleTypeDef;

void ctrl_init(Ctrl_HandleTypeDef *ctrl, float ctrl_gain, float i_time, float target_speed, uint16_t brickwall);
uint16_t ctrl_call(Ctrl_HandleTypeDef *ctrl, float measure);
void ctrl_update_target_speed(Ctrl_HandleTypeDef *ctrl, float target_speed);

#endif /* INC_CTRL_H_ */
