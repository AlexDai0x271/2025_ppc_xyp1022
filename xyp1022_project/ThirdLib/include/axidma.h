/****************************************************************************/
/*  AXIDMA Register Addresses                                         */
/****************************************************************************/

#if 1	//DMA0
#define AXIDMA_BASE					0xe9542000ull
#define AXIDMA_INT_NUM				27
#else	//DMA1
#define AXIDMA_BASE					0xe9543000ull
#define AXIDMA_INT_NUM				28
#endif
/*  add in 2017-11-29  */
#define DMA_CfgReg            (AXIDMA_BASE + 0x10)
#define DMA_ChEnReg           (AXIDMA_BASE + 0x18)

#define CH1_BASE              0x100

#define CH1_SAR_H             (AXIDMA_BASE + CH1_BASE + 0x4)
#define CH1_SAR               (AXIDMA_BASE + CH1_BASE + 0x0)
#define CH1_DAR_H             (AXIDMA_BASE + CH1_BASE + 0xc)
#define CH1_DAR               (AXIDMA_BASE + CH1_BASE + 0x8)
#define CH1_BLOCK_TS          (AXIDMA_BASE + CH1_BASE + 0x10)
#define CH1_CTL               (AXIDMA_BASE + CH1_BASE + 0x18)
#define CH1_CTL_H             (AXIDMA_BASE + CH1_BASE + 0x1c)
#define CH1_CFG               (AXIDMA_BASE + CH1_BASE + 0x20)
#define CH1_CFG_H             (AXIDMA_BASE + CH1_BASE + 0x24)
#define CH1_INTCLEARREG       (AXIDMA_BASE + CH1_BASE + 0x98)
#define CH1_INTSIGNAL_EN_REG  (AXIDMA_BASE + CH1_BASE + 0x90)

/***********************/


#define DMAC_ID_REG						(AXIDMA_BASE+0x0)
#define DMAC_COMP_VER_REG				(AXIDMA_BASE+0x8)
#define DMAC_CFG_REG					(AXIDMA_BASE+0x10)
#define DMAC_CH_EN_REG					(AXIDMA_BASE+0x18)
#define DMAC_CH_EN_REG_H				(AXIDMA_BASE+0x1c)
#define DMAC_INT_STATUS_REG				(AXIDMA_BASE+0x30)
#define DMAC_INT_STATUS_REG_H			(AXIDMA_BASE+0x34)
#define DMAC_COMMON_INT_CLEAR_REG		(AXIDMA_BASE+0x38)
#define DMAC_COMMON_INT_CLEAR_REG_H		(AXIDMA_BASE+0x3c)
#define DMAC_COMMON_INT_STATUS_EN_REG	(AXIDMA_BASE+0x40)
#define DMAC_COMMON_INT_SIGNAL_EN_REG	(AXIDMA_BASE+0x48)
#define DMAC_COMMON_INT_STATUS_REG		(AXIDMA_BASE+0x50)
#define DMAC_RESET_REG					(AXIDMA_BASE+0x58)

#define CH_SAR(n)						(AXIDMA_BASE+n*0x100)
#define CH_SAR_H(n)						(AXIDMA_BASE+n*0x100+0x4)
#define CH_DAR(n)						(AXIDMA_BASE+n*0x100+0x8)
#define CH_DAR_H(n)						(AXIDMA_BASE+n*0x100+0xc)
#define CH_BLOCK_TS(n)					(AXIDMA_BASE+n*0x100+0x10)
#define CH_CTL(n)						(AXIDMA_BASE+n*0x100+0x18)
#define CH_CTL_H(n)						(AXIDMA_BASE+n*0x100+0x1c)
#define CH_CFG(n)						(AXIDMA_BASE+n*0x100+0x20)
#define CH_CFG_H(n)						(AXIDMA_BASE+n*0x100+0x24)
#define CH_LLP(n)						(AXIDMA_BASE+n*0x100+0x28)
#define CH_LLP_H(n)						(AXIDMA_BASE+n*0x100+0x2c)

#define CH_STATUS_REG(n)				(AXIDMA_BASE+n*0x100+0x30)
#define CH_SWHSSRC_REG(n)				(AXIDMA_BASE+n*0x100+0x38)
#define CH_SWHSDST_REG(n)				(AXIDMA_BASE+n*0x100+0x40)
#define CH_BLK_TFR_RESUMEREQ_REG(n)		(AXIDMA_BASE+n*0x100+0x48)
#define CH_AXI_ID_REG(n)				(AXIDMA_BASE+n*0x100+0x50)
#define CH_AXI_QOS_REG(n)				(AXIDMA_BASE+n*0x100+0x58)
#define CH_SSTAT(n)						(AXIDMA_BASE+n*0x100+0x60)
#define CH_DSTAT(n)						(AXIDMA_BASE+n*0x100+0x68)
#define CH_SSTATAR(n)					(AXIDMA_BASE+n*0x100+0x70)
#define CH_DSTATAR(n)					(AXIDMA_BASE+n*0x100+0x78)
#define CH_INT_STATUS_EN_REG(n)			(AXIDMA_BASE+n*0x100+0x80)
#define CH_INT_STATUS_REG(n)			(AXIDMA_BASE+n*0x100+0x88)
#define	CH_INT_SIGNAL_EN_REG(n)			(AXIDMA_BASE+n*0x100+0x90)
#define CH_INT_CLEAR_REG(n)				(AXIDMA_BASE+n*0x100+0x98)

#define INT_EN							(1<<1)
#define DMAC_EN							(1<<0)

#define CH_EN(n)						(1<<(n-1))
#define CH_EN_WE(n)						(1<<(n+7))
#define CH_SUSP(n)						(1<<(n+15))
#define CH_SUSP_WE(n)					(1<<(n+23))
#define CH_ABORT(n)						(1<<(n-1))
#define CH_ABORT_WE(n)					(1<<(n+7))
#define CH_EN_ALL						0xff
#define CH_EN_WE_ALL					0xff00

#define DMAC_RST						(1<<0)

#define SHADOWREG_LLI_VALID				(1<<31)
#define SHADOWREG_LLI_LAST				(1<<30)
#define IOC_BLKTFR						(1<<26)
#define SRC_MSIZE_MASK					0xfffc3000
#define SRC_MSIZE_1						0
#define SRC_MSIZE_4						(1<<14)
#define SRC_MSIZE_8						(2<<14)
#define SRC_MSIZE_16					(3<<14)
#define SRC_MSIZE_32					(4<<14)
#define SRC_MSIZE_64					(5<<14)
#define SRC_MSIZE_128					(6<<14)
#define SRC_MSIZE_256					(7<<14)
#define SRC_MSIZE_512					(8<<14)
#define SRC_MSIZE_1024					(9<<14)
#define DST_MSIZE_MASK					0xffc30000
#define DST_MSIZE_1						0
#define DST_MSIZE_4						(1<<18)
#define DST_MSIZE_8						(2<<18)
#define DST_MSIZE_16					(3<<18)
#define DST_MSIZE_32					(4<<18)
#define DST_MSIZE_64					(5<<18)
#define DST_MSIZE_128					(6<<18)
#define DST_MSIZE_256					(7<<18)
#define DST_MSIZE_512					(8<<18)
#define DST_MSIZE_1024					(9<<18)
#define SRC_TR_WIDTH_MASK				0xfffff8ff
#define SRC_TR_WIDTH_8					0
#define SRC_TR_WIDTH_16					(1<<8)
#define SRC_TR_WIDTH_32					(2<<8)
#define SRC_TR_WIDTH_64					(3<<8)
#define SRC_TR_WIDTH_128				(4<<8)
#define SRC_TR_WIDTH_256				(5<<8)
#define SRC_TR_WIDTH_512				(6<<8)
#define DST_TR_WIDTH_MASK				0xffffc7ff
#define DST_TR_WIDTH_8					0
#define DST_TR_WIDTH_16					(1<<11)
#define DST_TR_WIDTH_32					(2<<11)
#define DST_TR_WIDTH_64					(3<<11)
#define DST_TR_WIDTH_128				(4<<11)
#define DST_TR_WIDTH_256				(5<<11)
#define DST_TR_WIDTH_512				(6<<11)
#define DINC							(1<<6)
#define SINC							(1<<4)
#define DMS_1							(0<<2)
#define DMS_2							(1<<2)
#define SMS_2							(1<<0)

#define DST_OSR_LMT(n)					(n<<27)
#define SRC_OSR_LMT(n)					(n<<23)
#define LOCK_CH							(1<<20)
#define CH_PRIOR_MASK					0xfff1ffff
#define CH_PRIOR(n)						(n<<17)
#define HS_SEL_SRC						(1<<3)
#define HS_SEL_DST						(1<<4)
#define TT_FC_MASK						0xfffffff8
#define TT_FC_MEM2MEM					0
#define TT_FC_MEM2PER					(1<<0)
#define TT_FC_PER2MEM					(2<<0)
#define TT_FC_PER2PER					(3<<0)
#define TT_FC_PER2MEM_SRC				(4<<0)
#define TT_FC_PER2PER_SRC				(5<<0)
#define TT_FC_MEM2PER_DST				(6<<0)
#define TT_FC_PER2PER_DST				(7<<0)
#define DST_MULTBLK_TYPE_MASK			0xfffffff3
#define DST_MULTBLK_TYPE_CONTIGUOUS		0
#define DST_MULTBLK_TYPE_RELOAD			(1<<2)
#define DST_MULTBLK_TYPE_SHADOW_REG		(2<<2)
#define DST_MULTBLK_TYPE_LINKED_LIST	(3<<2)
#define SRC_MULTBLK_TYPE_MASK			0xfffffffc
#define SRC_MULTBLK_TYPE_CONTIGUOUS		0
#define SRC_MULTBLK_TYPE_RELOAD			(1<<0)
#define SRC_MULTBLK_TYPE_SHADOW_REG		(2<<0)
#define SRC_MULTBLK_TYPE_LINKED_LIST	(3<<0)

#define BLK_TFR_RESUMEREQ				(1<<0)

#define DMA_TFR_DONE_INTSTAT			(1<<1)
#define BLK_TFR_DONE_INTSTAT			(1<<0)

#define SWHS_LST_WE						(1<<5)
#define SWHS_LST						(1<<4)
#define SWHS_SGLREQ_WE					(1<<3)
#define SWHS_SGLREQ						(1<<2)
#define SWHS_REQ_WE						(1<<1)
#define SWHS_REQ						(1<<0)




