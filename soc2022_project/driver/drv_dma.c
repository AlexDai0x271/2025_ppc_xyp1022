/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_dma.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

/*
功能描述：

名称：g_DeviceDmaDefaultConfig
功能：定义DMA设备的默认配置，仅在调试模式下使用
参数：
  - mode: DMA模式，设置为内存到内存模式
  - channel: DMA通道，设置为通道0
  - transfer_size: 传输大小，设置为16（需要绑定到实际大小）
  - priority: DMA优先级，设置为31
  - src_addr: 源地址，初始设置为0（需要绑定到实际地址）
  - src_addr_mode: 源地址模式，设置为递增模式
  - src_data_width: 源数据宽度，设置为8位
  - src_data_depth: 源数据深度，设置为1
  - dst_addr: 目的地址，初始设置为0（需要绑定到实际地址）
  - dst_addr_mode: 目的地址模式，设置为递增模式
  - dst_data_width: 目的数据宽度，设置为8位
  - dst_data_depth: 目的数据深度，设置为1
  - user_data: 用户自定义数据，初始设置为0
返回值：无
输入：无
输出：无
上层函数：无
下层函数：可能被DMA初始化函数如HW_DMA_InitConfig使用来应用这些默认设置

附加说明：
- 这个配置结构体通常用于初始化DMA设备，配置参数适用于基本的DMA操作需求
- 在非调试模式下，应避免使用这些默认参数，以免影响生产环境下的设备行为
- 结构体中的参数可以根据实际硬件和项目需求进行调整
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceDmaConfig_t g_DeviceDmaDefaultConfig =
    {
        .mode = eDmaMode_Mem2Mem,
        .channel = eDmaChannel_0,

        .transfer_size = 16, // NOTE: need bind to actual size
        .priority = 31,

        .src_addr = 0x0, // NOTE: need bind to actual address
        .src_addr_mode = eDmaAddrMode_Inc,
        .src_data_width = eDmaDataWidth_8Bit,
        .src_data_depth = eDmaDataDepth_1,

        .dst_addr = 0x0, // NOTE: need bind to actual address
        .dst_addr_mode = eDmaAddrMode_Inc,
        .dst_data_width = eDmaDataWidth_8Bit,
        .dst_data_depth = eDmaDataDepth_1,

        .user_data = 0,
};
#endif // #ifdef DEBUG

// Note:
//  1. 'outstanding' is not support
//  2. 'linked list pointer' is not support
//  3. we don't config any 'AXI signal', so burst transfer is not support

/*
功能描述：

名称：HW_DMA_InitConfig
功能：初始化DMA控制器的配置
参数：
  - DMAx: 指向DMA模块的指针
  - config: 指向DMA配置结构的指针
返回值：成功返回0，失败返回-1
输入：DMA模块的指针和配置结构
输出：DMA通道被配置为指定的传输设置
上层函数：在系统启动或需要重新配置DMA通道时调用
下层函数：HW_DMA_Reset, HW_DMA_ChannelDisable等

附加说明：
- 此函数用于配置DMA通道，包括设置源地址、目标地址、传输大小和其他传输参数
- 配置前先禁用DMA通道以防止配置过程中发生数据传输
*/
int HW_DMA_InitConfig(DMA_TypeDef *DMAx, DeviceDmaConfig_t *config)
{
    uint64_t reg_value;

    if ((NULL == DMAx) || (NULL == config)) {
        return -1;
    }

    // disable dma first
    DMAx->CFG = 0;

    HW_DMA_Reset(DMAx);
    HW_DMA_ChannelDisable(DMAx, config->channel); // NOTE: enable in 'HW_DMA_ChannelEnable'

    reg_value = 0;                                     // .CH_CFG2 inited to zero first
    reg_value |= (0ULL << DMA_CH_CFG_DST_OSR_LMT_POS); // NOTE: not support outstanding yet
    reg_value |= (0ULL << DMA_CH_CFG_SRC_OSR_LMT_POS);
    if (eDmaMode_Mem2Mem == config->mode) {
        SET_BIT(reg_value, DMA_CH_CFG_LOCK_CH_POS, DMA_ENABLED);
        reg_value |= (LOCK_CH_L_BLOCK << DMA_CH_CFG_LOCK_CH_L_POS);
    } else {
        SET_BIT(reg_value, DMA_CH_CFG_LOCK_CH_POS, DMA_DISABLED);
    }
    switch (config->mode) {
    case eDmaMode_Mem2Mem:
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_DST_POS, HS_SEL_SOFTWARE);
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_SRC_POS, HS_SEL_SOFTWARE);
        break;
    case eDmaMode_Mem2Dev:
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_DST_POS, HS_SEL_HARDWARE);
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_SRC_POS, HS_SEL_HARDWARE);
        break;
    case eDmaMode_Dev2Dev:
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_DST_POS, HS_SEL_HARDWARE);
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_SRC_POS, HS_SEL_HARDWARE);
        break;
    case eDmaMode_Dev2Mem:
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_DST_POS, HS_SEL_HARDWARE);
        SET_BIT(reg_value, DMA_CH_CFG_HS_SEL_SRC_POS, HS_SEL_HARDWARE);
        break;
    default:
        break;
    }
    reg_value |= ((uint64_t)config->mode << DMA_CH_CFG_TT_FC_POS);
    reg_value |= (MULTBLK_TYPE_CONTIGUOUS << DMA_CH_CFG_DST_MULTBLK_TYPE_POS);
    reg_value |= (MULTBLK_TYPE_CONTIGUOUS << DMA_CH_CFG_SRC_MULTBLK_TYPE_POS);
    DMAx->chn[config->channel].CH_CFG = reg_value;

    DMAx->chn[config->channel].CH_SAR = config->src_addr;
    DMAx->chn[config->channel].CH_DAR = config->dst_addr;

    reg_value = (config->transfer_size > 0) ? (config->transfer_size - 1) : 0;
    DMAx->chn[config->channel].CH_BLOCK_TS = reg_value;

    DMAx->chn[config->channel].CH_LLP = 0; // NOTE: not support linked list pointer yet

    reg_value = 0; // .CH_CTL inited to zero first
    SET_BIT(reg_value, DMA_CH_CTL_SHADOWREG_OR_LLI_VALID_POS, DMA_DISABLED);
    reg_value |= ((config->dst_data_depth & 0xf) << DMA_CH_CTL_DST_MSIZE_POS);
    reg_value |= ((config->src_data_depth & 0xf) << DMA_CH_CTL_SRC_MSIZE_POS);
    reg_value |= ((config->dst_data_width & 0x7) << DMA_CH_CTL_DST_TR_WIDTH_POS);
    reg_value |= ((config->src_data_width & 0x7) << DMA_CH_CTL_SRC_TR_WIDTH_POS);
    SET_BIT(reg_value, DMA_CH_CTL_DINC_POS, config->dst_addr_mode);
    SET_BIT(reg_value, DMA_CH_CTL_SINC_POS, config->src_addr_mode);
    SET_BIT(reg_value, DMA_CH_CTL_DMS_POS, MC_AXI_MASTER_1);
    SET_BIT(reg_value, DMA_CH_CTL_SMS_POS, MC_AXI_MASTER_1);
    DMAx->chn[config->channel].CH_CTL = reg_value; // NOTE: not support AXI signal yet

    reg_value = HW_DMA_GetStatus(DMAx, config->channel);
    UNUSED(reg_value);

    SET_BIT(DMAx->CFG, DMA_CFG_DMAC_EN_POS, DMA_ENABLED);

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：HW_DMA_Reset
功能：重置DMA控制器
参数：
  - DMAx: 指向DMA模块的指针
返回值：操作成功返回0
输入：DMA模块的指针
输出：DMA控制器被重置
上层函数：在初始化DMA设置或出现错误需要重置控制器时调用
下层函数：无

附加说明：
- 该函数通过设置相应的控制寄存器位来重置整个DMA控制器
- 重置操作将清除所有DMA通道的配置和状态信息，使控制器回到初始状态
*/
int HW_DMA_Reset(DMA_TypeDef *DMAx)
{
    SET_BIT(DMAx->RESET, DMA_RESET_DMAC_RST_POS, DMA_ENABLED);

    while (GET_BIT(DMAx->RESET, DMA_RESET_DMAC_RST_POS))
        ;
    return 0;
}

/*
功能描述：

名称：HW_DMA_ChannelEnable
功能：启用指定的DMA通道
参数：
  - DMAx: 指向DMA模块的指针
  - chn: 要启用的DMA通道
返回值：操作成功返回0
输入：DMA模块的指针和通道编号
输出：指定的DMA通道被启用
上层函数：在需要开始DMA传输时调用
下层函数：ChannelControl

附加说明：
- 此函数用于启动一个配置好的DMA通道，使其准备接收和处理传输请求
- 通过修改DMA通道使能寄存器实现启用操作
*/
static int ChannelControl(DMA_TypeDef *DMAx, DmaChannel_e chn, uint32_t reg_offset, uint32_t enable)
{
    uint64_t reg_value;

    if (chn >= eDmaChannel_Max) {
        return -1;
    }

    reg_value = DMAx->CHEN;
    SET_BIT(reg_value, (reg_offset + 8 + chn), DMA_ENABLED);
    SET_BIT(reg_value, (reg_offset + chn), enable);
    DMAx->CHEN = reg_value;

    return 0;
}

/*
功能描述：

名称：HW_DMA_ChannelEnable
功能：启用指定的DMA通道
参数：
  - DMAx: 指向DMA模块的指针
  - chn: 要启用的DMA通道
返回值：操作成功返回0
输入：DMA模块的指针和通道编号
输出：指定的DMA通道被启用
上层函数：在需要开始DMA传输时调用
下层函数：ChannelControl

附加说明：
- 此函数用于启动一个配置好的DMA通道，使其准备接收和处理传输请求
- 通过修改DMA通道使能寄存器实现启用操作
*/
int HW_DMA_ChannelEnable(DMA_TypeDef *DMAx, DmaChannel_e chn)
{
    return ChannelControl(DMAx, chn, 0, DMA_ENABLED);
}

/*
功能描述：

名称：HW_DMA_ChannelDisable
功能：禁用指定的DMA通道
参数：
  - DMAx: 指向DMA模块的指针
  - chn: 要禁用的DMA通道
返回值：操作成功返回0
输入：DMA模块的指针和通道编号
输出：指定的DMA通道被禁用
上层函数：在DMA传输完成或不再需要传输时调用
下层函数：ChannelControl

附加说明：
- 通过直接修改控制寄存器，此函数停止指定的DMA通道上的活动，防止进一步的数据传输
- 禁用通道是停止DMA操作的一种方法，特别是在传输错误或重新配置通道前需要确保通道不活动
*/
int HW_DMA_ChannelDisable(DMA_TypeDef *DMAx, DmaChannel_e chn)
{
    return ChannelControl(DMAx, chn, 0, DMA_DISABLED);
}

/*
名称：HW_DMA_ChannelSuspend
功能：暂停指定的DMA通道
参数：
  - DMA_TypeDef *DMAx: 指向DMA控制器的指针
  - DmaChannel_e chn: 指定要操作的DMA通道
返回值：返回操作成功或失败的状态码
输入：DMA控制器的指针和通道编号
输出：无直接输出，但会修改DMA控制器的内部状态，使指定通道进入暂停状态
上层函数：可能由中断管理或任务调度函数调用，以管理DMA通道的状态
下层函数：ChannelControl，用于设置通道控制寄存器

附加说明：
- 此函数通过调用ChannelControl函数来实现暂停功能，具体通过设置通道的控制位实现
- 16和DMA_ENABLED参数传递给ChannelControl可能表示启用通道的暂停功能的具体位字段和值
*/
int HW_DMA_ChannelSuspend(DMA_TypeDef *DMAx, DmaChannel_e chn)
{
    return ChannelControl(DMAx, chn, 16, DMA_ENABLED);
}

/*
名称：HW_DMA_ChannelAbort
功能：终止指定的DMA通道
参数：
  - DMA_TypeDef *DMAx: 指向DMA控制器的指针
  - DmaChannel_e chn: 指定要操作的DMA通道
返回值：返回操作成功或失败的状态码
输入：DMA控制器的指针和通道编号
输出：无直接输出，但会修改DMA控制器的内部状态，使指定通道停止当前操作
上层函数：可能由中断管理或错误处理函数调用，以快速停止DMA传输
下层函数：ChannelControl，用于设置通道控制寄存器

附加说明：
- 使用ChannelControl函数，传递32和DMA_ENABLED参数，这可能表示启用通道的终止功能的具体位字段和值
*/
int HW_DMA_ChannelAbort(DMA_TypeDef *DMAx, DmaChannel_e chn)
{
    return ChannelControl(DMAx, chn, 32, DMA_ENABLED);
}

/*
名称：HW_DMA_WaitComplete
功能：等待指定DMA通道的传输完成
参数：
  - DMA_TypeDef *DMAx: 指向DMA控制器的指针
  - DmaChannel_e chn: 指定要操作的DMA通道
返回值：如果通道完成传输返回0，否则返回1（用于外部循环检测）
输入：DMA控制器的指针和通道编号
输出：无直接输出，但根据传输状态返回相应的完成标志
上层函数：可能由应用程序或中断服务例程调用，以确认数据传输是否完成
下层函数：无

附加说明：
- 通过检查DMA控制器的INTSTATUS寄存器来确定是否完成传输
- 如果通道编号超出有效范围，则默认返回1，假装传输已完成，这可能是为了在错误配置时避免无限循环
*/
int HW_DMA_WaitComplete(DMA_TypeDef *DMAx, DmaChannel_e chn)
{
    if (chn >= eDmaChannel_Max) {
        return 1; // NOTE: return '1' to try to deceive external loop
    }

    uint64_t reg_value = DMAx->INTSTATUS;
    UNUSED(reg_value);

    return GET_BIT(DMAx->INTSTATUS, chn);
}

/*
名称：HW_DMA_GetStatus
功能：获取指定DMA通道的当前状态
参数：
  - DMA_TypeDef *DMAx: 指向DMA控制器的指针
  - DmaChannel_e chn: 指定要查询的DMA通道
返回值：返回DMA通道的状态寄存器值
输入：DMA控制器的指针和通道编号
输出：返回通道的状态，包括是否处于活跃状态、错误状态等
上层函数：可能由诊断或监控工具调用，以获取通道的运行状态
下层函数：无

附加说明：
- 返回的状态信息可以用于调试或在复杂应用中管理DMA资源
*/
uint64_t HW_DMA_GetStatus(DMA_TypeDef *DMAx, DmaChannel_e chn)
{
    if (chn >= eDmaChannel_Max) {
        return 0;
    }

    return DMAx->chn[chn].CH_STATUS;
}

#ifdef DEBUG_DMA

#include "xprintf.h"

static uint8_t tx_buffer[DMA_FIFO_MAX];
static uint8_t rx_buffer[DMA_FIFO_MAX];
static uint32_t length = sizeof(tx_buffer);

int selftest_dma(void)
{
    DMA_TypeDef *dma = DMA0;

    xprintf("DMA VERSION = %08x\n", DEFAULT_VERSION_AND_ID);
    xprintf("DMA ID = %08x\n", DEFAULT_VERSION_AND_ID);

    g_DeviceDmaDefaultConfig.src_addr = (uint32_t)&tx_buffer;
    g_DeviceDmaDefaultConfig.dst_addr = (uint32_t)&rx_buffer;
    g_DeviceDmaDefaultConfig.transfer_size = length;
    HW_DMA_InitConfig(dma, &g_DeviceDmaDefaultConfig);

    for (uint32_t i = 0; i < sizeof(tx_buffer); i++) {
        tx_buffer[i] = i % 256;
        rx_buffer[i] = 0xff;
    }

    xprintf("send %d.\n", length);
    HW_DMA_ChannelEnable(dma, g_DeviceDmaDefaultConfig.channel);

    while (!HW_DMA_WaitComplete(dma, g_DeviceDmaDefaultConfig.channel))
        ;

    xprintf("recv %d.\n", length);
    for (uint32_t i = 0; i < length; i++) {
        xprintf("%02x ", rx_buffer[i]);
    }
    xprintf("\n");

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_DMA
