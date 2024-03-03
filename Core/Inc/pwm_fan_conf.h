#ifndef PWM_FAN_CONF_H
#define PWM_FAN_CONF_H

#include "pwm_fan.h"

// thank you GPT
#define PWM_FAN_TACHO_PULSE_PER_REV 2
#define PWM_FAN_TACHO_STOPPED_THRESHOLD 100000 // around 3 sec
#define PWM_FAN_CALIBRATION_PERIOD 500000 // around 20 sec
#define PWM_FAN_CALIBRATION_INIT_DUTY 30.0f
#define PWM_FAN_TACHO_BRICKWALL_LIMIT 3200.0f

extern PWM_Fan_HandleTypeDef fan1;
extern PWM_Fan_HandleTypeDef fan2;

#endif //PWM_FAN_CONF_H
