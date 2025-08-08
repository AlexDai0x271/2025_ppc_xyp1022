/*
 * acu.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: yyang
 */
#include "acu.h"
#include "ppclib.h"
#include "common.h"

void acu_set_master_ctrl(int value)
{
	W32(ACU_CTRL_REG, value);
//	ISYNC;
}

void acu_set_ring_master_ctrl(int value)
{
	W32(ACU_RING_CTRL_REG, value);
}

void acu_set_master_ctrl_h20x8_JR(int value)
{
	W32(ACU_JR_h20x8_ADDR, value);
	ISYNC;
}

void acu_deal(void)
{
#if 0
	acu_set_master_ctrl(ACU_ALL_ON);
	acu_set_ring_master_ctrl(ACU_RING_ALLON);
	//acu_set_master_ctrl(ACU_ALL_OFF);
#else
	acu_set_master_ctrl(0);
	acu_set_ring_master_ctrl(0x0); 
	//acu_set_ring_master_ctrl(ACU_RING2_OFF);
	//acu_set_ring_master_ck(ACU_MST_RING2,0,ACU_CH_SIZE_256K,MST_CK_EN,SLAVE_HADDR0,SRAM0_START);
#endif
}

int acu_set_master_ck(unsigned int master_num, unsigned int chanel_num, unsigned int size, int chanel_en, unsigned int high_addr, unsigned int low_addr)
{
	unsigned int base_addr_ms;
	unsigned int base_addr_ls;
	unsigned int mst_ck_ms_value;
	unsigned int count;

	if(master_num>=ACU_MST_MAX || chanel_num>=MST_CK_CHANEL_MAX)
		return ACU_FAIL;

	base_addr_ms = (high_addr>>12)&0x000fffff;
	base_addr_ls = (low_addr>>12)&0x000fffff;
	base_addr_ls |= ((high_addr&0x00000fff)<<20);
	mst_ck_ms_value = base_addr_ms;
	if(chanel_en)
#if	ACU_903_TEST
		mst_ck_ms_value |= (MST_CK_EN<<26);								//old 903T and new 903 use this
#else
		mst_ck_ms_value |= (3 << 25);	//from 4x80000 to 6x80000		//905T/907T/908/H20x8
#endif
	if(size<0 || size>0x3f)
		return	ACU_FAIL;
	else
		mst_ck_ms_value |= (size<<MST_CK_SIZE_OFFSET);
#if 0
	W32(ACU_MST_CK_MS(master_num, chanel_num), (mst_ck_ms_value));
	W32(ACU_MST_CK_LS(master_num, chanel_num), (base_addr_ls));
#else
	if(master_num<6){
		W32(ACU_MST_CK_MS(master_num, chanel_num), (mst_ck_ms_value));			//only h20x8T add
		W32(ACU_MST_CK_LS(master_num, chanel_num), (base_addr_ls));
	}else{
		W32(ACU_MST_CK_MS(master_num, chanel_num)+0x40, (mst_ck_ms_value));		//only h20x8T add
		W32(ACU_MST_CK_LS(master_num, chanel_num)+0x40, (base_addr_ls));
	}
#endif
	count = 0;
	do{
#if 0
		if(mst_ck_ms_value != R32(ACU_MST_CK_MS(master_num, chanel_num)))
		{
			count++;
			continue;
		}
		if(base_addr_ls != R32(ACU_MST_CK_LS(master_num, chanel_num))){
			count ++;
			continue;
		}
#else																			//only h20x8T add
		if(master_num<6){	
			if(mst_ck_ms_value != R32(ACU_MST_CK_MS(master_num, chanel_num)))
			{
				count++;
				continue;
			}
			if(base_addr_ls != R32(ACU_MST_CK_LS(master_num, chanel_num))){
				count ++;
				continue;
			}
		}else{
			if(mst_ck_ms_value != R32(ACU_MST_CK_MS(master_num, chanel_num)+0x40))	
			{
				count++;
				continue;
			}
			if(base_addr_ls != R32(ACU_MST_CK_LS(master_num, chanel_num)+0x40)){
				count ++;
				continue;
			}
		}
#endif
		if(count == 0)
			break;
		else
			while(1);

	}while(1);

	return ACU_SUCCESS;
}

int acu_set_master_ck_h20x8_JR(unsigned int master_num, unsigned int chanel_num, unsigned int size, int chanel_en, unsigned int high_addr, unsigned int low_addr)
{
	unsigned int base_addr_ms;
	unsigned int base_addr_ls;
	unsigned int mst_ck_ms_value;

	if(master_num>=ACU_MST_MAX || chanel_num>=MST_CK_CHANEL_MAX)
		return ACU_FAIL;

	base_addr_ms = (high_addr>>12)&0x000fffff;
	base_addr_ls = (low_addr>>12)&0x000fffff;
	base_addr_ls |= ((high_addr&0x00000fff)<<20);
	mst_ck_ms_value = base_addr_ms;
	if(chanel_en)
//		mst_ck_ms_value |= (MST_CK_EN<<26);								//903T use this
		mst_ck_ms_value |= (3 << 25);	//from 4x80000 to 6x80000		//h20x8T add
	if(size<0 || size>0x3f)
		return	ACU_FAIL;
	else
		mst_ck_ms_value |= (size<<MST_CK_SIZE_OFFSET);

	if(master_num<3){
		W32(ACU_MST_CK_MS_h20x8_JR(master_num, chanel_num), (mst_ck_ms_value));			//the step of JR0--JR2: 0x40
		W32(ACU_MST_CK_LS_h20x8_JR(master_num, chanel_num), (base_addr_ls));
	}else{
		W32(ACU_MST_CK_MS_h20x8_JR_exp(master_num, chanel_num), (mst_ck_ms_value));		//the step between JR2 and JR3: 0x60
		W32(ACU_MST_CK_LS_h20x8_JR_exp(master_num, chanel_num), (base_addr_ls));
	}


	return ACU_SUCCESS;
}

void acu_set_rpt_fail_enable_h20x8(void)
{
	W32(ACU_RPT_CTRL_REG_h20x8, RPT_FAIL_EN);
}

void acu_set_rpt_fail_enable_h20x8_JR(void)
{
	W32(ACU_RPT_CTRL_REG_h20x8_JR, RPT_FAIL_EN);
}

void acu_clr_rpt_err_stat_h20x8(void)
{
	W32(ACU_RPT_CTRL_REG_h20x8, RPT_CLR_ERR);
}

void acu_clr_rpt_err_stat_h20x8_JR(void)
{
	W32(ACU_RPT_CTRL_REG_h20x8_JR, RPT_CLR_ERR);
}
