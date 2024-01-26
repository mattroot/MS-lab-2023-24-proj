#include "pwm_fan.h"
#include "tim.h"

void pwm_fan_init(PWM_Fan_HandleTypeDef *fan, TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_tacho, uint32_t pwm_channel, uint32_t tacho_channel)
{
    fan->htim_pwm = htim_pwm;
    fan->htim_tacho = htim_tacho;
    fan->pwm_channel = pwm_channel;
    fan->tacho_channel = tacho_channel;
    fan->target_speed = 0.0f;
    fan->current_speed = 0.0f;
    fan->error_integral = 0.0f;

    // Initialize PWM
    HAL_TIM_PWM_Start(fan->htim_pwm, fan->pwm_channel);

    // Initialize TACHO input capture
    HAL_TIM_IC_Start_IT(fan->htim_tacho, fan->tacho_channel);

    // Initialize PID
    arm_pid_init_f32(fan->pid, 1.0f);
}

void pwm_fan_set(PWM_Fan_HandleTypeDef *fan, double target_speed)
{
    // Ensure target speed is within the valid range
    if (target_speed > fan->max_speed) {
        target_speed = fan->max_speed;
    } else if (target_speed < fan->min_speed) {
        target_speed = fan->min_speed;
    }

    fan->target_speed = target_speed;
}

double pwm_fan_update(PWM_Fan_HandleTypeDef *fan)
{
     // Read TACHO input capture value to calculate current speed
    uint32_t tacho_pulse_count = __HAL_TIM_GET_COUNTER(fan->htim_tacho);
    fan->current_speed = (double)tacho_pulse_count * 60.0f / TACHO_PULSE_PER_REV;

    // Calculate error and update integral term
    double error = fan->target_speed - fan->current_speed;
    fan->error_integral += error;

    // Calculate PWM duty cycle using a PID controller
    double pid_output = arm_pid_f32(fan->pid, error);
    uint32_t pwm_value = (uint32_t)(pid_output * __HAL_TIM_GET_AUTORELOAD(fan->htim_pwm) / fan->max_speed);

    // Set PWM duty cycle
    __HAL_TIM_SET_COMPARE(fan->htim_pwm, fan->pwm_channel, pwm_value);

    return fan->current_speed;
}
