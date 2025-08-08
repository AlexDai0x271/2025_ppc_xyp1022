/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef __DRV_GMAC_H__
#define __DRV_GMAC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "XYR3201.h"
#include "lwip/opt.h"
#include "lwip/def.h"

#ifndef GMAC_DESCRIPTOR_NUM
#define GMAC_DESCRIPTOR_NUM (32)
#endif

#ifndef GMAC_BUFFER_SIZE
#define GMAC_BUFFER_SIZE (PBUF_POOL_BUFSIZE)
#endif

#define GET_PHY_INTERFACE()             GMACx = GMAC0

/**
 * @brief Register Field Define
 */

// GMAC Register

/* GMAC MCR */
#define GMAC_MCR_SARC_POS               (28) // 3-bit
#define GMAC_MCR_TWOKPE_POS             (27) // 1-bit
#define GMAC_MCR_SFTERR_POS             (26) // 1-bit
#define GMAC_MCR_CST_POS                (25) // 1-bit
#define GMAC_MCR_TC_POS                 (24) // 1-bit
#define GMAC_MCR_WD_POS                 (23) // 1-bit
#define GMAC_MCR_JD_POS                 (22) // 1-bit
#define GMAC_MCR_BE_POS                 (21) // 1-bit
#define GMAC_MCR_JE_POS                 (20) // 1-bit
#define GMAC_MCR_IFG_POS                (17) // 3-bit
#define GMAC_MCR_DCRS_POS               (16) // 1-bit
#define GMAC_MCR_FES_POS                (14) // 2-bit
#define GMAC_MCR_DO_POS                 (13) // 1-bit
#define GMAC_MCR_LM_POS                 (12) // 1-bit
#define GMAC_MCR_DM_POS                 (11) // 1-bit
#define GMAC_MCR_IPC_POS                (10) // 1-bit
#define GMAC_MCR_DR_POS                 (9)  // 1-bit
#define GMAC_MCR_LUD_POS                (8)  // 1-bit
#define GMAC_MCR_ACS_POS                (7)  // 1-bit
#define GMAC_MCR_BL_POS                 (5)  // 2-bit
#define GMAC_MCR_DC_POS                 (4)  // 1-bit
#define GMAC_MCR_TE_POS                 (3)  // 1-bit
#define GMAC_MCR_RE_POS                 (2)  // 1-bit
#define GMAC_MCR_PRELEN_POS             (0)  // 2-bit

/* GMAC MFF */
#define GMAC_MFF_RA_POS                 (31) // 1-bit
#define GMAC_MFF_DNTU_POS               (21) // 1-bit
#define GMAC_MFF_IPFE_POS               (20) // 1-bit
#define GMAC_MFF_VTFE_POS               (16) // 1-bit
#define GMAC_MFF_HPF_POS                (10) // 1-bit
#define GMAC_MFF_SAF_POS                (9)  // 1-bit
#define GMAC_MFF_SAIF_POS               (8)  // 1-bit
#define GMAC_MFF_PCF_POS                (6)  // 2-bit
#define GMAC_MFF_DBF_POS                (5)  // 1-bit
#define GMAC_MFF_PM_POS                 (4)  // 1-bit
#define GMAC_MFF_DAIF_POS               (3)  // 1-bit
#define GMAC_MFF_HMC_POS                (2)  // 1-bit
#define GMAC_MFF_HUC_POS                (1)  // 1-bit
#define GMAC_MFF_PR_POS                 (0)  // 1-bit

/* GMAC GMIIAR */
#define GMAC_GMIIAR_PA_POS              (11) // 5-bit
#define GMAC_GMIIAR_GR_POS              (6)  // 5-bit
#define GMAC_GMIIAR_CR_POS              (2)  // 4-bit
#define GMAC_GMIIAR_GW_POS              (1)  // 1-bit
#define GMAC_GMIIAR_GB_POS              (0)  // 1-bit

/* GMAC GMIIDR */
#define GMAC_GMIIDR_GD_POS              (0) // 16-bit

/* GMAC FCR */
#define GMAC_FCR_PT_POS                 (16) // 16-bit
#define GMAC_FCR_DZPQ_POS               (7)  // 1-bit
#define GMAC_FCR_PLT_POS                (4)  // 2-bit
#define GMAC_FCR_UP_POS                 (3)  // 1-bit
#define GMAC_FCR_RFE_POS                (2)  // 1-bit
#define GMAC_FCR_TFE_POS                (1)  // 1-bit
#define GMAC_FCR_FCBBPA_POS             (0)  // 1-bit

// GDMA Register

/* GDMA IMR */
#define GDMA_IMR_PCSANCIM_POS           (2) // 1-bit
#define GDMA_IMR_PCSLCHGIM_POS          (1) // 1-bit
#define GDMA_IMR_RGSIMIIIM_POS          (0) // 1-bit

/* GDMA BMR */
#define GDMA_BMR_RIB_POS                (31) // 1-bit
#define GDMA_BMR_PRWG_POS               (28) // 2-bit
#define GDMA_BMR_TXPR_POS               (27) // 1-bit
#define GDMA_BMR_MB_POS                 (26) // 1-bit
#define GDMA_BMR_AAL_POS                (25) // 1-bit
#define GDMA_BMR_PBLX8_POS              (24) // 1-bit
#define GDMA_BMR_USP_POS                (23) // 1-bit
#define GDMA_BMR_RPBL_POS               (17) // 6-bit
#define GDMA_BMR_FB_POS                 (16) // 1-bit
#define GDMA_BMR_PR_POS                 (14) // 2-bit
#define GDMA_BMR_PBL_POS                (8)  // 6-bit
#define GDMA_BMR_ATDS_POS               (7)  // 1-bit
#define GDMA_BMR_DSL_POS                (2)  // 5-bit
#define GDMA_BMR_DA_POS                 (1)  // 1-bit
#define GDMA_BMR_SWR_POS                (0)  // 1-bit

/* GDMA OMR */
#define GDMA_OMR_DT_POS                 (26) // 1-bit
#define GDMA_OMR_RSF_POS                (25) // 1-bit
#define GDMA_OMR_DFF_POS                (24) // 1-bit
#define GDMA_OMR_RFA2_POS               (23) // 1-bit
#define GDMA_OMR_TRFD2_POS              (22) // 1-bit
#define GDMA_OMR_TSF_POS                (21) // 1-bit
#define GDMA_OMR_FTF_POS                (20) // 1-bit
#define GDMA_OMR_TTC_POS                (14) // 3-bit
#define GDMA_OMR_ST_POS                 (13) // 1-bit
#define GDMA_OMR_RFD_POS                (11) // 2-bit
#define GDMA_OMR_RFA_POS                (9)  // 2-bit
#define GDMA_OMR_EFC_POS                (8)  // 1-bit
#define GDMA_OMR_FEF_POS                (7)  // 1-bit
#define GDMA_OMR_FUF_POS                (6)  // 1-bit
#define GDMA_OMR_DGF_POS                (5)  // 1-bit
#define GDMA_OMR_RTC_POS                (3)  // 2-bit
#define GDMA_OMR_OSF_POS                (2)  // 1-bit
#define GDMA_OMR_SR_POS                 (1)  // 1-bit

/**
 * @brief Register Field Define
 */

/* value of Register */
#define GMAC_DISABLED                   (0)
#define GMAC_ENABLED                    (1)

// GMAC Register

/* value of GMIIAR_CR */
#define CR_CSR_60_100M_MDC_DIV_42       (0x0)
#define CR_CSR_100_150M_MDC_DIV_62      (0x1)
#define CR_CSR_20_35M_MDC_DIV_16        (0x2)
#define CR_CSR_35_60M_MDC_DIV_26        (0x3)
#define CR_CSR_150_250M_MDC_DIV_102     (0x4)
#define CR_CSR_250_300M_MDC_DIV_124     (0x5)
#define CR_MDC_DIV_4                    (0x8)
#define CR_MDC_DIV_6                    (0x9)
#define CR_MDC_DIV_8                    (0xa)
#define CR_MDC_DIV_10                   (0xb)
#define CR_MDC_DIV_12                   (0xc)
#define CR_MDC_DIV_14                   (0xd)
#define CR_MDC_DIV_16                   (0xe)
#define CR_MDC_DIV_18                   (0xf)

/* value of GMIIAR_GW */
#define GW_READ                         (0)
#define GW_WRITE                        (1)

// GDMA Register

/* value of BMR_PBL */
#define PBL_LEN_1                       (1)
#define PBL_LEN_2                       (2)
#define PBL_LEN_4                       (4)
#define PBL_LEN_8                       (8)
#define PBL_LEN_16                      (16)
#define PBL_LEN_32                      (32)

/* value of OMR_TTC */
#define TTC_BIT_64                      (0x0)
#define TTC_BIT_128                     (0x1)
#define TTC_BIT_192                     (0x2)
#define TTC_BIT_256                     (0x3)
#define TTC_BIT_40                      (0x4)
#define TTC_BIT_32                      (0x5)
#define TTC_BIT_24                      (0x6)
#define TTC_BIT_16                      (0x7)

/* value of OMR_RTC */
#define RTC_BIT_64                      (0x0)
#define RTC_BIT_32                      (0x1)
#define RTC_BIT_96                      (0x2)
#define RTC_BIT_128                     (0x3)

/**
 * @brief Self Config Param Define
 */

#define GET_GMAC_HANDLE(x)              ((GMAC_TypeDef *)(x))
#define GET_GDMA_HANDLE(x)              ((GDMA_TypeDef *)(x))

#define BYTEARRAY2DWORD(b0, b1, b2, b3) (((b0) << 24) | ((b1) << 16) | ((b2) << 8) | (b3))

#define GMAC_DEFAULT_MAC_VALUE             \
    {                                      \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
    }

#define GMAC_GET_RX_LENGTH(x) (((x) >> 16) & 0x3fff)

typedef enum
{
    eGmacMode_TxRx = 0,
    eGmacMode_TxOnly,
    eGmacMode_RxOnly,
} GmacMode_e;

typedef enum
{
    eGmacSpeed_1000M = 0,
    eGmacSpeed_Nouse,
    eGmacSpeed_10M,
    eGmacSpeed_100M,
} GmacSpeed_e;

typedef struct
{
    uint8_t value[6];
} GmacMAC_t;

#if BYTE_ORDER == LITTLE_ENDIAN

typedef struct _GmacDes
{
    struct
    {
        uint32_t status : 31;
        uint32_t owner  : 1;
    };
    union
    {
        struct
        {
            uint32_t buffer1_size : 11;
            uint32_t buffer2_size : 11;
            uint32_t TTSE         : 1;
            uint32_t DP           : 1;
            uint32_t TCH          : 1;
            uint32_t TER          : 1;
            uint32_t DC           : 1;
            uint32_t CIC          : 2;
            uint32_t FS           : 1;
            uint32_t LS           : 1;
            uint32_t IC           : 1;
        } tx;
        struct
        {
            uint32_t buffer1_size : 11;
            uint32_t buffer2_size : 11;
            uint32_t reserved2    : 2;
            uint32_t RCH          : 1;
            uint32_t RER          : 1;
            uint32_t reserved1    : 5;
            uint32_t DIC          : 1;
        } rx;
    };
    uint32_t buffer1_addr;
    uint32_t next;
} GmacDes_t; // little-endian

#else

typedef struct _GmacDes
{
    struct
    {
        uint32_t owner  : 1;
        uint32_t status : 31;
    };
    union
    {
        struct
        {
            uint32_t IC           : 1;
            uint32_t LS           : 1;
            uint32_t FS           : 1;
            uint32_t CIC          : 2;
            uint32_t DC           : 1;
            uint32_t TER          : 1;
            uint32_t TCH          : 1;
            uint32_t DP           : 1;
            uint32_t TTSE         : 1;
            uint32_t buffer2_size : 11;
            uint32_t buffer1_size : 11;
        } tx;
        struct
        {
            uint32_t DIC          : 1;
            uint32_t reserved1    : 5;
            uint32_t RER          : 1;
            uint32_t RCH          : 1;
            uint32_t reserved2    : 2;
            uint32_t buffer2_size : 11;
            uint32_t buffer1_size : 11;
        } rx;
    };
    uint32_t buffer1_addr;
    uint32_t next;
} GmacDes_t; // big-endian

#endif

typedef struct
{
    uint32_t status;

    GmacMode_e mode;
    GmacSpeed_e speed;

    GMAC_TypeDef *gmac;
    GDMA_TypeDef *gdma;

    uint8_t phy_id;

    uint8_t mac_num;
    GmacMAC_t mac[16];

    uint32_t tx_desc_num;
    uint32_t rx_desc_num;
    GmacDes_t *tx_desc;
    GmacDes_t *rx_desc;

    uint32_t tx_buffer_size;
    uint32_t rx_buffer_size;
    uint8_t *tx_buffer;
    uint8_t *rx_buffer;

    uint8_t loopback;

    GmacDes_t *rx_desc_start;

    uint32_t rx_buffer_list[GMAC_DESCRIPTOR_NUM];
    uint32_t rx_buffer_len[GMAC_DESCRIPTOR_NUM];
    uint32_t rx_buffer_total_len;
    uint32_t rx_buffer_count;

    void *user_data;
} DeviceGmacConfig_t;

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
extern DeviceGmacConfig_t g_DeviceGmacDefaultConfig;
#endif // #ifdef DEBUG

extern uint8_t g_gmac_tx_flag[NUM_OF_GMAC];
extern uint8_t g_gmac_rx_flag[NUM_OF_GMAC];

/**
 * @brief Function Define
 */

int HW_GMAC_InitConfig(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);
int HW_GMAC_Reset(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config, uint32_t timeout);
int HW_GMAC_SetMAC(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);
void HW_GMAC_InitTxDescriptor(GmacDes_t *descriptor, uint32_t num, uint8_t *pbuf, uint32_t len);
void HW_GMAC_InitRxDescriptor(GmacDes_t *descriptor, uint32_t num, uint8_t *pbuf, uint32_t len);

#ifdef DEBUG_GMAC
void HW_GMAC_GenTxDescriptor(GmacDes_t *descriptor, uint8_t *pbuf, uint32_t len);
uint32_t HW_GMAC_GenPacket(uint8_t *pbuf, uint8_t *target_mac, uint8_t *source_mac, uint16_t type, uint8_t *pdata, uint32_t len);
int HW_GMAC_SendPacket(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config, uint8_t *pbuf, uint32_t len);
#endif // #ifdef DEBUG_GMAC

int HW_GMAC_GetSendPacket(GMAC_TypeDef *GMACx, uint32_t *pbuf);
int HW_GMAC_StartSendPacket(GMAC_TypeDef *GMACx, uint32_t len);

int HW_GMAC_GetRecvPacket(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);
int HW_GMAC_StopRecvPacket(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_GMAC_H__ */
