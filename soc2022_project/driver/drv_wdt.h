/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_WDT_H__
#define __DRV_WDT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* WDT CR */
#define WDT_CR_NO_NAME_POS     (5) // 1-bit
#define WDT_CR_RPL_POS         (2) // 3-bit
#define WDT_CR_RMOD_POS        (1) // 1-bit
#define WDT_CR_WDT_EN_POS      (0) // 1-bit

/* WDT TORR */
#define WDT_TORR_TOP_INIT_POS  (4) // 4-bit
#define WDT_TORR_TOP_POS       (0) // 4-bit

/**
 * @brief Register Field Define
 */

/* value of CR_RPL */
#define CR_RPL_PCLK_CYCLES_2   (0x0)
#define CR_RPL_PCLK_CYCLES_4   (0x1)
#define CR_RPL_PCLK_CYCLES_8   (0x2)
#define CR_RPL_PCLK_CYCLES_16  (0x3)
#define CR_RPL_PCLK_CYCLES_32  (0x4)
#define CR_RPL_PCLK_CYCLES_64  (0x5)
#define CR_RPL_PCLK_CYCLES_128 (0x6)
#define CR_RPL_PCLK_CYCLES_256 (0x7)

/* value of CR_RMOD */
#define RMOD_RESET             (0)
#define RMOD_INTERRUPT         (1)

/* value of CR_WDT_EN */
#define WDT_EN_DISABLED        (0)
#define WDT_EN_ENABLED         (1)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eWdtMode_Reset = RMOD_RESET,
    eWdtMode_Interrupt = RMOD_INTERRUPT,
} WdtMode_e;

typedef enum
{
    eWdtTimeout_64K = 0,
    eWdtTimeout_128K,
    eWdtTimeout_256K,
    eWdtTimeout_512K,
    eWdtTimeout_1M,
    eWdtTimeout_2M,
    eWdtTimeout_4M,
    eWdtTimeout_8M,
    eWdtTimeout_16M,
    eWdtTimeout_32M,
    eWdtTimeout_64M,
    eWdtTimeout_128M,
    eWdtTimeout_256M,
    eWdtTimeout_512M,
    eWdtTimeout_1G,
    eWdtTimeout_2G,
} WdtTimeout_e;

typedef struct
{
    uint32_t status;

    WdtMode_e mode;
    WdtTimeout_e timeout;

    void *user_data;
} DeviceWdtConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceWdtConfig_t g_DeviceWdtDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_WDT_InitConfig(WDT_TypeDef *WDTx, DeviceWdtConfig_t *config);
int HW_WDT_Start(WDT_TypeDef *WDTx);
int HW_WDT_Feed(WDT_TypeDef *WDTx);
uint32_t HW_WDT_GetValue(WDT_TypeDef *WDTx);
uint32_t HW_WDT_GetStatus(WDT_TypeDef *WDTx);
uint32_t HW_WDT_ClearInterrupt(WDT_TypeDef *WDTx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_WDT_H__ */
