/*
 * ctrl.c
 *
 *  Created on: Feb 26, 2024
 *      Author: Mateusz Korzeniewski <mateusz.korzeniewski@student.put.poznan.pl>
 */

#include "ctrl.h"
#include <stdbool.h>

void ctrl_init(Ctrl_HandleTypeDef *ctrl, float ctrl_gain, float target_speed, uint16_t brickwall, float max_speed) {
	ctrl->ctrl_gain = ctrl_gain;
	ctrl->p_gain = .8f;
	ctrl->target_speed = target_speed;
	ctrl->max_speed = max_speed;
	ctrl->error = 0;

	// Raw unsigned integer parameters
	ctrl->brickwall = brickwall;
	ctrl->value = 0;
	ctrl->closed_loop_delay_counter = 0;
}

uint16_t ctrl_call(Ctrl_HandleTypeDef *ctrl, float measure) {
	ctrl->error = ctrl->target_speed - measure;

	ctrl->value =
		ctrl->value
		+ (ctrl->p_gain * ctrl->ctrl_gain * ctrl->error);

	if(ctrl->value > ctrl->brickwall)
		ctrl->value = ctrl->brickwall;

	return ctrl->value;
}

uint16_t ctrl_update_target_speed(Ctrl_HandleTypeDef *ctrl, float target_speed) {
	if (target_speed > ctrl->max_speed) {
	        target_speed = ctrl->max_speed;
	} else if (target_speed < 0) {
		target_speed = 0;
	}

	ctrl->target_speed = target_speed;
	ctrl->value = ctrl->ctrl_gain * ctrl->target_speed;
	return ctrl->value;
}
