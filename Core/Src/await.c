#include "await.h"

void await(uint16_t ms) {

	uint32_t start_time = HAL_GetTick();  // Get the current time

	while ((HAL_GetTick() - start_time) < ms) {
		// Wait for the specified time
	}

}
