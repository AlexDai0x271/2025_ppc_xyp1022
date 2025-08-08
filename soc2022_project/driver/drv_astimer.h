/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __HW_ASTIMER_H__
#define __HW_ASTIMER_H__

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

/**
 * @brief Self Config Param Define
 */

#define addr2idx(addr) ((addr) >> 2)

typedef enum
{
    eAstimerMode_CTRLPERIOD = 0,
    eAstimerMode_SYNC,
    eAstimerMode_GPS,
    eAstimerMode_RTC,
} AstimerMode_e;

typedef enum
{
    eAstimerClockMode_Internal = 0,
    eAstimerClockMode_External,
} AstimerClockMode_e;

typedef enum
{
    eAstimerInterval_1US = 0x0,
    eAstimerInterval_10US,
    eAstimerInterval_50US,
    eAstimerInterval_100US,
} AstimerInterval_e;

typedef enum
{
    eAstimerOutSignal_High = 0x0,
    eAstimerOutSignal_Low,
    eAstimerOutSignal_Sync_x1,
    eAstimerOutSignal_Sync_x2,
    eAstimerOutSignal_Sync_x4,
    eAstimerOutSignal_Sync_x8,
    eAstimerOutSignal_Reserved6,
    eAstimerOutSignal_Reserved7,
    eAstimerOutSignal_GPS0,
    eAstimerOutSignal_GPS1,
    eAstimerOutSignal_GPS2,
    eAstimerOutSignal_GPS3,
    eAstimerOutSignal_CtrlPeriod,
    eAstimerOutSignal_Reserved13,
    eAstimerOutSignal_Reserved14,
    eAstimerOutSignal_Reserved15,
} AstimerOutSignal_e;

typedef enum
{
    eAstimerOutPulse_Pos = 0,
    eAstimerOutPulse_Neg,
} AstimerOutPulse_e;

typedef enum
{
    eAstimerGPSSelect_GPS0 = 0x0,
    eAstimerGPSSelect_GPS1,
    eAstimerGPSSelect_GPS2,
    eAstimerGPSSelect_GPS3,
} AstimerGPSSelect_e;

typedef enum
{
    eAstimerReadMode_Current = 0,
    eAstimerReadMode_CtrlPeriod,
    eAstimerReadMode_GPS0,
    eAstimerReadMode_GPS1,
    eAstimerReadMode_GPS2,
    eAstimerReadMode_GPS3,
    eAstimerReadMode_Signal0,
    eAstimerReadMode_Signal1,
    eAstimerReadMode_Signal2,
    eAstimerReadMode_Signal3,
} AstimerReadMode_e;

typedef struct
{
    uint32_t status;

    AstimerMode_e mode;

    AstimerClockMode_e clock_mode;

    uint32_t freq;

    AstimerInterval_e ctl_period_interval;
    uint32_t ctl_period_tick;

    AstimerInterval_e interval;

    AstimerOutSignal_e output_signal[4];
    AstimerOutPulse_e output_pulse[4];
    uint32_t output_pulse_width[4];

    AstimerGPSSelect_e gps_select;

    void *user_data;
} DeviceAstimerConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceAstimerConfig_t g_DeviceAstimerDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_ASTIMER_InitConfig(ASTIMER_TypeDef *ASTIMERx, DeviceAstimerConfig_t *config);
int HW_ASTIMER_ReadData(ASTIMER_TypeDef *ASTIMERx, AstimerReadMode_e mode, uint64_t *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HW_ASTIMER_H__ */
