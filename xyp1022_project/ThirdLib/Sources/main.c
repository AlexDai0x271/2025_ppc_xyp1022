//////////////////////////
//	Project Stationery  //
//////////////////////////

#include <stdio.h>
#include "e500mc.h"
#include "e500mc_asm.h"
#include "common.h"
#include "ppctimer.h"
#include "uart.h"
#include "mpic.h"
#include "gpio.h"
#include "ebc.h"

#define L1CSR0 1010
#define L1CSR1 1011

#define L2CSR0 1017
#define L2CSR1 1018
#define IO_MULTIPLEX            (0x28 + 0xeb009000)



#if 1
#define GMAC_TEST				1  ///ok
#define SRAM_TEST				1  ///ok
#define DDR_TEST				0
#define DDR_TEST2				0  ///ok,再次测试需要上电重启
#define UART_TEST				0		       //ok
#define CPU_32_BIT_MODE			0  ///ok
#define WHETSTONE_TEST			0  ///不用测
#define DRYSTONE_TEST			1  ///ok
#define COREMARK_TEST			0  ///fail
#define MMU_TEST				1  ///ok
#define CACHE_TEST				1  ///ok,only L2 开了不能循环跑
#define CONSISTENCY_TEST		0  ///fail
#define L3_TEST					0  ///fail
#define TIME_TEST				1  ///单独可测，不能和EMMC或USB或SATA同时开
#define FP_TEST					0  ///fail,未找到测试程序
#define AXI_DMA_TEST			1  ///ok		//ok
#define SWITCH_TEST				0  ///fail,需要连线
#define SEC_TEST				1  ///ok		//ok
#define ARNG_TEST				0  ///不用测
#define DRNG_TEST				0  ///不用测
#define WRNG_TEST				0  ///不用测
#define FUSE_TEST				0  ///不用测
#define SECMON_TEST				0  ///不用测
#define EBC_CLEAR_DOWNLOAD		1  ///ok
#define GPIO_M0_M1_TEST			1
#define CPU_PERFORMANCE_TEST	1  ///ok		//ok
#define MPIC_TEST				0  ///不用测 
#define QUICC_TEST				1
#define I2C_TEST				1		        //ok
#define SPI_TEST				0  ///ok
#define PCIE_TEST				1  
#define NAND_TEST				1
#define eMMC_TEST		    	0  ///ok	
#define USB_TEST		    	0  ///ok	   
#define LCD_TEST		    	1  ///ok
#define SATA_TEST		    	1  ///ok
#else
#define GMAC_TEST				0
#define SRAM_TEST				0
#define DDR_TEST				0
#define DDR_TEST2				0		//ok
#define UART_TEST				0		//ok
#define CPU_32_BIT_MODE			0
#define WHETSTONE_TEST			0
#define DRYSTONE_TEST			0
#define COREMARK_TEST			0
#define MMU_TEST				0
#define CACHE_TEST				0
#define CONSISTENCY_TEST		0
#define L3_TEST					0
#define TIME_TEST				0
#define FP_TEST					0
#define AXI_DMA_TEST			0		//ok
#define SWITCH_TEST				0
#define SEC_TEST				0		//ok
#define ARNG_TEST				0
#define DRNG_TEST				0
#define WRNG_TEST				0
#define FUSE_TEST				0
#define SECMON_TEST				0
#define EBC_CLEAR_DOWNLOAD		0
#define GPIO_M0_M1_TEST			0
#define CPU_PERFORMANCE_TEST	0		//ok
#define MPIC_TEST				0
#define QUICC_TEST				0
#define I2C_TEST				0		//ok
#define SPI_TEST				0
#define PCIE_TEST				1
#define SATA_TEST		    	0  ///ok
#endif

#define DMA_CoreNet_BUS		0		//if 0, DMA_AXI_BUS
#define SEC_CoreNet_BUS		0		//if 0, SEC_AXI_BUS



extern void dhry_test ();

void init_cache(void)
{
	unsigned int val=0;
	// flash invalidae the L1 cache
#if 0
 __asm__ (
	".set L1CSR0, 1010 \n"
	".set L1CSR1, 1011 \n"
	"xor 4,4,4 \n"
	"ori 5,4,0x0102 \n"
	// set CFI and CFLC bits
	"sync \n"
	"isync \n"
	"mtspr L1CSR0,5 \n"
	"isync \n"
	"dloop: \n"
	"mfspr 4,L1CSR0 \n"
	"and. 4,4,5 \n"
	"bne dloop \n"
	"isync \n"
	 // L1 instruction cache
	"xor 4,4,4 \n"
	"ori 5,4,0x0102 \n"
	"sync \n"
	"isync \n"
	"mtspr L1CSR1,5 \n"
	"isync \n"
	"iloop : \n"
	"mfspr 4,L1CSR1 \n"
	"and. 4,4,5 \n"
	"bne iloop \n"
	"isync"
	);
#endif

#if 1
 // L2 data cache
 __asm__ (
".set L2CSR0, 1017 \n"
 "xor 4,4,4 \n" // set r4 to 0
"ori 5,4,0x0400 \n" // set L2LFC bit
 "oris 5,5,0x0020 \n" // set L2FI
 "sync \n"
 "isync \n" // synchronize setting of L2CSR0
 "mtspr L2CSR0,5 \n" // flash invalidate L2 cache and locks
 "isync \n" // synchronize setting of L2CSR0
 "l2loop: \n"
 "mfspr 4,L2CSR0 \n" // get current value
 "and. 5,5,4 \n" // compare to see if complete
 "bne l2loop"
		 );
#endif

 //enable L1 ,L2
  MFSPR(val,L1CSR0);

 val |= 0x01;

 MTSPR(L1CSR0,val);
  asm("isync");

 MFSPR(val,L1CSR1);
  val |= 0x01;
  MTSPR(L1CSR1,val);
  asm("isync");

 //enable L2
 MFSPR(val,L2CSR0);
  val |= 0x01;
  MTSPR(L2CSR0,val);
  asm("isync");

}

void check_sram()
{
	unsigned int i, val;

	for(i=0; i<16; i+=4)
		val = *((volatile unsigned int *)(0xe0010000+i));
}

void spr_test()
{
	unsigned int val;
	MFSPR(val, TBL);
	MTSPR(TBL_W, 0xaaaaaaaa);
	MFSPR(val, TBL);

	MTSPR(TBU_W, 0x55555555);
	MFSPR(val, TBU);
}

void cpu_32_bit_test()
{
	unsigned int val, j;
	volatile long long aa=0, addr;
	unsigned long bb=0;

	set_e0_tlb(0x103e0000,0xc0000700,0x800000000000000f,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0100_0000, invalid cache
	/* if CM=0, processor is in 32-bit mode */
	MFMSR(val);
	val &= 0x7FFFFFFF;			//into 32 bit mode
	MTMSR(val);
	asm ("msync");
	asm ("isync");

	aa = 0x5a5a5a5a5a5a5a5a;	//unsigned long long is 64 bit when CPU 32 or 64 mode
	bb = 0x6b6b6b6b6b6b6b6b;	//unsigned long is also 64 bit when CPU 32 mode, because of code target mode is 64 bit

	addr = 0x8000000000000000ull;
	memset(0, 0, 0x2000);			//clr DDR

	for(j=0; j<0x100; j++)
		*((volatile unsigned long long *)addr+j) = aa + j;

	for(j=0; j<0x100; j++)
		if( *((volatile unsigned long long *)addr+j) != aa + j)
			while(1);

	addr += 0x1000;
	for(j=0; j<0x100; j++)
		*((volatile unsigned long *)addr+j) = bb + j;

	for(j=0; j<0x100; j++)
	if( *((volatile unsigned long *)addr+j) != bb + j)
		while(1);

}

void dma_axi_bus()
{
	unsigned int i;

	/* DMA0 DMA1 change to AXI_BUS */
	i = *(volatile unsigned int*)(0xeb009000);
	*(volatile unsigned int*)(0xeb009000) = i&0xffffff7b;
	i = *(volatile unsigned int*)(0xeb009000);
}

void sec_axi_bus()
{
	unsigned int i;

	/* SEC change to AXI_BUS */
	i = *(volatile unsigned int*)(0xeb009000);
	*(volatile unsigned int*)(0xeb009000) = i&0xfffffffd;
	i = *(volatile unsigned int*)(0xeb009000);
}

void wrng_io_multiplex()
{
	unsigned int i;

	/* SEC change to AXI_BUS */
	i = *(volatile unsigned int*)(0xeb009028);
	*(volatile unsigned int*)(0xeb009028) = i|0x7;
	i = *(volatile unsigned int*)(0xeb009028);
}

void clock_tb_en()
{
	unsigned int val;

	LDW(val, PCTBEN);
	val |= 0x01;
	STW(val, PCTBEN);
}

void new_setting()
{
	//acu
	*(volatile unsigned int *)(0xe9541000) = 0;
	*(volatile unsigned int *)(0xe9200410) = 0;

	//deco
	*(volatile unsigned int *)(0xe9205e00) = 1;
	*(volatile unsigned int *)(0xe9206e00) = 1;
	*(volatile unsigned int *)(0xe9207e00) = 1;
	*(volatile unsigned int *)(0xe9208e00) = 1;
	*(volatile unsigned int *)(0xe9209e00) = 1;
	*(volatile unsigned int *)(0xe920ae00) = 1;
	*(volatile unsigned int *)(0xe920be00) = 1;
	*(volatile unsigned int *)(0xe920ce00) = 1;

}

void GPIO_init( void )
{
	unsigned int reg,i;

	//gpio0/gpio1/gpio15
	reg = R32( GPIO_M_BASE_ADDR	|GPIO_IOCTL_OFFSET );
	W32(GPIO_M_BASE_ADDR	|GPIO_IOCTL_OFFSET, reg|(1 <<0)|(1 <<1));

	reg = R32( GPIO_M_BASE_ADDR	|GPIO_DIR_OFFSET );
	W32(GPIO_M_BASE_ADDR	|GPIO_DIR_OFFSET,	reg|(1 <<0)|(1 <<1));

	reg = R32( GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET );
	W32(GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET,	reg|(1 <<0)|(1 <<1));

}


void CPU_1500M_test( void )
{
	unsigned int val, reg, i, freq;
	unsigned long long time3,time4;
	float time1, time2;

	reg = R32( GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET );


//	for ( i = 0; i < 2; i++ )
//	while(1)
	{
//		asm("lis	7,(0x3B02338)@h");
//		asm("ori	7, 7, (0x3B02338)@l");
//		asm("mtctr	7");
		asm("lis	7,(0x9D5B34)@h");
		asm("ori	7, 7, (0x9D5B34)@l");
		asm("mtctr	7");
		W32(GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET,	(reg & 0xfffffffc) );
		time3 =get_tb_f();

		asm("DIV_LOOP4:");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm(".long	0x4200fe00");
			time4 =get_tb_f();
			time1 = time4 - time3;

//			asm("lis	7,(0x3B02338)@h");
//			asm("ori	7, 7, (0x3B02338)@l");
//			asm("mtctr	7");
			asm("lis	7,(0x9D5B34)@h");
			asm("ori	7, 7, (0x9D5B34)@l");
			asm("mtctr	7");
			W32(GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET,	(reg & 0xfffffffc)|0x03 );
			time3 =get_tb_f();
			asm("DIV_LOOP7:");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("addi 4,4,0x2");
			asm("sub  4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm("sub 4,4,0x2");
			asm(".long	0x4200ff00");

			time4 =get_tb_f();
			time2 = time4 - time3;

//	uart_printf( "time1:%f\r\n", time1 );
//	uart_printf( "time2:%f\r\n", time2 );
//	uart_printf( "time1-time2:%f\r\n", time1-time2 );

	//freq = ( 6204000000 / ( time1 - time2 ) );
	freq = ( 6187500000 / ( time1 - time2 ) );
	uart_printf( "CPU freq:%dMHz\r\n", freq );
	}
}


void CPU_1320M_test_2()
{

	unsigned int val, reg, i;

	reg = R32( GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET );

//	while(1)
	for ( i = 0; i < 5; i++ )
	{
		asm("lis	7,(0x3B02338)@h");
		asm("ori	7, 7, (0x3B02338)@l");
		asm("mtctr	7");
		W32(GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET,	(reg & 0xffffff0f) );
		asm("DIV_LOOP2:");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm(".long	0x4200fe00");


		asm("lis	7,(0x3B02338)@h");
		asm("ori	7, 7, (0x3B02338)@l");
		asm("mtctr	7");
		W32(GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET,	(reg & 0xffffff0f)|0xf0 );
		asm("DIV_LOOP3:");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("addi 4,4,0x2");
		asm("sub  4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm("sub 4,4,0x2");
		asm(".long	0x4200ff00");
	}

//	return 0;

}


//MTSPR(spr,data)
int main_cpu0()
{
	int i=0, j;
	unsigned int val, reg;
	volatile long long aa=0;
	unsigned long proc_id=0,atbl,atbu;
	volatile unsigned int cpu0_jmp_addr,daddr_l,datalen;
#if 1
	/* Updata MAS7 and Set CIGLSO */
	MFSPR(i,HID0);
	i |= 0x90;
	MTSPR(HID0, i);
	MSYNC;

	/* Select core timer base enable */
	clock_tb_en();

	/* enable int at MSR[EE], also valid CE ME FP */
	MFMSR(val);
	val |= 0x2B000;			//enable CE EE ME FP
	MTMSR(val);
	asm ("isync");

	char *p,*p1;
	TLBSX(0x00000004e007c000);
#endif	
	uart0_16750_init(UART_XTAL, 115200);
	//uart_printf_buf("XYP1022 uart0  test start. \n");
	uart_printf("XYP1022 board test start. \n");

#if 0	
	*(volatile unsigned int *)IO_MULTIPLEX |= 0x00000600 ;
	uart1_16750_init(UART_XTAL, 115200);
	uart1_printf_buf("XYP1022 uart1  test start. \n");
	
	uart2_16750_init(UART_XTAL, 115200);
	uart2_printf("XYP1022 uart2  test start. \n");
	
	uart3_16750_init(UART_XTAL, 115200);
	uart3_printf("XYP1022 uart3 test start. \n");

	//GPIO_init();
#endif	
	/* if ddr_init not done by FPGA, SW init DDR here*/
	ddr_init();

/*
	sram_chk(0x1000000,0x100000);				//DDR check	1M
	set_e0_tlb(0x10050000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);
	set_e0_tlb(0x10060000,0xc0000a00,0x40000004,0x4000003f,0x00000000,0x00000000);
	sram_chk(0x1000000,0x100000);				//DDR check	1M
*/
	/* if DDR not init by both HW and SW, clear DDR tlb */
//	set_e0_tlb(0x10020000,0x40000a00,0x00000004,0x0000003f,0x00000000,0x00000000);

//	MFSPR(i, 0x19D);
//	L3_init_clr();

//	MTSPR(TBL_W,0);
//	MTSPR(TBU_W,0);
//	MFTB(atbl,TBL);

//	MFSPR(atbl,TBU_R);
//	MFSPR(atbl,TBL_R);
//	MFSPR(atbl,TBU_R);
	//uart_printf_buf("XYP1022 test start. \n");
	*(volatile unsigned int *)0xeb009000 = 0xffffffff;				//corenet
	/* mpic reset and init, move to init_chip */
	mpic_reset();
	mpic_init();
	
	sata_init();
	
	pcie_rc_init();
//	mpic_unmask();
	/* gmac init, link up*/	
//	gmac_init(0);
#if 0
	/* tmp test */
	while(1){
		set_e0_tlb(0x10200000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);

		invalidate_l1_dcache();
		invalidate_l1_icache();
		invalidate_l2_cache();
		enable_branch();
		set_msr_ee();
		enable_l1_dcache();
		enable_l1_icache();
		enable_l2_cache();
	}
#endif
	while(1){

#if GMAC_TEST
		gmac_test();
#endif	
	#if SGMII_TEST
		uart_printf_buf("SGMII test start!\n");
		sgmii_test();
		uart_printf_buf("SGMII test end!\n");
	#endif
		
	#if DDR_TEST2
		uart_printf_buf("DDR3L test start. \r\n");
		set_e0_tlb(0x10050000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);
		set_e0_tlb(0x10060000,0xc0000a00,0x40000004,0x4000003f,0x00000000,0x00000000);
		set_e0_tlb(0x10030000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);
	
		sram_chk(0x00000000,0x100000);				//DDR check	1M
		sram_chk(0x10000000,0x100000);				//DDR check	1M
		sram_chk(0x20000000,0x100000);				//DDR check	1M
		sram_chk(0x30000000,0x100000);				//DDR check	1M
	
	//	sram_chk_byte(0x40000000,0x100000);
	//	sram_chk_byte(0x50000000,0x100000);
	//	sram_chk_byte(0x60000000,0x100000);
	//	sram_chk_byte(0x70000000,0x100000);
	//	sram_chk_byte(0x00000000,0x100000);
	//	sram_chk_byte(0x10000000,0x100000);
	//	sram_chk_byte(0x20000000,0x100000);
	//	sram_chk_byte(0x30000000,0x100000);
	
	//	sram_chk(0x40000000,0x40000000);			//DDR check	1G
	//	sram_chk(0x00000000,0x40000000);			//DDR check	2G
	
		set_e0_tlb(0x10050000,0xc0000a00,0x0000000a,0x0000003f,0x00000000,0x00000000);
		set_e0_tlb(0x10060000,0xc0000a00,0x4000000a,0x4000003f,0x00000000,0x00000000);
	
//		sram_chk(0x00000000,0x100000);				//DDR check	1M
//		sram_chk(0x10000000,0x100000);				//DDR check	1M
//		sram_chk(0x20000000,0x100000);				//DDR check	1M
//		sram_chk(0x30000000,0x100000);				//DDR check	1M
		sram_chk(0x40000000,0x100000);				//DDR check	1M
		sram_chk(0x50000000,0x100000);				//DDR check	1M
		sram_chk(0x60000000,0x100000);				//DDR check	1M
		sram_chk(0x70000000,0x100000);				//DDR check	1M
	
	//	sram_chk_byte(0x00000000,0x100000);
	//	sram_chk_byte(0x10000000,0x100000);
	//	sram_chk_byte(0x20000000,0x100000);
	//	sram_chk_byte(0x30000000,0x100000);
	//	sram_chk_byte(0x40000000,0x100000);
	//	sram_chk_byte(0x50000000,0x100000);
	//	sram_chk_byte(0x60000000,0x100000);
	//	sram_chk_byte(0x70000000,0x100000);
	
	
	//	sram_chk(0x00000000,0x40000000);				//DDR check	1G
	//	sram_chk(0x40000000,0x40000000);				//DDR check	2G
		uart_printf_buf("DDR3L test OK! \r\n");
	#endif
	
	#if SRAM_TEST
	//	sram_test();						//sram word_halfword_byet test
		uart_printf_buf("SRAM test start!\n");
		sram_chk(0xe0070000,0x8000);		//SRAM check	256K
		uart_printf_buf("SRAM test end!\n");
	#endif
	
	#if CPU_PERFORMANCE_TEST
	
		set_e0_tlb(0x103f0000,0xc0000500,0xe0000002,0xe000003f,0x00000000,0x00000004);
		clr_e0_tlb1_papa(0x10000000+(0x3<<16));
	
		uart_printf_buf("CPU Performance test start!\n");
		//		CPU_1320M_test_2();
		CPU_1500M_test();
		uart_printf_buf("CPU Performance test end!\n");
	
		set_e0_tlb(0x10030000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);
		clr_e0_tlb1_papa(0x10000000+(0x3f<<16));
	#endif
	
	
	#if I2C_TEST
		uart_printf_buf("IIC test start. \r\n");
		iic_test();
		uart_printf_buf("IIC test OK! \r\n");
	#endif
	
	
	
	
	#if DDR_TEST
		set_e0_tlb(0x10050000,0xc0000a00,0x00000004,0x0000003f,0x00000000,0x00000000);
	//	set_e0_tlb(0x10060000,0xc0000a00,0x40000004,0x4000003f,0x00000000,0x00000000);
		set_e0_tlb(0x10030000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);
	
		sram_chk(0,0x100000);				//DDR check	1M
		sram_chk(0x40000000,0x1000000);		//DDR check 10M
		mem_scan_test(0,0x100000);			//word/half_word/byte
	#endif
		
	#if AXI_DMA_TEST
	#if DMA_CoreNet_BUS
		uart_printf_buf("DMA_TEST start!\n");
		axidma_test();
		uart_printf_buf("DMA_TEST ok!\n\n");
	#else
		//DMA_AXI_BUS
		dma_axi_bus();
		uart_printf_buf("DMA_TEST start!\n");
		axidma_test();
		uart_printf_buf("DMA_TEST ok!\n\n");
	#endif
	#endif
	
	
	#if UART_TEST
		#if 0
		unsigned char buf[0x20];
		for(i=0; i<0x10; i++)
			buf[i] = uart0_16750_RX();
		uart_printf_buf(buf);
	//	while(1);
		#endif
		uart_printf_buf("UART_TEST start!\n");
		uart_printf_buf("UART_TEST start, should turn down the serial port!\n");
		uart_test1();
		uart_test2();
		uart_test3();
		uart_test4();
		//delay(10000);
		uart_printf_buf("UART_TEST ok!\n\n");
	#endif
	
	#if CPU_32_BIT_MODE
		uart_printf_buf("CPU_32_BIT test start. \r\n");
		cpu_32_bit_test();
		uart_printf_buf("CPU_32_BIT test end. \r\n");
	#endif
	
	#if MMU_TEST
		uart_printf_buf("MMU test start. \r\n");
		mmu_test();
		uart_printf_buf("MMU test end. \r\n");
	#endif
	
	#if CACHE_TEST
		cache_main();
	#endif
	
	#if CONSISTENCY_TEST
	//	cache_consistency_test_DMA_CoreNet();
		cache_consistency_test_DMA_AXI();
	//	cache_consistency_test_Multi_Core();
	#endif
	#if MPIC_TEST
		mpic_test();
	#endif
	#if QUICC_TEST
		quicc_test();
	#endif
	#if L3_TEST
		uart_printf_buf("L3 cache test start. \r\n");
//		L3_cache_count();
//		init_cache();

	//	L3_cache_dhry();
		L3_sram_test();
		uart_printf_buf("L3 cache test end. \r\n");
	#endif
	
	#if WHETSTONE_TEST
		whetstone_test();
	#endif
	
	#if COREMARK_TEST
		set_e0_tlb(0x103f0000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000, valid cache
		set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
		set_e0_tlb(0x103e0000,0xc0000700,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0100_0000, valid cache
		//don't re-config ebc register
		for(i=0; i<0x3c; i++){
			clr_e0_tlb1_papa(0x10000000+(i<<16));
		}
	
		uart_printf("benchmark_____test begin run %d\n");
		coremark_main();
	#endif
	
	#if TIME_TEST
		uart_printf_buf("Timer test start. \r\n");
		timer_test();
		uart_printf_buf("Timer test end. \r\n");
	#endif
	
	
	#if SWITCH_TEST
	//	switch_wr_rd_test();
	//	switch_reg_test();
	//	swicth_mode_test();
	//	swicth_portx_txrx_disable_test();
		switch_vlan_test();
	//	switch_state_monitor();
	//	phy_init();		 			 //initialize the ext phy, need DPAA
	//	switch_three_layer_test();	 //can't use
	#endif
	
	#if ARNG_TEST
		new_setting();
		sec_init();
	#if SEC_CoreNet_BUS
	
	#else
		//SEC_AXI_BUS
		sec_axi_bus();
	#endif
		analog_rng_test_SEC();							//Analog RNG SEC mode
	#endif
	
	#if DRNG_TEST
		new_setting();
		sec_init();
	#if SEC_CoreNet_BUS
	
	#else
		//SEC_AXI_BUS
		sec_axi_bus();
	#endif
	//	digital_rng_test_CPU();
		digital_rng_test_SEC();
	#endif
	
	#if WRNG_TEST
		sec_init();
		new_setting();
		wrng_io_multiplex();
	
	#if SEC_CoreNet_BUS
	
	#else
		//SEC_AXI_BUS
		sec_axi_bus();
	#endif
	
		wrng_test_CPU();
	//	wrng_test_SEC_no_bypass();
	//	wrng_test_SEC_bypass();
	#endif
	
	#if SEC_TEST
	#if SEC_CoreNet_BUS
	
	#else
		//SEC_AXI_BUS
		sec_axi_bus();
	#endif
		new_setting();
		uart_printf_buf("SEC TEST start!\n");
		sec_test();
	//	bn_test();
		uart_printf_buf("SEC TEST end!\n");
	#endif
	
	#if FUSE_TEST
		fuse_command_test();
		fuse_DECO_test();
		fuse_otpmk_hw();
		fuse_mpumk_hw();
		fuse_secmon_otpmk_bad_key_sv_test();
	#endif
	
	#if SECMON_TEST
		secmon_nonsecure_state_test();
		secmon_disable_nonsecure_state_test();
	//	secmon_ssm_st_dis_test();		//st is the status secure -> trusted
	//	secmon_reg_test();
	//	secmon_lock_hac_test();
	//	secmon_hac_counter_test();
	//	secmon_acu_sv_test();
	//	secmon_bad_key_sv_test();
	
	//	secmon_rtic_ae_sv_test();		//no rtic
	//	secmon_rtic_mis_sv_test();		//no rtic
	#endif
	#if DRYSTONE_TEST
		uart_printf_buf("Drystone TEST start!\n");
		/*Cache validated DMIPS_MHZ: 2.95425, DMIPS:-, -O3, FPGA*/
		/*Cache validated DMIPS_MHZ: 3.14418, DMIPS:-, -O3, CPU:1.33G, EVB*/
//		set_e0_tlb(0x103F0000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000, valid cache
	//	set_e0_tlb(0x103d0000,0xc0000100,0xe960800f,0xe960803f,0x00000000,0x00000004);//set uart tlb at entry 61
	//	set_e0_tlb(0x103e0000,0xc0000700,0x00000004,0x0000003f,0x00000000,0x00000000);//set ddr0 tlb at entry 62 from 0x0000_0000 to 0x0100_0000, valid cache
		//don't re-config ebc register
	//	for(i=0; i<0x3c; i++){
//		clr_e0_tlb1_papa(0x10000000+(3<<16));
	//	}
	
		main_dhry();
	//	set_e0_tlb(0x10030000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);//set sram0 tlb at entry 63 from 0xe000_0000 to 0xe010_0000, valid cache
	//	clr_e0_tlb1_papa(0x10000000+(0x3F<<16));
//		set_e0_tlb(0x10030000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);
//		clr_e0_tlb1_papa(0x10000000+(0x3F<<16));
		uart_printf_buf("Drystone TEST end!\n");
	#endif
	#if SPI_TEST
		uart_printf_buf("SPI test start. \r\n");
		dspi_main();
		uart_printf_buf("SPI test OK! \r\n");
	#endif
	#if EBC_CLEAR_DOWNLOAD
		//download OEM
		//erase flash
		uart_printf_buf("EBC TEST start!\n");
		set_e0_tlb(	0x10040000, // TLB1, ENTRY 30=1E, NEXT VICTIM 2
					0xc0000800,	// VALID, TID=FE, SIZE=16M(0111)	0xF800_0000-0xF8FF_FFFF(bank0) 16MB	
					(UINT64)( (0x00000000 <<32) | BANK0_ROM_ADDR|0xA ), 	// EPN[31:12], X0, X1, WIMGE
					BANK0_ROM_ADDR|0x3f, 	// RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
					0x00000000, // SGS=0, SLPID=1
					0x00000004  // RPN[35:32]
				);
		ebc_init();
	#if 1
		//all 32M
		daddr_l = 0xF8000000;
		//datalen = 0x2000000;
		datalen = 0x20000;
	#else
		//only the last sector
		//daddr_l = 0xF9F80000;
		daddr_l = 0xFBF80000;	
		datalen = 0x80000;
	#endif
//		for(i=0; i<datalen; i+=0x20000)
//		{
//			uart_printf("EBC Sector Erase addr:%x \n", daddr_l+i);
//			spansionSectorErase(daddr_l+i);
//		}
	
		//copy bin to cpu0_jmp_addr, restore -b D:/Users/ccore/Desktop/S1022_EVB_EBC_BOOT.bin i:0x4e0060000
		//copy bin to cpu0_jmp_addr, restore -b D:/Users/ccore/Desktop/S1022_EVB_EBC_BIN.bin i:0x4e0060000
	#if 0
		//program
	//	daddr_l = 0xF9FE0000;		//128K, for EBC_Boot
	//	daddr_l = 0xF9F80000;		//128K, for SRAM_Inc
		daddr_l = 0xFBFF0000;		//64K, for EBC_Boot
	
	//	datalen = 0x20000;			//128K
	//	cpu0_jmp_addr = 0x00b00000;
	//	daddr_l = 0xFBFF0000;		//64K
		datalen = 0x10000;			//64K
		cpu0_jmp_addr = 0xe0060000;

		for(i=0;i<datalen;i+=4)		//program to ebc_flash
		{
			//32bit/ 16bit  *2
			spansionProgram(daddr_l + i, *(unsigned int*)(cpu0_jmp_addr+i));
			//spansionProgram(daddr_l + i, daddr_l + i);
		}
	#else
//		uart_printf("EBC Sector Program ......\n");
//		for(i=0;i<datalen;i+=4)		//program to ebc_flash
//		{
//			//32bit/ 16bit  *2
//			spansionProgram(daddr_l + i, daddr_l + i);
//		}
		uart_printf("EBC data compare......\n", daddr_l+i);
		for(i=0;i<datalen;i+=4)		//program to ebc_flash
		{
			//32bit/ 16bit  *2
			if(*(unsigned int*)(daddr_l + i) != (daddr_l + i))
			{
				uart_printf("EBC flash check error1: 0x%x must be 0x%x ,but be 0x%x \n", daddr_l + i, daddr_l + i,*(unsigned int*)(daddr_l + i));
		    	while(1);
			}
		}
	#endif	
		uart_printf_buf("EBC TEST end!\n");	
	#endif
		
	#if eMMC_TEST
		uart_printf_buf("emmc TEST start!\n");
		emmc_main();
		uart_printf_buf("emmc TEST end!\n");
	#endif
			
	#if USB_TEST
		uart_printf_buf("USB 3.0 TEST start!\n");
		usb_main();
		uart_printf_buf("USB 3.0 TEST end!\n");
	#endif
		
	#if LCD_TEST
		uart_printf_buf("LCD TEST start!\n");
		lcdc_main();
		uart_printf_buf("LCD TEST end!\n");
	#endif
#if SATA_TEST	
		uart_printf_buf("SATA TEST start!\n");
		sata_test();
		uart_printf_buf("SATA TEST end!\n");	
#endif	
			
		
	#if	NAND_TEST
		uart_printf_buf("NAND flash TEST start!\n");
		mim_main();
		uart_printf_buf("NAND flash TEST end!\n");
	#endif			
	#if PCIE_TEST
		uart_printf("PCIE TEST start.\n");	
		pcie_test();
		uart_printf("PCIE TEST end.\n");
	#endif
	#if GPIO_M0_M1_TEST
	
		GPIO_init();
		W32(GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET,	(reg & 0xfffffffc)|0x03 );		//GPIO_M0 M1 all High
		delay(0x1000);
		W32(GPIO_M_BASE_ADDR	|GPIO_OUT_OFFSET,	(reg & 0xfffffffc)|0x00 );		//GPIO_M0 M1 all Low
		delay(0x1000);
	
	#endif
		//asm ("mfatbl %0" : "=r" (proc_id));
		//printf("Core%lu: Welcome to CodeWarrior!\r\n", proc_id>>5);
		//MFSPR(atbl,ATBL);
	//	MFSPR(atbu,ATBU);
		//printf("ATBU: %lu, ATBL:  %lu \n", atbl,atbu);
	//	MFSPR(atbl,ATBL);
	//	MFSPR(atbu,ATBU);
	//	printf("ATBU: %lu, ATBL:  %lu \n", atbl,atbu);
	}//end while(1)
	while (1) { i++;

	aa = i+1;
	aa+= 10;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("dnh 10,10");
//	IO_WRITE32(0x10000,0xaabbccdd);
//	IO_WRITE32(0x10010,0x12345678);
	asm("nop");
	asm("nop");
	asm("nop");
	} // loop forever
}

