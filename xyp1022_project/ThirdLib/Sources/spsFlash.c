
/*
*****************************************************************************
* Copyright (c) 2012 CCore, Inc.  All rights reserved.
* File	: 	spsFlash.h
* Author: 	gfyou
* Date	: 	2014-03-26
*****************************************************************************
* Description:
* CCFC9000TA EVB EBC flash(Spansion) driver
*****************************************************************************
* Changes log:
* Date:		Comment:
*
*
*****************************************************************************
*/
#include "e500mc_asm.h"
#include "spsFlash.h"

#if 1
void delaySysClk(unsigned int  nClk)
{
	unsigned int i = 0;

	for(i = 0; i < (nClk/4); i++)
	{
		ISYNC;
		//NOP
	}
}
#else
	extern void delaySysClk(unsigned int nClk);
#endif

unsigned int spansionReadID(unsigned int addr)
{
//	unsigned int val=0x0000;
	unsigned short val=0x0000;

	//disable_interrupt();

	spansionChipReset();	//Exit Read ID mode
	//uart_printf_buf("ccfc9000ta Core1 bootloader run successfully. \n");
	delaySysClk(1000);
	*(volatile unsigned short *)spansion_1st_addr=0x00AA;
	*(volatile unsigned short *)spansion_2nd_addr=0x0055;
	*(volatile unsigned short *)spansion_3rd_addr=0x0090;
	//uart_printf_buf("ccfc9000ta Core1 bootloader run successfully. \n");
	delaySysClk(1000);
	val = *(volatile unsigned short *)SPANSION_FLASH_ADDR(addr);
	//uart_printf_buf("ccfc9000ta Core0 bootloader run successfully. \n");
	spansionChipReset();	//Exit Read ID mode
	delaySysClk(1000);
	//enable_interrupt();
	return val;
}


void spansionChipReset()
{
	//UINT8 tmp;

	//disable_interrupt();

	*(volatile unsigned short *)SPANSION_FLASH_ADDR(0)=0x00F0;
	delaySysClk(1000);
	//enable_interrupt();
}

void spansionChipErase()
{
	//disable_interrupt();

	*(volatile unsigned short *)(spansion_1st_addr)=0x00AA;
	*(volatile unsigned short *)(spansion_2nd_addr)=0x0055;
	*(volatile unsigned short *)(spansion_3rd_addr)=0x0080;
	*(volatile unsigned short *)(spansion_4th_addr)=0x00AA;
	*(volatile unsigned short *)(spansion_5th_addr)=0x0055;
	*(volatile unsigned short *)(spansion_6th_addr)=0x0010;
	//enable_interrupt();
	delaySysClk(100);
	spansionWaitFlash();
	delaySysClk(100);
}

void spansionSectorErase(unsigned int addr) //128KB
{
	*(volatile unsigned short *)(spansion_1st_addr)=0x00AA;
	ISYNC;
	*(volatile unsigned short *)(spansion_2nd_addr)=0x0055;
	ISYNC;
	*(volatile unsigned short *)(spansion_3rd_addr)=0x0080;
	ISYNC;
	*(volatile unsigned short *)(spansion_4th_addr)=0x00AA;
	ISYNC;
	*(volatile unsigned short *)(spansion_5th_addr)=0x0055;
	delaySysClk(20);
	ISYNC;
	*(volatile unsigned short *) addr = 0x0030;
	
	//delaySysClk(400);
	ISYNC;
	//enable_interrupt();
	spansionWaitFlash();
	//delaySysClk(200);

}

void spansionProgram(unsigned int addr,unsigned int data)
{
	//disable_interrupt();
	unsigned short shortData = 0;

	shortData = (unsigned short)(data>>16);		//MSB
	*(volatile unsigned short *)(spansion_1st_addr)=0x00AA;
	ISYNC;
	*(volatile unsigned short *)(spansion_2nd_addr)=0x0055;
	ISYNC;
	*(volatile unsigned short *)(spansion_3rd_addr)=0x00A0;
	delaySysClk(20);
	ISYNC;
	*(volatile unsigned short*)addr = shortData;
	ISYNC;
	//delaySysClk(40);
	//enable_interrupt();
	spansionWaitFlash();
	//delaySysClk(100);
	
	
	shortData = (unsigned short)(data&0x0000ffff);
	*(volatile unsigned short *)(spansion_1st_addr)=0x00AA;
	ISYNC;
	*(volatile unsigned short *)(spansion_2nd_addr)=0x0055;
	ISYNC;
	*(volatile unsigned short *)(spansion_3rd_addr)=0x00A0;
	delaySysClk(20);
	ISYNC;
	*(volatile unsigned short*)(addr+2) = shortData;
	ISYNC;
	//delaySysClk(40);
	//enable_interrupt();
	spansionWaitFlash();
	//delaySysClk(100);
}

void spansionWaitFlash()
{
	volatile unsigned short *mp;
	unsigned short d1, d2;

	mp=(volatile unsigned short *)SPANSION_FLASH_BASE;

	//DisableInt();
	d1= (*(volatile unsigned short *)mp)&0x0040;//judge bit6 status
	d2= *(volatile unsigned short *)mp;
	while( d1 !=(d2&0x0040) )
	{
		d1=d2&0x0040;
		d2= *(volatile unsigned short *)mp;
	}
	//EnableInt();
}
