/*
 * ctrl.c
 *
 *  Created on: Feb 26, 2024
 *      Author: mk
 */

#include "ctrl.h"

void ctrl_init(Ctrl_HandleTypeDef *ctrl, float ctrl_gain, float i_time, float target_speed, uint16_t brickwall, float max_speed) {
	ctrl->ctrl_gain = ctrl_gain;
	ctrl->p_gain = 0;
	ctrl->i_time = i_time;
	ctrl->integral = 0;
	ctrl->closed_loop_threshold = 0.3; // 0 < CLT < 1
	ctrl->target_speed = target_speed;
	ctrl->max_speed = max_speed;
	ctrl->ctrl_error = 0;

	// Raw unsigned integer parameters
	ctrl->brickwall = brickwall;
	ctrl->value = 0;
}

uint16_t ctrl_call(Ctrl_HandleTypeDef *ctrl, float measure) {
	ctrl->ctrl_error = ctrl->target_speed - measure;

	ctrl->value =
		(ctrl->ctrl_gain * ctrl->target_speed)
		+ (5 * ctrl->ctrl_gain * ctrl->ctrl_error);
}

void ctrl_update_target_speed(Ctrl_HandleTypeDef *ctrl, float target_speed) {
	if (target_speed > ctrl->max_speed) {
	        target_speed = ctrl->max_speed;
	} else if (target_speed < 0) {
		target_speed = 0;
	}

	ctrl->target_speed = target_speed;
}
