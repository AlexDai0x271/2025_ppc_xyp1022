/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* I2C CTRLR0 */
#define I2C_CTRLR0_SMBUS_PERSISTENT_SLV_ADDR_EN_POS (19) // 1-bit
#define I2C_CTRLR0_SMBUS_ARP_EN_POS                 (18) // 1-bit
#define I2C_CTRLR0_SMBUS_SLAVE_QUICK_EN_POS         (17) // 1-bit
#define I2C_CTRLR0_OPTIONAL_SAR_CTRL_POS            (16) // 1-bit
#define I2C_CTRLR0_BUS_CLEAR_FEATURE_CTRL_POS       (11) // 1-bit
#define I2C_CTRLR0_STOP_DET_IF_MASTER_ACTIVE_POS    (10) // 1-bit
#define I2C_CTRLR0_RX_FIFO_FULL_HLD_CTRL_POS        (9)  // 1-bit
#define I2C_CTRLR0_TX_EMPTY_CTRL_POS                (8)  // 1-bit
#define I2C_CTRLR0_STOP_DET_IFADDRESSED_POS         (7)  // 1-bit
#define I2C_CTRLR0_IC_SLAVE_DISABLE_POS             (6)  // 1-bit
#define I2C_CTRLR0_IC_RESTART_EN_POS                (5)  // 1-bit
#define I2C_CTRLR0_IC_10BITADDR_MASTER_POS          (4)  // 1-bit
#define I2C_CTRLR0_IC_10BITADDR_SLAVE_POS           (3)  // 1-bit
#define I2C_CTRLR0_SPEED_POS                        (1)  // 2-bit
#define I2C_CTRLR0_MASTER_MODE_POS                  (0)  // 1-bit

/* I2C TAR */
#define I2C_TAR_SMBUS_QUICK_CMD_POS                 (16) // 1-bit
#define I2C_TAR_DEVICE_ID_POS                       (13) // 1-bit
#define I2C_TAR_IC_10BITADDR_MASTER_POS             (12) // 1-bit
#define I2C_TAR_SPECIAL_POS                         (11) // 1-bit
#define I2C_TAR_GC_OR_START_POS                     (10) // 1-bit
#define I2C_TAR_IC_TAR_POS                          (0)  // 10-bit

/* I2C SAR */
#define I2C_TAR_IC_SAR_POS                          (0) // 10-bit

/* I2C DATA_CMD */
#define I2C_DATA_CMD_FIRST_DATA_BYTE_POS            (11) // 1-bit
#define I2C_DATA_CMD_RESTART_POS                    (10) // 1-bit
#define I2C_DATA_CMD_STOP_POS                       (9)  // 1-bit
#define I2C_DATA_CMD_CMD_POS                        (8)  // 1-bit
#define I2C_DATA_CMD_DAT_POS                        (0)  // 8-bit

/* I2C INTR_MASK */
#define I2C_INTR_MASK_M_SCL_STUCK_AT_LOW_POS        (14) // 1-bit
#define I2C_INTR_MASK_M_MASTER_ON_HOLD_POS          (13) // 1-bit
#define I2C_INTR_MASK_M_RESTART_DET_POS             (12) // 1-bit
#define I2C_INTR_MASK_M_GEN_CALL_POS                (11) // 1-bit
#define I2C_INTR_MASK_M_START_DET_POS               (10) // 1-bit
#define I2C_INTR_MASK_M_STOP_DET_POS                (9)  // 1-bit
#define I2C_INTR_MASK_M_ACTIVITY_POS                (8)  // 1-bit
#define I2C_INTR_MASK_M_RX_DONE_POS                 (7)  // 1-bit
#define I2C_INTR_MASK_M_TX_ABRT_POS                 (6)  // 1-bit
#define I2C_INTR_MASK_M_RD_REQ_POS                  (5)  // 1-bit
#define I2C_INTR_MASK_M_TX_EMPTY_POS                (4)  // 1-bit
#define I2C_INTR_MASK_M_TX_OVER_POS                 (3)  // 1-bit
#define I2C_INTR_MASK_M_RX_FULL_POS                 (2)  // 1-bit
#define I2C_INTR_MASK_M_RX_OVER_POS                 (1)  // 1-bit
#define I2C_INTR_MASK_M_RX_UNDER_POS                (0)  // 1-bit

/* I2C RAW_INTR_STAT */
#define I2C_RAW_INTR_STAT_SCL_STUCK_AT_LOW_POS      (14) // 1-bit
#define I2C_RAW_INTR_STAT_MASTER_ON_HOLD_POS        (13) // 1-bit
#define I2C_RAW_INTR_STAT_RESTART_DET_POS           (12) // 1-bit
#define I2C_RAW_INTR_STAT_GEN_CALL_POS              (11) // 1-bit
#define I2C_RAW_INTR_STAT_START_DET_POS             (10) // 1-bit
#define I2C_RAW_INTR_STAT_STOP_DET_POS              (9)  // 1-bit
#define I2C_RAW_INTR_STAT_ACTIVITY_POS              (8)  // 1-bit
#define I2C_RAW_INTR_STAT_RX_DONE_POS               (7)  // 1-bit
#define I2C_RAW_INTR_STAT_TX_ABRT_POS               (6)  // 1-bit
#define I2C_RAW_INTR_STAT_RD_REQ_POS                (5)  // 1-bit
#define I2C_RAW_INTR_STAT_TX_EMPTY_POS              (4)  // 1-bit
#define I2C_RAW_INTR_STAT_TX_OVER_POS               (3)  // 1-bit
#define I2C_RAW_INTR_STAT_RX_FULL_POS               (2)  // 1-bit
#define I2C_RAW_INTR_STAT_RX_OVER_POS               (1)  // 1-bit
#define I2C_RAW_INTR_STAT_RX_UNDER_POS              (0)  // 1-bit

/* I2C RX_FL */
#define I2C_RX_FL_VAL_POS                           (0) // 8-bit

/* I2C TX_FL */
#define I2C_TX_FL_VAL_POS                           (0) // 8-bit

/* I2C ENABLE */
#define I2C_ENABLE_ENABLE_POS                       (0) // 1-bit

/* I2C STATUS */
#define I2C_STATUS_SMBUS_ALERT_STATUS_POS           (20) // 1-bit
#define I2C_STATUS_SMBUS_SUSPEND_STATUS_POS         (19) // 1-bit
#define I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED_POS    (18) // 1-bit
#define I2C_STATUS_SMBUS_SLAVE_ADDR_VALID_POS       (17) // 1-bit
#define I2C_STATUS_SMBUS_QUICK_CMD_BIT_POS          (16) // 1-bit
#define I2C_STATUS_SDA_STUCK_NOT_RECOVERED_POS      (11) // 1-bit
#define I2C_STATUS_SLV_HOLD_RX_FIFO_FULL_POS        (10) // 1-bit
#define I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY_POS       (9)  // 1-bit
#define I2C_STATUS_MST_HOLD_RX_FIFO_FULL_POS        (8)  // 1-bit
#define I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY_POS       (7)  // 1-bit
#define I2C_STATUS_SLV_ACTIVITY_POS                 (6)  // 1-bit
#define I2C_STATUS_MST_ACTIVITY_POS                 (5)  // 1-bit
#define I2C_STATUS_RFF_POS                          (4)  // 1-bit
#define I2C_STATUS_RFNE_POS                         (3)  // 1-bit
#define I2C_STATUS_TFE_POS                          (2)  // 1-bit
#define I2C_STATUS_TFNF_POS                         (1)  // 1-bit
#define I2C_STATUS_ACTIVITY_POS                     (0)  // 1-bit

/* I2C DMA_CR */
#define I2C_DMA_CR_TDMAE                            (1) // 1-bit
#define I2C_DMA_CR_RDMAE                            (0) // 1-bit

/* I2C COMP_PARAM_1 */
#define I2C_COMP_PARAM_TX_BUFFER_DEPTH_POS          (16) // 8-bit
#define I2C_COMP_PARAM_RX_BUFFER_DEPTH_POS          (8)  // 8-bit
#define I2C_COMP_PARAM_ADD_ENCODED_PARAMS_POS       (7)  // 1-bit
#define I2C_COMP_PARAM_HAS_DMA_POS                  (6)  // 1-bit
#define I2C_COMP_PARAM_INTR_IO_POS                  (5)  // 1-bit
#define I2C_COMP_PARAM_HC_COUNT_VALUES_POS          (4)  // 1-bit
#define I2C_COMP_PARAM_MAX_SPEED_MODE_POS           (2)  // 2-bit
#define I2C_COMP_PARAM_APB_DATA_WIDTH_POS           (0)  // 2-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define I2C_DISABLED                                (0)
#define I2C_ENABLED                                 (1)

/* value of CTRLR0_IC_SLAVE_DISABLE */
#define IC_SLAVE_ENABLED                            (0)
#define IC_SLAVE_DISABLED                           (1)

/* value of CTRLR0_IC_RESTART_ENS */
#define IC_RESTART_DISABLED                         (0)
#define IC_RESTART_ENABLED                          (1)

/* value of CTRLR0_IC_10BITADDR_MASTER */
/* value of TAR_IC_10BITADDR_MASTER */
#define MASTER_ADDR_7BITS                           (0)
#define MASTER_ADDR_10BITS                          (1)

/* value of CTRLR0_IC_10BITADDR_SLAVE */
#define SLAVE_ADDR_7BITS                            (0)
#define SLAVE_ADDR_10BITS                           (1)

/* value of CTRLR0_SPEED */
#define SPEED_STANDARD                              (0x1)
#define SPEED_FAST                                  (0x2)
#define SPEED_HIGH                                  (0x3)

/* value of CTRLR0_MASTER_MODE */
#define MASTER_MODE_SLAVE                           (0)
#define MASTER_MODE_MASTER                          (1)

/* value of DATA_CMD_CMD */
#define CMD_WRITE                                   (0)
#define CMD_READ                                    (1)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eI2cMode_Standard = 0,   // 100 kbits/s
    eI2cMode_FastSpeed,      // 400 kbits/s
    eI2cMode_FastSpeed_Plus, // 1000 kbits/s
    eI2cMode_HighSpeed,      // 3.4 Mbits/s
} I2cMode_e;

typedef struct
{
    uint32_t status;

    I2cMode_e mode;
    uint8_t restart;

    uint8_t master;
    uint8_t master_code;
    uint8_t addr_bits;
    uint8_t addr;

    uint32_t tx_dma;
    uint32_t tx_dma_level;
    uint32_t rx_dma;
    uint32_t rx_dma_level;

    void *user_data;
} DeviceI2cConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceI2cConfig_t g_DeviceI2cDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_I2C_InitConfig(I2C_TypeDef *I2Cx, DeviceI2cConfig_t *config);
uint32_t HW_I2C_WriteBlock(I2C_TypeDef *I2Cx, DeviceI2cConfig_t *config, uint8_t *pcmd, uint32_t cmd_len, uint8_t *pdata, uint32_t data_len);
uint32_t HW_I2C_ReadBlock(I2C_TypeDef *I2Cx, DeviceI2cConfig_t *config, uint8_t *pcmd, uint32_t cmd_len, uint8_t *pdata, uint32_t data_len);
uint32_t HW_I2C_GetCurrentSendFIFOLength(I2C_TypeDef *I2Cx);
uint32_t HW_I2C_GetCurrentRecvFIFOLength(I2C_TypeDef *I2Cx);
uint32_t HW_I2C_GetStatus(I2C_TypeDef *I2Cx);
uint32_t HW_I2C_GetInterruptStatus(I2C_TypeDef *I2Cx);
uint32_t HW_I2C_ClearInterrupt(I2C_TypeDef *I2Cx, uint32_t raw_intr_stat);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_I2C_H__ */
