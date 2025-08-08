/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_AMC_H__
#define __DRV_AMC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

/* AMC WRMEM_CONFIG */
#define AMC_WRMEM_CONFIG_WT_POS                  (14) // 2-bit
#define AMC_WRMEM_CONFIG_RMW_POS                 (12) // 1-bit
#define AMC_WRMEM_CONFIG_BS_POS                  (8)  // 4-bit
#define AMC_WRMEM_CONFIG_WS_POS                  (0)  // 8-bit

/* AMC ORMEM_CONFIG */
#define AMC_ORMEM_CONFIG_WEN_POS                 (16) // 1-bit
#define AMC_ORMEM_CONFIG_WT_POS                  (14) // 2-bit
#define AMC_ORMEM_CONFIG_RMW_POS                 (12) // 1-bit
#define AMC_ORMEM_CONFIG_BS_POS                  (8)  // 4-bit
#define AMC_ORMEM_CONFIG_WS_POS                  (0)  // 8-bit

/* AMC IO_CONFIG */
#define AMC_IO_CONFIG_RDY_POS                    (10) // 1-bit
#define AMC_IO_CONFIG_WT_POS                     (8)  // 2-bit
#define AMC_IO_CONFIG_WS_POS                     (0)  // 8-bit

/* AMC EDAC_CONFIG */
#define AMC_EDAC_CONFIG_DOUBLE_ERR_HRESP_POS     (8) // 1-bit
#define AMC_EDAC_CONFIG_WR_WRITE_BACK_POS        (7) // 1-bit
#define AMC_EDAC_CONFIG_WR_EDAC_WRITE_BYPASS_POS (6) // 1-bit
#define AMC_EDAC_CONFIG_WR_EDAC_READ_BYPASS_POS  (5) // 1-bit
#define AMC_EDAC_CONFIG_WR_EDAC_EN_POS           (4) // 1-bit
#define AMC_EDAC_CONFIG_OR_WRITE_BACK_POS        (3) // 1-bit
#define AMC_EDAC_CONFIG_OR_EDAC_WRITE_BYPASS_POS (2) // 1-bit
#define AMC_EDAC_CONFIG_OR_EDAC_READ_BYPASS_POS  (1) // 1-bit
#define AMC_EDAC_CONFIG_OR_EDAC_EN_POS           (0) // 1-bit

/* AMC INT_CLEAR */
#define AMC_INT_CLEAR_IO_RDY_TO_INT_CLR_POS      (3) // 1-bit
#define AMC_INT_CLEAR_WR2OR_ERR_INT_CLR_POS      (2) // 1-bit
#define AMC_INT_CLEAR_MULTI_ERR_INT_CLR_POS      (1) // 1-bit
#define AMC_INT_CLEAR_SINGLE_ERR_INT_CLR_POS     (0) // 1-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define AMC_DISABLED                             (0)
#define AMC_ENABLED                              (1)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eAmcType_SRAM = 0,
    eAmcType_NOR = 0,
} AmcType_e;

typedef enum
{
    eAmcSpace_RAM0 = 0,
    eAmcSpace_RAM1,
    eAmcSpace_RAM2,
    eAmcSpace_RAM3,
    eAmcSpace_ROM0,
    eAmcSpace_ROM1,
    eAmcSpace_ROM2,
    eAmcSpace_ROM3,
    eAmcSpace_IO0,
    eAmcSpace_IO1,
    eAmcSpace_IO2,
    eAmcSpace_IO3,
} AmcSpace_e;

typedef enum
{
    eAmcMemoryWidth_8 = 0x0,
    eAmcMemoryWidth_16 = 0x1,
    eAmcMemoryWidth_32 = 0x2,
} AmcMemoryWidth_e;

typedef enum
{
    eAmcMemorySize_8KB = 0x0,
    eAmcMemorySize_16KB = 0x1,
    eAmcMemorySize_32KB = 0x2,
    eAmcMemorySize_64KB = 0x3,
    eAmcMemorySize_128KB = 0x4,
    eAmcMemorySize_256KB = 0x5,
    eAmcMemorySize_512KB = 0x6,
    eAmcMemorySize_1MB = 0x7,
    eAmcMemorySize_2MB = 0x8,
    eAmcMemorySize_4MB = 0x9,
    eAmcMemorySize_8MB = 0xa,
    eAmcMemorySize_16MB = 0xb,
    eAmcMemorySize_32MB = 0xc,
    eAmcMemorySize_64MB = 0xd,
} AmcMemorySize_e;

typedef struct
{
    uint32_t status;

    AmcType_e type;
    AmcSpace_e space;

    uint32_t base_addr;

    AmcMemoryWidth_e memory_width;
    AmcMemorySize_e memory_size;

    union
    {
        uint8_t write_en;
        uint8_t ready_en;
    } common;

    union
    {
        struct
        {
            uint32_t h1 : 8;
            uint32_t h2 : 8;
            uint32_t s2 : 8;
            uint32_t s1 : 8;
        } ws;
        uint32_t value;
    } cycles;

    void *user_data;
} DeviceAmcConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceAmcConfig_t g_DeviceAmcDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_AMC_InitConfig(AMC_TypeDef *AMCx, DeviceAmcConfig_t *config);

int HW_AMC_NorFlashReadID(uint32_t base_addr, uint16_t *id);
int HW_AMC_NorFlashErase(uint32_t base_addr, uint32_t offset);
int HW_AMC_NorFlashRead(uint32_t base_addr, uint32_t offset, uint16_t *data);
int HW_AMC_NorFlashProgram(uint32_t base_addr, uint32_t offset, uint16_t data);

// Note:
//  1. only 16-bits nor flash are available
//  2. you must set '.memory_width' in 'DeviceAmcConfig_t' to 'eAmcMemoryWidth_16'

uint32_t NorFlash_Erase(uint32_t addr);
uint32_t NorFlash_Read(uint32_t addr, uint16_t *pbuf, uint32_t len);
uint32_t NorFlash_Write(uint32_t addr, uint16_t *pbuf, uint32_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_AMC_H__ */
