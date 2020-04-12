#ifndef __UIP_CONF_H
#define __UIP_CONF_H

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * 8 bit datatype
 *
 * This typedef defines the 8-bit type used throughout uIP.
 *
 * \hideinitializer
 */
typedef uint8_t u8_t;

/**
 * 16 bit datatype
 *
 * This typedef defines the 16-bit type used throughout uIP.
 *
 * \hideinitializer
 */
typedef uint16_t u16_t;

/**
 * Statistics datatype
 *
 * This typedef defines the dataype used for keeping statistics in
 * uIP.
 *
 * \hideinitializer
 */
typedef unsigned short uip_stats_t;

/**
 * Maximum number of TCP connections.
 *
 * \hideinitializer
 */
#define UIP_CONF_MAX_CONNECTIONS	40

/**
 * Maximum number of listening TCP ports.
 *
 * \hideinitializer
 */
#define UIP_CONF_MAX_LISTENPORTS	40

/**
 * uIP buffer size.
 *
 * \hideinitializer
 */
#define UIP_CONF_BUFFER_SIZE		600
/**
 * CPU byte order.
 *
 * \hideinitializer
 */
#define UIP_CONF_BYTE_ORDER			UIP_LITTLE_ENDIAN
//#define UIP_CONF_BYTE_ORDER      LITTLE_ENDIAN
/**
 * Logging on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_LOGGING			0
// todo Надо поставить 1
/**
 * UDP support on or off requires DHCP if ON
 *
 * \hideinitializer
 */
#define UIP_CONF_UDP				0

/**
 * UDP checksums on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_UDP_CHECKSUMS		0

/**
 * uIP statistics on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_STATISTICS			0

/**
 * Broadcast support. Needed for dhcp
 *
 * \hideinitializer
 */
#define UIP_CONF_BROADCAST			0

/**
 * The maximum amount of concurrent UDP connections.
 *
 * \hideinitializer
 */
#define UIP_CONF_UDP_CONNS			0

#define UIP_REASSEMBLY				1

//Include app configuration
#include "apps-conf.h"


#endif
