#ifndef PWM_FAN_H
#define PWM_FAN_H

#include "pwm_fan_conf.h"
#include "tim.h"

typedef struct {
    TIM_HandleTypeDef *htim_pwm;
    TIM_HandleTypeDef *htim_tacho;
    uint32_t pwm_channel;
    uint32_t tacho_channel;
    double max_speed;
    double min_speed;
    double target_speed;
    double current_speed;
    double error_integral;
} PWM_Fan_HandleTypeDef;

void pwm_fan_init(PWM_Fan_HandleTypeDef *fan, TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_tacho, uint32_t pwm_channel, uint32_t tacho_channel);
void pwm_fan_calibrate(PWM_Fan_HandleTypeDef *fan);
void pwm_fan_set(PWM_Fan_HandleTypeDef *fan, float target_speed);
void pwm_fan_set_duty_cycle(PWM_Fan_HandleTypeDef *fan, uint32_t duty_cycle);
float pwm_fan_read_speed(PWM_Fan_HandleTypeDef *fan);
float pwm_fan_update(PWM_Fan_HandleTypeDef *fan);

#endif //PWM_FAN_H
