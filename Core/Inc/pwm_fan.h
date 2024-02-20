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
    uint32_t tacho_autoreload;
    uint16_t tacho_prescaler;

    // fan characteristics
    float max_speed;
    float min_speed;

    // fan speed control
    float target_duty_cycle;
    float target_speed;

    // fan speed monitoring
    float current_speed;
    float speed_error;
    float ctrl_error;

    // TACHO estimation helpers
    uint32_t last_read;
    uint32_t current_read;

    // calibration helpers
    uint32_t calibration_cycle_counter;

    // controller settings
    float ctrl_gain;
    uint16_t ctrl_inertia;
    uint16_t mode;
} PWM_Fan_HandleTypeDef;

enum PWM_FAN_CTRL_MODE {
	PWM_FAN_UNCONFIGURED = 0,
	PWM_FAN_DIRECT  = 1,
	PWM_FAN_PCONTROL = 2,
	PWM_FAN_CALIBRATION_START = 3,
	// PWM_FAN_CALIBRATION_START_LEVEL = 4,
	PWM_FAN_CALIBRATION_MIN_SPEED = 5,
	PWM_FAN_CALIBRATION_MAX_SPEED = 6
};

void pwm_fan_init(PWM_Fan_HandleTypeDef *fan,
		TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_tacho,
		uint16_t pwm_channel,
		uint16_t tacho_channel);
void pwm_fan_schedule_calibration(PWM_Fan_HandleTypeDef *fan);
void pwm_fan_set(PWM_Fan_HandleTypeDef *fan, float target_speed);
void pwm_fan_set_duty_cycle(PWM_Fan_HandleTypeDef *fan, float target_duty_cycle);
float pwm_fan_set_duty_cycle_raw(PWM_Fan_HandleTypeDef *fan, uint16_t compare_register);
float pwm_fan_update_speed(PWM_Fan_HandleTypeDef *fan);
float pwm_fan_update(PWM_Fan_HandleTypeDef *fan);
uint16_t pwm_fan_is_stopped(PWM_Fan_HandleTypeDef *fan);
void pwm_fan_generate_display(PWM_Fan_HandleTypeDef *fan, char *str, uint16_t strlen);

#endif //PWM_FAN_H
