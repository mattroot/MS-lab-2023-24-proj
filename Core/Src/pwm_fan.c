#include "pwm_fan.h"
#include "tim.h"
#include "pcontrol.h"
#include "await.h"

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
    fan->use_pcontrol = 2;

    // Initialize timers
    HAL_TIM_Base_Start_IT(htim_pwm);
    if(htim_tacho != htim_pwm) HAL_TIM_Base_Start_IT(htim_tacho);
    // Initialize PWM
    HAL_TIM_PWM_Start_IT(fan->htim_pwm, fan->pwm_channel);
    // Initialize TACHO input capture
    HAL_TIM_IC_Start_IT(fan->htim_tacho, fan->tacho_channel);

    // Perform calibration
    pwm_fan_calibrate(fan);

    // get controller gain
    fan->ctrl_gain = 1.0 / ((fan->max_speed - fan->min_speed) / fan->autoreload);

    // during init sanely set the speed to around half
    pwm_fan_set(fan, (int) (fan->max_speed - fan->min_speed) / 200 * 100);
}

void pwm_fan_calibrate(PWM_Fan_HandleTypeDef *fan)
{
//	HAL_Delay(PWM_FAN_CALIBRATION_INIT_DELAY);
//
//	// Stop it, check speed
//	__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel, fan->autoreload);
//	HAL_Delay(PWM_FAN_CALIBRATION_DELAY);
//	pwm_fan_update(fan);
//	fan->max_speed = fan->current_speed;
//
//	// Spin it up to max, get speed
//	__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel,
//			fan->autoreload - (uint16_t)(6 * fan->autoreload / 100));
//	HAL_Delay(PWM_FAN_CALIBRATION_DELAY);
//	pwm_fan_update(fan);
//	fan->min_speed = fan->current_speed;

	// It's always on full speed when starting
	// Wait for a result
	while(fan->current_speed < 10) pwm_fan_update(fan);
	fan->max_speed = fan->current_speed;

	// assume it's always stopped for duty cycle 0
	fan->min_speed = 0;

	// Set it to half during init
	pwm_fan_set_duty_cycle(fan, 50);

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
    fan->use_pcontrol = 1;
}

void pwm_fan_set_duty_cycle(PWM_Fan_HandleTypeDef *fan, uint16_t duty_cycle) {
	// Ensure duty is within the valid range
	if (duty_cycle > 100) {
		duty_cycle = 100;
	} else if  (duty_cycle < 0) {
		duty_cycle = 0;
	}
	fan->target_speed = duty_cycle;
	fan->use_pcontrol = 0;
	__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel,
					fan->autoreload - (uint16_t)(fan->target_speed * fan->autoreload / 100));
}

float pwm_fan_update_speed(PWM_Fan_HandleTypeDef *fan)
{
	// Save the last capture time
	fan->last_read = fan->current_read;
	// Read the current capture time
	fan->current_read = __HAL_TIM_GET_COMPARE(fan->htim_tacho, fan->tacho_channel);

	// Calculate the time between the rising edges
	uint32_t timeDiff = fan->current_read - fan->last_read;

	// Calculate the speed of the fan
	if ((fan->current_read - fan->last_read) <= 30) { fan->current_speed = 0.0f; }
	else { fan->current_speed =
			60.0 * APB1PERIPH_BASE / (timeDiff * TACHO_PULSE_PER_REV
			* (fan->prescaler+1) * (fan->autoreload+1)); }

	return fan->current_speed;
}

float pwm_fan_update(PWM_Fan_HandleTypeDef *fan)
{
     // Read TACHO input capture value to calculate current speed

	if(fan->use_pcontrol == 1) {
		// Calculate error and update integral term
		double error = fan->target_speed - fan->current_speed;

		// Calculate PWM duty cycle using a controller
		float ctrl_output = pcontrol(error, fan->ctrl_gain);
		uint32_t pwm_value = (uint32_t)(ctrl_output * __HAL_TIM_GET_AUTORELOAD(fan->htim_pwm) / fan->max_speed);

		// Set PWM duty cycle
		__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel, pwm_value);
	}
	else if (fan->use_pcontrol == 0){
		uint16_t new_compare = fan->autoreload - (uint16_t)(fan->target_speed * fan->autoreload / 100);
		__HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel,
				new_compare);
	}

	if(pwm_fan_is_stopped(fan)) fan->current_speed = 0.0f;
    return fan->current_speed;
}

_Bool pwm_fan_is_stopped(PWM_Fan_HandleTypeDef *fan) {
	uint32_t metric = __HAL_TIM_GET_COMPARE(fan->htim_tacho, fan->tacho_channel);
	if(metric - fan->current_read > TACHO_STOPPED_THRESHOLD) return 1;
	return 0;
}

