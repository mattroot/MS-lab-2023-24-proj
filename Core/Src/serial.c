#include "serial.h"
#include "serial_conf.h"
#include "pwm_fan_conf.h"
#include "pwm_fan.h"
#include "strhelp.h"
#include <stdio.h>

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
    if(!strncmp(chop[0], "PNG", 3)) {
        snprintf(response, sizeof(response), "pong!\r\n");
    }
    else if(!strncmp(chop[0], "GCL", 3)) {
        if(!strncmp(chop[1], "fan1", 4)) {
            snprintf(response, sizeof(response), "ACK,GCL,fan1,%f,%f\r\n",
                    fan1.min_speed,
                    fan1.max_speed);
        } else if(!strncmp(chop[1], "fan2", 4)) {
            snprintf(response, sizeof(response), "ACK,GCL,fan2,%f,%f\r\n",
                    fan2.min_speed,
                    fan2.max_speed);
        } else {
            snprintf(response, sizeof(response), "NAK\r\n");
        }
    }
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
    else {
        snprintf(response, sizeof(response), "NAK\r\n");
    }

    // Send response
    int response_length = strlen(response);
    if (response_length > 0) {
        HAL_UART_Transmit(huart, (uint8_t *)response, response_length, 50);
    }
}
