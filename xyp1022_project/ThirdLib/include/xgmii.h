#define MAC0_BASE_ADDR                 0xeb800000 
#define MAC1_BASE_ADDR                 0xeb804000
#define XPCS0_BASE_ADDR		       0xeb808000
#define XPCS1_BASE_ADDR		       0xeb808200
#define PHY_MISC_BASE_ADDR	       0xeb808400
#define CH_NUM0                        0
#define CH_NUM1                        1
#define CH_NUM2                        2
#define CH_NUM3                        3
#define  XPCS_TOP0_NUM0        	      0x0 
#define  XPCS_TOP0_NUM1        	      0x1 

#define  XGMAC0_PMT_NUM 		73 
#define  XGMAC0_MAC_NUM  		72
#define  XGMAC0_CH_TX_NUM		71
#define  XGMAC0_CH_RX_NUM		70
#define  XGMAC0_XPCS_NUM 		69
#define  XGMAC1_PMT_NUM  		68
#define  XGMAC1_MAC_NUM  		67
#define  XGMAC1_CH_TX_NUM		66
#define  XGMAC1_CH_RX_NUM		65
#define  XGMAC1_XPCS_NUM 		64

// DMA Register
//xgmii 0
#define DMA0_BUS_MODE                          MAC0_BASE_ADDR + (0x3000)
#define DMA0_AXI_BUS_MODE                      MAC0_BASE_ADDR + (0x3004)
#define DMA0_CMN_STS                           MAC0_BASE_ADDR + (0x3008)
#define DMA0_AXI_ACE_RD_CTRL                   MAC0_BASE_ADDR + (0x3010)
#define DMA0_AXI_ACE_WR_CTRL                   MAC0_BASE_ADDR + (0x3018)
#define DMA0_TX_DESC_PREF_CTL                  MAC0_BASE_ADDR + (0x3040)
#define DMA0_RX_DESC_PREF_CTL                  MAC0_BASE_ADDR + (0x3044)
#define DMA0_AXI_ACE_ARAWC_AWARP_CTRL          MAC0_BASE_ADDR + (0x301C) //new register
//dma0 channel 0
#define DMA0_CH_CTRL_REG                       MAC0_BASE_ADDR + (0x3100 + CH_NUM0*128)
#define DMA0_CH_TX_CTRL_REG                    MAC0_BASE_ADDR + (0x3104 + CH_NUM0*128)
#define DMA0_CH_RX_CTRL_REG                    MAC0_BASE_ADDR + (0x3108 + CH_NUM0*128)
#define DMA0_CH_SLOT_FUNC                      MAC0_BASE_ADDR + (0x310C + CH_NUM0*128)
#define DMA0_CH_TX_DESC_HLST                   MAC0_BASE_ADDR + (0x3110 + CH_NUM0*128)
#define DMA0_CH_TX_DESC_LLST                   MAC0_BASE_ADDR + (0x3114 + CH_NUM0*128)
#define DMA0_CH_RX_DESC_HLST                   MAC0_BASE_ADDR + (0x3118 + CH_NUM0*128)
#define DMA0_CH_RX_DESC_LLST                   MAC0_BASE_ADDR + (0x311C + CH_NUM0*128)
#define DMA0_CH_TX_DESC_TPH                    MAC0_BASE_ADDR + (0x3120 + CH_NUM0*128)
#define DMA0_CH_TX_DESC_TPL                    MAC0_BASE_ADDR + (0x3124 + CH_NUM0*128)
#define DMA0_CH_RX_DESC_TPH                    MAC0_BASE_ADDR + (0x3128 + CH_NUM0*128)
#define DMA0_CH_RX_DESC_TPL                    MAC0_BASE_ADDR + (0x312C + CH_NUM0*128)
#define DMA0_CH_TX_RING_LEN                    MAC0_BASE_ADDR + (0x3130 + CH_NUM0*128)
#define DMA0_CH_RX_RING_LEN                    MAC0_BASE_ADDR + (0x3134 + CH_NUM0*128)
#define DMA0_CH_INTT_EN                        MAC0_BASE_ADDR + (0x3138 + CH_NUM0*128)
#define DMA0_CH_INTT_WDOG                      MAC0_BASE_ADDR + (0x313C + CH_NUM0*128)
#define DMA0_CH_CUR_TX_DESCH                   MAC0_BASE_ADDR + (0x3140 + CH_NUM0*128)
#define DMA0_CH_CUR_TX_DESCL                   MAC0_BASE_ADDR + (0x3144 + CH_NUM0*128)
#define DMA0_CH_CUR_RX_DESCH                   MAC0_BASE_ADDR + (0x3148 + CH_NUM0*128)
#define DMA0_CH_CUR_RX_DESCL                   MAC0_BASE_ADDR + (0x314C + CH_NUM0*128)
#define DMA0_CH_CUR_TX_BUFH                    MAC0_BASE_ADDR + (0x3150 + CH_NUM0*128)
#define DMA0_CH_CUR_TX_BUFL                    MAC0_BASE_ADDR + (0x3154 + CH_NUM0*128)
#define DMA0_CH_CUR_RX_BUFH                    MAC0_BASE_ADDR + (0x3158 + CH_NUM0*128)
#define DMA0_CH_CUR_RX_BUFL                    MAC0_BASE_ADDR + (0x315C + CH_NUM0*128)
#define DMA0_CH_STS                            MAC0_BASE_ADDR + (0x3160 + CH_NUM0*128)
#define DMA0_CH_AXI_CTRL                       MAC0_BASE_ADDR + (0x3164 + CH_NUM0*64)
//dma0 channel 1
#define DMA0_CH1_CTRL_REG                       MAC0_BASE_ADDR + (0x3100 + CH_NUM1*128)
#define DMA0_CH1_TX_CTRL_REG                    MAC0_BASE_ADDR + (0x3104 + CH_NUM1*128)
#define DMA0_CH1_RX_CTRL_REG                    MAC0_BASE_ADDR + (0x3108 + CH_NUM1*128)
#define DMA0_CH1_SLOT_FUNC                      MAC0_BASE_ADDR + (0x310C + CH_NUM1*128)
#define DMA0_CH1_TX_DESC_HLST                   MAC0_BASE_ADDR + (0x3110 + CH_NUM1*128)
#define DMA0_CH1_TX_DESC_LLST                   MAC0_BASE_ADDR + (0x3114 + CH_NUM1*128)
#define DMA0_CH1_RX_DESC_HLST                   MAC0_BASE_ADDR + (0x3118 + CH_NUM1*128)
#define DMA0_CH1_RX_DESC_LLST                   MAC0_BASE_ADDR + (0x311C + CH_NUM1*128)
#define DMA0_CH1_TX_DESC_TPH                    MAC0_BASE_ADDR + (0x3120 + CH_NUM1*128)
#define DMA0_CH1_TX_DESC_TPL                    MAC0_BASE_ADDR + (0x3124 + CH_NUM1*128)
#define DMA0_CH1_RX_DESC_TPH                    MAC0_BASE_ADDR + (0x3128 + CH_NUM1*128)
#define DMA0_CH1_RX_DESC_TPL                    MAC0_BASE_ADDR + (0x312C + CH_NUM1*128)
#define DMA0_CH1_TX_RING_LEN                    MAC0_BASE_ADDR + (0x3130 + CH_NUM1*128)
#define DMA0_CH1_RX_RING_LEN                    MAC0_BASE_ADDR + (0x3134 + CH_NUM1*128)
#define DMA0_CH1_INTT_EN                        MAC0_BASE_ADDR + (0x3138 + CH_NUM1*128)
#define DMA0_CH1_INTT_WDOG                      MAC0_BASE_ADDR + (0x313C + CH_NUM1*128)
#define DMA0_CH1_CUR_TX_DESCH                   MAC0_BASE_ADDR + (0x3140 + CH_NUM1*128)
#define DMA0_CH1_CUR_TX_DESCL                   MAC0_BASE_ADDR + (0x3144 + CH_NUM1*128)
#define DMA0_CH1_CUR_RX_DESCH                   MAC0_BASE_ADDR + (0x3148 + CH_NUM1*128)
#define DMA0_CH1_CUR_RX_DESCL                   MAC0_BASE_ADDR + (0x314C + CH_NUM1*128)
#define DMA0_CH1_CUR_TX_BUFH                    MAC0_BASE_ADDR + (0x3150 + CH_NUM1*128)
#define DMA0_CH1_CUR_TX_BUFL                    MAC0_BASE_ADDR + (0x3154 + CH_NUM1*128)
#define DMA0_CH1_CUR_RX_BUFH                    MAC0_BASE_ADDR + (0x3158 + CH_NUM1*128)
#define DMA0_CH1_CUR_RX_BUFL                    MAC0_BASE_ADDR + (0x315C + CH_NUM1*128)
#define DMA0_CH1_STS                            MAC0_BASE_ADDR + (0x3160 + CH_NUM1*128)
#define DMA0_CH1_AXI_CTRL                       MAC0_BASE_ADDR + (0x3164 + CH_NUM1*64)
//dma channel 2
#define DMA0_CH2_CTRL_REG                       MAC0_BASE_ADDR + (0x3100 + CH_NUM2*128)
#define DMA0_CH2_TX_CTRL_REG                    MAC0_BASE_ADDR + (0x3104 + CH_NUM2*128)
#define DMA0_CH2_RX_CTRL_REG                    MAC0_BASE_ADDR + (0x3108 + CH_NUM2*128)
#define DMA0_CH2_SLOT_FUNC                      MAC0_BASE_ADDR + (0x310C + CH_NUM2*128)
#define DMA0_CH2_TX_DESC_HLST                   MAC0_BASE_ADDR + (0x3110 + CH_NUM2*128)
#define DMA0_CH2_TX_DESC_LLST                   MAC0_BASE_ADDR + (0x3114 + CH_NUM2*128)
#define DMA0_CH2_RX_DESC_HLST                   MAC0_BASE_ADDR + (0x3118 + CH_NUM2*128)
#define DMA0_CH2_RX_DESC_LLST                   MAC0_BASE_ADDR + (0x311C + CH_NUM2*128)
#define DMA0_CH2_TX_DESC_TPH                    MAC0_BASE_ADDR + (0x3120 + CH_NUM2*128)
#define DMA0_CH2_TX_DESC_TPL                    MAC0_BASE_ADDR + (0x3124 + CH_NUM2*128)
#define DMA0_CH2_RX_DESC_TPH                    MAC0_BASE_ADDR + (0x3128 + CH_NUM2*128)
#define DMA0_CH2_RX_DESC_TPL                    MAC0_BASE_ADDR + (0x312C + CH_NUM2*128)
#define DMA0_CH2_TX_RING_LEN                    MAC0_BASE_ADDR + (0x3130 + CH_NUM2*128)
#define DMA0_CH2_RX_RING_LEN                    MAC0_BASE_ADDR + (0x3134 + CH_NUM2*128)
#define DMA0_CH2_INTT_EN                        MAC0_BASE_ADDR + (0x3138 + CH_NUM2*128)
#define DMA0_CH2_INTT_WDOG                      MAC0_BASE_ADDR + (0x313C + CH_NUM2*128)
#define DMA0_CH2_CUR_TX_DESCH                   MAC0_BASE_ADDR + (0x3140 + CH_NUM2*128)
#define DMA0_CH2_CUR_TX_DESCL                   MAC0_BASE_ADDR + (0x3144 + CH_NUM2*128)
#define DMA0_CH2_CUR_RX_DESCH                   MAC0_BASE_ADDR + (0x3148 + CH_NUM2*128)
#define DMA0_CH2_CUR_RX_DESCL                   MAC0_BASE_ADDR + (0x314C + CH_NUM2*128)
#define DMA0_CH2_CUR_TX_BUFH                    MAC0_BASE_ADDR + (0x3150 + CH_NUM2*128)
#define DMA0_CH2_CUR_TX_BUFL                    MAC0_BASE_ADDR + (0x3154 + CH_NUM2*128)
#define DMA0_CH2_CUR_RX_BUFH                    MAC0_BASE_ADDR + (0x3158 + CH_NUM2*128)
#define DMA0_CH2_CUR_RX_BUFL                    MAC0_BASE_ADDR + (0x315C + CH_NUM2*128)
#define DMA0_CH2_STS                            MAC0_BASE_ADDR + (0x3160 + CH_NUM2*128)
#define DMA0_CH2_AXI_CTRL                       MAC0_BASE_ADDR + (0x3164 + CH_NUM2*64)
//dma channel 3
#define DMA0_CH3_CTRL_REG                       MAC0_BASE_ADDR + (0x3100 + CH_NUM3*128)
#define DMA0_CH3_TX_CTRL_REG                    MAC0_BASE_ADDR + (0x3104 + CH_NUM3*128)
#define DMA0_CH3_RX_CTRL_REG                    MAC0_BASE_ADDR + (0x3108 + CH_NUM3*128)
#define DMA0_CH3_SLOT_FUNC                      MAC0_BASE_ADDR + (0x310C + CH_NUM3*128)
#define DMA0_CH3_TX_DESC_HLST                   MAC0_BASE_ADDR + (0x3110 + CH_NUM3*128)
#define DMA0_CH3_TX_DESC_LLST                   MAC0_BASE_ADDR + (0x3114 + CH_NUM3*128)
#define DMA0_CH3_RX_DESC_HLST                   MAC0_BASE_ADDR + (0x3118 + CH_NUM3*128)
#define DMA0_CH3_RX_DESC_LLST                   MAC0_BASE_ADDR + (0x311C + CH_NUM3*128)
#define DMA0_CH3_TX_DESC_TPH                    MAC0_BASE_ADDR + (0x3120 + CH_NUM3*128)
#define DMA0_CH3_TX_DESC_TPL                    MAC0_BASE_ADDR + (0x3124 + CH_NUM3*128)
#define DMA0_CH3_RX_DESC_TPH                    MAC0_BASE_ADDR + (0x3128 + CH_NUM3*128)
#define DMA0_CH3_RX_DESC_TPL                    MAC0_BASE_ADDR + (0x312C + CH_NUM3*128)
#define DMA0_CH3_TX_RING_LEN                    MAC0_BASE_ADDR + (0x3130 + CH_NUM3*128)
#define DMA0_CH3_RX_RING_LEN                    MAC0_BASE_ADDR + (0x3134 + CH_NUM3*128)
#define DMA0_CH3_INTT_EN                        MAC0_BASE_ADDR + (0x3138 + CH_NUM3*128)
#define DMA0_CH3_INTT_WDOG                      MAC0_BASE_ADDR + (0x313C + CH_NUM3*128)
#define DMA0_CH3_CUR_TX_DESCH                   MAC0_BASE_ADDR + (0x3140 + CH_NUM3*128)
#define DMA0_CH3_CUR_TX_DESCL                   MAC0_BASE_ADDR + (0x3144 + CH_NUM3*128)
#define DMA0_CH3_CUR_RX_DESCH                   MAC0_BASE_ADDR + (0x3148 + CH_NUM3*128)
#define DMA0_CH3_CUR_RX_DESCL                   MAC0_BASE_ADDR + (0x314C + CH_NUM3*128)
#define DMA0_CH3_CUR_TX_BUFH                    MAC0_BASE_ADDR + (0x3150 + CH_NUM3*128)
#define DMA0_CH3_CUR_TX_BUFL                    MAC0_BASE_ADDR + (0x3154 + CH_NUM3*128)
#define DMA0_CH3_CUR_RX_BUFH                    MAC0_BASE_ADDR + (0x3158 + CH_NUM3*128)
#define DMA0_CH3_CUR_RX_BUFL                    MAC0_BASE_ADDR + (0x315C + CH_NUM3*128)
#define DMA0_CH3_STS                            MAC0_BASE_ADDR + (0x3160 + CH_NUM3*128)
#define DMA0_CH3_AXI_CTRL                       MAC0_BASE_ADDR + (0x3164 + CH_NUM3*64)

//xgmii 1 controller
#define DMA1_BUS_MODE                          MAC1_BASE_ADDR + (0x3000)
#define DMA1_AXI_BUS_MODE                      MAC1_BASE_ADDR + (0x3004)
#define DMA1_CMN_STS                           MAC1_BASE_ADDR + (0x3008)
#define DMA1_AXI_ACE_RD_CTRL                   MAC1_BASE_ADDR + (0x3010)
#define DMA1_AXI_ACE_WR_CTRL                   MAC1_BASE_ADDR + (0x3018)
#define DMA1_TX_DESC_PREF_CTL                  MAC1_BASE_ADDR + (0x3040)
#define DMA1_RX_DESC_PREF_CTL                  MAC1_BASE_ADDR + (0x3044)
#define DMA1_AXI_ACE_ARAWC_AWARP_CTRL          MAC1_BASE_ADDR + (0x301C) //new register
//
#define DMA1_CH_CTRL_REG                       MAC1_BASE_ADDR + (0x3100 + CH_NUM0*128)
#define DMA1_CH_TX_CTRL_REG                    MAC1_BASE_ADDR + (0x3104 + CH_NUM0*128)
#define DMA1_CH_RX_CTRL_REG                    MAC1_BASE_ADDR + (0x3108 + CH_NUM0*128)
#define DMA1_CH_SLOT_FUNC                      MAC1_BASE_ADDR + (0x310C + CH_NUM0*128)
#define DMA1_CH_TX_DESC_HLST                   MAC1_BASE_ADDR + (0x3110 + CH_NUM0*128)
#define DMA1_CH_TX_DESC_LLST                   MAC1_BASE_ADDR + (0x3114 + CH_NUM0*128)
#define DMA1_CH_RX_DESC_HLST                   MAC1_BASE_ADDR + (0x3118 + CH_NUM0*128)
#define DMA1_CH_RX_DESC_LLST                   MAC1_BASE_ADDR + (0x311C + CH_NUM0*128)
#define DMA1_CH_TX_DESC_TPH                    MAC1_BASE_ADDR + (0x3120 + CH_NUM0*128)
#define DMA1_CH_TX_DESC_TPL                    MAC1_BASE_ADDR + (0x3124 + CH_NUM0*128)
#define DMA1_CH_RX_DESC_TPH                    MAC1_BASE_ADDR + (0x3128 + CH_NUM0*128)
#define DMA1_CH_RX_DESC_TPL                    MAC1_BASE_ADDR + (0x312C + CH_NUM0*128)
#define DMA1_CH_TX_RING_LEN                    MAC1_BASE_ADDR + (0x3130 + CH_NUM0*128)
#define DMA1_CH_RX_RING_LEN                    MAC1_BASE_ADDR + (0x3134 + CH_NUM0*128)
#define DMA1_CH_INTT_EN                        MAC1_BASE_ADDR + (0x3138 + CH_NUM0*128)
#define DMA1_CH_INTT_WDOG                      MAC1_BASE_ADDR + (0x313C + CH_NUM0*128)
#define DMA1_CH_CUR_TX_DESCH                   MAC1_BASE_ADDR + (0x3140 + CH_NUM0*128)
#define DMA1_CH_CUR_TX_DESCL                   MAC1_BASE_ADDR + (0x3144 + CH_NUM0*128)
#define DMA1_CH_CUR_RX_DESCH                   MAC1_BASE_ADDR + (0x3148 + CH_NUM0*128)
#define DMA1_CH_CUR_RX_DESCL                   MAC1_BASE_ADDR + (0x314C + CH_NUM0*128)
#define DMA1_CH_CUR_TX_BUFH                    MAC1_BASE_ADDR + (0x3150 + CH_NUM0*128)
#define DMA1_CH_CUR_TX_BUFL                    MAC1_BASE_ADDR + (0x3154 + CH_NUM0*128)
#define DMA1_CH_CUR_RX_BUFH                    MAC1_BASE_ADDR + (0x3158 + CH_NUM0*128)
#define DMA1_CH_CUR_RX_BUFL                    MAC1_BASE_ADDR + (0x315C + CH_NUM0*128)
#define DMA1_CH_STS                            MAC1_BASE_ADDR + (0x3160 + CH_NUM0*128)
#define DMA1_CH_AXI_CTRL                       MAC1_BASE_ADDR + (0x3164 + CH_NUM0*64)

//dma0 channel 1
#define DMA1_CH1_CTRL_REG                       MAC1_BASE_ADDR + (0x3100 + CH_NUM1*128)
#define DMA1_CH1_TX_CTRL_REG                    MAC1_BASE_ADDR + (0x3104 + CH_NUM1*128)
#define DMA1_CH1_RX_CTRL_REG                    MAC1_BASE_ADDR + (0x3108 + CH_NUM1*128)
#define DMA1_CH1_SLOT_FUNC                      MAC1_BASE_ADDR + (0x310C + CH_NUM1*128)
#define DMA1_CH1_TX_DESC_HLST                   MAC1_BASE_ADDR + (0x3110 + CH_NUM1*128)
#define DMA1_CH1_TX_DESC_LLST                   MAC1_BASE_ADDR + (0x3114 + CH_NUM1*128)
#define DMA1_CH1_RX_DESC_HLST                   MAC1_BASE_ADDR + (0x3118 + CH_NUM1*128)
#define DMA1_CH1_RX_DESC_LLST                   MAC1_BASE_ADDR + (0x311C + CH_NUM1*128)
#define DMA1_CH1_TX_DESC_TPH                    MAC1_BASE_ADDR + (0x3120 + CH_NUM1*128)
#define DMA1_CH1_TX_DESC_TPL                    MAC1_BASE_ADDR + (0x3124 + CH_NUM1*128)
#define DMA1_CH1_RX_DESC_TPH                    MAC1_BASE_ADDR + (0x3128 + CH_NUM1*128)
#define DMA1_CH1_RX_DESC_TPL                    MAC1_BASE_ADDR + (0x312C + CH_NUM1*128)
#define DMA1_CH1_TX_RING_LEN                    MAC1_BASE_ADDR + (0x3130 + CH_NUM1*128)
#define DMA1_CH1_RX_RING_LEN                    MAC1_BASE_ADDR + (0x3134 + CH_NUM1*128)
#define DMA1_CH1_INTT_EN                        MAC1_BASE_ADDR + (0x3138 + CH_NUM1*128)
#define DMA1_CH1_INTT_WDOG                      MAC1_BASE_ADDR + (0x313C + CH_NUM1*128)
#define DMA1_CH1_CUR_TX_DESCH                   MAC1_BASE_ADDR + (0x3140 + CH_NUM1*128)
#define DMA1_CH1_CUR_TX_DESCL                   MAC1_BASE_ADDR + (0x3144 + CH_NUM1*128)
#define DMA1_CH1_CUR_RX_DESCH                   MAC1_BASE_ADDR + (0x3148 + CH_NUM1*128)
#define DMA1_CH1_CUR_RX_DESCL                   MAC1_BASE_ADDR + (0x314C + CH_NUM1*128)
#define DMA1_CH1_CUR_TX_BUFH                    MAC1_BASE_ADDR + (0x3150 + CH_NUM1*128)
#define DMA1_CH1_CUR_TX_BUFL                    MAC1_BASE_ADDR + (0x3154 + CH_NUM1*128)
#define DMA1_CH1_CUR_RX_BUFH                    MAC1_BASE_ADDR + (0x3158 + CH_NUM1*128)
#define DMA1_CH1_CUR_RX_BUFL                    MAC1_BASE_ADDR + (0x315C + CH_NUM1*128)
#define DMA1_CH1_STS                            MAC1_BASE_ADDR + (0x3160 + CH_NUM1*128)
#define DMA1_CH1_AXI_CTRL                       MAC1_BASE_ADDR + (0x3164 + CH_NUM1*64)
//dma chann1l 2
#define DMA1_CH2_CTRL_REG                       MAC1_BASE_ADDR + (0x3100 + CH_NUM2*128)
#define DMA1_CH2_TX_CTRL_REG                    MAC1_BASE_ADDR + (0x3104 + CH_NUM2*128)
#define DMA1_CH2_RX_CTRL_REG                    MAC1_BASE_ADDR + (0x3108 + CH_NUM2*128)
#define DMA1_CH2_SLOT_FUNC                      MAC1_BASE_ADDR + (0x310C + CH_NUM2*128)
#define DMA1_CH2_TX_DESC_HLST                   MAC1_BASE_ADDR + (0x3110 + CH_NUM2*128)
#define DMA1_CH2_TX_DESC_LLST                   MAC1_BASE_ADDR + (0x3114 + CH_NUM2*128)
#define DMA1_CH2_RX_DESC_HLST                   MAC1_BASE_ADDR + (0x3118 + CH_NUM2*128)
#define DMA1_CH2_RX_DESC_LLST                   MAC1_BASE_ADDR + (0x311C + CH_NUM2*128)
#define DMA1_CH2_TX_DESC_TPH                    MAC1_BASE_ADDR + (0x3120 + CH_NUM2*128)
#define DMA1_CH2_TX_DESC_TPL                    MAC1_BASE_ADDR + (0x3124 + CH_NUM2*128)
#define DMA1_CH2_RX_DESC_TPH                    MAC1_BASE_ADDR + (0x3128 + CH_NUM2*128)
#define DMA1_CH2_RX_DESC_TPL                    MAC1_BASE_ADDR + (0x312C + CH_NUM2*128)
#define DMA1_CH2_TX_RING_LEN                    MAC1_BASE_ADDR + (0x3130 + CH_NUM2*128)
#define DMA1_CH2_RX_RING_LEN                    MAC1_BASE_ADDR + (0x3134 + CH_NUM2*128)
#define DMA1_CH2_INTT_EN                        MAC1_BASE_ADDR + (0x3138 + CH_NUM2*128)
#define DMA1_CH2_INTT_WDOG                      MAC1_BASE_ADDR + (0x313C + CH_NUM2*128)
#define DMA1_CH2_CUR_TX_DESCH                   MAC1_BASE_ADDR + (0x3140 + CH_NUM2*128)
#define DMA1_CH2_CUR_TX_DESCL                   MAC1_BASE_ADDR + (0x3144 + CH_NUM2*128)
#define DMA1_CH2_CUR_RX_DESCH                   MAC1_BASE_ADDR + (0x3148 + CH_NUM2*128)
#define DMA1_CH2_CUR_RX_DESCL                   MAC1_BASE_ADDR + (0x314C + CH_NUM2*128)
#define DMA1_CH2_CUR_TX_BUFH                    MAC1_BASE_ADDR + (0x3150 + CH_NUM2*128)
#define DMA1_CH2_CUR_TX_BUFL                    MAC1_BASE_ADDR + (0x3154 + CH_NUM2*128)
#define DMA1_CH2_CUR_RX_BUFH                    MAC1_BASE_ADDR + (0x3158 + CH_NUM2*128)
#define DMA1_CH2_CUR_RX_BUFL                    MAC1_BASE_ADDR + (0x315C + CH_NUM2*128)
#define DMA1_CH2_STS                            MAC1_BASE_ADDR + (0x3160 + CH_NUM2*128)
#define DMA1_CH2_AXI_CTRL                       MAC1_BASE_ADDR + (0x3164 + CH_NUM2*64)
//dma chann1l 3
#define DMA1_CH3_CTRL_REG                       MAC1_BASE_ADDR + (0x3100 + CH_NUM3*128)
#define DMA1_CH3_TX_CTRL_REG                    MAC1_BASE_ADDR + (0x3104 + CH_NUM3*128)
#define DMA1_CH3_RX_CTRL_REG                    MAC1_BASE_ADDR + (0x3108 + CH_NUM3*128)
#define DMA1_CH3_SLOT_FUNC                      MAC1_BASE_ADDR + (0x310C + CH_NUM3*128)
#define DMA1_CH3_TX_DESC_HLST                   MAC1_BASE_ADDR + (0x3110 + CH_NUM3*128)
#define DMA1_CH3_TX_DESC_LLST                   MAC1_BASE_ADDR + (0x3114 + CH_NUM3*128)
#define DMA1_CH3_RX_DESC_HLST                   MAC1_BASE_ADDR + (0x3118 + CH_NUM3*128)
#define DMA1_CH3_RX_DESC_LLST                   MAC1_BASE_ADDR + (0x311C + CH_NUM3*128)
#define DMA1_CH3_TX_DESC_TPH                    MAC1_BASE_ADDR + (0x3120 + CH_NUM3*128)
#define DMA1_CH3_TX_DESC_TPL                    MAC1_BASE_ADDR + (0x3124 + CH_NUM3*128)
#define DMA1_CH3_RX_DESC_TPH                    MAC1_BASE_ADDR + (0x3128 + CH_NUM3*128)
#define DMA1_CH3_RX_DESC_TPL                    MAC1_BASE_ADDR + (0x312C + CH_NUM3*128)
#define DMA1_CH3_TX_RING_LEN                    MAC1_BASE_ADDR + (0x3130 + CH_NUM3*128)
#define DMA1_CH3_RX_RING_LEN                    MAC1_BASE_ADDR + (0x3134 + CH_NUM3*128)
#define DMA1_CH3_INTT_EN                        MAC1_BASE_ADDR + (0x3138 + CH_NUM3*128)
#define DMA1_CH3_INTT_WDOG                      MAC1_BASE_ADDR + (0x313C + CH_NUM3*128)
#define DMA1_CH3_CUR_TX_DESCH                   MAC1_BASE_ADDR + (0x3140 + CH_NUM3*128)
#define DMA1_CH3_CUR_TX_DESCL                   MAC1_BASE_ADDR + (0x3144 + CH_NUM3*128)
#define DMA1_CH3_CUR_RX_DESCH                   MAC1_BASE_ADDR + (0x3148 + CH_NUM3*128)
#define DMA1_CH3_CUR_RX_DESCL                   MAC1_BASE_ADDR + (0x314C + CH_NUM3*128)
#define DMA1_CH3_CUR_TX_BUFH                    MAC1_BASE_ADDR + (0x3150 + CH_NUM3*128)
#define DMA1_CH3_CUR_TX_BUFL                    MAC1_BASE_ADDR + (0x3154 + CH_NUM3*128)
#define DMA1_CH3_CUR_RX_BUFH                    MAC1_BASE_ADDR + (0x3158 + CH_NUM3*128)
#define DMA1_CH3_CUR_RX_BUFL                    MAC1_BASE_ADDR + (0x315C + CH_NUM3*128)
#define DMA1_CH3_STS                            MAC1_BASE_ADDR + (0x3160 + CH_NUM3*128)
#define DMA1_CH3_AXI_CTRL                       MAC1_BASE_ADDR + (0x3164 + CH_NUM3*64)


//MAC Register
//xgmii 0
#define MAC0_TX_CFG_REG                        (MAC0_BASE_ADDR +0x0000)
#define MAC0_RX_CFG_REG                        (MAC0_BASE_ADDR +0x0004)
#define MAC0_PMT_CTR_STS		       (MAC0_BASE_ADDR +0x00c0)
#define MAC0_INT_STS		       	       (MAC0_BASE_ADDR +0x00b0)				
#define MAC0_FRM_FLT                           (MAC0_BASE_ADDR +0x0008)
#define MAC0_ADDRS0_HIGH                       (MAC0_BASE_ADDR +0x0300)
#define MAC0_ADDRS0_LOW                        (MAC0_BASE_ADDR +0x0304)
#define MAC0_ADDRS1_HIGH                       (MAC0_BASE_ADDR +0x0308)
#define MAC0_ADDRS1_LOW                        (MAC0_BASE_ADDR +0x030C)
#define MAC0_ADDRS2_HIGH                       (MAC0_BASE_ADDR +0x0310)
#define MAC0_ADDRS2_LOW                        (MAC0_BASE_ADDR +0x0314)
#define MAC0_ADDRS3_HIGH                       (MAC0_BASE_ADDR +0x0318)
#define MAC0_ADDRS3_LOW                        (MAC0_BASE_ADDR +0x031C)
#define MAC0_ADDRS4_HIGH                       (MAC0_BASE_ADDR +0x0320)
#define MAC0_ADDRS4_LOW                        (MAC0_BASE_ADDR +0x0324)
#define MAC0_ADDRS5_HIGH                       (MAC0_BASE_ADDR +0x0328)
#define MAC0_ADDRS5_LOW                        (MAC0_BASE_ADDR +0x032C)
#define MAC0_ADDRS6_HIGH                       (MAC0_BASE_ADDR +0x0330)
#define MAC0_ADDRS6_LOW                        (MAC0_BASE_ADDR +0x0334)
#define MAC0_ADDRS7_HIGH                       (MAC0_BASE_ADDR +0x0338)
#define MAC0_ADDRS7_LOW                        (MAC0_BASE_ADDR +0x033C)
#define MAC0_RX_Q_CONTROL0                     (MAC0_BASE_ADDR +0x00A0)
#define MAC0_RX_Q_CONTROL1                     (MAC0_BASE_ADDR +0x00A4)
#define MAC0_RX_Q_CONTROL2                     (MAC0_BASE_ADDR +0x00A8)
#define MAC0_RX_Q_CONTROL3                     (MAC0_BASE_ADDR +0x00AC)
#define MAC0_TX_Q0_FLOW_CTRL                   (MAC0_BASE_ADDR +0x0070)
#define MAC0_TX_Q1_FLOW_CTRL                   (MAC0_BASE_ADDR +0x0074)
#define MAC0_TX_Q2_FLOW_CTRL                   (MAC0_BASE_ADDR +0x0078)
#define MAC0_TX_Q3_FLOW_CTRL                   (MAC0_BASE_ADDR +0x007C)
#define MAC0_TX_Q4_FLOW_CTRL                   (MAC0_BASE_ADDR +0x0080)
#define MAC0_TX_Q5_FLOW_CTRL                   (MAC0_BASE_ADDR +0x0084)
#define MAC0_TX_Q6_FLOW_CTRL                   (MAC0_BASE_ADDR +0x0088)
#define MAC0_TX_Q7_FLOW_CTRL                   (MAC0_BASE_ADDR +0x008C)
#define MAC0_VLAN_TAG                          (MAC0_BASE_ADDR +0x0050)
#define MAC0_VLAN_INSRT                        (MAC0_BASE_ADDR +0x0060)
#define MAC0_VLAN_INNER_INSRT                  (MAC0_BASE_ADDR +0x0064)
#define MAC0_SINGLE_MDIO_CMD_ADR               (MAC0_BASE_ADDR +0x0200)
#define MAC0_SINGLE_MDIO_CMD_CTR_DATA          (MAC0_BASE_ADDR +0x0204)
#define MAC0_CONTINUS_WR_ADDR                  (MAC0_BASE_ADDR +0x0208)
#define MAC0_CONTINUS_WR_DATA                  (MAC0_BASE_ADDR +0x0208)
#define MAC0_CONTINUS_SCAN_PORT_EN             (MAC0_BASE_ADDR +0x020C)
#define MAC0_MDIO_INT_STS                      (MAC0_BASE_ADDR +0x0210)
#define MAC0_MDIO_INT_EN                       (MAC0_BASE_ADDR +0x0214)
#define MAC0_RSS_CTRL                          (MAC0_BASE_ADDR +0x0C80)
#define MAC0_RSS_ADDR                          (MAC0_BASE_ADDR +0x0C88)
#define MAC0_RSS_DATA                          (MAC0_BASE_ADDR +0x0C8C)
#define MAC0_HLF_ADDR                          (MAC0_BASE_ADDR +0x0C00)
#define MAC0_HLF_DATA                          (MAC0_BASE_ADDR +0x0C04)
#define MAC0_TS_CTRL                           (MAC0_BASE_ADDR +0x0D00)
#define MAC0_SYS_SEC_INC                       (MAC0_BASE_ADDR +0x0D04)
#define MAC0_SYS_TIME_SEC                      (MAC0_BASE_ADDR +0x0D08)
#define MAC0_SYS_TIME_NANO                     (MAC0_BASE_ADDR +0x0D0C)
#define MAC0_SYS_TIME_SEC_UPDATE               (MAC0_BASE_ADDR +0x0D10)
#define MAC0_SYS_TIME_NANO_UPDATE              (MAC0_BASE_ADDR +0x0D14)
#define MAC0_TXTS_STS_NANO                     (MAC0_BASE_ADDR +0x0D30)
#define MAC0_TXTS_STS_SEC                      (MAC0_BASE_ADDR +0x0D34)

//xgmii 1 
#define MAC1_TX_CFG_REG                        (MAC1_BASE_ADDR +0x0000)
#define MAC1_RX_CFG_REG                        (MAC1_BASE_ADDR +0x0004)
#define MAC1_PMT_CTR_STS		       (MAC1_BASE_ADDR +0x00c0)
#define MAC1_INT_STS		       	       (MAC1_BASE_ADDR +0x00b0)	
#define MAC1_FRM_FLT                           (MAC1_BASE_ADDR +0x0008)
#define MAC1_ADDRS0_HIGH                       (MAC1_BASE_ADDR +0x0300)
#define MAC1_ADDRS0_LOW                        (MAC1_BASE_ADDR +0x0304)
#define MAC1_ADDRS1_HIGH                       (MAC1_BASE_ADDR +0x0308)
#define MAC1_ADDRS1_LOW                        (MAC1_BASE_ADDR +0x030C)
#define MAC1_ADDRS2_HIGH                       (MAC1_BASE_ADDR +0x0310)
#define MAC1_ADDRS2_LOW                        (MAC1_BASE_ADDR +0x0314)
#define MAC1_ADDRS3_HIGH                       (MAC1_BASE_ADDR +0x0318)
#define MAC1_ADDRS3_LOW                        (MAC1_BASE_ADDR +0x031C)
#define MAC1_ADDRS4_HIGH                       (MAC1_BASE_ADDR +0x0320)
#define MAC1_ADDRS4_LOW                        (MAC1_BASE_ADDR +0x0324)
#define MAC1_ADDRS5_HIGH                       (MAC1_BASE_ADDR +0x0328)
#define MAC1_ADDRS5_LOW                        (MAC1_BASE_ADDR +0x032C)
#define MAC1_ADDRS6_HIGH                       (MAC1_BASE_ADDR +0x0330)
#define MAC1_ADDRS6_LOW                        (MAC1_BASE_ADDR +0x0334)
#define MAC1_ADDRS7_HIGH                       (MAC1_BASE_ADDR +0x0338)
#define MAC1_ADDRS7_LOW                        (MAC1_BASE_ADDR +0x033C)
#define MAC1_RX_Q_CONTROL0                     (MAC1_BASE_ADDR +0x00A0)
#define MAC1_RX_Q_CONTROL1                     (MAC1_BASE_ADDR +0x00A4)
#define MAC1_RX_Q_CONTROL2                     (MAC1_BASE_ADDR +0x00A8)
#define MAC1_RX_Q_CONTROL3                     (MAC1_BASE_ADDR +0x00AC)
#define MAC1_TX_Q0_FLOW_CTRL                   (MAC1_BASE_ADDR +0x0070)
#define MAC1_TX_Q1_FLOW_CTRL                   (MAC1_BASE_ADDR +0x0074)
#define MAC1_TX_Q2_FLOW_CTRL                   (MAC1_BASE_ADDR +0x0078)
#define MAC1_TX_Q3_FLOW_CTRL                   (MAC1_BASE_ADDR +0x007C)
#define MAC1_TX_Q4_FLOW_CTRL                   (MAC1_BASE_ADDR +0x0080)
#define MAC1_TX_Q5_FLOW_CTRL                   (MAC1_BASE_ADDR +0x0084)
#define MAC1_TX_Q6_FLOW_CTRL                   (MAC1_BASE_ADDR +0x0088)
#define MAC1_TX_Q7_FLOW_CTRL                   (MAC1_BASE_ADDR +0x008C)
#define MAC1_VLAN_TAG                          (MAC1_BASE_ADDR +0x0050)
#define MAC1_VLAN_INSRT                        (MAC1_BASE_ADDR +0x0060)
#define MAC1_VLAN_INNER_INSRT                  (MAC1_BASE_ADDR +0x0064)
#define MAC1_RSS_CTRL                          (MAC1_BASE_ADDR +0x0C80)
#define MAC1_RSS_ADDR                          (MAC1_BASE_ADDR +0x0C88)
#define MAC1_RSS_DATA                          (MAC1_BASE_ADDR +0x0C8C)
#define MAC1_HLF_ADDR                          (MAC1_BASE_ADDR +0x0C00)
#define MAC1_HLF_DATA                          (MAC1_BASE_ADDR +0x0C04)
#define MAC1_TS_CTRL                           (MAC1_BASE_ADDR +0x0D00)
#define MAC1_SYS_SEC_INC                       (MAC1_BASE_ADDR +0x0D04)
#define MAC1_SYS_TIME_SEC                      (MAC1_BASE_ADDR +0x0D08)
#define MAC1_SYS_TIME_NANO                     (MAC1_BASE_ADDR +0x0D0C)
#define MAC1_SYS_TIME_SEC_UPDATE               (MAC1_BASE_ADDR +0x0D10)
#define MAC1_SYS_TIME_NANO_UPDATE              (MAC1_BASE_ADDR +0x0D14)
#define MAC1_TXTS_STS_NANO                     (MAC1_BASE_ADDR +0x0D30)
#define MAC1_TXTS_STS_SEC                      (MAC1_BASE_ADDR +0x0D34)
//MTL reg
//xgmii 0
#define MTL0_OPMODE                            (MAC0_BASE_ADDR + 0x1000)
#define MTL0_INTR_EN                           (MAC0_BASE_ADDR + 0x101C)
#define MTL0_INTR_STS                          (MAC0_BASE_ADDR + 0x1020)

#define MTL0_TX_OPMODE                         (MAC0_BASE_ADDR + 0x1100+CH_NUM0*128)  // TX OPMODE REG
#define MTL0_TX_QNTMWEGHT                      (MAC0_BASE_ADDR + 0x1118+CH_NUM0*128)  // RX OPMODE REG
#define MTL0_RX_OPMODE                         (MAC0_BASE_ADDR + 0x1140+CH_NUM0*128)  // RX OPMODE REG
#define MTL0_TX_SEND_SLOP                      (MAC0_BASE_ADDR + 0x111C+CH_NUM0*128)  // Send Slope
#define MTL0_TX_HI_CREDIT                      (MAC0_BASE_ADDR + 0x1120+CH_NUM0*128)  // Send Slope
#define MTL0_TX_LO_CREDIT                      (MAC0_BASE_ADDR + 0x1124+CH_NUM0*128)  // Send Slope

#define MTL1_TX_OPMODE                         (MAC0_BASE_ADDR + 0x1100+CH_NUM1*128)  // TX OPMODE REG
#define MTL1_TX_QNTMWEGHT                      (MAC0_BASE_ADDR + 0x1118+CH_NUM1*128)  // RX OPMODE REG
#define MTL1_RX_OPMODE                         (MAC0_BASE_ADDR + 0x1140+CH_NUM1*128)  // RX OPMODE REG
#define MTL1_TX_SEND_SLOP                      (MAC0_BASE_ADDR + 0x111C+CH_NUM1*128)  // Send Slope
#define MTL1_TX_HI_CREDIT                      (MAC0_BASE_ADDR + 0x1120+CH_NUM1*128)  // Send Slope
#define MTL1_TX_LO_CREDIT                      (MAC0_BASE_ADDR + 0x1124+CH_NUM1*128)  // Send Slope

#define MTL2_TX_OPMODE                         (MAC0_BASE_ADDR + 0x1100+CH_NUM2*128)  // TX OPMODE REG
#define MTL2_TX_QNTMWEGHT                      (MAC0_BASE_ADDR + 0x1118+CH_NUM2*128)  // RX OPMODE REG
#define MTL2_RX_OPMODE                         (MAC0_BASE_ADDR + 0x1140+CH_NUM2*128)  // RX OPMODE REG
#define MTL2_TX_SEND_SLOP                      (MAC0_BASE_ADDR + 0x111C+CH_NUM2*128)  // Send Slope
#define MTL2_TX_HI_CREDIT                      (MAC0_BASE_ADDR + 0x1120+CH_NUM2*128)  // Send Slope
#define MTL2_TX_LO_CREDIT                      (MAC0_BASE_ADDR + 0x1124+CH_NUM2*128)  // Send Slope

#define MTL3_TX_OPMODE                         (MAC0_BASE_ADDR + 0x1100+CH_NUM3*128)  // TX OPMODE REG
#define MTL3_TX_QNTMWEGHT                      (MAC0_BASE_ADDR + 0x1118+CH_NUM3*128)  // RX OPMODE REG
#define MTL3_RX_OPMODE                         (MAC0_BASE_ADDR + 0x1140+CH_NUM3*128)  // RX OPMODE REG
#define MTL3_TX_SEND_SLOP                      (MAC0_BASE_ADDR + 0x111C+CH_NUM3*128)  // Send Slope
#define MTL3_TX_HI_CREDIT                      (MAC0_BASE_ADDR + 0x1120+CH_NUM3*128)  // Send Slope
#define MTL3_TX_LO_CREDIT                      (MAC0_BASE_ADDR + 0x1124+CH_NUM3*128)  // Send Slope

#define MTL0_Q2DMA_CH_MAP0                     (MAC0_BASE_ADDR + 0x1030)
#define MTL0_Q2DMA_CH_MAP1                     (MAC0_BASE_ADDR + 0x1034)
#define MTL0_Q2DMA_CH_MAP2                     (MAC0_BASE_ADDR + 0x1038)
#define MTL0_Q2DMA_CH_MAP3                     (MAC0_BASE_ADDR + 0x103C)
#define MTL0_Q0_RX_OPMODE                      (MAC0_BASE_ADDR + 0x1140)
#define MTL0_Q_INTR_EN                         (MAC0_BASE_ADDR + 0x1170+CH_NUM0*128)
#define MTL0_Q_INTR_STS                        (MAC0_BASE_ADDR + 0x1174+CH_NUM0*128)
#define MTL1_Q_INTR_EN                         (MAC0_BASE_ADDR + 0x1170+CH_NUM1*128)
#define MTL1_Q_INTR_STS                        (MAC0_BASE_ADDR + 0x1174+CH_NUM1*128)
#define MTL2_Q_INTR_EN                         (MAC0_BASE_ADDR + 0x1170+CH_NUM2*128)
#define MTL2_Q_INTR_STS                        (MAC0_BASE_ADDR + 0x1174+CH_NUM3*128)
#define MTL3_Q_INTR_EN                         (MAC0_BASE_ADDR + 0x1170+CH_NUM3*128)
#define MTL3_Q_INTR_STS                        (MAC0_BASE_ADDR + 0x1174+CH_NUM3*128)

//xgmac
#define MAC1_MTL0_OPMODE                            (MAC1_BASE_ADDR + 0x1000)
#define MAC1_MTL0_INTR_EN                           (MAC1_BASE_ADDR + 0x101C)
#define MAC1_MTL0_INTR_STS                          (MAC1_BASE_ADDR + 0x1020)

#define MAC1_MTL0_TX_OPMODE                         (MAC1_BASE_ADDR + 0x1100+CH_NUM0*128)  // TX OPMODE REG
#define MAC1_MTL0_TX_QNTMWEGHT                      (MAC1_BASE_ADDR + 0x1118+CH_NUM0*128)  // RX OPMODE REG
#define MAC1_MTL0_RX_OPMODE                         (MAC1_BASE_ADDR + 0x1140+CH_NUM0*128)  // RX OPMODE REG
#define MAC1_MTL0_TX_SEND_SLOP                      (MAC1_BASE_ADDR + 0x111C+CH_NUM0*128)  // Send Slope
#define MAC1_MTL0_TX_HI_CREDIT                      (MAC1_BASE_ADDR + 0x1120+CH_NUM0*128)  // Send Slope
#define MAC1_MTL0_TX_LO_CREDIT                      (MAC1_BASE_ADDR + 0x1124+CH_NUM0*128)  // Send Slope

#define MAC1_MTL1_TX_OPMODE                         (MAC1_BASE_ADDR + 0x1100+CH_NUM1*128)  // TX OPMODE REG
#define MAC1_MTL1_TX_QNTMWEGHT                      (MAC1_BASE_ADDR + 0x1118+CH_NUM1*128)  // RX OPMODE REG
#define MAC1_MTL1_RX_OPMODE                         (MAC1_BASE_ADDR + 0x1140+CH_NUM1*128)  // RX OPMODE REG
#define MAC1_MTL1_TX_SEND_SLOP                      (MAC1_BASE_ADDR + 0x111C+CH_NUM1*128)  // Send Slope
#define MAC1_MTL1_TX_HI_CREDIT                      (MAC1_BASE_ADDR + 0x1120+CH_NUM1*128)  // Send Slope
#define MAC1_MTL1_TX_LO_CREDIT                      (MAC1_BASE_ADDR + 0x1124+CH_NUM1*128)  // Send Slope

#define MAC1_MTL2_TX_OPMODE                         (MAC1_BASE_ADDR + 0x1100+CH_NUM2*128)  // TX OPMODE REG
#define MAC1_MTL2_TX_QNTMWEGHT                      (MAC1_BASE_ADDR + 0x1118+CH_NUM2*128)  // RX OPMODE REG
#define MAC1_MTL2_RX_OPMODE                         (MAC1_BASE_ADDR + 0x1140+CH_NUM2*128)  // RX OPMODE REG
#define MAC1_MTL2_TX_SEND_SLOP                      (MAC1_BASE_ADDR + 0x111C+CH_NUM2*128)  // Send Slope
#define MAC1_MTL2_TX_HI_CREDIT                      (MAC1_BASE_ADDR + 0x1120+CH_NUM2*128)  // Send Slope
#define MAC1_MTL2_TX_LO_CREDIT                      (MAC1_BASE_ADDR + 0x1124+CH_NUM2*128)  // Send Slope

#define MAC1_MTL3_TX_OPMODE                         (MAC1_BASE_ADDR + 0x1100+CH_NUM3*128)  // TX OPMODE REG
#define MAC1_MTL3_TX_QNTMWEGHT                      (MAC1_BASE_ADDR + 0x1118+CH_NUM3*128)  // RX OPMODE REG
#define MAC1_MTL3_RX_OPMODE                         (MAC1_BASE_ADDR + 0x1140+CH_NUM3*128)  // RX OPMODE REG
#define MAC1_MTL3_TX_SEND_SLOP                      (MAC1_BASE_ADDR + 0x111C+CH_NUM3*128)  // Send Slope
#define MAC1_MTL3_TX_HI_CREDIT                      (MAC1_BASE_ADDR + 0x1120+CH_NUM3*128)  // Send Slope
#define MAC1_MTL3_TX_LO_CREDIT                      (MAC1_BASE_ADDR + 0x1124+CH_NUM3*128)  // Send Slope

#define MAC1_MTL0_Q2DMA_CH_MAP0                     (MAC1_BASE_ADDR + 0x1030)
#define MAC1_MTL0_Q2DMA_CH_MAP1                     (MAC1_BASE_ADDR + 0x1034)
#define MAC1_MTL0_Q2DMA_CH_MAP2                     (MAC1_BASE_ADDR + 0x1038)
#define MAC1_MTL0_Q2DMA_CH_MAP3                     (MAC1_BASE_ADDR + 0x103C)
#define MAC1_MTL0_Q0_RX_OPMODE                      (MAC1_BASE_ADDR + 0x1140)
#define MAC1_MTL0_Q_INTR_EN                         (MAC1_BASE_ADDR + 0x1170+CH_NUM0*128)
#define MAC1_MTL0_Q_INTR_STS                        (MAC1_BASE_ADDR + 0x1174+CH_NUM0*128)
#define MAC1_MTL1_Q_INTR_EN                         (MAC1_BASE_ADDR + 0x1170+CH_NUM1*128)
#define MAC1_MTL1_Q_INTR_STS                        (MAC1_BASE_ADDR + 0x1174+CH_NUM1*128)
#define MAC1_MTL2_Q_INTR_EN                         (MAC1_BASE_ADDR + 0x1170+CH_NUM2*128)
#define MAC1_MTL2_Q_INTR_STS                        (MAC1_BASE_ADDR + 0x1174+CH_NUM3*128)
#define MAC1_MTL3_Q_INTR_EN                         (MAC1_BASE_ADDR + 0x1170+CH_NUM3*128)
#define MAC1_MTL3_Q_INTR_STS                        (MAC1_BASE_ADDR + 0x1174+CH_NUM3*128)
//xpcs
#define XPCS_STD_REG           0x00
#define XPCS_VENDOR_REG        0x80
  // Lane Address for CR Port
#define  ADDR_LANE0                   0x20
#define  ADDR_LANE1                   0x21
#define  ADDR_LANE2                   0x22
#define  ADDR_LANE3                   0x23

  // Loopback Register base Address
#define  ADDR_LB_BASE0                0x30
#define  ADDR_LB_BASE1                0x36

  // Loopback State 
#define  LB_IDLE                     0x0 
#define  LB_EN                       0x1 
#define  LB_LOOP                     0x2 
#define  LB_DIS                      0x3 

#define  PSEQ_GOOD_STATE             0x4

#define  ERR_IDLE                    0x0
#define  ERR_CNT                     0x1

  // Base Address Definitions
#define  BASE_SR                      0x00
#define  BASE_VR_DIG_CTRL             0x00
#define  BASE_VR_DIG_STS              0x10
#define  BASE_VR_ANG_CMN_STS          0x20
#define  BASE_VR_ANG_TX_CTRL          0x30
#define  BASE_VR_ANG_TX_STS           0x40
#define  BASE_VR_ANG_RX_CTRL          0x50
#define  BASE_VR_ANG_RX_STS           0x60
#define  BASE_VR_ANG_MPLL_CTRL        0x70
#define  BASE_VR_ANG_MPLL_STS         0x80
#define  BASE_VR_ANG_MISC_CTRL        0x90
#define  BASE_VR_CR_PORT              0xA0
#define  BASE_VR_B1K_DIG_CTRL         0xE0

  // Standard Registers Address Definitions
#define  CNTRL1                      BASE_SR + 0
#define  STATUS1                     BASE_SR + 1
#define  DEV_ID1                     BASE_SR + 2
#define  DEV_ID2                     BASE_SR + 3
#define  SPD_EBL                     BASE_SR + 4
#define  DEV_PKG1                    BASE_SR + 5
#define  DEV_PKG2                    BASE_SR + 6
#define  PCS_CNTRL2                  BASE_SR + 7
#define  STATUS2                     BASE_SR + 8
#define  TX_DIS                      BASE_SR + 9
#define  RCV_SIG_DET                 BASE_SR + 10
#define  PMA_EXT_ABL                 BASE_SR + 11
#define  TX_DLY_REG                  BASE_SR + 12
#define  RX_DLY_REG                  BASE_SR + 13
#define  PKG_ID1                     BASE_SR + 14
#define  PKG_ID2                     BASE_SR + 15
#define  PCS_WKERRCNT                BASE_SR + 22
#define  LANE_STS                    BASE_SR + 24
#define  TEST_CTRL                   BASE_SR + 25
#define  RPCS_STATUS1                BASE_SR + 32
#define  RPCS_STATUS2                BASE_SR + 33
#define  RPCS_SEEDA0                 BASE_SR + 34
#define  RPCS_SEEDA1                 BASE_SR + 35
#define  RPCS_SEEDA2                 BASE_SR + 36
#define  RPCS_SEEDA3                 BASE_SR + 37
#define  RPCS_SEEDB0                 BASE_SR + 38
#define  RPCS_SEEDB1                 BASE_SR + 39
#define  RPCS_SEEDB2                 BASE_SR + 40
#define  RPCS_SEEDB3                 BASE_SR + 41
#define  RPCS_TPC                    BASE_SR + 42
#define  RPCS_TPMC                   BASE_SR + 43

#define  KRTR_CTRL                   BASE_SR + 150
#define  KRTR_STATUS                 BASE_SR + 151
#define  KRTR_LPCU                   BASE_SR + 152
#define  KRTR_LPSTS                  BASE_SR + 153
#define  KRTR_LDCU                   BASE_SR + 154
#define  KRTR_LDSTS                  BASE_SR + 155

#define  KX_CTRL                     BASE_SR + 160
#define  KX_STATUS                   BASE_SR + 161
#define  FEC_ABL                     BASE_SR + 170
#define  FEC_CTRL                    BASE_SR + 171
#define  FEC_CERR_CNTR0              BASE_SR + 172
#define  FEC_CERR_CNTR1              BASE_SR + 173
#define  FEC_UCERR_CNTR0             BASE_SR + 174
#define  FEC_UCERR_CNTR1             BASE_SR + 175

#define  AN_CNTRL                    BASE_SR + 0
#define  AN_STATUS                   BASE_SR + 1
#define  AN_DEV_ID1                  BASE_SR + 2
#define  AN_DEV_ID2                  BASE_SR + 3
#define  AN_DEV_PKG1                 BASE_SR + 5
#define  AN_DEV_PKG2                 BASE_SR + 6
#define  AN_PKG_ID1                  BASE_SR + 14
#define  AN_PKG_ID2                  BASE_SR + 15
#define  AN_ADV1                     BASE_SR + 16
#define  AN_ADV2                     BASE_SR + 17
#define  AN_ADV3                     BASE_SR + 18
#define  AN_LP_BASE_ABL1             BASE_SR + 19
#define  AN_LP_BASE_ABL2             BASE_SR + 20
#define  AN_LP_BASE_ABL3             BASE_SR + 21
#define  AN_XNP_TX1                  BASE_SR + 22
#define  AN_XNP_TX2                  BASE_SR + 23
#define  AN_XNP_TX3                  BASE_SR + 24
#define  AN_LP_XNP_ABL1              BASE_SR + 25
#define  AN_LP_XNP_ABL2              BASE_SR + 26
#define  AN_LP_XNP_ABL3              BASE_SR + 27
#define  AN_BP_ETH_STATUS            BASE_SR + 48
#define  AN_EEE_CAPA                 BASE_SR + 60
#define  AN_EEE_LP_CAPA              BASE_SR + 61
#define  AN_EEE_CAP2                 BASE_SR + 62
#define  AN_EEE_LP_CAP2              BASE_SR + 63

#define  AN_INT_MSK                  BASE_VR_DIG_CTRL + 1
#define  AN_INT                      BASE_VR_DIG_CTRL + 2
#define  AN_CTRL                     BASE_VR_DIG_CTRL + 3

#define  VS_PMA_DEV_ID1              BASE_SR + 0
#define  VS_PMA_DEV_ID2              BASE_SR + 1
#define  VS_MMD1_DEV_ID1             BASE_SR + 2
#define  VS_MMD1_DEV_ID2             BASE_SR + 3
#define  VS_PCS_DEV_ID1              BASE_SR + 4
#define  VS_PCS_DEV_ID2              BASE_SR + 5
#define  VS_AN_DEV_ID1               BASE_SR + 6
#define  VS_AN_DEV_ID2               BASE_SR + 7
#define  VS_MMD1_STS                 BASE_SR + 8
#define  VS_MMD1_CTRL                BASE_SR + 9
#define  VS_MMD1_PKG_ID1             BASE_SR + 14
#define  VS_MMD1_PKG_ID2             BASE_SR + 15


  // Vendor Specific Registers Address Definitions
#define  DIG_CTRL1                   BASE_VR_DIG_CTRL + 0
#define  DIG_CTRL2                   BASE_VR_DIG_CTRL + 1
#define  ERR_SEL                     BASE_VR_DIG_CTRL + 2
#define  VR_TEST_CTL                 BASE_VR_DIG_CTRL + 3
#define  XAUI_MODE_CTRL              BASE_VR_DIG_CTRL + 4
#define  DEBUG_CTRL                  BASE_VR_DIG_CTRL + 5
#define  EEE_MODE_CTRL               BASE_VR_DIG_CTRL + 6
#define  VR_RPCS_TPC                 BASE_VR_DIG_CTRL + 7
#define  EEE_TX_TIMER                BASE_VR_DIG_CTRL + 8
#define  EEE_RX_TIMER                BASE_VR_DIG_CTRL + 9
#define  EEE_MODE_CTRL1              BASE_VR_DIG_CTRL + 11
#define  KRTR_TX_EQ_CTRL             BASE_VR_DIG_CTRL + 12
#define  KRTR_TX_EQ_STS              BASE_VR_DIG_CTRL + 13
#define  RX_EQ_PHY_CMD               BASE_VR_DIG_CTRL + 14
#define  RX_EQ_STS_CTRL              BASE_VR_DIG_CTRL + 15


#define  DIG_STS                     BASE_VR_DIG_STS + 0
#define  ERR_CNT1                    BASE_VR_DIG_STS + 1
#define  ERR_CNT2                    BASE_VR_DIG_STS + 2
#define  ERR_CNT3                    BASE_VR_DIG_STS + 3
#define  ERR_CNT4                    BASE_VR_DIG_STS + 4
#define  GPIO_STS                    BASE_VR_DIG_STS + 5
#define  B1K_LPI_STS                 BASE_VR_DIG_STS + 6
#define  B1K_WAK_ECNT                BASE_VR_DIG_STS + 7
#define  B1K_COMMA_DET               BASE_VR_DIG_STS + 8

#define  CMN_STS                     BASE_VR_ANG_CMN_STS + 0 

#define  BOOST_CTRL                  BASE_VR_ANG_TX_CTRL + 0
#define  ATTN_CTRL                   BASE_VR_ANG_TX_CTRL + 1
#define  TX_GENERAL_CTRL             BASE_VR_ANG_TX_CTRL + 2
#define  EDG_RATEL_CTRL              BASE_VR_ANG_TX_CTRL + 3
#define  UP5_BOOST_CTRL0             BASE_VR_ANG_TX_CTRL + 8
#define  UP5_BOOST_CTRL1             BASE_VR_ANG_TX_CTRL + 9
#define  UP5_TXLEVEL_CTRL0           BASE_VR_ANG_TX_CTRL + 10
#define  UP5_TXLEVEL_CTRL1           BASE_VR_ANG_TX_CTRL + 11
#define  UP5_TXGENERAL_CTRL0         BASE_VR_ANG_TX_CTRL + 12
#define  UP5_TXGENERAL_CTRL1         BASE_VR_ANG_TX_CTRL + 13
#define  UP5_TXGENERAL_CTRL2         BASE_VR_ANG_TX_CTRL + 14  	// Newly added for SSP PHY
#define  UP5_TXGENERAL_CTRL3         BASE_VR_ANG_TX_CTRL + 15 	// Newly added for SSP PHY
#define  UP12_TX_CTRL                BASE_VR_ANG_TX_CTRL + 4

#define  TX_DONE_STS                 BASE_VR_ANG_TX_STS + 0
#define  UP5_TX_STS                  BASE_VR_ANG_TX_STS + 8
#define  UP12_TX_STS                 BASE_VR_ANG_TX_STS + 9

#define  RX_GENERAL_CTRL             BASE_VR_ANG_RX_CTRL + 0
#define  EQULZN_CTRL                 BASE_VR_ANG_RX_CTRL + 1
#define  DPLL_MODE_CTRL              BASE_VR_ANG_RX_CTRL + 2
#define  DPLL_RST_CTRL               BASE_VR_ANG_RX_CTRL + 3
#define  LOS_CTRL                    BASE_VR_ANG_RX_CTRL + 4

#define  UP5_RX_GENERAL_CTRL0        BASE_VR_ANG_RX_CTRL + 8
#define  UP5_RX_GENERAL_CTRL1        BASE_VR_ANG_RX_CTRL + 9
#define  UP5_RX_LOS_CTRL             BASE_VR_ANG_RX_CTRL + 10
#define  EQULZN_CTRL_SSP             BASE_VR_ANG_RX_CTRL + 11
#define  UP12_RX_CTRL                BASE_VR_ANG_RX_CTRL + 12
#define  SSP_RX_LOS_CTRL0            BASE_VR_ANG_RX_CTRL + 13
#define  SSP_RX_LOS_CTRL1            BASE_VR_ANG_RX_CTRL + 14

#define  RX_STATUS                   BASE_VR_ANG_RX_STS + 0
#define  UP12_RX_STS                 BASE_VR_ANG_RX_STS + 1

#define  MPLL_CTRL0                  BASE_VR_ANG_MPLL_CTRL + 0
#define  MPLL_CTRL1                  BASE_VR_ANG_MPLL_CTRL + 1
#define  MPLL_CTRL2                  BASE_VR_ANG_MPLL_CTRL + 2
#define  UP5_MPLL_CTRL0              BASE_VR_ANG_MPLL_CTRL + 8
#define  UP5_MPLL_CTRL1              BASE_VR_ANG_MPLL_CTRL + 9
#define  UP12_PLL_CTRL               BASE_VR_ANG_MPLL_CTRL + 10

#define  MPLL_STS                    BASE_VR_ANG_MPLL_STS + 0
#define  UP5_MPLL_STS                BASE_VR_ANG_MPLL_STS + 8
#define  UP12_PLL_STS                BASE_VR_ANG_MPLL_STS + 9

#define  LEVEL_CTRL                  BASE_VR_ANG_MISC_CTRL + 0
#define  MISC_CTRL                   BASE_VR_ANG_MISC_CTRL + 1
#define  UP5_LEVEL_CTRL              BASE_VR_ANG_MISC_CTRL + 8
#define  UP5_MISC_CTRL0              BASE_VR_ANG_MISC_CTRL + 9
#define  UP5_MISC_CTRL1              BASE_VR_ANG_MISC_CTRL + 10
#define  UP5_MISC_STS                BASE_VR_ANG_MISC_CTRL + 11
#define  UP12_GEN_CTRL               BASE_VR_ANG_MISC_CTRL + 12
#define  UP12_TX_PSTATE_CTRL         BASE_VR_ANG_MISC_CTRL + 13
#define  UP12_RX_PSTATE_CTRL         BASE_VR_ANG_MISC_CTRL + 14
#define  UP12_EEE_CTRL               BASE_VR_ANG_MISC_CTRL + 2
#define  UP12_MISC_CTRL              BASE_VR_ANG_MISC_CTRL + 15
#define  UP12_MISC_STS               BASE_VR_ANG_MISC_CTRL + 7
#define  SSP_MISC_CTRL2              BASE_VR_ANG_MISC_CTRL + 3 

#define  E12_TXGENERAL_CTRL0        BASE_VR_ANG_TX_CTRL + 0 
#define  E12_TXGENERAL_CTRL1        BASE_VR_ANG_TX_CTRL + 1
#define  E12_TXGENERAL_CTRL2        BASE_VR_ANG_TX_CTRL + 2
#define  E12_TX_BOOST_CTRL          BASE_VR_ANG_TX_CTRL + 3
#define  E12_TX_RATE_CTRL           BASE_VR_ANG_TX_CTRL + 4
#define  E12_TX_PSTATE_CTRL         BASE_VR_ANG_TX_CTRL + 5 
#define  E12_TX_EQ_CTRL0            BASE_VR_ANG_TX_CTRL + 6
#define  E12_TX_EQ_CTRL1            BASE_VR_ANG_TX_CTRL + 7


#define  E12_TX_STS                 BASE_VR_ANG_TX_STS +  0 

#define  E12_RX_GENERAL_CTRL0       BASE_VR_ANG_RX_CTRL + 0
#define  E12_RX_GENERAL_CTRL1       BASE_VR_ANG_RX_CTRL + 1
#define  E12_RX_GENERAL_CTRL2       BASE_VR_ANG_RX_CTRL + 2 
#define  E12_RX_GENERAL_CTRL3       BASE_VR_ANG_RX_CTRL + 3 
#define  E12_RX_RATE_CTRL           BASE_VR_ANG_RX_CTRL + 4
#define  E12_RX_PSTATE_CTRL         BASE_VR_ANG_RX_CTRL + 5
#define  E12_RX_CDR_CTRL            BASE_VR_ANG_RX_CTRL + 6
#define  E12_RX_ATTN_CTRL           BASE_VR_ANG_RX_CTRL + 7
#define  E12_RX_EQ_CTRL0            BASE_VR_ANG_RX_CTRL + 8
#define  E12_RX_EQ_CTRL1            BASE_VR_ANG_RX_CTRL + 9
#define  E12_RX_EQ_CTRL2            BASE_VR_ANG_RX_CTRL + 10
#define  E12_RX_EQ_CTRL3            BASE_VR_ANG_RX_CTRL + 11
#define  E12_RX_EQ_CTRL4            BASE_VR_ANG_RX_CTRL + 12
#define  E12_DFE_TAP_CTRL0          BASE_VR_ANG_RX_CTRL + 14
#define  E12_DFE_TAP_CTRL1          BASE_VR_ANG_RX_CTRL + 15

#define  E12_RX_STS                 BASE_VR_ANG_RX_STS +  0

#define  E12_MPLL_CMN_CTRL          BASE_VR_ANG_MPLL_CTRL + 0
#define  E12_MPLLA_CTRL0            BASE_VR_ANG_MPLL_CTRL + 1
#define  E12_MPLLA_CTRL1            BASE_VR_ANG_MPLL_CTRL + 2
#define  E12_MPLLA_CTRL2            BASE_VR_ANG_MPLL_CTRL + 3
#define  E12_MPLLB_CTRL0            BASE_VR_ANG_MPLL_CTRL + 4
#define  E12_MPLLB_CTRL1            BASE_VR_ANG_MPLL_CTRL + 5
#define  E12_MPLLB_CTRL2            BASE_VR_ANG_MPLL_CTRL + 6
#define  E12_MPLLA_CTRL3            BASE_VR_ANG_MPLL_CTRL + 7
#define  E12_MPLLB_CTRL3            BASE_VR_ANG_MPLL_CTRL + 8

#define  E12_MISC_CTRL0             BASE_VR_ANG_MISC_CTRL + 0
#define  E12_REF_CLK_CTRL           BASE_VR_ANG_MISC_CTRL + 1
#define  E12_VCO_LD_CTRL0           BASE_VR_ANG_MISC_CTRL + 2
#define  E12_VCO_LD_CTRL1           BASE_VR_ANG_MISC_CTRL + 3
#define  E12_VCO_LD_CTRL2           BASE_VR_ANG_MISC_CTRL + 4
#define  E12_VCO_LD_CTRL3           BASE_VR_ANG_MISC_CTRL + 5
#define  E12_VCO_REF_CTRL0          BASE_VR_ANG_MISC_CTRL + 6
#define  E12_VCO_REF_CTRL1          BASE_VR_ANG_MISC_CTRL + 7
#define  E12_MISC_STS               BASE_VR_ANG_MISC_CTRL + 8
#define  E12_MISC_CTRL1             BASE_VR_ANG_MISC_CTRL + 9
#define  E12_EEE_CTRL               BASE_VR_ANG_MISC_CTRL + 10
#define  E12_SRAM_REG               BASE_VR_ANG_MISC_CTRL + 11 

#define  E12_AFE_DFE_EN_REG         BASE_VR_ANG_RX_CTRL + 13 

#define  CR_CTRL                    BASE_VR_CR_PORT + 0
#define  CR_ADDR                    BASE_VR_CR_PORT + 1
#define  CR_DATA                    BASE_VR_CR_PORT + 2

   // BASE1000X only configuration 
#define  MII_CNTRL                  0x00
#define  MII_STATUS                 0x01
#define  MII_DEV_ID1                0x02
#define  MII_DEV_ID2                0x03
#define  MII_AN_ADV                 0x04
#define  MII_AN_LP_ABL              0x05
#define  MII_AN_EXP                 0x06
#define  MII_EXT_STS                0x0F

#define  MII_AN_INT_MSK             BASE_VR_DIG_CTRL + 1
#define  MII_AN_INT                 BASE_VR_DIG_CTRL + 2
#define  MII_B1K_DIG_CTRL2          BASE_VR_B1K_DIG_CTRL + 1
#define  MII_B1K_ERR_SEL            BASE_VR_B1K_DIG_CTRL + 2
#define  MII_AN_LINK_TIME           BASE_VR_DIG_CTRL + 10

#define  DTE_DEV_ADDR               0x05
#define  PHY_DEV_ADDR               0x04
#define  DEV_ADDR_AN                0x07
#define  DEV_ADDR_PMA               0x01
#define  DEV_ADDR_PCS               0x03
#define  DEV_ADDR_VR_PCS            0x1E
#define  DEV_ADDR_VR_MII            0x1F
