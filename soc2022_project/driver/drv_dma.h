/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_DMA_H__
#define __DRV_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"

// NOTE: maximum number of transfers per transaction
#define DMA_FIFO_MAX                          (64)

/**
 * @brief Register Field Define
 */

/* DMA CFG */
#define DMA_CFG_INT_EN_POS                    (1) // 1-bit
#define DMA_CFG_DMAC_EN_POS                   (0) // 1-bit

/* DMA RESET */
#define DMA_RESET_DMAC_RST_POS                (0) // 1-bit

/* DMA CHx_CTL */
#define DMA_CH_CTL_SHADOWREG_OR_LLI_VALID_POS (63) // 1-bit
#define DMA_CH_CTL_SHADOWREG_OR_LLI_LAST_POS  (62) // 1-bit
#define DMA_CH_CTL_IOC_BLKTFR_POS             (58) // 1-bit
#define DMA_CH_CTL_DST_STAT_EN_POS            (57) // 1-bit
#define DMA_CH_CTL_SRC_STAT_EN_POS            (56) // 1-bit
#define DMA_CH_CTL_AWLEN_POS                  (48) // 8-bit
#define DMA_CH_CTL_AWLEN_EN_POS               (47) // 1-bit
#define DMA_CH_CTL_ARLEN_POS                  (39) // 8-bit
#define DMA_CH_CTL_ARLEN_EN_POS               (38) // 1-bit
#define DMA_CH_CTL_AW_PORT_POS                (35) // 3-bit
#define DMA_CH_CTL_AR_PORT_POS                (32) // 3-bit
#define DMA_CH_CTL_NONPOSTED_LASTWRITE_EN_POS (30) // 1-bit
#define DMA_CH_CTL_AW_CACHE_POS               (26) // 4-bit
#define DMA_CH_CTL_AR_CACHE_POS               (22) // 4-bit
#define DMA_CH_CTL_DST_MSIZE_POS              (18) // 4-bit
#define DMA_CH_CTL_SRC_MSIZE_POS              (14) // 4-bit
#define DMA_CH_CTL_DST_TR_WIDTH_POS           (11) // 3-bit
#define DMA_CH_CTL_SRC_TR_WIDTH_POS           (8)  // 3-bit
#define DMA_CH_CTL_DINC_POS                   (6)  // 1-bit
#define DMA_CH_CTL_SINC_POS                   (4)  // 1-bit
#define DMA_CH_CTL_DMS_POS                    (2)  // 1-bit
#define DMA_CH_CTL_SMS_POS                    (0)  // 1-bit

/* DMA CHx_CFG */
#define DMA_CH_CFG_DST_OSR_LMT_POS            (59) // 4-bit
#define DMA_CH_CFG_SRC_OSR_LMT_POS            (55) // 4-bit
#define DMA_CH_CFG_LOCK_CH_L_POS              (53) // 2-bit
#define DMA_CH_CFG_LOCK_CH_POS                (52) // 1-bit
#define DMA_CH_CFG_CH_PRIOR_POS               (49) // 5-bit
#define DMA_CH_CFG_DST_PER_POS                (44) // 6-bit
#define DMA_CH_CFG_SRC_PER_POS                (39) // 6-bit
#define DMA_CH_CFG_DST_HWHS_POL_POS           (38) // 1-bit
#define DMA_CH_CFG_SRC_HWHS_POL_POS           (37) // 1-bit
#define DMA_CH_CFG_HS_SEL_DST_POS             (36) // 1-bit
#define DMA_CH_CFG_HS_SEL_SRC_POS             (35) // 1-bit
#define DMA_CH_CFG_TT_FC_POS                  (32) // 3-bit
#define DMA_CH_CFG_DST_MULTBLK_TYPE_POS       (2)  // 2-bit
#define DMA_CH_CFG_SRC_MULTBLK_TYPE_POS       (0)  // 2-bit

/* DMA CHx_LLP */
#define DMA_CH_LLP_LOC_POS                    (6) // 58-bit
#define DMA_CH_LLP_LMS_POS                    (0) // 1-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define DMA_DISABLED                          (0)
#define DMA_ENABLED                           (1)

/* value of CHx_CTL_MC */
#define MC_AXI_MASTER_1                       (0)
#define MC_AXI_MASTER_2                       (1)

/* value of CHx_CFG_LOCK_CH_L */
#define LOCK_CH_L_DMA                         (0x0ULL)
#define LOCK_CH_L_BLOCK                       (0x1ULL)

/* value of CHx_CFG_HS_SEL */
#define HS_SEL_HARDWARE                       (0)
#define HS_SEL_SOFTWARE                       (1)

/* value of CHx_CFG_MULTBLK_TYPE */
#define MULTBLK_TYPE_CONTIGUOUS               (0x0ULL)
#define MULTBLK_TYPE_RELOAD                   (0x1ULL)
#define MULTBLK_TYPE_SHADOW_REGISTER          (0x2ULL)
#define MULTBLK_TYPE_LINKED_LIST              (0x3ULL)

/**
 * @brief Self Config Param Define
 */

typedef enum
{
    eDmaMode_Mem2Mem = 0,
    eDmaMode_Mem2Dev,
    eDmaMode_Dev2Dev,
    eDmaMode_Dev2Mem,
} DmaMode_e;

typedef enum
{
    eDmaChannel_0 = 0,
    eDmaChannel_1,
    eDmaChannel_2,
    eDmaChannel_3,
    eDmaChannel_4,
    eDmaChannel_5,
    eDmaChannel_6,
    eDmaChannel_7,

    eDmaChannel_Max,
} DmaChannel_e;

typedef enum
{
    eDmaAddrMode_Inc = 0,
    eDmaAddrMode_Fixed,
} DmaAddrMode_e;

typedef enum
{
    eDmaDataWidth_8Bit = 0,
    eDmaDataWidth_16Bit,
    eDmaDataWidth_32Bit,
    eDmaDataWidth_64Bit,
    eDmaDataWidth_128Bit,
    eDmaDataWidth_256Bit,
    eDmaDataWidth_512Bit,
} DmaDataWidth_e;

typedef enum
{
    eDmaDataDepth_1 = 0,
    eDmaDataDepth_4,
    eDmaDataDepth_8,
    eDmaDataDepth_16,
    eDmaDataDepth_32,
    eDmaDataDepth_64,
    eDmaDataDepth_128,
    eDmaDataDepth_256,
    eDmaDataDepth_512,
    eDmaDataDepth_1024,
} DmaDataDepth_e;

typedef struct
{
    uint32_t status;

    DmaMode_e mode;
    DmaChannel_e channel;

    uint32_t transfer_size;
    uint8_t priority; // NOTE: from '0' to '31', '0' is lowest

    uint64_t src_addr;
    DmaAddrMode_e src_addr_mode;
    DmaDataWidth_e src_data_width;
    DmaDataDepth_e src_data_depth;

    uint64_t dst_addr;
    DmaAddrMode_e dst_addr_mode;
    DmaDataWidth_e dst_data_width;
    DmaDataDepth_e dst_data_depth;

    void *user_data;
} DeviceDmaConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceDmaConfig_t g_DeviceDmaDefaultConfig;
#endif // #ifdef DEBUG

/**
 * @brief Function Define
 */

int HW_DMA_InitConfig(DMA_TypeDef *DMAx, DeviceDmaConfig_t *config);
int HW_DMA_Reset(DMA_TypeDef *DMAx);
int HW_DMA_ChannelEnable(DMA_TypeDef *DMAx, DmaChannel_e chn);
int HW_DMA_ChannelDisable(DMA_TypeDef *DMAx, DmaChannel_e chn);
int HW_DMA_ChannelSuspend(DMA_TypeDef *DMAx, DmaChannel_e chn);
int HW_DMA_ChannelAbort(DMA_TypeDef *DMAx, DmaChannel_e chn);
int HW_DMA_WaitComplete(DMA_TypeDef *DMAx, DmaChannel_e chn);
uint64_t HW_DMA_GetStatus(DMA_TypeDef *DMAx, DmaChannel_e chn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_DMA_H__ */
