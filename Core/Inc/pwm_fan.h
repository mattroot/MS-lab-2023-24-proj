#ifndef PWM_FAN_H
#define PWM_FAN_H

#include "pwm_fan_conf.h"
#include "tim.h"
#include "pcontrol.h"
#include "await.h"

typedef struct {
    TIM_HandleTypeDef *htim_pwm;
    TIM_HandleTypeDef *htim_tacho;
    uint32_t pwm_channel;
    uint32_t tacho_channel;
    uint16_t autoreload;
    uint16_t prescaler;
    double max_speed;
    double min_speed;
    double target_speed;
    double current_speed;
    uint16_t last_read;
    uint16_t current_read;
    double ctrl_gain;
    int use_pcontrol;
} PWM_Fan_HandleTypeDef;

void pwm_fan_init(PWM_Fan_HandleTypeDef *fan,
		TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_tacho,
		uint32_t pwm_channel,
		uint32_t tacho_channel);
void pwm_fan_calibrate(PWM_Fan_HandleTypeDef *fan);
void pwm_fan_set(PWM_Fan_HandleTypeDef *fan, float target_speed);
void pwm_fan_set_duty_cycle(PWM_Fan_HandleTypeDef *fan, uint16_t duty_cycle);
float pwm_fan_update_speed(PWM_Fan_HandleTypeDef *fan);
float pwm_fan_update(PWM_Fan_HandleTypeDef *fan);
_Bool pwm_fan_is_stopped(PWM_Fan_HandleTypeDef *fan);

extern TIM_HandleTypeDef *await_timer;

#endif //PWM_FAN_H
