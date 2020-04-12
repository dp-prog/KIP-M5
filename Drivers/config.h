/**
 ******************************************************************************
 * File Name          : config.h
 * Description        : 
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H
#define __CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

// �������������� ���������

#define Set_KIP_Version			0x0000000031302e31		/* ������ �� */
#define Set_KIP_Address_RS485	(1)			/* ����� �� modbus */

#define Set_m_EEProm			(1)			/*  */
#define Set_m_ADC				(1)			/*  */
#define Set_m_RS485				(1)			/*  */
#define Set_m_USB				(1)			/*  */
#define Set_m_LAN				(0)			/*  */
#define Set_m_Bluetooth			(0)			/*  */
#define Set_m_http				(0)			/*  */
#define Set_m_SIM				(0)			/*  */

#define Set_modules		((Set_m_EEProm <<0)|(Set_m_ADC <<1)|(Set_m_CAN <<2)|(Set_m_RS485 <<3)| \
						(Set_m_USIK <<4)|(Set_m_LCD <<5)|(Set_m_LAN <<6)|(Set_m_Lora <<7)| \
						(Set_m_Bluetooth <<8)|(Set_m_Wifi <<9)|(Set_m_http <<10)|(Set_m_SIM <<11)| \
						(0 <<12)|(0 <<13)|(0 <<14)|(0 <<15) )

#define ipaddr_device		{{192,168,1,100}}
#define ipaddr_dest			{{192,168,1,1}}
#define ipaddr_mask			{{255,255,255,0}}

typedef union {
	struct
	{
		uint64_t m_Config;
	} Modules_Set;
	struct
	{
		unsigned m_EEProm :1;
		unsigned m_ADC :1;
		unsigned m_mbus_BU :1;
		unsigned m_mbus_RTU :1;
		unsigned m_mbus_TCP :1;
		unsigned m_http :1;
		unsigned m_Mercury :1;
		unsigned m_Bluetooth :1;
		unsigned m_SIM :1;
		;
		;
	} Module_Set;
} Modules_Config_TypeDef;

#pragma pack(push, 8)					// ��� �������� ������ � ������ ������� �� 64 ����
#define T_points	(5)
#define V_points	(5)
#define ADC_channels_count	(5)

typedef struct {
	float	K;							/* ����������� ������� �������������� ��� */
	float	V[V_points];				/* ������� ����� ���������� �� �������� ���������� */
	float	V_T[T_points][V_points];	/* �������� ��� � ������ ���������� */
} Channel_Calibr_Table_TypeDef;


typedef struct {
  uint8_t addr[4];
} Ip_Addr_TypeDef;


typedef struct {
	uint8_t		Mercury_Have;			// ������� ��������
	uint8_t		Mercury_Count;			// ������� ������� ��������
	uint32_t	Mercury_ID;				// ��� �����
	float		Mercury_V;				// ����������
	uint32_t	Mercury_P;				// ��������
	uint32_t	Mercury_P_sum;			// �������� �����������
	uint32_t	Mercury_P_sum_1;		// �������� ����������� �� 1 ������
	uint32_t	Mercury_P_sum_2;		// �������� ����������� �� 2 ������
} Mercury_Data_TypeDef;


typedef struct
{
	uint32_t	KIP_Version;			/* ������ �� ��� */
	uint32_t	KIP_Type;				/* ��� ����������� ��� */

	uint8_t		KIP_Address_RS485;		/* ����� ��� */
	uint8_t		SKZ_Address_RS485;		/* ����� �� */

	Ip_Addr_TypeDef	ip_addr_KIP;
	Ip_Addr_TypeDef	ip_addr_dect;
	Ip_Addr_TypeDef	ip_addr_mask;
	uint16_t 		ip_port;

	Modules_Config_TypeDef Modules_Config;

	float		T[T_points];			/* ������� ����� ���������� �� ����������� */

	Channel_Calibr_Table_TypeDef		Channel_Calibr_Table[ADC_channels_count];	/* �������������� ������� ��������� */

	float		Shunt_Val;				/* ��� ����� */
} KIP_Settings;

#pragma pack(pop)


typedef struct {
	float		V_power;
	uint16_t	ADC_Val[100];
	uint16_t	ADC_Count;
} KIP_V_Power;

typedef struct {
	float		I_skz;
	float		V_skz;
	float		I_pol;
	float		V_pol;
	float		V_sum;
} KIP_ADC_TypeDef;

typedef struct {
	KIP_Settings	KIP_Setup;

	uint8_t		MB_RTU_Connected;
	uint8_t		MB_TCP_Connected;

	union
	{
		uint16_t	u16;				//
		struct
		{
			unsigned Open_Case_bit:	1;
		};
	} Flags;

	/* ����������� ��� ����������� ��� */
	float		T_stm;					//

	float		V_out;					//	���������� ���������� 0..10 �

	/* ��� */
	KIP_ADC_TypeDef		KIP_ADC;

	KIP_V_Power			KIP_V_Power;

	Mercury_Data_TypeDef Mercury_Data;

} KIP_Data_TypeDef;



#ifdef __cplusplus
}
#endif
#endif /*__CONFIG_H */

