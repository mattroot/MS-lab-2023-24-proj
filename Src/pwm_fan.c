#include "pwm_fan.h"
#include "tim.h"

void fan_pwm_init()
{
    // TODO: this is boilerplate, change it
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

void fan_pwm_set(uint16_t duty)
{
    // TODO: this is boilerplate, change it
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty);
}