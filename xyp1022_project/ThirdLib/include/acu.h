#ifndef ACU_H
#define ACU_H

#define ACU_BASE_ADDR                            0xE9541000ull
#define ACU_JR_h20x8_ADDR 						 0xE9200310ull

//#define SECMON_BASE_ADDR                         0xE020D000

//#define SECMON_INT
#define ACU_CTRL_REG							(ACU_BASE_ADDR+0x0)
#define ACU_RING_CTRL_REG						(ACU_BASE_ADDR+0x10)

#define ACU_CTRL                                 (*(volatile int *)(ACU_BASE_ADDR+0x000))
#define ACU_DUMADR_H                             (*(volatile int *)(ACU_BASE_ADDR+0x004))
#define ACU_DUMADR_L                             (*(volatile int *)(ACU_BASE_ADDR+0x008))
#define ACU_M1_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x010))
#define ACU_M1_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x014))
#define ACU_M1_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x018))
#define ACU_M1_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x01C))
#define ACU_M1_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x020))
#define ACU_M1_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x024))
#define ACU_M1_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x028))
#define ACU_M1_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x02C))
#define ACU_M1_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x030))
#define ACU_M1_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x034))
#define ACU_M1_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x038))
#define ACU_M1_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x03C))
#define ACU_M1_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x040))
#define ACU_M1_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x044))
#define ACU_M1_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x048))
#define ACU_M1_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x04C))
#define ACU_M2_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x050))
#define ACU_M2_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x054))
#define ACU_M2_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x058))
#define ACU_M2_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x05C))
#define ACU_M2_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x060))
#define ACU_M2_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x064))
#define ACU_M2_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x068))
#define ACU_M2_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x06C))
#define ACU_M2_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x070))
#define ACU_M2_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x074))
#define ACU_M2_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x078))
#define ACU_M2_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x07C))
#define ACU_M2_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x080))
#define ACU_M2_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x084))
#define ACU_M2_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x088))
#define ACU_M2_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x08C))
#define ACU_M3_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x090))
#define ACU_M3_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x094))
#define ACU_M3_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x098))
#define ACU_M3_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x09C))
#define ACU_M3_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0A0))
#define ACU_M3_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0A4))
#define ACU_M3_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0A8))
#define ACU_M3_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0AC))
#define ACU_M3_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0B0))
#define ACU_M3_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0B4))
#define ACU_M3_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0B8))
#define ACU_M3_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0BC))
#define ACU_M3_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0C0))
#define ACU_M3_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0C4))
#define ACU_M3_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0C8))
#define ACU_M3_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0CC))
#define ACU_M4_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0D0))
#define ACU_M4_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0D4))
#define ACU_M4_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0D8))
#define ACU_M4_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0DC))
#define ACU_M4_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0E0))
#define ACU_M4_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0E4))
#define ACU_M4_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0E8))
#define ACU_M4_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0EC))
#define ACU_M4_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0F0))
#define ACU_M4_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0F4))
#define ACU_M4_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x0F8))
#define ACU_M4_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x0FC))
#define ACU_M4_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x100))
#define ACU_M4_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x104))
#define ACU_M4_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x108))
#define ACU_M4_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x10C))
#define ACU_M5_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x110))
#define ACU_M5_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x114))
#define ACU_M5_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x118))
#define ACU_M5_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x11C))
#define ACU_M5_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x120))
#define ACU_M5_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x124))
#define ACU_M5_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x128))
#define ACU_M5_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x12C))
#define ACU_M5_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x130))
#define ACU_M5_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x134))
#define ACU_M5_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x138))
#define ACU_M5_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x13C))
#define ACU_M5_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x140))
#define ACU_M5_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x144))
#define ACU_M5_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x148))
#define ACU_M5_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x14C))
#define ACU_M6_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x150))
#define ACU_M6_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x154))
#define ACU_M6_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x158))
#define ACU_M6_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x15C))
#define ACU_M6_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x160))
#define ACU_M6_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x164))
#define ACU_M6_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x168))
#define ACU_M6_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x16C))
#define ACU_M6_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x170))
#define ACU_M6_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x174))
#define ACU_M6_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x178))
#define ACU_M6_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x17C))
#define ACU_M6_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x180))
#define ACU_M6_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x184))
#define ACU_M6_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x188))
#define ACU_M6_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x18C))
//#define ACU_M8_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x190))
//#define ACU_M8_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x194))
//#define ACU_M8_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x198))
//#define ACU_M8_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x19C))
//#define ACU_M8_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1A0))
//#define ACU_M8_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1A4))
//#define ACU_M8_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1A8))
//#define ACU_M8_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1AC))
//#define ACU_M8_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1B0))
//#define ACU_M8_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1B4))
//#define ACU_M8_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1B8))
//#define ACU_M8_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1BC))
//#define ACU_M8_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1C0))
//#define ACU_M8_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1C4))
//#define ACU_M8_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1C8))
//#define ACU_M8_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1CC))
#define ACU_M7_CK0_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1D0))
#define ACU_M7_CK0_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1D4))
#define ACU_M7_CK1_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1D8))
#define ACU_M7_CK1_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1DC))
#define ACU_M7_CK2_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1E0))
#define ACU_M7_CK2_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1E4))
#define ACU_M7_CK3_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1E8))
#define ACU_M7_CK3_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1EC))
#define ACU_M7_CK4_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1F0))
#define ACU_M7_CK4_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1F4))
#define ACU_M7_CK5_H                             (*(volatile int *)(ACU_BASE_ADDR+0x1F8))
#define ACU_M7_CK5_L                             (*(volatile int *)(ACU_BASE_ADDR+0x1FC))
#define ACU_M7_CK6_H                             (*(volatile int *)(ACU_BASE_ADDR+0x200))
#define ACU_M7_CK6_L                             (*(volatile int *)(ACU_BASE_ADDR+0x204))
#define ACU_M7_CK7_H                             (*(volatile int *)(ACU_BASE_ADDR+0x208))
#define ACU_M7_CK7_L                             (*(volatile int *)(ACU_BASE_ADDR+0x20C))
#define ACU_M8_CK0_H                            (*(volatile int *)(ACU_BASE_ADDR+0x210))
#define ACU_M8_CK0_L                            (*(volatile int *)(ACU_BASE_ADDR+0x214))
#define ACU_M8_CK1_H                            (*(volatile int *)(ACU_BASE_ADDR+0x218))
#define ACU_M8_CK1_L                            (*(volatile int *)(ACU_BASE_ADDR+0x21C))
#define ACU_M8_CK2_H                            (*(volatile int *)(ACU_BASE_ADDR+0x220))
#define ACU_M8_CK2_L                            (*(volatile int *)(ACU_BASE_ADDR+0x224))
#define ACU_M8_CK3_H                            (*(volatile int *)(ACU_BASE_ADDR+0x228))
#define ACU_M8_CK3_L                            (*(volatile int *)(ACU_BASE_ADDR+0x22C))
#define ACU_M8_CK4_H                            (*(volatile int *)(ACU_BASE_ADDR+0x230))
#define ACU_M8_CK4_L                            (*(volatile int *)(ACU_BASE_ADDR+0x234))
#define ACU_M8_CK5_H                            (*(volatile int *)(ACU_BASE_ADDR+0x238))
#define ACU_M8_CK5_L                            (*(volatile int *)(ACU_BASE_ADDR+0x23C))
#define ACU_M8_CK6_H                            (*(volatile int *)(ACU_BASE_ADDR+0x240))
#define ACU_M8_CK6_L                            (*(volatile int *)(ACU_BASE_ADDR+0x244))
#define ACU_M8_CK7_H                            (*(volatile int *)(ACU_BASE_ADDR+0x248))
#define ACU_M8_CK7_L                            (*(volatile int *)(ACU_BASE_ADDR+0x24C))
#define ACU_M9_CK0_H                            (*(volatile int *)(ACU_BASE_ADDR+0x250))
#define ACU_M9_CK0_L                            (*(volatile int *)(ACU_BASE_ADDR+0x254))
#define ACU_M9_CK1_H                            (*(volatile int *)(ACU_BASE_ADDR+0x258))
#define ACU_M9_CK1_L                            (*(volatile int *)(ACU_BASE_ADDR+0x25C))
#define ACU_M9_CK2_H                            (*(volatile int *)(ACU_BASE_ADDR+0x260))
#define ACU_M9_CK2_L                            (*(volatile int *)(ACU_BASE_ADDR+0x264))
#define ACU_M9_CK3_H                            (*(volatile int *)(ACU_BASE_ADDR+0x268))
#define ACU_M9_CK3_L                            (*(volatile int *)(ACU_BASE_ADDR+0x26C))
#define ACU_M9_CK4_H                            (*(volatile int *)(ACU_BASE_ADDR+0x270))
#define ACU_M9_CK4_L                            (*(volatile int *)(ACU_BASE_ADDR+0x274))
#define ACU_M9_CK5_H                            (*(volatile int *)(ACU_BASE_ADDR+0x278))
#define ACU_M9_CK5_L                            (*(volatile int *)(ACU_BASE_ADDR+0x27C))
#define ACU_M9_CK6_H                            (*(volatile int *)(ACU_BASE_ADDR+0x280))
#define ACU_M9_CK6_L                            (*(volatile int *)(ACU_BASE_ADDR+0x284))
#define ACU_M9_CK7_H                            (*(volatile int *)(ACU_BASE_ADDR+0x288))
#define ACU_M9_CK7_L                            (*(volatile int *)(ACU_BASE_ADDR+0x28C))
#define ACU_M10_CK0_H                            (*(volatile int *)(ACU_BASE_ADDR+0x290))
#define ACU_M10_CK0_L                            (*(volatile int *)(ACU_BASE_ADDR+0x294))
#define ACU_M10_CK1_H                            (*(volatile int *)(ACU_BASE_ADDR+0x298))
#define ACU_M10_CK1_L                            (*(volatile int *)(ACU_BASE_ADDR+0x29C))
#define ACU_M10_CK2_H                            (*(volatile int *)(ACU_BASE_ADDR+0x2A0))
#define ACU_M10_CK2_L                            (*(volatile int *)(ACU_BASE_ADDR+0x2A4))
#define ACU_M10_CK3_H                            (*(volatile int *)(ACU_BASE_ADDR+0x2A8))
#define ACU_M10_CK3_L                            (*(volatile int *)(ACU_BASE_ADDR+0x2AC))
#define ACU_M10_CK4_H                            (*(volatile int *)(ACU_BASE_ADDR+0x2B0))
#define ACU_M10_CK4_L                            (*(volatile int *)(ACU_BASE_ADDR+0x2B4))
#define ACU_M10_CK5_H                            (*(volatile int *)(ACU_BASE_ADDR+0x2B8))
#define ACU_M10_CK5_L                            (*(volatile int *)(ACU_BASE_ADDR+0x2BC))
#define ACU_M10_CK6_H                            (*(volatile int *)(ACU_BASE_ADDR+0x2C0))
#define ACU_M10_CK6_L                            (*(volatile int *)(ACU_BASE_ADDR+0x2C4))
#define ACU_M10_CK7_H                            (*(volatile int *)(ACU_BASE_ADDR+0x2C8))
#define ACU_M10_CK7_L                            (*(volatile int *)(ACU_BASE_ADDR+0x2CC))
#define ACU_RPT_CTRL                             (*(volatile int *)(ACU_BASE_ADDR+0x310))
#define ACU_ILL_ADDR_H                           (*(volatile int *)(ACU_BASE_ADDR+0x314))
#define ACU_ILL_ADDR_L                           (*(volatile int *)(ACU_BASE_ADDR+0x318))
#define ACU_ERR_ST                               (*(volatile int *)(ACU_BASE_ADDR+0x31c))

#define ACU_INT_NUM                              39   
#define ACU_VIOL_CLR                             0x10000000   

#define ACU_RPT_CTRL_REG_h20x8			(ACU_BASE_ADDR+0x310)
#define ACU_ILL_ADDR_HIGH_REG_h20x8		(ACU_BASE_ADDR+0x314)
#define ACU_ILL_ADDR_LOW_REG_h20x8		(ACU_BASE_ADDR+0x318)
#define ACU_ERR_ST_REG_h20x8			(ACU_BASE_ADDR+0x31C)

#define ACU_RPT_CTRL_REG_h20x8_JR		(ACU_JR_h20x8_ADDR+0xD0)
#define ACU_ILL_ADDR_HIGH_REG_h20x8_JR	(ACU_JR_h20x8_ADDR+0xD4)
#define ACU_ILL_ADDR_LOW_REG_h20x8_JR	(ACU_JR_h20x8_ADDR+0xD8)
#define ACU_ERR_ST_REG_h20x8_JR			(ACU_JR_h20x8_ADDR+0xDC)


#define ACU_MST_CK_MS(m,n)				(ACU_BASE_ADDR+0x10+m*0x40+n*0x08)
#define ACU_MST_CK_LS(m,n)				(ACU_BASE_ADDR+0x10+m*0x40+n*0x08+0x04)
#define ACU_MST_CK_MS_h20x8_JR(m,n)		(ACU_JR_h20x8_ADDR+0x10+m*0x40+n*0x08)
#define ACU_MST_CK_LS_h20x8_JR(m,n)		(ACU_JR_h20x8_ADDR+0x10+m*0x40+n*0x08+0x04)
#define ACU_MST_CK_MS_h20x8_JR_exp(m,n)	(ACU_JR_h20x8_ADDR+0x30+m*0x40+n*0x08)
#define ACU_MST_CK_LS_h20x8_JR_exp(m,n)	(ACU_JR_h20x8_ADDR+0x30+m*0x40+n*0x08+0x04)

#define ACU_ALL_OFF		0x3ff


enum acu_mst_name{
	ACU_MST_PCIE0 = 0x00,
	ACU_MST_RIO,
	ACU_MST_TAP,
	ACU_MST_SEC,
	ACU_MST_DMA0,
	ACU_MST_SATA,
	ACU_MST_DPAA,
	ACU_MST_GMAC,
	ACU_MST_QUICK,
	ACU_MST_DMA1,
	ACU_MST_MAX
};

enum acu_mst_name_JR{
	ACU_MST_JOBRING0 = 0x00,
	ACU_MST_JOBRING1,
	ACU_MST_JOBRING2,
	ACU_MST_JOBRING3,
	ACU_MST_JOBRING4,
	ACU_MST_JOBRING5,
	ACU_MST_JOBRING6,
	ACU_MST_JOBRING7,
	ACU_MST_JOBRING8,		//9
};
enum acu_mst_size{
	ACU_CH_SIZE_4K = 0x00,
	ACU_CH_SIZE_8K,
	ACU_CH_SIZE_16K,
	ACU_CH_SIZE_32K,
	ACU_CH_SIZE_64K,
	ACU_CH_SIZE_128K,
	ACU_CH_SIZE_256K,
	ACU_CH_SIZE_512K,
	ACU_CH_SIZE_1M,
	ACU_CH_SIZE_2M,
	ACU_CH_SIZE_4M,
	ACU_CH_SIZE_8M,
	ACU_CH_SIZE_16M,
	ACU_CH_SIZE_32M,
	ACU_CH_SIZE_64M,
	ACU_CH_SIZE_128M,
	ACU_CH_SIZE_256M,
	ACU_CH_SIZE_512M,
	ACU_CH_SIZE_1G,
	ACU_CH_SIZE_2G,
	ACU_CH_SIZE_4G,
	ACU_CH_SIZE_8G,
	ACU_CH_SIZE_16G,
	ACU_CH_SIZE_32G,
	ACU_CH_SIZE_64G,
	ACU_CH_SIZE_128G,
	ACU_CH_SIZE_256G,
	ACU_CH_SIZE_512G
};
#define MST_CK_CHANEL_MAX		(8)
#define MST_CK_SIZE_OFFSET		(20)
#define MST_CK_EN				(1)
#define MST_CK_DIS				(0)


#define RPT_FAIL_EN				(1<<0)
#define RPT_CLR_ERR				(1<<1)

#define ERR_ST_RD_VIOL_MASK		(1<<0)
#define ERR_ST_WR_VIOL_MASK		(1<<1)
#define ERR_ST_MST_ERR_OFFSET 	(2)

#define ACU_SUCCESS				(0)
#define ACU_FAIL				(1)

#define SLAVE_HADDR0			(0x80000000)
#define SLAVE_HADDR1			(0x80000001)
#define SLAVE_HPCIEDDR0			(0x00000000)
#define SLAVE_HPCIEDDR1			(0x00000001)
#define SLAVE_HPCIEDDR2			(0x00000002)
#define SLAVE_HPCIEDDR3			(0x00000003)
#define SRAM0_START				(0xe0300000)
#define SRAM1_START				(0xe0340000)
#define SNFC_START				(0xec000000)
#define EBC_START				(0xf0000000)
#define PCIE_DDR_START			(0x00000000)

#endif