/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_SMC_H__
#define __DRV_SMC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* SMC MEMC_STATUS */
#define SMC_MEMC_STATUS_RAW_ECC_INT1_POS         (12) // 1-bit
#define SMC_MEMC_STATUS_RAW_ECC_INT0_POS         (11) // 1-bit
#define SMC_MEMC_STATUS_ECC_INT1_POS             (10) // 1-bit
#define SMC_MEMC_STATUS_ECC_INT0_POS             (9)  // 1-bit
#define SMC_MEMC_STATUS_ECC_INT1_EN_POS          (8)  // 1-bit
#define SMC_MEMC_STATUS_ECC_INT0_EN_POS          (7)  // 1-bit
#define SMC_MEMC_STATUS_RAW_INT_STATUS1_POS      (6)  // 1-bit
#define SMC_MEMC_STATUS_RAW_INT_STATUS0_POS      (5)  // 1-bit
#define SMC_MEMC_STATUS_INT_STATUS1_POS          (4)  // 1-bit
#define SMC_MEMC_STATUS_INT_STATUS0_POS          (3)  // 1-bit
#define SMC_MEMC_STATUS_INT1_EN_POS              (2)  // 1-bit
#define SMC_MEMC_STATUS_INT0_EN_POS              (1)  // 1-bit
#define SMC_MEMC_STATUS_STATE_POS                (0)  // 1-bit

/* SMC MEMIF_CFG */
#define SMC_MEMIF_CFG_EXCLUSIVE_MONITORS_POS     (16) // 2-bit
#define SMC_MEMIF_CFG_REMAP1_POS                 (14) // 1-bit
#define SMC_MEMIF_CFG_MEMORY_WIDTH1_POS          (12) // 2-bit
#define SMC_MEMIF_CFG_MEMORY_CHIPS1_POS          (10) // 2-bit
#define SMC_MEMIF_CFG_MEMORY_TYPE1_POS           (8)  // 2-bit
#define SMC_MEMIF_CFG_REMAP0_POS                 (6)  // 1-bit
#define SMC_MEMIF_CFG_MEMORY_WIDTH0_POS          (4)  // 2-bit
#define SMC_MEMIF_CFG_MEMORY_CHIPS0_POS          (2)  // 2-bit
#define SMC_MEMIF_CFG_MEMORY_TYPE0_POS           (0)  // 2-bit

/* SMC MEMC_CFG_SET */
#define SMC_MEMC_CFG_SET_ECC_INT_ENABLE1_POS     (6) // 1-bit
#define SMC_MEMC_CFG_SET_ECC_INT_ENABLE0_POS     (5) // 1-bit
#define SMC_MEMC_CFG_SET_LOW_POWER_REQ_POS       (2) // 1-bit
#define SMC_MEMC_CFG_SET_INT_ENABLE1_POS         (1) // 1-bit
#define SMC_MEMC_CFG_SET_INT_ENABLE0_POS         (0) // 1-bit

/* SMC MEMC_CFG_CLR */
#define SMC_MEMC_CFG_CLR_ECC_INT_DISABLE1_POS    (6) // 1-bit
#define SMC_MEMC_CFG_CLR_ECC_INT_DISABLE0_POS    (5) // 1-bit
#define SMC_MEMC_CFG_CLR_INT_CLR1_POS            (4) // 1-bit
#define SMC_MEMC_CFG_CLR_INT_CLR0_POS            (3) // 1-bit
#define SMC_MEMC_CFG_CLR_LOW_POWER_EXIT_POS      (2) // 1-bit
#define SMC_MEMC_CFG_CLR_INT_DISABLE1_POS        (1) // 1-bit
#define SMC_MEMC_CFG_CLR_INT_DISABLE0_POS        (0) // 1-bit

/* SMC DIRECT_CMD */
#define SMC_DIRECT_CMD_CHIP_INTERFACE_SELECT_POS (25) // 1-bit
#define SMC_DIRECT_CMD_CHIP_BANK_SELECT_POS      (23) // 2-bit
#define SMC_DIRECT_CMD_CMD_TYPE_POS              (21) // 2-bit
#define SMC_DIRECT_CMD_SET_CRE_POS               (20) // 1-bit
#define SMC_DIRECT_CMD_ADDR_POS                  (0)  // 20-bit

/* SMC SET_OPMODE */
#define SMC_SET_OPMODE_SET_BURST_ALIGN_POS       (13) // 3-bit
#define SMC_SET_OPMODE_SET_BLS_POS               (12) // 1-bit
#define SMC_SET_OPMODE_SET_ADV_POS               (11) // 1-bit
#define SMC_SET_OPMODE_SET_BAA_POS               (10) // 1-bit
#define SMC_SET_OPMODE_SET_WR_BL_POS             (7)  // 3-bit
#define SMC_SET_OPMODE_SET_WR_SYNC_POS           (6)  // 1-bit
#define SMC_SET_OPMODE_SET_RD_BL_POS             (3)  // 3-bit
#define SMC_SET_OPMODE_SET_RD_SYNC_POS           (2)  // 1-bit
#define SMC_SET_OPMODE_SET_MW_POS                (0)  // 2-bit

/* SMC REFRESH_PERIOD */
#define SMC_REFRESH_PERIOD_PERIOD_POS            (0) // 4-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define SMC_DISABLED                             (0)
#define SMC_ENABLED                              (1)

/* value of MEMC_STATUS_STATE */
#define STATE_READY                              (0)
#define STATE_LOW_POWER                          (1)

/* value of DIRECT_CMD_CMD_TYPE */
#define CMD_TYPE_UPDATEREG_AND_AXI               (0x0)
#define CMD_TYPE_MODEREG                         (0x1)
#define CMD_TYPE_UPDATEREG                       (0x2)
#define CMD_TYPE_MODEREG_AND_UPDATEREG           (0x3)

/* value of DIRECT_CMD_SET_CRE */
#define CRE_LOW                                  (0)
#define CRE_HIGH_WHEN_MODEREG_WRITE              (1)

/* value of SET_OPMODE_SET_MW */
#define MW_8BIT                                  (0x0)
#define MW_16BIT                                 (0x1)
#define MW_32BIT                                 (0x2)

/**
 * @brief Self Config Param Define
 */

#define GET_INTERFACE_INDEX(bank)                (GET_BIT((bank), 2))
#define GET_INTERFACE_BANK(bank)                 ((bank)&0x3)

#define SMC_MEM_BANK_SIZE                        (64 * 1024 * 1024)
#define GET_SMC_MEM_BANK_BASE_ADDR(bn)           (SMC_MEM_BASE + (bn)*SMC_MEM_BANK_SIZE)

#if 1 // @@
// Note:
//  I don't like to define these in driver file, but i have to do this
//  If possible, please move them to separate device related files

#define NAND_COMMAND_PHASE                    (0)
#define NAND_DATA_PHASE                       (1)

#define NAND_END_COMMAND_INVALID              (0)
#define NAND_END_COMMAND_VALID                (1)

#define CMD_PHASE_ADDR(addr, nc, sc, ecv, ec) (((addr) << 24) |               \
                                               ((nc) << 21) |                 \
                                               ((ecv) << 20) |                \
                                               ((NAND_COMMAND_PHASE) << 19) | \
                                               ((ec) << 11) |                 \
                                               ((sc) << 3))

#define DATA_PHASE_ADDR(addr, last, ecc, ecv, ec) (((addr) << 24) |            \
                                                   ((last) << 21) |            \
                                                   ((ecv) << 20) |             \
                                                   ((NAND_DATA_PHASE) << 19) | \
                                                   ((ec) << 11) |              \
                                                   ((ecc) << 10))

#define DATA_PHASE_ADDR_MODIFY(dpa, last, ecc) ((uint32_t)((uint32_t)(dpa) & ~(BIT(21) | BIT(10))) | \
                                                ((last) << 21) | ((ecc) << 10))

/* Standard NAND flash commands */
#define NAND_CMD_READ0        (0x00)
#define NAND_CMD_READ1        (0x01)
#define NAND_CMD_RNDOUT       (0x05)
#define NAND_CMD_PAGEPROG     (0x10)
#define NAND_CMD_READOOB      (0x50)
#define NAND_CMD_ERASE1       (0x60)
#define NAND_CMD_STATUS       (0x70)
#define NAND_CMD_STATUS_MULTI (0x71)
#define NAND_CMD_SEQIN        (0x80)
#define NAND_CMD_RNDIN        (0x85)
#define NAND_CMD_READID       (0x90)
#define NAND_CMD_ERASE2       (0xd0)
#define NAND_CMD_RESET        (0xff)

/* Extended NAND flash commands for large page devices */
#define NAND_CMD_CACHEDPROG   (0x15)
#define NAND_CMD_READSTART    (0x30)
#define NAND_CMD_RNDOUTSTART  (0xe0)

/* Undefined NAND flash commands */
#define NAND_CMD_NONE         (0x00)

typedef struct
{
    uint32_t capacity; // NOTE: bits
    uint32_t blocks;
    uint32_t block_size; // NOTE: Bytes
    uint32_t pages;
    uint32_t page_size; // NOTE: Bytes
} NandFlashChipInfo_t;
#endif // @@

typedef enum
{
    eSmcType_NAND = 0,
} SmcType_e;

typedef enum
{
    eSmcBank_0 = 0,
    eSmcBank_1,
    eSmcBank_2,
    eSmcBank_3,
    eSmcBank_4,
    eSmcBank_5,
    eSmcBank_6,
    eSmcBank_7,
} SmcBank_e;

typedef enum
{
    eSmcMemoryWidth_8 = MW_8BIT,
    eSmcMemoryWidth_16 = MW_16BIT,
    eSmcMemoryWidth_32 = MW_32BIT,
} SmcMemoryWidth_e;

typedef struct
{
    uint32_t status;

    SmcType_e type;

    SmcBank_e bank;
    uint32_t bank_addr; // NOTE: this is a out data

    SmcMemoryWidth_e memory_width;
    uint32_t refresh_period;

    union
    {
        struct
        {
            uint32_t t_rc      : 4;
            uint32_t t_wc      : 4;
            uint32_t t_ea      : 3;
            uint32_t t_wp      : 3;
            uint32_t t_clr     : 3;
            uint32_t t_ar      : 3;
            uint32_t t_rr      : 4;
            uint32_t undefined : 8;
        } nand; // NOTE: only valid when .mode is 'eSmcType_NAND'
        struct
        {
            uint32_t t0        : 4;
            uint32_t t1        : 4;
            uint32_t t2        : 3;
            uint32_t t3        : 3;
            uint32_t t4        : 3;
            uint32_t t5        : 3;
            uint32_t t6        : 4;
            uint32_t undefined : 8;
        } common;
        uint32_t value;
    } cycles;

    uint32_t edac;

    void *user_data;
} DeviceSmcConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceSmcConfig_t g_DeviceSmcDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_SMC_InitConfig(SMC_TypeDef *SMCx, DeviceSmcConfig_t *config);
uint32_t HW_SMC_GetStatus(SMC_TypeDef *SMCx);

int HW_SMC_NandFlashReadID(uint32_t base_addr, uint8_t *id, uint32_t len);
int HW_SMC_NandFlashReset(uint32_t base_addr);
int HW_SMC_NandFlashErase(uint32_t base_addr, uint32_t row_addr);
int HW_SMC_NandFlashRead(uint32_t base_addr, uint32_t col_addr, uint32_t row_addr, uint8_t *pbuf, uint32_t len);
int HW_SMC_NandFlashProgram(uint32_t base_addr, uint32_t col_addr, uint32_t row_addr, uint8_t *pbuf, uint32_t len);

uint32_t NandFlash_Erase(NandFlashChipInfo_t *chip_info, uint32_t addr);
uint32_t NandFlash_Read(NandFlashChipInfo_t *chip_info, uint32_t addr, uint8_t *pbuf, uint32_t len);
uint32_t NandFlash_Write(NandFlashChipInfo_t *chip_info, uint32_t addr, uint8_t *pbuf, uint32_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_SMC_H__ */
