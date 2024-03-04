/*
 * strhelp.h
 *
 *  Created on: Feb 6, 2024
 *      Author: Mateusz Korzeniewski <mateusz.korzeniewski@student.put.poznan.pl>
 */

#ifndef INC_STRHELP_H_
#define INC_STRHELP_H_

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
void strpad(const char *str, char pad_chr, uint16_t str_target_length);

#endif /* INC_STRHELP_H_ */
