/**
 * \addtogroup httpd
 * @{
 */

/**
 * \file
 *         Web server script interface
 * \author
 *         Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2001-2006, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: httpd-cgi.c,v 1.2 2006/06/11 21:46:37 adam Exp $
 *
 */

/**/
#include "config.h"
extern KIP_Data_TypeDef KIP_Data;

extern char * fl2str(float Value, uint8_t dec);

//#include "stm32f3xx_hal.h"


#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd-cgi.h"
#include "httpd-fs.h"

#include <stdio.h>
#include <string.h>

HTTPD_CGI_CALL(file, "file-stats", file_stats);
HTTPD_CGI_CALL(tcp, "tcp-connections", tcp_stats);
HTTPD_CGI_CALL(net, "net-stats", net_stats);

HTTPD_CGI_CALL(cgi_Mercury_ID, "Mercury_ID", mycgi_Mercury_ID);		// KIP_Data.Mercury_Data.Mercury_ID
HTTPD_CGI_CALL(cgi_Mercury_V, "Mercury_V", mycgi_Mercury_V);		// KIP_Data.Mercury_Data.Mercury_V
HTTPD_CGI_CALL(cgi_Mercury_P, "Mercury_P", mycgi_Mercury_P);		// KIP_Data.Mercury_Data.Mercury_P
HTTPD_CGI_CALL(cgi_Mercury_sum_P, "Mercury_sum_P", mycgi_Mercury_sum_P);		// KIP_Data.Mercury_Data.Mercury_P_sum
HTTPD_CGI_CALL(cgi_Mercury_Have, "Mercury_Have", mycgi_Mercury_Have);// KIP_Data.Mercury_Data.Mercury_Have

HTTPD_CGI_CALL(cgi_IP_Device, "IP_Device", mycgi_IP_Device);		// KIP_Data.ip_addr_KIP

HTTPD_CGI_CALL(cgi_V_skz, "V_skz", mycgi_V_skz);					// KIP_Data.V_skz
HTTPD_CGI_CALL(cgi_I_skz, "I_skz", mycgi_I_skz);					// KIP_Data.I_skz

HTTPD_CGI_CALL(cgi_V_sum, "V_sum", mycgi_V_sum);					// KIP_Data.V_sum
HTTPD_CGI_CALL(cgi_V_pol, "V_pol", mycgi_V_pol);					// KIP_Data.V_pol
HTTPD_CGI_CALL(cgi_I_pol, "I_pol", mycgi_I_pol);					// KIP_Data.I_pol

HTTPD_CGI_CALL(cgi_T_stm, "T_stm", mycgi_T_stm);					// KIP_Data.T_stm
HTTPD_CGI_CALL(cgi_V_power, "V_power", mycgi_V_power);				// KIP_Data.V_Power.V_power
HTTPD_CGI_CALL(cgi_V_out, "V_out", mycgi_V_out);					// KIP_Data.V_out

HTTPD_CGI_CALL(cgi_door_flag, "door_flag", mycgi_door_flag);		// KIP_Data.Flags;
HTTPD_CGI_CALL(cgi_RS485_Addr, "RS485_Addr", mycgi_RS485_Addr);		// KIP_Data.KIP_Setup.KIP_Address_RS485
HTTPD_CGI_CALL(cgi_Shunt_Val, "Shunt_Val", mycgi_Shunt_Val);		// KIP_Data.KIP_Setup.Shunt_Val

///*&file, &tcp, &net, */
static const struct httpd_cgi_call *calls[] = { \
		&cgi_Mercury_ID, &cgi_Mercury_V, &cgi_Mercury_P, &cgi_Mercury_sum_P, &cgi_Mercury_Have,\
		&cgi_IP_Device, &cgi_V_skz, &cgi_I_skz, \
		&cgi_V_sum, &cgi_V_pol, &cgi_I_pol, \
		&cgi_T_stm, &cgi_V_power, &cgi_V_out, \
		&cgi_door_flag, &cgi_RS485_Addr, &cgi_Shunt_Val, NULL };

/*---------------------------------------------------------------------------*/
static PT_THREAD(nullfunction(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
httpd_cgifunction httpd_cgi(char *name)
{
  const struct httpd_cgi_call **f;
  /* Find the matching name in the table, return the function. */
  for(f = calls; *f != NULL; ++f) {
    if(strncmp((*f)->name, name, strlen((*f)->name)) == 0) {
			return (*f)->function;
    }
  }
  return nullfunction;
}
/*---------------------------------------------------------------------------*/
static unsigned short generate_file_stats(void *arg)
{
  char *f = (char *)arg;
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%5u", httpd_fs_count(f));
}
/*---------------------------------------------------------------------------*/
static PT_THREAD(file_stats(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_GENERATOR_SEND(&s->sout, generate_file_stats, strchr(ptr, ' ') + 1);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static const char closed[] =   /*  "CLOSED",*/
{0x43, 0x4c, 0x4f, 0x53, 0x45, 0x44, 0};
static const char syn_rcvd[] = /*  "SYN-RCVD",*/
{0x53, 0x59, 0x4e, 0x2d, 0x52, 0x43, 0x56,
 0x44,  0};
static const char syn_sent[] = /*  "SYN-SENT",*/
{0x53, 0x59, 0x4e, 0x2d, 0x53, 0x45, 0x4e,
 0x54,  0};
static const char established[] = /*  "ESTABLISHED",*/
{0x45, 0x53, 0x54, 0x41, 0x42, 0x4c, 0x49, 0x53, 0x48,
 0x45, 0x44, 0};
static const char fin_wait_1[] = /*  "FIN-WAIT-1",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49,
 0x54, 0x2d, 0x31, 0};
static const char fin_wait_2[] = /*  "FIN-WAIT-2",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49,
 0x54, 0x2d, 0x32, 0};
static const char closing[] = /*  "CLOSING",*/
{0x43, 0x4c, 0x4f, 0x53, 0x49,
 0x4e, 0x47, 0};
static const char time_wait[] = /*  "TIME-WAIT,"*/
{0x54, 0x49, 0x4d, 0x45, 0x2d, 0x57, 0x41,
 0x49, 0x54, 0};
static const char last_ack[] = /*  "LAST-ACK"*/
{0x4c, 0x41, 0x53, 0x54, 0x2d, 0x41, 0x43,
 0x4b, 0};

static const char *states[] = {
  closed,
  syn_rcvd,
  syn_sent,
  established,
  fin_wait_1,
  fin_wait_2,
  closing,
  time_wait,
  last_ack};


static unsigned short generate_tcp_stats(void *arg)
{
  struct uip_conn *conn;
  struct httpd_state *s = (struct httpd_state *)arg;

  conn = &uip_conns[s->count];
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,
		 "<tr><td>%d</td><td>%u.%u.%u.%u:%u</td><td>%s</td><td>%u</td><td>%u</td><td>%c %c</td></tr>\r\n",
		 htons(conn->lport),
		 htons(conn->ripaddr[0]) >> 8,
		 htons(conn->ripaddr[0]) & 0xff,
		 htons(conn->ripaddr[1]) >> 8,
		 htons(conn->ripaddr[1]) & 0xff,
		 htons(conn->rport),
		 states[conn->tcpstateflags & UIP_TS_MASK],
		 conn->nrtx,
		 conn->timer,
		 (uip_outstanding(conn))? '*':' ',
		 (uip_stopped(conn))? '!':' ');
}
/*---------------------------------------------------------------------------*/
static PT_THREAD(tcp_stats(struct httpd_state *s, char *ptr))
{

  PSOCK_BEGIN(&s->sout);

  for(s->count = 0; s->count < UIP_CONNS; ++s->count) {
    if((uip_conns[s->count].tcpstateflags & UIP_TS_MASK) != UIP_CLOSED) {
      PSOCK_GENERATOR_SEND(&s->sout, generate_tcp_stats, s);
    }
  }

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short generate_net_stats(void *arg)
{
  struct httpd_state *s = (struct httpd_state *)arg;
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,
		  "%5u\n", ((uip_stats_t *)&uip_stat)[s->count]);
}

static PT_THREAD(net_stats(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

#if UIP_STATISTICS

  for(s->count = 0; s->count < sizeof(uip_stat) / sizeof(uip_stats_t);
      ++s->count) {
    PSOCK_GENERATOR_SEND(&s->sout, generate_net_stats, s);
  }

#endif /* UIP_STATISTICS */

  PSOCK_END(&s->sout);
}


/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_Mercury_ID(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%u", KIP_Data.Mercury_Data.Mercury_ID);
}
static PT_THREAD(mycgi_Mercury_ID(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_Mercury_ID, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_Mercury_V(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.Mercury_Data.Mercury_V, 1));
}
static PT_THREAD(mycgi_Mercury_V(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_Mercury_V, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_Mercury_P(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%u", KIP_Data.Mercury_Data.Mercury_P);
}
static PT_THREAD(mycgi_Mercury_P(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_Mercury_P, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_Mercury_sum_P(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%u", KIP_Data.Mercury_Data.Mercury_P_sum);
}
static PT_THREAD(mycgi_Mercury_sum_P(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_Mercury_sum_P, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_Mercury_Have(void *arg)
{
	if(KIP_Data.Mercury_Data.Mercury_Have == 1)
	{
		return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", "Есть");
	}
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", "Нет");
}
static PT_THREAD(mycgi_Mercury_Have(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_Mercury_Have, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_IP_Device(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%u.%u.%u.%u", KIP_Data.KIP_Setup.ip_addr_KIP.addr[0], \
		  KIP_Data.KIP_Setup.ip_addr_KIP.addr[1], KIP_Data.KIP_Setup.ip_addr_KIP.addr[2], KIP_Data.KIP_Setup.ip_addr_KIP.addr[3]);
}
static PT_THREAD(mycgi_IP_Device(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_IP_Device, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_V_skz(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.KIP_ADC.V_skz, 2));
}
static PT_THREAD(mycgi_V_skz(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_V_skz, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_I_skz(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.KIP_ADC.I_skz, 2));
}
static PT_THREAD(mycgi_I_skz(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_I_skz, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_V_sum(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.KIP_ADC.V_sum, 2));
}
static PT_THREAD(mycgi_V_sum(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_V_sum, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_V_pol(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.KIP_ADC.V_pol, 2));
}
static PT_THREAD(mycgi_V_pol(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_V_pol, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_I_pol(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.KIP_ADC.I_pol, 2));
}
static PT_THREAD(mycgi_I_pol(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_I_pol, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_T_stm(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.T_stm, 1));
}
static PT_THREAD(mycgi_T_stm(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_T_stm, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_V_power(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.KIP_V_Power.V_power, 1));
}
static PT_THREAD(mycgi_V_power(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_V_power, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_V_out(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.V_out, 2));
}
static PT_THREAD(mycgi_V_out(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_V_out, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_door_flag(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%u", KIP_Data.Flags);
}
static PT_THREAD(mycgi_door_flag(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_door_flag, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_RS485_Addr(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%u", (uint32_t)KIP_Data.KIP_Setup.KIP_Address_RS485);
}
static PT_THREAD(mycgi_RS485_Addr(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_RS485_Addr, s);  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static unsigned short generate_cgi_Shunt_Val(void *arg)
{
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%s", fl2str(KIP_Data.KIP_Setup.Shunt_Val, 2));
}
static PT_THREAD(mycgi_Shunt_Val(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);	PSOCK_GENERATOR_SEND(&s->sout, generate_cgi_Shunt_Val, s);  PSOCK_END(&s->sout);
}





/*---------------------------------------------------------------------------*/
/** @} */
