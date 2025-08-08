/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __SYS_FREQ_H
#define __SYS_FREQ_H

#include "XYP1022.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TB_CLOCK_MHZ (CPU_CLOCK_MHZ / 128)
#define TB_CLOCK_KHZ (CPU_CLOCK_KHZ / 128)

typedef union
{
    struct
    {
        uint32_t CHIP_RST_MODE  : 1;
        uint32_t EFUSE_CLK_SEL  : 1;
        uint32_t CORE_SPEED     : 2;
        uint32_t DDR_SPEED      : 3;
        uint32_t BUS_CLK_FREQ   : 2;
        uint32_t CFG_ROM_LC     : 1;
        uint32_t CFG_SB_DIS     : 1;
        uint32_t PCIE0_TYPE     : 1;
        uint32_t PCIE1_TYPE     : 1;
        uint32_t PCIE2_TYPE     : 1;
        uint32_t PCIE3_TYPE     : 1;
        uint32_t reserved0      : 2;
        uint32_t CPU1_DISABLE   : 1;
        uint32_t PRESENT_N_MODE : 1;
        uint32_t EBC_NOT_NFC    : 1;
        uint32_t reserved1      : 2;
        uint32_t reserved2      : 1;
        uint32_t HALT_CFG_B     : 1;
        uint32_t SERDES_CFG     : 8;
    } f;
    uint32_t d;
} StrapPin_t;

static inline uint64_t read_time_base(void)
{
    uint32_t tbl, tbu, tbu_temp;

    // Read TBU (upper 32 bits) first, then TBL (lower 32 bits)
    __asm__ volatile(
        "1: mftbu %0\n"    // Read upper time base register
        "   mftb %1\n"     // Read lower time base register
        "   mftbu %2\n"    // Read upper time base register again
        "   cmpw %0, %2\n" // Check if upper register has changed
        "   bne 1b"        // If changed, repeat reading
        : "=r"(tbu), "=r"(tbl), "=r"(tbu_temp));

    return ((uint64_t)tbu << 32) | tbl;
}

int get_core_speed(int sel);
int get_ddr_speed(int sel);

uint32_t drv_get_sys_freq(void);
uint32_t drv_get_cpu_freq(uint32_t idx);
uint32_t drv_get_uart_freq(void);

void udelay(volatile uint32_t us);
void mdelay(volatile uint32_t ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SYS_FREQ_H */
