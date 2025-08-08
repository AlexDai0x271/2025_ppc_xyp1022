/*******************************************************
 * File: dwc_mmc.h
 * 	Synopsys DesignWare Multimedia Card Interface driver
 * 	DesignWare Cores Mobile Storage Host
 * Date: 2017
 * Author: ccore sys
 ******************************************************/

#ifndef _DW_MMC_H_
#define _DW_MMC_H_

//#include "types.h"
#include "common.h"
#include "sd_mmc_card.h"

//----------------------- base addr ------------------------------------------------------------

#if CHIP_CCP903T
	#define SD_EMMC_HOST_BASE			(0xE0000000)
	#define SDMMC_HOST_INT_NUM 			(50)
	#define SDMMC_BUFFERs_BASE_HI		0x80000000
#elif CHIP_CCP905T
	#define SD_EMMC_HOST_BASE			(0xE0000000)
	#define SDMMC_HOST_INT_NUM 			(48)
	#define SDMMC_BUFFERs_BASE_HI		0x80000000
#elif CHIP_CCP908T
	#define SD_EMMC_HOST_BASE			(0xE0000000)
	#define SDMMC_HOST_INT_NUM 			(48)

	//sram
	#define SDMMC_BUFFERs_BASE_HI		0x80000030
	#define SDMMC_BUFFERs_BASE			0xE0300000
	//ddr
//	#define SDMMC_BUFFERs_BASE_HI		0x80000000
//	#define SDMMC_BUFFERs_BASE			0x01000000
#elif CHIP_S1020
	#define SD_EMMC_HOST_BASE			(0xE9100000)
	#define SDMMC_HOST_INT_NUM 			(43)

	//sram
	#define SDMMC_BUFFERs_BASE_HI		0x04
	#define SDMMC_BUFFERs_BASE			0xE0070000		
	//ddr
//	#define SDMMC_BUFFERs_BASE_HI		0x00			
//	#define SDMMC_BUFFERs_BASE			0x00000000
#endif

//-----------------------------------------------
#if SD_HOST_TEST
	#define SDMMC_HOST_SD20_TEST			1
	#if SDMMC_HOST_SD20_TEST
		#define SDMMC_HOST_SD30_TEST		0
		#if SDMMC_HOST_SD30_TEST
			#define SDMMC_HOST_SD30_1D8V_TEST	0
		#endif
	#endif
	#define SDMMC_HOST_DWIDTH_1b_TEST		0
	#define SDMMC_HOST_DWIDTH_4b_TEST		1
#endif
#if EMMC_HOST_TEST
	#define SDMMC_HOST_EMMC_1D8V_TEST		1
	#define SDMMC_HOST_EMMC_HS400_TEST		0

	#define SDMMC_HOST_DWIDTH_1b_TEST		0
	#define SDMMC_HOST_DWIDTH_4b_TEST		0
	#define SDMMC_HOST_DWIDTH_8b_TEST		1
#endif

#define SDMMC_HOST_INT_TEST		0
#define USE_INTERNAL_DMAC  		1
//-----------------------------------------------

//----------------------- test env ------------------------------------------------------------
#define FPGA_TEST  		0
#define EVB_903T_TEST  	0  //SCK V2.0
#define EVB_905T_TEST  	0
#define EVB_908T_TEST  	0
//----------------------- DWC MSH------------------------------------------------------------
/*
 */
#define	SDMMC_CTRL				(SD_EMMC_HOST_BASE +0x00)
#define	SDMMC_PWREN				(SD_EMMC_HOST_BASE +0x04)
#define SDMMC_CLKDIV			(SD_EMMC_HOST_BASE +0x08)
#define SDMMC_CLKSRC			(SD_EMMC_HOST_BASE +0x0C)
#define SDMMC_CLKENA			(SD_EMMC_HOST_BASE +0x10)
#define SDMMC_TMOUT				(SD_EMMC_HOST_BASE +0x14)
#define SDMMC_CTYPE				(SD_EMMC_HOST_BASE +0x18)
#define SDMMC_BLKSIZ			(SD_EMMC_HOST_BASE +0x1C)
#define SDMMC_BYTCNT			(SD_EMMC_HOST_BASE +0x20)
#define SDMMC_INTMASK			(SD_EMMC_HOST_BASE +0x24) //Interrupt disable/enable.
#define SDMMC_CMDARG			(SD_EMMC_HOST_BASE +0x28)
#define SDMMC_CMD				(SD_EMMC_HOST_BASE +0x2C)
#define SDMMC_RESP0				(SD_EMMC_HOST_BASE +0x30) //RO
#define SDMMC_RESP1				(SD_EMMC_HOST_BASE +0x34)
#define SDMMC_RESP2				(SD_EMMC_HOST_BASE +0x38)
#define SDMMC_RESP3				(SD_EMMC_HOST_BASE +0x3C)
#define SDMMC_MINTSTS			(SD_EMMC_HOST_BASE +0x40) //RO:show disable/enable status [and] Gen status.MISTATS = RIINTSTS & INTMASK
#define SDMMC_RINTSTS			(SD_EMMC_HOST_BASE +0x44) //Interrupt Gen Status, logged regardless of interrupt mask status
#define SDMMC_STATUS			(SD_EMMC_HOST_BASE +0x48)
#define SDMMC_FIFOTH			(SD_EMMC_HOST_BASE +0x4C)
#define SDMMC_CDETECT			(SD_EMMC_HOST_BASE +0x50)
#define SDMMC_WRTPRT			(SD_EMMC_HOST_BASE +0x54)
#define SDMMC_GPIO				(SD_EMMC_HOST_BASE +0x58)
#define SDMMC_TCBCNT			(SD_EMMC_HOST_BASE +0x5C) //Transferred CIU Card Byte Count
#define SDMMC_TBBCNT			(SD_EMMC_HOST_BASE +0x60) //Transferred Host and BIU-FIFO Byte Count
#define SDMMC_DEBNCE			(SD_EMMC_HOST_BASE +0x64)
#define SDMMC_USRID				(SD_EMMC_HOST_BASE +0x68) //RO
#define SDMMC_VERID				(SD_EMMC_HOST_BASE +0x6C)
#define SDMMC_HCON				(SD_EMMC_HOST_BASE +0x70) //RO/Hardware Configuration Register
#define SDMMC_UHS_REG			(SD_EMMC_HOST_BASE +0x74)
#define SDMMC_RST_N				(SD_EMMC_HOST_BASE +0x78) //

#define SDMMC_BMOD				(SD_EMMC_HOST_BASE +0x80)
#define SDMMC_PLDMND			(SD_EMMC_HOST_BASE +0x84)

//#define SDMMC_DBADDR			(SD_EMMC_HOST_BASE +0x88) //Descriptor List Base Address
	#define	SDMMC_DBADDRL			(SD_EMMC_HOST_BASE +0x88) //64bit
	#define	SDMMC_DBADDRU			(SD_EMMC_HOST_BASE +0x8C)

//#define SDMMC_IDSTS				(SD_EMMC_HOST_BASE +0x8c) //Internal DMAC Status
	#define SDMMC_IDSTS				(SD_EMMC_HOST_BASE +0x90) //64bit

//#define SDMMC_IDINTEN			(SD_EMMC_HOST_BASE +0x90) //Internal DMAC Interrupt Enable
	#define SDMMC_IDINTEN			(SD_EMMC_HOST_BASE +0x94) //64bit

//#define SDMMC_DSCADDR			(SD_EMMC_HOST_BASE +0x94)
	#define	SDMMC_DSCADDRL			(SD_EMMC_HOST_BASE +0x98) //64bit
	#define	SDMMC_DSCADDRU			(SD_EMMC_HOST_BASE +0x9C)

//#define SDMMC_BUFADDR				(SD_EMMC_HOST_BASE +0x98)
	#define	SDMMC_BUFADDRL			(SD_EMMC_HOST_BASE +0xA0) //64bit
	#define	SDMMC_BUFADDRU			(SD_EMMC_HOST_BASE +0xA4)

#define SDMMC_CARDTHRCTL			(SD_EMMC_HOST_BASE +0x100)
#define SDMMC_BACK_END_POWER		(SD_EMMC_HOST_BASE +0x104)
#define SDMMC_UHS_EXT				(SD_EMMC_HOST_BASE +0x108)
#define SDMMC_EMMC_DDR				(SD_EMMC_HOST_BASE +0x10C)
#define SDMMC_ENABLE_SHIFT			(SD_EMMC_HOST_BASE +0x110)

#define SDMMC_DATA(x)				(x)
#define SDMMC_FIFO_DATA				(SD_EMMC_HOST_BASE +0x200)
#define DATA_OFFSET					0x200
//----------------------- BIT------------------------------------------------------------
/* shift bit field */
#define _SBF(f, v)		((v) << (f))
#ifndef BIT
#define BIT(x) 			(1 << (x))
#endif
#define DDR_REG(x)		(16+x)
/* SDMMC_CTRL 0x00:
 * Control register defines */
#define SDMMC_CTRL_USE_IDMAC		BIT(25)//en Internal DMAC
#define SDMMC_CTRL_EN_OD			BIT(24)//External open-drain pullup

#define SDMMC_CTRL_CEATA_INT_EN		BIT(11)//CE-ATA device Interrupts
#define SDMMC_CTRL_SEND_AS_CCSD		BIT(10)//CMD12/ auto_stop to CE-ATA device
#define SDMMC_CTRL_SEND_CCSD		BIT(9) //to CE-ATA device?
#define SDMMC_CTRL_ABRT_READ_DATA	BIT(8)
#define SDMMC_CTRL_SEND_IRQ_RESP	BIT(7) //CMD40
#define SDMMC_CTRL_READ_WAIT		BIT(6)
#define SDMMC_CTRL_DMA_ENABLE		BIT(5) //
#define SDMMC_CTRL_INT_ENABLE		BIT(4) //Global interrupt
#define SDMMC_CTRL_DMA_RESET		BIT(2) //issue a reset to DMA interface
#define SDMMC_CTRL_FIFO_RESET		BIT(1)
#define SDMMC_CTRL_RESET			BIT(0) //Reset DWC_mobile_storage controller

/* SDMMC_CLKSRC 0x00C:
 * SD Clock Source Register */
#define SDMMC_CLK_SOURCE_0		(0)
#define SDMMC_CLK_SOURCE_1		(1)
#define SDMMC_CLK_SOURCE_2		(2)
#define SDMMC_CLK_SOURCE_3		(3)

/* SDMMC_CLKENA 0x010:
 * Clock Enable register defines */
#define SDMMC_CLKEN_LOW_PWR		(16) //stop clock when card in IDLE
#define SDMMC_CLKEN_ENABLE		(0)

/*SDMMC_TMOUT 0x014:
 * time-out register defines */
#define SDMMC_TMOUT_DATA(n)		_SBF(8, (n))
#define SDMMC_TMOUT_DATA_MSK	0xFFFFFF00
#define SDMMC_TMOUT_RESP(n)		((n) & 0xFF)
#define SDMMC_TMOUT_RESP_MSK	0xFF

/* SDMMC_CTYPE 0x18:
 * card-type register defines */
#define SDMMC_CTYPE_8BIT		BIT(16) //eMMC
#define SDMMC_CTYPE_4BIT		1  //SD
#define SDMMC_CTYPE_1BIT		0

/* SDMMC_INTMASK 0x24/SDMMC_MINTSTS 0x40/SDMMC_RINTSTS 0x44:
 * Interrupt status & mask register defines
 * */
#define SDMMC_INT_SDIO(n)		BIT(16 + (n))
#define SDMMC_INT_EBE			BIT(15)//End-bit error
#define SDMMC_INT_ACD			BIT(14)
#define SDMMC_INT_SBE			BIT(13)//Start Bit Error
#define SDMMC_INT_BCI			BIT(13)//Busy Clear Interrupt
#define SDMMC_INT_HLE			BIT(12)//Hardware locked write error
#define SDMMC_INT_FRUN			BIT(11)//FIFO underrun/overrun error
#define SDMMC_INT_HTO			BIT(10)
#define SDMMC_INT_VOLT_SWITCH	BIT(10)
//#define SDMMC_INT_DTO			BIT(9) //
#define SDMMC_INT_DRTO			BIT(9) //Data read timeout
#define SDMMC_INT_RTO			BIT(8) //Response timeout
#define SDMMC_INT_DCRC			BIT(7) //Data CRC error
#define SDMMC_INT_RCRC			BIT(6) //Response CRC
#define SDMMC_INT_RXDR			BIT(5)
#define SDMMC_INT_TXDR			BIT(4)
#define SDMMC_INT_DATA_OVER		BIT(3) //DTO
#define SDMMC_INT_CMD_DONE		BIT(2) //CD
#define SDMMC_INT_RESP_ERR		BIT(1) //RE:Response error
#define SDMMC_INT_CD			BIT(0) //CD:Card detect
#define SDMMC_INT_ERROR			0xbfc2 //DW_MCI_ERROR_FLAGS:9/7/10/13/15+8/6/1+12+BIT(11)
	/*Dw_mmc.c   Common flag combinations */
#define DWC_MSH_DATA_ERROR_FLAGS	(SDMMC_INT_DRTO| SDMMC_INT_DCRC| SDMMC_INT_HTO| SDMMC_INT_SBE| SDMMC_INT_EBE)
#define DWC_MSH_CMD_ERROR_FLAGS	(SDMMC_INT_RTO | SDMMC_INT_RCRC| SDMMC_INT_RESP_ERR)
#define DWC_MSH_ERROR_FLAGS	(DWC_MSH_DATA_ERROR_FLAGS | \
							 DWC_MSH_CMD_ERROR_FLAGS | \
							 SDMMC_INT_HLE) //0xb7c2  //SDMMC_INT_ERROR=0xbfc2

/* SDMMC_CMD 0x2C:
 * Command register defines */
#define SDMMC_CMD_START			BIT(31)

#define SDMMC_CMD_USE_HOLD		BIT(29)
#define SDMMC_CMD_VOLT_SWITCH	BIT(28)

#define SDMMC_CMD_CCS_EXP		BIT(23)//Command Completion Signal
#define SDMMC_CMD_CEATA_RD		BIT(22)//disable read data timeout
#define SDMMC_CMD_UPD_CLK		BIT(21)//Do not send commands
#define SDMMC_CMD_INIT			BIT(15)//After power on, 80 clocks must be sent
#define SDMMC_CMD_STOP			BIT(14)//stop current data transfer
#define SDMMC_CMD_PRV_DAT_WAIT	BIT(13)//Wait for previous data transfer completion
//#define SDMMC_CMD_SEND_STOP	BIT(12)//
#define SDMMC_CMD_SEND_AUTO_STOP BIT(12)//
#define SDMMC_CMD_STRM_MODE		BIT(11)//Block/Stream
#define SDMMC_CMD_DAT_WR		BIT(10)//Wr
#define SDMMC_CMD_DAT_RD		(0 << 10)//Rd
#define SDMMC_CMD_DAT_EXP		BIT(9) //Data transfer expected
#define SDMMC_CMD_RESP_CRC		BIT(8) //Check CRC
#define SDMMC_CMD_RESP_LONG		BIT(7) //Short/Long response
#define SDMMC_CMD_RESP_EXP		BIT(6) //Response expected

#define SDMMC_CMD_INDX(n)		((n) & 0x3F)
/* SDMMC_STATUS	0x48:
 * Status register defines */
#define SDMMC_GET_FCNT(x)		(((x)>>17) & 0x1FFF) //fifo_count
#define SDMMC_GET_R_IDX(x)		(((x)>>11) & 0x3F)   //response_index
#define SDMMC_STATUS_BUSY		BIT(9)
#define SDMMC_FIFO_FULL			BIT(3)
#define SDMMC_FIFO_EMPTY		BIT(2)

/* SDMMC_CDETECT 0x50:
 * Card Detect Register */
#define SDMMC_CDETECT_CARD_PRESENT	0

/* SDMMC_VERID 0x6C:
 * Version ID register define */
#define SDMMC_GET_VERID(x)		((x) & 0xFFFF)

/* SDMMC_HCON 0x70:
 * Hardware Configuration Register */
//	#define SDMMC_HCON_CARD_TYPE(n)     	(n & 0x00000001)
#define  CARD_TYPE_MMC_ONLY			0
#define  CARD_TYPE_SD_MMC			1
//	#define SDMMC_HCON_NUM_CARDS(n)			(((n & 0x0000003E) >>1) +1)
//	#define SDMMC_HCON_H_BUS_TYPE(n)        ((n & 0x00000040) >>6)
#define  H_BUS_TYPE_APB				0
#define  H_BUS_TYPE_AHB				1
//	#define SDMMC_HCON_H_DATA_WIDTH(n)      ((n & 0x00000380) >>7)
#define  H_DATA_WIDTH_16BIT			0
#define  H_DATA_WIDTH_32BIT			1
#define  H_DATA_WIDTH_64BIT			2
//	#define SDMMC_HCON_H_ADDR_WIDTH(n)      (((n & 0x0000FC00) >>10) +1)
//	#define SDMMC_HCON_FIFO_RAM_INSIDE(n)	((n & 0x00200000) >>21)
#define  FIFO_RAM_OUTSIDE			0
#define  FIFO_RAM_INSIDE			1
//	#define SDMMC_HCON_ADDR_CONFIG(n)		((n & 0x08000000) >>27)
#define  ADDR_CONFIG_32BIT			0
#define  ADDR_CONFIG_64BIT			1

typedef struct
{
#if 0
	U32 card_type:1;
	U32 num_card:5;
	U32 h_bus_type:1;
	U32 h_data_width:3;
	U32 h_addr_width:6;
	U32 dma_interface:2;
	U32 dma_data_width:3;
	U32 fifo_ram_inside:1;
	U32 implement_hold_reg:1;
	U32 set_clk_false_path:1;
	U32 num_clk_divider:2;
	U32 area_optimized:1;
	U32 addr_config:5;
#else
	U32 reserved:4;     //bit31
	U32 addr_config:1;
	U32 area_optimized:1;
	U32 num_clk_divider:2;
	U32 set_clk_false_path:1;
	U32 implement_hold_reg:1;
	U32 fifo_ram_inside:1;
	U32 dma_data_width:3;
	U32 dma_interface:2;
	U32 h_addr_width:6;
	U32 h_data_width:3;
	U32 h_bus_type:1;
	U32 num_card:5;
	U32 card_type:1; //bit0
#endif
}HCON_Reg_t;


/* UHS_REG 0x78: Hardware reset */
#define SD2_3D3  0
#define UHS_1D8  1

/* SDMMC_RST_N 0x78: Hardware reset */
#define SDMMC_RST_SLOT_0       0xFFFFFFFE
#define SDMMC_RST_SLOT_1       0xFFFFFFFD

	#if USE_INTERNAL_DMAC
/* SDMMC_BMOD 0x80: Bus Mode Register */
#define SDMMC_BMOD_IDMAC_EN		BIT(7)
#define SDMMC_BMOD_FIXED_BURST	BIT(1) //use only SINGLE, INCR4, INCR8 or INCR16
#define SDMMC_BMOD_SWR			BIT(0) //Software Reset.

/* SDMMC_IDSTS 0x8C/SDMMC_IDINTEN 0x90:  Internal DMAC interrupt defines */
#define SDMMC_IDMAC_INT_AI		BIT(9)
#define SDMMC_IDMAC_INT_NI		BIT(8)
#define SDMMC_IDMAC_INT_CES		BIT(5)
#define SDMMC_IDMAC_INT_DU		BIT(4)
#define SDMMC_IDMAC_INT_FBE		BIT(2)
#define SDMMC_IDMAC_INT_RI		BIT(1) //Receive
#define SDMMC_IDMAC_INT_TI		BIT(0) //Transmit

/* SDMMC_BMOD 0x80: Internal DMAC bus mode bits */
#define SDMMC_IDMAC_ENABLE		BIT(7) //the IDMAC is enabled.
#define SDMMC_IDMAC_FB			BIT(1) //Fixed Burst.
#define SDMMC_IDMAC_SWRESET		BIT(0) //the DMA Controller resets all its internal registers
	#endif

/* SDMMC_CARDTHRCTL 0x100: Card Threshold Control */
#define SDMMC_WRTHR_EN			BIT(2) //Fixed Burst.
#define SDMMC_BSYCLR_EN			BIT(1) //
#define SDMMC_RDTHR_EN			BIT(0) //

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
#define FLAG_TIMEOUT		50000 //status
#define RESP_TIMEOUT		500000 //

/* slot distribution
 * Note:maybe different in other chip/EVB.
 * */

#if 1
#define EMMC_SLOT0    		0
#define SD_SLOT1     		1
#else
#define EMMC_SLOT0    		1
#define SD_SLOT1     		0
#endif

#if ( CHIP_CCP903T || CHIP_CCP905T )
	#define FIFO_DEPTH		0x1000 //word
#else
	#define FIFO_DEPTH		0x400 //word
//	#define FIFO_DEPTH		0x800 //word
#endif

#define BLOCK_SIZE			512

#define SDIO_PowerState_OFF ((U32)0x00000000)
#define SDIO_PowerState_ON  ((U32)0x00000003)

#if FPGA_TEST
	#define SDIO_CLK_IN  (12000)//kHz  in=12M

	#define FPGA_CLKOUT_400kHz_DIV  (SDIO_CLK_IN/400/2)
	#define FPGA_CLKOUT_100kHz_DIV  (SDIO_CLK_IN/100/2)

	#define FPGA_CLKOUT_1MHz_DIV  (SDIO_CLK_IN/1000/2)//div=6
	#define FPGA_CLKOUT_2MHz_DIV  (SDIO_CLK_IN/2000/2)//=3
	#define FPGA_CLKOUT_3MHz_DIV  (SDIO_CLK_IN/3000/2)//=2
	#define FPGA_CLKOUT_6MHz_DIV  (SDIO_CLK_IN/6000/2)//=1
//	#define FPGA_CLKOUT_8MHz_DIV  //no
//	#define FPGA_CLKOUT_4MHz_DIV  //=FPGA_CLKOUT_6MHz_DIV

	#define FPGA_CLKOUT_12MHz_DIV   (SDIO_CLK_IN/12000/2)//=0

	#if 0
	#define SDIO_CLK_IN  (12500/2)//kHz
	#endif
	#if 0
	#define SDIO_CLK_IN  (20000)//kHz
	#define FPGA_CLKOUT_20MHz_DIV   (SDIO_CLK_IN/20000/2)
	#endif
	#if 0
	#define SDIO_CLK_IN  (24000)//kHz
	#define FPGA_CLKOUT_12MHz_DIV   (SDIO_CLK_IN/12000/2)	
	#endif
#else

	#if 1
	#define SDIO_CLK_IN  (200000)//kHz  	996MHz-200M/cpr-0x40

	#define EVB_CLKOUT_400kHz_DIV  (SDIO_CLK_IN/400/2)//div=250
	#define EVB_CLKOUT_2MHz_DIV  (SDIO_CLK_IN/2000/2)//
	#define EVB_CLKOUT_4MHz_DIV  (SDIO_CLK_IN/4000/2)
	#define EVB_CLKOUT_8MHz_DIV  (SDIO_CLK_IN/8000/2)

	#define EVB_CLKOUT_12d5MHz_DIV (SDIO_CLK_IN/12500/2 )//=8
	#define EVB_CLKOUT_20MHz_DIV   (SDIO_CLK_IN/20000/2) //=5
	#define EVB_CLKOUT_25MHz_DIV   (SDIO_CLK_IN/25000/2) //=4
	#define EVB_CLKOUT_33d3MHz_DIV  (SDIO_CLK_IN/33000/2)//=3

	#define EVB_CLKOUT_50MHz_DIV   (SDIO_CLK_IN/50000/2) //=2
	#define EVB_CLKOUT_100MHz_DIV  (SDIO_CLK_IN/100000/2)//=1
	#define EVB_CLKOUT_200MHz_DIV  (0)//=1
	#endif
	#if 0
	#define SDIO_CLK_IN  (250000)//kHz  	996MHz-cpr-0x30

	#define EVB_CLKOUT_500kHz_DIV   (SDIO_CLK_IN/500/2)

	#define EVB_CLKOUT_62MHz_DIV   (SDIO_CLK_IN/62500/2)   //=2
	#define EVB_CLKOUT_125MHz_DIV   (SDIO_CLK_IN/125000/2) //=1
	#endif
	#if 0
	#define SDIO_CLK_IN  (332000)//kHz  	cpr-0x20

	#define EVB_CLKOUT_700kHz_DIV   (SDIO_CLK_IN/700/2)

	#define EVB_CLKOUT_80MHz_DIV   (SDIO_CLK_IN/80000/2)   //=2
	#define EVB_CLKOUT_160MHz_DIV   (SDIO_CLK_IN/160000/2) //=1
	#endif
#endif
////////////////////////////////////////////////////////////////////
#if USE_INTERNAL_DMAC
	/***********description**********************/
	//addr for Descriptor List
	#define IDMA_DESCRIPTION_ADDR	(SDMMC_BUFFERs_BASE +0x0) //single description size = 32byte

	struct idmac_desc_64addr {
		U32		des0;	/* Control Descriptor */
		U32		des1;	/* Reserved */

		U32		des2;	/*Buffer sizes */
		U32		des3;	/* Reserved */

		U32		des4;	/* Lower 32-bits of Buffer Address Pointer 1*/
		U32		des5;	/* Upper 32-bits of Buffer Address Pointer 1*/

		U32		des6;	/* Lower 32-bits of Next Descriptor Address */
		U32		des7;	/* Upper 32-bits of Next Descriptor Address */
	};

	//DES0
	#define IDMAC_DES0_OWN	BIT(31) //The descriptor is owned by the IDMAC
	#define IDMAC_DES0_CES	BIT(30) //Error
	#define IDMAC_DES0_ER	BIT(5) //End of Ring
	#define IDMAC_DES0_CH	BIT(4) //Chained/Next Descriptor address
	#define IDMAC_DES0_FD	BIT(3) //First buffer of the data
	#define IDMAC_DES0_LD	BIT(2) //Last buffers of the data
	#define IDMAC_DES0_DIC	BIT(1) //Disable Interrupt
	//DES2
	#define IDMAC_DESC2_BUFSIZE1	((1 <<13)-1)//[12:0]
#endif


///////////////////////////////function map////////////////////////////////
void SDIO_HCON(void);
U8 SDIO_PollStatus(U32 flag_s);
void SDIO_Start_CMD(void);
void SDIO_Init(void);
void SDIO_SetClk(U32 slot_num, U32 div_val);
void SDIO_SwitchClk( U8 flag,U8 card_number );
U8 SDIO_Reset(void);
U8 SDIO_FIFO_Reset(void);
U8 SDIO_CardDetect(void);
void SDIO_Card_Volt(U32 slot_num, U8 flag);
void SDIO_PowerUp(U32 slot_num, U8 flag);
U8 SDIO_GetPowerState(U32 slot_num);
extern void SDIO_Internal_Dmac_Ctrl(U8 flag);
extern void SDIO_Config_Idma_Desc( U32 des_start_addr,
							U32 dbuf_start_addr,U32 total_len );

//inline const char *R1_CurState_string( U8 val );
void CMD0_SD_tsk( U8 card_number );
void CMD8_SD_tsk( U32 arg,U8 card_number );
void CMD11_SD_tsk( U8 card_number );
void CMD55_SD_tsk( U32 rca,U8 card_number );
void ACMD41_SD_tsk( U32 arg,U8 card_number );
void CMD2_SD_tsk( U8 card_number );
void CMD3_SD_tsk( U8 card_number );
void CMD9_SD_tsk( U32 rca,U8 card_number );
void CMD7_SD_tsk( U32 rca,U8 card_number );
void CMD13_SD_tsk( U32 rca,U8 card_number );
void ACMD42_SD_tsk( U32 rca, U32 set_cd,U8 card_number );
void ACMD51_SD_tsk( U32 arg,U32 byte_num,U8 card_number );
void ACMD6_SD_tsk( U32 rca,U32 busw,U8 card_number );
void CMD16_SD_tsk( U16 blocklen,U8 card_number );
void CMD6_SD_tsk( U32 arg,U32 byte_num,U8 card_number );
void CMD17_SD_tsk( U32 arg,U32 byte_num,U8 card_number );
void CMD18_SD_tsk( U32 arg,U32 byte_num,U8 card_number );
void CMD12_SD_tsk( U8 card_number );
void CMD24_SD_tsk( U32 arg,U32 byte_num,U8 card_number );
void CMD25_SD_tsk( U32 arg,U32 byte_num,U8 card_number );

#if SDMMC_HOST_INT_TEST
	#if USE_INTERNAL_DMAC
		extern volatile U8 g_intr_DMA;
	#endif

	//extern volatile U8 g_intr_RTO;
	extern volatile U8 g_intr_RXDR;
	extern volatile U8 g_intr_TXDR;
	extern volatile U8 g_intr_DATA_OVER;
	extern volatile U8 g_intr_CMD_DONE;
	extern volatile U8 g_intr_CD;
	extern volatile U8 g_intr_VOIT_SWITCH;

	void SD_interrupt_isr( void );
	void SD_enable_sdio_irq( U32 slot_num, U8 enb );
#endif

#if EMMC_HOST_TEST
	void CMD0_MMC_tsk( U32 arg,U8 card_number );
	void CMD1_MMC_tsk( U32 arg,U8 card_number );
	void CMD3_MMC_tsk( U32 rca,U8 card_number );
	void CMD8_MMC_tsk( U32 byte_num,U8 card_number );
	void CMD6_MMC_tsk( U32 arg,U8 card_number );

	SD_Error MMC_Send_ExtendedCSD(U32 *ext_csd);
	SD_Error MMC_Switch( U8 set, U8 index, U8 value );
#endif


#endif /* DWC_CTRL_DRIVER_H_ */
