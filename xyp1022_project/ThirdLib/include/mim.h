/*
 * mim.h
 *
 *  Created on: 2023-7-24
 *      Author: DELL
 */
#ifndef MIM_CFG_H_
#define MIM_CFG_H_

//#include "types.h"
#include "common.h"

/***************** regs defines ************************************************/
/*******************************************************************************/
//sram
#define MIM_TEST_BUF_BASE		0xE0070000//dma upper 15 bit is tied to 0b111000000000000

/****************************************************************************/
/*  MIMS Modules Register Addresses 				          		 		*/
/****************************************************************************/
#define MIM_REG_BASE_ADDR	0xE9507000
#define NFC_BASE_ADDR 	 	0xF1000000 //ebc bank7

#define MIM_INT_ONFI30_NUM	16

#define SCSCR0     	    (MIM_REG_BASE_ADDR + 0x00000000)      /* CS Control Register 0 */
#define SCSCR1     	    (MIM_REG_BASE_ADDR + 0x00000004)	  /* CS Control Register 1 */
#define SCSCR2          (MIM_REG_BASE_ADDR + 0x00000008)      /* CS Control Register 2 */
#define NFCSCR          (MIM_REG_BASE_ADDR + 0x00000008)      /* Nand-Flash Chip Select Control Register */
#define NPCR            (MIM_REG_BASE_ADDR + 0x0000000c)      /* Nandflash Port Register */
#define IO_CTRL         (MIM_REG_BASE_ADDR + 0x00000010)      /* IO Select Register*/ //UINT16
#define IO_CTRL0        (MIM_REG_BASE_ADDR + 0x00000010)      /* IO Select Register 0*/
#define IO_CTRL1        (MIM_REG_BASE_ADDR + 0x00000011)      /* IO Select Register 1*/

#define ROM_WS          (MIM_REG_BASE_ADDR + 0x00000012)

#define GPIO_DO         (MIM_REG_BASE_ADDR + 0x00000014)
#define GPIO_DO23       (MIM_REG_BASE_ADDR + 0x00000014) //UINT16
#define GPIO_DO01       (MIM_REG_BASE_ADDR + 0x00000016)
#define GPIO_DO3        (MIM_REG_BASE_ADDR + 0x00000014)
#define GPIO_DO2        (MIM_REG_BASE_ADDR + 0x00000015)
#define GPIO_DO1        (MIM_REG_BASE_ADDR + 0x00000016)
#define GPIO_DO0        (MIM_REG_BASE_ADDR + 0x00000017)

#define GPIO_DIR        (MIM_REG_BASE_ADDR + 0x00000018)
#define GPIO_DIR23      (MIM_REG_BASE_ADDR + 0x00000018) //UINT16
#define GPIO_DIR01      (MIM_REG_BASE_ADDR + 0x0000001a)
#define GPIO_DIR3       (MIM_REG_BASE_ADDR + 0x00000018)
#define GPIO_DIR2       (MIM_REG_BASE_ADDR + 0x00000019)
#define GPIO_DIR1       (MIM_REG_BASE_ADDR + 0x0000001a)
#define GPIO_DIR0       (MIM_REG_BASE_ADDR + 0x0000001b)

#define GPIO_DI         (MIM_REG_BASE_ADDR + 0x0000001c)
#define GPIO_DI23       (MIM_REG_BASE_ADDR + 0x0000001c)
#define GPIO_DI01       (MIM_REG_BASE_ADDR + 0x0000001e)
#define GPIO_DI3        (MIM_REG_BASE_ADDR + 0x0000001c)
#define GPIO_DI2        (MIM_REG_BASE_ADDR + 0x0000001d)
#define GPIO_DI1        (MIM_REG_BASE_ADDR + 0x0000001e)
#define GPIO_DI0        (MIM_REG_BASE_ADDR + 0x0000001f)

#define ONFI30SCR       (MIM_REG_BASE_ADDR + 0x00000020)
#define ONFI30CR        (MIM_REG_BASE_ADDR + 0x00000022)
#define ONFI30CR8       (MIM_REG_BASE_ADDR + 0x00000023)
#define ONFI30CRH8      (MIM_REG_BASE_ADDR + 0x00000022)
#define ONFI30SR        (MIM_REG_BASE_ADDR + 0x00000021)

#define ONFI30PH90CR    (MIM_REG_BASE_ADDR + 0x00000024)
#define ONFI30PDCR_GPIO (MIM_REG_BASE_ADDR + 0x00000024)
#define FRE_DLY_SEL     (MIM_REG_BASE_ADDR + 0x00000025)
#define ONFI30PDCR      (MIM_REG_BASE_ADDR + 0x00000026) //UINT16
#define ONFI30PDCRH     (MIM_REG_BASE_ADDR + 0x00000026)
#define ONFI30PDCRL     (MIM_REG_BASE_ADDR + 0x00000027)

/****************************************************************************/
/* bit defines																*/
/****************************************************************************/
/* CS Control Register Bits */
#define CS_WATS0    0x00000000    /* Write Assert Time = 0 */
#define CS_WATS1    0x10000000    /* Write Assert Time = 1 */
#define CS_WATS2    0x20000000    /* Write Assert Time = 2 */
#define CS_WATS3    0x30000000    /* Write Assert Time = 3 */
#define CS_WATS4    0x40000000    /* Write Assert Time = 4 */
#define CS_WATS5    0x50000000    /* Write Assert Time = 5 */
#define CS_WATS6    0x60000000    /* Write Assert Time = 6 */
#define CS_WATS7    0x70000000    /* Write Assert Time = 7 */

#define CS_WNTS0    0x00000000    /* Write Negate Time = 0 */
#define CS_WNTS1    0x01000000    /* Write Negate Time = 1 */
#define CS_WNTS2    0x02000000    /* Write Negate Time = 2 */
#define CS_WNTS3    0x03000000    /* Write Negate Time = 3 */
#define CS_WNTS4    0x04000000    /* Write Negate Time = 4 */
#define CS_WNTS5    0x05000000    /* Write Negate Time = 5 */
#define CS_WNTS6    0x06000000    /* Write Negate Time = 6 */
#define CS_WNTS7    0x07000000    /* Write Negate Time = 7 */

#define CS_RATS0    0x00000000    /* Read Assert Time = 0 */
#define CS_RATS1    0x00100000    /* Read Assert Time = 1 */
#define CS_RATS2    0x00200000    /* Read Assert Time = 2 */
#define CS_RATS3    0x00300000    /* Read Assert Time = 3 */
#define CS_RATS4    0x00400000    /* Read Assert Time = 4 */
#define CS_RATS5    0x00500000    /* Read Assert Time = 5 */
#define CS_RATS6    0x00600000    /* Read Assert Time = 6 */
#define CS_RATS7    0x00700000    /* Read Assert Time = 7 */

#define CS_RNTS0    0x00000000    /* Read Negate Time = 0 */
#define CS_RNTS1    0x00010000    /* Read Negate Time = 1 */
#define CS_RNTS2    0x00020000    /* Read Negate Time = 2 */
#define CS_RNTS3    0x00030000    /* Read Negate Time = 3 */
#define CS_RNTS4    0x00040000    /* Read Negate Time = 4 */
#define CS_RNTS5    0x00050000    /* Read Negate Time = 5 */
#define CS_RNTS6    0x00060000    /* Read Negate Time = 6 */
#define CS_RNTS7    0x00070000    /* Read Negate Time = 7 */

#define CS_SO       0x00008000    /* Supervisor Only */
#define CS_RO       0x00004000    /* Read Only */
#define CS_ENDIAN   0x00004000    /* Endian==1 define Little Endian */
//PS[1:0]-13 12
#define CS_PS32     0x00000000    /* Port Size 32*/
#define CS_PS16     0x00002000    /* Port Size 16*/
#define CS_PS8      0x00001000    /* Port Size 8 */

#define CS_WE       0x00000004    /* Write Enables */
#define CS_WWS      0x00000002    /* Write Wait State */
#define CS_CSEN     0x00000001    /* CS Enable */
//WS[3:0]-11 10 9 8
#define CS_WS0      0x00000000    /* Wait States = 0 */
#define CS_WS1      0x00000100    /* Wait States = 1 */
#define CS_WS2      0x00000200    /* Wait States = 2 */
#define CS_WS3      0x00000300    /* Wait States = 3 */
#define CS_WS4      0x00000400    /* Wait States = 4 */
#define CS_WS5      0x00000500    /* Wait States = 5 */
#define CS_WS6      0x00000600    /* Wait States = 6 */
#define CS_WS7      0x00000700    /* Wait States = 7 */
#define CS_WS8      0x00000800    /* Wait States = 8 */
#define CS_WS9      0x00000900    /* Wait States = 9 */
#define CS_WS10     0x00000a00    /* Wait States = 10 */
#define CS_WS11     0x00000b00    /* Wait States = 11 */
#define CS_WS12     0x00000c00    /* Wait States = 12 */
#define CS_WS13     0x00000d00    /* Wait States = 13 */
#define CS_WS14     0x00000e00    /* Wait States = 14 */
#define CS_WS15     0x00000f00    /* Wait States = 15 */

/* Nand Flash Ports Control Register */
#define NDINV       0x01000000   /* NANDFLASH INVERT*/
#define FWP_EN      0x00000000   /* Nandflash write protect enabled. */
#define FWP_DIS     0x00100000   /* Nandflash write protect disabled. */

#define CS_PULLUPEN3 0x00080000   /* Pullup Enable d[7:0] */
#define CS_PULLUPEN2 0x00040000   /* Pullup Enable d[15:8] */ 
#define CS_PULLUPEN1 0x00020000   /* Pullup Enable d[23:16] */
#define CS_PULLUPEN0 0x00010000   /* Pullup Enable d[31:24] */
#define FRE_DELAY0   0x00000008    /* Fre Delay parameter bit 0 */
#define FRE_DELAY1   0x00000010    /* Fre Delay parameter bit 1 */
#define FRE_DELAY2   0x00000020    /* Fre Delay parameter bit 2 */
#define FRE_DELAY3   0x00000040    /* Fre Delay parameter bit 3 */
#define FRE_DELAY4   0x00000080    /* Fre Delay parameter bit 4 */


/* NFC
 * Nand Flash Chip Access Channels */
#define NFC_CMD            	(NFC_BASE_ADDR + 0x00000000)      /*Command Channel*/
#define NFC_ADDR           	(NFC_BASE_ADDR + 0x00000004)      /*Address Channel*/
#define NFC_D_NONECC	   	(NFC_BASE_ADDR + 0x00000008)      /*Non-ECC Data Channel*/
#define NFC_D_ECC			(NFC_BASE_ADDR + 0x0000000c)      /*ECC Data Channel*/
#define NFC_D_DUMMYECC		(NFC_BASE_ADDR + 0x00000010)      /*Dummy ECC Data Channel*/

	

/***************** regs defines ************************************************/
/*******************************************************************************/
/****************************************************************************/
/*  DMAC Modules Register Addresses 				          		 		*/
/****************************************************************************/
#define MIM_DMA_BASE_ADDR		0xE9507800
#define MIM_INT_DMAC_DONE		17
#define MIM_DMA_TEST			0

volatile UINT8  g_mim_dma_done_intc_flg;
#if MIM_DMA_TEST
	#define  DMA_CHANNEL_0_TEST  1
	#define  DMA_CHANNEL_1_TEST  0

	#define  DMA_CHANNELn_DONE_INTC_TEST  0
#endif

#define DMA_BAR0     	    (MIM_DMA_BASE_ADDR + 0x00000000) //Buffer Address Register of Channel0
#define DMA_BAR1     	    (MIM_DMA_BASE_ADDR + 0x00000004) //Channel1
#define DMA_PAR0     	    (MIM_DMA_BASE_ADDR + 0x00000008) //Periperal Address Register of Channel0
#define DMA_PAR1     	    (MIM_DMA_BASE_ADDR + 0x0000000c) //Channel1

#define DMA_GCR		        (MIM_DMA_BASE_ADDR + 0x00000010) //Global Control
#define DMA_DSR     	    (MIM_DMA_BASE_ADDR + 0x00000012) //DMA Status

#define DMA_BCR0     	    (MIM_DMA_BASE_ADDR + 0x00000014) //Byte Count Register of Channel0
#define DMA_BCR1     	    (MIM_DMA_BASE_ADDR + 0x00000018) //Channel1
#define DMA_TCR0     	    (MIM_DMA_BASE_ADDR + 0x0000001c) //Transfer Control Register of Channel0
#define DMA_TCR1     	    (MIM_DMA_BASE_ADDR + 0x00000020) //Channel1
#define DMA_DCR0     	    (MIM_DMA_BASE_ADDR + 0x00000024) //DMA Control Register of Channel0
#define DMA_DCR1     	    (MIM_DMA_BASE_ADDR + 0x00000028) //Channel1

#define DMA_TLCCR         	(MIM_DMA_BASE_ADDR + 0x0000002c) //TLC Control Register
#define DMA_TLCCA0_16     	(MIM_DMA_BASE_ADDR + 0x00000030) //TLC Column Address of Channel0
#define DMA_TLCCA1_16     	(MIM_DMA_BASE_ADDR + 0x00000034) //Channel1
#define DMA_TLCTS0_16     	(MIM_DMA_BASE_ADDR + 0x00000032) //TLC Transfer Size of Channel0
#define DMA_TLCTS1_16     	(MIM_DMA_BASE_ADDR + 0x00000036) //Channel1 

#define DMA_TLCP0ET0_16   	(MIM_DMA_BASE_ADDR + 0x00000038) //TLC Plane0 Error Table0~31
#define DMA_TLCP0ET1_16   	(MIM_DMA_BASE_ADDR + 0x0000003a) 
#define DMA_TLCP0ET2_16   	(MIM_DMA_BASE_ADDR + 0x0000003c) 
#define DMA_TLCP0ET3_16   	(MIM_DMA_BASE_ADDR + 0x0000003e) 
#define DMA_TLCP0ET4_16   	(MIM_DMA_BASE_ADDR + 0x00000040) 
#define DMA_TLCP0ET5_16   	(MIM_DMA_BASE_ADDR + 0x00000042) 
#define DMA_TLCP0ET6_16   	(MIM_DMA_BASE_ADDR + 0x00000044) 
#define DMA_TLCP0ET7_16   	(MIM_DMA_BASE_ADDR + 0x00000046) 
#define DMA_TLCP0ET8_16     (MIM_DMA_BASE_ADDR + 0x00000048) 
#define DMA_TLCP0ET9_16     (MIM_DMA_BASE_ADDR + 0x0000004a) 
#define DMA_TLCP0ET10_16    (MIM_DMA_BASE_ADDR + 0x0000004c) 
#define DMA_TLCP0ET11_16    (MIM_DMA_BASE_ADDR + 0x0000004e) 
#define DMA_TLCP0ET12_16    (MIM_DMA_BASE_ADDR + 0x00000050) 
#define DMA_TLCP0ET13_16    (MIM_DMA_BASE_ADDR + 0x00000052) 
#define DMA_TLCP0ET14_16    (MIM_DMA_BASE_ADDR + 0x00000054) 
#define DMA_TLCP0ET15_16    (MIM_DMA_BASE_ADDR + 0x00000056) 
#define DMA_TLCP0ET16_16    (MIM_DMA_BASE_ADDR + 0x00000058) 
#define DMA_TLCP0ET17_16    (MIM_DMA_BASE_ADDR + 0x0000005a) 
#define DMA_TLCP0ET18_16    (MIM_DMA_BASE_ADDR + 0x0000005c) 
#define DMA_TLCP0ET19_16    (MIM_DMA_BASE_ADDR + 0x0000005e) 
#define DMA_TLCP0ET20_16    (MIM_DMA_BASE_ADDR + 0x00000060) 
#define DMA_TLCP0ET21_16    (MIM_DMA_BASE_ADDR + 0x00000062) 
#define DMA_TLCP0ET22_16    (MIM_DMA_BASE_ADDR + 0x00000064) 
#define DMA_TLCP0ET23_16    (MIM_DMA_BASE_ADDR + 0x00000066) 
#define DMA_TLCP0ET24_16    (MIM_DMA_BASE_ADDR + 0x00000068) 
#define DMA_TLCP0ET25_16    (MIM_DMA_BASE_ADDR + 0x0000006a) 
#define DMA_TLCP0ET26_16    (MIM_DMA_BASE_ADDR + 0x0000006c) 
#define DMA_TLCP0ET27_16    (MIM_DMA_BASE_ADDR + 0x0000006e) 
#define DMA_TLCP0ET28_16    (MIM_DMA_BASE_ADDR + 0x00000070) 
#define DMA_TLCP0ET29_16    (MIM_DMA_BASE_ADDR + 0x00000072) 
#define DMA_TLCP0ET30_16    (MIM_DMA_BASE_ADDR + 0x00000074) 
#define DMA_TLCP0ET31_16    (MIM_DMA_BASE_ADDR + 0x00000076) 
#define DMA_TLCP1ET0_16     (MIM_DMA_BASE_ADDR + 0x00000078) //TLC Plane1 Error Table0~31
#define DMA_TLCP1ET1_16     (MIM_DMA_BASE_ADDR + 0x0000007a) 
#define DMA_TLCP1ET2_16     (MIM_DMA_BASE_ADDR + 0x0000007c) 
#define DMA_TLCP1ET3_16     (MIM_DMA_BASE_ADDR + 0x0000007e) 
#define DMA_TLCP1ET4_16     (MIM_DMA_BASE_ADDR + 0x00000080) 
#define DMA_TLCP1ET5_16     (MIM_DMA_BASE_ADDR + 0x00000082) 
#define DMA_TLCP1ET6_16     (MIM_DMA_BASE_ADDR + 0x00000084) 
#define DMA_TLCP1ET7_16     (MIM_DMA_BASE_ADDR + 0x00000086) 
#define DMA_TLCP1ET8_16     (MIM_DMA_BASE_ADDR + 0x00000088) 
#define DMA_TLCP1ET9_16     (MIM_DMA_BASE_ADDR + 0x0000008a) 
#define DMA_TLCP1ET10_16    (MIM_DMA_BASE_ADDR + 0x0000008c) 
#define DMA_TLCP1ET11_16    (MIM_DMA_BASE_ADDR + 0x0000008e) 
#define DMA_TLCP1ET12_16    (MIM_DMA_BASE_ADDR + 0x00000090) 
#define DMA_TLCP1ET13_16    (MIM_DMA_BASE_ADDR + 0x00000092) 
#define DMA_TLCP1ET14_16    (MIM_DMA_BASE_ADDR + 0x00000094) 
#define DMA_TLCP1ET15_16    (MIM_DMA_BASE_ADDR + 0x00000096) 
#define DMA_TLCP1ET16_16    (MIM_DMA_BASE_ADDR + 0x00000098) 
#define DMA_TLCP1ET17_16    (MIM_DMA_BASE_ADDR + 0x0000009a) 
#define DMA_TLCP1ET18_16    (MIM_DMA_BASE_ADDR + 0x0000009c) 
#define DMA_TLCP1ET19_16    (MIM_DMA_BASE_ADDR + 0x0000009e) 
#define DMA_TLCP1ET20_16    (MIM_DMA_BASE_ADDR + 0x000000a0) 
#define DMA_TLCP1ET21_16    (MIM_DMA_BASE_ADDR + 0x000000a2) 
#define DMA_TLCP1ET22_16    (MIM_DMA_BASE_ADDR + 0x000000a4) 
#define DMA_TLCP1ET23_16    (MIM_DMA_BASE_ADDR + 0x000000a6) 
#define DMA_TLCP1ET24_16    (MIM_DMA_BASE_ADDR + 0x000000a8) 
#define DMA_TLCP1ET25_16    (MIM_DMA_BASE_ADDR + 0x000000aa) 
#define DMA_TLCP1ET26_16    (MIM_DMA_BASE_ADDR + 0x000000ac) 
#define DMA_TLCP1ET27_16    (MIM_DMA_BASE_ADDR + 0x000000ae) 
#define DMA_TLCP1ET28_16    (MIM_DMA_BASE_ADDR + 0x000000b0) 
#define DMA_TLCP1ET29_16    (MIM_DMA_BASE_ADDR + 0x000000b2) 
#define DMA_TLCP1ET30_16    (MIM_DMA_BASE_ADDR + 0x000000b4) 
#define DMA_TLCP1ET31_16    (MIM_DMA_BASE_ADDR + 0x000000b6) 

#define DMA_TLCERN0_16      (MIM_DMA_BASE_ADDR + 0x000000b8) //TLC Error Number of Channel0
#define DMA_TLCERN1_16      (MIM_DMA_BASE_ADDR + 0x000000ba) //TLC Error Number of Channel1


/****************************************************************************/
/* bit defines																*/
/****************************************************************************/
/* DMAC Global Control Register Bits */
#define GCR_DMACEN    	0x0001
#define GCR_P_BUSY_EN 	0x0100 //DMAC auto check peripheral busy

/* DMA Status Register */
#define DSR_BUSY0     	0x0001
#define DSR_DONE0     	0x0002
#define DSR_START0    	0x0004
#define DSR_BUSY1     	0x0008
#define DSR_DONE1     	0x0010
#define DSR_START1    	0x0020

#define DSR_DCHNUM    	0x0040
#define DSR_SCHNUM    	0x0080
#define DSR_AEF0      	0x0100
#define DSR_AEF1     	0x0200

/* DMA Control Register of Channeln*/
#define DCRn_VALID		0x00000001 
#define DCRn_WEN_INFI	0x00000002 //infinite data to buffer
#define DCRn_WEN_FINI	0x00000000 //only finite data to buffer(TCR)
#define DCRn_AREN		0x00000004
#define DCRn_DONEIE    	0x00000010 //DMA Done Interrupt Enable
#define DCRn_STARTIE    0x00000020 //DMA Start Interrupt Enable

#define DCRn_TSIZ_WORD      0x00000000
#define DCRn_TSIZ_BYTE      0x00000100
#define DCRn_TSIZ_HALF      0x00000200
#define DCRn_TSIZ_AUTO      0x00000300

#define DCRn_P_FIX          0x00000000 //No change to PAR Address
#define DCRn_B_FIX          0x00000000 //No change to BAR Address
#define DCRn_PDEC           0x00000040 //Peripheral Address Decrement
#define DCRn_BDEC           0x00000080 //Buffer Address Decrement
#define DCRn_PINC           0x00000400 //Peripheral Address Increment
#define DCRn_BINC           0x00000800 //Buffer Address Increment

#define DCRn_BNUM_SRAM		0x00000000 
#define DCRn_BNUM_01		0x00001000
#define DCRn_BNUM_ECC       0x00002000 
#define DCRn_BNUM_03        0x00003000
#define DCRn_TDIR_P2B      	0x00004000 //Transfer from Peripheral to Buffer
#define DCRn_TDIR_B2P      	0x00000000 //Transfer from Buffer to Peripheral

/* TLC Control Register */
#define TLCCR_CH_START0       0x00000001 //Start to check TLC nandflash error of Channelx
#define TLCCR_CH_START1       0x00000002 
#define TLCCR_TLC_EN          0x00000100 //TLC nandflash operation enabled
#define TLCCR_PL_SEL          0x00000200 //Select TLC nandflash plane n
#define TLCCR_RDY0            0x00010000
#define TLCCR_RDY1            0x00020000


#define MIM_DMA_CH0		0
#define MIM_DMA_CH1		1


/***************** regs defines ************************************************/
/*******************************************************************************/
/****************************************************************************/
/*  BCH Module Register Addresses                                       	*/
/****************************************************************************/
#define   BCH_REG_BASE_ADDR       0xE9507400
#define   ECC_REG_BASE_ADDR       0xE9507400

#define   MIM_INT_ECC_CRC_DONE_NUM		96

#define   MIM_INT_ECC_ALL1_NUM			101
#define   MIM_INT_ECC_ALL0_NUM			100
#define   MIM_INT_ECC_CODE_DONE_NUM		99 //CDDNIE
#define   MIM_INT_ECC_DECODE_FAIL_NUM	98 //DFAILIE
#define   MIM_INT_ECC_DONE_NUM			97 //DONEIE

#define   MIM_INT_ECC_CHLF_NUM			103
#define   MIM_INT_ECC_CHIERR_NUM		102


#define BCHCODE         (BCH_REG_BASE_ADDR + 0x00000000) /* BCH Byte Registers0-3*/
#define PGNCR           (BCH_REG_BASE_ADDR + 0x00000007) /* Page Numbre Counter Register*/

#define BCHSR           (BCH_REG_BASE_ADDR + 0x00000008) /* BCH Status Register*/
#define BCHFSR          (BCH_REG_BASE_ADDR + 0x0000000c) /* BCH Fail Status Register*/

#define BCHCR           (BCH_REG_BASE_ADDR + 0x00000010) /* BCH Control Register*/
#define BCHCRH          (BCH_REG_BASE_ADDR + 0x00000010) /* BCH Control Register*/
#define BCHCRL          (BCH_REG_BASE_ADDR + 0x00000012) /* BCH Control Register*/
#define BCHCR8A         (BCH_REG_BASE_ADDR + 0x00000010) /* BCH Control Register*/
#define BCHCR8B         (BCH_REG_BASE_ADDR + 0x00000011) /* BCH Control Register*/
#define BCHCR8C         (BCH_REG_BASE_ADDR + 0x00000012) /* BCH Control Register*/
#define BCHCR8D         (BCH_REG_BASE_ADDR + 0x00000013) /* BCH Control Register*/

#define BCHEBSA         (BCH_REG_BASE_ADDR + 0x00000014) /* BCH Sram Base Address Register*/
#define BCHEBSA1        (BCH_REG_BASE_ADDR + 0x00000014) /* Channel 1 */
#define BCHEBSA0        (BCH_REG_BASE_ADDR + 0x00000016) /* Channel 0 */

#define BCHDLCR         (BCH_REG_BASE_ADDR + 0x00000018) /* BCH Data Length Control Register*/
#define BCHDLCR1        (BCH_REG_BASE_ADDR + 0x00000018) /* Channel 1 */
#define BCHDLCR0        (BCH_REG_BASE_ADDR + 0x0000001a) /* Channel 0 */

#define BCHLCR          (BCH_REG_BASE_ADDR + 0x0000001e) /* BCH Level Control Register*/
#define BCHCORCR        (BCH_REG_BASE_ADDR + 0x00000022) /* BCH Correct Control Register*/
#define BCHCORCR1		(BCH_REG_BASE_ADDR + 0x00000022) /* Channel 0 */
#define BCHCORCR0       (BCH_REG_BASE_ADDR + 0x00000023) /* Channel 1 */
  

	#define ECC_ECCB0_3      (ECC_REG_BASE_ADDR + 0x00) /* ECC Byte Registers0-3*/
	#define ECC_PAGENUM      (ECC_REG_BASE_ADDR + 0x07) /* ECC Byte Registers52*/
	
	#define ECC_ECCSR        (ECC_REG_BASE_ADDR + 0x08) /* ECC Status Register*/
	#define ECC_ECCSRL       (ECC_REG_BASE_ADDR + 0x0a) /* ECC Status Register*/
	#define ECC_ECCFSR       (ECC_REG_BASE_ADDR + 0x0c) /* ECC Fail Status Register*/
	#define ECC_ECCCR        (ECC_REG_BASE_ADDR + 0x10) /* ECC Control Register*/
	#define ECC_ECCCR16      (ECC_REG_BASE_ADDR + 0x12) /* ECC Control Register*/
	
	#define ECC_EBSA         (ECC_REG_BASE_ADDR + 0x14) /* ECC EBSA Register*/
	#define ECC_EBSA1        (ECC_REG_BASE_ADDR + 0x14) /* ECC EBSA Register*/
	#define ECC_EBSA0        (ECC_REG_BASE_ADDR + 0x16) /* ECC EBSA Register*/
	#define ECC_DLCR         (ECC_REG_BASE_ADDR + 0x18)
	#define ECC_DLCR1        (ECC_REG_BASE_ADDR + 0x18)
	#define ECC_DLCR0        (ECC_REG_BASE_ADDR + 0x1a)
	#define ECC_LCR          (ECC_REG_BASE_ADDR + 0x1e)
	#define ECC_CORCR        (ECC_REG_BASE_ADDR + 0x22)
	#define ECC_CORCR1       (ECC_REG_BASE_ADDR + 0x22)
	#define ECC_CORCR0       (ECC_REG_BASE_ADDR + 0x23)


#define CRCCR            (ECC_REG_BASE_ADDR + 0x26)	/* CRC Controll Register */
#define CRCDATA0_3       (ECC_REG_BASE_ADDR + 0x28) /* CRC DATA Register */
#define CRCDATA4_7       (ECC_REG_BASE_ADDR + 0x2c)
#define CRCDATA8_b       (ECC_REG_BASE_ADDR + 0x30)
#define CRCDATAc_f       (ECC_REG_BASE_ADDR + 0x34)

#define CRCVALUE         (ECC_REG_BASE_ADDR + 0x38)	/* CRC CODE Register */
#define CRCCODE          (ECC_REG_BASE_ADDR + 0x38)	/* CRC CODE Register */


/****************************************************************************/
/* bit defines																*/
/****************************************************************************/

/* BCH Control Register (BCHCR) */
#define BCHCR_SOFTRST               0x01000000 
#define BCHCR_MPTEN                 0x00100000
#define BCHCR_SKCOR                 0x00080000
#define BCHCR_DUMYCS                0x00040000
#define BCHCR_TM1_DECODE            0x00020000 //only valid in BCH Test mode.
#define BCHCR_TM1_ENCODE            0x00000000 //only valid in BCH Test mode.
#define BCH_DECODE	1
#define BCH_ENCODE	0
#define BCHCR_TM0_TESTMODE_EN	  	0x00010000 //Test Mode disabled/enable.

#define BCHCR_EBSAAI                0x00008000
#define BCHCR_LENCTRL               0x00004000 //(not) include control information length
#define BCHCR_CH1EN                 0x00002000
#define BCHCR_CH0EN                 0x00001000

#define BCHCR_DCTRLEN               0x00000800 //Double Control Channel used Enable.
#define BCHCR_CHENAC                0x00000400 //Channel enable auto clear
#define BCHCR_IGALL1                0x00000200
#define BCHCR_ALL1_IGNORE	        0x00000200 //decoding
#define BCHCR_ALL1_NOT_IGNORE       0x00000000 //
#define BCHCR_S_SKIP                0x00000100

#define BCHCR_CHLFIE                0x00000080
#define BCHCR_CHIERRIE              0x00000040
#define BCHCR_ALL1IE                0x00000020
#define BCHCR_ALL0IE                0x00000010
#define BCHCR_CDDNIE                0x00000004
#define BCHCR_DFAILE                0x00000002
#define BCHCR_BCH_DONEIE            0x00000001
  
/* BCH Status Register */
#define BCHSR_CTRLPTR             	0x00008000
#define BCHSR_CHLF                	0x00000080
#define BCHSR_CHIERR                0x00000040
#define BCHSR_ALL1              	0x00000020
#define BCHSR_ALL0                	0x00000010
#define BCHSR_CORBSY                0x00000008
#define BCHSR_CDDN                	0x00000004
#define BCHSR_FAIL                	0x00000002
#define BCHSR_DONE          		0x00000001

/* BCH Correct Control Register(BCHCORCR) */
#define CTRL1_EN              0x8000 //whether the control bytes will be correction
#define LEFT1_EN              0x4000 //Left half bytes data
#define RIGHT1_EN             0x2000 //Right half bytes data
#define CODE1_EN              0x1000 //whether the BCH code will be correction

#define CTRL0_EN              0x0080
#define LEFT0_EN              0x0040
#define RIGHT0_EN             0x0020
#define CODE0_EN              0x0010

#define BCH_CH0		0
#define BCH_CH1		1



/* CRC Ctrl Register */
#define CRCCR_MODE_ENCODE	(0 <<0)
#define CRCCR_MODE_DECODE	(1 <<0)

#define CRCCR_DATALEN(n)		(n <<1)
#define CRCCR_TYPE(n)		(n <<3)

#define CRCCR_START			(1 <<5)
#define CRCCR_SOFTRST		(1 <<7)
#define CRCCR_DONE			(1 <<8) //RO
#define CRCCR_ERR			(1 <<9) //RO
#define CRCCR_DONEIE		(1 <<15)
/*************** fun defines **************************************************/
/******************************************************************************/
void MIM_DMA_RegTest(void);
void mim_dma_config( UINT32 bar, UINT32 par, UINT32 len, 
					  UINT32 ch_ctrl,UINT8 chn );
void mim_dma_ch_start( UINT8 chn );
void mim_dma_disable( void );

void MIM_BCH_RegTest(void);

#endif /* DSPI_H_ */
