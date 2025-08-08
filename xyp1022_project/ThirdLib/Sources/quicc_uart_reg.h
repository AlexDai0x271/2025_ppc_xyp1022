
#ifndef _UART_REG_H
#define _UART_REG_H
/*!
* @file uart_reg.h
*
* @brief Uart access register inline function definition.
*
*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ===========================================  Includes  =========================================== */
#include "quicc_uart.h"


/* ============================================  Define  ============================================ */

/* ===========================================  Typedef  ============================================ */

/* ==========================================  Variables  =========================================== */

/* ====================================  Functions declaration  ===================================== */

/* ======================================  Functions define  ======================================== */
///*!
//* @brief Set UART Divisor Latch
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] div: UART divisor data
//* @return  none
//*/
//__STATIC_INLINE void UART_SetDivisor(UART_Type *UARTx, float div)
//{
//    uint32_t intDiv = (uint64_t)div;
//    uint32_t fracDiv = (uint32_t)((div - intDiv) * 32);
//
//    UARTx->DIV_L = intDiv & 0xFF;
//    UARTx->DIV_H = (intDiv >> 8UL) & 0xFF;
//
//    UARTx->DIV_FRAC = fracDiv;
//}

///*!
//* @brief Set UART Sample counter
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] sampleCounter: UART Sample counter, value can be 0 to 3
//*                         - UART_SMP_CNT0: baud_rate = APB CLOCK/16/{DLH, DLL}
//*                         - UART_SMP_CNT1: baud_rate = APB CLOCK/8/{DLH, DLL}
//*                         - UART_SMP_CNT2: baud_rate = APB CLOCK/4/{DLH, DLL}
//*                         - UART_SMP_CNT3: baud_rate = APB CLOCK/16/{DLM, DLL}
//* @return none
//*/
//__STATIC_INLINE void UART_SetSampleCounter(UART_Type *UARTx, UART_SampleCntType sampleCnt)
//{
//    UARTx->SMP_CNT = (sampleCnt & UART_SMP_CNT_SMP_CNT_Msk);
//}

///*!
//* @brief Set UART Guart Time and set Guard enable
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] guardTime: UART GuardTime and Guard enable
//*                       bit[4]:  1: enable  0: disable
//*                       bit[3:0]: GuardTime: 0 to 15
//* @return none
//*/
//__STATIC_INLINE void UART_SetGuard(UART_Type *UARTx, uint8_t guardTime)
//{
//    if (guardTime & UART_GUARD_GUARD_EN_Msk)
//    {
//        UARTx->GUARD |= UART_GUARD_GUARD_EN_Msk;
//        MODIFY_REG32(UARTx->GUARD, UART_GUARD_GUARD_CNT_Msk, UART_GUARD_GUARD_CNT_Pos, guardTime);
//    }
//    else
//    {
//        UARTx->GUARD &= ~UART_GUARD_GUARD_EN_Msk;
//        UARTx->GUARD &= ~UART_GUARD_GUARD_CNT_Msk;
//    }
//}

/*!
* @brief Set UART Data Bits Length
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] bits: UART Data Bits length, value can be 0 to 3
*                         - UART_WORD_LEN_5BIT
*                         - UART_WORD_LEN_6BIT
*                         - UART_WORD_LEN_7BIT
*                         - UART_WORD_LEN_8BIT
* @return none
*/
__STATIC_INLINE void UART_SetDataBits(UART_Type *UARTx, UART_WordLenType bits)
{
    if ((UART_WORD_LEN_5BIT <= bits) && (bits <= UART_WORD_LEN_8BIT))
    {
        MODIFY_REG32(UARTx->MCR, UART_MCR_DATEBIT_Msk, UART_MCR_DATEBIT_Pos, (bits));  /* set DATABIT */
    }
    else
    {
        /* do nothing */
    }
}

/*!
* @brief Set UART stop bit
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] stopBit: UART StopBit. value can be
*                         - UART_STOP_2BIT: 2 stop bit
*                         - UART_STOP_1BIT: 1 stop bit
* @return none
*/
__STATIC_INLINE void UART_SetStopBit(UART_Type *UARTx, UART_StopBitType stopBit)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_STB_Msk, UART_MCR_STB_Pos, stopBit);
}

///*!
//* @brief Set UART Inverse the tx output
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] InvTx: UART invTx.
//*                         -ENABLE: inverse tx output
//*                         -DISABLE: don't inverse tx output
//* @return none
//*/
//__STATIC_INLINE void UART_SetInvTx(UART_Type *UARTx, ACTION_Type state)
//{
//    MODIFY_REG32(UARTx->LCR1, UART_LCR1_INVTX_Msk, UART_LCR1_INVTX_Pos, state);
//}
//
///*!
//* @brief Set UART Inverse the rx input
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] invRx: UART invRx.
//*                         -ENABLE: inverse rx input
//*                         -DISABLE: don't inverse rx input
//* @return none
//*/
//__STATIC_INLINE void UART_SetInvRx(UART_Type *UARTx, ACTION_Type state)
//{
//    MODIFY_REG32(UARTx->LCR1, UART_LCR1_INVRX_Msk, UART_LCR1_INVRX_Pos, state);
//}

/*!
* @brief Set UART loop mode
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] Loop: UART loop
*                         -ENABLE: loop mode
*                         -DISABLE: normal mode
* @return none
*/
__STATIC_INLINE void UART_SetLoop(UART_Type *UARTx, ACTION_Type state)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_LOOP_Msk, UART_MCR_LOOP_Pos, state);
}

/*!
* @brief Set UART 2byte fifo enable
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] fifoByteEn: UART FIFO2ByteEn.
*                         -ENABLE: enable both tx&rx fifo
*                         -DISABLE: disable both tx&rx fifo
* @return none
*/
__STATIC_INLINE void UART_Set2ByteFIFO(UART_Type *UARTx, ACTION_Type state)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_FIFOE_Msk, UART_MCR_FIFOE_Pos, state);
}

__STATIC_INLINE void UART_SetTXFIFOSize(UART_Type *UARTx, UART_FIFOSize txsize)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_TXFIFOSIZE_Msk, UART_MCR_TXFIFOSIZE_Pos, txsize);
}

__STATIC_INLINE void UART_SetRXFIFOSize(UART_Type *UARTx, UART_FIFOSize rxsize)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_RXFIFOSIZE_Msk, UART_MCR_RXFIFOSIZE_Pos, rxsize);
}

///*!
//* @brief Set UART interrupt enable register, but doesnot set ETXE, ETC bit
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] interruptEn: UART interrupt enable
//* @return none
//*/
//__STATIC_INLINE void UART_SetInterruptEn(UART_Type *UARTx, uint16_t interruptEn)
//{
//    MODIFY_REG32(UARTx->IER, 0x1F9, 0, (interruptEn & 0x1F9));  /* set interrupt, but doesnot set ETXE, ETC bit   */
//}

///*!
//* @brief Set UART sleep enable
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] sleepEn: UART sleepEn.
//*                    -ENABLE: Activate hardware flow control according to software initial settings
//*                             when the chip enters the sleep mode. Release the hardware flow
//*                             when the chip wakes up.
//*                    -DISABLE: Does not deal with sleep mode indication signal
//* @return none
//*/
//__STATIC_INLINE void UART_SetSleepEn(UART_Type *UARTx, ACTION_Type state)
//{
//    MODIFY_REG32(UARTx->SLEEP_EN, UART_SLEEP_EN_SLEEP_EN_Msk, UART_SLEEP_EN_SLEEP_EN_Pos, state);
//}
//
///*!
//* @brief  Set UART DMA enable
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] dmaEn: UART DMA Enable set. value can be
//*                - UART_DMA_TXRX_NONE: both tx&rx transfer with normal mode
//*                - UART_DMA_RX_EN: rx with dma mode and tx with normal mode
//*                - UART_DMA_TX_EN: tx with dma mode and rx with normal mode
//*                - UART_DMA_TXRX_EN: both tx&rx transfer with dma mode
//* @return none
//*/
//__STATIC_INLINE void UART_EnableDMA(UART_Type *UARTx, UART_DmaEnType dmaEn)
//{
//    switch (dmaEn)
//    {
//        case UART_DMA_TXRX_NONE:         /* UART DMA TX and RX all disable */
//            UARTx->DMA_EN &= ~UART_DMA_EN_RX_DMA_EN_Msk;
//            UARTx->DMA_EN &= ~UART_DMA_EN_TX_DMA_EN_Msk;
//        break;
//
//        case UART_DMA_RX_EN:             /* UART DMA RX enable */
//            UARTx->DMA_EN |= UART_DMA_EN_RX_DMA_EN_Msk;
//            UARTx->DMA_EN &= ~UART_DMA_EN_TX_DMA_EN_Msk;
//        break;
//
//        case UART_DMA_TX_EN:             /* UART DMA TX enable */
//            UARTx->DMA_EN &= ~UART_DMA_EN_RX_DMA_EN_Msk;
//            UARTx->DMA_EN |= UART_DMA_EN_TX_DMA_EN_Msk;
//        break;
//
//        case UART_DMA_TXRX_EN:           /* UART DMA TX and RX all enable */
//            UARTx->DMA_EN |= UART_DMA_EN_RX_DMA_EN_Msk;
//            UARTx->DMA_EN |= UART_DMA_EN_TX_DMA_EN_Msk;
//        break;
//
//        default:
//        break;
//    }
//}
//
///*!
//* @brief Set UART rs485 Control
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] rs485CR: UART RS485CR. the corresponding control bit means
//*                   - bit7: enable rs485 or disable
//*                   - bit5: inverse or don't inverse the polarity of rts_n
//*                   - bit4: delay insert between the last stop bit
//* @return none
//*/
//__STATIC_INLINE void UART_SetRS485CR(UART_Type *UARTx, uint8_t rs485CR)
//{
//    if (rs485CR & UART_RS485CR_DLYEN_Msk)  /* set DLYEN: bit4 */
//    {
//      UARTx->RS485CR |= UART_RS485CR_DLYEN_Msk;
//    }
//    else
//    {
//      UARTx->RS485CR &= ~UART_RS485CR_DLYEN_Msk;
//    }
//
//    if (rs485CR & UART_RS485CR_INVPOL_Msk)  /* set INVPOL: bit5 */
//    {
//      UARTx->RS485CR |= UART_RS485CR_INVPOL_Msk;
//    }
//    else
//    {
//      UARTx->RS485CR &= ~UART_RS485CR_INVPOL_Msk;
//    }
//
//    if (rs485CR & UART_RS485CR_RS485EN_Msk)  /* set RS485EN: bit7 */
//    {
//      UARTx->RS485CR |= UART_RS485CR_RS485EN_Msk;
//    }
//    else
//    {
//      UARTx->RS485CR &= ~UART_RS485CR_RS485EN_Msk;
//    }
//}
//
///*!
//* @brief Set UART delay time in RS485 mode
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] counter: UART Counter. value can be
//*                   - 7:0: 0~255 bits time for time delay in RS485 mode
//* @return none
//*/
//__STATIC_INLINE void UART_SetRS485Cnt(UART_Type *UARTx, uint8_t counter)
//{
//    UARTx->CNTR = counter;
//}
//
///*!
//* @brief Set UART Idle line detect function enable.
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] state: UART Idle line detect function enable/disable
//*                         - ENABLE
//*                         - DISABLE
//* @return none
//*/
//__STATIC_INLINE void UART_SetIdleFuncEn(UART_Type *UARTx, ACTION_Type state)
//{
//    MODIFY_REG32(UARTx->IDLE, UART_IDLE_ILEN_Msk, UART_IDLE_ILEN_Pos, state);
//}
//
///*!
//* @brief Set UART idle interrupt enable.must set MULCOMEN=1
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @param[in] state: idle interrupt enable/disable
//*                         - ENABLE
//*                         - DISABLE
//* @return none
//*/
//__STATIC_INLINE void UART_SetIdleInterrupt(UART_Type *UARTx, ACTION_Type state)
//{
//    MODIFY_REG32(UARTx->IDLE, UART_IDLE_IDLEIE_Msk, UART_IDLE_IDLEIE_Pos, state);
//}

///*!
//* @brief Set UART Rx data is ready
//*
//* @param[in] UARTx: UART type pointer, x can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @return mul com idle line detect
//*/
//#define UART_IsIdle(UARTx)    ((uint32_t)((UARTx->LSR1 & UART_LSR1_IDLE_Msk) >> UART_LSR1_IDLE_Pos))

///*!
//* @brief Set UART LIN control
//*
//* @param[in] UARTx: UART type pointer, x can be 0 to 1
//*                         - UART0
//*                         - UART1
//* @param[in] linCtrl: UART linCtrl. the corresponding control bit means
//*                   - bit7: LIN Mode enable
//*                   - bit6: LIN Break character detect interrupt enable
//*                   - bit5: LIN Mode break detect length  1:11bits  0:10bits
//*                   - bit3: 1:0x55 used to auto baud rate detection  0: 0X55 not used to auto baud rate detection
//*                   - bit2: LIN sync byte error interrupt enable
//*                   - bit1: LIN wake up interrupt enable
//*
//* @return none
//*/
//__STATIC_INLINE void UART_SetLIN(UART_Type *UARTx, uint8_t linCtrl)
//{
//    if ((UARTx == UART0) || (UARTx == UART1))
//    {
//         MODIFY_REG32(UARTx->LINCR, 0xFE, 0, (linCtrl & 0xFE));
//    }
//    else
//    {
//        /* do nothing */
//    }
//}

/*!
* @brief Check if rx buffer get new data
*
* @param[in] UARTx: UART type pointer, x can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return 0:no date in buff or fifo,  1:RX buffer becoming full or RX FIFO not empty
*/
#define UART_RxIsDataReady(UARTx)    ((uint32_t)(((UARTx)->LSR & UART_LSR_DR_Msk) >> UART_LSR_DR_Pos))

/*!
* @brief Set UART Tx buffer is empty
*
* @param[in] UARTx: UART type pointer, x can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return 0:TX holding register or FIFO is not empty   1:TX holding register or FIFO is empty
*/
#define UART_TxIsEmpty(UARTx)    ((uint32_t)(((UARTx)->LSR & UART_LSR_THRE_Msk) >> UART_LSR_THRE_Pos))

///*!
//* @brief Set UART Tx buffer is empty
//*
//* @param[in] UARTx: UART type pointer, x can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @return Tx is full or not:
//*                         0:TX holding register or FIFO is not empty
//*                         1:TX holding register or FIFO is empty
//*/
//#define UART_TxIsFull(UARTx)    ((uint32_t)(((UARTx)->LSR & UART_LSR_TXDF_Msk) >> UART_LSR_TXDF_Pos))

/*!
* @brief Get UART Tx Finished or not
*
* @param[in] UARTx: UART type pointer, x can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return Tx tranfer has finished nor not
*                         0:transmitter has not finished
*                         1:transmitter has finished
*/
#define UART_TxIsFinished(UARTx)    ((uint32_t)(((UARTx)->LSR & UART_LSR_TC_Msk) >> UART_LSR_TC_Pos))

/*!
* @brief Enable UART TX
*
* @param[in] UARTx: UART type pointer, x can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] Enable/Disable UART tx
*                         - ENABLE
*                         - DISABLE
* @return none
*/
__STATIC_INLINE void UART_Enable(UART_Type *UARTx, ACTION_Type state)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_EN_Msk, UART_MCR_EN_Pos, state);
}

__STATIC_INLINE void UART_Reset(UART_Type *UARTx)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_EN_Msk, UART_MCR_EN_Pos, 1);
    MODIFY_REG32(UARTx->MCR, UART_MCR_EN_Msk, UART_MCR_EN_Pos, 0);
}

__STATIC_INLINE void UART_SetRST(UART_Type *UARTx)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_SETRTS_Msk, UART_MCR_SETRTS_Pos, 1);
}

__STATIC_INLINE void UART_ClearRST(UART_Type *UARTx)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_SETRTS_Msk, UART_MCR_SETRTS_Pos, 0);
}

///*!
//* @brief Send lin break
//*
//* @param[in] UARTx: UART type pointer, x can be 0 to 1
//*                         - UART0
//*                         - UART1
//* @return none
//*/
//__STATIC_INLINE void UART_SendLINBreak(UART_Type *UARTx)
//{
//    if ((UARTx == UART0) || (UARTx == UART1))
//    {
//        UARTx->LINCR |= UART_LINCR_SDBRK_Msk;
//    }
//}


///*!
//* @brief Lin break length for master
//*
//* @param[in] UARTx: UART type pointer, x can be 0 to 1
//*                         - UART0
//*                         - UART1
//* @param[in] length:LIN break length
//* @return none
//*/
//__STATIC_INLINE void UART_SetLINBreak(UART_Type *UARTx, uint8_t length)
//{
//    if ((UARTx == UART0) || (UARTx == UART1))
//    {
//      UARTx->BRKLGH = (length & UART_BRKLGH_BRKLGH_Msk);
//    }
//}

/*!
* @brief Enable rx buffer not empty interrupt
*
* @param[in] UARTx: UART type pointer, x can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] Enable/Disable Rx not empty interrupt
*                         - ENABLE
*                         - DISABLE
* @return none
*/
__STATIC_INLINE void UART_SetRXNEInterrupt(UART_Type *UARTx, ACTION_Type state)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_ERXNE_Msk, UART_MCR_ERXNE_Pos, state);
}

/*!
* @brief Enable tx holding register is empty interrupt
*
* @param[in] UARTx: UART type pointer, x can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] Enable/Disable Tx empty interrupt
*                         - ENABLE
*                         - DISABLE
*/
__STATIC_INLINE void UART_SetTXEInterrupt(UART_Type *UARTx, ACTION_Type state)
{
    MODIFY_REG32(UARTx->MCR, UART_MCR_ETXE_Msk, UART_MCR_ETXE_Pos, state);
}

///*!
//* @brief Check UART Data Bits is 9 bits or not
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                         - UART0
//*                         - UART1
//*                         - UART2
//* @return 1: 9 bits
//*         0: 5~8 bits
//*/
//__STATIC_INLINE uint8_t UART_IsNineDataBits(UART_Type *UARTx)
//{
//    return (uint8_t)(((UARTx->LCR1 & UART_LCR1_WLS2_Msk) >> UART_LCR1_WLS2_Pos));
//}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _UART_REG_H */

/* =============================================  EOF  ============================================== */
