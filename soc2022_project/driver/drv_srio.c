/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_srio.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

#ifdef DEBUG // @@
// Note:
//  This is only a group of parameters for one RapidIO
//  You must mount them to the 'DeviceSrioConfig_t' before initialize
//  And, the local buffer must aligned on 1024 Byte

static uint8_t _s_local_txbuffer[16 * 1024] __attribute__((aligned(1024)));
static uint8_t _s_local_rxbuffer[16 * 1024] __attribute__((aligned(1024)));

void gmac_compile_keep(void)
{
    UNUSED(_s_local_txbuffer);
    UNUSED(_s_local_rxbuffer);
}
#endif // #ifdef DEBUG

/*
功能描述：

名称：g_DeviceSrioDefaultConfig
功能：定义默认的SRIO设备配置，仅在调试模式下使用
参数：
  - type: SRIO类型，设置为非阻塞读写
  - device_id: 设备ID，设置为0x5a5a
  - cfg_base_addr: SRIO配置基地址
  - phy_base_addr: SRIO物理层基地址
  - map_base_addr: SRIO映射基地址
  - mem_base_addr: 内存基地址
  - mem_size: 内存大小，设置为128MB
  - user_data: 用户自定义数据，初始设置为0
返回值：无
输入：无
输出：无
上层函数：无
下层函数：可能被SRIO初始化函数如 HW_SRIO_InitConfig 使用来应用这些默认设置

附加说明：
- 这个配置结构体通常用于初始化SRIO，配置参数适用于基本的SRIO操作和数据传输需求
- 在非调试模式下，应避免使用这些默认参数，以免影响生产环境下的设备行为
- 结构体中的参数可以根据实际硬件和项目需求进行调整
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceSrioConfig_t g_DeviceSrioDefaultConfig =
    {
        .type = eSrioType_NREAD_NWRITE,

        .device_id = 0x5a5a,

        .cfg_base_addr = SRIO0_CFG_BASE,
        .phy_base_addr = SRIO0_SERDES_BASE,
        .map_base_addr = SRIO0_BASE,

        .mem_base_addr = SRAM_BASE,
        .mem_size = (128 * 1024 * 1024),

        .user_data = 0,
};
#endif // #ifdef DEBUG

static void _PageConfig(SRIO_TypeDef *SRIOx, uint32_t offset)
{
    SRIOx->PAGE0.Global.APBCS = (SRIOx->PAGE0.Global.APBCS & PAGE_SELECT_MASK) | ((offset / RAB_PAGE_SIZE) << PAGE_SELECT_SHIFT);
}

static void _BuildEntry(SRIO_TypeDef *SRIOx, SrioMode_e mode, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t index)
{
    DEBUG_ASSERT(!(len & 0x3));

    len >>= 2; // NOTE: aligned to 4-bytes

    SRIOx->PAGE1.DMA.DMAIADSR = (mode << 0) | (engine << 1) | ((index & 0xf) << 4); // NOTE: bit[0], =0,RDMA
                                                                                    //               =1,WDMA
                                                                                    //       bit[3:1], Engine
                                                                                    //       bit[7:4], Descriptor
    SRIOx->PAGE1.DMA.DMAIADCR = BIT(0) | BIT(1) | (len << 5);
    SRIOx->PAGE1.DMA.DMAIADSAR = (src_addr >> 2); // NOTE: aligned to 4-bytes
    SRIOx->PAGE1.DMA.DMAIADDAR = (dst_addr >> 2); // NOTE: aligned to 4-bytes
    SRIOx->PAGE1.DMA.DMAIADNPAR = GET_BIT(index, 7);
}

// Note:
//  1. axi address width is 32-bits
//  2. only support DMA mode
//  3. only support NREAD/NWRITE/NWRITE_R/SWRITE

/*
功能描述：

名称：HW_SRIO_InitConfig
功能：初始化指定的SRIO配置
参数：
  - SRIOx: 指向特定SRIO端口的指针
  - config: 指向SRIO配置结构体的指针
返回值：成功返回0，失败返回-1
输入：SRIO端口和配置信息
输出：根据输入的配置初始化SRIO
上层函数：可能由设备初始化函数或系统启动配置调用
下层函数：调用了_reg_addr和_reg_value来设置寄存器值

附加说明：
- 这个函数首先禁用SRIO，然后根据配置参数设置SRIO寄存器，最后使能SRIO
- 使用_pageconfig来设置页配置，_BuildEntry函数来构建传输描述符
*/
int HW_SRIO_InitConfig(SRIO_TypeDef *SRIOx, DeviceSrioConfig_t *config)
{
    uint32_t reg_addr;
    uint32_t reg_value;

    if ((NULL == SRIOx) || (NULL == config)) {
        return -1;
    }

    // stage-1, reset phy

    reg_addr = config->phy_base_addr;

    // Note:
    //  reg_addr[0x18] = 400 4x
    //  reg_addr[0x18] = 401 1x lane 0
    //  reg_addr[0x18] = 402 2x
    //  reg_addr[0x18] = 403 1x lane 0

    Reg_Write_32(reg_addr + 0x18, 0x403);

    Reg_Write_32(reg_addr + 0x84, 0x1);
    Reg_Write_32(reg_addr + 0x80, 0x1);

    // stage-2, init

    _PageConfig(SRIOx, 0x0);
    reg_addr = config->cfg_base_addr + 0x800; // NOTE: paged windows is mapped in high 2K space

    while (!(Reg_Read_32(reg_addr + 0x158) & BIT(25)))
        ; // NOTE: need bit swap in every byte, but not operate here

    reg_value = (config->device_id << 16);
    Reg_Write_32(reg_addr + 0x60, reg_value);

    // stage-3, create memory mapping

    SRIOx->PAGE0.Global.BCTLR = 0xf;

    SRIOx->PAGE0.RIO.RIO_Engine[eSrioEngine_0].PIOCTLR = (0x1 << 0);              // NOTE: bit[0], PIO Enable
    SRIOx->PAGE0.AXI.AXI_Engine[eSrioEngine_0].PIOCTLR = (0x1 << 0) | (0x1 << 1); // NOTE: bit[0], PIO Enable
                                                                                  //       bit[1], Memory Mapping Enable

    for (uint32_t lut = 0; lut < 16; lut++) {
        RIO_PIO_MAPPING_setting(SRIOx, lut, eSrioWindowSize_128M, eSrioBitSelect_SID_3_0, config->mem_base_addr);
    }

    AXI_PIO_MAPPING_setting(SRIOx, eSrioEngine_0, config->type, config->map_base_addr, config->mem_base_addr, config->mem_size, config->device_id);

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：RIO_PIO_MAPPING_setting
功能：配置指定SRIO端口的PIO映射
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - lut: 查找表（LUT）索引
  - window_size: 窗口大小
  - bit_select: 位选择标识
  - rio_addr: RIO地址
返回值：无
输入：SRIO设备端口、LUT索引、窗口大小、位选择和RIO地址
输出：配置SRIO设备的PIO映射
上层函数：可能由需要设置RIO映射的初始化或配置函数调用
下层函数：无

附加说明：
- 这个函数通过设置指定的查找表索引和相关配置来定义SRIO端口的地址映射
- 使用这个函数可以精确控制数据在RIO和本地系统之间的传输方式和路径
*/
void RIO_PIO_MAPPING_setting(SRIO_TypeDef *SRIOx, uint32_t lut, SrioWindowSize_e window_size, SrioBitSelect_e bit_select, uint32_t rio_addr)
{
    DEBUG_ASSERT(!(rio_addr & 0xfffff));

    rio_addr >>= 20; // NOTE: aligned to 1M-bytes

    SRIOx->PAGE0.RIO.RIO_LUT[lut] = BIT(0) | (window_size << 1) | (rio_addr << 14);
    SRIOx->PAGE0.RIO.RIO_AMISR = BIT(bit_select);
    SRIOx->PAGE0.RIO.RIO_AMBR = 0;
}

/*
功能描述：

名称：AXI_PIO_MAPPING_setting
功能：设置AXI到PIO的映射
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - engine: 使用的引擎编号
  - type: 传输类型
  - axi_addr: AXI地址
  - rio_addr: RIO地址
  - window_size: 窗口大小
  - dest_id: 目标设备ID
返回值：无
输入：SRIO设备端口、引擎编号、传输类型、AXI地址、RIO地址、窗口大小和目标设备ID
输出：在SRIO设备上配置AXI到PIO的映射
上层函数：可能由需要设置AXI映射的初始化或配置函数调用
下层函数：无

附加说明：
- 此函数负责在SRIO设备上设置AXI总线地址到RIO地址的映射，用于数据传输
- 映射配置包括选择窗口大小和目标设备ID，确保数据能正确路由到目标设备
*/
void AXI_PIO_MAPPING_setting(SRIO_TypeDef *SRIOx, SrioEngine_e engine, SrioType_e type, uint32_t axi_addr, uint32_t rio_addr, uint32_t window_size, uint32_t dest_id)
{
    DEBUG_ASSERT(type != eSrioType_Maintenance);

    SRIOx->PAGE0.AXI.AXI_Address_Map[engine].AXIAMCR = BIT(0) | (type << 1) | (dest_id << 16);
    SRIOx->PAGE0.AXI.AXI_Address_Map[engine].AXIAMSR = window_size;         // NOTE: 'window_size' is 1K-bytes-based
    SRIOx->PAGE0.AXI.AXI_Address_Map[engine].AXIAMAXIBR = (axi_addr >> 10); // NOTE: aligned to 1K-bytes
    SRIOx->PAGE0.AXI.AXI_Address_Map[engine].AXIAMRIOBR = (rio_addr >> 10);
}

/*
功能描述：

名称：_set_DMA_desc_node
功能：配置DMA描述符节点
参数：
  - desc: 指向DMA描述符节点的指针
  - desc_setting: 指向DMA描述符设置的指针
返回值：无
输入：DMA描述符节点和设置
输出：在指定的DMA描述符节点上应用配置
上层函数：可能由需要配置DMA操作的函数调用
下层函数：无

附加说明：
- 此函数为低级函数，直接操作硬件DMA描述符，设置如传输长度、源地址和目标地址等
- 使用此函数可以精确控制DMA引擎的行为，如启动和停止传输
*/
static void _set_DMA_desc_node(SRIO_DMA_desc_node *desc, SRIO_DMA_desc_setting *desc_setting)
{
    desc->desc_ctrl_w0 = (desc_setting->valid << 0) | (desc_setting->next_desc_pointer_valid << 1) | (((desc_setting->transmit_length >> 2) & 0x3ffff) << 5);

    desc->source_addr_w1 = (desc_setting->source_addr >> 2);

    desc->dest_addr_w2 = (desc_setting->dest_addr >> 2);

    desc->next_desc_addr_w3 = (desc_setting->next_desc_addr >> 3) & 0xffffffff;
}

/*
功能描述：

名称：set_WDMA_desc_node
功能：设置写DMA描述符节点
参数：
  - desc: 指向DMA描述符节点的指针
  - desc_setting: 指向DMA描述符设置的指针
返回值：无
输入：DMA描述符节点和设置
输出：配置写DMA操作的描述符节点
上层函数：可能由执行写DMA操作的函数调用
下层函数：调用_set_DMA_desc_node来实现具体的设置

附加说明：
- 这个函数专门用于配置写DMA操作，通过调用更通用的_set_DMA_desc_node函数来设置具体参数
*/
static void set_WDMA_desc_node(SRIO_DMA_desc_node *desc, SRIO_DMA_desc_setting *desc_setting)
{
    _set_DMA_desc_node(desc, desc_setting);
}

/*
功能描述：

名称：set_RDMA_desc_node
功能：设置读DMA描述符节点
参数：
  - desc: 指向DMA描述符节点的指针
  - desc_setting: 指向DMA描述符设置的指针
返回值：无
输入：DMA描述符节点和设置
输出：配置读DMA操作的描述符节点
上层函数：可能由执行读DMA操作的函数调用
下层函数：调用_set_DMA_desc_node来实现具体的设置

附加说明：
- 这个函数专门用于配置读DMA操作，通过调用更通用的_set_DMA_desc_node函数来设置具体参数
*/
static void set_RDMA_desc_node(SRIO_DMA_desc_node *desc, SRIO_DMA_desc_setting *desc_setting)
{
    _set_DMA_desc_node(desc, desc_setting);
}

/*
功能描述：

名称：SRIO_WDMA_desc_transmit
功能：启动SRIO设备的写DMA传输
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - engine: 使用的DMA引擎编号
  - desc: 指向配置好的DMA描述符节点的指针
  - dest_id: 目标设备ID
返回值：无
输入：SRIO设备、DMA引擎、DMA描述符和目标设备ID
输出：在SRIO设备上启动写DMA传输
上层函数：可能由需要进行高速数据传输的应用调用
下层函数：调用_PageConfig来配置页面，以及直接操作硬件寄存器来启动DMA

附加说明：
- 此函数配置SRIO设备的DMA引擎，使用提供的描述符开始数据的写操作
- 特别关注DMA描述符的配置，确保数据按预期发送到正确的目标地址
- 该函数监控DMA传输的完成状态，确保数据传输的完整性和正确性
*/
static void SRIO_WDMA_desc_transmit(SRIO_TypeDef *SRIOx, SrioEngine_e engine, SRIO_DMA_desc_node *desc, uint32_t dest_id)
{
    SRIOx->PAGE0.Global.BCTLR |= BCTLR_WDMA_EN;

    _PageConfig(SRIOx, 0x20800);

    SRIOx->PAGE1.DMA.Write_DMA[engine].WDMACR = (dest_id << 16);
    SRIOx->PAGE1.DMA.Write_DMA[engine].WDMADAR = ((((uint32_t)desc) >> 2) & 0xffffffff); // NOTE: aligned to 4-bytes, but alloc need 8-bytes
    SRIOx->PAGE1.DMA.Write_DMA[engine].WDMADAER = 0;                                     // NOTE: only support 32-bits axi address mode yet
    SRIOx->PAGE1.DMA.Write_DMA[engine].WDMACR |= DMACR_START;

    while (!(SRIOx->PAGE0.Interrupt.WDMAISR & BIT(engine)))
        ;
    SRIOx->PAGE0.Interrupt.WDMAISR = 0xffffffff;
}

/*
功能描述：

名称：SRIO_RDMA_desc_transmit
功能：启动SRIO设备的读DMA传输
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - engine: 使用的DMA引擎编号
  - desc: 指向配置好的DMA描述符节点的指针
  - dest_id: 目标设备ID
返回值：无
输入：SRIO设备、DMA引擎、DMA描述符和目标设备ID
输出：在SRIO设备上启动读DMA传输
上层函数：可能由需要进行高速数据接收的应用调用
下层函数：调用_PageConfig来配置页面，以及直接操作硬件寄存器来启动DMA

附加说明：
- 此函数配置SRIO设备的DMA引擎，使用提供的描述符开始数据的读操作
- 通过正确配置DMA描述符，确保数据能从正确的源地址按预定路线传输
- 监控DMA传输的完成状态，确保数据的完整接收和数据完整性
*/
static void SRIO_RDMA_desc_transmit(SRIO_TypeDef *SRIOx, SrioEngine_e engine, SRIO_DMA_desc_node *desc, uint32_t dest_id)
{
    SRIOx->PAGE0.Global.BCTLR |= BCTLR_RDMA_EN;

    _PageConfig(SRIOx, 0x20800);

    SRIOx->PAGE1.DMA.Read_DMA[engine].RDMACR = (dest_id << 16);
    SRIOx->PAGE1.DMA.Read_DMA[engine].RDMADAR = ((((uint32_t)desc) >> 2) & 0xffffffff); // NOTE: aligned to 4-bytes, but alloc need 8-bytes
    SRIOx->PAGE1.DMA.Read_DMA[engine].RDMADAER = 0;                                     // NOTE: only support 32-bits axi address mode yet
    SRIOx->PAGE1.DMA.Read_DMA[engine].RDMACR |= DMACR_START;

    while (!(SRIOx->PAGE0.Interrupt.RDMAISR & BIT(engine)))
        ;
    SRIOx->PAGE0.Interrupt.RDMAISR = 0xffffffff;
}

/*
功能描述：

名称：SRIO_WDMA_Transmit
功能：执行SRIO设备的写DMA传输操作
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - engine: 使用的DMA引擎编号
  - src_addr: 源地址
  - dst_addr: 目标地址
  - len: 传输长度
  - dest_id: 目标设备ID
返回值：无
输入：SRIO设备、DMA引擎、源地址、目标地址、传输长度和目标设备ID
输出：在SRIO设备上执行写DMA传输
上层函数：可能由需要快速发送大量数据的功能调用
下层函数：调用_BuildEntry来构建DMA传输条目

附加说明：
- 此函数为写操作提供了一个快速通道，通过DMA引擎直接从源地址向目标地址传输数据
- 确保DMA描述符正确配置，以便数据按预定路径高效传输
- 功能包括设置DMA描述符，启动DMA传输，监控状态直到传输完成
*/
int SRIO_WDMA_Transmit(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id)
{
    SRIOx->PAGE0.Global.BCTLR |= BCTLR_WDMA_EN;

    _PageConfig(SRIOx, 0x20800);

    SRIOx->PAGE1.DMA.Write_DMA[engine].WDMACR = BIT(2) | (dest_id << 16);

    // Note:
    //  only support one entry in a dma list
    //  if need transmit multi-packet, build dma list with multi entry

#if 1 // @@
    _BuildEntry(SRIOx, eSrioMode_WDMA, engine, src_addr, dst_addr, len, 0x00);
#else
    _BuildEntry(SRIOx, eSrioMode_WDMA, engine, src_addr + len * 0, dst_addr + len * 0, len, 0x80);
    _BuildEntry(SRIOx, eSrioMode_WDMA, engine, src_addr + len * 1, dst_addr + len * 1, len, 0x81);
    _BuildEntry(SRIOx, eSrioMode_WDMA, engine, src_addr + len * 2, dst_addr + len * 2, len, 0x02);
#endif

    SRIOx->PAGE1.DMA.Write_DMA[engine].WDMACR |= DMACR_START;

    while (!(SRIOx->PAGE0.Interrupt.WDMAISR & BIT(engine)))
        ;

    return 0;
}

/*
功能描述：

名称：SRIO_RDMA_Transmit
功能：执行SRIO设备的读DMA传输操作
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - engine: 使用的DMA引擎编号
  - src_addr: 源地址
  - dst_addr: 目标地址
  - len: 传输长度
  - dest_id: 目标设备ID
返回值：无
输入：SRIO设备、DMA引擎、源地址、目标地址、传输长度和目标设备ID
输出：在SRIO设备上执行读DMA传输
上层函数：可能由需要快速接收大量数据的功能调用
下层函数：调用_BuildEntry来构建DMA传输条目

附加说明：
- 此函数为读操作提供了一个快速通道，通过DMA引擎直接从源地址向目标地址传输数据
- 通过正确配置DMA描述符，确保数据能从正确的源地址按预定路线高效传输
- 功能包括设置DMA描述符，启动DMA传输，监控状态直到传输完成
*/
int SRIO_RDMA_Transmit(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id)
{
    SRIOx->PAGE0.Global.BCTLR |= BCTLR_RDMA_EN;

    _PageConfig(SRIOx, 0x20800);

    SRIOx->PAGE1.DMA.Read_DMA[engine].RDMACR = BIT(2) | (dest_id << 16);

    // Note:
    //  only support one entry in a dma list
    //  if need transmit multi-packet, build dma list with multi entry

#if 1 // @@
    _BuildEntry(SRIOx, eSrioMode_RDMA, engine, src_addr, dst_addr, len, 0x00);
#else
    _BuildEntry(SRIOx, eSrioMode_RDMA, engine, src_addr + len * 0, dst_addr + len * 0, len, 0x80);
    _BuildEntry(SRIOx, eSrioMode_RDMA, engine, src_addr + len * 1, dst_addr + len * 1, len, 0x81);
    _BuildEntry(SRIOx, eSrioMode_RDMA, engine, src_addr + len * 2, dst_addr + len * 2, len, 0x02);
#endif

    SRIOx->PAGE1.DMA.Read_DMA[engine].RDMACR |= DMACR_START;

    while (!(SRIOx->PAGE0.Interrupt.RDMAISR & BIT(engine)))
        ;

    return 0;
}

/*
功能描述：

名称：SRIO_WDMA_Transmit2
功能：使用DMA描述符执行SRIO设备的写DMA传输操作
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - engine: 使用的DMA引擎编号
  - src_addr: 源地址
  - dst_addr: 目标地址
  - len: 传输长度
  - dest_id: 目标设备ID
返回值：无
输入：SRIO设备、DMA引擎、源地址、目标地址、传输长度和目标设备ID
输出：在SRIO设备上执行写DMA传输
上层函数：可能由需要配置和启动写DMA传输的应用调用
下层函数：调用set_WDMA_desc_node和SRIO_WDMA_desc_transmit来配置和启动传输

附加说明：
- 此函数通过配置和使用DMA描述符来进行写DMA传输操作
- 通过set_WDMA_desc_node函数设置DMA描述符的参数，并调用SRIO_WDMA_desc_transmit函数启动传输
- 这种方式适用于复杂的DMA传输场景，允许对传输过程进行更精细的控制
*/
int SRIO_WDMA_Transmit2(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id)
{
    // Note:
    //  only support one entry in a dma list
    //  if need transmit multi-packet, build dma list with multi entry

    SRIO_DMA_desc_setting desc_setting;
    SRIO_DMA_desc_node desc;

    desc_setting.source_addr = src_addr;
    desc_setting.dest_addr = dst_addr;
    desc_setting.transmit_length = len;
    desc_setting.next_desc_pointer_valid = 1;
    desc_setting.next_desc_addr = 0;
    desc_setting.valid = 1;

    set_WDMA_desc_node(&desc, &desc_setting);

    SRIO_WDMA_desc_transmit(SRIOx, engine, &desc, dest_id);

    return 0;
}

/*
功能描述：

名称：SRIO_RDMA_Transmit2
功能：使用DMA描述符执行SRIO设备的读DMA传输操作
参数：
  - SRIOx: 指向特定SRIO设备的指针
  - engine: 使用的DMA引擎编号
  - src_addr: 源地址
  - dst_addr: 目标地址
  - len: 传输长度
  - dest_id: 目标设备ID
返回值：无
输入：SRIO设备、DMA引擎、源地址、目标地址、传输长度和目标设备ID
输出：在SRIO设备上执行读DMA传输
上层函数：可能由需要配置和启动读DMA传输的应用调用
下层函数：调用set_RDMA_desc_node和SRIO_RDMA_desc_transmit来配置和启动传输

附加说明：
- 此函数通过配置和使用DMA描述符来进行读DMA传输操作
- 通过set_RDMA_desc_node函数设置DMA描述符的参数，并调用SRIO_RDMA_desc_transmit函数启动传输
- 这种方式适用于复杂的DMA传输场景，允许对传输过程进行更精细的控制
*/
int SRIO_RDMA_Transmit2(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id)
{
    // Note:
    //  only support one entry in a dma list
    //  if need transmit multi-packet, build dma list with multi entry

    SRIO_DMA_desc_setting desc_setting;
    SRIO_DMA_desc_node desc;

    desc_setting.source_addr = src_addr;
    desc_setting.dest_addr = dst_addr;
    desc_setting.transmit_length = len;
    desc_setting.next_desc_pointer_valid = 1;
    desc_setting.next_desc_addr = 0;
    desc_setting.valid = 1;

    set_RDMA_desc_node(&desc, &desc_setting);

    SRIO_RDMA_desc_transmit(SRIOx, engine, &desc, dest_id);

    return 0;
}

#ifdef DEBUG_SRIO

#include "xprintf.h"

int selftest_srio(void)
{
    SRIO_TypeDef *srio = SRIO0;

    HW_SRIO_InitConfig(srio, &g_DeviceSrioDefaultConfig);

    xprintf("SRIO VERSION = %08x\n", srio->PAGE0.Global.BVERR);

    xprintf("\n");
    xprintf("Bridge Capability:\n");
    xprintf("    Number of AMBA PIO Supported : %d\n", ((srio->PAGE0.Global.BCAPR >> 0) & 0xf));
    xprintf("    Number of RIO PIO Supported  : %d\n", ((srio->PAGE0.Global.BCAPR >> 4) & 0xf));
    xprintf("    Number of Write DMA Supported: %d\n", ((srio->PAGE0.Global.BCAPR >> 8) & 0xf));
    xprintf("    Number of Read DMA Supported : %d\n", ((srio->PAGE0.Global.BCAPR >> 12) & 0xf));
    xprintf("    Depth of Descriptors Array   : %d\n", (1 << ((srio->PAGE0.Global.BCAPR >> 18) & 0x3)));
    xprintf("\n");

#ifdef DEBUG
    memset(_s_local_txbuffer, 0xff, sizeof(_s_local_txbuffer));
    memset(_s_local_rxbuffer, 0xff, sizeof(_s_local_rxbuffer));
#endif

#if 1 // @@
    uint32_t dummy = Reg_Read_32(g_DeviceSrioDefaultConfig.map_base_addr);

    xprintf("nwrite/nwrite_r:\n");
    xprintf("    read (old): %08x\n", *(uint32_t *)(g_DeviceSrioDefaultConfig.mem_base_addr));

    Reg_Write_32(g_DeviceSrioDefaultConfig.map_base_addr, 0x12345678);
    xprintf("    write     : %08x\n", 0x12345678);
    udelay(100);

    xprintf("    read (new): %08x\n", *(uint32_t *)(g_DeviceSrioDefaultConfig.mem_base_addr));
    xprintf("\n");

    Reg_Write_32(g_DeviceSrioDefaultConfig.map_base_addr, 0x12345678);

    xprintf("nread:\n");
    xprintf("    read (old): %08x\n", *(uint32_t *)(g_DeviceSrioDefaultConfig.map_base_addr));

    Reg_Write_32(g_DeviceSrioDefaultConfig.mem_base_addr, 0x87654321);
    xprintf("    write     : %08x\n", 0x87654321);
    udelay(100);

    xprintf("    read (new): %08x\n", *(uint32_t *)(g_DeviceSrioDefaultConfig.map_base_addr));
    xprintf("\n");

    Reg_Write_32(g_DeviceSrioDefaultConfig.map_base_addr, dummy);
#endif

    uint32_t length = 64; // NOTE: length of buffer in testing, must less than total length
    DEBUG_ASSERT((length <= sizeof(_s_local_txbuffer)) && (length <= sizeof(_s_local_rxbuffer)));

    for (uint32_t i = 0; i < length; i++) {
        _s_local_txbuffer[i] = i;
    }

#if 1 // @@
    SRIO_WDMA_Transmit(srio, eSrioEngine_0, (uint32_t)&_s_local_txbuffer, (uint32_t)&_s_local_rxbuffer, length, g_DeviceSrioDefaultConfig.device_id);
    // SRIO_RDMA_Transmit(srio, eSrioEngine_0, (uint32_t)&_s_local_txbuffer, (uint32_t)&_s_local_rxbuffer, length, g_DeviceSrioDefaultConfig.device_id);
#else
    SRIO_WDMA_Transmit2(srio, eSrioEngine_0, (uint32_t)&_s_local_txbuffer, (uint32_t)&_s_local_rxbuffer, length, g_DeviceSrioDefaultConfig.device_id);
    // SRIO_RDMA_Transmit2(srio, eSrioEngine_0, (uint32_t)&_s_local_txbuffer, (uint32_t)&_s_local_rxbuffer, length, g_DeviceSrioDefaultConfig.device_id);
#endif

    mdelay(500);

    xprintf("send %d.\n", length);
    for (uint32_t i = 0; i < length; i++) {
        xprintf("%02x ", _s_local_txbuffer[i]);
    }
    xprintf("\n");

    xprintf("recv %d.\n", length);
    for (uint32_t i = 0; i < length; i++) {
        xprintf("%02x ", _s_local_rxbuffer[i]);
    }
    xprintf("\n");

    xprintf("\n");
    return 0;
}

#endif // #ifdef DEBUG_SRIO
