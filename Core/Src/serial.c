#include "serial.h"
#include "serial_conf.h"
#include "pwm_fan_conf.h"
#include "pwm_fan.h"
#include "strhelp.h"
#include "gpio.h"
#include "temp.h"
#include <stdio.h>

_Bool serial_post_telemetry = true;

void serial_recv(UART_HandleTypeDef *huart, char* message, uint16_t length) {
    int i = 0;
    char chop[4][SERIAL_MSG_LEN] = {0}; // Initialize with null terminators
    char *token;
    char response[SERIAL_POST_LEN] = "";
    uint16_t mode;
    int16_t input;

    token = strtok(message, ",");
    snprintf(chop[i], sizeof(chop[i]), "%s", token);
    // We accept only 4 values here
    while(token != NULL && i < 4) {
        i++;
        token = strtok(NULL, ",");
        if (token != NULL) {
            snprintf(chop[i], sizeof(chop[i]), "%s", token);
        }
    }

    // The great switching
    // ping
    if(!strncmp(chop[0], "PNG", 3)) {
        snprintf(response, sizeof(response), "pong!\r\n");
    }
    // get calibration parameters
    else if(!strncmp(chop[0], "GCL", 3)) {
        if(!strncmp(chop[1], "fan1", 4)) {
            snprintf(response, sizeof(response), "ACK,GCL,fan1,%f,%f,%f\r\n",
                    fan1.min_speed,
                    fan1.max_speed,
					fan1.start_pwm_percent);
        } else if(!strncmp(chop[1], "fan2", 4)) {
            snprintf(response, sizeof(response), "ACK,GCL,fan2,%f,%f,%f\r\n",
                    fan2.min_speed,
                    fan2.max_speed,
					fan2.start_pwm_percent);
        } else {
            snprintf(response, sizeof(response), "NAK\r\n");
        }
    }
    // set fan value
    else if(!strncmp(chop[0], "SET", 3)) {
        if(!strncmp(chop[1], "fan1", 4)) {
            snprintf(response, sizeof(response), "ACK,SET,fan1\r\n");
            mode = atoi(chop[2]);
            input = atoi(chop[3]);
            fan1.mode = mode;
            switch(mode) {
                case PWM_FAN_DIRECT:
                    pwm_fan_set_duty_cycle(&fan1, input);
                    break;
                case PWM_FAN_PCONTROL:
                    pwm_fan_set(&fan1, input);
                    break;
                default:
                    snprintf(response, sizeof(response), "NAK\r\n");
            }
        } else if(!strncmp(chop[1], "fan2", 4)) {
            snprintf(response, sizeof(response), "ACK,SET,fan2\r\n");
            mode = atoi(chop[2]);
            input = atoi(chop[3]);
            fan2.mode = mode;
            switch(mode) {
                case PWM_FAN_DIRECT:
                    pwm_fan_set_duty_cycle(&fan2, input);
                    break;
                case PWM_FAN_PCONTROL:
                    pwm_fan_set(&fan2, input);
                    break;
                default:
                    snprintf(response, sizeof(response), "NAK\r\n");
            }
        } else {
            snprintf(response, sizeof(response), "NAK");
        }
    }
    // request calibration
    else if(!strncmp(chop[0], "CAL", 3)) {
        if(!strncmp(chop[1], "fan1", 4)) {
            snprintf(response, sizeof(response), "ACK,CAL,fan1\r\n");
            pwm_fan_schedule_calibration(&fan1);
        } else if(!strncmp(chop[1], "fan2", 4)) {
            snprintf(response, sizeof(response), "ACK,CAL,fan2\r\n");
            pwm_fan_schedule_calibration(&fan2);
        } else {
            snprintf(response, sizeof(response), "NAK\r\n");
        }
    }
    // soft reset
    else if(!strncmp(chop[0], "RST", 3)) {
    	NVIC_SystemReset();
    }
    // reinitialize display
    else if(!strncmp(chop[0], "RSD", 3)) {
    	prepare_display();
	}
    // enable or disable telemetry posting
    else if(!strncmp(chop[0], "TEL", 3)) {
            if(!strncmp(chop[1], "0", 1)) {
                snprintf(response, sizeof(response), "ACK,TEL,0\r\n");
                serial_post_telemetry = false;
            } else if(!strncmp(chop[1], "1", 1)) {
                snprintf(response, sizeof(response), "ACK,TEL,1\r\n");
                serial_post_telemetry = true;
            } else {
                snprintf(response, sizeof(response), "NAK\r\n");
            }
        }
    // set p_gain
	else if(!strncmp(chop[0], "SPG", 3)) {
		if(!strncmp(chop[1], "fan1", 4)) {
			snprintf(response, sizeof(response), "ACK,SPG,fan1\r\n");
			input = atoi(chop[2]);
			fan1.hctrl->p_gain = (float)input;
		} else if(!strncmp(chop[1], "fan2", 4)) {
			snprintf(response, sizeof(response), "ACK,SPG,fan2\r\n");
			input = atof(chop[2]);
			fan2.hctrl->p_gain = (float)input;
		} else {
			snprintf(response, sizeof(response), "NAK\r\n");
		}
	}
    else {
        snprintf(response, sizeof(response), "NAK\r\n");
    }

    // Send response
    int response_length = strlen(response);
    if (response_length > 0) {
        HAL_UART_Transmit(huart, (uint8_t *)response, response_length, 50);
    }
}

/**
  * @brief  Post sensor status via serial
  * @note   Posts current measurements via USART3 in CSV-like format
  * @param  None
  * @retval None
  */
void serial_post_sensors() {
	if(serial_post_telemetry) {
		char buffer[100] = "";
		sprintf(buffer, "fan1,%f,%f,%f,%u\r\n",
				fan1.current_speed,
				fan1.target_duty_cycle,
				fan1.hctrl->target_speed,
				(uint16_t)fan1.mode
				);
		HAL_UART_Transmit(&huart3, buffer, strlen(buffer), 100);

		sprintf(buffer, "fan2,%f,%f,%f,%u\r\n",
				fan2.current_speed,
				fan2.target_duty_cycle,
				fan2.hctrl->target_speed,
				(uint16_t)fan2.mode
				);
		HAL_UART_Transmit(&huart3, buffer, strlen(buffer), 100);

		sprintf(buffer, "temp,%f\r\n", temperature);
		HAL_UART_Transmit(&huart3, buffer, strlen(buffer), 100);
	}
}
