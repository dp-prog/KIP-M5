/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include "uip.h"
//#include "uip_arp.h"
//#include "enc28j60.h"

#include "ADS1115.h"

#include "config.h"
extern KIP_Data_TypeDef KIP_Data;

extern volatile	uint8_t delay_start;		// формирование паузы между замерами

static __IO uint32_t TimingDelay;

uint8_t	int_Eth = 0;

extern uint8_t delay_arp;

// Todo раскидать глобалки
//uint8_t pbuffer[32];
//uint8_t	count_i;


#include "Mercury.h"

extern void my_printf(UART_HandleTypeDef *huart_out, const char *fmt, ...);

extern void	Sim80x_RxCallBack(void);
//#include "Sim80x.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
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
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM16_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_CRC_Init();
  MX_RTC_Init();
  MX_TIM4_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

  HAL_GPIO_WritePin(Eth_RST_GPIO_Port, Eth_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(Eth_RST_GPIO_Port, Eth_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(1);

  /* Формирование настроек КИП */
  KIP_Data.KIP_Setup.KIP_Version = 1;
  KIP_Data.KIP_Setup.KIP_Type = 2;
  KIP_Data.KIP_Setup.SKZ_Address_RS485 = 1;
  KIP_Data.KIP_Setup.KIP_Address_RS485 = 1;
  KIP_Data.KIP_Setup.Shunt_Val = 50;

	// установим адрес хоста (не используем dhcp) KIP_Data
	Ip_Addr_TypeDef Ip_Addr_1 =  ipaddr_device;		KIP_Data.KIP_Setup.ip_addr_KIP = Ip_Addr_1;
	Ip_Addr_TypeDef Ip_Addr_2 =  ipaddr_dest;		KIP_Data.KIP_Setup.ip_addr_dect = Ip_Addr_2;
	Ip_Addr_TypeDef Ip_Addr_3 =  ipaddr_mask;		KIP_Data.KIP_Setup.ip_addr_mask = Ip_Addr_3;

  KIP_Data.KIP_Setup.ip_port = 502;
//  KIP_Data.KIP_Setup.Modules_Config.Modules_Set = Set_modules;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_ADC = 1;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_Bluetooth = 1;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_EEProm = 0;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_Mercury = 1;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_SIM = 1;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_http = 1;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_RTU = 0;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_TCP = 1;
	  KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_BU = 1;

  KIP_Data.Mercury_Data.Mercury_ID = 33736111;





  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C3|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_TIM1|RCC_PERIPHCLK_TIM16
                              |RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_TIM2
                              |RCC_PERIPHCLK_TIM34;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV6;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  PeriphClkInit.Tim16ClockSelection = RCC_TIM16CLK_HCLK;
  PeriphClkInit.Tim2ClockSelection = RCC_TIM2CLK_HCLK;
  PeriphClkInit.Tim34ClockSelection = RCC_TIM34CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{

#define tV_25   1.43f      // Напряжение (в вольтах) на датчике при температуре 25 °C.
#define tSlope  0.0043f    // Изменение напряжения (в вольтах) при изменении температуры на градус.
#define Vref    3.3f       // Образцовое напряжение АЦП (в вольтах).

//volatile uint16_t Power_V[2] = {0,0};	// 0 - значение АЦП при 0 В, 1 - амплитуда

	if(hadc->Instance == ADC1) //check if the interrupt comes from ACD1
	{
		if(KIP_Data.KIP_V_Power.ADC_Count > (sizeof(KIP_Data.KIP_V_Power.ADC_Val)/sizeof(uint16_t)))
		{
			KIP_Data.KIP_V_Power.ADC_Count = 0;
			HAL_TIM_Base_Stop_IT(&htim4);
		}

	KIP_Data.KIP_V_Power.ADC_Val[KIP_Data.KIP_V_Power.ADC_Count] = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);

	float tV = (float)HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2)/4096*Vref;		// Напряжение в вольтах на датчике
	KIP_Data.T_stm = ((tV_25 - tV)/tSlope) + 25;	// Температура в градусах.
	KIP_Data.KIP_V_Power.ADC_Count ++;
	}
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

//extern void net_poll(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin== GPIO_PIN_5){
//	  HAL_Delay(10);
//	  __disable_irq ();
//    net_poll();
//    __enable_irq ();
////	HAL_GPIO_TogglePin(LD7_GPIO_Port, LD7_Pin);
//	  int_Eth = 1;
  } else{
    __NOP();
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
		if (Mercury_Buf_Tx[4] == Read_V_I_P)	HAL_UART_Receive_IT(&Mercury_uart, Mercury_Buf_Rx, 14);
		if (Mercury_Buf_Tx[4] == Read_P_sum)	HAL_UART_Receive_IT(&Mercury_uart, Mercury_Buf_Rx, 23);
	}

}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
		if (Mercury_Buf_Rx[4] == Read_V_I_P)	Mercury_Read(Read_V_I_P);
		if (Mercury_Buf_Rx[4] == Read_P_sum)	Mercury_Read(Read_P_sum);
	}
	if(huart->Instance == USART1)
	{
		Sim80x_RxCallBack();
	}
}



/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if(htim->Instance == TIM2) {
//	    __NOP();
	  delay_arp = 1;
  }

  // Задержки для АЦП
  if(htim->Instance == _htim_ADS.Instance)
  {
	delay_start = 0;
  }

  if (htim->Instance == TIM3) {
	  TimingDelay_Decrement();

  }
  if (htim->Instance == TIM4) {
	    HAL_ADCEx_InjectedStart_IT(&hadc1);
  }
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
