#ifndef SERIAL_H
#define SERIAL_H

#include "serial_conf.h"
#include "usart.h"

typedef struct {
	UART_HandleTypeDef huart;
	char buffer[300];
	uint8_t msg_length;
	_Bool use_crc;
} serial_HandleTypedef;

void serial_init(UART_HandleTypeDef *huart);
void serial_post(serial_HandleTypedef *hserial);
void serial_encode_msg(serial_HandleTypedef *hserial, char **msg, uint32_t msg_length);
void serial_decode_msg(serial_HandleTypedef *hserial, char **msg, uint32_t msg_length);

#endif //SERIAL_H
