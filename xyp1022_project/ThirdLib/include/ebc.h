/*******************************************************
 * File: ebc.h
 * Created on: 2013/05/29
 * Author: YQin@ccore_sys
 ******************************************************/

#ifndef EBC_H_
#define EBC_H_

#define EBC_BASE_ADDR		0xEB00A000
/****** EBC2OPB Bank Memory ******/
//#define BANK0_ROM_ADDR			0xFF000000
//#define BANK7_ROM_ADDR			0xc0000000
//#define BANK6_RAM_ADDR			0xc8000000
//#define BANK5_RAM_ADDR			0xD0000000

#define BANK0_ROM_ADDR			0xF8000000
//#define BANK0_ROM_ADDR			0xF0000000

#define BANK1_RAM_ADDR			0xF7000000		//from 0xF7000000
#define BANK7_ROM_ADDR			0xc0000000
#define BANK6_RAM_ADDR			0xc8000000
#define BANK5_RAM_ADDR			0xD0000000

/****** EBC2OPB Registers ******/

//#define EBCx_CFGADDR			0x80
//#define EBCx_CFGDATA			0x81
//#define EBCx_CFGADDR			*(volatile unsigned int *)(EBC_BASE_ADDR+0)
//#define EBCx_CFGDATA			*(volatile unsigned int *)(EBC_BASE_ADDR+4)
#define EBCx_CFGADDR			*(volatile unsigned int *)(EBC_BASE_ADDR+0)
#define EBCx_CFGDATA			*(volatile unsigned int *)(EBC_BASE_ADDR+4)


#define EBCx_B0CR				0x0
#define EBCx_B1CR				0x1
#define EBCx_B2CR				0x2
#define EBCx_B3CR				0x3
#define EBCx_B4CR				0x4
#define EBCx_B5CR				0x5
#define EBCx_B6CR				0x6
#define EBCx_B7CR				0x7

#define EBCx_B0AP				0x10
#define EBCx_B1AP				0x11
#define EBCx_B2AP				0x12
#define EBCx_B3AP				0x13
#define EBCx_B4AP				0x14
#define EBCx_B5AP				0x15
#define EBCx_B6AP				0x16
#define EBCx_B7AP				0x17

#define EBCx_BEAR				0x20

#define EBCx_BESR				0x21

#define EBCx_CFG				0x23

#define EBCx_CID				0x24


/****** Mask Bits ******/

// EBCx_BnCR
#define EBCx_BnCR_BS_1M			0x0<<(31-14)
#define EBCx_BnCR_BS_2M			0x1<<(31-14)
#define EBCx_BnCR_BS_4M			0x2<<(31-14)
#define EBCx_BnCR_BS_8M			0x3<<(31-14)
#define EBCx_BnCR_BS_16M		0x4<<(31-14)
#define EBCx_BnCR_BS_32M		0x5<<(31-14)
#define EBCx_BnCR_BS_64M		0x6<<(31-14)
#define EBCx_BnCR_BS_128M		0x7<<(31-14)

#define EBCx_BnCR_BU_NA			0x0<<(31-16)
#define EBCx_BnCR_BU_RO			0x1<<(31-16)
#define EBCx_BnCR_BU_WO			0x2<<(31-16)
#define EBCx_BnCR_BU_RW			0x3<<(31-16)

#define EBCx_BnCR_BW_8b			0x0<<(31-18)
#define EBCx_BnCR_BW_16b		0x1<<(31-18)
#define EBCx_BnCR_BW_Rsv		0x2<<(31-18)
#define EBCx_BnCR_BW_32b		0x3<<(31-18)

// EBCx_BnAP
#define EBCx_BnAP_BME			0x1<<(31-0)

#define EBCx_BnAP_TWT_0			0x0<<(31-8)
#define EBCx_BnAP_TWT_3			0x3<<(31-8)
#define EBCx_BnAP_TWT_16		0x10<<(31-8)
#define EBCx_BnAP_TWT_32		0x20<<(31-8)
#define EBCx_BnAP_TWT_64		0x40<<(31-8)
#define EBCx_BnAP_TWT_128		0x80<<(31-8)
#define EBCx_BnAP_TWT_255		0xFF<<(31-8)

#define EBCx_BnAP_FWT_0			0x0<<(31-5)
#define EBCx_BnAP_FWT_3			0x3<<(31-5)
#define EBCx_BnAP_FWT_4			0x4<<(31-5)
#define EBCx_BnAP_FWT_8			0x8<<(31-5)
#define EBCx_BnAP_FWT_31		0x1F<<(31-5)

#define EBCx_BnAP_BWT_0			0x0<<(31-8)
#define EBCx_BnAP_BWT_2			0x2<<(31-8)
#define EBCx_BnAP_BWT_3			0x3<<(31-8)
#define EBCx_BnAP_BWT_5			0x5<<(31-8)
#define EBCx_BnAP_BWT_7			0x7<<(31-8)

#define EBCx_BnAP_BCE			0x1<<(31-9)

#define EBCx_BnAP_BCT_2			0x0<<(31-11)
#define EBCx_BnAP_BCT_4			0x1<<(31-11)
#define EBCx_BnAP_BCT_8			0x2<<(31-11)
#define EBCx_BnAP_BCT_16		0x3<<(31-11)

#define EBCx_BnAP_CSN_0			0x0<<(31-13)
#define EBCx_BnAP_CSN_1			0x1<<(31-13)
#define EBCx_BnAP_CSN_2			0x2<<(31-13)
#define EBCx_BnAP_CSN_3			0x3<<(31-13)

#define EBCx_BnAP_OEN_0			0x0<<(31-15)
#define EBCx_BnAP_OEN_1			0x1<<(31-15)
#define EBCx_BnAP_OEN_2			0x2<<(31-15)
#define EBCx_BnAP_OEN_3			0x3<<(31-15)

#define EBCx_BnAP_WBN_0			0x0<<(31-17)
#define EBCx_BnAP_WBN_1			0x1<<(31-17)
#define EBCx_BnAP_WBN_2			0x2<<(31-17)
#define EBCx_BnAP_WBN_3			0x3<<(31-17)

#define EBCx_BnAP_WBF_0			0x0<<(31-19)
#define EBCx_BnAP_WBF_1			0x1<<(31-19)
#define EBCx_BnAP_WBF_2			0x2<<(31-19)
#define EBCx_BnAP_WBF_3			0x3<<(31-19)

#define EBCx_BnAP_TH_0			0x0<<(31-22)
#define EBCx_BnAP_TH_3			0x3<<(31-22)
#define EBCx_BnAP_TH_6			0x6<<(31-22)
#define EBCx_BnAP_TH_7			0x7<<(31-13)

#define EBCx_BnAP_RE			0x1<<(31-23)

#define EBCx_BnAP_SOR			0x1<<(31-24)

#define EBCx_BnAP_BEM			0x1<<(31-25)

#define EBCx_BnAP_PEN			0x1<<(31-26)

// EBCx_CFG
#define EBCx_CFG_LE				0x1<<(31-0)

#define EBCx_CFG_PTD			0x1<<(31-1)

#define EBCx_CFG_RTC_16			0x0<<(31-4)
#define EBCx_CFG_RTC_256		0x4<<(31-4)
#define EBCx_CFG_RTC_512		0x5<<(31-4)
#define EBCx_CFG_RTC_1024		0x6<<(31-4)
#define EBCx_CFG_RTC_2048		0x7<<(31-4)

#define EBCx_CFG_ATC			0x1<<(31-5)

#define EBCx_CFG_DTC			0x1<<(31-6)

#define EBCx_CFG_CTC			0x1<<(31-7)

#define EBCx_CFG_OEO			0x1<<(31-8)

#define EBCx_CFG_EMC			0x1<<(31-9)

#define EBCx_CFG_PME			0x1<<(31-14)

#define EBCx_CFG_PMT_0			0x0<<(31-19)
#define EBCx_CFG_PMT_16			0x10<<(31-19)
#define EBCx_CFG_PMT_31			0x1F<<(31-19)

#define EBCx_CFG_PR_16			0x0<<(31-21)
#define EBCx_CFG_PR_32			0x1<<(31-21)
#define EBCx_CFG_PR_64			0x2<<(31-21)
#define EBCx_CFG_PR_128			0x3<<(31-21)

// EBCx_BESR
#define EBCx_BESR_PYE			0x1<<(31-0)

#define EBCx_BESR_PRE			0x1<<(31-1)

#define EBCx_BESR_EBE			0x1<<(31-2)

#define EBCx_BESR_ETE			0x1<<(31-3)

#define EBCx_BESR_RWS			0x1<<(31-4)

#define EBCx_BESR_SAS			0x1<<(31-5)

#define EBCx_BESR_EINT			0x1<<(31-6)

#define EBCx_BESR_DMAEP			0x1<<(31-7)

#endif /* EBC_H_ */
