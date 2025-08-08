
#ifndef _UART_H
#define _UART_H
/*!
* @file quicc_uart.h
*
* @brief This file provides uart integration functions interface.
*
*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ===========================================  Includes  =========================================== */
#include <stdint.h>
#include <stdio.h>


#define UART0_BASE                  0xE95BB600UL
#define UART1_BASE                  0xE95BB700UL
#define UART2_BASE                  0xE95BB800UL
//#define APB_BUS_FREQ										   16*1000*1000//16MHz


typedef enum {
	BaudRate_300bps = 0U,   /*!< 300bps */
	BaudRate_600bps = 1U,   /*!< 300bps */
	BaudRate_1200bps = 2U,   /*!< 300bps */
	BaudRate_2400bps = 3U,   /*!< 300bps */
	BaudRate_4800bps = 4U,   /*!< 300bps */
	BaudRate_9600bps = 5U,   /*!< 300bps */
	BaudRate_19200bps = 6U,   /*!< 300bps */
	BaudRate_38400bps = 7U,   /*!< 300bps */
	BaudRate_57600bps = 8U,   /*!< 300bps */
	BaudRate_115200bps = 9U,   /*!< 300bps */
	BaudRate_230400bps = 10U,   /*!< 300bps */
	BaudRate_460800bps = 11U,   /*!< 300bps */
	BaudRate_921600bps = 12U,   /*!< 300bps */
	BaudRate_1843200bps = 13U,   /*!< 300bps */
}UART_BaudRate;


typedef enum {
    UART_FIFOSize_4Byte = 0,    /*!< UART_FIFOSize_4Byte */
    UART_FIFOSize_8Byte,    	/*!< UART_FIFOSize_8Byte */
    UART_FIFOSize_16Byte,    	/*!< UART_FIFOSize_16Byte */
    UART_FIFOSize_32Byte,    	/*!< UART_FIFOSize_32Byte */
}UART_FIFOSize;

/* ===========================================  Regs  =========================================== */

#define		DEVICE_ASSERT

#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

/**
  * @brief Universal Asynchronous Receiver/Transmitter (UART)
  */
typedef struct {                                /*!< UART Structure                                             				*/
  __IOM uint32_t  MCR;                          /*!< (@ 0x00000000) Mode Control Register             							*/
  __IOM uint32_t  LSR;                          /*!< (@ 0x00000004) Line Status Register                        				*/
  __IOM uint32_t  RBR;                        	/*!< (@ 0x00000008) RX/TX Data Register                                			*/
} UART_Type;                                    /*!< Size = 12 (0x0E)                                                         	*/

#define UART_INSTANCE_MAX               (3UL)   /*!< Number of instances of the UART module                                    */

#define UART0                       ((UART_Type*)              UART0_BASE)
#define UART1                       ((UART_Type*)              UART1_BASE)
#define UART2                       ((UART_Type*)              UART2_BASE)

/* =========================================================================================================================== */
/* ================                                           UART                                            ================ */
/* =========================================================================================================================== */

/* ==========================================================  MCR  ========================================================== */
#define UART_MCR_DATEBIT_Pos          	 (0UL)                      /*!< DATEBIT (Bit 0~1)                                    */
#define UART_MCR_DATEBIT_Msk          	 (0x3UL)                    /*!< DATEBIT (Bitfield-Mask: 0x03)                        */
#define UART_MCR_STB_Pos          	  	 (2UL)                      /*!< STB (Bit 2~3)                                        */
#define UART_MCR_STB_Msk          	 	 (0xcUL)                    /*!< STB (Bitfield-Mask: 0x0c)                            */
#define UART_MCR_PEN_Pos                 (4UL)                     	/*!< PEN (Bit 4)                                          */
#define UART_MCR_PEN_Msk                 (0x10UL)                   /*!< PEN (Bitfield-Mask: 0x10)                            */
#define UART_MCR_EPS_Pos                 (5UL)                     	/*!< EPS (Bit 5)                                          */
#define UART_MCR_EPS_Msk                 (0x20UL)                  	/*!< EPS (Bitfield-Mask: 0x20)                            */
#define UART_MCR_FIFOE_Pos               (6UL)                      /*!< FIFOE (Bit 6)                                        */
#define UART_MCR_FIFOE_Msk               (0x40UL)                   /*!< FIFOE (Bitfield-Mask: 0x40)                          */
#define UART_MCR_LOOP_Pos                (7UL)                      /*!< LOOP (Bit 7)                                         */
#define UART_MCR_LOOP_Msk                (0x80UL)                   /*!< LOOP (Bitfield-Mask: 0x80)                           */
#define UART_MCR_BAUDRATE_Pos          	 (8UL)                     	/*!< BAUDRATE (Bit 8~11)                                  */
#define UART_MCR_BAUDRATE_Msk          	 (0xf00UL)                  /*!< BAUDRATE (Bitfield-Mask: 0xf00UL)                    */
#define UART_MCR_TXFIFOSIZE_Pos          (12UL)                 	/*!< TXFIFOSIZE (Bit 12~13)                               */
#define UART_MCR_TXFIFOSIZE_Msk          (0x3000UL)            		/*!< TXFIFOSIZE (Bitfield-Mask: 0x3000UL)                 */
#define UART_MCR_RXFIFOSIZE_Pos          (14UL)                 	/*!< RXFIFOSIZE (Bit 14~15)                               */
#define UART_MCR_RXFIFOSIZE_Msk          (0xc000UL)            		/*!< RXFIFOSIZE (Bitfield-Mask: 0xc000UL)                 */
#define UART_MCR_RTSCTSEN_Pos            (16UL)                     /*!< RTSCTSEN (Bit 16)                                    */
#define UART_MCR_RTSCTSEN_Msk            (0x10000UL)             	 /*!< RTSCTSEN (Bitfield-Mask: 0x10000UL)                 */
#define UART_MCR_AFCFEN_Pos              (17UL)                     /*!< RTSCTSEN (Bit 17)                                    */
#define UART_MCR_AFCFEN_Msk            	 (0x20000UL)             	 /*!< RTSCTSEN (Bitfield-Mask: 0x20000UL)                 */
#define UART_MCR_SETRTS_Pos              (18UL)                     /*!< RTSCTSEN (Bit 18)                                    */
#define UART_MCR_SETRTS_Msk            	 (0x40000UL)             	 /*!< RTSCTSEN (Bitfield-Mask: 0x40000UL)                 */
#define UART_MCR_RESET_Pos                (24UL)                     /*!< RESET (Bit 24)                                          */
#define UART_MCR_RESET_Msk                (0x01000000UL)             /*!< RESET (Bitfield-Mask: 0x01000000UL)                     */
#define UART_MCR_ERXNE_Pos               (27UL)                     /*!< ERXNE (Bit 27)                                       */
#define UART_MCR_ERXNE_Msk               (0x08000000UL)             /*!< ERXNE (Bitfield-Mask: 0x08000000UL)                  */
#define UART_MCR_ETXE_Pos                (28UL)                     /*!< ETXE (Bit 28)                                        */
#define UART_MCR_ETXE_Msk                (0x10000000UL)             /*!< ETXE (Bitfield-Mask: 0x10000000UL)                   */
#define UART_MCR_LSINTE_Pos              (29UL)                     /*!< LSINTE (Bit 31)                                      */
#define UART_MCR_LSINTE_Msk              (0x20000000UL)             /*!< LSINTE (Bitfield-Mask: 0x20000000UL)                 */
#define UART_MCR_FHFS_Pos                (30UL)                     /*!< FHFS (Bit 31)                                        */
#define UART_MCR_FHFS_Msk                (0x40000000UL)             /*!< FHFS (Bitfield-Mask: 0x40000000UL)                   */
#define UART_MCR_EN_Pos                	 (31UL)                     /*!< EN (Bit 31)                                          */
#define UART_MCR_EN_Msk                  (0x80000000UL)             /*!< EN (Bitfield-Mask: 0x80000000UL)                     */


/* =========================================================  LSR  ========================================================== */
#define UART_LSR_DR_Pos                  (0UL)                     /*!< DR (Bit 0)                                            */
#define UART_LSR_DR_Msk                  (0x1UL)                   /*!< DR (Bitfield-Mask: 0x01)                              */
#define UART_LSR_OE_Pos                  (1UL)                     /*!< OE (Bit 1)                                            */
#define UART_LSR_OE_Msk                  (0x2UL)                   /*!< OE (Bitfield-Mask: 0x01)                              */
#define UART_LSR_PE_Pos                  (2UL)                     /*!< PE (Bit 2)                                            */
#define UART_LSR_PE_Msk                  (0x4UL)                   /*!< PE (Bitfield-Mask: 0x01)                              */
#define UART_LSR_FE_Pos                  (3UL)                     /*!< FE (Bit 3)                                            */
#define UART_LSR_FE_Msk                  (0x8UL)                   /*!< FE (Bitfield-Mask: 0x01)                              */
#define UART_LSR_BI_Pos                  (4UL)                     /*!< BI (Bit 4)                                            */
#define UART_LSR_BI_Msk                  (0x10UL)                  /*!< BI (Bitfield-Mask: 0x01)                              */
#define UART_LSR_THRE_Pos                (5UL)                     /*!< THRE (Bit 5)                                          */
#define UART_LSR_THRE_Msk                (0x20UL)                  /*!< THRE (Bitfield-Mask: 0x01)                            */
#define UART_LSR_TC_Pos                  (6UL)                     /*!< TC (Bit 6)                                            */
#define UART_LSR_TC_Msk                  (0x40UL)                  /*!< TC (Bitfield-Mask: 0x01)                              */
#define UART_LSR_CTS_Pos                 (7UL)                     /*!< CTS (Bit 7)                                           */
#define UART_LSR_CTS_Msk                 (0x80UL)                  /*!< CTS (Bitfield-Mask: 0x80)                             */
#define UART_LSR_RXFIFOHF_Pos            (8UL)                     /*!< RXFIFOHF (Bit 8)                                      */
#define UART_LSR_RXFIFOHF_Msk            (0x100UL)                 /*!< RXFIFOHF (Bitfield-Mask: 0x100UL)					  */
#define UART_LSR_TXFIFOHF_Pos            (9UL)                     /*!< RXFIFOHF (Bit 9)                                      */
#define UART_LSR_TXFIFOHF_Msk            (0x200UL)                 /*!< RXFIFOHF (Bitfield-Mask: 0x200UL)					  */
#define UART_LSR_RXFIFOF_Pos             (10UL)                    /*!< RXFIFOF (Bit 10)                                      */
#define UART_LSR_RXFIFOF_Msk             (0x400UL)                 /*!< RXFIFOF (Bitfield-Mask: 0x400UL)					  */
#define UART_LSR_AFCI_Pos		         (11UL)                    /*!< RXFIFOF (Bit 11)                                      */
#define UART_LSR_AFCI_Msk             	 (0x800UL)                 /*!< RXFIFOF (Bitfield-Mask: 0x800UL)					  */

/* ==========================================================  RBR  ========================================================== */
#define UART_RBR_RBR_THR_Pos              (0UL)                     /*!< RBR_THR (Bit 0)                                       */
#define UART_RBR_RBR_THR_Msk              (0xffUL)                  /*!< RBR_THR (Bitfield-Mask: 0xff)                        */


/**
* @brief setting bits macro.
*/
#ifndef SET_BIT32
  #define SET_BIT32(reg, mask)              ((reg) |= (uint32_t)(mask))
#endif

/**
* @brief clearing bits macro.
*/
#ifndef CLEAR_BIT32
  #define CLEAR_BIT32(reg, mask)            ((reg) &= (~((uint32_t)(mask))))
#endif

/**
* @brief read bits macro.
*/
#ifndef READ_BIT32
  #define READ_BIT32(reg, mask)             ((reg) & ((uint32_t)(mask)))
#endif

/**
* @brief write register macro.
*/
#ifndef WRITE_REG32
  #define WRITE_REG32(reg, value)           ((reg) = (uint32_t)(value))
#endif

/**
* @brief clear bits and set with new value
*/
#ifndef MODIFY_REG32
  #define MODIFY_REG32(reg, mask, pos, value)  (WRITE_REG32((reg), (((reg) & (~((uint32_t)mask))) | (((uint32_t)value) << pos))))
#endif


typedef enum {FALSE = 0, TRUE = !FALSE} BOOL_Type;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} ACTION_Type;

typedef enum {ERROR = 0, SUCCESS = !ERROR} ERROR_Type;

typedef void (*DeviceCallback_Type)(void *device, uint32_t para);


/* ============================================  Define  ============================================ */
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE                        static __inline
#endif


#define UART_GUARDCNT_DEF           (0x10)

/*!
* @brief UART instance index macro.
*/
#define UART_INDEX(UARTx)    ((UART_IndexType)(((uint32_t)(UARTx) - UART0_BASE) >> 12))

/*!< Check function parameter: Is UART peripheral or not */
#define IS_UART_PERIPH(periph) (((periph) == UART0) || \
                                ((periph) == UART1) || \
                                ((periph) == UART2))

/*!< Check function parameter: Is UART baudrate or not?  sampleCnt_div: UARTn_SMP_CNT register division */
#define IS_UART_BAUDRATE(baudrate) (((baudrate) >= BaudRate_300bps) && ((baudrate) <= (BaudRate_1843200bps)))

/* ===========================================  Typedef  ============================================ */
typedef enum {
    UART0_INDEX = 0U,   /*!< UART0 index */
    UART1_INDEX,        /*!< UART1 index */
    UART2_INDEX,        /*!< UART2 index */
    UART_INDEX_MAX
}UART_IndexType;
/*!< Check function parameter: Is UART index or not */
#define IS_UART_INDEX(index) (((index) == UART0_INDEX) || \
                              ((index) == UART1_INDEX) || \
                              ((index) == UART2_INDEX))

typedef enum {
    UART_WORD_LEN_5BIT = 0U,   /*!< word length is 5 bits */
    UART_WORD_LEN_6BIT,        /*!< word length is 6 bits */
    UART_WORD_LEN_7BIT,        /*!< word length is 7 bits */
    UART_WORD_LEN_8BIT,        /*!< word length is 8 bits */
//    UART_WORD_LEN_9BIT         /*!< word length is 9 bits *///NOT SUPPORTED
}UART_WordLenType;
/*!< Check function parameter: Is UART word length or not */
#define IS_UART_WORD_LEN(wordLength) (((wordLength) == UART_WORD_LEN_5BIT) || \
                                      ((wordLength) == UART_WORD_LEN_6BIT) || \
                                      ((wordLength) == UART_WORD_LEN_7BIT) || \
                                      ((wordLength) == UART_WORD_LEN_8BIT))

typedef enum {
    UART_STOP_1BIT = 0U,      /*!< stop bits number: 1 bit */
    UART_STOP_1_5BIT,      	  /*!< stop bits number: 1.5 bit */
    UART_STOP_2BIT            /*!< stop bits number: 2 bit */
}UART_StopBitType;
/*!< Check function parameter: Is UART stop bits or not */
#define IS_UART_STOPBITS(stopBits) (((stopBits) == UART_STOP_1BIT) || \
									((stopBits) == UART_STOP_1_5BIT) || \
                                    ((stopBits) == UART_STOP_2BIT))

typedef enum {
    UART_PARI_NO = 0,        /*!< No parity */
    UART_PARI_ODD,           /*!< odd parity */
    UART_PARI_EVEN           /*!< even parity */
}UART_ParityType;
/*!< Check function parameter: Is UART parity or not */
#define IS_UART_PARITY(parity) (((parity) == UART_PARI_NO) || \
                                ((parity) == UART_PARI_ODD) || \
                                ((parity) == UART_PARI_EVEN))

typedef enum {
    UART_RTSCTS_NONE = 0,    /*!< RTS CTS: all disable */
//    UART_RTS_ONLY,           /*!< RTS noly */
//    UART_CTS_ONLY,           /*!< CTS noly */
    UART_RTSCTS_ALL         /*!< RTS CTS: all Enable */
}UART_RtsCtsType;

typedef enum {
    UART_DMA_TXRX_NONE = 0, /*!< UART DMA TX and RX: all disable */
    UART_DMA_RX_EN,         /*!< UART DMA RX enable */
    UART_DMA_TX_EN,         /*!< UART DMA TX enable */
    UART_DMA_TXRX_EN        /*!< UART DMA TX and RX: all enable */
}UART_DmaEnType;
/*!< Check function parameter: Is UART dma enable type or not */
#define IS_UART_DMAEN(dmaEn) (((dmaEn) == UART_DMA_TXRX_NONE) || \
                              ((dmaEn) == UART_DMA_RX_EN) || \
                              ((dmaEn) == UART_DMA_TX_EN) || \
                              ((dmaEn) == UART_DMA_TXRX_EN))

typedef struct {
    uint32_t baudrate;               /*!< uart baudrate */
    UART_WordLenType dataBits;       /*!< 5~9: select word length, 5~9 bits */
    UART_StopBitType stopBits;       /*!< 1: 1bit stop; 2: 2bits stop */
    UART_ParityType  parity;         /*!< 0: no parity; 1: odd; 2: even */
    ACTION_Type fifoByteEn;          /*!< 0: Disable; 1: Enable */
    UART_DmaEnType dmaEn;            /*!< 0: disable DMA;  1: DMA_RX Enable; 2: DMA_TX enable; 3: DMATX & DMARX enable */
    DeviceCallback_Type callBack;    /*!< IRQ Callback pointer */
}UART_ConfigType;


/* ==========================================  Variables  =========================================== */

/* ====================================  Functions declaration  ===================================== */
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
void UART_Init(UART_Type *UARTx, const UART_ConfigType *config);

/*!
* @brief UART uninitialize
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return none
*/
void UART_DeInit(UART_Type *UARTx);

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
void UART_SetBaudrate(UART_Type *UARTx, uint32_t baudrate);

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
UART_Type* UART_GetUARTCtrl(UART_IndexType uartIndex);

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
ERROR_Type UART_SendData(UART_Type *UARTx, uint16_t data);

/*!
* @brief  UART receive data by normal mode
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                         - UART0
*                         - UART1
*                         - UART2
* @return Get uart data
*/
uint16_t UART_ReceiveData(UART_Type *UARTx);

/*!
* @brief UART DMA transmit data
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                 - UART0
*                 - UART1
*                 - UART2
* @param[in] DMAx:DMA Channel type pointer,
*                 -DMA0_CHANNEL0
*                 -DMA0_CHANNEL1
*                 -DMA0_CHANNEL2
*                 -DMA0_CHANNEL3
* @param[in] txBuffer: point to the send data
* @param[in] length: transfmit data length by byte
*                    If UART is 8 bits, length must be 0~32767, because DMA CHAN_LENGTH is 0~32767
*                    If UART is 9 bits, length must be multiple of 2, because DMA CHAN_LENGTH is length/2,
*                    otherwise the last data cannot be transmit
* @param[in] callback: point to DMA callback function
* @return Function return state: SUCCESS/ERROR
*/
//ERROR_Type UART_TransmitDMA(UART_Type *UARTx, DMA_ChannelType *DMAx, \
//                                      uint8_t* txBuffer, uint16_t length, DeviceCallback_Type callback);

/*!
* @brief  UART DMA receive data
*
* @param[in] UARTx: UART type pointer, x value can be 0 to 2
*                 - UART0
*                 - UART1
*                 - UART2
* @param[in] DMAx:DMA Channel type pointer,
*                 -DMA0_CHANNEL0
*                 -DMA0_CHANNEL1
*                 -DMA0_CHANNEL2
*                 -DMA0_CHANNEL3
* @param[in] rxBuffer: point to the receive buffer
* @param[in] length: Receive data length by byte
*                    If UART is 8 bits, length must be 0~32767, because DMA CHAN_LENGTH is 0~32767
*                    If UART is 9 bits, length must be multiple of 2, because DMA CHAN_LENGTH is length/2,
*                    otherwise the last data cannot be receive
* @param[in] callback: point to DMA callback function
* @return Function return state: SUCCESS/ERROR
*/
//ERROR_Type UART_ReceiveDMA(UART_Type *UARTx, DMA_ChannelType *DMAx, \
//                                    uint8_t* rxBuffer, uint16_t length, DeviceCallback_Type callback);

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
void UART_SetParity(UART_Type *UARTx, UART_ParityType parity);

/*!
* @brief Set UART RTS CTS
*
* @param[in] UARTx: UART type pointer, x value should be 0, only UART0 has CTS/RTS function
*                         - UART0
* @param[in] RTSCTS: UART rtsCts, value can be 0 to 3
*                         -UART_RTSCTS_NONE：NO RTS or CTS
*                         -UART_RTS_ONLY: RTS only
*                         -UART_CTS_ONLY: CTS only
*                         -UART_RTSCTS_ALL: enable RTS and CTS
* @return none
*/
void UART_SetCTSRTS(UART_Type *UARTx, UART_RtsCtsType RTSCTS);

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
void UART_SetCallback(UART_Type *UARTx, const DeviceCallback_Type callback);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _UART_H */

/* =============================================  EOF  ============================================== */
