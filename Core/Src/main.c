/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp280.h"
#include "lcd.h"
#include "lcd_config.h"
#include "pwm_fan.h"
#include "serial.h"
#include "strhelp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// I'm going nuts
PWM_Fan_HandleTypeDef fan1;
PWM_Fan_HandleTypeDef fan2;
BMP280_HandleTypedef bmp;

uint16_t fan1_speed  = 0;
uint16_t fan2_speed  = 0;
float temperature = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void generate_fan_display_line(char *str, PWM_Fan_HandleTypeDef *fan);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_CRC_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  // start all the timers
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim5);

  // initialize I2C display
  LCD_I2C_Init(&hlcd3);
  prepare_display();

  // Initialize BMP280 sensor
  bmp280_init_default_params(&bmp.params);
  bmp.addr = BMP280_I2C_ADDRESS_0;
  bmp.i2c = &hi2c1;
  bmp280_init(&bmp, &bmp.params);

  // initialize fans
  pwm_fan_init(&fan1, &htim3, &htim2, TIM_CHANNEL_1, TIM_CHANNEL_1);
  pwm_fan_schedule_calibration(&fan1);

  pwm_fan_init(&fan2, &htim3, &htim2, TIM_CHANNEL_2, TIM_CHANNEL_3);
  pwm_fan_schedule_calibration(&fan2);

  // Start serial output timer
  HAL_TIM_PWM_Start_IT(&htim5, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief  TIM capture callback
  * @note   Called every time the timer grabs a revolution of the fan
  * @param  None
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	// This is pure insanity, don't you ever dare do it like this
	if(htim->Instance == TIM2) {
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
			//HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
			pwm_fan_update_speed(&fan1);
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
			//HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			pwm_fan_update_speed(&fan2);
	}
}

/**
  * @brief  Read BMP280 measurements
  * @param  None
  * @retval None
  */
void grab_bmp_measurement() {
	bmp280_read_float(&bmp, &temperature, NULL, NULL);
}

/**
  * @brief  Post sensor status via UART
  * @note   Posts current measurements via USART3 in CSV-like format
  * @param  None
  * @retval None
  */
void uart_post_sensors() {
	char buffer[100] = "";
	sprintf(buffer, "fan1,%f,%f,%f,%u\r\n",
			fan1.current_speed,
			fan1.target_duty_cycle,
			fan1.target_speed,
			(uint16_t)fan1.mode
			);
	HAL_UART_Transmit(&huart3, buffer, strlen(buffer), 100);

	sprintf(buffer, "fan2,%f,%f,%f,%u\r\n",
			fan2.current_speed,
			fan2.target_duty_cycle,
			fan2.target_speed,
			(uint16_t)fan2.mode
			);
	HAL_UART_Transmit(&huart3, buffer, strlen(buffer), 100);

	sprintf(buffer, "temp,%f\r\n", temperature);
	HAL_UART_Transmit(&huart3, buffer, strlen(buffer), 100);
}

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
			sprintf(str, "Init... Stage 1");
			break;
		case PWM_FAN_CALIBRATION_MIN_SPEED:
			sprintf(str, "Calibrating...1");
			break;
		case PWM_FAN_CALIBRATION_MAX_START:
			sprintf(str, "Init... Stage 2");
			break;
		case PWM_FAN_CALIBRATION_MAX_SPEED:
			sprintf(str, "Calibrating...2");
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
			sprintf(&tgt_speed_str, "%u", (uint16_t) fan->target_speed);
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

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM8 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

	if (htim->Instance == TIM3) {
	    fan1_speed = pwm_fan_update(&fan1);
	    fan2_speed = pwm_fan_update(&fan2);
	  }

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM8) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
