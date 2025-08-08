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
        ui32RegValue = PHY_Read(g_DeviceGmacDefaultConfig.gmac, uinPhyId, i, 10 * 1000);
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

    xprintf("link speed: %d Mbps\n", PHY_GetLinkSpeed(g_DeviceGmacDefaultConfig.gmac, &g_DeviceGmacDefaultConfig));
    xprintf("link duplex(0:link down/1:half/2:full): %d\n", PHY_GetLinkDuplex(g_DeviceGmacDefaultConfig.gmac, &g_DeviceGmacDefaultConfig));

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

/* FatFS core includes */
#include "ff.h"

// Note:
//  1. There are some bug of SD driver, we do not modify them now
//  2. We let 'fobj' link into SRAM to avoid SD timeout occured by DDR access

static FIL m_fobj;

int ShellCommand_Jump(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc > 1) {
        size_t uinAddress = strtoul(apszArgv[1], NULL, 16);

        extern void JUMP_FUC(size_t addr);
        JUMP_FUC(uinAddress);
    }

    return 0;
}

int ShellCommand_Echo(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    FRESULT res;
    UINT bw;

    switch (ui8Argc) {
    case 1:
        xprintf("missing params!\n");
        break;
    case 2:
        xprintf("%s\n", apszArgv[1]);
        break;
    case 3:
        res = f_open(&m_fobj, apszArgv[2], FA_CREATE_ALWAYS | FA_WRITE);
        if (res == FR_OK) {
            f_write(&m_fobj, apszArgv[1], strlen(apszArgv[1]), &bw);
            f_close(&m_fobj);
        } else {
            xprintf("file create failed!\n");
        }
        break;
    default:
        break;
    }

    return 0;
}

int ShellCommand_Cat(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    FRESULT res;
    UINT br;

    switch (ui8Argc) {
    case 1:
        xprintf("missing params!\n");
        break;
    case 2:
        res = f_open(&m_fobj, apszArgv[1], FA_OPEN_EXISTING | FA_READ);
        if (res == FR_OK) {
            do {
                f_read(&m_fobj, m_aui8DummyBuffer, ARRAY_SIZE(m_aui8DummyBuffer) - 1, &br);
                m_aui8DummyBuffer[br] = 0;
                xprintf("%s", m_aui8DummyBuffer);
            } while (br > 0);
            xprintf("\n");
            f_close(&m_fobj);
        } else {
            xprintf("file open failed!\n");
        }
        break;
    default:
        break;
    }

    return 0;
}

int ShellCommand_Del(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    FRESULT res;

    switch (ui8Argc) {
    case 1:
        xprintf("missing params!\n");
        break;
    case 2:
        res = f_unlink(apszArgv[1]);
        if (res == FR_OK) {
            xprintf("file delete success.\n");
        } else {
            xprintf("file delete failed!\n");
        }
        break;
    default:
        break;
    }

    return 0;
}

int ShellCommand_Ls(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    FRESULT res;
    DIR dobj;
    FILINFO fno;

    res = f_opendir(&dobj, "0:");

    if (res != FR_OK) {
        xprintf("dir open failed!\n");
        return 0;
    } else {
        for (;;) {
            res = f_readdir(&dobj, &fno);

            if ((res != FR_OK) || (fno.fname[0] == 0)) {
                break;
            }

            if (fno.fattrib & AM_DIR) {
                continue;
            } else {
                xprintf("%s\n", fno.fname);
            }
        }
    }

    f_closedir(&dobj);

    return 0;
}

int ShellCommand_Rb(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: rb -E\n");
        return 0;
    }

    Ymodem_ReceiveTask();

    return 0;
}

int ShellCommand_Dd(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: dd [file] [address]\n"
                "Where: address is in hex\n"
                "       dd test.txt 2000\n");
        return 0;
    }

    FRESULT res;
    UINT br;

    char *paFileName = apszArgv[1];
    size_t uinAddress = (ui8Argc > 2) ? strtoul(apszArgv[2], NULL, 16) : 0;
    size_t uinSize = 0;

    // uinAddress += FLASH_BOOT_BASE;

    res = f_open(&m_fobj, paFileName, FA_OPEN_EXISTING | FA_READ);
    if (res == FR_OK) {
        uinSize = f_size(&m_fobj);
    } else {
        xprintf("file open failed!\n");
        return 0;
    }

    xprintf("read from %s, size %d.\n", paFileName, uinSize);
    // xprintf("write to %08x.\n", uinAddress);

    size_t uinStartAddr = (uinAddress) & ~(FLASH_SECTOR_SIZE - 1);
    size_t uinEndAddr = (uinAddress + uinSize) & ~(FLASH_SECTOR_SIZE - 1);

    for (size_t addr = uinStartAddr; addr <= uinEndAddr; addr += FLASH_SECTOR_SIZE) {
        spansionSectorErase(addr);
    }
    mdelay(1000);

    do {
        f_read(&m_fobj, m_aui8DummyBuffer, ARRAY_SIZE(m_aui8DummyBuffer), &br);
        if (br > 0) {
            UINT len = (br + 3) / 4;
            for (UINT i = 0; i < len; i++) {
                uint32_t ui32Dummy = 0;
                ui32Dummy |= (m_aui8DummyBuffer[i * 4 + 0] << 24);
                ui32Dummy |= (m_aui8DummyBuffer[i * 4 + 1] << 16);
                ui32Dummy |= (m_aui8DummyBuffer[i * 4 + 2] << 8);
                ui32Dummy |= (m_aui8DummyBuffer[i * 4 + 3] << 0);
                spansionProgram(uinAddress, ui32Dummy);
                uinAddress += 4;
            }
        }
    } while (br > 0);

    f_close(&m_fobj);

    return 0;
}

int ShellCommand_Device(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    struct DEVICE_T
    {
        char *name;
        uint32_t address;
        uint32_t size;
    } asDevices[] = {
        {"uart0", (uint32_t)UART0, sizeof(UART_TypeDef)},
        {"uart1", (uint32_t)UART1, sizeof(UART_TypeDef)},
        {"uart2", (uint32_t)UART2, sizeof(UART_TypeDef)},
        {"uart3", (uint32_t)UART3, sizeof(UART_TypeDef)},
        {"gmac0", (uint32_t)GMAC0, sizeof(GDMA_TypeDef)},
    };

    xprintf("%-8s %-16s %s\n", "name", "address", "size");
    xprintf("------------------------------\n");

    for (size_t i = 0; i < ARRAY_SIZE(asDevices); i++) {
        xprintf("%-8s %016lx %d\n", asDevices[i].name, asDevices[i].address, asDevices[i].size);
    }

    return 0;
}

int ShellCommand_Ebc(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    if (ui8Argc < 2) {
        xprintf("Unknown command parameter!\n"
                "Usage: ebc [op] [address] [length | data | address] [size]\n"
                "Where: op = e[rase] | r[ead] | w[rite]\n"
                "       address/length/data/size is in hex\n"
                "       ebc erase 200\n"
                "       ebc read 200 40\n"
                "       ebc write 200 80\n");
        return 0;
    }

    char *pszOp = apszArgv[1];
    size_t uinAddress = (ui8Argc > 2) ? strtoul(apszArgv[2], NULL, 16) : 0;
    size_t uinLengthOrData = (ui8Argc > 3) ? strtoul(apszArgv[3], NULL, 16) : 0x10;

    uinAddress += FLASH_BASE;

    switch (pszOp[0]) {
    case 'e':
        uinAddress = (uinAddress / FLASH_SECTOR_SIZE) * FLASH_SECTOR_SIZE;
        xprintf("erase sector, address: 0x%08x\n", uinAddress);
        spansionSectorErase(uinAddress);
        break;
    case 'r':
        while (uinLengthOrData > 0) {
            uint32_t ui32CntInBlock = (uinLengthOrData > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : uinLengthOrData;
            for (uint32_t i = 0; i < ui32CntInBlock; i++) {
                m_aui8DummyBuffer[i] = REG_READ_8(uinAddress + i);
            }
            _ShellCommandDumpMemory(uinAddress, (size_t)&m_aui8DummyBuffer, ui32CntInBlock);
            uinAddress += ui32CntInBlock;
            uinLengthOrData -= ui32CntInBlock;
        }
        break;
    case 'w':
        xprintf("address[%08x] write: %08x, ", uinAddress, uinLengthOrData);
        spansionProgram(uinAddress, uinLengthOrData);
        uinLengthOrData = REG_READ_32(uinAddress);
        xprintf("read-back: %08x\n", uinLengthOrData);
        break;
    default:
        break;
    }

    return 0;
}

int ShellCommand_Led(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    extern uint32_t g_led_interval;

    g_led_interval = (ui8Argc > 1) ? strtoul(apszArgv[1], NULL, 10) : g_led_interval;
    xprintf("new interval: %d ms\n", g_led_interval);

    return 0;
}

int ShellCommand_Coremark(
    uint8_t ui8Argc,
    char *apszArgv[])
{
    extern void coremark_main(void);
    coremark_main();

    return 0;
}

const SHELL_COMMAND_T gsShellCommands[] = {
    {"help",  ShellCommand_Help,  "Show help information"    },
    {"clear", ShellCommand_Clear, "Clear screen"             },
    {"reg",   ShellCommand_Reg,   "Register read/write"      },
    {"mem",   ShellCommand_Mem,   "Memory read/write"        },
    {"gmac",  ShellCommand_Gmac,  "Gmac test"                },
    {"phy",   ShellCommand_Phy,   "Gmac Phy test"            },
    {"link",  ShellCommand_Link,  "Gmac Phy link check"      },
    {"ps",    ShellCommand_Ps,    "List task"                },
    {"tcp",   ShellCommand_Tcp,   "Running raw tcp send Test"},
    {"udp",   ShellCommand_Udp,   "Running raw udp send Test"},
    {"jump",  ShellCommand_Jump,  "Jump to target address"   },
    {"echo",  ShellCommand_Echo,  "Create file and add text" },
    {"cat",   ShellCommand_Cat,   "Display file"             },
    {"del",   ShellCommand_Del,   "Delete file"              },
    {"ls",    ShellCommand_Ls,    "List file"                },
    {"rb",    ShellCommand_Rb,    "Ymodem receive"           },
    {"dd",    ShellCommand_Dd,    "Data download"            },
    {"ebc",   ShellCommand_Ebc,   "EBC test"                 },
    {"led",   ShellCommand_Led,   "Gpio test"                },
};

const uint32_t gui32ShellCommandsNum = ARRAY_SIZE(gsShellCommands);
