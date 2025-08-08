/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __HW_COMMON_H__
#define __HW_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

#include "drv_uart.h"
#include "drv_timer.h"
#include "drv_wdt.h"
#include "drv_gpio.h"
#include "drv_spi.h"
#include "drv_qspi.h"
#include "drv_flash.h"
#include "drv_pwm.h"
#include "drv_epwm.h"
#include "drv_i2c.h"
#include "drv_smc.h"
#include "drv_amc.h"
#include "drv_can.h"
#include "drv_gmac.h"
#include "drv_gmac_phy.h"
#include "drv_srio.h"
#include "drv_astimer.h"

extern void cache_invalid(void *addr, int32_t dsize);
extern void cache_flush(void *addr, int32_t dsize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HW_COMMON_H__ */
