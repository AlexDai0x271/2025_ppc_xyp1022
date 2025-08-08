/*
 * mmu.c
 *
 *  Created on: 2020-10-26
 *      Author: ccore
 */
#include "iomacros.h"
#include "e500mc_asm.h"
#include "mmu.h"
#include "uart.h"
#include "ebc.h"

#define MMU_REG_TEST			1
#define DTLB_ERR_TEST			0
#define ITLB_ERR_TEST			0
#define TLB_READ_TEST			0
#define	TLB_CLR_TEST			0
#define	TLB_SEARCH_TEST			0
#define TLB_IVAX_TEST			0
#define TLB_4K_TEST				0
#define TLB_16K_TEST			0
#define TLB_64K_TEST			0
#define TLB_256K_TEST			0
#define TLB_1M_TEST				0
#define TLB_4M_TEST				0
#define TLB_16M_TEST			0		//cache test here
#define TLB_64M_TEST			0		//finish
#define TLB_256M_TEST			0
#define TLB_1G_TEST				0
#define TLB_2G_TEST				0
#define TLB_WIME_TEST			0
#define TLB0_READ_TEST			0
#define TLB0_4WAY_128ENTRY_TEST	0
#define MULTI_CORE_TLB			0

void set_e0_tlb(UINT32 r3,UINT32 r4,UINT64 r5,UINT32 r6,UINT32 r7,UINT32 r8)
{
	#if 0
	  li32  r3,0x10060000
	  li32  r4,0xc0000500
	  li32  r5,0xe0000000
	  li32  r6,0xe000003f
	  li32  r7,0x00000000
	  li32  r8,0x00000004
	  777
	#else
	  MTSPR(MAS0,r3);
	  MTSPR(MAS1,r4);
	  MTSPR(MAS2,r5);
	  MTSPR(MAS3,r6);
	  MTSPR(MAS5,r7);
	  MTSPR(MAS7,r8);
	#endif

	asm ("tlbwe");
	asm ("msync");
	asm ("isync");
}


void clr_e0_tlb1()
{
	unsigned int i;
	for(i=0; i<0x3b; i++)
	{
		MTSPR(MAS0,(0x10000000 | i<<16));
		MTSPR(MAS1,0);
		MTSPR(MAS2,0);
		MTSPR(MAS3,0);
		MTSPR(MAS5,0);
		MTSPR(MAS7,0);
		
		asm ("tlbwe");
		asm ("msync");
		asm ("isync");
	}
}


void clr_e0_tlb1_papa(unsigned int val)
{
	MTSPR(MAS0,val);
	MTSPR(MAS1,0);
	MTSPR(MAS2,0);
	MTSPR(MAS3,0);
	MTSPR(MAS5,0);
	MTSPR(MAS7,0);
	
	asm ("tlbwe");
	asm ("msync");
	asm ("isync");
}

void set_tlb1_valid_cache(unsigned int val)
{
	unsigned int r0, r1, r2, r3, r5, r7;
	MTSPR(MAS0,val);
	asm ("tlbre");
	MFSPR(r0, MAS0);
	MFSPR(r1, MAS1);
	MFSPR(r2, MAS2);
	MFSPR(r3, MAS3);
	MFSPR(r5, MAS5);
	MFSPR(r7, MAS7);
	asm ("msync");
	asm ("isync");

	r2 &= 0xfffffff7;
	
	MTSPR(MAS0,r0);
	MTSPR(MAS1,r1);
	MTSPR(MAS2,r2);
	MTSPR(MAS3,r3);
	MTSPR(MAS5,r5);
	MTSPR(MAS7,r7);
	
	asm ("tlbwe");
	asm ("msync");
	asm ("isync");
}

void mmu_reg_test()
{
	unsigned int tmp=0;

	MFMSR(tmp);
	tmp &= 0xEFFFBFFF;	//when GS=0 PR=0, the processor is in supervisor mode.
	MTMSR(tmp);
	MFMSR(tmp);			//make sure into hypervisor state
	ISYNC;
/*
	MFMSR(tmp);
	tmp |= 0x10000000;
	ISYNC;
	MTMSR(tmp);
	ISYNC;
	MFMSR(tmp);			//make sure into hypervisor state
*/
	MFSPR(tmp,LPIDR);	//hypervisor
	if(tmp)
		while(1);
	
	MFSPR(tmp,PID);		//Guest supervisor
	if(tmp)
		while(1);

	MFSPR(tmp,MMUCSR0);	//Hypervisor
	if(tmp)
		while(1);
	
	/*
	 * LPIDSIZE:0b0110 indicates LPIDR is 6 bits
	 * RASIZE:0b0100100 indicates 36 physical address bits 
	 * NPIDS:0b0001 indicates one PID register implemented
	 * PIDSIZE:0b00111 indicates PID is 8 bits
	 * NTLBS:0b01 indicates two TLBs
	 * MAVN:0b00 indicates Version 1.0
	 */
	MFSPR(tmp,MMUCFG);	
	if(tmp!=0x064809c4)
		while(1);

	/*
	 * ASSOC:0x4 Indicates TLB0 associativity is 4-way set associative
	 * MINSIZE:0x1 Indicates TLB0 smallest page size is 4 Kbytes
	 * MAXSIZE:0x1 Indicates TLB0 maximum page size is 4 Kbytes
	 * IPROT:0 Indicates invalidate protection capability not supported
	 * AVAIL:TLB0: 0 No variable-sized pages available (MINSIZE = MAXSIZE)
	 * NENTRY:0x200 Indicates TLB0 contains 512 entries.
	 */
	MFSPR(tmp,TLB0CFG);	//Hypervisor RO
	if(tmp!=0x04110200)
		while(1);
	
	/*
	 * ASSOC:0x40 Indicates TLB1 associativity is 64 (fully associative since ASSOC = NENTRY)
	 * MINSIZE:0x1 Indicates TLB1 smallest page size is 4 Kbytes
	 * MAXSIZE:0xB Indicates TLB1 maximum page size is 4 Gbytes
	 * IPROT:0x1 Indicates that TLB1 supports invalidate protection capability
	 * AVAIL:TLB1: 1 Indicates all page sizes between MINSIZE and MAXSIZE supported
	 * NENTRY:0x040 TLB1 contains 64 entries
	 */
	MFSPR(tmp,TLB1CFG);	//Hypervisor RO
	if(tmp!=0x401bc040)
		while(1);

	MFSPR(tmp,DEAR);	//allow access at Guest supervisor 
	if(tmp)
		while(1);

	MFSPR(tmp,GDEAR);	//allow access at Guest supervisor
	if(tmp)
		while(1);
}

void tlbre_test(unsigned int tlb_entry)
{
	unsigned int tmp=0;
	
	MFMSR(tmp);
	tmp &= 0xEFFFBFFF;
	MTMSR(tmp);
	MFMSR(tmp);			//tlbre must be executed by the hypervisor

	
	 tmp = 0x10000000 | (tlb_entry<<16);	//entry num
	 MTSPR(MAS0, tmp);
	  
	 asm volatile("tlbre");
	 MFSPR(tmp,MAS0);	
	 	 PSPRINTF("MAS0 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS1);
	 	 PSPRINTF("MAS1 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS2);
	 	 PSPRINTF("MAS2 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS3);
	 	 PSPRINTF("MAS3 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS4);
	 	 PSPRINTF("MAS4 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS5);
	 	 PSPRINTF("MAS5 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS6);
	 	 PSPRINTF("MAS6 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS7);
	 	 PSPRINTF("MAS7 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS8);	
	 	 PSPRINTF("MAS8 is: 0x%x\n", tmp); 
	 	 
	 MFSPR(tmp,LPIDR);	
	 	 PSPRINTF("LPIDR is: 0x%x\n", tmp); 
	 MFSPR(tmp,PID);	
		 PSPRINTF("PID is: 0x%x\n", tmp); 
		 	
}

/*
 * search invalid tlb from 0~64
 */
int invalid_tlb_search(void)
{
	int tmp, tlb_entry;
	
	MFMSR(tmp);
	tmp &= 0xEFFFBFFF;
	MTMSR(tmp);
	MFMSR(tmp);			//tlbre must be executed by the hypervisor
	
	for(tlb_entry=0; tlb_entry<64; tlb_entry++)
	{
		 tmp = 0x10000000 | (tlb_entry<<16);	//entry num
		 MTSPR(MAS0, tmp);

		 asm volatile("tlbre");
		 MFSPR(tmp,MAS1);
		 	 PSPRINTF("MAS1 is: 0x%x\n", tmp);

		 if(!(tmp & 0x80000000))
			 return tlb_entry;
	}
	
	return -1;
}

void mmu_test(){	
	volatile int i=0;
    volatile int j;
	unsigned int tmp=0, tlb_entry, val, val2, data;


//	asm ("lis  n,j");
//	asm ("ori  n,n,j");
	
	/*
	//Updata MAS7 for e500, e5500 forget this 
	MFSPR(i,HID0);
	i |= 0x80;
	MTSPR(HID0, i);
	*/
	
//	set_e0_tlb(0x10020000,0xc0000A00,0x00000008,0x0000003f,0x00000000,0x00000000);//cache off DDR
//	TLBRE(tmp, 624, 2);
	
#if MMU_REG_TEST	
	/*reg read test*/
	mmu_reg_test();				
#endif
	
#if	DTLB_ERR_TEST
	/* 
	 * dtlb err test
	 * retain sram to prevent clr code_obj
	 * test by uart operation without tlb
	 */
	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram tlb at entry 63
	clr_e0_tlb1();
	STW(0, 0);
//	uart_printf_buf("############ X #############!\n");
#endif
	
#if ITLB_ERR_TEST
	/* 
	 * itlb err test
	 * tlbclr all include sram, so when execute instruction at sram, go into _ITLB_PREHANDLER 
	 */
//	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram tlb at entry 63
	clr_e0_tlb1();
#endif

#if TLB_READ_TEST
	/*
	 * 64+1 entry TLB1 tlbre test(entry0~63)
	 * entry64 should same with entry0
	 */
	for(tlb_entry=0; tlb_entry<65; tlb_entry++)
		tlbre_test(tlb_entry);	
#endif

#if TLB_CLR_TEST
	/*
	 * tlb clr test
	 * only reserve sram from 0xe0000000 to 0xe0100000
	 * sram_chk test from 0xe0040000 to 0xe0080000
	 */
	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram tlb at entry 63
	set_e0_tlb(0x103e0000,0xc0000100,0xe960800a,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 62
	clr_e0_tlb1();
	sram_chk(0xe0040000,0x40000);
#endif
	
#if	TLB_SEARCH_TEST
	unsigned int addr1, addr2, addr3;
	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram tlb at entry 63
	set_e0_tlb(0x103e0000,0xc0000100,0xe960800a,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 62
	clr_e0_tlb1();
	
	addr1 = 0xe0004251;		//addr at sram locate tlb1-entry3f 
	addr2 = 0xe9608000;		//addr at uart locate tlb1-entry3e 
	addr3 = 0xA60FFF0F;		//addr from DDR not locate, so MAS1[V] should be 0
	
	/* test1, tlbsx rA, rB (preferred form: tlbsx 0, rB), rA is always 0 */
	TLBSX(addr1);
	MFSPR(val, MAS0);
	if(val != 0x103f0000)
		while(1);

	MFSPR(val, MAS1);
	if(val != 0xc0000500)
		while(1);
	
	MFSPR(val, MAS2);
	if(val != 0xe000000a)
		while(1);
		
	MFSPR(val, MAS3);
	if(val != 0xe000003f)
		while(1);

	MFSPR(val, MAS7);
	if(val != 0x00000004)
		while(1);

	/* test2, tlbsx rA, rB (preferred form: tlbsx 0, rB), rA is always 0 */
	TLBSX(addr2);
	MFSPR(val, MAS0);
	if(val != 0x103e0000)
		while(1);

	MFSPR(val, MAS1);
	if(val != 0xc0000100)
		while(1);
	
	MFSPR(val, MAS2);
	if(val != 0xe960800a)
		while(1);
		
	MFSPR(val, MAS3);
	if(val != 0xe960803f)
		while(1);

	MFSPR(val, MAS7);
	if(val != 0x00000004)
		while(1);
#if 0
	/* test3, tlbsx rA, rB (preferred form: tlbsx 0, rB), rA is always 0 */
	/* if use this test, duo to addr3 is not hit any tlb, reusme[F8] process will go into error, but if */
	TLBSX(addr3);
	MFSPR(val, MAS0);
//	if(val != 0x100b0000)
//		while(1);

	MFSPR(val, MAS1);
	if((val & 0x80000000)== 0x80000000)
	//if(val != 0xc0000900)
		while(1);
	
	MFSPR(val, MAS2);
//	if(val != 0xa000000a)
//		while(1);
		
	MFSPR(val, MAS3);
//	if(val != 0x0000003f)
//		while(1);

	MFSPR(val, MAS7);
//	if(val != 0x00000008)
//		while(1);
#endif
#endif
	
#if TLB_IVAX_TEST
	/*
	 * 16K size test by sram1
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000200,0xe004000a,0xe004003f,0x00000000,0x00000004);//set sram1 tlb at entry 62 from 0xe004_0000 to 0xe004_4000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
	set_e0_tlb(0x103c0000,0xc0000900,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 60 from 0x0000_0000 to 0x1000_0000, valid cache
	set_e0_tlb(0x103b0000,0xc0000900,0x10000004,0x1000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 59 from 0x1000_0000 to 0x2000_0000, valid cache
	clr_e0_tlb1();
	
	for(i=0; i<0x3f; i++){
		/* clear IPROT */
		val = 0x10000000+(i<<16);
		MTSPR(MAS0, val);
		asm volatile("tlbre");
		ISYNC;
		MFSPR(data, MAS1);
		MTSPR(MAS1, (data & 0xBFFFFFFF));
		asm volatile("tlbwe");
		ISYNC;
	}

	/* 4way*128sets TLB0 ddr: from 0x0000_0000 to 0x0010_0000, every block size is 4K*/
	for(j=0; j<4; j++){
		for(i=0; i<128; i++){					
			set_e0_tlb(0x00000000+0x10000*j,0xc0000100,0x00000004+0x1000*i+0x80000*j,0x0000003f+0x1000*i+0x80000*j,0x00000000,0x00000000);//set ddr tlb0 cache valid
		}
	}

//test1:See tlb0 by JTAG 
	//Use Window->Show View->Shell, Input displaytlb 0, should see 512 entry tlb0

//test2:Invalidate all tlb0, check by JTAG too, val=4 mean bit 61: IA, bit 59~60 choose which TLB, see EREF
	val = 0x4;				
	/* Invalidate TLB0 */
	TLBIVAX(val);

	//Use Window->Show View->Shell, Input displaytlb 0, should see none entry tlb0

//test3:Only invalidate uart of tlb1, val |= 8: bit 59~60 choose TLB1, see EREF
	/* Invalidate uart of TLB1 */
	val = 0xE9608000 | 0x8;
	TLBIVAX(val);
	//should goto dtlb_err
	uart_printf("############ if print here, it's wrong!#############!\n");

//test4:Invalidate part of DDR
	//move PC here from Data_TLB_Err
	/* set uart tlb */
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
	uart_printf("############ if print here, it's right!#############!\n");

	/* Invalidate 0x10000000(DDR) of TLB1 */
	val = 0x10000000 | 0x8;
	TLBIVAX(val);

	sram_chk(0,0x10000000);
	uart_printf("############ 0x00000000_256K is right#############!\n");

	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0x10000000
	sram_chk(0x10000000,0x10000000);
	uart_printf("############ if print here, it's wrong!#############!\n");
#endif

#if TLB_4K_TEST
	/*
	 * 4K size test by uart0
	 */
	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram tlb at entry 63
	set_e0_tlb(0x103e0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 62
	//should print endof ############ tlb:3d #############!
	for(i=0; i<0x3e; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	uart_printf("############ tlb:%x #############!\n", i);
	//here read out of tlb should go into DTLB_ERR
	val = *(volatile unsigned char *)(UART0_BASE+0x1000+0);
#endif
	
#if TLB_16K_TEST
	/*
	 * 16K size test by sram1
	 */
	uart_printf("MMU TLB size test by sram:");

	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000200,0xe004000a,0xe004003f,0x00000000,0x00000004);//set sram1 tlb at entry 62 from 0xe004_0000 to 0xe004_4000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61

	for(i=0; i<0x3d; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	sram_chk(0xe0040000,0x4000);
	uart_printf("############ 0xe0040000_16K is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xe0044000
	sram_chk(0xe0044000,0x10);
#endif

#if TLB_64K_TEST
	/*
	 * 64K size test by sram1
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000300,0xe0040004,0xe004003f,0x00000000,0x00000004);//set sram1 tlb at entry 62 from 0xe004_0000 to 0xe005_0000, valid cache
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61

	for(i=0; i<0x3d; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	sram_chk(0xe0040000,0x10000);
	uart_printf("############ 0xe0040000_64K is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xe0050000uart_printf_buf
	//sram_chk(0xe0050000,0x10);
#endif

#if TLB_256K_TEST
	/*
	 * 1M size test by ebc_sram0
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000400,0xf700000a,0xf700003f,0x00000000,0x00000004);//set ebc_sram tlb at entry 62 from 0xf700_0000 to 0xf704_0000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
	set_e0_tlb(0x103c0000,0xc0000800,0xe960800f,0xe960803f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	ebc_init();
	sram_chk(BANK1_RAM_ADDR,0x40000);			//EBC_SRAM check
	uart_printf("############ 0xf7000000_256K is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7040000
	sram_chk(0xf7040000,0x10);
#endif

#if TLB_1M_TEST
	/*
	 * 1M size test by ddr, Cache Valid
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000500,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0010_0000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	sram_chk(0,0x100000);				//DDR check
	uart_printf("############ 0x00000000_1M is right#############!\n");

	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7040000
	sram_chk(0x100000,0x10);
#endif

#if TLB_4M_TEST
	/*
	 * 4M size test by ddr
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000600,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0040_0000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
//	set_e0_tlb(0x103c0000,0xc0000800,0xe800000a,0xeb00003f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	sram_chk(0,0x400000);			//DDR check
	uart_printf("############ 0x00000000_4M is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7100000
	sram_chk(0x400000,0x10);
#endif
	
#if TLB_16M_TEST
	/*
	 * 16M size test by ddr
	 */
/*	L3_init_clr();
	
	disable_L1_cache();
	MFSPR(val, 1017);
	if(val!=0x80000000)
		while(1);
*/
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
//	set_e0_tlb(0x103c0000,0xc0000800,0xe800000a,0xeb00003f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

	set_e0_tlb(0x103e0000,0xc0000700,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0100_0000, valid cache
//	set_e0_tlb(0x103e0000,0xc0000800,0x00000000,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0400_0000, valid cache
//	set_e0_tlb(0x103e0000,0xc0000800,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0400_0000, valid cache


	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
//	sram_chk(0,0x1000000);				//DDR check
	mem_scan_test(0,0x1000000);			//word/half_word/\byet
		
//	sram_chk_byte(0,0x1000000);			//DDR check
//	sram_chk_kernel_test(0x2000, 0x100000, 0x200000);	//1M

//	sram_chk(0x20000,0x1000000);			//EBC_SRAM check
//	sram_chk_with_code(0x20000, 0x1000000, 0x2000000, 0xd000);	//16M
//	sram_chk_with_code(0x20000, 0x10000, 0x2000000, 0xd000);	//64K

	uart_printf("############ 0x00000000_16M is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7100000
	sram_chk(0x1000000,0x10);
#endif

#if TLB_64M_TEST
	/*
	 * 64M size test by ddr
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe0000004,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, valid cache
	set_e0_tlb(0x103e0000,0xc0000800,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0400_0000, valid cache
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
//	set_e0_tlb(0x103c0000,0xc0000800,0xe800000a,0xeb00003f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	sram_chk(0,0x4000000);			//DDR check
	uart_printf("############ 0x00000000_64M is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7100000
	sram_chk(0x4000000,0x10);
#endif

#if TLB_256M_TEST
	/*
	 * 256M size test by ddr
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
//	set_e0_tlb(0x103e0000,0xc0000900,0x0000000a,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x1000_0000, invalid cache
	set_e0_tlb(0x103e0000,0xc0000900,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x1000_0000, valid cache
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	sram_chk(0,0x10000000);			//DDR check
	uart_printf("############ 0x00000000_256M is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7100000
	sram_chk(0x10000000,0x10);
#endif

#if TLB_1G_TEST
	/*
	 * 1G size test by ddr
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe0000004,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103e0000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103d0000,0xc0000100,0xeb00d00f,0xeb00d03f,0x00000000,0x00000004);//set uart tlb at entry 61
//	set_e0_tlb(0x103c0000,0xc0000800,0xe800000a,0xeb00003f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
//	sram_chk_simple(0,0x40000000);			//DDR check
	sram_chk(0,0x40000000);			//DDR check
	uart_printf("############ 0x00000000_1G is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7100000
	LDB(val, 0x40000000-1);
	LDB(val, 0x40000000);		//goin _DTLB_PREHANDLER
	LDB(val, 0x40000000+1);
#endif

#if TLB_2G_TEST
	/*
	 * 4G size test by ddr
	 */
	set_e0_tlb(0x103f0000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000
	set_e0_tlb(0x103e0000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103d0000,0xc0000a00,0x40000004,0x4000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x4000_0000 to 0x8000_0000
	set_e0_tlb(0x103b0000,0xc0000100,0xeb00d00f,0xeb00d03f,0x00000000,0x00000004);//set uart tlb at entry 61
//	set_e0_tlb(0x103c0000,0xc0000800,0xe800000a,0xeb00003f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

	//don't re-config ebc register
	for(i=0; i<0x3b; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	sram_chk(0,0x80000000);			//DDR check
	uart_printf("############ 0x00000000_2G is right#############!\n");
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xf7100000
	sram_chk(0x80000000,0x10);
#endif

#if TLB_WIME_TEST
	/*
	 * 64K size test by sram1
	 */
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000, for code
	
	set_e0_tlb(0x103e0000,0xc0000300,0xe004000a,0xe004003f,0x00000000,0x00000004);//set sram1 tlb at entry 62 from 0xe004_0000 to 0xe005_0000, disable cache and W
	set_e0_tlb(0x103d0000,0xc0000300,0x00040000,0xe004003f,0x00000000,0x00000004);//set sram1 tlb at entry 61 from 0xe004_0000 to 0xe005_0000, enable cache, disable W
	
	set_e0_tlb(0x103c0000,0xc0000300,0xe005000a,0xe005003f,0x00000000,0x00000004);//set sram1 tlb at entry 60 from 0xe005_0000 to 0xe006_0000, disable cache and W
	set_e0_tlb(0x103b0000,0xc0000300,0x00050010,0xe005003f,0x00000000,0x00000004);//set sram1 tlb at entry 59 from 0xe005_0000 to 0xe006_0000, enable cache, enable W

	set_e0_tlb(0x103a0000,0xc0000300,0x00000019,0x0000003f,0x00000000,0x00000000);//set ddr tlb at entry 58 from 0x0000_0000 to 0x0001_0000, disable cache, disable W,enable E
	set_e0_tlb(0x10390000,0xc0000300,0x00010018,0x0001003f,0x00000000,0x00000000);//set ddr tlb at entry 58 from 0x0001_0000 to 0x0002_0000, disable cache, disable W,enable E

	set_e0_tlb(0x10380000,0xc0000100,0xe960800a,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 57

	for(i=0; i<0x38; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	//write to sram epn which enable cache, enable W
	for(i=0; i<0x1000; i+=4)
		STW(i+0x22, i+0x00050000);
	
	//read from sram epn which diable cache, should be same 
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, i+0xe0050000);
		if(val != (i+0x22))
			while(1);
	}
	uart_printf("############ storage attribute W is right#############!\n");
	
	//write to sram epn which enable cache, disable W
	for(i=0; i<0x1000; i+=4)
		STW(i+0x22, i+0x00040000);
	
	//read from sram epn which diable cache, should be different
	for(i=0; i<0x1000; i+=4)
	{
		LDW(val, i+0xe0040000);
		if(val == (i+0x22))
			while(1);
	}	
	uart_printf("############ storage attribute W is right#############!\n");

	//write to ddr epn which disable cache, disable W,enable E
	for(i=0; i<0x1000; i+=4)
		STW(0x12345678, i+0);
	for(i=0; i<0x1000; i+=4)
		STW(0x12345678, i+0x10000);
	
	//read from sram epn which diable cache, should be same 
	for(i=0; i<0x1000; i++)
	{
		LDB(val, i+0);
		LDB(val2, i+0x10000);
		if(val == val2)
			while(1);
	}
	uart_printf("############ storage attribute E is right#############!\n");
	
	//M: Memory coherency test wit hL1_d_cache_dcbi_test
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0xe0060000
	sram_chk(0xe0060000,0x10);
#endif

#if TLB0_READ_TEST

	L3_init_clr();

	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
//	set_e0_tlb(0x103e0000,0xc0000100,0x0000000a,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0xffff_ffff
//	set_e0_tlb(0x103d0000,0xc0000100,0xeb00d00f,0xeb00d03f,0x00000000,0x00000004);//set uart tlb at entry 61
//	set_e0_tlb(0x103c0000,0xc0000800,0xe800000a,0xeb00003f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	//tlb0 write test
	set_e0_tlb(0x00000000,0xc0000100,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr tlb0 entry0 way0, cache valid
	set_e0_tlb(0x00000000,0xc0000100,0x00001004,0x0000503f,0x00000000,0x00000000);//set ddr tlb0 entry1 way0, cache valid
	set_e0_tlb(0x00000000,0xc0000100,0xe960800a,0xe960803f,0x00000000,0x00000004);//set uart tlb0 at entry8 way0, cache invalid
	
	set_e0_tlb(0x00010000,0xc0000100,0x00002004,0x0000103f,0x00000000,0x00000000);//set ddr tlb0 entry2 way1, cache valid
	set_e0_tlb(0x00020000,0xc0000100,0x00003004,0x0000803f,0x00000000,0x00000000);//set ddr tlb0 entry3 way2, cache valid
	set_e0_tlb(0x00030000,0xc0000100,0x00004004,0x1000103f,0x00000000,0x00000000);//set ddr tlb0 entry4 way3, cache valid

	//tlb0 read test
	 tmp = 0x00000000 | (0<<16);	//entry 0
	 MTSPR(MAS0, tmp);
	 asm volatile("tlbre");
	 MFSPR(tmp,MAS0);	
	 	 uart_printf("MAS0 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS1);
	 	 uart_printf("MAS1 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS2);
	 	 uart_printf("MAS2 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS3);
	 	 uart_printf("MAS3 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS4);
	 	 uart_printf("MAS4 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS5);
	 	 uart_printf("MAS5 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS6);
	 	 uart_printf("MAS6 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS7);
	 	 uart_printf("MAS7 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS8);	
	 	 uart_printf("MAS8 is: 0x%x\n", tmp); 
	 		
//	set_e0_tlb(0x00010000,0xc0000100,0x0000100a,0x0000103f,0x00000000,0x00000000);//set ddr tlb0 entry0 way1
//	set_e0_tlb(0x00020000,0xc0000100,0x0000200a,0x0000203f,0x00000000,0x00000000);//set ddr tlb0 entry0 way2
//	set_e0_tlb(0x00030000,0xc0000100,0x0000300a,0x0000303f,0x00000000,0x00000000);//set ddr tlb0 entry0 way3

	 tmp = 0x00000000 | (0<<16);	//entry 0
	 MTSPR(MAS0, tmp);
	 asm volatile("tlbre");
	 MFSPR(tmp,MAS0);	
	 	 uart_printf("MAS0 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS1);
	 	 uart_printf("MAS1 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS2);
	 	 uart_printf("MAS2 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS3);
	 	 uart_printf("MAS3 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS4);
	 	 uart_printf("MAS4 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS5);
	 	 uart_printf("MAS5 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS6);
	 	 uart_printf("MAS6 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS7);
	 	 uart_printf("MAS7 is: 0x%x\n", tmp);
	 MFSPR(tmp,MAS8);	
	 	 uart_printf("MAS8 is: 0x%x\n", tmp); 

	sram_chk(0,0x4000);				//DDR check
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0x4000
	sram_chk(0x5000,0x10);			//DDR check
	uart_printf("############ TLB0 test is right#############!\n");
#endif

#if TLB0_4WAY_128ENTRY_TEST

	L3_Init();
	set_e0_tlb(0x103f0000,0xc0000400,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
//	set_e0_tlb(0x103e0000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x4000_0000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	
	/* ddr: from 0x0000_0000 to 0x0010_0000, every block size is 4K*/
	for(j=0; j<4; j++){
		for(i=0; i<128; i++){					
			set_e0_tlb(0x00000000+0x10000*j,0xc0000100,0x00000004+0x1000*i+0x80000*j,0x0000003f+0x1000*i+0x80000*j,0x00000000,0x00000000);//set ddr tlb0 cache valid
		}
	}
	
	/*4way * 128entrys * 4K =  0x200000*/
	sram_chk(0,0x200000);			//DDR check
	
	//here read out of tlb should go into DTLB_ERR, and DEAR should be 0x200000
	sram_chk(0x200000,0x10);			//DDR check
	uart_printf("############ if print here, it's wrong!#############!\n");
#endif

	
}
	
