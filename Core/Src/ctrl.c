/*
 * ctrl.c
 *
 *  Created on: Feb 26, 2024
 *      Author: mk
 */

#include "ctrl.h"
#include <stdbool.h>

void ctrl_init(Ctrl_HandleTypeDef *ctrl, float ctrl_gain, float target_speed, uint16_t brickwall, float max_speed) {
	ctrl->ctrl_gain = ctrl_gain;
	ctrl->p_gain = 15;
	ctrl->closed_loop_threshold = 0.5; // 0 < CLT < 1
	ctrl->target_speed = target_speed;
	ctrl->max_speed = max_speed;
	ctrl->error = 0;

	// booleans
	ctrl->closed_loop_engage = false;
	ctrl->last_cycle_bad = false;

	// Raw unsigned integer parameters
	ctrl->brickwall = brickwall;
	ctrl->value = 0;
	ctrl->closed_loop_delay_counter = 0;
}

uint16_t ctrl_call(Ctrl_HandleTypeDef *ctrl, float measure) {
	ctrl->error = ctrl->target_speed - measure;

	switch(ctrl->closed_loop_engage) {
	// When closed loop disengaged
	case false:
		ctrl->value = ctrl->ctrl_gain * ctrl->target_speed;
		// if it's past delay, reengage
		if(ctrl->closed_loop_delay_counter > CTRL_LOOP_DELAY) {
			ctrl->closed_loop_engage = true;
			ctrl->closed_loop_delay_counter = 0;
		} else
		{
			ctrl->closed_loop_delay_counter++;
		}
		break;
	case true:
		// check CLT
		if(
			((ctrl->error / ctrl->target_speed) > (ctrl->closed_loop_threshold))
			|| ((ctrl->error / ctrl->target_speed) < (0 - ctrl->closed_loop_threshold))
		) {
			// get the counter and penalty flag
			ctrl->last_cycle_bad = true;
			ctrl->closed_loop_delay_counter++;
			if(ctrl->closed_loop_delay_counter > CTRL_LOOP_DELAY) {
				// Penalty mode - make it get some sane values
				ctrl_disengage_closed_loop(ctrl);
			}
		} else {
			ctrl->last_cycle_bad = false;
			ctrl->closed_loop_delay_counter = 0;
		}

		// use closed-loop control
		ctrl->value =
			(ctrl->ctrl_gain * ctrl->target_speed)
			+ (ctrl->p_gain * ctrl->ctrl_gain * ctrl->error);
		break;
	}


	if(ctrl->value > ctrl->brickwall)
		ctrl->value = ctrl->brickwall;

	return ctrl->value;
}

void ctrl_update_target_speed(Ctrl_HandleTypeDef *ctrl, float target_speed) {
	if (target_speed > ctrl->max_speed) {
	        target_speed = ctrl->max_speed;
	} else if (target_speed < 0) {
		target_speed = 0;
	}

	ctrl->target_speed = target_speed;

	// disable closed loop, wait for it to settle down
	ctrl_disengage_closed_loop(ctrl);
}

void ctrl_disengage_closed_loop(Ctrl_HandleTypeDef *ctrl) {
	ctrl->closed_loop_delay_counter = 0;
	ctrl->closed_loop_engage = false;
}
