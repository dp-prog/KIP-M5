#ifndef _MERCURY_H_
#define _MERCURY_H_
//------------------------------------------------
#include "main.h"

#define	Mercury_uart		huart3

typedef enum{
	Read_V_I_P = 0x63,
	Read_P_sum = 0x27
}Mercury_Command;

unsigned char Mercury_Buf_Rx[32];		// Буфер данных
unsigned char Mercury_Buf_Tx[32];		// Буфер данных

volatile uint8_t Mercury_Page;
//
void Mercury_Write(Mercury_Command Command);
void Mercury_Read(Mercury_Command Command);

//------------------------------------------------
#endif /* _MERCURY_H_ */
