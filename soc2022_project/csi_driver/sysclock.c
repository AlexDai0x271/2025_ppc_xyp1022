/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "XYR3201.h"

uint32_t g_system_clock = CPU_CLOCK_HZ;

uint32_t drv_get_sys_freq(void)
{
    return g_system_clock;
}

uint32_t drv_get_cpu_freq(uint32_t idx)
{
    return drv_get_sys_freq();
}

uint32_t drv_get_bus_freq(void)
{
    return drv_get_sys_freq() / 4;
}

uint32_t drv_get_ahb_freq(void)
{
    return drv_get_sys_freq() / 6;
}

uint32_t drv_get_apb_freq(void)
{
#ifndef USE_QEMU_PLAT
    return drv_get_sys_freq() / 12;
#else
    return drv_get_sys_freq();
#endif // #ifndef USE_QEMU_PLAT
}
