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
#include "sysclock.h"

static inline void _udelay(void)
{
    // Calculate target time base ticks based on the clock frequency
    volatile uint64_t start, end, tb_ticks;

    tb_ticks = TB_CLOCK_MHZ; // Convert microseconds to ticks

    start = read_time_base(); // Get the current time base value
    end = start + tb_ticks;   // Calculate the end time base value

    // Busy-wait until the current time base reaches the end value
    while (read_time_base() < end) {
        // Do nothing, just wait
    }
}

static inline void _mdelay(void)
{
    // Calculate target time base ticks based on the clock frequency
    volatile uint64_t start, end, tb_ticks;

    tb_ticks = TB_CLOCK_KHZ; // Convert milliseconds to ticks

    start = read_time_base(); // Get the current time base value
    end = start + tb_ticks;   // Calculate the end time base value

    // Busy-wait until the current time base reaches the end value
    while (read_time_base() < end) {
        // Do nothing, just wait
    }
}

void udelay(volatile uint32_t us)
{
    if (us == 0) {
        return;
    }

    while (us--) {
        _udelay();
    }
}

void mdelay(volatile uint32_t ms)
{
    if (ms == 0) {
        return;
    }

    while (ms--) {
        _mdelay();
    }
}
