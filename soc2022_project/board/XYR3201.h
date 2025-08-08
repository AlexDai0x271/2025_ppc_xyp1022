/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __XYR3201_H
#define __XYR3201_H

#include "csi_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core_rv32.h"

/**
 * @brief XYR3201 Clock Frequency Definition
 */
#ifndef USE_QEMU_PLAT
#define CPU_CLOCK_MHZ                           ((uint32_t)1200)
#else
#define CPU_CLOCK_MHZ                           ((uint32_t)20)
#endif // #ifndef USE_QEMU_PLAT
#define CPU_CLOCK_KHZ                           ((uint32_t)(CPU_CLOCK_MHZ*1000))
#define CPU_CLOCK_HZ                            ((uint32_t)(CPU_CLOCK_MHZ*1000*1000))

#ifndef CONFIG_SYSTICK_MS
#define CONFIG_SYSTICK_MS                       ((uint32_t)1)
#endif

/**
 * @brief XYR3201 Data Type Definition
 */
#define __I                                     volatile const
#define __O                                     volatile
#define __IO                                    volatile

/**
 * @brief XYP1022 Memory Size Definition
 */

#define KiB(n)                                  ((n) * 1024UL)
#define MiB(n)                                  ((n) * 1024UL * 1024UL)

#include <stdint-gcc.h>
#include <string.h>

// For XShell >>>>>>>>

#define UINT64  unsigned long long 
#define UINT32  unsigned int
#define UINT16  unsigned short
#define UINT8	unsigned char
#define INT32   int
#define INT16   short
#define INT8 	char

typedef volatile unsigned       char  V_UINT8;
typedef volatile signed         char  V_INT8;
typedef volatile unsigned short int   V_UINT16;
typedef volatile signed   short int   V_INT16;
typedef volatile unsigned       int   V_UINT32;
typedef volatile signed         int   V_INT32;
typedef volatile float                V_FP32;

// <<<<<<<< For XShell

// For SpaceWire >>>>>>>>

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;
typedef volatile unsigned int V_U8;
typedef volatile unsigned int V_U16;
typedef volatile unsigned int V_U32;
typedef volatile unsigned int V_U64;

typedef int STATUS;

#define BSP_OK                                  0
#define BSP_ERROR                               -1

// <<<<<<<< For SpaceWire

/**
 * @brief XYR3201 Interrupt Number Definition
 */
typedef enum IRQn
{
    NMI_EXPn                                    = -2,

    Machine_Software_IRQn                       = 3,
    User_Timer_IRQn                             = 4,
    Supervisor_Timer_IRQn                       = 5,
    CORET_IRQn                                  = 7,
    Machine_External_IRQn                       = 11,

__IRQn_Ext_Start                                = 16,
    AHB_1e_IRQn                                 = __IRQn_Ext_Start,
    SRIO0_1e_IRQn                               = 17,
    AXI_1e_IRQn                                 = 18,
    CPU_1e_IRQn                                 = 19,
    AMC_1e_IRQn                                 = 20,
    SRIO0_IRQn                                  = 21,
    DMAC_IRQn                                   = 22,
    GMAC1_IRQn                                  = 23,
    GMAC0_IRQn                                  = 24,
    PWM5_IRQn                                   = 25,
    PWM4_IRQn                                   = 26,
    PWM3_IRQn                                   = 27,
    PWM2_IRQn                                   = 28,
    PWM1_IRQn                                   = 39,
    PWM0_IRQn                                   = 30,
    EPWM7_IRQn                                  = 31,
    EPWM6_IRQn                                  = 32,
    EPWM5_IRQn                                  = 33,
    EPWM4_IRQn                                  = 34,
    EPWM3_IRQn                                  = 35,
    EPWM2_IRQn                                  = 36,
    EPWM1_IRQn                                  = 37,
    EPWM0_IRQn                                  = 38,
    RTC_IRQn                                    = 39,
    SMC_IRQn                                    = 40,
    I2C1_IRQn                                   = 41,
    I2C0_IRQn                                   = 42,
    SPI7_IRQn                                   = 43,
    SPI6_IRQn                                   = 44,
    SPI5_IRQn                                   = 45,
    SPI4_IRQn                                   = 46,
    SPI3_IRQn                                   = 47,
    SPI2_IRQn                                   = 48,
    SPI1_IRQn                                   = 49,
    SPI0_IRQn                                   = 50,
    UART21_IRQn                                 = 51,
    UART20_IRQn                                 = 52,
    UART19_IRQn                                 = 53,
    UART18_IRQn                                 = 54,
    UART17_IRQn                                 = 55,
    UART16_IRQn                                 = 56,
    UART15_IRQn                                 = 57,
    UART14_IRQn                                 = 58,
    UART13_IRQn                                 = 59,
    UART12_IRQn                                 = 60,
    UART11_IRQn                                 = 61,
    UART10_IRQn                                 = 62,
    UART9_IRQn                                  = 63,
    UART8_IRQn                                  = 64,
    UART7_IRQn                                  = 65,
    UART6_IRQn                                  = 66,
    UART5_IRQn                                  = 67,
    UART4_IRQn                                  = 68,
    UART3_IRQn                                  = 69,
    UART2_IRQn                                  = 70,
    UART1_IRQn                                  = 71,
    UART0_IRQn                                  = 72,
    QSPI_IRQn                                   = 73,
    AMC_IRQn                                    = 74,
    TIMER1_IRQn                                 = 75,
    TIMER0_IRQn                                 = 76,
    SPW3_IRQn                                   = 77,
    SPW2_IRQn                                   = 78,
    SPW1_IRQn                                   = 79,
    SPW0_IRQn                                   = 80,
    CAN1_IRQn                                   = 81,
    CAN0_IRQn                                   = 82,
    BRM1553B1_IRQn                              = 83,
    BRM1553B0_IRQn                              = 84,
    GPIO15_IRQn                                 = 85,
    GPIO14_IRQn                                 = 86,
    GPIO13_IRQn                                 = 87,
    GPIO12_IRQn                                 = 88,
    GPIO11_IRQn                                 = 89,
    GPIO10_IRQn                                 = 90,
    GPIO9_IRQn                                  = 91,
    GPIO8_IRQn                                  = 92,
    GPIO7_IRQn                                  = 93,
    GPIO6_IRQn                                  = 94,
    GPIO5_IRQn                                  = 95,
    GPIO4_IRQn                                  = 96,
    GPIO3_IRQn                                  = 97,
    GPIO2_IRQn                                  = 98,
    GPIO1_IRQn                                  = 99,
    GPIO0_IRQn                                  = 100,
    APBG_BUS_2e_IRQn                            = 101,
    AHB_2e_IRQn                                 = 102,
    AXI_SRIO_2e_IRQn                            = 103,
    AXI_BUS_2e_IRQn                             = 104,
    DCACHE_2e_IRQn                              = 105,
    SMC_2e_IRQn                                 = 106,
    AMC_2e_IRQn                                 = 107,
    SRAM_2e_IRQn                                = 108,
    IRAM_2e_IRQn                                = 109,
__IRQn_Ext_End                                  = IRAM_2e_IRQn,

    IRQn_Max                                    = 128,
} IRQn_Type;

/**
  * @brief Timer
  */

typedef struct
{
    struct {
    __IO uint32_t LOADL;                        /*!< TIMER Timer N Load Count Register                                  Address offset:0x0      */
         uint32_t LOADH;
    __I  uint32_t COUNTL;                       /*!< TIMER Timer N Count Value Register                                 Address offset:0x8      */
         uint32_t COUNTH;
    __IO uint32_t CTRL;                         /*!< TIMER Timer N Control Register                                     Address offset:0x10     */
    __I  uint32_t EOI;                          /*!< TIMER Timer N End-of-Interrupt Register                            Address offset:0x14     */
    __I  uint32_t STATUS;                       /*!< TIMER Timer N Interrupt Status Register                            Address offset:0x18     */
    } ch[4];
         uint32_t reserve1[16];
    __I  uint32_t INTSTATUS;                    /*!< TIMER Timers Interrupt Status Register                             Address offset:0xb0     */
    __I  uint32_t INTEOI;                       /*!< TIMER Timers End-of-Interrupt Register                             Address offset:0xb4     */
    __I  uint32_t INTRAWSTATUS;                 /*!< TIMER Timers Raw Interrupt Status Register                         Address offset:0xb8     */
    __I  uint32_t VERSION;                      /*!< TIMER Timers Component Version Register                            Address offset:0xbc     */
    __IO uint32_t LOADCOUNT2[8];                /*!< TIMER Timer N Load Count Register 2                                Address offset:0xc0     */
    __IO uint32_t PROT_LEVEL[8];                /*!< TIMER Timer N Protection level Rgister                             Address offset:0xe0     */
} TIM_TypeDef;

/**
  * @brief Serial Peripheral Interface
  */

typedef struct
{
    __IO uint32_t CTRLR0;                       /*!< SPI Control Register 0                                             Address offset:0x0      */
    __IO uint32_t CTRLR1;                       /*!< SPI Control Register 1                                             Address offset:0x4      */
    __IO uint32_t SSIENR;                       /*!< SPI SSI Enable Register                                            Address offset:0x8      */
    __IO uint32_t MWCR;                         /*!< SPI Microwire Control Register                                     Address offset:0xc      */
    __IO uint32_t SER;                          /*!< SPI Slave Enable Register                                          Address offset:0x10     */
    __IO uint32_t BAUDR;                        /*!< SPI Baud Rate Select Register                                      Address offset:0x14     */
    __IO uint32_t TXFTLR;                       /*!< SPI Transmit FIFO Threshold Level Register                         Address offset:0x18     */
    __IO uint32_t RXFTLR;                       /*!< SPI Receive FIFO Threshold Level Register                          Address offset:0x1c     */
    __I  uint32_t TXFLR;                        /*!< SPI Transmit FIFO Level Register                                   Address offset:0x20     */
    __I  uint32_t RXFLR;                        /*!< SPI Receive FIFO Level Register                                    Address offset:0x24     */
    __I  uint32_t SR;                           /*!< SPI Status Register                                                Address offset:0x28     */
    __IO uint32_t IMR;                          /*!< SPI Interrupt Mask Register                                        Address offset:0x2c     */
    __I  uint32_t ISR;                          /*!< SPI Interrupt Status Register                                      Address offset:0x30     */
    __I  uint32_t RISR;                         /*!< SPI Raw Interrupt Status Register                                  Address offset:0x34     */
    __I  uint32_t TXOICR;                       /*!< SPI Transmit FIFO Overflow Interrupt Clear Register                Address offset:0x38     */
    __I  uint32_t RXOICR;                       /*!< SPI Receive FIFO Overflow Interrupt Clear Register                 Address offset:0x3c     */
    __I  uint32_t RXUICR;                       /*!< SPI Receive FIFO Underflow Interrupt Clear Register                Address offset:0x40     */
    __I  uint32_t MSTICR;                       /*!< SPI Multi-Master Interrupt Clear Register                          Address offset:0x44     */
    __I  uint32_t ICR;                          /*!< SPI Interrupt Clear Register                                       Address offset:0x48     */
    __IO uint32_t DMACR;                        /*!< SPI DMA Control Register                                           Address offset:0x4c     */
    __IO uint32_t DMATDLR;                      /*!< SPI DMA Transmit Data Level Register                               Address offset:0x50     */
    __IO uint32_t DMARDLR;                      /*!< SPI DMA Receive Data Level Register                                Address offset:0x54     */
    __I  uint32_t IDR;                          /*!< SPI Identification Register                                        Address offset:0x58     */
    __I  uint32_t SSIC_VERSION_ID;              /*!< SPI SPI Version Register                                           Address offset:0x5c     */
    __IO uint32_t DR[36];                       /*!< SPI Data Register                                                  Address offset:0x60     */
    __IO uint32_t RX_SAMPLE_DELAY;              /*!< SPI RX Sample Delay Register                                       Address offset:0xf0     */
    __IO uint32_t SPI_CTRLR0;                   /*!< SPI SPI Control Register                                           Address offset:0xf4     */
    __IO uint32_t DDR_DRIVE_EDGE;               /*!< SPI Transmit Drive Edge Register                                   Address offset:0xf8     */
    __IO uint32_t XIP_MODE_BITS;                /*!< SPI Mode Bits Register                                             Address offset:0xfc     */
} SPI_TypeDef;

/**
  * @brief Universal Asynchronous Receiver Transmitter
  */

typedef struct
{
    union {
    __I  uint32_t RBR;                          /*!< UART Receive Buffer Register                                       Address offset:0x0      */
    __IO uint32_t DLL;                          /*!< UART Divisor Latch Low Register                                    Address offset:0x0      */
    __O  uint32_t THR;                          /*!< UART Transmit Holding Register                                     Address offset:0x0      */
    };
    union {
    __IO uint32_t DLH;                          /*!< UART Divisor Latch High Register                                   Address offset:0x4      */
    __IO uint32_t IER;                          /*!< UART Interrupt Enable Register                                     Address offset:0x4      */
    };
    union {
    __O  uint32_t FCR;                          /*!< UART FIFO Control Register                                         Address offset:0x8      */
    __I  uint32_t IIR;                          /*!< UART Interrupt Identification Register                             Address offset:0x8      */
    };
    __IO uint32_t LCR;                          /*!< UART Line Control Register                                         Address offset:0xc      */
    __IO uint32_t MCR;                          /*!< UART Modem Control Register                                        Address offset:0x10     */
    __I  uint32_t LSR;                          /*!< UART Line Status Register                                          Address offset:0x14     */
    __I  uint32_t MSR;                          /*!< UART Modem Status Register                                         Address offset:0x18     */
    __IO uint32_t SCR;                          /*!< UART Scratchpad Register                                           Address offset:0x1c     */
    __IO uint32_t LPDLL;                        /*!< UART Low Power Divisor Latch Low Register                          Address offset:0x20     */
    __IO uint32_t LPDLH;                        /*!< UART Low Power Divisor Latch High Register                         Address offset:0x24     */
         uint32_t reserve1[2];
    union {
    __I  uint32_t SPBR[16];                     /*!< UART Shadow Receive Buffer Register                                Address offset:0x30     */
    __O  uint32_t STHR[16];                     /*!< UART Shadow Transmit Holding Register                              Address offset:0x30     */
    };
    __IO uint32_t FAR;                          /*!< UART FIFO Access Register                                          Address offset:0x70     */
    __I  uint32_t TFR;                          /*!< UART Transmit FIFO Read Register                                   Address offset:0x74     */
    __O  uint32_t RFW;                          /*!< UART Receive FIFO Write Register                                   Address offset:0x78     */
    __I  uint32_t USR;                          /*!< UART Status Register                                               Address offset:0x7c     */
    __I  uint32_t TFL;                          /*!< UART Transmit FIFO Level Register                                  Address offset:0x80     */
    __I  uint32_t RFL;                          /*!< UART Receive FIFO Level Register                                   Address offset:0x84     */
    __IO uint32_t SRR;                          /*!< UART Software Reset Register                                       Address offset:0x88     */
    __IO uint32_t SRTS;                         /*!< UART Shadow Request to Send Register                               Address offset:0x8c     */
    __IO uint32_t SBCR;                         /*!< UART Shadow Break Control Register                                 Address offset:0x90     */
    __IO uint32_t SDMAM;                        /*!< UART Shadow DMA Mode Register                                      Address offset:0x94     */
    __IO uint32_t SFE;                          /*!< UART Shadow Shadow FIFO Enable Register                            Address offset:0x98     */
    __IO uint32_t SRT;                          /*!< UART Shadow RCVR Trigger Register                                  Address offset:0x9c     */
    __IO uint32_t STET;                         /*!< UART Shadow TX Empty Trigger Register                              Address offset:0xa0     */
    __IO uint32_t HTX;                          /*!< UART Halt TX Register                                              Address offset:0xa4     */
    __IO uint32_t DMASA;                        /*!< UART DMA Software Acknowledge Register                             Address offset:0xa8     */
    __IO uint32_t TCR;                          /*!< UART Transceiver Control Register                                  Address offset:0xac     */
    __IO uint32_t DE_EN;                        /*!< UART Driver Output Enable Register                                 Address offset:0xb0     */
    __IO uint32_t RE_EN;                        /*!< UART Receiver Output Enable Register                               Address offset:0xb4     */
    __IO uint32_t DET;                          /*!< UART Driver Output Enable Timing Register                          Address offset:0xb8     */
    __IO uint32_t TAT;                          /*!< UART TurnAround Timing Register                                    Address offset:0xbc     */
    __IO uint32_t DLF;                          /*!< UART Divisor Latch Fraction Register                               Address offset:0xc0     */
    __IO uint32_t RAR;                          /*!< UART Receive Address Register                                      Address offset:0xc4     */
    __IO uint32_t TAR;                          /*!< UART Transmit Address Register                                     Address offset:0xc8     */
    __IO uint32_t LCR_EXT;                      /*!< UART Line Extended Control Register                                Address offset:0xcc     */
    __IO uint32_t UART_PROT_LEVEL;              /*!< UART Protection Level Register                                     Address offset:0xd0     */
    __IO uint32_t REG_TIMEOUT_RST;              /*!< UART Register Timeout Counter Reset Register                       Address offset:0xd4     */
         uint32_t reserve2[7];
    __I  uint32_t CPR;                          /*!< UART Component Parameter Register                                  Address offset:0xf4     */
    __I  uint32_t UCV;                          /*!< UART Component Version Register                                    Address offset:0xf8     */
    __I  uint32_t CTR;                          /*!< UART Component Type Register                                       Address offset:0xfc     */
} UART_TypeDef;

/**
  * @brief Watchdog Timer
  */

typedef struct
{
    __IO uint32_t CR;                           /*!< WDT Control Register                                               Address offset:0x0      */
    __IO uint32_t TORR;                         /*!< WDT Timeout Range Register                                         Address offset:0x4      */
    __I  uint32_t CCVR;                         /*!< WDT Current Counter Value Register                                 Address offset:0x8      */
    __O  uint32_t CRR;                          /*!< WDT Counter Restart Register                                       Address offset:0xc      */
    __I  uint32_t STAT;                         /*!< WDT Interrupt Status Register                                      Address offset:0x10     */
    __I  uint32_t EOI;                          /*!< WDT Interrupt Clear Register                                       Address offset:0x14     */
         uint32_t reserve1;
    __IO uint32_t PROT_LEVEL;                   /*!< WDT WDT Protection level                                           Address offset:0x1c     */
         uint32_t reserve2[49];
    __I  uint32_t COMP_PARAM_5;                 /*!< WDT Component Parameters Register 5                                Address offset:0xe4     */
    __I  uint32_t COMP_PARAM_4;                 /*!< WDT Component Parameters Register 4                                Address offset:0xe8     */
    __I  uint32_t COMP_PARAM_3;                 /*!< WDT Component Parameters Register 3                                Address offset:0xec     */
    __I  uint32_t COMP_PARAM_2;                 /*!< WDT Component Parameters Register 2                                Address offset:0xf0     */
    __I  uint32_t COMP_PARAM_1;                 /*!< WDT Component Parameters Register 1                                Address offset:0xf4     */
    __IO uint32_t COMP_VERSION;                 /*!< WDT Component Version Register                                     Address offset:0xf8     */
    __IO uint32_t COMP_TYPE;                    /*!< WDT Component Type Register                                        Address offset:0xfc     */
} WDT_TypeDef;

/**
  * @brief General Purpose Input/Output
  */

typedef struct
{
    __IO uint32_t PORTA_DR;                     /*!< GPIO Port A Data Register                                          Address offset:0x0      */
    __IO uint32_t PORTA_DDR;                    /*!< GPIO Port A Data Direction Register                                Address offset:0x4      */
    __IO uint32_t PORTA_CTL;                    /*!< GPIO Port A data Source Register                                   Address offset:0x8      */
         uint32_t reserve1[9];
    __IO uint32_t INTEN;                        /*!< GPIO Interrupt Enable Register                                     Address offset:0x30     */
    __IO uint32_t INTMASK;                      /*!< GPIO Interrupt Mask Register                                       Address offset:0x34     */
    __IO uint32_t INTTYPE_LEVEL;                /*!< GPIO Interrupt Level Register                                      Address offset:0x38     */
    __IO uint32_t INT_POLARITY;                 /*!< GPIO Interrupt Polarity Register                                   Address offset:0x3c     */
    __I  uint32_t INTSTATUS;                    /*!< GPIO Interrupt Status Register                                     Address offset:0x40     */
    __I  uint32_t RAW_INTSTATUS;                /*!< GPIO Raw Interrupt Status Register                                 Address offset:0x44     */
    __IO uint32_t DEBOUNCE;                     /*!< GPIO Debounce Enable Register                                      Address offset:0x48     */
    __IO uint32_t PORTA_EOI;                    /*!< GPIO Clear Interrupt Register                                      Address offset:0x4c     */
    __I  uint32_t EXT_PORTA;                    /*!< GPIO External Port A Register                                      Address offset:0x50     */
         uint32_t reserve2[3];
    __IO uint32_t LS_SYNC;                      /*!< GPIO Synchronization Level Register                                Address offset:0x60     */
    __I  uint32_t ID_CODE;                      /*!< GPIO ID Code Register                                              Address offset:0x64     */
    __IO uint32_t INT_BOTHEDGE;                 /*!< GPIO Interrupt Both Edge Type Register                             Address offset:0x68     */
    __I  uint32_t VER_ID_CODE;                  /*!< GPIO GPIO Component Version Register                               Address offset:0x6c     */
    __I  uint32_t CONFIG_REG2;                  /*!< GPIO GPIO Configuration Register 2                                 Address offset:0x70     */
    __I  uint32_t CONFIG_REG1;                  /*!< GPIO GPIO Configuration Register 1                                 Address offset:0x74     */
} GPIO_TypeDef;

/**
  * @brief Inter-Integrated Circuit
  */

typedef struct
{
    __IO uint32_t CON;                          /*!< I2C Control Register                                               Address offset:0x0      */
    __IO uint32_t TAR;                          /*!< I2C Target Address Register                                        Address offset:0x4      */
    __IO uint32_t SAR;                          /*!< I2C Slave Address Register                                         Address offset:0x8      */
    __IO uint32_t HS_MADDR;                     /*!< I2C High Speed Master Mode Code Address Register                   Address offset:0xc      */
    __IO uint32_t DATA_CMD;                     /*!< I2C Rx/Tx Data Buffer and Command Register                         Address offset:0x10     */
    union {
    __IO uint32_t SS_SCL_HCNT;                  /*!< I2C Standard Speed I2C Clock SCL High Count Register               Address offset:0x14     */
    __IO uint32_t UFM_SCL_HCNT;                 /*!< I2C Ultra-Fast Speed I2C Clock SCL High Count Register             Address offset:0x14     */
    };
    union {
    __IO uint32_t SS_SCL_LCNT;                  /*!< I2C Standard Speed I2C Clock SCL Low Count Register                Address offset:0x18     */
    __IO uint32_t UFM_SCL_LCNT;                 /*!< I2C Ultra-Fast Speed I2C Clock SCL Low Count Register              Address offset:0x18     */
    };
    union {
    __IO uint32_t FS_SCL_HCNT;                  /*!< I2C Fast Speed I2C Clock SCL High Count Register                   Address offset:0x1c     */
    __IO uint32_t UFM_TBUF_CNT;                 /*!< I2C Ultra-Fast Speed TBuf Idle Count Register                      Address offset:0x1c     */
    };
    __IO uint32_t FS_SCL_LCNT;                  /*!< I2C Fast Speed I2C Clock SCL Low Count Register                    Address offset:0x20     */
    __IO uint32_t HS_SCL_HCNT;                  /*!< I2C High Speed I2C Clock SCL High Count Register                   Address offset:0x24     */
    __IO uint32_t HS_SCL_LCNT;                  /*!< I2C High Speed I2C Clock SCL Low Count Register                    Address offset:0x28     */
    __I  uint32_t INTR_STAT;                    /*!< I2C Interrupt Status Register                                      Address offset:0x2c     */
    __IO uint32_t INTR_MASK;                    /*!< I2C Interrupt Mask Register                                        Address offset:0x30     */
    __I  uint32_t RAW_INTR_STAT;                /*!< I2C Raw Interrupt Status Register                                  Address offset:0x34     */
    __IO uint32_t RX_TL;                        /*!< I2C Receive FIFO Threshold Register                                Address offset:0x38     */
    __IO uint32_t TX_TL;                        /*!< I2C Transmit FIFO Threshold Register                               Address offset:0x3c     */
    __I  uint32_t CLR_INTR;                     /*!< I2C Clear Combined and Individual Interrupt Register               Address offset:0x40     */
    __I  uint32_t CLR_RX_UNDER;                 /*!< I2C Clear RX_UNDER Interrupt Register                              Address offset:0x44     */
    __I  uint32_t CLR_RX_OVER;                  /*!< I2C Clear RX_OVER Interrupt Register                               Address offset:0x48     */
    __I  uint32_t CLR_TX_OVER;                  /*!< I2C Clear TX_OVER Interrupt Register                               Address offset:0x4c     */
    __I  uint32_t CLR_RD_REQ;                   /*!< I2C Clear RD_REQ Interrupt Register                                Address offset:0x50     */
    __I  uint32_t CLR_TX_ABRT;                  /*!< I2C Clear TX_ABRT Interrupt Register                               Address offset:0x54     */
    __I  uint32_t CLR_RX_DONE;                  /*!< I2C Clear RX_DONE Interrupt Register                               Address offset:0x58     */
    __I  uint32_t CLR_ACTIVITY;                 /*!< I2C Clear ACTIVITY Interrupt Register                              Address offset:0x5c     */
    __I  uint32_t CLR_STOP_DET;                 /*!< I2C Clear STOP_DET Interrupt Register                              Address offset:0x60     */
    __I  uint32_t CLR_START_DET;                /*!< I2C Clear START_DET Interrupt Register                             Address offset:0x64     */
    __I  uint32_t CLR_GEN_CALL;                 /*!< I2C Clear GEN_CALL Interrupt Register                              Address offset:0x68     */
    __IO uint32_t ENABLE;                       /*!< I2C Enable Register                                                Address offset:0x6c     */
    __I  uint32_t STATUS;                       /*!< I2C Status Register                                                Address offset:0x70     */
    __I  uint32_t TXFLR;                        /*!< I2C Transmit FIFO Level Register                                   Address offset:0x74     */
    __I  uint32_t RXFLR;                        /*!< I2C Receive FIFO Level Register                                    Address offset:0x78     */
    __IO uint32_t SDA_HOLD;                     /*!< I2C SDA Hold Time Length Register                                  Address offset:0x7c     */
    __I  uint32_t TX_ABRT_SOURCE;               /*!< I2C Transmit Abort Source Register                                 Address offset:0x80     */
    __IO uint32_t SLV_DATA_NACK_ONLY;           /*!< I2C Generate Slave Data NACK Register                              Address offset:0x84     */
    __IO uint32_t DMA_CR;                       /*!< I2C DMA Control Register                                           Address offset:0x88     */
    __IO uint32_t DMA_TDLR;                     /*!< I2C DMA Transmit Data Level Register                               Address offset:0x8c     */
    __IO uint32_t DMA_RDLR;                     /*!< I2C Receive Data Level Register                                    Address offset:0x90     */
    __IO uint32_t SDA_SETUP;                    /*!< I2C SDA Setup Register                                             Address offset:0x94     */
    __IO uint32_t ACK_GENERAL_CALL;             /*!< I2C ACK General Call Register                                      Address offset:0x98     */
    __I  uint32_t ENABLE_STATUS;                /*!< I2C Enable Status Register                                         Address offset:0x9c     */
    union {
    __IO uint32_t FS_SPKLEN;                    /*!< I2C I2C/SS/FS/FM+ Spike Suppression Limit Register                 Address offset:0xa0     */
    __IO uint32_t UFM_SPKLEN;                   /*!< I2C Ultra-Fast Spike Suppression Limit Register                    Address offset:0xa0     */
    };
    __IO uint32_t HS_SPKLEN;                    /*!< I2C HS Spike Suppression Limit Register                            Address offset:0xa4     */
    __I  uint32_t CLR_RESTART_DET;              /*!< I2C Clear RESTART_DET Interrupt Register                           Address offset:0xa8     */
    __IO uint32_t SCL_STUCK_AT_LOW_TIMEOUT;     /*!< I2C SCL Stuck at Low Timeout Register                              Address offset:0xac     */
    __IO uint32_t SDA_STUCK_AT_LOW_TIMEOUT;     /*!< I2C SDA Stuck at Low Timeout Register                              Address offset:0xb0     */
    __I  uint32_t CLR_SCL_STUCK_DET;            /*!< I2C Clear SCL Stuck at Low Detect Interrupt Register               Address offset:0xb4     */
    __I  uint32_t DEVICE_ID;                    /*!< I2C Device-ID Register                                             Address offset:0xb8     */
    __IO uint32_t SMBUS_CLK_LOW_SEXT;           /*!< I2C SMBus Slave Clock Extend Timeout Register                      Address offset:0xbc     */
    __IO uint32_t SMBUS_CLK_LOW_MEXT;           /*!< I2C SMBus Master Clock Extend Timeout Register                     Address offset:0xc0     */
    __IO uint32_t SMBUS_THIGH_MAX_IDLE_COUNT;   /*!< I2C SMBus Master THigh MAX Bus-idle Count Register                 Address offset:0xc4     */
    __I  uint32_t SMBUS_INTR_STAT;              /*!< I2C SMBUS Interrupt Status Register                                Address offset:0xc8     */
    __IO uint32_t SMBUS_INTR_MASK ;             /*!< I2C SMBus Interrupt Mask Register                                  Address offset:0xcc     */
    __I  uint32_t SMBUS_RAW_INTR_STAT;          /*!< I2C SMBus Raw Interrupt Status Register                            Address offset:0xd0     */
    __O  uint32_t CLR_SMBUS_INTR;               /*!< I2C SMBus Clear Interrupt Register                                 Address offset:0xd4     */
    __IO uint32_t OPTIONAL_SAR;                 /*!< I2C Optional Slave Address Register                                Address offset:0xd8     */
    union {
    __IO uint32_t SMBUS_UDID_LSB;               /*!< I2C SMBUS ARP UDID LSB Register                                    Address offset:0xdc     */
    __IO uint32_t SMBUS_UDID_WORD0;             /*!< I2C SMBUS UDID WORD0 Register                                      Address offset:0xdc     */
    };
    __IO uint32_t SMBUS_UDID_WORD1;             /*!< I2C SMBUS UDID WORD1 Register                                      Address offset:0xe0     */
    __IO uint32_t SMBUS_UDID_WORD2;             /*!< I2C SMBUS UDID WORD2 Register                                      Address offset:0xe4     */
    __IO uint32_t SMBUS_UDID_WORD3;             /*!< I2C SMBUS UDID WORD3 Register                                      Address offset:0xe8     */
         uint32_t reserve1;
    __IO uint32_t REG_TIMEOUT_RST;              /*!< I2C Register Timeout Counter Reset ValueRegister                   Address offset:0xf0     */
    __I  uint32_t COMP_PARAM_1;                 /*!< I2C Component Parameter Register 1                                 Address offset:0xf4     */
    __I  uint32_t COMP_VERSION;                 /*!< I2C Component Version Register                                     Address offset:0xf8     */
    __I  uint32_t COMP_TYPE;                    /*!< I2C Component Type Register                                        Address offset:0xfc     */
} I2C_TypeDef;

/**
  * @brief Quad Serial Peripheral Interface
  */

// #define QSPI_SSIC_SPI_BRIDGE

typedef struct
{
#ifdef QSPI_SSIC_SPI_BRIDGE
    __IO uint32_t CTRLR0;                       /*!< QSPI Control Register 0                                            Address offset:0x0      */
         uint32_t reserve1;
    __IO uint32_t SSIENR;                       /*!< QSPI SSI Enable Register                                           Address offset:0x8      */
         uint32_t reserve2[2];
    __IO uint32_t RXFBTR;                       /*!< QSPI Receive FIFO Burst Threshold Register                         Address offset:0x14     */
    __IO uint32_t TXFTLR;                       /*!< QSPI Transmit FIFO Threshold Level Register                        Address offset:0x18     */
    __IO uint32_t RXFTLR;                       /*!< QSPI Receive FIFO Threshold Level Register                         Address offset:0x1c     */
         uint32_t reserve3[2];
    __I  uint32_t SR;                           /*!< QSPI Status Register                                               Address offset:0x28     */
    __IO uint32_t IMR;                          /*!< QSPI Interrupt Mask Register                                       Address offset:0x2c     */
    __I  uint32_t ISR;                          /*!< QSPI Interrupt Status Register                                     Address offset:0x30     */
    __I  uint32_t RISR;                         /*!< QSPI Raw Interrupt Status Register                                 Address offset:0x34     */
    __I  uint32_t TXUICR;                       /*!< QSPI Transmit FIFO Underflow Interrupt Clear Register              Address offset:0x38     */
    __I  uint32_t RXOICR;                       /*!< QSPI Receive FIFO Overflow Interrupt Clear Register                Address offset:0x3c     */
    __I  uint32_t SPIMECR;                      /*!< QSPI SPI Master Error interrupt Clear Register                     Address offset:0x40     */
    __I  uint32_t AHBECR;                       /*!< QSPI AHB Error Clear Register                                      Address offset:0x44     */
    __I  uint32_t ICR;                          /*!< QSPI Interrupt Clear Register                                      Address offset:0x48     */
         uint32_t reserve4[3];
    __I  uint32_t IDR;                          /*!< QSPI Identification Register                                       Address offset:0x58     */
    __I  uint32_t SSIC_VERSION_ID;              /*!< QSPI SPI Version Register                                          Address offset:0x5c     */
         uint32_t reserve5[40];
#else
    __IO uint32_t CTRLR0;                       /*!< QSPI Control Register 0                                            Address offset:0x0      */
    __IO uint32_t CTRLR1;                       /*!< QSPI Control Register 1                                            Address offset:0x4      */
    __IO uint32_t SSIENR;                       /*!< QSPI SSI Enable Register                                           Address offset:0x8      */
    __IO uint32_t MWCR;                         /*!< QSPI Microwire Control Register                                    Address offset:0xc      */
    __IO uint32_t SER;                          /*!< QSPI Slave Enable Register                                         Address offset:0x10     */
    __IO uint32_t BAUDR;                        /*!< QSPI Baud Rate Select Register                                     Address offset:0x14     */
    __IO uint32_t TXFTLR;                       /*!< QSPI Transmit FIFO Threshold Level Register                        Address offset:0x18     */
    __IO uint32_t RXFTLR;                       /*!< QSPI Receive FIFO Threshold Level Register                         Address offset:0x1c     */
    __I  uint32_t TXFLR;                        /*!< QSPI Transmit FIFO Level Register                                  Address offset:0x20     */
    __I  uint32_t RXFLR;                        /*!< QSPI Receive FIFO Level Register                                   Address offset:0x24     */
    __I  uint32_t SR;                           /*!< QSPI Status Register                                               Address offset:0x28     */
    __IO uint32_t IMR;                          /*!< QSPI Interrupt Mask Register                                       Address offset:0x2c     */
    __I  uint32_t ISR;                          /*!< QSPI Interrupt Status Register                                     Address offset:0x30     */
    __I  uint32_t RISR;                         /*!< QSPI Raw Interrupt Status Register                                 Address offset:0x34     */
    __I  uint32_t TXOICR;                       /*!< QSPI Transmit FIFO Overflow Interrupt Clear Register               Address offset:0x38     */
    __I  uint32_t RXOICR;                       /*!< QSPI Receive FIFO Overflow Interrupt Clear Register                Address offset:0x3c     */
    __I  uint32_t RXUICR;                       /*!< QSPI Receive FIFO Underflow Interrupt Clear Register               Address offset:0x40     */
    __I  uint32_t MSTICR;                       /*!< QSPI Multi-Master Interrupt Clear Register                         Address offset:0x44     */
    __I  uint32_t ICR;                          /*!< QSPI Interrupt Clear Register                                      Address offset:0x48     */
    __IO uint32_t DMACR;                        /*!< QSPI DMA Control Register                                          Address offset:0x4c     */
    __IO uint32_t DMATDLR;                      /*!< QSPI DMA Transmit Data Level Register                              Address offset:0x50     */
    __IO uint32_t DMARDLR;                      /*!< QSPI DMA Receive Data Level Register                               Address offset:0x54     */
    __I  uint32_t IDR;                          /*!< QSPI Identification Register                                       Address offset:0x58     */
    __I  uint32_t SSIC_VERSION_ID;              /*!< QSPI SPI Version Register                                          Address offset:0x5c     */
    __IO uint32_t DR[36];                       /*!< QSPI Data Register                                                 Address offset:0x60     */
    __IO uint32_t RX_SAMPLE_DELAY;              /*!< QSPI RX Sample Delay Register                                      Address offset:0xf0     */
    __IO uint32_t SPI_CTRLR0;                   /*!< QSPI SPI Control Register                                          Address offset:0xf4     */
    __IO uint32_t DDR_DRIVE_EDGE;               /*!< QSPI Transmit Drive Edge Register                                  Address offset:0xf8     */
    __IO uint32_t XIP_MODE_BITS;                /*!< QSPI Mode Bits Register                                            Address offset:0xfc     */
#endif // #ifndef QSPI_SSIC_SPI_BRIDGE
    __IO uint32_t XIP_INCR_INST;                /*!< XIP INCR Transfer Opcode Register                                  Address offset:0x100    */
    __IO uint32_t XIP_WRAP_INST;                /*!< XIP WRAP Transfer Opcode Register                                  Address offset:0x104    */
    __IO uint32_t XIP_CTRL;                     /*!< XIP Control Register                                               Address offset:0x108    */
    __IO uint32_t XIP_SER;                      /*!< XIP Slave Enable Register                                          Address offset:0x10c    */
    __I  uint32_t XRXOICR;                      /*!< XIP Receive FIFO Overflow Interrupt Clear Register                 Address offset:0x110    */
    __IO uint32_t XIP_CNT_TIME_OUT;             /*!< XIP Timeout Register                                               Address offset:0x114    */
} QSPI_TypeDef;

/**
  * @brief Pulse Width Modulator
  */

typedef struct
{
    __IO uint32_t PPR;                          /*!< PWM Pre-scale Register                                             Address offset:0x0      */
    __IO uint32_t DZ;                           /*!< PWM DZ Register                                                    Address offset:0x4      */
    __IO uint32_t PCSR;                         /*!< PWM Clock Select Register                                          Address offset:0x8      */
    __IO uint32_t PCR;                          /*!< PWM Control Register                                               Address offset:0xc      */
    __IO uint32_t PCR1;                         /*!< PWM Control Register 1                                             Address offset:0x10     */
    struct {
    __IO uint32_t PCNR;                         /*!< PWM Counter Register                                               Address offset:0x14     */
    __IO uint32_t PCMR;                         /*!< PWM Comparator Register                                            Address offset:0x18     */
    __I  uint32_t PTR;                          /*!< PWM Timer Register                                                 Address offset:0x1c     */
    } chn[4];
    __IO uint32_t PIER;                         /*!< PWM Interrupt Enable Register                                      Address offset:0x44     */
    __I  uint32_t PIFR;                         /*!< PWM Interrupt Flag Register                                        Address offset:0x48     */
    __IO uint32_t PCCR[4];                      /*!< PWM Capture Control Register                                       Address offset:0x4c     */
    __I  uint32_t PCRLR0;                       /*!< PWM Capture Rising Latch Register 0                                Address offset:0x5c     */
    __I  uint32_t PCFLR0;                       /*!< PWM Capture Falling Latch Register 0                               Address offset:0x60     */
    __I  uint32_t PCRLR1;                       /*!< PWM Capture Rising Latch Register 1                                Address offset:0x64     */
    __I  uint32_t PCFLR1;                       /*!< PWM Capture Falling Latch Register 1                               Address offset:0x68     */
    __I  uint32_t PCRLR2;                       /*!< PWM Capture Rising Latch Register 2                                Address offset:0x6c     */
    __I  uint32_t PCFLR2;                       /*!< PWM Capture Falling Latch Register 2                               Address offset:0x70     */
    __I  uint32_t PCRLR3;                       /*!< PWM Capture Rising Latch Register 3                                Address offset:0x74     */
    __I  uint32_t PCFLR3;                       /*!< PWM Capture Falling Latch Register 3                               Address offset:0x78     */
    __IO uint32_t PPCR;                         /*!< PWM Port Control Register                                          Address offset:0x7c     */
    __IO uint32_t PDCR[8];                      /*!< PWM Delay Count Register                                           Address offset:0x88     */
    __IO uint32_t PDER;                         /*!< PWM Delay Enable Register                                          Address offset:0xa8     */
} PWM_TypeDef;

/**
  * @brief Enhanced Pulse Width Modulator
  */

typedef struct
{
    __IO uint32_t CR1;                          /*!< EPWM Control Register 1                                            Address offset:0x0      */
    __IO uint32_t CR2;                          /*!< EPWM Control Register 2                                            Address offset:0x4      */
    __IO uint32_t SMCR;                         /*!< EPWM Slave Mode Control Register                                   Address offset:0x8      */
    __IO uint32_t DIER;                         /*!< EPWM DMA/Interrupt Enable Register                                 Address offset:0xc      */
    __IO uint32_t SR;                           /*!< EPWM Status Register                                               Address offset:0x10     */
    __IO uint32_t EGR;                          /*!< EPWM Event Generation Register                                     Address offset:0x14     */
    __IO uint32_t CCMR[2];                      /*!< EPWM Capture/Compare Mode Register                                 Address offset:0x18     */
    __IO uint32_t CCER;                         /*!< EPWM Capture/Compare Enable Register                               Address offset:0x20     */
    __IO uint32_t CNT;                          /*!< EPWM Counter Register                                              Address offset:0x24     */
    __IO uint32_t PSC;                          /*!< EPWM Prescaler Register                                            Address offset:0x28     */
    __IO uint32_t ARR;                          /*!< EPWM Auto-reload Register                                          Address offset:0x2c     */
    __IO uint32_t RCR;                          /*!< EPWM Repetition Counter Register                                   Address offset:0x30     */
    __IO uint32_t CCR[4];                       /*!< EPWM Capture/Compare Register                                      Address offset:0x34     */
    __IO uint32_t BDTR;                         /*!< EPWM Break and Dead-time Register                                  Address offset:0x44     */
         uint32_t reserve1[2];
    __IO uint32_t PMCR[4];                      /*!< EPWM PWM Mode Ceil Register                                        Address offset:0x50     */
    __IO uint32_t PMCREN;                       /*!< EPWM PMCR Enable Register                                          Address offset:0x60     */
    __IO uint32_t PUE;                          /*!< EPWM Pull Enable Register                                          Address offset:0x64     */
} EPWM_TypeDef;

/**
  * @brief Controller Area Network
  */

typedef struct
{
    __IO uint32_t MOD;                          /*!< CAN Mode Register                                                  Address offset:0x0      */
    __O  uint32_t CMR;                          /*!< CAN Command Register                                               Address offset:0x4      */
    __I  uint32_t SR;                           /*!< CAN Status Register                                                Address offset:0x8      */
    __I  uint32_t IR;                           /*!< CAN Interrupt Register                                             Address offset:0xc      */
    __IO uint32_t IER;                          /*!< CAN Interrupt Enable Register                                      Address offset:0x10     */
         uint32_t reserve1;
    __IO uint32_t BTR0;                         /*!< CAN Bus Timing Register 0                                          Address offset:0x18     */
    __IO uint32_t BTR1;                         /*!< CAN Bus Timing Register 1                                          Address offset:0x1c     */
    __IO uint32_t OCR;                          /*!< CAN Output Control Register                                        Address offset:0x20     */
         uint32_t reserve2[2];
    __I  uint32_t ALC;                          /*!< CAN Arbitration Lost Capture Register                              Address offset:0x2c     */
    __I  uint32_t ECC;                          /*!< CAN Error Code Capture Register                                    Address offset:0x30     */
    __I  uint32_t EWLR;                         /*!< CAN Error Warning Limit Register                                   Address offset:0x34     */
    __I  uint32_t RXERR;                        /*!< CAN Receive Error Counter Register                                 Address offset:0x38     */
    __I  uint32_t TXERR;                        /*!< CAN Transmit Error Counter Register                                Address offset:0x3c     */
    union {
    struct {
    __O  uint32_t TFI;                          /*!< CAN Transmit Frame Information Register                            Address offset:0x40     */
    __O  uint32_t TDI[12];                      /*!< CAN Transmit Data Information Register                             Address offset:0x44     */
    };
    struct {
    __I  uint32_t RFI;                          /*!< CAN Receive Frame Information Register                             Address offset:0x40     */
    __I  uint32_t RDI[12];                      /*!< CAN Receive Data Information Register                              Address offset:0x44     */
    };
    struct {
    __IO uint32_t ACR[4];                       /*!< CAN Acceptance Code Register                                       Address offset:0x40     */
    __IO uint32_t AMR[4];                       /*!< CAN Acceptance Mask Register                                       Address offset:0x50     */
         uint32_t reserve3[5];
    };
    };
    __I  uint32_t RMC;                          /*!< CAN Receive Message Counter Register                               Address offset:0x74     */
    __I  uint32_t RBSA;                         /*!< CAN Receive Buffer Start Address Register                          Address offset:0x78     */
    __IO uint32_t CDR;                          /*!< CAN Clock Divider Register                                         Address offset:0x7c     */
    __I  uint32_t RF[64];                       /*!< CAN Receive FIFO Register                                          Address offset:0x80     */
    __I  uint32_t TB[13];                       /*!< CAN Transmit Buffer Register                                       Address offset:0x180    */
         uint32_t reserve4[19];
} CAN_TypeDef;

/**
  * @brief Direct Memory Access
  */

typedef struct
{
    __I  uint64_t ID;                           /*!< DMA ID Register                                                    Address offset:0x0      */
    __I  uint64_t COMPVER;                      /*!< DMA Component Version Register                                     Address offset:0x8      */
    __IO uint64_t CFG;                          /*!< DMA Config Register                                                Address offset:0x10     */
    union {
    __O  uint64_t CHEN;                         /*!< DMA Channel Enable Register                                        Address offset:0x18     */
    __O  uint64_t CHEN2;                        /*!< DMA Channel Enable Register 2                                      Address offset:0x18     */
    };
    __O  uint64_t CHSUSP;                       /*!< DMA Channel Suspend Register                                       Address offset:0x20     */
    __O  uint64_t CHABORT;                      /*!< DMA Channel Abort Register                                         Address offset:0x28     */
    union {
    __I  uint64_t INTSTATUS;                    /*!< DMA Interrupt Status Register                                      Address offset:0x30     */
    __I  uint64_t INTSTATUS2;                   /*!< DMA Interrupt Status Register 2                                    Address offset:0x30     */
    };
    __O  uint64_t COMMON_INTCLEAR;              /*!< DMA Common Register Interrupt Status Clear Register                Address offset:0x38     */
    __IO uint64_t COMMON_INTSTATUS_ENABLE;      /*!< DMA Common Register Interrupt Status Enable Register               Address offset:0x40     */
    __IO uint64_t COMMON_INTSIGNAL_ENABLE;      /*!< DMA Common Register Interrupt Signal Enable Register               Address offset:0x48     */
    __I  uint64_t COMMON_INTSTATUS;             /*!< DMA Common Register Interrupt Status Register                      Address offset:0x50     */
    __IO uint64_t RESET;                        /*!< DMA Software Reset Register                                        Address offset:0x58     */
    __IO uint64_t LOWPOWER_CFG;                 /*!< DMA Low Power Config Register                                      Address offset:0x60     */
         uint64_t reserve1[19];
    struct {
    __IO uint64_t CH_SAR;                       /*!< DMA Channel Source Address Register                                Address offset:0x100    */
    __IO uint64_t CH_DAR;                       /*!< DMA Channel Destination Address Register                           Address offset:0x108    */
    __IO uint64_t CH_BLOCK_TS;                  /*!< DMA Channel Block Size Register                                    Address offset:0x110    */
    __IO uint64_t CH_CTL;                       /*!< DMA Channel Control Register                                       Address offset:0x118    */
    union {
    __IO uint64_t CH_CFG;                       /*!< DMA Channel Config Register                                        Address offset:0x120    */
    __IO uint64_t CH_CFG2;                      /*!< DMA Channel Config Register 2                                      Address offset:0x120    */
    };
    __IO uint64_t CH_LLP;                       /*!< DMA Channel Linked List Pointer Register                           Address offset:0x128    */
    __I  uint64_t CH_STATUS;                    /*!< DMA Channel Status Register                                        Address offset:0x130    */
    __IO uint64_t CH_SWHSSRC;                   /*!< DMA Channel Software Handshake Source Register                     Address offset:0x138    */
    __IO uint64_t CH_SWHSDST;                   /*!< DMA Channel Software Handshake Destination Register                Address offset:0x140    */
    __O  uint64_t CH_BLK_TFR_RESUMEREQ;         /*!< DMA Channel Block Transfer Resume Request Register                 Address offset:0x148    */
    __IO uint64_t CH_AXI_ID;                    /*!< DMA Channel AXI ID Register                                        Address offset:0x150    */
    __IO uint64_t CH_AXI_QOS;                   /*!< DMA Channel AXI QOS Register                                       Address offset:0x158    */
    __I  uint64_t CH_SSTAT;                     /*!< DMA Channel Source Status Register                                 Address offset:0x160    */
    __I  uint64_t CH_DSTAT;                     /*!< DMA Channel Destination Status Register                            Address offset:0x168    */
    __IO uint64_t CH_SSTATAR;                   /*!< DMA Channel Source Status Fetch Register                           Address offset:0x170    */
    __IO uint64_t CH_DSTATAR;                   /*!< DMA Channel Destination Status Fetch Register                      Address offset:0x178    */
    __IO uint64_t CH_INTSTATUS_ENABLE;          /*!< DMA Channel Interrupt Status Enable Register                       Address offset:0x180    */
    __I  uint64_t CH_INTSTATUS;                 /*!< DMA Channel Interrupt Status Register                              Address offset:0x188    */
    __IO uint64_t CH_INTSIGNAL_ENABLE;          /*!< DMA Channel Interrupt Signal Enable Register                       Address offset:0x190    */
    __O  uint64_t CH_INTCLEAR;                  /*!< DMA Channel Interrupt Status Clear Register                        Address offset:0x198    */
         uint64_t reserve2[12];
    } chn[8];
} DMA_TypeDef;

/**
  * @brief Static Memory Controller
  */

typedef struct
{
    __I  uint32_t MEMC_STATUS;                  /*!< SMC Memory Controller Status Register                              Address offset:0x0      */
    __I  uint32_t MEMIF_CFG;                    /*!< SMC Memory Interface Configuration Register                        Address offset:0x4      */
    __O  uint32_t MEMC_CFG_SET;                 /*!< SMC Set Configuration Register                                     Address offset:0x8      */
    __O  uint32_t MEMC_CFG_CLR;                 /*!< SMC Clear Configuration Register                                   Address offset:0xc      */
    __O  uint32_t DIRECT_CMD;                   /*!< SMC Direct Command Register                                        Address offset:0x10     */
    __O  uint32_t SET_CYCLES;                   /*!< SMC Set Cycles Register                                            Address offset:0x14     */
    __O  uint32_t SET_OPMODE;                   /*!< SMC Set Opmode Register                                            Address offset:0x18     */
         uint32_t reserve1;
    __IO uint32_t REFRESH_PERIOD[2];            /*!< SMC Refresh Period Register                                        Address offset:0x20     */
         uint32_t reserve2[54];
    struct {
    union {
    __I  uint32_t SRAM_CYCLES;                  /*!< SMC SRAM Cycles Register                                           Address offset:0x100    */
    __I  uint32_t NAND_CYCLES;                  /*!< SMC NAND Cycles Register                                           Address offset:0x100    */
    };
    __I  uint32_t OPMODE;                       /*!< SMC Opmode Register                                                Address offset:0x104    */
         uint32_t reserve3[6];
    } bank[2][4];
    __I  uint32_t USER_STATUS;                  /*!< SMC User Status Register                                           Address offset:0x200    */
    __O  uint32_t USER_CONFIG;                  /*!< SMC User Config Register                                           Address offset:0x204    */
         uint32_t reserve4[62];
    struct {
    __I  uint32_t ECC_STATUS;                   /*!< SMC ECC Status Register                                            Address offset:0x300    */
    __IO uint32_t ECC_CONFIG;                   /*!< SMC ECC Configuration Register                                     Address offset:0x304    */
    __IO uint32_t ECC_COMMAND[2];               /*!< SMC ECC Command Register                                           Address offset:0x308    */
    __IO uint32_t ECC_ADDR[2];                  /*!< SMC ECC Address Register                                           Address offset:0x310    */
    __IO uint32_t ECC_VALUE[5];                 /*!< SMC ECC Value Register                                             Address offset:0x318    */
         uint32_t reserve5[53];
    } interface[2];
         uint32_t reserve6[576];
    __IO uint32_t INT_CFG;                      /*!< SMC Integration Configuration Register                             Address offset:0xe00    */
    __IO uint32_t INT_INPUT;                    /*!< SMC Integration Inputs Register                                    Address offset:0xe04    */
    __IO uint32_t INT_OUTPUT;                   /*!< SMC Integration Outputs Register                                   Address offset:0xe08    */
         uint32_t reserve7[117];
    __I  uint32_t PERIPH_ID[4];                 /*!< SMC Peripheral Identification Register                             Address offset:0xfe0    */
    __I  uint32_t PCELL_ID[4];                  /*!< SMC PrimeCell Identification Register                              Address offset:0xff0    */
} SMC_TypeDef;

/**
  * @brief External Memory Interface
  */

typedef struct
{
    __IO uint32_t WRMEM_CONFIG;                 /*!< EMIF WRMEM_config_reg Register                                     Address offset:0x0      */
    __IO uint32_t ORMEM_CONFIG;                 /*!< EMIF ORMEM_config_reg Register                                     Address offset:0x4      */
    __IO uint32_t IO_CONFIG;                    /*!< EMIF IO_config_reg Register                                        Address offset:0x8      */
    __IO uint32_t EDAC_CONFIG;                  /*!< EMIF EDAC_config_reg Register                                      Address offset:0xc      */
         uint32_t reserve1[2];
    __IO uint32_t IO_RDY_TIMEOUT;               /*!< EMIF IO_Rdy_Timeout_reg Register                                   Address offset:0x18     */
    __IO uint32_t INT_CLEAR;                    /*!< EMIF INT_clear_reg Register                                        Address offset:0x1c     */
    __IO uint32_t WR_WS_CFG_REG;                /*!< EMIF WR_WS_CFG_REG Register                                        Address offset:0x20     */
    __IO uint32_t OR_WS_CFG_REG;                /*!< EMIF OR_WS_CFG_REG Register                                        Address offset:0x24     */
    __IO uint32_t IO_WS_CFG_REG;                /*!< EMIF IO_WS_CFG_REG Register                                        Address offset:0x28     */
         uint32_t reserve2;
    __IO uint32_t CERR_ADDR;                    /*!< EMIF CERR_addr_reg Register                                        Address offset:0x30     */
    __IO uint32_t CERR_DATA;                    /*!< EMIF CERR_data_reg Register                                        Address offset:0x34     */
    __IO uint32_t CERR_EDAC;                    /*!< EMIF CERR_edac_reg Register                                        Address offset:0x38     */
         uint32_t reserve3;
    __IO uint32_t DERR_ADDR;                    /*!< EMIF DERR_addr_reg Register                                        Address offset:0x40     */
    __IO uint32_t DERR_DATA;                    /*!< EMIF DERR_data_reg Register                                        Address offset:0x44     */
    __IO uint32_t DERR_EDAC;                    /*!< EMIF DERR_edac_reg Register                                        Address offset:0x48     */
         uint32_t reserve4;
    __IO uint32_t EDAC_READBYPASS_ADDR;         /*!< EMIF EDAC_readbypass_addr Register                                 Address offset:0x50     */
    __IO uint32_t EDAC_READBYPASS_EDAC;         /*!< EMIF EDAC_readbypass_edac Register                                 Address offset:0x54     */
    __IO uint32_t EDAC_READBYPASS_DATA;         /*!< EMIF EDAC_readbypass_data Register                                 Address offset:0x58     */
    __IO uint32_t EDAC_WRITEBYPASS_ADDR;        /*!< EMIF EDAC_writebypass_addr Register                                Address offset:0x5c     */
    __IO uint32_t EDAC_WRITEBYPASS_EDAC;        /*!< EMIF EDAC_writebypass_edac Register                                Address offset:0x60     */
} EMIF_TypeDef, AMC_TypeDef;

/**
  * @brief Gigabit Ethernet Media Access Controller
  */

typedef struct
{
    __IO uint32_t Register0;                    /*!< GMAC MAC Configuration Register                                    Address offset:0x0000   */
    __IO uint32_t Register1;                    /*!< GMAC MAC Frame Filter Register                                     Address offset:0x0004   */
    __IO uint32_t Register2;                    /*!< GMAC Hash Table High Register                                      Address offset:0x0008   */
    __IO uint32_t Register3;                    /*!< GMAC Hash Table Low Register                                       Address offset:0x000c   */
    __IO uint32_t Register4;                    /*!< GMAC GMII Address Register                                         Address offset:0x0010   */
    __IO uint32_t Register5;                    /*!< GMAC GMII Data Register                                            Address offset:0x0014   */
    __IO uint32_t Register6;                    /*!< GMAC Flow Control Register                                         Address offset:0x0018   */
    __IO uint32_t Register7;                    /*!< GMAC VLAN Tag Register                                             Address offset:0x001c   */
    __IO uint32_t Register8;                    /*!< GMAC Version Register                                              Address offset:0x0020   */
    __IO uint32_t Register9;                    /*!< GMAC Debug Register                                                Address offset:0x0024   */
    __IO uint32_t Register10;                   /*!< GMAC Remote Wake-Up Frame Filter Register                          Address offset:0x0028   */
    __IO uint32_t Register11;                   /*!< GMAC PMT Control and Status Register                               Address offset:0x002c   */
    __IO uint32_t Register12;                   /*!< GMAC LPI Control and Status Register                               Address offset:0x0030   */
    __IO uint32_t Register13;                   /*!< GMAC LPI Timers Control Register                                   Address offset:0x0034   */
    __IO uint32_t Register14;                   /*!< GMAC Interrupt Status Register                                     Address offset:0x0038   */
    __IO uint32_t Register15;                   /*!< GMAC Interrupt Mask Register                                       Address offset:0x003c   */
    __IO uint32_t Register16;                   /*!< GMAC MAC Address0 High Register                                    Address offset:0x0040   */
    __IO uint32_t Register17;                   /*!< GMAC MAC Address0 Low Register                                     Address offset:0x0044   */
    __IO uint32_t Register18;                   /*!< GMAC MAC Address1 High Register                                    Address offset:0x0048   */
    __IO uint32_t Register19;                   /*!< GMAC MAC Address1 Low Register                                     Address offset:0x004c   */
    __IO uint32_t Register20;                   /*!< GMAC MAC Address2 High Register                                    Address offset:0x0050   */
    __IO uint32_t Register21;                   /*!< GMAC MAC Address2 Low Register                                     Address offset:0x0054   */
    __IO uint32_t Register22;                   /*!< GMAC MAC Address3 High Register                                    Address offset:0x0058   */
    __IO uint32_t Register23;                   /*!< GMAC MAC Address3 Low Register                                     Address offset:0x005c   */
    __IO uint32_t Register24;                   /*!< GMAC MAC Address4 High Register                                    Address offset:0x0060   */
    __IO uint32_t Register25;                   /*!< GMAC MAC Address4 Low Register                                     Address offset:0x0064   */
    __IO uint32_t Register26;                   /*!< GMAC MAC Address5 High Register                                    Address offset:0x0068   */
    __IO uint32_t Register27;                   /*!< GMAC MAC Address5 Low Register                                     Address offset:0x006c   */
    __IO uint32_t Register28;                   /*!< GMAC MAC Address6 High Register                                    Address offset:0x0070   */
    __IO uint32_t Register29;                   /*!< GMAC MAC Address6 Low Register                                     Address offset:0x0074   */
    __IO uint32_t Register30;                   /*!< GMAC MAC Address7 High Register                                    Address offset:0x0078   */
    __IO uint32_t Register31;                   /*!< GMAC MAC Address7 Low Register                                     Address offset:0x007c   */
    __IO uint32_t Register32;                   /*!< GMAC MAC Address8 High Register                                    Address offset:0x0080   */
    __IO uint32_t Register33;                   /*!< GMAC MAC Address8 Low Register                                     Address offset:0x0084   */
    __IO uint32_t Register34;                   /*!< GMAC MAC Address9 High Register                                    Address offset:0x0088   */
    __IO uint32_t Register35;                   /*!< GMAC MAC Address9 Low Register                                     Address offset:0x008c   */
    __IO uint32_t Register36;                   /*!< GMAC MAC Address10 High Register                                   Address offset:0x0090   */
    __IO uint32_t Register37;                   /*!< GMAC MAC Address10 Low Register                                    Address offset:0x0094   */
    __IO uint32_t Register38;                   /*!< GMAC MAC Address11 High Register                                   Address offset:0x0098   */
    __IO uint32_t Register39;                   /*!< GMAC MAC Address11 Low Register                                    Address offset:0x009c   */
    __IO uint32_t Register40;                   /*!< GMAC MAC Address12 High Register                                   Address offset:0x00a0   */
    __IO uint32_t Register41;                   /*!< GMAC MAC Address12 Low Register                                    Address offset:0x00a4   */
    __IO uint32_t Register42;                   /*!< GMAC MAC Address13 High Register                                   Address offset:0x00a8   */
    __IO uint32_t Register43;                   /*!< GMAC MAC Address13 Low Register                                    Address offset:0x00ac   */
    __IO uint32_t Register44;                   /*!< GMAC MAC Address14 High Register                                   Address offset:0x00b0   */
    __IO uint32_t Register45;                   /*!< GMAC MAC Address14 Low Register                                    Address offset:0x00b4   */
    __IO uint32_t Register46;                   /*!< GMAC MAC Address15 High Register                                   Address offset:0x00b8   */
    __IO uint32_t Register47;                   /*!< GMAC MAC Address15 Low Register                                    Address offset:0x00bc   */
    __IO uint32_t Register48;                   /*!< GMAC AN Control Register                                           Address offset:0x00c0   */
    __IO uint32_t Register49;                   /*!< GMAC AN Status Register                                            Address offset:0x00c4   */
    __IO uint32_t Register50;                   /*!< GMAC Auto-Negotiation Advertisement Register                       Address offset:0x00c8   */
    __IO uint32_t Register51;                   /*!< GMAC Auto-Negotiation Link Partner Ability Register                Address offset:0x00cc   */
    __IO uint32_t Register52;                   /*!< GMAC Auto-Negotiation Expansion Register                           Address offset:0x00d0   */
    __IO uint32_t Register53;                   /*!< GMAC TBI Extended Status Register                                  Address offset:0x00d4   */
    __IO uint32_t Register54;                   /*!< GMAC SGMII/RGMII/SMII Control and Status Register                  Address offset:0x00d8   */
    __IO uint32_t Register55;                   /*!< GMAC Watchdog Timeout Register                                     Address offset:0x00dc   */
    __IO uint32_t Register56;                   /*!< GMAC General Purpose IO Register                                   Address offset:0x00e0   */
} GMAC_TypeDef;

/**
  * @brief Gigabit Ethernet Media Access Controller - Direct Memory Access
  */

typedef struct
{
    __IO uint32_t reserve1[1024];
    __IO uint32_t Register0;                    /*!< GDMA Bus Mode Register                                             Address offset:0x1000   */
    __IO uint32_t Register1;                    /*!< GDMA Transmit Poll Demand Register                                 Address offset:0x1004   */
    __IO uint32_t Register2;                    /*!< GDMA Receive Poll Demand Register                                  Address offset:0x1008   */
    __IO uint32_t Register3;                    /*!< GDMA Receive Descriptor List Address Register                      Address offset:0x100c   */
    __IO uint32_t Register4;                    /*!< GDMA Transmit Descriptor List Address Register                     Address offset:0x1010   */
    __IO uint32_t Register5;                    /*!< GDMA Status Register                                               Address offset:0x1014   */
    __IO uint32_t Register6;                    /*!< GDMA Operation Mode Register                                       Address offset:0x1018   */
    __IO uint32_t Register7;                    /*!< GDMA Interrupt Enable Register                                     Address offset:0x101c   */
    __IO uint32_t Register8;                    /*!< GDMA Missed Frame and Buffer Overflow Counter Register             Address offset:0x1020   */
    __IO uint32_t Register9;                    /*!< GDMA Receive Interrupt Watchdog Timer Register                     Address offset:0x1024   */
    __IO uint32_t Register10;                   /*!< GDMA AXI Bus Mode Register                                         Address offset:0x1028   */
    __IO uint32_t Register11;                   /*!< GDMA AHB or AXI Status Register                                    Address offset:0x102c   */
         uint32_t reserve2[6];
    __IO uint32_t Register18;                   /*!< GDMA Current Host Transmit Descriptor Register                     Address offset:0x1048   */
    __IO uint32_t Register19;                   /*!< GDMA Current Host Receive Descriptor Register                      Address offset:0x104c   */
    __IO uint32_t Register20;                   /*!< GDMA Current Host Transmit Buffer Address Register                 Address offset:0x1050   */
    __IO uint32_t Register21;                   /*!< GDMA Current Host Receive Buffer Address Register                  Address offset:0x1054   */
    __IO uint32_t Register22;                   /*!< GDMA HW Feature Register                                           Address offset:0x1058   */
} GDMA_TypeDef;

/**
  * @brief RapidIO
  */

typedef struct
{
    struct {
    __IO uint32_t BVERR;                        /*!< SRIO Bridge Version Register                                       Address offset:0x0      */
    __IO uint32_t BCAPR;                        /*!< SRIO Bridge Capability Register                                    Address offset:0x4      */
    __IO uint32_t BCTLR;                        /*!< SRIO Bridge Control Register                                       Address offset:0x8      */
    __IO uint32_t BSTAR;                        /*!< SRIO Bridge Status Register                                        Address offset:0xc      */
    __IO uint32_t AXITOR;                       /*!< SRIO AXI Timeout Register                                          Address offset:0x10     */
    __IO uint32_t DMETOR;                       /*!< SRIO DME Timeout Register                                          Address offset:0x14     */
    __IO uint32_t ERCR;                         /*!< SRIO Engine Reset Control Register                                 Address offset:0x18     */
    __IO uint32_t CPLCR;                        /*!< SRIO Cooperative Lock Control Register                             Address offset:0x1c     */
    __IO uint32_t STAESR;                       /*!< SRIO Statistics Engine Select Register                             Address offset:0x20     */
    __IO uint32_t STASR;                        /*!< SRIO Statistics Status Register                                    Address offset:0x24     */
    __IO uint32_t IBPCSR;                       /*!< SRIO Inbound Port-write Control and Status Register                Address offset:0x28     */
    __IO uint32_t IBPDR;                        /*!< SRIO Inbound Port-write Data Register                              Address offset:0x2c     */
    __IO uint32_t APBCS;                        /*!< SRIO APB Control and Status Register                               Address offset:0x30     */
    __IO uint32_t ARBTOR;                       /*!< SRIO Arbitration Timeout Register                                  Address offset:0x34     */
    __IO uint32_t DMADRTR;                      /*!< SRIO DMA Descriptor Ready Timeout Register                         Address offset:0x38     */
         uint32_t reserve1;
    } Global;

    struct {
    __IO uint32_t GENIER;                       /*!< SRIO General Interrupt Enable Register                             Address offset:0x40     */
    __IO uint32_t AXIIER;                       /*!< SRIO AXI PIO Interrupt Enable Register                             Address offset:0x44     */
    __IO uint32_t RIOIER;                       /*!< SRIO RIO PIO Interrupt Enable Register                             Address offset:0x48     */
    __IO uint32_t WDMAIER;                      /*!< SRIO Write DMA Interrupt Enable Register                           Address offset:0x4c     */
    __IO uint32_t RDMAIER;                      /*!< SRIO Read DMA Interrupt Enable Register                            Address offset:0x50     */
    __IO uint32_t IDMEIER;                      /*!< SRIO Inbound DME Interrupt Enable Register                         Address offset:0x54     */
    __IO uint32_t ODMEIER;                      /*!< SRIO Outbound DME Interrupt Enable Register                        Address offset:0x58     */
    __IO uint32_t MISCIER;                      /*!< SRIO Misc Interrupt Enable Register                                Address offset:0x5c     */
    __IO uint32_t GENISR;                       /*!< SRIO General Interrupt Status Register                             Address offset:0x60     */
    __IO uint32_t AXIISR;                       /*!< SRIO AXI PIO Interrupt Status Register                             Address offset:0x64     */
    __IO uint32_t RIOISR;                       /*!< SRIO RIO PIO Interrupt Status Register                             Address offset:0x68     */
    __IO uint32_t WDMAISR;                      /*!< SRIO Write DMA Interrupt Status Register                           Address offset:0x6c     */
    __IO uint32_t RDMAISR;                      /*!< SRIO READ DMA Interrupt Status Register                            Address offset:0x70     */
    __IO uint32_t IDMEISR;                      /*!< SRIO Inbound DME Interrupt Status Register                         Address offset:0x74     */
    __IO uint32_t ODMEISR;                      /*!< SRIO Outbound DME Interrupt Status Register                        Address offset:0x78     */
    __IO uint32_t MISCISR;                      /*!< SRIO Misc Interrupt Status Register                                Address offset:0x7c     */
    } Interrupt;

    struct {
    struct {
    __IO uint32_t PIOCTLR;                      /*!< SRIO RIO PIO Control Register                                      Address offset:0x80     */
    __IO uint32_t PIOSTAR;                      /*!< SRIO RIO PIO Status Register                                       Address offset:0x84     */
    } RIO_Engine[8];
         uint32_t reserve1[16];
    __IO uint32_t RIO_LUT[16];                  /*!< SRIO RIO Address Mapping Lookup Table Entry Register               Address offset:0x100    */
    __IO uint32_t RIO_AMISR;                    /*!< SRIO RIO Address Mapping Index Select Register                     Address offset:0x140    */
    __IO uint32_t RIO_AMBR;                     /*!< SRIO RIO Address Mapping Bypass Register                           Address offset:0x144    */
         uint32_t reserve2[14];
    } RIO;

    struct {
    struct {
    __IO uint32_t PIOCTLR;                      /*!< SRIO AXI PIO Control Register                                      Address offset:0x180    */
    __IO uint32_t PIOSTAR;                      /*!< SRIO AXI PIO Status Register                                       Address offset:0x184    */
    } AXI_Engine[8];
    struct {
    __IO uint32_t AXISCSR;                      /*!< SRIO AXI Slave Command Status Register                             Address offset:0x1c0    */
    __IO uint32_t AXISASR;                      /*!< SRIO AXI Slave Address Status Register                             Address offset:0x1c4    */
    } AXI_Slave[4];
    struct {
    __IO uint32_t AXIMSR;                       /*!< SRIO AXI Master Status Register                                    Address offset:0x1e0    */
    } AXI_Master[8];
    struct {
    __IO uint32_t AXIAMCR;                      /*!< SRIO AXI Address Mapping Control Register                          Address offset:0x200    */
    __IO uint32_t AXIAMSR;                      /*!< SRIO AXI Address Mapping Size Register                             Address offset:0x204    */
    __IO uint32_t AXIAMAXIBR;                   /*!< SRIO AXI Address Mapping AXI Base Register                         Address offset:0x208    */
    __IO uint32_t AXIAMRIOBR;                   /*!< SRIO AXI Address Mapping RIO Base Register                         Address offset:0x20c    */
    } AXI_Address_Map[32];
    } AXI;

    struct {
    struct {
    __IO uint32_t ODM0CSR;                      /*!< SRIO Outbound Doorbell Message 0 Control and Status Register       Address offset:0x400    */
    __IO uint32_t ODM0IR;                       /*!< SRIO Outbound Doorbell Message 0 Information Register              Address offset:0x404    */
    } Doorbell_Message[15];
    __IO uint32_t OIDMR;                        /*!< SRIO Outbound Interrupt Doorbell Message Register                  Address offset:0x478    */
    __IO uint32_t OIDMIR;                       /*!< SRIO Outbound Interrupt Doorbell Message Information Register      Address offset:0x47c    */
    __IO uint32_t IDMCSR;                       /*!< SRIO Inbound Doorbell Message Control and Status Register          Address offset:0x480    */
    __IO uint32_t IDMIR;                        /*!< SRIO Inbound Doorbell Message Information Register                 Address offset:0x484    */
    __IO uint32_t IDMCR[30];                    /*!< SRIO Inbound Doorbell Message Checker Register                     Address offset:0x488    */
    } Doorbell;

    struct {
    struct {
    __IO uint32_t ODMECR;                       /*!< SRIO Outbound DME Control Register                                 Address offset:0x500    */
    __IO uint32_t ODMEDAR;                      /*!< SRIO Outbound DME Address Register                                 Address offset:0x504    */
    __IO uint32_t ODMESR;                       /*!< SRIO Outbound DME Status Register                                  Address offset:0x508    */
    __IO uint32_t ODMEDR;                       /*!< SRIO Outbound DME Descriptor Register                              Address offset:0x50c    */
    } Outbound_DME[15];
    __IO uint32_t OIDMR;                        /*!< SRIO Outbound DME TID Mask Register                                Address offset:0x5f0    */
         uint32_t reserve1[3];
    struct {
    __IO uint32_t IDMECR;                       /*!< SRIO Inbound DME Control Register                                  Address offset:0x600    */
    __IO uint32_t IDMEDAR;                      /*!< SRIO Inbound DME Address Register                                  Address offset:0x604    */
    __IO uint32_t IDMESR;                       /*!< SRIO Inbound DME Status Register                                   Address offset:0x608    */
    __IO uint32_t IDMEDR;                       /*!< SRIO Inbound DME Descriptor Register                               Address offset:0x60c    */
    } Inbound_DME[32];
    } DME;
} SRIO_PAGE0_TypeDef; // NOTE: total 2K

typedef struct
{
    struct {
    struct {
    __IO uint32_t WDMACR;                       /*!< SRIO Write DMA Control Register                                    Address offset:0x800    */
    __IO uint32_t WDMADAR;                      /*!< SRIO Write DMA Descriptor Address Register                         Address offset:0x804    */
    __IO uint32_t WDMASR;                       /*!< SRIO Write DMA Status Register                                     Address offset:0x808    */
    __IO uint32_t WDMADAER;                     /*!< SRIO Write DMA Descriptor Register                                 Address offset:0x80c    */
    } Write_DMA[8];
    struct {
    __IO uint32_t RDMACR;                       /*!< SRIO Read DMA Control Register                                     Address offset:0x880    */
    __IO uint32_t RDMADAR;                      /*!< SRIO Read DMA Descriptor Address Register                          Address offset:0x884    */
    __IO uint32_t RDMASR;                       /*!< SRIO Read DMA Status Register                                      Address offset:0x888    */
    __IO uint32_t RDMADAER;                     /*!< SRIO Read DMA Descriptor Register                                  Address offset:0x88c    */
    } Read_DMA[8];
    __IO uint32_t DMAIADSR;                     /*!< SRIO DMA Indirect Descriptor Select Register                       Address offset:0x900    */
    __IO uint32_t DMAIADCR;                     /*!< SRIO DMA Indirect Descriptor Control Register                      Address offset:0x904    */
    __IO uint32_t DMAIADSAR;                    /*!< SRIO DMA Indirect Descriptor Source Address Register               Address offset:0x908    */
    __IO uint32_t DMAIADDAR;                    /*!< SRIO DMA Indirect Descriptor Destination Address Register          Address offset:0x90c    */
    __IO uint32_t DMAIADNPAR;                   /*!< SRIO DMA Indirect Descriptor Next Pointer Address Register         Address offset:0x910    */
    } DMA;
         uint32_t reserve1[443];
} SRIO_PAGE1_TypeDef; // NOTE: total 2K

typedef struct
{
    SRIO_PAGE0_TypeDef PAGE0;                   /*!< SRIO Register                                                      Address offset:0x0      */
    SRIO_PAGE1_TypeDef PAGE1;                   /*!< SRIO Register                                                      Address offset:0x800    */
} SRIO_TypeDef;

/**
  * @brief Star Timer
  */

typedef struct
{
    __IO uint32_t REG[128];                    /*!< Star Timer Register                                                 Address offset:0x0      */
} ASTIMER_TypeDef;

/** @addtogroup Peripheral memory map
  * @{
  */
#define IBUS_C_B_BASE                           ( (uint32_t) 0x00000000 )       // 1MB,   Cacheable, Bufferable
#define DBUS_C_B_BASE_0                         ( (uint32_t) 0x10000000 )       // 1MB,   Cacheable, Bufferable
#define DBUS_C_NB_BASE                          ( (uint32_t) 0x20000000 )       // 256MB, Cacheable, non-Bufferable
#define DBUS_SO_BASE_0                          ( (uint32_t) 0x30000000 )       // 256MB, StrongOrder
#define DBUS_C_B_BASE_1                         ( (uint32_t) 0x40000000 )       // 1GB,   Cacheable, Bufferable
#define DBUS_C_B_BASE_2                         ( (uint32_t) 0x80000000 )       // 512MB, Cacheable, Bufferable
#define DBUS_C_B_BASE_3                         ( (uint32_t) 0xA0000000 )       // 512MB, Cacheable, Bufferable
#define SBUS_SO_BASE_0                          ( (uint32_t) 0xC0000000 )       // 128MB, StrongOrder
#define SBUS_SO_BASE_1                          ( (uint32_t) 0xD0000000 )       // 256MB, StrongOrder
#define TCIP_SO_BASE                            ( (uint32_t) 0xE0000000 )       // 256MB, StrongOrder
#define DBUS_SO_BASE_1                          ( (uint32_t) 0xF0000000 )       // 256MB, StrongOrder

#define AXI_BASE                                (IBUS_C_B_BASE   + 0x0       )  // 5MB
#define SRAM_BASE                               (DBUS_C_B_BASE_0 + 0x0       )  // 1MB
#define SRIO_CFG_BASE                           (DBUS_C_B_BASE_0 + 0x00100000)  // 1MB
#define AHB1_BASE                               (DBUS_C_NB_BASE  + 0x0       )  // 512MB
#define DDR_MEM_BASE                            (DBUS_C_B_BASE_1 + 0x0       )  // 1GB
#define SMC_MEM_BASE                            (DBUS_C_B_BASE_2 + 0x0       )  // 512MB
#define AMC_MEM_BASE                            (DBUS_C_B_BASE_3 + 0x0       )  // 640MB
#define AHB0_BASE                               (SBUS_SO_BASE_1  + 0x0       )  // 256MB
#define SRIO0_BASE                              (DBUS_SO_BASE_1  + 0x00000000)  // 128MB
#define SRIO1_BASE                              (DBUS_SO_BASE_1  + 0x08000000)  // 128MB

/*!< SRIO_CFG memory map */
#define SRIO0_CFG_BASE                          (SRIO_CFG_BASE   + 0x0       )  // 64KB
#define SRIO0_SERDES_BASE                       (SRIO_CFG_BASE   + 0x00020000)  // 64KB
#define SRIO0_USR_CFG_BASE                      (SRIO_CFG_BASE   + 0x00040000)  // 64KB

/*!< AMC_MEM memory map */
#define AMC_ROMEM_BASE                          (AMC_MEM_BASE    + 0x0       )  // 256MB
#define AMC_RWMEM_BASE                          (AMC_MEM_BASE    + 0x10000000)  // 256MB
#define AMC_IOMEM_BASE                          (AMC_MEM_BASE    + 0x20000000)  // 128MB

/*!< AHB0 memory map */
#define APB_CFG_BASE                            (AHB0_BASE       + 0x0       )  // 16MB
#define DMAC_CFG_BASE                           (AHB0_BASE       + 0x01000000)  // 128KB
#define B1553B0_CFG_BASE                        (AHB0_BASE       + 0x02000000)  // 128KB
#define B1553B0_MEM_BASE                        (AHB0_BASE       + 0x02020000)  // 128KB
#define B1553B1_CFG_BASE                        (AHB0_BASE       + 0x02040000)  // 128KB
#define B1553B1_MEM_BASE                        (AHB0_BASE       + 0x02060000)  // 128KB
#define APB_G0_BASE                             (AHB0_BASE       + 0x03000000)  // 16MB
#define APB_G1_BASE                             (AHB0_BASE       + 0x04000000)  // 16MB

/*!< AHB1 memory map */
#define QSPI_MEM_BASE                           (DBUS_C_NB_BASE  + 0x0       )  // 256MB

#define SPW0_MEM_BASE                           (DBUS_SO_BASE_0  + 0x01000000)  // 64KB
#define SPW1_MEM_BASE                           (DBUS_SO_BASE_0  + 0x02000000)  // 64KB
#define SPW2_MEM_BASE                           (DBUS_SO_BASE_0  + 0x03000000)  // 64KB
#define SPW3_MEM_BASE                           (DBUS_SO_BASE_0  + 0x04000000)  // 64KB
#define QSPI_CFG_BASE                           (DBUS_SO_BASE_0  + 0x05000000)  // 64KB

/*!< APB0 memory map */
#define SYS_CTRL_BASE                           (APB_CFG_BASE    + 0x0       )  // 64KB
#define DDR_CFG_BASE                            (APB_CFG_BASE    + 0x00010000)  // 64KB
#define SMC_CFG_BASE                            (APB_CFG_BASE    + 0x00020000)  // 64KB
#define AMC_CFG_BASE                            (APB_CFG_BASE    + 0x00030000)  // 64KB
#define GMAC0_CFG_BASE                          (APB_CFG_BASE    + 0x00040000)  // 64KB
#define GMAC1_CFG_BASE                          (APB_CFG_BASE    + 0x00050000)  // 64KB
#define SPW0_CFG_BASE                           (APB_CFG_BASE    + 0x00100000)  // 64KB
#define SPW1_CFG_BASE                           (APB_CFG_BASE    + 0x00110000)  // 64KB
#define SPW2_CFG_BASE                           (APB_CFG_BASE    + 0x00120000)  // 64KB
#define SPW3_CFG_BASE                           (APB_CFG_BASE    + 0x00130000)  // 64KB
#define WDT_CFG_BASE                            (APB_CFG_BASE    + 0x00140000)  // 64KB
#define ASTIMER_CFG_BASE                        (APB_CFG_BASE    + 0x00151000)  // 64KB
#define GPIO_CFG_BASE                           (APB_CFG_BASE    + 0x00200000)  // 64KB
#define RTC_CFG_BASE                            (APB_CFG_BASE    + 0x00210000)  // 64KB

/*!< APB_G0 memory map */
#define I2C0_BASE                               (APB_G0_BASE     + 0x0       )  // 64KB
#define SPI0_BASE                               (APB_G0_BASE     + 0x00010000)  // 64KB
#define SPI1_BASE                               (APB_G0_BASE     + 0x00020000)  // 64KB
#define SPI2_BASE                               (APB_G0_BASE     + 0x00030000)  // 64KB
#define SPI3_BASE                               (APB_G0_BASE     + 0x00040000)  // 64KB
#define CAN0_BASE                               (APB_G0_BASE     + 0x00050000)  // 64KB
#define PWM0_BASE                               (APB_G0_BASE     + 0x00060000)  // 64KB
#define PWM1_BASE                               (APB_G0_BASE     + 0x00070000)  // 64KB
#define PWM2_BASE                               (APB_G0_BASE     + 0x00080000)  // 64KB
#define EPWM0_BASE                              (APB_G0_BASE     + 0x00090000)  // 64KB
#define EPWM1_BASE                              (APB_G0_BASE     + 0x000A0000)  // 64KB
#define EPWM2_BASE                              (APB_G0_BASE     + 0x000B0000)  // 64KB
#define EPWM3_BASE                              (APB_G0_BASE     + 0x000C0000)  // 64KB
#define UART0_BASE                              (APB_G0_BASE     + 0x000D0000)  // 64KB
#define UART1_BASE                              (APB_G0_BASE     + 0x000E0000)  // 64KB
#define UART2_BASE                              (APB_G0_BASE     + 0x000F0000)  // 64KB
#define UART3_BASE                              (APB_G0_BASE     + 0x00100000)  // 64KB
#define UART4_BASE                              (APB_G0_BASE     + 0x00110000)  // 64KB
#define UART5_BASE                              (APB_G0_BASE     + 0x00120000)  // 64KB
#define UART6_BASE                              (APB_G0_BASE     + 0x00130000)  // 64KB
#define UART7_BASE                              (APB_G0_BASE     + 0x00140000)  // 64KB
#define UART8_BASE                              (APB_G0_BASE     + 0x00150000)  // 64KB
#define UART9_BASE                              (APB_G0_BASE     + 0x00160000)  // 64KB
#define UART10_BASE                             (APB_G0_BASE     + 0x00170000)  // 64KB
#define TIMER0_BASE                             (APB_G0_BASE     + 0x00180000)  // 64KB

/*!< APB_G1 memory map */
#define I2C1_BASE                               (APB_G1_BASE     + 0x0       )  // 64KB
#define SPI4_BASE                               (APB_G1_BASE     + 0x00010000)  // 64KB
#define SPI5_BASE                               (APB_G1_BASE     + 0x00020000)  // 64KB
#define SPI6_BASE                               (APB_G1_BASE     + 0x00030000)  // 64KB
#define SPI7_BASE                               (APB_G1_BASE     + 0x00040000)  // 64KB
#define CAN1_BASE                               (APB_G1_BASE     + 0x00050000)  // 64KB
#define PWM3_BASE                               (APB_G1_BASE     + 0x00060000)  // 64KB
#define PWM4_BASE                               (APB_G1_BASE     + 0x00070000)  // 64KB
#define PWM5_BASE                               (APB_G1_BASE     + 0x00080000)  // 64KB
#define EPWM4_BASE                              (APB_G1_BASE     + 0x00090000)  // 64KB
#define EPWM5_BASE                              (APB_G1_BASE     + 0x000A0000)  // 64KB
#define EPWM6_BASE                              (APB_G1_BASE     + 0x000B0000)  // 64KB
#define EPWM7_BASE                              (APB_G1_BASE     + 0x000C0000)  // 64KB
#define UART11_BASE                             (APB_G1_BASE     + 0x000D0000)  // 64KB
#define UART12_BASE                             (APB_G1_BASE     + 0x000E0000)  // 64KB
#define UART13_BASE                             (APB_G1_BASE     + 0x000F0000)  // 64KB
#define UART14_BASE                             (APB_G1_BASE     + 0x00100000)  // 64KB
#define UART15_BASE                             (APB_G1_BASE     + 0x00110000)  // 64KB
#define UART16_BASE                             (APB_G1_BASE     + 0x00120000)  // 64KB
#define UART17_BASE                             (APB_G1_BASE     + 0x00130000)  // 64KB
#define UART18_BASE                             (APB_G1_BASE     + 0x00140000)  // 64KB
#define UART19_BASE                             (APB_G1_BASE     + 0x00150000)  // 64KB
#define UART20_BASE                             (APB_G1_BASE     + 0x00160000)  // 64KB
#define UART21_BASE                             (APB_G1_BASE     + 0x00170000)  // 64KB
#define TIMER1_BASE                             (APB_G1_BASE     + 0x00180000)  // 64KB

/** @addtogroup Peripheral_declaration
  * @{
  */
#define NUM_OF_ASTIMER                          (1)
#define ASTIMER0                                ((ASTIMER_TypeDef *) ASTIMER_CFG_BASE)

#define NUM_OF_GMAC                             (2)
#define GMAC0                                   ((GMAC_TypeDef    *) GMAC0_CFG_BASE  )
#define GMAC1                                   ((GMAC_TypeDef    *) GMAC1_CFG_BASE  )

#define NUM_OF_GDMA                             (2)
#define GDMA0                                   ((GDMA_TypeDef    *) GMAC0_CFG_BASE  )
#define GDMA1                                   ((GDMA_TypeDef    *) GMAC1_CFG_BASE  )

#define NUM_OF_SMC                              (1)
#define SMC0                                    ((SMC_TypeDef     *) SMC_CFG_BASE    )

#define NUM_OF_AMC                              (1)
#define AMC0                                    ((AMC_TypeDef     *) AMC_CFG_BASE    )

#define NUM_OF_SRIO                             (1)
#define SRIO0                                   ((SRIO_TypeDef    *) SRIO0_CFG_BASE  )

#define NUM_OF_DMA                              (1)
#define DMA0                                    ((DMA_TypeDef     *) DMAC_CFG_BASE   )

#define NUM_OF_WDT                              (1)
#define WDT0                                    ((WDT_TypeDef     *) WDT_CFG_BASE    )

#define NUM_OF_GPIO                             (1)
#define GPIOA                                   ((GPIO_TypeDef    *) GPIO_CFG_BASE   )

#define NUM_OF_I2C                              (2)
#define I2C0                                    ((I2C_TypeDef     *) I2C0_BASE       )
#define I2C1                                    ((I2C_TypeDef     *) I2C1_BASE       )

#define NUM_OF_SPI                              (6)
#define SPI0                                    ((SPI_TypeDef     *) SPI0_BASE       )
#define SPI1                                    ((SPI_TypeDef     *) SPI1_BASE       )
#define SPI2                                    ((SPI_TypeDef     *) SPI2_BASE       )
#define SPI3                                    ((SPI_TypeDef     *) SPI3_BASE       )
#define SPI4                                    ((SPI_TypeDef     *) SPI4_BASE       )
#define SPI5                                    ((SPI_TypeDef     *) SPI5_BASE       )

#define NUM_OF_QSPI                             (1)
#define QSPI0                                   ((QSPI_TypeDef    *) QSPI_CFG_BASE   )

#define NUM_OF_CAN                              (2)
#define CAN0                                    ((CAN_TypeDef     *) CAN0_BASE       )
#define CAN1                                    ((CAN_TypeDef     *) CAN1_BASE       )

#define NUM_OF_PWM                              (6)
#define PWM0                                    ((PWM_TypeDef     *) PWM0_BASE       )
#define PWM1                                    ((PWM_TypeDef     *) PWM1_BASE       )
#define PWM2                                    ((PWM_TypeDef     *) PWM2_BASE       )
#define PWM3                                    ((PWM_TypeDef     *) PWM3_BASE       )
#define PWM4                                    ((PWM_TypeDef     *) PWM4_BASE       )
#define PWM5                                    ((PWM_TypeDef     *) PWM5_BASE       )

#define NUM_OF_EPWM                             (8)
#define EPWM0                                   ((EPWM_TypeDef    *) EPWM0_BASE      )
#define EPWM1                                   ((EPWM_TypeDef    *) EPWM1_BASE      )
#define EPWM2                                   ((EPWM_TypeDef    *) EPWM2_BASE      )
#define EPWM3                                   ((EPWM_TypeDef    *) EPWM3_BASE      )
#define EPWM4                                   ((EPWM_TypeDef    *) EPWM4_BASE      )
#define EPWM5                                   ((EPWM_TypeDef    *) EPWM5_BASE      )
#define EPWM6                                   ((EPWM_TypeDef    *) EPWM6_BASE      )
#define EPWM7                                   ((EPWM_TypeDef    *) EPWM7_BASE      )

#define NUM_OF_UART                             (22)
#define UART0                                   ((UART_TypeDef    *) UART0_BASE      )
#define UART1                                   ((UART_TypeDef    *) UART1_BASE      )
#define UART2                                   ((UART_TypeDef    *) UART2_BASE      )
#define UART3                                   ((UART_TypeDef    *) UART3_BASE      )
#define UART4                                   ((UART_TypeDef    *) UART4_BASE      )
#define UART5                                   ((UART_TypeDef    *) UART5_BASE      )
#define UART6                                   ((UART_TypeDef    *) UART6_BASE      )
#define UART7                                   ((UART_TypeDef    *) UART7_BASE      )
#define UART8                                   ((UART_TypeDef    *) UART8_BASE      )
#define UART9                                   ((UART_TypeDef    *) UART9_BASE      )
#define UART10                                  ((UART_TypeDef    *) UART10_BASE     )
#define UART11                                  ((UART_TypeDef    *) UART11_BASE     )
#define UART12                                  ((UART_TypeDef    *) UART12_BASE     )
#define UART13                                  ((UART_TypeDef    *) UART13_BASE     )
#define UART14                                  ((UART_TypeDef    *) UART14_BASE     )
#define UART15                                  ((UART_TypeDef    *) UART15_BASE     )
#define UART16                                  ((UART_TypeDef    *) UART16_BASE     )
#define UART17                                  ((UART_TypeDef    *) UART17_BASE     )
#define UART18                                  ((UART_TypeDef    *) UART18_BASE     )
#define UART19                                  ((UART_TypeDef    *) UART19_BASE     )
#define UART20                                  ((UART_TypeDef    *) UART20_BASE     )
#define UART21                                  ((UART_TypeDef    *) UART21_BASE     )

#define NUM_OF_TIMER                            (2)
#define TIMER0                                  ((TIM_TypeDef     *) TIMER0_BASE     )
#define TIMER1                                  ((TIM_TypeDef     *) TIMER1_BASE     )

/** @addtogroup Exported_macro
  * @{
  */

#ifndef MAX
#define MAX(a,b)                                ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef MIN
#define MIN(a,b)                                ( ((a) < (b)) ? (a) : (b) )
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)                           ( sizeof (a) / sizeof ((a)[0]) )
#endif

#define BIT(bitn)                               ( 1ULL << (bitn) )

#define SET_BIT(reg,bitn,val)                   ( (val) ? ((reg) |=  BIT(bitn)) : ((reg) &=  ~BIT(bitn)) )
#define GET_BIT(reg,bitn)                       ( ((reg) >> (bitn)) & 0x1 )

#define REG_WRITE_32(addr,data)                 Reg_Write_32(addr, data)
#define REG_WRITE_16(addr,data)                 Reg_Write_16(addr, data)
#define REG_WRITE_8(addr,data)                  Reg_Write_8(addr, data)

#define REG_READ_32(addr)                       Reg_Read_32(addr)
#define REG_READ_16(addr)                       Reg_Read_16(addr)
#define REG_READ_8(addr)                        Reg_Read_8(addr)

#define ENTER_DEADLOOP()                        for(;;)

#define MCAUSE_IS_INT                           ( GET_BIT(__get_MCAUSE(), 31) )
#define MCAUSE_IRQN                             ( __get_MCAUSE() & 0xfff )

/**
  * @brief Exported_function
  */

void __trig_trap(void);

int SystemIsLittleEndian(void);
int SystemIsStructRightAlign(void);

void Reg_Write_8(volatile uint32_t Addr, volatile uint8_t Value);
void Reg_Write_16(volatile uint32_t Addr, volatile uint16_t Value);
void Reg_Write_32(volatile uint32_t Addr, volatile uint32_t Value);

uint8_t  Reg_Read_8(volatile uint32_t Addr);
uint16_t Reg_Read_16(volatile uint32_t Addr);
uint32_t Reg_Read_32(volatile uint32_t Addr);

void drv_irq_enable (uint32_t irq_num);
void drv_irq_disable(uint32_t irq_num);

void drv_irq_vectors_init(void);

void drv_irq_register  (uint32_t irq_num, void *irq_handler);
void drv_irq_unregister(uint32_t irq_num);

/**
  * @brief RISC-V Core Operation
  */

#include "csi_core.h"

/**
  * @brief For User Debug
  */

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define UART UART0

// Note: If you want to use the following features, define 'DEBUG' macro
//  1. use the default config params, e.g. 'g_DeviceUartDefaultConfig'
//  2. dump the value of register, e.g. 'I2C0->TXFLR' (need modify code manually to dump)
//  3. enable loop testing, e.g. 'SPI loop testing'
//  4. enable 'SelfTest' function (only for demonstration purposes)
//  5. enable xprintf in interrupt handling function (disable by undef 'DEBUG_PRINT_IN_INTERRUPT' macro)
//  6. enable 'DEBUG_ASSERT' function
//  7. correct some global params in FPGA platform, e.g. 'DEBUG_DELAY_CLOCK_CORRECTED'
//
// Note:
//  1. 'DEBUG' can be combined with 'DEBUG_xxx', 'DEBUG' provides some debugging assistance for 'DEBUG_xxx'

#define DEBUG

#ifdef DEBUG
// NOTE: some peripheral not have version register
//       but we need to use the ID to determine if they are working properly
#define DEFAULT_VERSION_AND_ID 0xfadec0de

// NOTE: do not perform time-consuming operations in interrupt handle
#define DEBUG_PRINT_IN_INTERRUPT

// NOTE: for fault-tolerant, memory access may slow down code execution efficiency
//       set 'DEBUG_DELAY_CLOCK_CORRECTED' to right a bias
//       but it's very pessimistic and there's always a bias
#define DEBUG_DELAY_CLOCK_CORRECTED 10
#endif

#ifdef DEBUG
#define DEBUG_ASSERT(e) ( (void)((e) ? ((void)0) : xprintf("'%s' assert failed at %s-%s.\n", #e, __FILE__, __LINE__)) )
#else
#define DEBUG_ASSERT(e) ( (void)0 )
#endif

// Note: If you want to do some demonstration, define 'DEBUG_xxx' macro
//  1. some demonstration have optional options, e.g. 'DEBUG_PWM'
//  2. if 'DEBUG' is no definition, demonstration will not work, but you can do it through some methods
//  3. if you don't want to compile the demonstration code, undef 'DEBUG' and all 'DEBUG_xxx'

// #define DEBUG_UART
// #define DEBUG_TIMER
// #define DEBUG_WDT
// #define DEBUG_GPIO // NOTE: disturb FPGA environment
#define DEBUG_SPI
#define DEBUG_QSPI
// #define DEBUG_PWM
// #define DEBUG_EPWM
// #define DEBUG_I2C
// #define DEBUG_DMA
#define DEBUG_SMC
#define DEBUG_AMC
#define DEBUG_CAN
#define DEBUG_GMAC
// #define DEBUG_SRIO
// #define DEBUG_ASTIMER

#ifdef DEBUG_UART
#define DEBUG_UART_NORMAL 0
#define DEBUG_UART_DMA 1
#define DEBUG_UART_RS485 2

// NOTE: set 'DEBUG_UART_CASE' to select test case
#define DEBUG_UART_CASE DEBUG_UART_RS485
#endif // #ifdef DEBUG_UART

#ifdef DEBUG_QSPI
#define DEBUG_QSPI_NORMAL 0
#define DEBUG_QSPI_DMA 0

// NOTE: set 'DEBUG_QSPI_CASE' to select test case
#define DEBUG_QSPI_CASE DEBUG_QSPI_NORMAL
#endif // #ifdef DEBUG_QSPI

#ifdef DEBUG_PWM
#define DEBUG_PWM_CASE_COMPARE 0
#define DEBUG_PWM_CASE_COMPARE_COMPLEMENT 1
#define DEBUG_PWM_CASE_CAPTURE 2

// NOTE: set 'DEBUG_PWM_CASE' to select test case
#define DEBUG_PWM_CASE DEBUG_PWM_CASE_COMPARE
#endif // #ifdef DEBUG_PWM

#ifdef DEBUG_EPWM
#define DEBUG_EPWM_CASE_COMPARE 0
#define DEBUG_EPWM_CASE_CAPTURE 1

// NOTE: set 'DEBUG_EPWM_CASE' to select test case
#define DEBUG_EPWM_CASE DEBUG_EPWM_CASE_CAPTURE
#endif // #ifdef DEBUG_EPWM

#ifdef DEBUG_I2C
#define DEBUG_I2C_NORMAL 0
#define DEBUG_I2C_DMA 1

// NOTE: set 'DEBUG_I2C_CASE' to select test case
#define DEBUG_I2C_CASE DEBUG_I2C_NORMAL
#endif // #ifdef DEBUG_I2C

#ifdef DEBUG_SMC
#define DEBUG_SMC_EDAC 0
#endif // #ifdef DEBUG_SMC

#ifdef DEBUG_GMAC
#define DEBUG_GMAC_AUTO_SEARCH_PHY_ID 1
#define DEBUG_GMAC_WAIT_PHY_LINK_UP 1
#define DEBUG_GMAC_LOOPBACK 0
#endif // #ifdef DEBUG_GMAC

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XYR3201_H */
