#ifndef PWM_FAN_CONF_H
#define PWM_FAN_CONF_H

#include "pwm_fan.h"

// thank you GPT
#define TACHO_PULSE_PER_REV 2
#define TACHO_STOPPED_THRESHOLD 125000 // around 3 sec
#define PWM_FAN_CALIBRATION_PERIOD 500000 // around 20 sec
#define PWM_FAN_CALIBRATION_INIT_DUTY 30.0f
#define TACHO_BRICKWALL_LIMIT 3200.0f

extern PWM_Fan_HandleTypeDef fan1;
extern PWM_Fan_HandleTypeDef fan2;

#endif //PWM_FAN_CONF_H
