/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __HW_UART_H__
#define __HW_UART_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* UART IER */
#define UART_IER_PTIME_POS     (7) // 1-bit
#define UART_IER_ELCOLR_POS    (4) // 1-bit
#define UART_IER_EDSSI_POS     (3) // 1-bit
#define UART_IER_ELSI_POS      (2) // 1-bit
#define UART_IER_ETBEI_POS     (1) // 1-bit
#define UART_IER_ERBFI_POS     (0) // 1-bit

/* UART FCR */
#define UART_FCR_RT_POS        (6) // 2-bit
#define UART_FCR_TET_POS       (4) // 2-bit
#define UART_FCR_DMAM_POS      (3) // 1-bit
#define UART_FCR_XFIFOR_POS    (2) // 1-bit
#define UART_FCR_RFIFOR_POS    (1) // 1-bit
#define UART_FCR_FIFOE_POS     (0) // 1-bit

/* UART LCR */
#define UART_LCR_DLAB_POS      (7) // 1-bit
#define UART_LCR_BC_POS        (6) // 1-bit
#define UART_LCR_SP_POS        (5) // 1-bit
#define UART_LCR_EPS_POS       (4) // 1-bit
#define UART_LCR_PEN_POS       (3) // 1-bit
#define UART_LCR_STOP_POS      (2) // 1-bit
#define UART_LCR_DLS_POS       (0) // 2-bit

/* UART MCR */
#define UART_MCR_SIRE_POS      (6) // 1-bit
#define UART_MCR_AFCE_POS      (5) // 1-bit
#define UART_MCR_LOOPBACK_POS  (4) // 1-bit
#define UART_MCR_OUT_2_POS     (3) // 1-bit
#define UART_MCR_OUT_1_POS     (2) // 1-bit
#define UART_MCR_RTS_POS       (1) // 1-bit
#define UART_MCR_DTR_POS       (0) // 1-bit

/* UART LSR */
#define UART_LSR_ADDR_RCVD_POS (8) // 1-bit
#define UART_LSR_RFE_POS       (7) // 1-bit
#define UART_LSR_TEMT_POS      (6) // 1-bit
#define UART_LSR_THRE_POS      (5) // 1-bit
#define UART_LSR_BI_POS        (4) // 1-bit
#define UART_LSR_FE_POS        (3) // 1-bit
#define UART_LSR_PE_POS        (2) // 1-bit
#define UART_LSR_OE_POS        (1) // 1-bit
#define UART_LSR_DR_POS        (0) // 1-bit

/* UART FAR */
#define UART_FAR_FAR_POS       (0) // 1-bit

/* UART USR */
#define UART_USR_RFF_POS       (4) // 1-bit
#define UART_USR_RFNE_POS      (3) // 1-bit
#define UART_USR_TFE_POS       (2) // 1-bit
#define UART_USR_TFNF_POS      (1) // 1-bit
#define UART_USR_BUSY_POS      (0) // 1-bit

/* UART TCR */
#define UART_TCR_XFER_MODE_POS (3) // 4-bit
#define UART_TCR_DE_POL_POS    (2) // 1-bit
#define UART_TCR_RE_POL_POS    (1) // 1-bit
#define UART_TCR_RS485_EN_POS  (0) // 1-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define UART_DISABLED          (0)
#define UART_ENABLED           (1)

/* value of LCR_EPS */
#define EPS_ODD_PARITY         (0)
#define EPS_EVEN_PARITY        (1)

/* value of LCR_STOP */
#define STOP_1BIT              (0)
#define STOP_1_5BIT_OR_2BIT    (1)

/* value of LCR_DLS */
#define DLS_CHAR_5BITS         (0)
#define DLS_CHAR_6BITS         (1)
#define DLS_CHAR_7BITS         (2)
#define DLS_CHAR_8BITS         (3)

/* value of FCR_TET */
#define TET_EMPTY              (0)
#define TET_CHAR_2             (1)
#define TET_QUARTER_FULL       (2)
#define TET_HALF_FULL          (3)

/* value of FCR_RT */
#define RT_CHAR_1              (0)
#define RT_QUARTER_FULL        (1)
#define RT_HALF_FULL           (2)
#define RT_FULL_2              (3)

/* value of TCR_RE_POL */
#define RE_POL_LOW             (0)
#define RE_POL_HIGH            (1)

/* value of TCR_DE_POL */
#define DE_POL_LOW             (0)
#define DE_POL_HIGH            (1)

/* value of TCR_XFER_MODE */
#define XFER_MODE_MODE0        (0x0)
#define XFER_MODE_MODE1        (0x1)
#define XFER_MODE_MODE2        (0x2)

/**
 * @brief Self Config Param Define
 */

// Note:
//  1. UART_IRQ_SUBTYPE is select from FCR_TET and FCR_RT

#define UART_IRQ_TYPE_MASK     (0xf0)
#define UART_IRQ_TYPE_MISC     (0x10)
#define UART_IRQ_TYPE_TX       (0x20)
#define UART_IRQ_TYPE_RX       (0x30)

#define UART_IRQ_SUBTYPE_MASK  (0x0f)

typedef enum
{
    eUartData_Bit_5 = DLS_CHAR_5BITS,
    eUartData_Bit_6 = DLS_CHAR_6BITS,
    eUartData_Bit_7 = DLS_CHAR_7BITS,
    eUartData_Bit_8 = DLS_CHAR_8BITS,
} UartDataBit_e;

typedef enum
{
    eUartStop_Bit_1 = 0,
    eUartStop_Bit_1p5,
    eUartStop_Bit_2,
} UartStopBit_e;

typedef enum
{
    eUartParity_None = 0,
    eUartParity_Odd,
    eUartParity_Even,
} UartParityBit_e;

typedef enum
{
    eUartIrq_None = 0,

    eUartIrq_XferEmpty = (UART_IRQ_TYPE_TX | TET_EMPTY),
    eUartIrq_XferChar2 = (UART_IRQ_TYPE_TX | TET_CHAR_2),
    eUartIrq_XferQuarterFull = (UART_IRQ_TYPE_TX | TET_QUARTER_FULL),
    eUartIrq_XferHalfFull = (UART_IRQ_TYPE_TX | TET_HALF_FULL),

    eUartIrq_RecvChar1 = (UART_IRQ_TYPE_RX | RT_CHAR_1),
    eUartIrq_RecvQuarterFull = (UART_IRQ_TYPE_RX | RT_QUARTER_FULL),
    eUartIrq_RecvHalfFull = (UART_IRQ_TYPE_RX | RT_HALF_FULL),
    eUartIrq_RecvFullLess2 = (UART_IRQ_TYPE_RX | RT_FULL_2),
} UartIrq_e;

typedef struct
{
    uint32_t status;

    UartDataBit_e data_width;
    UartStopBit_e stop;
    UartParityBit_e parity;
    uint32_t baud;

    uint32_t dma;
    UartIrq_e irq;
    uint32_t rs485;

    uint8_t int_enable;
    IRQn_Type int_irqn;

    void *user_data;
} DeviceUartConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceUartConfig_t g_DeviceUartDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_UART_InitConfig(UART_TypeDef *UARTx, DeviceUartConfig_t *config);
void HW_UART_WriteEnable(UART_TypeDef *UARTx);
void HW_UART_ReadEnable(UART_TypeDef *UARTx);
uint8_t HW_UART_Write(UART_TypeDef *UARTx, uint8_t value);
uint32_t HW_UART_WriteBlock(UART_TypeDef *UARTx, uint8_t *txbuf, uint32_t length);
uint8_t HW_UART_Read(UART_TypeDef *UARTx);
uint32_t HW_UART_GetFIFODepth(UART_TypeDef *UARTx);
uint32_t HW_UART_GetCurrentSendFIFOLength(UART_TypeDef *UARTx);
uint32_t HW_UART_GetCurrentRecvFIFOLength(UART_TypeDef *UARTx);
uint32_t HW_UART_GetStatus(UART_TypeDef *UARTx);
void HW_UART_EnableSendInterrupt(UART_TypeDef *UARTx);
void HW_UART_DisableSendInterrupt(UART_TypeDef *UARTx);
void HW_UART_EnableTHRE(UART_TypeDef *UARTx);
void HW_UART_DisableTHRE(UART_TypeDef *UARTx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HW_UART_H__ */
