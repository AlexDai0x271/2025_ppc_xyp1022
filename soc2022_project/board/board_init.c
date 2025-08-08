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

void Reg_Write_8(volatile uint32_t Addr, volatile uint8_t Value)
{
    volatile uint8_t *LocalAddr = (volatile uint8_t *)Addr;
    *LocalAddr = Value;
}

void Reg_Write_16(volatile uint32_t Addr, volatile uint16_t Value)
{
    volatile uint16_t *LocalAddr = (volatile uint16_t *)Addr;
    *LocalAddr = Value;
}

void Reg_Write_32(volatile uint32_t Addr, volatile uint32_t Value)
{
    volatile uint32_t *LocalAddr = (volatile uint32_t *)Addr;
    *LocalAddr = Value;
}

uint8_t Reg_Read_8(volatile uint32_t Addr)
{
    return *(volatile uint8_t *)Addr;
}

uint16_t Reg_Read_16(volatile uint32_t Addr)
{
    return *(volatile uint16_t *)Addr;
}

uint32_t Reg_Read_32(volatile uint32_t Addr)
{
    return *(volatile uint32_t *)Addr;
}

void cache_invalid(void *addr, int32_t dsize)
{
    csi_dcache_invalid_range((uint32_t *)addr, dsize);
}

void cache_flush(void *addr, int32_t dsize)
{
    csi_dcache_clean_range((uint32_t *)addr, dsize);
}

void board_init(void)
{
    HW_UART_InitConfig(UART, &g_DeviceUartDefaultConfig);
}
