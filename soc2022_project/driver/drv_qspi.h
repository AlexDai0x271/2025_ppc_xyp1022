/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_QSPI_H__
#define __DRV_QSPI_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

// NOTE: 34 is include 1-cmd, 2-address, 32-data
#define QSPI_FIFO_MAX (1 + 1 + 32)

/**
 * @brief Register Field Define
 */

#include "drv_spi.h"

/* QSPI CTRLR0 */
#define QSPI_CTRLR0_SSI_IS_MST_POS          (31) // 1-bit
#define QSPI_CTRLR0_SPI_HYPERBUS_EN_POS     (24) // 1-bit
#define QSPI_CTRLR0_SPI_FRF_POS             (22) // 2-bit
#define QSPI_CTRLR0_CFS_POS                 (16) // 4-bit
#define QSPI_CTRLR0_SSTE_POS                (14) // 1-bit
#define QSPI_CTRLR0_SRL_POS                 (13) // 1-bit
#define QSPI_CTRLR0_SLV_OE_POS              (12) // 1-bit
#define QSPI_CTRLR0_TMOD_POS                (10) // 2-bit
#define QSPI_CTRLR0_SCPOL_POS               (9)  // 1-bit
#define QSPI_CTRLR0_SCPH_POS                (8)  // 1-bit
#define QSPI_CTRLR0_FRF_POS                 (6)  // 2-bit
#define QSPI_CTRLR0_DFS_POS                 (0)  // 5-bit

/* QSPI TXFTLR */
#define QSPI_TXFTLR_TXFTHR                  (16) // 16-bit
#define QSPI_TXFTLR_TFT                     (0)  // 16-bit

/* QSPI DMACR */
#define QSPI_DMACR_TDMAE                    (1) // 1-bit
#define QSPI_DMACR_RDMAE                    (0) // 1-bit

/* QSPI SPI_CTRLR0 */
#define QSPI_SPI_CTRLR0_CLK_STRETCH_EN_POS  (30) // 1-bit
#define QSPI_SPI_CTRLR0_XIP_PREFETCH_EN_POS (29) // 1-bit
#define QSPI_SPI_CTRLR0_XIP_MBL_POS         (26) // 2-bit
#define QSPI_SPI_CTRLR0_SPI_RXDS_SIG_EN_POS (25) // 1-bit
#define QSPI_SPI_CTRLR0_SPI_DM_EN_POS       (24) // 1-bit
#define QSPI_SPI_CTRLR0_XIP_CONTXFER_EN_POS (21) // 1-bit
#define QSPI_SPI_CTRLR0_XIP_INST_EN_POS     (20) // 1-bit
#define QSPI_SPI_CTRLR0_XIP_DFS_HC_POS      (19) // 1-bit
#define QSPI_SPI_CTRLR0_SPI_RXDS_EN_POS     (18) // 1-bit
#define QSPI_SPI_CTRLR0_INST_DDR_EN_POS     (17) // 1-bit
#define QSPI_SPI_CTRLR0_SPI_DDR_EN_POS      (16) // 1-bit
#define QSPI_SPI_CTRLR0_WAIT_CYCLES_POS     (11) // 5-bit
#define QSPI_SPI_CTRLR0_INST_L_POS          (8)  // 2-bit
#define QSPI_SPI_CTRLR0_XIP_MD_BIT_EN_POS   (7)  // 1-bit
#define QSPI_SPI_CTRLR0_ADDR_L_POS          (2)  // 4-bit
#define QSPI_SPI_CTRLR0_TRANS_TYPE_POS      (0)  // 2-bit

/**
 * @brief Register Field Define
 */

/* value of CTRLR0_SSI_IS_MST */
#define SSI_IS_MST_SLAVE                    (0)
#define SSI_IS_MST_MASTER                   (1)

/* value of CTRLR0_DFS */
#define DFS_17_BIT                          (0x10)
#define DFS_18_BIT                          (0x11)
#define DFS_19_BIT                          (0x12)
#define DFS_20_BIT                          (0x13)
#define DFS_21_BIT                          (0x14)
#define DFS_22_BIT                          (0x15)
#define DFS_23_BIT                          (0x16)
#define DFS_24_BIT                          (0x17)
#define DFS_25_BIT                          (0x18)
#define DFS_26_BIT                          (0x19)
#define DFS_27_BIT                          (0x1a)
#define DFS_28_BIT                          (0x1b)
#define DFS_29_BIT                          (0x1c)
#define DFS_30_BIT                          (0x1d)
#define DFS_31_BIT                          (0x1e)
#define DFS_32_BIT                          (0x1f)

/* value of SPI_CTRLR0_XIP_MBL */
#define XIP_MBL_2                           (0x0)
#define XIP_MBL_4                           (0x1)
#define XIP_MBL_8                           (0x2)
#define XIP_MBL_16                          (0x3)

/* value of SPI_CTRLR0_INST_L */
#define INST_L0                             (0x0)
#define INST_L4                             (0x1)
#define INST_L8                             (0x2)
#define INST_L16                            (0x3)

/* value of SPI_CTRLR0_ADDR_L */
#define ADDR_L0                             (0x0)
#define ADDR_L4                             (0x1)
#define ADDR_L8                             (0x2)
#define ADDR_L12                            (0x3)
#define ADDR_L16                            (0x4)
#define ADDR_L20                            (0x5)
#define ADDR_L24                            (0x6)
#define ADDR_L28                            (0x7)
#define ADDR_L32                            (0x8)
#define ADDR_L36                            (0x9)
#define ADDR_L40                            (0xa)
#define ADDR_L44                            (0xb)
#define ADDR_L48                            (0xc)
#define ADDR_L52                            (0xd)
#define ADDR_L56                            (0xe)
#define ADDR_L60                            (0xf)

/* value of SPI_CTRLR0_TRANS_TYPE */
#define TRANS_TYPE_ISAS                     (0x0)
#define TRANS_TYPE_ISAX                     (0x1)
#define TRANS_TYPE_IXAX                     (0x2)
#define TRANS_TYPE_RSVD                     (0x3)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eQspiMode_Standard = 0,
    eQspiMode_QuadTx,
    eQspiMode_QuadRx,
} QspiMode_e;

typedef enum
{
    eQspiTransType_11X = 0,
    eQspiTransType_1XX,
    eQspiTransType_XXX,
} QspiTransType_e;

typedef enum
{
    eQspiInstLen_0 = 0,
    eQspiInstLen_4,
    eQspiInstLen_8,
    eQspiInstLen_16,
} QspiInstLen_e;

typedef enum
{
    eQspiAddrLen_24 = 0,
    eQspiAddrLen_32,
} QspiAddrLen_e;

typedef struct
{
    uint32_t status;

    QspiMode_e quad_mode;
    QspiTransType_e quad_trans_type;
    QspiInstLen_e quad_inst_len;
    QspiAddrLen_e quad_addr_len;
    uint8_t quad_wait_cycles;

    SpiMode_e mode;
    SpiFormat_e format;
    uint8_t data_width;
    uint32_t rate;

    void *user_data;
} DeviceQspiConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceQspiConfig_t g_DeviceQspiDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_QSPI_InitConfig(QSPI_TypeDef *QSPIx, DeviceQspiConfig_t *config);

int HW_QSPI_WriteBlock(QSPI_TypeDef *QSPIx, QspiMode_e quad_mode, uint8_t cmd, uint32_t *paddr, uint32_t addr_len, uint8_t *txbuf, uint8_t *rxbuf, uint32_t data_len);
int HW_QSPI_ReadBlock(QSPI_TypeDef *QSPIx, QspiMode_e quad_mode, uint8_t cmd, uint32_t *paddr, uint32_t addr_len, uint8_t *txbuf, uint8_t *rxbuf, uint32_t data_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_QSPI_H__ */
