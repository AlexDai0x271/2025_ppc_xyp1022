/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_smc.h"
#include "sysclock.h"

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceSmcConfig_t g_DeviceSmcDefaultConfig =
    {
        .type = eSmcType_NAND,

        .bank = eSmcBank_4,
        .bank_addr = 0x0, // NOTE: need bind to actual address

        .memory_width = eSmcMemoryWidth_8,
        .refresh_period = 0,

        .cycles.value = 0xffffffff,

#ifdef DEBUG_SMC_EDAC
        .edac = 1,
#else
        .edac = 0,
#endif

        .user_data = 0,
};
#endif // #ifdef DEBUG

typedef enum
{
    eNandCommand_ReadData = 0,     // 0
    eNandCommand_RandomDataOutput, // 1
    eNandCommand_ReadID,           // 2
    eNandCommand_ReadStatus,       // 3
    eNandCommand_PageProgram,      // 4
    eNandCommand_RandomDataInput,  // 5
    eNandCommand_EraseBlock,       // 6
    eNandCommand_Reset,            // 7
    eNandCommand_None,             // 8

    eNandCommand_Max,
} NandCommand_e;

typedef struct
{
    uint8_t start_cmd;
    uint8_t end_cmd;
    uint8_t addr_cycles;
    uint8_t end_cmd_valid1;
    uint8_t end_cmd_valid2;
} NandCommandInfo_t;

static NandCommandInfo_t _s_NandCommandInfoList[eNandCommand_Max] =
    {
        {NAND_CMD_READ0,  NAND_CMD_READSTART,   5, NAND_END_COMMAND_VALID,   NAND_END_COMMAND_INVALID}, // 0
        {NAND_CMD_RNDOUT, NAND_CMD_RNDOUTSTART, 2, NAND_END_COMMAND_VALID,   NAND_END_COMMAND_INVALID}, // 1
        {NAND_CMD_READID, NAND_CMD_NONE,        1, NAND_END_COMMAND_INVALID, NAND_END_COMMAND_INVALID}, // 2
        {NAND_CMD_STATUS, NAND_CMD_NONE,        0, NAND_END_COMMAND_INVALID, NAND_END_COMMAND_INVALID}, // 3
        {NAND_CMD_SEQIN,  NAND_CMD_PAGEPROG,    5, NAND_END_COMMAND_INVALID, NAND_END_COMMAND_VALID  }, // 4
        {NAND_CMD_RNDIN,  NAND_CMD_NONE,        2, NAND_END_COMMAND_INVALID, NAND_END_COMMAND_INVALID}, // 5
        {NAND_CMD_ERASE1, NAND_CMD_ERASE2,      3, NAND_END_COMMAND_VALID,   NAND_END_COMMAND_INVALID}, // 6
        {NAND_CMD_RESET,  NAND_CMD_READ0,       0, NAND_END_COMMAND_VALID,   NAND_END_COMMAND_INVALID}, // 7
        {NAND_CMD_NONE,   NAND_CMD_NONE,        0, NAND_END_COMMAND_INVALID, NAND_END_COMMAND_INVALID}, // 8
};

typedef struct
{
    union
    {
        uint8_t data8;
        uint16_t data16;
        uint32_t data32;
    };
} NandPhase_t;

// Note:
//  If some commands do not have corresponding phase, pointing to these default phase
//  You don't have to do this, if you know clearly
//  So, don't modify these default values, they are only used for exception debugging

static uint32_t _s_default_cmd_phase = 0xdeadbeef;
static uint32_t _s_default_data_phase = 0xdeadbeef;

int HW_SMC_InitConfig(SMC_TypeDef *SMCx, DeviceSmcConfig_t *config)
{
    uint32_t reg_value;
    uint32_t chip_interface;
    uint32_t chip_bank;

    if ((NULL == SMCx) || (NULL == config)) {
        return -1;
    }

    config->bank_addr = GET_SMC_MEM_BANK_BASE_ADDR(config->bank);

    chip_interface = GET_INTERFACE_INDEX(config->bank);
    chip_bank = GET_INTERFACE_BANK(config->bank);

    reg_value = SMCx->MEMIF_CFG;
    UNUSED(reg_value);

    reg_value = SMCx->MEMC_STATUS;
    UNUSED(reg_value);

    SMCx->SET_CYCLES = config->cycles.value;

    reg_value = 0; // .SET_OPMODE inited to zero first
    if (eSmcType_NAND == config->type) {
        reg_value |= (config->memory_width << SMC_SET_OPMODE_SET_MW_POS);
    }
    SMCx->SET_OPMODE = reg_value;

    reg_value = 0; // .REFRESH_PERIOD inited to zero first
    reg_value = ((config->refresh_period & 0xf) << SMC_REFRESH_PERIOD_PERIOD_POS);
    SMCx->REFRESH_PERIOD[chip_interface] = reg_value;

    reg_value = 0; // .DIRECT_CMD inited to zero first
    SET_BIT(reg_value, SMC_DIRECT_CMD_CHIP_INTERFACE_SELECT_POS, chip_interface);
    reg_value |= (chip_bank << SMC_DIRECT_CMD_CHIP_BANK_SELECT_POS);
    reg_value |= (CMD_TYPE_UPDATEREG << SMC_DIRECT_CMD_CMD_TYPE_POS);
    SET_BIT(reg_value, SMC_DIRECT_CMD_SET_CRE_POS, CRE_LOW);
    SMCx->DIRECT_CMD = reg_value;

    reg_value = SMCx->bank[chip_interface][chip_bank].NAND_CYCLES;
    UNUSED(reg_value);

    reg_value = SMCx->bank[chip_interface][chip_bank].OPMODE;
    UNUSED(reg_value);

    if (config->edac) {
        reg_value = 0; // .MEMC_CFG_SET inited to zero first
        // reg_value |= (1 << (5 + chip_interface));
        // reg_value |= (1 << (0 + chip_interface));
        SMCx->MEMC_CFG_SET = reg_value;
    }

    config->status = 0;
    return 0;
}

uint32_t HW_SMC_GetStatus(SMC_TypeDef *SMCx)
{
    return SMCx->MEMC_STATUS;
}

static NandCommandInfo_t *GetNandCommandInfo(NandCommand_e cmd)
{
    if (cmd >= eNandCommand_Max) {
        return &_s_NandCommandInfoList[eNandCommand_None];
    } else {
        return &_s_NandCommandInfoList[cmd];
    }
}

static int GetNandPhaseAddr(uint32_t base_addr, NandCommand_e cmd, uint32_t *cmd_phase, uint32_t *data_phase)
{
    NandCommandInfo_t *cmd_info = GetNandCommandInfo(cmd);
    uint32_t cmd_phase_addr = 0;
    uint32_t data_phase_addr = 0;

    if ((cmd_phase == NULL) || (data_phase == NULL)) {
        return -1;
    }

    cmd_phase_addr = CMD_PHASE_ADDR(
        0, // addr
        cmd_info->addr_cycles,
        cmd_info->start_cmd,
        cmd_info->end_cmd_valid1,
        cmd_info->end_cmd);

    data_phase_addr = DATA_PHASE_ADDR(
        0, // addr
        1, // last, NOTE: set to '1' first, modify when using it
        0, // ecc_last, NOTE: set to '0' first, modify when using it
        cmd_info->end_cmd_valid2,
        cmd_info->end_cmd);

    *cmd_phase = (base_addr + cmd_phase_addr);
    *data_phase = (base_addr + data_phase_addr);

    return 0;
}

static void WaitTicks(__IO uint32_t ticks)
{
    ticks <<= 4;
    while (ticks--) {
        __asm volatile("nop");
    }
}

int HW_SMC_NandFlashReadID(uint32_t base_addr, uint8_t *id, uint32_t len)
{
    NandPhase_t *cmd_phase = (NandPhase_t *)&_s_default_cmd_phase;
    NandPhase_t *data_phase = (NandPhase_t *)&_s_default_data_phase;

    if (0 != GetNandPhaseAddr(base_addr, eNandCommand_ReadID, (uint32_t *)&cmd_phase, (uint32_t *)&data_phase)) {
        return -1;
    }

    memset(id, 0, len);

    cmd_phase->data8 = 0x00;
    WaitTicks(200);

    for (uint32_t i = 0; i < len; i++) {
        id[i] = data_phase->data8;
    }

    return 0;
}

int HW_SMC_NandFlashReset(uint32_t base_addr)
{
    NandPhase_t *cmd_phase = (NandPhase_t *)&_s_default_cmd_phase;
    NandPhase_t *data_phase = (NandPhase_t *)&_s_default_data_phase;

    if (0 != GetNandPhaseAddr(base_addr, eNandCommand_Reset, (uint32_t *)&cmd_phase, (uint32_t *)&data_phase)) {
        return -1;
    }

    cmd_phase->data8 = 0x00;
    WaitTicks(200);

    UNUSED(data_phase);

    return 0;
}

int HW_SMC_NandFlashErase(uint32_t base_addr, uint32_t row_addr)
{
    NandPhase_t *cmd_phase = (NandPhase_t *)&_s_default_cmd_phase;
    NandPhase_t *data_phase = (NandPhase_t *)&_s_default_data_phase;

    if (0 != GetNandPhaseAddr(base_addr, eNandCommand_EraseBlock, (uint32_t *)&cmd_phase, (uint32_t *)&data_phase)) {
        return -1;
    }

    cmd_phase->data32 = (row_addr & 0xffffff);
    WaitTicks(200);

    GetNandPhaseAddr(base_addr, eNandCommand_ReadStatus, (uint32_t *)&cmd_phase, (uint32_t *)&data_phase);
    while (!GET_BIT(data_phase->data8, 1))
        ;

    UNUSED(data_phase);

    return 0;
}

int HW_SMC_NandFlashRead(uint32_t base_addr, uint32_t col_addr, uint32_t row_addr, uint8_t *pbuf, uint32_t len)
{
    NandPhase_t *cmd_phase = (NandPhase_t *)&_s_default_cmd_phase;
    NandPhase_t *data_phase = (NandPhase_t *)&_s_default_data_phase;

    if (0 != GetNandPhaseAddr(base_addr, eNandCommand_ReadData, (uint32_t *)&cmd_phase, (uint32_t *)&data_phase)) {
        return -1;
    }

    cmd_phase->data32 = (col_addr >> 8);
    cmd_phase->data32 = (col_addr << 24) | (row_addr & 0xffffff);
    WaitTicks(200);

    for (uint32_t i = 0; i < len; i++) {
#ifdef DEBUG_SMC_EDAC
        if (i == (len - 1)) {
            data_phase = (NandPhase_t *)DATA_PHASE_ADDR_MODIFY(data_phase, (i == (len - 1)), 1);
        } else {
#endif
            data_phase = (NandPhase_t *)DATA_PHASE_ADDR_MODIFY(data_phase, (i == (len - 1)), 0);
#ifdef DEBUG_SMC_EDAC
        }
#endif
        pbuf[i] = data_phase->data8;
    }

    return 0;
}

int HW_SMC_NandFlashProgram(uint32_t base_addr, uint32_t col_addr, uint32_t row_addr, uint8_t *pbuf, uint32_t len)
{
    NandPhase_t *cmd_phase = (NandPhase_t *)&_s_default_cmd_phase;
    NandPhase_t *data_phase = (NandPhase_t *)&_s_default_data_phase;

    if (0 != GetNandPhaseAddr(base_addr, eNandCommand_PageProgram, (uint32_t *)&cmd_phase, (uint32_t *)&data_phase)) {
        return -1;
    }

    cmd_phase->data32 = (col_addr >> 8);
    cmd_phase->data32 = (col_addr << 24) | (row_addr & 0xffffff);
    WaitTicks(200);

    for (uint32_t i = 0; i < len; i++) {
#ifdef DEBUG_SMC_EDAC
        if (i == (len - 1)) {
            data_phase = (NandPhase_t *)DATA_PHASE_ADDR_MODIFY(data_phase, (i == (len - 1)), 1);
        } else {
#endif
            data_phase = (NandPhase_t *)DATA_PHASE_ADDR_MODIFY(data_phase, (i == (len - 1)), 0);
#ifdef DEBUG_SMC_EDAC
        }
#endif
        data_phase->data8 = pbuf[i];
    }

    GetNandPhaseAddr(base_addr, eNandCommand_ReadStatus, (uint32_t *)&cmd_phase, (uint32_t *)&data_phase);
    while (!GET_BIT(data_phase->data8, 1))
        ;

    return 0;
}

uint32_t NandFlash_Erase(NandFlashChipInfo_t *chip_info, uint32_t addr)
{
    DeviceSmcConfig_t *config_info = &g_DeviceSmcDefaultConfig;

    uint32_t row = addr / chip_info->block_size;

    HW_SMC_NandFlashErase(config_info->bank_addr, row);

    return addr;
}

uint32_t NandFlash_Read(NandFlashChipInfo_t *chip_info, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    DeviceSmcConfig_t *config_info = &g_DeviceSmcDefaultConfig;

    uint32_t row = addr / chip_info->block_size;
    uint32_t col = addr % chip_info->block_size;

    HW_SMC_NandFlashRead(config_info->bank_addr, col, row, pbuf, len);

    return len;
}

uint32_t NandFlash_Write(NandFlashChipInfo_t *chip_info, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    DeviceSmcConfig_t *config_info = &g_DeviceSmcDefaultConfig;

    uint32_t row = addr / chip_info->block_size;
    uint32_t col = addr % chip_info->block_size;

    HW_SMC_NandFlashProgram(config_info->bank_addr, col, row, pbuf, len);

    return len;
}
