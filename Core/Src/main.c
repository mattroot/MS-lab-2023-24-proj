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
#include "lcd.h"
#include "lcd_config.h"
#include "pwm_fan.h"
#include "temp_sensor.h"
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

uint16_t fan1_speed=0;
uint16_t fan2_speed=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_CRC_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  // initialize I2C display
  // workaround: run the init a few times to make sure the display is empty
  for(int i = 0; i <= 3; i++) {
	  LCD_I2C_Init(&hlcd3);
	  HAL_Delay(500);
  }
  prepare_display();


  // start all the timers
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start_IT(&htim5);

  // initialize fans
  pwm_fan_init(&fan1, &htim3, &htim2, TIM_CHANNEL_1, TIM_CHANNEL_1);
  //pwm_fan_schedule_calibration(&fan1);
  // manual calibration - fan 1
//  fan1.max_speed = 1400;
//  fan1.min_speed = 200;
//  fan1.start_duty_cycle = 5;
//  fan1.target_speed =  600;
//  fan1.ctrl_inertia = fan1.start_duty_cycle * (fan1.autoreload / 100);
//  fan1.ctrl_gain = (fan1.autoreload - fan1.ctrl_inertia) / (fan1.max_speed - fan1.min_speed);
//  fan1.mode = PWM_FAN_PCONTROL;

  // hard set duty cycle - fan 1
  fan1.target_duty_cycle = 100.0f;
  fan1.mode = PWM_FAN_DIRECT;

  pwm_fan_init(&fan2, &htim4, &htim2, TIM_CHANNEL_1, TIM_CHANNEL_3);
  //pwm_fan_schedule_calibration(&fan2);
  // manual calibration - fan 2
//  fan2.max_speed = 3000;
//  fan2.min_speed = 200;
//  fan2.start_duty_cycle = 5;
//  fan2.target_speed =  1200;
//  fan2.ctrl_inertia = fan2.start_duty_cycle * (fan2.autoreload / 100);
//  fan2.ctrl_gain = (fan2.autoreload - fan2.ctrl_inertia) / (fan2.max_speed - fan2.min_speed);
//  fan2.mode = PWM_FAN_PCONTROL;

  // hard set duty cycle - fan 2
    fan2.target_duty_cycle = 100.0f;
    fan2.mode = PWM_FAN_DIRECT;

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
}

void prepare_display() {
  LCD_I2C_printStr(&hlcd3, "SNSR READ TRGT DUTY");
  LCD_I2C_SetCursor(&hlcd3, 1, 0);
  LCD_I2C_printStr(&hlcd3, "Fan1");
  LCD_I2C_SetCursor(&hlcd3, 2, 0);
  LCD_I2C_printStr(&hlcd3, "Fan2");
  LCD_I2C_SetCursor(&hlcd3, 3, 0);
  LCD_I2C_printStr(&hlcd3, "Temp");
}

void update_display() {
	// fan 1
	LCD_I2C_SetCursor(&hlcd3, 1, 5);
	switch(fan1.mode) {
	case PWM_FAN_CALIBRATION_START:
		LCD_I2C_printStr(&hlcd3, "Calibrating...");
		break;
	case PWM_FAN_CALIBRATION_START_LEVEL:
		LCD_I2C_printStr(&hlcd3, "Calibrating...1");
		break;
	case PWM_FAN_CALIBRATION_MIN_SPEED:
		LCD_I2C_printStr(&hlcd3, "Calibrating...2");
		break;
	case PWM_FAN_CALIBRATION_MAX_SPEED:
		LCD_I2C_printStr(&hlcd3, "Calibrating...3");
		break;
	case PWM_FAN_UNCONFIGURED:
		LCD_I2C_printStr(&hlcd3, "Unconfigured");
		break;
	case PWM_FAN_DIRECT:
		LCD_I2C_printf(&hlcd3, "%u Manu %u",
				(uint16_t) fan1.current_speed,
				(uint16_t) fan1.target_duty_cycle);
		break;
	case PWM_FAN_PCONTROL:
		LCD_I2C_printStr(&hlcd3, "Trgt");
		break;
	default:
		LCD_I2C_printStr(&hlcd3, "!!! ERROR !!!");
		break;
	}

	// fan 2
	LCD_I2C_SetCursor(&hlcd3, 2, 5);
	switch(fan2.mode) {
	case PWM_FAN_CALIBRATION_START:
		LCD_I2C_printStr(&hlcd3, "Calibrating...");
		break;
	case PWM_FAN_CALIBRATION_START_LEVEL:
		LCD_I2C_printStr(&hlcd3, "Calibrating...1");
		break;
	case PWM_FAN_CALIBRATION_MIN_SPEED:
		LCD_I2C_printStr(&hlcd3, "Calibrating...2");
		break;
	case PWM_FAN_CALIBRATION_MAX_SPEED:
		LCD_I2C_printStr(&hlcd3, "Calibrating...3");
		break;
	case PWM_FAN_UNCONFIGURED:
		LCD_I2C_printStr(&hlcd3, "Unconfigured");
		break;
	case PWM_FAN_DIRECT:
		LCD_I2C_printf(&hlcd3, "%u Manu %u",
						(uint16_t) fan2.current_speed,
						(uint16_t) fan2.target_duty_cycle);
		break;
	case PWM_FAN_PCONTROL:
		LCD_I2C_printStr(&hlcd3, "Trgt");
		break;
	default:
		LCD_I2C_printStr(&hlcd3, "!!! ERROR !!!");
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

	  }
	if (htim->Instance == TIM4) {
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
