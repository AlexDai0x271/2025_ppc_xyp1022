/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_spi.h"
#include "sysclock.h"

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceSpiConfig_t g_DeviceSpiDefaultConfig =
    {
        .mode = eSpiMode_3,
        .format = eSpiFormat_SPI,
        .data_width = 8,
        .rate = 1 * 1000 * 1000,

        .user_data = 0,
};
#endif // #ifdef DEBUG

#define ENABLE_SPI(handle)                                                 \
    do {                                                                   \
        SET_BIT((handle)->SSIENR, SPI_SSIENR_SSIC_EN_POS, SSI_EN_ENABLED); \
    } while (0)
#define DISABLE_SPI(handle)                                                 \
    do {                                                                    \
        SET_BIT((handle)->SSIENR, SPI_SSIENR_SSIC_EN_POS, SSI_EN_DISABLED); \
    } while (0)

#define SELECT_TARGET(handle, target) \
    do {                              \
        (handle)->SER = 0xff;         \
    } while (0)
#define UNSELECT_TARGRT(handle, target) \
    do {                                \
        (handle)->SER = 0x00;           \
    } while (0)

int HW_SPI_InitConfig(SPI_TypeDef *SPIx, DeviceSpiConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == SPIx) || (NULL == config)) {
        return -1;
    }

    DISABLE_SPI(SPIx);

    { // config CTRLR0 start
        reg_value = 0;

        // Standard SPI
        reg_value |= (SPI_FRF_STANDARD << SPI_CTRLR0_SPI_FRF_POS);
        reg_value |= (TMOD_TX_AND_RX << SPI_CTRLR0_TMOD_POS);

        if (config->data_width <= 8) {
            if (config->format == eSpiFormat_MICROWIRE) {
                reg_value |= (CFS_SIZE_08_BIT << SPI_CTRLR0_CFS_POS);
            }
            reg_value |= (DFS_08_BIT << SPI_CTRLR0_DFS_POS);
        } else {
            if (config->format == eSpiFormat_MICROWIRE) {
                reg_value |= (CFS_SIZE_16_BIT << SPI_CTRLR0_CFS_POS);
            }
            reg_value |= (DFS_16_BIT << SPI_CTRLR0_DFS_POS);
        }

        switch (config->format) {
        case eSpiFormat_SPI:
            reg_value |= (FRF_SPI << SPI_CTRLR0_FRF_POS);
            break;
        case eSpiFormat_SSP:
            reg_value |= (FRF_SSP << SPI_CTRLR0_FRF_POS);
            break;
        case eSpiFormat_MICROWIRE:
            reg_value |= (FRF_MICROWIRE << SPI_CTRLR0_FRF_POS);
            break;
        default:
            reg_value |= (FRF_RESERVED << SPI_CTRLR0_FRF_POS);
            break;
        }

        switch (config->mode) {
        case eSpiMode_0:
            SET_BIT(reg_value, SPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_LOW);
            SET_BIT(reg_value, SPI_CTRLR0_SCPH_POS, SCPH_MIDDLE_BIT);
            break;
        case eSpiMode_1:
            SET_BIT(reg_value, SPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_LOW);
            SET_BIT(reg_value, SPI_CTRLR0_SCPH_POS, SCPH_START_BIT);
            break;
        case eSpiMode_2:
            SET_BIT(reg_value, SPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_HIGH);
            SET_BIT(reg_value, SPI_CTRLR0_SCPH_POS, SCPH_MIDDLE_BIT);
            break;
        case eSpiMode_3:
        default:
            SET_BIT(reg_value, SPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_HIGH);
            SET_BIT(reg_value, SPI_CTRLR0_SCPH_POS, SCPH_START_BIT);
            break;
        }

        SPIx->CTRLR0 = reg_value;
    } // config CTRLR0 end

    reg_value = drv_get_apb_freq() / config->rate;
    reg_value = reg_value + 1;

    SPIx->BAUDR = reg_value;

    ENABLE_SPI(SPIx);

    config->status = 0;
    return 0;
}

uint32_t HW_SPI_ReadWriteBlock(SPI_TypeDef *SPIx, uint8_t *txbuf, uint8_t *rxbuf, uint32_t length)
{
    uint8_t dummy_data;

    for (uint32_t i = 0; i < length; i++) {
        SPIx->DR[0] = (txbuf) ? txbuf[i] : 0xff;
    }

    SELECT_TARGET(SPIx, 0);
    while (GET_BIT(SPIx->SR, SPI_SR_BUSY_POS))
        ;
    UNSELECT_TARGRT(SPIx, 0);

    for (uint32_t i = 0; i < length; i++) {
        dummy_data = SPIx->DR[0];
        if (rxbuf) {
            rxbuf[i] = dummy_data;
        } else {
            UNUSED(dummy_data);
        }
    }

    return length;
}
