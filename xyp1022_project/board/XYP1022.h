/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __XYP1022_H
#define __XYP1022_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief XYP1022 Clock Frequency Definition
 */
#define CPU_CLOCK_MHZ                           ((uint32_t)1200)
#define CPU_CLOCK_KHZ                           ((uint32_t)(CPU_CLOCK_MHZ*1000))
#define CPU_CLOCK_HZ                            ((uint32_t)(CPU_CLOCK_MHZ*1000*1000))

#ifndef CONFIG_SYSTICK_MS
#define CONFIG_SYSTICK_MS                       ((uint32_t)1)
#endif

/**
 * @brief XYP1022 Data Type Definition
 */
#define __I                                     volatile const
#define __O                                     volatile
#define __IO                                    volatile

/**
 * @brief XYP1022 Memory Size Definition
 */

#define KiB(n)                                  ((n) * 1024UL)
#define MiB(n)                                  ((n) * 1024UL * 1024UL)

/**
 * @brief XYP1022 DDR Space Definition
 */
#define FW_DDR_USERSPACE_OFFSET                 0x01000000 // offset: 16MiB

#define FW_DDR_FREERTOS_OFFSET                  (FW_DDR_USERSPACE_OFFSET + 0x0)
#define FW_DDR_GMAC_DESCRIPTOR_OFFSET           (FW_DDR_USERSPACE_OFFSET + MiB(16) + 0x0)
#define FW_DDR_GMAC_BUFFER_OFFSET               (FW_DDR_USERSPACE_OFFSET + MiB(16) + MiB(1))

#define FW_GMAC_DDR
#define FW_FREERTOS_DDR

#ifndef FW_GMAC_DDR
#define FW_GMAC_ADDR_START_HIGH                 0x00000004
#else
#define FW_GMAC_ADDR_START_HIGH                 0x00000000
#endif

/**
 * @brief XYP1022 Interrupt Number Definition
 */
typedef enum IRQn
{
    NotUsed_0_IRQn                              = 0,
    NotUsed_1_IRQn                              = 1,
    PCIe0_PhyReady_IRQn                         = 2,
    PCIe0_Misc_IRQn                             = 3,
    PCIe0_MsiControl_IRQn                       = 4,
    PCIe0_Edma_IRQn                             = 5,
    PCIe1_PhyReady_IRQn                         = 6,
    PCIe1_Misc_IRQn                             = 7,
    PCIe1_MsiControl_IRQn                       = 8,
    PCIe1_Edma_IRQn                             = 9,
    PCIe2_PhyReady_IRQn                         = 10,
    PCIe2_Misc_IRQn                             = 11,
    PCIe2_MsiControl_IRQn                       = 12,
    PCIe2_Edma_IRQn                             = 13,
    PCIe3_PhyReady_IRQn                         = 14,
    PCIe3_Misc_IRQn                             = 15,
    PCIe3_MsiControl_IRQn                       = 16,
    PCIe3_Edma_IRQn                             = 17,
    PCIe0_Prereset_IRQn                         = 18,
    PCIe1_Prereset_IRQn                         = 19,
    PCIe2_Prereset_IRQn                         = 20,
    PCIe3_Prereset_IRQn                         = 21,
    Ebco_IRQn                                   = 22,
    NotUsed_23_IRQn                             = 23,
    NotUsed_24_IRQn                             = 24,
    NotUsed_25_IRQn                             = 25,
    NotUsed_26_IRQn                             = 26,
    DMA0_IRQn                                   = 27,
    DMA1_IRQn                                   = 28,
    UART0_IRQn                                  = 29,
    UART1_IRQn                                  = 30,
    UART2_IRQn                                  = 31,
    UART3_IRQn                                  = 32,
    IIC0_IRQn                                   = 33,
    IIC1_IRQn                                   = 34,
    IIC2_IRQn                                   = 35,
    IIC3_IRQn                                   = 36,
    USB0_HostSmi_IRQn                           = 37,
    USB0_Otg_IRQn                               = 38,
    USB0_Subsystem_IRQn                         = 39,
    USB1_HostSmi_IRQn                           = 40,
    USB1_Otg_IRQn                               = 41,
    USB1_Subsystem_IRQn                         = 42,
    EMMC_IRQn                                   = 43,
    SPI0_Tfuf_Rfof_IRQn                         = 44,
    SPI0_Rfdf_IRQn                              = 45,
    SPI0_Tfff_IRQn                              = 46,
    SPI0_Eoqf_IRQn                              = 47,
    SPI0_Tcf_IRQn                               = 48,
    GPIO_M_EXPORT_15                            = 49,
    GPIO_M_EXPORT_14                            = 50,
    GPIO_M_EXPORT_13                            = 51,
    GPIO_M_EXPORT_12                            = 52,
    GPIO_M_EXPORT_11                            = 53,
    GPIO_M_EXPORT_10                            = 54,
    GPIO_M_EXPORT_9                             = 55,
    GPIO_M_EXPORT_8                             = 56,
    GPIO_M_EXPORT_7                             = 57,
    GPIO_M_EXPORT_6                             = 58,
    GPIO_M_EXPORT_5                             = 59,
    GPIO_M_EXPORT_4                             = 60,
    GPIO_M_EXPORT_3                             = 61,
    GPIO_M_EXPORT_2                             = 62,
    GPIO_M_EXPORT_1                             = 63,
    GPIO_M_EXPORT_0                             = 64,
    NotUsed_65_IRQn                             = 65,
    GPIO_G_Group_IRQn                           = 66,
    GPIO_F_Group_IRQn                           = 67,
    GPIO_E_Group_IRQn                           = 68,
    GPIO_D_Group_IRQn                           = 69,
    GPIO_C_Group_IRQn                           = 70,
    GPIO_B_Group_IRQn                           = 71,
    GPIO_A_Group_IRQn                           = 72,
    SATA0_Pme_IRQn                              = 73,
    SATA0_Msi_IRQn                              = 74,
    SATA0_IRQn                                  = 75,
    SATA1_Pme_IRQn                              = 76,
    SATA1_Msi_IRQn                              = 77,
    SATA1_IRQn                                  = 78,
    Icdc_IRQn                                   = 79,
    SEC_Misc_IRQn                               = 80,
    SEC_8_IRQn                                  = 81,
    SEC_7_IRQn                                  = 82,
    SEC_6_IRQn                                  = 83,
    SEC_5_IRQn                                  = 84,
    SEC_4_IRQn                                  = 85,
    SEC_3_IRQn                                  = 86,
    SEC_2_IRQn                                  = 87,
    SEC_1_IRQn                                  = 88,
    SEC_0_IRQn                                  = 89,
    SEC_Mon_IRQn                                = 90,
    DDR_Normal_2_IRQn                           = 91,
    DDR_Normal_1_IRQn                           = 92,
    DDR_Normal_0_IRQn                           = 93,
    Aurora_IRQn                                 = 94,
    MIM_Onfi30_IRQn                             = 95,
    MIM_CrcDone_IRQn                            = 96,
    MIM_EccDone_IRQn                            = 97,
    MIM_Ecc_DecodeFail_IRQn                     = 98,
    MIM_Ecc_CodeDonw_IRQn                       = 99,

    IRQn_Max                                    = 128,
} IRQn_Type;

/**
  * @brief Universal Asynchronous Receiver Transmitter
  */

typedef struct
{
    union {
    __I  uint8_t RBR;                           /*!< UART Receive Buffer Register                                       Address offset:0x0      */
    __O  uint8_t THR;                           /*!< UART Transmit Holding Register                                     Address offset:0x0      */
    __IO uint8_t DLL;                           /*!< UART Divisor Latch Low Register                                    Address offset:0x0      */
    };
    union {
    __IO uint8_t IER;                           /*!< UART Interrupt Enable Register                                     Address offset:0x1      */
    __IO uint8_t DLM;                           /*!< UART Divisor Latch High Register                                   Address offset:0x1      */
    };
    union {
    __I  uint8_t IIR;                           /*!< UART Interrupt Identification Register                             Address offset:0x2      */
    __O  uint8_t FCR;                           /*!< UART FIFO Control Register                                         Address offset:0x2      */
    };
    __IO uint8_t LCR;                           /*!< UART Line Control Register                                         Address offset:0x3      */
    __IO uint8_t MCR;                           /*!< UART Modem Control Register                                        Address offset:0x4      */
    __IO uint8_t LSR;                           /*!< UART Line Status Register                                          Address offset:0x5      */
    __IO uint8_t MSR;                           /*!< UART Modem Status Register                                         Address offset:0x6      */
    __IO uint8_t SCR;                           /*!< UART Scratchpad Register                                           Address offset:0x7      */
} UART_TypeDef;

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

/** @addtogroup Peripheral memory map
  * @{
  */

#define DPAA_CFG_BASE                           (0xea000000)

#define GMAC0_CFG_BASE                          ((DPAA_CFG_BASE | 0x00400000 | 0x4d0000) + 0x4000 * 2)  // 32KB
#define UART0_BASE                              (0xe9608000)  // 4KB
#define UART1_BASE                              (0xe9609000)  // 4KB
#define UART2_BASE                              (0xe960a000)  // 4KB
#define UART3_BASE                              (0xe960b000)  // 4KB
#define SDHC0_BASE                              (0xe9100000)  // 1MB

/** @addtogroup Peripheral_declaration
  * @{
  */
#define NUM_OF_GMAC                             (1)
#define GMAC0                                   ((GMAC_TypeDef    *) GMAC0_CFG_BASE  )

#define NUM_OF_GDMA                             (1)
#define GDMA0                                   ((GDMA_TypeDef    *) GMAC0_CFG_BASE  )

#define NUM_OF_UART                             (4)
#define UART0                                   ((UART_TypeDef    *) UART0_BASE      )
#define UART1                                   ((UART_TypeDef    *) UART1_BASE      )
#define UART2                                   ((UART_TypeDef    *) UART2_BASE      )
#define UART3                                   ((UART_TypeDef    *) UART3_BASE      )

/** @addtogroup Exported_macro
  * @{
  */

#define ENTER_CRITICAL()                        asm volatile("wrteei 0 \n\t msync \n\t isync" ::: "memory")
#define EXIT_CRITICAL()                         asm volatile("msync \n\t isync \n\t wrteei 1" ::: "memory")

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

/**
  * @brief Exported_function
  */

// int SystemIsLittleEndian(void);
// int SystemIsStructRightAlign(void);

void Reg_Write_8(volatile uint32_t Addr, volatile uint8_t Value);
void Reg_Write_16(volatile uint32_t Addr, volatile uint16_t Value);
void Reg_Write_32(volatile uint32_t Addr, volatile uint32_t Value);

uint8_t  Reg_Read_8(volatile uint32_t Addr);
uint16_t Reg_Read_16(volatile uint32_t Addr);
uint32_t Reg_Read_32(volatile uint32_t Addr);

/**
  * @brief For User Debug
  */

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define UART UART0

// Note: If you want to use the following features, define 'DEBUG' macro
//  1. use the default config params, e.g. 'g_DeviceUartDefaultConfig'
//  2. enable 'SelfTest' function (only for demonstration purposes)
//  3. enable xprintf in interrupt handling function (disable by undef 'DEBUG_PRINT_IN_INTERRUPT' macro)
//  4. enable 'DEBUG_ASSERT' function
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
#define DEBUG_GMAC

#ifdef DEBUG_UART
#define DEBUG_UART_NORMAL 0

// NOTE: set 'DEBUG_UART_CASE' to select test case
#define DEBUG_UART_CASE DEBUG_UART_NORMAL
#endif // #ifdef DEBUG_UART

#ifdef DEBUG_GMAC
#define DEBUG_GMAC_CHECK_PHY_ID 0
#define DEBUG_GMAC_WAIT_PHY_LINK_UP 0
#define DEBUG_GMAC_LOOPBACK 0
#endif // #ifdef DEBUG_GMAC

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XYP1022_H */
