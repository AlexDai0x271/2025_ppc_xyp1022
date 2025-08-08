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
#include "sysclock.h"

static inline void _udelay(void)
{
    register uint32_t cnt = (drv_get_sys_freq() / 1000 / 1000);

    register uint32_t startl, starth;
    register uint32_t curl, curh;
    register uint32_t delta;

    startl = csi_coret_get_value();
    starth = csi_coret_get_valueh();

    while (1) {
        curl = csi_coret_get_value();
        curh = csi_coret_get_valueh();

        delta = (((unsigned long long)curh << 32) | curl) - (((unsigned long long)starth << 32) | startl);

        if (delta >= cnt) {
            return;
        }
    }
}

void udelay(uint32_t us)
{
    if (us == 0) {
        return;
    }

#ifdef DEBUG_DELAY_CLOCK_CORRECTED
#if (DEBUG_DELAY_CLOCK_CORRECTED != 0)
    us /= DEBUG_DELAY_CLOCK_CORRECTED;
#endif
#endif

    while (us--) {
        _udelay();
    }
}

void mdelay(uint32_t ms)
{
    if (ms == 0) {
        return;
    }

    while (ms--) {
        udelay(1000);
    }
}
