/**
 ******************************************************************************
 * File Name          : ADS1115.c
 * Description        : 
 ******************************************************************************
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "gpio.h"


#include "ADC_Izm.h"
extern ADC_Data_TypeDef ADC_Data;

#include "ADS1115.h"

#if (_ADC_USE_FREERTOS==1)
#include "cmsis_os.h"
#define	ADS_Delay(x)			osDelay(x)
#else
#define	ADS_Delay(x)			HAL_Delay(x)
#endif

/*------------------------------------------------------------------*/



/*------------------------------------------------------------------*/
int16_t ADS_Value_temp;						// Времянка для хранения результатов АЦП
volatile uint8_t delay_start;				// формирование паузы между замерами

/*------------------------------------------------------------------*/
// Прототипы функций




/*------------------------------------------------------------------*/
// Вкл/вык питания микросхемы АЦП
void ADS1115_Set_Power(ADS1115_Address ADS_Addr, ADS1115_On_State ADS_On)
{
	_ADS_1_on();	// ToDo для тестов отключил управление питанием АЦП
	_ADS_2_on();
}

void ADS1115_SW_Rele(ADS1115_Switch	SW_Rele)
{	// Если идет расчет поляризационного потенциала, то размыкаю нужные ключи

	/* Ключ тока поляризации  */
//	typedef enum {
//		SW_I_pol	= 0,
//		SW_V_pol	= 1,
//		SW_V_sum	= 2,
//		SW_all_Off	= 3
//	} ADS1115_Switch;

	switch (SW_Rele) {
	case 0:
		_V_EC_Off;
		_V_Tr_On;
		ADS_Delay(3);		// Задержка
		break;
	case 1:
		_V_EC_On;
		ADS_Delay(3);		// Задержка тут!
		_V_Tr_Off;
		break;
	case 2:
		_V_EC_On;
		_V_Tr_On;
		HAL_Delay(3);		// Задержка
		break;
	case 3:
		_V_EC_Off;
		_V_Tr_On;
		break;
	}
}

ADS1115_StatusTypeDef Init_ADS1115(ADS1115_Address ADS_Addr, ADS1115_MUX ADS_Channel, ADS1115_PGA ADS_PGA)
{
	uint16_t ADS1115_Config = COMP_QUE_Disable | COMP_LAT_Disable | COMP_POL | COMP_MODE \
			| (dr_860 << 5) | MODE | (ADS_PGA << 9) | (ADS_Channel << 12) | Op_status;

	uint8_t i2c_buf_wr[3] = { Config_Reg_Select, ADS1115_Config / 0x100, ADS1115_Config % 0x100 };

	if (HAL_I2C_Master_Transmit(&_hi2c_ADS, (0x48 + ADS_Addr) << 1, (uint8_t*) &i2c_buf_wr, 3, 6) != HAL_OK)
		return ADS_ERROR;
	ADS_Delay(1);
	return ADS_OK;
}

ADS1115_StatusTypeDef Read_ADS1115(ADS1115_Address ADS_Addr) {
	uint8_t i2c_buf_wr = Conversion_Reg_Select;
	uint8_t i2c_buf_rd[2] = { 0, 0 };

	// здесь нужна задержка для измерения 1 раз за 1,2 мсек
	delay_start = 1;
	while (delay_start) {
		;
	};
	if (HAL_I2C_Master_Transmit(&_hi2c_ADS,  (0x48 + ADS_Addr) << 1, (uint8_t*) &i2c_buf_wr, 1,
			5) != HAL_OK) {
		return ADS_ERROR;
	}
	if (HAL_I2C_Master_Receive(&_hi2c_ADS,  (0x48 + ADS_Addr) << 1, (uint8_t*) &i2c_buf_rd, 2,
			5) != HAL_OK) {
		return ADS_ERROR;
	}
	ADS_Value_temp = (int16_t) ((i2c_buf_rd[0] << 8) | i2c_buf_rd[1]);
	return ADS_OK;
}

ADS1115_StatusTypeDef Read_ADS1115_to_Buf(ADS1115_Address ADS_Addr, ADS1115_MUX ADS_Channel, \
		ADS1115_PGA ADS_PGA, ADS1115_Switch SW_Rele, uint16_t Count) {
//	uint16_t ADC_Buf_cnt = Count;						// Установил количество замеров
	ADS1115_Set_Power(ADS_Addr, ADS_On);				// Включил питание
	ADS_Delay(100);										// Задержка для стабилизации напряжения питания

	HAL_TIM_Base_Start_IT(&_htim_ADS);					// Включил таймер периода опроса АЦП

//	float V_op = ((float)(8192 >> ADS_PGA)) / 1000.0f;	// Расчет опорного напряжения

	// Подготовка буфера
	ADC_Data.DataFloatIsValid = false;
	ADC_Data.DataIntIsValid = false;
	ADC_Data.i16_ADC_Buf_Cnt = Count;

	if (Init_ADS1115(ADS_Addr, ADS_Channel, ADS_PGA) != ADS_OK) {
		return ADS_ERROR;
	}

	// Коммутирую реле
	ADS1115_SW_Rele(SW_Rele);

	for (uint16_t i = 0; i < Count; i++) {
		if (Read_ADS1115(ADS_Addr) != ADS_OK) {
			return ADS_ERROR;
		}
		ADC_Data.i16_ADC_Buf[i] = ADS_Value_temp;
	}
	ADC_Data.DataIntIsValid = true;

	HAL_TIM_Base_Stop_IT(&_htim_ADS);
	ADS1115_Set_Power(ADS_Addr, ADS_Off);				// Выключил питание
	return ADS_OK;
}


/*****************************END OF FILE****/
