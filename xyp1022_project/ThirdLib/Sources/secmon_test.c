/*
 * secmon_test.c
 *
 *  Created on: 2017Äê11ÔÂ20ÈÕ
 *      Author: yyang
 */
#include "acu.h"
#include "axidma.h"
#include "secmon.h"
#include "sfm.h"
#include "rtic.h"
#include "ppclib.h"
#include "sec_common.h"
#include "common.h"
#include "e500mc_asm.h"

#define SECMON_903_TEST			0

volatile int secmon_int_flag = 0;

extern unsigned char data_e[256];
extern unsigned char data_n[256];
extern unsigned char data_sign[256];
extern unsigned char data_hash_padding[256];

void secmon_int_handler(void)
{
	unsigned int value;
	unsigned int data;

	value = R32(SECMON_HPSR_REG);
	if((HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL) && (HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_HARD_FAIL))
	{
		while(1);
	}
#if 0
	if(HPSR_SSM_ST_VALUE(value) == HPSR_SSM_ST_SOFT_FAIL)
	{
		W32(SECMON_HPSICR_REG, 0);//disable interrupt
	}
	else if(HPSR_SSM_ST_VALUE(value) == HPSR_SSM_ST_HARD_FAIL)
	{
		W32(SECMON_HPSICR_REG, 0);//disable interrupt
	}
	else
	{
		//error state
		W32(SECMON_HPSICR_REG, 0);//disable interrupt
	}
#endif
	//handle sv
	data = R32(SECMON_HPSVSR_REG);
	if(data&HPSVSR_SEC_VIO_ACU)
	{
#if SECMON_903_TEST
		value = R32(ACU_ERR_ST_REG);
#else
		value = R32(ACU_ERR_ST_REG_h20x8);
#endif
		if(value & (3<<(ACU_MST_DMA0+2))){
			//axidma_reset(); //to reset acu sv
		}
		else if(value & (1<<(ACU_MST_PCIE0+2))){
			//to reset pcie epdma
			//W32(PCIE0_DBI_BASE+0xa1c, 0x01000100);
			//W32(PCIE0_DBI_BASE+0x9c8, 0x01000100);
		}
#if SECMON_903_TEST
		value = R32(ACU_ILL_ADDR_HIGH_REG);
		value = R32(ACU_ILL_ADDR_LOW_REG);
		acu_clr_rpt_err_stat(); 			//903 clear acu sv
#else
		value = R32(ACU_ILL_ADDR_HIGH_REG_h20x8);
		value = R32(ACU_ILL_ADDR_LOW_REG_h20x8);
		acu_clr_rpt_err_stat_h20x8();		//h20x8 clear acu sv
#endif

#if SECMON_903_TEST
		value = R32(ACU_ERR_ST_REG);
		value |= R32(ACU_ILL_ADDR_HIGH_REG);
		value |= R32(ACU_ILL_ADDR_LOW_REG);
		if(value)
		{
			return ;//fail
		}
		acu_set_rpt_fail_enable();//enable sv report
#else
		value = R32(ACU_ERR_ST_REG_h20x8);
		value |= R32(ACU_ILL_ADDR_HIGH_REG_h20x8);
		value |= R32(ACU_ILL_ADDR_LOW_REG_h20x8);
		if(value)
		{
			return ;//fail
		}
		acu_set_rpt_fail_enable_h20x8();//enable sv report

#endif
	}
#if SECMON_903_TEST
	if(data&HPSVSR_SEC_VIO_RTIC)
	{
		value = R32(RTIC_RSTA_REG);
		W32(RTIC_RCMD_REG, 0x80000000);
		value = R32(RTIC_RSTA_REG);
		if(value & (3<<5|1<<15|0xf<<20|0xf<<24|1<<28|1<<29|1<<31))
		{
			return;
		}
	}
#else
	if(data&HPSVSR_SEC_VIO_RTIC_h20x8)
	{
		value = R32(RTIC_RSTA_REG);
		W32(RTIC_RCMD_REG, 0x80000000);
		value = R32(RTIC_RSTA_REG);
		if(value & (3<<5|1<<15|0xf<<20|0xf<<24|1<<28|1<<29|1<<31))
		{
			return;
		}
	}
#endif
	if(data&HPSVSR_SEC_VIO_BAD_KEY)
	{
		value = R32(SECMON_HPSR_REG);
//		if((value&0x00f0ff90) != 0x00300380) //check ssm state & optmk syndrome
//			return;

		W32(SFM_OTPMKR0_REG, 0xef0f928b);//to clear bad key sv
		W32(SFM_OTPMKR1_REG, 0x52255d2b);
		W32(SFM_OTPMKR2_REG, 0xfc05be54);
		W32(SFM_OTPMKR3_REG, 0x19fd8d72);
		W32(SFM_OTPMKR4_REG, 0x4241eced);
		W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
		W32(SFM_OTPMKR6_REG, 0xf2f24698);
		W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	}
	if(data&HPSVSR_SEC_VIO_SFM)
	{
		value = R32(SFM_STATE_REG);												
		if(value&(STATUS_EFUSE_READ_ERR_MASK|STATUS_EFUSE_WRITE_ERR_MASK))
		{
			W32(SFM_STATE_REG, value);
			value = R32(SFM_STATE_REG);
			if(value)
				return;
		}
	}

//	data = R32(SECMON_HPSVSR_REG);
	W32(SECMON_HPSVSR_REG, data);//clear sv status
	data = R32(SECMON_HPSVSR_REG);
	if(data != 0)
	{
		return;
	}

	secmon_int_flag = 1;

	return ;
}

#if 1
int secmon_lock_hac_test(void)
{
	unsigned int value;
	volatile unsigned int i;

	//set sfm master key
/*	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	value = R32(SECMON_HPSVSR_REG);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk
*/
	W32(SECMON_HPCMDR_REG, 0x80000000);
	value = R32(SECMON_HPSR_REG);
	if((value&SECMON_HPSR_REG != 0x00b00000) && (value&SECMON_HPSR_REG != 0x00d00000))
	{
		while(1);
	}

//	secmon_disable_hrst_req(); //disable hard rest caused by sv

	W32(SECMON_HPHACIVR_REG, 0x100);			//set init hac count
	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_LOAD);	//SECMON_HPHACR_REG now load count from SECMON_HPHACIVR_REG
	value = R32(SECMON_HPHACR_REG);
	if(value != 0x100)
	{
		while(1);
	}

	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_CLEAR);	//clear SECMON_HPHACR_REG test
	value = R32(SECMON_HPHACR_REG);
	if(value != 0x00)
	{
		while(1);
	}

	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_LOAD);	//SECMON_HPHACR_REG reload 
	W32(SECMON_HPLR_REG, HPLR_HAC_LOCK);		//lock hac
	value = R32(SECMON_HPHACR_REG);
	if(value != 0x100)
	{
		while(1);
	}

	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_EN);		//can't en after lock hac
	W32(SECMON_HPCMDR_REG, HPCMDR_SW_SV);		//trigger SW_SV

	for(i=0; i<1000; i++);
	value = R32(SECMON_HPSR_REG);
	while(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		value = R32(SECMON_HPSR_REG);
	}

	W32(SECMON_HPLR_REG, 0);//cannot unlock hac if set 1
	W32(SECMON_HPCMDR_REG, (HPCMDR_HAC_EN));//start hac should fail

	for(i=0; i<1000; i++);
	value = R32(SECMON_HPHACR_REG);
	if(value != 0x100)
	{
		while(1);
	}
	value = R32(SECMON_HPSR_REG);
	while(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		value = R32(SECMON_HPSR_REG);
	}

//SEC_MON state check, now non-secure
	W32(SECMON_HPCMDR_REG, 1<<31);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}

int secmon_reg_test(void)
{
	unsigned int value;
	
	/* ID0 */
	value = R32(SECMON_HPVIDR0_REG);
	if(value != 0x10)
		while(1);
	/* ID1 */
	value = R32(SECMON_HPVIDR1_REG);
	if(value != 0x10)
		while(1);

	/* HPLR */
	W32(SECMON_HPLR_REG, 0);
	/* Count Init */
	W32(SECMON_HPHACIVR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPHACIVR_REG);
	if(value != 0x5b5b5b5b)
	{
		while(1);
	}
	/* Now Can be changed by software */
	W32(SECMON_HPHACIVR_REG, 0);

	/* Interrupt Control */
	W32(SECMON_HPSICR_REG, 0x7ff00000);
	value = R32(SECMON_HPSICR_REG);
	if(value != 0x7ff00000)
	{
		while(1);
	}
	W32(SECMON_HPSICR_REG, 0);

	/* SV Control Set*/
	W32(SECMON_HPSVCR_REG, 0x7ff00000);
	value = R32(SECMON_HPSVCR_REG);
	if(value != 0x7ff00000)
	{
		while(1);
	}
	W32(SECMON_HPSVCR_REG, 0);

	/* HPFILT Count Register */
	W32(SECMON_HPMISCR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPMISCR_REG);
	if(value != 0x5b5b5b5b)
	{
		while(1);
	}
	W32(SECMON_HPMISCR_REG, 0);

	/* Be changed by software */
	W32(SECMON_HPFILTR_REG, 0x005b5b5b);
	value = R32(SECMON_HPFILTR_REG);
	if(value != 0x005b5b5b)
	{
		while(1);           
	}
	W32(SECMON_HPFILTR_REG, 0);

	/* Register Lock Function */
	W32(SECMON_HPLR_REG, HPLR_HAC_LOCK|HPLR_HPSICR_LOCK|HPLR_HPSVCR_LOCK|HPLR_MISC_LOCK);
	/* Enable HP_HAC */
	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_EN);
	value = R32(SECMON_HPCMDR_REG);
	if(value & HPCMDR_HAC_EN)
	{
		while(1);
	}
	/* Now Can't be writed */
	W32(SECMON_HPHACIVR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPHACIVR_REG);
	if(value != 0)
	{
		while(1);
	}

	/* add test */
	W32(SECMON_HPHACR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPHACIVR_REG);
	if(value != 0)
	{
		while(1);
	}
	/* Lockeed */
	W32(SECMON_HPSICR_REG, 0x7ff00000);
	value = R32(SECMON_HPSICR_REG);
	if(value != 0)
	{
		while(1);
	}
	/* Lockeed */
	W32(SECMON_HPSVCR_REG, 0x7ff00000);
	value = R32(SECMON_HPSVCR_REG);
	if(value != 0)
	{
		while(1);
	}
	/* Lockeed */
	W32(SECMON_HPMISCR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPMISCR_REG);
	if(value != 0)
	{
		while(1);
	}
	/* Lockeed */
	W32(SECMON_HPFILTR_REG, 0x005b5b5b);
	value = R32(SECMON_HPFILTR_REG);
	if(value != 0)
	{
		while(1);
	}

	/* unlock register */
	W32(SECMON_HPLR_REG, 0);
	value = R32(SECMON_HPLR_REG);
	if(value == 0)
	{
		while(1);
	}

	/* can not unlock if set 1 */
	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_EN);
	value = R32(SECMON_HPCMDR_REG);
	if(value & HPCMDR_HAC_EN)
	{
		while(1);
	}

	W32(SECMON_HPHACIVR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPHACIVR_REG);
	if(value != 0)
	{
		while(1);
	}

	W32(SECMON_HPHACR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPHACIVR_REG);
	if(value != 0)
	{
		while(1);
	}

	W32(SECMON_HPSICR_REG, 0x7ff00000);
	value = R32(SECMON_HPSICR_REG);
	if(value != 0)
	{
		while(1);
	}

	W32(SECMON_HPSVCR_REG, 0x7ff00000);
	value = R32(SECMON_HPSVCR_REG);
	if(value != 0)
	{
		while(1);
	}

	W32(SECMON_HPMISCR_REG, 0x5b5b5b5b);
	value = R32(SECMON_HPMISCR_REG);
	if(value != 0)
	{
		while(1);
	}

	W32(SECMON_HPFILTR_REG, 0x005b5b5b);
	value = R32(SECMON_HPFILTR_REG);
	if(value != 0)
	{
		while(1);
	}

	/* test ssm_st_dis bit */
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_CHECK)
	{
		while(1);
	}
	/* trigger ST */
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);	
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SECURE)
	{
		while(1);
	}

	/* disable secure -> tursted */
	W32(SECMON_HPCMDR_REG, HPCMDR_ST_DIS);
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) == HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}
	/* can not modify if set 1 */
	W32(SECMON_HPCMDR_REG, 0);
	value = R32(SECMON_HPCMDR_REG);
	if((value&HPCMDR_ST_DIS) == 0)
	{
		while(1);
	}
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) == HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}

	return 0;
}

int secmon_hac_counter_test(void)
{
	unsigned int value;
	unsigned int value_ssm_st;
	unsigned int value_count;
	volatile int i;
	volatile int count;

	//set fsm master key
/*	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk
*/
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_CHECK)
	{
		while(1);
	}

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SECURE)
	{
		while(1);
	}

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}

//	secmon_disable_hrst_req(); //disable hard rest caused by sv

	W32(SECMON_HPHACIVR_REG, 0x00000100);//init counter value
	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_LOAD);

	//trig event0
	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_EN|HPCMDR_SW_SV);//trigger sv, ssm state change to soft fail

	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_STOP);//stop hac

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		while(1);
	}
	value_ssm_st = R32(SECMON_HPSR_REG);
	value_count = R32(SECMON_HPHACR_REG);
	if(value_count == 0x00000100) //counter decrease
	{
		while(1);
	}
	W32(SECMON_HPCMDR_REG, HPCMDR_HAC_EN);//start hac

	for(i=0; i<1000; i++)
	{
		;
	}

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_HARD_FAIL)
	{
		while(1);
	}

	return 0;
}


int secmon_bad_key_sv_test(void)
{
	unsigned int value;
	unsigned int i;

	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);

	//set fsm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk

	//nomore useful
//	secmon_disable_hrst_req(); //disable hard rest caused by sv

	secmon_svi_enable(HPSICR_SVI_EN_BAD_KEY|HPSICR_SVI_EN_TMP);//enable bad key interrupt
	secmon_sv_enable(HPSVCR_CFG_BAD_KEY|HPSICR_SVI_EN_TMP);//config bad key sv

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger to trusted state
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}

//	W32(SECMON_HPCMDR_REG, HPCMDR_SW_SV);//trigger sv to soft fail
//	W32(SFM_OTPMKR0_REG, 0xef0f9283); //trigger bad key sv
//	W32(SFM_OTPMKR1_REG, 0x52255d23);
//	W32(SFM_OTPMKR0_REG, 0xef0f928a);//otpmk bit0
//	W32(SFM_OTPMKR0_REG, 0xef0f9289);//otpmk bit1
//	W32(SFM_OTPMKR0_REG, 0xef0f928f);//otpmk bit2
	W32(SFM_OTPMKR2_REG, 0xfc05be55);//optmk bit64

	while(secmon_int_flag == 0);

	value = R32(SECMON_HPSR_REG);
	do{
		value = R32(SECMON_HPSR_REG);
		if((value&0x00f0ff90) == 0x00300000) //check ssm state & optmk syndrome
			break;
	}while(1);

/*
	W32(SFM_OTPMKR0_REG, 0xef0f928b);//to clear status
	value = R32(SECMON_HPSVSR_REG);
	W32(SECMON_HPSVSR_REG, value);
	value = R32(SECMON_HPSVSR_REG);
	if(value != 0)
	{
		return  1;
	}
*/

//SEC_MON state check
	W32(SECMON_HPCMDR_REG, 1<<31);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}

void rng_imp_init(void)
{
	*(volatile unsigned int *)(0xe0100000 + 0x74c) = 0xf; //TRNG_CLKCRL
	*(volatile unsigned int *)(0xe0100000 + 0x234) = 0x5500; //CFG1
	*(volatile unsigned int *)(0xe0100000 + 0x700) = 0x040f0100; //TRNG_CR
	*(volatile unsigned int *)(0xe0100000 + 0x750) = 0xffffffff; //TRNG_RST
	*(volatile unsigned int *)(0xe0100000 + 0x630) = 0x2; //TRNG_HASHCTRL
}

int secmon_trng_key_test(void)
{
	unsigned int value;

	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
#if 0
	//set fsm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
#endif

/*
	secmon_disable_hrst_req(); //disable hard rest caused by sv
	secmon_svi_enable(HPSICR_SVI_EN_BAD_KEY);//enable bad key interrupt
	secmon_sv_enable(HPSVCR_CFG_BAD_KEY);//config bad key sv

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger to trusted state
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}
*/
	secmon_disable_hrst_req(); //disable hard rest caused by sv
	secmon_sv_enable(0);//disable bad key sv

	//config trng
	rng_imp_init();
	//end config trng

	W32(SFM_MK_HWP_MODE_REG, 0x03);//hardware program
	W32(SFM_ISTR_REG, 0x04);  //generate OTPMK by call TRNG
	value = R32(SFM_ISTR_REG);
	while((value&0x04) != 0){        //check if done
		value = R32(SFM_ISTR_REG);
	}

	secmon_svi_enable(HPSICR_SVI_EN_BAD_KEY);//enable bad key interrupt
	secmon_sv_enable(HPSVCR_CFG_BAD_KEY);//config bad key sv

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger to trusted state
	value = R32(SECMON_HPSR_REG);
//	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_TRUSTED)
	if(value != 0x00d00000)//if load master key success from trng
	{
		while(1);
	}

	W32(SFM_MK_HWP_MODE_REG, 0x00);//software program
	//trigger fail
//	value = R32(SFM_OTPMKR0_REG); //return 0xffffffff
//	value ^= 0x0000008;
//	W32(SFM_OTPMKR0_REG, value);

	value = R32(SECMON_HPSR_REG);
	do{
		value = R32(SECMON_HPSR_REG);
		if((value&0x00f0ff90) == 0x00300300) //check ssm state & optmk syndrome
			break;
	}while(1);

	return 0;
}


int secmon_acu_sv_test(void)
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

	ch = 1;
	i = 0;
	j = 0;
	k = 4; //total size 0x1004
	src_msize = 0;
	dst_msize = 0;

	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
//	secmon_disable_hrst_req(); //disable hard rest caused by sv
	secmon_svi_enable(HPSICR_SVI_EN_ACU);
	acu_set_rpt_fail_enable_h20x8();
	secmon_sv_enable(HPSVCR_CFG_ACU);//config acu sv

	//set fsm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger to trusted state
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}

//	acu_set_master_ctrl(1<<ACU_MST_DMA0 | 1<<ACU_MST_DMA1);
	acu_set_master_ctrl(1<<ACU_MST_DMA0);
	//src dma0 master
	ret = acu_set_master_ck(ACU_MST_DMA0, 0, ACU_CH_SIZE_4K, MST_CK_EN, 0x00000000, 0x10000000);

	if(ret)
	{
		while(1);
	}
	
	//des dma0 master
	ret = acu_set_master_ck(ACU_MST_DMA0, 1, ACU_CH_SIZE_8K, MST_CK_EN, 0x00000000, 0x20000000);			//use one DMA

	if(ret)
	{
		while(1);
	}
	
	dma_axi_bus();
	axidma_reset();
	buf_init(0x10000000+i, 0x1000+k, ch+src_msize+dst_msize+i+j+k);

//	axidma_transfer_one_block(ch, 0xe0005000+i, 0xe0007000+j, 0x1000+k-1, DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));		//use two DMA
//	axidma_transfer_one_block(ch, 0xe0005000+i, 0xe0006C00+j, 0x1000+k-1, DMS_1 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));		//use one DMA
	axidma_transfer_one_block_h20x8(ch, 0x00000000, 0x10000000, 0, 0x20000000, 0x1000+k,  DMS_2 | (src_msize << 14) | (SRC_TR_WIDTH_8 ) | (dst_msize << 18) | (DST_TR_WIDTH_8 ));
	while(secmon_int_flag == 0);

	value = R32(CH_INT_STATUS_REG(ch));
	if(value & DMA_TFR_DONE_INTSTAT)
	{
//		uart_printf("set acu read test fail\n");
		W32(CH_INT_CLEAR_REG(ch), value);
		while(1);
	}

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		while(1);
	}

	return 0;
}


int secmon_nonsecure_state_test()
{
	unsigned int value;
	
	/* now non-use this to disable hard rest */
//	secmon_disable_hrst_req(); //disable hard rest caused by sv

	value = R32(SECMON_HPSR_REG);

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger
	value = R32(SECMON_HPSR_REG);

	W32(SECMON_HPCMDR_REG, HPCMDR_SW_SV);//trigger sv to soft fail
	value = R32(SECMON_HPSR_REG);

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger soft fail to secure
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}

int secmon_disable_nonsecure_state_test()
{
	unsigned int value;

//	secmon_disable_hrst_req(); //disable hard rest caused by sv
	W32(SECMON_HPCMDR_REG, HPCMDR_SFNS_DIS);

	value = R32(SECMON_HPSR_REG);

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger
	value = R32(SECMON_HPSR_REG);

	W32(SECMON_HPCMDR_REG, HPCMDR_SW_SV);//trigger sv to soft fail
	value = R32(SECMON_HPSR_REG);

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) == HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}

void secmon_ssm_st_dis_test()
{
	unsigned int value;
	volatile unsigned int i;

	//set sfm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_CHECK)
	{
		while(1);
	}

	W32(SECMON_HPCMDR_REG, HPCMDR_ST_DIS);//disable secure -> tursted

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SECURE)
	{
		while(1);
	}


	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) == HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}
}
#endif

int secmon_rtic_ae_sv_test(void)
{
	int i;
	unsigned int value;
#if 0
	//set fsm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk
#endif

	value = R32(SECMON_HPSR_REG);
/*
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_CHECK)
	{
		while(1);
	}
*/
	secmon_int_flag = 0;

	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
//	secmon_disable_hrst_req(); //disable hard rest caused by sv
#if 0
	//90T, new903T
	secmon_svi_enable(HPSICR_SVI_EN_RTIC);
	secmon_sv_enable(HPSVCR_CFG_RTIC);//config rtic sv
#else
	//h20x8T
	secmon_svi_enable(HPSICR_SVI_EN_RTIC);
	secmon_sv_enable(HPSVCR_CFG_RTIC);//config rtic sv
#endif
	//config rtic
	//W32(RTIC_RCTL_REG, 0x7<<24);
	W32(RTIC_RCTL_REG, (0x1<<24 | 0x1<<12));


	W32(RTIC_RMAA_MS_REG, 0xffeeddcc);
	W32(RTIC_RMAA_LS_REG, 0xddccbbaa);
/*	W32(RTIC_RMBA_MS_REG, 0xffeeddcc);
	W32(RTIC_RMBA_LS_REG, 0xddccbbaa);
	W32(RTIC_RMCA_MS_REG, 0xffeeddcc);
	W32(RTIC_RMCA_LS_REG, 0xddccbbaa);
	W32(RTIC_RMDA_MS_REG, 0xffeeddcc);
	W32(RTIC_RMDA_LS_REG, 0xddccbbaa);
*/
	W32(RTIC_RMAL_REG, 0x200);
//	W32(RTIC_RMBL_REG, 0x200);
//	W32(RTIC_RMCL_REG, 0x200);
//	W32(RTIC_RMDL_REG, 0x200);

	//trig rtic sv
	W32(RTIC_RCMD_REG, 1<<30);

	while(secmon_int_flag == 0);

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		while(1);
	}

//SEC_MON state check
	W32(SECMON_HPCMDR_REG, 1<<31);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}

int secmon_rtic_mis_sv_test(void)
{
	int i,j;
	unsigned int value;
	unsigned char test_data[512];
	unsigned int addr;
	unsigned int hash_value[8];
	unsigned int hash_fail[8];

	//set fsm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk

	for(i=0; i<512; i++)
		test_data[i] = (unsigned char)(i+0x10);

	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
//	secmon_disable_hrst_req(); //disable hard rest caused by sv
#if 0
	//90T, new903T
	secmon_svi_enable(HPSICR_SVI_EN_RTIC);
	secmon_sv_enable(HPSVCR_CFG_RTIC);//config rtic sv
#else
	//h20x8T
	secmon_svi_enable(HPSICR_SVI_EN_RTIC);
	secmon_sv_enable(HPSVCR_CFG_RTIC);//config rtic sv
#endif

	addr = (unsigned int)&test_data[0];
	W32(RTIC_RCTL_REG, (0x1<<24 | 0x1<<20));
	W32(RTIC_RMAA_MS_REG, 0x80000000);
	W32(RTIC_RMAA_LS_REG, addr);
	W32(RTIC_RMAL_REG, 0x200);

	for(j=0; j<1; j++)
	{
		W32(RTIC_RCMD_REG, 1<<30);//once hash start

		value = R32(RTIC_RSTA_REG);
		while(1){
			value = R32(RTIC_RSTA_REG);
			if(value & (1<<30))//once hash done
				break;
		}
		for(i=0; i<8; i++)
			hash_value[i] = R32(RTIC_BASE_ADDR + 0x54 + i*4);

		W32(RTIC_RCMD_REG, 0x80000000);
	}

	//watch dog error test
	W32(RTIC_RWDOG_MS_REG, 0x00000000);
	W32(RTIC_RWDOG_LS_REG, 0x00000010);
	W32(RTIC_RTHR_MS_REG, 0x00000000);
	W32(RTIC_RTHR_LS_REG, 0x00001000);
	W32(RTIC_RCMD_REG, 1<<29);//rtic hash //watch dog error

	while(secmon_int_flag == 0);

	//rtic hash error test
	secmon_int_flag = 0;
	W32(RTIC_RCTL_REG, (1<<24 | 1<<20));
	W32(RTIC_RMAA_MS_REG, 0x80000000);
	W32(RTIC_RMAA_LS_REG, addr);
	W32(RTIC_RWDOG_MS_REG, 0x00000000);
	W32(RTIC_RWDOG_LS_REG, 0x01000000);
	W32(RTIC_RTHR_MS_REG, 0x00000000);
	W32(RTIC_RTHR_LS_REG, 0x00001000);

	test_data[0] ^= 0x80;//modify memory data

	W32(RTIC_RCMD_REG, 1<<29);//rtc hash start

	while(secmon_int_flag == 0);

	for(i=0; i<8; i++)
	{
		hash_fail[i] = R32(RTIC_BASE_ADDR + 0x54 + i*4);
		if(hash_fail[i] == hash_value[i])
			while(1);
	}

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		while(1);
	}

	return 0;
}

int secmon_sfm_ccore_sv_test(void)
{
	unsigned int value;
	int i;

#if 1
	//set fsm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, 0xfff00000);//clear bad key sv after init optmk

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_CHECK)
	{
		while(1);
	}
#endif

	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
//	secmon_disable_hrst_req(); //disable hard rest caused by sv
	secmon_svi_enable(HPSICR_SVI_EN_BAD_KEY);
	secmon_sv_enable(HPSVCR_CFG_BAD_KEY);//config rtic sv

//	sfm_write_test();

	//config sfm
	W32(SFM_CCUID0_REG, 0x00105a5a);
	W32(SFM_CCUID1_REG, 0x00205a5a);
	W32(SFM_CCGPR0_REG, 0xf65e01d4);
	W32(SFM_CCGPR1_REG, 0x45de8920);

	W32(SFM_CCPRT_REG, 0x80000000);//ccore write protect

	value = R32(SFM_BASE_ADDR+0x8f0);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_BASE_ADDR+0x8f0);
	}

//	W32(SFM_CCUID0_REG, 0x12345678); //trig sfm
//	W32(SFM_CCUID1_REG, 0x12345678);
//	W32(SFM_CCGPR0_REG, 0x12345678);
//	W32(SFM_CCGPR1_REG, 0x12345678);


//	value = R32(SFM_BASE_ADDR+0x8f0);
//	while(value){
//		value = R32(SFM_BASE_ADDR+0x8f0);
//	}

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//command before ISTR[1]
	W32(SFM_ISTR_REG, 0x2);

	value = R32(SFM_BASE_ADDR+0x8f0);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_BASE_ADDR+0x8f0);
	}

	//clear
	for(i=0; i<0x10; i+=4)
		W32(SFM_CCUID0_REG+i, 0x00000000);

	//read again to ensure write protect effective
	value = R32(SFM_BASE_ADDR+0x8f0);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_BASE_ADDR+0x8f0);
	}

	W32(SFM_ISTR_REG, 0x1);

	value = R32(SFM_BASE_ADDR+0x8f0);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_BASE_ADDR+0x8f0);
	}

//	W32(SFM_CCWP_REG, 0x80000000);//ccore write protect //write again
	W32(SFM_CCUID0_REG, 0x12345678); //trig sfm
	//	W32(SFM_CCUID1_REG, 0x12345678);
	//	W32(SFM_CCGPR0_REG, 0x12345678);
	//	W32(SFM_CCGPR1_REG, 0x12345678);

	while(secmon_int_flag == 0);

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		while(1);
	}

	return 0;
}

int secmon_sfm_oem_sv_test(void)
{
	unsigned int value;
	int i;

#if 0
	//set fsm master key
	W32(SFM_OTPMKR0_REG, 0xef0f928b);
	W32(SFM_OTPMKR1_REG, 0x52255d2b);
	W32(SFM_OTPMKR2_REG, 0xfc05be54);
	W32(SFM_OTPMKR3_REG, 0x19fd8d72);
	W32(SFM_OTPMKR4_REG, 0x4241eced);
	W32(SFM_OTPMKR5_REG, 0xfbaaaddb);
	W32(SFM_OTPMKR6_REG, 0xf2f24698);
	W32(SFM_OTPMKR7_REG, 0x9fb271c1);
	W32(SECMON_HPSVSR_REG, 0xfff00000);//clear bad key sv after init optmk

	value = R32(SECMON_HPSR_REG);
/*	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_CHECK)
	{
		while(1);
	}
*/
#endif

	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
//	secmon_disable_hrst_req(); //disable hard rest caused by sv
	secmon_svi_enable(HPSICR_SVI_EN_BAD_KEY);
	secmon_sv_enable(HPSVCR_CFG_BAD_KEY);//config rtic sv

	//config oem sfm
	W32(SFM_OEMGPR0_REG, 0x00105a5a);

	value = R32(SFM_OEMPRT_REG);
	value |= 0x80000000;
	W32(SFM_OEMPRT_REG, value);										//oem write protect


	value = R32(SFM_STATE_REG);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//command before ISTR[1]
	W32(SFM_ISTR_REG, 0x2);

	value = R32(SFM_STATE_REG);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_STATE_REG);
	}
	//clear
	W32(SFM_OEMGPR0_REG, 0x00000000);

	//read again to ensure write protect effective
	value = R32(SFM_STATE_REG);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_STATE_REG);
	}
	W32(SFM_ISTR_REG, 0x1);

	value = R32(SFM_STATE_REG);
	while(value & STATUS_EFUSE_BUSY_MASK){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_OEMGPR0_REG, 0x12345678); 								//trig sfm
	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//command before ISTR[1]
	W32(SFM_ISTR_REG, 0x2);

	while(secmon_int_flag == 0);

	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_SOFT_FAIL)
	{
		while(1);
	}

//SEC_MON state check
	W32(SECMON_HPCMDR_REG, 1<<31);
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}
