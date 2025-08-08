/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/**
 * @brief Register Field Define
 */

/* value of Register */
#define GPIO_DISABLED            (0)
#define GPIO_ENABLED             (1)

/* value of PORTx_DDR */
#define DDR_IN                   (0)
#define DDR_OUT                  (1)

/* value of PORTx_CTL */
#define CTL_SOFTWARE_MODE        (0)
#define CTL_HARDWARE_MODE        (1)

/* value of INTMASK */
#define INTMASK_UNMASKED         (0)
#define INTMASK_MASKED           (1)

/* value of INTTYPE_LEVEL */
#define INTTYPE_LEVEL_SENSITIVE  (0)
#define INTTYPE_EDGE_SENSITIVE   (1)

/* value of INT_POLARITY */
#define INT_POLARITY_ACTIVE_LOW  (0)
#define INT_POLARITY_ACTIVE_HIGH (1)

/**
 * @brief Self Config Param Define
 */

#define LEVEL_LOW                (0)
#define LEVEL_HIGH               (1)

#define PAout(n, lvl)            (SET_BIT(GPIOA->PORTA_DR, n, lvl))
#define PAstate(n)               (GET_BIT(GPIOA->PORTA_DR, n))
#define PAin(n)                  (GET_BIT(GPIOA->EXT_PORTA, n))

typedef enum
{
    eGpioMode_Input = 0,
    eGpioMode_InputInterrupt,
    eGpioMode_Output,
} GpioMode_e;

typedef enum
{
    eGpioIntMode_LevelLow = 0,
    eGpioIntMode_LevelHigh,
    eGpioIntMode_EdgeFalling,
    eGpioIntMode_EdgeRising,
    eGpioIntMode_EdgeBoth,
} GpioIntMode_e;

typedef struct
{
    uint32_t status;

    uint8_t pin;
    GpioMode_e mode;
    uint8_t init_level;     // NOTE: only valid when .mode is 'eGpioMode_Output'
    GpioIntMode_e int_mode; // NOTE: only valid when .mode is 'eGpioMode_InputInterrupt'

    void *user_data;
} DeviceGpioConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceGpioConfig_t g_DeviceGpioDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_GPIO_InitConfig(GPIO_TypeDef *GPIOx, DeviceGpioConfig_t *config);
uint32_t HW_GPIO_ClearInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin);
uint32_t HW_GPIO_GetStatus(GPIO_TypeDef *GPIOx);
uint32_t HW_GPIO_GetPreMaskStatus(GPIO_TypeDef *GPIOx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_GPIO_H__ */
