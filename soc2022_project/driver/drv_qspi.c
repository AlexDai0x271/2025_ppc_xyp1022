/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_qspi.h"
#include "sysclock.h"

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceQspiConfig_t g_DeviceQspiDefaultConfig =
    {
        .quad_mode = eQspiMode_Standard,
        .quad_trans_type = eQspiTransType_11X,
        .quad_inst_len = eQspiInstLen_8,
        .quad_addr_len = eQspiAddrLen_24,
        .quad_wait_cycles = 8,

        .mode = eSpiMode_3,
        .format = eSpiFormat_SPI,
        .data_width = 8,
        .rate = 25 * 1000 * 1000,

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

int HW_QSPI_InitConfig(QSPI_TypeDef *QSPIx, DeviceQspiConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == QSPIx) || (NULL == config)) {
        return -1;
    }

    DISABLE_SPI(QSPIx);

    QSPIx->TXFTLR = 0;
    QSPIx->RXFTLR = 0;

    // mask all inerrupts
    QSPIx->IMR = 0x3f;

    {
        // config CTRLR0 start
        reg_value = 0;

        // Standard SPI or Quad SPI
        if (config->quad_mode == eQspiMode_Standard) {
            reg_value |= (SPI_FRF_STANDARD << QSPI_CTRLR0_SPI_FRF_POS);
            reg_value |= (TMOD_TX_AND_RX << SPI_CTRLR0_TMOD_POS);
        } else {
            reg_value |= (SPI_FRF_QUAD << QSPI_CTRLR0_SPI_FRF_POS);
            reg_value |= (config->quad_mode << QSPI_CTRLR0_TMOD_POS);
        }

        if (config->data_width <= 8) {
            reg_value |= (DFS_08_BIT << QSPI_CTRLR0_DFS_POS);
        } else {
            reg_value |= (DFS_16_BIT << QSPI_CTRLR0_DFS_POS);
        }

        // Only support Motlrola Quad SPI
        reg_value |= (FRF_SPI << QSPI_CTRLR0_FRF_POS);

        switch (config->mode) {
        case eSpiMode_0:
            SET_BIT(reg_value, QSPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_LOW);
            SET_BIT(reg_value, QSPI_CTRLR0_SCPH_POS, SCPH_MIDDLE_BIT);
            break;
        case eSpiMode_1:
            SET_BIT(reg_value, QSPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_LOW);
            SET_BIT(reg_value, QSPI_CTRLR0_SCPH_POS, SCPH_START_BIT);
            break;
        case eSpiMode_2:
            SET_BIT(reg_value, QSPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_HIGH);
            SET_BIT(reg_value, QSPI_CTRLR0_SCPH_POS, SCPH_MIDDLE_BIT);
            break;
        case eSpiMode_3:
        default:
            SET_BIT(reg_value, QSPI_CTRLR0_SCPOL_POS, SCPOL_INACTIVE_HIGH);
            SET_BIT(reg_value, QSPI_CTRLR0_SCPH_POS, SCPH_START_BIT);
            break;
        }

        QSPIx->CTRLR0 = reg_value;
    } // config CTRLR0 end

    {
        // config SPI_CTRLR0 start
        reg_value = 0;

        reg_value |= (config->quad_trans_type << QSPI_SPI_CTRLR0_TRANS_TYPE_POS);
        if (config->quad_addr_len) {
            reg_value |= (ADDR_L32 << QSPI_SPI_CTRLR0_ADDR_L_POS);
        } else {
            reg_value |= (ADDR_L24 << QSPI_SPI_CTRLR0_ADDR_L_POS);
        }

        reg_value |= (config->quad_inst_len << QSPI_SPI_CTRLR0_INST_L_POS);
        reg_value |= (config->quad_wait_cycles << QSPI_SPI_CTRLR0_WAIT_CYCLES_POS);

        QSPIx->SPI_CTRLR0 = reg_value;
    } // config SPI_CTRLR0 end

    reg_value = drv_get_ahb_freq() / config->rate;
    reg_value = reg_value + 1;

    QSPIx->BAUDR = reg_value;

    config->status = 0;
    return 0;
}

static int _Qspi_Switch(QSPI_TypeDef *QSPIx, QspiMode_e quad_mode)
{
    UINT32 reg_value;

    {
        // config CTRLR0 start
        reg_value = QSPIx->CTRLR0;

        reg_value &= ~(0x3 << QSPI_CTRLR0_SPI_FRF_POS);
        reg_value &= ~(0x3 << QSPI_CTRLR0_TMOD_POS);

        // Standard SPI or Quad SPI
        if (quad_mode == eQspiMode_Standard) {
            reg_value |= (SPI_FRF_STANDARD << QSPI_CTRLR0_SPI_FRF_POS);
            reg_value |= (TMOD_TX_AND_RX << SPI_CTRLR0_TMOD_POS);
        } else {
            reg_value |= (SPI_FRF_QUAD << QSPI_CTRLR0_SPI_FRF_POS);
            reg_value |= (quad_mode << QSPI_CTRLR0_TMOD_POS);
        }

        QSPIx->CTRLR0 = reg_value;
    } // config CTRLR0 end

    return 0;
}

static int _Qspi_Transfer(QSPI_TypeDef *QSPIx, QspiMode_e quad_mode, uint8_t cmd, uint32_t addr, uint32_t addr_len, uint8_t *txbuf, uint8_t *rxbuf, uint32_t data_len)
{
    DISABLE_SPI(QSPIx);

    if (quad_mode == eQspiMode_Standard) {
        QSPIx->CTRLR1 = 0;
    } else {
        QSPIx->CTRLR1 = (data_len > 0) ? (data_len - 1) : 0;
    }

    ENABLE_SPI(QSPIx);

    QSPIx->DR[0] = cmd;

    if (quad_mode == eQspiMode_Standard) {
        if (addr_len != 0) {
            if (addr_len > 3) {
                QSPIx->DR[0] = (addr >> 24) & 0xff;
            }

            QSPIx->DR[1] = (addr >> 16) & 0xff;
            QSPIx->DR[2] = (addr >> 8) & 0xff;
            QSPIx->DR[3] = (addr >> 0) & 0xff;
        }

        for (uint32_t index = 0; index < data_len; index++) {
            QSPIx->DR[0] = txbuf ? txbuf[index] : 0xff;
        }
    } else {
        if (addr_len != 0) {
            QSPIx->DR[0] = addr;
        }

        if (txbuf) {
            for (uint32_t index = 0; index < data_len; index++) {
                QSPIx->DR[0] = txbuf[index];
            }
        }
    }

    SELECT_TARGET(QSPIx, 0);
    while (GET_BIT(QSPIx->SR, SPI_SR_BUSY_POS))
        ;
    UNSELECT_TARGRT(QSPIx, 0);

    if (rxbuf) {
        if (quad_mode == eQspiMode_Standard) {
            for (uint32_t index = 0; index < (1 + addr_len); index++) {
                UNUSED(QSPIx->DR[0]);
            }
        }

        for (uint32_t index = 0; index < data_len; index++) {
            volatile uint32_t dummy = QSPIx->DR[0];
            rxbuf[index] = dummy;
        }
    }

    DISABLE_SPI(QSPIx);

    return 0;
}

static int _Qspi_TransferBlock(QSPI_TypeDef *QSPIx, QspiMode_e quad_mode, uint8_t cmd, uint32_t *paddr, uint32_t addr_len, uint8_t *txbuf, uint8_t *rxbuf, uint32_t data_len)
{
    int result = 0;
    uint32_t addr_local = 0x0;

    if (paddr) {
        addr_local = *paddr;
    }

    if (data_len > QSPI_FIFO_MAX) {
        uint32_t block_num = data_len / QSPI_FIFO_MAX;
        uint32_t remain = data_len % QSPI_FIFO_MAX;

        for (uint32_t index = 0; index < block_num; index++) {
            result = _Qspi_Transfer(QSPIx, quad_mode, cmd, addr_local, addr_len, (txbuf ? &txbuf[index * QSPI_FIFO_MAX] : NULL), (rxbuf ? &rxbuf[index * QSPI_FIFO_MAX] : NULL), QSPI_FIFO_MAX);

            if (result != 0) {
                break;
            }

            if (paddr) {
                addr_local += QSPI_FIFO_MAX;
            }
        }

        if ((result == 0) && (remain > 0)) {
            result = _Qspi_Transfer(QSPIx, quad_mode, cmd, addr_local, addr_len, (txbuf ? &txbuf[block_num * QSPI_FIFO_MAX] : NULL), (rxbuf ? &rxbuf[block_num * QSPI_FIFO_MAX] : NULL), remain);
        }
    } else {
        result = _Qspi_Transfer(QSPIx, quad_mode, cmd, addr_local, addr_len, txbuf, rxbuf, data_len);
    }

    return result;
}

int HW_QSPI_WriteBlock(QSPI_TypeDef *QSPIx, QspiMode_e quad_mode, uint8_t cmd, uint32_t *paddr, uint32_t addr_len, uint8_t *txbuf, uint8_t *rxbuf, uint32_t data_len)
{
    _Qspi_Switch(QSPIx, quad_mode);
    return _Qspi_TransferBlock(QSPIx, quad_mode, cmd, paddr, addr_len, txbuf, rxbuf, data_len);
}

int HW_QSPI_ReadBlock(QSPI_TypeDef *QSPIx, QspiMode_e quad_mode, uint8_t cmd, uint32_t *paddr, uint32_t addr_len, uint8_t *txbuf, uint8_t *rxbuf, uint32_t data_len)
{
    _Qspi_Switch(QSPIx, quad_mode);
    return _Qspi_TransferBlock(QSPIx, quad_mode, cmd, paddr, addr_len, txbuf, rxbuf, data_len);
}
