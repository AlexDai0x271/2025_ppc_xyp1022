/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_amc.h"
#include "sysclock.h"

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceAmcConfig_t g_DeviceAmcDefaultConfig =
    {
        .type = eAmcType_NOR,
        .space = eAmcSpace_ROM0, // NOTE: the hardware provided only supports 'eAmcSpace_RAM0/AMC_RWMEM_BASE'

        .base_addr = AMC_ROMEM_BASE, // NOTE: the hardware provided only supports 'eAmcSpace_RAM0/AMC_RWMEM_BASE'

        .memory_width = eAmcMemoryWidth_16,
        .memory_size = eAmcMemorySize_64MB,

        .common.write_en = 1,

        .cycles.value = 0x08080808,

        .user_data = 0,
};
#endif // #ifdef DEBUG

#define AMC_MEM_UC16(addr) \
    *((__IO uint16_t *)(base_addr + (addr)))

#define __byte_to_halfword_(addr) \
    ((addr) << 1)

int HW_AMC_InitConfig(AMC_TypeDef *AMCx, DeviceAmcConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == AMCx) || (NULL == config)) {
        return -1;
    }

    switch (config->space) {
    case eAmcSpace_RAM0:
    case eAmcSpace_RAM1:
    case eAmcSpace_RAM2:
    case eAmcSpace_RAM3:
        reg_value = 0; // .WRMEM_CONFIG inited to zero first
        SET_BIT(reg_value, AMC_WRMEM_CONFIG_RMW_POS, AMC_DISABLED);
        reg_value |= (config->memory_width << AMC_WRMEM_CONFIG_WT_POS);
        reg_value |= (config->memory_size << AMC_WRMEM_CONFIG_BS_POS);
        reg_value |= (0x80 << AMC_WRMEM_CONFIG_WS_POS); // NOTE: using reset value
        AMCx->WRMEM_CONFIG = reg_value;
        AMCx->WR_WS_CFG_REG = config->cycles.value;
        break;

    case eAmcSpace_ROM0:
    case eAmcSpace_ROM1:
    case eAmcSpace_ROM2:
    case eAmcSpace_ROM3:
        reg_value = 0; // .ORMEM_CONFIG inited to zero first
        SET_BIT(reg_value, AMC_ORMEM_CONFIG_WEN_POS, config->common.write_en);
        SET_BIT(reg_value, AMC_ORMEM_CONFIG_RMW_POS, AMC_DISABLED);
        reg_value |= (config->memory_width << AMC_ORMEM_CONFIG_WT_POS);
        reg_value |= (config->memory_size << AMC_ORMEM_CONFIG_BS_POS);
        reg_value |= (0x80 << AMC_ORMEM_CONFIG_WS_POS); // NOTE: using reset value
        AMCx->ORMEM_CONFIG = reg_value;
        AMCx->OR_WS_CFG_REG = config->cycles.value;
        break;

    case eAmcSpace_IO0:
    case eAmcSpace_IO1:
    case eAmcSpace_IO2:
    case eAmcSpace_IO3:
        reg_value = 0; // .IO_CONFIG inited to zero first
        SET_BIT(reg_value, AMC_IO_CONFIG_RDY_POS, config->common.ready_en);
        reg_value |= (config->memory_width << AMC_IO_CONFIG_WT_POS);
        reg_value |= (0x80 << AMC_IO_CONFIG_WS_POS); // NOTE: using reset value
        AMCx->IO_CONFIG = reg_value;
        AMCx->IO_WS_CFG_REG = config->cycles.value;
        break;

    default:
        break;
    }

    AMCx->EDAC_CONFIG = 0;

    config->status = 0;
    return 0;
}

int HW_AMC_NorFlashReadID(uint32_t base_addr, uint16_t *id)
{
    AMC_MEM_UC16(__byte_to_halfword_(0x555)) = 0x00AA;
    AMC_MEM_UC16(__byte_to_halfword_(0x2AA)) = 0x0055;
    AMC_MEM_UC16(__byte_to_halfword_(0x555)) = 0x0090;

    id[0] = AMC_MEM_UC16(__byte_to_halfword_(0x0));
    id[1] = AMC_MEM_UC16(__byte_to_halfword_(0x1));

    AMC_MEM_UC16(__byte_to_halfword_(0x000)) = 0x00F0;

    return 0;
}

int HW_AMC_NorFlashUnlock(uint32_t base_addr, uint32_t offset)
{
    return 0;
}

int HW_AMC_NorFlashErase(uint32_t base_addr, uint32_t offset)
{
    AMC_MEM_UC16(__byte_to_halfword_(0x555)) = 0x00AA;
    AMC_MEM_UC16(__byte_to_halfword_(0x2AA)) = 0x0055;
    AMC_MEM_UC16(__byte_to_halfword_(0x555)) = 0x0080;

    AMC_MEM_UC16(__byte_to_halfword_(0x555)) = 0x00AA;
    AMC_MEM_UC16(__byte_to_halfword_(0x2AA)) = 0x0055;
    AMC_MEM_UC16(offset) = 0x0030;

    while ((AMC_MEM_UC16(offset) & 0x0020) != 0x0020) {
        if (AMC_MEM_UC16(0) == AMC_MEM_UC16(0)) {
            break;
        }
    }

    return 0;
}

int HW_AMC_NorFlashRead(uint32_t base_addr, uint32_t offset, uint16_t *data)
{
    if (data == NULL) {
        return -1;
    }

    data[0] = AMC_MEM_UC16(offset);

    return 0;
}

int HW_AMC_NorFlashProgram(uint32_t base_addr, uint32_t offset, uint16_t data)
{
    AMC_MEM_UC16(__byte_to_halfword_(0x555)) = 0x00AA;
    AMC_MEM_UC16(__byte_to_halfword_(0x2AA)) = 0x0055;
    AMC_MEM_UC16(__byte_to_halfword_(0x555)) = 0x00A0;

    AMC_MEM_UC16(offset) = data;

    while ((AMC_MEM_UC16(offset) & 0x0020) != 0x0020) {
        if (AMC_MEM_UC16(0) == AMC_MEM_UC16(0)) {
            break;
        }
    }

    return 0;
}

uint32_t NorFlash_Erase(uint32_t addr)
{
    DeviceAmcConfig_t *config_info = &g_DeviceAmcDefaultConfig;

    HW_AMC_NorFlashErase(config_info->base_addr, addr);

    return addr;
}

uint32_t NorFlash_Read(uint32_t addr, uint16_t *pbuf, uint32_t len)
{
    DeviceAmcConfig_t *config_info = &g_DeviceAmcDefaultConfig;

    for (uint32_t i = 0; i < len; i++) {
        HW_AMC_NorFlashRead(config_info->base_addr, addr + __byte_to_halfword_(i), &pbuf[i]);
    }

    return len;
}

uint32_t NorFlash_Write(uint32_t addr, uint16_t *pbuf, uint32_t len)
{
    DeviceAmcConfig_t *config_info = &g_DeviceAmcDefaultConfig;

    for (uint32_t i = 0; i < len; i++) {
        HW_AMC_NorFlashProgram(config_info->base_addr, addr + __byte_to_halfword_(i), pbuf[i]);
    }

    return len;
}
