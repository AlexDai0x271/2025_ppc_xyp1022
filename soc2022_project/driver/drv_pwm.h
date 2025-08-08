/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* PWM PPR */
#define PWM_PPR_CP3_POS        (24) // 8-bit
#define PWM_PPR_CP2_POS        (16) // 8-bit
#define PWM_PPR_CP1_POS        (8)  // 8-bit
#define PWM_PPR_CP0_POS        (0)  // 8-bit

/* PWM DZ */
#define PWM_DZ_DZI3_POS        (24) // 8-bit
#define PWM_DZ_DZI2_POS        (16) // 8-bit
#define PWM_DZ_DZI1_POS        (8)  // 8-bit
#define PWM_DZ_DZI0_POS        (0)  // 8-bit

/* PWM PCSR */
#define PWM_PCSR_CSR7_POS      (28) // 4-bit
#define PWM_PCSR_CSR6_POS      (24) // 4-bit
#define PWM_PCSR_CSR5_POS      (20) // 4-bit
#define PWM_PCSR_CSR4_POS      (16) // 4-bit
#define PWM_PCSR_CSR3_POS      (12) // 4-bit
#define PWM_PCSR_CSR2_POS      (8)  // 4-bit
#define PWM_PCSR_CSR1_POS      (4)  // 4-bit
#define PWM_PCSR_CSR0_POS      (0)  // 4-bit

/* PWM PCR */
#define PWM_PCR_CH3MOD_POS     (27) // 1-bit
#define PWM_PCR_CH3INV_POS     (26) // 1-bit
#define PWM_PCR_CH7EN_POS      (25) // 1-bit
#define PWM_PCR_CH3EN_POS      (24) // 1-bit
#define PWM_PCR_CH2MOD_POS     (19) // 1-bit
#define PWM_PCR_CH2INV_POS     (18) // 1-bit
#define PWM_PCR_CH6EN_POS      (17) // 1-bit
#define PWM_PCR_CH2EN_POS      (16) // 1-bit
#define PWM_PCR_CH1MOD_POS     (11) // 1-bit
#define PWM_PCR_CH1INV_POS     (10) // 1-bit
#define PWM_PCR_CH5EN_POS      (9)  // 1-bit
#define PWM_PCR_CH1EN_POS      (8)  // 1-bit
#define PWM_PCR_DZEN1_POS      (5)  // 1-bit
#define PWM_PCR_DZEN0_POS      (4)  // 1-bit
#define PWM_PCR_CH0MOD_POS     (3)  // 1-bit
#define PWM_PCR_CH0INV_POS     (2)  // 1-bit
#define PWM_PCR_CH4EN_POS      (1)  // 1-bit
#define PWM_PCR_CH0EN_POS      (0)  // 1-bit

/* PWM PCR1 */
#define PWM_PCR_CH7MOD_POS     (27) // 1-bit
#define PWM_PCR_CH7INV_POS     (26) // 1-bit
#define PWM_PCR_CH6MOD_POS     (19) // 1-bit
#define PWM_PCR_CH6INV_POS     (18) // 1-bit
#define PWM_PCR_CH5MOD_POS     (11) // 1-bit
#define PWM_PCR_CH5INV_POS     (10) // 1-bit
#define PWM_PCR_DZEN3_POS      (5)  // 1-bit
#define PWM_PCR_DZEN2_POS      (4)  // 1-bit
#define PWM_PCR_CH4MOD_POS     (3)  // 1-bit
#define PWM_PCR_CH4INV_POS     (2)  // 1-bit

/* PWM PCCRx */
#define PWM_PCCR_CFLRD1_POS    (23) // 1-bit
#define PWM_PCCR_CRLRD1_POS    (22) // 1-bit
#define PWM_PCCR_CAPIF1_POS    (20) // 1-bit
#define PWM_PCCR_CAPCH1_EN_POS (19) // 1-bit
#define PWM_PCCR_FL_IE1_POS    (18) // 1-bit
#define PWM_PCCR_RL_IE1_POS    (17) // 1-bit
#define PWM_PCCR_INV1_POS      (16) // 1-bit
#define PWM_PCCR_CFLRD0_POS    (7)  // 1-bit
#define PWM_PCCR_CRLRD0_POS    (6)  // 1-bit
#define PWM_PCCR_CAPIF0_POS    (4)  // 1-bit
#define PWM_PCCR_CAPCH0_EN_POS (3)  // 1-bit
#define PWM_PCCR_FL_IE0_POS    (2)  // 1-bit
#define PWM_PCCR_RL_IE0_POS    (1)  // 1-bit
#define PWM_PCCR_INV0_POS      (0)  // 1-bit

#define PWM_PCCR_CFLRD3_POS    (23) // 1-bit
#define PWM_PCCR_CRLRD3_POS    (22) // 1-bit
#define PWM_PCCR_CAPIF3_POS    (20) // 1-bit
#define PWM_PCCR_CAPCH3_EN_POS (19) // 1-bit
#define PWM_PCCR_FL_IE3_POS    (18) // 1-bit
#define PWM_PCCR_RL_IE3_POS    (17) // 1-bit
#define PWM_PCCR_INV3_POS      (16) // 1-bit
#define PWM_PCCR_CFLRD2_POS    (7)  // 1-bit
#define PWM_PCCR_CRLRD2_POS    (6)  // 1-bit
#define PWM_PCCR_CAPIF2_POS    (4)  // 1-bit
#define PWM_PCCR_CAPCH2_EN_POS (3)  // 1-bit
#define PWM_PCCR_FL_IE2_POS    (2)  // 1-bit
#define PWM_PCCR_RL_IE2_POS    (1)  // 1-bit
#define PWM_PCCR_INV2_POS      (0)  // 1-bit

#define PWM_PCCR_CFLRD5_POS    (23) // 1-bit
#define PWM_PCCR_CRLRD5_POS    (22) // 1-bit
#define PWM_PCCR_CAPIF5_POS    (20) // 1-bit
#define PWM_PCCR_CAPCH5_EN_POS (19) // 1-bit
#define PWM_PCCR_FL_IE5_POS    (18) // 1-bit
#define PWM_PCCR_RL_IE5_POS    (17) // 1-bit
#define PWM_PCCR_INV5_POS      (16) // 1-bit
#define PWM_PCCR_CFLRD4_POS    (7)  // 1-bit
#define PWM_PCCR_CRLRD4_POS    (6)  // 1-bit
#define PWM_PCCR_CAPIF4_POS    (4)  // 1-bit
#define PWM_PCCR_CAPCH4_EN_POS (3)  // 1-bit
#define PWM_PCCR_FL_IE4_POS    (2)  // 1-bit
#define PWM_PCCR_RL_IE4_POS    (1)  // 1-bit
#define PWM_PCCR_INV4_POS      (0)  // 1-bit

#define PWM_PCCR_CFLRD7_POS    (23) // 1-bit
#define PWM_PCCR_CRLRD7_POS    (22) // 1-bit
#define PWM_PCCR_CAPIF7_POS    (20) // 1-bit
#define PWM_PCCR_CAPCH7_EN_POS (19) // 1-bit
#define PWM_PCCR_FL_IE7_POS    (18) // 1-bit
#define PWM_PCCR_RL_IE7_POS    (17) // 1-bit
#define PWM_PCCR_INV7_POS      (16) // 1-bit
#define PWM_PCCR_CFLRD6_POS    (7)  // 1-bit
#define PWM_PCCR_CRLRD6_POS    (6)  // 1-bit
#define PWM_PCCR_CAPIF6_POS    (4)  // 1-bit
#define PWM_PCCR_CAPCH6_EN_POS (3)  // 1-bit
#define PWM_PCCR_FL_IE6_POS    (2)  // 1-bit
#define PWM_PCCR_RL_IE6_POS    (1)  // 1-bit
#define PWM_PCCR_INV6_POS      (0)  // 1-bit

/* PWM PCRLRx */
#define PWM_PCRLR_CRLR4_POS    (16) // 16-bit
#define PWM_PCRLR_CRLR0_POS    (0)  // 16-bit

#define PWM_PCRLR_CRLR5_POS    (16) // 16-bit
#define PWM_PCRLR_CRLR1_POS    (0)  // 16-bit

#define PWM_PCRLR_CRLR6_POS    (16) // 16-bit
#define PWM_PCRLR_CRLR2_POS    (0)  // 16-bit

#define PWM_PCRLR_CRLR7_POS    (16) // 16-bit
#define PWM_PCRLR_CRLR3_POS    (0)  // 16-bit

/* PWM PCRLRx */
#define PWM_PCFLR_CFLR4_POS    (16) // 16-bit
#define PWM_PCFLR_CFLR0_POS    (0)  // 16-bit

#define PWM_PCFLR_CFLR5_POS    (16) // 16-bit
#define PWM_PCFLR_CFLR1_POS    (0)  // 16-bit

#define PWM_PCFLR_CFLR6_POS    (16) // 16-bit
#define PWM_PCFLR_CFLR2_POS    (0)  // 16-bit

#define PWM_PCFLR_CFLR7_POS    (16) // 16-bit
#define PWM_PCFLR_CFLR3_POS    (0)  // 16-bit

/* PWM PPCR */
#define PWM_PPCR_PDDR_POS      (16) // 8-bit
#define PWM_PPCR_PULLUP_EN_POS (8)  // 8-bit
#define PWM_PPCR_PDR_POS       (0)  // 8-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define PWM_DISABLED           (0)
#define PWM_ENABLED            (1)

/* value of PCSR_CSRx */
#define CSR_CLOCK_DIVID_1      (0x4)
#define CSR_CLOCK_DIVID_2      (0x0)
#define CSR_CLOCK_DIVID_4      (0x1)
#define CSR_CLOCK_DIVID_8      (0x2)
#define CSR_CLOCK_DIVID_16     (0x3)

/* value of PCRx_CHxEN */
#define CHEN_DISABLED          (0)
#define CHEN_ENABLED           (1)

/* value of PCRx_CHxMOD */
#define CHMOD_ONE_SHOT         (0)
#define CHMOD_AUTO_LOAD        (1)

/* value of PCRx_CHxINV */
#define CHINV_OFF              (0)
#define CHINV_ON               (1)

/* value of PCCRx_CAPCHxEN */
#define CAPCHEN_DISABLED       (0)
#define CAPCHEN_ENABLED        (1)

/* value of PPCR_PDDR */
#define PDDR_INPUT             (0)
#define PDDR_OUTPUT            (1)

/* value of PPCR_PULLUP_EN */
#define PULLUP_EN_DISABLED     (0)
#define PULLUP_EN_ENABLED      (1)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    ePwmMode_Capture = 0,
    ePwmMode_Compare,
} PwmMode_e;

typedef enum
{
    ePwmPolarity_Rising = 0,
    ePwmPolarity_Falling,
    ePwmPolarity_Both,
} PwmPolarity_e;

typedef enum
{
    ePwmChannel_0 = 0,
    ePwmChannel_1,
    ePwmChannel_2,
    ePwmChannel_3,
    ePwmChannel_4,
    ePwmChannel_5,
    ePwmChannel_6,
    ePwmChannel_7,

    ePwmChannel_Max,
} PwmChannel_e;

// Note:
//  1. for pwm out, the duty is configured as a percentage
//     but the dead time should be configured with actual ticks
//  2. for pwm out, complement mode is valid only when 'dead_time > 0'

typedef struct
{
    uint32_t status;

    uint32_t clock_freq;

    PwmMode_e mode;

    union
    {
        struct
        {
            PwmPolarity_e polarity;
        } in; // NOTE: only valid when .mode is 'eEPwmMode_Capture'
        struct
        {
            uint32_t freq;
            uint32_t ticks; // NOTE: this is a out data
            uint8_t single_pulse;
            uint16_t duty; // NOTE: one decimal precision
            uint8_t invert;
            uint8_t dead_time; // NOTE: only configed for ch0/ch2/ch4/ch6
            uint8_t pull_up;
        } out; // NOTE: only valid when .mode is 'eEPwmMode_Compare'
    };

    void *user_data;
} DevicePwmConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DevicePwmConfig_t g_DevicePwmDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_PWM_InitConfig(PWM_TypeDef *PWMx, PwmChannel_e chn, DevicePwmConfig_t *config);
int HW_PWM_CompInitConfig(PWM_TypeDef *PWMx, PwmChannel_e chn, DevicePwmConfig_t *config);
int HW_PWM_Start(PWM_TypeDef *PWMx, PwmChannel_e chn);
int HW_PWM_CompStart(PWM_TypeDef *PWMx, PwmChannel_e chn);
int HW_PWM_CaptureStart(PWM_TypeDef *PWMx, PwmChannel_e chn);
int HW_PWM_Stop(PWM_TypeDef *PWMx, PwmChannel_e chn);
int HW_PWM_CompStop(PWM_TypeDef *PWMx, PwmChannel_e chn);
int HW_PWM_CaptureStop(PWM_TypeDef *PWMx, PwmChannel_e chn);
uint32_t HW_PWM_GetCaptureValue(PWM_TypeDef *PWMx, PwmChannel_e chn, uint32_t *fval, uint32_t *rval);
uint32_t HW_PWM_ClearInterrupt(PWM_TypeDef *PWMx, PwmChannel_e chn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_PWM_H__ */
