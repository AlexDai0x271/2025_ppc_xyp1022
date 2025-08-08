/*
 * sfm.c
 *
 *  Created on: 2019Äê12ÔÂ24ÈÕ
 *      Author: sytu
 */
#include "e500mc_asm.h"
#include "sfm.h"
#include "secmon.h"
#include "acu.h"
#include "common.h"

//volatile static int secmon_int_flag = 0;
extern volatile int secmon_int_flag;
extern void secmon_int_handler(void);
extern unsigned char data_e[256];
extern unsigned char data_n[256];
extern unsigned char data_sign[256];
extern unsigned char data_hash_padding[256];

int sfm_read_test()
{
	volatile unsigned int value, UID, GPR;
	int i;

	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_ISTR_REG, 0x1);
	ISYNC;
	
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	UID = 0x00105a5a|0xcbcbcbcb;
	GPR = 0x1001234d|0xfafafafa;
	
	value = R32(SFM_CCUID0_REG);
	if(value != UID)
		while(1);
	
	value = R32(SFM_CCUID1_REG);
	if(value != GPR)
		while(1);

	return 0;
}



/*
 * without program_permission command and without protect
 * when read back, data should be different from UID/GPR
 */
int sfm_write_test1()
{
	volatile unsigned int value;
	unsigned int i, UID=0x00105a5a, GPR=0x1001234d;

	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_CCUID0_REG, UID);
	W32(SFM_CCUID1_REG, GPR);

//	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//delete program_permission command for test
	W32(SFM_ISTR_REG, 0x2);											//write command									
	ISYNC;
	
	value = R32(SFM_STATE_REG);
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//clear
	for(i=0; i<0x10; i+=4)
		W32(SFM_CCUID0_REG+i, 0x00000000);

	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_ISTR_REG, 0x1);											//read command
	ISYNC;
	
	value = R32(SFM_STATE_REG);
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//read should be all 0
	if( R32(SFM_CCUID0_REG) == UID || R32(SFM_CCUID1_REG) == GPR)
		while(1);

	return 0;
}

/*
 * with program_permission command but without protect
 * when read back, data should be same with UID/GPR
 */
int sfm_write_test2()
{
	volatile unsigned int value;
	unsigned int i, UID=0x00105a5a, GPR=0x1001234d;
	
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_CCUID0_REG, UID);
	W32(SFM_CCUID1_REG, GPR);

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//program_permission command before Write_Fuse by ISTR[1]
	W32(SFM_ISTR_REG, 0x2);											//write command
	ISYNC;
	
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//clear 
	for(i=0; i<0x10; i+=4)
		W32(SFM_CCUID0_REG+i, 0x00000000);

	W32(SFM_ISTR_REG, 0x1);											//read command
	ISYNC;
	
	value = R32(SFM_STATE_REG);
	while(value){
		value = R32(SFM_STATE_REG);
	}
	
	//read should same with UID/GPR
	if( R32(SFM_CCUID0_REG) != UID || R32(SFM_CCUID1_REG) != GPR)
		while(1);

	return 0;
}

/*
 * with command but without protect
 * when read back, data should be bem UID|0xcbcbcbcb and GPR|0xfafafafa
 */
int sfm_write_test3()
{
	volatile unsigned int value;
	unsigned int i, UID=0x00105a5a, GPR=0x1001234d;

	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_CCUID0_REG, 0xcbcbcbcb);
	W32(SFM_CCUID1_REG, 0xfafafafa);

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//program_permission command
	W32(SFM_ISTR_REG, 0x2);
	ISYNC;
	
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//clear 
	for(i=0; i<0x10; i+=4)
		W32(SFM_CCUID0_REG+i, 0x00000000);

	W32(SFM_ISTR_REG, 0x1);											//read command
	ISYNC;
	
	value = R32(SFM_STATE_REG);
	while(value){
		value = R32(SFM_STATE_REG);
	}
	
	//read should same with UID/GPR
	value = R32(SFM_CCUID0_REG);
	uart_printf("CCUID0 data is: %x\n", value);
	value = R32(SFM_CCUID1_REG);
	uart_printf("CCUID1 data is: %x\n", value);
	
	if( (R32(SFM_CCUID0_REG) != (UID|0xcbcbcbcb)) || (R32(SFM_CCUID1_REG) != (GPR|0xfafafafa)))
		while(1);

	return 0;

}

/*
 * sfm process test with specified sector and the size by byte
 */
int sfm_sector_test(unsigned int src_addr, unsigned int size)
{
	volatile unsigned int value;
	unsigned int i; 
	unsigned char *rand_data1, *rand_data2;

	rand_data1 = (unsigned char *)malloc(size);
	rand_data2 = (unsigned char *)malloc(size);
	
	/* test 1 */
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	for(i=0; i<size; i+=4){
		W32(rand_data1+i, rand());
		W32(src_addr+i, R32(rand_data1+i));
	}

//	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//delete programme_permission command for test
	W32(SFM_ISTR_REG, 0x2);											//write command									
	ISYNC;
	
	value = R32(SFM_STATE_REG);
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//clear
	for(i=0; i<size; i+=4)
		W32(src_addr+i, 0x00000000);

	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	W32(SFM_ISTR_REG, 0x1);											//read command
	ISYNC;
	
	value = R32(SFM_STATE_REG);
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//read should be all 0
	for(i=0; i<size; i+=4){
		if(R32(src_addr+i) != 0)
			while(1);
	}
	
	/* test 2 */
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}
	
	for(i=0; i<size; i+=4){
		W32(rand_data1+i, rand());
		W32(src_addr+i, R32(rand_data1+i));
	}

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//program_permission command
	W32(SFM_ISTR_REG, 0x2);											//write command
	ISYNC;
	
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//clear 
	for(i=0; i<size; i+=4)
		W32(src_addr+i, 0x00000000);

	W32(SFM_ISTR_REG, 0x1);											//read command
	ISYNC;
	

	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}
	
	//read should be same with data of rand_data[]
	for(i=0; i<size; i+=4){
		if(R32(src_addr+i) != R32(rand_data1+i))
			while(1);
	}

	/* test 3 */
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	for(i=0; i<size; i+=4){
		W32(rand_data2+i, rand());
		W32(src_addr+i, R32(rand_data2+i));
	}

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);						//program_permission command
	W32(SFM_ISTR_REG, 0x2);											//write command
	ISYNC;
	
	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//clear 
	for(i=0; i<size; i+=4)
		W32(src_addr+i, 0x00000000);

	W32(SFM_ISTR_REG, 0x1);											//read command
	ISYNC;
	
	value = R32(SFM_STATE_REG);
	while(value){
		value = R32(SFM_STATE_REG);
	}
	
	//read should be same with data of rand_data1 | rand_data2
	for(i=0; i<size; i+=4){
		if(R32(src_addr+i) != (R32(rand_data1+i) | R32(rand_data2+i)) )
			while(1);
	}

	free(rand_data1);
	free(rand_data2);
	return 0;
}

/*
 *  sfm close the specified DECO x
 *  SEC only open specified DECO x
 *  so, job should not be finished
 */
void sfm_deco_test(unsigned int num)
{
	volatile unsigned int value;
	int i,ret,index,count=0;
	unsigned char in_e[256];
	unsigned char in_n[256];
	unsigned char *in_sign = 0xe007f000;
	unsigned char *out_hash = 0xe0078000;

	value = R32(SFM_STATE_REG);										//if busy, while(1)
	while(value){
		value = R32(SFM_STATE_REG);
	}

	//simple rsa normal test
	memset_c(in_e, 0, 256);
	memset_c(in_n, 0, 256);
	memset_c(in_sign, 0, 256);

	memcpy_c(in_e, data_e, 256);
	memcpy_c(in_n, data_n, 256);
	memcpy_c(in_sign, data_sign, 256);

	//not set acu
	acu_set_master_ctrl(0);
	acu_set_master_ctrl_h20x8_JR(0);
	sec_init();
	
	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
//	secmon_disable_hrst_req(); //disable hard rest caused by sv
	secmon_svi_enable(HPSICR_SVI_EN_SFM);
	secmon_sv_enable(HPSVCR_CFG_SFM);//config rtic sv

	//open all deco
	for(i=0; i<8; i++)
	{
		W32(0xe9205e00+i*0x1000, 1);
	}

	memset_c(out_hash, 0, 256);
	rsa_verify_for_ACU(out_hash, in_n, in_e, in_sign);

	if(!memcmp_c(out_hash, data_hash_padding, 256))
	{
		PSPRINTF("std rsa test ok!\n");
		count++;
	}
	else
	{
		PSPRINTF("std error rsa test!\n");
		while(1);
	}

	//test 1, fuse open DECO_num
	//close all deco
	for(i=0; i<8; i++)
	{
		W32(0xe9205e00+i*0x1000, 0);
	}

	//only open the NO.num deco
	W32(0xe9205e00+num*0x1000, 1);

	memset_c(out_hash, 0, 256);
	ret = rsa_verify_for_ACU(out_hash, in_n, in_e, in_sign);			//should be can't finish the job, while in run_descriptor_jr_idx

	if(ret != 0)
	{
		PSPRINTF("#### if print here, fuse is error! ####\n");
		while(1);
	}
	
	//test 2, fuse close DECO_num
	W32(SFM_CCMODE_REG, 1<<num);										

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);							//command before ISTR[1]
	W32(SFM_ISTR_REG, 0x2);
	ISYNC;
	
	//reset sec
	sec_init();
	memset_c(out_hash, 0, 256);
	ret = rsa_verify_for_ACU(out_hash, in_n, in_e, in_sign);			//should be can't finish the job, while in run_descriptor_jr_idx

	if(ret == 0)
	{
		PSPRINTF("#### if print here, fuse is error! it should be can't finished ####\n");
		while(1);
	}
}

void fuse_otpmk_hw()
{
	unsigned int i;
	
	rng_impl_init();													//analogy rng sec mode set
	
	for(i=0; i<8; i++)
		if(R32(SFM_OTPMKR0_REG+4*i))
			while(1);

	W32(SFM_MK_HWP_MODE_REG, 1);										//HW Programming mode of OTPMK
	W32(SFM_ISTR_REG, 0x1<<2);											//START HW_RANDOM_OPTMK
	ISYNC;

	for(i=0; i<8; i++)
		if(!R32(SFM_OTPMKR0_REG+4*i))
			while(1);
}

void fuse_mpumk_hw()
{

	rng_impl_init();													//analogy rng sec mode set
	
	if(R32(SFM_MPUMK_REG))
		while(1);

	W32(SFM_MK_HWP_MODE_REG, 2);										//HW Programming mode of MPU
	W32(SFM_ISTR_REG, 0x1<<3);											//START HW_RANDOM_MPU
	delay(1000);

	if(!R32(SFM_MPUMK_REG))
		while(1);
}

int fuse_secmon_otpmk_bad_key_sv_test(void)
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

	/* if open, when secmon is triggered, will cause hard reset */
//	secmon_disable_hrst_req(); 	//disable hard reset caused by sv

	secmon_svi_enable(HPSICR_SVI_EN_BAD_KEY|HPSICR_SVI_EN_TMP);//enable bad key interrupt
	secmon_sv_enable(HPSVCR_CFG_BAD_KEY|HPSICR_SVI_EN_TMP);//config bad key sv

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);		//trigger to trusted state
	ISYNC;
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

/* SEC_MON state check */
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	ISYNC;
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}

int fuse_secmon_mpumk_bad_key_sv_test(void)
{
	unsigned int value;
	unsigned int i;

	secmon_int_flag = 0;
	InstallInterruptHandler0(SECMON_INT_NUM, secmon_int_handler);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk

	/* if open, when secmon is triggered, will cause hard reset */
//	secmon_disable_hrst_req(); //disable hard reset caused by sv

	secmon_svi_enable(HPSICR_SVI_EN_BAD_KEY|HPSICR_SVI_EN_TMP);//enable bad key interrupt
	secmon_sv_enable(HPSVCR_CFG_BAD_KEY|HPSICR_SVI_EN_TMP);//config bad key sv
//	secmon_sv_enable(HPSVCR_CFG_BAD_KEY);//config bad key sv

	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);//trigger to trusted state
	ISYNC;
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_TRUSTED)
	{
		while(1);
	}
	//set fsm master key
	W32(SFM_MPUMK_REG, 0xef0f928b);
	W32(SECMON_HPSVSR_REG, HPSVSR_SEC_VIO_BAD_KEY);//clear bad key sv after init optmk

	W32(SFM_FUSE_PRORAM_KEY_REG, 0xc00df00d);							//command before ISTR[1]
	W32(SFM_ISTR_REG, 0x2);

//	W32(SECMON_HPCMDR_REG, HPCMDR_SW_SV);//trigger sv to soft fail
//	W32(SFM_OTPMKR0_REG, 0xef0f9283); //trigger bad key sv
//	W32(SFM_OTPMKR1_REG, 0x52255d23);
//	W32(SFM_OTPMKR0_REG, 0xef0f928a);//otpmk bit0
//	W32(SFM_OTPMKR0_REG, 0xef0f9289);//otpmk bit1
//	W32(SFM_OTPMKR0_REG, 0xef0f928f);//otpmk bit2
	W32(SFM_MPUMK_REG, 0xfc05be55);//optmk bit64

	while(secmon_int_flag == 0);

	value = R32(SECMON_HPSR_REG);
	do{
		value = R32(SECMON_HPSR_REG);
		if((value&0x00f0ff90) == 0x00300000) //check ssm state & optmk syndrome
			break;
	}while(1);

/* SEC_MON state check */
	W32(SECMON_HPCMDR_REG, HPCMDR_SSM_ST);
	ISYNC;
	value = R32(SECMON_HPSR_REG);
	if(HPSR_SSM_ST_VALUE(value) != HPSR_SSM_ST_NON_SECURE)
	{
		while(1);
	}

	return 0;
}

void fuse_rscp_test()
{
	sec_init();
	init_rscp_pm();
	
	while(1)
		rscp_cpu_sm4_test();
}
/*
 * test1: fuse should not take effect without command and write_protect
 * test2: fuse should not take effect with command but without write_protect
 * test3: fuse should take effect with command and write_protect
 */
void fuse_command_test()
{
	sfm_write_test1();					//no-cammand, no-write protect
	sfm_write_test2();					//cammand, no-write protect, wr 0x00105a5a and 0x1001234d
	sfm_write_test3();					//re-write, no-write protect, wr 0xcbcbcbcb and 0xfafafafa
	sfm_read_test();					//read should be 0xcbdbdbdb and 0xfafbfbff

	sfm_sector_test(SFM_PKHR0_REG, 32);
}

/*
 * fuse close DECO test
 */
void fuse_DECO_test()
{
	sfm_deco_test(0);					//finally, job should be can't finished
	sfm_deco_test(1);
	sfm_deco_test(2);
	sfm_deco_test(3);
	sfm_deco_test(4);
	sfm_deco_test(5);
	sfm_deco_test(6);
	sfm_deco_test(7);

}
