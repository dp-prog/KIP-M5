#ifndef __UIP_MODBUS_H
#define __UIP_MODBUS_H

#include "uipopt.h"
#include "psock.h"



#if defined UIP_APPCALL || defined UIP_UDP_APPCALL
//#warn "*** PORT APP MAPPER WILL REDEFINE APPCALL ****"
	#undef UIP_APPCALL
//	#undef UIP_UDP_APPCALL
#endif

//typedef int uip_tcp_appstate_t;
//typedef int uip_tcp_mb_appstate_t;


void uip_modbus_appcall(void);

#ifndef UIP_APPCALL
#define UIP_APPCALL uip_modbus_appcall
#endif

void uip_modbus_init(void);
void uip_modbus_appcall(void);

#if defined PORT_APP_MAPPER
	#define MB_SERVER_APP_CALL_MAP {uip_modbus_appcall, 502, 0},
//	struct httpd_state httpd_state_list[UIP_CONF_MAX_CONNECTIONS];
#else
	#define WEBSERVER_APP_CALL_MAP
	typedef struct httpd_state uip_tcp_appstate_t;
#endif

#endif
