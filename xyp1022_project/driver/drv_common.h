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

#include "XYP1022.h"

#include "environment.h"
#include "e500mc_asm.h"

#include "drv_uart.h"
#include "drv_gmac.h"
#include "drv_gmac_phy.h"

extern void mpic_unmask_Num(int VectorNum);

extern void cache_invalid(void *addr, int32_t dsize);
extern void cache_flush(void *addr, int32_t dsize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HW_COMMON_H__ */
