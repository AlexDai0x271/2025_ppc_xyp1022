/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H

#include "xyp1022.h"

#ifdef USE_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#endif // #ifdef USE_OS_FREERTOS

#define LWIP_DEBUG_BY_USER

#ifdef LWIP_DEBUG_BY_USER
#define LWIP_DEBUG_BY_USER_IPERF_SERVER 0
#define LWIP_DEBUG_BY_USER_IPERF_CLIENT 1
#endif

// NOTE: notify 'NETIN'' thread by recv interrupt
// #define LWIP_FREERTOS_WITH_INTERRUPT

// NOTE: notify 'NETIN'' thread by recv interrupt compensation
// #define LWIP_FREERTOS_XFER_COMPENSATION

// NOTE: select the function when running 'iperf_client'
#define LWIP_IPERF_CLIENT_RECV
// #define LWIP_IPERF_CLIENT_SEND

#define CHECKSUM_GEN_IP     0
#define CHECKSUM_GEN_UDP    0
#define CHECKSUM_GEN_TCP    0
#define CHECKSUM_GEN_ICMP   0

#define CHECKSUM_CHECK_IP   0
#define CHECKSUM_CHECK_UDP  0
#define CHECKSUM_CHECK_TCP  0
#define CHECKSUM_CHECK_ICMP 0

// #define LWIP_DEBUG

#ifdef LWIP_DEBUG
#define LWIP_PLATFORM_DIAG(x) \
    do {                      \
        yprintf x;            \
    } while (0)

#define LWIP_DBG_MIN_LEVEL LWIP_DBG_LEVEL_ALL

#define PPP_DEBUG          LWIP_DBG_OFF
#define MEM_DEBUG          LWIP_DBG_OFF
#define MEMP_DEBUG         LWIP_DBG_OFF
#define PBUF_DEBUG         LWIP_DBG_ON
#define API_LIB_DEBUG      LWIP_DBG_OFF
#define API_MSG_DEBUG      LWIP_DBG_OFF
#define TCPIP_DEBUG        LWIP_DBG_OFF
#define NETIF_DEBUG        LWIP_DBG_OFF
#define SOCKETS_DEBUG      LWIP_DBG_OFF
#define DNS_DEBUG          LWIP_DBG_OFF
#define AUTOIP_DEBUG       LWIP_DBG_OFF
#define DHCP_DEBUG         LWIP_DBG_OFF
#define IP_DEBUG           LWIP_DBG_OFF
#define IP_REASS_DEBUG     LWIP_DBG_OFF
#define ICMP_DEBUG         LWIP_DBG_OFF
#define IGMP_DEBUG         LWIP_DBG_OFF
#define UDP_DEBUG          LWIP_DBG_OFF
#define TCP_DEBUG          LWIP_DBG_OFF
#define TCP_INPUT_DEBUG    LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG   LWIP_DBG_OFF
#define TCP_RTO_DEBUG      LWIP_DBG_OFF
#define TCP_CWND_DEBUG     LWIP_DBG_OFF
#define TCP_WND_DEBUG      LWIP_DBG_OFF
#define TCP_FR_DEBUG       LWIP_DBG_OFF
#define TCP_QLEN_DEBUG     LWIP_DBG_OFF
#define TCP_RST_DEBUG      LWIP_DBG_OFF

#define LWIP_DBG_TYPES_ON  (LWIP_DBG_ON | LWIP_DBG_TRACE | LWIP_DBG_STATE | LWIP_DBG_FRESH | LWIP_DBG_HALT)
#endif

#define NO_SYS         0

#define LWIP_IPV4      1
#define LWIP_IPV6      0

#define LWIP_SOCKET    (NO_SYS == 0)
#define LWIP_NETCONN   (NO_SYS == 0)
#define LWIP_NETIF_API 0

#define LWIP_IGMP      LWIP_IPV4
#define LWIP_ICMP      LWIP_IPV4

#define LWIP_SNMP      LWIP_IPV4

#ifdef LWIP_HAVE_MBEDTLS
#define LWIP_SNMP_V3 (LWIP_SNMP)
#endif

#define LWIP_DNS                      0
#define LWIP_MDNS_RESPONDER           0

#define LWIP_TCPIP_CORE_LOCKING       1
#define LWIP_TCPIP_CORE_LOCKING_INPUT 1

/**
 * NO_SYS_NO_TIMERS==1: Drop support for sys_timeout when NO_SYS==1
 * Mainly for compatibility to old versions.
 */
#define NO_SYS_NO_TIMERS              0

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT                 4U

#define MEM_SIZE                      KiB(25)

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF                 25
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB              4
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB              6
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN       6
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG              150
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT          6

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE                32

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE             1600

/** SYS_LIGHTWEIGHT_PROT
 * define SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task protection
 * for certain critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT          (NO_SYS == 0)

/* ---------- TCP options ---------- */
#define LWIP_TCP                      1
#define TCP_TTL                       255

#define LWIP_ALTCP                    (LWIP_TCP)

#ifdef LWIP_HAVE_MBEDTLS
#define LWIP_ALTCP_TLS         (LWIP_TCP)
#define LWIP_ALTCP_TLS_MBEDTLS (LWIP_TCP)
#endif

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ                 0

/* TCP Maximum segment size. */
#define TCP_MSS                         (1500 - 40)

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF                     (10 * TCP_MSS)

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
#define TCP_SND_QUEUELEN                (8 * TCP_SND_BUF / TCP_MSS)

/* TCP receive window. */
#define TCP_WND                         (11 * TCP_MSS)

/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX                      4

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX                   4

/* ---------- ARP options ---------- */
#define LWIP_ARP                        1
#define ARP_TABLE_SIZE                  10
#define ARP_QUEUEING                    1

/* ---------- IP options ---------- */
/* Define IP_FORWARD to 1 if you wish to have the ability to forward
   IP packets across network interfaces. If you are going to run lwIP
   on a device with only one network interface, define this to 0. */
#define IP_FORWARD                      0

/* IP reassembly and segmentation.These are orthogonal even
 * if they both deal with IP fragments */
#define IP_REASSEMBLY                   0
#define IP_REASS_MAX_PBUFS              (10 * ((1500 + PBUF_POOL_BUFSIZE - 1) / PBUF_POOL_BUFSIZE))
#define MEMP_NUM_REASSDATA              IP_REASS_MAX_PBUFS
#define IP_FRAG                         0
#define IPV6_FRAG_COPYHEADER            0

/* ---------- ICMP options ---------- */
#define ICMP_TTL                        255

/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. */
#define LWIP_DHCP                       0

/* 1 if you want to do an ARP check on the offered address
   (recommended). */
#define DHCP_DOES_ARP_CHECK             (LWIP_DHCP)

/* ---------- AUTOIP options ------- */
#define LWIP_AUTOIP                     (LWIP_DHCP)
#define LWIP_DHCP_AUTOIP_COOP           (LWIP_DHCP && LWIP_AUTOIP)

/* ---------- UDP options ---------- */
#define LWIP_UDP                        1
#define LWIP_UDPLITE                    LWIP_UDP
#define UDP_TTL                         255

/* ---------- RAW options ---------- */
#define LWIP_RAW                        1

/* ---------- Statistics options ---------- */
#define LWIP_STATS                      0
#define LWIP_STATS_DISPLAY              0

/* ---------- NETBIOS options ---------- */
#define LWIP_NETBIOS_RESPOND_NAME_QUERY 1

/* The following defines must be done even in OPTTEST mode: */

#if !defined(NO_SYS) || !NO_SYS /* default is 0 */
void sys_check_core_locking(void);
#define LWIP_ASSERT_CORE_LOCKED() sys_check_core_locking()
#endif

#ifndef LWIP_PLATFORM_ASSERT
#include "xprintf.h"
#define LWIP_PLATFORM_ASSERT(x) yprintf("Assert : %s %d, %s\r\n", x, __LINE__, __FILE__)
#endif

#if !defined(NO_SYS) || !NO_SYS /* default is 0 */

/* ---------- User define options ---------- */

#define IP_ADDR0                    192
#define IP_ADDR1                    168
#define IP_ADDR2                    0
#define IP_ADDR3                    122

#define NETMASK_ADDR0               255
#define NETMASK_ADDR1               255
#define NETMASK_ADDR2               255
#define NETMASK_ADDR3               0

#define GW_ADDR0                    192
#define GW_ADDR1                    168
#define GW_ADDR2                    0
#define GW_ADDR3                    1

#define GMAC_DESCRIPTOR_NUM         (32)
#define GMAC_BUFFER_SIZE            (PBUF_POOL_BUFSIZE)

#define DEFAULT_UDP_RECVMBOX_SIZE   20
#define DEFAULT_TCP_RECVMBOX_SIZE   20
#define DEFAULT_ACCEPTMBOX_SIZE     20
#define DEFAULT_THREAD_STACKSIZE    (configMINIMAL_STACK_SIZE)

#define TCPIP_MBOX_SIZE             20

#define NETIF_IN_THREAD_NAME        "NETIN"
#define NETIF_IN_THREAD_STACK_SIZE  DEFAULT_THREAD_STACKSIZE
#define NETIF_IN_THREAD_PRIORITY    (3)

#define TCPIP_THREAD_NAME           "lwip"
#define TCPIP_THREAD_STACKSIZE      DEFAULT_THREAD_STACKSIZE
#define TCPIP_THREAD_PRIO           (5)

#define TCP_SERVER_THREAD_NAME      "iperf"
#define TCP_SERVER_THREAD_STACKSIZE DEFAULT_THREAD_STACKSIZE
#define TCP_SERVER_THREAD_PRIO      (4)

#define LWIP_NOASSERT

#endif /* !defined(NO_SYS) || !NO_SYS */

#endif /* LWIP_LWIPOPTS_H */
