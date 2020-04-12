/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Led3_Pin GPIO_PIN_1
#define Led3_GPIO_Port GPIOC
#define Led2_Pin GPIO_PIN_2
#define Led2_GPIO_Port GPIOC
#define Led1_Pin GPIO_PIN_3
#define Led1_GPIO_Port GPIOC
#define Eth_CS_Pin GPIO_PIN_4
#define Eth_CS_GPIO_Port GPIOA
#define Eth_WOL_Pin GPIO_PIN_4
#define Eth_WOL_GPIO_Port GPIOC
#define Eth_INT_Pin GPIO_PIN_5
#define Eth_INT_GPIO_Port GPIOC
#define Eth_INT_EXTI_IRQn EXTI9_5_IRQn
#define Eth_CLK_Pin GPIO_PIN_0
#define Eth_CLK_GPIO_Port GPIOB
#define Eth_RST_Pin GPIO_PIN_1
#define Eth_RST_GPIO_Port GPIOB
#define Gerkon_Pin GPIO_PIN_12
#define Gerkon_GPIO_Port GPIOB
#define V_Tr_Off_Pin GPIO_PIN_15
#define V_Tr_Off_GPIO_Port GPIOB
#define V_EC_On_Pin GPIO_PIN_6
#define V_EC_On_GPIO_Port GPIOC
#define ADC2_Pwr_Pin GPIO_PIN_7
#define ADC2_Pwr_GPIO_Port GPIOC
#define ADC1_Pwr_Pin GPIO_PIN_8
#define ADC1_Pwr_GPIO_Port GPIOC
#define USB_PwrOn_Pin GPIO_PIN_10
#define USB_PwrOn_GPIO_Port GPIOA
#define BT_State_Pin GPIO_PIN_15
#define BT_State_GPIO_Port GPIOA
#define O_1_Pin GPIO_PIN_12
#define O_1_GPIO_Port GPIOC
#define O_2_Pin GPIO_PIN_2
#define O_2_GPIO_Port GPIOD
#define O_3_Pin GPIO_PIN_3
#define O_3_GPIO_Port GPIOB
#define O_4_Pin GPIO_PIN_4
#define O_4_GPIO_Port GPIOB
#define SIM_Rst_Pin GPIO_PIN_5
#define SIM_Rst_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

void TimingDelay_Decrement(void);
//void Delay(__IO uint32_t nTime);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
