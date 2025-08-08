/*
 * acu_test.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: yyang
 */

#include "acu.h"
#include "axidma.h"
#include "secmon.h"
#include "e500mc_asm.h"
#include "std_data.h"
#include "sec_common.h"
#include "hash.h"
#include "jr.h"
#include "fsl_hash.h"
#include <stdlib.h>
#include "desc.h"
#include "desc_constr.h"

static int acu_int_flag = 0;
static int jr_index = 0;

extern int jr_index;

const u8 data[] = {
		/* DATA 2 */
			0x2C,0xB6,0xA0,0x6E,0x92,0xDC,0x95,0x70,0x24,0x2E,0x4E,0x77,0xA1,0x8D,0x6D,0xF0,
			0xBF,0xA1,0x63,0xB7,0x33,0xA6,0x81,0xDF,0xA8,0x6F,0x48,0x65,0x0A,0x25,0x1F,0x76,
			0x2C,0xB6,0xA0,0x6E,0x92,0xDC,0x95,0x70,0x24,0x2E,0x4E,0x77,0xA1,0x8D,0x6D,0xF0,
			0xBF,0xA1,0x63,0xB7,0x33,0xA6,0x81,0xDF,0xA8,0x6F,0x48,0x65,0x0A,0x25,0x1F,0x76,
			0x2C,0xB6,0xA0,0x6E,0x92,0xDC,0x95,0x70,0x24,0x2E,0x4E,0x77,0xA1,0x8D,0x6D,0xF0,
			0xBF,0xA1,0x63,0xB7,0x33,0xA6,0x81,0xDF,0xA8,0x6F,0x48,0x65,0x0A,0x25,0x1F,0x76,
			0x2C,0xB6,0xA0,0x6E,0x92,0xDC,0x95,0x70,0x24,0x2E,0x4E,0x77,0xA1,0x8D,0x6D,0xF0,
			0xBF,0xA1,0x63,0xB7,0x33,0xA6,0x81,0xDF,0xA8,0x6F,0x48,0x65,0x0A,0x25,0x1F,0x76,
};

CONST_ST u8 sha1_stddata_ex[] = {
	/* STD OUT 2 */
	0xf8,0x7b,0x6b,0x9a,0x7f,0x44,0x2d,0x26,0x88,0x6f,0xd1,0x5c,0xc3,0xf9,0xd0,0x2a,0xa3,0x4f,0xd8,0x88,

	0xe3,0xa0,0xc8,0x57,0xbc,0xaf,0xf9,0x3a,0x60,0xf6,0x1e,0x49,0x95,0x3a,0xc2,0x97,0x42,0x41,0x47,0xe5,
};

CONST_ST u8 sha0_stddata_ex[] = {
	/* STD OUT 2 */
	0x7c,0x49,0xa0,0x3c,0xba,0xd5,0x1a,0x78,0xdb,0x4e,0x81,0xd6,0xd7,0x0b,0x8a,0x60,0xe0,0x5b,0x8d,0xdb,

	0x86,0xa5,0x9e,0x66,0xf7,0x1e,0x57,0xa0,0xf8,0xba,0xf4,0xb6,0x51,0xcb,0xbc,0xe8,0x20,0x38,0xd6,0x09,
};

CONST_ST u8 sm3_stddata_ex[] = {
	/* STD OUT 2 */
	0x8E,0xD6,0xCD,0xEC,0x55,0x96,0xEA,0x9B,0x57,0x7A,0x19,0x5E,0x0A,0x41,0x9F,0xD9,
	0xFD,0x40,0x2C,0xCF,0x8B,0x11,0xB5,0x47,0xEC,0x99,0xAC,0xEE,0x81,0xB4,0xF4,0x88,

	0x88,0x32,0x99,0xb3,0x41,0x00,0x10,0x19,0x69,0xca,0x14,0x52,0xb6,0x4d,0x82,0xad,
	0x9e,0xfd,0xa0,0x2b,0xb4,0x74,0x9d,0x8b,0x1b,0xd1,0x21,0x37,0xc1,0x15,0x7b,0x6f,
};


const unsigned char data_e[256] = "\x01\x00\x01";

const unsigned char data_n[256] =
		"\xdb\x89\x6b\x19\x98\x33\xb3\x4b\xd9\xe5\xed\xc2\x72\xea\xc0\x2f"
		"\xc4\x9e\xde\x34\x07\x01\xe6\x3a\x7e\xc5\x7c\x61\xe4\xd0\xb2\xc0"
		"\xb0\xee\x9e\x34\x84\x92\xd3\x3c\x89\x6c\x1a\xe7\x2f\x25\xab\xc6"
		"\xeb\xbf\x32\x29\xbd\xcf\x5e\x09\x38\x22\x25\x2f\xf9\x8c\x55\xf4"
		"\x6f\x01\x6e\xff\xa4\xeb\x8e\x2b\x5d\xd3\x8e\x83\xa4\xe0\x56\xfe"
		"\x23\x82\x00\xf8\x06\xa4\xf3\xf5\xda\xc3\x52\x02\xc5\xbc\x55\x2f"
		"\x18\xf8\xf9\x32\x2f\x8b\x62\xd5\xa1\xcd\x1b\x91\x60\xe1\xaa\xbb"
		"\x6d\xf5\x71\x75\x62\x24\x9f\x18\x3d\x4c\xb8\x3f\x51\x6b\x94\xe1"
		"\x1c\xc9\xd1\x22\xf5\x1f\xb5\xfa\xfb\x26\x1f\xf5\xc4\xb9\x80\xc0"
		"\x93\xa3\x98\x87\x9a\x30\xf7\x47\x9f\xf7\x51\x86\x76\xee\x65\xe9"
		"\x36\x00\x63\x68\x74\x48\x3d\x0a\xce\x7f\xe6\x96\x6b\xc0\x5f\x4d"
		"\x24\x85\x1e\xca\xfc\x0b\x60\xe1\xfa\xed\xc7\x53\x9b\x0d\x37\x1a"
		"\xf7\xf0\x56\x12\xe4\x82\xa8\x24\x54\x29\x3a\x95\x4c\x42\xa9\x57"
		"\x9c\xb1\x47\xf5\xe7\xfc\x38\xce\xd5\x81\xf2\x75\x98\x6a\xc1\x40"
		"\x04\x3b\xd1\x9e\xd9\x9e\xff\x27\x03\x18\x1b\xc4\x2c\x24\x03\x16"
		"\xa3\xe7\x7a\xb5\x82\x85\xc3\xa7\xf4\x91\xed\x92\x7d\x4e\xbe\xad";

const unsigned char data_sign[256] =
		"\x18\x47\xd4\xd0\xf2\x6d\x2d\xcf\xc2\x0d\x5a\x58\xda\x50\xa0\xc2"
		"\x8b\x9c\x53\x98\xfe\x82\xe5\x3f\x1b\xcd\x5a\xe8\x1c\x63\xc6\x3d"
		"\x66\x83\x5b\x86\xfc\xbc\xac\xb6\x9b\x68\xc5\xd4\xc9\x33\x47\x17"
		"\xe7\x8d\x82\x41\x3a\x31\xab\x1f\x0b\x3c\x49\xf9\xc8\x02\x26\x57"
		"\x7a\x50\x5c\x91\x4f\xae\x97\x40\x7c\x9b\x95\x79\x7a\x33\xe1\xcd"
		"\xa5\x21\x17\x2f\xd0\x33\x17\xea\x3b\x04\x8c\xc1\x96\x8b\x1d\xed"
		"\x55\x61\x1d\x09\x87\xdb\x72\xd2\x1e\x31\xe2\xab\xf7\x9f\xf9\x65"
		"\xa9\x62\x5d\x8c\xde\xc1\xb5\x4a\x62\x5e\xe7\x7e\xbd\xe7\x9f\x75"
		"\x7e\xb6\x11\xf2\x7e\x8e\x10\xff\x93\xfa\xc9\xc6\xf8\x00\xd9\xa1"
		"\xf5\x2d\xc5\x61\x45\x49\x39\xf8\xb1\xd6\xda\x08\x5a\x49\xab\x4f"
		"\xd8\xd7\x59\x16\x0e\x5d\x54\xc8\x11\x97\xbe\xde\x99\x21\x06\x8d"
		"\x30\x84\x1c\x4c\x2d\xc7\x05\x4b\x3a\x4b\x58\x1e\x4c\x7b\x65\x9f"
		"\xd5\x6a\xe5\x02\x23\x83\xd7\xdf\x66\xff\xae\x26\x3f\x10\x37\xd4"
		"\x54\x45\xb1\x9a\xf4\x40\xf6\x51\xb9\xac\x68\xb9\xb0\x84\x64\x86"
		"\x57\x33\x00\xb4\xa0\x4d\x4c\x32\x23\x05\xdf\x76\x6c\xb5\x10\xd0"
		"\x3a\x6f\x25\xe2\x17\x6b\xc9\xd8\xf2\xbe\x10\xb1\x66\xc3\xf7\x71";
const unsigned char data_hash_padding[256] =
		"\x22\x32\xf6\x23\x69\x80\xf6\x25\xd2\x59\x37\xb2\xe4\x31\x25\xbd"
		"\x1c\x58\x94\xa1\xae\x1e\x29\x19\x86\xfb\x10\xe2\xfe\x71\x28\xa1"
		"\x20\x04\x00\x05\x01\x02\x04\x03\x65\x01\x48\x86\x60\x09\x06\x0d"
		"\x30\x31\x30\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
		"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01\x00";

void acu_int_handler_h20x8(void)
{
	unsigned int data;
	unsigned int ill_addr_hight;
	unsigned int ill_addr_low;
	unsigned int i;

	data = R32(ACU_ERR_ST_REG_h20x8);
	if(data&ERR_ST_WR_VIOL_MASK)
	{
		acu_int_flag |= 0x01;
	}
	if(data&ERR_ST_RD_VIOL_MASK)
	{
		acu_int_flag |= 0x02;
	}

	data = (data >> ERR_ST_MST_ERR_OFFSET);
	for(i=0; i<ACU_MST_MAX; i++)
	{
		if(data&0x01)
		{
//			uart_printf("master error num:%d\n", i);
			break;
		}
		data = (data >> 1);
	}

	ill_addr_hight = R32(ACU_ILL_ADDR_HIGH_REG_h20x8);
	ill_addr_low = R32(ACU_ILL_ADDR_LOW_REG_h20x8);

	acu_clr_rpt_err_stat_h20x8();
//	W32(ACU_RPT_CTRL_REG, 0x3);

	data = R32(ACU_ERR_ST_REG_h20x8);
	if(data != 0)
	{
//		uart_printf("clear ACU_ERR_ST_REG fail\n");
		acu_int_flag |= 0x10;
	}
	ill_addr_hight = R32(ACU_ILL_ADDR_HIGH_REG_h20x8);
	if(ill_addr_hight)
	{
//		uart_printf("clear ill_addr_hight fail\n");
		acu_int_flag |= 0x20;
	}
	ill_addr_low = R32(ACU_ILL_ADDR_LOW_REG_h20x8);
	if(ill_addr_low)
	{
//		uart_printf("clear ill_addr_low fail\n");
		acu_int_flag |= 0x40;
	}
	return;
}

void acu_int_handler_h20x8_JR(void)
{
	unsigned int data;
	unsigned int ill_addr_hight;
	unsigned int ill_addr_low;
	unsigned int i;

	data = R32(ACU_ERR_ST_REG_h20x8_JR);
	if(data&ERR_ST_WR_VIOL_MASK)
	{
		acu_int_flag |= 0x01;
	}
	if(data&ERR_ST_RD_VIOL_MASK)
	{
		acu_int_flag |= 0x02;
	}

	data = (data >> ERR_ST_MST_ERR_OFFSET);
	for(i=0; i<ACU_MST_MAX; i++)
	{
		if(data&0x01)
		{
//			uart_printf("master error num:%d\n", i);
			break;
		}
		data = (data >> 1);
	}

	ill_addr_hight = R32(ACU_ILL_ADDR_HIGH_REG_h20x8_JR);
	ill_addr_low = R32(ACU_ILL_ADDR_LOW_REG_h20x8_JR);

	acu_clr_rpt_err_stat_h20x8_JR();
//	W32(ACU_RPT_CTRL_REG, 0x3);

	data = R32(ACU_ERR_ST_REG_h20x8_JR);
	if(data != 0)
	{
//		uart_printf("clear ACU_ERR_ST_REG fail\n");
		acu_int_flag |= 0x10;
	}
	ill_addr_hight = R32(ACU_ILL_ADDR_HIGH_REG_h20x8_JR);
	if(ill_addr_hight)
	{
//		uart_printf("clear ill_addr_hight fail\n");
		acu_int_flag |= 0x20;
	}
	ill_addr_low = R32(ACU_ILL_ADDR_LOW_REG_h20x8_JR);
	if(ill_addr_low)
	{
//		uart_printf("clear ill_addr_low fail\n");
		acu_int_flag |= 0x40;
	}

	return;
}

/*
 * need AXIDMA_CONTIGUOUS_TEST 0
 * DMA0 RD test, SRAM, 4K
 */
int acu_axdmi0_test1()
{
	int ret;
	static count=0;
	unsigned int src_msize, src_width, dst_msize, dst_width, i, j, k, l;
	unsigned int offset[4] = {
			1, 2, 4, 0
	};
	unsigned char ch;
	volatile unsigned int acu_err_stat = 0;
	unsigned int ill_addr_hight = 0;
	unsigned int ill_addr_low = 0;
	unsigned int value;
	unsigned int index;
	unsigned int dma_src_addr = 0xe0070000;
	unsigned int dma_dst_addr = 0xe0078000;
	
	for(ch=1; ch<=7; ch++)
	{
		i = 0;
		j = 0;
		k = 4; //total size 0x1004

		src_msize = 0;
		dst_msize = 0;

		/* test 1, disable acu, normal dma transmit */
		acu_set_master_ctrl(0);
		for(index=0; index<8; index++){
			acu_set_master_ck(ACU_MST_DMA0, index, 0, MST_CK_DIS, 0, 0);
//			acu_set_master_ck(ACU_MST_DMA1, index, 0, MST_CK_DIS, 0, 0);
		}
		axidma_reset();

		buf_init(dma_src_addr+i, 0x1000+k, ch+src_msize+dst_msize+i+j+k);
		axidma_transfer_one_block(ch, dma_src_addr+i, dma_dst_addr+j, 0x1000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));	
		for(l=0; l<0x1000+k; l++)
		{
			if(R8(dma_src_addr+i+l) != R8(dma_dst_addr+j+l))
			{
				while(1);
			}
		}

		/*test 2, enable acu, dma transmit trigger illegal WR */
		secmon_svi_enable(HPSICR_SVI_EN_ACU);		//enable acu interrupt
		acu_int_flag = 0;

		acu_set_rpt_fail_enable_h20x8();
		InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);
		acu_set_master_ctrl(1<<ACU_MST_DMA0);

		/* acu source address fail, src dma0 master */
		ret = acu_set_master_ck(ACU_MST_DMA0, ch-1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, dma_src_addr);			//sram
//		ret = acu_set_master_ck(ACU_MST_DMA0, ch-1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000000, dma_src_addr);			//ddr
//		ret = acu_set_master_ck(ACU_MST_DMA0, ch-1, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000004, dma_src_addr);
		if(ret)
		{
			while(1);
		}
		
		/* dst dma0 master */
		ret = acu_set_master_ck(ACU_MST_DMA0, ch, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000004, dma_dst_addr);				//sram
//		ret = acu_set_master_ck(ACU_MST_DMA1, ch-1, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000004, dma_dst_addr);			//sram
//		ret = acu_set_master_ck(ACU_MST_DMA1, ch-1, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000000, dma_dst_addr);			//ddr
//		ret = acu_set_master_ck(ACU_MST_DMA1, ch-1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, dma_dst_addr);
		if(ret)
		{
			while(1);
		}

		delay(1000);

		axidma_reset();
		buf_init(dma_src_addr+i, 0x1000+k, ch+src_msize+dst_msize+i+j+k+0x10);								//add 0x10 for test
		axidma_transfer_one_block(ch, dma_src_addr+i, dma_dst_addr+j, 0x1000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));	
		axidma_transfer_one_block_h20x8(ch, 0x00000004, dma_src_addr+i, 0x00000004, dma_dst_addr+j, 0x1000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));

		while(acu_int_flag == 0);
		if(acu_int_flag != 0x02)
		{
			while(1); 		//illegal addr occur when read data at 0xe0071000+i 
		}

		value = R32(CH_INT_STATUS_REG(ch));
		if(value & DMA_TFR_DONE_INTSTAT)
		{
	//		uart_printf("set acu read test fail\n");
			W32(CH_INT_CLEAR_REG(ch), value);
			while(1);
		}
		if(!(value&0x20))//SRC_DEC_ERR_IntStat
		{
			while(1);
		}
		W32(CH_INT_CLEAR_REG(ch), value);

		/* 0xF20 is when acu catch illegal dma addr, shut dma burst transmit */
		for(l=0; l<0xF20; l++)															//h20x8T need this
		{
			if(R8(dma_src_addr+i+l) != R8(dma_dst_addr+j+l))
			{
				//uart_printf("dma data err!!  \n\n\t");
				while(1);
			}
		}
		for(l=0xF20; l<0x1000+k; l++)
		{
			if(R8(dma_src_addr+i+l) == R8(dma_dst_addr+j+l))
			{
				//uart_printf("dma data err!!  \n\n\t");
				while(1);
			}
		}
		
		/* test 3, set the correct src window */
		k = 0;//transfer size 0x1000
		axidma_reset();
		buf_init(dma_src_addr+i, 0x1000+k, ch+src_msize+dst_msize+i+j+k);
//		axidma_transfer_one_block(ch, dma_src_addr+i, dma_dst_addr+j, 0x1000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));
		axidma_transfer_one_block_h20x8(ch, 0x00000004, dma_src_addr+i, 0x00000004, dma_dst_addr+j, 0x1000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));

		do {
			value = R32(CH_INT_STATUS_REG(ch));
		}while((value & DMA_TFR_DONE_INTSTAT) == 0);
		W32(CH_INT_CLEAR_REG(ch), value);
		for(l=0; l<0x1000+k; l++)
		{
			if(R8(dma_src_addr+i+l) != R8(dma_dst_addr+j+l))
			{
				while(1);
			}
		}

		/* SEC_MON state check */
		value = R32(SECMON_HPSVSR_REG);
		if(((value>>28)&0x01) != 1 )
		{
			while(1);
		}

		W32(SECMON_HPSVSR_REG, (1<<28));

		value = R32(SECMON_HPSVSR_REG);
		if(((value>>28)&0x01) != 0 )
		{
			while(1);
		}
		PSPRINTF("\n count is: %d\n", count++);
	}

	return 0;
}

/*
 * need AXIDMA_CONTIGUOUS_TEST 0
 * DMA1 WR test, DDR, 8K
 */
int acu_axdmi0_test2()
{
	int ret;
	unsigned int src_msize, src_width, dst_msize, dst_width, i, j, k, l;
	unsigned int offset[4] = {
			1, 2, 4, 0
	};
	unsigned char ch;
	volatile unsigned int acu_err_stat = 0;
	unsigned int ill_addr_hight = 0;
	unsigned int ill_addr_low = 0;
	unsigned int value;
	unsigned int index;
	unsigned int dma_src_addr = 0x10000000;				//ddr
	unsigned int dma_dst_addr = 0x20000000;
	
	for(ch=1; ch<=8; ch++)
	{
		i = 0;
		j = 0;
		k = 0x10;//total size 0x2010
		src_msize = 0;
		dst_msize = 0;
	
		secmon_svi_enable(HPSICR_SVI_EN_ACU);
		acu_int_flag = 0;
		acu_set_rpt_fail_enable_h20x8();
		InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);

		acu_set_master_ctrl(1<<ACU_MST_DMA1);
		ret = acu_set_master_ck(ACU_MST_DMA1, ch-1, ACU_CH_SIZE_16K, MST_CK_EN, 0x00000000, dma_src_addr);	//ddr
//		ret = acu_set_master_ck(ACU_MST_DMA0, ch-1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, dma_src_addr);
		if(ret)
		{
			while(1);
		}
		ret = acu_set_master_ck(ACU_MST_DMA1, ch, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000000, dma_dst_addr);		//WR illegal addr according to this window set the size only 8K
//		ret = acu_set_master_ck(ACU_MST_DMA0, ch, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000004, dma_dst_addr);
		if(ret)
		{
			while(1);
		}
		delay(1000);

		axidma_reset();
		buf_init(dma_src_addr+i, 0x2000+k, ch+src_msize+dst_msize+i+j+k);
		axidma_transfer_one_block_h20x8(ch, 0, dma_src_addr+i, 0, dma_dst_addr+j, 0x2000+k-1, DMS_1 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));

		while(acu_int_flag == 0);
		if(acu_int_flag != 0x01)
		{
			while(1); //flag eroor
		}

		value = R32(CH_INT_STATUS_REG(ch));
		if(value & DMA_TFR_DONE_INTSTAT)
		{
	//		uart_printf("set acu write test fail\n");
			W32(CH_INT_CLEAR_REG(ch), value);
			while(1);
		}
		if(!(value&0x40))//DST_DEC_ERR_IntStat
		{
			while(1);
		}
		W32(CH_INT_CLEAR_REG(ch), value);

		for(l=0; l<0x2000; l++)
		{
			if(R8(dma_src_addr+i+l) != R8(dma_dst_addr+j+l))
			{
				while(1);
			}
		}

		for(l=0; l<k; l++)
		{
			if(R8(dma_src_addr+0x2000+i+l) == R8(dma_dst_addr+0x2000+j+l))
			{
					while(1);
			}
		}
	}

	return 0;
}

/*
 * need AXIDMA_CONTIGUOUS_TEST 0
 * DMA0 Multi_Window test, SRAM, 8K
 */
int acu_axdmi0_test3()
{
	int ret;
	unsigned int src_msize, src_width, dst_msize, dst_width, i, j, k, l;
	unsigned int offset[4] = {
			1, 2, 4, 0
	};
	unsigned char ch;
	volatile unsigned int acu_err_stat = 0;
	unsigned int ill_addr_hight = 0;
	unsigned int ill_addr_low = 0;
	unsigned int value;
	unsigned int index;
	unsigned int dma_src_addr = 0xe0070000;
	unsigned int dma_dst_addr = 0xe0078000;

	for(ch=1; ch<=8; ch++)
	{
		i = 0;
		j = 0;
		k = 4;//total size 0x1004
		src_msize = 0;
		dst_msize = 0;

		for(index=0; index<8; index++){
			acu_set_master_ck(ACU_MST_DMA0, index, 0, MST_CK_DIS, 0, 0);
		}
		secmon_svi_enable(HPSICR_SVI_EN_ACU);
		acu_int_flag = 0;

		acu_set_rpt_fail_enable_h20x8();
		InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);

		acu_set_master_ctrl(1<<ACU_MST_DMA0);
		ret = acu_set_master_ck(ACU_MST_DMA0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, dma_src_addr);
		if(ret)
		{
			while(1);
		}
		ret = acu_set_master_ck(ACU_MST_DMA0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, dma_src_addr+0x1000);
		if(ret)
		{
			while(1);
		}
		ret = acu_set_master_ck(ACU_MST_DMA0, 2, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000004, dma_dst_addr);
		if(ret)
		{
			while(1);
		}

		ret = acu_set_master_ck(ACU_MST_DMA0, 3, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000004, dma_dst_addr+0x2000);
		if(ret)
		{
			while(1);
		}

		axidma_reset();
		buf_init(dma_src_addr+i, 0x2000+k, ch+src_msize+dst_msize+i+j+k);
		/* size is 8K */
		axidma_transfer_one_block_h20x8(ch, 0x00000004, dma_src_addr+i, 0x00000004, dma_dst_addr+j, 0x2000-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));

		do {
			value = R32(CH_INT_STATUS_REG(ch));
		}while((value & DMA_TFR_DONE_INTSTAT) == 0);
		W32(CH_INT_CLEAR_REG(ch), value);

		for(l=0; l<0x1000+k; l++)
		{
			if(R8(dma_src_addr+i+l) != R8(dma_dst_addr+j+l))
			{
				//uart_printf("dma data err!!  \n\n\t");
				while(1);
			}
		}
		
		/* size is 8K+4*/
		axidma_transfer_one_block_h20x8(ch, 0x00000004, dma_src_addr+i, 0x00000004, dma_dst_addr+j, 0x2000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));
		while(acu_int_flag == 0);
		if(acu_int_flag != 0x02)
		{
			while(1); 		//illegal addr occur when read data at 0xe0071000+i 
		}

		value = R32(CH_INT_STATUS_REG(ch));
		if(value & DMA_TFR_DONE_INTSTAT)
		{
	//		uart_printf("set acu read test fail\n");
			W32(CH_INT_CLEAR_REG(ch), value);
			while(1);
		}
		if(!(value&0x20))//SRC_DEC_ERR_IntStat
		{
			while(1);
		}
		W32(CH_INT_CLEAR_REG(ch), value);	
	}

	return 0;
}

/*
 * DMA0 multi channel test
 * SRC:DDR	DST:DDR
 */
int acu_axdmi0_test4()
{
	int ret;
	static count=0;
	unsigned int src_msize, src_width, dst_msize, dst_width, i, j, k, l;
	unsigned int offset[4] = {
			1, 2, 4, 0
	};
	unsigned char ch;
	volatile unsigned int acu_err_stat = 0;
	unsigned int ill_addr_hight = 0;
	unsigned int ill_addr_low = 0;
	unsigned int value;
	unsigned int index;
	unsigned int dma_src_addr = 0x10000000;
	unsigned int dma_dst_addr = 0x20000000;

		ch=1;
		i = 0;
		j = 0;
		k = 0; //total size 0x1000
	//	k = 4; //total size 0x1004

		src_msize = 0;
		dst_msize = 0;

		/* test1 DMA0 and DMA1 run the same time, check data */
		secmon_svi_enable(HPSICR_SVI_EN_ACU);//enable acu interrupt
		acu_int_flag = 0;

		acu_set_rpt_fail_enable_h20x8();
		InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);
		acu_set_master_ctrl(1<<ACU_MST_DMA0 | 1<<ACU_MST_DMA1);
		
		/* source dma0 master */
		ret = acu_set_master_ck(ACU_MST_DMA0, ch-1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000000, dma_src_addr);		//ACU-DMA0 master ch0 as src
		if(ret)
		{
			while(1);
		}
		
		/* destination dma0 master */
		ret = acu_set_master_ck(ACU_MST_DMA0, ch, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000000, dma_dst_addr);			//ACU-DMA0 master ch1 as dst
		if(ret)
		{
			while(1);
		}

		delay(1000);
		/* DMA0 use ch 1 */
		axidma_reset();
		buf_init(dma_src_addr+i, 0x1000+k, ch+src_msize+dst_msize+i+j+k);										//init src data for DMA0
		axidma0_transfer_no_transmit_en(ch, dma_src_addr+i, dma_dst_addr+j, 0x1000+k-1, DMS_1 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));

		/* source dma1 master */
		ret = acu_set_master_ck(ACU_MST_DMA1, ch+1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000000, dma_src_addr+0x20000);	//ACU-DMA1 master ch2 as src
		if(ret)
		{
			while(1);
		}

		/* destination dma1 master */
		ret = acu_set_master_ck(ACU_MST_DMA1, ch+2, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000000, dma_dst_addr+0x20000);	//ACU-DMA1 master ch3 as dst
		if(ret)
		{
			while(1);
		}

		delay(1000);
		/* DMA1 use ch 1 */
		buf_init(dma_src_addr+0x20000+i, 0x1000+k, ch+src_msize+0x20000+dst_msize+i+j+k);							//init src data for DMA1
		axidma1_transfer_no_transmit_en(ch, dma_src_addr+0x20000+i, dma_dst_addr+0x20000+j, 0x1000+k-1, SMS_2 | DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));

		/* enable DMA0 and DMA1*/
		W32(DMAC_CH_EN_REG, CH_EN(ch) | CH_EN_WE(ch));
		W32(DMAC_CH_EN_REG+0x1000, CH_EN(ch) | CH_EN_WE(ch));
		
		/* data check */
		for(l=0; l<0x1000+k; l++)
		{
			if(R8(dma_src_addr+i+l) != R8(dma_dst_addr+j+l))
			{
				while(1);
			}
		}

		for(l=0; l<0x1000+k; l++)
		{
			if(R8(dma_src_addr+0x20000+i+l) != R8(dma_dst_addr+0x20000+j+l))
			{
				while(1);
			}
		}

		/* test2 acu source address fail, DMA0 and DMA1 run the same time */
		k=4;
		ch++;
		secmon_svi_enable(HPSICR_SVI_EN_ACU);//enable acu interrupt
		acu_int_flag = 0;

		acu_set_rpt_fail_enable_h20x8();
		InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);

		acu_set_master_ctrl(1<<ACU_MST_DMA0 | 1<<ACU_MST_DMA1);
		/* source dma0 master */
		ret = acu_set_master_ck(ACU_MST_DMA0, ch-1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000000, dma_src_addr);		//ACU-DMA0 master ch0 as src
		if(ret)
		{
			while(1);
		}
		
		/* destination dma0 master */
		ret = acu_set_master_ck(ACU_MST_DMA0, ch, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000000, dma_dst_addr);			//ACU-DMA0 master ch1 as dst
		if(ret)
		{
			while(1);
		}

		delay(1000);
		/* DMA0 use ch 1 */
		axidma_reset();
		buf_init(dma_src_addr+i, 0x1000+k, ch+src_msize+dst_msize+i+j+k+0x10);										//init src data
		axidma0_transfer_no_transmit_en(ch, dma_src_addr+i, dma_dst_addr+j, 0x1000+k-1, DMS_1 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));
//		*(volatile unsigned int *)0xe021d018 = ((1<<0) | (1<<8));				//W32(DMAC_CH_EN_REG, CH_EN(n) | CH_EN_WE(n));

		/* source dma1 master */
		ret = acu_set_master_ck(ACU_MST_DMA1, ch+1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000000, dma_src_addr+0x20000);	//ACU-DMA1 master ch2 as src
		if(ret)
		{
			while(1);
		}

		/* destination dma1 master */
		ret = acu_set_master_ck(ACU_MST_DMA1, ch+2, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000000, dma_dst_addr+0x20000);	//ACU-DMA1 master ch3 as dst
		if(ret)
		{
			while(1);
		}

		delay(1000);
		/* DMA1 use ch 2 */
		buf_init(dma_src_addr+0x20000+i, 0x1000+k, ch+src_msize+0x20000+dst_msize+i+j+k);		//DMA1
		axidma1_transfer_no_transmit_en(ch+1, dma_src_addr+0x20000+i, dma_dst_addr+0x20000+j, 0x1000+k-1, SMS_2 | DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));

		/* enable DMA0 and DMA1*/
		W32(DMAC_CH_EN_REG, CH_EN(ch) | CH_EN_WE(ch));
		W32(DMAC_CH_EN_REG+0x1000, CH_EN(ch) | CH_EN_WE(ch));
		
		while(acu_int_flag == 0);
		if(acu_int_flag != 0x02)
		{
			while(1); //flag eroor
		}

		value = R32(CH_INT_STATUS_REG(ch));
		if(value & DMA_TFR_DONE_INTSTAT)
		{
	//		uart_printf("set acu read test fail\n");
			W32(CH_INT_CLEAR_REG(ch), value);
			while(1);
		}
		if(!(value&0x20))//SRC_DEC_ERR_IntStat
		{
			while(1);
		}
		W32(CH_INT_CLEAR_REG(ch), value);

//		for(l=0; l<0x1000+k; l++)
		for(l=0; l<0xF20; l++)
		{
			if(R8(dma_src_addr+i+l) != R8(dma_dst_addr+j+l))
			{
				//uart_printf("dma data err!!  \n\n\t");
				while(1);
			}
		}
		for(l=0x1000; l<0x1000+k; l++)
		{
			if(R8(dma_src_addr+i+l) == R8(dma_dst_addr+j+l))
			{
				//uart_printf("dma data err!!  \n\n\t");
				while(1);
			}
		}
}

unsigned int rsa_verify_for_ACU( unsigned char *r, unsigned char *n, unsigned char *e, unsigned char *sign)
{
	unsigned int len = 256, ret = -1;
	unsigned int desc[128];
	memset_c(desc, 0, 128);
#if 0
	unsigned char *r, *r_raw;
	r_raw = (unsigned char *)OPENSSL_malloc(len + DATA_ALIGN);
	r = (unsigned char *)(((unsigned long)r_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
#endif

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);

	append_fifo_load(desc, virt_to_phys(n), len, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys(sign), len, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	append_key(desc, E_dma, E_size, CLASS_1 | KEY_DEST_PKHA_E);
	append_fifo_load(desc, virt_to_phys(e), len, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);

	append_fifo_store(desc, virt_to_phys(r), len, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
//	ret = run_descriptor_jr(desc, &jr_t);
//	jump_cmd = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
//	set_jump_tgt_here(desc,jump_cmd);
/*
	append_fifo_load(desc, virt_to_phys(n), len, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys(hash), len, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	append_fifo_load(desc, virt_to_phys(R), len, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
   	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_0 | 0x1);
*/

	jr_index = jr_index%9;
		ret = run_descriptor_jr_index(jr_index, desc);

	jr_index++;

	if (ret)
		debug("Error %x\n", ret);

	return ret;
}

/* acu_jobring_test_normal is 903T, tested ACU by sm3 sha0 sha1 */
int acu_jobring_test_normal()
{
#if 0
	void *ctx;
#else
	struct hash_ctx *ctx = 0xe033f000;
#endif
	unsigned char dst_sm3_buf[64];
	unsigned char dst_sha0_buf[64];
	unsigned char dst_sha1_buf[64];

	memset_c(dst_sm3_buf, 0, 64);
	memset_c(dst_sha0_buf, 0, 64);
	memset_c(dst_sha1_buf, 0, 64);
#if 0
	struct std_data *sm3_raw;
	struct std_data *sha0_raw;
	struct std_data *sha1_raw;
#else
	unsigned char *sm3_data = 0xe0338000;
	unsigned char *sha0_data = 0xe0338000;
	unsigned char *sha1_data = 0xe0338000;
#endif
	unsigned int index;
	int ret;

#if 0
	sm3_raw = malloc(sizeof(struct std_data)+DATA_ALIGN);
	sm3_raw = (struct std_data *)(((unsigned long)sm3_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	memcpy_c(sm3_raw->data,data,128);
	memcpy_c(sm3_raw->stdout,sm3_stddata,64);


	sha0_raw = malloc(sizeof(struct std_data)+DATA_ALIGN);
	sha0_raw = (struct std_data *)(((unsigned long)sha0_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	memcpy_c(sha0_raw->data,data,128);
	memcpy_c(sha0_raw->stdout,sha0_stddata,40);

	sha1_raw= malloc(sizeof(struct std_data)+DATA_ALIGN);
	sha1_raw = (struct std_data *)(((unsigned long)sha1_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	memcpy_c(sha1_raw->data,data,128);
	memcpy_c(sha1_raw->stdout,sha1_stddata,40);
#else
	memcpy_c(sm3_data,data,128);
	memcpy_c(dst_sm3_buf,sm3_stddata_ex,64);

	memcpy_c(sha0_data,data,128);
	memcpy_c(dst_sha0_buf,sha0_stddata_ex,40);

	memcpy_c(sha1_data,data,128);
	memcpy_c(dst_sha1_buf,sha1_stddata_ex,40);
#endif
	//not set acu
	acu_set_master_ctrl(0);

	sec_init();

	//test acu source address fail
	secmon_svi_enable(HPSICR_SVI_EN_ACU);			//enable acu interrupt
	acu_set_rpt_fail_enable();
	acu_int_flag = 0;
//	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler);
	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);

	acu_set_master_ctrl_h20x8_JR(1<<ACU_MST_JOBRING0 | 1<<ACU_MST_JOBRING1 | 1<<ACU_MST_JOBRING2);

	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING1, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING1, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING1, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING2, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING2, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING2, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

#if 0
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(sm3_raw->data),32,1);
	caam_hash_finish(ctx,dst_buf,32,SM3);

	if(!memcmp_c(dst_buf,sm3_raw->stdout,32))
	{
		debug("std sm3 test ok!\n");
	}
	else
	{
		//debug("std error sm3 test!\n");
		while(1);
	}


	caam_hash_init(&ctx);
	caam_hash_update(ctx,(sha0_raw->data),32,1);
	caam_hash_finish(ctx,dst_buf,20,SHA0);

	if(!memcmp_c(dst_buf,sha0_raw->stdout,20))
	{
		debug("std sha0 test ok!\n");
	}
	else
	{
		debug("std error sha0 test!\n");
		while(1);
	}


	caam_hash_init(&ctx);
	caam_hash_update(ctx,(sha1_raw->data),32,1);
	caam_hash_finish(ctx,dst_buf,32,SHA1);

	if(!memcmp_c(dst_buf,sha1_raw->stdout,20))
	{
		debug("std sha1 test ok!\n");
	}
	else
	{
		debug("std error sha1 test!\n");
		while(1);
	}

	free(sm3_raw);
	free(sha0_raw);
	free(sha1_raw);
#else

	memset_c(ctx, 0x00, sizeof(struct hash_ctx));
	caam_hash_init(ctx);
	caam_hash_finish(ctx,sm3_data,32,SM3);

	if(!memcmp_c(ctx->hash,dst_sm3_buf,32))
	{
		debug("std sm3 test ok!\n");
	}
	else
	{
		debug("std error sm3 test!\n");
		soft_fail();
	}


	memset_c(ctx, 0x00, sizeof(struct hash_ctx));
	caam_hash_init(ctx);
	caam_hash_finish(ctx,sha0_data,32,SHA0);

	if(!memcmp_c(ctx->hash,dst_sha0_buf,20))
	{
		debug("std sha0 test ok!\n");
	}
	else
	{
		debug("std error sha0 test!\n");
		soft_fail();
	}


	memset_c(ctx, 0x00, sizeof(struct hash_ctx));
	caam_hash_init(ctx);
	caam_hash_finish(ctx,sha1_data,32,SHA1);

	if(!memcmp_c(ctx->hash,dst_sha1_buf,20))
	{
		debug("std sha1 test ok!\n");
	}
	else
	{
		debug("std error sha1 test!\n");
		soft_fail();
	}
#endif

	return 0;
}

/* acu_jobring_test_normal_exp is 903T, tested ACU by PKHA_exp */
int acu_jobring_test_normal_exp()
{
	int i,ret,index,count=0;
	unsigned char in_e[256];
	unsigned char in_n[256];
	unsigned char *in_sign = 0xe033f000;
	unsigned char *out_hash = 0xe0338000;
//	unsigned char *in_sign = 0xe0348000;
//	unsigned char *out_hash = 0xe0340000;

	memset_c(in_e, 0, 256);
	memset_c(in_n, 0, 256);
	memset_c(in_sign, 0, 256);

	memcpy_c(in_e, data_e, 256);
	memcpy_c(in_n, data_n, 256);
	memcpy_c(in_sign, data_sign, 256);

	//not set acu
	acu_set_master_ctrl(0);

	sec_init();

	//test acu source address fail
	secmon_svi_enable(HPSICR_SVI_EN_ACU);//enable acu interrupt
	acu_set_rpt_fail_enable();
	acu_int_flag = 0;
//	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler);
	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);
	acu_set_master_ctrl_h20x8_JR(1<<ACU_MST_JOBRING0 | 1<<ACU_MST_JOBRING1 | 1<<ACU_MST_JOBRING2);

	ret = acu_set_master_ck(ACU_MST_JOBRING0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_JOBRING0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_JOBRING0, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

	ret = acu_set_master_ck(ACU_MST_JOBRING1, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_JOBRING1, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_JOBRING1, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

	ret = acu_set_master_ck(ACU_MST_JOBRING2, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_JOBRING2, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_JOBRING2, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

	for(index=0; index<20; index++)
	{
		for(i=0; i<3; i++)
		{
			memset_c(out_hash, 0, 256);
			rsa_verify_for_ACU(out_hash, in_n, in_e, in_sign);

			if(!memcmp_c(out_hash, data_hash_padding, 256))
			{
				debug("std rsa test ok!\n");
				count++;
			}
			else
			{
				debug("std error rsa test!\n");
				while(1);
			}
		}
	}
	return 0;
}

int acu_jobring_test_viol_exp()
{
	int i,ret,index,count=0;
	unsigned char in_e[256];
	unsigned char in_n[256];
	unsigned char *in_sign = 0xe033f000;
	unsigned char *out_hash = 0xe0338000;

	memset_c(in_e, 0, 256);
	memset_c(in_n, 0, 256);
	memset_c(in_sign, 0, 256);

	memcpy_c(in_e, data_e, 256);
	memcpy_c(in_n, data_n, 256);
	memcpy_c(in_sign, data_sign, 256);

	//not set acu
	acu_set_master_ctrl(0);

	sec_init();

	//test acu source address fail
	secmon_svi_enable(HPSICR_SVI_EN_ACU);//enable acu interrupt
	acu_set_rpt_fail_enable();
	acu_int_flag = 0;
//	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler);
	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);
	acu_set_master_ctrl_h20x8_JR(1<<ACU_MST_JOBRING0 | 1<<ACU_MST_JOBRING1 | 1<<ACU_MST_JOBRING2);

/*	ret = acu_set_master_ck(ACU_MST_JOBRING0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0301e00);		//0xe0301e00 is jobin_array0 at jr.h
	if(ret)
	{
		while(1);
	}
*/
	acu_int_flag = 0;

	for(index=0; index<3; index++)		//the deep is 8,so index must less than 7
	{
		for(i=0; i<3; i++)
		{
			memset_c(out_hash, 0, 256);
			rsa_verify_for_ACU(out_hash, in_n, in_e, in_sign);

			while(acu_int_flag == 0);
			if(acu_int_flag != 0x03)
			{
				while(1); //flag eroor
			}

			secmon_svi_state_clear(HPSICR_SVI_EN_ACU);
			acu_set_rpt_fail_enable();
			acu_int_flag = 0;
		}
	}
	return 0;
}

int acu_jobring_test_viol_h20x8()
{
	int i,ret,index,count=0;
	unsigned char in_e[256];
	unsigned char in_n[256];
	unsigned char *in_sign = 0xe007f000;
	unsigned char *out_hash = 0xe0078000;
	
	memset_c(in_e, 0, 256);
	memset_c(in_n, 0, 256);
	memset_c(in_sign, 0, 256);

	memcpy_c(in_e, data_e, 256);
	memcpy_c(in_n, data_n, 256);
	memcpy_c(in_sign, data_sign, 256);

	//not set acu
	acu_set_master_ctrl(0);
	for(index=0; index<8; index++){
		acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, index, 0, MST_CK_DIS, 0, 0);
	}
	sec_init();

	//test acu source address fail
//	secmon_svi_enable(HPSICR_SVI_EN_ACU);//enable acu interrupt
//	acu_set_rpt_fail_enable_h20x8();
	secmon_svi_enable(HPSICR_SVI_EN_ACU_JR);		//enable acu-JR interrupt
	acu_set_rpt_fail_enable_h20x8_JR();
	acu_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8_JR);
//	acu_set_master_ctrl(1<<ACU_MST_JOBRING0);		//here ACU_MST_JOBRING0 is the SEC-master in fact
	acu_set_master_ctrl_h20x8_JR(1<<ACU_MST_JOBRING0 | 1<<ACU_MST_JOBRING1 | 1<<ACU_MST_JOBRING2 | 1<<ACU_MST_JOBRING3 |
			1<<ACU_MST_JOBRING4 | 1<<ACU_MST_JOBRING5 | 1<<ACU_MST_JOBRING6 | 1<<ACU_MST_JOBRING7 | 1<<ACU_MST_JOBRING8);	//enable JR_acu

	acu_int_flag = 0;

	for(index=0; index<20; index++)
	{
		for(i=0; i<9; i++)					//h20x8-----9jobs
		{
			memset_c(out_hash, 0, 256);
			rsa_verify_for_ACU(out_hash, in_n, in_e, in_sign);

			while(acu_int_flag == 0);
			if(acu_int_flag != 0x03)
			{
				while(1); //flag eroor
			}

			secmon_svi_state_clear(HPSICR_SVI_EN_ACU_JR);
			acu_set_rpt_fail_enable_h20x8_JR();
			acu_int_flag = 0;
		}
	}
	return 0;
}

/*
 * jobring need two addr:
 * 1. job ring array address at jr.h
 * 2. _RAM_START_ADDR
 */
int acu_jobring_test_normal_h20x8()
{

	int i,ret,index,count=0;
	unsigned char in_e[256];
	unsigned char in_n[256];
	unsigned char *in_sign = 0xe007f000;
	unsigned char *out_hash = 0xe0078000;

	memset_c(in_e, 0, 256);
	memset_c(in_n, 0, 256);
	memset_c(in_sign, 0, 256);

	memcpy_c(in_e, data_e, 256);
	memcpy_c(in_n, data_n, 256);
	memcpy_c(in_sign, data_sign, 256);

	new_setting();
	sec_init();
	
	//not set acu
	acu_set_master_ctrl(0);
	for(index=0; index<8; index++){
		acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, index, 0, MST_CK_DIS, 0, 0);
//		acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING1, index, 0, MST_CK_DIS, 0, 0);
//		acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING2, index, 0, MST_CK_DIS, 0, 0);
	}

	//test acu source address fail
	secmon_svi_enable(HPSICR_SVI_EN_ACU_JR);		//enable acu-JR interrupt
	acu_set_rpt_fail_enable_h20x8_JR();
	acu_int_flag = 0;

	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8_JR);

//	acu_set_master_ctrl(1<<ACU_MST_JOBRING0);		//here ACU_MST_JOBRING0 is the SEC-master in fact
//	acu_set_master_ctrl_h20x8_JR(1<<ACU_MST_JOBRING0 | 1<<ACU_MST_JOBRING1 | 1<<ACU_MST_JOBRING2);	//JR	 window

	//test1 -- JB0
/*	acu_set_master_ctrl_h20x8_JR(1<<ACU_MST_JOBRING0);														//only JR0	 window
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000030, 0xe0348000);		//here is zhe DECO operation, so jr no useful
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000030, 0xe034f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000030, 0xe0370000);	//virt addr for job ring array address---jr.h
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING0, 3, ACU_CH_SIZE_128K, MST_CK_EN, 0x80000030, 0xe0310000);	//jobring can read the whole _RAM_START_ADDR
	if(ret)
	{
		while(1);
	}
*/
	//test2 -- JB8
	jr_index = 8;		//test JR8
	acu_set_master_ctrl_h20x8_JR(1<<ACU_MST_JOBRING8);																//only JR8	 window
	
	/*0xe007f000 and 0xe0078000 is the DECO operation, so jr no useful */
/*	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING8, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, 0xe007f000);		
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING8, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, 0xe0078000);
	if(ret)
	{
		while(1);
	}
*/
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING8, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x00000004, 0xe007c000);		//virt addr for job ring array address---jr.h
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck_h20x8_JR(ACU_MST_JOBRING8, 3, ACU_CH_SIZE_128K, MST_CK_EN, 0x00000004, 0xe000c000);		//jobring can read the whole _RAM_START_ADDR
	if(ret)
	{
		while(1);
	}

	for(index=0; index<20; index++)
	{
		for(i=0; i<9; i++)
		{
			memset_c(out_hash, 0, 256);

			//add for test
//			*(volatile unsigned int *)0xe0240000 = 0;
//			*(volatile unsigned int *)0xe0100410 = 0;
			rsa_verify_for_ACU(out_hash, in_n, in_e, in_sign);

			if(!memcmp_c(out_hash, data_hash_padding, 256))
			{
				debug("std rsa test ok!\n");
				count++;
			}
			else
			{
				debug("std error rsa test!\n");
				while(acu_int_flag == 0);
				while(1);
			}
		}
	}
	return 0;
}

int acu_pcie_test_normal_h20x8_test1()
{

	int i,ret,index,count=0, val;

	//not set acu
	acu_set_master_ctrl(0);
	for(index=0; index<8; index++){
		acu_set_master_ck(ACU_MST_PCIE0, index, 0, MST_CK_DIS, 0, 0);
	}

	//test acu source address fail
	secmon_svi_enable(HPSICR_SVI_EN_ACU);								//enable acu interrupt
	acu_set_rpt_fail_enable_h20x8();
	acu_int_flag = 0;

	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);

	acu_set_master_ctrl(1<<ACU_MST_PCIE0);

	ret = acu_set_master_ck(ACU_MST_PCIE0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000004, 0xe0070000);
	if(ret)
	{
		while(1);
	}
/*
 	ret = acu_set_master_ck(ACU_MST_PCIE0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_PCIE0, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

	ret = acu_set_master_ck(ACU_MST_PCIE0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_PCIE0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_PCIE0, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}

	ret = acu_set_master_ck(ACU_MST_PCIE0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_PCIE0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	ret = acu_set_master_ck(ACU_MST_PCIE0, 2, ACU_CH_SIZE_16K, MST_CK_EN, 0x80000000, 0xe0300000);
	if(ret)
	{
		while(1);
	}
*/
	while(acu_int_flag == 0);
	if(acu_int_flag != 0x01)									//write error, the PC overwrite begin at 0xe0371000
	{
		while(1);
	}

	for(count=0; count<0x1000; count++)							//data check
	{
		val = R8(0xe0070000+count);

		if(val != ((0x43 + count)&0xff))
			COND_TRAP(31, 0, 0);
	}

	for(count=0; count<0x1000; count++)
	{
		val = R8(0xe0070000 + 0x10000 + count);

		if(val != 0)
			COND_TRAP(31, 0, 0);
	}

	return 0;
}

int acu_jobring_test_viol()
{
	struct hash_ctx *ctx = 0xe033f000;
	unsigned char dst_sm3_buf[64];
	unsigned char dst_sha0_buf[64];
	unsigned char dst_sha1_buf[64];

	memset_c(dst_sm3_buf, 0, 64);
	memset_c(dst_sha0_buf, 0, 64);
	memset_c(dst_sha1_buf, 0, 64);

	unsigned char *sm3_data = (unsigned char *)0xe0338000;
	unsigned char *sha0_data = (unsigned char *)0xe0338000;
	unsigned char *sha1_data = (unsigned char *)0xe0338000;
	unsigned int index;
	int ret = 0;

	memcpy_c(sm3_data,data,128);
	memcpy_c(dst_sm3_buf,sm3_stddata_ex,64);

	memcpy_c(sha0_data,data,128);
	memcpy_c(dst_sha0_buf,sha0_stddata_ex,40);

	memcpy_c(sha1_data,data,128);
	memcpy_c(dst_sha1_buf,sha1_stddata_ex,40);

	//not set acu
	acu_set_master_ctrl(0);

	sec_init();

	//test acu source address fail
	secmon_svi_enable(HPSICR_SVI_EN_ACU);//enable acu interrupt
//	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler);
	InstallInterruptHandler0(SECMON_INT_NUM, acu_int_handler_h20x8);

	acu_set_master_ctrl(1<<ACU_MST_JOBRING0 | 1<<ACU_MST_JOBRING1 | 1<<ACU_MST_JOBRING2);

	//acu not effect data addr
/*	ret = acu_set_master_ck(ACU_MST_JOBRING0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
*/
	//acu desc
//	ret = acu_set_master_ck(ACU_MST_JOBRING0, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	//acu ring addr
//	ret = acu_set_master_ck(ACU_MST_JOBRING0, 2, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0304000);
	if(ret)
	{
		while(1);
	}

/*
	ret = acu_set_master_ck(ACU_MST_JOBRING1, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
*/
//	ret = acu_set_master_ck(ACU_MST_JOBRING1, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	//acu ring addr
//	ret = acu_set_master_ck(ACU_MST_JOBRING1, 2, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0304000);
	if(ret)
	{
		while(1);
	}

/*
	ret = acu_set_master_ck(ACU_MST_JOBRING2, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0338000);
	if(ret)
	{
		while(1);
	}
*/
//	ret = acu_set_master_ck(ACU_MST_JOBRING2, 1, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe033f000);
	if(ret)
	{
		while(1);
	}
	//acu ring addr
//	ret = acu_set_master_ck(ACU_MST_JOBRING2, 2, ACU_CH_SIZE_4K, MST_CK_EN, 0x80000000, 0xe0304000);
	if(ret)
	{
		while(1);
	}

#if 1
	acu_set_rpt_fail_enable();
	acu_int_flag = 0;
	memset_c(ctx, 0x00, sizeof(struct hash_ctx));
	jr_index = 0;
	caam_hash_init(ctx);
	caam_hash_finish(ctx,sm3_data,32,SM3);

	while(acu_int_flag == 0);
	if(acu_int_flag != 0x03)
	{
		while(1); //flag eroor
	}
#endif
	acu_set_rpt_fail_enable();
	acu_int_flag = 0;
	jr_t.nr[0] = 1;
	memset_c(ctx, 0x00, sizeof(struct hash_ctx));
	jr_index = 1;
	caam_hash_init(ctx);
	caam_hash_finish(ctx,sha0_data,32,SHA0);

	while(acu_int_flag == 0);
	if(acu_int_flag != 0x03)
	{
		while(1); //flag eroor
	}

	acu_set_rpt_fail_enable();
	acu_int_flag = 0;
	jr_t.nr[0] = 2;
	memset_c(ctx, 0x00, sizeof(struct hash_ctx));
	jr_index = 2;
	caam_hash_init(ctx);
	caam_hash_finish(ctx,sha1_data,32,SHA1);

	while(acu_int_flag == 0);
	if(acu_int_flag != 0x03)
	{
		while(1); //flag eroor
	}

	return 0;
}

void acu_dma_test()
{
	/*DMA_AXI_BUS*/
	dma_axi_bus();
	
//	acu_axdmi0_test1();				//1 close ACU, DMA0 transmit, 2 set ACU, DMA0 transmit, read violation
//	acu_axdmi0_test2();				//1 close ACU, DMA1 transmit, 2 set ACU, DMA1 transmit, write violation
//	acu_axdmi0_test3();				//DMA0 Multi windows test
	acu_axdmi0_test4();				//DMA0 Multi channel test
}

