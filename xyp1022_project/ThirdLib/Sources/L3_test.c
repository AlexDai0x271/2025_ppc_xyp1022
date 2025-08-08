/*
 * L3_test.c
 *
 *  Created on: 2021-8-5
 *      Author: ccore
 */

#include "cpc.h"
#include "e500mc_asm.h"
#include "ppctimer.h"
#include "common.h"

int plb_square(unsigned int num)
{
	if(num<0)
		return -1;
	else if(num<=1)
		return 1;
	else
		return 2*plb_square(num-1);
}

/* 
 * L3 Cache size is 1M, same as H2040 
 * L3 as Cache
 */
void L3_Init() {
volatile unsigned int rddata;
unsigned int mask_v;

// set CPCSR0[CPCFI] CPCCSR0[CPCFC] using the same write operation
mask_v = (1<<BIT_CPCLFC) | (1<<BIT_CPCFI);
*(volatile unsigned int*)(CPCCSR0) = mask_v;
rddata = 1;

while( (rddata & mask_v) != 0) {
    rddata = *(volatile unsigned int*)(CPCCSR0);
}

	MBAR;
	ISYNC;	
//Enable L3 Cache
    rddata = *(volatile unsigned int*)(CPCCSR0);
    *(volatile unsigned int*)(CPCCSR0) = rddata | (1<<BIT_CPCE | (1<<BIT_CPCPE));
    MBAR;
    ISYNC;
}

//for reference
void L3_Init_From_T1040() {
volatile unsigned int rddata;
unsigned int mask_v;

// set CPCSR0[CPCFI] CPCCSR0[CPCFC] using the same write operation
mask_v = (1<<BIT_CPCLFC) | (1<<BIT_CPCFI);
*(volatile unsigned int*)(CPCCSR0) = mask_v;
rddata = 1;

while( (rddata & mask_v) != 0) {
    rddata = *(volatile unsigned int*)(CPCCSR0);
}

#if L3_SRAM
*(volatile unsigned int*)(CPCSRCR1) = 0;
*(volatile unsigned int*)(CPCSRCR0) = (0<<BIT_CPCSRCR0_SRBARL) | (0<<BIT_CPCSRCR0_INTLVEN) | (0<<BIT_CPCSRCR0_SRAMSZ) | (1<<BIT_CPCSRCR0_SRAMEN);
asm("isync");
#endif

//Enable L3 Cache
    rddata = *(volatile unsigned int*)(CPCCSR0);
    *(volatile unsigned int*)(CPCCSR0) = rddata | (1<<BIT_CPCE);
}

/*
 * invalid L3 Cache Mode
 */
void L3_Cache_Invalid() {
	volatile unsigned int rddata;
	unsigned int mask_v;

	*(volatile unsigned int*)(CPCPAR0) = 0;
	*(volatile unsigned int*)(CPCCSR0) = ((1<<BIT_CPCFL) | (*(volatile unsigned int*)(CPCCSR0)));
	
	do{
		rddata = *(volatile unsigned int*)(CPCCSR0);
	}while((rddata&(1<<BIT_CPCFL)) != 0);
	
	*(volatile unsigned int*)(CPCCSR0) = ((1<<BIT_CPCLFC) | (*(volatile unsigned int*)(CPCCSR0))) ;
	
	do{
		rddata = *(volatile unsigned int*)(CPCCSR0);
	}while((rddata&(1<<BIT_CPCLFC)) != 0);
	
	*(volatile unsigned int*)(CPCCSR0) = 0<<BIT_CPCE;
}

/* Set the 36bit Physical Address by CPCSRCR1 and CPCSRCR0, then need set new tlb according to this PA
 * if L3 as Sram, the PA if L3 is same with DDR, but the data writed to the PA is not to DDR, but to L3  
 * SRAM size bit 26¨C30:
 * 3'b001 64 Kbytes
 * 3'b010 128 Kbytes
 * 3'b011 256 Kbytes
 * 3'b100 512 Kbytes
 * 3'b101 1   Mbytes
 */
void L3_Sram_Init(unsigned long long Va_Addr, unsigned int High_Addr, unsigned int Low_Addr, unsigned int SRAM_Size) {
	volatile unsigned int rddata;
	unsigned int mask_v;

	//clear CPCPARn
	*(volatile unsigned int*)(CPCPAR15) = 0;
	*(volatile unsigned int*)(CPCPAR14) = 0;
	*(volatile unsigned int*)(CPCPAR13) = 0;
	*(volatile unsigned int*)(CPCPAR12) = 0;
	*(volatile unsigned int*)(CPCPAR11) = 0;
	*(volatile unsigned int*)(CPCPAR10) = 0;
	*(volatile unsigned int*)(CPCPAR9) = 0;
	*(volatile unsigned int*)(CPCPAR8) = 0;
	*(volatile unsigned int*)(CPCPAR7) = 0;
	*(volatile unsigned int*)(CPCPAR6) = 0;
	*(volatile unsigned int*)(CPCPAR5) = 0;
	*(volatile unsigned int*)(CPCPAR4) = 0;
	*(volatile unsigned int*)(CPCPAR3) = 0;
	*(volatile unsigned int*)(CPCPAR2) = 0;
	*(volatile unsigned int*)(CPCPAR1) = 0;
	*(volatile unsigned int*)(CPCPAR0) = 0;
	
	//flush CPC
	*(volatile unsigned int*)(CPCCSR0) = ((1<<BIT_CPCFL) | (*(volatile unsigned int*)(CPCCSR0)));
	
	//wait for flush CPC
	while((rddata&(1<<BIT_CPCFL)) != 0)	{
		rddata = *(volatile unsigned int*)(CPCCSR0);
	}
	
	//clear lock bit and CPC flash invalidate 
	mask_v = (1<<BIT_CPCLFC) | (1<<BIT_CPCFI);
	*(volatile unsigned int*)(CPCCSR0) = mask_v;

	//wait for LFC and FI
	while( (rddata & mask_v) != 0) {
	    rddata = *(volatile unsigned int*)(CPCCSR0);
	}
	
	//disable CPC
	mask_v = *(volatile unsigned int*)(CPCCSR0) & (~(1<<BIT_CPCE));
	*(volatile unsigned int*)(CPCCSR0) = mask_v;
	
	COMMENT("Enable All CPC memory as SRAM.\n");
	*(volatile unsigned int*)(CPCSRCR0) = ((1<<BIT_CPCSRCR0_SRAMEN) | (SRAM_Size<<BIT_CPCSRCR0_SRAMSZ) | (Low_Addr&0xffff0000));			//64K ~ 1M   SRAM
	*(volatile unsigned int*)(CPCSRCR1) = (High_Addr&0xf);			
//	*(volatile unsigned int*)(CPCSRCR0) = ((1<<BIT_CPCSRCR0_SRAMEN) | (SRAM_Size<<BIT_CPCSRCR0_SRAMSZ) | (High_Addr&0x000f0000));			//64K ~ 1M   SRAM
//	*(volatile unsigned int*)(CPCSRCR1) = ((Low_Addr<<16)&0xffff);			

	*(volatile unsigned int*)(CPCPAR0) = 0x3FB;

	//enable CPC
    *(volatile unsigned int*)(CPCCSR0) = (rddata | (1<<BIT_CPCE) | (1<<BIT_CPCPE));
    ISYNC;
    
    /*
     * set L3 Sram tlb at entry 61, Va Base addr 0x8000_0000_0000_0000, size 4M for test, invalid cache
     * Va 0x8_0000_0000 ~ 0x8_0040_0000
     */
	set_e0_tlb(0x103d0000, 0xc0000600, Va_Addr|0xa, Low_Addr|0x3f, 0x00000000, 0x00000000|High_Addr);	

}


/* 
 * Invalid L3 Sram_mode
 */
void L3_Sram_Invalid(){
	volatile unsigned int rddata;
	unsigned int mask_v;
	//clear CPCPARn
	*(volatile unsigned int*)(CPCPAR15) = 0;
	*(volatile unsigned int*)(CPCPAR14) = 0;
	*(volatile unsigned int*)(CPCPAR13) = 0;
	*(volatile unsigned int*)(CPCPAR12) = 0;
	*(volatile unsigned int*)(CPCPAR11) = 0;
	*(volatile unsigned int*)(CPCPAR10) = 0;
	*(volatile unsigned int*)(CPCPAR9) = 0;
	*(volatile unsigned int*)(CPCPAR8) = 0;
	*(volatile unsigned int*)(CPCPAR7) = 0;
	*(volatile unsigned int*)(CPCPAR6) = 0;
	*(volatile unsigned int*)(CPCPAR5) = 0;
	*(volatile unsigned int*)(CPCPAR4) = 0;
	*(volatile unsigned int*)(CPCPAR3) = 0;
	*(volatile unsigned int*)(CPCPAR2) = 0;
	*(volatile unsigned int*)(CPCPAR1) = 0;
	*(volatile unsigned int*)(CPCPAR0) = 0;
	
	//flush CPC
	*(volatile unsigned int*)(CPCCSR0) = ((1<<BIT_CPCFL) | (*(volatile unsigned int*)(CPCCSR0)));
	
	//wait for flush CPC
	while((rddata&(1<<BIT_CPCFL)) != 0)	{
		rddata = *(volatile unsigned int*)(CPCCSR0);
	}
	
	//clear lock bit and CPC flash invalidate 
	mask_v = (1<<BIT_CPCLFC) | (1<<BIT_CPCFI);
	*(volatile unsigned int*)(CPCCSR0) = mask_v;
	
	//wait for LFC and FI
	while( (rddata & mask_v) != 0) {
		rddata = *(volatile unsigned int*)(CPCCSR0);
	}
	
	//disable CPC
	mask_v = *(volatile unsigned int*)(CPCCSR0) & (~(1<<BIT_CPCE));
	*(volatile unsigned int*)(CPCCSR0) = mask_v;
	
	COMMENT("Invalid All CPC memory as SRAM.\n");
	
}

//L3 not as Cache but as Sram
void L3_sram_test()
{
	/* L3 as SRAM, look for T1040RM_CPC*/
	volatile unsigned int rddata;
	unsigned int i, mask_v;
	unsigned long long Va_Addr;
	unsigned int High_Addr, Low_Addr, Size, data_len;
	
	High_Addr = 0x0;				//Pa_addr High_addr
	Low_Addr = 0;					//Pa_addr Low_addr
	Va_Addr = 0x0000000800000000;	//Va
	
	//	Size = 0x5;						//Size 	  1M
	//	Size = 0x4;						//Size 	  512K
	//	Size = 0x3;						//Size 	  256K
	//	Size = 0x2;						//Size 	  128K
	//	Size = 0x1;						//Size 	  64K
	for(Size=1; Size<6; Size++){
				
		data_len = 0x10000 * plb_square(Size);
		
		//Valid L3 Sram mode, Va 0x8_0000_0000 ~ 0x8_0040_0000
		L3_Sram_Init(Va_Addr, High_Addr, Low_Addr, Size);
		
		for(i=0; i<0x180000; i+=0x4)
			*(volatile UINT32 *)(Va_Addr+i) = 0x5a5a5a5a+i;
		
		for(i=0; i<0x180000; i+=0x4)
		{
			rddata = (UINT32)(*(volatile UINT32 *)(Va_Addr+i));
			if(rddata != (0x5a5a5a5a+i))
				while(1);
		}
	
		//Invalid L3 Sram mode, and Set L3 as Cache
		L3_Sram_Invalid();
		
		//Set L3 Cache mode
		L3_Init();
	
		for(i=0; i<data_len; i+=0x4)
		{
			rddata = (UINT32)(*(volatile UINT32 *)(Va_Addr+i));
			if(rddata == (0x5a5a5a5a+i))
				while(1);
		}
	
		for(i=data_len; i<0x180000; i+=0x4)
		{
			rddata = (UINT32)(*(volatile UINT32 *)(Va_Addr+i));
			if(rddata != (0x5a5a5a5a+i))
				while(1);
		}
	}
}

/* count DDR write/read speed when L3 Cache valid/invalid */
void L3_cache_count()
{
	/* L3 Cache size is 1M, same as H2040 */
	unsigned int i;
	unsigned long long time1, time2, User_Time;

	/*
	 * 1M size L3 test by DDR
	 */
	/* invalidate L1 L2 cache, if L1/L2valid, only when DDR data write/read heavy, L3 will not get involved */
	disable_L1_cache();
	disable_L2_cache();

#if 1	
	/* validate L3 cache */
	//L3_Init();		//user time:59575086
#else
	/* invalidate L3 cache */
	L3_Cache_Invalid();	//user time:61231705
#endif

	set_e0_tlb(0x103f0000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe004_0000
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800a,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
//	set_e0_tlb(0x103c0000,0xc0000800,0xe800000a,0xeb00003f,0x00000000,0x00000004);//set ebc_controller tlb at entry 60

//	set_e0_tlb(0x103e0000,0xc0000700,0x00000000,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0100_0000, valid cache
//	set_e0_tlb(0x103e0000,0xc0000800,0x00000000,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0400_0000, valid cache
	set_e0_tlb(0x103e0000,0xc0000800,0x00000004,0x00000015,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0400_0000, valid cache

	//don't re-config ebc register
	//for(i=0; i<0x3b; i++){
	//	clr_e0_tlb1_papa(0x10000000+(i<<16));
	//}

	time1 =get_tb_f();
	sram_chk(0x2000000,0x100000);			//DDR check
	time2 =get_tb_f();
	User_Time = (time2-time1);
	uart_printf("############ User_Time_Cost is %d s#############!\n, User_Time");
}

/* drystone test with L3, L3 Cache valid/invalid */
void L3_cache_dhry()
{
	/* L3 Cache size is 1M, same as H2040 */
	unsigned int i;
	unsigned long long time1, time2, User_Time;

	/*
	 * 1M size L3 test by DDR
	 */
	
	/* invalidate L1 L2 cache, if L1/L2valid, only when DDR data write/read heavy, L3 will not get involved */
	disable_L1_cache();
	disable_L2_cache();

#if 0	
	/* validate L3 cache */
	L3_Init();		//DMIPS:18.6705, -O3, n=100000
#else
	/* invalidate L3 cache */
	L3_Cache_Invalid();	//DMIPS:4.89585, -O3, n=100000
#endif
	/*Cache validated DMIPS_MHZ: 2.72428, DMIPS:163.457, -O3, L3 valid*/
	set_e0_tlb(0x103f0000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000, valid cache
	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
	set_e0_tlb(0x103e0000,0xc0000700,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0100_0000, valid cache

	//don't re-config ebc register
	for(i=0; i<0x3c; i++){
		clr_e0_tlb1_papa(0x10000000+(i<<16));
	}
	main_dhry();

}
