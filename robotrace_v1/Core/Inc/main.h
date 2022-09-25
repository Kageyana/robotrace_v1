/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
// #include "adc.h"
// #include "io.h"
#include "timer.h"
#include "AQM0802A.h"
// #include "ICM20608.h"
#include "BNO055.h"
#include "setup.h"
#include "control.h"
#include "encoder.h"
#include "switch.h"
#include "led.h"
#include "markerSensor.h"
#include "lineSensor.h"
#include "lineTrace.h""
#include "velocityctrl.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Sidesensor1_Pin GPIO_PIN_13
#define Sidesensor1_GPIO_Port GPIOC
#define Output1_Pin GPIO_PIN_14
#define Output1_GPIO_Port GPIOC
#define Output2_Pin GPIO_PIN_15
#define Output2_GPIO_Port GPIOC
#define SW_5xis_Pin GPIO_PIN_0
#define SW_5xis_GPIO_Port GPIOC
#define Sidesensor2_Pin GPIO_PIN_1
#define Sidesensor2_GPIO_Port GPIOB
#define Input2_Pin GPIO_PIN_2
#define Input2_GPIO_Port GPIOB
#define CS_IMU_Pin GPIO_PIN_12
#define CS_IMU_GPIO_Port GPIOB
#define CS_MSD_Pin GPIO_PIN_13
#define CS_MSD_GPIO_Port GPIOB
#define Input1_Pin GPIO_PIN_8
#define Input1_GPIO_Port GPIOC
#define SW_MSD_Pin GPIO_PIN_9
#define SW_MSD_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_12
#define LED_R_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_4
#define LED_G_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_5
#define LED_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
extern uint16_t 		analogVal[14];		// ADC結果格納�?��??

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern I2C_HandleTypeDef hi2c1;

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
