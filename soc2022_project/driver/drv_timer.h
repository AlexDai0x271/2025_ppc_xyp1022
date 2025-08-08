/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_TIMER_H__
#define __DRV_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* TIMER CTRL */
#define TIMER_CTRL_0N100PWM_EN_POS    (4) // 1-bit
#define TIMER_CTRL_PWM_POS            (3) // 1-bit
#define TIMER_CTRL_INTERRUPT_MASK_POS (2) // 1-bit
#define TIMER_CTRL_MODE_POS           (1) // 1-bit
#define TIMER_CTRL_ENABLE_POS         (0) // 1-bit

/**
 * @brief Register Field Define
 */

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eTimerMode_FreeRunning = 0, // auto-load from 0xffffffff, first-load from LOAD register
    eTimerMode_UserDefined,     // auto-load from LOAD register
} TimerMode_e;

typedef enum
{
    eTimerChannel_0 = 0,
    eTimerChannel_1,

    eTimerChannel_Max,
} TimerChannel_e;

typedef struct
{
    uint32_t status;

    TimerMode_e mode;
    uint32_t tick1us;  // NOTE: ticks contained in 1-us
    uint32_t interval; // NOTE: how many 1-us
    uint8_t int_enable;

    void *user_data;
} DeviceTimerConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceTimerConfig_t g_DeviceTimerDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_TIMER_InitConfig(TIM_TypeDef *TIMERx, TimerChannel_e chn, DeviceTimerConfig_t *config);
int HW_TIMER_Start(TIM_TypeDef *TIMERx, TimerChannel_e chn);
int HW_TIMER_Stop(TIM_TypeDef *TIMERx, TimerChannel_e chn);
uint32_t HW_TIMER_ClearInterrupt(TIM_TypeDef *TIMERx, TimerChannel_e chn);
uint32_t HW_TIMER_GetStatus(TIM_TypeDef *TIMERx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_TIMER_H__ */
