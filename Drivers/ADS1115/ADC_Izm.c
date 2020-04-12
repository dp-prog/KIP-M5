/**
 ******************************************************************************
 * File Name          : ADC_Izm.c
 * Description        : 
 ******************************************************************************
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "config.h"

KIP_Data_TypeDef	KIP_Data;
KIP_ADC_TypeDef		KIP_ADC_Temp;

#include "ADC_Izm.h"
//#include "arm_math.h"

//#include "dipol.h"

//extern KIP_Settings KIP_Setup_EEPROM;

#define R_I_pol		1.5f
#define k_I_pol		(200000.0f/15000.0f)

#define R_I_skz		(0.075f/100.0f)
#define k_I_skz		(200000.0f/15000.0f)


ADC_Data_TypeDef ADC_Data;
// Коэффициенты для каналов АЦП  - fl_k_ADC[ADS_Addr][ADS_Channel]
		/*		I										V																*/
float fl_k_ADC[4][8] = {\
		{(1000.0/(R_I_pol*k_I_pol)),	1.0f,	1.0f,	-100.0f,	1.0f,	1.0f,	1.0f,	1.0f},		/* Труба, электроды		*/
		{75.0f,							1.0f,	1.0f,	-100.0f,	1.0f,	1.0f,	1.0f,	1.0f},		/* Станция				*/
		{100.0f,						1.0f,	1.0f,	100.0f,		1.0f,	1.0f,	1.0f,	1.0f},		/* Нет микросхемы		*/
		{100.0f,						1.0f,	1.0f,	100.0f,		1.0f,	1.0f,	1.0f,	1.0f}		/* Нет микросхемы		*/
		};


//void fft_izm(void);




ADS1115_StatusTypeDef ADC_izmerenie(ADS1115_Address ADS_Addr, ADS1115_MUX ADS_Channel, ADS1115_PGA ADS_PGA, \
		ADS1115_Switch SW_Rele, uint16_t Count)
{
	ADC_Data.DataFloatIsValid = 0;												// Актуальность результата
	if(Read_ADS1115_to_Buf(ADS_Addr, ADS_Channel, ADS_PGA, SW_Rele, Count) != ADS_OK)
	{
		return ADS_ERROR;
	}
	// Заполнение буфера float оцифрованными значениями напряжения на входе канала АЦП
	int32_t data_adc_min = ADC_Data.i16_ADC_Buf[0];
	int32_t data_adc_max = ADC_Data.i16_ADC_Buf[0];
	int32_t data_adc_sum = ADC_Data.i16_ADC_Buf[0];

	for (uint16_t i = 1; i < Count; i++) {
		if (data_adc_max < ADC_Data.i16_ADC_Buf[i]) {
			data_adc_max = ADC_Data.i16_ADC_Buf[i];
		}
		if (data_adc_min > ADC_Data.i16_ADC_Buf[i]) {
			data_adc_min = ADC_Data.i16_ADC_Buf[i];
		}
		data_adc_sum += ADC_Data.i16_ADC_Buf[i];
	}
	float V_op = ((float)(8192 >> ADS_PGA)) / 1000.0f;							// Расчет опорного напряжения
	float Val_Out = (V_op * (data_adc_sum - data_adc_min - data_adc_max) /(Count - 2))/0x7FFF;		// значения в вольтах на входе АЦП

	ADC_Data.fl_ADC_Val = Val_Out * fl_k_ADC[ADS_Addr][ADS_Channel];			// значение на входе канала измерения
	ADC_Data.DataFloatIsValid = 1;												// Актуальность результата
	return ADS_OK;
}

void adc_izm(void)
{
	// Измерения по аналоговым каналам
	ADC_izmerenie(Sel_ADC_I_skz, 5);	KIP_ADC_Temp.I_skz = -ADC_Data.fl_ADC_Val;	// Коррекция влияния канала напряжения не требуется
	ADC_izmerenie(Sel_ADC_V_skz, 5);	KIP_ADC_Temp.V_skz = ADC_Data.fl_ADC_Val;	// Нужна коррекция по току станции

	ADC_izmerenie(Sel_ADC_I_pol, 5);	KIP_ADC_Temp.I_pol = -ADC_Data.fl_ADC_Val;	// Коррекция влияния канала напряжения не требуется
	ADC_izmerenie(Sel_ADC_V_sum, 5);	KIP_ADC_Temp.V_sum = -ADC_Data.fl_ADC_Val;	// Нужна коррекция по току натекания

	ADC_izmerenie(Sel_ADC_V_pol, 5);	KIP_ADC_Temp.V_pol = -ADC_Data.fl_ADC_Val;	// Коррекция влияния канала напряжения не требуется

	ADS1115_SW_Rele(SW_I_pol);														// Возврат реле в штатный режим работы

	// Коррекция взаимного влияния каналов друг на друга
//	KIP_Data_Temp.V_skz = KIP_Data_Temp.V_skz - ((200.0f + 15.0f)*(KIP_Data_Temp.I_skz * 200.0f/15.0f + KIP_Data_Temp.I_skz)/430.0f)/1000.0f;
//	KIP_Data_Temp.V_sum = KIP_Data_Temp.V_sum - ((200.0f + 15.0f)*(KIP_Data_Temp.I_pol * 200.0f/15.0f + KIP_Data_Temp.I_pol)/430.0f)/1000.0f;

	// Коррекция результатов по коэффициентам каналов
//	KIP_Data.I_skz = KIP_Data.KIP_Setup.Channel_Calibr_Table[0].K * KIP_Data_Temp.I_skz * KIP_Data.KIP_Setup.Shunt_Val/75.0f;
//	KIP_Data.V_skz = KIP_Data.KIP_Setup.Channel_Calibr_Table[1].K * KIP_Data_Temp.V_skz;
//	KIP_Data.I_pol = KIP_Data.KIP_Setup.Channel_Calibr_Table[2].K * KIP_Data_Temp.I_pol;
//	KIP_Data.V_sum = KIP_Data.KIP_Setup.Channel_Calibr_Table[3].K * KIP_Data_Temp.V_sum;
//	KIP_Data.V_pol = KIP_Data.KIP_Setup.Channel_Calibr_Table[4].K * KIP_Data_Temp.V_pol;

	KIP_Data.KIP_ADC.I_skz = KIP_ADC_Temp.I_skz * 50.0f/75.0f;
	KIP_Data.KIP_ADC.V_skz = KIP_ADC_Temp.V_skz;
	KIP_Data.KIP_ADC.I_pol = KIP_ADC_Temp.I_pol;
	KIP_Data.KIP_ADC.V_sum = KIP_ADC_Temp.V_sum;
	KIP_Data.KIP_ADC.V_pol = KIP_ADC_Temp.V_pol;

}



/*****************************END OF FILE****/
