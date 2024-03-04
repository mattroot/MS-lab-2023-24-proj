#ifndef PWM_FAN_CONF_H
#define PWM_FAN_CONF_H

#include "pwm_fan.h"

#define PWM_FAN_TACHO_PULSE_PER_REV 2
#define PWM_FAN_TACHO_STOPPED_THRESHOLD 100000 // around 3 sec
#define PWM_FAN_START_DELAY 8
#define PWM_FAN_CALIBRATION_PERIOD 8 // around 20 sec
#define PWM_FAN_CALIBRATION_INIT_DUTY 30.0f // unused, left for... "compatibility reasons"
#define PWM_FAN_TACHO_BRICKWALL_LIMIT 3200.0f

// **************************************
// *   FAN SPEED FILTER CONFIGURATION   *
// *      Make sure to select one!      *
// **************************************

// Cutoff filter configuration
#define PWM_FAN_USE_CUTOFF_FILTER
#ifdef PWM_FAN_USE_CUTOFF_FILTER

// factor of threshold
#define PWM_FAN_CUTOFF_FILTER_FACTOR 1.1f

#endif

// CMSIS FIR filter configuration
//#define PWM_FAN_USE_FIR_FILTER
#ifdef PWM_FAN_USE_FIR_FILTER

#define PWM_FAN_FIR_FILTER_ORDER 6
static const float PWM_FAN_FIR_FILTER_COEFFICIENTS[PWM_FAN_FIR_FILTER_ORDER] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

#endif

// EWMA filter configuration
//#define PWM_FAN_USE_EWMA_FILTER
#ifdef PWM_FAN_USE_EWMA_FILTER

// filter factor
#define PWM_FAN_EWMA_FILTER_FACTOR 0.05f

#endif

// **************************************
// * END FAN SPEED FILTER CONFIGURATION *
// **************************************

extern PWM_Fan_HandleTypeDef fan1;
extern PWM_Fan_HandleTypeDef fan2;

#endif //PWM_FAN_CONF_H
