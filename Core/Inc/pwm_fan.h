#ifndef PWM_FAN_H
#define PWM_FAN_H

#include "pwm_fan_conf.h"
#include "tim.h"

typedef struct {
	// handles
    TIM_HandleTypeDef *htim_pwm;
    TIM_HandleTypeDef *htim_tacho;
    uint16_t pwm_channel;
    uint16_t tacho_channel;

    // constant registers for easy access
    uint16_t autoreload;
    uint16_t prescaler;

    // fan characteristics
    float max_speed;
    float min_speed;
    float start_duty_cycle;

    // fan speed control
    float target_duty_cycle;
    float target_speed;

    // fan speed monitoring
    float current_speed;
    float speed_error;

    // TACHO estimation helpers
    uint16_t last_read;
    uint16_t current_read;

    // calibration helpers
    uint16_t calibration_halt_counter;
    uint16_t calibration_cycle_counter;

    // controller settings
    float ctrl_gain;
    uint16_t mode;
} PWM_Fan_HandleTypeDef;

enum PWM_FAN_CTRL_MODE {
	PWM_FAN_DIRECT,
	PWM_FAN_PCONTROL,
	PWM_FAN_CALIBRATION_START,
	PWM_FAN_CALIBRATION_MIN_SPEED,
	PWM_FAN_CALIBRATION_MAX_SPEED
};

void pwm_fan_init(PWM_Fan_HandleTypeDef *fan,
		TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_tacho,
		uint16_t pwm_channel,
		uint16_t tacho_channel);
void pwm_fan_calibrate(PWM_Fan_HandleTypeDef *fan);
void pwm_fan_set(PWM_Fan_HandleTypeDef *fan, float target_speed);
void pwm_fan_set_duty_cycle(PWM_Fan_HandleTypeDef *fan, float target_duty_cycle);
float pwm_fan_update_speed(PWM_Fan_HandleTypeDef *fan);
float pwm_fan_update(PWM_Fan_HandleTypeDef *fan);
_Bool pwm_fan_is_stopped(PWM_Fan_HandleTypeDef *fan);

extern TIM_HandleTypeDef *await_timer;

#endif //PWM_FAN_H
