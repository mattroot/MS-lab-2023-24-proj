/*
 * ctrl.h
 *
 *  Created on: Feb 26, 2024
 *      Author: mk
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

#include <stdint.h>

// I don't wanna split this, I'll just add this single macro here
#define CTRL_LOOP_DELAY 100000

typedef struct {
	// Floating point parameters
	float 		ctrl_gain;
	float		p_gain;
	float 		target_speed;
	float		max_speed;
	float		error;

	// Raw unsigned integer parameters
	uint16_t 	brickwall;
	uint16_t    value;
	uint32_t	closed_loop_delay_counter;

} Ctrl_HandleTypeDef;

void ctrl_init(Ctrl_HandleTypeDef *ctrl, float ctrl_gain, float target_speed, uint16_t brickwall, float max_speed);
uint16_t ctrl_call(Ctrl_HandleTypeDef *ctrl, float measure);
uint16_t ctrl_update_target_speed(Ctrl_HandleTypeDef *ctrl, float target_speed);

#endif /* INC_CTRL_H_ */
