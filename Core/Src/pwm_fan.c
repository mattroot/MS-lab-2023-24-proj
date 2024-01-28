#include "pwm_fan.h"
#include "tim.h"

void pwm_fan_init(PWM_Fan_HandleTypeDef *fan,
		TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_tacho,
		uint32_t pwm_channel,
		uint32_t tacho_channel)
{
    fan->htim_pwm = htim_pwm;
    fan->htim_tacho = htim_tacho;
    fan->pwm_channel = pwm_channel;
    fan->tacho_channel = tacho_channel;
    fan->target_speed = 0.0f;
    fan->current_speed = 0.0f;
    fan->autoreload = __HAL_TIM_GET_AUTORELOAD(htim_pwm);
    fan->prescaler = __HAL_TIM_GET_ICPRESCALER(fan->htim_tacho, fan->tacho_channel);

    // Initialize PWM
    HAL_TIM_PWM_Start_IT(fan->htim_pwm, fan->pwm_channel);
    // Initialize TACHO input capture
    HAL_TIM_IC_Start_IT(fan->htim_tacho, fan->tacho_channel);

    // Put fan in calibration mode
    pwm_fan_schedule_calibration(fan);
}

void pwm_fan_set(PWM_Fan_HandleTypeDef *fan, float target_speed)
{
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
					fan->autoreload - (uint16_t)(fan->target_speed * fan->autoreload / 100));
}

float pwm_fan_set_duty_cycle_raw(PWM_Fan_HandleTypeDef *fan, uint16_t compare_register) {
	__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel, compare_register);

	// return generic duty cycle value
	fan->target_duty_cycle = (float)(fan->autoreload - compare_register) / (float)fan->autoreload * 100;
	return fan->target_duty_cycle;
}

void pwm_fan_schedule_calibration(PWM_Fan_HandleTypeDef *fan) {
	fan->calibration_halt_counter = 0;
	fan->calibration_cycle_counter = 0;
	fan->mode = PWM_FAN_CALIBRATION_START;
}

float pwm_fan_update_speed(PWM_Fan_HandleTypeDef *fan)
{
	// Save the last capture time
	fan->last_read = fan->current_read;
	// Read the current capture time
	fan->current_read = __HAL_TIM_GET_COMPARE(fan->htim_tacho, fan->tacho_channel);

	// Calculate the time between the falling edges
	uint32_t timeDiff = fan->current_read - fan->last_read;

	// Calculate the speed of the fan
	fan->current_speed =
			60.0 * APB1PERIPH_BASE / (timeDiff * TACHO_PULSE_PER_REV
			* (fan->prescaler+1) * (fan->autoreload+1));

	return fan->current_speed;
}

float pwm_fan_update(PWM_Fan_HandleTypeDef *fan)
{
     // Read TACHO input capture value to calculate current speed
	switch(fan->mode) {
	case PWM_FAN_PCONTROL: {
		// Calculate error
		float error = fan->target_speed - fan->current_speed;
		// Calculate PWM duty cycle
		uint32_t pwm_value = fan->ctrl_gain * fan->target_speed;
		// Set PWM duty cycle
		pwm_fan_set_duty_cycle_raw(fan, pwm_value);
		break;
	}
	case PWM_FAN_CALIBRATION_START:
		break;
	case PWM_FAN_CALIBRATION_MIN_SPEED:
		break;
	case PWM_FAN_CALIBRATION_MAX_SPEED:
		break;
	default:
		break;
	}

	if(pwm_fan_is_stopped(fan)) fan->current_speed = 0.0f;
    return fan->current_speed;
}

_Bool pwm_fan_is_stopped(PWM_Fan_HandleTypeDef *fan) {
	uint32_t metric = __HAL_TIM_GET_COMPARE(fan->htim_tacho, fan->tacho_channel);
	if(metric - fan->current_read > TACHO_STOPPED_THRESHOLD) return 1;
	return 0;
}

