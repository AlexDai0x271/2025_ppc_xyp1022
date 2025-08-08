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
#include "sysclock.h"

/**
 * @brief Default UART device configuration structure for debug mode
 * @details This structure defines the default configuration parameters for UART devices,
 *          intended for use in debug mode only.
 *
 * Configuration parameters:
 * - data_width: Set to 8 bits
 * - stop: Set to 1 stop bit
 * - parity: Set to no parity
 * - baud: Set to 115200
 * - irq: Set to no interrupt
 * - int_enable: Set to 0 (disabled)
 * - int_irqn: Set to UART0 interrupt number
 * - user_data: Set to 0
 *
 * @note This default configuration is suitable for basic serial communication needs
 * @warning Avoid using these default parameters in production environment
 * @see Parameters can be adjusted based on actual hardware and project requirements
 */
#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceUartConfig_t g_DeviceUartDefaultConfig = {
    .data_width = eUartData_Bit_8,
    .stop = eUartStop_Bit_1,
    .parity = eUartParity_None,
    .baud = 115200,

    .irq = eUartIrq_None,

    .int_enable = 0,
    .int_irqn = UART0_IRQn,

    .user_data = 0,
};
#endif // #ifdef DEBUG

/**
 * @brief Initialize the specified UART communication port
 * @details Configures the UART port with specified parameters including baud rate,
 *          data width, stop bits, parity, and interrupt settings
 *
 * @param UARTx Pointer to UART peripheral
 * @param config Pointer to UART configuration structure
 *
 * @return 0 on success, -1 on failure
 *
 * @details Configuration process:
 * - Baud rate calculation: APB bus frequency divided by 16 times configured baud rate minus 1
 * - MCR (Modem Control Register): Disables all mode control options
 * - LCR (Line Control Register):
 *   - Sets data width (8-bit or other)
 *   - Configures stop bits (1 or 1.5/2)
 *   - Sets parity (none, even, or odd)
 * - FCR (FIFO Control Register):
 *   - Enables FIFO, clears TX and RX FIFOs
 *   - Configures interrupt type
 * - IER (Interrupt Enable Register):
 *   - Sets interrupt enable status
 *
 * @warning Both UARTx and config pointers must be valid
 * @note Uses config->status to track UART initialization state
 */
int HW_UART_InitConfig(UART_TypeDef *UARTx, DeviceUartConfig_t *config)
{
    uint32_t uart_div;
    uint32_t reg_value;

    if ((NULL == UARTx) || (NULL == config)) {
        return -1;
    }

    uart_div = drv_get_uart_freq() / (16 * config->baud);

    // config: baudrate N 8 1

    reg_value = 0; // .MCR inited to zero first
    SET_BIT(reg_value, UART_MCR_LOOPBACK_POS, 0);
    SET_BIT(reg_value, UART_MCR_OUT_2_POS, 1);
    SET_BIT(reg_value, UART_MCR_OUT_1_POS, 1);
    SET_BIT(reg_value, UART_MCR_RTS_POS, 1);
    SET_BIT(reg_value, UART_MCR_DTR_POS, 1);
    UARTx->MCR = reg_value;

    SET_BIT(UARTx->LCR, UART_LCR_DLAB_POS, 1);
    UARTx->DLM = (uart_div >> 8) & 0xFF;
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
    UARTx->FCR = reg_value;

    reg_value = UARTx->IER;
    SET_BIT(reg_value, UART_IER_EDSSI_POS, 0);
    SET_BIT(reg_value, UART_IER_ELSI_POS, 0);
    SET_BIT(reg_value, UART_IER_ETBEI_POS, 0);
    SET_BIT(reg_value, UART_IER_ERBFI_POS, 0);
    UARTx->IER = reg_value;

    reg_value = UARTx->IIR;
    UNUSED(reg_value);

    if (config->int_enable) {
        // TODO
    }

    config->status = 0;
    return 0;
}

/**
 * @brief Write a single byte to the specified UART port
 * @details Waits for the transmit holding register to be empty before writing the byte
 *
 * @param UARTx Pointer to UART peripheral
 * @param value Byte to be transmitted
 *
 * @return The transmitted byte
 *
 * @note This function blocks until the transmit holding register is empty
 */
uint8_t HW_UART_Write(UART_TypeDef *UARTx, uint8_t value)
{
    while (!GET_BIT(UARTx->LSR, UART_LSR_TEMT_POS))
        ;

    UARTx->THR = value;
    return value;
}

/**
 * @brief Write a block of data to the specified UART port
 * @details Transmits multiple bytes of data through the UART port
 *
 * @param UARTx Pointer to UART peripheral
 * @param txbuf Pointer to the data buffer to be transmitted
 * @param length Number of bytes to transmit
 *
 * @return Number of bytes actually transmitted
 *
 * @warning txbuf must not be NULL
 * @note Enables transmit interrupt after sending data
 */
uint32_t HW_UART_WriteBlock(UART_TypeDef *UARTx, uint8_t *txbuf, uint32_t length)
{
    if (!txbuf) {
        return 0;
    }

    for (uint32_t i = 0; i < length; i++) {
        while (!GET_BIT(UARTx->LSR, UART_LSR_TEMT_POS))
            ;
        UARTx->THR = txbuf[i];
    }

    HW_UART_EnableSendInterrupt(UARTx);

    return length;
}

/**
 * @brief Read a single byte from the specified UART port
 * @details Waits for data to be available in the receive buffer before reading
 *
 * @param UARTx Pointer to UART peripheral
 *
 * @return The received byte
 *
 * @note This function blocks until data is available
 */
uint8_t HW_UART_Read(UART_TypeDef *UARTx)
{
    uint8_t value;

    while (!GET_BIT(UARTx->LSR, UART_LSR_DR_POS))
        ;

    value = UARTx->RBR;
    return value;
}

/**
 * @brief Get the status of the specified UART port
 * @details Retrieves the current state of the UART's Interrupt Identification Register (IIR)
 *
 * @param UARTx Pointer to UART peripheral
 *
 * @return Current status of the UART's IIR
 */
uint32_t HW_UART_GetStatus(UART_TypeDef *UARTx)
{
    return UARTx->IIR;
}

/**
 * @brief Enable transmit interrupt for the specified UART port
 * @details Sets the Transmit Holding Register Empty Interrupt bit in IER
 *
 * @param UARTx Pointer to UART peripheral
 */
void HW_UART_EnableSendInterrupt(UART_TypeDef *UARTx)
{
    UARTx->IER |= (1 << UART_IER_ETBEI_POS);
}

/**
 * @brief Disable transmit interrupt for the specified UART port
 * @details Clears the Transmit Holding Register Empty Interrupt bit in IER
 *
 * @param UARTx Pointer to UART peripheral
 */
void HW_UART_DisableSendInterrupt(UART_TypeDef *UARTx)
{
    UARTx->IER &= ~(1 << UART_IER_ETBEI_POS);
}

#ifdef DEBUG_UART

#include "xprintf.h"

/**
 * @brief Test buffer and length for UART self-test
 */
static uint8_t tx_buffer[1000];
static uint32_t tx_length = sizeof(tx_buffer);

/**
 * @brief Self-test function for UART functionality
 * @details Initializes UART with test configuration and sends test data
 *
 * @return 0 on completion
 *
 * @note Only available in DEBUG_UART mode
 */
int selftest_uart(void)
{
#if (DEBUG_UART_CASE == DEBUG_UART_NORMAL)
    UART_TypeDef *uart = UART1;
    IRQn_Type irqn = UART1_IRQn;

    DeviceUartConfig_t uart_config =
        {
            .data_width = eUartData_Bit_8,
            .stop = eUartStop_Bit_1,
            .parity = eUartParity_None,
            .baud = 9600,

            .irq = eUartIrq_XferChar2,

            .int_enable = 1,
            .int_irqn = irqn,

            .user_data = 0,
        };

    for (uint32_t i = 0; i < sizeof(tx_buffer); i++) {
        tx_buffer[i] = i % 256;
    }

    HW_UART_InitConfig(uart, &uart_config);

    tx_length = HW_UART_WriteBlock(uart, tx_buffer, tx_length);
    xprintf("send %d.\n", tx_length);
#endif // #if ( DEBUG_UART_CASE == DEBUG_UART_NORMAL )

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_UART
