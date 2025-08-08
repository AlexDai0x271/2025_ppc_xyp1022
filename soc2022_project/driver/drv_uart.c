/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_uart.h"
#if (DEBUG_UART_CASE == DEBUG_UART_DMA)
#include "drv_dma.h"
#endif
#include "sysclock.h"

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceUartConfig_t g_DeviceUartDefaultConfig =
    {
        .data_width = eUartData_Bit_8,
        .stop = eUartStop_Bit_1,
        .parity = eUartParity_None,
        .baud = 38400,

        .dma = 0,
        .irq = eUartIrq_None,
        .rs485 = 0,

        .int_enable = 0,
        .int_irqn = UART0_IRQn,

        .user_data = 0,
};
#endif // #ifdef DEBUG

// Note:
//  1. UART0/1、UART11/12 (total 4)
//      a. support RS485
//      b. support auto-stream-control mode
//      c. support DMA
//      d. TX_FIFO: 1024*9
//         RX_FIFO: 1024*11
//  2. UART2、UART13 (total 2)
//      a. support DMA
//      b. TX_FIFO: 256*9
//         RX_FIFO: 256*11
//  3. UART3-10、UART14-21 (total 16)
//      a. TX_FIFO: 256*9
//         RX_FIFO: 256*11

int HW_UART_InitConfig(UART_TypeDef *UARTx, DeviceUartConfig_t *config)
{
    uint32_t uart_div;
    uint32_t reg_value;

    if ((NULL == UARTx) || (NULL == config)) {
        return -1;
    }

    uart_div = drv_get_apb_freq() / (16 * config->baud) - 1;

    // config: baudrate N 8 1

    reg_value = 0; // .MCR inited to zero first
    SET_BIT(reg_value, UART_MCR_LOOPBACK_POS, 0);
    SET_BIT(reg_value, UART_MCR_OUT_2_POS, 0);
    SET_BIT(reg_value, UART_MCR_OUT_1_POS, 0);
    SET_BIT(reg_value, UART_MCR_RTS_POS, 0);
    SET_BIT(reg_value, UART_MCR_DTR_POS, 0);
    UARTx->MCR = reg_value;

    SET_BIT(UARTx->LCR, UART_LCR_DLAB_POS, 1);
    UARTx->DLH = (uart_div >> 8) & 0xFF;
    UARTx->DLL = (uart_div)&0xFF;
    SET_BIT(UARTx->LCR, UART_LCR_DLAB_POS, 0);

    reg_value = 0; // .MCR inited to zero first
    reg_value |= ((config->data_width & 0x3) << UART_LCR_DLS_POS);
    if (eUartStop_Bit_1 == config->stop) {
        SET_BIT(reg_value, UART_LCR_STOP_POS, STOP_1BIT);
    } else {
        SET_BIT(reg_value, UART_LCR_STOP_POS, STOP_1_5BIT_OR_2BIT);
    }
    if (eUartParity_None == config->parity) {
        SET_BIT(reg_value, UART_LCR_PEN_POS, 0);
        SET_BIT(reg_value, UART_LCR_EPS_POS, 0);
    } else {
        SET_BIT(reg_value, UART_LCR_PEN_POS, 1);
        SET_BIT(reg_value, UART_LCR_EPS_POS, (config->parity - 1));
    }
    SET_BIT(reg_value, UART_LCR_SP_POS, 0);
    SET_BIT(reg_value, UART_LCR_BC_POS, 0);
    UARTx->LCR = reg_value;

    reg_value = 0; // .FCR is a WO register
    SET_BIT(reg_value, UART_FCR_FIFOE_POS, 1);
    SET_BIT(reg_value, UART_FCR_RFIFOR_POS, 1); // SW => Transmit FIFO => TX
    SET_BIT(reg_value, UART_FCR_XFIFOR_POS, 1); // SW <= Receive FIFO <= RX
    SET_BIT(reg_value, UART_FCR_DMAM_POS, config->dma);
    if ((config->irq & UART_IRQ_TYPE_MASK) == UART_IRQ_TYPE_TX) {
        reg_value |= ((config->irq & UART_IRQ_SUBTYPE_MASK) << UART_FCR_TET_POS);
    }
    if ((config->irq & UART_IRQ_TYPE_MASK) == UART_IRQ_TYPE_RX) {
        reg_value |= ((config->irq & UART_IRQ_SUBTYPE_MASK) << UART_FCR_RT_POS);
    }
    UARTx->FCR = reg_value;

    reg_value = UARTx->IER;
    SET_BIT(reg_value, UART_IER_PTIME_POS, 0); // NOTE: enable when finish fill data to fifo
    SET_BIT(reg_value, UART_IER_ELCOLR_POS, 0);
    SET_BIT(reg_value, UART_IER_EDSSI_POS, 0);
    SET_BIT(reg_value, UART_IER_ELSI_POS, 0);
    SET_BIT(reg_value, UART_IER_ETBEI_POS, 0); // NOTE: enable when finish fill data to fifo
    SET_BIT(reg_value, UART_IER_ERBFI_POS, 0);
    UARTx->IER = reg_value;

    reg_value = UARTx->IIR;
    UNUSED(reg_value);

    if (config->int_enable) {
        drv_irq_enable(config->int_irqn);
    }

    reg_value = 0; // .TCR inited to zero first
    if (config->rs485) {
        reg_value |= (XFER_MODE_MODE0 << UART_TCR_XFER_MODE_POS);
        SET_BIT(reg_value, UART_TCR_DE_POL_POS, DE_POL_HIGH);
        SET_BIT(reg_value, UART_TCR_RE_POL_POS, RE_POL_LOW);
        SET_BIT(reg_value, UART_TCR_RS485_EN_POS, UART_ENABLED);
    }
    UARTx->TCR = reg_value;

    config->status = 0;
    return 0;
}

void HW_UART_WriteEnable(UART_TypeDef *UARTx)
{
    UARTx->RE_EN = UART_DISABLED;
    UARTx->DE_EN = UART_ENABLED;
}

void HW_UART_ReadEnable(UART_TypeDef *UARTx)
{
    UARTx->DE_EN = UART_DISABLED;
    UARTx->RE_EN = UART_ENABLED;
}

uint8_t HW_UART_Write(UART_TypeDef *UARTx, uint8_t value)
{
    while (!GET_BIT(UARTx->LSR, UART_LSR_TEMT_POS))
        ;

    UARTx->THR = value;
    while (!GET_BIT(UARTx->LSR, UART_LSR_TEMT_POS))
        ;

    return value;
}

uint32_t HW_UART_WriteBlock(UART_TypeDef *UARTx, uint8_t *txbuf, uint32_t length)
{
    uint32_t depth;

    if (!txbuf) {
        return 0;
    }

    depth = HW_UART_GetFIFODepth(UARTx);

    length = (length > depth) ? depth : length;

    for (uint32_t i = 0; i < length; i++) {
        while (!GET_BIT(UARTx->USR, UART_USR_TFNF_POS))
            ;
        UARTx->THR = txbuf[i];
    }

    HW_UART_EnableTHRE(UARTx);
    HW_UART_EnableSendInterrupt(UARTx);

    return length;
}

uint8_t HW_UART_Read(UART_TypeDef *UARTx)
{
    uint8_t value;

    while (!GET_BIT(UARTx->LSR, UART_LSR_DR_POS))
        ;

    value = UARTx->RBR;
    return value;
}

uint32_t HW_UART_GetFIFODepth(UART_TypeDef *UARTx)
{
    uint32_t depth = 0;

    switch ((uint32_t)UARTx) {
    case (uint32_t)UART0:
        depth = 1024;
        break;
    case (uint32_t)UART1:
        depth = 1024;
        break;
    case (uint32_t)UART2:
        depth = 256;
        break;
    case (uint32_t)UART3:
        depth = 256;
        break;
    case (uint32_t)UART4:
        depth = 256;
        break;
    case (uint32_t)UART5:
        depth = 256;
        break;
    case (uint32_t)UART6:
        depth = 256;
        break;
    case (uint32_t)UART7:
        depth = 256;
        break;
    case (uint32_t)UART8:
        depth = 256;
        break;
    case (uint32_t)UART9:
        depth = 256;
        break;
    case (uint32_t)UART10:
        depth = 256;
        break;
    case (uint32_t)UART11:
        depth = 1024;
        break;
    case (uint32_t)UART12:
        depth = 1024;
        break;
    case (uint32_t)UART13:
        depth = 256;
        break;
    case (uint32_t)UART14:
        depth = 256;
        break;
    case (uint32_t)UART15:
        depth = 256;
        break;
    case (uint32_t)UART16:
        depth = 256;
        break;
    case (uint32_t)UART17:
        depth = 256;
        break;
    case (uint32_t)UART18:
        depth = 256;
        break;
    case (uint32_t)UART19:
        depth = 256;
        break;
    case (uint32_t)UART20:
        depth = 256;
        break;
    case (uint32_t)UART21:
        depth = 256;
        break;
    }

    return depth;
}

uint32_t HW_UART_GetCurrentSendFIFOLength(UART_TypeDef *UARTx)
{
    return UARTx->TFL;
}

uint32_t HW_UART_GetCurrentRecvFIFOLength(UART_TypeDef *UARTx)
{
    return UARTx->RFL;
}

uint32_t HW_UART_GetStatus(UART_TypeDef *UARTx)
{
    return UARTx->IIR;
}

void HW_UART_EnableSendInterrupt(UART_TypeDef *UARTx)
{
    UARTx->IER |= (1 << UART_IER_ETBEI_POS);
}

void HW_UART_DisableSendInterrupt(UART_TypeDef *UARTx)
{
    UARTx->IER &= ~(1 << UART_IER_ETBEI_POS);
}

void HW_UART_EnableTHRE(UART_TypeDef *UARTx)
{
    UARTx->IER |= (1 << UART_IER_PTIME_POS);
}

void HW_UART_DisableTHRE(UART_TypeDef *UARTx)
{
    UARTx->IER &= ~(1 << UART_IER_PTIME_POS);
}

#ifdef DEBUG_UART

#include "xprintf.h"

static uint8_t tx_buffer[1000];
static uint32_t tx_length = sizeof(tx_buffer);

int selftest_uart(void)
{
#if ((DEBUG_UART_CASE == DEBUG_UART_NORMAL) || (DEBUG_UART_CASE == DEBUG_UART_RS485))
    UART_TypeDef *uart = UART1;
    IRQn_Type irqn = UART1_IRQn;

    xprintf("UART VERSION = %08x\n", uart->UCV);
    xprintf("UART CPR = %d\n", uart->CPR);

    DeviceUartConfig_t uart_config =
    {
        .data_width = eUartData_Bit_8,
        .stop = eUartStop_Bit_1,
        .parity = eUartParity_None,
        .baud = 9600,

        .dma = 0,
        .irq = eUartIrq_XferChar2,
#if (DEBUG_UART_CASE == DEBUG_UART_RS485)
        .rs485 = 1,
#else
        .rs485 = 0,
#endif

        .int_enable = 1,
        .int_irqn = irqn,

        .user_data = 0,
    };

    for (uint32_t i = 0; i < sizeof(tx_buffer); i++) {
        tx_buffer[i] = i % 256;
    }

    HW_UART_InitConfig(uart, &uart_config);

#if (DEBUG_UART_CASE == DEBUG_UART_RS485)
    HW_UART_WriteEnable(uart);
#endif
    tx_length = HW_UART_WriteBlock(uart, tx_buffer, tx_length);
    xprintf("send %d.\n", tx_length);
#endif // #if ( (DEBUG_UART_CASE == DEBUG_UART_NORMAL) || (DEBUG_UART_CASE == DEBUG_UART_RS485) )

#if (DEBUG_UART_CASE == DEBUG_UART_DMA)
    UART_TypeDef *uart = UART1;
    DMA_TypeDef *dma = DMA0;

    xprintf("UART VERSION = %08x\n", uart->UCV);
    xprintf("UART CPR = %d\n", uart->CPR);

    DeviceUartConfig_t uart_config =
        {
            .data_width = eUartData_Bit_8,
            .stop = eUartStop_Bit_1,
            .parity = eUartParity_None,
            .baud = 9600,

            .dma = 1,
            .irq = eUartIrq_None,
            .rs485 = 0,

            .int_enable = 0,
            .int_irqn = 0,

            .user_data = 0,
        };

    DeviceDmaConfig_t dma_config =
        {
            .mode = eDmaMode_Mem2Dev,
            .channel = eDmaChannel_0,

            .transfer_size = tx_length,
            .priority = 31,

            .src_addr = (uint32_t)&tx_buffer,
            .src_addr_mode = eDmaAddrMode_Inc,
            .src_data_width = eDmaDataWidth_8Bit,
            .src_data_depth = eDmaDataDepth_1,

            .dst_addr = (uint32_t)uart->THR,
            .dst_addr_mode = eDmaAddrMode_Fixed,
            .dst_data_width = eDmaDataWidth_8Bit,
            .dst_data_depth = eDmaDataDepth_1,

            .user_data = 0,
        };

    for (uint32_t i = 0; i < sizeof(tx_buffer); i++) {
        tx_buffer[i] = i % 256;
    }

    HW_UART_InitConfig(uart, &uart_config);

    HW_DMA_InitConfig(dma, &dma_config);
    HW_DMA_ChannelEnable(dma, dma_config.channel);

    while (!HW_DMA_WaitComplete(dma, dma_config.channel))
        ;
    xprintf("send %d.\n", tx_length);
#endif // #if ( DEBUG_UART_CASE == DEBUG_UART_DMA )

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_UART
