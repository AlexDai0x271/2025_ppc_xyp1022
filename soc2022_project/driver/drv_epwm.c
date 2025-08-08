/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_epwm.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

/*
功能描述：

名称：g_DeviceEPwmDefaultConfig
功能：定义EPWM设备的默认配置，仅在调试模式下使用
参数：
  - clock_freq: 时钟频率，设置为10MHz
  - mode: EPWM模式，设置为比较模式
  - out.freq: 输出频率，设置为100kHz
  - out.single_pulse: 单脉冲模式，设置为0
  - out.break_enable: 断裂保护使能，设置为0
  - out.break_polarity: 断裂保护极性，设置为1
  - out.dead_time: 死区时间，设置为0（单位为纳秒）
  - out.param[eEPwmChannel_0]: 频道0的输出参数
    - duty: 占空比，设置为120（表示12.0%）
    - oc_active: 输出比较激活，设置为1
    - ocn_active: 输出比较反激活，设置为0
    - delay_rate: 延迟率，设置为0（表示0.0%）
    - pull_up: 上拉使能，设置为1
  - out.param[eEPwmChannel_1]: 频道1的输出参数
    - duty: 占空比，设置为250（表示25.0%）
    - oc_active: 输出比较激活，设置为1
    - ocn_active: 输出比较反激活，设置为0
    - delay_rate: 延迟率，设置为0（表示0.0%）
    - pull_up: 上拉使能，设置为1
  - out.param[eEPwmChannel_2]: 频道2的输出参数
    - duty: 占空比，设置为500（表示50.0%）
    - oc_active: 输出比较激活，设置为1
    - ocn_active: 输出比较反激活，设置为0
    - delay_rate: 延迟率，设置为0（表示0.0%）
    - pull_up: 上拉使能，设置为1
  - out.param[eEPwmChannel_3]: 频道3的输出参数
    - duty: 占空比，设置为760（表示76.0%）
    - oc_active: 输出比较激活，设置为1
    - ocn_active: 输出比较反激活，设置为0
    - delay_rate: 延迟率，设置为0（表示0.0%）
    - pull_up: 上拉使能，设置为1
  - user_data: 用户自定义数据，初始设置为0
返回值：无
输入：无
输出：无
上层函数：无
下层函数：可能被EPWM初始化函数如HW_EPWM_InitConfig使用来应用这些默认设置

附加说明：
- 这个配置结构体通常用于初始化EPWM设备，配置参数适用于基本的EPWM操作和信号生成需求
- 在非调试模式下，应避免使用这些默认参数，以免影响生产环境下的设备行为
- 结构体中的参数可以根据实际硬件和项目需求进行调整
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceEPwmConfig_t g_DeviceEPwmDefaultConfig =
    {
        .clock_freq = 10 * 1000 * 1000,

        .mode = eEPwmMode_Compare,

        .out.freq = 100 * 1000,
        .out.single_pulse = 0,
        .out.break_enable = 0,
        .out.break_polarity = 1,
        .out.dead_time = 0, // NOTE: how many 1-ns

        .out.param[eEPwmChannel_0] = {
                                      .duty = 120,     // NOTE: one decimal precision
            .oc_active = 1,
                                      .ocn_active = 0,
                                      .delay_rate = 0, // NOTE: one decimal precision
            .pull_up = 1,
                                      },
        .out.param[eEPwmChannel_1] = {
                                      .duty = 250,
                                      .oc_active = 1,
                                      .ocn_active = 0,
                                      .delay_rate = 0,
                                      .pull_up = 1,
                                      },
        .out.param[eEPwmChannel_2] = {
                                      .duty = 500,
                                      .oc_active = 1,
                                      .ocn_active = 0,
                                      .delay_rate = 0,
                                      .pull_up = 1,
                                      },
        .out.param[eEPwmChannel_3] = {
                                      .duty = 760,
                                      .oc_active = 1,
                                      .ocn_active = 0,
                                      .delay_rate = 0,
                                      .pull_up = 1,
                                      },

        .user_data = 0,
};
#endif // #ifdef DEBUG

static uint32_t s_last_capture_value[NUM_OF_EPWM][eEPwmChannel_Max];

/*功能描述：

名称：GetIndex
功能：根据EPWM模块的地址获取其索引
参数：
  - EPWMx: 指向EPWM模块的指针
返回值：EPWM模块的索引
输入：EPWM模块的指针
输出：EPWM模块的索引
上层函数：多个EPWM相关函数，如HW_EPWM_InitConfig等
下层函数：无

附加说明：
- 此函数用于将EPWM的内存地址映射到其对应的索引，通常用于数组操作或特定逻辑判断
*/
static uint32_t GetIndex(EPWM_TypeDef *EPWMx)
{
    uint32_t index;

    switch ((uint32_t)EPWMx) {
    case (uint32_t)EPWM0:
        index = 0;
        break;
    case (uint32_t)EPWM1:
        index = 1;
        break;
    case (uint32_t)EPWM2:
        index = 2;
        break;
    case (uint32_t)EPWM3:
        index = 3;
        break;
    case (uint32_t)EPWM4:
        index = 4;
        break;
    case (uint32_t)EPWM5:
        index = 5;
        break;
    case (uint32_t)EPWM6:
        index = 6;
        break;
    case (uint32_t)EPWM7:
        index = 7;
        break;
    default:
        index = 0;
        break;
    }

    return index;
}

/*
功能描述：

名称：HW_EPWM_InitConfig
功能：初始化指定的EPWM模块及其配置
参数：
  - EPWMx: 指向EPWM模块的指针
  - config: 指向EPWM配置结构的指针
返回值：成功返回0，失败返回-1
输入：EPWM模块的指针和配置结构的指针
输出：配置应用于EPWM模块
上层函数：可能被高层初始化或配置更改函数调用
下层函数：GetIndex、HW_EPWM_Stop、HW_EPWM_CaptureStop、HW_EPWM_Lock、HW_EPWM_Unlock等

附加说明：
- 该函数用于根据提供的配置参数设置EPWM模块，包括时钟设置、模式选择、中断配置等
- 在配置过程中，首先禁用EPWM以避免配置时的错误触发
- 根据模式选择，可能配置为比较模式或捕获模式等
*/
int HW_EPWM_InitConfig(EPWM_TypeDef *EPWMx, DeviceEPwmConfig_t *config)
{
    uint32_t clk_div;
    uint32_t pwm_div;
    uint32_t reg_offset;
    uint32_t reg_value;
    uint32_t dtg_value;
    EPwmChannel_e chn;

    if ((NULL == EPWMx) || (NULL == config)) {
        return -1;
    }

    // disable pwm first
    for (chn = eEPwmChannel_0; chn < eEPwmChannel_Max; chn++) {
        HW_EPWM_Stop(EPWMx, chn);
        HW_EPWM_CaptureStop(EPWMx, chn);
    }

    SET_BIT(EPWMx->CR1, EPWM_CR1_CEN_POS, EPWM_DISABLED);

    reg_value = 0; // .CR1 inited to zero first
    reg_value |= (CKD_CLOCK_DIV1 << EPWM_CR1_CKD_POS);
    reg_value |= (CMS_ALIGN_EDGE << EPWM_CR1_CMS_POS);
    SET_BIT(reg_value, EPWM_CR1_ARPE_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CR1_DIR_POS, DIR_DOWNCOUNTER);
    SET_BIT(reg_value, EPWM_CR1_OPM_POS, ((eEPwmMode_Compare == config->mode) ? config->out.single_pulse : 0));
    SET_BIT(reg_value, EPWM_CR1_OPM_POS, ((eEPwmMode_Compare == config->mode) ? config->out.single_pulse : 0));
    SET_BIT(reg_value, EPWM_CR1_URS_POS, URS_ONLY);
    SET_BIT(reg_value, EPWM_CR1_UDIS_POS, UDIS_ENABLED);
    SET_BIT(reg_value, EPWM_CR1_CEN_POS, EPWM_DISABLED); // NOTE: enable in 'HW_EPWM_AllStart'
    EPWMx->CR1 = reg_value;

    reg_value = 0;                                 // .CR2 inited to zero first
    reg_value |= (MMS_CNT_EN << EPWM_CR2_MMS_POS); // NOTE: not use 'TRGO' to trig external timer
    SET_BIT(reg_value, EPWM_CR2_OIS4N_POS, 0);     // NOTE: output level in idle state
    SET_BIT(reg_value, EPWM_CR2_OIS4_POS, 0);
    SET_BIT(reg_value, EPWM_CR2_OIS3N_POS, 0);
    SET_BIT(reg_value, EPWM_CR2_OIS3_POS, 0);
    SET_BIT(reg_value, EPWM_CR2_OIS2N_POS, 0);
    SET_BIT(reg_value, EPWM_CR2_OIS2_POS, 0);
    SET_BIT(reg_value, EPWM_CR2_OIS1N_POS, 0);
    SET_BIT(reg_value, EPWM_CR2_OIS1_POS, 0);
    SET_BIT(reg_value, EPWM_CR2_TI1S_POS, TI1S_CH1);
    SET_BIT(reg_value, EPWM_CR2_CCDS_POS, CCDS_UEV);
    SET_BIT(reg_value, EPWM_CR2_CCUS_POS, CCUS_COMG); // NOTE: only valid when 'CCPC = 1'
    SET_BIT(reg_value, EPWM_CR2_CCPC_POS, EPWM_DISABLED);
    EPWMx->CR2 = reg_value;

    EPWMx->SMCR = 0; // NOTE: not use internal trigger
    EPWMx->DIER = 0; // NOTE: not use dma

    reg_value = EPWMx->SR;
    UNUSED(reg_value);

    EPWMx->EGR = 0; // NOTE: clear all event generation
    EPWMx->RCR = 0; // NOTE: invalid when preload registers are disable

    HW_EPWM_Unlock(EPWMx);

    if (eEPwmMode_Compare == config->mode) {
        DEBUG_ASSERT(config->clock_freq <= drv_get_apb_freq());

        clk_div = drv_get_apb_freq() / config->clock_freq;
        clk_div = (clk_div > 0) ? (clk_div - 1) : 0;

        EPWMx->PSC = clk_div;

        pwm_div = (config->clock_freq / config->out.freq);
        pwm_div = (pwm_div > 0) ? (pwm_div - 1) : 0;

        EPWMx->CNT = pwm_div;
        EPWMx->ARR = pwm_div;

        dtg_value = config->out.dead_time / HW_EPWM_GetDeadTimeIntervalStep(EPWMx);
        DEBUG_ASSERT(dtg_value <= 0x7f);

        reg_value = EPWMx->BDTR;
        reg_value &= ~(0xff << EPWM_BDTR_DTG_POS);
        reg_value |= (dtg_value << EPWM_BDTR_DTG_POS);
        SET_BIT(reg_value, EPWM_BDTR_MOE_POS, EPWM_ENABLED);
        SET_BIT(reg_value, EPWM_BDTR_AOE_POS, EPWM_ENABLED);
        SET_BIT(reg_value, EPWM_BDTR_BKP_POS, (config->out.break_enable && config->out.break_polarity));
        SET_BIT(reg_value, EPWM_BDTR_BKE_POS, config->out.break_enable);
        SET_BIT(reg_value, EPWM_BDTR_OSSR_POS, EPWM_ENABLED);
        SET_BIT(reg_value, EPWM_BDTR_OSSI_POS, EPWM_DISABLED);
        EPWMx->BDTR = reg_value;

        EPWMx->DIER = 0; // NOTE: not use interrupt in compare mode

        for (chn = eEPwmChannel_0; chn < eEPwmChannel_Max; chn++) {
            DEBUG_ASSERT(config->out.param[chn].duty <= 1000);

            pwm_div = (config->clock_freq / config->out.freq) * config->out.param[chn].duty / 1000;
            pwm_div = (pwm_div > 0) ? (pwm_div - 1) : 0;

            EPWMx->CCR[chn] = pwm_div;

            reg_value = EPWMx->CCER;
            reg_offset = (chn * 4);
            reg_value &= ~(0xf << reg_offset);
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NP_POS, config->out.param[chn].ocn_active);
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NE_POS, EPWM_DISABLED);
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1P_POS, config->out.param[chn].oc_active);
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1E_POS, EPWM_DISABLED); // NOTE: enable in 'HW_EPWM_AllStart'
            EPWMx->CCER = reg_value;

            reg_value = EPWMx->CCMR[chn / 2];
            reg_offset = ((chn % 2) * 8);
            reg_value &= ~(0xff << reg_offset);
            SET_BIT(reg_value, reg_offset + EPWM_CCMR1_OC1CE_POS, OCCE_IGNORED);
            SET_BIT(reg_value, reg_offset + EPWM_CCMR1_OC1PE_POS, EPWM_ENABLED);
            SET_BIT(reg_value, reg_offset + EPWM_CCMR1_OC1FE_POS, EPWM_DISABLED); // NOTE: not use fast mode
            reg_value |= (OCM_PWM_MODE1 << (reg_offset + EPWM_CCMR1_OC1M_POS));
            reg_value |= (CCS_OUTPUT << (reg_offset + EPWM_CCMR1_CC1S_POS));
            EPWMx->CCMR[chn / 2] = reg_value;

            DEBUG_ASSERT(config->out.param[chn].delay_rate <= 1000);

            reg_value = (EPWMx->ARR + 1);
            reg_value = (reg_value * config->out.param[chn].delay_rate) / 1000;
            DEBUG_ASSERT(reg_value <= 0xffff);

            EPWMx->PMCR[chn] = (reg_value << EPWM_PMCR_PMDV1_POS) | (0 << EPWM_PMCR_PMCR1_POS);

            reg_offset = (chn * 3);
            SET_BIT(EPWMx->PMCREN, reg_offset + EPWM_PMCREN_PMCR1N_DELAY_EN_POS, (reg_value != 0));
            SET_BIT(EPWMx->PMCREN, reg_offset + EPWM_PMCREN_PMCR1_DELAY_EN_POS, (reg_value != 0));
            SET_BIT(EPWMx->PMCREN, reg_offset + EPWM_PMCREN_PMCR1_EN_POS, EPWM_DISABLED);

            SET_BIT(EPWMx->PUE, chn, config->out.param[chn].pull_up);
        }
    } else {
        EPWMx->PSC = 0;
        config->clock_freq = drv_get_apb_freq() / (EPWMx->PSC + 1);

        EPWMx->CNT = 0;
        EPWMx->ARR = 0;

        EPWMx->DIER = 0;

        for (chn = eEPwmChannel_0; chn < eEPwmChannel_Max; chn++) {
            s_last_capture_value[GetIndex(EPWMx)][chn] = EPWMx->CCR[chn];

            HW_EPWM_ClearInterrupt(EPWMx, chn);

            reg_value = EPWMx->DIER;
            reg_value &= (0xf << EPWM_DIER_CC1IE_POS);
            SET_BIT(reg_value, EPWM_DIER_CC1IE_POS + chn, (config->in.polarity[chn] != eEPwmPolarity_Reserved));
            EPWMx->DIER = reg_value;

            reg_value = EPWMx->CCER;
            reg_offset = (chn * 4);
            reg_value &= ~(0xf << reg_offset);
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NP_POS, GET_BIT(config->in.polarity[chn], 1));
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NE_POS, EPWM_DISABLED);
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1P_POS, GET_BIT(config->in.polarity[chn], 0));
            SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1E_POS, EPWM_DISABLED);
            EPWMx->CCER = reg_value;

            reg_value = EPWMx->CCMR[chn / 2];
            reg_offset = ((chn % 2) * 8);
            reg_value &= ~(0xff << reg_offset);
            reg_value |= (ICF_DTS_N1 << (reg_offset + EPWM_CCMR1_IC1F_POS));
            reg_value |= (ICPSC_1_EVENT << (reg_offset + EPWM_CCMR1_IC1PSC_POS));
            reg_value |= (CCS_INPUT_TI1 << (reg_offset + EPWM_CCMR1_CC1S_POS));
            EPWMx->CCMR[chn / 2] = reg_value;
        }

        switch ((uint32_t)EPWMx) {
        case (uint32_t)EPWM0:
            drv_irq_enable(EPWM0_IRQn);
            break;
        case (uint32_t)EPWM1:
            drv_irq_enable(EPWM1_IRQn);
            break;
        case (uint32_t)EPWM2:
            drv_irq_enable(EPWM2_IRQn);
            break;
        case (uint32_t)EPWM3:
            drv_irq_enable(EPWM3_IRQn);
            break;
        case (uint32_t)EPWM4:
            drv_irq_enable(EPWM4_IRQn);
            break;
        case (uint32_t)EPWM5:
            drv_irq_enable(EPWM5_IRQn);
            break;
        case (uint32_t)EPWM6:
            drv_irq_enable(EPWM6_IRQn);
            break;
        case (uint32_t)EPWM7:
            drv_irq_enable(EPWM7_IRQn);
            break;
        default:
            break;
        }

        SET_BIT(EPWMx->CR1, EPWM_CR1_CEN_POS, EPWM_ENABLED);
    }

    HW_EPWM_Lock(EPWMx);

    // NOTE: start timer by call 'HW_EPWM_AllStart'

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：HW_EPWM_GetDeadTimeIntervalStep
功能：计算EPWM死区时间步长
参数：
  - EPWMx: 指向EPWM模块的指针
返回值：死区时间步长（以ns为单位）
输入：EPWM模块的指针
输出：计算出的死区时间步长
上层函数：配置死区时间时调用此函数
下层函数：无

附加说明：
- 此函数根据APB总线频率和EPWM预分频设置，计算每个死区时间单位的实际长度（纳秒）
*/
uint32_t HW_EPWM_GetDeadTimeIntervalStep(EPWM_TypeDef *EPWMx)
{
    uint32_t reg_value;
    uint32_t dts_value;

    reg_value = (EPWMx->CR1 >> EPWM_CR1_CKD_POS) & 0x3;
    DEBUG_ASSERT(reg_value <= CKD_CLOCK_DIV4);

    dts_value = (drv_get_apb_freq() / 1000000) << reg_value;

    return (1000 / dts_value);
}

/*
功能描述：

名称：HW_EPWM_Lock
功能：锁定EPWM模块的配置
参数：
  - EPWMx: 指向EPWM模块的指针
返回值：当前锁定状态
输入：EPWM模块的指针
输出：EPWM模块被锁定防止配置被修改
上层函数：通常在配置完成后调用此函数以防止误修改
下层函数：无

附加说明：
- 调用此函数后，EPWM模块的某些寄存器将无法修改，直到解锁
- 通常用于配置完成后提高系统稳定性
*/
uint32_t HW_EPWM_Lock(EPWM_TypeDef *EPWMx)
{
    uint32_t reg_value;

    reg_value = EPWMx->BDTR;
    reg_value &= ~(0x3 << EPWM_BDTR_LOCK_POS);
    reg_value &= ~(LOCK_LEVEL3 << EPWM_BDTR_LOCK_POS);
    EPWMx->BDTR = reg_value;

    return reg_value;
}

/*
功能描述：

名称：HW_EPWM_Unlock
功能：解锁EPWM模块的配置
参数：
  - EPWMx: 指向EPWM模块的指针
返回值：当前解锁状态
输入：EPWM模块的指针
输出：EPWM模块解锁允许修改配置
上层函数：在需要重新配置EPWM前调用此函数
下层函数：无

附加说明：
- 此函数允许修改之前通过HW_EPWM_Lock函数锁定的配置寄存器
- 在配置修改前调用，确保修改的有效性和安全性
*/
uint32_t HW_EPWM_Unlock(EPWM_TypeDef *EPWMx)
{
    uint32_t reg_value;

    reg_value = EPWMx->BDTR;
    reg_value &= ~(0x3 << EPWM_BDTR_LOCK_POS);
    reg_value &= ~(LOCK_OFF << EPWM_BDTR_LOCK_POS);
    EPWMx->BDTR = reg_value;

    return reg_value;
}

/*
功能描述：

名称：HW_EPWM_Start
功能：启动指定EPWM通道的PWM输出
参数：
  - EPWMx: 指向EPWM模块的指针
  - chn: 要启动的EPWM通道
返回值：成功返回0，失败返回-1
输入：EPWM模块的指针和通道号
输出：指定通道的PWM输出被启动
上层函数：在需要开始PWM信号输出时调用
下层函数：无

附加说明：
- 此函数用于启动指定的EPWM通道，使能其PWM输出
- 通过设置相关寄存器使PWM输出有效
*/
int HW_EPWM_Start(EPWM_TypeDef *EPWMx, EPwmChannel_e chn)
{
    uint32_t reg_value;
    uint32_t reg_offset;

    if (chn >= eEPwmChannel_Max) {
        return -1;
    }

    reg_value = EPWMx->CCER;
    reg_offset = (chn * 4);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NE_POS, EPWM_ENABLED);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1E_POS, EPWM_ENABLED);
    EPWMx->CCER = reg_value;

    return 0;
}

/*
功能描述：

名称：HW_EPWM_AllStart
功能：启动EPWM模块的所有通道
参数：
  - EPWMx: 指向EPWM模块的指针
返回值：操作成功返回0
输入：EPWM模块的指针
输出：所有PWM通道被启动
上层函数：在需要同时启动所有通道的PWM输出时调用
下层函数：无

附加说明：
- 该函数用于启动EPWM模块的所有PWM通道
- 适用于需要同步启动所有通道的情况
*/
int HW_EPWM_AllStart(EPWM_TypeDef *EPWMx)
{
    uint32_t reg_value;

    reg_value = EPWMx->CCER;
    SET_BIT(reg_value, EPWM_CCER_CC4NE_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CCER_CC4E_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CCER_CC3NE_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CCER_CC3E_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CCER_CC2NE_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CCER_CC2E_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CCER_CC1NE_POS, EPWM_ENABLED);
    SET_BIT(reg_value, EPWM_CCER_CC1E_POS, EPWM_ENABLED);
    EPWMx->CCER = reg_value;

    SET_BIT(EPWMx->CR1, EPWM_CR1_CEN_POS, EPWM_ENABLED);

    return 0;
}

/*
功能描述：

名称：HW_EPWM_CaptureStart
功能：启动EPWM通道的捕获功能
参数：
  - EPWMx: 指向EPWM模块的指针
  - chn: 要启动捕获的EPWM通道
返回值：成功返回0，失败返回-1
输入：EPWM模块的指针和
*/
int HW_EPWM_CaptureStart(EPWM_TypeDef *EPWMx, EPwmChannel_e chn)
{
    uint32_t reg_value;
    uint32_t reg_offset;

    if (chn >= eEPwmChannel_Max) {
        return -1;
    }

    reg_value = EPWMx->CCER;
    reg_offset = (chn * 4);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NE_POS, EPWM_DISABLED);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1E_POS, EPWM_ENABLED);
    EPWMx->CCER = reg_value;

    return 0;
}

/*
功能描述：

名称：HW_EPWM_Stop
功能：停止指定EPWM通道的PWM输出
参数：
  - EPWMx: 指向EPWM模块的指针
  - chn: 要停止的EPWM通道
返回值：成功返回0，失败返回-1
输入：EPWM模块的指针和通道号
输出：指定通道的PWM输出被停止
上层函数：在需要停止PWM信号输出时调用
下层函数：无

附加说明：
- 此函数用于停止指定的EPWM通道，禁用其PWM输出
- 通过清除相关寄存器位来禁用PWM输出
*/
int HW_EPWM_Stop(EPWM_TypeDef *EPWMx, EPwmChannel_e chn)
{
    uint32_t reg_value;
    uint32_t reg_offset;

    if (chn >= eEPwmChannel_Max) {
        return -1;
    }

    reg_value = EPWMx->CCER;
    reg_offset = (chn * 4);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NE_POS, EPWM_DISABLED);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1E_POS, EPWM_DISABLED);
    EPWMx->CCER = reg_value;

    return 0;
}

/*
功能描述：

名称：HW_EPWM_AllStop
功能：停止EPWM模块的所有通道
参数：
  - EPWMx: 指向EPWM模块的指针
返回值：操作成功返回0
输入：EPWM模块的指针
输出：所有PWM通道被停止
上层函数：在需要同时停止所有通道的PWM输出时调用
下层函数：无

附加说明：
- 该函数用于停止EPWM模块的所有PWM通道
- 适用于需要同步停止所有通道的情况
*/
int HW_EPWM_AllStop(EPWM_TypeDef *EPWMx)
{
    uint32_t reg_value;

    SET_BIT(EPWMx->CR1, EPWM_CR1_CEN_POS, EPWM_DISABLED);

    reg_value = EPWMx->CCER;
    SET_BIT(reg_value, EPWM_CCER_CC4NE_POS, EPWM_DISABLED);
    SET_BIT(reg_value, EPWM_CCER_CC4E_POS, EPWM_DISABLED);
    SET_BIT(reg_value, EPWM_CCER_CC3NE_POS, EPWM_DISABLED);
    SET_BIT(reg_value, EPWM_CCER_CC3E_POS, EPWM_DISABLED);
    SET_BIT(reg_value, EPWM_CCER_CC2NE_POS, EPWM_DISABLED);
    SET_BIT(reg_value, EPWM_CCER_CC2E_POS, EPWM_DISABLED);
    SET_BIT(reg_value, EPWM_CCER_CC1NE_POS, EPWM_DISABLED);
    SET_BIT(reg_value, EPWM_CCER_CC1E_POS, EPWM_DISABLED);
    EPWMx->CCER = reg_value;

    return 0;
}

/*
功能描述：

名称：HW_EPWM_CaptureStop
功能：停止指定EPWM通道的捕获功能
参数：
  - EPWMx: 指向EPWM模块的指针
  - chn: 要停止捕获的EPWM通道
返回值：成功返回0，失败返回-1
输入：EPWM模块的指针和通道号
输出：指定通道的捕获功能被停止
上层函数：在需要停止捕获外部事件时调用
下层函数：无

附加说明：
- 此函数用于停止指定通道的捕获功能，终止捕获外部事件如脉冲宽度等
- 通过修改相关寄存器来禁用捕获功能
*/
int HW_EPWM_CaptureStop(EPWM_TypeDef *EPWMx, EPwmChannel_e chn)
{
    uint32_t reg_value;
    uint32_t reg_offset;

    if (chn >= eEPwmChannel_Max) {
        return -1;
    }

    reg_value = EPWMx->CCER;
    reg_offset = (chn * 4);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1NE_POS, EPWM_DISABLED);
    SET_BIT(reg_value, reg_offset + EPWM_CCER_CC1E_POS, EPWM_DISABLED);
    EPWMx->CCER = reg_value;

    return 0;
}

// Note:
//  1. return value is the non-converted pulse width

/*
功能描述：

名称：HW_EPWM_GetCaptureValue
功能：获取指定EPWM通道最近一次捕获的脉冲宽度
参数：
  - EPWMx: 指向EPWM模块的指针
  - chn: 查询捕获值的EPWM通道
返回值：返回捕获到的脉冲宽度值
输入：EPWM模块的指针和通道号
输出：返回脉冲宽度数值
上层函数：在需要获取最近一次捕获的脉冲宽度时调用
下层函数：无

附加说明：
- 该函数返回指定通道捕获的脉冲宽度值，用于测量外部信号的时间特性
- 捕获值反映了最近一次捕获事件的具体数值
*/
uint32_t HW_EPWM_GetCaptureValue(EPWM_TypeDef *EPWMx, EPwmChannel_e chn)
{
    uint32_t cur_value;
    uint32_t reg_value;
    uint32_t index;

    if (chn >= eEPwmChannel_Max) {
        return 0;
    }

    cur_value = EPWMx->CCR[chn];
    index = GetIndex(EPWMx);

    if (cur_value <= s_last_capture_value[index][chn]) {
        reg_value = 0xffff + cur_value - s_last_capture_value[index][chn];
    } else {
        reg_value = cur_value - s_last_capture_value[index][chn];
    }

    s_last_capture_value[index][chn] = cur_value;

    return reg_value;
}

// Note:
//  1. this function only clear capture interrupt flag, not pwm timer interrupt

/*
功能描述：

名称：HW_EPWM_ClearInterrupt
功能：清除指定EPWM通道的中断标志
参数：
  - EPWMx: 指向EPWM模块的指针
  - chn: 清除中断标志的EPWM通道
返回值：操作成功返回0
输入：EPWM模块的指针和通道号
输出：指定通道的中断标志被清除
上层函数：在中断服务例程中，用于清除中断标志，以允许新的中断发生
下层函数：无

附加说明：
- 此函数用于清除EPWM通道上的中断标志，包括捕获和比较中断
- 清除中断标志是中断服务例程的常见任务，以确保中断控制器可以正常响应新的中断事件
*/
uint32_t HW_EPWM_ClearInterrupt(EPWM_TypeDef *EPWMx, EPwmChannel_e chn)
{
    uint32_t reg_value;

    reg_value = EPWMx->SR;

    switch (chn) {
    case eEPwmChannel_0:
        SET_BIT(reg_value, EPWM_SR_CC1OF_POS, 0);
        SET_BIT(reg_value, EPWM_SR_CC1IF_POS, 0);
        break;
    case eEPwmChannel_1:
        SET_BIT(reg_value, EPWM_SR_CC2OF_POS, 0);
        SET_BIT(reg_value, EPWM_SR_CC2IF_POS, 0);
        break;
    case eEPwmChannel_2:
        SET_BIT(reg_value, EPWM_SR_CC3OF_POS, 0);
        SET_BIT(reg_value, EPWM_SR_CC3IF_POS, 0);
        break;
    case eEPwmChannel_3:
        SET_BIT(reg_value, EPWM_SR_CC4OF_POS, 0);
        SET_BIT(reg_value, EPWM_SR_CC4IF_POS, 0);
        break;
    default:
        break;
    }

    SET_BIT(reg_value, EPWM_SR_UIF_POS, 0);
    EPWMx->SR = reg_value;

    reg_value = EPWMx->CCR[chn];
    UNUSED(reg_value);

    return 0;
}

#ifdef DEBUG_EPWM

#include "xprintf.h"

int selftest_epwm(void)
{
    EPWM_TypeDef *epwm = EPWM0;

    xprintf("EPWM VERSION = %08x\n", DEFAULT_VERSION_AND_ID);
    xprintf("EPWM ID = %08x\n", DEFAULT_VERSION_AND_ID);

    HW_EPWM_AllStop(epwm);

#if (DEBUG_EPWM_CASE == DEBUG_EPWM_CASE_COMPARE)
    HW_EPWM_InitConfig(epwm, &g_DeviceEPwmDefaultConfig);
    HW_EPWM_AllStart(epwm);
#endif // #if ( DEBUG_EPWM_CASE == DEBUG_EPWM_CASE_COMPARE )

#if (DEBUG_EPWM_CASE == DEBUG_EPWM_CASE_CAPTURE)
    DeviceEPwmConfig_t g_DeviceEPwmCapture1Config =
        {
            .clock_freq = 0, // NOTE: get by 'HW_EPWM_InitConfig'
            .mode = eEPwmMode_Capture,
            .in.polarity[eEPwmChannel_0] = eEPwmPolarity_Reserved,
            .in.polarity[eEPwmChannel_1] = eEPwmPolarity_Rising,
            .in.polarity[eEPwmChannel_2] = eEPwmPolarity_Reserved,
            .in.polarity[eEPwmChannel_3] = eEPwmPolarity_Reserved,
        };

    HW_EPWM_InitConfig(EPWM0, &g_DeviceEPwmCapture1Config); // NOTE: PWM[0] out to EPWM[1]
    xprintf("clock_freq = %d.\n", g_DeviceEPwmCapture1Config.clock_freq);

    HW_EPWM_CaptureStart(epwm, eEPwmChannel_1);
#endif // #if ( DEBUG_EPWM_CASE == DEBUG_EPWM_CASE_CAPTURE )

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_EPWM
