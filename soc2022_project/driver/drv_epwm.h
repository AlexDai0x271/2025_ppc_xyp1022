/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_EPWM_H__
#define __DRV_EPWM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* EPWM CR1 */
#define EPWM_CR1_CKD_POS                (8) // 2-bit
#define EPWM_CR1_ARPE_POS               (7) // 1-bit
#define EPWM_CR1_CMS_POS                (5) // 2-bit
#define EPWM_CR1_DIR_POS                (4) // 1-bit
#define EPWM_CR1_OPM_POS                (3) // 1-bit
#define EPWM_CR1_URS_POS                (2) // 1-bit
#define EPWM_CR1_UDIS_POS               (1) // 1-bit
#define EPWM_CR1_CEN_POS                (0) // 1-bit

/* EPWM CR2 */
#define EPWM_CR2_OIS4N_POS              (15) // 1-bit
#define EPWM_CR2_OIS4_POS               (14) // 1-bit
#define EPWM_CR2_OIS3N_POS              (13) // 1-bit
#define EPWM_CR2_OIS3_POS               (12) // 1-bit
#define EPWM_CR2_OIS2N_POS              (11) // 1-bit
#define EPWM_CR2_OIS2_POS               (10) // 1-bit
#define EPWM_CR2_OIS1N_POS              (9)  // 1-bit
#define EPWM_CR2_OIS1_POS               (8)  // 1-bit
#define EPWM_CR2_TI1S_POS               (7)  // 1-bit
#define EPWM_CR2_MMS_POS                (4)  // 3-bit
#define EPWM_CR2_CCDS_POS               (3)  // 1-bit
#define EPWM_CR2_CCUS_POS               (2)  // 1-bit
#define EPWM_CR2_CCPC_POS               (0)  // 1-bit

/* EPWM SMCR */
#define EPWM_EGR_ETP_POS                (15) // 1-bit
#define EPWM_EGR_ECE_POS                (14) // 1-bit
#define EPWM_EGR_ETPS_POS               (12) // 2-bit
#define EPWM_EGR_ETF_POS                (8)  // 4-bit
#define EPWM_EGR_MSM_POS                (7)  // 1-bit
#define EPWM_EGR_TS_POS                 (4)  // 3-bit
#define EPWM_EGR_SMS_POS                (0)  // 3-bit

/* EPWM DIER */
#define EPWM_DIER_TDE_POS               (14) // 1-bit
#define EPWM_DIER_COMDE_POS             (13) // 1-bit
#define EPWM_DIER_CC4DE_POS             (12) // 1-bit
#define EPWM_DIER_CC3DE_POS             (11) // 1-bit
#define EPWM_DIER_CC2DE_POS             (10) // 1-bit
#define EPWM_DIER_CC1DE_POS             (9)  // 1-bit
#define EPWM_DIER_UDE_POS               (8)  // 1-bit
#define EPWM_DIER_BIE_POS               (7)  // 1-bit
#define EPWM_DIER_TIE_POS               (6)  // 1-bit
#define EPWM_DIER_COMIE_POS             (5)  // 1-bit
#define EPWM_DIER_CC4IE_POS             (4)  // 1-bit
#define EPWM_DIER_CC3IE_POS             (3)  // 1-bit
#define EPWM_DIER_CC2IE_POS             (2)  // 1-bit
#define EPWM_DIER_CC1IE_POS             (1)  // 1-bit
#define EPWM_DIER_UIE_POS               (0)  // 1-bit

/* EPWM SR */
#define EPWM_SR_CC4OF_POS               (12) // 1-bit
#define EPWM_SR_CC3OF_POS               (11) // 1-bit
#define EPWM_SR_CC2OF_POS               (10) // 1-bit
#define EPWM_SR_CC1OF_POS               (9)  // 1-bit
#define EPWM_SR_BIF_POS                 (7)  // 1-bit
#define EPWM_SR_TIF_POS                 (6)  // 1-bit
#define EPWM_SR_COMIF_POS               (5)  // 1-bit
#define EPWM_SR_CC4IF_POS               (4)  // 1-bit
#define EPWM_SR_CC3IF_POS               (3)  // 1-bit
#define EPWM_SR_CC2IF_POS               (2)  // 1-bit
#define EPWM_SR_CC1IF_POS               (1)  // 1-bit
#define EPWM_SR_UIF_POS                 (0)  // 1-bit

/* EPWM EGR */
#define EPWM_EGR_BG_POS                 (7) // 1-bit
#define EPWM_EGR_TG_POS                 (6) // 1-bit
#define EPWM_EGR_COMG_POS               (5) // 1-bit
#define EPWM_EGR_CC4G_POS               (4) // 1-bit
#define EPWM_EGR_CC3G_POS               (3) // 1-bit
#define EPWM_EGR_CC2G_POS               (2) // 1-bit
#define EPWM_EGR_CC1G_POS               (1) // 1-bit
#define EPWM_EGR_UG_POS                 (0) // 1-bit

/* EPWM CCMRx */
#define EPWM_CCMR1_OC2CE_POS            (15) // 1-bit
#define EPWM_CCMR1_OC2M_POS             (12) // 3-bit
#define EPWM_CCMR1_OC2PE_POS            (11) // 1-bit
#define EPWM_CCMR1_OC2FE_POS            (10) // 1-bit
#define EPWM_CCMR1_CC2S_POS             (8)  // 2-bit
#define EPWM_CCMR1_OC1CE_POS            (7)  // 1-bit
#define EPWM_CCMR1_OC1M_POS             (4)  // 3-bit
#define EPWM_CCMR1_OC1PE_POS            (3)  // 1-bit
#define EPWM_CCMR1_OC1FE_POS            (2)  // 1-bit
#define EPWM_CCMR1_CC1S_POS             (0)  // 2-bit

#define EPWM_CCMR1_OC4CE_POS            (15) // 1-bit
#define EPWM_CCMR1_OC4M_POS             (12) // 3-bit
#define EPWM_CCMR1_OC4PE_POS            (11) // 1-bit
#define EPWM_CCMR1_OC4FE_POS            (10) // 1-bit
#define EPWM_CCMR1_CC4S_POS             (8)  // 2-bit
#define EPWM_CCMR1_OC3CE_POS            (7)  // 1-bit
#define EPWM_CCMR1_OC3M_POS             (4)  // 3-bit
#define EPWM_CCMR1_OC3PE_POS            (3)  // 1-bit
#define EPWM_CCMR1_OC3FE_POS            (2)  // 1-bit
#define EPWM_CCMR1_CC3S_POS             (0)  // 2-bit

#define EPWM_CCMR1_IC2F_POS             (12) // 4-bit
#define EPWM_CCMR1_IC2PSC_POS           (10) // 2-bit
#define EPWM_CCMR1_IC1F_POS             (4)  // 4-bit
#define EPWM_CCMR1_IC1PSC_POS           (2)  // 2-bit

#define EPWM_CCMR1_IC4F_POS             (12) // 4-bit
#define EPWM_CCMR1_IC4PSC_POS           (10) // 2-bit
#define EPWM_CCMR1_IC3F_POS             (4)  // 4-bit
#define EPWM_CCMR1_IC3PSC_POS           (2)  // 2-bit

/* EPWM CCER */
#define EPWM_CCER_CC4NP_POS             (15) // 1-bit
#define EPWM_CCER_CC4NE_POS             (14) // 1-bit
#define EPWM_CCER_CC4P_POS              (13) // 1-bit
#define EPWM_CCER_CC4E_POS              (12) // 1-bit
#define EPWM_CCER_CC3NP_POS             (11) // 1-bit
#define EPWM_CCER_CC3NE_POS             (10) // 1-bit
#define EPWM_CCER_CC3P_POS              (9)  // 1-bit
#define EPWM_CCER_CC3E_POS              (8)  // 1-bit
#define EPWM_CCER_CC2NP_POS             (7)  // 1-bit
#define EPWM_CCER_CC2NE_POS             (6)  // 1-bit
#define EPWM_CCER_CC2P_POS              (5)  // 1-bit
#define EPWM_CCER_CC2E_POS              (4)  // 1-bit
#define EPWM_CCER_CC1NP_POS             (3)  // 1-bit
#define EPWM_CCER_CC1NE_POS             (2)  // 1-bit
#define EPWM_CCER_CC1P_POS              (1)  // 1-bit
#define EPWM_CCER_CC1E_POS              (0)  // 1-bit

/* EPWM BDTR */
#define EPWM_BDTR_MOE_POS               (15) // 1-bit
#define EPWM_BDTR_AOE_POS               (14) // 1-bit
#define EPWM_BDTR_BKP_POS               (13) // 1-bit
#define EPWM_BDTR_BKE_POS               (12) // 1-bit
#define EPWM_BDTR_OSSR_POS              (11) // 1-bit
#define EPWM_BDTR_OSSI_POS              (10) // 1-bit
#define EPWM_BDTR_LOCK_POS              (8)  // 2-bit
#define EPWM_BDTR_DTG_POS               (0)  // 8-bit

/* EPWM PMCRx */
#define EPWM_PMCR_PMDV1_POS             (16) // 16-bit
#define EPWM_PMCR_PMCR1_POS             (0)  // 16-bit

#define EPWM_PMCR_PMDV2_POS             (16) // 16-bit
#define EPWM_PMCR_PMCR2_POS             (0)  // 16-bit

#define EPWM_PMCR_PMDV3_POS             (16) // 16-bit
#define EPWM_PMCR_PMCR3_POS             (0)  // 16-bit

#define EPWM_PMCR_PMDV4_POS             (16) // 16-bit
#define EPWM_PMCR_PMCR4_POS             (0)  // 16-bit

/* EPWM PMCREN */
#define EPWM_PMCREN_PMCR4N_DELAY_EN_POS (11) // 1-bit
#define EPWM_PMCREN_PMCR4_DELAY_EN_POS  (10) // 1-bit
#define EPWM_PMCREN_PMCR4_EN_POS        (9)  // 1-bit
#define EPWM_PMCREN_PMCR3N_DELAY_EN_POS (8)  // 1-bit
#define EPWM_PMCREN_PMCR3_DELAY_EN_POS  (7)  // 1-bit
#define EPWM_PMCREN_PMCR3_EN_POS        (6)  // 1-bit
#define EPWM_PMCREN_PMCR2N_DELAY_EN_POS (5)  // 1-bit
#define EPWM_PMCREN_PMCR2_DELAY_EN_POS  (4)  // 1-bit
#define EPWM_PMCREN_PMCR2_EN_POS        (3)  // 1-bit
#define EPWM_PMCREN_PMCR1N_DELAY_EN_POS (2)  // 1-bit
#define EPWM_PMCREN_PMCR1_DELAY_EN_POS  (1)  // 1-bit
#define EPWM_PMCREN_PMCR1_EN_POS        (0)  // 1-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define EPWM_DISABLED                   (0)
#define EPWM_ENABLED                    (1)

/* value of CR1_CMS */
#define CMS_ALIGN_EDGE                  (0x0)
#define CMS_ALIGN_CENTER1               (0x1)
#define CMS_ALIGN_CENTER2               (0x2)
#define CMS_ALIGN_CENTER3               (0x3)

/* value of CR1_CKD */
#define CKD_CLOCK_DIV1                  (0x0)
#define CKD_CLOCK_DIV2                  (0x1)
#define CKD_CLOCK_DIV4                  (0x2)

/* value of CR1_URS */
#define URS_ANY                         (0)
#define URS_ONLY                        (1)

/* value of CR1_UDIS */
#define UDIS_ENABLED                    (0)
#define UDIS_DISABLED                   (1)

/* value of CR1_DIR */
#define DIR_UPCOUNTER                   (0)
#define DIR_DOWNCOUNTER                 (1)

/* value of CR2_TI1S */
#define TI1S_CH1                        (0)
#define TI1S_CH234XOR                   (1)

/* value of CR2_MMS */
#define MMS_UG                          (0x0)
#define MMS_CNT_EN                      (0x1)
#define MMS_UEV                         (0x2)
#define MMS_CCIF                        (0x3)
#define MMS_OC1REF                      (0x4)
#define MMS_OC2REF                      (0x5)
#define MMS_OC3REF                      (0x6)
#define MMS_OC4REF                      (0x7)

/* value of CR2_TI1S */
#define TI1S_CH1                        (0)
#define TI1S_CH234XOR                   (1)

/* value of CR2_CCDS */
#define CCDS_CCXEV                      (0)
#define CCDS_UEV                        (1)

/* value of CR2_CCUS */
#define CCUS_COMG                       (0)
#define CCUS_COMG_OR_TRGI               (1)

/* value of CMMRx_OCxCE */
#define OCCE_IGNORED                    (0)
#define OCCE_ENABLED                    (1)

// Note:
//  1. CCM_PWM_MODE1
//     a. up-count:   Active to Inactive
//     b. down-count: Inactive to Active
//  2. CCM_PWM_MODE2
//     a. up-count:   Inactive to Active
//     b. down-count: Active to Inactive

/* value of CMMRx_OCxM */
#define OCM_TIMER_ONLY                  (0x0)
#define OCM_MATCH_ACTIVE                (0x1)
#define OCM_MATCH_INACTIVE              (0x2)
#define OCM_MATCH_TOOGLE                (0x3)
#define OCM_FORCE_INACTIVE              (0x4)
#define OCM_FORCE_ACTIVE                (0x5)
#define OCM_PWM_MODE1                   (0x6)
#define OCM_PWM_MODE2                   (0x7)

/* value of CMMRx_ICxF */
#define ICF_DTS_N1                      (0x0)
#define ICF_CK_N2                       (0x1)
#define ICF_CK_N4                       (0x2)
#define ICF_CK_N8                       (0x3)
#define ICF_DTS_DIV2_N6                 (0x4)
#define ICF_DTS_DIV2_N8                 (0x5)
#define ICF_DTS_DIV4_N6                 (0x6)
#define ICF_DTS_DIV4_N8                 (0x7)
#define ICF_DTS_DIV8_N6                 (0x8)
#define ICF_DTS_DIV8_N8                 (0x9)
#define ICF_DTS_DIV16_N5                (0xa)
#define ICF_DTS_DIV16_N6                (0xb)
#define ICF_DTS_DIV16_N8                (0xc)
#define ICF_DTS_DIV32_N5                (0xd)
#define ICF_DTS_DIV32_N6                (0xe)
#define ICF_DTS_DIV32_N8                (0xf)

/* value of CMMRx_ICxPSC */
#define ICPSC_1_EVENT                   (0x0)
#define ICPSC_2_EVENT                   (0x1)
#define ICPSC_4_EVENT                   (0x2)
#define ICPSC_8_EVENT                   (0x3)

/* value of CMMRx_CCxS */
#define CCS_OUTPUT                      (0x0)
#define CCS_INPUT_TI1                   (0x1)
#define CCS_INPUT_TI2                   (0x2)
#define CCS_INPUT_TS                    (0x3)

/* value of BDTR_LOCK */
#define LOCK_OFF                        (0x0)
#define LOCK_LEVEL1                     (0x1)
#define LOCK_LEVEL2                     (0x2)
#define LOCK_LEVEL3                     (0x3)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eEPwmMode_Capture = 0,
    eEPwmMode_Compare,
} EPwmMode_e;

typedef enum
{
    eEPwmPolarity_Rising = 0,
    eEPwmPolarity_Falling,
    eEPwmPolarity_Reserved,
    eEPwmPolarity_Both,
} EPwmPolarity_e;

typedef enum
{
    eEPwmChannel_0 = 0,
    eEPwmChannel_1,
    eEPwmChannel_2,
    eEPwmChannel_3,

    eEPwmChannel_Max,
} EPwmChannel_e;

// Note:
//  1. for pwm out, the duty is configured as a percentage
//     and the dead time is configured as a percentage too

typedef struct
{
    uint32_t status;

    uint32_t clock_freq;

    EPwmMode_e mode;

    union
    {
        struct
        {
            EPwmPolarity_e polarity[eEPwmChannel_Max];
        } in; // NOTE: only valid when .mode is 'eEPwmMode_Capture'
        struct
        {
            uint32_t freq;
            uint8_t single_pulse;
            uint8_t break_enable;
            uint8_t break_polarity;
            uint8_t dead_time; // NOTE: how many 1-ns

            struct
            {
                uint16_t duty; // NOTE: one decimal precision, e.g. '.duty = 234' is 23.4%
                uint8_t oc_active;
                uint8_t ocn_active;
                uint16_t delay_rate; // NOTE: one decimal precision
                uint8_t pull_up;
            } param[eEPwmChannel_Max];
        } out; // NOTE: only valid when .mode is 'eEPwmMode_Compare'
    };

    void *user_data;
} DeviceEPwmConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceEPwmConfig_t g_DeviceEPwmDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_EPWM_InitConfig(EPWM_TypeDef *EPWMx, DeviceEPwmConfig_t *config);
uint32_t HW_EPWM_GetDeadTimeIntervalStep(EPWM_TypeDef *EPWMx);
uint32_t HW_EPWM_Lock(EPWM_TypeDef *EPWMx);
uint32_t HW_EPWM_Unlock(EPWM_TypeDef *EPWMx);
int HW_EPWM_Start(EPWM_TypeDef *EPWMx, EPwmChannel_e chn);
int HW_EPWM_AllStart(EPWM_TypeDef *EPWMx);
int HW_EPWM_CaptureStart(EPWM_TypeDef *EPWMx, EPwmChannel_e chn);
int HW_EPWM_Stop(EPWM_TypeDef *EPWMx, EPwmChannel_e chn);
int HW_EPWM_AllStop(EPWM_TypeDef *EPWMx);
int HW_EPWM_CaptureStop(EPWM_TypeDef *EPWMx, EPwmChannel_e chn);
uint32_t HW_EPWM_GetCaptureValue(EPWM_TypeDef *EPWMx, EPwmChannel_e chn);
uint32_t HW_EPWM_ClearInterrupt(EPWM_TypeDef *EPWMx, EPwmChannel_e chn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_EPWM_H__ */
