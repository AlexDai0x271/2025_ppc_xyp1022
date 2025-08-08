/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "xshell.h"
#include "xprintf.h"
#include "sysclock.h"
#include "drv_common.h"

#ifndef TRUE
#define TRUE ((uint8_t)(1 == 1))
#endif

#ifndef FALSE
#define FALSE ((uint8_t)(0 == 1))
#endif

#define SHELL_UNKNOWN_OPERATION(op) xprintf("Unknown operation [%s]!\n", (op));

static uint8_t m_aui8DummyBuffer[FLASH_PAGE_SIZE];

static uint8_t
_ShellCommandMatchOps(
    char szParam,
    const char *apszList)
{
    for (int i = 0; i < strlen(apszList); i++) {
        if (szParam == apszList[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static uint8_t
_ShellCommandMatchName(
    char *szName,
    const char *szPattern)
{
    return (strncmp(szName, szPattern, strlen(szName)) == 0);
}

static uint8_t
_ShellCommandCheckAligned(
    char szOp,
    size_t uinAddress)
{
    UINT8 ui8Mask;

    switch (szOp) {
    case 'b':
        ui8Mask = 0x0;
        break;
    case 'h':
        ui8Mask = 0x1;
        break;
    case 'w':
        ui8Mask = 0x3;
        break;
    default:
        ui8Mask = 0x3;
        break;
    }

    return ((uinAddress & ui8Mask) ? FALSE : TRUE);
}

static char *
_ShellCommandGetEchoInfo(
    char szReadWrite,
    char szOp)
{
#define COMMAND_READ_U8_ECHO_INFO   "address[%08x] read: %02x\n"
#define COMMAND_READ_U16_ECHO_INFO  "address[%08x] read: %04x\n"
#define COMMAND_READ_U32_ECHO_INFO  "address[%08x] read: %08x\n"

#define COMMAND_WRITE_U8_ECHO_INFO  "address[%08x] write: %02x, read-back: %02x\n"
#define COMMAND_WRITE_U16_ECHO_INFO "address[%08x] write: %04x, read-back: %04x\n"
#define COMMAND_WRITE_U32_ECHO_INFO "address[%08x] write: %08x, read-back: %08x\n"

    char *pazEchoInfo;

    switch (szOp) {
    case 'b':
        pazEchoInfo = (szReadWrite == 'r') ? COMMAND_READ_U8_ECHO_INFO : COMMAND_WRITE_U8_ECHO_INFO;
        break;
    case 'h':
        pazEchoInfo = (szReadWrite == 'r') ? COMMAND_READ_U16_ECHO_INFO : COMMAND_WRITE_U16_ECHO_INFO;
        break;
    case 'w':
        pazEchoInfo = (szReadWrite == 'r') ? COMMAND_READ_U32_ECHO_INFO : COMMAND_WRITE_U32_ECHO_INFO;
        break;
    default:
        pazEchoInfo = (szReadWrite == 'r') ? COMMAND_READ_U32_ECHO_INFO : COMMAND_WRITE_U32_ECHO_INFO;
        break;
    }

    return pazEchoInfo;
}

static void
_ShellCommandReadMemory(
    char szOp,
    size_t uinAddress)
{
    char *pazEchoInfo = _ShellCommandGetEchoInfo('r', szOp);

    switch (szOp) {
    case 'b':
        xprintf(pazEchoInfo, uinAddress, REG_READ_8(uinAddress));
        break;
    case 'h':
        xprintf(pazEchoInfo, uinAddress, REG_READ_16(uinAddress));
        break;
    case 'w':
        xprintf(pazEchoInfo, uinAddress, REG_READ_32(uinAddress));
        break;
    default:
        break;
    }
}

static void
_ShellCommandWriteMemory(
    char szOp,
    size_t uinAddress,
    size_t uinData)
{
    char *pazEchoInfo = _ShellCommandGetEchoInfo('w', szOp);

    switch (szOp) {
    case 'b':
        REG_WRITE_8(uinAddress, uinData);
        xprintf(pazEchoInfo, uinAddress, uinData, REG_READ_8(uinAddress));
        break;
    case 'h':
        REG_WRITE_16(uinAddress, uinData);
        xprintf(pazEchoInfo, uinAddress, uinData, REG_READ_16(uinAddress));
        break;
    case 'w':
        REG_WRITE_32(uinAddress, uinData);
        xprintf(pazEchoInfo, uinAddress, uinData, REG_READ_32(uinAddress));
        break;
    default:
        break;
    }
}

static int32_t
_ShellCommandDumpMemory(
    size_t uinAddressForDisplay,
    size_t uinAddress,
    size_t uinLength)
{
    size_t uinStart = (uinAddressForDisplay / 16) * 16;
    size_t uinEnd = ((uinAddressForDisplay + uinLength + 15) / 16) * 16;

    xprintf("        "); // NOTE: 8-block
    for (int i = 0; i < 16; i++) {
        xprintf("%c%02x", (((i % 16) == 8) ? '-' : ' '), i);
    }
    xprintf("\n");

    uint8_t aui8Data[16] = {0};
    uint8_t aui8Mask[16] = {0};

    for (size_t i = uinStart; i < uinEnd; i++) {
        if ((i % 16) == 0) {
            xprintf("%08x", i);
        }

        if ((i >= uinAddressForDisplay) && (i < (uinAddressForDisplay + uinLength))) {
            aui8Data[i % 16] = REG_READ_8(uinAddress++);
            aui8Mask[i % 16] = 1;
            xprintf("%c%02x", (((i % 16) == 8) ? '-' : ' '), aui8Data[i % 16]);
        } else {
            aui8Data[i % 16] = 0;
            aui8Mask[i % 16] = 0;
            xprintf("   ");
        }

        if ((i % 16) == 15) {
            xprintf(" ");
            for (int i = 0; i < 16; i++) {
                if (aui8Mask[i] == 1) {
                    xprintf("%c", (isprint(aui8Data[i]) ? aui8Data[i] : '.'));
                } else {
                    xprintf(" ");
                }
            }
            xprintf("\n");
        }
    }

    (void)aui8Data;
    (void)aui8Mask;

    return 0;
}

int ShellCommand_Help(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    for (int i = 0; i < gui32ShellCommandsNum; i++) {
        xprintf("%-12s- %s\n", gsShellCommands[i].pszCommand, gsShellCommands[i].pszHelpStr);
    }

    return 0;
}

int ShellCommand_Clear(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    xprintf("\033[2J\033[0;0H");

    return 0;
}

int ShellCommand_Reg(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 3) {
        xprintf("Unknown command parameter!\n"
                "Usage: reg [op] [address] [data]\n"
                "Where: op = r[b|h|w] | w[b|h|w]\n"
                "       address and data is in hex\n"
                "       reg r[b|h|w] address\n"
                "       reg w[b|h|w] address data\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    size_t uinAddress = strtoul(apszArgv[2], NULL, 16);
    size_t uinData = (ui8Argc > 3) ? strtoul(apszArgv[3], NULL, 16) : 0;

    if (strlen(pszOp) < 2) {
        SHELL_UNKNOWN_OPERATION(pszOp);
        return 0;
    } else if (_ShellCommandMatchOps(pszOp[0], "rw")) {
        if (!_ShellCommandCheckAligned(pszOp[1], uinAddress)) {
            xprintf("Un-aligned address!\n");
            return 0;
        }
    } else {
        SHELL_UNKNOWN_OPERATION(pszOp);
        ;
        return 0;
    }

    switch (pszOp[0]) {
    case 'r':
        _ShellCommandReadMemory(pszOp[1], uinAddress);
        break;
    case 'w':
        _ShellCommandWriteMemory(pszOp[1], uinAddress, uinData);
        break;
    default:
        break;
    }

    return 0;
}

int ShellCommand_Mem(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 4) {
        xprintf("Unknown command parameter!\n"
                "Usage: mem [op] [address] [length | data]\n"
                "Where: op = r | w[b|h|w]\n"
                "       address/length/data is in hex\n"
                "       mem r address length\n"
                "       mem w[b|h|w] address data\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    size_t uinAddress = strtoul(apszArgv[2], NULL, 16);
    size_t uinLengthOrData = strtoul(apszArgv[3], NULL, 16);

    if (pszOp[0] == 'r') {
        // Need to do nothing!
    } else if ((pszOp[0] == 'w') && (strlen(pszOp) >= 2)) {
        if (!_ShellCommandCheckAligned(pszOp[1], uinAddress)) {
            xprintf("Un-aligned address!\n");
            return 0;
        }
    } else {
        SHELL_UNKNOWN_OPERATION(pszOp);
        return 0;
    }

    switch (pszOp[0]) {
    case 'r':
        _ShellCommandDumpMemory(uinAddress, uinAddress, uinLengthOrData);
        break;
    case 'w':
        _ShellCommandWriteMemory(pszOp[1], uinAddress, uinLengthOrData);
        break;
    default:
        break;
    }

    return 0;
}

int ShellCommand_Nand(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: nand [op] [address]\n"
                "Where: op = e[rase] | w[rite] | r[ead]\n"
                "       address is in hex\n"
                "       nand erase 0\n"
                "       nand write 0\n"
                "       nand read 0\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    size_t uinAddress = (ui8Argc > 2) ? strtoul(apszArgv[2], NULL, 16) : 0;

    SMC_TypeDef *psDevice = SMC0;

    xprintf("SMC PERIPH_ID = %02x%02x%02x%02x\n", psDevice->PERIPH_ID[3], psDevice->PERIPH_ID[2], psDevice->PERIPH_ID[1], psDevice->PERIPH_ID[0]);
    xprintf("SMC PCELL_ID = %02x%02x%02x%02x\n", psDevice->PCELL_ID[3], psDevice->PCELL_ID[2], psDevice->PCELL_ID[1], psDevice->PCELL_ID[0]);

    HW_SMC_InitConfig(psDevice, &g_DeviceSmcDefaultConfig);

    HW_SMC_NandFlashReset(g_DeviceSmcDefaultConfig.bank_addr);
    udelay(500);

    uint8_t aui8FlashId[8];
    HW_SMC_NandFlashReadID(g_DeviceSmcDefaultConfig.bank_addr, aui8FlashId, sizeof(aui8FlashId));
    xprintf("nand flash id: %02x %02x %02x %02x %02x %02x %02x %02x\n",
            aui8FlashId[0],
            aui8FlashId[1],
            aui8FlashId[2],
            aui8FlashId[3],
            aui8FlashId[4],
            aui8FlashId[5],
            aui8FlashId[6],
            aui8FlashId[7]);

    NandFlashChipInfo_t sChipInfo;
    memset(&sChipInfo, 0, sizeof(NandFlashChipInfo_t));

    sChipInfo.capacity = 64;
    sChipInfo.pages = 256;
    sChipInfo.page_size = (16 * 1024);
    sChipInfo.blocks = 2096;
    sChipInfo.block_size = (sChipInfo.pages * sChipInfo.page_size);

    uint32_t ui32TmpLength = MIN(sizeof(m_aui8DummyBuffer) / sizeof(m_aui8DummyBuffer[0]), 256);
    uint32_t ui32TmpAddr = uinAddress * sChipInfo.block_size; // modify here to change the target address
    memset(m_aui8DummyBuffer, 0, ui32TmpLength);

    switch (pszOp[0]) {
    case 'e':
        NandFlash_Erase(&sChipInfo, ui32TmpAddr);
        xprintf("erase done, address %08x.\n", ui32TmpAddr);
        break;
    case 'w':
        for (uint32_t i = 0; i < ui32TmpLength; i++) {
            m_aui8DummyBuffer[i] = i;
        }
        NandFlash_Write(&sChipInfo, ui32TmpAddr, m_aui8DummyBuffer, ui32TmpLength);
        xprintf("write done, address: %08x, size %d.\n", ui32TmpAddr, ui32TmpLength);
    case 'r':
        NandFlash_Read(&sChipInfo, ui32TmpAddr, m_aui8DummyBuffer, ui32TmpLength);
        xprintf("read  done, address: %08x, size %d.\n", ui32TmpAddr, ui32TmpLength);
        for (uint32_t i = 0; i < ui32TmpLength; i++) {
            xprintf("%02x ", m_aui8DummyBuffer[i]);
        }
        xprintf("\n");
        break;
    default:
        SHELL_UNKNOWN_OPERATION(pszOp);
        break;
    }

    return 0;
}

int ShellCommand_Nor(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: nor [op] [address]\n"
                "Where: op = e[rase] | w[rite] | r[ead]\n"
                "       address is in hex\n"
                "       nor erase 0\n"
                "       nor write 0\n"
                "       nor read 0\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    size_t uinAddress = (ui8Argc > 2) ? strtoul(apszArgv[2], NULL, 16) : 0;

    AMC_TypeDef *psDevice = AMC0;

    HW_AMC_InitConfig(psDevice, &g_DeviceAmcDefaultConfig);
    udelay(500);

    uint16_t aui16FlashId[2];
    HW_AMC_NorFlashReadID(g_DeviceAmcDefaultConfig.base_addr, aui16FlashId);
    xprintf("nor flash id: %04x %04x\n", aui16FlashId[0], aui16FlashId[1]);

    uint32_t ui32TmpLength = MIN(sizeof(m_aui8DummyBuffer) / sizeof(m_aui8DummyBuffer[0]), 16);
    uint32_t ui32TmpAddr = uinAddress; // modify here to change the target address
    memset(m_aui8DummyBuffer, 0, ui32TmpLength);

    switch (pszOp[0]) {
    case 'e':
        NorFlash_Erase(ui32TmpAddr);
        xprintf("erase done, address %08x.\n", ui32TmpAddr);
        break;
    case 'w':
        for (uint32_t i = 0; i < (ui32TmpLength * 2); i++) {
            m_aui8DummyBuffer[i] = i;
        }
        NorFlash_Write(ui32TmpAddr, (uint16_t *)m_aui8DummyBuffer, ui32TmpLength);
        xprintf("write done, address: %08x, size %d.\n", ui32TmpAddr, ui32TmpLength);
    case 'r':
        NorFlash_Read(ui32TmpAddr, (uint16_t *)m_aui8DummyBuffer, ui32TmpLength);
        xprintf("read  done, address: %08x, size %d.\n", ui32TmpAddr, ui32TmpLength);
        for (uint32_t i = 0; i < (ui32TmpLength * 2); i++) {
            xprintf("%02x ", m_aui8DummyBuffer[i]);
        }
        xprintf("\n");
        break;
    default:
        SHELL_UNKNOWN_OPERATION(pszOp);
        break;
    }

    return 0;
}

int ShellCommand_Qspi(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: qspi [op] [address] [length | data]\n"
                "Where: op = e[rase] | r[ead] | w[rite]\n"
                "       address/length/data/size is in hex\n"
                "       qspi erase 200\n"
                "       qspi read 200 40\n"
                "       qspi write 200 80\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    size_t uinAddress = (ui8Argc > 2) ? strtoul(apszArgv[2], NULL, 16) : 0;
    size_t uinLengthOrData = (ui8Argc > 3) ? strtoul(apszArgv[3], NULL, 16) : 0x10;

    Flash_Init();
    xprintf("flash id: %06x.\n", Flash_ReadID());

    switch (pszOp[0]) {
    case 'e':
        uinAddress = (uinAddress / FLASH_SECTOR_SIZE) * FLASH_SECTOR_SIZE;
        xprintf("erase sector, address: 0x%08x\n", uinAddress);
        Flash_EraseSector(uinAddress);
        break;
    case 'r':
        while (uinLengthOrData > 0) {
            uint32_t ui32CntInBlock = (uinLengthOrData > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : uinLengthOrData;
            Flash_ReadData(uinAddress, m_aui8DummyBuffer, ui32CntInBlock);
            _ShellCommandDumpMemory(uinAddress, (size_t)m_aui8DummyBuffer, ui32CntInBlock);
            uinAddress += ui32CntInBlock;
            uinLengthOrData -= ui32CntInBlock;
        }
        break;
    case 'w':
        xprintf("address[%08x] write: %08x, ", uinAddress, uinLengthOrData);
        Flash_ProgramData(uinAddress, &uinLengthOrData, 4);
        uinLengthOrData = 0;
        Flash_ReadData(uinAddress, &uinLengthOrData, 4);
        xprintf("read-back: %08x\n", uinLengthOrData);
        break;
    default:
        SHELL_UNKNOWN_OPERATION(pszOp);
        break;
    }

    return 0;
}

int ShellCommand_Spi(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: spi [op] [index]\n"
                "Where: op = p[robe]\n"
                "       index is in dec\n"
                "       spi probe 0\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    size_t uinIndex = (ui8Argc > 2) ? MIN(strtoul(apszArgv[2], NULL, 10), 5) : 0;

    SPI_TypeDef *psDevice = SPI0;

    switch (pszOp[0]) {
    case 'p':
        switch (uinIndex) {
        case 0:
            psDevice = SPI0;
            break;
        case 1:
            psDevice = SPI1;
            break;
        case 2:
            psDevice = SPI2;
            break;
        case 3:
            psDevice = SPI3;
            break;
        case 4:
            psDevice = SPI4;
            break;
        case 5:
            psDevice = SPI5;
            break;
        default:
            break;
        }
        break;
    default:
        SHELL_UNKNOWN_OPERATION(pszOp);
        break;
    }

    uint8_t aui8FlashId[4];
    aui8FlashId[0] = FLASH_CMD_READ_ID;

    HW_SPI_InitConfig(psDevice, &g_DeviceSpiDefaultConfig);
    HW_SPI_ReadWriteBlock(psDevice, aui8FlashId, aui8FlashId, ARRAY_SIZE(aui8FlashId));

    xprintf("flash <%d> id: %06x.\n", uinIndex, ((aui8FlashId[1] << 16) | (aui8FlashId[2] << 8) | aui8FlashId[3]));

    return 0;
}

int ShellCommand_Can(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: can [op] [mode | id]\n"
                "Where: op = i[nit] | s[end] | r[ecv]\n"
                "       mode is in string\n"
                "       id is in dec\n"
                "       can init std\n"
                "       can init ext\n"
                "       can send 1\n"
                "       can recv\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    uint32_t ui32Id = (ui8Argc > 2) ? strtoul(apszArgv[2], NULL, 10) : 0;
    char *pszMode = (ui8Argc > 2) ? apszArgv[2] : "std";

    CAN_TypeDef *psDevice = CAN0;
    CanFormat_e sFormat = eCanFormat_Standard;

    static uint8_t m_can_init_flag = 0;

    if (m_can_init_flag == 0) {
        if (pszOp[0] != 'i') {
            xprintf("device not init!\n");
            return 0;
        }
    }

    uint32_t ui32TmpLength = MIN(sizeof(m_aui8DummyBuffer) / sizeof(m_aui8DummyBuffer[0]), 8);

    for (uint32_t i = 0; i < ui32TmpLength; i++) {
        m_aui8DummyBuffer[i] = i % 256;
    }

    switch (pszOp[0]) {
    case 'i':
        if (_ShellCommandMatchName(pszMode, "ext")) {
            g_DeviceCanDefaultConfig.format = eCanFormat_Extend;
            xprintf("format: extend\n");
        } else {
            g_DeviceCanDefaultConfig.format = eCanFormat_Standard;
            xprintf("format: standard\n");
        }
        HW_CAN_InitConfig(psDevice, &g_DeviceCanDefaultConfig);
        xprintf("init done.\n");
        m_can_init_flag = 1;
        break;
    case 's':
        sFormat = g_DeviceCanDefaultConfig.format;
        ui32TmpLength = HW_CAN_WriteBlock(psDevice, sFormat, ui32Id, m_aui8DummyBuffer, ui32TmpLength);
        xprintf("send %d to id %08x (%s).\n", ui32TmpLength, ui32Id, ((sFormat == eCanFormat_Extend) ? "extend" : "standard"));
        for (uint32_t i = 0; i < ui32TmpLength; i++) {
            xprintf("%02x ", m_aui8DummyBuffer[i]);
        }
        xprintf("\n");
        break;
    case 'r':
        HW_CAN_ReadBlock(psDevice, &sFormat, &ui32Id, m_aui8DummyBuffer, ui32TmpLength);
        xprintf("recv %d from id %08x (%s).\n", ui32TmpLength, ui32Id, ((sFormat == eCanFormat_Extend) ? "extend" : "standard"));
        for (uint32_t i = 0; i < ui32TmpLength; i++) {
            xprintf("%02x ", m_aui8DummyBuffer[i]);
        }
        xprintf("\n");
        break;
    default:
        SHELL_UNKNOWN_OPERATION(pszOp);
        break;
    }

    return 0;
}

int ShellCommand_Gmac(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: gmac [op] [count]\n"
                "Where: op = init | send\n"
                "       count is in dec\n"
                "       gmac init\n"
                "       gmac send 32\n");
        return 0;
    }

    size_t uinCount = (ui8Argc > 2) ? strtoul(apszArgv[2], NULL, 10) : 1;

    GMAC_TypeDef *psDevice = g_DeviceGmacDefaultConfig.gmac;

    uint8_t pui8TargetMac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t pui8SourceMac[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};

    if (_ShellCommandMatchName(apszArgv[1], "init")) {
        xprintf("GMAC VERSION = %08x\n", psDevice->Register8);

        memcpy(&g_DeviceGmacDefaultConfig.mac[0], pui8SourceMac, 6);
        g_DeviceGmacDefaultConfig.mac_num = 1;

        HW_GMAC_InitConfig(psDevice, &g_DeviceGmacDefaultConfig);

        xprintf("link speed: %d Mbps\n", PHY_GetLinkSpeed(psDevice, &g_DeviceGmacDefaultConfig));
        xprintf("link duplex(0:link down/1:half/2:full): %d\n", PHY_GetLinkDuplex(psDevice, &g_DeviceGmacDefaultConfig));
    }

#ifdef DEBUG_GMAC
    if (_ShellCommandMatchName(apszArgv[1], "send")) {
        uint8_t pui8Payload[46] = {
            0x00,
            0x01, /* hardware type */
            0x08,
            0x00, /* protocol type */
            0x06,
            0x04,
            0x00,
            0x01, /* op code */
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00, /* source mac */
            10,
            211,
            102,
            127, /* source ip */
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00, /* target mac */
            10,
            211,
            102,
            118, /* target ip */
        };

        memcpy(&pui8Payload[8], pui8SourceMac, 6);

        uint8_t ui8Packet[GMAC_BUFFER_SIZE];
        uint32_t ui32Length;

        while (uinCount--) {
            ui32Length = HW_GMAC_GenPacket(ui8Packet, pui8TargetMac, pui8SourceMac, 0x0806, pui8Payload, sizeof(pui8Payload));
            xprintf("send a packet.\n");

            xprintf("send %d.\n", ui32Length);
            for (uint32_t i = 0; i < ui32Length; i++) {
                xprintf("%02x ", ui8Packet[i]);
            }
            xprintf("\n");

            int iResult = 0;
            do {
                iResult = HW_GMAC_SendPacket(psDevice, &g_DeviceGmacDefaultConfig, ui8Packet, ui32Length);
                if (iResult < 0) {
                    xprintf("allocate xmit descriptor failed!\n");
                    mdelay(10);
                }
            } while (iResult < 0);
        }
    }
#endif // #ifdef DEBUG_GMAC

    return 0;
}

int ShellCommand_Phy(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    uint32_t ui32RegValue;

    size_t uinPhyId = (ui8Argc > 2) ? strtoul(apszArgv[1], NULL, 10) : g_DeviceGmacDefaultConfig.phy_id;

    xprintf("phy id: %d\n", uinPhyId);

    for (int i = 0; i < 30; i++) {
        ui32RegValue = PHY_Read(GMAC0, uinPhyId, i, 10 * 1000);
        xprintf("reg[%02d] = %04x ", i, ui32RegValue);
        if ((i % 4) == 3) {
            xprintf("\n");
        }
    }

    xprintf("\n");

    return 0;
}

int ShellCommand_Link(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    xprintf("phy id: %d\n", g_DeviceGmacDefaultConfig.phy_id);

    xprintf("link speed: %d Mbps\n", PHY_GetLinkSpeed(GMAC0, &g_DeviceGmacDefaultConfig));
    xprintf("link duplex(0:link down/1:half/2:full): %d\n", PHY_GetLinkDuplex(GMAC0, &g_DeviceGmacDefaultConfig));

    return 0;
}

#ifdef USE_OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif // #ifdef USE_OS_FREERTOS

int ShellCommand_Ps(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    xprintf("%2s %-8s %3s %-8s %-8s %s\n",
            "Nr",
            "Name",
            "Pri",
            "Stack",
            "Sp",
            "State");

    xprintf("-- -------- --- -------- -------- -----\n");

#ifdef USE_OS_FREERTOS
    vTaskList(0);
#endif

    return 0;
}

/* lwIP core includes */
#include "lwip/opt.h"

#include "lwip/err.h"
#include "lwip/netif.h"

#include "lwip/init.h"
#include "lwip/ip4_addr.h"

#include "lwip/sys.h"
#include "lwip/api.h"

#include "lwip/sockets.h"

mTaskDefine(_tcp_client_task)
{
    extern void iperf_tcp_client(void *pvParameters);
    iperf_tcp_client(NULL);
}

int ShellCommand_Tcp(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    static uint8_t m_tcp_client_created = 0;

    if (!m_tcp_client_created) {
        m_tcp_client_created = 1;
        mTaskCreate(_tcp_client_task, "_tcp", TCP_SERVER_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);
    } else {
        xprintf("tcp client already created!\n");
    }

    return 0;
}

int ShellCommand_Udp(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    extern void iperf_udp_client(void *pvParameters);
    iperf_udp_client(NULL);

    return 0;
}

const SHELL_COMMAND_T gsShellCommands[] = {
    {"help",  ShellCommand_Help,  "Show help information"    },
    {"clear", ShellCommand_Clear, "Clear screen"             },
    {"reg",   ShellCommand_Reg,   "Register read/write"      },
    {"mem",   ShellCommand_Mem,   "Memory read/write"        },
    {"nand",  ShellCommand_Nand,  "Nand flash test"          },
    {"nor",   ShellCommand_Nor,   "Nor flash test"           },
    {"qspi",  ShellCommand_Qspi,  "Qspi flash test"          },
    {"spi",   ShellCommand_Spi,   "Spi flash test"           },
    {"can",   ShellCommand_Can,   "Can test"                 },
    {"gmac",  ShellCommand_Gmac,  "Gmac test"                },
    {"phy",   ShellCommand_Phy,   "Gmac Phy test"            },
    {"link",  ShellCommand_Link,  "Gmac Phy link check"      },
    {"ps",    ShellCommand_Ps,    "List task"                },
    {"tcp",   ShellCommand_Tcp,   "Running raw tcp send Test"},
    {"udp",   ShellCommand_Udp,   "Running raw udp send Test"},
};

const uint32_t gui32ShellCommandsNum = ARRAY_SIZE(gsShellCommands);
