#ifndef SERIAL_H
#define SERIAL_H

#include "serial_conf.h"
#include "usart.h"

void serial_post(UART_HandleTypeDef *huart, char *message, uint16_t length);

#endif //SERIAL_H
