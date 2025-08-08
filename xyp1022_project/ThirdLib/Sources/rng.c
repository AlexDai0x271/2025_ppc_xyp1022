/*
 * drng.c
 *
 *  Created on: 2019Äê9ÔÂ21ÈÕ
 *      Author: sytu
 */
#include "rng.h"
#include "ppclib.h"
#include "sec_common.h"

#include "common.h"
#include "desc.h"
#include "jr.h"
#include "sec.h"
#include "desc_constr.h"
#include "jobdesc.h"

#define FREQUENCY_DIV					0
#define CPU_MODE						1		//if 0: SEC_MODE
#define EXT_WRNG						1
#define BYPASS							1		//wrng need bypass

//SEC MODE
rng_impl_init()
{
	*(unsigned int *)(SEC_ADDR(0) + 0x600) = 0x0;			//0x20 -- CPU,  0 -- SEC
	*(unsigned int *)(SEC_ADDR(0) + 0x74c) = 0xf;			//TRNG_CLKCRL
	*(unsigned int *)(SEC_ADDR(0) + 0x234) = 0x5500;		//TRNG_CLKCRL
	*(unsigned int *)(SEC_ADDR(0) + 0x700) = 0x040f0100;	//TRNG_CR
	*(unsigned int *)(SEC_ADDR(0) + 0x750) = 0xffffffff;	//RST
	*(unsigned int *)(SEC_ADDR(0) + 0x630) = 2;			//HASHCTRL
}

//CPU_MODE
rng_cpu_impl_init()
{
	*(unsigned int *)(SEC_ADDR(0) + 0x600) = 0x20;			//0x20 -- CPU,  0 -- SEC
	*(unsigned int *)(SEC_ADDR(0) + 0x74c) = 0xf;			//TRNG_CLKCRL
	*(unsigned int *)(SEC_ADDR(0) + 0x234) = 0x5500;		//TRNG_CLKCRL
	*(unsigned int *)(SEC_ADDR(0) + 0x700) = 0x040f0100;	//TRNG_CR
	*(unsigned int *)(SEC_ADDR(0) + 0x750) = 0xffffffff;	//RST
	*(unsigned int *)(SEC_ADDR(0) + 0x630) = 2;			//HASHCTRL
}

int drng_sec_init(void)
{
	unsigned int val;

	W32(RNG_IE_REG, 0);									//disable IE
	W32(RNG_AUTO_RQSTS_REG, 0xffffffff);
	W32(RNG_AUTO_AGE_REG, 0xffffffff);

	W32(RNG_MODE_REG, 0x08);							//if bit[3] is 1, 256-bit mode; otherwise 128-bit mode

	val = R32(RNG_SMODE_REG);
	W32(RNG_SMODE_REG, 0x80100);

	W32(RNG_ISTAT_REG, 0xf);
	val = R32(RNG_ISTAT_REG);

	W32(RNG_CTRL_REG, 0x02);							//0x2, Execute a random reseed
	val = R32(RNG_ISTAT_REG);
	while((val & 0x2) != 0x2)							//why not 0 is complete?
		val = R32(RNG_ISTAT_REG);

	W32(RNG_ISTAT_REG, 0x01);
	W32(RNG_CTRL_REG, 0x01);							//0x01, Generate a random number
	val = R32(RNG_ISTAT_REG);
	while((val & 0x1) != 0x1)							//why not 0 is complete?
		val = R32(RNG_ISTAT_REG);
	
	val = R32(DRNG_SEC_EN_REG);							//Digital RNG_SEC enable
	val |= (0x01<<7);
	W32(DRNG_SEC_EN_REG, val);

}

/*
 * backup
 */
void wrng_impl_init(void)
{
	int i;
	//unsigned int trng_data[8] = {0x0};
	unsigned int trng_data  = 0xe0340000, sec_idx = 0;

	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;		//the flush of internal RNG source, WRNG no effect whether set this

#if FREQUENCY_DIV
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0x8000031f;		//b is the third pat to catch data, a is the second pat, 9 is the first pat, 8 is the 0 pat
#else
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0x8000030f;		//the bit 4 is enable RNG DIV-frequency of 1/4; if clear bit 4, then RNG equal the internal frequency
#endif

#if CPU_MODE		//CPU-mode
//	*(unsigned int *)0xe020f028 = 0xf;								//for ic-elf use, no need here, is it?
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x20;			//0x20 -- CPU,  0 -- SEC

#if EXT_WRNG
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x00000130;		//close internal RNG by clear the 40f
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x00018000;		//CPU-mode the same whther bypass or no-bypass
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80018000;		//external WRNG,bypass
#else
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x000f0130;		//903T evb must open the 0xf<<16
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x040f0130;		//bit28 can't be 1, should clr it, internal RNG need open 0x040f<<16
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0;
#endif

#else		//SEC-mode
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x00;			//0x20 -- CPU,  0 -- SEC

#if EXT_WRNG
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x00000130;		//disable internal RNG, and enable the prng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x40000130;		//disable internal RNG, and enable the drng
#if BYPASS
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80018000;		//external WRNG,bypass
#else
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;				//hash-enable, if not bypass, need hash-SM3
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x00018000;		//not bypass SM3
#endif
#else
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x04000100;		//bit30-31 influence SEC-mode, clr bit30 become 1--prng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x44000130;		//bit30-31 influence SEC-mode, set bit30 become 1--drng
#if BYPASS
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80000000;		//internal RNG,bypass, rise-edge
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0xC0000000;		//internal RNG,bypass, fall-edge
#else
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;				//hash-enable, if not bypass, need hash-SM3
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0;				//not bypass SM3
#endif
#endif

#endif

	//the former RNG of internal and no-bypass FPGA RNG
/*		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0xf;
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x040f0100;
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;
*/


	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x234) = 0x5500;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x000f0100;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x40000100;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x01018000;		//0x010f8000
	//*(unsigned int *)0xe0220460 = 0xf;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;
/*
	while(1)
	{
		while((*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600)&0x1)==0x0)
		{

		}

		for(i=0;i<8;i++)
		{
			*(unsigned int *)(trng_data+i*4) = *(unsigned int *)(0xe0100640 + i*4);
		}
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x21;
		trng_data+=32;
		if(trng_data == 0xe0341000)
		{
			trng_data = 0xe0340000;
		}
	}*/
}


void wrng_impl_init_CPU(void)
{
	int i;
	unsigned int sec_idx = 0;

	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;		//the flush of internal RNG source, WRNG no effect whether set this
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0x8000030f;		//the bit 4 is enable RNG DIV-frequency of 1/4; if clear bit 4, then RNG equal the internal frequency
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x20;			//0x20 -- CPU,  0 -- SEC
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x00000130;		//close internal RNG by clear the 40f
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x00018000;		//CPU-mode the same whther bypass or no-bypass
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80018000;		//external WRNG,bypass. but CPU-mode the same whther bypass or no-bypass
}

void wrng_impl_init_SEC_bypass(void)
{
	int i;
	unsigned int sec_idx = 0;
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;		//the flush of internal RNG source, WRNG no effect whether set this
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0x8000031f;		//the bit 4 is enable RNG DIV-frequency of 1/4; if clear bit 4, then RNG equal the internal frequency
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x00;			//0x20 -- CPU,  0 -- SEC

//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x00000130;		//disable internal RNG, and enable the prng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x40000130;		//disable internal RNG, and enable the drng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80018000;		//external WRNG,bypass
}

void wrng_impl_init_SEC_no_bypass(void)
{
	int i;
	unsigned int sec_idx = 0;
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;		//the flush of internal RNG source, WRNG no effect whether set this
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0x8000031f;		//the bit 4 is enable RNG DIV-frequency of 1/4; if clear bit 4, then RNG equal the internal frequency
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x00;			//0x20 -- CPU,  0 -- SEC

//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x00000130;		//disable internal RNG, and enable the prng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x40000130;		//disable internal RNG, and enable the drng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;				//hash-enable, if not bypass, need hash-SM3
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x00018000;		//not bypass SM3
}

void rng_impl_init_rng_test(void)
{
	int i;
	//unsigned int trng_data[8] = {0x0};
	unsigned int trng_data  = 0xe0340000, sec_idx = 0;

	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;		//the flush of internal RNG source, WRNG no effect whether set this

#if FREQUENCY_DIV
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0x8000031f;		//b is the third pat to catch data, a is the second pat, 9 is the first pat, 8 is the 0 pat
#else
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0x8000030f;		//the bit 4 is enable RNG DIV-frequency of 1/4; if clear bit 4, then RNG equal the internal frequency
#endif

#if CPU_MODE		//CPU-mode
	*(unsigned int *)0xe020f028 = 0xf;								//for ic-elf use, no need here, is it?
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x20;			//0x20 -- CPU,  0 -- SEC

#if EXT_WRNG
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x00000130;		//close internal RNG by clear the 40f
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x00018000;		//CPU-mode the same whther bypass or no-bypass
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80018000;		//external WRNG,bypass
#else
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x000f0130;		//903T evb must open the 0xf<<16
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x040f0130;		//bit28 can't be 1, should clr it, internal RNG need open 0x040f<<16
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0;
#endif

#else		//SEC-mode
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x00;			//0x20 -- CPU,  0 -- SEC

#if EXT_WRNG
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x00000130;		//disable internal RNG, and enable the prng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x40000130;		//disable internal RNG, and enable the drng
#if BYPASS
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80018000;		//external WRNG,bypass
#else
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;				//hash-enable, if not bypass, need hash-SM3
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x00018000;		//not bypass SM3
#endif
#else
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x04000100;		//bit30-31 influence SEC-mode, clr bit30 become 1--prng
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x44000130;		//bit30-31 influence SEC-mode, set bit30 become 1--drng
#if BYPASS
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x80000000;		//internal RNG,bypass, rise-edge
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0xC0000000;		//internal RNG,bypass, fall-edge
#else
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;				//hash-enable, if not bypass, need hash-SM3
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0;				//not bypass SM3
#endif
#endif

#endif

	//the former RNG of internal and no-bypass FPGA RNG
/*		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0xf;
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x040f0100;
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;
*/


	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x234) = 0x5500;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x000f0100;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x40000100;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;
//	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x748) = 0x01018000;		//0x010f8000
	//*(unsigned int *)0xe0220460 = 0xf;
	//*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;
/*
	while(1)
	{
		while((*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600)&0x1)==0x0)
		{

		}

		for(i=0;i<8;i++)
		{
			*(unsigned int *)(trng_data+i*4) = *(unsigned int *)(0xe0100640 + i*4);
		}
		*(unsigned int *)(SEC_ADDR(sec_idx) + 0x600) = 0x21;
		trng_data+=32;
		if(trng_data == 0xe0341000)
		{
			trng_data = 0xe0340000;
		}
	}*/
}

int caam_trng_gen(unsigned char *to, unsigned int flen)
{
    /* begin of desc */

	uint32_t trng_desc[64];
	int ret;
	unsigned int *jump_cmd_now;

	init_job_desc(trng_desc, START_INDEX);
	
#if 1	//DRNG or ARNG	
if(flen>0x10){
	append_cmd(trng_desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM| LDST_SRCDST_WORD_RNGDATASZ_REG |4);
	append_cmd(trng_desc, 16);	//here the size of RNG should same with the size of data in the following fifo_store

//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(trng_desc, virt_to_phys((void *)to), 16, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);

	jump_cmd_now = append_jump(trng_desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(trng_desc, jump_cmd_now);

	append_cmd(trng_desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM| LDST_SRCDST_WORD_RNGDATASZ_REG |4);
	append_cmd(trng_desc, flen-16);	//here the size of RNG should same with the size of data in the following fifo_store
	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(trng_desc, virt_to_phys((void *)to+16), flen-16, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);
}else{
	append_cmd(trng_desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM| LDST_SRCDST_WORD_RNGDATASZ_REG |4);
	append_cmd(trng_desc, flen);	//here the size of RNG should same with the size of data in the following fifo_store

	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(trng_desc, virt_to_phys((void *)to), flen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);
}
#else	//only ARNG
	append_cmd(trng_desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM| LDST_SRCDST_WORD_RNGDATASZ_REG |4);
	append_cmd(trng_desc, flen);	//here the size of RNG should same with the size of data in the following fifo_store

	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(trng_desc, virt_to_phys((void *)to), flen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);

#endif
	ret = run_descriptor_jr_sec0(trng_desc);

	return ret;
}

int caam_trng_gen_internal_test(unsigned char *to, unsigned char *to2, unsigned char *to3, unsigned int flen)
{
    /* begin of desc */

	uint32_t trng_desc[64];
	int ret;
	unsigned int *jump_cmd_now;

	init_job_desc(trng_desc, START_INDEX);
	append_operation(trng_desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
//	append_load(desc, virt_to_phys(rng_seed), 32, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);

	append_cmd(trng_desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM| LDST_SRCDST_WORD_RNGDATASZ_REG |4);
	append_cmd(trng_desc, flen/2);	//here the size of RNG should same with the size of data in the following fifo_store

//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);

	append_fifo_store(trng_desc, virt_to_phys((void *)to3+flen/2), flen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);

	jump_cmd_now = append_jump(trng_desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(trng_desc, jump_cmd_now);



	append_cmd(trng_desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM| LDST_SRCDST_WORD_RNGDATASZ_REG |4);
	append_cmd(trng_desc, flen);	//here the size of RNG should same with the size of data in the following fifo_store

//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);

	append_fifo_store(trng_desc, virt_to_phys((void *)to), flen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);

	jump_cmd_now = append_jump(trng_desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(trng_desc, jump_cmd_now);

//add for delay
	append_fifo_load(trng_desc, virt_to_phys((void *)to), flen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(trng_desc, virt_to_phys((void *)to3), flen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(trng_desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(trng_desc, virt_to_phys((void *)to3), flen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

//time 2
/*	append_fifo_load(trng_desc, virt_to_phys((void *)to), flen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(trng_desc, virt_to_phys((void *)to3), flen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(trng_desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	append_fifo_store(trng_desc, virt_to_phys((void *)to), flen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
*/

	append_cmd(trng_desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM| LDST_SRCDST_WORD_RNGDATASZ_REG |4);
	append_cmd(trng_desc, flen);	//here the size of RNG should same with the size of data in the following fifo_store

//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
//	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_operation(trng_desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);

	append_fifo_store(trng_desc, virt_to_phys((void *)to2), flen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);

	ret = run_descriptor_jr2(trng_desc);

	return ret;
}

unsigned int sm_get_random_CPU(unsigned char *pbuf, unsigned int datalen)
{
	unsigned int i, j, m, tmp, ext;
	unsigned char ch;

	//CPU-mode
	for(j=0;j<datalen;j++){
		while((*(unsigned int *)(SEC_ADDR(0) + 0x600)&0x1)==0x0);
		//normal catch every int
		pbuf[j] = *(unsigned char *)(SEC_ADDR(0) + 0x604 + j);			//903T-fixed¡¢905T
		*(unsigned int *)(SEC_ADDR(0) + 0x600) = 0x21;
	}
}


unsigned int sm_get_random_SEC(unsigned char *pbuf, unsigned int datalen)
{
	//SEC-mode
	caam_trng_gen((unsigned char *)pbuf, datalen);		
}

void analog_rng_test_CPU(void)
{
	//add for test the same word, CPU mode get internal RNG
	unsigned char *buf, *buf2;
	unsigned int count, data_num, m, i, data_size;

	rng_cpu_impl_init();						//analogy rng cpu mode set
	
	/*
	 * scan from 4byte to 256(drng) or 65536
	 */
	data_size=0x10000;
	for(data_num=0x1; data_num<data_size; data_num++)
	{
		buf = (unsigned char *)malloc(data_num);
		memset(buf, 0, data_num);

		memset(buf, 0, data_num);
		sm_get_random_CPU(buf, data_num);

		PSPRINTF("the First data:\n");
		for(count=0; count<data_num; count++)
			PSPRINTF("%x",buf[count]);
		PSPRINTF("\n");
		free(buf);
		PSPRINTF("######data_num is 0x%x#######\n",data_num);
	}
}


void analog_rng_test_SEC(void)
{
	//add for test the same word, CPU mode get internal RNG
	unsigned char *buf, *buf2;
	unsigned int count, data_num, m, i, data_size;

	rng_impl_init();							//analogy rng sec mode set

	/*
	 * scan from 4byte to 256(drng) or 65536
	 */
	data_size=0x10000;
	for(data_num=0x1; data_num<data_size; data_num++)
//	for(data_num=0x1; data_num<data_size; data_num)
	{
		buf = (unsigned char *)malloc(data_num);
		memset(buf, 0, data_num);

		memset(buf, 0, data_num);
		sm_get_random_SEC(buf, data_num);

		PSPRINTF("the first data: %x \n", *(unsigned int *)buf);
		
		free(buf);
		PSPRINTF("######data_num is 0x%x#######\n",data_num);
	}
}

void wrng_test(void)
{

	wrng_impl_init();

#if 1
	//add for test the same word, CPU mode get internal RNG
	unsigned char buf[32], buf2[32], buf3[32], m, count=0;

	sm_get_random_SEC(buf, 32);
	if(*(volatile unsigned char *)buf == 0xff)
		while(1);

	while(1)
	{
		sm_get_random_SEC(buf2, 32);

		for(m=0;m<8;m++)
		{
			if(	*(volatile unsigned int *)buf == *((volatile unsigned int *)buf2+m) )
		//	if(	0 == *((volatile unsigned int *)buf2+m) )
				while(1);
		}
	//	memset(buf3, 0, 64);
	//	caam_trng_gen_internal_test(buf, buf2, buf3, 32);		//just for internal RNG test
	//	if(	*(unsigned int *)buf == *(unsigned int *)buf2 )
	//		while(1);

		PSPRINTF("#### count is %d ####\n", count++);
		count++;
	}
#endif
}

void wrng_test_CPU(void)
{
	unsigned char *buf, *buf2, m, i,count=0;

	wrng_impl_init_CPU();

	buf = (unsigned char *)malloc(32);
	sm_get_random_CPU(buf, 32);
	if(*(volatile unsigned int *)buf == 0xffffffff)
		while(1);

	for(i=1; i<32; i++)
	{
		buf2 = (unsigned char *)malloc(i);
		sm_get_random_CPU(buf2, i);

		for(m=0;m<8;m++)
		{
			if(	*(volatile unsigned int *)buf == *((volatile unsigned int *)buf2+m) )
				while(1);
		}
		PSPRINTF("#### count is %d ####\n", count++);
		
		free(buf2);
	}
	
	free(buf);
}

void wrng_test_SEC_bypass(void)
{
	unsigned char *buf, *buf2;
	unsigned int m, i, size, count;

	wrng_impl_init_SEC_bypass();
	
	size = 0x1000;
	count = 1;
	
	buf = (unsigned char *)malloc(size);
	sm_get_random_SEC(buf, size);
	if(*(volatile unsigned int *)buf == 0xffffffff)
		while(1);

	for(i=1; i<size; i++)
	{
		buf2 = (unsigned char *)malloc(i);
		sm_get_random_SEC(buf2, i);

		for(m=0;m<8;m++)
		{
			if(	*(volatile unsigned int *)buf == *((volatile unsigned int *)buf2+m) )
				while(1);
		}
		PSPRINTF("#### count is %d ####\n", count++);
		
		free(buf2);
	}
	
	free(buf);
	
}


void wrng_test_SEC_no_bypass(void)
{
	unsigned char *buf, *buf2;
	unsigned int m, i, size, count;

	wrng_impl_init_SEC_no_bypass();

	size = 0x1000;
	count = 1;
	
	buf = (unsigned char *)malloc(size);
	sm_get_random_SEC(buf, size);
	if(*(volatile unsigned int *)buf == 0xffffffff)
		while(1);

	for(i=1; i<size; i++)
	{
		buf2 = (unsigned char *)malloc(i);
		sm_get_random_SEC(buf2, i);

		for(m=0;m<8;m++)
		{
			if(	*(volatile unsigned int *)buf == *((volatile unsigned int *)buf2+m) )
				while(1);
		}
		PSPRINTF("#### count is %d ####\n", count++);
		
		free(buf2);
	}
	
	free(buf);

}

int digital_rng_test_CPU(void)
{
	unsigned int rddata32;
	unsigned int i, j, rand_count;
	unsigned int data[8];

	W32(RNG_IE_REG, 0);							//disable IE
	W32(RNG_AUTO_RQSTS_REG, 0xffffffff);
	W32(RNG_AUTO_AGE_REG, 0xffffffff);

	W32(RNG_MODE_REG, 0x08);					//if bit[3] is 1, 256-bit mode; otherwise 128-bit mode

	rddata32 = R32(RNG_SMODE_REG);
	W32(RNG_SMODE_REG, 0x80100);

	W32(RNG_ISTAT_REG, 0xffffffff);
	rddata32 = R32(RNG_ISTAT_REG);

	W32(RNG_CTRL_REG, 0x02);					//0x2, Execute a random reseed
	rddata32 = R32(RNG_ISTAT_REG);
	while((rddata32 & 0x2) != 0x2)				//why not 0 is complete?
		rddata32 = R32(RNG_ISTAT_REG);

	W8(DRNG_SEC_EN_REG, 0x80);					//drng enable

	rand_count = 1000;

	for(i=0; i<rand_count; i++)
	{
		W32(RNG_ISTAT_REG, 0x01);
		W32(RNG_CTRL_REG, 0x01);				//0x01, Generate a random number
		rddata32 = R32(RNG_ISTAT_REG);
		while((rddata32 & 0x1) != 0x1)			//why not 0 is complete?
			rddata32 = R32(RNG_ISTAT_REG);

		data[0] = R32(RNG_RAND0_REG);
		data[1]  = R32(RNG_RAND1_REG);
		data[2]  = R32(RNG_RAND2_REG);
		data[3]  = R32(RNG_RAND3_REG);
		data[4]  = R32(RNG_RAND4_REG);
		data[5]  = R32(RNG_RAND5_REG);
		data[6]  = R32(RNG_RAND6_REG);
		data[7]  = R32(RNG_RAND7_REG);

		PSPRINTF("######## the rand data is:");
		for(j=0; j<8; j++)
			PSPRINTF("0x%x\r\n", data[j]);
		PSPRINTF("########\n");
	}
}//

void digital_rng_test_SEC(void)
{
	unsigned char *buf, *buf2;
	unsigned int size, m;
	
	drng_sec_init();

	buf = (unsigned char *)malloc(0x6000);
	sm_get_random_SEC(buf, 0x6000);
	
	for(size=1; size<0x6000; size++){
		buf2 = (unsigned char *)malloc(size);
		memset(buf2, 0, size);
		sm_get_random_SEC(buf2, size);
	
		for(m=0;m<size/16;m++)
		{
			if(	*(volatile unsigned long long *)buf == *((volatile unsigned long long *)buf2+m) )
				while(1);
		}

		free(buf2);
		PSPRINTF("#### size is %x,  *buf2 is 0x%x####\n", size, *(volatile unsigned long long *)buf2);
	}
	free(buf);
}

