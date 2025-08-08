/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_CAN_H__
#define __DRV_CAN_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

// NOTE: define some special identifiers
#define CAN_INVALID_ID       (0xfffffff4)
#define CAN_INVALID_MASK     (0xffffffff)

/**
 * @brief Register Field Define
 */

/* CAN MOD */
#define CAN_MOD_SM_POS       (4) // 1-bit
#define CAN_MOD_AFM_POS      (3) // 1-bit
#define CAN_MOD_STM_POS      (2) // 1-bit
#define CAN_MOD_LOM_POS      (1) // 1-bit
#define CAN_MOD_RM_POS       (0) // 1-bit

/* CAN CMR */
#define CAN_CMR_SRR_POS      (4) // 1-bit
#define CAN_CMR_CDO_POS      (3) // 1-bit
#define CAN_CMR_RRB_POS      (2) // 1-bit
#define CAN_CMR_AT_POS       (1) // 1-bit
#define CAN_CMR_TR_POS       (0) // 1-bit

/* CAN SR */
#define CAN_SR_BS_POS        (7) // 8-bit
#define CAN_SR_ES_POS        (6) // 8-bit
#define CAN_SR_TS_POS        (5) // 8-bit
#define CAN_SR_RS_POS        (4) // 8-bit
#define CAN_SR_TCS_POS       (3) // 8-bit
#define CAN_SR_TBS_POS       (2) // 8-bit
#define CAN_SR_DOS_POS       (1) // 8-bit
#define CAN_SR_RBS_POS       (0) // 8-bit

/* CAN IR */
#define CAN_IR_BEI_POS       (7) // 8-bit
#define CAN_IR_ALI_POS       (6) // 8-bit
#define CAN_IR_EPI_POS       (5) // 8-bit
#define CAN_IR_WUI_POS       (4) // 8-bit
#define CAN_IR_DOI_POS       (3) // 8-bit
#define CAN_IR_EI_POS        (2) // 8-bit
#define CAN_IR_TI_POS        (1) // 8-bit
#define CAN_IR_RI_POS        (0) // 8-bit

/* CAN IER */
#define CAN_IER_BEIE_POS     (7) // 8-bit
#define CAN_IER_ALIE_POS     (6) // 8-bit
#define CAN_IER_EPIE_POS     (5) // 8-bit
#define CAN_IER_WUIE_POS     (4) // 8-bit
#define CAN_IER_DOIE_POS     (3) // 8-bit
#define CAN_IER_EIE_POS      (2) // 8-bit
#define CAN_IER_TIE_POS      (1) // 8-bit
#define CAN_IER_RIE_POS      (0) // 8-bit

/* CAN BTR0 */
#define CAN_BTR0_SJW_POS     (6) // 2-bit
#define CAN_BTR0_BRP_POS     (0) // 6-bit

/* CAN BTR1 */
#define CAN_BTR1_SAM_POS     (7) // 1-bit
#define CAN_BTR1_TSEG2_POS   (4) // 3-bit
#define CAN_BTR1_TSEG1_POS   (0) // 4-bit

/* CAN OCR */
#define CAN_OCR_OCMOD1_POS   (1) // 1-bit
#define CAN_OCR_OCMOD0_POS   (0) // 1-bit

/* CAN ECC */
#define CAN_ECC_ERRCODE_POS  (6) // 2-bit
#define CAN_ECC_DIR_POS      (5) // 1-bit
#define CAN_ECC_SEGCODE_POS  (0) // 5-bit

/* CAN TDI */
#define CAN_TDI_FF_POS       (7) // 1-bit
#define CAN_TDI_RTR_POS      (6) // 1-bit
#define CAN_TDI_DLC_POS      (0) // 4-bit

/* CAN CDR */
#define CAN_CDR_CLOCKOFF_POS (3) // 1-bit
#define CAN_CDR_CDR_POS      (0) // 3-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define CAN_DISABLED         (0)
#define CAN_ENABLED          (1)

/* value of BTR1_SAM */
#define SAM_ONCE             (0)
#define SAM_TRIPLE           (1)

/* value of OCR_OCMOD0 */
#define OCMOD0_NORMAL_OUTPUT (0)
#define OCMOD0_CLOCK_OUTPUT  (1)

/* value of TDI_FF */
#define FF_STANDARD          (0)
#define FF_EXTEND            (1)

/* value of TDI_RTR */
#define RTR_DATA             (0)
#define RTR_REMOTE           (1)

/* value of CDR_CDR */
#define CDR_OSC_DIV2         (0x0)
#define CDR_OSC_DIV4         (0x1)
#define CDR_OSC_DIV6         (0x2)
#define CDR_OSC_DIV8         (0x3)
#define CDR_OSC_DIV10        (0x4)
#define CDR_OSC_DIV12        (0x5)
#define CDR_OSC_DIV14        (0x6)
#define CDR_OSC              (0x7)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eCanMode_Normal = 0,
    eCanMode_SelfTest,
} CanMode_e;

typedef enum
{
    eCanFormat_Standard = 0,
    eCanFormat_Extend = 1,
} CanFormat_e;

typedef enum
{
    eCanFilter_Single = 0,
    eCanFilter_Dual = 1,
} CanFilter_e;

// Note:
//  1. if 'filter == eCanFilter_Single', only '.id[0]' and '.mask[0]' is valid
//  2. for '.mask', bit value '1' identify this bit as ignore

// Note:
//  1. standard frame format, single filter
//     [id.28 - id.21]  [id.20 .... id.18 RTR] xxxx       data.byte.1  data.byte.2
//     8-bit            4-bit                  4-bit      8-bit        8-bit
//     ACR0[7:0]        ACR1[7:4]              ACR1[3:0]  ACR2[7:0]    ACR3[7:0]
//     AMR0[7:0]        AMR1[7:4]              AMR1[3:0]  AMR2[7:0]    AMR3[7:0]
//
//  2. standard frame format, dual filter
//     [id.28 - id.21]  [id.20 .... id.18 RTR] xxxx       data.byte.1  data.byte.2
//     8-bit            4-bit                  4-bit      8-bit        8-bit
//     ACR0[7:0]        ACR1[7:4]                         ACR1[3:0]    ACR3[3:0]
//     AMR0[7:0]        AMR1[7:4]                         AMR1[3:0]    AMR3[3:0]
//     ACR2[7:0]        ACR3[7:4]
//     AMR2[7:0]        AMR3[7:4]
//
//  3. extend frame format, single filter
//     [id.28 - id.21]  [id.20 - id.23]  [id.12 - id.5]   [id.4 .... id.0 RTR] xx
//     8-bit            4-bit            8-bit            6-bit                2-bit
//     ACR0[7:0]        ACR1[7:0]        ACR2[7:0]        ACR3[7:2]            ACR3[1:0]
//     AMR0[7:0]        AMR1[7:0]        AMR2[7:0]        AMR3[7:2]            AMR3[1:0]
//
//  4. extend frame format, dual filter
//     [id.28 - id.21]  [id.20 - id.23]  [id.12 - id.5]   [id.4 .... id.0 RTR] xx
//     8-bit            4-bit            8-bit            6-bit                2-bit
//     ACR0[7:0]        ACR1[7:0]
//     AMR0[7:0]        AMR1[7:0]
//     ACR2[7:0]        ACR3[7:0]
//     AMR2[7:0]        AMR3[7:0]
//
//  5. filter can mask id segment and data segment, it is not recommended to filter the data sewgment
//     the program will auto set this bit-field to '1' to not match this bits
//     so '.mask' will be modified that you should pay attention
//     in addition, mask of 'RTR' field is ignored too

typedef struct
{
    uint32_t status;

    CanMode_e mode;
    CanFormat_e format;
    CanFilter_e filter;

    uint32_t rate;

    struct
    {
        uint8_t tseg1 : 4;
        uint8_t tseg2 : 3;
        uint8_t rsvd  : 1;
    };

    uint32_t id[2];
    uint32_t mask[2];

    void *user_data;
} DeviceCanConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceCanConfig_t g_DeviceCanDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_CAN_InitConfig(CAN_TypeDef *CANx, DeviceCanConfig_t *config);
uint32_t HW_CAN_WriteBlock(CAN_TypeDef *CANx, CanFormat_e format, uint32_t id, uint8_t *txbuf, uint32_t length);
uint32_t HW_CAN_ReadBlock(CAN_TypeDef *CANx, CanFormat_e *format, uint32_t *id, uint8_t *rxbuf, uint32_t length);
uint32_t HW_CAN_GetStatus(CAN_TypeDef *CANx);
uint32_t HW_CAN_GetErrorCode(CAN_TypeDef *CANx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_CAN_H__ */
