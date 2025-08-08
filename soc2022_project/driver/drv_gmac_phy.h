/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_GMAC_PHY_H__
#define __DRV_GMAC_PHY_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Register Field Define
 */

#include "drv_gmac.h"

/**
 * @brief Register Field Define
 */

/**
 * @brief Self Config Param Define
 */

/* Physical Layer Address for Marvell 88E1111 */
#define PHY_ID                     (5)

/* Physical Identifier for Marvell 88E1111 */
#define PHY_IDENTIFIER_MAKER(a, b) (((a) << 16) | (b))
#define PHY_IDENTIFIER             PHY_IDENTIFIER_MAKER(0x0141, 0x0cc2)

/* Register for Marvell 88E1111 */
#define PHY_REG_CONTROL            (0)
#define PHY_REG_STATUS             (1)
#define PHY_REG_ID0                (2)
#define PHY_REG_ID1                (3)
#define PHY_REG_16                 (16)
#define PHY_REG_17                 (17)
#define PHY_REG_21                 (21)
#define PHY_REG_PAGESELECT22       (22) // NOTE: select the page[0,1] of register 0-1,4-8,17-19
#define PHY_REG_27                 (27)
#define PHY_REG_PAGESELECT29       (29) // NOTE: select the page[3,7,12,16,18] of register 30

/* Page for Marvell 88E1111, only followd is supported */
#define PHY_PAGE_0                 (0)
#define PHY_PAGE_1                 (1)
#define PHY_PAGE_2                 (2)
#define PHY_PAGE_3                 (3)
#define PHY_PAGE_7                 (7)
#define PHY_PAGE_12                (12)
#define PHY_PAGE_16                (16)
#define PHY_PAGE_18                (18)

/* Default Page for Marvell 88E1111, only support page 0 or 1 */
#define PHY_PAGE_DEF               (PHY_PAGE_0)

/**
 * @brief Function Define
 */

uint32_t PHY_Read(GMAC_TypeDef *GMACx, uint8_t phyid, uint32_t addr, uint32_t timeout);
uint32_t PHY_Write(GMAC_TypeDef *GMACx, uint8_t phyid, uint32_t addr, uint32_t data, uint32_t timeout);
int PHY_InitConfig(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);
int PHY_GetLinkStatus(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);
uint32_t PHY_GetLinkSpeed(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);
uint32_t PHY_GetLinkDuplex(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_GMAC_PHY_H__ */
