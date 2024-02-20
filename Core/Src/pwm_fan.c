#include "pwm_fan.h"
#include "tim.h"

void pwm_fan_init(PWM_Fan_HandleTypeDef *fan,
		TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_tacho,
		uint16_t pwm_channel,
		uint16_t tacho_channel) {
	// handles
    fan->htim_pwm = htim_pwm;
    fan->htim_tacho = htim_tacho;
    fan->pwm_channel = pwm_channel;
    fan->tacho_channel = tacho_channel;

    // constant registers for easy access
    fan->autoreload = htim_pwm->Instance->ARR;
    fan->prescaler = htim_pwm->Instance->PSC;
    fan->tacho_autoreload = htim_tacho->Instance->ARR;
    fan->tacho_prescaler = htim_tacho->Instance->PSC;

    // fan characteristics
    fan->max_speed = 0;
    fan->min_speed = 0;

	// fan speed control
	fan->target_duty_cycle = 0.0f;
	fan->target_speed = 0.0f;

	// fan speed monitoring
	fan->current_speed = 0;
	fan->speed_error = 0;
	fan->ctrl_error = 0;

	// TACHO estimation helpers
	fan->last_read = 0;
	fan->current_read = 0;

	// calibration helpers
	fan->calibration_cycle_counter = 0;
	fan->calibration_min = 0;
	fan->calibration_max = 0;

	// controller settings
	fan->ctrl_gain = 0;
	fan->mode = PWM_FAN_UNCONFIGURED;

    // Initialize PWM
    HAL_TIM_PWM_Start_IT(fan->htim_pwm, fan->pwm_channel);
    // Initialize TACHO input capture
    HAL_TIM_IC_Start_IT(fan->htim_tacho, fan->tacho_channel);
}

void pwm_fan_schedule_calibration(PWM_Fan_HandleTypeDef *fan) {
	fan->calibration_cycle_counter = 0;
	fan->min_speed = 999999.0f;
	fan->max_speed = 0.0f;
	fan->mode = PWM_FAN_CALIBRATION_START;
}

void pwm_fan_set(PWM_Fan_HandleTypeDef *fan, float target_speed) {
    // Ensure target speed is within the valid range
    if (target_speed > fan->max_speed) {
        target_speed = fan->max_speed;
    } else if (target_speed < fan->min_speed) {
        target_speed = fan->min_speed;
    }

    fan->target_speed = target_speed;
}

void pwm_fan_set_duty_cycle(PWM_Fan_HandleTypeDef *fan, float duty_cycle) {
	// Ensure duty is within the valid range
	if (duty_cycle > 100) {
		duty_cycle = 100;
	} else if  (duty_cycle < 0) {
		duty_cycle = 0;
	}
	fan->target_duty_cycle = duty_cycle;
	__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel,
					((uint16_t)(fan->target_duty_cycle * (fan->autoreload+1) / 100) - 1));
}

float pwm_fan_set_duty_cycle_raw(PWM_Fan_HandleTypeDef *fan, uint16_t compare_register) {
	__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel, compare_register);

	// return generic duty cycle value
	fan->target_duty_cycle = (float)(compare_register) / (float)fan->autoreload * 100;
	return fan->target_duty_cycle;
}

float pwm_fan_update_speed(PWM_Fan_HandleTypeDef *fan) {
	// Save the last capture time
	fan->last_read = fan->current_read;
	// Read the current capture time
	fan->current_read = __HAL_TIM_GET_COUNTER(fan->htim_tacho);

	// Calculate the time between the falling edges
	uint32_t timeDiff = fan->current_read - fan->last_read;

	// NOTE: this is extremely specific for STM32F767ZITx timers @ APB1
	// TODO: why still unreliable
	fan->current_speed =
					60.0f * (float)HAL_RCC_GetPCLK1Freq() * 2.0f
					/ (float)timeDiff / (float)(fan->tacho_prescaler+1) / TACHO_PULSE_PER_REV;

	return fan->current_speed;
}

float pwm_fan_update(PWM_Fan_HandleTypeDef *fan) {
     // Read TACHO input capture value to calculate current speed
	switch(fan->mode) {
//	case PWM_FAN_DIRECT:
//		pwm_fan_set_duty_cycle(fan, fan->target_duty_cycle);
//		break;
	case PWM_FAN_PCONTROL: {
		// Calculate error
		fan->ctrl_error = fan->target_speed - fan->current_speed;
		// Calculate PWM duty cycle
		uint16_t pwm_value = fan->ctrl_gain * fan->target_speed;
		// Set PWM duty cycle
		pwm_fan_set_duty_cycle_raw(fan, pwm_value);
		break;
	}
	case PWM_FAN_CALIBRATION_START:
		pwm_fan_set_duty_cycle(fan, 30.0f);
		if(fan->calibration_cycle_counter++ > PWM_FAN_CALIBRATION_PERIOD) {
			fan->calibration_cycle_counter = 0;
			fan->mode = PWM_FAN_CALIBRATION_MIN_SPEED;
		}
		break;
	case PWM_FAN_CALIBRATION_MIN_SPEED:
		fan->calibration_cycle_counter++;
		if(fan->calibration_cycle_counter > PWM_FAN_CALIBRATION_PERIOD) {
			fan->min_speed = fan->current_speed;
			fan->calibration_cycle_counter = 0;
			fan->mode = PWM_FAN_CALIBRATION_MAX_START;
		}
		break;
	case PWM_FAN_CALIBRATION_MAX_START:
		pwm_fan_set_duty_cycle(fan, 100.0f);
		if(fan->calibration_cycle_counter++ > PWM_FAN_CALIBRATION_PERIOD) {
			fan->calibration_cycle_counter = 0;
			fan->mode = PWM_FAN_CALIBRATION_MAX_SPEED;
		}
		break;
	case PWM_FAN_CALIBRATION_MAX_SPEED:
		fan->calibration_cycle_counter++;
		if(fan->calibration_cycle_counter > PWM_FAN_CALIBRATION_PERIOD) {
			fan->max_speed = fan->current_speed;
			fan->calibration_cycle_counter = 0;
			fan->ctrl_gain = (float) fan->autoreload / fan->max_speed;

			// set to 1000 RPM after calibration
			fan->mode = PWM_FAN_PCONTROL;
			pwm_fan_set(fan, 1000.0f);
		}
		break;
	default:
		break;
	}

	if(fan->mode != PWM_FAN_UNCONFIGURED) {
		if(pwm_fan_is_stopped(fan)) fan->current_speed = 0.0f;
	} else {
		fan->current_speed = 0.0f;
	}
    return fan->current_speed;
}

uint16_t pwm_fan_is_stopped(PWM_Fan_HandleTypeDef *fan) {
	uint32_t metric = fan->htim_tacho->Instance->CNT;
	if(metric - fan->current_read > TACHO_STOPPED_THRESHOLD) return 1;
	return 0;
}

