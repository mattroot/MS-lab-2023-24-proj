/*
 * lcdal.c - LCD Abstraction Layer
 *
 *  Created on: Mar 3, 2024
 *      Author: why
 */

#include "lcdal.h"
#include "lcd.h"
#include "lcd_config.h"
#include "pwm_fan_conf.h"
#include "strhelp.h"
#include "temp.h"

/**
  * @brief  Prepare display to display measurements
  * @note   Renders a table on the character display to render the measurements in
  * @param  None
  * @retval None
  */
void prepare_display() {
  LCD_I2C_printStr(&hlcd3, "SNSR READ TRGT DUTY");
  LCD_I2C_SetCursor(&hlcd3, 1, 0);
  LCD_I2C_printStr(&hlcd3, "Fan1");
  LCD_I2C_SetCursor(&hlcd3, 2, 0);
  LCD_I2C_printStr(&hlcd3, "Fan2");
  LCD_I2C_SetCursor(&hlcd3, 3, 0);
  LCD_I2C_printStr(&hlcd3, "Temp");
}

/**
  * @brief  Display update callback
  * @note   This function is called every time display needs to be updated with fresh measurements
  * @param  None
  * @retval None
  */
void update_display() {

	// display str buffer
	char disp[15];

	// fan 1
	LCD_I2C_SetCursor(&hlcd3, 1, 5);
	generate_fan_display_line(disp, &fan1);
	LCD_I2C_printStr(&hlcd3, disp);

	// fan 2
	LCD_I2C_SetCursor(&hlcd3, 2, 5);
	generate_fan_display_line(disp, &fan2);
	LCD_I2C_printStr(&hlcd3, disp);
	// BMP280
	LCD_I2C_SetCursor(&hlcd3, 3, 5);
	sprintf(disp, "%dC", (int) temperature);
	LCD_I2C_printStr(&hlcd3, disp);
}

/**
  * @brief  Generate fan status line for LCD display
  * @note   This function is called  when TIM8 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  str : string to save the status line into
  * @param  fan : fan handle
  * @retval None
  */
void generate_fan_display_line(char *str, PWM_Fan_HandleTypeDef *fan) {
	char* curr_speed_str[5], tgt_duty_str[5], tgt_speed_str[5];
	switch(fan->mode) {
		case PWM_FAN_CALIBRATION_START:
			sprintf(str, "C. Start       ");
			break;
		case PWM_FAN_CALIBRATION_START_DUTY:
			sprintf(str, "C. Strt Duty   ");
			break;
		case PWM_FAN_CALIBRATION_MIN_SPEED:
			sprintf(str, "C. Min Speed   ");
			break;
		case PWM_FAN_CALIBRATION_MAX_START:
			sprintf(str, "C. Max Strt    ");
			break;
		case PWM_FAN_CALIBRATION_MAX_SPEED:
			sprintf(str, "C. Max Speed   ");
			break;
		case PWM_FAN_UNCONFIGURED:
			sprintf(str, "Unconfigured   ");
			break;
		case PWM_FAN_DIRECT:
			sprintf(&curr_speed_str, "%u", (uint16_t) fan->current_speed);
			strpad(&curr_speed_str, ' ', 4);
			sprintf(&tgt_duty_str, "%u%%", (uint16_t) fan->target_duty_cycle);
			strpad(&tgt_duty_str, ' ', 4);
			sprintf(str, "%s Manu %s ",
					&curr_speed_str, &tgt_duty_str);
			break;
		case PWM_FAN_PCONTROL:
			sprintf(&curr_speed_str, "%u", (uint16_t) fan->current_speed);
			strpad(&curr_speed_str, ' ', 4);
			sprintf(&tgt_speed_str, "%u", (uint16_t) fan->hctrl->target_speed);
			strpad(&tgt_speed_str, ' ', 4);
			sprintf(&tgt_duty_str, "%u%%", (uint16_t) fan->target_duty_cycle);
			strpad(&tgt_duty_str, ' ', 4);
			sprintf(str, "%s %s %s ",
					&curr_speed_str,
					&tgt_speed_str,
					&tgt_duty_str);
			break;
		default:
			sprintf(str, " !!! ERROR !!! ");
			break;
		}
}
