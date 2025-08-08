#include "T1022RDB_init.h"

#pragma section code_type ".init"

#ifdef __cplusplus
extern "C" {
#endif

void __reset(void) __attribute__ ((section (".init")));
void usr_init1() __attribute__ ((section (".init")));
void usr_init2() __attribute__ ((section (".init")));

extern void __start();
extern unsigned long gInterruptVectorTable;
extern unsigned long gInterruptVectorTableEnd;



#ifdef __cplusplus
}
#endif

void __reset(void)
{
	//
	//   Enable machine check exceptions, floating-point
	//
	asm("lis    3, 0x0000");
	asm("ori    3, 3, 0x3000\n");
	asm("mtmsr  3");
	asm("b      __start");
}

#define CCSRBAR 			0xFE000000ULL
#define LAW_GROUP_OFFSET	0x00000000
#define IFC_GROUP_OFFSET	0x00124000
#define ESPI_GROUP_OFFSET	0x00110000
#define DDRC_GROUP_OFFSET	0x00008000
#define DCFG_GROUP_OFFSET	0x000E0000

register unsigned int  	ddr_type asm ("r20");

void usr_init1() {
	//########################################################################################
	//# Initialization file for T1022 RDB
	//# Clock Configuration:
	//#         CPU: 1400 MHz,   CCB: 600 MHz,
	//#         DDR: 1600 MHz,   IFC: 150 MHz,
	//#       FMAN1:  600 MHz,  QMAN: 300 MHz,  PME: 300 MHz
	//########################################################################################
	//
	//##################################################################################
	//#
	//#	Memory Map
	//#
	//#   0x00000000  0x7FFFFFFF  DDR                 2G
	//#   0xE0000000  0xEFFFFFFF  NOR               256M
	//#   0xF0000000  0xF03FFFFF  DCSR                4M
	//#   0xFE000000  0xFEFFFFFF  CCSR Space         16M
	//#   0xFF800000  0xFF8FFFFF  NAND                1M
	//#   0xFFDF0000  0xFFDF0FFF  CPLD                4K
	//#   0xFFFFF000  0xFFFFFFFF  Boot Page           4K
	//#
	//##################################################################################
	//
	//
	//##################################################################################
	//# MMU initialization

	//# define 16MB  TLB entry  1: 0xFE000000 - 0xFEFFFFFF for CCSR    cache inhibited, guarded
	asm ("lis	5, 0x1001");
	asm ("ori 5, 5, 0");
	asm ("mtspr 624, 5");

	asm ("lis	5, 0xC000");
	asm ("ori 5, 5, 0x0700");
	asm ("mtspr 625, 5");

	asm ("lis	5, 0xfe00");
	asm ("ori 5, 5, 0x000a");
	asm ("rlwinm 5, 5, 0, 0, 31");
	asm ("mtspr 626, 5");

	asm ("lis	5, 0xfe00");
	asm ("ori 5, 5, 0x0015");
	asm ("mtspr 627, 5");

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");


	//# define 256MB TLB entry  2: 0xE0000000 - 0xEFFFFFFF for NOR      cache inhibited, guarded
	asm ("lis	5, 0x1002");
	asm ("ori 5, 5, 0");
	asm ("mtspr 624, 5");

	asm ("lis	5, 0xC000");
	asm ("ori 5, 5, 0x0900");
	asm ("mtspr 625, 5");

	asm ("lis	5, 0xe000");
	asm ("ori 5, 5, 0x000a");
	asm ("rlwinm 5, 5, 0, 0, 31");
	asm ("mtspr 626, 5");

	asm ("lis	5, 0xe000");
	asm ("ori 5, 5, 0x0015");
	asm ("mtspr 627, 5");

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");


	//# define   1GB TLB entry  3: 0x00000000 - 0x3FFFFFFF for DDR     cache inhibited
	asm ("lis	5, 0x1003");
	asm ("ori 5, 5, 0");
	asm ("mtspr 624, 5");

	asm ("lis	5, 0xC000");
	asm ("ori 5, 5, 0x0a00");
	asm ("mtspr 625, 5");

	asm ("lis	5, 0x0000");
	asm ("ori 5, 5, 0x0008");
	asm ("rlwinm 5, 5, 0, 0, 31");
	asm ("mtspr 626, 5");

	asm ("lis	5, 0x0000");
	asm ("ori 5, 5, 0x0015");
	asm ("mtspr 627, 5");

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");


	//# define   1GB TLB entry  4: 0x40000000 - 0x7FFFFFFF for DDR     cache inhibited
	asm ("lis	5, 0x1004");
	asm ("ori 5, 5, 0");
	asm ("mtspr 624, 5");

	asm ("lis	5, 0xC000");
	asm ("ori 5, 5, 0x0a00");
	asm ("mtspr 625, 5");

	asm ("lis	5, 0x4000");
	asm ("ori 5, 5, 0x0008");
	asm ("rlwinm 5, 5, 0, 0, 31");
	asm ("mtspr 626, 5");

	asm ("lis	5, 0x4000");
	asm ("ori 5, 5, 0x0015");
	asm ("mtspr 627, 5");

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");


	//# define   4MB TLB entry 5 : 0xF0000000 - 0xF03FFFFF for DCSR    cache inhibited, guarded
	asm ("lis	5, 0x1005");
	asm ("ori 5, 5, 0");
	asm ("mtspr 624, 5");

	asm ("lis	5, 0xC000");
	asm ("ori 5, 5, 0x0600");
	asm ("mtspr 625, 5");

	asm ("lis	5, 0xf000");
	asm ("ori 5, 5, 0x000a");
	asm ("rlwinm 5, 5, 0, 0, 31");
	asm ("mtspr 626, 5");

	asm ("lis	5, 0xf000");
	asm ("ori 5, 5, 0x0015");
	asm ("mtspr 627, 5");

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");


	//# define   4KB TLB entry 6: 0xFFDF0000 - 0xFFDF0FFF for CPLD   cache inhibited, guarded
	asm ("lis	5, 0x1006");
	asm ("ori 5, 5, 0");
	asm ("mtspr 624, 5");

	asm ("lis	5, 0xC000");
	asm ("ori 5, 5, 0x0100");
	asm ("mtspr 625, 5");

	asm ("lis	5, 0xffdf");
	asm ("ori 5, 5, 0x000a");
	asm ("rlwinm 5, 5, 0, 0, 31");
	asm ("mtspr 626, 5");

	asm ("lis	5, 0xffdf");
	asm ("ori 5, 5, 0x0015");
	asm ("mtspr 627, 5");

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");


	//# define   1MB TLB entry 7: 0xFF800000 - 0xFF8FFFFF for NAND    cache inhibited, guarded
	asm ("lis	5, 0x1007");
	asm ("ori 5, 5, 0");
	asm ("mtspr 624, 5");

	asm ("lis	5, 0xC000");
	asm ("ori 5, 5, 0x0500");
	asm ("mtspr 625, 5");

	asm ("lis	5, 0xff80");
	asm ("ori 5, 5, 0x000a");
	asm ("rlwinm 5, 5, 0, 0, 31");
	asm ("mtspr 626, 5");

	asm ("lis	5, 0xff80");
	asm ("ori 5, 5, 0x0015");
	asm ("mtspr 627, 5");

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");

	//
	//##################################################################################
	//# Local Access Windows Setup

	SET_GROUP_BASE(CCSRBAR + LAW_GROUP_OFFSET);
	//
	//# LAW0 to IFC (NOR) - 128M
	CCSR_SET_W(0xC00, 0x00000000);
	CCSR_SET_W(0xC04, 0xE8000000);
	CCSR_SET_W(0xC08, 0x81f0001A);

	//# LAW1 to IFC (CPLD) - 4KB
	CCSR_SET_W(0xC10, 0x00000000);
	CCSR_SET_W(0xC14, 0xFFDF0000);
	CCSR_SET_W(0xC18, 0x81F0000B);

	//# LAW2 to DCSR - 4MB
	CCSR_SET_W(0xC20, 0x00000000);
	CCSR_SET_W(0xC24, 0xF0000000);
	CCSR_SET_W(0xC28, 0x81D00015);

	//# LAW3 to IFC (NAND) - 1MB
	CCSR_SET_W(0xC30, 0x00000000);
	CCSR_SET_W(0xC34, 0xFF800000);
	CCSR_SET_W(0xC38, 0x81F00013);

	//# LAW15 to DDR (Memory Complex 1) - 2GB
	CCSR_SET_W(0xcf0, 0x00000000);
	CCSR_SET_W(0xcf4, 0x00000000);
	CCSR_SET_W(0xcf8, 0x8100001E);
	//
	//
	//##################################################################################
	//# DDR Controller Setup
	//

	SET_GROUP_BASE(CCSRBAR + DCFG_GROUP_OFFSET);

	// Get DDR type from DCFG_CCSR_PORSR2
	CCSR_GET_W(0x0004);
	asm ("mr %0, 19" : "=r"(ddr_type));

	// DDR4 - 0; DDR3 - 1
	ddr_type = (ddr_type & 0x20000000) >> 29;
	SET_GROUP_BASE(CCSRBAR + DDRC_GROUP_OFFSET);

	if (ddr_type == 0) {
		//# DDR_SDRAM_CFG
		CCSR_SET_W(0x110, 0x65044000);
		//# DDR_CS0_BNDS
		CCSR_SET_W(0x0000, 0x0000007f);
		//# DDR_CS1_BNDS
		CCSR_SET_W(0x008, 0x000000ff);
		//# DDR_CS0_CONFIG
		CCSR_SET_W(0x080, 0x80040322);
		//# DDR_CS1_CONFIG
		CCSR_SET_W(0x084, 0x80000322);
		//# DDR_CS0_CONFIG_2
		CCSR_SET_W(0x0c0, 0x00000000);
		//# DDR_CS1_CONFIG_2
		CCSR_SET_W(0x0c4, 0x00000000);
		//# DDR_TIMING_CFG_3
		CCSR_SET_W(0x100, 0x010c1000);
		//# DDR_TIMING_CFG_0
		CCSR_SET_W(0x104, 0x90550018);
		//# DDR_TIMING_CFG_1
		CCSR_SET_W(0x108, 0xbbb48c42);
		//# DDR_TIMING_CFG_2
		CCSR_SET_W(0x10c, 0x0048c111);
		//# DDR_SDRAM_CFG_2
		CCSR_SET_W(0x114, 0x00401111);
		//# DDR_SDRAM_MODE
		CCSR_SET_W(0x118, 0x03010210);
		//# DDR_SDRAM_MODE_2
		CCSR_SET_W(0x11c, 0x00000000);
		//# DDR_SDRAM_MD_CNTL
		CCSR_SET_W(0x120, 0x1600041f);
		//# DDR_SDRAM_INTERVAL
		CCSR_SET_W(0x124, 0x18600100);
		//# DDR_DATA_INIT
		CCSR_SET_W(0x128, 0xdeadbeef);
		//# DDR_SDRAM_CLK_CNTL
		CCSR_SET_W(0x130, 0x02000000);
		//# DDR_INIT_ADDR
		CCSR_SET_W(0x148, 0x00000000);
		//# DDR_INIT_EXT_ADDR
		CCSR_SET_DUP_W(0x14c);
		//# TIMING_CFG_4
		CCSR_SET_W(0x160, 0x00000002);
		//# TIMING_CFG_5
		CCSR_SET_W(0x164, 0x03401400);
		//# TIMING_CFG_6
		CCSR_SET_W(0x168, 0x00000000);
		//# TIMING_CFG_7
		CCSR_SET_W(0x16C, 0x13300000);
		//# TIMING_CFG_8
		CCSR_SET_W(0x250, 0x02115600);
		//# DDR_ZQ_CNTL
		CCSR_SET_W(0x170, 0x8a090705);
		//# DDR_WRLVL_CNTL
		CCSR_SET_W(0x174, 0xc675f606);
		//# DDR_SR_CNTR
		CCSR_SET_W(0x17c, 0x00000000);
		//# DDR_WRLVL_CNTL_2
		CCSR_SET_W(0x190, 0x07090a0d);
		//# DDR_WRLVL_CNTL_3
		CCSR_SET_W(0x194, 0x0d0f100a);
		//# DDR_SDRAM_MODE_3
		CCSR_SET_W(0x200, 0x00010210);
		//# DDR_SDRAM_MODE_4
		CCSR_SET_W(0x204, 0x00000000);
		//# DDR_SDRAM_MODE_5
		CCSR_SET_DUP_W(0x208);
		//# DDR_SDRAM_MODE_6
		CCSR_SET_DUP_W(0x20C);
		//# DDR_SDRAM_MODE_7
		CCSR_SET_DUP_W(0x210);
		//# DDR_SDRAM_MODE_8
		CCSR_SET_DUP_W(0x214);
		//# DDR_SDRAM_MODE_9
		CCSR_SET_W(0x220, 0x00000400);
		//# DDR_SDRAM_MODE_11
		CCSR_SET_DUP_W(0x228);
		//# DDR_SDRAM_MODE_10
		CCSR_SET_W(0x224, 0x04000000);
		//# DDR_SDRAM_MODE_12
		CCSR_SET_DUP_W(0x22c);
		//# DDR_DQ_MAP0
		CCSR_SET_W(0x400, 0x32c57554);
		//# DDR_DQ_MAP1
		CCSR_SET_W(0x404, 0xd4bb0bd4);
		//# DDR_DQ_MAP2
		CCSR_SET_W(0x408, 0x2ec2f554);
		//# DDR_DQ_MAP3
		CCSR_SET_W(0x40c, 0xd95d4001);
		//# DDR_DDRCDR_1
		CCSR_SET_W(0xb28, 0x80040000);
		//# DDRCDR_2
		CCSR_SET_W(0xb2c, 0x0000a181);
		//# DDR_ERR_DISABLE - DISABLE
		CCSR_SET_W(0xe44, 0x00000000);
		//# ERR_SBE
		CCSR_SET_W(0xe58, 0x00000000);

		//# delay before enable
		asm ("lis	5, 0x0000");
		asm ("ori	5, 5, 0x0fff");
		asm ("mtspr 9 ,5");
		asm ("wait_loop1_0:");
		asm ("bc    16, 0, wait_loop1_0 ");

		//# DDR_SDRAM_CFG
		CCSR_SET_W(0x110, 0xe5044000);

	} else if (ddr_type == 1) {

        SET_GROUP_BASE(CCSRBAR + DDRC_GROUP_OFFSET);
        //# DDR_SDRAM_CFG
        CCSR_SET_W(0x110, 0x67044000);
        //# DDR_CS0_BNDS
        CCSR_SET_W(0x000, 0x0000007f);
        //# DDR_CS1_BNDS
        CCSR_SET_W(0x008, 0x0000007f);
        //# DDR_CS0_CONFIG
        CCSR_SET_W(0x080, 0x80044302);
        //# DDR_CS1_CONFIG
        CCSR_SET_W(0x084, 0x80004302);
        //# DDR_CS0_CONFIG_2
        CCSR_SET_W(0x0c0, 0x00000000);
        //# DDR_CS1_CONFIG_2
        CCSR_SET_W(0x0c4, 0x00000000);
        //# DDR_TIMING_CFG_3
        CCSR_SET_W(0x100, 0x01071000);
        //# DDR_TIMING_CFG_0
        CCSR_SET_W(0x104, 0x50110004);
        //# DDR_TIMING_CFG_1
        CCSR_SET_W(0x108, 0xbcb58c56);
        //# DDR_TIMING_CFG_2
        CCSR_SET_W(0x10c, 0x0040c0d8);
        //# DDR_SDRAM_CFG_2
        CCSR_SET_W(0x114, 0x00401111);
        //# DDR_SDRAM_MODE
        CCSR_SET_W(0x118, 0x00441c70);
        //# DDR_SDRAM_MODE_2
        CCSR_SET_W(0x11c, 0x00980000);
        //# DDR_SDRAM_MODE_4
        CCSR_SET_DUP_W(0x204);
        //# DDR_SDRAM_MODE_6
        CCSR_SET_W(0x20C, 0x00000000);
        //# DDR_SDRAM_MODE_8
        CCSR_SET_DUP_W(0x214);
        //# DDR_SDRAM_MODE_3
        CCSR_SET_W(0x200, 0x00001c70);
        //# DDR_SDRAM_MODE_5
        CCSR_SET_W(0x208, 0x00000000);
        //# DDR_SDRAM_MODE_7
        CCSR_SET_DUP_W(0x210);
        //# DDR_SDRAM_MD_CNTL
        CCSR_SET_W(0x120, 0x00000000);
        //# DDR_SDRAM_INTERVAL
        CCSR_SET_W(0x124, 0x0c300100);
        //# DDR_DATA_INIT
        CCSR_SET_W(0x128, 0xdeadbeef);
        //# DDR_SDRAM_CLK_CNTL
        CCSR_SET_W(0x130, 0x02000000);
        //# DDR_INIT_ADDR
        CCSR_SET_W(0x148, 0x00000000);
        //# DDR_INIT_EXT_ADDR
        CCSR_SET_DUP_W(0x14c);
        //# TIMING_CFG_4
        CCSR_SET_W(0x160, 0x00000001);
        //# TIMING_CFG_5
        CCSR_SET_W(0x164, 0x04401400);
        //# DDR_ZQ_CNTL
        CCSR_SET_W(0x170, 0x89080600);
        //# DDR_WRLVL_CNTL
        CCSR_SET_W(0x174, 0xc675f607);
        //# DDR_SR_CNTR
        CCSR_SET_W(0x17c, 0x00000000);
        //# DDR_WRLVL_CNTL_2
        CCSR_SET_W(0x190, 0x0808090b);
        //# DDR_WRLVL_CNTL_3
        CCSR_SET_W(0x194, 0x0c0d0e0a);
        //# DDR_DDRCDR_1
        CCSR_SET_W(0xb28, 0x80000000);
        //# DDRCDR_2
        CCSR_SET_W(0xb2c, 0x00000000);
        //# DDR_ERR_DISABLE - DISABLE
        CCSR_SET_W(0xe44, 0x00000000);
        //# ERR_SBE
        CCSR_SET_W(0xe58, 0x00000000);

        //# delay before enable
        asm ("lis	5, 0x0000");
        asm ("ori	5, 5, 0x0fff");
        asm ("mtspr 9 ,5");
        asm ("wait_loop1_1:");
        asm ("bc    16, 0, wait_loop1_1 ");

        //# DDR_SDRAM_CFG
        CCSR_SET_W(0x110, 0xe7044000);
    }

	//# wait for DRAM data initialization
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x2ffd");
	asm ("mtspr 9 ,5");
	asm ("wait_loop2:");
	asm ("bc    16,0,wait_loop2 ");

	//# wait for D_INIT bits to clear
	asm ("xor 5, 5, 5");
	asm ("wait_loop3:");
	CCSR_GET_W(0x114);
	asm ("mr 5, 19");
	asm ("rlwinm 5, 5, 0, 27, 27");
	asm ("cmpwi 5, 0x0010");
	asm ("bt eq, wait_loop3");
}

void usr_init2() {

	//##################################################################################
	//# eSPI Controller Setup
	SET_GROUP_BASE(CCSRBAR + ESPI_GROUP_OFFSET);
	CCSR_SET_W(0x00, 0x80000403);
	CCSR_SET_W(0x08, 0x00000000);
	CCSR_SET_W(0x20, 0x2e170008);


	//##################################################################################
	//# IFC Controller Setup

	SET_GROUP_BASE(CCSRBAR + IFC_GROUP_OFFSET);

	// CS0 - NOR Flash, addr 0xE8000000, 128MB size, 16-bit NOR
	// CSPR_EXT
	CCSR_SET_W(0x00C, 0x00000000);
	// CSPR
	CCSR_SET_W(0x010, 0xE8000101);
	// AMASK
	CCSR_SET_W(0x0A0, 0xF8000000);
	// CSOR
	CCSR_SET_W(0x130, 0x0000000C);

	// IFC_FTIM0
	CCSR_SET_W(0x1C0, 0x40050005);
	// IFC_FTIM1
	CCSR_SET_W(0x1C4, 0x35001A13);
	// IFC_FTIM2
	CCSR_SET_W(0x1C8, 0x0410381C);
	// IFC_FTIM3
	CCSR_SET_W(0x1CC, 0x00000000);


	// CS1 - NAND Flash, addr 0xFF800000, 64KB size, 8-bit NAND
	// CSPR_EXT
	CCSR_SET_W(0x018, 0x00000000);
	// CSPR
	CCSR_SET_W(0x01C, 0xFF800083);
	// AMASK
	CCSR_SET_W(0x0AC, 0xFFFF0000);
	// CSOR
	CCSR_SET_W(0x13C, 0x0110A100);

	// IFC_FTIM0
	CCSR_SET_W(0x1F0, 0x0E18070A);
	// IFC_FTIM1
	CCSR_SET_W(0x1F4, 0x32390E18);
	// IFC_FTIM2
	CCSR_SET_W(0x1F8, 0x01E0501E);
	// IFC_FTIM3
	CCSR_SET_W(0x1FC, 0x00000000);


	// CS2 - CPLD,   addr 0xFFDF0000,   4KB size,  8-bit, GPCM, Valid
	// CSPR_EXT
	CCSR_SET_W(0x024, 0x00000000);
	// CSPR
	CCSR_SET_W(0x028, 0xFFDF0085);
	// AMASK
	CCSR_SET_W(0x0B8, 0xFFFF0000);
	// CSOR
	CCSR_SET_W(0x148, 0x00000000);

	// IFC_FTIM0
	CCSR_SET_W(0x220, 0xE00E000E);
	// IFC_FTIM1
	CCSR_SET_W(0x224, 0x0E001F00);
	// IFC_FTIM2
	CCSR_SET_W(0x228, 0x0E00001F);
	// IFC_FTIM3
	CCSR_SET_W(0x22C, 0x00000000);
	//
	//
	//##################################################################################
	//# Interrupt vectors initialization
	//
	//
	//# interrupt vectors in RAM at 0x00000000
	//writereg	IVPR 0x00000000 	# IVPR (default reset value)
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0000");
	asm ("mtspr 63, 5");
	//
	//# interrupt vector offset registers
	//writespr	400 0x00000100	# IVOR0 - critical input
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0100");
	asm ("mtspr 400, 5");
	//writespr	401 0x00000200	# IVOR1 - machine check
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0200");
	asm ("mtspr 401, 5");
	//writespr	402 0x00000300	# IVOR2 - data storage
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0300");
	asm ("mtspr 402, 5");
	//writespr	403 0x00000400	# IVOR3 - instruction storage
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0400");
	asm ("mtspr 403, 5");
	//writespr	404 0x00000500	# IVOR4 - external input
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0500");
	asm ("mtspr 404, 5");
	//writespr	405 0x00000600	# IVOR5 - alignment
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0600");
	asm ("mtspr 405, 5");
	//writespr	406 0x00000700	# IVOR6 - program
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0700");
	asm ("mtspr 406, 5");
	//writespr	407 0x00000800	# IVOR7 - floating point unavailable
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0800");
	asm ("mtspr 407, 5");
	//writespr	408 0x00000c00	# IVOR8 - system call
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0c00");
	asm ("mtspr 408, 5");
	//writespr	410 0x00000900	# IVOR10 - decrementer
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0900");
	asm ("mtspr 410, 5");
	//writespr	411 0x00000f00	# IVOR11 - fixed-interval timer interrupt
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0f00");
	asm ("mtspr 411, 5");
	//writespr	412 0x00000b00	# IVOR12 - watchdog timer interrupt
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x0b00");
	asm ("mtspr 412, 5");
	//writespr	413 0x00001100	# IVOR13 - data TLB errror
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x1100");
	asm ("mtspr 413, 5");
	//writespr	414 0x00001000	# IVOR14 - instruction TLB error
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x1000");
	asm ("mtspr 414, 5");
	//writespr	415 0x00001500	# IVOR15 - debug
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x1500");
	asm ("mtspr 415, 5");
	//writespr	531 0x00001900	# IVOR35 - performance monitor
	asm ("lis	5, 0x0000");
	asm ("ori	5, 5, 0x1900");
	asm ("mtspr 531, 5");
	//
	// Enable branch prediction
	//writespr	1013 0x01400201
	asm ("lis	5, 0x0140");
	asm ("ori	5, 5, 0x0201");
	asm ("mtspr 1013, 5");

	//
	//##################################################################################
	//# Debugger settings
	//
	//# enable machine check
	//writereg 	HID0 	0x80000000
	asm ("lis	5, 0x8000");
	asm ("ori	5, 5, 0x0000");
	asm ("mtspr 1008, 5");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Copy the exception vectors from ROM to RAM
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	asm ("__copy_vectors:");

	asm ("xor 		3, 3, 3");
	asm ("oris		3, 3, gInterruptVectorTable@h");
	asm ("ori		3, 3, gInterruptVectorTable@l");
	asm ("subi		3, 3, 0x0004");

	asm ("xor		4, 4, 4");
	asm ("oris		4, 4, gInterruptVectorTableEnd@h");
	asm ("ori		4, 4, gInterruptVectorTableEnd@l");

	asm ("xor  		5, 5, 5");
	asm ("subi		5, 5, 0x0004");

	asm ("loop:");
	asm ("	lwzu	6, 4(3)");
	asm ("	stwu	6, 4(5)");

	asm ("	cmpw	3, 4");
	asm ("	blt		loop");

	asm ("msync");
	asm ("isync");
}
