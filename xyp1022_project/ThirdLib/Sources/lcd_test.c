/*******************************************************
 * File: uart_test.c
 * Date: 2013/05/07
 * Author: YQin@ccore_sys
 ******************************************************/

#include "lcd.h"

#define CCM_AXI_IO_MUL 0xEB009028
 
unsigned int LCD_HEIGHT = 320;
unsigned int LCD_WIDTH = 240;
unsigned int lcdc_addr = 0xe0070000;
//unsigned int lcdc_addr = 0x00000000;
/*
--------时序相关寄存器--------
----800*480 ALIENEK----
(HTR_ADDR=0x30583228);//800
(VTR1_ADDR= 0x00200d03);  
(VTR2_ADDR= 0x000001e0);//480
(PCTR_ADDR= 0x0000001d); //

----1024*600 W101----
(HTR_ADDR=0x64644078);
(VTR1_ADDR= 0x000f050f);  
(VTR2_ADDR= 0x000000258);
(PCTR_ADDR= 0x00000004); 
----320*240 TD35 ----
(HTR_ADDR=0x06150f35);//240
(VTR1_ADDR= 0x00040405);  //test vsw vbp vfp
(VTR2_ADDR= 0x00000140);//320
(PCTR_ADDR= 0x00000003); //24M
----480*272 H43 ----
(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
(VTR2_ADDR= 0x00000110);// LPP=272
(PCTR_ADDR= 0x00000051); //7M

*/
void LCD_Display_SB(unsigned int bpp)
{
	unsigned int horizon_words = LCD_WIDTH/(32/bpp);
	unsigned int i,j;
	unsigned int color[8];
	for(i=0;i<8;i++)
	{
		if(i%2==0)
			color[i]=0x00000000;
	}
	switch(bpp)
	{
		case 1:{
				color[1]=0xffffffff;
				color[3]=0xffffffff;
				color[5]=0xffffffff;
				color[7]=0xffffffff;
				break;
			}
		case 2:{
				color[1]=0x55555555;
				color[3]=0xaaaaaaaa;
				color[5]=0xffffffff;
				color[7]=0x55555555;
				break;
			}
		case 4:{
				color[1]=0x11111111;
				color[3]=0x22222222;
				color[5]=0x33333333;
				color[7]=0x11111111;
				break;
			}
		case 8:{

				color[1]=0x01010101;
				color[3]=0x02020202;
				color[5]=0x03030303;
				color[7]=0x01010101;
				break;
			}	
	}

	for(i=0;i<LCD_HEIGHT;i++)
	{
		for(j=0;j<horizon_words;j++)
		{
			*((volatile unsigned int *)(lcdc_addr + (i*horizon_words+j)*4)) = color[i/(LCD_HEIGHT/8)];
		}	
	}		
}

void LCD_Display_FB(unsigned int bpp)
{
	unsigned int horizon_words = LCD_WIDTH/(32/bpp);
	unsigned int i,j;
	unsigned int color[8];
	unsigned int start_encode_pixel;
	for(i=0;i<8;i++)
	{
		if(i%2==0)
			color[i]=0x00000000;
	}
	switch(bpp)
	{
		case 1:{
				color[1]=0xffffffff;
				color[3]=0xffffffff;
				color[5]=0xffffffff;
				color[7]=0xffffffff;
				start_encode_pixel=0x20;
				break;
			}
		case 2:{
				color[1]=0x55555555;//1
				color[3]=0xaaaaaaaa;//2
				color[5]=0xffffffff;//3
				color[7]=0x55555555;//1
				start_encode_pixel=0x20;
				break;
			}
		case 4:{
				color[1]=0x11111111;
				color[3]=0x22222222;
				color[5]=0x33333333;
				color[7]=0x11111111;
				start_encode_pixel=0x40;
				break;
			}
		case 8:{

				color[1]=0x01010101;
				color[3]=0x02020202;
				color[5]=0x03030303;
				color[7]=0x01010101;
				start_encode_pixel=0x400;
				break;
			}	
	}

	*((volatile unsigned int *)(lcdc_addr )) = 0x07e0001f;//绿色1 蓝色0
	*((volatile unsigned int *)(lcdc_addr+8 )) =0xf8000000; //红色3  黑色2

	for(i=0;i<LCD_HEIGHT;i++)
	{
		for(j=0;j<horizon_words;j++)
		{
			*((volatile unsigned int *)(lcdc_addr + start_encode_pixel + (i*horizon_words+j)*4)) = color[i/(LCD_HEIGHT/8)];
		}	
	}	
}

void Lcd_H43_SB_1bpp()
{
    (CR1_ADDR=0x00020f90);
    EXT_ADDR = 0x4;
	/***********时序相关寄存器**********/

	/*480*272 H43*/
	(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
	(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
	(VTR2_ADDR= 0x00000110);// LPP=272
	(PCTR_ADDR= 0x00000051); //7M
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW
	
	
	(DBAR_ADDR= lcdc_addr);
	(CR1_ADDR=0x00020d83);
	(PALT_ADDR = 0xF8000000);//红 黑
	LCD_Display_SB(1);
}

void Lcd_H43_FB_1bpp()
{

	(CR1_ADDR=0x00028d80);
	 EXT_ADDR = 0x4;
	/***********时序相关寄存器**********/
	/*480*272 H43*/
	(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
	(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
	(VTR2_ADDR= 0x00000110);// LPP=272
	(PCTR_ADDR= 0x00000051); //7M
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW

	(DBAR_ADDR= lcdc_addr);
	(PALT_ADDR= 0x0);
	(CR1_ADDR=0x00028d83);
	LCD_Display_FB(1);
		
}

void Lcd_H43_SB_2bpp()
{
	(CR1_ADDR=0x00020d84);
	 EXT_ADDR = 0x4;
	
	/***********时序相关寄存器**********/
	/*480*272 H43*/
	(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
	(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
	(VTR2_ADDR= 0x00000110);// LPP=272
	(PCTR_ADDR= 0x00000051); //7M
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW
	
	(DBAR_ADDR= lcdc_addr);
	(CR1_ADDR=0x00020d87);

	(*(volatile unsigned int *)(LCD_BASE + 0x200)) = 0x07e0001f;//绿色1 蓝色0
	(*(volatile unsigned int *)(LCD_BASE + 0x204)) = 0xf8000000;//红色3  黑色2
	
	 LCD_Display_SB(2);
}

void Lcd_H43_FB_2bpp()
{
	(CR1_ADDR=0x00028d84);
	EXT_ADDR = 0x4;
	/***********时序相关寄存器**********/
	/*480*272 H43*/
	(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
	(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
	(VTR2_ADDR= 0x00000110);// LPP=272
	(PCTR_ADDR= 0x00000051); //7M
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW
	
	(DBAR_ADDR= lcdc_addr);
	(CR1_ADDR=0x00028d87);
	
	LCD_Display_FB(2);
}


void Lcd_H43_SB_4bpp()
{
	(CR1_ADDR=0x00020d88);
	 EXT_ADDR = 0x4;
	
	/***********时序相关寄存器**********/
	/*480*272 H43*/
	(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
	(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
	(VTR2_ADDR= 0x00000110);// LPP=272
	(PCTR_ADDR= 0x00000051); //7M
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW
	
	
	(DBAR_ADDR= lcdc_addr);
	(CR1_ADDR=0x00020d8b);

	(*(volatile unsigned int *)(LCD_BASE + 0x200)) = 0x07e0001f;//绿色1 蓝色0
	(*(volatile unsigned int *)(LCD_BASE + 0x204)) = 0xf8000000;//红色3  黑色2
	
	LCD_Display_SB(4);
}

void Lcd_H43_SB_8bpp()
{
	(CR1_ADDR=0x00020d8c);
	 EXT_ADDR = 0x4;
	
	/***********时序相关寄存器**********/
	 /*----320*240 TD35 ----*/
	 (HTR_ADDR=0x06150f35);//240
	 (VTR1_ADDR= 0x00040405);  //test vsw vbp vfp
	 (VTR2_ADDR= 0x00000140);//320
	 (PCTR_ADDR= 0x00000003); //24M
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW
	
	
	(DBAR_ADDR= lcdc_addr);
	(CR1_ADDR=0x00020d8f);
	
	(*(volatile unsigned int *)(LCD_BASE + 0x200)) = 0x07e0001f;//绿色1 蓝色0
	(*(volatile unsigned int *)(LCD_BASE + 0x204)) = 0xf8000000;//红色3  黑色2
	LCD_Display_SB(8);
}

void Lcd_H43_SB_16bpp() // 不需要调色盘
{
	unsigned int horizon_words;
	unsigned int i,j;
	unsigned int color[8];
	(CR1_ADDR=0x00020d90);
	EXT_ADDR = 0x4;
	
	/***********时序相关寄存器**********/
//	/*480*272 H43*/
//	(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
//	(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
//	(VTR2_ADDR= 0x00000110);// LPP=272
//	(PCTR_ADDR= 0x00000051); //7M
	
	 /*----320*240 TD35 ----*/
	 (HTR_ADDR=0x06150f35);//240
	 (VTR1_ADDR= 0x00040405);  //test vsw vbp vfp
	 (VTR2_ADDR= 0x00000140);//320
	 (PCTR_ADDR= 0x00000003); //24M
	 
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW
	
	(DBAR_ADDR= lcdc_addr);
	(CR1_ADDR=0x00020d93);
	

	horizon_words = LCD_WIDTH/(32/16);
	for(i=0;i<8;i++)
	{
		if(i%2==0)
			color[i]=0x001f001f;
	}
	
	color[1]=0x07e007e0;
	color[3]=0x00000000;
	color[5]=0xf800f800;
	color[7]=0x07e007e0;

	for(i=0;i<LCD_HEIGHT;i++)
	{
		for(j=0;j<horizon_words;j++)
		{
			*((volatile unsigned int *)(lcdc_addr + (i*horizon_words+j)*4)) = color[i/(LCD_HEIGHT/8)];
		}	
	}		
}

void Register_BitSet(volatile unsigned int *reg, unsigned char pos,unsigned char flag)
{
	unsigned char buf[100];
	sprintf(buf,"reg_addr:0x%x ,reg_val:0x%x\n",reg,*reg);
	uart_printf_buf(buf);
	if(flag==0)
	{
		*reg &= ~(1 << pos);
	}
	else
	{
		*reg |= (1<<pos);
	}
}
void Lcd_H43_CR1_Test() // 不需要调色盘
{
	unsigned int horizon_words;
	unsigned int i,j;
	unsigned int color[8];
	unsigned char buf[100];
	unsigned char bpp=16;
	(CR1_ADDR=0x00020d9c);
	EXT_ADDR = 0x0;
	
	/***********时序相关寄存器**********/
	/*480*272 H43*/
	(HTR_ADDR=  0x4B281e05);// HSP=600 HSW=600-40-5-480=0    HBP=40 PPL=480/16   HFP=5
	(VTR1_ADDR= 0x0008080c);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-272=12 VSP=300  
	(VTR2_ADDR= 0x00000110);// LPP=272
//	(VTR1_ADDR= 0x000808bb);  // VBP=8 VFP=8  VSW=VSP-VBP-VFP-LPP=300-8-8-100=84 VSP=300  
//	(VTR2_ADDR= 0x00000064);// LPP=100
	(PCTR_ADDR= 0x00000005); //	71/7=10
	/***********中断相关寄存器**********/
	(ISR_ADDR= 0x00000fff);
	(IMR_ADDR= 0x00000000);
	(ISCR_ADDR= 0x00000007);   //VSW
	
	(DBAR_ADDR= lcdc_addr);
	
	for(i=0;i<8;i++)
	{
		if(i%2==0)
			color[i]=0xffffffff;//白色
	}
	
//	color[1]=0x07e007e0;//绿色
//	color[3]=0x00000000;//黑色
//	color[5]=0xf800f800;//红色
//	color[7]=0x07e007e0;//绿色

	switch(bpp)
		{
			case 1:{
//					CR1_ADDR &= ~(1 << 4);
//					CR1_ADDR &= ~(1 << 3);
//					CR1_ADDR |= (1<<2);
					Register_BitSet(&CR1_ADDR,4,0);
					Register_BitSet(&CR1_ADDR,3,0);
					Register_BitSet(&CR1_ADDR,2,0);
					break;
				}
			case 2:{
					Register_BitSet(&CR1_ADDR,4,0);
					Register_BitSet(&CR1_ADDR,3,0);
					Register_BitSet(&CR1_ADDR,2,1);
					break;
				}
			case 4:{
					Register_BitSet(&CR1_ADDR,4,0);
					Register_BitSet(&CR1_ADDR,3,1);
					Register_BitSet(&CR1_ADDR,2,0);
					break;
				}
			case 8:{
					Register_BitSet(&CR1_ADDR,4,0);
					Register_BitSet(&CR1_ADDR,3,1);
					Register_BitSet(&CR1_ADDR,2,1);
					break;
				}	
			case 16:{
					Register_BitSet(&CR1_ADDR,4,1);
					Register_BitSet(&CR1_ADDR,3,0);
					Register_BitSet(&CR1_ADDR,2,0);
					color[1]=0x07e007e0;//绿色
					color[3]=0xf800f800;//红色
					color[5]=0x001f001f;//蓝色
					color[7]=0x07e007e0;//绿色
					break;
				}	
			case 18:{
					Register_BitSet(&CR1_ADDR,4,1);
					Register_BitSet(&CR1_ADDR,3,0);
					Register_BitSet(&CR1_ADDR,2,1);
					color[1]=0x00000fc0;//绿色
					color[3]=0x0003F000;//红色
					color[5]=0x0000003f;//蓝色
					color[7]=0x00000fc0;//绿色
					break;
				}	
			case 24:{
					Register_BitSet(&CR1_ADDR,4,1);
					Register_BitSet(&CR1_ADDR,3,1);
					Register_BitSet(&CR1_ADDR,2,0);
					color[1]=0x0000FF00;//绿色
					color[3]=0x00FF0000;//红色
					color[5]=0x000000ff;//蓝色
					color[7]=0x0000FF00;//绿色
					break;
				}	
			default:{
					Register_BitSet(&CR1_ADDR,4,1);
					Register_BitSet(&CR1_ADDR,3,1);
					Register_BitSet(&CR1_ADDR,2,1);
					break;
				}			
		}

	
//	CR1_ADDR |= (1<<12);//OPS0=1
//	CR1_ADDR|=0x2; //LCE=0 LPE=1
	Register_BitSet(&CR1_ADDR,5,1);//RGB=1
	Register_BitSet(&CR1_ADDR,12,1);//OPS0=1
	sprintf(buf,"CR1_ADDR_val:0x%x\n",CR1_ADDR);
	uart_printf_buf(buf);
	horizon_words = LCD_WIDTH/(32/bpp);

//	while(1)
//	{
//		Register_BitSet(&CR1_ADDR,0,0);
		for(i=0;i<LCD_HEIGHT;i++)
		{
//			if(i%(LCD_HEIGHT/8)==0)
//			{
//				sprintf(buf,"i:%d,ADDR:0x%x,color:0x%x\n",i,lcdc_addr + (i*horizon_words)*4,color[i/(LCD_HEIGHT/8)]);
//				uart_printf_buf(buf);
//			}
			for(j=0;j<horizon_words;j++)
			{
	
				*((volatile unsigned int *)(lcdc_addr + (i*horizon_words+j)*4)) = color[i/(LCD_HEIGHT/8)];//行条带
//				*((volatile unsigned int *)(lcdc_addr + (i*horizon_words+j)*4)) = color[i/(LCD_HEIGHT/8)];//列条带
//				if(i<=LCD_HEIGHT/2)
//					*((volatile unsigned int *)(lcdc_addr + (i*horizon_words+j)*4)) = 0x7c007c00;
//				else
//					*((volatile unsigned int *)(lcdc_addr + (i*horizon_words+j)*4)) = 0x001f001f;
			}	
		}
		
		Register_BitSet(&CR1_ADDR,0,1);
//		delay(0x1000);
//		Register_BitSet(&CR1_ADDR,0,0);
//		for(i=LCD_HEIGHT/2;i<LCD_HEIGHT;i++)
//		{
//			for(j=0;j<horizon_words;j++)
//			{
//				*((volatile unsigned int *)(lcdc_addr + (i*horizon_words+j)*4)) = color[i/(LCD_HEIGHT/8)+4];
//			}	
//		}
//		Register_BitSet(&CR1_ADDR,0,1);
//	}
}
void lcdc_dis()//LCD失能
{
	(CR1_ADDR=0x0);
	delay(0x1000);
}
void lcdc_main()
{

	unsigned int tmp = 0;
//	MTDCR(EXT_ADDR, 0x80000000);
	//*(volatile unsigned int*)(0xeb009000) = 0x13fb;
	tmp = *((volatile unsigned int *)(CCM_AXI_IO_MUL));
	*((volatile unsigned int *)(CCM_AXI_IO_MUL)) = tmp|0x100;//lcdc pin en
	tmp = *((volatile unsigned int *)(CCM_AXI_IO_MUL));
	
	
//	Lcd_H43_SB_1bpp();
    
//	Lcd_H43_FB_1bpp();

//	Lcd_H43_SB_2bpp();

//	Lcd_H43_FB_2bpp();

//	Lcd_H43_SB_4bpp();

//	Lcd_H43_SB_8bpp();

	Lcd_H43_SB_16bpp();
	
//	Lcd_H43_CR1_Test();
	
	lcdc_dis();
}
