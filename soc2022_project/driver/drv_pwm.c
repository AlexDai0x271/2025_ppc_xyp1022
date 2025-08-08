/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_pwm.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

/*
功能描述：

名称：g_DevicePwmDefaultConfig
功能：定义默认的PWM设备配置，仅在调试模式下使用
参数：
  - clock_freq: 时钟频率，设置为10MHz
  - mode: PWM模式，设置为比较模式
  - out.freq: 输出频率，设置为100kHz
  - out.single_pulse: 单脉冲模式，设置为0
  - out.duty: 占空比，设置为500（表示50.0%）
  - out.invert: 输出反转，设置为0
  - out.dead_time: 死区时间，设置为0（仅适用于ch0/ch2/ch4/ch6）
  - out.pull_up: 上拉使能，设置为1
  - user_data: 用户自定义数据，初始设置为0
返回值：无
输入：无
输出：无
上层函数：无
下层函数：可能被PWM初始化函数如HW_PWM_InitConfig使用来应用这些默认设置

附加说明：
- 这个配置结构体通常用于初始化PWM设备，配置参数适用于基本的PWM操作和信号生成需求
- 在非调试模式下，应避免使用这些默认参数，以免影响生产环境下的设备行为
- 结构体中的参数可以根据实际硬件和项目需求进行调整
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DevicePwmConfig_t g_DevicePwmDefaultConfig =
    {
        .clock_freq = 10 * 1000 * 1000,

        .mode = ePwmMode_Compare,

        .out.freq = 100 * 1000,
        .out.single_pulse = 0,
        .out.duty = 500, // NOTE: one decimal precision
        .out.invert = 0,
        .out.dead_time = 0, // NOTE: only configed for ch0/ch2/ch4/ch6
        .out.pull_up = 1,

        .user_data = 0,
};
#endif // #ifdef DEBUG

/*
功能描述：

名称：HW_PWM_InitConfig
功能：初始化指定的PWM配置
参数：
  - PWMx: 指向特定PWM端口的指针
  - chn: PWM通道号
  - config: 指向PWM配置结构体的指针
返回值：成功返回0，失败返回-1
输入：PWM端口、通道号和配置信息
输出：根据输入的配置初始化PWM
上层函数：可能由设备初始化函数或系统启动配置调用
下层函数：调用reg_value来设置寄存器值

附加说明：
- 该函数首先配置PWM寄存器，包括时钟分频、占空比、模式选择等参数
- 配置包括设置输出频率、死区时间和上拉使能等
*/
int HW_PWM_InitConfig(PWM_TypeDef *PWMx, PwmChannel_e chn, DevicePwmConfig_t *config)
{
    uint32_t clk_div;
    uint32_t pwm_div;
    uint32_t reg_value;

    if ((NULL == PWMx) || (chn >= ePwmChannel_Max) || (NULL == config)) {
        return -1;
    }

    // disable pwm first
    HW_PWM_Stop(PWMx, chn);
    HW_PWM_CaptureStop(PWMx, chn);

    SET_BIT(PWMx->PIER, chn, PWM_DISABLED);

    reg_value = PWMx->PIFR;
    UNUSED(reg_value);

    reg_value = PWMx->PCSR;
    reg_value &= ~(0xf << (PWM_PCSR_CSR0_POS + chn * 4));
    reg_value |= (CSR_CLOCK_DIVID_1 << (PWM_PCSR_CSR0_POS + chn * 4));
    PWMx->PCSR = reg_value;

    DEBUG_ASSERT(config->clock_freq <= drv_get_apb_freq());

    clk_div = drv_get_apb_freq() / config->clock_freq;
    clk_div = (clk_div > 0) ? (clk_div - 1) : 0;

    reg_value = PWMx->PPR;
    reg_value &= ~(0xff << (PWM_PPR_CP0_POS + (chn / 2) * 8));
    reg_value |= (clk_div << (PWM_PPR_CP0_POS + (chn / 2) * 8));
    PWMx->PPR = reg_value;

    if (ePwmMode_Compare == config->mode) {
        DEBUG_ASSERT(config->out.duty <= 1000);

        reg_value = PWMx->PPCR;
        SET_BIT(reg_value, (PWM_PPCR_PDDR_POS + chn), PDDR_OUTPUT);
        SET_BIT(reg_value, (PWM_PPCR_PULLUP_EN_POS + chn), config->out.pull_up);
        PWMx->PPCR = reg_value;

        if (config->out.dead_time > 0) {
#ifdef DEBUG
            xprintf("PWMx chn[%d]: Old DZ Register = %08x\n", chn, PWMx->DZ);
#endif
            reg_value = PWMx->DZ;
            reg_value &= ~(0xff << (PWM_DZ_DZI0_POS + (chn / 2) * 8));
            reg_value |= (config->out.dead_time << (PWM_DZ_DZI0_POS + (chn / 2) * 8));
            PWMx->DZ = reg_value;
#ifdef DEBUG
            xprintf("PWMx chn[%d]: New DZ Register = %08x\n", chn, PWMx->DZ);
#endif
        }

        pwm_div = (config->clock_freq / config->out.freq);
        pwm_div = (pwm_div > 0) ? (pwm_div - 1) : 0;

        config->out.ticks = (pwm_div + 1);

        reg_value = PWMx->chn[chn % 4].PCNR;
        reg_value &= ~(0xffff << ((chn / 4) * 16));
        reg_value |= (pwm_div << ((chn / 4) * 16));
        PWMx->chn[chn % 4].PCNR = reg_value;

        pwm_div = (config->clock_freq / config->out.freq) * config->out.duty / 1000;
        pwm_div = (pwm_div > 0) ? (pwm_div - 1) : 0;

        reg_value = PWMx->chn[chn % 4].PCMR;
        reg_value &= ~(0xffff << ((chn / 4) * 16));
        reg_value |= (pwm_div << ((chn / 4) * 16));
        PWMx->chn[chn % 4].PCMR = reg_value;

        switch (chn) {
        case ePwmChannel_0:
            SET_BIT(PWMx->PCR, PWM_PCR_CH0MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR, PWM_PCR_CH0INV_POS, config->out.invert);
            SET_BIT(PWMx->PCR, PWM_PCR_DZEN0_POS, (config->out.dead_time > 0));
            break;
        case ePwmChannel_1:
            SET_BIT(PWMx->PCR, PWM_PCR_CH1MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR, PWM_PCR_CH1INV_POS, config->out.invert);
            break;
        case ePwmChannel_2:
            SET_BIT(PWMx->PCR, PWM_PCR_CH2MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR, PWM_PCR_CH2INV_POS, config->out.invert);
            SET_BIT(PWMx->PCR, PWM_PCR_DZEN1_POS, (config->out.dead_time > 0));
            break;
        case ePwmChannel_3:
            SET_BIT(PWMx->PCR, PWM_PCR_CH3MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR, PWM_PCR_CH3INV_POS, config->out.invert);
            break;
        case ePwmChannel_4:
            SET_BIT(PWMx->PCR1, PWM_PCR_CH4MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR1, PWM_PCR_CH4INV_POS, config->out.invert);
            SET_BIT(PWMx->PCR1, PWM_PCR_DZEN2_POS, (config->out.dead_time > 0));
            break;
        case ePwmChannel_5:
            SET_BIT(PWMx->PCR1, PWM_PCR_CH5MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR1, PWM_PCR_CH5INV_POS, config->out.invert);
            break;
        case ePwmChannel_6:
            SET_BIT(PWMx->PCR1, PWM_PCR_CH6MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR1, PWM_PCR_CH6INV_POS, config->out.invert);
            SET_BIT(PWMx->PCR1, PWM_PCR_DZEN3_POS, (config->out.dead_time > 0));
            break;
        case ePwmChannel_7:
            SET_BIT(PWMx->PCR1, PWM_PCR_CH7MOD_POS, GET_BIT(~config->out.single_pulse, 0));
            SET_BIT(PWMx->PCR1, PWM_PCR_CH7INV_POS, config->out.invert);
            break;
        default:
            break;
        }
    } else {
        reg_value = PWMx->PPCR;
        SET_BIT(reg_value, (PWM_PPCR_PDDR_POS + chn), PDDR_INPUT);
        SET_BIT(reg_value, (PWM_PPCR_PULLUP_EN_POS + chn), PULLUP_EN_DISABLED);
        PWMx->PPCR = reg_value;

        reg_value = PWMx->chn[chn % 4].PCNR;
        reg_value |= (0xffff << ((chn / 4) * 16));
        PWMx->chn[chn % 4].PCNR = reg_value;

        reg_value = PWMx->PCCR[chn / 2];
        SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_INV0_POS, PWM_DISABLED);
        if (config->in.polarity == ePwmPolarity_Rising) {
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_RL_IE0_POS, PWM_ENABLED);
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_FL_IE0_POS, PWM_DISABLED);
        } else if (config->in.polarity == ePwmPolarity_Falling) {
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_RL_IE0_POS, PWM_DISABLED);
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_FL_IE0_POS, PWM_ENABLED);
        } else if (config->in.polarity == ePwmPolarity_Both) {
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_RL_IE0_POS, PWM_ENABLED);
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_FL_IE0_POS, PWM_ENABLED);
        } else {
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_RL_IE0_POS, PWM_DISABLED);
            SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_FL_IE0_POS, PWM_DISABLED);
        }
        SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_CAPIF0_POS, 1); // NOTE: write '1' to clear interrupt flag
        SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_CRLRD0_POS, 1);
        SET_BIT(reg_value, (chn % 2) * 16 + PWM_PCCR_CFLRD0_POS, 1);
        PWMx->PCCR[chn / 2] = reg_value;

        switch ((uint32_t)PWMx) {
        case (uint32_t)PWM0:
            drv_irq_enable(PWM0_IRQn);
            break;
        case (uint32_t)PWM1:
            drv_irq_enable(PWM1_IRQn);
            break;
        case (uint32_t)PWM2:
            drv_irq_enable(PWM2_IRQn);
            break;
        case (uint32_t)PWM3:
            drv_irq_enable(PWM3_IRQn);
            break;
        case (uint32_t)PWM4:
            drv_irq_enable(PWM4_IRQn);
            break;
        case (uint32_t)PWM5:
            drv_irq_enable(PWM5_IRQn);
            break;
        default:
            break;
        }
    }

    SET_BIT(PWMx->PDER, chn, PWM_DISABLED);
    PWMx->PDCR[chn] = 0;

    config->status = 0;
    return 0;
}

// Note:
//  1. when config complementary pwm output, make sure that '.deadzone > 0'

/*
功能描述：

名称：HW_PWM_CompInitConfig
功能：初始化互补PWM配置
参数：
  - PWMx: 指向特定PWM端口的指针
  - chn: PWM通道号
  - config: 指向PWM配置结构体的指针
返回值：成功返回0，失败返回-1
输入：PWM端口、通道号和配置信息
输出：根据输入的配置初始化互补PWM
上层函数：可能由设备初始化函数或系统启动配置调用
下层函数：调用HW_PWM_InitConfig配置具体通道

附加说明：
- 该函数初始化互补PWM通道，确保两个通道的配置相同，包括死区时间等参数
*/
int HW_PWM_CompInitConfig(PWM_TypeDef *PWMx, PwmChannel_e chn, DevicePwmConfig_t *config)
{
    if ((NULL == PWMx) || (chn >= ePwmChannel_4) || (NULL == config)) {
        return -1;
    }

    HW_PWM_InitConfig(PWMx, (chn * 2), config);
    HW_PWM_InitConfig(PWMx, (chn * 2 + 1), config);

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：HW_PWM_Start
功能：启动指定的PWM通道
参数：
  - PWMx: 指向特定PWM端口的指针
  - chn: PWM通道号
返回值：成功返回0
输入：PWM端口和通道号
输出：启动指定的PWM通道
上层函数：可能由需要启动PWM的功能调用
下层函数：无

附加说明：
- 该函数通过设置相关寄存器，启动指定的PWM通道进行信号输出
*/
int HW_PWM_Start(PWM_TypeDef *PWMx, PwmChannel_e chn)
{
    switch (chn) {
    case ePwmChannel_0:
        SET_BIT(PWMx->PCR, PWM_PCR_CH0EN_POS, CHEN_ENABLED);
        break;
    case ePwmChannel_1:
        SET_BIT(PWMx->PCR, PWM_PCR_CH1EN_POS, CHEN_ENABLED);
        break;
    case ePwmChannel_2:
        SET_BIT(PWMx->PCR, PWM_PCR_CH2EN_POS, CHEN_ENABLED);
        break;
    case ePwmChannel_3:
        SET_BIT(PWMx->PCR, PWM_PCR_CH3EN_POS, CHEN_ENABLED);
        break;
    case ePwmChannel_4:
        SET_BIT(PWMx->PCR, PWM_PCR_CH4EN_POS, CHEN_ENABLED);
        break;
    case ePwmChannel_5:
        SET_BIT(PWMx->PCR, PWM_PCR_CH5EN_POS, CHEN_ENABLED);
        break;
    case ePwmChannel_6:
        SET_BIT(PWMx->PCR, PWM_PCR_CH6EN_POS, CHEN_ENABLED);
        break;
    case ePwmChannel_7:
        SET_BIT(PWMx->PCR, PWM_PCR_CH7EN_POS, CHEN_ENABLED);
        break;
    default:
        break;
    }

    return 0;
}

int HW_PWM_CompStart(PWM_TypeDef *PWMx, PwmChannel_e chn)
{
    if (chn >= ePwmChannel_4) {
        return -1;
    }

    HW_PWM_Start(PWMx, (chn * 2));
    HW_PWM_Start(PWMx, (chn * 2 + 1));

    return 0;
}

int HW_PWM_CaptureStart(PWM_TypeDef *PWMx, PwmChannel_e chn)
{
    switch (chn) {
    case ePwmChannel_0:
        SET_BIT(PWMx->PCCR[0], PWM_PCCR_CAPCH0_EN_POS, CAPCHEN_ENABLED);
        break;
    case ePwmChannel_1:
        SET_BIT(PWMx->PCCR[0], PWM_PCCR_CAPCH1_EN_POS, CAPCHEN_ENABLED);
        break;
    case ePwmChannel_2:
        SET_BIT(PWMx->PCCR[1], PWM_PCCR_CAPCH2_EN_POS, CAPCHEN_ENABLED);
        break;
    case ePwmChannel_3:
        SET_BIT(PWMx->PCCR[1], PWM_PCCR_CAPCH3_EN_POS, CAPCHEN_ENABLED);
        break;
    case ePwmChannel_4:
        SET_BIT(PWMx->PCCR[2], PWM_PCCR_CAPCH4_EN_POS, CAPCHEN_ENABLED);
        break;
    case ePwmChannel_5:
        SET_BIT(PWMx->PCCR[2], PWM_PCCR_CAPCH5_EN_POS, CAPCHEN_ENABLED);
        break;
    case ePwmChannel_6:
        SET_BIT(PWMx->PCCR[3], PWM_PCCR_CAPCH6_EN_POS, CAPCHEN_ENABLED);
        break;
    case ePwmChannel_7:
        SET_BIT(PWMx->PCCR[3], PWM_PCCR_CAPCH7_EN_POS, CAPCHEN_ENABLED);
        break;
    default:
        break;
    }

    return HW_PWM_Start(PWMx, chn);
}

/*
功能描述：

名称：HW_PWM_Stop
功能：停止指定的PWM通道
参数：
  - PWMx: 指向特定PWM端口的指针
  - chn: PWM通道号
返回值：成功返回0
输入：PWM端口和通道号
输出：停止指定的PWM通道
上层函数：可能由需要停止PWM的功能调用
下层函数：无

附加说明：
- 该函数通过设置相关寄存器，停止指定的PWM通道，停止信号输出
*/
int HW_PWM_Stop(PWM_TypeDef *PWMx, PwmChannel_e chn)
{
    switch (chn) {
    case ePwmChannel_0:
        SET_BIT(PWMx->PCR, PWM_PCR_CH0EN_POS, CHEN_DISABLED);
        break;
    case ePwmChannel_1:
        SET_BIT(PWMx->PCR, PWM_PCR_CH1EN_POS, CHEN_DISABLED);
        break;
    case ePwmChannel_2:
        SET_BIT(PWMx->PCR, PWM_PCR_CH2EN_POS, CHEN_DISABLED);
        break;
    case ePwmChannel_3:
        SET_BIT(PWMx->PCR, PWM_PCR_CH3EN_POS, CHEN_DISABLED);
        break;
    case ePwmChannel_4:
        SET_BIT(PWMx->PCR, PWM_PCR_CH4EN_POS, CHEN_DISABLED);
        break;
    case ePwmChannel_5:
        SET_BIT(PWMx->PCR, PWM_PCR_CH5EN_POS, CHEN_DISABLED);
        break;
    case ePwmChannel_6:
        SET_BIT(PWMx->PCR, PWM_PCR_CH6EN_POS, CHEN_DISABLED);
        break;
    case ePwmChannel_7:
        SET_BIT(PWMx->PCR, PWM_PCR_CH7EN_POS, CHEN_DISABLED);
        break;
    default:
        break;
    }

    return 0;
}

int HW_PWM_CompStop(PWM_TypeDef *PWMx, PwmChannel_e chn)
{
    if (chn >= ePwmChannel_4) {
        return -1;
    }

    HW_PWM_Stop(PWMx, (chn * 2));
    HW_PWM_Stop(PWMx, (chn * 2 + 1));

    return 0;
}

int HW_PWM_CaptureStop(PWM_TypeDef *PWMx, PwmChannel_e chn)
{
    switch (chn) {
    case ePwmChannel_0:
        SET_BIT(PWMx->PCCR[0], PWM_PCCR_CAPCH0_EN_POS, CAPCHEN_DISABLED);
        break;
    case ePwmChannel_1:
        SET_BIT(PWMx->PCCR[0], PWM_PCCR_CAPCH1_EN_POS, CAPCHEN_DISABLED);
        break;
    case ePwmChannel_2:
        SET_BIT(PWMx->PCCR[1], PWM_PCCR_CAPCH2_EN_POS, CAPCHEN_DISABLED);
        break;
    case ePwmChannel_3:
        SET_BIT(PWMx->PCCR[1], PWM_PCCR_CAPCH3_EN_POS, CAPCHEN_DISABLED);
        break;
    case ePwmChannel_4:
        SET_BIT(PWMx->PCCR[2], PWM_PCCR_CAPCH4_EN_POS, CAPCHEN_DISABLED);
        break;
    case ePwmChannel_5:
        SET_BIT(PWMx->PCCR[2], PWM_PCCR_CAPCH5_EN_POS, CAPCHEN_DISABLED);
        break;
    case ePwmChannel_6:
        SET_BIT(PWMx->PCCR[3], PWM_PCCR_CAPCH6_EN_POS, CAPCHEN_DISABLED);
        break;
    case ePwmChannel_7:
        SET_BIT(PWMx->PCCR[3], PWM_PCCR_CAPCH7_EN_POS, CAPCHEN_DISABLED);
        break;
    default:
        break;
    }

    return HW_PWM_Stop(PWMx, chn);
}

// Note:
//  1. 'fval' and 'rval' is the converted pulse width
//  2. using which one is depended on '.polarity'

/*
功能描述：

名称：HW_PWM_GetCaptureValue
功能：获取PWM捕获的值
参数：
  - PWMx: 指向特定PWM端口的指针
  - chn: PWM通道号
  - fval: 指向存储前沿捕获值的指针
  - rval: 指向存储后沿捕获值的指针
返回值：返回捕获周期值
输入：PWM端口、通道号、前沿捕获值指针和后沿捕获值指针
输出：返回捕获周期值，并通过指针输出捕获的前沿和后沿值
上层函数：可能由需要获取PWM捕获值的功能调用
下层函数：无

附加说明：
- 该函数通过读取相关寄存器，获取指定PWM通道的捕获值，用于测量输入信号的脉宽
*/
uint32_t HW_PWM_GetCaptureValue(PWM_TypeDef *PWMx, PwmChannel_e chn, uint32_t *fval, uint32_t *rval)
{
    uint32_t reg_value[2];

    switch (chn) {
    case ePwmChannel_0:
        reg_value[0] = (PWMx->PCRLR0) & 0xffff;
        reg_value[1] = (PWMx->PCFLR0) & 0xffff;
        break;
    case ePwmChannel_1:
        reg_value[0] = (PWMx->PCRLR1) & 0xffff;
        reg_value[1] = (PWMx->PCFLR1) & 0xffff;
        break;
    case ePwmChannel_2:
        reg_value[0] = (PWMx->PCRLR2) & 0xffff;
        reg_value[1] = (PWMx->PCFLR2) & 0xffff;
        break;
    case ePwmChannel_3:
        reg_value[0] = (PWMx->PCRLR3) & 0xffff;
        reg_value[1] = (PWMx->PCFLR3) & 0xffff;
        break;
    case ePwmChannel_4:
        reg_value[0] = (PWMx->PCRLR0 >> 16) & 0xffff;
        reg_value[1] = (PWMx->PCFLR0 >> 16) & 0xffff;
        break;
    case ePwmChannel_5:
        reg_value[0] = (PWMx->PCRLR1 >> 16) & 0xffff;
        reg_value[1] = (PWMx->PCFLR1 >> 16) & 0xffff;
        break;
    case ePwmChannel_6:
        reg_value[0] = (PWMx->PCRLR2 >> 16) & 0xffff;
        reg_value[1] = (PWMx->PCFLR2 >> 16) & 0xffff;
        break;
    case ePwmChannel_7:
        reg_value[0] = (PWMx->PCRLR3 >> 16) & 0xffff;
        reg_value[1] = (PWMx->PCFLR3 >> 16) & 0xffff;
        break;
    default:
        break;
    }

    reg_value[0] = 0xffff + 1 - reg_value[0];
    reg_value[1] = 0xffff + 1 - reg_value[1];

    if (fval)
        *fval = reg_value[0];
    if (rval)
        *rval = reg_value[1];

    return (reg_value[0] + reg_value[1]);
}

// Note:
//  1. this function only clear capture interrupt flag, not pwm timer interrupt
//  2. call this function will reload the CNRx

/*
功能描述：

名称：HW_PWM_ClearInterrupt
功能：清除PWM捕获中断标志
参数：
  - PWMx: 指向特定PWM端口的指针
  - chn: PWM通道号
返回值：成功返回0
输入：PWM端口和通道号
输出：清除指定PWM通道的捕获中断标志
上层函数：可能由需要清除PWM捕获中断标志的功能调用
下层函数：无

附加说明：
- 该函数通过设置相关寄存器，清除指定的PWM通道的捕获中断标志，用于处理捕获中断
*/
uint32_t HW_PWM_ClearInterrupt(PWM_TypeDef *PWMx, PwmChannel_e chn)
{
    switch (chn) {
    case ePwmChannel_0:
        SET_BIT(PWMx->PCCR[0], PWM_PCCR_CAPIF0_POS, 1);
        break;
    case ePwmChannel_1:
        SET_BIT(PWMx->PCCR[0], PWM_PCCR_CAPIF1_POS, 1);
        break;
    case ePwmChannel_2:
        SET_BIT(PWMx->PCCR[1], PWM_PCCR_CAPIF2_POS, 1);
        break;
    case ePwmChannel_3:
        SET_BIT(PWMx->PCCR[1], PWM_PCCR_CAPIF3_POS, 1);
        break;
    case ePwmChannel_4:
        SET_BIT(PWMx->PCCR[2], PWM_PCCR_CAPIF4_POS, 1);
        break;
    case ePwmChannel_5:
        SET_BIT(PWMx->PCCR[2], PWM_PCCR_CAPIF5_POS, 1);
        break;
    case ePwmChannel_6:
        SET_BIT(PWMx->PCCR[3], PWM_PCCR_CAPIF6_POS, 1);
        break;
    case ePwmChannel_7:
        SET_BIT(PWMx->PCCR[3], PWM_PCCR_CAPIF7_POS, 1);
        break;
    default:
        break;
    }

    return 0;
}

#ifdef DEBUG_PWM

#include "xprintf.h"

int selftest_pwm(void)
{
    PWM_TypeDef *pwm = PWM0;

    xprintf("PWM VERSION = %08x\n", DEFAULT_VERSION_AND_ID);
    xprintf("PWM ID = %08x\n", DEFAULT_VERSION_AND_ID);

    for (PwmChannel_e chn = ePwmChannel_0; chn < ePwmChannel_Max; chn++) {
        HW_PWM_Stop(pwm, chn);
        HW_PWM_CaptureStop(pwm, chn);
    }

#if (DEBUG_PWM_CASE == DEBUG_PWM_CASE_COMPARE || DEBUG_PWM_CASE == DEBUG_PWM_CASE_CAPTURE)
    g_DevicePwmDefaultConfig.out.duty = 230; // NOTE: duty is restricted by clock frequency
    HW_PWM_InitConfig(pwm, ePwmChannel_0, &g_DevicePwmDefaultConfig);

#if (DEBUG_PWM_CASE == DEBUG_PWM_CASE_COMPARE)
    g_DevicePwmDefaultConfig.out.duty = 320;
    HW_PWM_InitConfig(pwm, ePwmChannel_1, &g_DevicePwmDefaultConfig);
#else
    DevicePwmConfig_t g_DevicePwmCapture1Config =
        {
            .clock_freq = 10 * 1000 * 1000,
            .mode = ePwmMode_Capture,
            .in.polarity = ePwmPolarity_Both,
        };
    HW_PWM_InitConfig(pwm, ePwmChannel_1, &g_DevicePwmCapture1Config); // NOTE: PWM[0] out to PWM[1]
#endif // #if ( DEBUG_PWM_CASE == DEBUG_PWM_CASE_COMPARE )

    g_DevicePwmDefaultConfig.out.duty = 400;
    HW_PWM_InitConfig(pwm, ePwmChannel_2, &g_DevicePwmDefaultConfig);

    g_DevicePwmDefaultConfig.out.duty = 500;
    HW_PWM_InitConfig(pwm, ePwmChannel_3, &g_DevicePwmDefaultConfig);

    g_DevicePwmDefaultConfig.out.duty = 555;
    HW_PWM_InitConfig(pwm, ePwmChannel_4, &g_DevicePwmDefaultConfig);

    g_DevicePwmDefaultConfig.out.duty = 63;
    HW_PWM_InitConfig(pwm, ePwmChannel_5, &g_DevicePwmDefaultConfig);

    g_DevicePwmDefaultConfig.out.duty = 750;
    HW_PWM_InitConfig(pwm, ePwmChannel_6, &g_DevicePwmDefaultConfig);

    g_DevicePwmDefaultConfig.out.duty = 800;
    HW_PWM_InitConfig(pwm, ePwmChannel_7, &g_DevicePwmDefaultConfig);

    for (PwmChannel_e chn = ePwmChannel_0; chn < ePwmChannel_Max; chn++) {
#if (DEBUG_PWM_CASE == DEBUG_PWM_CASE_CAPTURE)
        if (chn == ePwmChannel_1) {
            HW_PWM_CaptureStart(pwm, chn);
        } else
#endif // #if ( DEBUG_PWM_CASE == DEBUG_PWM_CASE_CAPTURE )
        {
            HW_PWM_Start(pwm, chn);
        }
    }
#endif // #if ( DEBUG_PWM_CASE == DEBUG_PWM_CASE_COMPARE || DEBUG_PWM_CASE == DEBUG_PWM_CASE_CAPTURE )

#if (DEBUG_PWM_CASE == DEBUG_PWM_CASE_COMPARE_COMPLEMENT)
    g_DevicePwmDefaultConfig.out.duty = 300;
    g_DevicePwmDefaultConfig.out.dead_time = 10;
    g_DevicePwmDefaultConfig.out.invert = 1;
    HW_PWM_CompInitConfig(pwm, ePwmChannel_0, &g_DevicePwmDefaultConfig);

    HW_PWM_CompStart(pwm, ePwmChannel_0);
#endif // #if ( DEBUG_PWM_CASE == DEBUG_PWM_CASE_COMPARE_COMPLEMENT )

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_PWM
