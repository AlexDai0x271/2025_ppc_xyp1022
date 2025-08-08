/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
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

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
#include "netif/ppp/pppoe.h"

#include "lwip/sys.h"
#include "lwip/tcpip.h"

#include "drv_gmac.h"
#include "drv_gmac_phy.h"

extern void cache_invalid(void *addr, int32_t dsize);
extern void cache_flush(void *addr, int32_t dsize);

#ifdef DEBUG
#include "xprintf.h"
#endif

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
    struct eth_addr *ethaddr;
    /* Add whatever per-interface state that is needed here. */
};

/* Forward declarations. */
void ethernetif_input(void *pvParameters);

#ifdef LWIP_FREERTOS_WITH_INTERRUPT
SemaphoreHandle_t g_eth_semaphore;
#endif

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
    uint8_t source_mac[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};

    memcpy(&g_DeviceGmacDefaultConfig.mac[0], source_mac, 6);
    g_DeviceGmacDefaultConfig.mac_num = 1;

    HW_GMAC_InitConfig(GMAC0, &g_DeviceGmacDefaultConfig);

    xprintf("link speed: %d Mbps\n", PHY_GetLinkSpeed(g_DeviceGmacDefaultConfig.gmac, &g_DeviceGmacDefaultConfig));
    xprintf("link duplex(0:link down/1:half/2:full): %d\n", PHY_GetLinkDuplex(g_DeviceGmacDefaultConfig.gmac, &g_DeviceGmacDefaultConfig));

    /* Set netif link flag */
    netif->flags |= NETIF_FLAG_LINK_UP;

#if LWIP_ARP || LWIP_ETHERNET

    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] = source_mac[0];
    netif->hwaddr[1] = source_mac[1];
    netif->hwaddr[2] = source_mac[2];
    netif->hwaddr[3] = source_mac[3];
    netif->hwaddr[4] = source_mac[4];
    netif->hwaddr[5] = source_mac[5];

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
#if LWIP_ARP
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#else
    netif->flags |= NETIF_FLAG_BROADCAST;
#endif /* LWIP_ARP */

#endif /* LWIP_ARP || LWIP_ETHERNET */

    /* create the task that handles the ETH_MAC */
#ifdef LWIP_FREERTOS_WITH_INTERRUPT
    g_eth_semaphore = xSemaphoreCreateCounting(TCPIP_MBOX_SIZE, 0);
#endif
    sys_thread_new(NETIF_IN_THREAD_NAME, ethernetif_input, netif, NETIF_IN_THREAD_STACK_SIZE, NETIF_IN_THREAD_PRIORITY);

    /* Do whatever else is needed to initialize interface. */
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    static sys_sem_t ousem = {0};

    if (ousem.sem == NULL) {
        sys_sem_new(&ousem, 0);
        sys_sem_signal(&ousem);
    }

    err_t errval;

    uint32_t packet;
    int len;

    if (((g_DeviceGmacDefaultConfig.gdma->Register5 >> 20) & 0x7) == 0x6) {
        g_DeviceGmacDefaultConfig.gdma->Register1 = GMAC_ENABLED;
    }

    len = HW_GMAC_GetSendPacket(g_DeviceGmacDefaultConfig.gmac, &packet);

    if (len < 0) {
        errval = ERR_USE;
        goto __low_level_output_error;
    }

    sys_sem_wait(&ousem);

    uint32_t bufferoffset = 0;
    uint32_t framelength = 0;

    for (struct pbuf *q = p; q != NULL; q = q->next) {
        memcpy((void *)(packet + bufferoffset), (void *)(q->payload), q->len);
        cache_flush((uint32_t *)(packet + bufferoffset), q->len);

        bufferoffset += q->len;
        framelength += q->len;
    }

    HW_GMAC_StartSendPacket(g_DeviceGmacDefaultConfig.gmac, framelength);

    errval = ERR_OK;

__low_level_output_error:

    if (g_DeviceGmacDefaultConfig.gdma->Register5 & BIT(5)) { // ETH_DMASR_TUS
        g_DeviceGmacDefaultConfig.gdma->Register5 = BIT(5);
        g_DeviceGmacDefaultConfig.gdma->Register1 = 0;
    }

    sys_sem_signal(&ousem);

    return errval;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
    struct pbuf *p, *q;

    int len;
    len = HW_GMAC_GetRecvPacket(g_DeviceGmacDefaultConfig.gmac, &g_DeviceGmacDefaultConfig);

    if (len == 0) {
        return NULL;
    }

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    q = p;

    if (p != NULL) {
        uint32_t payload_offset = 0;

        uint32_t buffer_offset = 0;
        uint32_t buffer_index = 0;

        uint32_t total_index = 0;

        while (total_index < g_DeviceGmacDefaultConfig.rx_buffer_total_len) {
            uint32_t src_remaining = g_DeviceGmacDefaultConfig.rx_buffer_len[buffer_index] - buffer_offset;
            uint32_t dst_remaining = q->len - payload_offset;

            uint32_t copy_size = (src_remaining < dst_remaining) ? src_remaining : dst_remaining;

            cache_invalid((uint32_t *)(g_DeviceGmacDefaultConfig.rx_buffer_list[buffer_index] + buffer_offset), copy_size);
            memcpy((char *)(q->payload + payload_offset),
                   (char *)(g_DeviceGmacDefaultConfig.rx_buffer_list[buffer_index] + buffer_offset),
                   copy_size);

            payload_offset += copy_size;
            buffer_offset += copy_size;

            total_index += copy_size;

            if (buffer_offset == g_DeviceGmacDefaultConfig.rx_buffer_len[buffer_index]) {
                buffer_index++;
                buffer_offset = 0;
            }

            if (payload_offset == q->len) {
                q = q->next;
                payload_offset = 0;
            }
        }
    }

    HW_GMAC_StopRecvPacket(g_DeviceGmacDefaultConfig.gmac, &g_DeviceGmacDefaultConfig);

    if (g_DeviceGmacDefaultConfig.gdma->Register5 & BIT(7)) { // ETH_DMASR_RBUS
        g_DeviceGmacDefaultConfig.gdma->Register5 = BIT(7);
        g_DeviceGmacDefaultConfig.gdma->Register2 = 0;
    }

    return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(void *pvParameters)
{
    struct netif *netif = (struct netif *)pvParameters;
    struct pbuf *p = NULL;

    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));

    while (1) {
#ifdef LWIP_FREERTOS_WITH_INTERRUPT
        if (xSemaphoreTake(g_eth_semaphore, portMAX_DELAY) == pdTRUE)
#endif
        {
            taskENTER_CRITICAL();
TRY_GET_NEXT_FRAGMENT:
            p = low_level_input(netif);
            taskEXIT_CRITICAL();

            if (p != NULL) {
                taskENTER_CRITICAL();
                if (netif->input(p, netif) != ERR_OK) {
                    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                    pbuf_free(p);
                    p = NULL;
                } else {
#ifdef LWIP_FREERTOS_WITH_INTERRUPT
                    xSemaphoreTake(g_eth_semaphore, 0);
#endif
                    goto TRY_GET_NEXT_FRAGMENT;
                }
                taskEXIT_CRITICAL();
            }

#ifndef LWIP_FREERTOS_WITH_INTERRUPT
            vTaskDelay(1);
#endif
        }
    }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
    struct ethernetif *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    ethernetif = mem_malloc(sizeof(struct ethernetif));
    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
#if LWIP_IPV4
    netif->output = etharp_output;
#endif /* LWIP_IPV4 */
    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);
    ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    return ERR_OK;
}
