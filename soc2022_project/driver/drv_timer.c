/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_timer.h"
#include "sysclock.h"

/*
功能描述：

名称：g_DeviceTimerDefaultConfig
功能：定义默认的定时器设备配置，仅在调试模式下使用
参数：
  - mode: 定时器模式，设置为用户定义
  - tick1us: 每微秒的tick数，设置为1000000
  - interval: 定时器间隔，以微秒为单位，设置为1000000微秒
  - int_enable: 中断使能，设置为1（使能）
  - user_data: 用户自定义数据，初始设置为0
返回值：无
输入：无
输出：无
上层函数：无
下层函数：可能被定时器初始化函数如 HW_TIMER_InitConfig 使用来应用这些默认设置

附加说明：
- 这个配置结构体通常用于初始化定时器，配置参数适用于基本的定时和中断需求
- 在非调试模式下，应避免使用这些默认参数，以免影响生产环境下的设备行为
- 结构体中的参数可以根据实际硬件和项目需求进行调整
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceTimerConfig_t g_DeviceTimerDefaultConfig =
    {
        .mode = eTimerMode_UserDefined,
        .tick1us = 1000000,      // NOTE: ticks contained in 1-us
        .interval = 1000 * 1000, // NOTE: how many 1-us
        .int_enable = 1,

        .user_data = 0,
};
#endif // #ifdef DEBUG

// Note:
//  1. timing accuracy is 1-us
//  2. call 'HW_TIMER_Start' to start timer after 'HW_TIMER_InitConfig'
//  3. soft reset won't reset the register value, so you must press the hardware reset

/*
功能描述：

名称：HW_TIMER_InitConfig
功能：初始化指定的定时器配置
参数：
  - TIMERx: 指向特定定时器的指针
  - chn: 指定的定时器通道
  - config: 指向定时器配置结构体的指针
返回值：成功返回0，失败返回-1
输入：定时器端口，通道和配置信息
输出：根据输入的配置初始化定时器
上层函数：可能由设备初始化函数或系统启动配置调用
下层函数：调用了 drv_get_apb_freq 和 drv_irq_enable 来获取APB频率和使能中断
*/
int HW_TIMER_InitConfig(TIM_TypeDef *TIMERx, TimerChannel_e chn, DeviceTimerConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == TIMERx) || (chn >= eTimerChannel_Max) || (NULL == config)) {
        return -1;
    }

    // disable timer first
    SET_BIT(TIMERx->ch[chn].CTRL, TIMER_CTRL_ENABLE_POS, 0);

    reg_value = TIMERx->ch[chn].EOI;
    UNUSED(reg_value);

    if (config->int_enable) {
        if (TIMER0 == TIMERx) {
            drv_irq_enable(TIMER0_IRQn);
        } else if (TIMER1 == TIMERx) {
            drv_irq_enable(TIMER1_IRQn);
        }
    }

    reg_value = drv_get_apb_freq() / config->tick1us; // calc count of 1-us
    reg_value = config->interval * reg_value;
    TIMERx->ch[chn].LOADL = reg_value;
    TIMERx->ch[chn].LOADH = 0;

    reg_value = 0;
    SET_BIT(reg_value, TIMER_CTRL_INTERRUPT_MASK_POS, !config->int_enable);
    SET_BIT(reg_value, TIMER_CTRL_MODE_POS, config->mode);
    TIMERx->ch[chn].CTRL = reg_value;

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：HW_TIMER_Start
功能：启动指定的定时器通道
参数：
  - TIMERx: 指向特定定时器的指针
  - chn: 指定的定时器通道
返回值：成功返回0，失败返回-1
输入：定时器端口和通道
输出：启动指定的定时器通道
上层函数：可能由需要开始定时操作的功能调用
下层函数：无
*/
int HW_TIMER_Start(TIM_TypeDef *TIMERx, TimerChannel_e chn)
{
    if (chn >= eTimerChannel_Max) {
        return -1;
    }

    SET_BIT(TIMERx->ch[chn].CTRL, TIMER_CTRL_ENABLE_POS, 1);

    return 0;
}

/*
功能描述：

名称：HW_TIMER_Stop
功能：停止指定的定时器通道
参数：
  - TIMERx: 指向特定定时器的指针
  - chn: 指定的定时器通道
返回值：成功返回0，失败返回-1
输入：定时器端口和通道
输出：停止指定的定时器通道
上层函数：可能由需要结束定时操作的功能调用
下层函数：无
*/
int HW_TIMER_Stop(TIM_TypeDef *TIMERx, TimerChannel_e chn)
{
    if (chn >= eTimerChannel_Max) {
        return -1;
    }

    SET_BIT(TIMERx->ch[chn].CTRL, TIMER_CTRL_ENABLE_POS, 0);

    return 0;
}

/*
功能描述：

名称：HW_TIMER_ClearInterrupt
功能：清除指定定时器通道的中断
参数：
  - TIMERx: 指向特定定时器的指针
  - chn: 指定的定时器通道
返回值：返回读取到的中断状态
输入：定时器端口和通道
输出：清除中断并返回中断状态
上层函数：可能由中断服务例程调用
下层函数：无
*/
uint32_t HW_TIMER_ClearInterrupt(TIM_TypeDef *TIMERx, TimerChannel_e chn)
{
    uint32_t reg_value;

    if (chn >= eTimerChannel_Max) {
        return 0;
    }

    reg_value = TIMERx->ch[chn].EOI;
    UNUSED(reg_value);

    return reg_value;
}

/*
功能描述：

名称：HW_TIMER_GetStatus
功能：获取定时器的整体中断状态
参数：
  - TIMERx: 指向特定定时器的指针
返回值：返回当前定时器的中断状态
输入：定时器端口
输出：返回定时器的中断状态
上层函数：可能由系统状态监控或诊断功能调用
下层函数：无
*/
uint32_t HW_TIMER_GetStatus(TIM_TypeDef *TIMERx)
{
    return TIMERx->INTSTATUS;
}

#ifdef DEBUG_TIMER

#include "xprintf.h"

int selftest_timer(void)
{
    TIM_TypeDef *timer0 = TIMER0;
    TIM_TypeDef *timer1 = TIMER1;

    xprintf("TIMER0 VERSION = %08x\n", timer0->VERSION);
    xprintf("TIMER1 VERSION = %08x\n", timer1->VERSION);

    g_DeviceTimerDefaultConfig.interval = 1000 * 1000;
    HW_TIMER_InitConfig(timer0, eTimerChannel_0, &g_DeviceTimerDefaultConfig);

    g_DeviceTimerDefaultConfig.interval = 2000 * 1000;
    HW_TIMER_InitConfig(timer0, eTimerChannel_1, &g_DeviceTimerDefaultConfig);

    g_DeviceTimerDefaultConfig.interval = 3000 * 1000;
    HW_TIMER_InitConfig(timer1, eTimerChannel_0, &g_DeviceTimerDefaultConfig);

    g_DeviceTimerDefaultConfig.interval = 4000 * 1000;
    HW_TIMER_InitConfig(timer1, eTimerChannel_1, &g_DeviceTimerDefaultConfig);

    HW_TIMER_Start(timer0, eTimerChannel_0);
    HW_TIMER_Start(timer0, eTimerChannel_1);
    HW_TIMER_Start(timer1, eTimerChannel_0);
    HW_TIMER_Start(timer1, eTimerChannel_1);

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_TIMER
