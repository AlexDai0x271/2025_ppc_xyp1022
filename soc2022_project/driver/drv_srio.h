/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_SRIO_H__
#define __DRV_SRIO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

/**
 * @brief Register Field Define
 */

#define RAB_PAGE_SIZE      0x800 // NOTE: 2K per page
#define PAGE_SELECT_MASK   0xf000ffff
#define PAGE_SELECT_SHIFT  16

/**
 * @brief Register Field Define
 */

/* value of Register */
#define SRIO_DISABLED      (0)
#define SRIO_ENABLED       (1)

/* value of Interrupt.GENIER */
#define GENIER_RDMA_EN     (BIT(3))
#define GENIER_WDMA_EN     (BIT(2))
#define GENIER_RIO_PIO_EN  (BIT(1))
#define GENIER_AMBA_PIO_EN (BIT(0))

/* value of Global.BCTLR */
#define BCTLR_RDMA_EN      (BIT(3))
#define BCTLR_WDMA_EN      (BIT(2))
#define BCTLR_RIO_PIO_EN   (BIT(1))
#define BCTLR_AMBA_PIO_EN  (BIT(0))

/* value of PAGE1.DMA.x_DMAx.xDMACR */
#define DMACR_START        (1 << 0)
#define DMACR_STOP         (0 << 0)

/* value of  */

/**
 * @brief Self Config Param Define
 */

/* Desc Setting Define */

typedef struct
{
    uint32_t valid;
    uint32_t next_desc_pointer_valid;

    uint32_t transmit_length;
    uint32_t source_addr;
    uint32_t dest_addr;

    uint32_t next_desc_addr;
} SRIO_DMA_desc_setting;

/* Node define */

typedef struct
{
    uint32_t desc_ctrl_w0;
    uint32_t source_addr_w1;
    uint32_t dest_addr_w2;
    uint32_t next_desc_addr_w3;
} SRIO_DMA_desc_node;

typedef enum
{
    eSrioMode_RDMA = 0,
    eSrioMode_WDMA,
} SrioMode_e;

typedef enum
{
    eSrioType_Maintenance = 0x0,
    eSrioType_NREAD_NWRITE,
    eSrioType_NREAD_NWRITE_R,
    eSrioType_SWRITE,
} SrioType_e;

typedef enum
{
    eSrioEngine_0 = 0,
    eSrioEngine_1,
    eSrioEngine_2,
    eSrioEngine_3,
    eSrioEngine_4,
    eSrioEngine_5,
    eSrioEngine_6,
    eSrioEngine_7,
} SrioEngine_e;

typedef enum
{
    eSrioWindowSize_1M = 0x0,
    eSrioWindowSize_2M,
    eSrioWindowSize_4M,
    eSrioWindowSize_8M,
    eSrioWindowSize_16M,
    eSrioWindowSize_32M,
    eSrioWindowSize_64M,
    eSrioWindowSize_128M,
    eSrioWindowSize_256M,
    eSrioWindowSize_512M,
    eSrioWindowSize_1G,
    eSrioWindowSize_2G,
} SrioWindowSize_e;

typedef enum
{
    eSrioBitSelect_SID_3_0 = 0,
    eSrioBitSelect_SID_4_1,
    eSrioBitSelect_SID_5_2,
    eSrioBitSelect_SID_6_3,
    eSrioBitSelect_SID_7_4,
} SrioBitSelect_e;

typedef struct
{
    uint32_t status;

    SrioType_e type;

    uint16_t device_id;

    uint32_t cfg_base_addr;
    uint32_t phy_base_addr;
    uint32_t map_base_addr;

    uint32_t mem_base_addr;
    uint32_t mem_size;

    void *user_data;
} DeviceSrioConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceSrioConfig_t g_DeviceSrioDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_SRIO_InitConfig(SRIO_TypeDef *SRIOx, DeviceSrioConfig_t *config);

void RIO_PIO_MAPPING_setting(SRIO_TypeDef *SRIOx, uint32_t lut, SrioWindowSize_e window_size, SrioBitSelect_e bit_select, uint32_t rio_addr);
void AXI_PIO_MAPPING_setting(SRIO_TypeDef *SRIOx, SrioEngine_e engine, SrioType_e type, uint32_t axi_addr, uint32_t rio_addr, uint32_t window_size, uint32_t dest_id);

int SRIO_WDMA_Transmit(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id);
int SRIO_RDMA_Transmit(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id);

int SRIO_WDMA_Transmit2(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id);
int SRIO_RDMA_Transmit2(SRIO_TypeDef *SRIOx, SrioEngine_e engine, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t dest_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_SRIO_H__ */
