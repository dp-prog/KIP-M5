/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

extern IWDG_HandleTypeDef hiwdg;

#include "adc.h"

#include "enc28j60.h"

#include "config.h"
KIP_Data_TypeDef KIP_Data;
//extern KIP_Data_TypeDef KIP_Data;

#include "usbd_cdc_if.h"

#include "mb.h"
#include "mbutils.h"

#include "uip.h"
#include "uip_arp.h"
#include "apps-conf.h"

#include "ADC_Izm.h"

#include "Mercury.h"
extern volatile uint8_t Mercury_Page;



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Sim80x.h"

/*----------------------------------------------------------------------------*/
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/*----------------------------------------------------------------------------*/
//буфер, нужный для UIP
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
extern uint8_t	int_Eth;



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

extern TIM_HandleTypeDef	htim2;
extern TIM_HandleTypeDef	htim4;

extern UART_HandleTypeDef	huart4;		// bluetooth
#define BT_uart				&huart4

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

char FLT_buf[16];	// Буфер для преобразований плавающей точки
char * fl2str(float Value, uint8_t dec);

uint8_t delay_arp = 0;
uint8_t delay_arp_10sek = 0;

/* USER CODE END Variables */
osThreadId Task_DefHandle;
osThreadId Task_LedHandle;
osThreadId Task_uIPHandle;
osThreadId Task_ADCHandle;
osMutexId rtos_mutex_FSHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void Start_Default(void const * argument);
void Start_Led(void const * argument);
void Start_uIP(void const * argument);
void Start_ADC(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
void vprint(UART_HandleTypeDef *huart_out, const char *fmt, va_list argp)
{
	char string[512];
	if(0 < vsprintf(string,fmt,argp)) // build string
	{
		HAL_UART_Transmit(huart_out, (uint8_t*)string, strlen(string), 0x10); 	// отправка сообщений в Bloutooth
	}
}
void my_printf(UART_HandleTypeDef *huart_out, const char *fmt, ...) // custom printf() function
{
	va_list argp;
	va_start(argp, fmt);
	vprint(huart_out, fmt, argp);
	va_end(argp);
}

#define bt_printf( xxx )	my_printf(BT_uart, xxx )



/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

	// это будет наш МАС-адрес
	struct uip_eth_addr mac = MAC_ADDR;
	enc28j60_ini();

	// инициализация стека
	uip_init();
	uip_arp_init();

	// установим наш МАС
	uip_setethaddr(mac);

	uip_ipaddr_t ipaddr;
	uip_ipaddr(ipaddr, KIP_Data.KIP_Setup.ip_addr_KIP.addr[0], KIP_Data.KIP_Setup.ip_addr_KIP.addr[1],\
			KIP_Data.KIP_Setup.ip_addr_KIP.addr[2], KIP_Data.KIP_Setup.ip_addr_KIP.addr[3]);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, KIP_Data.KIP_Setup.ip_addr_dect.addr[0], KIP_Data.KIP_Setup.ip_addr_dect.addr[1],\
			KIP_Data.KIP_Setup.ip_addr_dect.addr[2], KIP_Data.KIP_Setup.ip_addr_dect.addr[3]);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, KIP_Data.KIP_Setup.ip_addr_mask.addr[0], KIP_Data.KIP_Setup.ip_addr_mask.addr[1],\
			KIP_Data.KIP_Setup.ip_addr_mask.addr[2], KIP_Data.KIP_Setup.ip_addr_mask.addr[3]);
	uip_setnetmask(ipaddr);


	if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_http == 1){
		httpd_init();
	}
	if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_TCP == 1){
		eMBTCPInit(502);
	    eMBEnable();
	}
	if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_RTU == 1){
		eMBInit( MB_RTU, 1, 2, 9600, MB_PAR_NONE );
	    eMBEnable();
	}


  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of rtos_mutex_FS */
  osMutexDef(rtos_mutex_FS);
  rtos_mutex_FSHandle = osMutexCreate(osMutex(rtos_mutex_FS));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Task_Def */
  osThreadDef(Task_Def, Start_Default, osPriorityNormal, 0, 1024);
  Task_DefHandle = osThreadCreate(osThread(Task_Def), NULL);

  /* definition and creation of Task_Led */
  osThreadDef(Task_Led, Start_Led, osPriorityLow, 0, 256);
  Task_LedHandle = osThreadCreate(osThread(Task_Led), NULL);

  /* definition and creation of Task_uIP */
  osThreadDef(Task_uIP, Start_uIP, osPriorityHigh, 0, 1024);
  Task_uIPHandle = osThreadCreate(osThread(Task_uIP), NULL);

  /* definition and creation of Task_ADC */
  osThreadDef(Task_ADC, Start_ADC, osPriorityAboveNormal, 0, 1024);
  Task_ADCHandle = osThreadCreate(osThread(Task_ADC), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_Default */
/**
  * @brief  Function implementing the Task_Def thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_Start_Default */
void Start_Default(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN Start_Default */

  Sim80x_Init(osPriorityLow);
  osDelay(10000);

//  Gsm_MsgSendText("+79172061888","KIP-M5 SMS testing - Ok\r\n");	// Sasha
//  Gsm_MsgSendText("+79173105206","KIP-M5 SMS testing - Ok\r\n");	// Мой тел
//  Gsm_MsgSendText("+79093331223","KIP-M5 SMS testing - Ok\r\n");	// Мират
//  GPRS_SetAPN("internet.tele2.ru","","");
  osDelay(1000);

//  GPRS_StartUpGPRS();
//  osDelay(5000);
//  char IP[100];
//  GPRS_GetLocalIP(IP);



  char txt_out[512];
  volatile uint16_t Len_text = 0;

  /* Infinite loop */
  for(;;)
  {

	  Len_text = sprintf(txt_out,  "%c-------------- KIP-M5 --------------%c%c", 0x0C,0x0D,0x0A);
	  if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_TCP){
		  Len_text += sprintf(txt_out, "%sРежим работы TCP       - %s\r\n", txt_out, "Modbus TCP");
	  }
	  if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_http){
		  Len_text += sprintf(txt_out, "%sРежим работы TCP       - %s%d.%d.%d.%d\r\n", txt_out, "Web http://",\
				  KIP_Data.KIP_Setup.ip_addr_KIP.addr[0], KIP_Data.KIP_Setup.ip_addr_KIP.addr[1], \
				  KIP_Data.KIP_Setup.ip_addr_KIP.addr[2], KIP_Data.KIP_Setup.ip_addr_KIP.addr[3]);
	  }
	  Len_text += sprintf(txt_out, "%sАдрес КИП              - %d.%d.%d.%d\r\n", txt_out, \
			  KIP_Data.KIP_Setup.ip_addr_KIP.addr[0], KIP_Data.KIP_Setup.ip_addr_KIP.addr[1], \
			  KIP_Data.KIP_Setup.ip_addr_KIP.addr[2], KIP_Data.KIP_Setup.ip_addr_KIP.addr[3]);
	  Len_text += sprintf(txt_out, "%sАдрес шлюза            - %d.%d.%d.%d\r\n", txt_out, \
			  KIP_Data.KIP_Setup.ip_addr_dect.addr[0], KIP_Data.KIP_Setup.ip_addr_dect.addr[1], \
			  KIP_Data.KIP_Setup.ip_addr_dect.addr[2], KIP_Data.KIP_Setup.ip_addr_dect.addr[3]);
	  Len_text += sprintf(txt_out, "%sМаска сети             - %d.%d.%d.%d\r\n", txt_out, \
			  KIP_Data.KIP_Setup.ip_addr_mask.addr[0], KIP_Data.KIP_Setup.ip_addr_mask.addr[1], \
			  KIP_Data.KIP_Setup.ip_addr_mask.addr[2], KIP_Data.KIP_Setup.ip_addr_mask.addr[3]);
	  Len_text += sprintf(txt_out, "%sПорт Modbus TCP        - %d\r\n", txt_out, KIP_Data.KIP_Setup.ip_port);
//	  my_printf(BT_uart, (uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));
	  bt_printf((uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));

//		HAL_UART_Transmit(&huart1, (uint8_t*)txt_out, strlen(txt_out), 0x10); 	// отправка сообщений в gsm

	  Len_text = sprintf(txt_out,  "\r\n");
	  if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_Bluetooth){
		  Len_text += sprintf(txt_out, "%sРежим работы Bluetooth - %s\r\n", txt_out, "терминал");
	  }
	  if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_SIM){
		  Len_text += sprintf(txt_out, "%sРежим работы через GSM - %s\r\n", txt_out, "GPRS");
	  }

	  if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_BU){
		  Len_text += sprintf(txt_out, "%sРежим работы RS485     - %s\r\n", txt_out, "Управление СКЗ");
		  Len_text += sprintf(txt_out, "%sАдрес СКЗ              - %d\r\n \r\n", txt_out, KIP_Data.KIP_Setup.SKZ_Address_RS485);
		  Len_text += sprintf(txt_out, "%sОпрос каналов СКЗ:\r\n \r\n", txt_out);
	  }
	  if(KIP_Data.KIP_Setup.Modules_Config.Module_Set.m_mbus_RTU){
		  Len_text += sprintf(txt_out, "%sРежим работы RS485  - %s\r\n", txt_out, "RTU (Внешнее управление)");
		  Len_text += sprintf(txt_out, "%sАдрес КИП           - %d\r\n\r\n", txt_out, KIP_Data.KIP_Setup.KIP_Address_RS485);
	  }
//	  my_printf(&BT_uart, (uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));
	  bt_printf((uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));

	  Len_text =  sprintf(txt_out,   "Опрос каналов КИП:\r\n");
	  Len_text += sprintf(txt_out, "%sВыходное напряжение СКЗ, В   = %s\r\n", txt_out, fl2str(KIP_Data.KIP_ADC.V_skz, 2));
	  Len_text += sprintf(txt_out, "%sВыходной ток СКЗ, А          = %s\r\n", txt_out, fl2str(KIP_Data.KIP_ADC.I_skz, 2));
	  Len_text += sprintf(txt_out, "%sЗащитный потенциал СОС, В    = %s\r\n", txt_out, fl2str(KIP_Data.KIP_ADC.V_sum, 2));
	  Len_text += sprintf(txt_out, "%sЗащитный потенциал БОС, В    = %s\r\n", txt_out, fl2str(KIP_Data.KIP_ADC.V_pol, 2));
	  Len_text += sprintf(txt_out, "%sТок поляризации, мА          = %s\r\n", txt_out, fl2str(KIP_Data.KIP_ADC.I_pol, 2));
	  Len_text += sprintf(txt_out, "%sНапряжение сети, В           = %s (%s) (%s)\r\n", txt_out, fl2str(KIP_Data.KIP_V_Power.V_power, 2), "Норма", "Отсутствует");
	  Len_text += sprintf(txt_out, "%sТемпература внутри КИП, °С   = %s\r\n", txt_out, fl2str(KIP_Data.T_stm,1));
	  Len_text += sprintf(txt_out, "%sНапряжение управления СКЗ, В = %s\r\n", txt_out, fl2str(KIP_Data.V_out,2));
	  Len_text += sprintf(txt_out, "%sДатчик вскрытия              - %s %s\r\n\r\n", txt_out, "Норма", "Вскрытие");
//	  my_printf(&BT_uart, (uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));
	  bt_printf((uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));
//
	  Len_text =  sprintf(txt_out,   "Опрос счетчика эл.энергии:\r\n");
	  if(KIP_Data.Mercury_Data.Mercury_Have == 1){
		  Len_text += sprintf(txt_out, "%sЗаводской номер счетчика     - %u\r\n", txt_out, KIP_Data.Mercury_Data.Mercury_ID);
		  Len_text += sprintf(txt_out, "%sНапряжение сети, В           = %s\r\n", txt_out, fl2str(KIP_Data.Mercury_Data.Mercury_V, 1));
		  Len_text += sprintf(txt_out, "%sМощность, Вт                 = %u\r\n", txt_out, KIP_Data.Mercury_Data.Mercury_P);
		  Len_text += sprintf(txt_out, "%sНакопленная мощность, Вт     = %u\r\n\r\n", txt_out, KIP_Data.Mercury_Data.Mercury_P_sum);
	  }
	  else{
		  Len_text += sprintf(txt_out, "%sЗаводской номер счетчика     - %u (Нет связи)\r\n\r\n", txt_out, KIP_Data.Mercury_Data.Mercury_ID);
	  }
//	  my_printf(&BT_uart, (uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));
	  bt_printf((uint8_t*)txt_out); CDC_Transmit_FS((uint8_t*)txt_out, strlen(txt_out));

	  osDelay(2500);
//	  Mercury_Write(Read_V_I_P);
  }
  /* USER CODE END Start_Default */
}

/* USER CODE BEGIN Header_Start_Led */
/**
* @brief Function implementing the Task_Led thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Led */
void Start_Led(void const * argument)
{
  /* USER CODE BEGIN Start_Led */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(Led1_GPIO_Port, Led1_Pin);
	osDelay(250);
  }
  /* USER CODE END Start_Led */
}

/* USER CODE BEGIN Header_Start_uIP */
/**
* @brief Function implementing the Task_uIP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_uIP */
void Start_uIP(void const * argument)
{
  /* USER CODE BEGIN Start_uIP */

	HAL_TIM_Base_Start_IT(&htim2);			//
	for (;;)
	{
		uint16_t i = 0;

		  char txt_out_usb[512];
		HAL_GPIO_TogglePin(Led3_GPIO_Port, Led3_Pin);

		eMBPoll();

		uip_len = enc28j60_packetReceive((uint8_t *) uip_buf, UIP_BUFSIZE);

		if (uip_len > 0)
		{
			if (BUF->type == htons(UIP_ETHTYPE_IP))
			{
				// если пакет IP, то засылаем в стек
				uip_arp_ipin();
				uip_input();
				if (uip_len > 0)
				{
					// если есть что-то на выход, засылаем в сеть
					uip_arp_out();
					enc28j60_packetSend((uint8_t *) uip_buf, uip_len);
				}
			}
			else if (BUF->type == htons(UIP_ETHTYPE_ARP))
			{
				// если это касается ARP, то передаем в блок ARP
				uip_arp_arpin();
				if (uip_len > 0)
				{
					enc28j60_packetSend((uint8_t *) uip_buf, uip_len);
				}
			}
		}
		if(delay_arp)
		{
			HAL_GPIO_TogglePin(Led2_GPIO_Port, Led2_Pin);

			delay_arp = 0;
			HAL_TIM_Base_Start_IT(&htim2);			//
			//			osDelay(configTICK_RATE_HZ/2); 			// полсекунды

			for (uint32_t i = 0; i < UIP_CONNS; i++)
			{
				uip_periodic(i);
				if (uip_len > 0)
				{
					uip_arp_out();
					enc28j60_packetSend((uint8_t *)uip_buf, uip_len);
				}
			}

			#if UIP_UDP
			for(uint32_t i = 0; i < UIP_UDP_CONNS; i++)
			{
				uip_udp_periodic(i);
				if(uip_len > 0)
				{
					uip_arp_out();
					enc28j60_packetSend((uint8_t *)uip_buf, uip_len);
				}
			}
			#endif /* UIP_UDP */

			delay_arp_10sek++;
			if (delay_arp_10sek >= 20)
			{ // один раз за 20 проходов цикла, около 10 сек
				delay_arp_10sek = 0;
				uip_arp_timer();
			}
		}

		osDelay(5);

//	    vTaskDelay(100);
//     taskYIELD();
	}
  /* USER CODE END Start_uIP */
}

/* USER CODE BEGIN Header_Start_ADC */
/**
* @brief Function implementing the Task_ADC thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_ADC */
void Start_ADC(void const * argument)
{
  /* USER CODE BEGIN Start_ADC */
  Mercury_Page = 1;

  /* Infinite loop */
  for(;;)
  {
	HAL_IWDG_Refresh(&hiwdg);
	  if (Mercury_Page == 1) Mercury_Write(Read_V_I_P);		// Опрос счетчика электроэнергии
	  if (Mercury_Page == 2) Mercury_Write(Read_P_sum);		// Опрос счетчика электроэнергии
//	  Mercury_Busy++;

	  //HAL_TIM_Base_Start(&htim4);
		HAL_TIM_Base_Start_IT(&htim4);	// Запуск HAL_ADCEx_InjectedStart_IT(&hadc1);

		// Измерения каналов ADC1115
		adc_izm();

		// Измерение напряжения сети
		int32_t ADC_Val_sum = 0;
		for(uint16_t i = 0; i < sizeof(KIP_Data.KIP_V_Power.ADC_Val)/sizeof(uint16_t); i++)
		{
			int16_t sum_val = ((int32_t)KIP_Data.KIP_V_Power.ADC_Val[i]) - 2048;
				if (sum_val > 0)	ADC_Val_sum = ADC_Val_sum + sum_val;
				else				ADC_Val_sum = ADC_Val_sum - sum_val;
		}
		ADC_Val_sum = ADC_Val_sum / (sizeof(KIP_Data.KIP_V_Power.ADC_Val)/sizeof(uint16_t));
		KIP_Data.KIP_V_Power.V_power = (float)(ADC_Val_sum) * 0.230f;	// Коэффициент приведения к эффективному напряжению

		osDelay(1000);
  }
  /* USER CODE END Start_ADC */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//#if UIP_CONF_LOGGING == 1
//void uip_log(char *m)
//{
////    sendString(m);
//    printf("%s", m);
//    //TODO: Get debug information out here somehow, does anybody know a smart way to do that?
//}
//#endif

//-----------------------------------------------
char * fl2str(float Value, uint8_t dec)
{
	uint32_t dec_smesh = 1;
	for(uint8_t i = 0; i < dec; i++)
	{
		dec_smesh = dec_smesh * 10;
	}

	int32_t	dec_val = (int32_t)(Value * dec_smesh);
	switch(dec)
	{
		case 0:{
			sprintf(FLT_buf, "%d", dec_val);
			break;
		}
		case 1:{
			sprintf(FLT_buf, "%d.%01d", dec_val/10, abs(dec_val%10));
			break;
		}
		case 2:{
			sprintf(FLT_buf, "%d.%02d", dec_val/100, abs(dec_val%100));
			break;
		}
		case 3:{
			sprintf(FLT_buf, "%d.%03d", dec_val/1000, abs(dec_val%1000));
			break;
		}
		case 4:{
			sprintf(FLT_buf, "%d.%04d", dec_val/10000, abs(dec_val%10000));
			break;
		}
		case 5:{
			sprintf(FLT_buf, "%d.%05d", dec_val/100000, abs(dec_val%100000));
			break;
		}
		case 6:{
			sprintf(FLT_buf, "%d.%06d", dec_val/1000000, abs(dec_val%1000000));
			break;
		}
	}
return FLT_buf;
};

static volatile uint32_t lock_nesting_count = 0;
void __critical_enter(void)
{
    __disable_irq();
    ++lock_nesting_count;
}
void __critical_exit(void)
{
    /* Unlock interrupts only when we are exiting the outermost nested call. */
    --lock_nesting_count;

    if (lock_nesting_count == 0) {
        __enable_irq();
    }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
