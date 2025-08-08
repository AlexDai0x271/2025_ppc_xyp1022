/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "XYP1022.h"
#include "environment.h"
#include "sysclock.h"

#define CLOCK_HZ_3700K (3700 * 1000)

int get_core_speed(int sel)
{
    int speed = 0;

    switch (sel) {
    case 0x0:
        speed = 600;
        break;
    case 0x1:
        speed = 1330;
        break;
    case 0x2:
        speed = 1200;
        break;
    case 0x3:
        speed = 1000;
        break;
    }

    return speed;
}

int get_ddr_speed(int sel)
{
    int speed;

    switch (sel) {
    case 0x0:
        speed = 1200;
        break;
    case 0x1:
        speed = 1096;
        break;
    case 0x2:
        speed = 933;
        break;
    case 0x3:
        speed = 800;
        break;
    case 0x4:
        speed = 667;
        break;
    case 0x5:
        speed = 533;
        break;
    case 0x6:
        speed = 400;
        break;
    case 0x7:
        speed = 226;
        break;
    }

    return speed;
}

uint32_t drv_get_sys_freq(void)
{
    return CPU_CLOCK_HZ;
}

uint32_t drv_get_cpu_freq(uint32_t idx)
{
    return drv_get_sys_freq();
}

uint32_t drv_get_uart_freq(void)
{
    return CLOCK_HZ_3700K;
}
