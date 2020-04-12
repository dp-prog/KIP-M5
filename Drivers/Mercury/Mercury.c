
#include "main.h"

#include "config.h"
#include "stm32f3xx_hal_uart.h"

#include "Mercury.h"

extern	UART_HandleTypeDef	Mercury_uart;		// Используемый интерфейс

extern KIP_Data_TypeDef KIP_Data;

// Расчет КС modbus
uint16_t crc_mb_calc(unsigned char* data, uint8_t length);


//------------------------------------------------
uint16_t crc_mb_calc(uint8_t* data, uint8_t length)
{
	uint16_t reg_crc = 0xFFFF;			// создаем регистр, в котором будем сохранять высчитанный CRC
	// Указываем полином, он может быть как 0xA001(старший бит справа), так и его реверс 0x8005(старший бит слева, здесь не рассматривается), при сдвиге вправо используется 0xA001
	#define Polynom		0xA001
	while(length--) 					// для каждого байта в принятом\отправляемом сообщении проводим следующие операции(байты сообщения без принятого CRC)
	{
		reg_crc ^= *data++;				// Делим через XOR регистр на выбранный байт сообщения(от младшего к старшему)
		for(uint8_t j=0; j<8; j++)		// для каждого бита в выбранном байте делим полученный регистр на полином
		{
			if(reg_crc & 0x01) reg_crc = (reg_crc>>1) ^ Polynom;	// сдвигаем на один бит вправо, делим регистр на полином по XOR
			else {reg_crc = reg_crc>>1;}							// сдвигаем регистр вправо
		}
	}
	return reg_crc;
}

//------------------------------------------------
void Mercury_Write(Mercury_Command Command)
{
	extern KIP_Data_TypeDef KIP_Data;
//	Mercury_Page = 0;
	uint16_t crc;
	Mercury_Buf_Tx[0] = (KIP_Data.Mercury_Data.Mercury_ID>>24)&0xFF;
	Mercury_Buf_Tx[1] = (KIP_Data.Mercury_Data.Mercury_ID>>16)&0xFF;
	Mercury_Buf_Tx[2] = (KIP_Data.Mercury_Data.Mercury_ID>>8)&0xFF;
	Mercury_Buf_Tx[3] = (KIP_Data.Mercury_Data.Mercury_ID)&0xFF;
	Mercury_Buf_Tx[4] = (uint8_t)Command;
	crc = crc_mb_calc(Mercury_Buf_Tx, 5);
	Mercury_Buf_Tx[5] = (crc)&0xFF;
	Mercury_Buf_Tx[6] = (crc>>8)&0xFF;
	HAL_UART_Transmit_IT(&Mercury_uart, Mercury_Buf_Tx, 7);
	for(uint8_t i = 0; i < sizeof(Mercury_Buf_Rx); i++) Mercury_Buf_Rx[i] = 0;
}
//------------------------------------------------
void Mercury_Read(Mercury_Command Command)
{
	uint16_t crc;
	uint16_t Mercury_Fail;
	if(strncmp(Mercury_Buf_Rx, Mercury_Buf_Tx, 4) == 0)
	{
		if (Command == Read_V_I_P)
		{
			crc = crc_mb_calc(Mercury_Buf_Rx, 12);
			if((Mercury_Buf_Rx[12] == ((crc)&0xFF)) &  (Mercury_Buf_Rx[13] == ((crc>>8)&0xFF)))
			{
				uint32_t V = (Mercury_Buf_Rx[5]/16)&0x0F;
				V = V*10 + ((Mercury_Buf_Rx[5])&0x0F);
				V = V*10 + ((Mercury_Buf_Rx[6]/16)&0x0F);
				V = V*10 + ((Mercury_Buf_Rx[6])&0x0F);

				KIP_Data.Mercury_Data.Mercury_V = ((float) V)/10;

				uint32_t P = (Mercury_Buf_Rx[9]/16)&0x0F;
				P = P*10 + ((Mercury_Buf_Rx[9])&0x0F);
				P = P*10 + ((Mercury_Buf_Rx[10]/16)&0x0F);
				P = P*10 + ((Mercury_Buf_Rx[10])&0x0F);
				P = P*10 + ((Mercury_Buf_Rx[11]/16)&0x0F);
				P = P*10 + ((Mercury_Buf_Rx[11])&0x0F);

				KIP_Data.Mercury_Data.Mercury_P = P;
				KIP_Data.Mercury_Data.Mercury_Count = 0;
				KIP_Data.Mercury_Data.Mercury_Have = 1;
				Mercury_Page = 2;
			}
			else
			{
				KIP_Data.Mercury_Data.Mercury_Count ++;
				if(KIP_Data.Mercury_Data.Mercury_Count > 10)
				{
					KIP_Data.Mercury_Data.Mercury_Count = 0;
					KIP_Data.Mercury_Data.Mercury_Have = 0;
					KIP_Data.Mercury_Data.Mercury_V = 0;
					KIP_Data.Mercury_Data.Mercury_P = 0;
					KIP_Data.Mercury_Data.Mercury_P_sum = 0;
				}
			}
		}
		if (Command == Read_P_sum)
		{
			crc = crc_mb_calc(Mercury_Buf_Rx, 21);
			if((Mercury_Buf_Rx[21] == ((crc)&0xFF)) &  (Mercury_Buf_Rx[22] == ((crc>>8)&0xFF)))
			{
				uint32_t P_sum = (Mercury_Buf_Rx[5]/16)&0x0F;
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[5])&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[6]/16)&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[6])&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[7]/16)&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[7])&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[8]/16)&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[8])&0x0F);

				KIP_Data.Mercury_Data.Mercury_P_sum_1 = ((float) P_sum)/100;

				P_sum = (Mercury_Buf_Rx[9]/16)&0x0F;
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[9])&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[10]/16)&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[10])&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[11]/16)&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[11])&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[12]/16)&0x0F);
				P_sum = P_sum*10 + ((Mercury_Buf_Rx[12])&0x0F);

				KIP_Data.Mercury_Data.Mercury_P_sum_2 = ((float) P_sum)/100;

				KIP_Data.Mercury_Data.Mercury_P_sum = KIP_Data.Mercury_Data.Mercury_P_sum_1 + \
						KIP_Data.Mercury_Data.Mercury_P_sum_2;
				KIP_Data.Mercury_Data.Mercury_Have = 1;
				Mercury_Page = 1;
			}
			else
			{
				KIP_Data.Mercury_Data.Mercury_Count ++;
				if(KIP_Data.Mercury_Data.Mercury_Count > 10)
				{
					KIP_Data.Mercury_Data.Mercury_Count = 0;
					KIP_Data.Mercury_Data.Mercury_Have = 0;
					KIP_Data.Mercury_Data.Mercury_V = 0;
					KIP_Data.Mercury_Data.Mercury_P = 0;
					KIP_Data.Mercury_Data.Mercury_P_sum = 0;
				}
			}
		}
	}
	else
	{
		KIP_Data.Mercury_Data.Mercury_Count ++;
		if(KIP_Data.Mercury_Data.Mercury_Count > 10)
		{
			KIP_Data.Mercury_Data.Mercury_Count = 0;
			KIP_Data.Mercury_Data.Mercury_Have = 0;
			KIP_Data.Mercury_Data.Mercury_V = 0;
			KIP_Data.Mercury_Data.Mercury_P = 0;
			KIP_Data.Mercury_Data.Mercury_P_sum = 0;
		}
	}
}
