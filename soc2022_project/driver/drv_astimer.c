
/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_astimer.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

/*
功能描述：

名称：g_DeviceAstimerDefaultConfig
功能：定义默认的ASTIMER设备配置，仅在调试模式下使用
参数：
  - mode: 设定ASTIMER的模式，此处为实时时钟模式
  - clock_mode: 时钟模式，设置为内部时钟
  - interval: 中断间隔，设定为1微秒
  - freq: 频率，设置为1MHz
  - ctl_period_interval: 控制周期间隔，设置为1微秒
  - ctl_period_tick: 控制周期的时钟周期数，设置为100
  - output_signal: 输出信号的类型，四个通道均设置为同步信号x1
  - output_pulse: 输出脉冲的极性，四个通道均设置为负脉冲
  - output_pulse_width: 输出脉冲的宽度，分别设置为1, 2, 4, 8单位
  - gps_select: GPS选择，设置为GPS0
  - user_data: 用户自定义数据，初始设置为0
返回值：无
输入：无
输出：无
上层函数：无
下层函数：可能被ASTIMER初始化函数如HW_ASTIMER_InitConfig使用来应用这些默认设置

附加说明：
- 这个配置结构体通常用于初始化ASTIMER设备，配置参数适用于基本的时间测量和信号生成需求
- 在非调试模式下，应避免使用这些默认参数，以免影响生产环境下的设备行为
- 结构体中的参数可以根据实际硬件和项目需求进行调整
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceAstimerConfig_t g_DeviceAstimerDefaultConfig =
    {
        .mode = eAstimerMode_RTC,

        .clock_mode = eAstimerClockMode_Internal,
        .interval = eAstimerInterval_1US,

        .freq = 1 * 1000 * 1000,

        .ctl_period_interval = eAstimerInterval_1US,
        .ctl_period_tick = 100,

        .output_signal = {
                          eAstimerOutSignal_Sync_x1,
                          eAstimerOutSignal_Sync_x1,
                          eAstimerOutSignal_Sync_x1,
                          eAstimerOutSignal_Sync_x1                                                                  },
        .output_pulse = {eAstimerOutPulse_Neg,      eAstimerOutPulse_Neg, eAstimerOutPulse_Neg, eAstimerOutPulse_Neg},
        .output_pulse_width = {1,                         2,                    4,                    8                   },

        .gps_select = eAstimerGPSSelect_GPS0,

        .user_data = 0,
};
#endif // #ifdef DEBUG

/*
功能描述：

名称：HW_ASTIMER_InitConfig
功能：初始化ASTIMER设备的配置
参数：
  - ASTIMERx: 指向ASTIMER设备的指针
  - config: 指向ASTIMER配置结构的指针
返回值：成功返回0，失败返回-1
输入：ASTIMER设备指针和配置结构
输出：无
上层函数：可能由主系统初始化时调用
下层函数：无

附加说明：
- 此函数设置ASTIMER的多种工作模式，包括RTC、同步信号等
- 设置前，函数首先禁用ASTIMER，配置完成后再启用
- 根据不同的模式，设置相关寄存器以匹配预期的硬件行为
*/
int HW_ASTIMER_InitConfig(ASTIMER_TypeDef *ASTIMERx, DeviceAstimerConfig_t *config)
{
    uint32_t astimer_div;
    uint32_t reg_value;

    if ((NULL == ASTIMERx) || (NULL == config)) {
        return -1;
    }

    DEBUG_ASSERT(config->freq <= 0x100000);

    astimer_div = drv_get_apb_freq() / config->freq;
    astimer_div = (astimer_div > 0) ? (astimer_div - 1) : 0;

    ASTIMERx->REG[addr2idx(0x000)] = astimer_div;

    if (eAstimerClockMode_Internal == config->clock_mode) {
        ASTIMERx->REG[addr2idx(0x00c)] = (eAstimerClockMode_Internal << 2) | (config->ctl_period_interval << 0);
    } else {
        ASTIMERx->REG[addr2idx(0x00c)] = (eAstimerClockMode_External << 2);
    }

    reg_value = (config->ctl_period_tick >> 1); // NOTE: div 2 first
    ASTIMERx->REG[addr2idx(0x004)] = (reg_value & 0xffff);
    ASTIMERx->REG[addr2idx(0x008)] = (reg_value >> 16);

    ASTIMERx->REG[addr2idx(0x040)] = (config->interval << 0);

    switch (config->mode) {
    case eAstimerMode_CTRLPERIOD:
        break;
    case eAstimerMode_SYNC:
        ASTIMERx->REG[addr2idx(0x0c0)] = BIT(config->output_pulse[0]) | BIT(config->output_pulse[1]) | BIT(config->output_pulse[2]) | BIT(config->output_pulse[3]);
        for (uint32_t i = 0; i < 4; i++) {
            DEBUG_ASSERT(config->output_pulse_width[i] <= 0x100000);

            ASTIMERx->REG[addr2idx(0x090 + 8 * i)] = (config->output_pulse_width[i] & 0xffff);
            ASTIMERx->REG[addr2idx(0x094 + 8 * i)] = (config->output_pulse_width[i] >> 16);
        }
        ASTIMERx->REG[addr2idx(0x044)] = 0x0; // NOTE: select ctl period as the source of sync signal
        break;
    case eAstimerMode_GPS:
        ASTIMERx->REG[addr2idx(0x0c0)] = BIT(config->output_pulse[0]) | BIT(config->output_pulse[1]) | BIT(config->output_pulse[2]) | BIT(config->output_pulse[3]);
        for (uint32_t i = 0; i < 4; i++) {
            DEBUG_ASSERT(config->output_pulse_width[i] > 0);

            ASTIMERx->REG[addr2idx(0x104 + 4 * i)] = (config->output_pulse_width[i] - 1);
        }
        ASTIMERx->REG[addr2idx(0x0c8)] = BIT(config->gps_select);
        break;
    case eAstimerMode_RTC:
        break;
    default:
        break;
    }

    ASTIMERx->REG[addr2idx(0x0c4)] = (config->output_signal[0] << 0) | (config->output_signal[1] << 4) | (config->output_signal[2] << 8) | (config->output_signal[3] << 12);

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：HW_ASTIMER_ReadData
功能：从ASTIMER读取数据
参数：
  - ASTIMERx: 指向ASTIMER设备的指针
  - mode: 读取数据的模式
  - data: 存储读取数据的指针
返回值：成功返回0，失败返回-1
输入：ASTIMER设备指针，读取模式和数据存储指针
输出：读取的数据
上层函数：可能由需要ASTIMER数据的功能调用
下层函数：无

附加说明：
- 此函数根据提供的模式从ASTIMER读取不同类型的数据，如当前时间、控制周期、GPS时间等
- 读取的数据通过指针参数返回
*/
int HW_ASTIMER_ReadData(ASTIMER_TypeDef *ASTIMERx, AstimerReadMode_e mode, uint64_t *data)
{
    if (!data) {
        return -1;
    }

    switch (mode) {
    case eAstimerReadMode_Current:
    case eAstimerReadMode_CtrlPeriod:
    case eAstimerReadMode_GPS0:
    case eAstimerReadMode_GPS1:
    case eAstimerReadMode_GPS2:
    case eAstimerReadMode_GPS3:
    case eAstimerReadMode_Signal0:
    case eAstimerReadMode_Signal1:
    case eAstimerReadMode_Signal2:
    case eAstimerReadMode_Signal3:
        ASTIMERx->REG[addr2idx(0x144)] = BIT(mode);
        data[0] = ((uint64_t)(ASTIMERx->REG[addr2idx(0x154 + 16 * mode)]) << 48) | ((uint64_t)(ASTIMERx->REG[addr2idx(0x150 + 16 * mode)]) << 32) | ((uint64_t)(ASTIMERx->REG[addr2idx(0x14c + 16 * mode)]) << 16) | ((uint64_t)(ASTIMERx->REG[addr2idx(0x148 + 16 * mode)]) << 0);
        break;

    default:
        data[0] = 0;
        break;
    }

    return 0;
}

#ifdef DEBUG_ASTIMER

#include "xprintf.h"

int selftest_astimer(void)
{
    ASTIMER_TypeDef *astimer = ASTIMER0;

    xprintf("ASTIMER VERSION = %04x\n", astimer->REG[addr2idx(0x1fc)]); // NOTE: should be '0x5025'

    HW_ASTIMER_InitConfig(astimer, &g_DeviceAstimerDefaultConfig);

    if (eAstimerMode_RTC == g_DeviceAstimerDefaultConfig.mode) {
        uint64_t start, end;

        HW_ASTIMER_ReadData(astimer, eAstimerReadMode_Current, &start);

        xprintf("delay ...\n");
        mdelay(5000);

        HW_ASTIMER_ReadData(astimer, eAstimerReadMode_Current, &end);
        xprintf("rtc excape: %d\n", (uint32_t)(end - start)); // NOTE: may be overflow
    }

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_ASTIMER
