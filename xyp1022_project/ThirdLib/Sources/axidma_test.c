#include "axidma.h"

//#include "timer.h"
#include "common.h"
#include "e500mc_asm.h"
//#include "spi.h"
#include "axidma.h"
//#define AXIDMA_CONTIGUOUS_TEST			1
//#define AXIDMA_LLI_TEST					0
//#define AXIDMA_INTERRUPT_TEST			0
//#define AXIDMA_SHADOWREG_TEST			0
//#define AXIDMA_RELOAD_TEST				0
//#define AXIDMA_MIXEDTYPE_TEST			0
//#define AXIDMA_CHSTAT_TEST				0
//#define AXIDMA_CHPRIOR_TEST				0
//#define AXIDMA_SW_HS_TEST				0
//#define AXIDMA_MEM2PER_TRANSFER_TEST	0
//#define AXIDMA_DDR_TEST					1

#define AXIDMA_CONTIGUOUS_TEST			1
#define AXIDMA_LLI_TEST					0
#define AXIDMA_INTERRUPT_TEST			0
#define AXIDMA_SHADOWREG_TEST			0
#define AXIDMA_RELOAD_TEST				0
#define AXIDMA_MIXEDTYPE_TEST			0
#define AXIDMA_CHSTAT_TEST				0
#define AXIDMA_CHPRIOR_TEST				1
#define AXIDMA_SW_HS_TEST				1
#define AXIDMA_MEM2PER_TRANSFER_TEST	0
#define AXIDMA_DDR_TEST					1

//CKStruct_IRQHandler axidma_irqhandler;
volatile unsigned int axidma_int_flag = 0;
volatile unsigned int axidma_int_tmp = 0;

#if 1
unsigned int swap_word(unsigned int data)
{
	return (((data & 0x000000ff) << 24) | ((data & 0x0000ff00) << 8) | ((data & 0x00ff0000) >> 8) | ((data & 0xff000000) >> 24));
}
#else
extern unsigned int swap_word(unsigned int data);
#endif
void buf_init(unsigned int addr, unsigned int size, unsigned char data)
{
	unsigned int i;

	for(i=0; i<size; i++)
		IO_WRITE8(addr+i, data++);
}

#if AXIDMA_CHPRIOR_TEST
volatile unsigned char prior_array[8]={0};
volatile unsigned int index=0;
void axidma_chprior_int()
{
	unsigned int reg;
int i;
	reg = IO_READ32(DMAC_INT_STATUS_REG);
	if(reg & 0x1)
		{
			axidma_int_flag |= 0x1;
			IO_WRITE32(CH_INT_CLEAR_REG(1), 0xffffffff);
			prior_array[index++] = 1;
			//PSPRINTF("1");
//			IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x1);
//			axidma_int_tmp+=4;
		}
	if(reg & 0x2)
		{
			axidma_int_flag |= 0x2;
			IO_WRITE32(CH_INT_CLEAR_REG(2), 0xffffffff);
			prior_array[index++] = 2;
			//PSPRINTF("2");
//			IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x2);
//			axidma_int_tmp+=4;
		}
	if(reg & 0x4)
		{
			axidma_int_flag |= 0x4;
			IO_WRITE32(CH_INT_CLEAR_REG(3), 0xffffffff);
			prior_array[index++] = 3;
			//PSPRINTF("3");
//			IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x3);
//			axidma_int_tmp+=4;
		}

	if(reg & 0x8)
		{
			axidma_int_flag |= 0x8;
			IO_WRITE32(CH_INT_CLEAR_REG(4), 0xffffffff);
			prior_array[index++] = 4;
			//PSPRINTF("4");
//			IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x4);
//			axidma_int_tmp+=4;
		}

	if(reg & 0x10)
	{
		axidma_int_flag |= 0x10;
		IO_WRITE32(CH_INT_CLEAR_REG(5), 0xffffffff);
		prior_array[index++] = 5;
		//PSPRINTF("5");
//		IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x5);
//		axidma_int_tmp+=4;
	}
	if(reg & 0x20)
		{
			axidma_int_flag |= 0x20;
			IO_WRITE32(CH_INT_CLEAR_REG(6), 0xffffffff);
			prior_array[index++] = 6;
			//PSPRINTF("6");
//			IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x6);
//			axidma_int_tmp+=4;
		}
	if(reg & 0x40)
	{
		axidma_int_flag |= 0x40;
		IO_WRITE32(CH_INT_CLEAR_REG(7), 0xffffffff);
		prior_array[index++] = 7;
		//PSPRINTF("7");
//		IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x7);
//		axidma_int_tmp+=4;
	}

	if(reg & 0x80)
	{
		axidma_int_flag |= 0x80;
		IO_WRITE32(CH_INT_CLEAR_REG(8), 0xffffffff);
		prior_array[index++] = 8;
		//PSPRINTF("8");
//		IO_WRITE32(0xe006Fe00+axidma_int_tmp,0x8);
//		axidma_int_tmp+=4;
	}

	reg = IO_READ32(DMAC_COMMON_INT_STATUS_REG);
	IO_WRITE32(DMAC_COMMON_INT_CLEAR_REG, reg);
}
#endif

#if AXIDMA_INTERRUPT_TEST
void axidma_int()
{
	unsigned int reg, i;
	axidma_int_flag = 1;

	reg = IO_READ32(DMAC_INT_STATUS_REG);
	//printf("DMAC_INT_STATUS_REG : 0x%x\r\n", reg);
	//reg = IO_READ32(DMAC_INT_STATUS_REG_H);
	//printf("DMAC_INT_STATUS_REG_H : 0x%x\r\n", reg);

	reg = IO_READ32(DMAC_COMMON_INT_STATUS_REG);
	IO_WRITE32(DMAC_COMMON_INT_CLEAR_REG, reg);
	//IO_WRITE32(DMAC_COMMON_INT_CLEAR_REG_H, 0xffffffff);

	for(i=1; i<=8; i++)
	{
		reg = IO_READ32(CH_INT_STATUS_REG(i));
		PSPRINTF("CH_INT_STATUS_REG : 0x%x\r\n", reg);
		IO_WRITE32(CH_INT_CLEAR_REG(i), reg);
	}
}
#endif

#if AXIDMA_RELOAD_TEST
void axidma_reload_int()
{
	unsigned int reg, i;
	axidma_int_flag = 1;

	for(i=0; i<0x200; i++)
	{
		if(R8(0xe0058000+i) != R8(0xe0060000+i))
			while(1);
	}
	reg = R8(0xe0058000);
	buf_init(0xe0058000, 0x1000, reg+3);

	for(i=1; i<=8; i++)
	{
		reg = IO_READ32(CH_INT_STATUS_REG(i));
		//PSPRINTF("CH_INT_STATUS_REG : 0x%x\r\n", reg);
		IO_WRITE32(CH_INT_CLEAR_REG(i), reg);
	}
}
#endif

void axidma_reset()
{
	unsigned int reg;

	W32(DMAC_RESET_REG, DMAC_RST);
	do {
		reg = R32(DMAC_RESET_REG);
	}while(reg & DMAC_RST);
}

void axidma_init_one_channel(unsigned char n, unsigned int sar, unsigned int dar, unsigned int ts, unsigned int ctrl, unsigned int cfg_h, unsigned int cfg_l)
{
	unsigned int reg, i;

	IO_WRITE32(CH_SAR(n), sar);
	reg = IO_READ32(CH_SAR(n));
//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x80000000);				//sram
	reg = IO_READ32(CH_SAR_H(n));
//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), dar);
	reg = IO_READ32(CH_DAR(n));
//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x80000000);
	reg = IO_READ32(CH_DAR_H(n));
//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), ts);
	reg = IO_READ32(CH_BLOCK_TS(n));
	//PSPRINTF("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), ctrl);
	reg = IO_READ32(CH_CTL(n));
//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), 0);
	reg = IO_READ32(CH_CTL_H(n));
///	printf("CH_CTL(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CFG_H(n), cfg_h);
	reg = IO_READ32(CH_CFG_H(n));
	//PSPRINTF("CH_CFG_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CFG(n), cfg_l);
//	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_RELOAD | SRC_MULTBLK_TYPE_RELOAD );
	reg = IO_READ32(CH_CFG(n));
//	printf("CH_CFG(n) : 0x%x\r\n", reg);
}

void axidma_transfer_one_block(unsigned char n, unsigned int sar, unsigned int dar, unsigned int ts, unsigned int ctrl)
{
	unsigned int reg, i;
	unsigned int cfg_l, cfg_h;

	//axidma_reset();
/*
	ctl = 0;
	if(len % 2 != 0)
	{
		ts = len-1;
		ctl |= SRC_MSIZE_1 | DST_MSIZE_1 | SRC_TR_WIDTH_8 | DST_TR_WIDTH_8;
	}
	else if(len % 4 != 0)
	{
		switch(sar % 2)
		{
		case 0:
			ts = len/2 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_16;
			break;
		case 1:
			ts = len - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_8;
			break;
		}

		switch(dar % 2)
		{
		case 0:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_16;
			break;
		case 1:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_8;
			break;
		}
	}
	else if(len % 8 != 0)
	{
		switch(sar % 4)
		{
		case 0:
			ts = len/4 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_32;
			break;
		case 1:
		case 3:
			ts = len - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_8;
			break;
		case 2:
			ts = len/2 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_16;
			break;
		}

		switch(dar % 4)
		{
		case 0:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_32;
			break;
		case 1:
		case 3:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_8;
			break;
		case 2:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_16;
			break;
		}
	}
	else
	{
		switch(sar % 8)
		{
		case 0:
			ts = len/8 - 1;

		}
	}
*/
#if AXIDMA_INTERRUPT_TEST
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
#else
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
#endif
	reg = IO_READ32(DMAC_CFG_REG);
	//PSPRINTF("DMAC_CFG_REG : 0x%x\r\n", reg);

	IO_WRITE32(CH_SAR(n), sar);
	reg = IO_READ32(CH_SAR(n));
//	PSPRINTF("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);										//sram
	//IO_WRITE32(CH_SAR_H(n), 0x0);
	reg = IO_READ32(CH_SAR_H(n));
//	PSPRINTF("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), dar);
	reg = IO_READ32(CH_DAR(n));
//	PSPRINTF("CH_DAR(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_DAR_H(n), 0x00000004);										//sram

	//IO_WRITE32(CH_DAR_H(n), 0x0);
	reg = IO_READ32(CH_DAR_H(n));
//	PSPRINTF("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), ts);
	reg = IO_READ32(CH_BLOCK_TS(n));
	//PSPRINTF("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), ctrl);
	reg = IO_READ32(CH_CTL(n));
//	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), 0);
	reg = IO_READ32(CH_CTL_H(n));
//	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);

	cfg_l = IO_READ32(CH_CFG(n));
	cfg_h = IO_READ32(CH_CFG_H(n));
#if AXIDMA_CHSTAT_TEST
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM | LOCK_CH);
#else
//for new 903 test
//	cfg_h = 0;
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
#endif
	reg = IO_READ32(CH_CFG_H(n));
//	PSPRINTF("CH_CFG_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_CONTIGUOUS | SRC_MULTBLK_TYPE_CONTIGUOUS );
//	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_RELOAD | SRC_MULTBLK_TYPE_RELOAD );
	reg = IO_READ32(CH_CFG(n));
//PSPRINTF("CH_CFG(n) : 0x%x\r\n", reg);


	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
	reg = IO_READ32(DMAC_CH_EN_REG);
//PSPRINTF("DMAC_CH_EN_REG : 0x%x\r\n", reg);

#if AXIDMA_CONTIGUOUS_TEST
#if AXIDMA_INTERRUPT_TEST
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;
#else
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
	//	PSPRINTF("temp=%x\n\t\n",reg);
	}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
	IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
#endif
#endif
}

void axidma_transfer_one_block_s1020(unsigned char n, unsigned int sah, unsigned int sar,unsigned int dah, unsigned int dar, unsigned int ts, unsigned int ctrl)
{
	unsigned int reg, i;
	unsigned int cfg_l, cfg_h;
//	unsigned int start, finish;

	//axidma_reset();
/*
	ctl = 0;
	if(len % 2 != 0)
	{
		ts = len-1;
		ctl |= SRC_MSIZE_1 | DST_MSIZE_1 | SRC_TR_WIDTH_8 | DST_TR_WIDTH_8;
	}
	else if(len % 4 != 0)
	{
		switch(sar % 2)
		{
		case 0:
			ts = len/2 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_16;
			break;
		case 1:
			ts = len - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_8;
			break;
		}

		switch(dar % 2)
		{
		case 0:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_16;
			break;
		case 1:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_8;
			break;
		}
	}
	else if(len % 8 != 0)
	{
		switch(sar % 4)
		{
		case 0:
			ts = len/4 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_32;
			break;
		case 1:
		case 3:
			ts = len - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_8;
			break;
		case 2:
			ts = len/2 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_16;
			break;
		}

		switch(dar % 4)
		{
		case 0:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_32;
			break;
		case 1:
		case 3:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_8;
			break;
		case 2:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_16;
			break;
		}
	}
	else
	{
		switch(sar % 8)
		{
		case 0:
			ts = len/8 - 1;

		}
	}
*/
#if AXIDMA_INTERRUPT_TEST
	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_int);
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
#else
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
#endif
	reg = IO_READ32(DMAC_CFG_REG);
	//PSPRINTF("DMAC_CFG_REG : 0x%x\r\n", reg);

	IO_WRITE32(CH_SAR(n), sar);
	reg = IO_READ32(CH_SAR(n));
//	PSPRINTF("CH_SAR(n) : 0x%x\r\n", reg);

//	IO_WRITE32(CH_SAR_H(n), 0x80000000);										//now 908T ddr
	IO_WRITE32(CH_SAR_H(n), sah);												//now 908T sram

	//IO_WRITE32(CH_SAR_H(n), 0x0);
	reg = IO_READ32(CH_SAR_H(n));
//	PSPRINTF("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), dar);
	reg = IO_READ32(CH_DAR(n));
//	PSPRINTF("CH_DAR(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_DAR_H(n), dah);												//sram
//	IO_WRITE32(CH_DAR_H(n), 0x80000000);										//ddr

	//IO_WRITE32(CH_DAR_H(n), 0x0);
	reg = IO_READ32(CH_DAR_H(n));
//	PSPRINTF("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), ts);
	reg = IO_READ32(CH_BLOCK_TS(n));
	//PSPRINTF("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), ctrl);
	reg = IO_READ32(CH_CTL(n));
//	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), 0);
	reg = IO_READ32(CH_CTL_H(n));
//	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);

	cfg_l = IO_READ32(CH_CFG(n));
	cfg_h = IO_READ32(CH_CFG_H(n));
#if AXIDMA_CHSTAT_TEST
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM | LOCK_CH);
#else
//for new 903 test
//	cfg_h = 0;
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
#endif
	reg = IO_READ32(CH_CFG_H(n));
//	PSPRINTF("CH_CFG_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_CONTIGUOUS | SRC_MULTBLK_TYPE_CONTIGUOUS );
//	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_RELOAD | SRC_MULTBLK_TYPE_RELOAD );
	reg = IO_READ32(CH_CFG(n));
//PSPRINTF("CH_CFG(n) : 0x%x\r\n", reg);

//	MFSPR(start, SPR_TBLR);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
	reg = IO_READ32(DMAC_CH_EN_REG);
//PSPRINTF("DMAC_CH_EN_REG : 0x%x\r\n", reg);

#if AXIDMA_CONTIGUOUS_TEST
#if AXIDMA_INTERRUPT_TEST
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;
#else
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
	//	PSPRINTF("temp=%x\n\t\n",reg);
	}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
	IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
#endif

//	MFSPR(finish, SPR_TBLR);
//	PSPRINTF("cost %d cycle\r\n", finish-start);
#endif
}


void axidma_transfer_no_transmit_en(unsigned char n, unsigned int sar, unsigned int dar, unsigned int ts, unsigned int ctrl)
{
	unsigned int reg, i;
	unsigned int cfg_l, cfg_h;

	//axidma_reset();
/*
	ctl = 0;
	if(len % 2 != 0)
	{
		ts = len-1;
		ctl |= SRC_MSIZE_1 | DST_MSIZE_1 | SRC_TR_WIDTH_8 | DST_TR_WIDTH_8;
	}
	else if(len % 4 != 0)
	{
		switch(sar % 2)
		{
		case 0:
			ts = len/2 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_16;
			break;
		case 1:
			ts = len - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_8;
			break;
		}

		switch(dar % 2)
		{
		case 0:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_16;
			break;
		case 1:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_8;
			break;
		}
	}
	else if(len % 8 != 0)
	{
		switch(sar % 4)
		{
		case 0:
			ts = len/4 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_32;
			break;
		case 1:
		case 3:
			ts = len - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_8;
			break;
		case 2:
			ts = len/2 - 1;
			ctl |= SRC_MSIZE_1 | SRC_TR_WIDTH_16;
			break;
		}

		switch(dar % 4)
		{
		case 0:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_32;
			break;
		case 1:
		case 3:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_8;
			break;
		case 2:
			ctl |= DST_MSIZE_1 | DST_TR_WIDTH_16;
			break;
		}
	}
	else
	{
		switch(sar % 8)
		{
		case 0:
			ts = len/8 - 1;

		}
	}
*/
#if AXIDMA_INTERRUPT_TEST
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
#else
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
#endif
	reg = IO_READ32(DMAC_CFG_REG);
	//PSPRINTF("DMAC_CFG_REG : 0x%x\r\n", reg);

	IO_WRITE32(CH_SAR(n), sar);
	reg = IO_READ32(CH_SAR(n));
//	PSPRINTF("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x80000000);
	//IO_WRITE32(CH_SAR_H(n), 0x0);
	reg = IO_READ32(CH_SAR_H(n));
//	PSPRINTF("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), dar);
	reg = IO_READ32(CH_DAR(n));
//	PSPRINTF("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x80000000);
	//IO_WRITE32(CH_DAR_H(n), 0x0);
	reg = IO_READ32(CH_DAR_H(n));
//	PSPRINTF("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), ts);
	reg = IO_READ32(CH_BLOCK_TS(n));
	//PSPRINTF("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), ctrl);
	reg = IO_READ32(CH_CTL(n));
//	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), 0);
	reg = IO_READ32(CH_CTL_H(n));
//	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);

	cfg_l = IO_READ32(CH_CFG(n));
	cfg_h = IO_READ32(CH_CFG_H(n));
#if AXIDMA_CHSTAT_TEST
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM | LOCK_CH);
#else
//for new 903 test
//	cfg_h = 0;
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
#endif
	reg = IO_READ32(CH_CFG_H(n));
//	PSPRINTF("CH_CFG_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_CONTIGUOUS | SRC_MULTBLK_TYPE_CONTIGUOUS );
//	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_RELOAD | SRC_MULTBLK_TYPE_RELOAD );
	reg = IO_READ32(CH_CFG(n));
//PSPRINTF("CH_CFG(n) : 0x%x\r\n", reg);


/*	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
	reg = IO_READ32(DMAC_CH_EN_REG);
//PSPRINTF("DMAC_CH_EN_REG : 0x%x\r\n", reg);

#if AXIDMA_CONTIGUOUS_TEST
#if AXIDMA_INTERRUPT_TEST
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;
#else
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
	//	PSPRINTF("temp=%x\n\t\n",reg);
	}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
	IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
#endif
#endif
*/
}

#if AXIDMA_LLI_TEST
void axidma_lli_test(unsigned char n)
{
	unsigned int *llp1 = (unsigned int *)0xe0050000;
		unsigned int *llp2 = (unsigned int *)0xe0051000;
		unsigned int *llp3 = (unsigned int *)0xe0052000;
		unsigned int *llp4 = (unsigned int *)0xe0053000;
		unsigned int *llp5 = (unsigned int *)0xe0054000;
		unsigned int reg, cfg_l, cfg_h, i,j;

	#if AXIDMA_INTERRUPT_TEST
		InstallInterruptHandler0(18,axidma_int);
	#endif

		buf_init(0xe0060000, 0x5000, n+0xaa);

		llp1[0] = 0xe0060000;
		llp1[1] = 0x80000000;
		llp1[2] = 0xe0068000;
		llp1[3] = 0x80000000;
		llp1[4] = 0x1f;
		llp1[5] = 0;
//		llp1[6] = (unsigned int)llp2;
//		llp1[7] = 0x80000000;
//		llp1[8] = SRC_TR_WIDTH_32 | DST_TR_WIDTH_8 | DMS_2;
//		llp1[9] = SHADOWREG_LLI_VALID;
		llp1[6] = 0;
		llp1[7] = 0;
		llp1[8] = SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2;
		llp1[9] = SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST;

		llp2[0] = 0xe0069000;
		llp2[1] = 0x80000000;
		llp2[2] = 0xe0061002;
		llp2[3] = 0x80000050;
		llp2[4] = 0x1f;
		llp2[5] = 0;
		llp2[6] = (unsigned int)llp3;
		llp2[7] = 0x80000000;
		llp2[8] = SRC_TR_WIDTH_32 | DST_TR_WIDTH_16 | DMS_2;
		llp2[9] = SHADOWREG_LLI_VALID;


		llp3[0] = 0xe006a000;
		llp3[1] = 0x80000000;
		llp3[2] = 0xe0062004;
		llp3[3] = 0x80000000;
		llp3[4] = 0x1f;
		llp3[5] = 0;
		llp3[6] = (unsigned int)llp4;
		llp3[7] = 0x80000000;
		llp3[8] = SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2;
		llp3[9] = SHADOWREG_LLI_VALID;


		llp4[0] = 0xe006b000;
		llp4[1] = 0x80000000;
		llp4[2] = 0xe0063008;
		llp4[3] = 0x80000000;
		llp4[4] = 0x1f;
		llp4[5] = 0;
		llp4[6] = 0;
		llp4[7] = 0;
		llp4[8] = SRC_TR_WIDTH_32 | DST_TR_WIDTH_64 | DMS_2;
		llp4[9] = SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST;
	/*
		llp5[0] = 0x134000;
		llp5[1] = 0;
		llp5[2] = 0x144008;
		llp5[3] = 0;
		llp5[4] = 0x3ff;
		llp5[5] = 0;
		llp5[6] = 0;
		llp5[7] = 0;
		llp5[8] = SRC_TR_WIDTH_32 | DST_TR_WIDTH_64 | DMS_2;
		llp5[9] = SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST;
	*/
		axidma_reset();

	#if AXIDMA_INTERRUPT_TEST
		IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
	#else
		IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
		reg = IO_READ32(DMAC_CFG_REG);
		//printf("DMAC_CFG_REG : 0x%x\r\n", reg);
	#endif

		cfg_l = IO_READ32(CH_CFG(n));
		cfg_h = IO_READ32(CH_CFG_H(n));
		IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_LINKED_LIST | SRC_MULTBLK_TYPE_LINKED_LIST );
		reg = IO_READ32(CH_CFG(n));
		//printf("CH_CFG(n) : 0x%x\r\n", reg);
		IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
		reg = IO_READ32(CH_CFG_H(n));
		//printf("CH_CFG_H(n) : 0x%x\r\n", reg);

		IO_WRITE32(CH_LLP(n), (unsigned int)llp1);
		IO_WRITE32(CH_LLP_H(n), 0x80000000);

		IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
		reg = IO_READ32(DMAC_CH_EN_REG);
		//printf("DMAC_CH_EN_REG : 0x%x\r\n", reg);

	#if AXIDMA_INTERRUPT_TEST
		while(axidma_int_flag == 0);
		axidma_int_flag = 0;
	#else
		do {
			reg = IO_READ32(CH_INT_STATUS_REG(n));
		}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
		IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
	#endif

		for(i=0; i<0x1f; i++)
		{

			if(R8(i+0xe0068000) != R8(i+0xe0060001))
				while(1);
		}

		for(i=0; i<0x1f; i++)
		{

			if(R8(i+0xe0069000) != R8(i+0xe0061002))
				while(1);
		}

		for(i=0; i<0x1f; i++)
		{
			if(R8(i+0xe006a000) != R8(i+0xe0062004))
				while(1);
		}

		for(i=0; i<0x1f; i++)
		{
			if(R8(i+0xe006b000) != R8(i+0xe0063008))
				while(1);
		}
	/*
		//Resume Test
		llp4[6] = (unsigned int)llp5;
		llp4[7] = 0;
		IO_WRITE32(CH_BLK_TFR_RESUMEREQ_REG(n), BLK_TFR_RESUMEREQ);

	#if AXIDMA_INTERRUPT_TEST
		while(axidma_int_flag == 0);
		axidma_int_flag = 0;
	#else
		do {
			reg = IO_READ32(CH_INT_STATUS_REG(n));
		}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
		IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
	#endif

		for(i=0; i<0x1000; i++)
		{
			if(R8(i+0x134000) != R8(i+0x144008))
				asm("bkpt");
		}
	*/
	}

void axidma_lli_test1(unsigned char n)
{
		unsigned int *llp1 = (unsigned int *)0xe0050000;
		unsigned int *llp2 = (unsigned int *)0xe0051000;
		unsigned int *llp3 = (unsigned int *)0xe0052000;
		unsigned int *llp4 = (unsigned int *)0xe0053000;
		unsigned int *llp5 = (unsigned int *)0xe0054000;
		unsigned int reg, cfg_l, cfg_h, i,j;

 	 	buf_init(0xe0068000, 0x5f00, n+0xaa);

 	 	llp1[0] = swap_word(0xe0068000);
		llp1[1] = swap_word(0x00000004);
		llp1[2] = swap_word(0xe0060001);			//dst width is 8, so can 1 byte align
		llp1[3] = swap_word(0x00000004);
		llp1[4] = swap_word(0xff);
		llp1[5] = swap_word(0);
		llp1[6] = swap_word((unsigned int)llp2);
		llp1[7] = swap_word(0x00000004);
		llp1[8] = swap_word(SRC_TR_WIDTH_128 | DST_TR_WIDTH_8 | DMS_2);
		llp1[9] = swap_word(SHADOWREG_LLI_VALID);

 	 	llp2[0] = swap_word(0xe0069000);
		llp2[1] = swap_word(0x00000004);
		llp2[2] = swap_word(0xe0061ffe);			//dst width is 16, so can 1 byte align
		llp2[3] = swap_word(0x00000004);
		llp2[4] = swap_word(0x1ff);
		llp2[5] = swap_word(0);
		llp2[6] = swap_word((unsigned int)llp3);
		llp2[7] = swap_word(0x00000004);
		llp2[8] = swap_word(SRC_TR_WIDTH_64 | DST_TR_WIDTH_16 | DMS_2);
		llp2[9] = swap_word(SHADOWREG_LLI_VALID);

		llp3[0] = swap_word(0xe006a00c);
		llp3[1] = swap_word(0x00000004);
		llp3[2] = swap_word(0xe0063ff8);
		llp3[3] = swap_word(0x00000004);			//ddr_H is 0x80000000
		llp3[4] = swap_word(0x3ff);
		llp3[5] = swap_word(0);
		llp3[6] = swap_word((unsigned int)llp4);
		llp3[7] = swap_word(0x00000004);
		llp3[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
		llp3[9] = swap_word(SHADOWREG_LLI_VALID);

		llp4[0] = swap_word(0xe006bfff);
		llp4[1] = swap_word(0x00000004);
		llp4[2] = swap_word(0xe0065ff0);
		llp4[3] = swap_word(0x00000004);
		llp4[4] = swap_word(0xfff);
		llp4[5] = swap_word(0);
		llp4[6] = swap_word(0);
		llp4[7] = swap_word(0);
		llp4[8] = swap_word(SRC_TR_WIDTH_8 | DST_TR_WIDTH_128 | DMS_2);
		llp4[9] = swap_word(SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST);

		axidma_reset();

	#if AXIDMA_INTERRUPT_TEST
		InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_int);
		IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
	#else
		IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
		//reg = IO_READ32(DMAC_CFG_REG);
		//printf("DMAC_CFG_REG : 0x%x\r\n", reg);
	#endif

		cfg_l = IO_READ32(CH_CFG(n));
		cfg_h = IO_READ32(CH_CFG_H(n));
		IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_LINKED_LIST | SRC_MULTBLK_TYPE_LINKED_LIST );
		reg = IO_READ32(CH_CFG(n));
		PSPRINTF("CH_CFG(n) : 0x%x\r\n", reg);
		IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
		//reg = IO_READ32(CH_CFG_H(n));
		//printf("CH_CFG_H(n) : 0x%x\r\n", reg);

		IO_WRITE32(CH_LLP(n), (unsigned int)llp1);
		IO_WRITE32(CH_LLP_H(n), 0x00000004);

		IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
		//reg = IO_READ32(DMAC_CH_EN_REG);
		//printf("DMAC_CH_EN_REG : 0x%x\r\n", reg);

	#if AXIDMA_INTERRUPT_TEST
		while(axidma_int_flag == 0);
		axidma_int_flag = 0;
	#else
		do {
			reg = IO_READ32(CH_INT_STATUS_REG(n));
			PSPRINTF("CH_INT_STATUS_REG(n)--- : 0x%x\r\n", reg);
		}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
		IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
	#endif

		for(i=0; i<0x1000; i++)
		{
			if(R8(i+0xe0068000) != R8(i+0xe0060001))
				while(1);
		}

		for(i=0; i<0x1000; i++)
		{
			if(R8(i+0xe0069000) != R8(i+0xe0061ffe))
				while(1);
		}

		for(i=0; i<0x1000; i++)
		{
			if(R8(i+0xe006a00c) != R8(i+0xe0063ff8))
				while(1);
		}

		for(i=0; i<0x1000; i++)
		{
			if(R8(i+0xe006bfff) != R8(i+0xe0065ff0))
				while(1);
		}
	/*
		//Resume Test
		llp4[6] = (unsigned int)llp5;
		llp4[7] = 0;
		IO_WRITE32(CH_BLK_TFR_RESUMEREQ_REG(n), BLK_TFR_RESUMEREQ);

	#if AXIDMA_INTERRUPT_TEST
		while(axidma_int_flag == 0);
		axidma_int_flag = 0;
	#else
		do {
			reg = IO_READ32(CH_INT_STATUS_REG(n));
		}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
		IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
	#endif

		for(i=0; i<0x1000; i++)
		{
			if(R8(i+0x134000) != R8(i+0x144008))
				asm("bkpt");
		}
	*/
	}

void axidma_lli_test2(unsigned char n)
{
	unsigned int *llp1 = (unsigned int *)0xe0318000;
		unsigned int *llp2 = (unsigned int *)0xe0319000;
		unsigned int *llp3 = (unsigned int *)0xe031a000;
		unsigned int *llp4 = (unsigned int *)0xe031b000;
		unsigned int *llp5 = (unsigned int *)0xe031c000;
		unsigned int reg, cfg_l, cfg_h, i,j;

	#if AXIDMA_INTERRUPT_TEST
		InstallInterruptHandler0(18,axidma_int);
	#endif

		buf_init(0xe0324000, 0x3f00, n+0xaa);

		llp1[0] = swap_word(0xe0324000);
		llp1[1] = swap_word(0x80000000);
		llp1[2] = swap_word(0xe0320000);
		llp1[3] = swap_word(0x80000000);
		llp1[4] = swap_word(0x1f);
		llp1[5] = swap_word(0);
		llp1[6] = swap_word((unsigned int)llp2);
		llp1[7] =swap_word (0x80000000);
		llp1[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
		llp1[9] = swap_word(SHADOWREG_LLI_VALID);

		llp2[0] = swap_word(0xe0325000);
		llp2[1] = swap_word(0x80000000);
		llp2[2] = swap_word(0xe0321000);
		llp2[3] =swap_word (0x80000000);
		llp2[4] = swap_word(0x1f);
		llp2[5] = swap_word(0);
		llp2[6] = swap_word((unsigned int)llp3);
		llp2[7] =swap_word (0x80000000);
		llp2[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
		llp2[9] = swap_word(SHADOWREG_LLI_VALID);


		llp3[0] = swap_word(0xe0326000);
		llp3[1] =swap_word(0x80000000);
		llp3[2] = swap_word(0xe0322000);
		llp3[3] =swap_word(0x80000000);
		llp3[4] =swap_word( 0x1f);
		llp3[5] = swap_word(0);
		llp3[6] =swap_word ((unsigned int)llp4);
		llp3[7] = swap_word(0x80000000);
		llp3[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
		llp3[9] = swap_word(SHADOWREG_LLI_VALID);


		llp4[0] = swap_word(0xe0327000);
		llp4[1] =swap_word (0x80000000);
		llp4[2] = swap_word(0xe0323000);
		llp4[3] =swap_word (0x80000000);
		llp4[4] = swap_word(0x1f);
		llp4[5] = swap_word(0);
		llp4[6] = swap_word(0);
		llp4[7] = swap_word(0);
		llp4[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
		llp4[9] = swap_word(SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST);
	/*
		llp5[0] = 0x134000;
		llp5[1] = 0;
		llp5[2] = 0x144008;
		llp5[3] = 0;
		llp5[4] = 0x3ff;
		llp5[5] = 0;
		llp5[6] = 0;
		llp5[7] = 0;
		llp5[8] = SRC_TR_WIDTH_32 | DST_TR_WIDTH_64 | DMS_2;
		llp5[9] = SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST;
	*/
		axidma_reset();

	#if AXIDMA_INTERRUPT_TEST
		InstallInterruptHandler0(18,axidma_int);
		IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
	#else
		IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
		//reg = IO_READ32(DMAC_CFG_REG);
		//printf("DMAC_CFG_REG : 0x%x\r\n", reg);
	#endif

		cfg_l = IO_READ32(CH_CFG(n));
		cfg_h = IO_READ32(CH_CFG_H(n));
		IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_LINKED_LIST | SRC_MULTBLK_TYPE_LINKED_LIST );
		reg = IO_READ32(CH_CFG(n));
		PSPRINTF("CH_CFG(n) : 0x%x\r\n", reg);
		IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
		//reg = IO_READ32(CH_CFG_H(n));
		//printf("CH_CFG_H(n) : 0x%x\r\n", reg);

		IO_WRITE32(CH_LLP(n), (unsigned int)llp1);
		IO_WRITE32(CH_LLP_H(n), 0x80000000);

		IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
		//reg = IO_READ32(DMAC_CH_EN_REG);
		//printf("DMAC_CH_EN_REG : 0x%x\r\n", reg);

	#if AXIDMA_INTERRUPT_TEST
		while(axidma_int_flag == 0);
		axidma_int_flag = 0;
	#else
		do {
			reg = IO_READ32(CH_INT_STATUS_REG(n));
			PSPRINTF("CH_INT_STATUS_REG(n)--- : 0x%x\r\n", reg);
		}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
		IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
	#endif

		for(i=0; i<0x80; i++)
		{

			if(R8(i+0xe0324000) != R8(i+0xe0320000))
				while(1);
		}

		for(i=0; i<0x80; i++)
		{

			if(R8(i+0xe0325000) != R8(i+0xe0321000))
				while(1);
		}

		for(i=0; i<0x80; i++)
		{
			if(R8(i+0xe0326000) != R8(i+0xe0322000))
				while(1);
		}

		for(i=0; i<0x80; i++)
		{
			if(R8(i+0xe0327000) != R8(i+0xe0323000))
				while(1);
		}
	/*
		//Resume Test
		llp4[6] = (unsigned int)llp5;
		llp4[7] = 0;
		IO_WRITE32(CH_BLK_TFR_RESUMEREQ_REG(n), BLK_TFR_RESUMEREQ);

	#if AXIDMA_INTERRUPT_TEST
		while(axidma_int_flag == 0);
		axidma_int_flag = 0;
	#else
		do {
			reg = IO_READ32(CH_INT_STATUS_REG(n));
		}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
		IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
	#endif

		for(i=0; i<0x1000; i++)
		{
			if(R8(i+0x134000) != R8(i+0x144008))
				asm("bkpt");
		}
	*/
	}
#endif

#if AXIDMA_CONTIGUOUS_TEST
void axidma_contiguous_test()
{
	unsigned int src_msize=0, src_width, dst_msize=0, dst_width, i, j, k, l, count;
	unsigned int offset[4] = {
			1, 2, 4, 0
	};
	unsigned char ch;

	//need to change to IO_WRITE32(CH_DAR_H(n), 0x80000030);	at axidma_transfer_one_block
#if AXIDMA_DDR_TEST
	axidma_reset();
#if AXIDMA_INTERRUPT_TEST
	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_int);
	//Init_Irq_Struct(&axidma_irqhandler,93,axidma_int,0,0);
#endif
	//#1 sram2ddr
#if 1
	PSPRINTF("sram2ddr!\r\n");
	axidma_reset();
	buf_init(0xe0073ff0, 0x1100, 0x20);
	axidma_transfer_one_block_s1020(1, 0x00000004, 0xe0073fff, 0, 1, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8) | (dst_msize << 18) | (DST_TR_WIDTH_8));
	//can't using 8bit and 16bit when dst is ddr
	for(l=0; l<0x1000; l++)
	{
		if(R8(0xe0073fff+l) != R8(1+l))
		{
			PSPRINTF("dma data err!!  \n\n\t");
			while(1);
		}
	}

	axidma_reset();
	buf_init(0xe0073ff0, 0x2100, 0x30);
	axidma_transfer_one_block_s1020(1, 0x00000004, 0xe0073ffe, 0, 2, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_16) | (dst_msize << 18) | (DST_TR_WIDTH_16));
	//can't using 8bit and 16bit when dst is ddr
	for(l=0; l<0x2000; l++)
	{
		if(R8(0xe0073ffe + l) != R8(2+l))
		{
			PSPRINTF("dma data err!!  \n\n\t");
			while(1);
		}
	}

	axidma_reset();
	buf_init(0xe0073ff0, 0x4100, 0x30);
	axidma_transfer_one_block_s1020(1, 0x00000004, 0xe0073ffc, 0, 4, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_32) | (dst_msize << 18) | (DST_TR_WIDTH_32));
	for(l=0; l<0x4000; l++)
	{
		if(R8(0xe0073ffc+l) != R8(4+l))
		{
			PSPRINTF("the sram is:%x; the ddr is:%x!!\n\n\t", R8(0xe005fffc+l), R8(4+l));
			while(1);
		}
	}

	axidma_reset();
	buf_init(0xe0073ff0, 0x8100, 0x40);
	axidma_transfer_one_block_s1020(1, 0x00000004, 0xe0073ff8, 0, 8, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_64) | (dst_msize << 18) | (DST_TR_WIDTH_64));
	for(l=0; l<0x8000; l++)
	{
		if(R8(0xe0073ff8+l) != R8(8+l))
		{
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0xe0073ff0, 0x8100, 0x50);
	axidma_transfer_one_block_s1020(1, 0x00000004, 0xe0073ff0, 0, 0x10, 0x7ff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_128) | (dst_msize << 18) | (DST_TR_WIDTH_128));
	for(l=0; l<0x8000; l++)
	{
		if(R8(0xe0073ff0+l) != R8(0x10+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0xe0073ff0, 0x8100, 0x60);
	axidma_transfer_one_block_s1020(1, 0x00000004, 0xe0073ff0, 0, 0x10, 0x3ff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_256) | (dst_msize << 18) | (DST_TR_WIDTH_256));
	for(l=0; l<0x4000; l++)
	{
		if(R8(0xe0073ff0+l) != R8(0x10+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0xe0073ff0, 0x8100, 0x70);
	axidma_transfer_one_block_s1020(1, 0x00000004, 0xe0073ff0, 0, 0x10, 0x1ff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_512) | (dst_msize << 18) | (DST_TR_WIDTH_512));

	for(l=0; l<0x2000; l++)
	{
		if(R8(0xe0073ff0+l) != R8(0x10+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}
#endif

	//#1 ddr2sram
#if 1
	PSPRINTF("ddr2sram!\r\n");
	src_msize = 4;
	dst_msize = 4;
	axidma_reset();
	int tmpdata1,tmpdata2,tmpdata3;

	buf_init(0x10000, 0x1100, 0x10);
	axidma_transfer_one_block_s1020(1, 0, 0x10001, 0x00000004, 0xe0073fff, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8) | (dst_msize << 18) | (DST_TR_WIDTH_8));
	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x1000; l++)
	{
		if(R8(0xe0073fff+l) != R8(0x10001+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0x10000, 0x2100, 0x20);
	axidma_transfer_one_block_s1020(1, 0, 0x10002, 0x00000004, 0xe0073ffe, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_16) | (dst_msize << 18) | (DST_TR_WIDTH_16));
	//can't using 8bit and 16bit when between sram and ddr
	for(l=0; l<0x2000; l++)
	{
		if(R8(0xe0073ffe + l) != R8(0x10002+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0x10000, 0x4100, 0x30);
	axidma_transfer_one_block_s1020(1, 0, 0x10004, 0x00000004, 0xe0073ffc, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_32) | (dst_msize << 18) | (DST_TR_WIDTH_32));
	for(l=0; l<0x4000; l++)
	{
		if(R8(0xe0073ffc+l) != R8(0x10004+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0x10000, 0x8100, 0x40);
	axidma_transfer_one_block_s1020(1, 0, 0x10008, 0x00000004, 0xe0073ff8, 0xfff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_64) | (dst_msize << 18) | (DST_TR_WIDTH_64));
	for(l=0; l<0x8000; l++)
	{
		if(R8(0xe0073ff8+l) != R8(0x10008+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0x10000, 0x8100, 0x50);
	axidma_transfer_one_block_s1020(1, 0, 0x10010, 0x00000004, 0xe0073ff0, 0x7ff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_128) | (dst_msize << 18) | (DST_TR_WIDTH_128));
	for(l=0; l<0x8000; l++)
	{
		if(R8(0xe0073ff0+l) != R8(0x10010+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0x10000, 0x8100, 0x60);
	axidma_transfer_one_block_s1020(1, 0, 0x10010, 0x00000004, 0xe0073ff0, 0x3ff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_256) | (dst_msize << 18) | (DST_TR_WIDTH_256));
	for(l=0; l<0x4000; l++)
	{
		if(R8(0xe0073ff0+l) != R8(0x10010+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}

	axidma_reset();
	buf_init(0x10000, 0x8100, 0x70);
	axidma_transfer_one_block_s1020(1, 0, 0x10010, 0x00000004, 0xe0073ff0, 0x1ff,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_512) | (dst_msize << 18) | (DST_TR_WIDTH_512));
	for(l=0; l<0x2000; l++)
	{
		if(R8(0xe0073ff0+l) != R8(0x10010+l))
		{
			//PSPRINTF("dma data err!!  \n\n\t");
			   while(1);
		}
	}
#endif

#endif

#if AXIDMA_INTERRUPT_TEST
	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_int);
	//Init_Irq_Struct(&axidma_irqhandler,93,axidma_int,0,0);
#endif
	//#1 随机通道，地址,8bit位宽， 8bit，地址对齐 sram2sram
#if 0
	axidma_reset();
	for(ch=rand()%0x8+1; ch<=8; ch+=8)
	{
		for(src_msize=0; src_msize<=0; src_msize++)
		{
			for(dst_msize=0; dst_msize<=0; dst_msize++)
			{
				for(i=0; i<4; i++)
				{
					for(j=0; j<4; j++)
					{
						for(k=0; k<4; k++)
						{
							PSPRINTF("SRAM<==>SRAM:ch=%d, src_msize=%d, src_width=%d, dst_msize=%d, dst_width=%d(%d%d%d\r\n", ch, src_msize, src_width, dst_msize, dst_width,i,j,k);
							buf_init(0xe0070000+i, 0x10000+k, ch+src_msize+dst_msize+i+j+k);
						    axidma_transfer_one_block(ch, 0xe0070000+i, 0xe0080020+j, 0x10000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));
							for(l=0; l<0x10000+k; l++)
							{
								if(R8(0xe0070000+i+l) != R8(0xe0080020+j+l))
								{
									//PSPRINTF("dma data err!!  \n\n\t");
								    while(1);
								}
							}
						}
					}
				}
			}
		}
	}


	 //#2 遍历通道，随机地址, 32bit位宽， 32bit地址对齐 sram2ddr
	axidma_reset();
	for(ch=1; ch<=8; ch++)
	{
		for(src_msize=0; src_msize<=0; src_msize++)
		{
			for(dst_msize=0; dst_msize<=0; dst_msize++)
			{
				for(i=0; i<4; i++)
				{
					for(j=0; j<4; j++)
					{
						for(k=rand()%0x100; k<0x100; k+=0x100)
						{
							PSPRINTF("SRAM<==>DDR:ch=%d, src_msize=%d, src_width=%d, dst_msize=%d, dst_width=%d(%d%d%d\r\n", ch, src_msize, src_width, dst_msize, dst_width,i,j,k);
							buf_init(0xe0070000+i*4, k, k);
							axidma_transfer_one_block_s1020(ch, 0x00000004, 0xe0070000+i*4, 0, 0x800+j*4, k-1,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_32 ) | (dst_msize << 18) | (DST_TR_WIDTH_32 ));
							for(l=0; l<k; l++)
							{
								if(R8(0xe0070000+i*4+l) != R8(0x800+j*4+l))
								{
									PSPRINTF("dma data err!!  \n\n\t");
									PSPRINTF("the src is:%x, the dst is:%x, the size is:%x", R8(0xe0070000+i*4+l), R8(0x800+j*4+l), k);
									while(1);
								}
							}
						}
					}
				}
			}
		}
	}

//遍历数据请求量，位宽
	for(i=0;i<=9;i++)
	{
		axidma_reset();
		buf_init(0xe00700f0, 0x20, 0x33);
		axidma_transfer_one_block(1, 0xe00700f0, 0xe00800f0, 0x1f,  (i << 14) | (SRC_TR_WIDTH_8 ) | (i << 18) | (DST_TR_WIDTH_8 ));
		for(l=0; l<0x20; l++)
		{
			if(R8(0xe00700f0+l) != R8(0xe00800f0+l))
			{
				 while(1);
			}
		}


		axidma_reset();
		buf_init(0xe00700f0, 0x40, 0x44);
		axidma_transfer_one_block(1, 0xe00700f0, 0xe00800f0, 0x1f,  (i << 14) | (SRC_TR_WIDTH_16 ) | (i << 18) | (DST_TR_WIDTH_16 ));
		for(l=0; l<0x40; l++)
		{
			if(R8(0xe00700f0+l) != R8(0xe00800f0+l))
			{
				 while(1);
			}
		}


		axidma_reset();
		buf_init(0xe00700f0, 0x80, 0x55);
		axidma_transfer_one_block(1, 0xe00700f0, 0xe00800f0, 0x1f,  (i << 14) | (SRC_TR_WIDTH_32 ) | (i << 18) | (DST_TR_WIDTH_32));
		for(l=0; l<0x80; l++)
		{
			if(R8(0xe00700f0+l) != R8(0xe00800f0+l))
			{
				 while(1);
			}
		}

		axidma_reset();
		buf_init(0xe00700f0, 0x100, 0x66);
		axidma_transfer_one_block(1, 0xe00700f0, 0xe00800f0, 0x1f,  (i << 14) | (SRC_TR_WIDTH_64 ) | (i << 18) | (DST_TR_WIDTH_64));
		for(l=0; l<0x100; l++)
		{
			if(R8(0xe00700f0+l) != R8(0xe00800f0+l))
			{
				 while(1);
			}
		}

		//0xe0068ffe need SRC_TR_WIDTH_8
		axidma_reset();
		buf_init(0xe0078000, 0x2000, 0x66);
		axidma_transfer_one_block(1, 0xe0078ffe, 0xe0070000, 0x7f,  (i << 14) | (SRC_TR_WIDTH_16 ) | (i << 18) | (DST_TR_WIDTH_128));
		for(l=0; l<0x100; l++)
		{
			if(R8(0xe0078ffe + l) != R8(0xe0070000 + l))
			{
				 while(1);
			}
		}
	}
#endif
}

void axi_mem2mem_test_change_addr(unsigned int tc, unsigned int sah, unsigned int sal, unsigned int dah, unsigned int dal, unsigned int data_width, unsigned int step_len)
{
	unsigned char ch_num;
	unsigned int src_msize, src_width, dst_msize, dst_width, i, j, k, l, count;

	for (ch_num=rand()%0x8+1; ch_num<9; ch_num+=8) {
		for(src_msize=rand()%0x8; src_msize<9; src_msize+=9){
			for(dst_msize=rand()%0x8; dst_msize<9; dst_msize+=9){
				for(k=0; k<step_len; k++){							//0xfff times, so the sal will not same dal

					//PSPRINTF("tc=%d, ch_num=%d, data_width=%d, src_msize=%d, dst_msize=%d, sal=0x%x, dal=0x%x\n", tc, ch_num, data_width, src_msize, dst_msize, sal, dal);
					axidma_reset();
					for (i = 0; i < (tc*(0x01<<data_width)); i++) {
						*(volatile unsigned char *)(sal + i) = rand()&0xff;
					}

					axidma_transfer_one_block_s1020(ch_num, sah, sal, dah, dal, tc-1, DMS_2 | (src_msize << 14) | (data_width << 8) | (dst_msize << 18) | (data_width << 11));

					for(l=0; l<(tc*(0x01<<data_width)); l++)
					{
						if(R8(dal+l) != R8(sal+l))
							{
								PSPRINTF("dma data err!!, sal=%x, dst=%x  \n\n\t", R8(sal+l), R8(dal+l));
								while(1);
							}
					}

					sal += plb_square(data_width+1);				    	//sal from 0xe005ff00 + data_width
					dal -= plb_square(data_width+1);					    //dal from 0xe0360100 - data_width
				}
			}
		}
	}
}
#endif

#if AXIDMA_SHADOWREG_TEST
void axidma_shadowreg_test(unsigned char n)
{
	unsigned int reg, cfg_l, cfg_h, i;

	buf_init(0xe0058000, 0x4000, 0xe9+n);
	axidma_reset();

#if AXIDMA_INTERRUPT_TEST
	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_int);
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
#else
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
	//reg = IO_READ32(DMAC_CFG_REG);
	//printf("DMAC_CFG_REG : 0x%x\r\n", reg);
#endif

	cfg_l = IO_READ32(CH_CFG(n));
	cfg_h = IO_READ32(CH_CFG_H(n));
	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_SHADOW_REG | SRC_MULTBLK_TYPE_SHADOW_REG );
	reg = IO_READ32(CH_CFG(n));
	//printf("CH_CFG(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
	reg = IO_READ32(CH_CFG_H(n));
	//printf("CH_CFG_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_SAR(n), 0xe0058008);
	//reg = IO_READ32(CH_SAR(n));
//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);
	//reg = IO_READ32(CH_SAR_H(n));
//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0060000);
	//reg = IO_READ32(CH_DAR(n));
//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	//reg = IO_READ32(CH_DAR_H(n));
//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x1ff);
	//reg = IO_READ32(CH_BLOCK_TS(n));
//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_64 | DST_TR_WIDTH_64 | SRC_MSIZE_4 | DST_MSIZE_16);

	//reg = IO_READ32(CH_CTL(n));
//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID);
	reg = IO_READ32(CH_CTL_H(n));
	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
	//reg = IO_READ32(DMAC_CH_EN_REG);
//	printf("DMAC_CH_EN_REG : 0x%x\r\n", reg);

	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
		PSPRINTF("CH_INT_STATUS_REG(n)--- : 0x%x\r\n", reg);
		reg = IO_READ32(CH_CTL_H(n));
	}while(reg & SHADOWREG_LLI_VALID);

	for(i=0; i<0x1000; i++)
	{
		if(R8(i+0xe0058008) != R8(i+0xe0060000))
			while(1);
	}

	IO_WRITE32(CH_SAR(n), 0xe005800c);
	reg = IO_READ32(CH_SAR(n));
	//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);
	reg = IO_READ32(CH_SAR_H(n));
	//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0061000);
	reg = IO_READ32(CH_DAR(n));
	//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	reg = IO_READ32(CH_DAR_H(n));
	//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x3ff);
	reg = IO_READ32(CH_BLOCK_TS(n));
	//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_32 | DST_TR_WIDTH_64 | SRC_MSIZE_128  | DST_MSIZE_1);
	reg = IO_READ32(CH_CTL(n));
	//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID);
	reg = IO_READ32(CH_CTL_H(n));
	PSPRINTF("CH_CTL(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLK_TFR_RESUMEREQ_REG(n),BLK_TFR_RESUMEREQ);								//add
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
				PSPRINTF("CH_INT_STATUS_REG(n)--- : 0x%x\r\n", reg);
		reg = IO_READ32(CH_CTL_H(n));
	} while (reg & SHADOWREG_LLI_VALID);

	for(i=0; i<0x1000; i++)
	{
		if(R8(i+0xe005800c) != R8(i+0xe0061000))
			while(1);
	}

	IO_WRITE32(CH_SAR(n), 0xe005800e);
	reg = IO_READ32(CH_SAR(n));
	//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);
	reg = IO_READ32(CH_SAR_H(n));
	//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0062000);
	reg = IO_READ32(CH_DAR(n));
	//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	reg = IO_READ32(CH_DAR_H(n));
	//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x7ff);
	reg = IO_READ32(CH_BLOCK_TS(n));
	//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_16 | DST_TR_WIDTH_64 | SRC_MSIZE_512  | DST_MSIZE_8);
	reg = IO_READ32(CH_CTL(n));
	//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID);
	reg = IO_READ32(CH_CTL_H(n));
	///	printf("CH_CTL(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLK_TFR_RESUMEREQ_REG(n),BLK_TFR_RESUMEREQ);							//add
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
				PSPRINTF("CH_INT_STATUS_REG(n)--- : 0x%x\r\n", reg);
		reg = IO_READ32(CH_CTL_H(n));
	} while (reg & SHADOWREG_LLI_VALID);

	for(i=0; i<0x1000; i++)
	{
		if(R8(i+0xe005800e) != R8(i+0xe0062000))
			while(1);
	}

	IO_WRITE32(CH_SAR(n), 0xe005800f);
//	reg = IO_READ32(CH_SAR(n));
	//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);
	//reg = IO_READ32(CH_SAR_H(n));
	//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0063000);
	//reg = IO_READ32(CH_DAR(n));
	//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
//	reg = IO_READ32(CH_DAR_H(n));
	//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0xfff);
	///reg = IO_READ32(CH_BLOCK_TS(n));
	//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_8 | DST_TR_WIDTH_64 | SRC_MSIZE_1024  | SRC_MSIZE_256 );
	//reg = IO_READ32(CH_CTL(n));
	//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST);
	//reg = IO_READ32(CH_CTL_H(n));
	///	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_BLK_TFR_RESUMEREQ_REG(n),BLK_TFR_RESUMEREQ);							//add

#if AXIDMA_INTERRUPT_TEST
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;
#else
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
	}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
	IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
#endif

	delay(0x100);
	for(i=0; i<0x1000; i++)
	{
		unsigned char tmp1, tmp2;
		if(R8(i+0xe005800f) != R8(i+0xe0063000))
			while(1);
	}
}
#endif

#if AXIDMA_RELOAD_TEST
void axidma_reload_test(unsigned char n)
{
	unsigned int reg, i;
	unsigned int cfg_l, cfg_h;

	buf_init(0xe0058000, 0xfff, 0x9a);
	axidma_reset();

	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_reload_int);
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);

	reg = IO_READ32(DMAC_CFG_REG);
//	printf("DMAC_CFG_REG : 0x%x\r\n", reg);

	IO_WRITE32(CH_SAR(n), 0xe0058000);
	reg = IO_READ32(CH_SAR(n));
//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);
	reg = IO_READ32(CH_SAR_H(n));
//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0060000);
	reg = IO_READ32(CH_DAR(n));
//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	reg = IO_READ32(CH_DAR_H(n));
//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x1000);
	reg = IO_READ32(CH_BLOCK_TS(n));
//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), DMS_2);
	reg = IO_READ32(CH_CTL(n));
//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), IOC_BLKTFR);
	reg = IO_READ32(CH_CTL_H(n));
///	printf("CH_CTL(n) : 0x%x\r\n", reg);

	cfg_l = IO_READ32(CH_CFG(n));
	cfg_h = IO_READ32(CH_CFG_H(n));
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
	reg = IO_READ32(CH_CFG_H(n));
//	printf("CH_CFG_H(n) : 0x%x\r\n", reg);
//	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_CONTIGUOUS | SRC_MULTBLK_TYPE_CONTIGUOUS );
	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_RELOAD | SRC_MULTBLK_TYPE_RELOAD );
	reg = IO_READ32(CH_CFG(n));
	PSPRINTF("CH_CFG(n) : 0x%x\r\n", reg);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
	reg = IO_READ32(DMAC_CH_EN_REG);
//	printf("DMAC_CH_EN_REG : 0x%x\r\n", reg);

	while(1)
	{
		while(axidma_int_flag == 0);
		axidma_int_flag = 0;
		PSPRINTF("Reload test[%d] is OK\r\n", i++);
	}

}
#endif

#if AXIDMA_MIXEDTYPE_TEST
void axidma_mixedtype_test(unsigned char n)
{
	unsigned int reg, cfg_l, cfg_h, i;
	unsigned int *llp1 = (unsigned int *)0xe0050000;
	unsigned int *llp2 = (unsigned int *)0xe0051000;
	unsigned int *llp3 = (unsigned int *)0xe0052000;
	unsigned int *llp4 = (unsigned int *)0xe0053000;

	//#1 contiguous & shadowreg
	buf_init(0xe0058000, 0x10000, 0x4d+n);
	axidma_reset();

#if AXIDMA_INTERRUPT_TEST
	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_int);
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
#else
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
	reg = IO_READ32(DMAC_CFG_REG);
	//printf("DMAC_CFG_REG : 0x%x\r\n", reg);
#endif

	cfg_l = IO_READ32(CH_CFG(n));
	cfg_h = IO_READ32(CH_CFG_H(n));
	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_SHADOW_REG | SRC_MULTBLK_TYPE_CONTIGUOUS );
	reg = IO_READ32(CH_CFG(n));
	//printf("CH_CFG(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
	reg = IO_READ32(CH_CFG_H(n));
	//printf("CH_CFG_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_SAR(n), 0xe0058000);
	reg = IO_READ32(CH_SAR(n));
//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);
	reg = IO_READ32(CH_SAR_H(n));
//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0068000);
	reg = IO_READ32(CH_DAR(n));
//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	reg = IO_READ32(CH_DAR_H(n));
//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x1FF6);				// 0x1ff7*2 = 16K-18
	reg = IO_READ32(CH_BLOCK_TS(n));
//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_16 | DST_TR_WIDTH_16);
	reg = IO_READ32(CH_CTL(n));
//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID);
	reg = IO_READ32(CH_CTL_H(n));
///	printf("CH_CTL(n) : 0x%x\r\n", reg);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
	reg = IO_READ32(DMAC_CH_EN_REG);
//	printf("DMAC_CH_EN_REG : 0x%x\r\n", reg);

	do {
		reg = IO_READ32(CH_CTL_H(n));
	}while(reg & SHADOWREG_LLI_VALID);

	//IO_WRITE32(CH_SAR(n), 0x131ffc);
	//reg = IO_READ32(CH_SAR(n));
	//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	//IO_WRITE32(CH_SAR_H(n), 0);
	//reg = IO_READ32(CH_SAR_H(n));
	//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xE006BFEE);
	reg = IO_READ32(CH_DAR(n));
	//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	reg = IO_READ32(CH_DAR_H(n));
	//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x2010);				//0x2011*2 = 16K+34
	reg = IO_READ32(CH_BLOCK_TS(n));
	//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_16 | DST_TR_WIDTH_16);
	reg = IO_READ32(CH_CTL(n));
	//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID);
	reg = IO_READ32(CH_CTL_H(n));
	///	printf("CH_CTL(n) : 0x%x\r\n", reg);

	do {
		reg = IO_READ32(CH_CTL_H(n));
	} while (reg & SHADOWREG_LLI_VALID);

	//IO_WRITE32(CH_SAR(n), 0x133ffe);
	//reg = IO_READ32(CH_SAR(n));
	//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	//IO_WRITE32(CH_SAR_H(n), 0);
	//reg = IO_READ32(CH_SAR_H(n));
	//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0070010);
	reg = IO_READ32(CH_DAR(n));
	//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	reg = IO_READ32(CH_DAR_H(n));
	//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x1ffa);				//0x1FFB*2 = 16K-10
	reg = IO_READ32(CH_BLOCK_TS(n));
	//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_16 | DST_TR_WIDTH_16);
	reg = IO_READ32(CH_CTL(n));
	//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID);
	reg = IO_READ32(CH_CTL_H(n));
	///	printf("CH_CTL(n) : 0x%x\r\n", reg);

	do {
		reg = IO_READ32(CH_CTL_H(n));
	} while (reg & SHADOWREG_LLI_VALID);

	//IO_WRITE32(CH_SAR(n), 0x135fff);
	//reg = IO_READ32(CH_SAR(n));
	//	printf("CH_SAR(n) : 0x%x\r\n", reg);
	//IO_WRITE32(CH_SAR_H(n), 0x80000050);
	//reg = IO_READ32(CH_SAR_H(n));
	//	printf("CH_SAR_H(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR(n), 0xe0074006);
	reg = IO_READ32(CH_DAR(n));
	//	printf("CH_DAR(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_DAR_H(n), 0x00000004);
	reg = IO_READ32(CH_DAR_H(n));
	//	printf("CH_DAR_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_BLOCK_TS(n), 0x1ffc);				//0x1FFD*2 = 8K-6
	reg = IO_READ32(CH_BLOCK_TS(n));
	//	printf("CH_BLOCK_TS(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_CTL(n), SMS_2 | SRC_TR_WIDTH_16 | DST_TR_WIDTH_16);
	reg = IO_READ32(CH_CTL(n));
	//	printf("CH_CTL(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CTL_H(n), SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST);
	reg = IO_READ32(CH_CTL_H(n));
	///	printf("CH_CTL(n) : 0x%x\r\n", reg);

#if AXIDMA_INTERRUPT_TEST
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;
#else
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
	}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
	IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
#endif

	for(i=0; i<0x10000; i++)
	{
		if(R8(i+0xe0058000) != R8(i+0xe0068000))
			while(1);
	}


	//#2 reload & lli
	buf_init(0xe0058000, 0x8000, 0x7f+n);
	axidma_reset();

	llp1[0] = swap_word(0);
	llp1[1] = swap_word(0);
	llp1[2] = swap_word(0xe0068000);
	llp1[3] = swap_word(0x00000004);
	llp1[4] = swap_word(0x7ff);
	llp1[5] = swap_word(0);
	llp1[6] = swap_word((unsigned int)llp2);
	llp1[7] = swap_word(0x00000004);
	llp1[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
	llp1[9] = swap_word(SHADOWREG_LLI_VALID);

	llp2[0] = swap_word(0);
	llp2[1] = swap_word(0);
	llp2[2] = swap_word(0xe006a000);
	llp2[3] = swap_word(0x00000004);
	llp2[4] = swap_word(0x7ff);
	llp2[5] = swap_word(0);
	llp2[6] = swap_word((unsigned int)llp3);
	llp2[7] = swap_word(0x00000004);
	llp2[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
	llp2[9] = swap_word(SHADOWREG_LLI_VALID);


	llp3[0] = swap_word(0);
	llp3[1] = swap_word(0);
	llp3[2] = swap_word(0xe006c000);
	llp3[3] = swap_word(0x00000004);
	llp3[4] = swap_word(0x7ff);
	llp3[5] = swap_word(0);
	llp3[6] = swap_word((unsigned int)llp4);
	llp3[7] = swap_word(0x00000004);
	llp3[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
	llp3[9] = swap_word(SHADOWREG_LLI_VALID);


	llp4[0] = swap_word(0);
	llp4[1] = swap_word(0);
	llp4[2] = swap_word(0xe006e000);
	llp4[3] = swap_word(0x00000004);
	llp4[4] = swap_word(0x7ff);
	llp4[5] = swap_word(0);
	llp4[6] = swap_word(0);
	llp4[7] = swap_word(0);
	llp4[8] = swap_word(SRC_TR_WIDTH_32 | DST_TR_WIDTH_32 | DMS_2);
	llp4[9] = swap_word(SHADOWREG_LLI_VALID | SHADOWREG_LLI_LAST);

#if AXIDMA_INTERRUPT_TEST
	InstallInterruptHandler0(18,axidma_int);
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
#else
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN);
	reg = IO_READ32(DMAC_CFG_REG);
	//printf("DMAC_CFG_REG : 0x%x\r\n", reg);
#endif

	IO_WRITE32(CH_SAR(n), 0xe0058000);
	IO_WRITE32(CH_SAR_H(n), 0x00000004);

	cfg_l = IO_READ32(CH_CFG(n));
	cfg_h = IO_READ32(CH_CFG_H(n));
	IO_WRITE32(CH_CFG(n), (cfg_l & DST_MULTBLK_TYPE_MASK & SRC_MULTBLK_TYPE_MASK) | DST_MULTBLK_TYPE_LINKED_LIST | SRC_MULTBLK_TYPE_RELOAD );
	reg = IO_READ32(CH_CFG(n));
	//printf("CH_CFG(n) : 0x%x\r\n", reg);
	IO_WRITE32(CH_CFG_H(n), (cfg_h & TT_FC_MASK) | TT_FC_MEM2MEM);
	reg = IO_READ32(CH_CFG_H(n));
	//printf("CH_CFG_H(n) : 0x%x\r\n", reg);

	IO_WRITE32(CH_LLP(n), (unsigned int)llp1);
	IO_WRITE32(CH_LLP_H(n), 0x00000004);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));
	reg = IO_READ32(DMAC_CH_EN_REG);
	//printf("DMAC_CH_EN_REG : 0x%x\r\n", reg);

#if AXIDMA_INTERRUPT_TEST
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;
#else
	do {
		reg = IO_READ32(CH_INT_STATUS_REG(n));
	}while((reg & DMA_TFR_DONE_INTSTAT) == 0);
	IO_WRITE32(CH_INT_CLEAR_REG(n), reg);
#endif

	for(i=0; i<0x2000; i++)
	{
		if(R8(i+0xe0058000) != R8(i+0xe0068000))
			while(1);
	}

	for(i=0; i<0x2000; i++)
	{
		if(R8(i+0xe0058000) != R8(i+0xe006a000))
			while(1);
	}

	for(i=0; i<0x2000; i++)
	{
		if(R8(i+0xe0058000) != R8(i+0xe006c000))
			while(1);
	}

	for(i=0; i<0x2000; i++)
	{
		if(R8(i+0xe0058000) != R8(i+0xe006e000))
			while(1);
	}
}
#endif

#if AXIDMA_CHSTAT_TEST
void axidma_chstat_test(unsigned char n)
{
	unsigned int reg, i;
	//Init_Irq_Struct(&axidma_irqhandler,93,axidma_int,0,0);
	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_int);
	//#1 disable status
	axidma_reset();
	buf_init(0xe0050000, 0x10000, 0x66+n);
	axidma_transfer_one_block(n, 0xe0050000, 0xe0060000, 0x10000-1, SMS_2);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN_WE(n));
	while(axidma_int_flag == 0);
		axidma_int_flag = 0;

	//#2 abort status
	axidma_reset();
	buf_init(0xe0050000, 0x10000, 0x16+n);
	memset(0xe0060000, 0, 0x10000);
	axidma_transfer_one_block(n, 0xe0050000, 0xe0060000, 0x10000-1, SMS_2);

	IO_WRITE32(DMAC_CH_EN_REG_H, CH_ABORT_WE(n) | CH_ABORT(n));
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;

	//#3 suspend status
	axidma_reset();
	buf_init(0xe0050000, 0x10000, 0x36+n);
	memset(0xe0060000, 0, 0x10000);
	axidma_transfer_one_block(n, 0xe0050000, 0xe0060000, 0x10000-1, SMS_2);

	IO_WRITE32(DMAC_CH_EN_REG, CH_SUSP(n) | CH_SUSP_WE(n));
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;

	IO_WRITE32(DMAC_CH_EN_REG, CH_SUSP_WE(n));
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;

	for(i=0; i<0x10000; i++)
	{
		if(R8(0xe0050000+i) != R8(0xe0060000+i))
			while(1);
	}

	//#4 channel lock cleared status
	axidma_reset();
	buf_init(0xe0050000, 0x10000, 0x36+n);
	memset(0xe0060000, 0, 0x10000);
	//axidma_transfer_one_block(n, 0x80000000, 0x80010000, 0x10000-1, SMS_2);

	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
	IO_WRITE32(DMAC_COMMON_INT_STATUS_EN_REG, 0);
		for(i=1; i<=8; i++)
		{
			IO_WRITE32(CH_INT_STATUS_EN_REG(i), DMA_TFR_DONE_INTSTAT);
		}
	axidma_init_one_channel(n, 0xe0050000, 0xe0060000, 0x10000-1, SMS_2 , LOCK_CH, 0);
	IO_WRITE32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));

	//reg = CH_CFG_H(n);
	IO_WRITE32(CH_CFG_H(n), reg & (LOCK_CH));
	while(axidma_int_flag == 0);
	axidma_int_flag = 0;
}
#endif

#if AXIDMA_CHPRIOR_TEST
void axidma_chprior_test()
{
	unsigned int i;
	unsigned int tmp;
	//Init_Irq_Struct(&axidma_irqhandler,93,axidma_chprior_int,0,0);

	//normal prior test
	axidma_reset();
	InstallInterruptHandler0(AXIDMA_INT_NUM,axidma_chprior_int);
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);

//	axidma_reset();
//	IO_WRITE32(DMAC_CFG_REG, DMAC_EN);

	IO_WRITE32(DMAC_COMMON_INT_STATUS_EN_REG, 0);
	for(i=1; i<=8; i++)
	{
		IO_WRITE32(CH_INT_STATUS_EN_REG(i), DMA_TFR_DONE_INTSTAT);
	}
	buf_init(0xe0050000, 0x10000, 0xaa);
	//31064572
	axidma_init_one_channel(1, 0xe0050000, 0xe0058000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(0), 0);
	axidma_init_one_channel(2, 0xe0050000, 0xe0059000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(1), 0);
	axidma_init_one_channel(3, 0xe0050000, 0xe005a000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(2), 0);
	axidma_init_one_channel(4, 0xe0050000, 0xe005b000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(3), 0);
	axidma_init_one_channel(5, 0xe0050000, 0xe005c000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(4), 0);
	axidma_init_one_channel(6, 0xe0050000, 0xe005d000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(5), 0);
	axidma_init_one_channel(7, 0xe0050000, 0xe005e000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(6), 0);
	axidma_init_one_channel(8, 0xe0050000, 0xe005f000, 0x10000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7), 0);
//	unsigned char reg, reg2, tmp2[8] = {0};
	IO_WRITE32(DMAC_CH_EN_REG, CH_EN_WE_ALL | CH_EN_ALL);
/*	while(index<8){
		do{
			reg2 = reg;
			reg = IO_READ32(DMAC_INT_STATUS_REG);
		}while((reg == 0) || (reg == reg2));

		tmp2[index++] = reg;
		IO_WRITE32(CH_INT_CLEAR_REG(index), 0xffffffff);
	}
*/
	while(axidma_int_flag != 0xff);
	axidma_int_flag = 0;
	for(i=0; i<8; i++)
		PSPRINTF("%d", prior_array[i]);
	PSPRINTF("\r\n");



	axidma_reset();
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
	IO_WRITE32(DMAC_COMMON_INT_STATUS_EN_REG, 0);
	for(i=1; i<=8; i++)
	{
		IO_WRITE32(CH_INT_STATUS_EN_REG(i), DMA_TFR_DONE_INTSTAT);
	}
	//31064572
	axidma_init_one_channel(1, 0xe0050000, 0xe0058000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(3) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);
	axidma_init_one_channel(2, 0xe0050000, 0xe0059000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(1) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);
	axidma_init_one_channel(3, 0xe0050000, 0xe005a000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(0) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);
	axidma_init_one_channel(4, 0xe0050000, 0xe005a000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(6) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);
	axidma_init_one_channel(5, 0xe0050000, 0xe005b000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(4) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);
	axidma_init_one_channel(6, 0xe0050000, 0xe005c000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(5) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);
	axidma_init_one_channel(7, 0xe0050000, 0xe005d000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);
	axidma_init_one_channel(8, 0xe0050000, 0xe005e000, 0x8000-1, SMS_2| SRC_MSIZE_256 | DST_MSIZE_256, CH_PRIOR(2) | HS_SEL_SRC | HS_SEL_DST| LOCK_CH, 0);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN_WE_ALL | CH_EN_ALL);

	while(axidma_int_flag != 0xff);
	axidma_int_flag = 0;
	PSPRINTF("running channel num : 0x%x\r\n", axidma_int_tmp);
	axidma_int_tmp=0;
	for(i=0; i<0x20; i+=4)
	{
		 tmp = IO_READ32(0xe006fe00+i);
		 PSPRINTF("running channel num : 0x%x\r\n", tmp);
	}

	IO_WRITE32(0xe006ff00, 0x2);
	tmp = IO_READ32(0xe006ff00);
	PSPRINTF("running channel num : 0x%x\r\n", tmp);

	printf("\r\n");

	axidma_reset();
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
	IO_WRITE32(DMAC_COMMON_INT_STATUS_EN_REG, 0);
	for(i=1; i<=8; i++)
	{
		IO_WRITE32(CH_INT_STATUS_EN_REG(i), DMA_TFR_DONE_INTSTAT);
	}
	buf_init(0xe0050000, 0x8000, 0xaa);
	//21436587
	axidma_init_one_channel(1, 0xe0050000, 0xe0058000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);
	axidma_init_one_channel(2, 0xe0050000, 0xe0059000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);
	axidma_init_one_channel(3, 0xe0050000, 0xe005a000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);
	axidma_init_one_channel(4, 0xe0050000, 0xe005b000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15) | LOCK_CH, 0);
	axidma_init_one_channel(5, 0xe0050000, 0xe005c000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);
	axidma_init_one_channel(6, 0xe0050000, 0xe005d000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);
	axidma_init_one_channel(7, 0xe0050000, 0xe005e000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);
	axidma_init_one_channel(8, 0xe0050000, 0xe005f000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN_WE_ALL | CH_EN_ALL);

	while(axidma_int_flag != 0xff);
	axidma_int_flag = 0;
	for(i=0; i<8; i++)
		PSPRINTF("%d", prior_array[i]);
	PSPRINTF("\r\n");

	axidma_reset();
	IO_WRITE32(DMAC_CFG_REG, DMAC_EN | INT_EN);
	IO_WRITE32(DMAC_COMMON_INT_STATUS_EN_REG, 0);
	for(i=1; i<=8; i++)
	{
//		IO_WRITE32(CH_INT_STATUS_EN_REG(i), DMA_TFR_DONE_INTSTAT);
	}
	buf_init(0xe0050000, 0x8000, 0xaa);
	axidma_init_one_channel(1, 0xe0050000, 0xe0058000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(0) | SRC_OSR_LMT(0), 0);
	axidma_init_one_channel(2, 0xe0050000, 0xe0059000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(0) | SRC_OSR_LMT(0), 0);
	axidma_init_one_channel(3, 0xe0050000, 0xe005a000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(0) | SRC_OSR_LMT(0), 0);
	axidma_init_one_channel(4, 0xe0050000, 0xe005b000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(15) | SRC_OSR_LMT(15), 0);
	axidma_init_one_channel(5, 0xe0050000, 0xe005c000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(0) | SRC_OSR_LMT(0), 0);
	axidma_init_one_channel(6, 0xe0050000, 0xe005d000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(0) | SRC_OSR_LMT(0), 0);
	axidma_init_one_channel(7, 0xe0050000, 0xe005e000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(0) | SRC_OSR_LMT(0), 0);
	axidma_init_one_channel(8, 0xe0050000, 0xe005f000, 0x8000-1, SMS_2 | SRC_MSIZE_1024 | DST_MSIZE_1024, CH_PRIOR(7) | HS_SEL_SRC | HS_SEL_DST | DST_OSR_LMT(0) | SRC_OSR_LMT(0), 0);

	IO_WRITE32(DMAC_CH_EN_REG, CH_EN_WE_ALL | CH_EN_ALL);

	while(axidma_int_flag != 0xff);
	axidma_int_flag = 0;
	for(i=0; i<8; i++)
		PSPRINTF("%d", prior_array[i]);
	PSPRINTF("\r\n");

}
#endif

#if AXIDMA_SW_HS_TEST
void axidma_sw_hs_test(unsigned char n)
{
	unsigned int reg, i;

	//#1 mem2per
	buf_init(0x80000000, 0x10000, 0x30);
	axidma_init_one_channel(n, 0x80000000, 0x80010000, 0x1000-1, DMS_2, HS_SEL_SRC | HS_SEL_DST | TT_FC_MEM2PER, DST_MULTBLK_TYPE_CONTIGUOUS | SRC_MULTBLK_TYPE_CONTIGUOUS);
	IO_WRITE32(DMAC_CH_EN_REG, CH_EN_WE(n) | CH_EN(n));

	IO_WRITE32(CH_SWHSDST_REG(n), SWHS_REQ_WE | SWHS_REQ);
	do
	{
	reg = IO_READ32(CH_SWHSDST_REG(n));
	}while(reg & SWHS_REQ);

	for(i=0; i<0x1000; i++)
	{
		if(R8(0x80000000+i) != R8(0x80010000+i))
			while(1);
	}
}
#endif
/*
#if AXIDMA_MEM2PER_TRANSFER_TEST
void axidma_mem2per_transfer()
{
	unsigned char i, data;
	unsigned int reg_val;

	//****** Channel 0 test ******
	// Initialize UART0
//	uart0_init();

	// Store data to source address
	data = 0x12;
	for(i=0;i<0x10;i++)
		IO_WRITE8(0xE0340000+i, data+i);

	// Set control status
	MFDCR(reg_val, DMAx_CR0);
	reg_val |= CIE|PL|PW_BYTE|SAI|BEN|TM_PER|PSC_1|PWC_2|PHC_1|ETD|TCE|SL;
	MTDCR(DMAx_CR0, reg_val);

	// Set source address
	MTDCR(DMAx_SAH0, 0x50);
	MTDCR(DMAx_SAL0, 0xE0020000);

	// Set destination address
	MTDCR(DMAx_DAH0, UART_HADDR);
	MTDCR(DMAx_DAL0, UART0_THR);

	// Set transfer count
	MTDCR(DMAx_CTC0, TC_16);

	// Clear status
	MTDCR(DMAx_SR, 0xFFFFFFFF);

	// Start transfer
	MFDCR(reg_val,DMAx_CR0);
	reg_val |= 0x80000000;
	MTDCR(DMAx_CR0,reg_val);

	// Wait for transfer to be finished
	MFDCR(reg_val, DMAx_SR);
	while(reg_val != 0x80000000)
		MFDCR(reg_val, DMAx_SR);

	MTDCR(DMAx_SR, 0xFFFFFFFF);

	for(i=0; i<0x10; i++) {
		if(read_b(UART0_RBR) != (data+i))
			uart_printf("dma_plb4_per2mem_tranfer: Output data mismatch, addr 0x%x, data 0x%x\n", (0xE0020000+i), (data+i));
	}

	while(1);
}
#endif
*/
void axidma_test()
{
	unsigned int tmp, tc;
	unsigned int i, step_len, data_width;

#if AXIDMA_CONTIGUOUS_TEST
	//test 1, need change 	axidma_transfer_one_block->sah and dah
	axidma_contiguous_test();
	#if 0
	//test2, data_width=0 -- 8bit, data_width=1 -- 16bit, data_width=2 -- 32bit, data_width=6 -- 512bit,
	for(tc=0x01; tc<0x430; tc+=0x7)								//tc<0x430
	{
		step_len = 0x2;					//0x60
		for(data_width=0; data_width<5; data_width++)			//axi width from 32bit to 512bit(fact is 128bit)
		{
			//axi_sram to axi_sram
			//axi_mem2mem_test_change_addr(tc, 0x00000004, 0xe004ffe0, 0x00000004, 0xe005f020, data_width, step_len);
			//axi_sram to DDR
			//axi_mem2mem_test_change_addr(tc, 0x00000004, 0xe004ffe0, 0x00000000, 0x100020, data_width, step_len);
			//DDR to DDR
			axi_mem2mem_test_change_addr(tc, 0x00000000, 0xfe0, 0x00000000, 0x100020, data_width, step_len);
			//DDR to axi_sram
			//axi_mem2mem_test_change_addr(tc, 0x00000000, 0xfe0, 0x00000004, 0xe005f020, data_width, step_len);
		}
	}

	PSPRINTF("###### AXIDMA transmission test finish. ######\r\n");
	//while(1);
	#endif
#endif

#if AXIDMA_LLI_TEST
	for(tmp=1; tmp<=8; tmp++)
		axidma_lli_test1(tmp);
#endif

#if AXIDMA_SHADOWREG_TEST
	for(tmp=1; tmp<=8; tmp++)
		axidma_shadowreg_test(tmp);
#endif

#if AXIDMA_RELOAD_TEST
	axidma_reload_test(3);
#endif

#if AXIDMA_MIXEDTYPE_TEST
	axidma_mixedtype_test(5);
#endif

#if AXIDMA_CHSTAT_TEST
	axidma_chstat_test(2);
#endif

	//while(1);
}
