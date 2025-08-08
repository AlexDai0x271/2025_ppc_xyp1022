
/*!
* @file uart.c
*
* @brief This file provides uart integration functions.
*
*/

/* ===========================================  Includes  =========================================== */
#include "quicc_uart_reg.h"


/* ============================================  Define  ============================================ */


/* ===========================================  Typedef  ============================================ */
static const UART_Type * const s_uartCtrlMap[UART_INDEX_MAX] =
{UART0, UART1, UART2};


/* ==========================================  Variables  =========================================== */
DeviceCallback_Type s_uartCallback[UART_INDEX_MAX] = {0};


/* ====================================  Functions declaration  ===================================== */


/* ======================================  Functions define  ======================================== */
/*!
* @brief UART initialize
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] config: uart setting type pointer which contains the configuration
*                         information for the specified UART.
* @return none
*/
void UART_Init(UART_Type *UARTx, const UART_ConfigType *config)
{
    UART_IndexType uartIndex = UART_INDEX(UARTx);;

    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));
    DEVICE_ASSERT(IS_UART_WORD_LEN(config->dataBits));
    DEVICE_ASSERT(IS_UART_STOPBITS(config->stopBits));
    DEVICE_ASSERT(IS_UART_PARITY(config->parity));
    DEVICE_ASSERT(IS_UART_DMAEN(config->dmaEn));

    switch ((uint32_t)UARTx)
    {
        case UART0_BASE:
//            CKGEN_Enable(CLK_UART0, ENABLE);
//            CKGEN_SoftReset(SRST_UART0, ENABLE);
        break;

        case UART1_BASE:
//            CKGEN_Enable(CLK_UART1, ENABLE);
//            CKGEN_SoftReset(SRST_UART1, ENABLE);
        break;

        case UART2_BASE:
//            CKGEN_Enable(CLK_UART2, ENABLE);
//            CKGEN_SoftReset(SRST_UART2, ENABLE);
        break;

        default:
        break;
    }

    s_uartCallback[uartIndex] = config->callBack;
    
    UART_Reset(UARTx);
    
    //UART_SetLoop(UARTx, 1);
    
    UART_SetDataBits(UARTx, config->dataBits);
    UART_SetParity(UARTx, config->parity);
    UART_SetBaudrate(UARTx, config->baudrate);
    UART_SetStopBit(UARTx, config->stopBits);
    
    UART_SetCTSRTS(UARTx, UART_RTSCTS_ALL);
    //UART_SetCTSRTS(UARTx, UART_RTSCTS_NONE);

    if ((config->dmaEn) || (config->fifoByteEn))
    {
        UART_Set2ByteFIFO(UARTx, ENABLE);
        
        UART_SetTXFIFOSize(UARTx, UART_FIFOSize_32Byte);
        UART_SetRXFIFOSize(UARTx, UART_FIFOSize_32Byte);
    }
    else
    {
       UART_Set2ByteFIFO(UARTx, DISABLE);
    }

//    if ( config->dmaEn )
//    {
//    	UART_EnableDMA(UARTx, config->dmaEn);
//    }

    UART_ClearRST(UARTx);
    
    UART_Enable(UARTx, ENABLE);
}


/*!
* @brief UART uninitialize
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return none
*/
void UART_DeInit(UART_Type *UARTx)
{
    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));

    switch ((uint32_t)UARTx)
    {
        case UART0_BASE:
//            NVIC_DisableIRQ(UART0_IRQn);
//            NVIC_ClearPendingIRQ(UART0_IRQn);
//            CKGEN_SoftReset(SRST_UART0, DISABLE);
//            CKGEN_Enable(CLK_UART0, DISABLE);
            s_uartCallback[UART0_INDEX] = NULL;
        break;

        case UART1_BASE:
//            NVIC_DisableIRQ(UART1_IRQn);
//            NVIC_ClearPendingIRQ(UART1_IRQn);
//            CKGEN_SoftReset(SRST_UART1, DISABLE);
//            CKGEN_Enable(CLK_UART1, DISABLE);
            s_uartCallback[UART1_INDEX] = NULL;
        break;

        case UART2_BASE:
//            NVIC_DisableIRQ(UART2_IRQn);
//            NVIC_ClearPendingIRQ(UART2_IRQn);
//            CKGEN_SoftReset(SRST_UART2, DISABLE);
//            CKGEN_Enable(CLK_UART2, DISABLE);
            s_uartCallback[UART2_INDEX] = NULL;
        break;

        default:
        break;
    }
}

/*!
* @brief Set UART baudrate
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] baudrate: UART baudrate
* @return none
*/
void UART_SetBaudrate(UART_Type *UARTx, uint32_t baudrate)
{
    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));
    DEVICE_ASSERT(IS_UART_BAUDRATE(baudrate));

    MODIFY_REG32(UARTx->MCR, UART_MCR_BAUDRATE_Msk, UART_MCR_BAUDRATE_Pos, baudrate);
}

/*!
* @brief Get UART type pointer
*
* @param[in] uartIndex: UART index, value can be 0 to 2
*                         - UART0_INDEX
*                         - UART1_INDEX
*                         - UART2_INDEX
* @return UART type pointer
*                         - UART0
*                         - UART1
*                         - UART2
*/
UART_Type* UART_GetUARTCtrl(UART_IndexType uartIndex)
{
    UART_Type* UARTx = 0;

    DEVICE_ASSERT(IS_UART_INDEX(uartIndex));

    if (uartIndex < UART_INDEX_MAX)
    {
        UARTx = (UART_Type*)(UART0_BASE + ((uint32_t)uartIndex << 12));
    }

    return UARTx;
}

/*!
* @brief UART send data by normal mode
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return Function return state:
*                         -SUCCESS
*                         -ERROR: timeout
*/
ERROR_Type UART_SendData(UART_Type *UARTx, uint16_t data)
{
    ERROR_Type ret = ERROR;
    uint32_t i = 0;

    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));

    for (i = 0; i < 0xFFF; i++)
    {
//        if (!UART_TxIsFull(UARTx))
        if ( UART_TxIsEmpty(UARTx) )
        {
            UARTx->RBR = (data & (UART_RBR_RBR_THR_Msk));
            ret = SUCCESS;
            break;
        }
    }

    return ret;
}

/*!
* @brief UART receive data by normal mode
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return Get uart data
*/
uint16_t UART_ReceiveData(UART_Type *UARTx)
{
    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));

    return (uint16_t)(UARTx->RBR & UART_RBR_RBR_THR_Msk);
}
//
///*!
//* @brief UART DMA transmit data
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                 - UART0
//*                 - UART1
//*                 - UART2
//* @param[in] DMAx:DMA Channel type pointer,
//*                 -DMA0_CHANNEL0
//*                 -DMA0_CHANNEL1
//*                 -DMA0_CHANNEL2
//*                 -DMA0_CHANNEL3
//* @param[in] txBuffer: point to the send data
//* @param[in] length: transfmit data length by byte
//*                    If UART is 8 bits, length must be 0~32767, because DMA CHAN_LENGTH is 0~32767
//*                    If UART is 9 bits, length must be multiple of 2, because DMA CHAN_LENGTH is length/2,
//*                    otherwise the last data cannot be transmit
//* @param[in] callback: point to DMA callback function
//* @return Function return state: SUCCESS/ERROR
//*/
//ERROR_Type UART_TransmitDMA(UART_Type *UARTx, DMA_ChannelType *DMAx, \
//                                      uint8_t* txBuffer, uint16_t length, DeviceCallback_Type callback)
//{
//    ERROR_Type ret = SUCCESS;
//    DMA_ConfigType dmaConfig;
//
//    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));
//    DEVICE_ASSERT(IS_DMA_PERIPH(DMAx));
//
//    if ((uint32_t)txBuffer & 0x03) /* txBuffer is not word alignment */
//    {
//        ret = ERROR;
//        return ret;
//    }
//
//    dmaConfig.memStartAddr = (uint32_t)&txBuffer[0];
//    dmaConfig.periphStartAddr = (uint32_t)(&(UARTx->RBR));
//    dmaConfig.channelEn = ENABLE;
//    dmaConfig.finishInterruptEn = ENABLE;
//    dmaConfig.halfFinishInterruptEn = DISABLE;
//    dmaConfig.errorInterruptEn = ENABLE;
//    dmaConfig.channelPriority = DMA_PRIORITY_VERY_HIGH;
//    dmaConfig.circular = DISABLE;
//    dmaConfig.direction = DMA_READ_FROM_MEM;
//    dmaConfig.MEM2MEM = DISABLE;
//    dmaConfig.memIncrement = ENABLE;
//    dmaConfig.periphIncrement = DISABLE;
//    dmaConfig.memSize = DMA_MEM_SIZE_32BIT;
//
//    switch ((uint32_t)UARTx)
//    {
//        case UART0_BASE:
//            dmaConfig.periphSelect = DMA_PEPIRH_UART0_TX;
//        break;
//
//        case UART1_BASE:
//            dmaConfig.periphSelect = DMA_PEPIRH_UART1_TX;
//        break;
//
//        case UART2_BASE:
//            dmaConfig.periphSelect = DMA_PEPIRH_UART2_TX;
//        break;
//
//        default:
//        break;
//    }
//
//    /* set memory end address and byte mode */
//    if (UART_IsNineDataBits(UARTx)) /* Data is 9 bits */
//    {
//        dmaConfig.transferNum = length >> 1u;
//        dmaConfig.memByteMode = DMA_MEM_BYTE_MODE_2TIME;  /* 16 bits each time */
//        dmaConfig.periphSize = DMA_PERIPH_SIZE_16BIT;
//    }
//    else  /* Data is 5~8 bits */
//    {
//        dmaConfig.transferNum = length;
//        dmaConfig.memByteMode = DMA_MEM_BYTE_MODE_4TIME;  /* 8 bits each time */
//        dmaConfig.periphSize = DMA_PERIPH_SIZE_8BIT;
//    }
//    dmaConfig.memEndAddr = dmaConfig.memStartAddr + length;
//    dmaConfig.callBack = callback;
//
//    DMA_DeInit(DMAx);
//    DMA_Init(DMAx, &dmaConfig);
//
//    return ret;
//}
//
///*!
//* @brief  UART DMA receive data
//*
//* @param[in] UARTx: UART type pointer, x value can be 0 to 2
//*                 - UART0
//*                 - UART1
//*                 - UART2
//* @param[in] DMAx:DMA Channel type pointer,
//*                 -DMA0_CHANNEL0
//*                 -DMA0_CHANNEL1
//*                 -DMA0_CHANNEL2
//*                 -DMA0_CHANNEL3
//* @param[in] rxBuffer: point to the receive buffer
//* @param[in] length: Receive data length by byte
//*                    If UART is 8 bits, length must be 0~32767, because DMA CHAN_LENGTH is 0~32767
//*                    If UART is 9 bits, length must be multiple of 2, because DMA CHAN_LENGTH is length/2,
//*                    otherwise the last data cannot be receive
//* @param[in] callback: point to DMA callback function
//* @return Function return state: SUCCESS/ERROR
//*/
//ERROR_Type UART_ReceiveDMA(UART_Type *UARTx, DMA_ChannelType *DMAx, \
//                                    uint8_t* rxBuffer, uint16_t length, DeviceCallback_Type callback)
//{
//    ERROR_Type ret = SUCCESS;
//    DMA_ConfigType dmaConfig;
//
//    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));
//    DEVICE_ASSERT(IS_DMA_PERIPH(DMAx));
//
//    dmaConfig.memStartAddr = (uint32_t)&rxBuffer[0];
//    dmaConfig.periphStartAddr = (uint32_t)(&(UARTx->RBR));
//    dmaConfig.channelEn = ENABLE;
//    dmaConfig.finishInterruptEn = ENABLE;
//    dmaConfig.halfFinishInterruptEn = DISABLE;
//    dmaConfig.errorInterruptEn = ENABLE;
//    dmaConfig.channelPriority = DMA_PRIORITY_VERY_HIGH;
//    dmaConfig.circular = DISABLE;
//    dmaConfig.direction = DMA_READ_FROM_PERIPH;
//    dmaConfig.MEM2MEM = DISABLE;
//    dmaConfig.memIncrement = ENABLE;
//    dmaConfig.periphIncrement = DISABLE;
//    dmaConfig.periphSize = DMA_PERIPH_SIZE_8BIT;
//
//    switch ((uint32_t)UARTx)
//    {
//        case UART0_BASE:
//            dmaConfig.periphSelect = DMA_PEPIRH_UART0_RX;
//        break;
//
//        case UART1_BASE:
//            dmaConfig.periphSelect = DMA_PEPIRH_UART1_RX;
//        break;
//
//        case UART2_BASE:
//            dmaConfig.periphSelect = DMA_PEPIRH_UART2_RX;
//        break;
//
//        default:
//        break;
//    }
//
//    if (UART_IsNineDataBits(UARTx)) /* Data is 9 bits */
//    {
//        if ((uint32_t)rxBuffer & 0x01) /* rxBuffer is not half-word alignment */
//        {
//            ret = ERROR;
//            return ret;
//        }
//        dmaConfig.memSize = DMA_MEM_SIZE_16BIT;
//        dmaConfig.transferNum = length >> 1u;
//    }
//    else
//    {
//        dmaConfig.memSize = DMA_MEM_SIZE_8BIT;
//        dmaConfig.transferNum = length;
//    }
//    dmaConfig.memByteMode = DMA_MEM_BYTE_MODE_1TIME;  /* 16 bits each time */
//    dmaConfig.memEndAddr = dmaConfig.memStartAddr + length;
//    dmaConfig.callBack = callback;     /* Set dma interrupt callback */
//
//    DMA_DeInit(DMAx);
//    DMA_Init(DMAx, &dmaConfig);
//
//    return ret;
//}

/*!
* @brief Set UART Parity
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] parity: UART parity set
*                         - UART_PARI_NO: no parity
*                         - UART_PARI_ODD: ODD parity
*                         - UART_PARI_EVEN: EVEN parity
* @return none
*/
void UART_SetParity(UART_Type *UARTx, UART_ParityType parity)
{
    switch (parity)
    {
      case UART_PARI_ODD:                         /* UART_PARI_ODD */
        UARTx->MCR |= UART_MCR_PEN_Msk;         /* parity enable */
        UARTx->MCR |= UART_MCR_EPS_Msk;         /* set EPS,  0: even  1: odd*/
      break;

      case UART_PARI_EVEN:                        /* UART_PARI_EVEN */
        UARTx->MCR |= UART_MCR_PEN_Msk;         /* parity enable */
        UARTx->MCR &= ~UART_MCR_EPS_Msk;        /* set EPS,  0: even  1: odd*/
      break;

      case UART_PARI_NO:                          /* UART_PARI_NO */
        UARTx->MCR &= ~UART_MCR_PEN_Msk;        /* parity disable */
        UARTx->MCR &= ~UART_MCR_EPS_Msk;        /* set EPS,  0: even  1: odd*/
      break;

      default:
        UARTx->MCR &= ~UART_MCR_PEN_Msk;        /* parity disable */
        UARTx->MCR &= ~UART_MCR_EPS_Msk;        /* set EPS,  0: even  1: odd*/
      break;
    }
}

/*!
* @brief Set UART RTS CTS
*
* @param[in] UARTx: UART type pointer, x value should be 0, only UART0 has CTS/RTS function
*                         - UART0
* @param[in] RTSCTS: UART rtsCts, value can be 0 to 3
*                         -UART_RTSCTS_NONE£ºNO RTS or CTS
*                         -UART_RTS_ONLY: RTS only
*                         -UART_CTS_ONLY: CTS only
*                         -UART_RTSCTS_ALL: enable RTS and CTS
* @return none
*/
void UART_SetCTSRTS(UART_Type *UARTx, UART_RtsCtsType RTSCTS)
{
    switch (RTSCTS)
    {
        case UART_RTSCTS_NONE:            /* NO RTS or CTS */
            UARTx->MCR &= ~UART_MCR_RTSCTSEN_Msk;
            UARTx->MCR &= ~UART_MCR_AFCFEN_Msk;
        break;
        case UART_RTSCTS_ALL:         /* enable RTS and CTS */
            UARTx->MCR |= UART_MCR_RTSCTSEN_Msk;
            UARTx->MCR |= UART_MCR_AFCFEN_Msk;
        break;

        default:
        break;
    }
}

/*!
* @brief Set UART call back
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @param[in] Callback: UART Callback function which will be called in UARTx_IRQHandler.
* @return none
*/
void UART_SetCallback(UART_Type *UARTx, const DeviceCallback_Type callback)
{
    uint8_t uartIndex = 0;

    DEVICE_ASSERT(IS_UART_PERIPH(UARTx));

    for (uartIndex = 0; uartIndex < (uint8_t)UART_INDEX_MAX; uartIndex++)
    {
        if (s_uartCtrlMap[uartIndex] == UARTx)
        {
            s_uartCallback[uartIndex] = callback;
            break;
        }
    }
}

/*!
* @brief UART Interrupt Handler
*
* @param[in] uartIndex: Uart index, value can be 0 to 2
* @return none
*/
static void UART_InterruptHandler(UART_Type *UARTx)
{
    UART_IndexType uartIndex = UART0_INDEX;

    uint32_t lsr = UARTx->LSR;

    UARTx->LSR = lsr;

    uartIndex = UART_INDEX(UARTx);

//    if (0 == (UARTx->IER & UART_IER_ERXNE_Msk))
//    {
//        lsr &= ~UART_LSR_DR_Msk;
//    }

    if ((uartIndex < UART_INDEX_MAX) && (s_uartCallback[uartIndex]))
    {
        s_uartCallback[uartIndex](UARTx, lsr);
    }
}

/*!
* @brief UART0 Interrupt Handler
*
* @param[in] none
* @return none
*/
void UART0_IRQHandler(void)
{
    UART_InterruptHandler(UART0);
}

/*!
* @brief UART1 Interrupt Handler
*
* @param[in] none
* @return none
*/
void UART1_IRQHandler(void)
{
    UART_InterruptHandler(UART1);
}

/*!
* @brief UART2 Interrupt Handler
*
* @param[in] none
* @return none
*/
void UART2_IRQHandler(void)
{
    UART_InterruptHandler(UART2);
}


/* =============================================  EOF  ============================================== */
