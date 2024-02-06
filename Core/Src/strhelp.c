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
  * @brief  Pad string with spaces
  * @note   Fill the string with a character until the desired length
  * @param  str : string to pad
  * @param  pad_chr : padding character
  * @param  str_target_length : target length of string
  */
void strpad(const char *str, char pad_chr, uint16_t str_target_length) {

}
