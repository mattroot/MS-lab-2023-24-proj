/*
 * strhelp.c
 *
 *  Created on: Feb 6, 2024
 *      Author: why
 */

#include "strhelp.h"

/**
  * @brief  strlen implementation from FreeBSD
  * @note   Returns length of a string inside a char array
  * @param  str : string
  * @retval String length
  */

size_t strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s);
    return(s - str);
}

/**
  * @brief  Pad string with specified character
  * @note   Fill the string with a character until the desired length
  * @param  str : string to pad
  * @param  pad_chr : padding character
  * @param  str_target_length : target length of string
  */
void strpad(const char *str, char pad_chr, uint16_t str_target_length) {
	char* string = str;
	size_t l = strlen(string);
	if(l > str_target_length) {
		for(int i = 0; i < str_target_length; i++) {
			string[i] = '?';
		}
	}
	else if(l < str_target_length) {
		for(int i = l; i < str_target_length; i++) {
			string[i] = pad_chr;
		}
	}
	string[str_target_length] = '\0';
}
