/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_common.h"
#include "drv_gmac.h"
#include "drv_gmac_phy.h"
#include "drv_gmac_crc.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

#include "lwipopts.h"

#ifdef LWIP_FREERTOS_WITH_INTERRUPT
extern SemaphoreHandle_t g_eth_semaphore;
#endif

uint8_t g_gmac_tx_flag[NUM_OF_GMAC] = {0};
uint8_t g_gmac_rx_flag[NUM_OF_GMAC] = {0};

#ifdef DEBUG // @@
// Note:
//  This is only a group of parameters for one GMAC
//  You must mount them to the 'DeviceGmacConfig_t' before initialize
//  And, the descriptor must aligned on 256 Byte, buffer must aligned on 1024 Byte

#ifndef FW_GMAC_DDR
static GmacDes_t _s_tx_descriptor[GMAC_DESCRIPTOR_NUM] __attribute__((aligned(256)));
static GmacDes_t _s_rx_descriptor[GMAC_DESCRIPTOR_NUM] __attribute__((aligned(256)));

static uint8_t _s_tx_buffer[GMAC_DESCRIPTOR_NUM][GMAC_BUFFER_SIZE] __attribute__((aligned(1024)));
static uint8_t _s_rx_buffer[GMAC_DESCRIPTOR_NUM][GMAC_BUFFER_SIZE] __attribute__((aligned(1024)));
#endif
#endif // #ifdef DEBUG

static GmacDes_t *_s_last_tx_descriptor = NULL;
static GmacDes_t *_s_last_rx_descriptor = NULL;

void GMAC_IRQHandler(void)
{
    GDMA_TypeDef *gdma = GDMA0;

    uint32_t rx_flag = 0;
    uint32_t tx_flag = 0;

    uint32_t status = gdma->Register5;

    gdma->Register5 = 0xffffffff;

    if (status & BIT(6)) {
        rx_flag = 1;
    }
    if (status & BIT(0)) {
        tx_flag = 1;
    }

#ifdef LWIP_FREERTOS_WITH_INTERRUPT
    if (rx_flag > 0) {
        rx_flag = 0;

        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(g_eth_semaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
#endif

    UNUSED(rx_flag);
    UNUSED(tx_flag);
}

/**
 * @brief   Default configuration for GMAC controller
 * @details This structure configures the basic operating parameters for the GMAC
 *          (Gigabit Media Access Controller) including mode, speed, PHY interface,
 *          descriptors, and buffers for network communication.
 *
 * Configuration fields:
 * @note    - mode: Sets GMAC operation mode (eGmacMode_TxRx for both transmit and receive)
 *          - speed: Sets GMAC speed (100 Mbps)
 *          - phy_id: Physical layer device identifier using predefined PHY_ID
 *          - mac_num: Number of MAC addresses to configure (set to 1)
 *          - mac[0].value: MAC address using predefined GMAC_DEFAULT_MAC_VALUE
 *          - tx_desc_num/rx_desc_num: Number of transmit/receive descriptors
 *          - tx_desc/rx_desc: Pointers to descriptor arrays
 *          - tx_buffer_size/rx_buffer_size: Size of transmit/receive buffers
 *          - tx_buffer/rx_buffer: Pointers to data buffers
 *          - loopback: Enables loopback mode based on DEBUG_GMAC_LOOPBACK
 *          - user_data: Additional user-defined data (initialized to 0)
 *
 * @warning This configuration is primarily for debug purposes and should be
 *          modified for production use based on specific hardware requirements
 */
#ifdef DEBUG
DeviceGmacConfig_t g_DeviceGmacDefaultConfig = {
    .mode = eGmacMode_TxRx,
    .speed = eGmacSpeed_100M,

    .phy_id = PHY_ID,

    .mac_num = 1,
    .mac[0].value = GMAC_DEFAULT_MAC_VALUE,

    .tx_desc_num = GMAC_DESCRIPTOR_NUM,
    .rx_desc_num = GMAC_DESCRIPTOR_NUM,
#ifndef FW_GMAC_DDR
    .tx_desc = _s_tx_descriptor,
    .rx_desc = _s_rx_descriptor,
#else
    .tx_desc = (GmacDes_t *)(FW_DDR_GMAC_DESCRIPTOR_OFFSET + sizeof(GmacDes_t) * 0),
    .rx_desc = (GmacDes_t *)(FW_DDR_GMAC_DESCRIPTOR_OFFSET + sizeof(GmacDes_t) * GMAC_DESCRIPTOR_NUM),
#endif

    .tx_buffer_size = GMAC_BUFFER_SIZE,
    .rx_buffer_size = GMAC_BUFFER_SIZE,
#ifndef FW_GMAC_DDR
    .tx_buffer = (uint8_t *)_s_tx_buffer,
    .rx_buffer = (uint8_t *)_s_rx_buffer,
#else
    .tx_buffer = (uint8_t *)(FW_DDR_GMAC_BUFFER_OFFSET + GMAC_BUFFER_SIZE * 0),
    .rx_buffer = (uint8_t *)(FW_DDR_GMAC_BUFFER_OFFSET + GMAC_BUFFER_SIZE * GMAC_DESCRIPTOR_NUM),
#endif

#if (DEBUG_GMAC_LOOPBACK != 0)
    .loopback = 1,
#else
    .loopback = 0,
#endif

    .user_data = 0,
};
#endif // #ifdef DEBUG

void ext_cfg(GmacSpeed_e speed)
{
#define GMAC_DS_CFG (0xeb009000 + 0x108)

    for (int i = 0; i < 16; i++) {
        STW(i | (i << 4) | (i << 8) | (i << 12), GMAC_DS_CFG);
        MSYNC;
    }

    if (speed == eGmacSpeed_1000M) {
        STW(0xf, EMAC_CLK_SEL_CFG); // select the 125Mhz clock
        STW(0x0, 0xeb009118);       // ccm dpaa ctl
    } else {
        STW(0x0, EMAC_CLK_SEL_CFG); // select the 125Mhz clock
        STW(0x3, 0xeb009118);       // ccm dpaa ctl
    }

    STW((0x000 | 0x0), 0xea4820a8);
    STW((0x000 | 0x0), 0xea4824a8);
    STW((0x000 | 0x0), 0xea4828a8);
    STW((0x000 | 0x0), 0xea482ca8);
    STW((0x000 | 0x0), 0xea4840a8);

    STW(FW_GMAC_ADDR_START_HIGH, 0xea4820ac);
    STW(FW_GMAC_ADDR_START_HIGH, 0xea4824ac);
    STW(FW_GMAC_ADDR_START_HIGH, 0xea4828ac);
    STW(FW_GMAC_ADDR_START_HIGH, 0xea482cac);
    STW(FW_GMAC_ADDR_START_HIGH, 0xea4840ac);

#undef GMAC_DS_CFG
}

uint32_t swap_data(uint32_t data)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    return data;
#else
    return ((data & 0xff000000) >> 24) | ((data & 0x00ff0000) >> 8) | ((data & 0x0000ff00) << 8) | ((data & 0x000000ff) << 24);
#endif
}

void swap_desc(GmacDes_t *desc, uint32_t num)
{
    for (uint32_t *p = (uint32_t *)&desc[0]; p < (uint32_t *)&desc[num]; p++) {
        *p = swap_data(*p);
    }
}

void load_desc(GmacDes_t *out, GmacDes_t *in)
{
    for (uint32_t i = 0; i < 4; i++) {
        ((uint32_t *)out)[i] = swap_data(((uint32_t *)in)[i]);
    }
}

void push_desc(GmacDes_t *out, GmacDes_t *in)
{
    for (uint32_t i = 0; i < 4; i++) {
        ((uint32_t *)out)[i] = swap_data(((uint32_t *)in)[i]);
    }
}

/**
 * @brief   Initializes the GMAC device configuration
 * @param   GMACx: GMAC device base address
 * @param   config: GMAC device configuration structure pointer
 * @return  0 on success, -1 on failure
 *
 * @details This function sets the GMAC mode, speed, MAC address, descriptors, and
 *          other parameters based on the provided configuration structure.
 */
int HW_GMAC_InitConfig(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == GMACx) || (NULL == config)) {
        return -1;
    }

    config->gmac = GMACx;
    config->gdma = GET_GDMA_HANDLE(GMACx);

    switch ((uint32_t)GMACx) {
    case (uint32_t)GMAC0:
        config->phy_id = 5;
        break;
    default:
        break;
    }

    GDMA_TypeDef *GDMAx = config->gdma;

    ext_cfg(config->speed);

    if (config->loopback) {
        DEBUG_ASSERT(config->mode == eGmacMode_TxRx);
        config->mode = eGmacMode_TxRx;
    }

    HW_GMAC_Reset(GMACx, config, 10 * 1000);

    {
        GDMAx->Register0 = 0x03010100;

        HW_GMAC_InitTxDescriptor(config->tx_desc, config->tx_desc_num, config->tx_buffer, config->tx_buffer_size);
        HW_GMAC_InitRxDescriptor(config->rx_desc, config->rx_desc_num, config->rx_buffer, config->rx_buffer_size);

        _s_last_tx_descriptor = config->tx_desc;
        _s_last_rx_descriptor = config->rx_desc;

        GDMAx->Register3 = (uint32_t)(config->rx_desc);
        GDMAx->Register4 = (uint32_t)(config->tx_desc);

        reg_value = 0; // .Register6 inited to zero first
        SET_BIT(reg_value, GDMA_OMR_TSF_POS, GMAC_ENABLED);
        SET_BIT(reg_value, GDMA_OMR_FEF_POS, GMAC_ENABLED);
        reg_value |= (TTC_BIT_128 << GDMA_OMR_TTC_POS);
        reg_value |= (RTC_BIT_128 << GDMA_OMR_RTC_POS);
        GDMAx->Register6 = reg_value;
    }

    {
        reg_value = 0; // .Register0 inited to zero first
        SET_BIT(reg_value, GMAC_MCR_DM_POS, GMAC_ENABLED);
        SET_BIT(reg_value, GMAC_MCR_DR_POS, GMAC_ENABLED);
        SET_BIT(reg_value, GMAC_MCR_TE_POS, GMAC_ENABLED);
        SET_BIT(reg_value, GMAC_MCR_RE_POS, GMAC_ENABLED);
        reg_value |= (config->speed << GMAC_MCR_FES_POS);
        GMACx->Register0 = reg_value;

        reg_value = GMACx->Register0;
        mdelay(10);
        GMACx->Register0 = reg_value;

        HW_GMAC_SetMAC(GMACx, config);

        reg_value = 0; // .Register6 inited to zero first
        SET_BIT(reg_value, GMAC_FCR_RFE_POS, GMAC_ENABLED);
        SET_BIT(reg_value, GMAC_FCR_TFE_POS, GMAC_ENABLED);
        reg_value |= (0x200 << GMAC_FCR_PT_POS);
        GMACx->Register6 = reg_value;
    }

    Reg_Write_32((uint32_t)(GMACx) + 0x10c, ~0);
    Reg_Write_32((uint32_t)(GMACx) + 0x110, ~0);

    GDMAx->Register5 = 0xffffffff; // NOTE: clear all interrupt
#ifdef LWIP_FREERTOS_WITH_INTERRUPT
    GMACx->Register15 = BIT(0); // NOTE: enable corresponding interrupt
    GDMAx->Register7 = (BIT(16) | BIT(6));
#else
    GDMAx->Register7 = 0;
#endif

#ifdef LWIP_FREERTOS_WITH_INTERRUPT
    InstallInterruptHandler0(120, GMAC_IRQHandler);
    REG_WRITE_32(0xe8050200 + 120 * 0x20, REG_READ_32(0xe8050200 + 120 * 0x20) & 0x7fffffff);
    InstallInterruptHandler0(121, GMAC_IRQHandler);
    REG_WRITE_32(0xe8050200 + 121 * 0x20, REG_READ_32(0xe8050200 + 121 * 0x20) & 0x7fffffff);
    InstallInterruptHandler0(122, GMAC_IRQHandler);
    REG_WRITE_32(0xe8050200 + 122 * 0x20, REG_READ_32(0xe8050200 + 122 * 0x20) & 0x7fffffff);
    InstallInterruptHandler0(123, GMAC_IRQHandler);
    REG_WRITE_32(0xe8050200 + 123 * 0x20, REG_READ_32(0xe8050200 + 123 * 0x20) & 0x7fffffff);
    InstallInterruptHandler0(124, GMAC_IRQHandler);
    REG_WRITE_32(0xe8050200 + 124 * 0x20, REG_READ_32(0xe8050200 + 124 * 0x20) & 0x7fffffff);
#endif

    reg_value = GDMAx->Register6;
    switch (config->mode) {
    case eGmacMode_TxRx:
        SET_BIT(reg_value, GDMA_OMR_ST_POS, GMAC_ENABLED);
        SET_BIT(reg_value, GDMA_OMR_SR_POS, GMAC_ENABLED);
        break;
    case eGmacMode_TxOnly:
        SET_BIT(reg_value, GDMA_OMR_ST_POS, GMAC_ENABLED);
        SET_BIT(reg_value, GDMA_OMR_SR_POS, GMAC_DISABLED);
        break;
    case eGmacMode_RxOnly:
        SET_BIT(reg_value, GDMA_OMR_ST_POS, GMAC_DISABLED);
        SET_BIT(reg_value, GDMA_OMR_SR_POS, GMAC_ENABLED);
        break;
    default:
        SET_BIT(reg_value, GDMA_OMR_ST_POS, GMAC_DISABLED);
        SET_BIT(reg_value, GDMA_OMR_SR_POS, GMAC_DISABLED);
        break;
    }
    GDMAx->Register6 = reg_value;

    PHY_InitConfig(GMACx, config);

    config->status = 0;
    return 0;
}

/**
 * @brief   Resets the GMAC device
 * @param   GMACx: GMAC device base address
 * @param   config: GMAC device configuration structure pointer
 * @param   timeout: Timeout value
 * @return  0 on success, -1 on failure
 *
 * @details This function resets the GMAC device by setting the reset bit and
 *          waiting for the reset to complete within the specified timeout.
 */
int HW_GMAC_Reset(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config, uint32_t timeout)
{
    GDMA_TypeDef *GDMAx = GET_GDMA_HANDLE(GMACx);

    UNUSED(config);

    SET_BIT(GDMAx->Register0, GDMA_BMR_SWR_POS, GMAC_ENABLED);

    for (uint32_t i = 0; i < timeout; i++) {
        if (!GET_BIT(GDMAx->Register0, GDMA_BMR_SWR_POS)) {
            return 0;
        }
        udelay(1);
    }

    return -1;
}

/**
 * @brief   Sets the GMAC device MAC address
 * @param   GMACx: GMAC device base address
 * @param   config: GMAC device configuration structure pointer
 * @return  0 on success
 *
 * @details This function sets the GMAC device MAC address using the provided
 *          configuration structure.
 */
int HW_GMAC_SetMAC(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    __IO uint32_t *reg_addr = &(GMACx->Register16);

    for (uint32_t i = 0; i < config->mac_num; i++) {
        reg_addr[2 * i + 0] = BYTEARRAY2DWORD(0, 0, config->mac[i].value[5], config->mac[i].value[4]);
        reg_addr[2 * i + 1] = BYTEARRAY2DWORD(config->mac[i].value[3], config->mac[i].value[2], config->mac[i].value[1], config->mac[i].value[0]);
    }

#ifdef DEBUG
    // SET_BIT(GMACx->Register1, GMAC_MFF_RA_POS, GMAC_ENABLED); // NOTE: receive all frame and pass to up-level module
#endif

    return 0;
}

/**
 * @brief   Initializes the GMAC device transmit descriptor
 * @param   descriptor: Transmit descriptor array
 * @param   num: Number of descriptors
 * @param   pbuf: Data buffer
 * @param   len: Buffer length
 *
 * @details This function initializes the transmit descriptor and sets the data
 *          buffer and descriptor chain.
 */
void HW_GMAC_InitTxDescriptor(GmacDes_t *descriptor, uint32_t num, uint8_t *pbuf, uint32_t len)
{
    GmacDes_t *base_descriptor = descriptor;
    GmacDes_t *current_descriptor = descriptor;

    for (uint32_t i = 0; i < num; i++) {
        memset(current_descriptor, 0x00, sizeof(GmacDes_t));

        current_descriptor->tx.IC = 0;
        current_descriptor->tx.DC = 0;
        current_descriptor->tx.TCH = 1;

        current_descriptor->buffer1_addr = (uint32_t)&pbuf[i * len];
        current_descriptor->next = (uint32_t)(current_descriptor + 1);

        current_descriptor++;
    }

    if (num > 0) {
        base_descriptor[num - 1].next = (uint32_t)base_descriptor;
    }

    swap_desc(descriptor, num);

    cache_flush(descriptor, sizeof(GmacDes_t) * num);
}

/**
 * @brief   Initializes the GMAC device receive descriptor
 * @param   descriptor: Receive descriptor array
 * @param   num: Number of descriptors
 * @param   pbuf: Data buffer
 * @param   len: Buffer length
 *
 * @details This function initializes the receive descriptor and sets the data
 *          buffer and descriptor chain.
 */
void HW_GMAC_InitRxDescriptor(GmacDes_t *descriptor, uint32_t num, uint8_t *pbuf, uint32_t len)
{
    GmacDes_t *base_descriptor = descriptor;
    GmacDes_t *current_descriptor = descriptor;

    for (uint32_t i = 0; i < num; i++) {
        memset(current_descriptor, 0x00, sizeof(GmacDes_t));

        current_descriptor->owner = 1;
        current_descriptor->status = 0x00400300;

#ifdef LWIP_FREERTOS_WITH_INTERRUPT
        current_descriptor->rx.DIC = 0;
#else
        current_descriptor->rx.DIC = 1;
#endif
        current_descriptor->rx.RCH = 1;
        current_descriptor->rx.buffer1_size = len;
        current_descriptor->rx.buffer2_size = len;

        current_descriptor->buffer1_addr = (uint32_t)&pbuf[i * len];
        current_descriptor->next = (uint32_t)(current_descriptor + 1);

        current_descriptor++;
    }

    if (num > 0) {
        base_descriptor[num - 1].next = (uint32_t)base_descriptor;
    }

    swap_desc(descriptor, num);

    cache_flush(descriptor, sizeof(GmacDes_t) * num);
}

#ifdef DEBUG_GMAC

/**
 * @brief   Generates the GMAC device transmit descriptor
 * @param   descriptor: Transmit descriptor pointer
 * @param   pbuf: Data buffer
 * @param   len: Buffer length
 *
 * @details This function generates the transmit descriptor based on the data
 *          buffer and length.
 */
void HW_GMAC_GenTxDescriptor(GmacDes_t *descriptor, uint8_t *pbuf, uint32_t len)
{
    cache_invalid(descriptor, sizeof(GmacDes_t));
    {
        memcpy((uint8_t *)descriptor->buffer1_addr, pbuf, len);
        cache_flush((void *)descriptor->buffer1_addr, len);

        descriptor->tx.buffer1_size = len;
        descriptor->owner = 1;
    }
    cache_flush(descriptor, sizeof(GmacDes_t));
}

/**
 * @brief   Generates the GMAC device packet
 * @param   pbuf: Packet buffer
 * @param   target_mac: Target MAC address
 * @param   source_mac: Source MAC address
 * @param   type: Packet type
 * @param   pdata: Packet data
 * @param   len: Packet data length
 * @return  Packet length
 *
 * @details This function generates an Ethernet packet with the specified target
 *          MAC address, source MAC address, packet type, and data.
 */
uint32_t HW_GMAC_GenPacket(uint8_t *pbuf, uint8_t *target_mac, uint8_t *source_mac, uint16_t type, uint8_t *pdata, uint32_t len)
{
    uint8_t *pkt = pbuf;

    memcpy(pkt, target_mac, 6);
    pkt += 6;

    memcpy(pkt, source_mac, 6);
    pkt += 6;

    if (type > 1536) {
        *pkt++ = (type >> 8) & 0xff;
        *pkt++ = (type)&0xff;
    } else {
        *pkt++ = (len >> 8) & 0xff;
        *pkt++ = (len)&0xff;
    }

    memcpy(pkt, pdata, len);
    pkt += len;

    uint32_t crc = CRC_Calc(pkt, (pkt - pbuf));
    *pkt++ = (crc)&0xff;
    *pkt++ = (crc >> 8) & 0xff;
    *pkt++ = (crc >> 16) & 0xff;
    *pkt++ = (crc >> 24) & 0xff;

    return (pkt - pbuf);
}

/**
 * @brief   Sends the GMAC device packet
 * @param   GMACx: GMAC device base address
 * @param   config: GMAC device configuration structure pointer
 * @param   pbuf: Packet buffer
 * @param   len: Packet length
 * @return  0 on success, -1 on failure
 *
 * @details This function sends the packet using the provided configuration and
 *          packet buffer.
 */
int HW_GMAC_SendPacket(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config, uint8_t *pbuf, uint32_t len)
{
    GDMA_TypeDef *GDMAx = GET_GDMA_HANDLE(GMACx);
    GmacDes_t current_descriptor;

    cache_invalid(_s_last_tx_descriptor, sizeof(GmacDes_t));
    load_desc(&current_descriptor, _s_last_tx_descriptor);

    if (current_descriptor.owner) {
        return -1;
    }

    uint32_t packet_num = ((len + config->tx_buffer_size - 1) / config->tx_buffer_size);

    for (uint32_t i = 0; i < packet_num; i++) {
        uint32_t packet_len;

        if (i == (packet_num - 1)) {
            packet_len = len;
        } else {
            packet_len = config->tx_buffer_size;
        }

        HW_GMAC_GenTxDescriptor(&current_descriptor, pbuf, packet_len);
        pbuf += packet_len;
        len -= packet_len;

        current_descriptor.tx.FS = 0;
        current_descriptor.tx.LS = 0;

        if (i == 0) {
            current_descriptor.tx.FS = 1;
        }
        if (i == (packet_num - 1)) {
            current_descriptor.tx.LS = 1;
        }

        push_desc(_s_last_tx_descriptor, &current_descriptor);
        cache_flush(_s_last_tx_descriptor, sizeof(GmacDes_t));

        _s_last_tx_descriptor = (GmacDes_t *)(current_descriptor.next);

        cache_invalid(_s_last_tx_descriptor, sizeof(GmacDes_t));
        load_desc(&current_descriptor, _s_last_tx_descriptor);
    }

    GDMAx->Register1 = 0;

    return 0;
}

#endif // #ifdef DEBUG_GMAC

/**
 * @brief   Sends/Receives the GMAC device packet
 * @param   GMACx: GMAC device base address
 * @param   pbuf: Packet buffer
 * @param   len: Buffer length
 * @return  Received packet length
 *
 * @details This function sends/receives the packet using the provided configuration and
 *          packet buffer.
 */
int HW_GMAC_GetSendPacket(GMAC_TypeDef *GMACx, uint32_t *pbuf)
{
    GmacDes_t current_descriptor;

    cache_invalid(_s_last_tx_descriptor, sizeof(GmacDes_t));
    load_desc(&current_descriptor, _s_last_tx_descriptor);

    if (current_descriptor.owner) {
        return -1;
    }

    if (pbuf) {
        pbuf[0] = current_descriptor.buffer1_addr;
    }

    return 0;
}

int HW_GMAC_StartSendPacket(GMAC_TypeDef *GMACx, uint32_t len)
{
    GmacDes_t current_descriptor;

    cache_invalid(_s_last_tx_descriptor, sizeof(GmacDes_t));
    load_desc(&current_descriptor, _s_last_tx_descriptor);

    current_descriptor.tx.buffer1_size = len;
    current_descriptor.owner = 1;

    current_descriptor.tx.CIC = 0x3;

    current_descriptor.tx.FS = 1;
    current_descriptor.tx.LS = 1;

    push_desc(_s_last_tx_descriptor, &current_descriptor);
    cache_flush(_s_last_tx_descriptor, sizeof(GmacDes_t));

    _s_last_tx_descriptor = (GmacDes_t *)(current_descriptor.next);

    if (GET_GDMA_HANDLE(GMACx)->Register5 & BIT(2)) { // ETH_DMASR_TUS
        GET_GDMA_HANDLE(GMACx)->Register5 = BIT(2);
        GET_GDMA_HANDLE(GMACx)->Register1 = 0;
    }

    return 0;
}

int HW_GMAC_GetRecvPacket(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    GmacDes_t current_descriptor;

    cache_invalid(_s_last_rx_descriptor, sizeof(GmacDes_t));
    load_desc(&current_descriptor, _s_last_rx_descriptor);

    if (current_descriptor.owner) {
        return 0;
    }

    int len;
    len = 0;

    if (current_descriptor.status & BIT(9)) { // FS
        config->rx_desc_start = _s_last_rx_descriptor;
        config->rx_buffer_total_len = 0;
        config->rx_buffer_count = 0;
    }

    config->rx_buffer_list[config->rx_buffer_count] = current_descriptor.buffer1_addr;
    config->rx_buffer_len[config->rx_buffer_count] = GMAC_GET_RX_LENGTH(current_descriptor.status);
    config->rx_buffer_count++;

    config->rx_buffer_total_len += GMAC_GET_RX_LENGTH(current_descriptor.status);

    if (current_descriptor.status & BIT(8)) { // LS
        len = config->rx_buffer_total_len;
    }

    _s_last_rx_descriptor = (GmacDes_t *)(current_descriptor.next);

    return len;
}

int HW_GMAC_StopRecvPacket(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    GmacDes_t current_descriptor;
    GmacDes_t *next_descriptor = config->rx_desc_start;

    for (uint32_t i = 0; i < config->rx_buffer_count; i++) {
        cache_invalid(next_descriptor, sizeof(GmacDes_t));
        load_desc(&current_descriptor, next_descriptor);

        current_descriptor.owner = 1;
        current_descriptor.status = 0x00400300;

        push_desc(next_descriptor, &current_descriptor);
        cache_flush(next_descriptor, sizeof(GmacDes_t));

        next_descriptor = (GmacDes_t *)current_descriptor.next;
    }

    return 0;
}
