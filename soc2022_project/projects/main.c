/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "XYR3201.h"
#include "sysclock.h"
#include "drv_common.h"
#include "xprintf.h"
#include "xshell.h"

#ifdef USE_OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif // #ifdef USE_OS_FREERTOS

/* lwIP core includes */
#include "lwip/opt.h"

#include "lwip/err.h"
#include "lwip/netif.h"

#include "lwip/init.h"
#include "lwip/ip4_addr.h"

#include "lwip/sys.h"
#include "lwip/tcpip.h"

static struct netif g_netif;

/* ----- ----- ----- ----- USER TASK DEFINE START ----- ----- ----- ----- */

#ifdef USE_OS_FREERTOS

#ifndef OS_WAIT_FOREVER
#define OS_WAIT_FOREVER ((TickType_t)(~0))
#endif

#ifdef DEMO_TASK
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_task.h"
#endif // #ifdef DEMO_TASK

#ifdef DEMO_EVENT
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_event.h"
#endif // #ifdef DEMO_EVENT

#ifdef DEMO_MQ
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_mq.h"
#endif // #ifdef DEMO_MQ

#ifdef DEMO_SEMPHR
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_semphr.h"
#endif // #ifdef DEMO_SEMPHR

#ifdef DEMO_MUTEX
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_mutex.h"
#endif // #ifdef DEMO_MUTEX

#ifdef DEMO_MEMPOOL
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_mempool.h"
#endif // #ifdef DEMO_MEMPOOL

#ifdef DEMO_TIMER
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_timer.h"
#endif // #ifdef DEMO_TIMER

#ifdef DEMO_ALL
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_all.h"
#endif // #ifdef DEMO_ALL

#ifdef DEMO_OSDRIVER1
#include "./demo_osdriver1.h"
#endif // #ifdef DEMO_OSDRIVER1

mTaskDefine(xshell_task)
{
    for (;;) {
        char aszCommand[128];
        uint32_t ui32CommandLengh = Shell_GetCommand(aszCommand, ARRAY_SIZE(aszCommand));
        aszCommand[ui32CommandLengh] = 0;
        Shell_HandleCommand(aszCommand, ui32CommandLengh);
    }
}

mTaskDefine(iperf_tcp_task)
{
    extern void iperf_tcp_server(void *pvParameters);
    iperf_tcp_server(NULL);
}

mTaskDefine(iperf_udp_task)
{
    extern void iperf_udp_server(void *pvParameters);
    iperf_udp_server(NULL);
}

void uTaskCreate(void)
{
    mTaskCreate(iperf_tcp_task, TCP_SERVER_THREAD_NAME "tcp", TCP_SERVER_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);
    mTaskCreate(iperf_udp_task, TCP_SERVER_THREAD_NAME "udp", TCP_SERVER_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);

    mTaskCreate(xshell_task, "xshell", 2048, (tskIDLE_PRIORITY + 1));
}

#endif // #ifdef USE_OS_FREERTOS

/* ----- ----- ----- ----- USER TASK DEFINE END   ----- ----- ----- ----- */

/* ----- ----- ----- ----- LwIP INIT DEFINE START ----- ----- ----- ----- */

static err_t _ethernetif_init(struct netif *netif)
{
    extern err_t ethernetif_init(struct netif * netif);
    return ethernetif_init(netif);
}

static err_t _tcpip_input(struct pbuf *p, struct netif *netif)
{
    extern err_t tcpip_input(struct pbuf * p, struct netif * inp);
    return tcpip_input(p, netif);
}

void LwIP_Init(void)
{
    ip4_addr_t ipaddr;
    ip4_addr_t netmask;
    ip4_addr_t gw;

    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    tcpip_init(NULL, NULL);

    /* add the network interface (IPv4/IPv6) without RTOS */
    netif_add(&g_netif, &ipaddr, &netmask, &gw, NULL, (void *)_ethernetif_init, (void *)_tcpip_input);

    /* Registers the default network interface */
    netif_set_default(&g_netif);

    if (netif_is_link_up(&g_netif)) {
        netif_set_up(&g_netif);
        netif_set_link_up(&g_netif);
    } else {
        netif_set_down(&g_netif);
        netif_set_link_down(&g_netif);
    }

    if (!ip4_addr_isany(netif_ip4_addr((&g_netif)))) {
        xprintf("Static assigned IP: %s\n", ip4addr_ntoa(netif_ip4_addr(&g_netif)));
    }
}

/* ----- ----- ----- ----- LwIP INIT DEFINE END   ----- ----- ----- ----- */

int main(void)
{
    xprintf("Hello, world.\n");
    xprintf("build %s %s.\n", __DATE__, __TIME__);
    xprintf("\n");

#ifdef USE_OS_FREERTOS
    portDISABLE_INTERRUPTS();

    LwIP_Init();

    uTaskCreate();
    vTaskStartScheduler();
#endif // #ifdef USE_OS_FREERTOS

    for (;;) {
        mdelay(100);
    }

    return 0;
}
