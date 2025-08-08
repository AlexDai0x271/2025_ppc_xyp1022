/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_GMAC_CRC_H__
#define __DRV_GMAC_CRC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYP1022.h"

#define CRC_TABLE_DEPTH (256)

/**
 * @brief Register Field Define
 */

/**
 * @brief Register Field Define
 */

/**
 * @brief Self Config Param Define
 */

/**
 * @brief Function Define
 */

uint32_t CRC_Calc(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_GMAC_CRC_H__ */
