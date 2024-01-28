#include "pcontrol.h"

float pcontrol(float error, float gain) {
	return error * gain;
}
