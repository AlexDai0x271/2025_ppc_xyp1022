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
#include "drv_flash.h"

void Flash_Init(void)
{
    HW_QSPI_InitConfig(QSPI0, &g_DeviceQspiDefaultConfig);
}

static void Flash_WriteEnable(QSPI_TypeDef *QSPIx)
{
    HW_QSPI_WriteBlock(QSPIx, eQspiMode_Standard, FLASH_CMD_WRITE_ENABLE, NULL, 0, NULL, NULL, 0);
}

static void Flash_WaitFence(QSPI_TypeDef *QSPIx)
{
    uint8_t ui8Status;

    do {
        HW_QSPI_ReadBlock(QSPIx, eQspiMode_Standard, FLASH_CMD_WINBOND_READ_SR1, NULL, 0, NULL, &ui8Status, 1);
    } while (GET_BIT(ui8Status, 0));
}

uint32_t Flash_ReadID(void)
{
    uint8_t aui8FlashId[3];

    HW_QSPI_ReadBlock(QSPI0, eQspiMode_Standard, FLASH_CMD_READ_ID, NULL, 0, NULL, aui8FlashId, ARRAY_SIZE(aui8FlashId));

    return (aui8FlashId[0] << 16) | (aui8FlashId[1] << 8) | aui8FlashId[2];
}

int Flash_ReadData(uint32_t addr, void *rxdata, uint32_t length)
{
    if (((addr + length) > FLASH_SIZE) || (rxdata == NULL) || (length == 0)) {
        return -1;
    }

    HW_QSPI_ReadBlock(QSPI0, eQspiMode_QuadRx, FLASH_CMD_READ_114, &addr, 3, NULL, rxdata, length);

    return 0;
}

int Flash_ProgramData(uint32_t addr, const void *txdata, uint32_t length)
{
    uint32_t block_length = 0;
    uint32_t offset = 0;

    if (((addr + length) > FLASH_SIZE) || (txdata == NULL) || (length == 0)) {
        return -1;
    }

    while (length) {
        block_length = FLASH_PAGE_SIZE - (addr & (FLASH_PAGE_SIZE - 1));
        block_length = (length > block_length) ? block_length : length;

        Flash_WriteEnable(QSPI0);

        HW_QSPI_WriteBlock(QSPI0, eQspiMode_QuadTx, FLASH_CMD_PAGE_PROGRAM_114, &addr, 3, ((uint8_t *)txdata + offset), NULL, block_length);
        Flash_WaitFence(QSPI0);

        addr += block_length;
        offset += block_length;
        length -= block_length;
    }

    return 0;
}

int Flash_EraseSector(uint32_t addr)
{
    Flash_WriteEnable(QSPI0);

    HW_QSPI_WriteBlock(QSPI0, eQspiMode_Standard, FLASH_CMD_SECTOR_ERASE, &addr, 3, NULL, NULL, 0);
    Flash_WaitFence(QSPI0);

    return 0;
}

int Flash_EraseChip(void)
{
    Flash_WriteEnable(QSPI0);

    HW_QSPI_WriteBlock(QSPI0, eQspiMode_Standard, FLASH_CMD_CHIP_ERASE, NULL, 0, NULL, NULL, 0);
    Flash_WaitFence(QSPI0);

    return 0;
}
