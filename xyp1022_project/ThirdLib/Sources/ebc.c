/*
 * ebc.c
 *
 *  Created on: 2017年10月25日
 *      Author: ccore
 */

#include "uart.h"
#include "ebc.h"
//#include "cpr.h"
//#include "spsFlash.h"
#include "sst_flash.h"
//#include "common.h"
//#include "mpu.h"

#define NEED_DELAY_10CLK	//ISYNC//NOP
//#define uart_printf
#define EBC_ROM_START				1

#define EBC_REG_INIT_TEST           1
#define EBC_SINGLE_TRANSFER_TEST  	0
#define EBC_SINGLE_TIMMING_TEST    0
#define EBC_BURST_TRANSFER_TEST   0
#define EBC_BURST_TIMMING_TEST    0
#define EBC_NOR_FLASH_TEST		1

#define EBC_INT_TEST  0
#if EBC_INT_TEST
	#define EBC_PEI_TEST                  /*in ebc_single_transfer*/
//	#define EBC_TOEI_TEST    1            /*in ebc_devicePaced_transfer*/
//	#define EBC_PYEI_TEST					/*in ebc_single_transfer*/
#endif

unsigned char bin_data[] = {
		0x5a, 0x5a, 0x5a, 0x5a
};
unsigned char bin_data2[] = {
		0xa5, 0xa5, 0xa5, 0xa5
};

/************ebc_isr*************/
void ebc_isr()
{
	unsigned int err_addr;
	unsigned int err_stat;

	// Get the address where an error occurred
	EBCx_CFGADDR = EBCx_BEAR;
	err_addr = EBCx_CFGDATA;
	//MTDCR(EBCx_CFGADDR,EBCx_BEAR);
	//MFDCR(err_addr,EBCx_CFGDATA);
	uart_printf("ebc error addr:0x%x\n",err_addr);

	// Get the status where an error occurred
	//MTDCR(EBCx_CFGADDR,EBCx_BESR);
	//MFDCR(err_stat,EBCx_CFGDATA);
	EBCx_CFGADDR = EBCx_BESR;
	err_stat = EBCx_CFGDATA;
	uart_printf("ebc error stat:0x%x\n",err_stat);

	// Clear EBCx_BESR to allow another error to be logged
	//MTDCR(EBCx_CFGADDR,EBCx_BESR);
	//MTDCR(EBCx_CFGDATA,0x0);
	EBCx_CFGADDR = EBCx_BESR;
	EBCx_CFGDATA = 0x0;

}
/*************************************test function********************************************/
/****ebc_regs_test*******/
#if  EBC_REG_INIT_TEST
void ebc_regs_test()
{
	unsigned int reg_val;

	EBCx_CFGADDR = EBCx_B0CR;
	reg_val = EBCx_CFGDATA;
	//MTDCR(EBCx_CFGADDR,EBCx_B3CR);
	//MFDCR(reg_val,EBCx_CFGDATA);
	if(reg_val!=0)
		uart_printf(" EBCx_B3CR fail \n");

//	*(volatile unsigned int *)(0xEB048000+0) = 0x10;
//	reg_val = *(volatile unsigned int *)(0xEB048000+4);

	EBCx_CFGADDR = EBCx_B0AP;
	reg_val = EBCx_CFGDATA;
	//MTDCR(EBCx_CFGADDR,EBCx_B3AP);
	//MFDCR(reg_val,EBCx_CFGDATA);
	if(reg_val!=0)
		uart_printf(" EBCx_B3AP fail \n");

	EBCx_CFGADDR = EBCx_CFG;
	reg_val = EBCx_CFGDATA;
	//MTDCR(EBCx_CFGADDR,EBCx_CFG);
	//MFDCR(reg_val,EBCx_CFGDATA);
	if(reg_val!=0)
		uart_printf(" EBCx_CFG fail,vaule = 0x%x \n", reg_val);  //0x07c00000

	EBCx_CFGADDR = EBCx_BEAR;
	reg_val = EBCx_CFGDATA;
	//MTDCR(EBCx_CFGADDR,EBCx_BEAR);
	//MFDCR(reg_val,EBCx_CFGDATA);
	if(reg_val!=0)
		uart_printf(" EBCx_BEAR fail \n");

	EBCx_CFGADDR = EBCx_BESR;
	reg_val = EBCx_CFGDATA;
	//MTDCR(EBCx_CFGADDR,EBCx_BESR);
	//MFDCR(reg_val,EBCx_CFGDATA);
	if(reg_val!=0)
		uart_printf(" EBCx_BESR fail \n");

	while(1);
}
#endif
/******ebc_single_transfer********/
#if EBC_SINGLE_TRANSFER_TEST
void ebc_single_transfer()
{
	unsigned int i;
	unsigned int check_cnt=1;
	unsigned int val,tmp=0;
	InstallInterruptHandler0(21,ebc_isr);

	// Set TLB of EBC
	//SetTLB(0,BANK3_RAM_ADDR,TLB0_SIZE_16M,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,BANK3_RAM_ADDR|EBCx_UADDR);
	//SetTLB(0x0, BANK1_RAM_ADDR,TLB0_SIZE_16M,TLB2_I_MASK | TLB2_U_RW   | TLB2_S_RW   | TLB2_G_MASK, BANK1_RAM_ADDR|0x00000001);
	// Set EBCx_BnCR to enable memory in bank n
#ifdef EBC_PEI_TEST
	val = (BANK1_RAM_ADDR|EBCx_BnCR_BS_1M|EBCx_BnCR_BU_RO|EBCx_BnCR_BW_16b);   //EBCx_BESR = 0x42000000
#else
	val = (BANK1_RAM_ADDR|EBCx_BnCR_BS_1M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
#endif
	EBCx_CFGADDR = EBCx_B1CR;
	EBCx_CFGDATA = val;
#ifdef EBC_PYEI_TEST
	val = (EBCx_BnAP_TWT_16|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_OEN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_PEN|\
			EBCx_BnAP_TH_7);		//BEM is only for JN02,other from C9000 ebc config
			//EBCx_BnAP_WBN_2|\     //EBCx_BESR = 0x8a000000
			//EBCx_BnAP_WBF_2|
#else
	val = (EBCx_BnAP_TWT_16|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_OEN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_TH_7);		//BEM is only for JN02,other from C9000 ebc config
			//EBCx_BnAP_WBN_2|\
			//EBCx_BnAP_WBF_2|
/*
	val = (EBCx_BnAP_TWT_32|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_OEN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_TH_7|\
			EBCx_BnAP_WBN_1|\
			EBCx_BnAP_WBF_1);		//BEM is only for JN02,other from C9000 ebc config
*/
#endif
	EBCx_CFGADDR = EBCx_B1AP;
	EBCx_CFGDATA = val;

	while(1)
	{
#if 1
		for(i=0; i<0x20000; i++)
//		for(i=0; i<0x1; i++)
		{
//			i = 0;
			NEED_DELAY_10CLK;
			W32((BANK1_RAM_ADDR+i*4),0x5A5A5A5A);
//			NEED_DELAY_10CLK;
//			i++;
/*
			W32((BANK1_RAM_ADDR+i*4),0x6B6B6B6B);
			NEED_DELAY_10CLK;
			i++;
			W32((BANK1_RAM_ADDR+i*4),0x7C7C7C7C);
			NEED_DELAY_10CLK;
			i++;

			W32((BANK1_RAM_ADDR+i*4),0x8D8D8D8D);
			NEED_DELAY_10CLK;
			i++;
//			W32((BANK1_RAM_ADDR+i*4),0x12345678);
//			W16((BANK1_RAM_ADDR+i*2),0x7b7b);
//			W8((BANK1_RAM_ADDR+i),0x6c);
*/			NEED_DELAY_10CLK;
		}

//		DCBI(0xF7000100);

		for(i=0; i<0x20000; i++)
//		for(i=0; i<0x4; i++)
		{
			tmp = 0;
			tmp = R32(BANK1_RAM_ADDR+i*4);

		if((tmp = R32(BANK1_RAM_ADDR+i*4)) != 0x5A5A5A5A)
//		if((tmp = R32(BANK1_RAM_ADDR+i*4)) != 0x12345678)
//		if((tmp = R16(BANK1_RAM_ADDR+i*2)) != 0x5A5A)
				uart_printf("Wrong address: 0x%x	\n", BANK1_RAM_ADDR+i*4);
//				uart_printf("Wrong data: 0x%x	\n", *(volatile unsigned short *)(BANK1_RAM_ADDR+i*4));

		}


		for(i=0; i<0x20000; i++)
		{
			NEED_DELAY_10CLK;
			W32((BANK1_RAM_ADDR+i*4),0xA5A5A5A5);
			NEED_DELAY_10CLK;
		}


		for(i=0; i<0x20000; i++)
		{
			tmp = 0;
	//		DCBI((BANK1_RAM_ADDR+i*4));
	//		DCBF((BANK1_RAM_ADDR+i*4));
			if((R32(BANK1_RAM_ADDR+i*4)) != 0xA5A5A5A5)
				uart_printf("Wrong address: 0x%x	\n", BANK1_RAM_ADDR+i*4);
		}

		uart_printf("EBC single transfer NO.%d finished.\n",check_cnt);
		check_cnt++;
#else
		for(i=0; i<0x20000; i++)
//		for(i=0; i<0x1; i++)
		{
//			i = 0;
			NEED_DELAY_10CLK;
			W32((BANK1_RAM_ADDR+i*4),0x5A5A5A5A);
			NEED_DELAY_10CLK;
			tmp = R32(BANK1_RAM_ADDR+i*4);
		}
#endif
	}
}

#endif

/********ebc_single_timming_test***********/
#if EBC_SINGLE_TIMMING_TEST
void ebc_single_timming_test()
{
	unsigned int i,reg_val;
	unsigned int twt,csn,oen,th;
	unsigned int check_cnt=1;
	unsigned int val;

	// Set TLB of EBC
	//SetTLB(0,BANK3_RAM_ADDR,TLB0_SIZE_16M,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,BANK3_RAM_ADDR|EBCx_UADDR);
	SetTLB(0x0, BANK1_RAM_ADDR,TLB0_SIZE_16M,TLB2_I_MASK |
				                       TLB2_U_RW   |
				                       TLB2_S_RW   |
				                       TLB2_G_MASK, BANK1_RAM_ADDR|0x00000001);
	// Set EBCx_BnCR to enable memory in bank n
	//MTDCR(EBCx_CFGADDR,EBCx_B3CR);
	//MTDCR(EBCx_CFGDATA,BANK3_RAM_ADDR|EBCx_BxCR_BS_1MB|EBCx_BxCR_BU_RW|EBCx_BxCR_BW_16);
	val = (BANK1_RAM_ADDR|EBCx_BnCR_BS_1M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
	EBCx_CFGADDR = EBCx_B1CR;
	EBCx_CFGDATA = val;

	/*
	MTDCR(EBCx_CFGADDR,EBCx_B3AP);

	MTDCR(EBCx_CFGDATA,EBCx_BnAP_TWT_16|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_OEN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_TH_7);		//BEM is only for JN02,other from C9000 ebc config
			//EBCx_BnAP_WBN_2|\
			//EBCx_BnAP_WBF_2|\
	 */

	for(csn=0,oen=0;csn<4;csn++,oen++)          //NO.216
	{
		for(th=0;th<8;th++)
		{
			for(twt=csn+oen+1;twt<256;twt<<=1)
			{
				//Setup EBCO timming
				reg_val = (twt<<(31-8))|(csn<<(31-13))|(oen<<(31-15))|(th<<(31-22))|EBCx_BnAP_BEM;
				//MTDCR(EBCx_CFGADDR,EBCx_B3AP);
				//MTDCR(EBCx_CFGDATA,reg_val);
				EBCx_CFGADDR = EBCx_B1AP;
				EBCx_CFGDATA = reg_val;

				for(i=0; i<0x20000; i++)
					W32((BANK1_RAM_ADDR+i*4),(0x5A5A5A5A+i));

				for(i=0; i<0x20000; i++)
					if(R32(BANK1_RAM_ADDR+i*4) != (0x5A5A5A5A+i))
						uart_printf("Wrong address: 0x%x	\n", BANK1_RAM_ADDR+i*4);

				for(i=0; i<0x20000; i++)
					W32((BANK1_RAM_ADDR+i*4),(0xA5A5A5A5+i));

				for(i=0; i<0x20000; i++)
					if(R32(BANK1_RAM_ADDR+i*4) != (0xA5A5A5A5+i))
						uart_printf("Wrong address: 0x%x	\n", BANK1_RAM_ADDR+i*4);

				uart_printf("EBC single transfer NO.%d finished.\n",check_cnt);
				uart_printf("EBC timming is twt=%d,csn=%d,oen=%d,th=%d.\n",twt,csn,oen,th);
				check_cnt++;

			}
		}
	}
}
#endif

/********ebc_burst_transfer*************/
#if EBC_BURST_TRANSFER_TEST
void ebc_burst_transfer()
{
	unsigned int i;
	unsigned int check_cnt=1;
	unsigned int val;
	// Set TLB of EBC
	//SetTLB(0,BANK3_RAM_ADDR,TLB0_SIZE_16M,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,BANK3_RAM_ADDR|EBCx_UADDR);
/*	SetTLB(0x0, BANK1_RAM_ADDR,TLB0_SIZE_16M,TLB2_I_MASK |
				                       TLB2_U_RW   |
				                       TLB2_S_RW   |
				                       TLB2_G_MASK, BANK1_RAM_ADDR|0x00000001);
*/	// Set EBCx_BnCR to enable memory in bank n
	val = (BANK1_RAM_ADDR|EBCx_BnCR_BS_1M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
	EBCx_CFGADDR = EBCx_B1CR;
	EBCx_CFGDATA = val;

	val = (EBCx_BnAP_BME|\
			EBCx_BnAP_FWT_8|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_OEN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_TH_7);
			//EBCx_BnAP_BWT_5|
	EBCx_CFGADDR = EBCx_B1AP;
	EBCx_CFGDATA = val;

	while(1)
	{
		for(i=0; i<0x20000; i++)
			W32((BANK1_RAM_ADDR+i*4),0x5A5A5A5A);

		for(i=0; i<0x20000; i++)
			if(R32(BANK1_RAM_ADDR+i*4) != 0x5A5A5A5A)
				uart_printf("Wrong address: 0x%x	\n", BANK1_RAM_ADDR+i*4);

		for(i=0; i<0x20000; i++)
			W32((BANK1_RAM_ADDR+i*4),0xA5A5A5A5);

		for(i=0; i<0x20000; i++)
			if(R32(BANK1_RAM_ADDR+i*4) != 0xA5A5A5A5)
				uart_printf("Wrong address: 0x%x	\n", BANK1_RAM_ADDR+i*4);

		uart_printf("EBC single transfer NO.%d finished.\n",check_cnt);
		check_cnt++;
	}
}
#endif

/**********ebc_burst_timming_test****************/
#if EBC_BURST_TIMMING_TEST
void ebc_burst_timming_test()
{
	unsigned int i,reg_val;
	unsigned int fwt,csn,oen,th;
	unsigned int check_cnt=1;
	unsigned int val;

	InstallInterruptHandler0(16,ebc_isr);
	// Set TLB of EBC
	//SetTLB(0,BANK3_RAM_ADDR,TLB0_SIZE_16M,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,BANK3_RAM_ADDR|EBCx_UADDR);
	SetTLB(0x0, BANK1_RAM_ADDR,TLB0_SIZE_16M,TLB2_I_MASK |
					                       TLB2_U_RW   |
					                       TLB2_S_RW   |
					                       TLB2_G_MASK, BANK1_RAM_ADDR|0x00000001);
	// Set EBCx_BnCR to enable memory in bank n
	//MTDCR(EBCx_CFGADDR,EBCx_B3CR);
	//MTDCR(EBCx_CFGDATA,BANK3_RAM_ADDR|EBCx_BxCR_BS_1MB|EBCx_BxCR_BU_RW|EBCx_BxCR_BW_16);
	val = (BANK1_RAM_ADDR|EBCx_BnCR_BS_1M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
	EBCx_CFGADDR = EBCx_B1CR;
	EBCx_CFGDATA = val;

	//MTDCR(EBCx_CFGADDR,EBCx_B3AP);
	//MTDCR(EBCx_CFGDATA,EBCx_BnAP_BME|\
			EBCx_BnAP_FWT_8|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_OEN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_TH_7);
			//EBCx_BnAP_BWT_5|\

	for(csn=1,oen=1;csn<4;csn++,oen++)                        //NO.120
	{
		for(th=4;th<8;th++)
		{
			for(fwt=csn+oen+1;fwt<32;fwt<<=1)
			{
				//Setup EBCO timming
				reg_val = (fwt<<(31-5))|(csn<<(31-13))|(oen<<(31-15))|(th<<(31-22))|EBCx_BnAP_BEM;
				//MTDCR(EBCx_CFGADDR,EBCx_B3AP);
				//MTDCR(EBCx_CFGDATA,reg_val);
				EBCx_CFGADDR = EBCx_B1AP;
				EBCx_CFGDATA = reg_val;

				for(i=0; i<0x20000; i++)
				{
					NEED_DELAY_10CLK;
					W32((BANK1_RAM_ADDR+i*4),(0x5A5A5A5A+i));
					NEED_DELAY_10CLK;
				}
				for(i=0; i<0x20000; i++)
					if(R32(BANK1_RAM_ADDR+i*4) != (0x5A5A5A5A+i))
						uart_printf("Wrong address: 0x%x	\n", (BANK1_RAM_ADDR+i*4));

				for(i=0; i<0x20000; i++)
				{
					NEED_DELAY_10CLK;
					W32((BANK1_RAM_ADDR+i*4),(0xA5A5A5A5+i));
					NEED_DELAY_10CLK;
				}

				for(i=0; i<0x20000; i++)
					if(R32(BANK1_RAM_ADDR+i*4) != (0xA5A5A5A5+i))
						uart_printf("Wrong address: 0x%x	\n", (BANK1_RAM_ADDR+i*4));
				uart_printf("EBC burst transfer NO.%d finished.\n",check_cnt);
				uart_printf("EBC timming is fwt=%d,csn=%d,oen=%d,th=%d.\n",fwt,csn,oen,th);
				check_cnt++;
			}
		}
	}
}
#endif


/*
Wrong address: 0xf7048000
EBC burst transfer NO.60 finished.
EBC timming is fwt=24,csn=1,oen=1,th=4.

Wrong address: 0xf7048000
EBC burst transfer NO.64 finished.
EBC timming is fwt=24,csn=1,oen=1,th=5.

Wrong address: 0xf7048000
EBC burst transfer NO.72 finished.
EBC timming is fwt=24,csn=1,oen=1,th=7.

Wrong address: 0xf7048000
EBC burst transfer NO.114 finished.
EBC timming is fwt=28,csn=3,oen=3,th=5.
*/
/*****interrupt****ebc_devicePaced_transfer*****************/
#if EBC_TOEI_TEST
void ebc_devicePaced_transfer()
{
	unsigned int i;
	unsigned int val;

	InstallInterruptHandler0(16,ebc_isr);
	// Set TLB of EBC
	//SetTLB(0,BANK3_RAM_ADDR,TLB0_SIZE_16M,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,BANK3_RAM_ADDR|EBCx_UADDR);
	SetTLB(0x0, BANK1_RAM_ADDR,TLB0_SIZE_16M,TLB2_I_MASK |
					                       TLB2_U_RW   |
					                       TLB2_S_RW   |
					                       TLB2_G_MASK, BANK1_RAM_ADDR|0x00000001);
	// Set EBCx_B3CR to enable memory in bank 3
	//MTDCR(EBCx_CFGADDR,EBCx_B3CR);
	//MTDCR(EBCx_CFGDATA,BANK3_RAM_ADDR|EBCx_BxCR_BS_1MB|EBCx_BxCR_BU_RW|EBCx_BxCR_BW_16);
	val = (BANK1_RAM_ADDR|EBCx_BnCR_BS_1M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
	EBCx_CFGADDR = EBCx_B1CR;
	EBCx_CFGDATA = val;

	/*
	MTDCR(EBCx_CFGADDR,EBCx_B3AP);
	MTDCR(EBCx_CFGDATA,EBCx_BnAP_BME|\
			EBCx_BnAP_RE|\
			EBCx_BnAP_FWT_8|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_OEN_3|
			EBCx_BnAP_TH_7);
*/
	val = (EBCx_BnAP_TWT_16|\
			EBCx_BnAP_RE|\
			EBCx_BnAP_CSN_3|\
			EBCx_BnAP_OEN_3|\
			EBCx_BnAP_BEM|\
			EBCx_BnAP_TH_7);		//BEM is only for JN02,other from C9000 ebc config
	EBCx_CFGADDR = EBCx_B1AP;
	EBCx_CFGDATA = val;


	//MTDCR(EBCx_CFGADDR,EBCx_CFG);
	//MTDCR(EBCx_CFGDATA,EBCx_CFG_RTC_16);
	EBCx_CFGADDR = EBCx_CFG;
	EBCx_CFGDATA = EBCx_CFG_RTC_16;
	//Read CFG to verify
	//MTDCR(EBCx_CFGADDR,EBCx_CFG);
	//MFDCR(i,EBCx_CFGDATA);
	EBCx_CFGADDR = EBCx_CFG;
	i = EBCx_CFGDATA;

	while(1){
#ifdef EBC_TOEI_TEST                     //J1.040,      U1S.EBC_READY,触发
		W32((BANK1_RAM_ADDR),(0x12345678));
		while(1)
			if(R32(BANK1_RAM_ADDR) != 0x12345678)
						asm("nop");;
#endif
	for(i=0; i<0x20000; i++)
		W32((BANK1_RAM_ADDR+i*4),(0x5A5A5A5A));

	for(i=0; i<0x20000; i++)
		if(R32(BANK1_RAM_ADDR+i*4) != 0x5A5A5A5A)
			asm("nop");

	for(i=0; i<0x20000; i++)
		W32((BANK1_RAM_ADDR+i*4),(0xA5A5A5A5));

	for(i=0; i<0x20000; i++)
		if(R32(BANK1_RAM_ADDR+i*4) != 0xA5A5A5A5)
			asm("nop");
	}
}

#endif

void ebc_init()
{
	unsigned int val;

	//SetTLB(0x0, BANK0_ROM_ADDR,TLB0_SIZE_16M,TLB2_I_MASK | TLB2_U_RW   |  TLB2_S_RW   | TLB2_G_MASK, BANK0_ROM_ADDR|0x00000001);

	//SetTLB(0x0, BANK1_RAM_ADDR,TLB0_SIZE_16M,TLB2_I_MASK | TLB2_U_RW   | TLB2_S_RW   | TLB2_G_MASK, BANK1_RAM_ADDR|0x00000001);

	/************** Bank 0 ***************/
	//Configure EBCx_B0CR
	//val = (BANK0_ROM_ADDR|EBCx_BnCR_BS_16M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_32b);
#if 0
//	val = (BANK0_ROM_ADDR|EBCx_BnCR_BS_16M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
	val = (BANK0_ROM_ADDR|EBCx_BnCR_BS_64M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);

	EBCx_CFGADDR = 0;		//0
	EBCx_CFGDATA = val;
	//Configure EBCx_B0AP
	//val = (EBCx_BnAP_BME|EBCx_BnAP_FWT_4|EBCx_BnAP_BWT_2|EBCx_BnAP_WBN_1|EBCx_BnAP_WBF_1|EBCx_BnAP_CSN_1|EBCx_BnAP_OEN_1|EBCx_BnAP_TH_3);

	val = (EBCx_BnAP_BME|EBCx_BnAP_FWT_4|EBCx_BnAP_BWT_2|EBCx_BnAP_WBN_1|EBCx_BnAP_WBF_1|EBCx_BnAP_CSN_1|EBCx_BnAP_OEN_1|EBCx_BnAP_TH_3);
//	val = (EBCx_BnAP_TWT_16|EBCx_BnAP_CSN_1|EBCx_BnAP_OEN_1|EBCx_BnAP_WBN_1|EBCx_BnAP_WBF_1|EBCx_BnAP_TH_7);

	EBCx_CFGADDR = 0x10;	//0x10
	EBCx_CFGDATA = val;
#else
	val = (BANK0_ROM_ADDR|EBCx_BnCR_BS_64M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
	EBCx_CFGADDR = 0;
	EBCx_CFGDATA = val;
	//Configure EBCx_B0AP
	//val = (EBCx_BnAP_BME|EBCx_BnAP_FWT_4|EBCx_BnAP_BWT_2|EBCx_BnAP_WBN_1|EBCx_BnAP_WBF_1|EBCx_BnAP_CSN_1|EBCx_BnAP_OEN_1|EBCx_BnAP_TH_3);
	val = (EBCx_BnAP_BME|EBCx_BnAP_FWT_8|EBCx_BnAP_BWT_5|EBCx_BnAP_WBN_2|EBCx_BnAP_WBF_2|EBCx_BnAP_CSN_2|EBCx_BnAP_OEN_2|EBCx_BnAP_TH_7);
	EBCx_CFGADDR = 0x10;
	EBCx_CFGDATA = val;
#endif

#if 0
	/************** Bank 1 ***************/
	//Configure EBCx_B1CR
	val = (BANK1_RAM_ADDR|EBCx_BnCR_BS_16M|EBCx_BnCR_BU_RW|EBCx_BnCR_BW_16b);
	EBCx_CFGADDR = 1;		//1
	EBCx_CFGDATA = val;
	//Configure EBCx_B1AP
	val = (EBCx_BnAP_BME|EBCx_BnAP_FWT_8|EBCx_BnAP_BWT_5|EBCx_BnAP_WBN_2|EBCx_BnAP_WBF_2|EBCx_BnAP_CSN_2|EBCx_BnAP_OEN_2|EBCx_BnAP_BEM|EBCx_BnAP_TH_7);
	EBCx_CFGADDR = 0x11;	//0x11
	EBCx_CFGDATA = val;
#endif
}

void sst_flash_test()
{
	int i;
	unsigned int val;
	short mfid;
	short did;
	char tmp1, tmp2;
	volatile void *saddr_l,*daddr_l;
    long slen;

	mfid = sst_read_id(0);	//00bf:	byte0 0xbf, byte1 0x00
	did  = sst_read_id(1);	//236d:	byte0 0x6d, byte1 0x23
	tmp1 = *((unsigned char *)&mfid);		//if tmp1==0x00, big_end	if tmp1==0xbf, lit_end(C0, only can use lit_end)
	tmp2 = *((unsigned char *)&did);		//if tmp2==0x23, big_end	if tmp2==0x6d, lit_end(C0, only can use lit_end)
//	sst_chip_erase(FLASH_BASE_ADDR);
	if((tmp1 != 0xbf) || (tmp2 != 0x6d))
		while(1);

	//saddr_l=(volatile unsigned *)0xe0320000;
	daddr_l=(volatile unsigned *)BANK0_ROM_ADDR;
	slen = sizeof(bin_data);

	//sst_sector_erase(BANK0_ROM_ADDR);
	sst_chip_erase();
	for(i=0; i<0x20000; i+=4)
	{
		val = *(volatile unsigned int *)(daddr_l+i);
		if(val != 0xffffffff){
			while(1);			//maybe 0xfffbffff
		}
	}

#if EBC_ROM_START
	saddr_l=(UINT8*)0x8000;		//load .bin to 0x8000, size about 0x4000
	for(i=0;i<0x4000;i+=4)		//copy .bin from ram to ebc_flash
	{
		val = *(volatile unsigned int *)saddr_l;
	//	val = 0xffffffff;
		sst_program(daddr_l,val);			//ebc_end_lit
		sst_program(daddr_l+2,val>>16);
//		sst_program(daddr_l,val>>16);
//		sst_program(daddr_l+2,val);
		saddr_l+=4;
		daddr_l+=4;
	}

	daddr_l=(volatile unsigned int *)BANK0_ROM_ADDR;
	for(i=0; i<0x4000; i+=4)		//size add to 0x200000
	{
		val = *(volatile unsigned int *)(daddr_l+i);
		if(val == 0xffffffff){
			while(1);
		}
	}
	while(1);
#else
	saddr_l=0x40001000;
	memset(0x40001000, 0, 0x100000);

	for(i = 0; i < 0x100000; i+=4)
	{
		*(unsigned int *)(0x40001000+i) = i;
	}

	for(i=0;i<0x100000;i+=4)		//size add to 0x200000
	{
		val = *(volatile unsigned int *)saddr_l;
	//	val = 0xffffffff;
		sst_program(daddr_l,val);		//ebc_end_lit
		sst_program(daddr_l+2,val>>16);
//		sst_program(daddr_l,val>>16);
//		sst_program(daddr_l+2,val);
		saddr_l+=4;
		daddr_l+=4;
	}

	if(memcmp(BANK0_ROM_ADDR, 0x40001000, 0x100000))
		while(1);
#endif
	daddr_l=(volatile unsigned int *)BANK0_ROM_ADDR;
	for(i=0;i<0x20000;i+=0x1000)
	{
		sst_sector_erase(daddr_l+i);
	}

	for(i=0; i<0x20000; i+=4)
	{
		val = *(volatile unsigned int *)(daddr_l+i);
		if(val != 0xffffffff){
			while(1);				//maybe 0xfffbffff
		}
	}

	while(1);

}
void ebc_test()
{
	unsigned int i;
	unsigned int tmp;
#if	EBC_REG_INIT_TEST
	ebc_regs_test();
#endif

#if	EBC_SINGLE_TRANSFER_TEST
	ebc_single_transfer();
#endif

#if	EBC_SINGLE_TIMMING_TEST
	ebc_single_timming_test();
#endif

#if	EBC_BURST_TRANSFER_TEST
	ebc_burst_transfer();
#endif

#if	EBC_BURST_TIMMING_TEST
	ebc_burst_timming_test();
#endif


#if EBC_TOEI_TEST
	ebc_devicePaced_transfer();
#endif

#if EBC_NOR_FLASH_TEST
	unsigned int val;
	unsigned int daddr_l = BANK0_ROM_ADDR;
	ebc_init();

	sram_chk(BANK1_RAM_ADDR,0x40000);			//EBC_SRAM check
	sst_flash_test();							//EBC_FLASH test

#endif
	while(1);
}

#if 0
/********mpu***define*********/
#define MPU_REGS_TEST    0
/*********************************************/
#if MPU_REGS_TEST
void MPU_regs_test(void)
{
	unsigned int reg_val;

	//MFDCR(reg_val,MPU_RGN0_H);
#if 0
	reg_val = MPU_RGN0_H;
	if(reg_val!=0)
		uart_printf(" MPU_RGN0_H fail \n");

	//MFDCR(reg_val,MPU_RGN0_L);
	reg_val = MPU_RGN0_L;
	if(reg_val!=0)
		uart_printf(" MPU_RGN0_L fail \n");

	//MFDCR(reg_val,MPU_RGN1_H);
	reg_val = MPU_RGN1_H;
	if(reg_val!=0)
		uart_printf(" MPU_RGN1_H fail \n");

	//MFDCR(reg_val,MPU_RGN1_L);
	reg_val = MPU_RGN1_L;
	if(reg_val!=0)
		uart_printf(" MPU_RGN1_L fail \n");

	//MFDCR(reg_val,MPU_RGN2_H);
	reg_val = MPU_RGN2_H;
	if(reg_val!=0)
		uart_printf(" MPU_RGN2_H fail \n");

	//MFDCR(reg_val,MPU_RGN2_L);
	reg_val = MPU_RGN2_L;
	if(reg_val!=0)
		uart_printf(" MPU_RGN2_L fail \n");

	//MFDCR(reg_val,MPU_RGN3_H);
	reg_val = MPU_RGN3_H;
	if(reg_val!=0)
		uart_printf(" MPU_RGN3_H fail \n");

	//MFDCR(reg_val,MPU_RGN3_L);
	reg_val = MPU_RGN3_L;
	if(reg_val!=0)
		uart_printf(" MPU_RGN3_L fail \n");

	//MFDCR(reg_val,MPU_SEED);
	reg_val = MPU_SEED;
	if(reg_val!=0)
		uart_printf(" MPU_SEED fail \n");

	//MFDCR(reg_val,MPU_SR);
	reg_val = MPU_SR;
	if(reg_val!=0)
		uart_printf(" MPU_SR fail \n");

	//MFDCR(reg_val,MPU_CR);
	reg_val = MPU_CR;
	if(reg_val!=0)
		uart_printf(" MPU_CR fail \n");
#endif
/*RGN0*/
	reg_val = MPU_RGN0_H;
	uart_printf(" MPU_RGN0_H = 0x%x \r\n", MPU_RGN0_H);
	reg_val = MPU_RGN0_L;
	uart_printf(" MPU_RGN0_L = 0x%x \r\n", MPU_RGN0_L);
/*RGN1*/
	reg_val = MPU_RGN1_H;
	uart_printf(" MPU_RGN1_H = 0x%x \r\n", MPU_RGN1_H);
	reg_val = MPU_RGN1_L;
	uart_printf(" MPU_RGN1_L = 0x%x \r\n", MPU_RGN1_L);
/*RGN2*/
	reg_val = MPU_RGN2_H;
	uart_printf(" MPU_RGN2_H = 0x%x \r\n", MPU_RGN2_H);
	reg_val = MPU_RGN2_L;
	uart_printf(" MPU_RGN2_L = 0x%x \r\n", MPU_RGN2_L);
/*RGN3*/
	reg_val = MPU_RGN3_H;
	uart_printf(" MPU_RGN3_H = 0x%x \r\n", MPU_RGN3_H);
	reg_val = MPU_RGN3_L;
	uart_printf(" MPU_RGN3_L = 0x%x \r\n", MPU_RGN3_L);
/*RGN4*/
	reg_val = MPU_RGN4_H;
	uart_printf(" MPU_RGN4_H = 0x%x \r\n", MPU_RGN4_H);
	reg_val = MPU_RGN4_L;
	uart_printf(" MPU_RGN4_L = 0x%x \r\n", MPU_RGN4_L);
/*RGN5*/
	reg_val = MPU_RGN5_H;
	uart_printf(" MPU_RGN5_H = 0x%x \r\n", MPU_RGN5_H);
	reg_val = MPU_RGN5_L;
	uart_printf(" MPU_RGN5_L = 0x%x \r\n", MPU_RGN5_L);
/*RGN6*/
	reg_val = MPU_RGN6_H;
	uart_printf(" MPU_RGN6_H = 0x%x \r\n", MPU_RGN6_H);
	reg_val = MPU_RGN6_L;
	uart_printf(" MPU_RGN6_L = 0x%x \r\n", MPU_RGN6_L);
/*RGN7*/
	reg_val = MPU_RGN7_H;
	uart_printf(" MPU_RGN7_H = 0x%x \r\n", MPU_RGN7_H);
	reg_val = MPU_RGN7_L;
	uart_printf(" MPU_RGN7_L = 0x%x \r\n", MPU_RGN7_L);
/*SR*/
	reg_val = MPU_SR;
	uart_printf(" MPU_SR = 0x%x \r\n", MPU_SR);
/*CR*/
	reg_val = MPU_CR;
	uart_printf(" MPU_CR = 0x%x \r\n", MPU_CR);

	uart_printf(" MPU REG test over------\n");
	while(1);
}
#endif
/******MPU_test******/
void MPU_test(void)
{
	unsigned int mpu_value;

	//Register Test
#if MPU_REGS_TEST
	MPU_regs_test();
#endif

#if 1
	//Set Register
	MPU_RGN0_H = MPU_TESTADDR0_H;
	MPU_RGN0_L = MPU_TESTADDR0_L;

	MPU_RGN1_H = MPU_TESTADDR1_H;
	MPU_RGN1_L = MPU_TESTADDR1_L;

	MPU_RGN2_H = MPU_TESTADDR2_H;
	MPU_RGN2_L = MPU_TESTADDR2_L;

	MPU_RGN3_H = MPU_TESTADDR3_H;
	MPU_RGN3_L = MPU_TESTADDR3_L;

	MPU_RGN4_H = MPU_TESTADDR4_H;
	MPU_RGN4_L = MPU_TESTADDR4_L;

	MPU_RGN5_H = MPU_TESTADDR5_H;
	MPU_RGN5_L = MPU_TESTADDR5_L;

	MPU_RGN6_H = MPU_TESTADDR6_H;
	MPU_RGN6_L = MPU_TESTADDR6_L;

	MPU_RGN7_H = MPU_TESTADDR7_H;
	MPU_RGN7_L = MPU_TESTADDR7_L;



	//*(unsigned int *)(0xe020205C) = 0x400;
	MPU_SEED = MPU_SEED_VALUE;
	//Wait for Key Done
	do{
		//MFDCR(mpu_value,MPU_SR);
		mpu_value = MPU_SR;
	}while(mpu_value!=MPU_KEYDONE_FLAG);
	//Set MPU CR
	//MTDCR(MPU_CR,0x1ff);
//	MPU_CR = 0xaaaa;
	MPU_CR = 0x5555;
//	MPU_CR = 0xffff;
//	MPU_CR = 0x0;
#endif
//	mpu_value = MPU_CR;
//	mpu_value |= MPU_RGN0_GLOBAL_EN_FLAG;
//	MPU_CR = mpu_value;

//	MPU_FIR = 0x1;
//	MPU_FCR = 0xAAAAAAAA;
//	MPU_FAR = 0xF7000000;
//	MPU_FDR = 0xA5A5A5A5;
//	MPU_FSR							(*(volatile unsigned int *)(MPU_BASEADDRESS+0x5C))
//	MPU_C0DR						(*(volatile unsigned int *)(MPU_BASEADDRESS+0x60))
//	MPU_C0DIER						(*(volatile unsigned int *)(MPU_BASEADDRESS+0x64))
//	MPU_C0_SRAMDisR					(*(volatile unsigned int *)(MPU_BASEADDRESS+0x68))





//	MPU_RGN0_H = 0xF77FFFFC;
//	MPU_RGN0_L = 0xF7000000;



//	while(1);
}
#endif
