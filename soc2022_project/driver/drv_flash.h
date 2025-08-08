/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

#define FLASH_SIZE                          (16 * 1024 * 1024)
#define FLASH_BLOCK_SIZE                    (64 * 1024)
#define FLASH_SECTOR_SIZE                   (4 * 1024)
#define FLASH_PAGE_SIZE                     (256)
#define FLASH_BLOCK_COUNT                   (FLASH_SIZE / FLASH_BLOCK_SIZE)
#define FLASH_SECTOR_COUNT                  (FLASH_SIZE / FLASH_SECTOR_SIZE)

// Note:
//  1. The following defines only generic commands

#define FLASH_CMD_NOP                       (0x00U)

#define FLASH_CMD_READ_SFDP                 (0x5AU)
#define FLASH_CMD_READ_ID                   (0x9FU)

#define FLASH_CMD_READ                      (0x03U) // 1-1-1，0 dummy cycle
#define FLASH_CMD_FAST_READ                 (0x0BU) // 1-1-1，8 dummy cycle
#define FLASH_CMD_READ_122                  (0xBBU) // 1-2-2，4 dummy cycle
#define FLASH_CMD_READ_114                  (0x6BU) // 1-1-4，8 dummy cycle
#define FLASH_CMD_READ_144                  (0xEBU) // 1-4-4，6 dummy cycle

#define FLASH_CMD_PAGE_PROGRAM              (0x02U)
#define FLASH_CMD_PAGE_PROGRAM_114          (0x32U)

#define FLASH_CMD_SECTOR_ERASE              (0x20U)
#define FLASH_CMD_BLOCK_ERASE_32KB          (0x52U)
#define FLASH_CMD_BLOCK_ERASE_64KB          (0xD8U)
#define FLASH_CMD_CHIP_ERASE                (0x60U)

#define FLASH_CMD_WRITE_ENABLE              (0x06U)
#define FLASH_CMD_WRITE_DISABLE             (0x04U)

#define FLASH_CMD_SUSPENDS_PROGRAM_ERASE    (0x75U)
#define FLASH_CMD_RESUMES_PROGRAM_ERASE     (0x7AU)

#define FLASH_CMD_RESET_ENABLE              (0x66U)
#define FLASH_CMD_RESET_DEVICE              (0x99U)

#define FLASH_CMD_GANG_BLOCK_LOCK           (0x7EU)
#define FLASH_CMD_GANG_BLOCK_UNLOCK         (0x98U)

// Note:
//  1. The following defines only unique commands
//  2. Commands unique to different models are defined after 'ReadID'

/* Winbond */

#define FLASH_CMD_WINBOND_VOLATILE_SR_WREN  (0x50U)

#define FLASH_CMD_WINBOND_READ_SR1          (0x05U)
#define FLASH_CMD_WINBOND_WRITE_SR1         (0x01U)
#define FLASH_CMD_WINBOND_READ_SR2          (0x35U)
#define FLASH_CMD_WINBOND_WRITE_SR2         (0x31U)
#define FLASH_CMD_WINBOND_READ_SR3          (0x15U)
#define FLASH_CMD_WINBOND_WRITE_SR3         (0x11U)

#define FLASH_CMD_WINBOND_INDIVIDUAL_LOCK   (0x36U)
#define FLASH_CMD_WINBOND_INDIVIDUAL_UNLOCK (0x39U)

/**
 * @brief Function Define
 */

void Flash_Init(void);
uint32_t Flash_ReadID(void);
int Flash_ReadData(uint32_t addr, void *rxdata, uint32_t length);
int Flash_ProgramData(uint32_t addr, const void *txdata, uint32_t length);
int Flash_EraseSector(uint32_t addr);
int Flash_EraseChip(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_FLASH_H__ */
