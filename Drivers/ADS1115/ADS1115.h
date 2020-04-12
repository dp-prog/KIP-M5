/**
 ******************************************************************************
 * File Name          : ADS1115.h
 * Description        : 
 ******************************************************************************
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADS1115_H
#define __ADS1115_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include <stdbool.h>

/* Добавить в void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  // Задержки для АЦП
  if(htim->Instance == TIM6)
  {
	delay_start = 0;
  }

также надо настроить таймер для измерения мкс и включить прерывания для таймера

 */
// Настройки подключения к проекту
#define	_ADC_USE_FREERTOS		    1

#define	_hi2c_ADS				hi2c1
extern	I2C_HandleTypeDef 	_hi2c_ADS;		// Используемый интерфейс

#define	_htim_ADS				htim6
extern	TIM_HandleTypeDef 	_htim_ADS;


#define	_ADS_1_off()	HAL_GPIO_WritePin(ADC1_Pwr_GPIO_Port, ADC1_Pwr_Pin, GPIO_PIN_SET);
#define	_ADS_1_on()		HAL_GPIO_WritePin(ADC1_Pwr_GPIO_Port, ADC1_Pwr_Pin, GPIO_PIN_RESET);
#define	_ADS_2_off()	HAL_GPIO_WritePin(ADC2_Pwr_GPIO_Port, ADC2_Pwr_Pin, GPIO_PIN_SET);
#define	_ADS_2_on()		HAL_GPIO_WritePin(ADC2_Pwr_GPIO_Port, ADC2_Pwr_Pin, GPIO_PIN_RESET);

//
#define	_V_EC_On		HAL_GPIO_WritePin(V_EC_On_GPIO_Port, V_EC_On_Pin, GPIO_PIN_SET);
#define	_V_EC_Off		HAL_GPIO_WritePin(V_EC_On_GPIO_Port, V_EC_On_Pin, GPIO_PIN_RESET);
#define	_V_Tr_On		HAL_GPIO_WritePin(V_Tr_Off_GPIO_Port, V_Tr_Off_Pin, GPIO_PIN_RESET);
#define	_V_Tr_Off		HAL_GPIO_WritePin(V_Tr_Off_GPIO_Port, V_Tr_Off_Pin, GPIO_PIN_SET);


/*------------------------------------------------------------------*/
typedef enum
{
	ADS_Off		= 0x00,
	ADS_On		= 0x01
}ADS1115_On_State;

typedef enum {
	ADS_OK		= 0x00,
	ADS_ERROR	= 0x01
} ADS1115_StatusTypeDef;

//#define	ADS_OK		0x00
//#define	ADS_ERROR	0x01

/* Адреса используемых АЦП имеют смещение (0x48 << 1) */
typedef enum {
	Addr_ADC_0	= 0,
	Addr_ADC_1	= 1,
	Addr_ADC_2	= 2,
	Addr_ADC_3	= 3
} ADS1115_Address;

/* Регистры в АЦП */
//typedef enum {
//	Conversion_Reg_Select	= 0,
//	Config_Reg_Select		= 1,
//	Lo_thresh_Reg_Select	= 2,
//	Hi_thresh_Reg_Select	= 3
//} ADS1115_Regs;

#define	Conversion_Reg_Select	0
#define	Config_Reg_Select		1
#define	Lo_thresh_Reg_Select	2
#define	Hi_thresh_Reg_Select	3

/* Данные регистров в АЦП */
typedef struct {
	uint16_t Conversion_Reg;	//
	uint16_t Config_Reg;		//
	uint16_t Lo_thresh_Reg;		// Регистр с порогом компаратора
	uint16_t Hi_thresh_Reg;		// Регистр с порогом компаратора
} ADS1115_regs_TypeDef;

/* ADS1115_Data_rates */
//typedef enum {
//	dr_8 = 0,
//	dr_16 = 1,
//	dr_32 = 2,
//	dr_64 = 3,
//	dr_128 = 4,
//	dr_250 = 5,
//	dr_475 = 6,
//	dr_860 = 7
//} ADS1115_Data_rates;

#define	dr_8	0
#define	dr_16	1
#define	dr_32	2
#define	dr_64	3
#define	dr_128	4
#define	dr_250	5
#define	dr_475	6
#define	dr_860	7

/* ADS1115_PGA */
typedef enum {
	PGA_6144 = 0,
	PGA_4096 = 1,
	PGA_2048 = 2,
	PGA_1024 = 3,
	PGA_0512 = 4,
	PGA_0256 = 5
} ADS1115_PGA;

/* ADS1115_MUX */
typedef enum {
	A0_A1 = 0,
	A0_A3 = 1,
	A1_A3 = 2,
	A2_A3 = 3,
	A0_GND = 4,
	A1_GND = 5,
	A2_GND = 6,
	A3_GND = 7
} ADS1115_MUX;

/* Ключ тока поляризации  */
typedef enum {
	SW_I_pol	= 0,		// От
	SW_V_pol	= 1,
	SW_V_sum	= 2,
	SW_all_Off	= 3
} ADS1115_Switch;
//#define	SW_I_pol	0
//#define	SW_V_pol	1
//#define	SW_V_sum	2
//#define	SW_all_Off	3


/* ADS1115_Config_reg */
#define	COMP_QUE_Enable		0				/* 0 - каждое измерение заканчивается прерыванием */
#define	COMP_QUE_Disable	3				/* 3 - отключаем компараторы с выводом на ALERT-RDY */
#define	COMP_LAT_Enable		1 << 2			/* прерывания включить  */
#define	COMP_LAT_Disable	0 << 2			/* непрерывный компаратор */
#define	COMP_POL			0 << 3 			/* (не использую) полярность компаратора */
#define	COMP_MODE			0 << 4 			/* (не использую) 0 - Traditional comparator (default), 1 - Window comparator */
#define	DataRate			dr_860 << 5 	/* */
#define	MODE				0 << 8 			/* 0 - Continuous-conversion mode, 1 - Single-shot mode or power-down state (default) */
#define	PGA4096				PGA_4096 << 9 	/* */
#define	PGA2048				PGA_2048 << 9 	/* */
#define	PGA1024				PGA_1024 << 9 	/* */
#define	PGA0512				PGA_0512 << 9 	/* */
#define	PGA0256				PGA_0256 << 9 	/* */
#define	MUX_0				A0_A1 << 12 	/* */
#define	MUX_1				A2_A3 << 12 	/* */
#define	Op_status			1 << 15 		/* */


void ADS1115_Set_Power(ADS1115_Address ADS_Addr, ADS1115_On_State ADS_On);
void ADS1115_SW_Rele(ADS1115_Switch	SW_Rele);

ADS1115_StatusTypeDef Init_ADS1115(ADS1115_Address ADS_Addr, ADS1115_MUX ADS_Channel,	\
		ADS1115_PGA ADS_PGA);	// инициализация

ADS1115_StatusTypeDef Read_ADS1115_to_Buf(ADS1115_Address ADS_Addr, ADS1115_MUX ADS_Channel,	\
		ADS1115_PGA ADS_PGA, ADS1115_Switch SW_Rele, uint16_t Count);

ADS1115_StatusTypeDef Read_ADS1115(ADS1115_Address ADS_Addr);	// Чтение данных из выбранной ADS1115 0-3


#ifdef __cplusplus
}
#endif
#endif /*__ADS1115_H */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
