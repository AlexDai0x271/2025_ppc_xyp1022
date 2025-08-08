#ifndef GMAC_H
#define GMAC_H

//#ifdef GMAC2_TEST
//        #define GMAC_REG_BASE_ADDR	        0xE0238000
//        #define GMAC_DMAC_REG_BASE_ADDR	        0xE0239000
//        #define EPHY_BASE_ADDR                  0xf5002000
//#else
//        #ifdef GMAC1_TEST
//                #define GMAC_REG_BASE_ADDR	0xE0234000
//                #define GMAC_DMAC_REG_BASE_ADDR	0xE0235000
//                #define EPHY_BASE_ADDR          0xf5001000
//        #else
//                #define GMAC_REG_BASE_ADDR	0xE0230000
//                #define GMAC_DMAC_REG_BASE_ADDR	0xE0231000
//                #define EPHY_BASE_ADDR          0xf5000000
//        #endif
//#endif
//#ifdef GMAC2_TEST
//      #define GMAC_SDB_INT		        42
//      #define GMAC_PMT_INT		        41
//      #define	GMAC_LPI_INT		        40
//#else
//        #ifdef GMAC1_TEST
//                #define GMAC_SDB_INT		38
//                #define GMAC_PMT_INT		37
//                #define	GMAC_LPI_INT		36
//        #else
//                #define GMAC_SDB_INT		35
//                #define GMAC_PMT_INT		34
//                #define	GMAC_LPI_INT		33
//        #endif
//#endif
//#define EMAC_CLK_SEL_CFG                (CPR_BASE_ADDR + EMAC_CLK_SEL_CFG_OFFSET

#define BUF_ADDR                        0xe0000000              //the sram mem to be used for tx and rx
#define DDR_ADDR                        0x00000000              //the ddr mem to be used for tx and rx
#define GMAC_REG_BASE_ADDR		0xEB700000
#define GMAC_DMAC_REG_BASE_ADDR		0xEB701000
#define GMAC_SDB_INT		        42
//GMAC DMA register
#define GMAC_DMA_BMR		(GMAC_DMAC_REG_BASE_ADDR + 0x00)  
#define GMAC_DMA_TPDR		(GMAC_DMAC_REG_BASE_ADDR + 0x04)
#define GMAC_DMA_RPDR		(GMAC_DMAC_REG_BASE_ADDR + 0x08)
#define GMAC_DMA_RDLAR		(GMAC_DMAC_REG_BASE_ADDR + 0x0c)
#define GMAC_DMA_TDLAR		(GMAC_DMAC_REG_BASE_ADDR + 0x10)
#define GMAC_DMA_SR			(GMAC_DMAC_REG_BASE_ADDR + 0x14)
#define GMAC_DMA_OMR		(GMAC_DMAC_REG_BASE_ADDR + 0x18)
#define GMAC_DMA_IER		(GMAC_DMAC_REG_BASE_ADDR + 0x1c)
#define GMAC_DMA_MFBOCR		(GMAC_DMAC_REG_BASE_ADDR + 0x20)
#define GMAC_DMA_RIWTR		(GMAC_DMAC_REG_BASE_ADDR + 0x24)
#define GMAC_DMA_AXI_BMR	(GMAC_DMAC_REG_BASE_ADDR + 0x28)
#define GMAC_DMA_AXI_STR	(GMAC_DMAC_REG_BASE_ADDR + 0x2c)
#define GMAC_DMA_CSR_HTDR	(GMAC_DMAC_REG_BASE_ADDR + 0x48)
#define GMAC_DMA_CSR_HRDR	(GMAC_DMAC_REG_BASE_ADDR + 0x4c)
#define GMAC_DMA_CSR_HTBAR	(GMAC_DMAC_REG_BASE_ADDR + 0x50)
#define GMAC_DMA_CSR_HRBAR	(GMAC_DMAC_REG_BASE_ADDR + 0x54)
#define GMAC_DMA_FW_CFGR	(GMAC_DMAC_REG_BASE_ADDR + 0x58)

//GMAC register
#define GMAC_CR			(GMAC_REG_BASE_ADDR + 0x00)  
#define GMAC_FFR 		(GMAC_REG_BASE_ADDR + 0x04)
#define GMAC_HTHR		(GMAC_REG_BASE_ADDR + 0x08) 
#define GMAC_HTLR 		(GMAC_REG_BASE_ADDR + 0x0c)
#define GMAC_AR 		(GMAC_REG_BASE_ADDR + 0x10)
#define GMAC_DR 		(GMAC_REG_BASE_ADDR + 0x14)
#define GMAC_FCR 		(GMAC_REG_BASE_ADDR + 0x18)
#define GMAC_VLAN_TAGR 		(GMAC_REG_BASE_ADDR + 0x1c)
#define GMAC_VR			(GMAC_REG_BASE_ADDR + 0x20) 
#define GMAC_DBGR		(GMAC_REG_BASE_ADDR + 0x24)
#define GMAC_RWKFFR 		(GMAC_REG_BASE_ADDR + 0x28)
#define GMAC_PMT_CSR 		(GMAC_REG_BASE_ADDR + 0x2c)
#define GMAC_LPI_CSR 		(GMAC_REG_BASE_ADDR + 0x30)
#define GMAC_LPI_TCR		(GMAC_REG_BASE_ADDR + 0x34)
#define GMAC_ISR 		(GMAC_REG_BASE_ADDR + 0x38)
#define GMAC_IMR 		(GMAC_REG_BASE_ADDR + 0x3c)
#define GMAC_MAC_ADDR00_HR	(GMAC_REG_BASE_ADDR + 0x40)
#define GMAC_MAC_ADDR00_LR 	(GMAC_REG_BASE_ADDR + 0x44)
#define GMAC_MAC_ADDR01_HR	(GMAC_REG_BASE_ADDR + 0x48)
#define GMAC_MAC_ADDR01_LR 	(GMAC_REG_BASE_ADDR + 0x4c)
#define GMAC_MAC_ADDR02_HR	(GMAC_REG_BASE_ADDR + 0x50)
#define GMAC_MAC_ADDR02_LR 	(GMAC_REG_BASE_ADDR + 0x54)
#define GMAC_MAC_ADDR03_HR	(GMAC_REG_BASE_ADDR + 0x58)
#define GMAC_MAC_ADDR03_LR 	(GMAC_REG_BASE_ADDR + 0x5c)
#define GMAC_MAC_ADDR04_HR	(GMAC_REG_BASE_ADDR + 0x60)
#define GMAC_MAC_ADDR04_LR 	(GMAC_REG_BASE_ADDR + 0x64)
#define GMAC_MAC_ADDR05_HR	(GMAC_REG_BASE_ADDR + 0x68)
#define GMAC_MAC_ADDR05_LR 	(GMAC_REG_BASE_ADDR + 0x6c)
#define GMAC_MAC_ADDR06_HR	(GMAC_REG_BASE_ADDR + 0x70)
#define GMAC_MAC_ADDR06_LR 	(GMAC_REG_BASE_ADDR + 0x74)
#define GMAC_MAC_ADDR07_HR	(GMAC_REG_BASE_ADDR + 0x78)
#define GMAC_MAC_ADDR07_LR 	(GMAC_REG_BASE_ADDR + 0x7c)
#define GMAC_MAC_ADDR08_HR	(GMAC_REG_BASE_ADDR + 0x80)
#define GMAC_MAC_ADDR08_LR 	(GMAC_REG_BASE_ADDR + 0x84)
#define GMAC_MAC_ADDR09_HR	(GMAC_REG_BASE_ADDR + 0x88)
#define GMAC_MAC_ADDR09_LR 	(GMAC_REG_BASE_ADDR + 0x8c)
#define GMAC_MAC_ADDR10_HR	(GMAC_REG_BASE_ADDR + 0x90)
#define GMAC_MAC_ADDR10_LR 	(GMAC_REG_BASE_ADDR + 0x94)
#define GMAC_MAC_ADDR11_HR	(GMAC_REG_BASE_ADDR + 0x98)
#define GMAC_MAC_ADDR11_LR 	(GMAC_REG_BASE_ADDR + 0x9c)
#define GMAC_MAC_ADDR12_HR	(GMAC_REG_BASE_ADDR + 0xa0)
#define GMAC_MAC_ADDR12_LR 	(GMAC_REG_BASE_ADDR + 0xa4)
#define GMAC_MAC_ADDR13_HR	(GMAC_REG_BASE_ADDR + 0xa8)
#define GMAC_MAC_ADDR13_LR 	(GMAC_REG_BASE_ADDR + 0xac)
#define GMAC_MAC_ADDR14_HR	(GMAC_REG_BASE_ADDR + 0xb0)
#define GMAC_MAC_ADDR14_LR 	(GMAC_REG_BASE_ADDR + 0xb4)
#define GMAC_MAC_ADDR15_HR	(GMAC_REG_BASE_ADDR + 0xb8)
#define GMAC_MAC_ADDR15_LR 	(GMAC_REG_BASE_ADDR + 0xbc)
#define GMAC_PHY_STSR		(GMAC_REG_BASE_ADDR + 0xd8)

//GMAC external register
#define GMAC_MAC_ADDR16_HR	(GMAC_REG_BASE_ADDR + 0x800)
#define GMAC_MAC_ADDR16_LR 	(GMAC_REG_BASE_ADDR + 0x804)
#define GMAC_MAC_ADDR17_HR	(GMAC_REG_BASE_ADDR + 0x808)
#define GMAC_MAC_ADDR17_LR 	(GMAC_REG_BASE_ADDR + 0x80c)
#define GMAC_MAC_ADDR18_HR	(GMAC_REG_BASE_ADDR + 0x810)
#define GMAC_MAC_ADDR18_LR 	(GMAC_REG_BASE_ADDR + 0x814)
#define GMAC_MAC_ADDR19_HR	(GMAC_REG_BASE_ADDR + 0x818)
#define GMAC_MAC_ADDR19_LR 	(GMAC_REG_BASE_ADDR + 0x81c)
#define GMAC_MAC_ADDR20_HR	(GMAC_REG_BASE_ADDR + 0x820)
#define GMAC_MAC_ADDR20_LR 	(GMAC_REG_BASE_ADDR + 0x824)
#define GMAC_MAC_ADDR21_HR	(GMAC_REG_BASE_ADDR + 0x828)
#define GMAC_MAC_ADDR21_LR 	(GMAC_REG_BASE_ADDR + 0x82c)
#define GMAC_MAC_ADDR22_HR	(GMAC_REG_BASE_ADDR + 0x830)
#define GMAC_MAC_ADDR22_LR 	(GMAC_REG_BASE_ADDR + 0x834)
#define GMAC_MAC_ADDR23_HR	(GMAC_REG_BASE_ADDR + 0x838)
#define GMAC_MAC_ADDR23_LR 	(GMAC_REG_BASE_ADDR + 0x83c)
#define GMAC_MAC_ADDR24_HR	(GMAC_REG_BASE_ADDR + 0x840)
#define GMAC_MAC_ADDR24_LR 	(GMAC_REG_BASE_ADDR + 0x844)
#define GMAC_MAC_ADDR25_HR	(GMAC_REG_BASE_ADDR + 0x848)
#define GMAC_MAC_ADDR25_LR 	(GMAC_REG_BASE_ADDR + 0x84c)
#define GMAC_MAC_ADDR26_HR	(GMAC_REG_BASE_ADDR + 0x850)
#define GMAC_MAC_ADDR26_LR 	(GMAC_REG_BASE_ADDR + 0x854)
#define GMAC_MAC_ADDR27_HR	(GMAC_REG_BASE_ADDR + 0x858)
#define GMAC_MAC_ADDR27_LR 	(GMAC_REG_BASE_ADDR + 0x85c)
#define GMAC_MAC_ADDR28_HR	(GMAC_REG_BASE_ADDR + 0x860)
#define GMAC_MAC_ADDR28_LR 	(GMAC_REG_BASE_ADDR + 0x864)
#define GMAC_MAC_ADDR29_HR	(GMAC_REG_BASE_ADDR + 0x868)
#define GMAC_MAC_ADDR29_LR 	(GMAC_REG_BASE_ADDR + 0x86c)
#define GMAC_MAC_ADDR30_HR	(GMAC_REG_BASE_ADDR + 0x870)
#define GMAC_MAC_ADDR30_LR 	(GMAC_REG_BASE_ADDR + 0x874)
#define GMAC_MAC_ADDR31_HR	(GMAC_REG_BASE_ADDR + 0x878)
#define GMAC_MAC_ADDR31_LR 	(GMAC_REG_BASE_ADDR + 0x87c)

//
#define GMAC_IEEE1588_REG0	(GMAC_REG_BASE_ADDR + 0x700)
#define GMAC_IEEE1588_REG1	(GMAC_REG_BASE_ADDR + 0x704)
#define GMAC_IEEE1588_REG2	(GMAC_REG_BASE_ADDR + 0x708)
#define GMAC_IEEE1588_REG3	(GMAC_REG_BASE_ADDR + 0x70c)
#define GMAC_IEEE1588_REG4	(GMAC_REG_BASE_ADDR + 0x710)
#define GMAC_IEEE1588_REG5	(GMAC_REG_BASE_ADDR + 0x714)
#define GMAC_IEEE1588_REG6	(GMAC_REG_BASE_ADDR + 0x718)
#define GMAC_IEEE1588_REG7	(GMAC_REG_BASE_ADDR + 0x71c)
#define GMAC_IEEE1588_REG8	(GMAC_REG_BASE_ADDR + 0x720)
#define GMAC_IEEE1588_REG9	(GMAC_REG_BASE_ADDR + 0x724)
#define GMAC_IEEE1588_REG10	(GMAC_REG_BASE_ADDR + 0x728)
#define GMAC_IEEE1588_REG11	(GMAC_REG_BASE_ADDR + 0x72c)
#define GMAC_IEEE1588_REG12	(GMAC_REG_BASE_ADDR + 0x730)
#define GMAC_IEEE1588_REG13	(GMAC_REG_BASE_ADDR + 0x734)
#define GMAC_IEEE1588_REG14	(GMAC_REG_BASE_ADDR + 0x738)
//                                                        
#define	MMC_CNTRL		(GMAC_REG_BASE_ADDR + 0x100)
#define	MMC_INTR_RX		(GMAC_REG_BASE_ADDR + 0x104)
#define	MMC_INTR_TX		(GMAC_REG_BASE_ADDR + 0x108)
#define	MMC_INTR_MASK_RX	(GMAC_REG_BASE_ADDR + 0x10c)
#define	MMC_INTR_MASK_TX	(GMAC_REG_BASE_ADDR + 0x110)
#define MMC_TXBCASTG_CNT_R	(GMAC_REG_BASE_ADDR + 0x11c)
#define MMC_TXMCASTG_CNT_R	(GMAC_REG_BASE_ADDR + 0x120)
#define MMC_TX64_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x124)
#define MMC_TX_65_127_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x128)
#define MMC_TX_128_255_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x12c)
#define MMC_TX_256_511_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x130)
#define MMC_TX_512_1023_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x134)
#define MMC_TX_1024_MORE_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x138)

#define MMC_TXUNRFLW_CNT_R	(GMAC_REG_BASE_ADDR + 0x148)
#define MMC_TXSNGLCOLG_CNT_R	(GMAC_REG_BASE_ADDR + 0x14C)
#define MMC_TXMULTCOLG_CNT_R	(GMAC_REG_BASE_ADDR + 0x150)
#define MMC_TXDEFRD_CNT_R	(GMAC_REG_BASE_ADDR + 0x154)
#define MMC_TXLATECOL_CNT_R	(GMAC_REG_BASE_ADDR + 0x158)
#define MMC_TXEXSCOL_CNT_R	(GMAC_REG_BASE_ADDR + 0x15C)
#define MMC_TXPAUSE_CNT_R	(GMAC_REG_BASE_ADDR + 0x170)
#define MMC_RXBCASTG_CNT_R	(GMAC_REG_BASE_ADDR + 0x18C) 
#define MMC_RXMCASTG_CNT_R	(GMAC_REG_BASE_ADDR + 0x190)
#define MMC_RX64_OCTETS_GB_R		(GMAC_REG_BASE_ADDR + 0x1ac)
#define MMC_RX_65_127_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x1b0)
#define MMC_RX_128_255_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x1b4)
#define MMC_RX_256_511_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x1b8)
#define MMC_RX_512_1023_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x1bc)
#define MMC_RX_1024_MORE_OCTETS_GB_R	(GMAC_REG_BASE_ADDR + 0x1c0)
#define MMC_RXRUNTERR_CNT_R 	(GMAC_REG_BASE_ADDR + 0x19C)
#define MMC_RXJABERR_CNT_R 	(GMAC_REG_BASE_ADDR + 0x1a0)
#define MMC_RXUNDERSZG_CNT_R 	(GMAC_REG_BASE_ADDR + 0x1a4)
#define MMC_RXOVERSZG_CNT_R 	(GMAC_REG_BASE_ADDR + 0x1A8)
#define MMC_RXUCASTG_CNT_R 	(GMAC_REG_BASE_ADDR + 0x1c4)
#define MMC_RXPAUSEFRM_CNT_R 	(GMAC_REG_BASE_ADDR + 0x1d0)

//DA/SA value
#define DA_1st			0x09080706	//Group addr
#define DA_2nd			0x00000b0a	//
//#define DA_1st			0x03020100	//Group addr
//#define DA_2nd			0x00000504	//
//#define DA_1st			0xffffffff	//908
//#define DA_2nd			0x0000ffff	//908
#define SA_1st			0x01000000	//
#define SA_2nd			0x05040302	//
//tx descriptor
#define intr_on_cmpl		0x1<<(32-1)
#define last_desc		0x1<<(32-2)
#define first_desc		0x1<<(32-3)
#define tx_dis_crc		0x1<<(32-6)
#define tx_end_of_ring		0x1<<(32-7)
#define tx_sec_addr_chained	0x1<<(32-8)
#define tx_dis_padding	0x1<<(32-9)
#define insert_ipv4_header	0x1<<(32-5)
#define tx_own		0x1<<(32-1)
//for big endian
//#define intr_on_cmpl		0x1<<(32-1)
//#define last_desc		0x1<<(32-2)
//#define first_desc		0x1<<(32-3)
//#define tx_dis_crc		0x1<<(32-6)
//#define tx_end_of_ring	        0x1<<(32-7)
//#define tx_sec_addr_chained	0x1<<(32-8)
//#define tx_dis_padding	        0x1<<(32-9)
//#define insert_ipv4_header	0x1<<(32-5)
//#define tx_own		        0x1<<(0)

//rx descriptor
#define rx_end_of_ring		0x1<<(32-7)
#define rx_sec_addr_chained	0x1<<(32-8)
#define	rx_own		0x1<<(32-1)
//for big endian
//#define rx_end_of_ring	        0x1<<(32-7)
//#define rx_sec_addr_chained	0x1<<(32-8)
//#define rx_own		        0x1<<(0)
//
#define StoreAndForward		0x1<<(32-7)
#define TSF                     0x1<<(32-11)
#define	TxThreshold_64		0x0<<(32-18) 
#define	TxThreshold_128		0x1<<(32-18) 
#define	TxThreshold_192		0x2<<(32-18) 
#define	TxThreshold_256		0x3<<(32-18) 
#define	TxThreshold_40		0x4<<(32-18) 
#define	TxThreshold_32		0x5<<(32-18) 
#define	TxThreshold_24		0x6<<(32-18) 
#define	TxThreshold_16		0x7<<(32-18) 
#define	StartStopTx		0x1<<(32-19)
#define	RxThreshold_64		0x0<<(32-29)
#define	RxThreshold_32		0x1<<(32-29)
#define	RxThreshold_96		0x2<<(32-29)
#define	RxThreshold_128		0x3<<(32-29)
#define	Osf			0x1<<(32-30)
#define	StartStopRx		0x1<<(32-31)
//GMAC
//mac configure register
#define	watchdogdis		0x1<<(32-9) 
#define	jabberdis		0x1<<(32-10)
#define	frameburstena 		0x1<<(32-11)
#define	jumboena 		0x1<<(32-12)
#define	ifg_96bit		0x0<<(32-15)
#define	ifg_88bit		0x1<<(32-15)
#define	ifg_80bit		0x2<<(32-15)
#define	ifg_72bit		0x3<<(32-15)
#define	ifg_64bit		0x4<<(32-15)
#define	ifg_56bit		0x5<<(32-15)
#define	ifg_48bit		0x6<<(32-15)
#define	ifg_40bit		0x7<<(32-15)
#define	portselect_GMII		0x0<<(32-17)
#define	portselect_MII		0x1<<(32-17)
#define	speed_select_10M	0x2<<(32-18)
#define	speed_select_100M	0x3<<(32-18)
#define	speed_select_1000M	0x0<<(32-18)
#define	disreceiveown 		0x1<<(32-19)
#define	loopbackmode 		0x1<<(32-20)
#define	duplexmode		0x1<<(32-21)
#define	ipc_en 			0x1<<(32-22)
#define	disableretry		0x1<<(32-23)
#define lud             0x1<<(32-24)
#define	autocrcstrip		0x1<<(32-25)
#define	backofflimit_00 	0x0<<(32-27)
#define	backofflimit_01 	0x1<<(32-27)
#define	backofflimit_10 	0x2<<(32-27)
#define	backofflimit_11 	0x3<<(32-27)
#define	deferralcheck 		0x1<<(32-28)
#define	transmitena 		0x1<<(32-29)
#define	receiveena 		0x1<<(32-30)

#define	WKUP_PKT_EN		0x1<<(32-30)
#define	MAGIC_PKT_EN		0x1<<(32-31)
//Filter register
#define	RECV_ALL		0x1<<(32-1)
#define HPF			0x1<<(32-22)
#define	SAF			0x1<<(32-23)
#define	SAIF			0x1<<(32-24)
//#define	PCF			
#define	DBF			0x1<<(32-27)
#define	PM			0x1<<(32-28)
#define	DAIF			0x1<<(32-29)
#define	HMC			0x1<<(32-30)
#define	HUC			0x1<<(32-31)
#define	PR			0x1<<(32-32)

//dma config register
//BMR
#define	USE_SEP_PBL		0x1<<(32-9)
#define	PBLX8_MODE		0x1<<(32-8)
#define PBL_1			0x1<<(32-24)
#define PBL_2			0x2<<(32-24)
#define PBL_4			0x4<<(32-24)
#define PBL_8			0x8<<(32-24)
#define PBL_16			0x10<<(32-24)
#define PBL_32			0x20<<(32-24)
#define RX_PBL_1		0x1<<(32-15)
#define RX_PBL_2		0x2<<(32-15)
#define RX_PBL_4		0x4<<(32-15)
#define RX_PBL_8		0x8<<(32-15)
#define RX_PBL_16		0x10<<(32-15)
#define RX_PBL_32		0x20<<(32-15)
//To support VLAN packet
#define TPID			0x8100
#define TAG_CTL			0x0000
#define PRIORITY0 		(TAG_CTL|0x0000)
#define PRIORITY1 		(TAG_CTL|0x2000)
#define PRIORITY2 		(TAG_CTL|0x4000)
#define PRIORITY3 		(TAG_CTL|0x6000)
#define PRIORITY4 		(TAG_CTL|0x8000)
#define PRIORITY5 		(TAG_CTL|0xa000)
#define PRIORITY6 		(TAG_CTL|0xc000)
#define PRIORITY7 		(TAG_CTL|0xe000)
#define VLAN_ID			0x2233
//
#define	GTX_ENABLE		0x1<<(32-32)
#define GTX_DISABLE		0x0<<(32-32)	
#endif 
