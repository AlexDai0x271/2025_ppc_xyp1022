/*******************************************************
 * File: sd_mmc_card.h
 * 	Card driver specific definitions.
 * 	Header for SD Memory Card/MultiMediaCard (MMC)
 * Date: 2017
 * Author: ccore sys
 ******************************************************/

#ifndef SD_MMC_CARD_H
#define SD_MMC_CARD_H

//#include "types.h"
//#include "sec_common.h"
#include "common.h"

#define SD_HOST_TEST   			0
#define EMMC_HOST_TEST 		 	1

#define DEBUG_PRINT				0

#define SINGLE_BLOCK_TEST		1
#define MULTI_BLOCK_TEST		0
#define WHOLE_BLOCK_TEST		0
#define ERASE_EMMC				0

#if 1
	//H2040
//	#include "e500mc_asm.h"
//	extern  void set_e0_tlb(UINT32 r3,UINT32 r4,UINT32 r5,UINT32 r6,UINT32 r7,UINT32 r8);
	#define CHIP_CCP903T		0
	#define CHIP_CCP908T		0
	#define	CHIP_CCP907T		0
	#define	CHIP_RAID			0
	#define	CHIP_H2040			0

	#define	CHIP_S1020			1
	#define CHIP_STRING   		"S1020"
#endif


#if 0//def MEM_BIGENDIA
#define cpu_to_le32_emmc(v32)	 ( ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8) ) //
#define le32_to_cpu_emmc(v32) 	 ( ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8) )

#define cpu_to_le16_emmc(v16)	 ( ((v16 &0xff)<<8)| ((v16 &0xff00)>>8) ) //
#define le16_to_cpu_emmc(v16) 	 ( ((v16 &0xff)<<8)| ((v16 &0xff00)>>8) )

#else
#define cpu_to_le32_emmc(v32)	 ((v32))
#define le32_to_cpu_emmc(v32) 	 ((v32))
#endif

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	/* SDIO specific error defines */
	SD_CMD_CRC_FAIL                    = (1), /* Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /* Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /* Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /* Data time out */
	SD_TX_UNDERRUN                     = (5), /* Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /* Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /* Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /* CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /* Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /* Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /* An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /* An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /* Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /* Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /* CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /* Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /* Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /* Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /* General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /* The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /* The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /* CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /* only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /* Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /* Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /* Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27), //read ACMD41 times beyond range
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),

	/* Standard error defines */
	SD_INTERNAL_ERROR,
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING,
	SD_REQUEST_NOT_APPLICABLE,
	SD_INVALID_PARAMETER,
	SD_UNSUPPORTED_FEATURE,
	SD_UNSUPPORTED_HW,
	SD_ERROR,
	SD_OK,
} SD_Error;


/* CMD: SDIO Commands  Index */
#define SDIO_GO_IDLE_STATE                       ((U8)0)
#define SDIO_SEND_OP_COND                        ((U8)1)
#define SDIO_ALL_SEND_CID                        ((U8)2)
#define SDIO_SET_REL_ADDR                        ((U8)3) /* SDIO_SEND_REL_ADDR for SD Card */
#define SDIO_SET_DSR                             ((U8)4)
#define SDIO_SDIO_SEN_OP_COND                    ((U8)5)
#define SDIO_HS_SWITCH                           ((U8)6)
#define SDIO_SEL_DESEL_CARD                      ((U8)7)
#define SDIO_HS_SEND_EXT_CSD                     ((U8)8)
#define SDIO_SEND_CSD                            ((U8)9)
#define SDIO_SEND_CID                            ((U8)10)
#define SDIO_READ_DAT_UNTIL_STOP                 ((U8)11) /* SD Card doesn't support it */
#define SDIO_STOP_TRANSMISSION                   ((U8)12)
#define SDIO_SEND_STATUS                         ((U8)13)
#define SDIO_HS_BUSTEST_READ                     ((U8)14)
#define SDIO_GO_INACTIVE_STATE                   ((U8)15)

#define SDIO_SET_BLOCKLEN                        ((U8)16)
#define SDIO_READ_SINGLE_BLOCK                   ((U8)17)
#define SDIO_READ_MULT_BLOCK                     ((U8)18)
#define SDIO_HS_BUSTEST_WRITE                    ((U8)19)
#define SDIO_WRITE_DAT_UNTIL_STOP                ((U8)20) /* SD Card doesn't support it */
#define SDIO_SET_BLOCK_COUNT                     ((U8)23) /* SD Card doesn't support it */

#define SDIO_WRITE_SINGLE_BLOCK                  ((U8)24)
#define SDIO_WRITE_MULT_BLOCK                    ((U8)25)
#define SDIO_PROG_CID                            ((U8)26) /* reserved for manufacturers */
#define SDIO_PROG_CSD                            ((U8)27)
#define SDIO_SET_WRITE_PROT                      ((U8)28)
#define SDIO_CLR_WRITE_PROT                      ((U8)29)
#define SDIO_SEND_WRITE_PROT                     ((U8)30)
#define SDIO_SD_ERASE_GRP_START                  ((U8)32) /* To set the address of the first write
                                                             block to be erased. (For SD card only) */
#define SDIO_SD_ERASE_GRP_END                    ((U8)33) /* To set the address of the last write block of the
                                                             continuous range to be erased. (For SD card only) */
#define SDIO_ERASE_GRP_START                     ((U8)35) /* To set the address of the first write block to be erased.
                                                             (For MMC card only spec 3.31) */

#define SDIO_ERASE_GRP_END                       ((U8)36) /* To set the address of the last write block of the
                                                             continuous range to be erased. (For MMC card only spec 3.31) */

#define SDIO_ERASE                               ((U8)38)
#define SDIO_FAST_IO                             ((U8)39) /* SD Card doesn't support it */
#define SDIO_GO_IRQ_STATE                        ((U8)40) /* SD Card doesn't support it */
#define SDIO_LOCK_UNLOCK                         ((U8)42)
#define SDIO_APP_CMD                             ((U8)55)
#define SDIO_GEN_CMD                             ((U8)56)
#define SDIO_NO_CMD                              ((U8)64)

/* ACMD:  Following commands are SD Card Specific commands.
   SDIO_APP_CMD should be sent before sending these
   commands. */
#define SDIO_APP_SD_SET_BUSWIDTH                 ((U8)6)  /* For SD Card only */
#define SDIO_SD_APP_STAUS                        ((U8)13) /* For SD Card only */
#define SDIO_SD_APP_SEND_NUM_WRITE_BLOCKS        ((U8)22) /* For SD Card only */
#define SDIO_SD_APP_OP_COND                      ((U8)41) /* For SD Card only */
#define SDIO_SD_APP_SET_CLR_CARD_DETECT          ((U8)42) /* For SD Card only */
#define SDIO_SD_APP_SEND_SCR                     ((U8)51) /* For SD Card only */
#define SDIO_SDIO_RW_DIRECT                      ((U8)52) /* For SD I/O Card only */
#define SDIO_SDIO_RW_EXTENDED                    ((U8)53) /* For SD I/O Card only */

/* Following commands are SD Card Specific security commands.
   SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SD_APP_GET_MKB                      ((U8)43) /* For SD Card only */
#define SDIO_SD_APP_GET_MID                      ((U8)44) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RN1                  ((U8)45) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RN2                  ((U8)46) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RES2                 ((U8)47) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RES1                 ((U8)48) /* For SD Card only */
#define SDIO_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((U8)18) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((U8)25) /* For SD Card only */
#define SDIO_SD_APP_SECURE_ERASE                 ((U8)38) /* For SD Card only */
#define SDIO_SD_APP_CHANGE_SECURE_AREA           ((U8)49) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MKB             ((U8)48) /* For SD Card only */

typedef enum
{
	SD_NO_TRANSFER  = 0,
	SD_TRANSFER_IN_PROGRESS
} SDTransferState;

typedef struct
{
	U16 TransferredBytes;
	SD_Error TransferError;
	U8  padding;
} SDLastTransferInfo;

typedef struct       /* CSD: Card Specific Data */
{
	volatile U8  CSDStruct;            /* CSD structure */	//[127:126]
	volatile U8  SysSpecVersion;       /* System specification version */
	volatile U8  Reserved1;            /* Reserved */
	volatile U8  TAAC;                 /* Data read access-time 1 */	//[119:112]
	volatile U8  NSAC;                 /* Data read access-time 2 in CLK cycles */ //[111:104]
	volatile U8  MaxBusClkFrec;        /* Max. bus clock frequency */	//[103:96]
	volatile U16 CardComdClasses;      /* Card command classes */
	volatile U8  RdBlockLen;           /* Max. read data block length */
	volatile U8  PartBlockRead;        /* Partial blocks for read allowed */
	volatile U8  WrBlockMisalign;      /* Write block misalignment */
	volatile U8  RdBlockMisalign;      /* Read block misalignment */
	volatile U8  DSRImpl;              /* DSR implemented */
	volatile U8  Reserved2;            /* Reserved */
	volatile U32 DeviceSize;           /* Device Size */
	volatile U8  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
	volatile U8  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
	volatile U8  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
	volatile U8  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
	volatile U8  DeviceSizeMul;        /* Device size multiplier */
	volatile U8  EraseGrSize;          /* Erase group size */
	volatile U8  EraseGrMul;           /* Erase group size multiplier */
	volatile U8  WrProtectGrSize;      /* Write protect group size */
	volatile U8  WrProtectGrEnable;    /* Write protect group enable */
	volatile U8  ManDeflECC;           /* Manufacturer default ECC */
	volatile U8  WrSpeedFact;          /* Write speed factor */
	volatile U8  MaxWrBlockLen;        /* Max. write data block length */
	volatile U8  WriteBlockPaPartial;  /* Partial blocks for write allowed */
	volatile U8  Reserved3;            /* Reserded */
	volatile U8  ContentProtectAppli;  /* Content protection application */
	volatile U8  FileFormatGrouop;     /* File format group */
	volatile U8  CopyFlag;             /* Copy flag (OTP) */
	volatile U8  PermWrProtect;        /* Permanent write protection */
	volatile U8  TempWrProtect;        /* Temporary write protection */
	volatile U8  FileFormat;           /* File Format */
	volatile U8  ECC;                  /* ECC code */
	volatile U8  CSD_CRC;              /* CSD CRC */
	volatile U8  Reserved4;            /* always 1*/
} SD_CSD;

typedef struct      /* CID: Card Identification Data*/
{
	volatile U8  ManufacturerID;       /* ManufacturerID */
	volatile U16 OEM_AppliID;          /* OEM/Application ID */
	volatile U32 ProdName1;            /* Product Name part1 */
	volatile U8  ProdName2;            /* Product Name part2*/
	volatile U8  ProdRev;              /* Product Revision */
	volatile U32 ProdSN;               /* Product Serial Number */
	volatile U8  Reserved1;            /* Reserved1 */
	volatile U16 ManufactDate;         /* Manufacturing Date */
	volatile U8  CID_CRC;              /* CID CRC */
	volatile U8  Reserved2;            /* always 1 */
} SD_CID;

typedef struct
{
	SD_CSD SD_csd;
	SD_CID SD_cid;
	U32 CardCapacity; /* Card Capacity */
	U32 CardBlockSize; /* Card Block Size */
	U16 RCA;
	U8 CardType;
} SD_CardInfo;


//Card Status/R1
#define R1_OUT_OF_RANGE		(1 << 31)	/* er, c */
#define R1_ADDRESS_ERROR	(1 << 30)	/* erx, c */
#define R1_BLOCK_LEN_ERROR	(1 << 29)	/* er, c */
#define R1_ERASE_SEQ_ERROR  (1 << 28)	/* er, c */
#define R1_ERASE_PARAM		(1 << 27)	/* ex, c */
#define R1_WP_VIOLATION		(1 << 26)	/* erx, c */
#define R1_CARD_IS_LOCKED	(1 << 25)	/* sx, a */
#define R1_LOCK_UNLOCK_FAILED	(1 << 24)	/* erx, c */
#define R1_COM_CRC_ERROR	(1 << 23)	/* er, b */
#define R1_ILLEGAL_COMMAND	(1 << 22)	/* er, b */
#define R1_CARD_ECC_FAILED	(1 << 21)	/* ex, c */
#define R1_CC_ERROR			(1 << 20)	/* erx, c */
#define R1_ERROR			(1 << 19)	/* erx, c */
#define R1_UNDERRUN			(1 << 18)	/* ex, c */
#define R1_OVERRUN			(1 << 17)	/* ex, c */
#define R1_CID_CSD_OVERWRITE	(1 << 16)	/* erx, c, CID/CSD overwrite */
#define R1_WP_ERASE_SKIP		(1 << 15)	/* sx, c */
#define R1_CARD_ECC_DISABLED	(1 << 14)	/* sx, a */
#define R1_ERASE_RESET			(1 << 13)	/* sr, c */

#define R1_STATUS(x)            (x & 0xFFFFE000)
#define R1_CURRENT_STATE(x)	((x & 0x00001E00) >> 9)	/* sx, b (4 bits) */
	#define R1_STATE_IDLE	0
	#define R1_STATE_READY	1
	#define R1_STATE_IDENT	2
	#define R1_STATE_STBY	3
	#define R1_STATE_TRAN	4
	#define R1_STATE_DATA	5
	#define R1_STATE_RCV	6
	#define R1_STATE_PRG	7
	#define R1_STATE_DIS	8
#define R1_READY_FOR_DATA	(1 << 8)	/* sx, a */
#define R1_SWITCH_ERROR		(1 << 7)	/* sx, c */
#define R1_APP_CMD			(1 << 5)	/* sr, c */

#if EMMC_HOST_TEST
	//CMD0
	#define CMD0ARG_GO_PRE_IDLE_STATE 	0xF0F0F0F0
	#define CMD0ARG_BOOT_INITIATION   	0xFFFFFFFA
	#define CMD0ARG_GO_IDLE_STATE     	0x00000000
#endif

//ACMD41-R3
#define AC41_S18R_Bit24  	BIT(24) //1.8V switching request
#define AC41_XPC_Bit28   	BIT(28) //SDXC power control

#define OCR_BUSY_Bit31  	BIT(31)
	//This bit is set to LOW if the Device has not finished the power up routine
	#define OCR_POWER_BUSY  (0 <<31)
	#define OCR_POWER_OK	(1 <<31)
#define OCR_CCS_BIT30		BIT(30) //CCS/Card Capacity Status
	#define SD_HIGH_CAPACITY  (1 <<30)
#define OCR_S18A_Bit24  	AC41_S18R_Bit24 //1.8V switching accepted by card

#define SD_MAX_VOLT_TRIAL               ((U32)0x0000FFFF)


//CMD3-R6
#define SD_R6_GENERAL_UNKNOWN_ERROR     (1 <<13) //= R1_ERROR
#define SD_R6_ILLEGAL_CMD               (1 <<14) //= R1_ILLEGAL_COMMAND
#define SD_R6_COM_CRC_FAILED            (1 <<15) //= R1_COM_CRC_ERROR


//ACMD6-para
#define SD_ACMD6_BUS_WIDTH_4BIT    	2
#define SD_ACMD6_BUS_WIDTH_1BIT 	0

//ACMD42-para
#define SD_ACMD42_CONNECT   1
#define SD_ACMD42_DISCONN 	0

//CMD6-para
//[31]
#define SD_CMD6_CHEAK_FUNCTION			(0 <<31)
#define SD_CMD6_SWITCH_FUNCTION			(1 <<31)
//[3:0]
#define SD_CMD6_Group1_ACCESS( Func_n )		(Func_n <<0)
#define SD_CMD6_Group2_CMDSYS( Func_n )		(Func_n <<4)
//[11:8] funciton group 3 for drive strength
#define SD_CMD6_Group3_DRIVES( Func_n )		(Func_n <<8)
//[15:12] function group 4 for current limit
#define SD_CMD6_Group4_CURLIM( Func_n )		(Func_n <<12)
	//Available Functions
	#define SD_Switch_Func_DefaultSDR12		0 //in Group 1
	#define SD_Switch_Func_HighSpeed		1
	#define SD_Switch_Func_HighSpeedSDR25	1
	#define SD_Switch_Func_SDR50			2
	#define SD_Switch_Func_SDR104			3
	#define SD_Switch_Func_DDR50			4
	#define SD_Switch_Func_Limit200mA		0 //in Group 4
	#define SD_Switch_Func_Limit400mA		1
	#define SD_Switch_Func_Limit600mA		2
	#define SD_Switch_Func_Limit800mA		3

#if EMMC_HOST_TEST
//CMD6-para
	// [25:24] MMC_SWITCH access modes
	#define MMC_CMD6_ACCESSMODE_CMD_SET		(0x00 <<24)/* Change the command set */
	#define MMC_CMD6_ACCESSMODE_SET_BITS	(0x01 <<24)/* Set bits which are 1 in value */
	#define MMC_CMD6_ACCESSMODE_CLEAR_BITS	(0x02 <<24)/* Clear bits which are 1 in value */
	#define MMC_CMD6_ACCESSMODE_WRITE_BYTE	(0x03 <<24)/* Set target to value */

	#define MMC_CMD6_INDEX(n)		(n <<16)
	#define MMC_CMD6_VALUE(val)		(val <<8)
	#define MMC_CMD6_CMDSET(class)	(class <<0)

	/* EXT_CSD register fields/MMC_CMD6_INDEX(n)
	 * */
	#define EXT_CSD_FLUSH_CACHE				32  /* W */
	#define EXT_CSD_CACHE_CTRL				33  /* R/W */
	#define EXT_CSD_POWER_OFF_NOTIFICATION	34	/* R/W */
	#define EXT_CSD_DATA_SECTOR_SIZE		61	/* R */
	#define EXT_CSD_GP_SIZE_MULT			143	/* R/W */
	#define EXT_CSD_PARTITION_ATTRIBUTE		156	/* R/W */
	#define EXT_CSD_PARTITION_SUPPORT		160	/* RO */
	#define EXT_CSD_HPI_MGMT				161	/* R/W */
	#define EXT_CSD_RST_N_FUNCTION			162	/* R/W */
	#define EXT_CSD_SANITIZE_START			165 /* W */
	#define EXT_CSD_WR_REL_PARAM			166	/* RO */
	#define EXT_CSD_BOOT_WP					173	/* R/W */
	#define EXT_CSD_ERASE_GROUP_DEF			175	/* R/W */
	#define EXT_CSD_PART_CONFIG				179	/* R/W */
	#define EXT_CSD_ERASED_MEM_CONT			181	/* RO */
	#define EXT_CSD_BUS_WIDTH				183	/* R/W */
	#define EXT_CSD_HS_TIMING				185	/* R/W */
	#define EXT_CSD_POWER_CLASS				187	/* R/W */

	#define EXT_CSD_REV						192	/* RO */
	#define EXT_CSD_STRUCTURE				194	/* RO */
	#define EXT_CSD_CARD_TYPE				196	/* RO */
	#define EXT_CSD_OUT_OF_INTERRUPT_TIME	198	/* RO */
	#define EXT_CSD_PART_SWITCH_TIME        199 /* RO */
	#define EXT_CSD_PWR_CL_52_195			200	/* RO */
	#define EXT_CSD_PWR_CL_26_195			201	/* RO */
	#define EXT_CSD_PWR_CL_52_360			202	/* RO */
	#define EXT_CSD_PWR_CL_26_360			203	/* RO */
	#define EXT_CSD_SEC_CNT					212	/* RO, 4 bytes */
	#define EXT_CSD_S_A_TIMEOUT				217	/* RO */
	#define EXT_CSD_REL_WR_SEC_C			222	/* RO */
	#define EXT_CSD_HC_WP_GRP_SIZE			221	/* RO */
	#define EXT_CSD_ERASE_TIMEOUT_MULT		223	/* RO */
	#define EXT_CSD_HC_ERASE_GRP_SIZE		224	/* RO */
	#define EXT_CSD_BOOT_MULT				226	/* RO */
	#define EXT_CSD_SEC_TRIM_MULT			229	/* RO */
	#define EXT_CSD_SEC_ERASE_MULT			230	/* RO */
	#define EXT_CSD_SEC_FEATURE_SUPPORT		231	/* RO */
	#define EXT_CSD_TRIM_MULT				232	/* RO */
	#define EXT_CSD_PWR_CL_200_195			236	/* RO */
	#define EXT_CSD_PWR_CL_200_360			237	/* RO */
	#define EXT_CSD_PWR_CL_DDR_52_195		238	/* RO */
	#define EXT_CSD_PWR_CL_DDR_52_360		239	/* RO */
	#define EXT_CSD_POWER_OFF_LONG_TIME		247	/* RO */
	#define EXT_CSD_GENERIC_CMD6_TIME		248	/* RO */
	#define EXT_CSD_CACHE_SIZE				249	/* RO, 4 bytes */
	#define EXT_CSD_TAG_UNIT_SIZE			498	/* RO */
	#define EXT_CSD_DATA_TAG_SUPPORT		499	/* RO */
	#define EXT_CSD_HPI_FEATURES			503	/* RO */

	/* EXT_CSD field definitions/ MMC_CMD6_VALUE()
	 */
	//EXT_CSD_Sanitize		165
	#define EXT_CSD_SANNTIZE_ENABLE		1

	//EXT_CSD_BUS_WIDTH		183
	#define EXT_CSD_BUS_WIDTH_1			0	/* Card is in 1 bit mode */
	#define EXT_CSD_BUS_WIDTH_4			1	/* Card is in 4 bit mode */
	#define EXT_CSD_BUS_WIDTH_8			2	/* Card is in 8 bit mode */
	#define EXT_CSD_DDR_BUS_WIDTH_4		5	/* Card is in 4 bit DDR mode */
	#define EXT_CSD_DDR_BUS_WIDTH_8		6	/* Card is in 8 bit DDR mode */
	//EXT_CSD_HS_TIMING		185
	#define EXT_CSD_HS_TIMING_BACK		0	/* backwards compatibility */
	#define EXT_CSD_HS_TIMING_HSPEED	1	/* High Speed */
	#define EXT_CSD_HS_TIMING_HS200		2
	#define EXT_CSD_HS_TIMING_HS400		3
	//EXT_CSD_CARD_TYPE		196
	#define EXT_CSD_CARD_TYPE_26		(1<<0)	/* Card can run at 26MHz */
	#define EXT_CSD_CARD_TYPE_52		(1<<1)	/* Card can run at 52MHz */
	#define EXT_CSD_CARD_TYPE_MASK		0x3F	/* Mask out reserved bits */
	#define EXT_CSD_CARD_TYPE_DDR_1_8V  (1<<2)  /* Card can run at 52MHz */
							 	 	 	 	 	/* DDR mode @1.8V or 3V I/O */
	#define EXT_CSD_CARD_TYPE_DDR_1_2V  (1<<3)  /* Card can run at 52MHz */
												/* DDR mode @1.2V I/O */
	#define EXT_CSD_CARD_TYPE_DDR_52    (EXT_CSD_CARD_TYPE_DDR_1_8V  \
										| EXT_CSD_CARD_TYPE_DDR_1_2V)
	#define EXT_CSD_CARD_TYPE_SDR_1_8V	(1<<4)	/* Card can run at 200MHz */
	#define EXT_CSD_CARD_TYPE_SDR_1_2V	(1<<5)	/* Card can run at 200MHz */
												/* SDR mode @1.2V I/O */
	#define EXT_CSD_CARD_TYPE_SDR_200	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
						 	 	 	 	 EXT_CSD_CARD_TYPE_SDR_1_2V)
	#define EXT_CSD_CARD_TYPE_SDR_ALL	(EXT_CSD_CARD_TYPE_SDR_200 | \
											 EXT_CSD_CARD_TYPE_52 | \
											 EXT_CSD_CARD_TYPE_26)
//	#define	EXT_CSD_CARD_TYPE_SDR_1_2V_ALL	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
//											 EXT_CSD_CARD_TYPE_52 | \
//											 EXT_CSD_CARD_TYPE_26)
	#define	EXT_CSD_CARD_TYPE_SDR_1_8V_ALL	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
											 EXT_CSD_CARD_TYPE_52 | \
											 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_1_8V	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
//												 EXT_CSD_CARD_TYPE_DDR_1_8V | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_1_8V	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
//												 EXT_CSD_CARD_TYPE_DDR_1_8V | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_1_2V	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
//												 EXT_CSD_CARD_TYPE_DDR_1_2V | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_1_2V	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
//												 EXT_CSD_CARD_TYPE_DDR_1_2V | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_52	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
//												 EXT_CSD_CARD_TYPE_DDR_52 | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_52	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
//												 EXT_CSD_CARD_TYPE_DDR_52 | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_ALL_DDR_1_8V	(EXT_CSD_CARD_TYPE_SDR_200 | \
//												 EXT_CSD_CARD_TYPE_DDR_1_8V | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_ALL_DDR_1_2V	(EXT_CSD_CARD_TYPE_SDR_200 | \
//												 EXT_CSD_CARD_TYPE_DDR_1_2V | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
//	#define EXT_CSD_CARD_TYPE_SDR_ALL_DDR_52	(EXT_CSD_CARD_TYPE_SDR_200 | \
//												 EXT_CSD_CARD_TYPE_DDR_52 | \
//												 EXT_CSD_CARD_TYPE_52 | \
//												 EXT_CSD_CARD_TYPE_26)
	#define EXT_CSD_CARD_TYPE_DDR_200_1_8V	(1<<6)	/* Card can run at HS400 200MHz 1.8V */
	#define EXT_CSD_CARD_TYPE_DDR_200_1_2V	(1<<7)	/* Card can run at HS400 200MHz 1.2V */
												/* SDR mode @1.2V I/O */
	#define EXT_CSD_CARD_TYPE_DDR_200	(EXT_CSD_CARD_TYPE_DDR_200_1_8V | \
										EXT_CSD_CARD_TYPE_DDR_200_1_2V)
#endif


/* R1: Mask for errors Card Status R1 */
#define SD_R1_ADDR_OUT_OF_RANGE        ((U32)0x80000000) //The command?¡¥s argument
#define SD_R1_ADDR_MISALIGNED          ((U32)0x40000000) //misaligned address
#define SD_R1_BLOCK_LEN_ERR            ((U32)0x20000000)
#define SD_R1_ERASE_SEQ_ERR            ((U32)0x10000000)
#define SD_R1_BAD_ERASE_PARAM          ((U32)0x08000000)
#define SD_R1_WRITE_PROT_VIOLATION     ((U32)0x04000000)
#define SD_R1_CARD_IS_LOCKED   		   ((U32)0x02000000)
#define SD_R1_LOCK_UNLOCK_FAILED       ((U32)0x01000000)
#define SD_R1_COM_CRC_FAILED           ((U32)0x00800000)
#define SD_R1_ILLEGAL_CMD              ((U32)0x00400000)
#define SD_R1_CARD_ECC_FAILED          ((U32)0x00200000)
#define SD_R1_CC_ERROR                 ((U32)0x00100000)
#define SD_R1_GENERAL_UNKNOWN_ERROR    ((U32)0x00080000)
#define SD_R1_STREAM_READ_UNDERRUN     ((U32)0x00040000) //??
#define SD_R1_STREAM_WRITE_OVERRUN     ((U32)0x00020000) //??
#define SD_R1_CID_CSD_OVERWRIETE       ((U32)0x00010000)
#define SD_R1_WP_ERASE_SKIP            ((U32)0x00008000)
#define SD_R1_CARD_ECC_DISABLED        ((U32)0x00004000)
#define SD_R1_ERASE_RESET              ((U32)0x00002000)
#define SD_R1_AKE_SEQ_ERROR            ((U32)0x00000008)
#define SD_R1_ERRORBITS                ((U32)0xFDF9E008)//((U32)0xFDFFE008) //31-26/24-13/3

#define SD_DATATIMEOUT                  ((U32)0x001FFFFF)
#define SD_CARD_PROGRAMMING             ((U32)0x00000007) //12:9/CURRENT_STATE
#define SD_CARD_RECEIVING               ((U32)0x00000006)

/* Exported constants --------------------------------------------------------*/
#define SD_DMA_MODE                     ((U32)0x00000000)
#define SD_INTERRUPT_MODE               ((U32)0x00000001)
#define SD_POLLING_MODE                 ((U32)0x00000002)

/* Supported Memory Cards */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1     ((U32)0x0)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0     ((U32)0x1)
#define SDIO_HIGH_CAPACITY_SD_CARD         ((U32)0x2) //OCR bit[30]:CCS
#define SDIO_HIGH_CAPACITY_SD_CARD_V3_0    ((U32)0x3) //SCR[]

#define SDIO_MULTIMEDIA_CARD               ((U32)0x4)
#define SDIO_SECURE_DIGITAL_IO_CARD        ((U32)0x5)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD    ((U32)0x6)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD  ((U32)0x7)
#define SDIO_HIGH_CAPACITY_MMC_CARD        ((U32)0x8)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
SD_Error SD_Init(void);
SD_Error SD_PowerON(void);
SD_Error SD_PowerOFF(void);
SD_Error SD_InitializeCards(void);
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
SD_Error SD_EnableWideBusOperation(U32 WideMode);
SD_Error SD_SetDeviceMode(U32 Mode);
SD_Error SD_SelectDeselect(U32 rca);
SD_Error SD_ReadBlock(U32 addr, U32 *readbuff, U16 BlockSize, U32 NumberOfBlocks);
SD_Error SD_WriteBlock(U32 addr, U32 *writebuff, U16 BlockSize, U32 NumberOfBlocks);
SD_Error SD_StopTransfer(void);
SD_Error SD_Erase(U32 startaddr, U32 endaddr);
SD_Error SD_SendSDStatus(U32 *psdstatus);
SD_Error SD_EnumCard(void);
SD_Error SD_SendStatus(void);
SD_Error SD_SendSCR(U32 *scr);
SD_Error SD_EnWideBus(void);
SD_Error SD_SwitchFunc_V2_0(void);
SD_Error SD_SwitchFunc_V3_0(void);
SD_Error SD_SwitchFunc_1d8V_V3_0(void);
SD_Error SD_CmdResp1Error(void);


#endif /* SD_MMC_CARD_H */
