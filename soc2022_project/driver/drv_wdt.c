/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_wdt.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

/*
功能描述：

名称：g_DeviceWdtDefaultConfig
功能：配置看门狗定时器(WDT)的默认参数
参数：无
返回值：无
输入：无
输出：无
上层函数：无
下层函数：无

附加说明：
- 此结构体配置了看门狗定时器的操作模式和超时时间
- 主要用于监控系统的运行状态，如果系统在设定的时间内未能喂狗，则看门狗定时器将复位系统

详细字段说明：
- mode: 看门狗的操作模式，这里设置为复位模式（eWdtMode_Reset）
- timeout: 看门狗的超时时间，这里设置为2^32个时钟周期（eWdtTimeout_2G）
- user_data: 用于存储额外的用户定义数据，初始设置为0

注意：
- timeout字段在此配置中不可修改，意味着一旦看门狗启动，系统必须在超时前刷新看门狗，否则将导致系统复位
- 在设计系统时，需合理配置看门狗的超时时间，以避免因频繁复位导致的系统不稳定
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceWdtConfig_t g_DeviceWdtDefaultConfig =
    {
        .mode = eWdtMode_Reset,
        .timeout = eWdtTimeout_2G, // NOTE: params is not modifiabled

        .user_data = 0,
};
#endif // #ifdef DEBUG

// Note:
//  1. call 'HW_WDT_Start' to start timer after 'HW_WDT_InitConfig'

/*
功能描述：

名称：HW_WDT_InitConfig
功能：初始化指定的看门狗定时器（WDT）配置
参数：
  - WDTx: 指向特定看门狗定时器的指针
  - config: 指向看门狗配置结构体的指针
返回值：成功返回0，失败返回-1
输入：看门狗端口和配置信息
输出：根据输入的配置初始化看门狗
上层函数：可能由设备初始化函数或系统启动配置调用
下层函数：无
*/
int HW_WDT_InitConfig(WDT_TypeDef *WDTx, DeviceWdtConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == WDTx) || (NULL == config)) {
        return -1;
    }

    reg_value = WDTx->EOI;
    UNUSED(reg_value);

    reg_value = 0xffffff00;                                  // .TORR inited to zero first
    reg_value |= (config->timeout << WDT_TORR_TOP_INIT_POS); // NOTE: 2^(16+TOP_INIT)
    reg_value |= (config->timeout << WDT_TORR_TOP_POS);      // NOTE: 2^(16+TOP)
    WDTx->TORR = reg_value;

    reg_value = 0; // .CR inited to zero first
    reg_value |= (CR_RPL_PCLK_CYCLES_16 << WDT_CR_RPL_POS);
    SET_BIT(reg_value, WDT_CR_RMOD_POS, config->mode);
    SET_BIT(reg_value, WDT_CR_WDT_EN_POS, WDT_EN_DISABLED); // NOTE: enable when call 'HW_WDT_Start'
    WDTx->CR = reg_value;

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：HW_WDT_Start
功能：启动指定的看门狗定时器
参数：
  - WDTx: 指向特定看门狗定时器的指针
返回值：成功返回0
输入：看门狗端口
输出：启动看门狗定时器
上层函数：可能由系统启动或看门狗配置完成后调用
下层函数：无
*/
int HW_WDT_Start(WDT_TypeDef *WDTx)
{
    SET_BIT(WDTx->CR, WDT_CR_WDT_EN_POS, WDT_EN_ENABLED);
    return 0;
}

/*
功能描述：

名称：HW_WDT_Feed
功能：喂狗操作，重置看门狗定时器的计数器，防止系统复位
参数：
  - WDTx: 指向特定看门狗定时器的指针
返回值：成功返回0
输入：看门狗端口
输出：重置看门狗计数器
上层函数：可能由定时任务或需要延长看门狗超时的功能调用
下层函数：无
*/
int HW_WDT_Feed(WDT_TypeDef *WDTx)
{
    WDTx->CRR = 0x76;

#ifdef DEBUG
    xprintf("wdt feed.\n");
#endif

    return 0;
}

/*
功能描述：

名称：HW_WDT_GetValue
功能：获取当前看门狗定时器的计数值
参数：
  - WDTx: 指向特定看门狗定时器的指针
返回值：当前看门狗定时器的计数值
输入：看门狗端口
输出：返回看门狗当前的计数值
上层函数：可能由系统监控或调试功能调用
下层函数：无
*/
uint32_t HW_WDT_GetValue(WDT_TypeDef *WDTx)
{
    return WDTx->CCVR;
}

/*
功能描述：

名称：HW_WDT_GetStatus
功能：获取看门狗定时器的状态
参数：
  - WDTx: 指向特定看门狗定时器的指针
返回值：看门狗定时器的状态
输入：看门狗端口
输出：返回看门狗的状态信息
上层函数：可能由系统状态监控或诊断功能调用
下层函数：无
*/
uint32_t HW_WDT_GetStatus(WDT_TypeDef *WDTx)
{
    return WDTx->STAT;
}

/*
功能描述：

名称：HW_WDT_ClearInterrupt
功能：清除看门狗定时器的中断
参数：
  - WDTx: 指向特定看门狗定时器的指针
返回值：中断状态寄存器的值
输入：看门狗端口
输出：清除看门狗的中断，并返回中断状态
上层函数：可能由中断服务例程调用
下层函数：无
*/
uint32_t HW_WDT_ClearInterrupt(WDT_TypeDef *WDTx)
{
    uint32_t reg_value;

    reg_value = WDTx->EOI;
    UNUSED(reg_value);

    return reg_value;
}

#ifdef DEBUG_WDT

#include "xprintf.h"

void DumpWDTParam(WDT_TypeDef *WDTx)
{
    uint32_t reg_value;
    reg_value = WDTx->COMP_PARAM_1;

    xprintf("\n");
    xprintf("COMP_PARAM_1 = %08x\n", WDTx->COMP_PARAM_1);
    xprintf("    .WDT_CNT_WIDTH     = %d\n", ((reg_value >> 24) & 0x1f) + 16); // 5-bit
    xprintf("    .WDT_DFLT_TOP_INIT = %d\n", (reg_value >> 20) & 0xf);         // 4-bit
    xprintf("    .WDT_DFLT_TOP      = %d\n", (reg_value >> 16) & 0xf);         // 4-bit
    xprintf("    .WDT_DFLT_RPL      = %d\n", (reg_value >> 10) & 0x7);         // 3-bit
    xprintf("    .APB_DATA_WIDTH    = %d\n", (reg_value >> 8) & 0x3);          // 2-bit
    xprintf("    .WDT_PAUSE         = %s\n", GET_BIT(reg_value, 7) ? "ENABLED(1)" : "DISABLED(0)");
    xprintf("    .WDT_USE_FIX_TOP   = %s\n", GET_BIT(reg_value, 6) ? "PREDEFINED(1)" : "USERDEFINED(0)");
    xprintf("    .WDT_HC_TOP        = %s\n", GET_BIT(reg_value, 5) ? "HARDCODED(1)" : "PROGRAMMABLE(0)");
    xprintf("    .WDT_HC_RPL        = %s\n", GET_BIT(reg_value, 4) ? "HARDCODED(1)" : "PROGRAMMABLE(0)");
    xprintf("    .WDT_HC_RMOD       = %s\n", GET_BIT(reg_value, 3) ? "HARDCODED(1)" : "PROGRAMMABLE(0)");
    xprintf("    .WDT_DUAL_TOP      = %s\n", GET_BIT(reg_value, 2) ? "ENABLED(1)" : "DISABLED(0)");
    xprintf("    .WDT_DFLT_RMOD     = %s\n", GET_BIT(reg_value, 1) ? "Interrupt&Reset(1)" : "OnlyReset(0)");
    xprintf("    .WDT_ALWAYS_EN     = %s\n", GET_BIT(reg_value, 0) ? "ENABLED(1)" : "DISABLED(0)");
    xprintf("COMP_PARAM_2 = %08x\n", WDTx->COMP_PARAM_2);
    xprintf("    .WDT_CNT_RST       = %08x\n", WDTx->COMP_PARAM_2);
    xprintf("COMP_PARAM_3 = %08x\n", WDTx->COMP_PARAM_3);
    xprintf("COMP_PARAM_4 = %08x\n", WDTx->COMP_PARAM_4);
    xprintf("COMP_PARAM_5 = %08x\n", WDTx->COMP_PARAM_5);
    xprintf("\n");
}

int selftest_wdt(void)
{
    WDT_TypeDef *wdt = WDT0;

    xprintf("WDT VERSION = %08x\n", wdt->COMP_VERSION);
    xprintf("WDT TYPE = %08x\n", wdt->COMP_TYPE);

    DumpWDTParam(wdt);

    HW_WDT_InitConfig(wdt, &g_DeviceWdtDefaultConfig);
    HW_WDT_Start(wdt);
    HW_WDT_Feed(wdt);

    xprintf("\n");
    return 0;
}

void selftest_wdt_loop(void)
{
    HW_WDT_Feed(WDT0);
}

#endif // #ifdef DEBUG_WDT
