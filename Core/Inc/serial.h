#ifndef SERIAL_H
#define SERIAL_H

#include "usart.h"
#include "stm32f7xx_it.h"

extern _Bool serial_post_telemetry;

void serial_recv(UART_HandleTypeDef *huart, char* message, uint16_t length);
void serial_post_sensors();


#endif //SERIAL_H
