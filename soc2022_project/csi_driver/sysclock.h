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

#include "XYR3201.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint32_t drv_get_sys_freq(void);
uint32_t drv_get_cpu_freq(uint32_t idx);
uint32_t drv_get_bus_freq(void);
uint32_t drv_get_ahb_freq(void);
uint32_t drv_get_apb_freq(void);

void udelay(uint32_t us);
void mdelay(uint32_t ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SYS_FREQ_H */
