/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_i2c.h"
#if (DEBUG_I2C_CASE == DEBUG_I2C_DMA)
#include "drv_dma.h"
#endif
#include "sysclock.h"

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceI2cConfig_t g_DeviceI2cDefaultConfig =
    {
        .mode = eI2cMode_Standard,
        .restart = 0,

        .master = 1,
        .master_code = 0x3,
        .addr_bits = 7,
        .addr = 0x01,

        .tx_dma = 0,
        .tx_dma_level = 0x0,
        .rx_dma = 0,
        .rx_dma_level = 0x0,

        .user_data = 0,
};
#endif // #ifdef DEBUG

// Note:
//  1. config as master, i2c is only enabled during 'HW_I2C_WriteBlock' or 'HW_I2C_ReadBlock'
//     and no need to explicitly call 'HW_I2C_InitConfig'
//  2. config as slave, i2c is enabled when 'HW_I2C_InitConfig' is called

int HW_I2C_InitConfig(I2C_TypeDef *I2Cx, DeviceI2cConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == I2Cx) || (NULL == config)) {
        return -1;
    }

    SET_BIT(I2Cx->ENABLE, I2C_ENABLE_ENABLE_POS, I2C_DISABLED);

    for (uint32_t *addr = (uint32_t *)&I2Cx->CLR_INTR; addr <= (uint32_t *)&I2Cx->CLR_GEN_CALL; addr++) {
        reg_value = *addr;
        UNUSED(reg_value);
    }

    { // config CON start
        reg_value = 0;

        SET_BIT(reg_value, I2C_CTRLR0_RX_FIFO_FULL_HLD_CTRL_POS, I2C_DISABLED);

        if (config->addr_bits > 7) {
            SET_BIT(reg_value, I2C_CTRLR0_IC_10BITADDR_MASTER_POS, MASTER_ADDR_10BITS);
            SET_BIT(reg_value, I2C_CTRLR0_IC_10BITADDR_SLAVE_POS, SLAVE_ADDR_10BITS);
        } else {
            SET_BIT(reg_value, I2C_CTRLR0_IC_10BITADDR_MASTER_POS, MASTER_ADDR_7BITS);
            SET_BIT(reg_value, I2C_CTRLR0_IC_10BITADDR_SLAVE_POS, SLAVE_ADDR_7BITS);
        }

        switch (config->mode) {
        case eI2cMode_Standard:
            reg_value |= (SPEED_STANDARD << I2C_CTRLR0_SPEED_POS);
            break;
        case eI2cMode_FastSpeed:
        case eI2cMode_FastSpeed_Plus:
            reg_value |= (SPEED_FAST << I2C_CTRLR0_SPEED_POS);
            break;
        case eI2cMode_HighSpeed:
            reg_value |= (SPEED_HIGH << I2C_CTRLR0_SPEED_POS);
            break;
        default:
            break;
        }

        SET_BIT(reg_value, I2C_CTRLR0_MASTER_MODE_POS, (config->master ? MASTER_MODE_MASTER : MASTER_MODE_SLAVE));
        SET_BIT(reg_value, I2C_CTRLR0_IC_SLAVE_DISABLE_POS, (config->master ? IC_SLAVE_DISABLED : IC_SLAVE_ENABLED));
        SET_BIT(reg_value, I2C_CTRLR0_IC_RESTART_EN_POS, (config->restart ? IC_RESTART_ENABLED : IC_RESTART_DISABLED));

        I2Cx->CON = reg_value;
    } // config CON end

    reg_value = 0; // .TAR inited to zero first
    SET_BIT(reg_value, I2C_TAR_SMBUS_QUICK_CMD_POS, I2C_DISABLED);
    SET_BIT(reg_value, I2C_TAR_DEVICE_ID_POS, I2C_DISABLED);
    SET_BIT(reg_value, I2C_TAR_IC_10BITADDR_MASTER_POS, ((config->addr_bits > 7) ? MASTER_ADDR_10BITS : MASTER_ADDR_7BITS));
    SET_BIT(reg_value, I2C_TAR_SPECIAL_POS, I2C_DISABLED);
    SET_BIT(reg_value, I2C_TAR_GC_OR_START_POS, I2C_DISABLED); // NOTE: valid only when bit.SPECIAL=1 and bit.DEVICE_ID=0
    reg_value |= (config->addr << I2C_TAR_IC_TAR_POS);
    I2Cx->TAR = reg_value;

    I2Cx->SAR = (config->addr << I2C_TAR_IC_TAR_POS);

    if (config->master && (config->mode == eI2cMode_HighSpeed)) {
        I2Cx->HS_MADDR = (config->master_code & 0x7); // NOTE: extend to '0000_1xxx'
    }

    I2Cx->TX_TL = ((1 & 0xff) << I2C_TX_FL_VAL_POS);
    I2Cx->RX_TL = ((1 & 0xff) << I2C_RX_FL_VAL_POS);

    if (config->master) {
        I2Cx->INTR_MASK = 0;
    } else {
        reg_value = 0; // .INTR_MASK inited to zero first
        SET_BIT(reg_value, I2C_INTR_MASK_M_RD_REQ_POS, I2C_ENABLED);
        SET_BIT(reg_value, I2C_INTR_MASK_M_RX_DONE_POS, I2C_ENABLED);
        I2Cx->INTR_MASK = reg_value;

        switch ((uint32_t)I2Cx) {
        case (uint32_t)I2C0:
            drv_irq_enable(I2C0_IRQn);
            break;
        case (uint32_t)I2C1:
            drv_irq_enable(I2C1_IRQn);
            break;
        default:
            break;
        }
    }

    reg_value = 0; // .DMA_CR inited to zero first
    if (config->tx_dma) {
        reg_value |= (I2C_ENABLED << I2C_DMA_CR_TDMAE);
        I2Cx->DMA_TDLR = config->tx_dma_level;
    }
    if (config->rx_dma) {
        reg_value |= (I2C_ENABLED << I2C_DMA_CR_RDMAE);
        I2Cx->DMA_RDLR = config->rx_dma_level;
    }
    I2Cx->DMA_CR = reg_value;

    if (config->master) {
        SET_BIT(I2Cx->ENABLE, I2C_ENABLE_ENABLE_POS, I2C_DISABLED);
    } else {
        SET_BIT(I2Cx->ENABLE, I2C_ENABLE_ENABLE_POS, I2C_ENABLED);
    }

    config->status = 0;
    return 0;
}

static int _wait_status(I2C_TypeDef *I2Cx, uint32_t flag, uint32_t value, int32_t timeout)
{
    do {
        if (GET_BIT(I2Cx->STATUS, flag) == value) {
            return 1;
        }

        timeout--;

        if (timeout > 0) {
            udelay(1);
        }
    } while (timeout > 0);

    return 0;
}

uint32_t HW_I2C_WriteBlock(I2C_TypeDef *I2Cx, DeviceI2cConfig_t *config, uint8_t *pcmd, uint32_t cmd_len, uint8_t *pdata, uint32_t data_len)
{
    uint32_t reg_value;

    if (NULL == config) {
        return 0;
    }

    if (config->master) {
        SET_BIT(I2Cx->ENABLE, I2C_ENABLE_ENABLE_POS, I2C_ENABLED);
    }

    uint32_t tx_cnt = 0;

    while (tx_cnt < (cmd_len + data_len)) {
        reg_value = 0;

        SET_BIT(reg_value, I2C_DATA_CMD_CMD_POS, CMD_WRITE);
        SET_BIT(reg_value, I2C_DATA_CMD_RESTART_POS, I2C_DISABLED);

        if ((tx_cnt + 1) < (cmd_len + data_len)) {
            SET_BIT(reg_value, I2C_DATA_CMD_STOP_POS, I2C_DISABLED);
        } else {
            SET_BIT(reg_value, I2C_DATA_CMD_STOP_POS, I2C_ENABLED);
        }

        if (tx_cnt < cmd_len) {
            reg_value |= (pcmd[tx_cnt] << I2C_DATA_CMD_DAT_POS);
        } else {
            reg_value |= (pdata[tx_cnt - cmd_len] << I2C_DATA_CMD_DAT_POS);
        }

        if (!_wait_status(I2Cx, I2C_STATUS_TFNF_POS, 1, 10 * 1000)) {
            goto __HW_I2C_WriteBlock_Exit;
        }

        I2Cx->DATA_CMD = reg_value;
        tx_cnt++;
    }

    _wait_status(I2Cx, I2C_STATUS_TFE_POS, 1, 10 * 1000);

__HW_I2C_WriteBlock_Exit:

    if (config->master) {
        SET_BIT(I2Cx->ENABLE, I2C_ENABLE_ENABLE_POS, I2C_DISABLED);
    }

    return tx_cnt;
}

uint32_t HW_I2C_ReadBlock(I2C_TypeDef *I2Cx, DeviceI2cConfig_t *config, uint8_t *pcmd, uint32_t cmd_len, uint8_t *pdata, uint32_t data_len)
{
    uint32_t reg_value;

    if (NULL == config) {
        return 0;
    }

    if (config->master) {
        SET_BIT(I2Cx->ENABLE, I2C_ENABLE_ENABLE_POS, I2C_ENABLED);
    }

    uint32_t tx_cnt = 0;
    uint32_t rx_cnt = 0;

    while (tx_cnt < cmd_len) {
        reg_value = 0;

        SET_BIT(reg_value, I2C_DATA_CMD_CMD_POS, CMD_WRITE);
        SET_BIT(reg_value, I2C_DATA_CMD_RESTART_POS, I2C_DISABLED);
        SET_BIT(reg_value, I2C_DATA_CMD_STOP_POS, I2C_DISABLED);

        reg_value |= (pcmd[tx_cnt] << I2C_DATA_CMD_DAT_POS);

        if (!_wait_status(I2Cx, I2C_STATUS_TFNF_POS, 1, 10 * 1000)) {
            goto __HW_I2C_ReadBlock_Exit;
        }

        I2Cx->DATA_CMD = reg_value;
        tx_cnt++;
    }

    while (rx_cnt < data_len) {
        reg_value = 0;

        SET_BIT(reg_value, I2C_DATA_CMD_CMD_POS, CMD_READ);

        if ((tx_cnt > 0) && (rx_cnt == 0)) {
            SET_BIT(reg_value, I2C_DATA_CMD_RESTART_POS, I2C_ENABLED);
        } else {
            SET_BIT(reg_value, I2C_DATA_CMD_RESTART_POS, I2C_DISABLED);
        }

        if ((rx_cnt + 1) < data_len) {
            SET_BIT(reg_value, I2C_DATA_CMD_STOP_POS, I2C_DISABLED);
        } else {
            SET_BIT(reg_value, I2C_DATA_CMD_STOP_POS, I2C_ENABLED);
        }

        reg_value |= (0xff << I2C_DATA_CMD_DAT_POS);

        I2Cx->DATA_CMD = reg_value;

        if (!_wait_status(I2Cx, I2C_STATUS_RFNE_POS, 1, 10 * 1000)) {
            goto __HW_I2C_ReadBlock_Exit;
        }

        pdata[rx_cnt] = I2Cx->DATA_CMD;
        rx_cnt++;
    }

__HW_I2C_ReadBlock_Exit:

    if (config->master) {
        SET_BIT(I2Cx->ENABLE, I2C_ENABLE_ENABLE_POS, I2C_DISABLED);
    }

    return rx_cnt;
}

uint32_t HW_I2C_GetCurrentSendFIFOLength(I2C_TypeDef *I2Cx)
{
    return I2Cx->TXFLR;
}

uint32_t HW_I2C_GetCurrentRecvFIFOLength(I2C_TypeDef *I2Cx)
{
    return I2Cx->RXFLR;
}

uint32_t HW_I2C_GetStatus(I2C_TypeDef *I2Cx)
{
    return I2Cx->STATUS;
}

uint32_t HW_I2C_GetInterruptStatus(I2C_TypeDef *I2Cx)
{
    return I2Cx->INTR_STAT;
}

uint32_t HW_I2C_ClearInterrupt(I2C_TypeDef *I2Cx, uint32_t raw_intr_stat)
{
    uint32_t reg_value;

    switch (raw_intr_stat) {
    case I2C_INTR_MASK_M_RX_OVER_POS:
        reg_value = I2Cx->CLR_RX_OVER;
        break;
    case I2C_INTR_MASK_M_TX_OVER_POS:
        reg_value = I2Cx->CLR_TX_OVER;
        break;
    case I2C_INTR_MASK_M_RD_REQ_POS:
        reg_value = I2Cx->CLR_RD_REQ;
        break;
    case I2C_INTR_MASK_M_TX_ABRT_POS:
        reg_value = I2Cx->CLR_TX_ABRT;
        break;
    case I2C_INTR_MASK_M_RX_DONE_POS:
        reg_value = I2Cx->CLR_RX_DONE;
        break;
    case I2C_INTR_MASK_M_ACTIVITY_POS:
        reg_value = I2Cx->CLR_ACTIVITY;
        break;
    case I2C_INTR_MASK_M_STOP_DET_POS:
        reg_value = I2Cx->CLR_STOP_DET;
        break;
    case I2C_INTR_MASK_M_START_DET_POS:
        reg_value = I2Cx->CLR_START_DET;
        break;
    case I2C_INTR_MASK_M_GEN_CALL_POS:
        reg_value = I2Cx->CLR_GEN_CALL;
        break;
    default:
        reg_value = I2Cx->CLR_INTR;
        break;
    }

    return reg_value;
}

#ifdef DEBUG_I2C

#include "xprintf.h"

static uint8_t tx_buffer[8];
static uint8_t rx_buffer[8];
static uint32_t length = sizeof(rx_buffer);

int selftest_i2c(void)
{
#if (DEBUG_I2C_CASE == DEBUG_I2C_NORMAL)
    I2C_TypeDef *i2c0 = I2C0;
    I2C_TypeDef *i2c1 = I2C1;

    xprintf("I2C0 VERSION = %08x\n", i2c0->COMP_VERSION);
    xprintf("I2C0 TYPE = %08x\n", i2c0->COMP_TYPE);
    xprintf("I2C1 VERSION = %08x\n", i2c1->COMP_VERSION);
    xprintf("I2C1 TYPE = %08x\n", i2c1->COMP_TYPE);

    DeviceI2cConfig_t i2c_master_config =
        {
            .mode = eI2cMode_Standard,
            .restart = 0,

            .master = 1,
            .master_code = 0x3,
            .addr_bits = 7,
            .addr = 0x01,
        };

    DeviceI2cConfig_t i2c_slave_config =
        {
            .mode = eI2cMode_Standard,
            .restart = 0,

            .master = 0,
            .addr_bits = 7,
            .addr = 0x01,
        };

    for (uint32_t i = 0; i < sizeof(rx_buffer); i++) {
        rx_buffer[i] = 0xff;
    }

    HW_I2C_InitConfig(i2c0, &i2c_master_config);
    HW_I2C_InitConfig(i2c1, &i2c_slave_config);

    length = HW_I2C_ReadBlock(i2c0, &i2c_master_config, NULL, 0, rx_buffer, length);

    xprintf("recv %d.\n", length);
    for (uint32_t i = 0; i < length; i++) {
        xprintf("%02x ", rx_buffer[i]);
        tx_buffer[i] = rx_buffer[i];
    }
    xprintf("\n");

    length = HW_I2C_WriteBlock(i2c0, &i2c_master_config, NULL, 0, tx_buffer, length);
    xprintf("send %d.\n", length);
#endif // #if ( DEBUG_I2C_CASE == DEBUG_I2C_NORMAL )

#if (DEBUG_I2C_CASE == DEBUG_I2C_DMA)
    I2C_TypeDef *i2c0 = I2C0;
    I2C_TypeDef *i2c1 = I2C1;
    DMA_TypeDef *dma = DMA0;

    xprintf("I2C0 VERSION = %08x\n", i2c0->COMP_VERSION);
    xprintf("I2C0 TYPE = %08x\n", i2c0->COMP_TYPE);
    xprintf("I2C1 VERSION = %08x\n", i2c1->COMP_VERSION);
    xprintf("I2C1 TYPE = %08x\n", i2c1->COMP_TYPE);

    DeviceI2cConfig_t i2c_master_config =
        {
            .mode = eI2cMode_Standard,
            .restart = 0,

            .master = 1,
            .master_code = 0x3,
            .addr_bits = 7,
            .addr = 0x01,

            .tx_dma = 1,
            .tx_dma_level = 0x1,
            .rx_dma = 0,
            .rx_dma_level = 0x0,
        };

    DeviceI2cConfig_t i2c_slave_config =
        {
            .mode = eI2cMode_Standard,
            .restart = 0,

            .master = 0,
            .addr_bits = 7,
            .addr = 0x01,

            .tx_dma = 0,
            .tx_dma_level = 0,
            .rx_dma = 0,
            .rx_dma_level = 0,
        };

    DeviceDmaConfig_t dma_config =
        {
            .mode = eDmaMode_Mem2Dev,
            .channel = eDmaChannel_0,

            .transfer_size = length,
            .priority = 31,

            .src_addr = (uint32_t)&tx_buffer,
            .src_addr_mode = eDmaAddrMode_Inc,
            .src_data_width = eDmaDataWidth_8Bit,
            .src_data_depth = eDmaDataDepth_1,

            .dst_addr = (uint32_t)i2c0->DATA_CMD,
            .dst_addr_mode = eDmaAddrMode_Fixed,
            .dst_data_width = eDmaDataWidth_8Bit,
            .dst_data_depth = eDmaDataDepth_1,

            .user_data = 0,
        };

    for (uint32_t i = 0; i < sizeof(rx_buffer); i++) {
        rx_buffer[i] = 0xff;
    }

    HW_I2C_InitConfig(i2c0, &i2c_master_config);
    HW_I2C_InitConfig(i2c1, &i2c_slave_config);

    length = HW_I2C_ReadBlock(i2c0, &i2c_master_config, NULL, 0, rx_buffer, length);

    xprintf("recv %d.\n", length);
    for (uint32_t i = 0; i < length; i++) {
        xprintf("%02x ", rx_buffer[i]);
        tx_buffer[i] = rx_buffer[i];
    }
    xprintf("\n");

    HW_DMA_InitConfig(dma, &dma_config);
    HW_DMA_ChannelEnable(dma, dma_config.channel);

    while (!HW_DMA_WaitComplete(dma, dma_config.channel))
        ;
    xprintf("send %d.\n", length);
#endif // #if ( DEBUG_I2C_CASE == DEBUG_I2C_DMA )

#if 0 // @@
    xprintf("I2C0 TXFLR = %d(total: %d)\n", i2c0->TXFLR, ((i2c0->COMP_PARAM_1 >> I2C_COMP_PARAM_TX_BUFFER_DEPTH_POS) & 0xff) + 1);
    xprintf("I2C0 RXFLR = %d(total: %d)\n", i2c0->RXFLR, ((i2c0->COMP_PARAM_1 >> I2C_COMP_PARAM_RX_BUFFER_DEPTH_POS) & 0xff) + 1);
    xprintf("I2C1 TXFLR = %d(total: %d)\n", i2c1->TXFLR, ((i2c1->COMP_PARAM_1 >> I2C_COMP_PARAM_TX_BUFFER_DEPTH_POS) & 0xff) + 1);
    xprintf("I2C1 RXFLR = %d(total: %d)\n", i2c1->RXFLR, ((i2c1->COMP_PARAM_1 >> I2C_COMP_PARAM_RX_BUFFER_DEPTH_POS) & 0xff) + 1);
#endif

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_I2C
