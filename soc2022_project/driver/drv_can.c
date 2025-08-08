/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_can.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceCanConfig_t g_DeviceCanDefaultConfig =
    {
#ifdef DEBUG
        .mode = eCanMode_Normal,
#else
        .mode = eCanMode_Normal,
#endif
        .format = eCanFormat_Standard,
        .filter = eCanFilter_Single,

        .rate = 500 * 1000,

        .tseg1 = 10,
        .tseg2 = 7,

        .id[0] = 0x00000001,
        .id[1] = CAN_INVALID_ID,

        .mask[0] = 0xffffff00,
        .mask[1] = CAN_INVALID_MASK,

        .user_data = 0,
};
#endif // #ifdef DEBUG

int HW_CAN_InitConfig(CAN_TypeDef *CANx, DeviceCanConfig_t *config)
{
    uint32_t can_div;
    uint32_t reg_value;

    if ((NULL == CANx) || (NULL == config)) {
        return -1;
    }

    // check config data
    if ((eCanFormat_Standard == config->format) && (eCanFilter_Single == config->filter)) {
        config->mask[0] |= 0x001fffff;
        config->mask[1] |= 0x001fffff;
    }
    if ((eCanFormat_Standard == config->format) && (eCanFilter_Dual == config->filter)) {
        config->mask[0] |= 0x001f;
        config->mask[1] |= 0x001f;
    }
    if ((eCanFormat_Extend == config->format) && (eCanFilter_Single == config->filter)) {
        config->mask[0] |= 0x00000007;
        config->mask[1] |= 0x00000007;
    }

    // config to reset mode first
    if (eCanMode_SelfTest == config->mode) {
        CANx->MOD = BIT(CAN_MOD_STM_POS) | ((eCanFilter_Single == config->filter) ? BIT(CAN_MOD_AFM_POS) : 0) | BIT(CAN_MOD_RM_POS);
    } else {
        CANx->MOD = ((eCanFilter_Single == config->filter) ? BIT(CAN_MOD_AFM_POS) : 0) | BIT(CAN_MOD_RM_POS);
    }

    can_div = drv_get_apb_freq() / (1 + config->tseg1 + config->tseg2) / config->rate / 2;
    can_div = (can_div > 0) ? (can_div - 1) : 0;

    reg_value = 0; // .BTR0 inited to zero first
    reg_value |= (0 << CAN_BTR0_SJW_POS);
    reg_value |= (can_div << CAN_BTR0_BRP_POS);
    CANx->BTR0 = reg_value;

    reg_value = 0;                                              // .BTR1 inited to zero first
    SET_BIT(reg_value, CAN_BTR1_SAM_POS, SAM_ONCE);             // NOTE: only for low/medium speed buses
    reg_value |= ((config->tseg2 & 0x7) << CAN_BTR1_TSEG2_POS); // NOTE: NxTQ for '2nd Phase Buffer', up to 8xTQ
    reg_value |= ((config->tseg1 & 0xf) << CAN_BTR1_TSEG1_POS); // NOTE: MxTQ for 'Propagation Segment' and '1st Phase Buffer', up to 16xTQ
    CANx->BTR1 = reg_value;

    reg_value = 0; // .CDR inited to zero first
    SET_BIT(reg_value, CAN_CDR_CLOCKOFF_POS, CAN_ENABLED);
    reg_value |= (CDR_OSC_DIV8 << CAN_CDR_CDR_POS);
    CANx->CDR = reg_value;

    reg_value = 0; // .OCR inited to zero first
    SET_BIT(reg_value, CAN_OCR_OCMOD1_POS, 1);
    SET_BIT(reg_value, CAN_OCR_OCMOD0_POS, OCMOD0_NORMAL_OUTPUT);
    CANx->OCR = reg_value;

    if (eCanFilter_Single == config->filter) {
        DEBUG_ASSERT(config->id[0] != CAN_INVALID_ID);

        CANx->ACR[0] = (config->id[0] >> 24) & 0xff;
        CANx->ACR[1] = (config->id[0] >> 16) & 0xff;
        CANx->ACR[2] = (config->id[0] >> 8) & 0xff;
        CANx->ACR[3] = (config->id[0] >> 0) & 0xff;

        CANx->AMR[0] = (config->mask[0] >> 24) & 0xff;
        CANx->AMR[1] = (config->mask[0] >> 16) & 0xff;
        CANx->AMR[2] = (config->mask[0] >> 8) & 0xff;
        CANx->AMR[3] = (config->mask[0] >> 0) & 0xff;
    }
    if (eCanFilter_Dual == config->filter) {
        DEBUG_ASSERT(config->id[0] != CAN_INVALID_ID);
        DEBUG_ASSERT(config->id[1] != CAN_INVALID_ID);

        CANx->ACR[0] = (config->id[0] >> 8) & 0xff;
        CANx->ACR[1] = (config->id[0] >> 0) & 0xff;
        CANx->ACR[2] = (config->id[1] >> 8) & 0xff;
        CANx->ACR[3] = (config->id[1] >> 0) & 0xff;

        CANx->AMR[0] = (config->mask[0] >> 8) & 0xff;
        CANx->AMR[1] = (config->mask[0] >> 0) & 0xff;
        CANx->AMR[2] = (config->mask[1] >> 8) & 0xff;
        CANx->AMR[3] = (config->mask[1] >> 0) & 0xff;
    }

    CANx->IER = 0xf;

    SET_BIT(CANx->MOD, CAN_MOD_RM_POS, CAN_DISABLED);

    config->status = 0;
    return 0;
}

uint32_t HW_CAN_WriteBlock(CAN_TypeDef *CANx, CanFormat_e format, uint32_t id, uint8_t *txbuf, uint32_t length)
{
    uint32_t dummy_data;

    SET_BIT(CANx->MOD, CAN_MOD_RM_POS, CAN_DISABLED);

    while (!GET_BIT(CANx->SR, CAN_SR_TBS_POS))
        ;

    length = MIN(length, 8);

    dummy_data = (format << CAN_TDI_FF_POS) | (RTR_DATA << CAN_TDI_RTR_POS) | (length << CAN_TDI_DLC_POS);
    CANx->TFI = dummy_data;

    uint8_t offset;
    offset = 0;

    if (format == eCanFormat_Standard) {
        id <<= 5;
        CANx->TDI[offset++] = ((id >> 8) & 0xff);
        CANx->TDI[offset++] = ((id)&0xff);
    } else {
        id <<= 3;
        CANx->TDI[offset++] = ((id >> 24) & 0xff);
        CANx->TDI[offset++] = ((id >> 16) & 0xff);
        CANx->TDI[offset++] = ((id >> 8) & 0xff);
        CANx->TDI[offset++] = ((id)&0xff);
    }

    for (uint32_t i = 0; i < length; i++) {
        CANx->TDI[offset++] = txbuf[i];
    }

    CANx->CMR = BIT(CAN_CMR_SRR_POS);

    return length;
}

uint32_t HW_CAN_ReadBlock(CAN_TypeDef *CANx, CanFormat_e *format, uint32_t *id, uint8_t *rxbuf, uint32_t length)
{
    uint32_t dummy_data;

    SET_BIT(CANx->MOD, CAN_MOD_RM_POS, CAN_DISABLED);

    while (!GET_BIT(CANx->SR, CAN_SR_RBS_POS))
        ;

    dummy_data = CANx->TFI;
    length = MIN(length, (dummy_data & 0xf));

    uint8_t offset;
    offset = 0;

    if (!GET_BIT(dummy_data, CAN_TDI_FF_POS)) {
        dummy_data = 0;
        dummy_data |= (CANx->TDI[offset++] << 8);
        dummy_data |= (CANx->TDI[offset++]);
        dummy_data >>= 5;

        if (format) {
            *format = eCanFormat_Standard;
        }
    } else {
        dummy_data = 0;
        dummy_data |= (CANx->TDI[offset++] << 24);
        dummy_data |= (CANx->TDI[offset++] << 16);
        dummy_data |= (CANx->TDI[offset++] << 8);
        dummy_data |= (CANx->TDI[offset++]);
        dummy_data >>= 3;

        if (format) {
            *format = eCanFormat_Extend;
        }
    }

    if (id) {
        *id = dummy_data;
    }

    for (uint32_t i = 0; i < length; i++) {
        rxbuf[i] = CANx->TDI[offset++];
    }

    CANx->CMR = BIT(CAN_CMR_RRB_POS);

    return length;
}

uint32_t HW_CAN_GetStatus(CAN_TypeDef *CANx)
{
    return CANx->SR;
}

uint32_t HW_CAN_GetErrorCode(CAN_TypeDef *CANx)
{
    return CANx->ECC;
}
