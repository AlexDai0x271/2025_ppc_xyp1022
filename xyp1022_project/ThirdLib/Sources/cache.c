/*
 * cache.c
 *
 *  Created on: 2020-10-23
 *      Author: ccore
 */
#include <stdio.h>
//#include "gpio.h"
#include "iomacros.h"
#include "e500mc_asm.h"
#include "mmu.h"
#include "e500mc.h"
#include "cache.h"
#include "common.h"
#include "axidma.h"
#include "ppctimer.h"

#define TARGET_SPACE			0x50000000

#define JTAG_SNOOP_DISABLE		0xeb009058ull

extern  set_e0_tlb(UINT32 r3,UINT32 r4,UINT32 r5,UINT32 r6,UINT32 r7,UINT32 r8);

void disable_L1_cache()
{
	unsigned int val;
	/*
	 * CE:0 L1 Data Cache disable.
	 */
	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);
	
	val=0x0;
	MTSPR(L1CSR0, val);
	MFSPR(val, L1CSR0);
	if(val!=0x00)
		while(1);

	/*
	 * ICE:0 L1 Instruction Cache disable.
	 */
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);
	val=0x0;
	MTSPR(L1CSR1, val);
	MFSPR(val, L1CSR1);
	if(val!=0x00)
		while(1);	
}

void disable_L2_cache()
{
	unsigned int val;
	/*
	 * L2E:0 L2 Cache disable.
	 */
	//MFSPR(val, L2CSR0);
	//if(val!=(0x01<<31))
	//	while(1);
	
	val=0x0;
	delay(0x1000);
	MTSPR(L2CSR0, 0);
	delay(0x1000);
	MFSPR(val, L2CSR0);
	if(val!=0x00)
	{
		delay(0x1000);
		MTSPR(L2CSR0, 0);
		MFSPR(val, L2CSR0);
	}
}

void L1_cache_reg_test()
{
	unsigned int tmp=0, val;

	MFMSR(tmp);
	tmp &= 0xEFFFBFFF;
	MTMSR(tmp);
	MFMSR(tmp);			//make sure into hypervisor state
	ISYNC;
	
	MFSPR(val, L1CFG0);
	if(val!=0x00b83820)
		while(1);
	/*
	 * ICFISWA:0, Direct cache flush available. 0 indicates unavailable
	 * ICBSIZE:1, Instruction cache block size. 1 indicates 64 bytes
	 * ICREPL:1, Instruction cache replacement policy. 1 indicates pseudo-LRU policy
	 * ICLA:1, Instruction cache locking available. 1 indicates available
	 * ICPA:1, Cache parity available. 1 indicates available
	 * ICNWAY:7, Instruction cache number of ways. 7 indicates eight ways
	 * ICSIZE:32, indicates Instruction cache size 32 Kbytes
	 */
	MFSPR(val, L1CFG1);
	if(val!=0x00b83820)
		while(1);
	
	/*
	 * CE:1 (Data) Cache enable.
	 */
	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);
	
	/*
	 * ICE:1 (Instruction) Cache enable.
	 */
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);

	MFSPR(val, L1CSR2);
	if(val!=0x00)
		while(1);

}

void L2_cache_reg_test()
{
	unsigned int tmp=0, val;

	MFMSR(tmp);
	tmp &= 0xEFFFBFFF;
	MTMSR(tmp);
	MFMSR(tmp);			//make sure into hypervisor state
	ISYNC;
		
	/*
	 * L2CTEHA:1, L2 cache tags error handling available. 1 indicates parity detection.
	 * L2CDEHA:0b11 L2 cache data error handling available. 0b11 indicates both parity and ECC correction available
	 * L2CIDPA:1, Cache instruction and data partitioning available. 1 indicates available.
	 * L2CBSIZE:1, Cache line size. 1 indicates 64 bytes
	 * L2CREPL: Cache default replacement policy. This is the default line replacement policy at power-on-reset. If an
		implementation allows software to change the replacement policy it is not reflected here.1 indicates
		pseudo-LRU.
	 * L2CLA:1 Cache line locking available. 1 indicates available.
	 * L2CNWAY:7, Number of cache ways. 7 indicates 8 ways.
	 * L2CSIZE:4, Cache size as a multiple of 64 Kbytes. 4 indicates 256-Kbyte cache.
	 */
	MFSPR(val, L2CFG0);
	if(val!=0x3cb1c004)
		while(1);
	
	/*
	* L2E:1 L2 Cache enable.
	*/
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);
}

void L1_i_cache_effective_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-icahe whether effective:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR, use DCBF to make sure the instructions have been writed 
	 * run, then can stop at {for(num=0; num<0x40; num+=4) STW(0, tgt_addr+num);}
	 * now clean the ddr by code(CPU), run can printf
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable
	CACHE_DEBUG("L1 i cache test begin:\n");

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;
	MTSPR(CTR, tgt_addr);
	BCTRL;
	
	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;

	CACHE_DEBUG("#### if print here, L1 icache is ok! ####\n");
}

/*
 * 1. Execute 511groups NOP + 1group M, fill all 32K ICache
 * 2. Clear the memory which stored 511groups NOP + 1group M 
 * 3. Execute 511groups NOP + 1group M again
 * 4. If CPU can return by instruction_blr, indicate 32K Icache is valid
 */
void L1_i_cache_effective_test_32K()
{
	unsigned int i, val, d, num, data, size, error_count=0;	
	unsigned int tgt_addr_A, tgt_addr_B, tgt_set;

	/*
	 * test-icahe whether effective:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR, use DCBF to make sure the instructions have been writed 
	 * run, then can stop at {for(num=0; num<0x40; num+=4) STW(0, tgt_addr+num);}
	 * now clean the ddr by code(CPU), run can printf
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, 0xeb047058);		//JTAG snoop disable
	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M, WRX
	set_e0_tlb(0x103c0000,0xc0000500,0x0000000a,0x0000000f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x0000_0000 to 0x0010_0000, va from 0x0000_0000 to 0x0010_0000, invalid cache, 1M, Only WR

	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	i=64*0x40;										//32K I-cache has 64 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr_A = TARGET_SPACE;						//start address is 0x50000000
	tgt_addr_B= 0x00000000;
	
	/*511 groups NOPs, addr: 0x50000000~0x50007FC0*/
	for(num=0; num<0x40*0x1ff; num+=4)
		STW(0x60000000, tgt_addr_A+num);				
	
	/*15 NOP + 1 BLR, addr: 0x50007FC0~0x50008000*/
	for(num=0x40*0x1ff; num<0x40*0x200-4; num+=4)
		STW(0x60000000, tgt_addr_A+num);						
	STW(0x4E800020, tgt_addr_A+0x40*0x1ff+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;
	
	/* data check1: data at 0x60000000~0x60008000 should be different from 0x50000000~0x50008000*/
	for(num=0; num<0x40*0x200-4; num+=4)
	{
		LDW(data, tgt_addr_B+num);
		if(data == 0x60000000)
			while(1);
	}
	LDW(data, tgt_addr_B+0x40*0x1ff+0x3C);
	if(data == 0x4E800020)
		while(1);		
	ISYNC;
	
	/*Flush data to PA: 0x1000~0x9000*/
	for(num=0; num<0x40*0x200; num+=0x40)
	{
		DCBF(tgt_addr_A+num);
		ISYNC;
	}
	
	/* data check2: data at 0x60000000~0x60008000 should be same with 0x50000000~0x50008000*/
	for(num=0; num<0x40*0x200-4; num+=4)
	{
		LDW(data, tgt_addr_B+num);
		if(data != 0x60000000)
			while(1);
	}
	LDW(data, tgt_addr_B+0x40*0x1ff+0x3C);
	if(data != 0x4E800020)
		while(1);		
	ISYNC;

	/* execute 32K instructions*/
	MTSPR(CTR, tgt_addr_A);
	BCTRL;
	
	/*clear data at 0x50000000~0x50008000*/
	for(num=0; num<0x40*0x200; num+=4)
		STW(0, tgt_addr_A+num);
	/*Flush data to PA: 0x0000~0x8000*/
	for(num=0; num<0x40*0x200; num+=0x40)
	{
		DCBF(tgt_addr_A+num);
		ISYNC;
	}
	
	for(num=0; num<0x40*0x200; num+=4)
	{
		LDW(data, tgt_addr_A+num);
		if(data != 0x00000000)
			while(1);
	}

	for(num=0; num<0x40*0x200; num+=4)
	{
		LDW(data, tgt_addr_B+num);
		if(data != 0x00000000)
			while(1);
	}
	
	/*test 32K ICache*/
	MTSPR(CTR, tgt_addr_A);
	BCTRL;
		
	/*Instruct should back here*/
	CACHE_DEBUG("should print here !:\n");
}


void L1_i_cache_icbi_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-icbi:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR, use DCBF to make sure the instructions have been writed 
	 * run, then can stop at {for(num=0; num<0x40; num+=4) STW(0, tgt_addr+num);}
	 * now clean the ddr by code(CPU), run can printf
	 * invalid specified cacheline by icbi(addr), test whether it effective
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
	//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;
	MTSPR(CTR, tgt_addr);
	BCTRL;
	
	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;
		
	CACHE_DEBUG("should print here !:\n");
		
	/* ICBI test */
	CACHE_DEBUG("TEST _ICBI:\n");
	
	MFSPR(val, L1CFG1);
	if(val!=0x00b83820)
		while(1);

	delay(0x1000);
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);

	ICBI(tgt_addr); // invalid the target L1 I-cacheline
	ISYNC;

	MFSPR(val, L1CFG1);
	if(val!=0x00b83820)
		while(1);

	delay(0x1000);
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);
	
	MTSPR(CTR, tgt_addr);
	BCTRL;
	
	CACHE_DEBUG("if print here, it's wrong");
}

void L1_i_cache_icbt_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-icbt:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * L1 icbt should be treat as NOP, so it not effective
	 * should goto expection
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;
	/* don't implement the instructions by CPU, but by ICBT */
//	MTSPR(CTR, tgt_addr);
//	BCTRL;

	/* L1 ICBT test, in fact, L1 icbt is treat as NOP, CT now is 0*/
	CACHE_DEBUG("TEST _ICBT:\n");
	ICBT(tgt_addr);
	ISYNC;
	
	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;
		
	CACHE_DEBUG("if print here, it's wrong");
}

void L1_i_cache_icbtls_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-icbtls:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * L1 icbtls should pefetech and lock every cache block, total 8way * 64set = 512 cache blocks
	 * should printf the finial  CACHE_DEBUG
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
//	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 61 from 0xe000_0000 to 0xe004_0000, size 512K

	i=64*0x40;							//32K I-cache has 64 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;
	/* don't implement the instructions by CPU, but by ICBTLS */
//	MTSPR(CTR, tgt_addr);
//	BCTRL;
	
	/* L1 ICBTLS test, should work as refetch and lock set*/
	CACHE_DEBUG("TEST _ICBTLS:\n");
//	MTSPR(MSRP, 0x04000000);
//	ISYNC;
	ICBTLS(tgt_addr);
	ISYNC;
	
	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;
		
	MFSPR(val, L1CFG1);
	if(val!=0x00b83820)
		while(1);
	
	/*
	 * CE:1 (Data) Cache enable.
	 */
	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);
	
	/*
	 * ICE:1 (Instruction) Cache enable.
	 * ICUL:0 is right;	1 is meaning can't be locked
	 * ICLO:0 is right; 1 is meaning lock overflow condition
	 * so L1CSR1 should be 1
	 */
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);

	MFSPR(val, L1CSR2);
	if(val!=0x00)
		while(1);

	CACHE_DEBUG("if print here, it's right!");
}

void L1_i_cache_icbtls2_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-icbtls:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * L1 icbtls should pefetech and lock set
	 * should printf the finial  CACHE_DEBUG
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable
	
	CACHE_DEBUG("L1 i cache icbtls test begin:\n");

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight ways with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	/* ICBTLS 8 ways of instruction and lock*/
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
		ISYNC;
		DCBF(tgt_addr);
		ISYNC;
		
		/* don't implement the instructions by CPU, but by ICBTLS */
//		MTSPR(CTR, tgt_addr);
//		BCTRL;
		/* L1 ICBTLS test, should work as pefetech and lock set*/
		CACHE_DEBUG("L1 ICBTLS pefetech and lock %xth way\n", i);
		ICBTLS(tgt_addr);
		ISYNC;

		delay(0x1000);
		MFSPR(val, L1CSR1);
		if(val!=0x01)
			while(1);
	}
	
	i=64*0x40;							//32K I-cache has 64 sets of eight ways with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		for(num=0; num<0x40; num+=4)
			STW(0, tgt_addr+num);
		DCBF(tgt_addr);
		ISYNC;	
	}
	
	tgt_addr = 0x50001000;			//0x50001000
	MTSPR(CTR, tgt_addr);
	BCTRL;

	/* ICBTLS the 9th way of instruction and lock*/
	tgt_addr = 0x50009000;			//0x50009000
	for(num=0; num<0x20-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
	STW(0x4E800020, tgt_addr+0x1C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;
	/* don't implement the instructions by CPU, but by ICBTLS */
//	MTSPR(CTR, tgt_addr);
//		CTRL;
	/* L1 ICBTLS test, should work as refetch and lock set*/
	CACHE_DEBUG("L1 ICBTLS the 9th way of instruction and lock \n");
	ICBTLS(tgt_addr);
	ISYNC;
	
	/* ICLO shuold be set, indicate a lock overflow condition was encountered in the cache */
	delay(0x1000);
	MFSPR(val, L1CSR1);
	if(val!=0x201)
		while(1);
	else
		CACHE_DEBUG("L1 ICLO is set\n");

	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;

	CACHE_DEBUG("if print here, it's right!");
}


void L1_i_cache_icbtls3_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-icbtls:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * L1 icbtls should pefetech and lock set
	 * should printf the finial  CACHE_DEBUG
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	/* invaldate L2 cache */
	disable_L2_cache();

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight ways with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	/* ICBTLS 8 ways of instruction and lock*/
	for(i=0; i<8*0x40; i++, tgt_addr+=0x40){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
		ISYNC;
		DCBF(tgt_addr);
		ISYNC;
		
		/* don't implement the instructions by CPU, but by ICBTLS */
//		MTSPR(CTR, tgt_addr);
//		BCTRL;
		/* L1 ICBTLS test, should work as refetch and lock set*/
		CACHE_DEBUG("TEST _ICBTLS:\n");
		ICBTLS(tgt_addr);
		ISYNC;

		delay(0x1000);
		MFSPR(val, L1CSR1);
		if(val!=0x01)
			while(1);
	}
	
	i=64*0x40;							//32K I-cache has 64 sets of eight ways with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		for(num=0; num<0x40; num+=4)
			STW(0, tgt_addr+num);
		DCBF(tgt_addr);
		ISYNC;	
	}
	
	tgt_addr = 0x50001000;			//0x50001000
	MTSPR(CTR, tgt_addr);
	BCTRL;

	/* ICBTLS the 9th way of instruction and lock*/
	tgt_addr = 0x50009000;			//0x50009000
	for(num=0; num<0x20-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
	STW(0x4E800020, tgt_addr+0x1C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;
	/* don't implement the instructions by CPU, but by ICBTLS */
//	MTSPR(CTR, tgt_addr);
//		CTRL;
	/* L1 ICBTLS test, should work as refetch and lock set*/
	CACHE_DEBUG("TEST _ICBTLS:\n");
	ICBTLS(tgt_addr);
	ISYNC;
	
	/* ICLO shuold be set, indicate a lock overflow condition was encountered in the cache */
	delay(0x1000);
	MFSPR(val, L1CSR1);
	if(val!=0x201)
		while(1);

	/* lock all 8way*64set cachelint, and disable L2, so the former instructions will not arrive icache*/
	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;

	CACHE_DEBUG("if print here, it's wrong!");
}

void L1_i_cache_icblc_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-icblc:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight ways with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	/* ICBTLS 8 ways of instruction and lock*/
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
		ISYNC;
		DCBF(tgt_addr);
		ISYNC;
		
		/* L1 ICBTLS test, should work as refetch and lock set*/
		CACHE_DEBUG("TEST _ICBLC:\n");
		ICBTLS(tgt_addr);
		ISYNC;

		/* should be only ICE */
		delay(0x1000);
		MFSPR(val, L1CSR1);
		if(val!=0x01)
			while(1);
	}
	
	/* clear PA memory data */
	tgt_addr = 0x50001000;			//0x50001000
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		for(num=0; num<0x40; num+=4)
			STW(0, tgt_addr+num);
		DCBF(tgt_addr);
		ISYNC;	
	}
	
	/* implement the first cacheline */
	tgt_addr = 0x50001000;			//0x50001000
	MTSPR(CTR, tgt_addr);
	BCTRL;

	/* icblc test here, unlock 8 ways */
	tgt_addr = 0x50001000;			//0x50001000
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		ICBLC(tgt_addr); // ulock the target L1 I-cacheline
		ISYNC;
	}
	
	/* ICBTLS the 9th way of instruction and lock*/
	tgt_addr = 0x50009000;			//0x50009000
	for(num=0; num<0x20-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
	STW(0x4E800020, tgt_addr+0x1C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;

	/* L1 ICBTLS test, should work as refetch and lock set*/
	CACHE_DEBUG("TEST _ICBLC:\n");
	ICBTLS(tgt_addr);
	ISYNC;
	
	/* should be only ICE */
	delay(0x1000);
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);

	/* implement the 9th cacheline */
	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;

	CACHE_DEBUG("if print here, it's right!");
}

void L1_i_cache_invalid_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test--L1CSR1:ICFI
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * L1 cache fill by 0x60000000 and 0x4E800020,
	 * invalidate Instruction cache flash(line) by write ICFI bit at L1CSR1
	 * run should goto illegial instruction error
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;
	
	/*
	 * ICE:1 (Instruction) Cache enable.
	 * ICUL:0 is right;	1 is meaning can't be locked
	 * ICLO:0 is right; 1 is meaning lock overflow condition
	 * so L1CSR1 should be 1
	 */
	 delay(0x1000);
	 MFSPR(val, L1CSR1);
	 if(val!=0x01)
		 while(1);

	 delay(0x1000);
	 MFSPR(val, L1CFG1);
	 if(val!=0x00b83820)
		 while(1);

	/*Instruction cache flash(line) invalidate*/
	MTSPR(L1CSR1, (ICE | ICFI));
	
	delay(0x1000);
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);

	delay(0x1000);
	MFSPR(val, L1CFG1);
	if(val!=0x00b83820)
		while(1);

	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;
		
	CACHE_DEBUG("if print here, it's wrong");
}

void L1_i_cache_unlock_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-L1CSR1 bit55:ICLFC:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * should printf the finial  CACHE_DEBUG
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight ways with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	/* ICBTLS 8 ways of instruction and lock*/
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_addr+0x3C);		//0x4E800020 is instruction: blr
		ISYNC;
		DCBF(tgt_addr);
		ISYNC;
		
		/* L1 ICBTLS test, should work as refetch and lock set*/
		CACHE_DEBUG("TEST _ICBT:\n");
		ICBTLS(tgt_addr);
		ISYNC;

		/* should be only ICE */
		delay(0x1000);
		MFSPR(val, L1CSR1);
		if(val!=0x01)
			while(1);
	}
	
	/* clear PA memory data */
	tgt_addr = 0x50001000;			//0x50001000
	for(i=0; i<8; i++, tgt_addr+=0x1000){
		for(num=0; num<0x40; num+=4)
			STW(0, tgt_addr+num);
		DCBF(tgt_addr);
		ISYNC;	
	}
	
	/* implement the first cacheline */
	tgt_addr = 0x50001000;			//0x50001000
	MTSPR(CTR, tgt_addr);
	BCTRL;

	/* ICLFC test here, unlock all 512 bolcks */
	delay(0x1000);
	MTSPR(L1CSR1, 0x101);
	
	/* ICBTLS the 9th way of instruction and lock*/
	tgt_addr = 0x50009000;			//0x50009000
	for(num=0; num<0x20-4; num+=4)
		STW(0x60000000, tgt_addr+num);	//0x60000000 is instruction: NOP	
	STW(0x4E800020, tgt_addr+0x1C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr);
	ISYNC;

	/* L1 ICBTLS test, should work as refetch and lock set*/
	CACHE_DEBUG("TEST ICLFC:\n");
	ICBTLS(tgt_addr);
	ISYNC;
	
	/* should be only ICE */
	delay(0x1000);
	MFSPR(val, L1CSR1);
	if(val!=0x01)
		while(1);

	/* implement the 9th cacheline */
	for(num=0; num<0x40; num+=4)
		STW(0, tgt_addr+num);
	DCBF(tgt_addr);
	ISYNC;	
	MTSPR(CTR, tgt_addr);
	BCTRL;

	CACHE_DEBUG("if print here, it's right!");
}

void L1_i_cache_LRU_test()
{
	unsigned int i, j, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-icblc:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	i=64*0x40;							//32K I-cache has 64 sets of eight ways with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000
#if 0
	//prepare test data, way0
	for(i=0;i<32;i=i+4)
		STW((0x11111111),(tgt_addr+i));
	ISYNC;
	//pre-fetch and lock at way0
 	ICBTLS(tgt_addr); // should be stored in way0
	ISYNC;

	//prepare test data, way1
	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, tgt_addr+0x2000+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, tgt_addr+0x2000+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;
	DCBF(tgt_addr+0x2000);
	ISYNC;
	/* implement the instructions by CPU, not locked */
	MTSPR(CTR, tgt_addr+0x2000);
	BCTRL;

	// prepare test data, way2~way7, and one more set for use,
	for(j=2;j<9;j++){ // write 4 cacheline(4*8 words)
		for(i=0;i<32;i=i+4){
			STW((0x11111111<<j),(tgt_addr+j*0x2000+i));
			ISYNC;
		}
	}
	
	//implement the instructions at way1 
//	MTSPR(CTR, tgt_addr+0x2000);
//	BCTRL;
//	CACHE_DEBUG("if print here, it's right!");
	
	//pre-fetch and lock way2~way8, and then, pre-fetch and lock one more way, test whether it's at way1
	for(i=2;i<9;i++){
		ICBTLS(tgt_addr+i*0x2000); // should be stored in way1
		ISYNC;
	}
#endif
	
	// prepare test data, way0~way7
	for(j=0;j<8;j++){ // write 4 cacheline(4*8 words)
		for(i=0;i<32;i=i+4){
			STW((0x11111111<<j),(tgt_addr+j*0x2000+i));
			ISYNC;
		}
	}

	//pre-fetch and lock way0~way7
	for(i=0;i<8;i++){
		ICBTLS(tgt_addr+i*0x2000); // should be stored in way1
		ISYNC;
	}
	
	// icblc way1
	ICBLC(tgt_addr+0x2000); // ulock the target L1 I-cacheline
	ISYNC;

	//pre-fetch and lock way1, data at 0x50011000
	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, tgt_addr+0x10000+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, tgt_addr+0x10000+0x3C);		//0x4E800020 is instruction: blr

	ICBTLS(tgt_addr+0x10000); // should be stored in way1
	ISYNC;
	
	//in fact, here should be read as 0x60000000 at cache line. but due to can't read cache line, it's wrong to read the VA data. now VA data != WAY1 Cache line data
	MTSPR(CTR, tgt_addr+0x2000);		//test icache at 0x50003000
	BCTRL;

	CACHE_DEBUG("if print here, it's wrong!");
}

void L1_d_cache_effective_test()
{
	unsigned int i, val;	

	/*
	 * test-dcahe whether effective:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * should printf the finial  CACHE_DEBUG
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable
	
	/*
	 * test sram and ddr
	 * for example--ddr test: set two ddr tlbs which use two different va(0x500000000 and 0x00000000) but mapping the same pa(0x00000000)
	 * the va(0x50000000) valid cache, but va(0x00000000) invalid cache
	 * write data to va(0x50000000), then read va(0x00000000), va(0x00000000) should be different from va(0x50000000)
	 * after Data Flush by DCBF, va(0x00000000) should be same with va(0x50000000)
	 */
	
	set_e0_tlb(0x10190000, 0xc0000900, 0x50000004, 0x0000003f, 0, 0);		//set another ddr tlb which valid cache
	set_e0_tlb(0x101a0000, 0xc0000400, 0x60000004, 0xe004003f, 0, 0x4);		//set another sram tlb which valid cache
	set_e0_tlb(0x101b0000, 0xc0000400, 0x7000000a, 0xe004003f, 0, 0x4);		//set another sram tlb which invalid cache

	//write to ddr epn which enable cache
	for(i=0; i<0x8000; i+=4)
		STW(i+0x11, i+0x50000000);	
	
	//read from ddr epn which diable cache, should different from 0x50000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i);
		if(val == (i+0x11))
			while(1);
	}
	
	//flush dcache
	for(i=0; i<0x8000; i+=0x40){
		DCBF(0x50000000+i);
		ISYNC;
	}
	
	//read from ddr epn which diable cache, should same with 0x50000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i);
		if(val != (i+0x11))
			while(1);
	}


	
	//write to sram epn which enable cache
	for(i=0; i<0x8000; i+=4)
		STW(i+0x22, i+0x60000000);

	//read from sram epn which diable cache, should different from 0x60000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i+0x70000000);
		if(val == (i+0x22))
			while(1);
	}
	
	//flush cache
	for(i=0; i<0x8000; i+=0x40){
		DCBF(0x60000000+i);
		ISYNC;
	}
	
	//read from sram epn which diable cache, should same with 0x60000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i+0x70000000);
		if(val != (i+0x22))
			while(1);
	}
}

void L1_d_cache_dcbi_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-dcbi:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000000,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	/*
	 * test 1:no dcbi, normal dcache test, just flush cache by dcbf
	 */
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x5a5a5a5a, tgt_addr+i);	
	
	//read from ddr epn 0x1000 which diable cache, should different from 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val == 0x5a5a5a5a)
			while(1);
	}

	//flush dcache, meanwhile invalidate 0x50001000 cacheline by dcbf
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x1000 which diable cache, should same with 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val != 0x5a5a5a5a)
			while(1);
	}
	CACHE_DEBUG("if print here, it's right");

	MFSPR(val, L1CFG0);
	if(val!=0x00b83820)
		while(1);

	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);
		
	/*
	 * test 2: dcbi_1, not marked as Memory Coherence Required, (WIMGE=0bx00xx), dcbi should be effiective
	 */
	/* DCBI test 1*/
	CACHE_DEBUG("TEST _ICBI:\n");
	tgt_addr = TARGET_SPACE+0x11000;			//0x500011000
	
	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x6b6b6b6b, tgt_addr+i);	
	
	//read from ddr epn 0x11000 which diable cache, should different from 0x50011000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val == 0x6b6b6b6b)
			while(1);
	}
	
	//invalid cache line from 0x50012000 to 0x50014000
	for(i=0x100; i<0x300; i+=0x40){
		DCBI(tgt_addr+i); 			// invalid the target L1 D-cacheline
		ISYNC;
	}
	
	//flush dcache
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x11000 which diable cache, from 0x12000 to 0x14000 should different , other should be same
	for(i=0; i<0x100; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x6b6b6b6b)		//should be same
			while(1);
	}
	
	for(i=0x100; i<0x300; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val == 0x6b6b6b6b)		//should be different
			while(1);
	}

	for(i=0x300; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x6b6b6b6b)		//should be same
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");	
	
	/*
	 * test 3: dcbi_2, marked as Memory Coherence Required, (WIMGE=0bx01xx), dcbi should be not effiective
	 */
	/* DCBI test 2*/
	set_e0_tlb(0x103c0000,0xc0000500,0x60000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x6000_0000 to 0x6010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	CACHE_DEBUG("TEST _ICBI:\n");
	tgt_addr = TARGET_SPACE+0x10021000;			//0x600021000
	
	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x6b6b6b6b, tgt_addr+i);	
	
	//read from ddr epn 0x11000 which diable cache, should different from 0x50011000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x21000+i);
		if(val == 0x6b6b6b6b)
			while(1);
	}
	
	//invalid cache line from 0x50012000 to 0x50014000
	for(i=0x100; i<0x300; i+=0x40){
		DCBI(tgt_addr+i); 			// invalid the target L1 D-cacheline
		ISYNC;
	}
	
	//flush dcache
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x11000 which diable cache, from 0x12000 to 0x14000 should different , other should be same
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x21000+i);
		if(val != 0x6b6b6b6b)		//should be same
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");	
}

void L1_d_cache_dcbz_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-dcbz:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000000,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/*
	 * no dcbz, normal dcache test, just flush cache by dcbf
	 */
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x5a5a5a5a, tgt_addr+i);	
	
	//read from ddr epn 0x1000 which diable cache, should different from 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val == 0x5a5a5a5a)
			while(1);
	}

	//flush dcache, meanwhile invalidate 0x50001000 cacheline by dcbf
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x1000 which diable cache, should same with 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val != 0x5a5a5a5a)
			while(1);
	}
	CACHE_DEBUG("if print here, it's right");

	MFSPR(val, L1CFG0);
	if(val!=0x00b83820)
		while(1);

	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);
		
	/*
	 * dcbz test
	 */
	CACHE_DEBUG("TEST _DCBZ:\n");
	tgt_addr = TARGET_SPACE+0x11000;			//0x500011000
	
	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x6b6b6b6b, tgt_addr+i);	
	
	//read from ddr epn 0x11000 which diable cache, should different from 0x50011000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val == 0x6b6b6b6b)
			while(1);
	}
	
	//invalid cache line from 0x50012000 to 0x50014000
	for(i=0x100; i<0x300; i+=0x40){
		DCBZ(tgt_addr+i); 			// Set to the target L1 D-cacheline
		ISYNC;
	}
	
	//flush dcache
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x11000 which diable cache, from 0x12000 to 0x14000 should different , other should be same
	for(i=0; i<0x100; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x6b6b6b6b)		//should be same
			while(1);
	}
	
	for(i=0x100; i<0x300; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != (0))				//should be different
			while(1);
	}

	for(i=0x300; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x6b6b6b6b)		//should be same
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");	
}

void L1_d_cache_dcbst_test()
{
	unsigned int i, val;	
	
	/*
	 * dcbst is same as dsbf
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * should printf the finial  CACHE_DEBUG
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable
	
	/*
	 * test sram and ddr
	 * for example--ddr test: set two ddr tlbs which use two different va(0x500000000 and 0x00000000) but mapping the same pa(0x00000000)
	 * the va(0x50000000) valid cache, but va(0x00000000) invalid cache
	 * write data to va(0x50000000), then read va(0x00000000), va(0x00000000) should be different from va(0x50000000)
	 * after Data Flush by DCBF,  va(0x00000000) should be same with va(0x50000000)
	 */
	
//	set_e0_tlb(0x103f0000, 0xc0000900, 0x50000004, 0x0000003f, 0, 0);		//set another ddr tlb which valid cache
	set_e0_tlb(0x103d0000, 0xc0000400, 0x60000004, 0xe004003f, 0, 0x4);		//set another sram tlb which valid cache
	set_e0_tlb(0x103e0000, 0xc0000400, 0x7000000a, 0xe004003f, 0, 0x4);		//set another sram tlb which invalid cache

	//write to sram epn which enable cache
	for(i=0; i<0x4000; i+=4)
		STW(i+0x22, i+0x60010000);

	//read from sram epn which diable cache, should different from 0x60000000
	for(i=0; i<0x4000; i+=4)
	{
		LDW(val, i+0x70010000);
		if(val == (i+0x22))
			while(1);
	}
	
	//flush cache
	for(i=0; i<0x4000; i+=0x40){
		DCBST(0x60010000+i);
		ISYNC;
	}
	
	//read from sram epn which diable cache, should same with 0x60000000
	for(i=0; i<0x4000; i+=4)
	{
		LDW(val, i+0x70010000);
		if(val != (i+0x22))
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");
}

void L1_d_cache_dcbtls_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-dcbtls:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * L1 dcbtls should pefetech and lock every cache block, total 8way * 64set = 512 cache blocks
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to VA ddr at 0x1000 which disable cache
	for(i=0; i<0x8000; i+=4)

		STW(0x5a5a5a5a, 0x1000+i);	

	//pre-fetch and lock 
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}

	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);
	
	for(i=8*0x40; i<8*0x40+1; i++){
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* (Data) Cache lock overflow */
	MFSPR(val, L1CSR0);
	if(val != 0x201)
		while(1);
}

void L1_d_cache_dcblc_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-dcblc:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to VA ddr at 0x1000 which disable cache
	for(i=0; i<0x8000; i+=4)
		STW(0x5a5a5a5a, 0x1000+i);	

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);

	/* unlock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBLC(tgt_addr+i*0x40);
		ISYNC;
	}

	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* pre-fetch and lock 513th dcache blocks */
	for(i=8*0x40; i<8*0x40+1; i++){
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	delay(0x1000);
	/* (Data) Cache should lock overflow */
	MFSPR(val, L1CSR0);
	if(val != 0x201)
		while(1);

	CACHE_DEBUG("if print here, it's right!");
}

void L1_d_cache_dcbtstls_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-dcbtstls:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to VA ddr at 0x1000 which disable cache
	for(i=0; i<0x8000; i+=4)
		STW(0x5a5a5a5a, 0x1000+i);	

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBTSTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);

	/* unlock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBLC(tgt_addr+i*0x40);
		ISYNC;
	}
	
	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBTSTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* pre-fetch and lock 513th dcache blocks */
	for(i=8*0x40; i<8*0x40+1; i++){
		DCBTSTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	delay(0x1000);
	/* (Data) Cache should lock overflow */
	MFSPR(val, L1CSR0);
	if(val != 0x201)
		while(1);

	CACHE_DEBUG("if print here, it's right!");
}

void L1_d_cache_unlock_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-L1CSR0 bit55:DCLFC:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to VA ddr at 0x1000 which disable cache
	for(i=0; i<0x8000; i+=4)
		STW(0x5a5a5a5a, 0x1000+i);	

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);

	/* DCLFC test here, unlock all dcache blocks */
	MTSPR(L1CSR0, 0x101);

	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x40; i++){				//8ways * 64entry = 512 dcache blocks 
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* pre-fetch and lock 513th dcache blocks */
	for(i=8*0x40; i<8*0x40+1; i++){
		DCBTLS(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* (Data) Cache should lock overflow */
	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val != 0x201)
		while(1);

	CACHE_DEBUG("if print here, it's right!");
}

void L1_d_cache_invalid_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test--L1CSR0:CFI
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * invalidate Data cache flash(line) by write CFI bit at L1CSR0
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/*
	 * CE:1 (DATA) Cache enable.
	 * CUL:0 is right;	1 is meaning can't be locked
	 * CLO:0 is right; 1 is meaning lock overflow condition
	 * so L1CSR0 should be 1
	 */
	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);

	MFSPR(val, L1CFG0);
	if(val!=0x00b83820)
		while(1);

	/* cache line invalidated test by CFI */ 
	CACHE_DEBUG("TEST CFI:\n");
	tgt_addr = TARGET_SPACE+0x11000;			//0x500011000
	
	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x6b6b6b6b, tgt_addr+i);	
	
	//read from ddr epn 0x11000 which diable cache, should different from 0x50011000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val == 0x6b6b6b6b)
			while(1);
	}
	
	/*Data cache flash(line) invalidate*/
	MTSPR(L1CSR0, (DCE | DCFI));
	
	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);

	delay(0x1000);
	MFSPR(val, L1CFG0);
	if(val!=0x00b83820)
		while(1);

	//flush dcache
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x11000 which diable cache, from 0x12000 to 0x14000 should different , other should be same
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x21000+i);
		if(val == 0x6b6b6b6b)		//should be different
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");	

}

void L1_dcache_LRU_replacement_test()
{		
	unsigned int i,j,d,d1,d2,error_count;
	unsigned int tgt_addr;
	unsigned int tgt_set;

	set_e0_tlb(0x10190000, 0xc0000900, 0x50000004, 0x3f, 0, 0);				//set another ddr tlb which valid cache
//	set_e0_tlb(0x101a0000, 0xc0000500, 0x60000004, 0xe000003f, 0, 0x4);		//set another sram tlb which valid cache
	
	/*
	 * test-dcbi:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR, use DCBF to make sure the instructions have been writed 
	 * run, then can stop at {for(num=0; num<0x40; num+=4) STW(0, tgt_addr+num);}
	 * now clean the ddr by code(CPU), run can printf
	 * invalid specified cacheline by icbi(addr), test whether it effective
	 */
	/*-------D-cache prepare-------*/	
	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	error_count = 0;
	i=64*0x40;							//32K I-cache has 64 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	// {{{ ------- test D-cache LRU replacement -------
	PSPRINTF("TEST_D_LRU:\n");
	// prepare test data
		for(j=1;j<9;j++) // write 8 ways to the same set
			for(i=0;i<32;i=i+4)
				STW(((0x11111111<<j)+i),(tgt_addr+j*0x1000+i));
		MSYNC;
		DCBTLS(tgt_addr+0x1000); // allocate and lock in way1
		DCBTLS(tgt_addr+0x2000); // allocate and lock in way2
		DCBTLS(tgt_addr+0x3000); // allocate and lock in way3
		DCBTLS(tgt_addr+0x4000); // allocate and lock in way4
		DCBTLS(tgt_addr+0x5000); // allocate and lock in way5
		DCBTLS(tgt_addr+0x6000); // allocate and lock in way6
		DCBTLS(tgt_addr+0x7000); // allocate and lock in way7
		DCBTLS(tgt_addr+0x8000); // allocate and lock in way0, even though all ways locked
		MSYNC;

		MFSPR(d,L1CSR0);
		if (d!=0xF03CC00F) { // all ways should be valid and locked
			error_count++;
			PSPRINTF("D_LRU DCDBTRL check fail, the value is: %x\n",d);
		}

		MFSPR(d,DCDBTRH);
		CLEAR_RIGHT(d,d,12); // clear bit 20:31
		if (d!=TARGET_SPACE+0x6000+0x1000) { // tag should be valid
			error_count++;
			PSPRINTF("D_LRU DCDBTRH check fail, the value is: %x\n",d);
		}
}

void L1_DCWS_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-L1CSR2-DCWS:Data cache write shadow
	 * Setting L1CSR2[DCWS], should automatically sets L1CSR0[CFI] to flash invalidate the data cache
	 * write 0x6b6b6b6b,should be writen to L1 Dcache and L2 Cache simultaneous
	 * Invalidate L1 Dcache and then flush data to memory by DCBF 
	 * data should flush to memory by L2 Cache, so shoulde be same with 0x6b6b6b6b
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to PA ddr at 0x50001000 which cache valid
	for(i=0; i<0x40*8; i+=4)
		STW(0x5a5a5a5a, tgt_addr+i);	
	
	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val != 0x01)
		while(1);

	/* read back, should be same with the data writen before */
	for(i=0; i<0x40*8; i+=4){
		LDW(val, tgt_addr+i);	
		if(val != 0x5a5a5a5a)
			while(1);
	}
	
	/* Setting L1CSR2[DCWS] */
	MTSPR(L1CSR2, 0x40000000);
	
	/* read back, the data should be flushed */
	for(i=0; i<0x40*8; i+=4){
		LDW(val, 0x1000+i);	
		if(val == 0x5a5a5a5a)
			while(1);
	}

	//write to PA ddr at 0x50005000 which cache valid
	for(i=0; i<0x40*8; i+=4)
		STW(0x6b6b6b6b, tgt_addr+0x4000+i);	

	/*Data cache flash(line) invalidate*/
	MTSPR(L1CSR0, (DCE | DCFI));

	delay(0x1000);
	MFSPR(val, L1CSR0);
	if(val!=0x01)
		while(1);

	//flush dcache
	for(i=0; i<0x40*8; i+=0x40){
		DCBF(tgt_addr+0x4000+i);
		ISYNC;
	}
	
	//read from ddr epn 0x5000 which diable cache, due to read from L2 Cache, should be same
	for(i=0; i<0x40*8; i+=4)
	{
		LDW(val, 0x5000+i);
		if(val != 0x6b6b6b6b)		//should be same
			while(1);
	}

	CACHE_DEBUG("if print here, it's right!");
}

void L2_cache_effective_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-L2 cache whether effective:
	 * because L2 cache only has one pool containing icache and dcache, can't test icache 
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR, use DCBF to make sure the instructions have been writed 
	 * run, then can stop at {for(num=0; num<0x40; num+=4) STW(0, tgt_addr+num);}
	 * now clean the ddr by code(CPU), run can printf
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	set_e0_tlb(0x103c0000,0xc0000500,0x60000004,0xe000003f,0x00000000,0x00000004);	//set sram tlb at entry 60, valid cache

	/* invalidate L1 cache */
	disable_L1_cache();
	delay(0x1000);
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	i=512*8;								//each core has 256KB L2 cache, L2 cache has 512 sets of eight blocks cache line, each cache line has 64 bytes.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+tgt_set;		//0x50001000

	/* ddr test */
	//write to ddr epn which enable cache
	for(i=0; i<tgt_set*0x40; i+=0x4)		//512 sets * 8 ways * 64 bytes
		STW(i+0x11, i+tgt_addr);			//stw different data to each word at 0x50001000 which valid cache, so will not trigger PLRU
	
	//read from ddr epn which diable cache, should different from 0x50000000
	for(i=0; i<tgt_set*0x40; i+=0x4)
	{
		LDW(val, i+tgt_set);				//ldw from 0x1000 which invalid cache
		if(val == (i+0x11))
			while(1);
	}
	
	//flush dcache
	for(i=0; i<tgt_set*0x40; i+=0x40){
		DCBF(i+tgt_addr);					//flush each cache line from 0x50001000
		ISYNC;
	}
	
	//read from ddr epn which diable cache, should same with 0x50000000
	for(i=0; i<tgt_set*0x40; i+=4)
	{
		LDW(val, i+tgt_addr);
		if(val != (i+0x11))
			while(1);
	}

	/* sram test */
	//write to sram epn which enable cache
	for(i=0; i<0x8000; i+=4)
		STW(i+0x22, i+0x60010000);

	//read from sram epn which diable cache, should different from 0x60000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i+0xe0010000);
		if(val == (i+0x22))
			while(1);
	}
	
	//flush cache
	for(i=0; i<0x8000; i+=0x40){
		DCBF(0x60010000+i);
		ISYNC;
	}
	
	//read from sram epn which diable cache, should same with 0x60000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i+0xe0010000);
		if(val != (i+0x22))
			while(1);
	}

	CACHE_DEBUG("should print here !:\n");
}

void L2_d_cache_dcbz_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-dcbz:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	CACHE_DEBUG("L2 dcache dcbz test begin:\n");

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	/*
	 * no dcbz, normal dcache test, just flush cache by dcbf
	 */
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x11+i, tgt_addr+i);	
	
	//read from ddr epn 0x1000 which diable cache, should different from 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val == 0x11+i)
			while(1);
	}

	//flush dcache, meanwhile invalidate 0x50001000 cacheline by dcbf
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x1000 which diable cache, should same with 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val != 0x11+i)
			while(1);
	}
	CACHE_DEBUG("L2 dcache data check without dcbz is right!");
	/*
	 * dcbz test
	 */
	CACHE_DEBUG("L2 dcache dcbz test2 :\n");
	tgt_addr = TARGET_SPACE+0x11000;			//0x500011000
	
	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x22+i, tgt_addr+i);	
	
	//read from ddr epn 0x11000 which diable cache, should different from 0x50011000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val == 0x22+i)
			while(1);
	}
	
	//invalid cache line from 0x50012000 to 0x50014000
	for(i=0x100; i<0x300; i+=0x40){
		DCBZ(tgt_addr+i); 			// Set to the target L1 D-cacheline
		ISYNC;
	}
	
	//flush dcache
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x11000 which diable cache, from 0x12000 to 0x14000 should different , other should be same
	for(i=0; i<0x100; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x22+i)		//should be same
			while(1);
	}
	
	for(i=0x100; i<0x300; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != (0))		//should be different
			while(1);
	}

	for(i=0x300; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x22+i)		//should be same
			while(1);
	}
	
	CACHE_DEBUG("L2 dcache data check with dcbz is right!");
}

void L2_d_cache_dcbi_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-L2 dcbi:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000000,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	/*
	 * test 1:no dcbi, normal dcache test, just flush cache by dcbf
	 */
	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x11+i, tgt_addr+i);	
	
	//read from ddr epn 0x1000 which diable cache, should different from 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val == 0x11+i)
			while(1);
	}

	//flush dcache, meanwhile invalidate 0x50001000 cacheline by dcbf
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x1000 which diable cache, should same with 0x50001000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x1000+i);
		if(val != 0x11+i)
			while(1);
	}
	CACHE_DEBUG("if print here, it's right");
		
	/*
	 * test 2: dcbi_1, not marked as Memory Coherence Required, (WIMGE=0bx00xx), dcbi should be effiective
	 */
	/* DCBI test 1*/
	CACHE_DEBUG("TEST _ICBI:\n");
	tgt_addr = TARGET_SPACE+0x11000;			//0x500011000
	
	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x22+i, tgt_addr+i);	
	
	//read from ddr epn 0x11000 which diable cache, should different from 0x50011000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val == 0x22+i)
			while(1);
	}
	
	//invalid cache line from 0x50011100 to 0x50011300
	for(i=0x100; i<0x300; i+=0x40){
		DCBI(tgt_addr+i); 			// invalid the target L1 D-cacheline
		ISYNC;
	}
	
	//flush dcache
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x11000 which diable cache, from 0x12000 to 0x14000 should different , other should be same
	for(i=0; i<0x100; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x22+i)		//should be same
			while(1);
	}
	
	for(i=0x100; i<0x300; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val == 0x22+i)		//should be different
			while(1);
	}

	for(i=0x300; i<0x1000; i+=4)
	{
		LDW(val, 0x11000+i);
		if(val != 0x22+i)		//should be same
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");	
	
	/*
	 * test 3: dcbi_2, marked as Memory Coherence Required, (WIMGE=0bx01xx), dcbi should be not effiective
	 */
	/* DCBI test 2*/
	set_e0_tlb(0x103c0000,0xc0000500,0x60000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x6000_0000 to 0x6010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	CACHE_DEBUG("TEST _ICBI:\n");
	tgt_addr = TARGET_SPACE+0x10021000;			//0x600021000
	
	//write to ddr epn which enable cache
	for(i=0; i<0x1000; i+=4)
		STW(0x33+i, tgt_addr+i);	
	
	//read from ddr epn 0x11000 which diable cache, should different from 0x50011000
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x21000+i);
		if(val == 0x33+i)
			while(1);
	}
	
	//invalid cache line from 0x50012000 to 0x50014000
	for(i=0x100; i<0x300; i+=0x40){
		DCBI(tgt_addr+i); 			// invalid the target L1 D-cacheline
		ISYNC;
	}
	
	//flush dcache
	for(i=0; i<0x1000; i+=0x40){
		DCBF(tgt_addr+i);
		ISYNC;
	}
	
	//read from ddr epn 0x11000 which diable cache, from 0x12000 to 0x14000 should different , other should be same
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, 0x21000+i);
		if(val != 0x33+i)		//should be same
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");	
}

void L2_d_cache_dcbst_test()
{
	unsigned int i, val;	
	
	/*
	 * dcbst is same as dsbf
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * should printf the finial  CACHE_DEBUG
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable
	
	/*
	 * test sram and ddr
	 * for example--ddr test: set two ddr tlbs which use two different va(0x500000000 and 0x00000000) but mapping the same pa(0x00000000)
	 * the va(0x50000000) valid cache, but va(0x00000000) invalid cache
	 * write data to va(0x50000000), then read va(0x00000000), va(0x00000000) should be different from va(0x50000000)
	 * after Data Flush by DCBF,  va(0x00000000) should be same with va(0x50000000)
	 */
	
	set_e0_tlb(0x10190000, 0xc0000900, 0x50000004, 0x3f, 0, 0);				//set another ddr tlb which valid cache
	set_e0_tlb(0x101a0000, 0xc0000500, 0x60000004, 0xe000003f, 0, 0x4);		//set another sram tlb which valid cache

	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	//write to ddr epn which enable cache
	for(i=0; i<0x8000; i+=4)
		STW(i+0x11, i+0x50000000);	
	
	//read from ddr epn which diable cache, should different from 0x50000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i);
		if(val == (i+0x11))
			while(1);
	}
	
	//flush dcache
	for(i=0; i<0x8000; i+=0x40){
		DCBST(0x50000000+i);
		ISYNC;
	}
	
	//read from ddr epn which diable cache, should same with 0x50000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i);
		if(val != (i+0x11))
			while(1);
	}

	//write to sram epn which enable cache
	for(i=0; i<0x8000; i+=4)
		STW(i+0x22, i+0x60010000);

	//read from sram epn which diable cache, should different from 0x60000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i+0xe0010000);
		if(val == (i+0x22))
			while(1);
	}
	
	//flush cache
	for(i=0; i<0x8000; i+=0x40){
		DCBF(0x60010000+i);
		ISYNC;
	}
	
	//read from sram epn which diable cache, should same with 0x60000000
	for(i=0; i<0x8000; i+=4)
	{
		LDW(val, i+0xe0010000);
		if(val != (i+0x22))
			while(1);
	}
	
	CACHE_DEBUG("if print here, it's right");
}

void L2_d_cache_dcbtls_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test-L2dcbtls:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * L1 dcbtls should pefetech and lock every cache block, total 8way * 64set = 512 cache blocks
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to VA ddr at 0x1000 which disable cache, 256k
	for(i=0; i<0x50000; i+=4)
		STW(0x5a5a5a5a, 0x1000+i);	

	for(i=0; i<8*0x200; i++){				//8ways * 512entry dcache blocks, total 256K 
		DCBTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);
	
	for(i=8*0x40*4; i<8*0x200+1; i++){
		DCBTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* L2 lock overflow, L2LO is set */
	delay(0x1000);
	MFSPR(val, L2CSR0);
	if(val != 0x80000020)
		while(1);
}

void L2_d_cache_dcblc_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-L2dcblc:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to VA ddr at 0x1000 which disable cache, 256k+64K
	for(i=0; i<0x50000; i+=4)
		STW(0x5a5a5a5a, 0x1000+i);	

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x200; i++){				//8ways * 512entry dcache blocks, total 256K 
		DCBTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);

	/* unlock 512 dcache blocks */
	for(i=0; i<8*0x200; i++){				//8ways * 512entry dcache blocks, total 256K 
		DCBLC2(tgt_addr+i*0x40);
		ISYNC;
	}

	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x200; i++){				//8ways * 512entry dcache blocks, total 256K 
		DCBTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* pre-fetch and lock 513th dcache blocks */
	for(i=8*0x200; i<8*0x200+1; i++){
		DCBTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* L2 lock overflow, L2LO is set */
	delay(0x1000);
	MFSPR(val, L2CSR0);
	if(val != 0x80000020)
		while(1);

	CACHE_DEBUG("if print here, it's right!");
}

void L2_d_cache_dcbtstls_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/*
	 * test-L2dcbtstls:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------D-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	tgt_addr = TARGET_SPACE+0x1000;			//0x50001000

	//write to VA ddr at 0x1000 which disable cache, 256k+64K
	for(i=0; i<0x50000; i+=4)
		STW(0x5a5a5a5a, 0x1000+i);	

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x200; i++){				//8ways * 512entry dcache blocks, total 256K 
		DCBTSTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);

	/* unlock 512 dcache blocks */
	for(i=0; i<8*0x200; i++){				//8ways * 512entry dcache blocks, total 256K 
		DCBLC2(tgt_addr+i*0x40);
		ISYNC;
	}

	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);

	/* pre-fetch and lock 512 dcache blocks */
	for(i=0; i<8*0x200; i++){				//8ways * 512entry dcache blocks, total 256K 
		DCBTSTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* pre-fetch and lock 513th dcache blocks */
	for(i=8*0x200; i<8*0x200+1; i++){
		DCBTSTLS2(tgt_addr+i*0x40);
		ISYNC;
	}
	
	/* L2 lock overflow, L2LO is set */
	delay(0x1000);
	MFSPR(val, L2CSR0);
	if(val != 0x80000020)
		while(1);

	CACHE_DEBUG("if print here, it's right!");
}

void L2_i_cache_icbi_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test: L2 icbi:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR(PA), instructe the instructions at DDR(VA), so now L2 Cache fill the instructions
	 * clear DDR(PA), instructe the instructions at DDR(VA), should be same as before
	 * invalid specified cacheline by icbi(addr), test whether it effective
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
	//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);
	
	i=64*0x40;							//just similar to L1 cache test
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	/* write to PA */
	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, 0x1000+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, 0x1000+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;

	/* instruct the instructions, and fill the L2 cache line at the same time */
	MTSPR(CTR, tgt_addr);
	BCTRL;

	/* clear PA */
	for(num=0; num<0x40; num+=4)
		STW(0, 0x1000+num);

	/* instruct from L2 Cache */
	MTSPR(CTR, tgt_addr);
	BCTRL;
	CACHE_DEBUG("should print here !:\n");
		
	/* ICBI test */
	CACHE_DEBUG("TEST _ICBI:\n");
	
	/* invalid the target L2 cacheline */
	ICBI(tgt_addr); 
	ISYNC;
	
	MTSPR(CTR, tgt_addr);
	BCTRL;
	
	CACHE_DEBUG("if print here, it's wrong");
}

void L2_i_cache_icbtls_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test: L2 icbtls2£¬ pre-fetch function :
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR(PA), 
	 * prefetech and lock by ICBTLS2, should print
	 * clear DDR(PA), instructe the instructions at DDR(VA), should be same as before
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
	//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);
	
	i=64*0x40;							//just similar to L1 cache test
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000

	/* write to PA */
	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, 0x1000+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, 0x1000+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;

	/* pre-fetch the instructins and lock, test pre-fetch function here */
	ICBTLS2(tgt_addr);
	ISYNC;

	/* clear PA */
	for(num=0; num<0x40; num+=4)
		STW(0, 0x1000+num);

	/* instruct from L2 Cache */
	MTSPR(CTR, tgt_addr);
	BCTRL;
	
	CACHE_DEBUG("should print here !:\n");
}

void L2_i_cache_icbtls2_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test: L2 icbtls2 lock function:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR(PA), 
	 * prefetech and lock by ICBTLS2, should print
	 * clear DDR(PA), instructe the instructions at DDR(VA), should be same as before
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
	//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);
	
	i=64*0x40;							//just similar to L1 cache test
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000
	
	/* ICBTLS2 8 ways of instruction and lock, test lock function here, each way has 512*64 bytes*/
	for(i=0; i<8; i++, tgt_set+=0x8000, tgt_addr+=0x8000){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_set+num);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_set+0x3C);		//0x4E800020 is instruction: blr
		ISYNC;
		ICBTLS2(tgt_addr);
		ISYNC;
	}

	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);

	/* ICBTLS2 the 9th way of instruction and lock*/
	tgt_addr = 0x50001000+0x8000*8;			
	tgt_set = tgt_addr;
	for(num=0; num<0x20-4; num+=4)
		STW(0x60000000, tgt_set+num);	//0x60000000 is instruction: NOP	
	STW(0x4E800020, tgt_set+0x1C);		//0x4E800020 is instruction: blr
	ISYNC;
	ICBTLS2(tgt_addr);
	ISYNC;

	/* L2 lock overflow, L2LO is set */
	delay(0x1000);
	MFSPR(val, L2CSR0);
	if(val != 0x80000020)
		while(1);

	CACHE_DEBUG("should print here !:\n");
}

void L2_i_cache_icblc_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test: L2 icblc:
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * first write some instruction at DDR(PA), 
	 * prefetech and lock by ICBTLS2, should print
	 * clear DDR(PA), instructe the instructions at DDR(VA), should be same as before
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
	//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);
	
	i=64*0x40;							//just similar to L1 cache test
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000
	
	/* ICBTLS2 8 ways of instruction and lock, test lock function here, each way has 512*64 bytes*/
	for(i=0; i<8; i++, tgt_set+=0x8000, tgt_addr+=0x8000){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_set+num);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_set+0x3C);		//0x4E800020 is instruction: blr
		ISYNC;
		ICBTLS2(tgt_addr);
		ISYNC;
	}

	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);

	tgt_addr = TARGET_SPACE + 64*0x40;
	for(i=0; i<8; i++, tgt_set+=0x8000, tgt_addr+=0x8000){
		ICBLC2(tgt_addr);
		ISYNC;
	}

	/* ICBTLS2 the 9th way of instruction and lock*/
	tgt_addr = 0x50001000+0x8000*8;			
	tgt_set = tgt_addr;
	for(num=0; num<0x20-4; num+=4)
		STW(0x60000000, tgt_set+num);	//0x60000000 is instruction: NOP	
	STW(0x4E800020, tgt_set+0x1C);		//0x4E800020 is instruction: blr
	ISYNC;
	ICBTLS2(tgt_addr);
	ISYNC;

	/* L2 not lock overflow*/
	delay(0x1000);
	MFSPR(val, L2CSR0);
	if(val != 0x80000000)
		while(1);

	MTSPR(CTR, tgt_addr);
	BCTRL;

	CACHE_DEBUG("should print here !:\n");
}

void L2_L2WP_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/*
	 * test: L2 L2WP:
	 * set 2ways for instruction cache
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 * clear DDR(PA), instructe the instructions at DDR(VA), should be same as before
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, size 256K
	//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M
	
	/* invaldate L1 cache */
	disable_L1_cache();
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);
	
	i=64*0x40;							//just similar to L1 cache test
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50001000
	
	/* set 2 way to be icache */
	MFSPR(val, L2CSR0);
	val |= 0x8000000;					//010 2 ways available for instruction allocation
	MTSPR(L2CSR0, val);
	
	/* ICBTLS2 2 ways of instruction and lock, lock function is useless here, just pre-fetch*/
	for(i=0; i<2*512; i++){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_set+num+i*0x40);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_set+0x3C+i*0x40);		//0x4E800020 is instruction: blr
		ISYNC;
		ICBTLS2(tgt_addr+i*0x40);
		ISYNC;
	}

	/* test 2ways of instruction cache usefully*/
	for(i=0; i<2*512; i++){
		MTSPR(CTR, tgt_addr+i*0x40);
		BCTRL;	
	}
	
	/* ICBTLS2 the 3rd way of instruction*/
	for(i=2*512; i<3*512; i++){
		for(num=0; num<0x40-4; num+=4)
			STW(0x60000000, tgt_set+num+i*0x40);	//0x60000000 is instruction: NOP	
		STW(0x4E800020, tgt_set+0x3C+i*0x40);		//0x4E800020 is instruction: blr
		ISYNC;
		ICBTLS2(tgt_addr+i*0x40);
		ISYNC;
	}

	/* clear DDR(PA) of the 3ways */
	for(i=0; i<3*512; i++){
		for(num=0; num<0x40; num+=4)
			STW(0, tgt_set+num+i*0x40);	//0x60000000 is instruction: NOP	
		ISYNC;
	}

	/* test 2ways of instruction cache, should can be instructed by L2 Cache(the 2ways used for instruction cache) */
	for(i=0; i<2*512; i++){
		MTSPR(CTR, tgt_addr+i*0x40);
		BCTRL;	
	}
	CACHE_DEBUG("should print here !:\n");

	/* test 3rd ways of instruction cache, should can't be instructed*/
	for(i=2*512; i<3*512; i++){
		MTSPR(CTR, tgt_addr+i*0x40);
		BCTRL;	
	}
	
	CACHE_DEBUG("if print here, it's wrong !:\n");
}

void L2_L2IO_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;

	/* L2 can't Data only when Instruction invalid! see 5.4.2 Write Shadow Mode
	 * test-L2 cache only used for data:
	 * need invaldate L1 cache
	 * need valdate L2 cache
	 * only instruction is writed to L2 Cache
	 * due to L2IO, data is now not write to L2 Cache, but to PA
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	/* invaldate L1 cache */
	disable_L1_cache();
	/* invaldate L3 cache */
	L3_Cache_Invalid();
	/* valdate L2 cache */
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);
	
	i=256*0x40;							//128K L2 cache has 512 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50004000

	/* set L2IO, only can used for i cache */
	MFSPR(val, L2CSR0);
	val |= 0x110000;					
	MTSPR(L2CSR0, val);
	val &= 0xFFFEFFFF;					
	MTSPR(L2CSR0, val);

	memset(0x4000, 0, 0x1000);			//clear DDR EPN
	
	//write to ddr epn which enable cache
	for(i=0; i<0x40; i+=4)
		STW(0x5a5a5a5a+i, tgt_addr+i);	
	
	//due to L2IO, the data is writed to PA instead of L2 cache
	for(i=0; i<0x40; i+=4)
	{
		LDW(val, 0x4000+i);
		if(val != 0x5a5a5a5a+i)
			while(1);
	}

	/* check instruction, write to PA */
	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, 0x1000+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, 0x1000+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;

	/* instruct the instructions, and fill the L2 cache line at the same time */
	MTSPR(CTR, 0x50001000);
	BCTRL;

	/* clear PA */
	for(num=0; num<0x40; num+=4)
		STW(0, 0x1000+num);

	/* instruct from L2 Cache */
	MTSPR(CTR, 0x50001000);
	BCTRL;

	CACHE_DEBUG("should print here !:\n");
}

void L2_L2DO_test()
{
	unsigned int i, val, d, num, error_count=0;	
	unsigned int tgt_addr, tgt_set;
	
	/* L2 can't Data only when Instruction invalid! see 5.4.2 Write Shadow Mode
	 * test-L2 cache only used for data:
	 * need invaldate L1 cache
	 * need valdate L2 cache
	 * only data is writed to L2 Cache
	 * due to L2DO, instruction is now not write to L2 Cache, but to PA
	 * disable JTAG snoop is read PA data, enable JTAG snoop is read cache data 
	 * ddr should valid cache
	 */
	/*-------I-cache prepare-------*/	
//	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

//	set_e0_tlb(0x103f0000,0xc0000400,0xe001000a,0xe000003f,0x00000000,0x00000004);	//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
//	set_e0_tlb(0x103e0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);	//set uart tlb at entry 62
	set_e0_tlb(0x103d0000,0xc0000500,0x50000004,0x0000003f,0x00000000,0x00000000);	//set ddr tlb at entry 61 from 0x5000_0000 to 0x5010_0000, va from 0x0000_0000 to 0x0010_0000, valid cache, 1M

	/* invaldate L1 cache */
	disable_L1_cache();
	/* invaldate L3 cache */
	L3_Cache_Invalid();
	/* valdate L2 cache */
	MFSPR(val, L2CSR0);
	if(val!=0x80000000)
		while(1);

	i=256*0x40;							//128K L2 cache has 512 sets of eight blocks with 64 bytes in each cache line.
	tgt_set = i;
	tgt_addr = TARGET_SPACE+i;			//0x50004000

	memset(0x4000, 0, 0x1000);			//clear DDR EPN

	/* set L2DO, only can used for d cache */
	MFSPR(val, L2CSR0);
	val |= 0x10000;					
	MTSPR(L2CSR0, val);
	do{
		MFSPR(val, L2CSR0);				
	}while(val != 0x80010000);

	//write to ddr epn which enable cache
	for(i=0; i<0x40; i+=4)
		STW(0x6b6b6b6b+i, tgt_addr+i);	
	
	//due to L2DO, the data is writed L2 cache
	for(i=0; i<0x40; i+=4)
	{
		LDW(val, 0x4000+i);
//		val = *(volatile unsigned int *)(0x8000+i);
		if(val == 0x6b6b6b6b+i)
			while(1);
	}

	DCBF(tgt_addr);
	ISYNC;

	//the data at PA should be same after flush
	for(i=0; i<0x40; i+=4)
	{
		LDW(val, 0x4000+i);
		if(val != 0x6b6b6b6b+i)
			while(1);
	}
	
	/* check instruction, write to PA */
	for(num=0; num<0x40-4; num+=4)
		STW(0x60000000, 0x1000+num);	//0x60000000 is instruction: NOP	
		
	STW(0x4E800020, 0x1000+0x3C);		//0x4E800020 is instruction: blr
	ISYNC;

	/* instruct the instructions, and fill the L2 cache line at the same time */
	MTSPR(CTR, 0x50001000);
	BCTRL;

	/* clear PA */
	for(num=0; num<0x40; num+=4)
		STW(0, 0x1000+num);

	/* instruct from L2 Cache */
	MTSPR(CTR, 0x50001000);
	BCTRL;

	CACHE_DEBUG("should not print here");		//when L2 Data only, the instruction will not writed to L2 cache but Memory
}
/* 
 * if DMA(src/dst cache valided ) with CoreNet, can maintain consistency
 */
void cache_consistency_test_DMA_CoreNet()
{
	unsigned int src_msize, src, dst_msize, dst, i, j, k, l, count;
	src_msize = 4;
	dst_msize = 4;

	/*
	 * disable JTAG snoop, so can read data by JTAG, JTAG now read real data at PA
	 * ddr should valid cache
	 */
	/*-------I-cache prepare-------*/	
	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000
	set_e0_tlb(0x103e0000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103d0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);//set uart tlb at entry 61 from 0xeb00_0000 to 0xeb40_0000

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}

	/* DDR to DDR */
	buf_init(0x00010000, 0x40, 0xa);
	src = 0x00010000;
	dst = 0x00020000;
	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0, src, 0, dst, 0x40/0x10-1,  (src_msize << 14) | (SRC_TR_WIDTH_128) | (dst_msize << 18) | (DST_TR_WIDTH_128));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x40; l++)
	{
		if(R8(src+l) != R8(dst+l))
		{
			   while(1);
		}
	}	
	CACHE_DEBUG("CoreNet cache consistency test1 right\n");

	/* SRAM to DDR */
	buf_init(0xe000fff0, 0x1000, 0xf);
	src = 0xe000fff0;
	dst = 0x000040010;

	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0x00000004, src, 0, dst, 0x1000/0x8-1,  (src_msize << 14) | (SRC_TR_WIDTH_64) | (dst_msize << 18) | (DST_TR_WIDTH_128));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x1000/4; l+=4)
	{
		if(R32(src+l) != R32(dst+l))
		{
			  while(1);
		}
	}
	CACHE_DEBUG("CoreNet cache consistency test2 right\n");

	/* DDR to SRAM */
	buf_init(0x00070000, 0x1000, 0xf);
	src = 0x0007fff0;
	dst = 0xe0020010;

	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0, src, 0x00000004, dst, 0x1000-1,  (src_msize << 14) | (SRC_TR_WIDTH_8) | (dst_msize << 18) | (DST_TR_WIDTH_32));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x1000/4; l+=4)
	{
		if(R32(src+l) != R32(dst+l))
		{
			  while(1);
		}
	}
	CACHE_DEBUG("CoreNet cache consistency test3 right\n");
	
	/* DDR to DDR, less than L2_256K size */
	buf_init(0x01000010, 0x30000, 0xf);
	src = 0x01000010;
	dst = 0x017ffff0;

	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0, src, 0, dst, 0x30000/0x2-1,  (src_msize << 14) | (SRC_TR_WIDTH_16) | (dst_msize << 18) | (DST_TR_WIDTH_64));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x30000/4; l+=4)
	{
		if(R32(src+l) != R32(dst+l))
		{
			  while(1);
		}
	}
	CACHE_DEBUG("CoreNet cache consistency test4 right\n");
	
	/* DDR to DDR, large than L2_256K size */
	buf_init(0x02000010, 0x100000, 0xf);
	src = 0x02000010;
	dst = 0x022ffff0;

	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0, src, 0, dst, 0x100000/0x4-1,  (src_msize << 14) | (SRC_TR_WIDTH_32) | (dst_msize << 18) | (DST_TR_WIDTH_32));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x100000/4; l+=4)
	{
		if(R32(src+l) != R32(dst+l))
		{
			  while(1);
		}
	}
	CACHE_DEBUG("CoreNet cache consistency test4 right\n");
}

/* 
 * if DMA(src/dst cache valided ) with Axi, can't maintain consistency
 */
void cache_consistency_test_DMA_AXI()
{
	unsigned int src_msize, src, dst_msize, dst, i, j, k, l, count;
	src_msize = 4;
	dst_msize = 4;

	/*
	 * disable JTAG snoop, so can read data by JTAG, JTAG now read real data at PA
	 * ddr should valid cache
	 */
	/*-------I-cache prepare-------*/	
	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	/* DMA change to AXI_BUS */
	i = *(volatile unsigned int*)(0xeb009000);
	*(volatile unsigned int*)(0xeb009000) = i&0xffffff7b;
	i = *(volatile unsigned int*)(0xeb009000);

	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000
	set_e0_tlb(0x103e0000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103d0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);//set uart tlb at entry 61 from 0xeb00_0000 to 0xeb40_0000

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
#if 0
	//test for L1 L2 cache replace
	buf_init(0x10010000, 0x21000, 0xa);
	src = 0x10010000;
	dst = 0x10080000;
	axidma_transfer_one_block_s1020(1, 0, src, 0, dst, 0x21000/0x10-1,  (src_msize << 14) | (SRC_TR_WIDTH_128) | (dst_msize << 18) | (DST_TR_WIDTH_128));
	for(l=0; l<0x21000/4; l+=4)
	{
		if(R32(src+l) == R32(dst+l))				//data should be different
		{
			CACHE_DEBUG("addr is %x, src is %x, dst is %x\n", src+l, R32(src+l), R32(dst+l));
		}
	}
	CACHE_DEBUG("AXI cache consistency test1 right\n");
#endif
	
	buf_init(0x00010000, 0x40, 0x7);
	src = 0x00010000;
	dst = 0x00020000;
	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0, src, 0, dst, 0x40/0x10-1,  (src_msize << 14) | (SRC_TR_WIDTH_128) | (dst_msize << 18) | (DST_TR_WIDTH_128));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x40/4; l+=4)
	{
		if(R32(src+l) == R32(dst+l))				//data should be different
		{
			   while(1);
		}
	}
	CACHE_DEBUG("AXI cache consistency test1 right\n");
	
	buf_init(0xe0010000, 0x1000, 0xf);
	src = 0xe0010000;
	dst = 0x00040000;

	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0x00000004, src, 0, dst, 0x1000/0x10-1,  (src_msize << 14) | (SRC_TR_WIDTH_128) | (dst_msize << 18) | (DST_TR_WIDTH_128));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x1000/4; l+=4)
	{
		if(R32(src+l) != R32(dst+l))			//data is from cache_invalid sram, so should be same
		{
			  while(1);
		}
	}
	CACHE_DEBUG("AXI cache consistency test2 right\n");

	buf_init(0x00080000, 0x1000, 0xf);
	src = 0x00080000;
	dst = 0xe0020000;

	axidma_reset();
	axidma_transfer_one_block_s1020(1, 0, src, 0x00000004, dst, 0x1000/0x10-1,  (src_msize << 14) | (SRC_TR_WIDTH_128) | (dst_msize << 18) | (DST_TR_WIDTH_128));

	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x1000/4; l+=4)
	{
		if(R32(src+l) == R32(dst+l))			//data is from cache_valid ddr, so should be different
		{
			  while(1);
		}
	}
	CACHE_DEBUG("AXI cache consistency test3 right\n");
}


void cache_consistency_test_Multi_Core()
{
	unsigned int val, va_data;
	unsigned int src_msize, src, dst_msize, dst, i, j, k, l, count;
	src_msize = 4;
	dst_msize = 4;
	va_data = 0x5a;

	/*
	 * disable JTAG snoop, so can read data by JTAG, JTAG now read real data at PA
	 * ddr should valid cache
	 * when CPU1 DMA operation, SRC 0x10000000, DST 0x10fffff0, due to CPU1 changed to AXI_BUS, when read data from src, will trigger cache_consistency, data will be writed to EPN
	 */
	/*-------I-cache prepare-------*/	
	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000
	set_e0_tlb(0x103e0000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103d0000,0xc0000800,0xe960800a,0xe800003f,0x00000000,0x00000004);//set uart tlb at entry 61 from 0xeb00_0000 to 0xeb40_0000
	set_e0_tlb(0x103c0000,0xc0000a00,0x4000000f,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 60 from 0x0000_0000 to 0x4000_0000

	//don't re-config ebc register
	for(i=0; i<0x3b; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}

	/* CPU0 write Cache located at DDR, data should be write to cache now */
	for(i=0; i<0x20000; i+=0x4)		
		STW(i+0x5a, i+0x10000000);			//CPU write data to 0x10000000-Cache 
	/* CPU read back for Cacha data test */
	for(i=0; i<0x20000; i+=0x4){		
		LDW(va_data, i+0x50000000);			//CPU read data to 0x50000000-VA(0x10000000-PA), should be different from 0x10000000-Cache
		if(va_data == i+0x5a)
			while(1);
	}
	
	/* Wait for CPU1 operation*/

}

void jtag_snoop_test()
{
	unsigned int val, va_data;
	unsigned int src_msize, src, dst_msize, dst, i, j, k, l, count;
	src_msize = 4;
	dst_msize = 4;

	/*
	 * disable JTAG snoop, so can read data by JTAG, JTAG now read real data at PA
	 * ddr should valid cache
	 * JTAG translate v:0x00000000 p:0x00000000 0x40000000 at .mem: p is meaning JTAG can read data at cache
	 */
	/*-------I-cache prepare-------*/	
	STW(0x202, JTAG_SNOOP_DISABLE);		//JTAG snoop disable

	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103d0000,0xc0000a00,0x5000000f,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103c0000,0xc0000800,0xe800000f,0xe800003f,0x00000000,0x00000004);//set uart tlb at entry 61 from 0xeb00_0000 to 0xeb40_0000

	//don't re-config ebc register
	for(i=0; i<0x3b; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	

	/* CPU0 write Cache located at DDR */
	buf_init(0x00000000, 0x2000, 0x5a);
	
	va_data = 0x5a5b5c5d;
	for(i=0; i<0x2000; i+=4){
		val = R32(0x50000000+i);
		if(val == va_data+i){
			CACHE_DEBUG("#######CPU Data Check Err!! Data is:%x Addr is:%x#######\n", R8(0x00000000+i), 0x00000000+i);
			while(1);
		}
	}

	/* now JTAG read memory 0x0, if data == 0x5a+i, it's wrong */	
}
	
void cache_main()
{
	unsigned int i;
	MFSPR(i,HID0);
	i |= 0x80;
	MTSPR(HID0, i);

	/*L1 L2 Reg test*/
	L1_cache_reg_test();
	L2_cache_reg_test();
	
	/*L1 32K ICache test*/
//	L1_i_cache_effective_test_32K();		//32K Icache test
	
	
	
	
	/*L1 I Cache test*/
//	L1_i_cache_effective_test();			//ok, use dcbf
//	L1_i_cache_icbi_test();
//	L1_i_cache_icbt_test();
//	L1_i_cache_icbtls_test();				//pre-fetch
//	L1_i_cache_icbtls2_test();				//lock
//	L1_i_cache_icbtls3_test();
//	L1_i_cache_icblc_test();
//	L1_i_cache_invalid_test();
//	L1_i_cache_unlock_test();
//	L1_i_cache_LRU_test();					//can't see cahce line, so it's can't used
	
	/*L1 D Cache test, no dcbt and dcbtst, can't test by memory*/
//	L1_d_cache_effective_test();
//	L1_d_cache_dcbi_test();
//	L1_d_cache_dcbz_test();
//	L1_d_cache_dcbst_test();
//	L1_d_cache_dcbtls_test();	
//	L1_d_cache_dcblc_test();	
//	L1_d_cache_dcbtstls_test();
//	L1_d_cache_unlock_test();			
//	L1_d_cache_invalid_test();			
//	L1_dcache_LRU_replacement_test(); 		//can't see cahce line, so it's can't used

//	L1_DCWS_test();							//Data cache write shadow

	/*L2 Cache test*/
//	L2_cache_effective_test();				//only one cache pool, can't test icache, only dcache here	
//	L2_d_cache_dcbz_test();	    //2024.6.11			
//	L2_d_cache_dcbi_test();				
//	L2_d_cache_dcbst_test();			
//	L2_d_cache_dcbtls_test();			
//	L2_d_cache_dcblc_test();	
//	L2_d_cache_dcbtstls_test();	

//	L2_i_cache_icbi_test();					
//	L2_i_cache_icbtls_test();				//pre-fetch
//	L2_i_cache_icbtls2_test();				//lock
//	L2_i_cache_icblc_test();
	
//	L2_L2WP_test();							//L2 Instruction/Data Way Partitioning			
//	L2_L2IO_test();							//L2 cache instruction only		
//	L2_L2DO_test();							//L2 cache data only		

//	L2_L2FI_test();							//
	
	/*JTAG test*/
//	jtag_snoop_test();

}

