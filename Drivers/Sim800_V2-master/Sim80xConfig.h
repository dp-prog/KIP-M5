#ifndef	_SIM80XCONF_H
#define	_SIM80XCONF_H


//	0: No DEBUG				1:High Level Debug .Use printf		2:All RX Data.Use printf

#define	_SIM80X_DEBUG				0

#define	_SIM80X_USART				huart1

#define	_SIM80X_USE_POWER_KEY		1

#define	_SIM80X_BUFFER_SIZE			512

#define _SIM80X_DMA_TRANSMIT		0

#define _SIM80X_USE_BLUETOOTH		0
#define _SIM80X_USE_GPRS			1

#define SIM_Rst_Pin					GPIO_PIN_5
#define SIM_Rst_GPIO_Port			GPIOB

#define	_SIM80X_POWER_KEY_GPIO		SIM_Rst_GPIO_Port
#define	_SIM80X_POWER_KEY_PIN		SIM_Rst_Pin

//#define	_SIM80X_POWER_KEY_GPIO		GSM_POWER_KEY_GPIO_Port
//#define	_SIM80X_POWER_KEY_PIN		GSM_POWER_KEY_Pin

#endif
