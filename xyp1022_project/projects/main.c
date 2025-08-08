/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "XYP1022.h"
#include "sysclock.h"
#include "drv_common.h"
#include "xprintf.h"
#include "xshell.h"

#ifdef USE_OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif // #ifdef USE_OS_FREERTOS

/* FatFS core includes */
#include "ff.h"

static FATFS g_fatfs_handle;
static BYTE g_fatfs_work[BLOCK_SIZE];

/* lwIP core includes */
#include "lwip/opt.h"

#include "lwip/err.h"
#include "lwip/netif.h"

#include "lwip/init.h"
#include "lwip/ip4_addr.h"

#include "lwip/sys.h"
#include "lwip/tcpip.h"

static struct netif g_netif;

/* ----- ----- ----- ----- USER TASK DEFINE START ----- ----- ----- ----- */

#ifdef USE_OS_FREERTOS

#ifndef OS_WAIT_FOREVER
#define OS_WAIT_FOREVER ((TickType_t)(~0))
#endif

#ifdef DEMO_TASK
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_task.h"
#endif // #ifdef DEMO_TASK

#ifdef DEMO_EVENT
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_event.h"
#endif // #ifdef DEMO_EVENT

#ifdef DEMO_MQ
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_mq.h"
#endif // #ifdef DEMO_MQ

#ifdef DEMO_SEMPHR
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_semphr.h"
#endif // #ifdef DEMO_SEMPHR

#ifdef DEMO_MUTEX
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_mutex.h"
#endif // #ifdef DEMO_MUTEX

#ifdef DEMO_MEMPOOL
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_mempool.h"
#endif // #ifdef DEMO_MEMPOOL

#ifdef DEMO_TIMER
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_timer.h"
#endif // #ifdef DEMO_TIMER

#ifdef DEMO_ALL
#include "../../../FreeRTOS/FreeRTOS-Demo/demo_all.h"
#endif // #ifdef DEMO_ALL

#ifdef DEMO_OSDRIVER1
#include "./demo_osdriver1.h"
#endif // #ifdef DEMO_OSDRIVER1

#define DMA_BUFFER_SIZE (32)

static uint8_t m_dma_src_buffer[DMA_BUFFER_SIZE];
static uint8_t m_dma_dst_buffer[DMA_BUFFER_SIZE];
static SemaphoreHandle_t m_dma_done;

uint32_t g_led_interval;

mTaskDefine(xshell_task)
{
    for (;;) {
        char aszCommand[128];
        uint32_t ui32CommandLengh = Shell_GetCommand(aszCommand, ARRAY_SIZE(aszCommand));
        aszCommand[ui32CommandLengh] = 0;
        Shell_HandleCommand(aszCommand, ui32CommandLengh);
    }
}

mTaskDefine(iperf_tcp_task)
{
    extern void iperf_tcp_server(void *pvParameters);
    iperf_tcp_server(pvParameters);
}

mTaskDefine(iperf_udp_task)
{
    extern void iperf_udp_server(void *pvParameters);
    iperf_udp_server(pvParameters);
}

mTaskDefine(self_dma_s_task)
{
    extern void axidma_reset(void);
    extern void axidma_transfer_one_block(unsigned char n, unsigned int sar, unsigned int dar, unsigned int ts, unsigned int ctrl);

    for (int i = 0; i < DMA_BUFFER_SIZE; i++) {
        m_dma_src_buffer[i] = i;
        m_dma_dst_buffer[i] = 0x0;
    }

    axidma_reset();
    axidma_transfer_one_block(1, (unsigned int)m_dma_src_buffer, (unsigned int)m_dma_dst_buffer, (DMA_BUFFER_SIZE - 1), (SRC_TR_WIDTH_8 | DST_TR_WIDTH_8));

    xprintf("dma transfer done, wake up recv task.\n");
    xSemaphoreGive(m_dma_done);

    for (;;) {
    }
}

mTaskDefine(self_dma_r_task)
{
    xSemaphoreTake(m_dma_done, OS_WAIT_FOREVER);

    portENTER_CRITICAL();
    {
        yprintf("data transfer by dma:\n");
        for (int i = 0; i < DMA_BUFFER_SIZE; i++) {
            yprintf("%02x ", m_dma_dst_buffer[i]);
        }
        yprintf("\n");
    }
    portEXIT_CRITICAL();

    for (;;) {
    }
}

mTaskDefine(self_gpio_task)
{
    uint32_t reg_addr;
    uint32_t reg_value;

    reg_addr = (GPIO_M_BASE_ADDR | GPIO_OUT_OFFSET);

    for (;;) {
        reg_value = REG_READ_32(reg_addr);
        reg_value &= 0xfffffffc;
        reg_value |= 0x03;
        REG_WRITE_32(reg_addr, reg_value);
        vTaskDelay(pdMS_TO_TICKS(g_led_interval));

        reg_value = REG_READ_32(reg_addr);
        reg_value &= 0xfffffffc;
        REG_WRITE_32(reg_addr, reg_value);
        vTaskDelay(pdMS_TO_TICKS(g_led_interval));
    }
}

mTaskDefine(self_spi_task)
{
    Dspi_Init_for_W25q_SpiFlash();

    uint8_t dummy_buffer[32];

    for (int i = 0; i < ARRAY_SIZE(dummy_buffer); i++) {
        dummy_buffer[i] = i;
    }

    W25q_SpiFlash_SectorEarse(0x0);
    W25q_SpiFlash_PageProgram(0x0, ARRAY_SIZE(dummy_buffer), dummy_buffer);

    W25q_SpiFlash_ReadData(0x0, ARRAY_SIZE(dummy_buffer), dummy_buffer);

    for (int i = 0; i < ARRAY_SIZE(dummy_buffer); i++) {
        xprintf("%02x ", dummy_buffer[i]);
    }
    xprintf("\n");

    for (;;) {
    }
}

mTaskDefine(self_i2c_task)
{
    IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
    IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;

    extern void i2c0_init(void);
    i2c0_init();

    i2c0_master_write(1, 0x0002);
    i2c0_master_read(2, 0xa2);

    for (;;) {
    }
}

void uTaskCreate(void)
{
    mTaskCreate(iperf_tcp_task, TCP_SERVER_THREAD_NAME "tcp", TCP_SERVER_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);
    mTaskCreate(iperf_udp_task, TCP_SERVER_THREAD_NAME "udp", TCP_SERVER_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);

    mTaskCreate(xshell_task, "xshell", 2048, (tskIDLE_PRIORITY + 1));

    mTaskCreate(self_dma_s_task, "m_dma_s", 2048, (tskIDLE_PRIORITY + 1));
    mTaskCreate(self_dma_r_task, "m_dma_r", 2048, (tskIDLE_PRIORITY + 1));
    mTaskCreate(self_gpio_task, "p_gpio", 2048, (tskIDLE_PRIORITY + 1));
    // mTaskCreate(self_spi_task, "p_spi", 2048, (tskIDLE_PRIORITY + 1));
    // mTaskCreate(self_i2c_task, "p_i2c", 2048, (tskIDLE_PRIORITY + 1));
}

#endif // #ifdef USE_OS_FREERTOS

uint32_t gui32SystickInterval = 0;

void systick_init(uint32_t timeout)
{
    gui32SystickInterval = timeout * TB_CLOCK_KHZ;
}

void systick_reinit(void)
{
    MTSPR(DEC, gui32SystickInterval);
    MTSPR(DECAR, gui32SystickInterval);

    MTSPR(TCR, TCR_DIE_MASK);
}

void check_endian()
{
    unsigned int value = 0x12345678;
    unsigned char *ptr = (unsigned char *)&value;

    if (ptr[0] == 0x12) {
        xprintf("big-endian\n");
    } else {
        xprintf("little-endian\n");
    }
}

/* ----- ----- ----- ----- USER TASK DEFINE END   ----- ----- ----- ----- */

/* ----- ----- ----- ----- LwIP INIT DEFINE START ----- ----- ----- ----- */

static err_t _ethernetif_init(struct netif *netif)
{
    extern err_t ethernetif_init(struct netif * netif);
    return ethernetif_init(netif);
}

static err_t _tcpip_input(struct pbuf *p, struct netif *netif)
{
    extern err_t tcpip_input(struct pbuf * p, struct netif * inp);
    return tcpip_input(p, netif);
}

void LwIP_Init(void)
{
    ip4_addr_t ipaddr;
    ip4_addr_t netmask;
    ip4_addr_t gw;

    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    tcpip_init(NULL, NULL);

    /* add the network interface (IPv4/IPv6) without RTOS */
    netif_add(&g_netif, &ipaddr, &netmask, &gw, NULL, (void *)_ethernetif_init, (void *)_tcpip_input);

    /* Registers the default network interface */
    netif_set_default(&g_netif);

    if (netif_is_link_up(&g_netif)) {
        netif_set_up(&g_netif);
        netif_set_link_up(&g_netif);
    } else {
        netif_set_down(&g_netif);
        netif_set_link_down(&g_netif);
    }

    if (!ip4_addr_isany(netif_ip4_addr((&g_netif)))) {
        xprintf("Static assigned IP: %s\n", ip4addr_ntoa(netif_ip4_addr(&g_netif)));
    }
}

/* ----- ----- ----- ----- LwIP INIT DEFINE END   ----- ----- ----- ----- */

static void CHIP_SelfCheck(void)
{
    StrapPin_t strap_pin_value;
    LDW(strap_pin_value.d, STRAP_PIN);
    xprintf("strap pin: 0x%08x\n", strap_pin_value.d);

    xprintf("  CHIP_RST_MODE: %d\n", strap_pin_value.f.CHIP_RST_MODE);
    xprintf("  EFUSE_CLK_SEL: %d\n", strap_pin_value.f.EFUSE_CLK_SEL);
    xprintf("  CORE_SPEED: %d MHz\n", get_core_speed(strap_pin_value.f.CORE_SPEED));
    xprintf("  DDR_SPEED: %d MHz\n", get_ddr_speed(strap_pin_value.f.DDR_SPEED));
    xprintf("  BUS_CLK_FREQ: 0x%x (0x0:div2 0x1:div4 0x2:div6 0x3:div8)\n", strap_pin_value.f.BUS_CLK_FREQ);
    xprintf("  CFG_ROM_LC: %d (1:16bit 0:8bit)\n", strap_pin_value.f.CFG_ROM_LC);
    xprintf("  CFG_SB_DIS: %d (1:EBC BANK0 boot 0:on-chip ROM boot)\n", strap_pin_value.f.CFG_SB_DIS);
    // xprintf("  PCIE0_TYPE: %d\n", strap_pin_value.f.PCIE0_TYPE);
    // xprintf("  PCIE1_TYPE: %d\n", strap_pin_value.f.PCIE1_TYPE);
    // xprintf("  PCIE2_TYPE: %d\n", strap_pin_value.f.PCIE2_TYPE);
    // xprintf("  PCIE3_TYPE: %d\n", strap_pin_value.f.PCIE3_TYPE);
    xprintf("  CPU1_DISABLE: %d (1:cpu1 disable)\n", strap_pin_value.f.CPU1_DISABLE);
    xprintf("  PRESENT_N_MODE: %d (1:power-on reset 0:interrupt)\n", strap_pin_value.f.PRESENT_N_MODE);
    xprintf("  EBC_NOT_NFC: %d (1:EBC 0:NFC)\n", strap_pin_value.f.EBC_NOT_NFC);
    xprintf("  HALT_CFG_B: %d\n", strap_pin_value.f.HALT_CFG_B);
    xprintf("  SERDES_CFG: 0x%02x\n", strap_pin_value.f.SERDES_CFG);

    check_endian();
}

static void MEM_Map(void)
{
    set_e0_tlb(0x10030000, 0xc0000500, 0xe000000a, 0xe000003f, 0x00000000, 0x00000004); // SRAM, 0x5=0101, 1MiB
    set_e0_tlb(0x10040000, 0xc0000800, 0xf800000a, 0xf800003f, 0x00000000, 0x00000004); // EBC,  0x8=1000, 64MiB
    set_e0_tlb(0x10050000, 0xc0000a00, 0x0000000a, 0x0000003f, 0x00000000, 0x00000000); // DDR,  0xa=1001, 256MiB
}

static void GPIO_Init(void)
{
    uint32_t reg_addr;
    uint32_t reg_value;

    reg_addr = (GPIO_M_BASE_ADDR | GPIO_IOCTL_OFFSET);
    reg_value = REG_READ_32(reg_addr);
    reg_value |= (BIT(1) | BIT(0));
    REG_WRITE_32(reg_addr, reg_value);

    reg_addr = (GPIO_M_BASE_ADDR | GPIO_DIR_OFFSET);
    reg_value = REG_READ_32(reg_addr);
    reg_value |= (BIT(1) | BIT(0));
    REG_WRITE_32(reg_addr, reg_value);

    reg_addr = (GPIO_M_BASE_ADDR | GPIO_OUT_OFFSET);
    reg_value = REG_READ_32(reg_addr);
    reg_value |= (BIT(1) | BIT(0));
    REG_WRITE_32(reg_addr, reg_value);
}

static void EBC_Init(void)
{
    EBCx_CFGADDR = 0x0;
    EBCx_CFGDATA = (BANK0_ROM_ADDR | EBCx_BnCR_BS_64M | EBCx_BnCR_BU_RW | EBCx_BnCR_BW_16b);

    EBCx_CFGADDR = 0x10;
    EBCx_CFGDATA = (EBCx_BnAP_BME | EBCx_BnAP_FWT_8 | EBCx_BnAP_BWT_5 | EBCx_BnAP_WBN_2 | EBCx_BnAP_WBF_2 | EBCx_BnAP_CSN_2 | EBCx_BnAP_OEN_2 | EBCx_BnAP_TH_7);
}

static void SD_Mount(void)
{
    FRESULT res;

_fatfs_mount:
    res = f_mount(&g_fatfs_handle, "0:", 1);
    if (res != FR_OK) {
        if (res == FR_NO_FILESYSTEM) {
            f_mkfs("0:", NULL, &g_fatfs_work, BLOCK_SIZE);
            f_mount(NULL, "0:", 1);
            goto _fatfs_mount;
        }
        xprintf("fs mount failed!\n");
    } else {
        xprintf("fs mount success.\n");
    }
}

int main(void)
{
    xprintf("Hello, world.\n");
    xprintf("build %s %s.\n", __DATE__, __TIME__);
    xprintf("\n");

    g_led_interval = 500;

    CHIP_SelfCheck();

    MEM_Map();

    GPIO_Init();
    EBC_Init();

    SD_Mount();

#ifdef USE_OS_FREERTOS
    portDISABLE_INTERRUPTS();

    LwIP_Init();

    systick_init(CONFIG_SYSTICK_MS);

    m_dma_done = xSemaphoreCreateCounting(1, 0);
    if (!m_dma_done) {
        xprintf("[INFO]: Semphr create failed!\n");
    }

    uTaskCreate();
    vTaskStartScheduler();
#endif // #ifdef USE_OS_FREERTOS

    ENTER_DEADLOOP();

    return 0;
}
