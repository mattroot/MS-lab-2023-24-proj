#ifndef SERIAL_H
#define SERIAL_H

#include "usart.h"
#include "stm32f7xx_it.h"

void serial_post(UART_HandleTypeDef *huart, char *message, uint16_t length);
void serial_recv(UART_HandleTypeDef *huart, char* message, uint16_t length);



#endif //SERIAL_H
