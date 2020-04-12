/**
 ******************************************************************************
 * File Name          : ADC_Izm.h
 * Description        : 
 ******************************************************************************
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_Izm_H
#define __ADC_Izm_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"

#include <stdbool.h>

#include "ADS1115.h"

//#include "arm_math.h"
//#include "arm_const_structs.h"


#define 	ADC_Buf_cnt_max			(64)





/*------------------------------------------------------------------*/


/* Ключ тока поляризации  */
// Определение адресов и каналов АЦП
#define Sel_ADC_I_skz			(ADS1115_Address)Addr_ADC_1, (ADS1115_MUX)A0_A1, (ADS1115_PGA)PGA_1024, (ADS1115_Switch)SW_all_Off
#define Sel_ADC_V_skz 			(ADS1115_Address)Addr_ADC_1, (ADS1115_MUX)A2_A3, (ADS1115_PGA)PGA_1024, (ADS1115_Switch)SW_all_Off
#define Sel_ADC_I_pol			(ADS1115_Address)Addr_ADC_0, (ADS1115_MUX)A0_A1, (ADS1115_PGA)PGA_1024, (ADS1115_Switch)SW_I_pol
#define Sel_ADC_V_pol 			(ADS1115_Address)Addr_ADC_0, (ADS1115_MUX)A2_A3, (ADS1115_PGA)PGA_1024, (ADS1115_Switch)SW_V_pol
#define Sel_ADC_V_sum 			(ADS1115_Address)Addr_ADC_0, (ADS1115_MUX)A2_A3, (ADS1115_PGA)PGA_0256, (ADS1115_Switch)SW_V_sum
#define Sel_ADC_V_sum_fft 		(ADS1115_Address)Addr_ADC_0, (ADS1115_MUX)A2_A3, (ADS1115_PGA)PGA_2048, (ADS1115_Switch)SW_V_sum

typedef enum {
	ADC_Channel_0 = 0,						// Напряжение СКЗ
	ADC_Channel_1 = 1,						// Ток СКЗ
	ADC_Channel_2 = 2,						// Напряжение суммарное
	ADC_Channel_3 = 3,						// Напряжение поляризационное
	ADC_Channel_4 = 4,						// Ток поляризации
//	ADC_Channel_5 = 5,
//	ADC_Channel_6 = 6,
//	ADC_Channel_7 = 7
} ADC_Channel_TypeDef;

typedef struct {
	ADC_Channel_TypeDef	ADC_Channel;
	bool		DataFloatIsValid;				// Актуальность результата
	bool		DataIntIsValid;					// Актуальность результата
	uint16_t	i16_ADC_Buf_Cnt;				// количество оцифровок
	int16_t		i16_ADC_Buf[ADC_Buf_cnt_max];	// Буфер для сырых результатов измерений
	float		fl_ADC_Val;						// Среднее по больнице
} ADC_Data_TypeDef;

ADS1115_StatusTypeDef ADC_izmerenie(ADS1115_Address ADS_Addr, ADS1115_MUX ADS_Channel, ADS1115_PGA ADS_PGA, \
		ADS1115_Switch SW_Rele, uint16_t Count);	// Измерения, усреднение данных по буферу

#define ADC_Izmerenie_V_skz(_x) 		ADC_izmerenie(Sel_ADC_V_skz, _x)
#define ADC_Izmerenie_I_skz(_x) 		ADC_izmerenie(Sel_ADC_I_skz, _x)
#define ADC_Izmerenie_V_sum(_x) 		ADC_izmerenie(Sel_ADC_V_sum, _x)
#define ADC_Izmerenie_V_pol(_x) 		ADC_izmerenie(Sel_ADC_V_pol, _x)
#define ADC_Izmerenie_I_pol(_x) 		ADC_izmerenie(Sel_ADC_I_pol, _x)


void adc_izm(void);


#ifdef __cplusplus
}
#endif
#endif /*__ADC_Izm_H */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
