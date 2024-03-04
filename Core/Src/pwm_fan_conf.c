/*
 * pwm_fan_conf.c
 *
 *  Created on: Feb 21, 2024
 *      Author: why
 */

#include "pwm_fan_conf.h"

PWM_Fan_HandleTypeDef fan1 = {
		.htim_pwm = NULL,
		.htim_tacho = NULL,
		.pwm_channel = 0,
		.tacho_channel = 0,
		.autoreload = 0,
		.prescaler = 0,
		.tacho_autoreload = 0,
		.tacho_prescaler = 0,
		.max_speed = 0,
		.min_speed = 0,
		.target_duty_cycle = 0,
		.current_speed = 0,
		.last_read = 0,
		.current_read = 0,
		.calibration_cycle_counter = 0,
		.calibration_min = 0,
		.calibration_max = 0,
		.ctrl_gain = 0,
		.mode = 0
};
PWM_Fan_HandleTypeDef fan2 = {
		.htim_pwm = NULL,
		.htim_tacho = NULL,
		.pwm_channel = 0,
		.tacho_channel = 0,
		.autoreload = 0,
		.prescaler = 0,
		.tacho_autoreload = 0,
		.tacho_prescaler = 0,
		.max_speed = 0,
		.min_speed = 0,
		.target_duty_cycle = 0,
		.current_speed = 0,
		.last_read = 0,
		.current_read = 0,
		.calibration_cycle_counter = 0,
		.calibration_min = 0,
		.calibration_max = 0,
		.ctrl_gain = 0,
		.mode = 0
};

//const float PWM_FAN_FIR_FILTER_COEFFICIENTS = ;
