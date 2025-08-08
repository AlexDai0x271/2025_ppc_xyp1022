/*
 * Copyright     (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* SPI CTRLR0 */
#define SPI_CTRLR0_SSTE_POS    (24) // 1-bit
#define SPI_CTRLR0_SPI_FRF_POS (21) // 2-bit
#define SPI_CTRLR0_DFS_32_POS  (16) // 5-bit
#define SPI_CTRLR0_CFS_POS     (12) // 4-bit
#define SPI_CTRLR0_SRL_POS     (11) // 1-bit
#define SPI_CTRLR0_SLV_OE_POS  (10) // 1-bit
#define SPI_CTRLR0_TMOD_POS    (8)  // 2-bit
#define SPI_CTRLR0_SCPOL_POS   (7)  // 1-bit
#define SPI_CTRLR0_SCPH_POS    (6)  // 1-bit
#define SPI_CTRLR0_FRF_POS     (4)  // 2-bit
#define SPI_CTRLR0_DFS_POS     (0)  // 4-bit

/* SPI CTRLR1 */
#define SPI_CTRLR1_NDF_POS     (0) // 1-bit

/* SPI SSIENR */
#define SPI_SSIENR_SSIC_EN_POS (0) // 1-bit

/* SPI SR */
#define SPI_SR_DCOL_POS        (6) // 1-bit
#define SPI_SR_TXE_POS         (5) // 1-bit
#define SPI_SR_RFF_POS         (4) // 1-bit
#define SPI_SR_RFNE_POS        (3) // 1-bit
#define SPI_SR_TFE_POS         (2) // 1-bit
#define SPI_SR_TFNF_POS        (1) // 1-bit
#define SPI_SR_BUSY_POS        (0) // 1-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define SPI_DISABLED           (0)
#define SPI_ENABLED            (1)

/* value of CTRLR0_SSTE */
#define SSTE_TOGGLE_DISABLE    (0)
#define SSTE_TOGGLE_EN         (1)

/* value of CTRLR0_SPI_FRF */
#define SPI_FRF_STANDARD       (0x0)
#define SPI_FRF_DUAL           (0x1)
#define SPI_FRF_QUAD           (0x2)
#define SPI_FRF_OCTAL          (0x3)

/* value of CTRLR0_CFS */
#define CFS_SIZE_01_BIT        (0x0)
#define CFS_SIZE_02_BIT        (0x1)
#define CFS_SIZE_03_BIT        (0x2)
#define CFS_SIZE_04_BIT        (0x3)
#define CFS_SIZE_05_BIT        (0x4)
#define CFS_SIZE_06_BIT        (0x5)
#define CFS_SIZE_07_BIT        (0x6)
#define CFS_SIZE_08_BIT        (0x7)
#define CFS_SIZE_09_BIT        (0x8)
#define CFS_SIZE_10_BIT        (0x9)
#define CFS_SIZE_11_BIT        (0xa)
#define CFS_SIZE_12_BIT        (0xb)
#define CFS_SIZE_13_BIT        (0xc)
#define CFS_SIZE_14_BIT        (0xd)
#define CFS_SIZE_15_BIT        (0xe)
#define CFS_SIZE_16_BIT        (0xf)

/* value of CTRLR0_SRL */
#define SRL_NORMAL_MODE        (0)
#define SRL_TESTING_MODE       (1)

/* value of CTRLR0_SLV_OE */
#define SLV_OE_ENABLED         (0)
#define SLV_OE_DISABLED        (1)

/* value of CTRLR0_TMOD */
#define TMOD_TX_AND_RX         (0x0)
#define TMOD_TX_ONLY           (0x1)
#define TMOD_RX_ONLY           (0x2)
#define TMOD_EEPROM_READ       (0x3)

/* value of CTRLR0_SCPOL */
#define SCPOL_INACTIVE_LOW     (0)
#define SCPOL_INACTIVE_HIGH    (1)

/* value of CTRLR0_SCPH */
#define SCPH_MIDDLE_BIT        (0)
#define SCPH_START_BIT         (1)

/* value of CTRLR0_FRF */
#define FRF_SPI                (0x0)
#define FRF_SSP                (0x1)
#define FRF_MICROWIRE          (0x2)
#define FRF_RESERVED           (0x3)

/* value of CTRLR0_DFS */
#define DFS_01_BIT_RESERVED    (0x0)
#define DFS_02_BIT_RESERVED    (0x1)
#define DFS_03_BIT_RESERVED    (0x2)
#define DFS_04_BIT             (0x3)
#define DFS_05_BIT             (0x4)
#define DFS_06_BIT             (0x5)
#define DFS_07_BIT             (0x6)
#define DFS_08_BIT             (0x7)
#define DFS_09_BIT             (0x8)
#define DFS_10_BIT             (0x9)
#define DFS_11_BIT             (0xa)
#define DFS_12_BIT             (0xb)
#define DFS_13_BIT             (0xc)
#define DFS_14_BIT             (0xd)
#define DFS_15_BIT             (0xe)
#define DFS_16_BIT             (0xf)

/* value of SSIENR_SSI_EN */
#define SSI_EN_DISABLED        (0)
#define SSI_EN_ENABLED         (1)

/* value of SER_SER */
#define SERS_NOT_SELECTED      (0)
#define SERS_SELECTED          (1)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eSpiMode_0 = 0,
    eSpiMode_1,
    eSpiMode_2,
    eSpiMode_3,
} SpiMode_e;

typedef enum
{
    eSpiFormat_SPI = 0,
    eSpiFormat_SSP,
    eSpiFormat_MICROWIRE,
} SpiFormat_e;

typedef struct
{
    uint32_t status;

    SpiMode_e mode;
    SpiFormat_e format;
    uint8_t data_width;
    uint32_t rate;

    void *user_data;
} DeviceSpiConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceSpiConfig_t g_DeviceSpiDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_SPI_InitConfig(SPI_TypeDef *SPIx, DeviceSpiConfig_t *config);
uint32_t HW_SPI_ReadWriteBlock(SPI_TypeDef *SPIx, uint8_t *txbuf, uint8_t *rxbuf, uint32_t length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_SPI_H__ */
