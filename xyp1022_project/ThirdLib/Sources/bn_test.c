#include "bn_lcl.h"
#include "ec_lcl.h"
#include "e500mc_asm.h"
#include "e500mc.h"
#include "desc_constr.h"
#include "jobdesc.h"
//#include "as_archppc.h"
//#include "sm2.h"

#define BN_MOD_ADD_TEST						0					//1
#define BN_MOD_SUB_TEST						0					//1
#define BN_MOD_MUL_TEST						0					//1
#define BN_MOD_INV_TEST						0					//1
#define BN_MOD_EXP_TEST						0					//1
#define BN_MOD_MUL_MONT_TEST				0					//1
#define BN_ECC_MOD_ADD_TEST					0					//1
#define BN_ECC_MOD_MUL_TEST					0					//0	ecc_mod_mul_mont error
#define BN_ECC_MOD_DBL_TEST					0					//1
#define BN_ECC_F2M_ADD_TEST					0					//1
#define BN_ECC_F2M_DBL_TEST					0					//1
#define BN_ECC_F2M_MUL_TEST					0					//1
#define BN_PKHA_FP_MOD_TEST					0					//1
#define BN_PKHA_FP_MOD_ADD_TEST				0					//1
#define BN_PKHA_FP_MOD_SUB_TEST				0					//0
#define BN_PKHA_FP_MOD_MUL_TEST				0					//1
#define BN_PKHA_FP_MOD_EXP_TEST				0					//1
#define BN_PKHA_FP_MOD_EXP_MONT_TEST		0					//1
#define BN_PKHA_FP_MOD_EXP_TEQ_TEST			0					//1
#define BN_PKHA_FP_MOD_INV_TEST				0					//1
#define BN_PKHA_FP_MOD_GCD_TEST				0					//1
#define BN_PKHA_FP_MOD_R2_TEST				0					//1
#define BN_PKHA_FP_MOD_SML_EXP_TEST			0					//no this
#define BN_PKHA_F2M_ADD_TEST				0					//1
#define	BN_PKHA_F2M_MUL_TEST				0					//1
#define	BN_PKHA_F2M_MUL_MONT_TEST			0					//1
#define BN_PKHA_F2M_EXP_TEST				0					//1
#define BN_PKHA_F2M_EXP_TEQ_TEST			0					//1
#define BN_PKHA_F2M_MOD_TEST				0					//1
#define BN_PKHA_F2M_INV_TEST				0					//1
#define BN_PKHA_F2M_GCD_TEST				0					//1
#define BN_PKHA_F2M_R2_TEST					0					//1
#define BN_PKHA_PRIME_TEST					0					//1
#define BN_PKHA_SHIFT_TEST					0					//1
#define BN_PKHA_BASIS_CONV_TEST				0					//1
#define BN_PKHA_MASS_RAND_TEST				0					//1
#define BN_PKHA_FP256_SM2_TEST				0					//0
#define BN_PKHA_F2M257_SM2_TEST				0					//0
#define PKHA_FP256_SM2_TEST					0					//1
#define PKHA_F2M257_SM2_TEST				0					//1
#define PKHA_SM2_KEY_EXCHANGE_TEST			0					//1
#define PKHA_SM9_TEST						0					//1
#define PKHA_ERROR_TEST						0					//1
#define PKHA_RSA_END_BIG_TEST				1					//1
#define PKHA_RSA_END_LIT_TEST				0					//1
/*
unsigned int one[16]={
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
};
*/
unsigned char one = 1;

unsigned int A[16]={
		0x1c85d9ee,
		0xdd858d21,
		0x3f004ccd,
		0x9e854052,
		0xa07b0c7a,
		0xfe0a33d7,
		0xa270d8a3,
		0x5b995b35,
		0x46d77d6e,
		0x14c1ddc6,
		0x32159fa6,
		0xe2ac3e20,
		0x4f696186,
		0x9342dc99,
		0xbc2684ed,
		0xd71c57ab
};

unsigned int B[16]={
		0x6edc0182,
		0xf56e3897,
		0xe30cfe4e,
		0x9415b277,
		0x3e07c094,
		0xed783221,
		0xb0796845,
		0x9a108a3e,
		0x98ed7c8d,
		0x4eda6f04,
		0x8292e939,
		0xf216ad84,
		0xd08b3f1f,
		0x0dd751d0,
		0x5f662d20,
		0x248b7d4b
};

unsigned int N[16]={
		0x800fffff,
		0xffffffff,
		0xffffffff,
		0xfffffffe,
		0xffffffff,
		0xffffffff,
		0xffffffff,
		0xffffffff,
		0xaaab3980,
		0x3df5099c,
		0x5f9056e8,
		0xc895283e,
		0xd7d71623,
		0x97e62a5e,
		0xb3fb73b1,
		0x2d0d2677
};
#if BN_MOD_ADD_TEST
unsigned int C[16]={
		0x0b51db71,
		0xd2f3c5b9,
		0x220d4b1c,
		0x329af2ca,
		0xde82cd0f,
		0xeb8265f9,
		0x52ea40e8,
		0xf5a9e574,
		0x3519c07b,
		0x25a7432e,
		0x551831f8,
		0x0c2dc366,
		0x481d8a82,
		0x0934040b,
		0x67913e5c,
		0xce9aae7f
};

void bn_mod_add_test()
{
	BIGNUM *bn_a = BN_bin2bn((const unsigned char *)A, 16*4, NULL);
	BIGNUM *bn_b = BN_bin2bn((const unsigned char *)B, 16*4, NULL);
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)N, 16*4, NULL);
	BIGNUM *bn_c = BN_bin2bn((const unsigned char *)C, 16*4, NULL);
	BIGNUM *bn_r = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=64, i;
	dma_addr_t bignum_addr, dst_addr;

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size);

	dst_addr = virt_to_phys(dst);
//	bignum_addr = virt_to_phys(bignum);

	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	bignum_addr = virt_to_phys(bn_b->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);

	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != bn_c->d[i])
			COND_TRAP(31, 0, 0);
	}

	BN_mod_add(bn_r, bn_a, bn_b, bn_n, ctx);

	if(BN_cmp(bn_r, bn_c) != 0)
		COND_TRAP(31, 0, 0);

//	free(bignum_raw);
	free(dst);
	free(desc);
//	while(1);
	BN_free(bn_a);
	BN_free(bn_b);
	BN_free(bn_c);
	BN_free(bn_n);
	BN_free(bn_r);
	BN_CTX_free(ctx);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_MOD_SUB_TEST
unsigned int C[16] = {
		0x2db9d86b,
		0xe8175489,
		0x5bf34e7f,
		0x0a6f8dda,
		0x62734be6,
		0x109201b5,
		0xf1f7705d,
		0xc188d0f6,
		0x58953a61,
		0x03dc785e,
		0x0f130d55,
		0xb92ab8da,
		0x56b5388b,
		0x1d51b528,
		0x10bbcb7e,
		0xdf9e00d7
};

void bn_mod_sub_test()
{
	BIGNUM *bn_a = BN_bin2bn((const unsigned char *)A, 16*4, NULL);
	BIGNUM *bn_b = BN_bin2bn((const unsigned char *)B, 16*4, NULL);
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)N, 16*4, NULL);
	BIGNUM *bn_c = BN_bin2bn((const unsigned char *)C, 16*4, NULL);
	BIGNUM *bn_r = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=64, i;
	dma_addr_t bignum_addr, dst_addr;

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size);

	dst_addr = virt_to_phys(dst);
//	bignum_addr = virt_to_phys(bignum);

	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	bignum_addr = virt_to_phys(bn_b->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);

	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != bn_c->d[i])
			COND_TRAP(31, 0, 0);
	}

	BN_mod_sub(bn_r, bn_a, bn_b, bn_n, ctx);

	if(BN_cmp(bn_r, bn_c) != 0)
		COND_TRAP(31, 0, 0);

	//	free(bignum_raw);
		free(dst);
		free(desc);
	//	while(1);
		BN_free(bn_a);
		BN_free(bn_b);
		BN_free(bn_c);
		BN_free(bn_n);
		BN_free(bn_r);
		BN_CTX_free(ctx);

		COND_TRAP(31, 0, 0);
}
#endif

#if BN_MOD_MUL_TEST || BN_MOD_MUL_MONT_TEST
unsigned int C[16] = {
		0x6ad3ea63,
		0xf6694bdd,
		0x92370103,
		0x29eb0858,
		0x6977d5ac,
		0x49142a06,
		0x303c11d5,
		0xad4e4dac,
		0x480e4ea3,
		0x2962320f,
		0x486df057,
		0x1e142e15,
		0x4792b7b1,
		0x825311d5,
		0x71a28ca4,
		0xcbbd660f
};
#endif

#if BN_MOD_MUL_TEST
void bn_mod_mul_test()
{
	BIGNUM *bn_a = BN_bin2bn((const unsigned char *)A, 16*4, NULL);
	BIGNUM *bn_b = BN_bin2bn((const unsigned char *)B, 16*4, NULL);
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)N, 16*4, NULL);
	BIGNUM *bn_c = BN_bin2bn((const unsigned char *)C, 16*4, NULL);
	BIGNUM *bn_r = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=64, i;
	dma_addr_t bignum_addr, dst_addr;

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size);

	dst_addr = virt_to_phys(dst);
//	bignum_addr = virt_to_phys(bignum);

	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	bignum_addr = virt_to_phys(bn_b->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);

	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != bn_c->d[i])
			COND_TRAP(31, 0, 0);
	}

	BN_mod_mul(bn_r, bn_a, bn_b, bn_n, ctx);

	if(BN_cmp(bn_r, bn_c) != 0)
		COND_TRAP(31, 0, 0);

	free(dst);
	free(desc);
//	while(1);
	BN_free(bn_a);
	BN_free(bn_b);
	BN_free(bn_c);
	BN_free(bn_n);
	BN_free(bn_r);
	BN_CTX_free(ctx);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_MOD_MUL_MONT_TEST
void bn_mod_mul_mont_test()
{
	BIGNUM *bn_a = BN_bin2bn((const unsigned char *)A, 16*4, NULL);
	BIGNUM *bn_b = BN_bin2bn((const unsigned char *)B, 16*4, NULL);
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)N, 16*4, NULL);
	BIGNUM *bn_c = BN_bin2bn((const unsigned char *)C, 16*4, NULL);
	BIGNUM *bn_r = BN_new();
	BIGNUM *mont_a = BN_new();
	BIGNUM *mont_b = BN_new();
	BIGNUM *mont_c = BN_new();
	BN_CTX *ctx = BN_CTX_new();
	BN_MONT_CTX *mont = BN_MONT_CTX_new();
//	BIGNUM *bn_one = BN_bin2bn((const unsigned char *)one, 1, NULL);
	BIGNUM *bn_one = BN_new();

	memset(bn_one, 0, sizeof(BIGNUM));
	bn_one->d = (BN_ULONG *)malloc(16*4);
	memset(bn_one->d, 0, 64);
	bn_one->d[0] = 1;
	bn_one->dmax = 16;
	bn_one->top = 16;

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=64, i;
	dma_addr_t bignum_addr, dst_addr;

	BN_MONT_CTX_set(mont, bn_n, ctx);

//	BN_nnmod(bn_a, bn_a, bn_n, ctx);
//	BN_nnmod(bn_b, bn_b, bn_n, ctx);
	BN_to_montgomery(mont_a, bn_a, mont, ctx);
	BN_to_montgomery(mont_b, bn_b, mont, ctx);
	BN_mod_mul_montgomery(mont_c, mont_a, mont_b, mont, ctx);
	BN_from_montgomery(bn_r, mont_c, mont, ctx);

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size*2);

	dst_addr = virt_to_phys(dst);
//	bignum_addr = virt_to_phys(bignum);
#if 1
/*
	desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
	bignum_addr = virt_to_phys(bn_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	bignum_addr = virt_to_phys(mont_b->d);
//	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT | OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
*/
	pkha_fp_mod_mul_im_om(bn_a->d, bn_a->top*4, mont->RR.d, mont->RR.top*4, bn_n->d, bn_n->top*4, dst);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != mont_a->d[i])
			COND_TRAP(31, 0, 0);
	}
/*
	desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
	bignum_addr = virt_to_phys(bn_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	bignum_addr = virt_to_phys(mont_b->d);
//	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT | OP_ALG_PKMODE_MOD_IN_MONTY);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
*/
	pkha_fp_mod_mul_im(bn_a->d, bn_a->top*4, mont->RR.d, mont->RR.top*4, bn_n->d, bn_n->top*4, dst);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != bn_a->d[i])
			COND_TRAP(31, 0, 0);
	}
#endif
#if 1
/*
	desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
	bignum_addr = virt_to_phys(mont_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	bignum_addr = virt_to_phys(bn_one->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
//	append_fifo_load_as_imm(desc, bn_one->d, 4, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT | OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
*/
	pkha_fp_mod_mul_im_om(mont_a->d, mont_a->top*4, bn_one->d, bn_one->top*4, bn_n->d, bn_n->top*4, dst);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != bn_a->d[i])
			COND_TRAP(31, 0, 0);
	}
#endif
#if 1
//	desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
	bignum_addr = virt_to_phys(bn_b->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	bignum_addr = virt_to_phys(mont_b->d);
//	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT | OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY);

	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != mont_b->d[i])
			COND_TRAP(31, 0, 0);
	}
#endif
#if 1
/*
	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(mont_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	bignum_addr = virt_to_phys(mont_b->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT | OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY);

	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
*/
	pkha_fp_mod_mul_im_om(mont_a->d, mont_a->top*4, mont_b->d, mont_b->top*4, bn_n->d, bn_n->top*4, dst);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != mont_c->d[i])
			COND_TRAP(31, 0, 0);
	}

	pkha_fp_mod_mul_im(mont_a->d, mont_a->top*4, mont_b->d, mont_b->top*4, bn_n->d, bn_n->top*4, dst);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != bn_r->d[i])
			COND_TRAP(31, 0, 0);
	}
#endif
	if(BN_cmp(bn_r, bn_c) != 0)
		COND_TRAP(31, 0, 0);

	free(desc);
	free(dst);
	BN_free(bn_a);
	BN_free(bn_b);
	BN_free(bn_c);
	BN_free(bn_n);
	BN_free(bn_r);
	BN_free(mont_a);
	BN_free(mont_b);
	BN_free(mont_c);
	BN_CTX_free(ctx);
	BN_MONT_CTX_free(mont);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_MOD_INV_TEST
unsigned int C[16] = {
		0x1ff666b4,
		0x73626b50,
		0xb621a81c,
		0x9b2ff124,
		0x5a672683,
		0xa6dbc99e,
		0x6a531037,
		0x5f288b01,
		0x14302bf5,
		0x24c87714,
		0xad358e12,
		0xdb28a0bc,
		0x2e7544c7,
		0x472c182c,
		0xc388ecec,
		0x7e6c265f
};

void bn_mod_inv_test()
{
	BIGNUM *bn_a = BN_bin2bn((const unsigned char *)A, 16*4, NULL);
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)N, 16*4, NULL);
	BIGNUM *bn_c = BN_bin2bn((const unsigned char *)C, 16*4, NULL);
	BIGNUM *bn_r = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=64, i;
	dma_addr_t bignum_addr, dst_addr;

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
	dst = (uint32_t *)malloc(bignum_size);

	dst_addr = virt_to_phys(dst);

	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);

	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != bn_c->d[i])
			COND_TRAP(31, 0, 0);
	}

	BN_mod_inverse(bn_r, bn_a, bn_n, ctx);

	if(BN_cmp(bn_r, bn_c) != 0)
		COND_TRAP(31, 0, 0);

	free(dst);
	free(desc);
	BN_free(bn_a);
	BN_free(bn_c);
	BN_free(bn_n);
	BN_free(bn_r);
	BN_CTX_free(ctx);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_MOD_EXP_TEST
unsigned char A_exp[5] = {
		0x80, 0x00, 0x00, 0x00, 0x00
};

unsigned char P_exp = 0x20;

unsigned char N_exp[5] = {
		0x00, 0x20, 0xC8, 0xEB, 0xFF
};

unsigned char C_exp[5] = {
		0x00, 0x1D, 0xA5, 0x9C, 0xD1
};

void bn_mod_exp_test()
{
	BIGNUM *bn_a = BN_bin2bn((const unsigned char *)A_exp, 5, NULL);
	BIGNUM *bn_p = BN_bin2bn((const unsigned char *)&P_exp, 1, NULL);
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)N_exp, 5, NULL);
	BIGNUM *bn_c = BN_bin2bn((const unsigned char *)C_exp, 5, NULL);
	BIGNUM *bn_r = BN_new();
	BN_CTX *ctx = BN_CTX_new();
#if 0
	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=5, i;
	dma_addr_t bignum_addr, dst_addr;

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size);

	dst_addr = virt_to_phys(dst);
//	bignum_addr = virt_to_phys(bignum);

	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, 5, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a->d);
	append_fifo_load(desc, bignum_addr, 5, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	bignum_addr = virt_to_phys(bn_p->d);
//	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_key(desc, bignum_addr, 1, KEY_DEST_CLASS_1 | KEY_NWB | KEY_DEST_PKHA_E);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);

	append_fifo_store(desc, dst_addr, 5, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);
	if(memcmp(dst, bn_c->d, 5))
		COND_TRAP(31, 0, 0);
#endif
	BN_mod_exp(bn_r, bn_a, bn_p, bn_n, ctx);

	if(BN_cmp(bn_r, bn_c) != 0)
		COND_TRAP(31, 0, 0);

#if 0
	free(dst);
	free(desc);
#endif
//	while(1);
	BN_free(bn_a);
	BN_free(bn_p);
	BN_free(bn_c);
	BN_free(bn_n);
	BN_free(bn_r);
	BN_CTX_free(ctx);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_ECC_MOD_ADD_TEST
unsigned int p[8] = {
		0x8542D69E,
		0x4C044F18,
		0xE8B92435,
		0xBF6FF7DE,
		0x45728391,
		0x5C45517D,
		0x722EDB8B,
		0x08F1DFC3
};
unsigned int a[8] = {
		0x787968B4,
		0xFA32C3FD,
		0x2417842E,
		0x73BBFEFF,
		0x2F3C848B,
		0x6831D7E0,
		0xEC65228B,
		0x3937E498
};

unsigned int b[8] = {
		0x63E4C6D3,
		0xB23B0C84,
		0x9CF84241,
		0x484BFE48,
		0xF61D59A5,
		0xB16BA06E,
		0x6E12D1DA,
		0x27C5249A
};

unsigned int px[8] = {
		0x421DEBD6,
		0x1B62EAB6,
		0x746434EB,
		0xC3CC315E,
		0x32220B3B,
		0xADD50BDC,
		0x4C4E6C14,
		0x7FEDD43D
};

unsigned int py[8] = {
		0x0680512B,
		0xCBB42C07,
		0xD47349D2,
		0x153B70C4,
		0xE5D7FDFC,
		0xBFA36EA1,
		0xA85841B9,
		0xE46E09A2
};

unsigned int qx[8] = {
		0x4665c2bc,
		0xfe0f5b4a,
		0xe030efc9,
		0x2d6b4bee,
		0x6c332dda,
		0x90a05d55,
		0x38b8c834,
		0xf27a294d
};

unsigned int qy[8] = {
		0x83a5fb8d,
		0xd7205a4f,
		0xb86aa9d3,
		0xc50bb6d1,
		0x5c31426d,
		0xc9c1ed10,
		0xbd738abd,
		0xdcb5d414
};

void bn_ecc_mod_add_test()
{
	BIGNUM *bn_a0 = BN_bin2bn((const unsigned char *)px, 8*4, NULL);
	BIGNUM *bn_a1 = BN_bin2bn((const unsigned char *)py, 8*4, NULL);
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)p, 8*4, NULL);
	BIGNUM *bn_a3 = BN_bin2bn((const unsigned char *)a, 8*4, NULL);
	BIGNUM *bn_b0= BN_bin2bn((const unsigned char *)b, 8*4, NULL);
	BIGNUM *bn_b1= BN_bin2bn((const unsigned char *)qx, 8*4, NULL);
	BIGNUM *bn_b2= BN_bin2bn((const unsigned char *)qy, 8*4, NULL);
//	BIGNUM *bn_c0 = BN_bin2bn((const unsigned char *)a, 8*4, NULL);
//	BIGNUM *bn_c1 = BN_bin2bn((const unsigned char *)b, 8*4, NULL);
	BIGNUM *bn_r2 = BN_new();
	BN_MONT_CTX *mont = BN_MONT_CTX_new();
//	BIGNUM *bn_r = BN_new();
	BN_CTX *ctx = BN_CTX_new();
    EC_GROUP *group = (EC_GROUP *)malloc(sizeof(EC_GROUP));
    EC_POINT *point1 = (EC_POINT *)malloc(sizeof(EC_POINT));
    EC_POINT *point2 = (EC_POINT *)malloc(sizeof(EC_POINT));
    EC_POINT *r = (EC_POINT *)malloc(sizeof(EC_POINT));
    BIGNUM *bn_one = BN_bin2bn((const unsigned char *)&one, 1, NULL);

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=32, i;
	dma_addr_t bignum_addr, dst_addr;

	memset(group, 0, sizeof(EC_GROUP));
	memset(point1, 0, sizeof(EC_POINT));
	memset(point2, 0, sizeof(EC_POINT));
	memset(r, 0, sizeof(EC_POINT));

	if(!BN_copy(&group->field, bn_n))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&group->a, bn_a3))
		COND_TRAP(31, 0, 0);

	if(!BN_copy(&point1->X, bn_a0))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point1->Y, bn_a1))
		COND_TRAP(31, 0, 0);
	point1->Z.d = (BN_ULONG *)malloc(4);
	point1->Z.d[0] = 1;
	point1->Z.dmax = 1;
	point1->Z.top = 1;
	point1->Z.neg = 0;
	point1->Z.flags = 0x1;
	point1->Z_is_one = 1;

	if(!BN_copy(&point2->X, bn_b1))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point2->Y, bn_b2))
		COND_TRAP(31, 0, 0);
	point2->Z.d = (BN_ULONG *)malloc(4);
	point2->Z.d[0] = 1;
	point2->Z.dmax = 1;
	point2->Z.top = 1;
	point2->Z.neg = 0;
	point2->Z.flags = 0x1;
	point2->Z_is_one = 1;

	if(!ec_GFp_simple_add(group, r, point1, point2, ctx))
		COND_TRAP(31, 0, 0);

	if(!ec_GFp_simple_make_affine(group, r, ctx))
		COND_TRAP(31, 0, 0);

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size*2);

//	bignum_addr = virt_to_phys(bignum);

	init_job_desc(desc, START_INDEX);
#if 1
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_b1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B1);
	bignum_addr = virt_to_phys(bn_b2->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B2);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_ADD);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#else
	if(!BN_MONT_CTX_set(mont, bn_n, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(bn_r2, &mont->RR))
		COND_TRAP(31, 0, 0);

	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_b1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B1);
	bignum_addr = virt_to_phys(bn_b2->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B2);
	bignum_addr = virt_to_phys(bn_r2->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B3);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_ADD | OP_ALG_PKMODE_MOD_R2_IN);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#endif
	run_descriptor_jr(desc, &jr_t);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] !=r->X.d[i])
			COND_TRAP(31, 0, 0);
	}
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i+bignum_size/4] != r->Y.d[i])
			COND_TRAP(31, 0, 0);
	}

	//	free(bignum_raw);
	free(dst);
	free(desc);
	//	while(1);
	BN_free(bn_a0);
	BN_free(bn_a1);
	BN_free(bn_a3);
	BN_free(bn_b0);
	BN_free(bn_b1);
	BN_free(bn_b2);
//	BN_free(bn_c0);
//	BN_free(bn_c1);
	BN_free(bn_n);
	BN_free(bn_r2);
	BN_MONT_CTX_free(mont);
	BN_CTX_free(ctx);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_ECC_MOD_MUL_TEST
unsigned int p[8] = {
		0x8542D69E,
		0x4C044F18,
		0xE8B92435,
		0xBF6FF7DE,
		0x45728391,
		0x5C45517D,
		0x722EDB8B,
		0x08F1DFC3
};
unsigned int a[8] = {
		0x787968B4,
		0xFA32C3FD,
		0x2417842E,
		0x73BBFEFF,
		0x2F3C848B,
		0x6831D7E0,
		0xEC65228B,
		0x3937E498
};

unsigned int b[8] = {
		0x63E4C6D3,
		0xB23B0C84,
		0x9CF84241,
		0x484BFE48,
		0xF61D59A5,
		0xB16BA06E,
		0x6E12D1DA,
		0x27C5249A
};

unsigned int gx[8] = {
		0x421DEBD6,
		0x1B62EAB6,
		0x746434EB,
		0xC3CC315E,
		0x32220B3B,
		0xADD50BDC,
		0x4C4E6C14,
		0x7FEDD43D
};

unsigned int gy[8] = {
		0x0680512B,
		0xCBB42C07,
		0xD47349D2,
		0x153B70C4,
		0xE5D7FDFC,
		0xBFA36EA1,
		0xA85841B9,
		0xE46E09A2
};

unsigned int k[8] = {
		0x6CB28D99,
		0x385C175C,
		0x94F94E93,
		0x4817663F,
		0xC176D925,
		0xDD72B727,
		0x260DBAAE,
		0x1FB2F96F
};

unsigned int rx[8] = {
		0x110FCDA5,
		0x7615705D,
		0x5E7B9324,
		0xAC4B856D,
		0x23E6D918,
		0x8B2AE477,
		0x59514657,
		0xCE25D112
};

unsigned int ry[8] = {
		0x1C65D68A,
		0x4A08601D,
		0xF24B431E,
		0x0CAB4EBE,
		0x084772B3,
		0x817E8581,
		0x1A8510B2,
		0xDF7ECA1A
};
void bn_ecc_mod_mul_test()
{
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)p, 8*4, NULL);
	BIGNUM *bn_e = BN_bin2bn((const unsigned char *)k, 8*4, NULL);
	BIGNUM *bn_a0 = BN_bin2bn((const unsigned char *)gx, 8*4, NULL);
	BIGNUM *bn_a1 = BN_bin2bn((const unsigned char *)gy, 8*4, NULL);
	BIGNUM *bn_a3= BN_bin2bn((const unsigned char *)a, 8*4, NULL);
	BIGNUM *bn_b0= BN_bin2bn((const unsigned char *)b, 8*4, NULL);
	BIGNUM *bn_b1= BN_bin2bn((const unsigned char *)rx, 8*4, NULL);
	BIGNUM *bn_b2 = BN_bin2bn((const unsigned char *)ry, 8*4, NULL);
	BIGNUM *bn_r2 = BN_new();
	BN_MONT_CTX *mont = BN_MONT_CTX_new();
	BN_CTX *ctx = BN_CTX_new();
    EC_GROUP *group = (EC_GROUP *)malloc(sizeof(EC_GROUP));
    EC_POINT *point = (EC_POINT *)malloc(sizeof(EC_POINT));
    EC_POINT *r = (EC_POINT *)malloc(sizeof(EC_POINT));
    const EC_POINT **points = malloc(sizeof(EC_POINT*));
    const BIGNUM **scalars = malloc(sizeof(BIGNUM*));

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=32, i;
	dma_addr_t bignum_addr, dst_addr;

	memset(group, 0, sizeof(EC_GROUP));
	memset(point, 0, sizeof(EC_POINT));
	memset(r, 0, sizeof(EC_POINT));

	if(!BN_copy(&group->field, bn_n))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&group->a, bn_a3))
		COND_TRAP(31, 0, 0);

	if(!BN_copy(&point->X, bn_a0))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point->Y, bn_a1))
		COND_TRAP(31, 0, 0);
	point->Z.d = (BN_ULONG *)malloc(4);
	point->Z.d[0] = 1;
	point->Z.dmax = 1;
	point->Z.top = 1;
	point->Z.neg = 0;
	point->Z.flags = 0x1;
	point->Z_is_one = 1;

    points[0] = point;
    scalars[0] = bn_e;

	if(!ec_wNAF_mul(group, r, NULL, 1, points, scalars, ctx))
		COND_TRAP(31, 0, 0);

	if(!ec_GFp_simple_make_affine(group, r, ctx))
		COND_TRAP(31, 0, 0);

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size*2);
//	bignum_addr = virt_to_phys(bignum);

	init_job_desc(desc, START_INDEX);
#if 0
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#else
	if(!BN_MONT_CTX_set(mont, bn_n, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(bn_r2, &mont->RR))
		COND_TRAP(31, 0, 0);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
//	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
//	dst_addr = virt_to_phys(dst);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_N);
//	dst_addr = virt_to_phys(dst+bignum_size/4);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_r2->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B1);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_MOD_R2_IN);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);

#endif
	run_descriptor_jr(desc, &jr_t);
	print_bn(bn_e);
	print_bn(bn_n);
	print_bn(bn_a0);
	print_bn(bn_a1);
	print_bn(bn_a3);
	print_bn(bn_b0);
	print_bn(bn_r2);
	print_bn(r->X);
	print_bn(r->Y);
	for(i=0; i<bignum_size/4; i++)
	{
		//if(dst[i] != bn_b1->d[i])
		if(dst[i] != r->X.d[i])
			COND_TRAP(31, 0, 0);
	}
	for(i=0; i<bignum_size/4; i++)
	{
		//if(dst[i+bignum_size/4] != bn_b2->d[i])
		if(dst[i+bignum_size/4] != r->Y.d[i])
			COND_TRAP(31, 0, 0);
	}

	free(dst);
	free(desc);
	//	while(1);
	BN_free(bn_a0);
	BN_free(bn_a1);
	BN_free(bn_a3);
	BN_free(bn_b0);
	BN_free(bn_b1);
	BN_free(bn_b2);
	BN_free(bn_e);
	BN_free(bn_n);
	BN_free(bn_r2);
	BN_CTX_free(ctx);
	BN_MONT_CTX_free(mont);
	free(group);
	free(point);
	free(r);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_ECC_MOD_DBL_TEST
unsigned int p[8] = {
		0x8542D69E,
		0x4C044F18,
		0xE8B92435,
		0xBF6FF7DE,
		0x45728391,
		0x5C45517D,
		0x722EDB8B,
		0x08F1DFC3
};
unsigned int a[8] = {
		0x787968B4,
		0xFA32C3FD,
		0x2417842E,
		0x73BBFEFF,
		0x2F3C848B,
		0x6831D7E0,
		0xEC65228B,
		0x3937E498
};

unsigned int b[8] = {
		0x63E4C6D3,
		0xB23B0C84,
		0x9CF84241,
		0x484BFE48,
		0xF61D59A5,
		0xB16BA06E,
		0x6E12D1DA,
		0x27C5249A
};

unsigned int gx[8] = {
		0x421DEBD6,
		0x1B62EAB6,
		0x746434EB,
		0xC3CC315E,
		0x32220B3B,
		0xADD50BDC,
		0x4C4E6C14,
		0x7FEDD43D
};

unsigned int gy[8] = {
		0x0680512B,
		0xCBB42C07,
		0xD47349D2,
		0x153B70C4,
		0xE5D7FDFC,
		0xBFA36EA1,
		0xA85841B9,
		0xE46E09A2
};
void bn_ecc_mod_dbl_test()
{
	BIGNUM *bn_n = BN_bin2bn((const unsigned char *)p, 8*4, NULL);
	BIGNUM *bn_a0 = BN_bin2bn((const unsigned char *)gx, 8*4, NULL);
	BIGNUM *bn_a1 = BN_bin2bn((const unsigned char *)gy, 8*4, NULL);
	BIGNUM *bn_a3= BN_bin2bn((const unsigned char *)a, 8*4, NULL);
	BIGNUM *bn_b0= BN_bin2bn((const unsigned char *)b, 8*4, NULL);
	BIGNUM *bn_b1= BN_new();
	BIGNUM *bn_b2 = BN_new();
//	BIGNUM *bn_r2 = BN_new();
	BN_MONT_CTX *mont = BN_MONT_CTX_new();
	BN_CTX *ctx = BN_CTX_new();
    EC_GROUP *group = (EC_GROUP *)malloc(sizeof(EC_GROUP));
    EC_POINT *point = (EC_POINT *)malloc(sizeof(EC_POINT));
    EC_POINT *r = (EC_POINT *)malloc(sizeof(EC_POINT));
    BIGNUM *bn_one = BN_bin2bn((const unsigned char *)&one, 1, NULL);

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=32, i;
	dma_addr_t bignum_addr, dst_addr;

	memset(group, 0, sizeof(EC_GROUP));
	memset(point, 0, sizeof(EC_POINT));
	memset(r, 0, sizeof(EC_POINT));

	if(!BN_copy(&group->field, bn_n))
		COND_TRAP(31, 0, 0);
/*
	if(!BN_MONT_CTX_set(mont, bn_n, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_to_montgomery(&group->a, bn_a3, mont, ctx))
		COND_TRAP(31, 0, 0);
	group->field_data1 = mont;
	group->field_data2 = BN_new();
	if(!BN_to_montgomery(group->field_data2, bn_one, mont, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_to_montgomery(&point->X, bn_a0, mont, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_to_montgomery(&point->Y, bn_a1, mont, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point->Z, group->field_data2))
		COND_TRAP(31, 0, 0);
	point->Z_is_one = 1;
*/
///*
	if(!BN_copy(&group->a, bn_a3))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point->X, bn_a0))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point->Y, bn_a1))
		COND_TRAP(31, 0, 0);
	point->Z.d = (BN_ULONG *)malloc(4);
	point->Z.d[0] = 1;
	point->Z.dmax = 1;
	point->Z.top = 1;
	point->Z.neg = 0;
	point->Z.flags = BN_FLG_STATIC_DATA;
	point->Z_is_one = 1;
//*/
	if(!ec_GFp_simple_dbl(group, r, point, ctx))
		COND_TRAP(31, 0, 0);
//	BN_from_montgomery(bn_b1, &r->X, mont, ctx);
//	BN_from_montgomery(bn_b2, &r->Y, mont, ctx);
	if(!ec_GFp_simple_make_affine(group, r, ctx))
		COND_TRAP(31, 0, 0);

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size*2);

//	bignum_addr = virt_to_phys(bignum);

	init_job_desc(desc, START_INDEX);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_DBL);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);

	run_descriptor_jr(desc, &jr_t);
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i] != r->X.d[i])
			COND_TRAP(31, 0, 0);
	}
	for(i=0; i<bignum_size/4; i++)
	{
		if(dst[i+bignum_size/4] != r->Y.d[i])
			COND_TRAP(31, 0, 0);
	}

	free(dst);
	free(desc);
	//	while(1);
	BN_free(bn_a0);
	BN_free(bn_a1);
	BN_free(bn_a3);
	BN_free(bn_b0);
	BN_free(bn_b1);
	BN_free(bn_b2);
	BN_free(bn_one);
	BN_free(bn_n);
//	BN_free(bn_r2);
	BN_CTX_free(ctx);
	BN_MONT_CTX_free(mont);
	free(group);
	free(point);
	free(r);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_TEST
void bn_pkha_fp_mod_test()
{
	BIGNUM *a, *b, *c, *d;
	unsigned int bits, i, step = 1;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	d = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

	for (bits = 1; bits <= 4096; bits += step) {
//		for (i = 1; i <= bits; i += step) {
    		BN_rand(a, bits, 0, 0);
    		BN_rand(b, bits, 0, 0);
    		BN_mod(c, a, b, ctx);

    		if(BN_copy(d, a) == NULL)
    			COND_TRAP(31, 0, 0);

			init_job_desc(desc, START_INDEX);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
			append_fifo_load(desc, virt_to_phys(b->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_fifo_load(desc, virt_to_phys(a->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
			append_fifo_store(desc, virt_to_phys(d->d), (bits+7)/8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

			run_descriptor_jr(desc, &jr_t);
    		bn_correct_top(d);
    		PSPRINTF("a:");
    		print_bn(a);

    		PSPRINTF("b:");
    		print_bn(b);

    		PSPRINTF("c:");
    		print_bn(c);

    		PSPRINTF("d:");
    		print_bn(d);
    		if(BN_cmp(c, d) != 0)
    		{
    			PSPRINTF("%d mod test fail!\r\n", bits);
    			PSPRINTF("###################################\r\n");
    			COND_TRAP(31, 0, 0);
    		}
    		else
    		{
    			PSPRINTF("%d mod test ok!\r\n", bits);
    			PSPRINTF("###################################\r\n");
    		}
			BN_clear(a);
			BN_clear(b);
			BN_clear(c);
			BN_clear(d);
//    	}
    }
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_ADD_TEST
void bn_pkha_fp_mod_add_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 1; bits <= 4096; bits+=step) {
		for (i = 1; i <= bits; i+=step) {
			for (j = 1; j <= bits; j+=step) {
				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				BN_rand(c, bits, 0, 0);

				if (BN_cmp(a, c) >= 0)
				{
					BN_mod(a, a, c, ctx);
					BN_clear_top(a);
				}
				if (BN_cmp(b, c) >= 0)
				{
					BN_mod(b, b, c, ctx);
					BN_clear_top(b);
				}

				BN_mod_add(d, a, b, c, ctx);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
				append_fifo_store(desc, virt_to_phys(e->d), (bits+7)/8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);
				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				PSPRINTF("e:");
				print_bn(e);

				if (BN_cmp(e, d) != 0) {
					PSPRINTF("%d mod add test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d mod add test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_SUB_TEST
void bn_pkha_fp_mod_sub_test() {
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j, step = 128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			for (j = 1; j <= bits; j += step) {
				do{
					BN_rand(a, i, 0, 0);
					BN_rand(b, j, 0, 0);
					BN_rand(c, bits, 0, 0);
				}while((BN_cmp(c, a) < 0)	||	(BN_cmp(c, b) < 0));

				BN_mod_sub(d, a, b, c, ctx);
				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
				append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);
				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				PSPRINTF("e:");
				print_bn(e);

				if (BN_cmp(e, d) != 0) {
					PSPRINTF("%d mod sub test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d mod sub test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}

				BN_mod_sub(d, b, a, c, ctx);
				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_BA);
				append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);
				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				PSPRINTF("e:");
				print_bn(e);

				if (BN_cmp(e, d) != 0) {
					PSPRINTF("%d mod sub test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d mod sub test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_MUL_TEST
void bn_pkha_fp_mod_mul_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j, step = 1;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

#if 0
	for (bits = 1; bits <= 4096; bits += step) {
	for (i = 1; i <= bits; i += step) {
		for (j = 1; j <= bits; j += step) {
#else
	for (bits = 65; bits <= 72; bits += 0) {
		for (i = 57; i <= 65; i += 0) {
			for (j = 57; j <= 65; j += 0) {
#endif
				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				do {
					BN_rand(c, bits, 0, 0);
				} while (!BN_is_odd(c));

				BN_mod_mul(d, a, b, c, ctx);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);


/*	bits = 72;
	unsigned char input_n[9]={
		0xD7, 0xC9, 0x57, 0xCD, 0x48, 0x07, 0x8C, 0x6C, 0xF1
	};

	unsigned char input_a[9]={
		0x12, 0x52, 0x80, 0xD7, 0x42, 0xC4, 0xF3, 0x65
	};

	unsigned char input_b[9]={
		0xA1, 0x81, 0x87, 0xDB, 0x25, 0x96, 0x38, 0x7A
	};

	unsigned char input_r[9]={
		0x74, 0x58, 0x18 ,0x66, 0x61, 0xA1, 0xB8, 0x1B, 0xA3
	};

	for(i=0; i<9; i++)
	{
		*((unsigned char *)(c->d) + i) = input_n[i];
		*((unsigned char *)(a->d) + i) = input_a[i];
		*((unsigned char *)(b->d) + i) = input_b[i];
	}

	e->d = (unsigned int *) OPENSSL_malloc(128);
*/
				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
				append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);
				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(e, d) != 0) {
					PSPRINTF("%d mod mul test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d mod mul test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_EXP_TEST
void bn_pkha_fp_mod_exp_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j, step = 128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;
	unsigned int start, finish;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

#if 0
	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			for (j = 1; j <= bits; j += step) {
#else
	while(1){
		{
			{
				bits = 256;
				i = 256;
				j = 256;
#endif
				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				do {
					BN_rand(c, bits, 0, 0);
				} while (!BN_is_odd(c));

				if (!BN_mod_exp(d, a, b, c, ctx))
					COND_TRAP(31, 0, 0);
				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_key(desc, virt_to_phys(b->d), (bits + 7) / 8, KEY_DEST_CLASS_1 | KEY_NWB | KEY_DEST_PKHA_E);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
				append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				MFSPR(start, SPR_TBLR);
				run_descriptor_jr(desc, &jr_t);
				MFSPR(finish, SPR_TBLR);
				PSPRINTF("cost %d cycle\r\n", finish-start);
				bn_correct_top(e);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(e, d) != 0) {
					PSPRINTF("%d mod exp test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d mod exp test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_INV_TEST
void bn_pkha_fp_mod_inv_test()
{
	BIGNUM *a, *b, *c, *d;
	unsigned int bits, i, step = 128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	d = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

#if 0
	for (bits = 2; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
#else
	while(1){
		{
			bits = 256;
			i = 256;
#endif
			do {
				BN_rand(a, i, 0, 0);
				BN_rand(b, bits, 0, 0);
				if (BN_cmp(a, b) > 0) {
					BN_mod(a, a, b, ctx);
					BN_clear_top(a);
				}
				if (!BN_gcd(c, a, b, ctx))
					COND_TRAP(31, 0, 0);
			} while (!BN_is_one(c) || BN_is_zero(a));

			if (!BN_mod_inverse(c, a, b, ctx))
				COND_TRAP(31, 0, 0);

			PSPRINTF("a:");
			print_bn(a);

			PSPRINTF("b:");
			print_bn(b);

			PSPRINTF("c:");
			print_bn(c);

			if (BN_copy(d, b) == NULL)
				COND_TRAP(31, 0, 0);
			init_job_desc(desc, START_INDEX);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
			append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
			append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

			run_descriptor_jr(desc, &jr_t);
			bn_correct_top(d);

			PSPRINTF("d:");
			print_bn(d);
			if (BN_cmp(c, d) != 0) {
				PSPRINTF("%d mod inv test fail!\r\n", bits);
				PSPRINTF("###################################\r\n");
				COND_TRAP(31, 0, 0);
			} else {
				PSPRINTF("%d mod inv test ok!\r\n", bits);
				PSPRINTF("###################################\r\n");
			}
			BN_clear(a);
			BN_clear(b);
			BN_clear(c);
			BN_clear(d);
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_GCD_TEST
void bn_pkha_fp_mod_gcd_test()
{
	BIGNUM *a, *b, *c, *d;
	unsigned int bits, i, step = 3;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	d = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			BN_rand(a, i, 0, 0);
			BN_rand(b, bits, 0, 0);
			if (BN_cmp(a, b) > 0)
			{
				BN_mod(a, a, b, ctx);
				BN_clear_top(a);
			}

			if (!BN_gcd(c, a, b, ctx))
				COND_TRAP(31, 0, 0);

			PSPRINTF("a:");
			print_bn(a);

			PSPRINTF("b:");
			print_bn(b);

			PSPRINTF("c:");
			print_bn(c);
			if (BN_copy(d, b) == NULL)
				COND_TRAP(31, 0, 0);

			init_job_desc(desc, START_INDEX);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
			append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
			append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

			run_descriptor_jr(desc, &jr_t);
			bn_correct_top(d);

			PSPRINTF("d:");
			print_bn(d);
			if (BN_cmp(c, d) != 0) {
				PSPRINTF("%d gcd test fail!\r\n", bits);
				PSPRINTF("###################################\r\n");
				COND_TRAP(31, 0, 0);
			} else {
				PSPRINTF("%d gcd test ok!\r\n", bits);
				PSPRINTF("###################################\r\n");
			}
			BN_clear(a);
			BN_clear(b);
			BN_clear(c);
			BN_clear(d);
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_R2_TEST
void bn_pkha_fp_mod_r2_test()
{
	BIGNUM *a, *b;
	unsigned int bits, step = 1;
	BN_CTX *ctx = BN_CTX_new();
	BN_MONT_CTX *mont = BN_MONT_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

	for (bits = 1; bits <= 4096; bits += step) {
		do {
			BN_rand(a, bits, 0, 0);
		} while (!BN_is_odd(a));

		if(!BN_MONT_CTX_set(mont, a, ctx))
			COND_TRAP(31, 0, 0);

		PSPRINTF("a:");
		print_bn(a);

		PSPRINTF("rr:");
		print_bn(&mont->RR);

		if (BN_copy(b, a) == NULL)
			COND_TRAP(31, 0, 0);
		init_job_desc(desc, START_INDEX);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
		append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
		append_fifo_store(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

		run_descriptor_jr(desc, &jr_t);
		bn_correct_top(b);

		PSPRINTF("b:");
		print_bn(b);
		if (BN_cmp(b, &mont->RR) != 0) {
			PSPRINTF("%d mod r2 test fail!\r\n", bits);
			PSPRINTF("###################################\r\n");
			COND_TRAP(31, 0, 0);
		} else {
			PSPRINTF("%d mod r2 test ok!\r\n", bits);
			PSPRINTF("###################################\r\n");
		}
		BN_clear(a);
		BN_clear(b);
	}
	BN_free(a);
	BN_free(b);
	BN_CTX_free(ctx);
	BN_MONT_CTX_free(mont);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_ADD_TEST
void bn_pkha_f2m_add_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			for (j = 1; j <= bits; j += step) {
				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				BN_rand(c, bits, 0, 0);
				if (!BN_GF2m_add(d, a, b))
					COND_TRAP(31, 0, 0);

				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_ADD);
				append_fifo_store(desc, virt_to_phys(e->d), (bits+7)/8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(d, e) != 0) {
					PSPRINTF("%d f2m add test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d f2m add test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_MUL_TEST
void bn_pkha_f2m_mul_test()
{
    int p_arr[6] = {0};//prime in array form
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			for (j = 1; j <= bits; j += step) {
				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				BN_rand_arr(p_arr, bits);
				BN_GF2m_arr2poly(p_arr, c);
				if (!BN_GF2m_mod_mul(d, a, b, c, ctx))
					COND_TRAP(31, 0, 0);

				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);
				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_MULT);
				append_fifo_store(desc, virt_to_phys(e->d), (bits+7)/8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(d, e) != 0) {
					PSPRINTF("%d f2m mul test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d f2m mul test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_EXP_TEST
void bn_pkha_f2m_exp_test()
{
    int p_arr[6] = {0};//prime in array form
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 2; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			for (j = 1; j <= bits; j += step) {
				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				BN_rand_arr(p_arr, bits);
				BN_GF2m_arr2poly(p_arr, c);
				if (BN_cmp(a, c) >= 0)
				{
					BN_mod(a, a, c, ctx);
					BN_clear_top(a);
				}
				if (!BN_GF2m_mod_exp(d, a, b, c, ctx))
					COND_TRAP(31, 0, 0);

				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);
				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_key(desc, virt_to_phys(b->d), (bits + 7) / 8, KEY_DEST_CLASS_1 | KEY_NWB | KEY_DEST_PKHA_E);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_EXPO);
				append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(e, d) != 0) {
					PSPRINTF("%d f2m exp test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d f2m exp test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_MOD_TEST
void bn_pkha_f2m_mod_test()
{
    int p_arr[6] = {0};//prime in array form
	BIGNUM *a, *b, *c, *d;
	unsigned int bits, i, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	d = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 2; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			BN_rand(a, i, 0, 0);
			BN_rand_arr(p_arr, bits);
			BN_GF2m_arr2poly(p_arr, b);

			if (!BN_GF2m_mod(c, a, b))
				COND_TRAP(31, 0, 0);

			PSPRINTF("a:");
			print_bn(a);

			PSPRINTF("b:");
			print_bn(b);

			PSPRINTF("c:");
			print_bn(c);

			if (BN_copy(d, b) == NULL)
				COND_TRAP(31, 0, 0);

			init_job_desc(desc, START_INDEX);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
			append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_REDUCT);
			append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

			run_descriptor_jr(desc, &jr_t);
			bn_correct_top(d);

			PSPRINTF("d:");
			print_bn(d);
			if (BN_cmp(c, d) != 0) {
				PSPRINTF("%d f2m mod test fail!\r\n", bits);
				PSPRINTF("###################################\r\n");
				COND_TRAP(31, 0, 0);
			} else {
				PSPRINTF("%d f2m mod test ok!\r\n", bits);
				PSPRINTF("###################################\r\n");
			}
			BN_clear(a);
			BN_clear(b);
			BN_clear(c);
			BN_clear(d);
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_INV_TEST
void bn_pkha_f2m_inv_test()
{
    int p_arr[6] = {0};//prime in array form
	BIGNUM *a, *b, *c, *d;
	unsigned int bits, i, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	d = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 2; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			do {
				BN_rand(a, i, 0, 0);
				BN_rand_arr(p_arr, bits);
				BN_GF2m_arr2poly(p_arr, b);
				if (BN_cmp(a, b) > 0) {
					BN_mod(a, a, b, ctx);
					BN_clear_top(a);
				}

				if (!BN_GF2m_mod_inv(c, a, b, ctx))
				COND_TRAP(31, 0, 0);
			}while(BN_is_zero(c));

			PSPRINTF("a:");
			print_bn(a);

			PSPRINTF("b:");
			print_bn(b);

			PSPRINTF("c:");
			print_bn(c);

			if (BN_copy(d, b) == NULL)
				COND_TRAP(31, 0, 0);

			init_job_desc(desc, START_INDEX);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
			append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_INV);
			append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

			run_descriptor_jr(desc, &jr_t);
			bn_correct_top(d);

			PSPRINTF("d:");
			print_bn(d);
			if (BN_cmp(d, c) != 0) {
				PSPRINTF("%d f2m inv test fail!\r\n", bits);
				PSPRINTF("###################################\r\n");
				COND_TRAP(31, 0, 0);
			} else {
				PSPRINTF("%d f2m inv test ok!\r\n", bits);
				PSPRINTF("###################################\r\n");
			}
			BN_clear(a);
			BN_clear(b);
			BN_clear(c);
			BN_clear(d);
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_R2_TEST
void bn_pkha_f2m_r2_test()
{
    int p_arr[6] = {0};//prime in array form
    int r_arr[2] = {0, -1};//for f2m montgomery conversion
	BIGNUM *a, *b, *c;
	unsigned int bits, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	c = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 1; bits <= 4096; bits += step) {
		BN_rand_arr(p_arr, bits);
		BN_GF2m_arr2poly(p_arr, a);
		r_arr[0] = (p_arr[0] + 128) / 128 * 128;
		BN_GF2m_arr2poly(r_arr, b);

		if (!BN_GF2m_mod_sqr(b, b, a, ctx))
			COND_TRAP(31, 0, 0);

		PSPRINTF("a:");
		print_bn(a);

		PSPRINTF("b:");
		print_bn(b);

		if (BN_copy(c, a) == NULL)
			COND_TRAP(31, 0, 0);

		init_job_desc(desc, START_INDEX);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
		append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_MONT_CNST);
		append_fifo_store(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

		run_descriptor_jr(desc, &jr_t);
		bn_correct_top(c);

		PSPRINTF("c:");
		print_bn(c);
		if (BN_cmp(b, c) != 0) {
			PSPRINTF("%d f2m r2 test fail!\r\n", bits);
			PSPRINTF("###################################\r\n");
			COND_TRAP(31, 0, 0);
		} else {
			PSPRINTF("%d f2m r2 test ok!\r\n", bits);
			PSPRINTF("###################################\r\n");
		}
		BN_clear(a);
		BN_clear(b);
		BN_clear(c);
	}

	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_PRIME_TEST
void bn_pkha_prime_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, step=2;
	BN_CTX *ctx = BN_CTX_new();
	int ret;
	char two = 2;
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
    b = BN_new();
    b = bn_expand2(b, 128);
    c = BN_new();
    c = bn_expand2(c, 128);
    d = BN_new();
    e = BN_bin2bn((const unsigned char *)&two, 1, NULL);
    desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);
    //素数位数为偶数时，软算法检测会出错
    for (bits = 5; bits <= 2048; bits += step) {
		c->d[0] = 5;
		c->top = 1;
		do {
			BN_rand(a, bits, 0, 0);
		} while (!BN_is_odd(a));

		do {
			//BN_bntest_rand(b, bits, 0, 0);
			BN_rand(b, rand()%(bits-3)+3, 0, 0);
			BN_sub(d, a, b);
		} while (d->neg || BN_is_zero(d) || BN_cmp(b, c) < 0);

		ret = BN_is_prime_ex(a, BN_prime_checks, ctx, NULL);

		PSPRINTF("a:");
		print_bn(a);

		PSPRINTF("b:");
		print_bn(b);

		PSPRINTF("c:");
		print_bn(c);

		if (BN_copy(d, a) == NULL)
			COND_TRAP(31, 0, 0);
		init_job_desc(desc, START_INDEX);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
		append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
		append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
		append_fifo_load(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_PRIMALITY);
		append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

		run_descriptor_jr(desc, &jr_t);
		if (d->d[0] != ret) {
			PSPRINTF("%d prime test fail!\r\n", bits);
			PSPRINTF("###################################\r\n");
			COND_TRAP(31, 0, 0);
		} else {
			PSPRINTF("%d prime test ok!\r\n", bits);
			PSPRINTF("###################################\r\n");
		}
		BN_clear(a);
		BN_clear(b);
		BN_clear(c);
		BN_clear(d);
		if(!ret)
			bits -= step;
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_SHIFT_TEST
void bn_pkha_shift_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, step = 2;
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	c = BN_new();
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

	for (bits = 1; bits <= 4096; bits += step) {
		BN_rand(a, bits, 0, 0);
		if(!BN_lshift1(b, a))
			COND_TRAP(31, 0, 0);
		if(!BN_rshift1(c, a))
			COND_TRAP(31, 0, 0);

		PSPRINTF("a:");
		print_bn(a);
		PSPRINTF("b:");
		print_bn(b);
		PSPRINTF("c:");
		print_bn(c);

		if (BN_copy(d, a) == NULL)
			COND_TRAP(31, 0, 0);
		if (BN_copy(e, a) == NULL)
			COND_TRAP(31, 0, 0);
		init_job_desc(desc, START_INDEX);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
//		append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_PKHA_N_SZ|4);
//		append_cmd(desc, (bits + 7) / 8);
		append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
		append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_LEFT_SHIFT_A);
		append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_RIGHT_SHIFT_A);
		append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A);

		run_descriptor_jr(desc, &jr_t);
		bn_correct_top(d);
		bn_correct_top(e);

		PSPRINTF("d:");
		print_bn(d);
		PSPRINTF("e:");
		print_bn(e);
		if (BN_cmp(b, d) != 0 || BN_cmp(c,e) != 0) {
			PSPRINTF("%d shift test fail!\r\n", bits);
			PSPRINTF("###################################\r\n");
			COND_TRAP(31, 0, 0);
		} else {
			PSPRINTF("%d shift test ok!\r\n", bits);
			PSPRINTF("###################################\r\n");
		}

		if (BN_copy(d, a) == NULL)
			COND_TRAP(31, 0, 0);
		if (BN_copy(e, a) == NULL)
			COND_TRAP(31, 0, 0);
		init_job_desc(desc, START_INDEX);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
//		append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_PKHA_N_SZ|4);
//		append_cmd(desc, (bits + 7) / 8);
		append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
		append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_LEFT_SHIFT_B);
		append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_RIGHT_SHIFT_B);
		append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

		run_descriptor_jr(desc, &jr_t);
		bn_correct_top(d);
		bn_correct_top(e);

		PSPRINTF("d':");
		print_bn(d);
		PSPRINTF("e':");
		print_bn(e);
		if (BN_cmp(b, d) != 0 || BN_cmp(c,e) != 0) {
			PSPRINTF("%d shift test fail!\r\n", bits);
			PSPRINTF("###################################\r\n");
			COND_TRAP(31, 0, 0);
		} else {
			PSPRINTF("%d shift test ok!\r\n", bits);
			PSPRINTF("###################################\r\n");
		}
		BN_clear(a);
		BN_clear(b);
		BN_clear(c);
		BN_clear(d);
		BN_clear(e);
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_BASIS_CONV_TEST
void bn_pkha_basis_conv_test()
{
	BIGNUM *a, *b, *c, *d;
	unsigned int bits, step = 1;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc, *desc_t;
	int i, h_bits, h_loop, n_size, last_size;

	a = BN_new();
	a = bn_expand2(a, 8192);
	b = BN_new();
	b = bn_expand2(b, 16);
	c = BN_new();
	d = BN_new();
	desc_t = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4 * 3);

	for (bits = 450; bits <= 512; bits += step) {
		h_bits = (bits+127)/128*128;

    	BN_rand(b, bits, 0, 0);
    	for(i=bits-1; i>=0; i--)
    	{
    		BN_rand(a, bits, 0, 0);
    		if(BN_is_bit_set(b, i))
    		{
    			if (!BN_GF2m_add(c, a, c))
    				COND_TRAP(31, 0, 0);
    		}
    		a->top = (bits-i)*h_bits/32;
    		if(i != 0)
    		{
    			if(!BN_lshift(a, a, h_bits))
    				COND_TRAP(31, 0, 0);
    		}
    	}

    	if(BN_copy(d, b) == NULL)
    		COND_TRAP(31, 0, 0);

    	n_size = 4096/h_bits*h_bits;
    	h_loop = (bits*h_bits)/n_size;
    	last_size = bits*h_bits - h_loop*n_size;

    	desc = desc_t;
		init_job_desc(desc, START_INDEX);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
		append_fifo_load(desc, virt_to_phys(b->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
		for(i=0; i<h_loop; i++)
		{
			append_fifo_load(desc, virt_to_phys(a->d+n_size*i/32), n_size/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_BASIS_CONV);
			if(i==29 || i==60)
			{
				append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
				append_ptr(desc, virt_to_phys(desc+128));
				desc = desc + 128;
				init_job_desc(desc, START_INDEX);
			}

		}
		if(last_size)
		{
			append_fifo_load(desc, virt_to_phys(a->d+n_size*i/32), last_size/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_BASIS_CONV);
			if(i==29 || i==60)
			{
				append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
				append_ptr(desc, virt_to_phys(desc+128));
				desc = desc + 128;
				init_job_desc(desc, START_INDEX);
			}
		}
		append_fifo_store(desc, virt_to_phys(d->d), (bits+7)/8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

		run_descriptor_jr(desc_t, &jr_t);
    	bn_correct_top(d);
//    	PSPRINTF("a:");
//    	print_bn(a);

    	PSPRINTF("b:");
    	print_bn(b);

    	PSPRINTF("c:");
    	print_bn(c);

    	PSPRINTF("d:");
    	print_bn(d);
    	if(BN_cmp(c, d) != 0)
    	{
    		PSPRINTF("%d basis conv test fail!\r\n", bits);
    		PSPRINTF("###################################\r\n");
    		COND_TRAP(31, 0, 0);
    	}
    	else
    	{
    		PSPRINTF("%d basis conv test ok!\r\n", bits);
    		PSPRINTF("###################################\r\n");
    	}
		BN_clear(a);
		BN_clear(b);
		BN_clear(c);
		BN_clear(d);
    }
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_CTX_free(ctx);
	free(desc_t);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_SML_EXP_TEST
void bn_pkha_fp_mod_sml_exp_test()
{
	BIGNUM *a, *b, *c, *d, *e, *f, *g;
	unsigned int bits, i;
	BN_CTX *ctx = BN_CTX_new();
//	BN_MONT_CTX *mont = BN_MONT_CTX_new();

    a = BN_new();
    b = BN_new();
    c = BN_new();
    d = BN_new();
    e = BN_new();
    f = BN_new();
    g = BN_new();

	for (bits = 128; bits <= 2048; bits+=128) {
		do {
			BN_bntest_rand(a, bits, 0, 0);
		} while (!BN_is_odd(a));

		BN_bntest_rand(b, bits, 0, 0);
		BN_bntest_rand(c, bits, 0, 0);
		if(BN_cmp(b, a) >=0)
			BN_mod(b, b, a, ctx);
		if(BN_cmp(c, a) >=0)
			BN_mod(c, c, a, ctx);
		BN_rand(d, 4, 0, 0);
		BN_rand(e, 4, 0, 0);

		if(!BN_exp(f, b, d, ctx))
			COND_TRAP(31, 0, 0);
		if(!BN_exp(g, c, e, ctx))
			COND_TRAP(31, 0, 0);
		if(!BN_mod_mul(f, f, g, a, ctx))
			COND_TRAP(31, 0, 0);

		PSPRINTF("a:");
		print_bn(a);
		PSPRINTF("b:");
		print_bn(b);
		PSPRINTF("c:");
		print_bn(c);
		PSPRINTF("d:");
		print_bn(d);
		PSPRINTF("e:");
		print_bn(e);
		PSPRINTF("f:");
		print_bn(f);

		if(BN_copy(g, a) == NULL)
			COND_TRAP(31, 0, 0);
		pkha_fp_mod_sml_exp((unsigned char *) b->d, (unsigned char *) c->d, (unsigned char *)a->d, bits/8, (unsigned char *)d->d, d->top*4, (unsigned char *)e->d, e->top*4, (unsigned char *)g->d);
		bn_correct_top(g);

		PSPRINTF("g:");
		print_bn(g);
		if (BN_cmp(f, g) != 0) {
			PSPRINTF("%d mod sul exp test fail!\r\n", bits);
			PSPRINTF("###################################\r\n");
		} else {
			PSPRINTF("%d mod sul exp test ok!\r\n", bits);
			PSPRINTF("###################################\r\n");
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_free(f);
	BN_free(g);
	BN_CTX_free(ctx);
//	BN_MONT_CTX_free(mont);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_EXP_TEQ_TEST
void bn_pkha_fp_mod_exp_teq_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i, j;
	BN_CTX *ctx = BN_CTX_new();
	int ret;

    a = BN_new();
    b = BN_new();
    c = BN_new();
    d = BN_new();
    e = BN_new();

	do
	{
		BN_bntest_rand(c, 4096, 0, 0);
	}while(!BN_is_odd(c));

    for (i = 0; i < 10; i++) {
		j = 16;
		BN_bntest_rand(a, 4096, 0, 0);
		BN_rand(b, j, 0, 0);

		if (BN_cmp(a, c) > 0) {
			if (!BN_mod(a, a, c, ctx))
				COND_TRAP(31, 0, 0);
		}

//		if (!BN_mod_exp(d, a, b, c, ctx))
//			COND_TRAP(31, 0, 0);
//		PSPRINTF("a:");
//		print_bn(a);

//		PSPRINTF("b:");
//		print_bn(b);

//		PSPRINTF("c:");
//		print_bn(c);

//		PSPRINTF("d:");
///		print_bn(d);

		if (BN_copy(e, c) == NULL)
			COND_TRAP(31, 0, 0);
		pkha_fp_mod_exp_teq((unsigned char *) a->d, a->top * 4,
				(unsigned char *) b->d, b->top * 4, (unsigned char *) c->d,
				4096 / 8, (unsigned char *) e->d);
		bn_correct_top(e);

//		PSPRINTF("e:");
//		print_bn(e);
//		if (BN_cmp(e, d) != 0) {
//			PSPRINTF("%d^%d mod %d test fail!\r\n", 4096, j, 4096);
//			PSPRINTF("###################################\r\n");
//		} else {
//			PSPRINTF("%d^%d mod %d test ok!\r\n", 4096, j, 4096);
//			PSPRINTF("###################################\r\n");
//		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_EXP_TEQ_TEST
unsigned char p[72] = {
        0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x25
};
void bn_pkha_f2m_exp_teq_test()
{
	BIGNUM *a, *b, *c, *d, *e;
	unsigned int bits, i;
	BN_CTX *ctx = BN_CTX_new();
	int j;
    int p0[] = { 163, 7, 6, 3, 0, -1 };
    int p1[] = { 193, 15, 0, -1 };
    int p2[] = { 4093, 109, 7, 3, 0, -1};

    a = BN_new();
    b = BN_new();
    c = BN_new();
    d = BN_new();
    e = BN_new();
    BN_GF2m_arr2poly(p2, e);
 //   e = BN_bin2bn(p, 72, NULL);

	for (i=0; i<10; i++) {
		BN_rand(a, 4096, 0, 0);
		BN_rand(b, 4096, 0, 0);

		if(!BN_GF2m_mod(a, a, e))
			COND_TRAP(31, 0, 0);
//		if(!BN_GF2m_mod_exp(c, a, b, e, ctx))
//			COND_TRAP(31, 0, 0);

//		PSPRINTF("e:");
//		print_bn(e);

//		PSPRINTF("a:");
//		print_bn(a);

//		PSPRINTF("b:");
//		print_bn(b);

///		PSPRINTF("c:");
//		print_bn(c);

		if (BN_copy(d, a) == NULL)
			COND_TRAP(31, 0, 0);
		pkha_f2m_exp_teq((unsigned char *) a->d, a->top*4, (unsigned char *) b->d, b->top*4, (unsigned char *)e->d, e->top*4, (unsigned char *)d->d);
		bn_correct_top(d);

//		PSPRINTF("d:");
//		print_bn(d);
//		if (BN_cmp(c, d) != 0) {
//			PSPRINTF("%d f2m exp test fail!\r\n", 576);
//			PSPRINTF("###################################\r\n");
//		} else {
//			PSPRINTF("%d f2m exp test ok!\r\n", 576);
//			PSPRINTF("###################################\r\n");
//		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(ctx);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_ECC_F2M_MUL_TEST
void bn_ecc_f2m_mul_test()
{
	int p0[] = { 257, 12, 0, -1 };
	int a[9] = {0};
	char b[] = "00E78BCD09746C202378A7E72B12BCE00266B9627ECB0B5A25367AD1AD4CC6242B";
	char gx[] = "00CDB9CA7F1E6B0441F658343F4B10297C0EF9B6491082400A62E7A7485735FADD";
	char gy[] = "013DE74DA65951C4D76DC89220D5F7777A611B1C38BAE260B175951DC8060C2B3E";
	int k[] = {
			0,
			0x6D3B4971,
			0x53E3E925,
			0x24E5C122,
			0x682DBDC8,
			0x705062E2,
			0x0B917A5F,
			0x8FCDB8EE,
			0x4C66663D
	};
	BIGNUM *bn_n = BN_new();
	BIGNUM *bn_a0 = BN_new();
	BIGNUM *bn_a1 = BN_new();
	BIGNUM *bn_a3 = BN_bin2bn(a, 9*4, NULL);
	BIGNUM *bn_e = BN_bin2bn(k, 9*4, NULL);
	BIGNUM *bn_b0 = BN_new();
	BN_CTX *ctx = BN_CTX_new();
    EC_GROUP *group = (EC_GROUP *)OPENSSL_malloc(sizeof(EC_GROUP));
    EC_POINT *point = (EC_POINT *)OPENSSL_malloc(sizeof(EC_POINT));
    EC_POINT *r = (EC_POINT *)OPENSSL_malloc(sizeof(EC_POINT));
    const EC_POINT **points = malloc(sizeof(EC_POINT*));
    const BIGNUM **scalars = malloc(sizeof(BIGNUM*));

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=36, i;
	dma_addr_t bignum_addr, dst_addr;

	BN_GF2m_arr2poly(p0, bn_n);
	BN_hex2bn(&bn_a0, gx);
	BN_hex2bn(&bn_a1, gy);
	BN_hex2bn(&bn_b0, b);

	if(!ec_GF2m_simple_group_set_curve(group, bn_n, bn_a3, bn_b0, ctx))
		COND_TRAP(31, 0, 0);

	if(!BN_copy(&point->X, bn_a0))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point->Y, bn_a1))
		COND_TRAP(31, 0, 0);
	point->Z.d = (BN_ULONG *)malloc(4);
	point->Z.d[0] = 1;
	point->Z.dmax = 1;
	point->Z.top = 1;
	point->Z.neg = 0;
	point->Z.flags = BN_FLG_STATIC_DATA;
	point->Z_is_one = 1;

    points[0] = point;
    scalars[0] = bn_e;

	if(!ec_GF2m_simple_mul(group, r, NULL, 1, points, scalars, ctx))
		COND_TRAP(31, 0, 0);

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size*2);
/*
	init_job_desc(desc, START_INDEX);
#if 1
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_MOD_F2M);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#else
	if(!BN_MONT_CTX_set(mont, bn_n, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(bn_r2, &mont->RR))
		COND_TRAP(31, 0, 0);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
//	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
//	dst_addr = virt_to_phys(dst);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_N);
//	dst_addr = virt_to_phys(dst+bignum_size/4);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_r2->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B1);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_MOD_R2_IN);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#endif

	run_descriptor_jr(desc, &jr_t);
*/
	pkha_ecc_f2m_mul(bn_n->d, bn_e->d, bn_a0->d, bn_a1->d, bn_a3->d, bn_b0->d, bignum_size, dst);
	for(i=0; i<bignum_size/4; i++)
	{
		//if(dst[i] != bn_b1->d[i])
		if(dst[i] != r->X.d[i])
			COND_TRAP(31, 0, 0);
	}
	for(i=0; i<bignum_size/4; i++)
	{
		//if(dst[i+bignum_size/4] != bn_b2->d[i])
		if(dst[i+bignum_size/4] != r->Y.d[i])
			COND_TRAP(31, 0, 0);
	}

	free(dst);
	free(desc);
	BN_free(bn_n);
	BN_free(bn_a3);
	BN_free(bn_b0);
	BN_free(bn_a0);
	BN_free(bn_a1);
	BN_free(bn_e);
	BN_CTX_free(ctx);
	free(group);
	free(point);
	free(r);
	free(points);
	free(scalars);
	COND_TRAP(31, 0, 0);
}
#endif

#if BN_ECC_F2M_DBL_TEST
void bn_ecc_f2m_dbl_test()
{
	int p0[] = { 257, 12, 0, -1 };
	int a[9] = {0};
	char b[] = "00E78BCD09746C202378A7E72B12BCE00266B9627ECB0B5A25367AD1AD4CC6242B";
	char gx[] = "00CDB9CA7F1E6B0441F658343F4B10297C0EF9B6491082400A62E7A7485735FADD";
	char gy[] = "013DE74DA65951C4D76DC89220D5F7777A611B1C38BAE260B175951DC8060C2B3E";
	BIGNUM *bn_n = BN_new();
	BIGNUM *bn_a0 = BN_new();
	BIGNUM *bn_a1 = BN_new();
	BIGNUM *bn_a3 = BN_bin2bn(a, 9*4, NULL);
	BIGNUM *bn_b0 = BN_new();
	BN_CTX *ctx = BN_CTX_new();
    EC_GROUP *group = (EC_GROUP *)OPENSSL_malloc(sizeof(EC_GROUP));
    EC_POINT *point = (EC_POINT *)OPENSSL_malloc(sizeof(EC_POINT));
    EC_POINT *r = (EC_POINT *)OPENSSL_malloc(sizeof(EC_POINT));

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=36, i;
	dma_addr_t bignum_addr, dst_addr;

	BN_GF2m_arr2poly(p0, bn_n);
	BN_hex2bn(&bn_a0, gx);
	BN_hex2bn(&bn_a1, gy);
	BN_hex2bn(&bn_b0, b);

	if(!ec_GF2m_simple_group_set_curve(group, bn_n, bn_a3, bn_b0, ctx))
		COND_TRAP(31, 0, 0);

	if(!BN_copy(&point->X, bn_a0))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point->Y, bn_a1))
		COND_TRAP(31, 0, 0);
	point->Z.d = (BN_ULONG *)malloc(4);
	point->Z.d[0] = 1;
	point->Z.dmax = 1;
	point->Z.top = 1;
	point->Z.neg = 0;
	point->Z.flags = BN_FLG_STATIC_DATA;
	point->Z_is_one = 1;

	if(!ec_GF2m_simple_dbl(group, r, point, ctx))
		COND_TRAP(31, 0, 0);

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size*2);
/*
	init_job_desc(desc, START_INDEX);
#if 1
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_MOD_F2M);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#else
	if(!BN_MONT_CTX_set(mont, bn_n, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(bn_r2, &mont->RR))
		COND_TRAP(31, 0, 0);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
//	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
//	dst_addr = virt_to_phys(dst);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_N);
//	dst_addr = virt_to_phys(dst+bignum_size/4);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_r2->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B1);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_MOD_R2_IN);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#endif

	run_descriptor_jr(desc, &jr_t);
*/
	pkha_ecc_f2m_dbl(bn_n->d, bn_a0->d, bn_a1->d, bn_a3->d, bn_b0->d, bignum_size, dst);
	for(i=0; i<bignum_size/4; i++)
	{
		//if(dst[i] != bn_b1->d[i])
		if(dst[i] != r->X.d[i])
			COND_TRAP(31, 0, 0);
	}
	for(i=0; i<bignum_size/4; i++)
	{
		//if(dst[i+bignum_size/4] != bn_b2->d[i])
		if(dst[i+bignum_size/4] != r->Y.d[i])
			COND_TRAP(31, 0, 0);
	}

	free(dst);
	free(desc);
	BN_free(bn_n);
	BN_free(bn_a3);
	BN_free(bn_b0);
	BN_free(bn_a0);
	BN_free(bn_a1);
	BN_CTX_free(ctx);
	free(group);
	free(point);
	free(r);
	COND_TRAP(31, 0, 0);
}
#endif

#if BN_ECC_F2M_ADD_TEST
void bn_ecc_f2m_add_test()
{
	int p0[] = { 257, 12, 0, -1 };
	int a[9] = {0};
	char b[] = "00E78BCD09746C202378A7E72B12BCE00266B9627ECB0B5A25367AD1AD4CC6242B";
	char gx[] = "00CDB9CA7F1E6B0441F658343F4B10297C0EF9B6491082400A62E7A7485735FADD";
	char gy[] = "013DE74DA65951C4D76DC89220D5F7777A611B1C38BAE260B175951DC8060C2B3E";
	char qx[] = "00A67941E6DE8A61805F7BCFF0985BB3BED986F1C297E4D8880D82B821C624EE57";
	char qy[] = "0193ED5A6707B5908781B860841085F52EEFA7FE329A5C811843533A874D027271";
	BIGNUM *bn_n = BN_new();
	BIGNUM *bn_a0 = BN_new();
	BIGNUM *bn_a1 = BN_new();
	BIGNUM *bn_a3 = BN_bin2bn(a, 9*4, NULL);
	BIGNUM *bn_b0 = BN_new();
	BIGNUM *bn_b1 = BN_new();
	BIGNUM *bn_b2 = BN_new();
	BN_CTX *ctx = BN_CTX_new();
    EC_GROUP *group = (EC_GROUP *)OPENSSL_malloc(sizeof(EC_GROUP));
    EC_POINT *point = (EC_POINT *)OPENSSL_malloc(sizeof(EC_POINT));
    EC_POINT *pointq = (EC_POINT *)OPENSSL_malloc(sizeof(EC_POINT));
    EC_POINT *r = (EC_POINT *)OPENSSL_malloc(sizeof(EC_POINT));

	uint32_t *desc;
	uint32_t *dst;
	uint32_t bignum_size=36, i;
	dma_addr_t bignum_addr, dst_addr;

	BN_GF2m_arr2poly(p0, bn_n);
	BN_hex2bn(&bn_a0, gx);
	BN_hex2bn(&bn_a1, gy);
	BN_hex2bn(&bn_b0, b);
	BN_hex2bn(&bn_b1, qx);
	BN_hex2bn(&bn_b2, qy);

	if(!ec_GF2m_simple_group_set_curve(group, bn_n, bn_a3, bn_b0, ctx))
		COND_TRAP(31, 0, 0);

	if(!BN_copy(&point->X, bn_a0))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&point->Y, bn_a1))
		COND_TRAP(31, 0, 0);
	point->Z.d = (BN_ULONG *)malloc(4);
	point->Z.d[0] = 1;
	point->Z.dmax = 1;
	point->Z.top = 1;
	point->Z.neg = 0;
	point->Z.flags = BN_FLG_STATIC_DATA;
	point->Z_is_one = 1;

	if(!BN_copy(&pointq->X, bn_b1))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(&pointq->Y, bn_b2))
		COND_TRAP(31, 0, 0);
	pointq->Z.d = (BN_ULONG *)malloc(4);
	pointq->Z.d[0] = 1;
	pointq->Z.dmax = 1;
	pointq->Z.top = 1;
	pointq->Z.neg = 0;
	pointq->Z.flags = BN_FLG_STATIC_DATA;
	pointq->Z_is_one = 1;

	if(!ec_GF2m_simple_add(group, r, point, pointq, ctx))
		COND_TRAP(31, 0, 0);

	desc = (uint32_t *)malloc(MAX_CAAM_DESCSIZE*4);
//	bignum_raw = (uint8_t *)malloc(bignum_size*3+DATA_ALIGN);
	dst = (uint32_t *)malloc(bignum_size*2);
/*
	init_job_desc(desc, START_INDEX);
#if 1
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_MOD_F2M);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#else
	if(!BN_MONT_CTX_set(mont, bn_n, ctx))
		COND_TRAP(31, 0, 0);
	if(!BN_copy(bn_r2, &mont->RR))
		COND_TRAP(31, 0, 0);
	bignum_addr = virt_to_phys(bn_n->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
//	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MONT_CNST);
//	dst_addr = virt_to_phys(dst);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_N);
//	dst_addr = virt_to_phys(dst+bignum_size/4);
//	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	bignum_addr = virt_to_phys(bn_a0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0);
	bignum_addr = virt_to_phys(bn_a1->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1);
	bignum_addr = virt_to_phys(bn_a3->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3);
	bignum_addr = virt_to_phys(bn_b0->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0);
	bignum_addr = virt_to_phys(bn_r2->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B1);
	bignum_addr = virt_to_phys(bn_e->d);
	append_fifo_load(desc, bignum_addr, bignum_size, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_MOD_R2_IN);
	dst_addr = virt_to_phys(dst);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1);
	dst_addr = virt_to_phys(dst+bignum_size/4);
	append_fifo_store(desc, dst_addr, bignum_size, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2);
#endif

	run_descriptor_jr(desc, &jr_t);
*/
	pkha_ecc_f2m_add(bn_n->d, bn_a0->d, bn_a1->d, bn_a3->d, bn_b0->d, bn_b1->d, bn_b2->d, bignum_size, dst);
	for(i=0; i<r->X.top; i++)
	{
		//if(dst[i] != bn_b1->d[i])
		if(dst[i] != r->X.d[i])
			COND_TRAP(31, 0, 0);
	}
	for(i=0; i<r->Y.top; i++)
	{
		//if(dst[i+bignum_size/4] != bn_b2->d[i])
		if(dst[i+bignum_size/4] != r->Y.d[i])
			COND_TRAP(31, 0, 0);
	}

	free(dst);
	free(desc);
	BN_free(bn_n);
	BN_free(bn_a3);
	BN_free(bn_b0);
	BN_free(bn_b1);
	BN_free(bn_b2);
	BN_free(bn_a0);
	BN_free(bn_a1);
	BN_CTX_free(ctx);
	free(group);
	free(point);
	free(pointq);
	free(r);
	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_GCD_TEST
void bn_pkha_f2m_gcd_test()
{
	BIGNUM *a, *b, *c, *d, *t;
	unsigned int bits, i, step=128;
	BN_CTX *ctx = BN_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	d = BN_new();
	desc = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4);

	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			BN_rand(a, i, 0, 0);
			BN_rand(b, bits, 0, 0);
			if (BN_cmp(a, b) > 0) {
				t = a;
				a = b;
				b = t;
			}

			if (!BN_GF2m_gcd(c, b, a, ctx))
				COND_TRAP(31, 0, 0);

			PSPRINTF("a:");
			print_bn(a);

			PSPRINTF("b:");
			print_bn(b);

			PSPRINTF("c:");
			print_bn(c);
			if (BN_copy(d, b) == NULL)
				COND_TRAP(31, 0, 0);

			init_job_desc(desc, START_INDEX);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
			append_fifo_load(desc, virt_to_phys(b->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
			append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
			append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_GCD);
			append_fifo_store(desc, virt_to_phys(d->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

			run_descriptor_jr(desc, &jr_t);
			bn_correct_top(d);

			PSPRINTF("d:");
			print_bn(d);
			if (BN_cmp(c, d) != 0) {
				PSPRINTF("%d f2m gcd test fail!\r\n", bits);
				PSPRINTF("###################################\r\n");
				COND_TRAP(31, 0, 0);
			} else {
				PSPRINTF("%d f2m gcd test ok!\r\n", bits);
				PSPRINTF("###################################\r\n");
			}
			BN_clear(a);
			BN_clear(b);
			BN_clear(c);
			BN_clear(d);
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_F2M_MUL_MONT_TEST
void bn_pkha_f2m_mul_mont_test()
{
	BIGNUM *a, *b, *c, *d, *e, *r, *mont_r;
	unsigned int bits, i, j, step=128;
	BN_CTX *ctx = BN_CTX_new();
    int p_arr[6] = {0};
    int r_arr[2] = {0, -1};
    unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
    d = BN_new();
    e = BN_new();
    r = BN_new();
    mont_r = BN_new();

	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			for (j = 1; j <= bits; j += step) {

				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				BN_rand_arr(p_arr, bits);
				BN_GF2m_arr2poly(p_arr, c);
				r_arr[0] = (p_arr[0] + 128) / 128 * 128;
				BN_GF2m_arr2poly(r_arr, d);

				if (!BN_GF2m_mod_mul(r, a, b, c, ctx))
					COND_TRAP(31, 0, 0);

				if (!BN_GF2m_mod_mul(a, a, d, c, ctx))
					COND_TRAP(31, 0, 0);
				BN_clear_top(a);
				if (!BN_GF2m_mod_mul(b, b, d, c, ctx))
					COND_TRAP(31, 0, 0);
				BN_clear_top(b);
				if (!BN_GF2m_mod_mul(mont_r, r, d, c, ctx))
					COND_TRAP(31, 0, 0);

				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("r:");
				print_bn(r);

				PSPRINTF("d:");
				print_bn(d);

				PSPRINTF("R:");
				print_bn(mont_r);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_MULT);
				append_fifo_store(desc, virt_to_phys(e->d), (bits+7)/8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(r, e) != 0) {
					PSPRINTF("%d f2m mul im test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d f2m mul im test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_fifo_load(desc, virt_to_phys(b->d), (bits+7)/8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
				append_fifo_store(desc, virt_to_phys(e->d), (bits+7)/8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(mont_r, e) != 0) {
					PSPRINTF("%d f2m mul im om test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d f2m mul im om test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
				BN_clear(r);
				BN_clear(mont_r);
			}
		}
	}

	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_free(r);
	BN_free(mont_r);
	BN_CTX_free(ctx);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_FP_MOD_EXP_MONT_TEST
void bn_pkha_fp_mod_exp_mont_test()
{
	BIGNUM *a, *b, *c, *d, *e, *f;
	unsigned int bits, i, j, step = 128;
	BN_CTX *ctx = BN_CTX_new();
	BN_MONT_CTX *mont = BN_MONT_CTX_new();
	unsigned int *desc;

	a = BN_new();
	a = bn_expand2(a, 128);
	b = BN_new();
	b = bn_expand2(b, 128);
	c = BN_new();
	c = bn_expand2(c, 128);
	d = BN_new();
	e = BN_new();
	desc = (unsigned int *) OPENSSL_malloc(MAX_CAAM_DESCSIZE * 4);

	for (bits = 1; bits <= 4096; bits += step) {
		for (i = 1; i <= bits; i += step) {
			for (j = 1; j <= bits; j += step) {
				BN_rand(a, i, 0, 0);
				BN_rand(b, j, 0, 0);
				do {
					BN_rand(c, bits, 0, 0);
				} while (!BN_is_odd(c));

				if (!BN_mod_exp(d, a, b, c, ctx))
					COND_TRAP(31, 0, 0);

				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("b:");
				print_bn(b);

				PSPRINTF("c:");
				print_bn(c);

				PSPRINTF("d:");
				print_bn(d);

				BN_MONT_CTX_set(mont, c, ctx);
				BN_to_montgomery(a, a, mont, ctx);
				BN_clear_top(a);

				if (BN_copy(e, c) == NULL)
					COND_TRAP(31, 0, 0);

				init_job_desc(desc, START_INDEX);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
				append_fifo_load(desc, virt_to_phys(c->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
				append_fifo_load(desc, virt_to_phys(a->d), (bits + 7) / 8, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
				append_key(desc, virt_to_phys(b->d), (bits + 7) / 8, KEY_DEST_CLASS_1 | KEY_NWB | KEY_DEST_PKHA_E);
				append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_EXPO);
				append_fifo_store(desc, virt_to_phys(e->d), (bits + 7) / 8, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

				run_descriptor_jr(desc, &jr_t);
				bn_correct_top(e);

				PSPRINTF("a:");
				print_bn(a);

				PSPRINTF("e:");
				print_bn(e);
				if (BN_cmp(e, d) != 0) {
					PSPRINTF("%d mod exp test fail!\r\n", bits);
					PSPRINTF("###################################\r\n");
					COND_TRAP(31, 0, 0);
				} else {
					PSPRINTF("%d mod exp test ok!\r\n", bits);
					PSPRINTF("###################################\r\n");
				}
				BN_clear(a);
				BN_clear(b);
				BN_clear(c);
				BN_clear(d);
				BN_clear(e);
			}
		}
	}
	BN_free(a);
	BN_free(b);
	BN_free(c);
	BN_free(d);
	BN_free(e);
	BN_free(f);
	BN_CTX_free(ctx);
	BN_MONT_CTX_free(mont);
	free(desc);

	COND_TRAP(31, 0, 0);
}
#endif

#if BN_PKHA_MASS_RAND_TEST
void bn_pkha_mass_rand_test()
{
	struct pkha_ctx *pctx = (struct pkha_ctx *)OPENSSL_malloc(sizeof(struct pkha_ctx));
	BN_CTX *ctx = BN_CTX_new();
	BN_MONT_CTX *mont = BN_MONT_CTX_new();
	unsigned int pkha_mode[38] = {
			0x00002, 0x00003, 0x00004, 0x00005, 0x80005, 0xc0005, 0x00006, 0x00406, 0x80006, 0x00007, 0x00008, 0x0000c,
			0x0000e, 0x20002, 0x20005, 0xa0005, 0xe0005, 0x20006, 0x20406, 0x20007, 0x20008, 0x2000c, 0x2000e, 0x0000f,
			0x00009, 0x10009, 0x0000a, 0x0000b, 0x0040b, 0x1000b, 0x1040b, 0x20009, 0x30009, 0x2000a, 0x2000b, 0x2040b,
			0x3000b, 0x3040b
	};//all the pkha mode word
    int p_arr[6] = {0};//prime in array form
    int r_arr[2] = {0, -1};//for f2m montgomery conversion
    char two = 2, five = 5;
    int ret;
	BIGNUM *bn[5], *bn_r, *bn_tmp, *bn_two, *bn_five;
	unsigned int i, tmp, bits=2000, bytes, mode, ebits, num=0;

//	MFSPR(tmp, SPR_TBLR);//get random seed from TBL
//	srand(tmp);
	//srand(0xe30cdd0e);
//	PSPRINTF("Test start,random seed is 0x%x\r\n", tmp);
	for(i=0; i<5; i++)
	{
		bn[i] = BN_new();
		bn[i]->d = (BN_ULONG *)OPENSSL_malloc(PKHA_RAM_MAX_SIZE);
		bn[i]->dmax = 128;
//		memset(bn[i]->d, 0, 0x200);
	}
//	bn_r = BN_new();
//	bn_tmp = BN_new();
	bn_two = BN_bin2bn((const unsigned char *)&two, 1, NULL);
	bn_five = BN_bin2bn((const unsigned char *)&five, 1, NULL);

	while (1) {
		bn_r = BN_new();
		bn_tmp = BN_new();
//		srand(0xe30cdd0e);
		tmp = rand() % 24;
		mode = pkha_mode[tmp];
//		mode = pkha_mode[0];
		if(mode == pkha_mode[15] || mode == pkha_mode[16])
			bits = rand() % 2048 + 1;
		else if(mode == pkha_mode[23])
		{
			bits = rand() % 498 + 15;
//			bits++;
		}
		else
		{

			bits = rand() % 4096 + 1;
//			bits = rand() % 8 + 2049;
		}
		bytes = (bits + 7) / 8;
//		bytes = (bytes+15)/16*16;//16 bytes align
		BN_rand(bn[0], bits, 0, 0);
		for(i=1; i<3; i++)
		{
			if(i==1 && mode == pkha_mode[23])
				BN_rand(bn[i], rand()%(bits-10)+11, 0, 0);
			else if(i==2 && mode == pkha_mode[23])
			{
				bn[i]->d[0] = bits/8-1;
				bn[i]->top = 1;
			}
			else
				BN_rand(bn[i], rand()%bits+1, 0, 0);
		}
		ebits = bits < 32 ? bits : 32;
		BN_rand(bn[3], rand()%ebits+1, 0, 0);
//		bn[3]->d[0] = 1;
//		bn[3]->top = 1;
/*
		for (i = 0; i < 5; i++) {
			if (i == 0) {
				if((mode & 0x3f) == OP_ALG_PKMODE_MOD_REDUCT)
				{
					p_arr[i] = BN_num_bits(bn[1])-1;
				}
				else
				{
					p_arr[i] = bits-1;
				}
			} else if (i == 4) {
				p_arr[i] = 0;
			} else {
				p_arr[i] = rand() % p_arr[i - 1];
			}

			if (p_arr[i] == 0)
				break;
		}
		p_arr[i + 1] = -1;
*/
		if((mode & 0x3f) == OP_ALG_PKMODE_MOD_REDUCT)
			BN_rand_arr(p_arr, BN_num_bits(bn[1]));
		else
			BN_rand_arr(p_arr, bits);
		r_arr[0] = (p_arr[0] + 128) / 128 * 128;

		pctx->mode = mode;
		PSPRINTF("NO.%d -------------------------------------------------------------\r\n", num++);
		PSPRINTF("mode:0x%x, bits:%d\r\n", mode, bits);
		pctx->n_len = bytes;
		pctx->n_ram = (unsigned char *)bn[0]->d;
		pctx->a_len = bytes;
//		pctx->a_len = BN_num_bytes(bn[1]);
		pctx->a_ram = (unsigned char *)bn[1]->d;
		pctx->b_len = bytes;
//		pctx->b_len = BN_num_bytes(bn[2]);
		pctx->b_ram = (unsigned char *)bn[2]->d;
		pctx->e_len = bytes;
//		pctx->e_len = BN_num_bytes(bn[3]);
		pctx->e_ram = (unsigned char *)bn[3]->d;
		pctx->out_len = pctx->n_len;
		pctx->out_ram = (unsigned char *)bn[4]->d;

		switch (mode & 0x3f) {
		case OP_ALG_PKMODE_MOD_ADD:
			if(mode & OP_ALG_PKMODE_MOD_F2M)
			{
				if(!BN_GF2m_add(bn_r, bn[1], bn[2]))
					COND_TRAP(31, 0, 0);
			}
			else
			{
		    	if(BN_cmp(bn[1], bn[0]) > 0)
		    	{
		    		BN_mod(bn[1], bn[1], bn[0], ctx);
		    		//pctx->a_len = BN_num_bytes(bn[1]);
		    	}
		    	if(BN_cmp(bn[2], bn[0]) > 0)
		    	{
		    		BN_mod(bn[2], bn[2], bn[0], ctx);
		    		//pctx->b_len = BN_num_bytes(bn[2]);
		    	}
		    	if(!BN_mod_add(bn_r, bn[1], bn[2], bn[0], ctx))
		    		COND_TRAP(31, 0, 0);
			}
			break;
		case OP_ALG_PKMODE_MOD_SUB_AB:
	    	if(BN_cmp(bn[1], bn[0]) > 0)
	    	{
	    		BN_mod(bn[1], bn[1], bn[0], ctx);
	    		//pctx->a_len = BN_num_bytes(bn[1]);
	    	}
	    	if(BN_cmp(bn[2], bn[0]) > 0)
	    	{
	    		BN_mod(bn[2], bn[2], bn[0], ctx);
	    		//pctx->b_len = BN_num_bytes(bn[2]);
	    	}
	    	if(BN_cmp(bn[1], bn[2]) >= 0)
	    	{
	    		if(!BN_mod_sub(bn_r, bn[1], bn[2], bn[0], ctx))
	    			COND_TRAP(31, 0, 0);
	    	}
	    	else
	    	{
	    		if(!BN_add(bn_r, bn[1], bn[0]))
	    			COND_TRAP(31, 0, 0);
	    		if(!BN_sub(bn_r, bn_r, bn[2]))
	    			COND_TRAP(31, 0, 0);
	    	}
			break;
		case OP_ALG_PKMODE_MOD_SUB_BA:
	    	if(BN_cmp(bn[1], bn[0]) > 0)
	    	{
	    		BN_mod(bn[1], bn[1], bn[0], ctx);
	    		//pctx->a_len = BN_num_bytes(bn[1]);
	    	}
	    	if(BN_cmp(bn[2], bn[0]) > 0)
	    	{
	    		BN_mod(bn[2], bn[2], bn[0], ctx);
	    		//pctx->b_len = BN_num_bytes(bn[2]);
	    	}
	    	if(BN_cmp(bn[1], bn[2]) < 0)
	    	{
	    		if(!BN_mod_sub(bn_r, bn[2], bn[1], bn[0], ctx))
	    			COND_TRAP(31, 0, 0);
	    	}
	    	else
	    	{
	    		if(!BN_add(bn_r, bn[2], bn[0]))
	    			COND_TRAP(31, 0, 0);
	    		if(!BN_sub(bn_r, bn_r, bn[1]))
	    			COND_TRAP(31, 0, 0);
	    	}
			break;
		case OP_ALG_PKMODE_MOD_MULT:
			if(mode & OP_ALG_PKMODE_MOD_F2M)
			{
				if(!BN_GF2m_arr2poly(p_arr, bn[0]))
					COND_TRAP(31, 0, 0);
				if(!BN_GF2m_mod_mul(bn_r, bn[1], bn[2], bn[0], ctx))
					COND_TRAP(31, 0, 0);
				if(mode & OP_ALG_PKMODE_MOD_IN_MONTY)
				{
					if(!BN_GF2m_arr2poly(r_arr, bn_tmp))
						COND_TRAP(31, 0, 0);
					if(!BN_GF2m_mod_mul(bn[1], bn[1], bn_tmp, bn[0], ctx))
						COND_TRAP(31, 0, 0);
					if(!BN_GF2m_mod_mul(bn[2], bn[2], bn_tmp, bn[0], ctx))
						COND_TRAP(31, 0, 0);
					//pctx->a_len = BN_num_bytes(bn[1]);
					//pctx->b_len = BN_num_bytes(bn[2]);
					if(mode & OP_ALG_PKMODE_MOD_OUT_MONTY)
					{
						if(!BN_GF2m_mod_mul(bn_r, bn_r, bn_tmp, bn[0], ctx))
							COND_TRAP(31, 0, 0);
					}
				}
			}
			else
			{
				while(!BN_is_odd(bn[0]) || BN_num_bits(bn[0]) != bits)
				{
					BN_rand(bn[0], bits, 0, 0);
				}
		    	if(BN_cmp(bn[1], bn[0]) > 0)
		    	{
		    		BN_mod(bn[1], bn[1], bn[0], ctx);
		    		//pctx->a_len = BN_num_bytes(bn[1]);
		    	}
	    		//PSPRINTF("a[%d]:", BN_num_bytes(bn[1]));
	    		//print_bn(bn[1]);
		    	if(BN_cmp(bn[2], bn[0]) > 0)
		    	{
		    		BN_mod(bn[2], bn[2], bn[0], ctx);
		    		//pctx->b_len = BN_num_bytes(bn[2]);
		    	}
	    		//PSPRINTF("b[%d]:", BN_num_bytes(bn[2]));
	    		//print_bn(bn[2]);
				if(!BN_mod_mul(bn_r, bn[1], bn[2], bn[0], ctx))
					COND_TRAP(31, 0, 0);
				if(mode & OP_ALG_PKMODE_MOD_IN_MONTY)
				{
					if(!BN_MONT_CTX_set(mont, bn[0], ctx))
						COND_TRAP(31, 0, 0);
					if(!BN_to_montgomery(bn[1], bn[1], mont, ctx))
						COND_TRAP(31, 0, 0);
					if(!BN_to_montgomery(bn[2], bn[2], mont, ctx))
						COND_TRAP(31, 0, 0);
					//pctx->a_len = BN_num_bytes(bn[1]);
					//pctx->b_len = BN_num_bytes(bn[2]);
					if(mode & OP_ALG_PKMODE_MOD_OUT_MONTY)
					{
						if(!BN_to_montgomery(bn_r, bn_r, mont, ctx))
							COND_TRAP(31, 0, 0);
					}
				}
			}
			break;
		case OP_ALG_PKMODE_MOD_EXPO:
//			pctx->b_len = 0;
//			pctx->b_ram = 0;
//			pctx->e_len = bytes;//(BN_num_bits(bn[3])+7)/8;
//			pctx->e_ram = (unsigned char *)bn[3]->d;
			if(mode & OP_ALG_PKMODE_MOD_F2M)
			{
				if(!BN_GF2m_arr2poly(p_arr, bn[0]))
					COND_TRAP(31, 0, 0);
				if(!BN_GF2m_mod_exp(bn_r, bn[1], bn[3], bn[0], ctx))
					COND_TRAP(31, 0, 0);
			}
			else
			{
				while(!BN_is_odd(bn[0]) || BN_num_bits(bn[0]) != bits)
				{
					BN_rand(bn[0], bits, 0, 0);
				}
		    	if(BN_cmp(bn[1], bn[0]) > 0)
		    	{
		    		BN_mod(bn[1], bn[1], bn[0], ctx);
		    		//pctx->a_len = BN_num_bytes(bn[1]);
		    	}
	        	if(!BN_mod_exp(bn_r, bn[1], bn[3], bn[0], ctx))
	        		COND_TRAP(31, 0, 0);
	        	if(mode & OP_ALG_PKMODE_MOD_IN_MONTY)
	        	{
					if(!BN_MONT_CTX_set(mont, bn[0], ctx))
						COND_TRAP(31, 0, 0);
					if(!BN_to_montgomery(bn[1], bn[1], mont, ctx))
						COND_TRAP(31, 0, 0);
					//pctx->a_len = BN_num_bytes(bn[1]);
	        	}
			}
			break;
		case OP_ALG_PKMODE_MOD_REDUCT:
			pctx->n_len = bytes;
			pctx->n_ram = (unsigned char *)bn[1]->d;
			pctx->a_len = bytes;
			pctx->a_ram = (unsigned char *)bn[0]->d;
//			pctx->b_len = 0;
//			pctx->b_ram = 0;
//			pctx->out_len = pctx->n_len;
			if(mode & OP_ALG_PKMODE_MOD_F2M)
			{
				if(!BN_GF2m_arr2poly(p_arr, bn[1]))
					COND_TRAP(31, 0, 0);
				if (!BN_GF2m_mod(bn_r, bn[0], bn[1]))
					COND_TRAP(31, 0, 0);
			}
			else
			{
				if(!BN_mod(bn_r, bn[0], bn[1], ctx))
					COND_TRAP(31, 0, 0);
			}
			break;
		case OP_ALG_PKMODE_MOD_INV:
//			pctx->b_len = 0;
//			pctx->b_ram = 0;
			if(mode & OP_ALG_PKMODE_MOD_F2M)
			{
				do {
					if (!BN_GF2m_arr2poly(p_arr, bn[0]))
						COND_TRAP(31, 0, 0);
					ret = BN_GF2m_mod_inv(bn_r, bn[1], bn[0], ctx);
					BN_rand_arr(p_arr, bits);
				} while (ret == -1);
				if(!ret)
					COND_TRAP(31, 0, 0);
			}
			else
			{
		    	if(BN_cmp(bn[1], bn[0]) > 0)
		    	{
		    		BN_mod(bn[1], bn[1], bn[0], ctx);
		    		//pctx->a_len = BN_num_bytes(bn[1]);
		    	}
		    	if(!BN_gcd(bn_tmp, bn[1], bn[0], ctx))
	    			COND_TRAP(31, 0, 0);
	    		while(!BN_is_one(bn_tmp) || BN_num_bits(bn[0]) != bits)
	    		{
	    			BN_rand(bn[0], bits, 0, 0);
	    			BN_rand(bn[1], rand()%bits+1, 0, 0);
			    	if(BN_cmp(bn[1], bn[0]) > 0)
			    	{
			    		BN_mod(bn[1], bn[1], bn[0], ctx);
			    		//pctx->a_len = BN_num_bytes(bn[1]);
			    	}
			    	if(!BN_gcd(bn_tmp, bn[1], bn[0], ctx))
		    			COND_TRAP(31, 0, 0);
	    		}
	    		if(!BN_mod_inverse(bn_r, bn[1], bn[0], ctx))
	    			COND_TRAP(31, 0, 0);
			}
			break;
		case OP_ALG_PKMODE_MOD_MONT_CNST:
//			pctx->a_len = 0;
//			pctx->a_ram = 0;
//			pctx->b_len = 0;
//			pctx->b_ram = 0;
			if(mode & OP_ALG_PKMODE_MOD_F2M)
			{
				if(!BN_GF2m_arr2poly(p_arr, bn[0]))
					COND_TRAP(31, 0, 0);
				if(!BN_GF2m_arr2poly(r_arr, bn_tmp))
					COND_TRAP(31, 0, 0);
				if (!BN_GF2m_mod_sqr(bn_r, bn_tmp, bn[0], ctx))
					COND_TRAP(31, 0, 0);
			}
			else
			{
				while(!BN_is_odd(bn[0]) || BN_num_bits(bn[0]) != bits)
				{
					BN_rand(bn[0], bits, 0, 0);
				}
				if(!BN_MONT_CTX_set(mont, bn[0], ctx))
					COND_TRAP(31, 0, 0);
				if(!BN_copy(bn_r, &mont->RR))
					COND_TRAP(31, 0, 0);
			}
			break;
		case OP_ALG_PKMODE_MOD_GCD:
//			pctx->b_len = 0;
//			pctx->b_ram = 0;
			if(mode & OP_ALG_PKMODE_MOD_F2M)
			{
		    	if(BN_cmp(bn[1], bn[0]) > 0)
		    	{
		    		BN_mod(bn[1], bn[1], bn[0], ctx);
		    		//pctx->a_len = BN_num_bytes(bn[1]);
		    	}
	    		if(!BN_GF2m_gcd(bn_r, bn[0], bn[1], ctx))
	    			COND_TRAP(31, 0, 0);
			}
			else
			{
		    	if(BN_cmp(bn[1], bn[0]) > 0)
		    	{
		    		BN_mod(bn[1], bn[1], bn[0], ctx);
		    		//pctx->a_len = BN_num_bytes(bn[1]);
		    	}
		    	if(!BN_gcd(bn_r, bn[1], bn[0], ctx))
	    			COND_TRAP(31, 0, 0);
			}
			break;
		case OP_ALG_PKMODE_MOD_PRIMALITY:

			while(!BN_is_odd(bn[0]))
			{
				BN_rand(bn[0], bits, 0, 0);
			}

//			BN_generate_prime_ex(bn[0], bits, 0, NULL, NULL, NULL);
			if(!BN_sub(bn_tmp, bn[0], bn[1]))
				COND_TRAP(31, 0, 0);
			while(BN_cmp(bn_tmp, bn_two)<=0 || BN_cmp(bn[1], bn_two) <=0)
			{
				BN_rand(bn[1], rand()%bits+1, 0, 0);
				if(!BN_sub(bn_tmp, bn[0], bn[1]))
					COND_TRAP(31, 0, 0);
			}
			while(bn[1]->top == 1 && bn[1]->d[0] <= 5*bn[2]->d[0])
			{
				BN_rand(bn[2], rand()%8+1, 0, 0);
			}
			ret = BN_is_prime_ex(bn[0], BN_prime_checks, ctx, NULL);
			//ret = 1;
			if(ret == 1)
			{
				BN_one(bn_r);
			}
			else if(ret == 0)
			{
				BN_zero(bn_r);
			}
			else
			{
				COND_TRAP(31, 0, 0);
			}
			break;
		}
		for(i=0; i<4; i++)
		{
			memset(bn[i]->d+bn[i]->top, 0, (bn[i]->dmax-bn[i]->top)*4);
		}
		PSPRINTF("N[%d]:", BN_num_bytes(bn[0]));
		print_bn(bn[0]);
		PSPRINTF("A[%d]:", BN_num_bytes(bn[1]));
		print_bn(bn[1]);
		PSPRINTF("B[%d]:", BN_num_bytes(bn[2]));
		print_bn(bn[2]);
		PSPRINTF("E[%d]:", BN_num_bytes(bn[3]));
		print_bn(bn[3]);
		PSPRINTF("r[%d]:", BN_num_bytes(bn_r));
		print_bn(bn_r);

		pkha_calc(pctx);
		bn[4]->top = (pctx->n_len+3)/4;
		bn_correct_top(bn[4]);

		PSPRINTF("R[%d]:", BN_num_bytes(bn[4]));
		print_bn(bn[4]);
		if(BN_cmp(bn_r, bn[4]) != 0)
		{
			PSPRINTF("test failed!\r\n");
			PSPRINTF("#########################################################################\r\n");
			COND_TRAP(31, 0, 0);
		}
		else
		{
			PSPRINTF("test succeeded!\r\n");
			PSPRINTF("#########################################################################\r\n");
		}

		BN_free(bn_tmp);
		BN_free(bn_r);
		for(i=0; i<5; i++)
		{
			memset(bn[i]->d, 0, 0x200);
			//BN_zero(bn[i]);
		}
		memset(pctx, 0, sizeof(struct pkha_ctx));

	}

	free(pctx);
	BN_CTX_free(ctx);
	BN_MONT_CTX_free(mont);
	for(i=0; i<5; i++)
	{
		BN_free(bn[i]);
	}
//	BN_free(bn_r);
//	BN_free(bn_tmp);
	BN_free(bn_two);
	BN_free(bn_five);
}
#endif

#if PKHA_ERROR_TEST
void pkha_error_test()
{
	int ret, i;
	/*
	 * 1.clear memory error
	 */
	ret = pkha_clear_memory_error();
	if(ret != 0x20000181)
		COND_TRAP(31, 0, 0);

	/*
	 * 2.copy memory error
	 */
	for(i=0; i<3; i++)
	{
		ret = pkha_copy_memory_error(i);
		if(ret != 0x20000581)
			COND_TRAP(31, 0, 0);
	}

	/*
	 * 3.data size error
	 */
	for(i=1; i<2; i++)
	{
		ret = pkha_data_size_error(i);
		if(ret != 0x20000382)
			COND_TRAP(31, 0, 0);
	}

	/*
	 * 4.A size error
	 */
	ret = pkha_a_size_error();
	if(ret != 0x20000584)
			COND_TRAP(31, 0, 0);

	/*
	 * 5.B size error
	 */
	ret = pkha_b_size_error();
	if(ret != 0x20000585)
			COND_TRAP(31, 0, 0);

	/*
	 * 6.modulus even error
	 */
	ret = pkha_modulus_even_error();
	if(ret != 0x20000a88)
			COND_TRAP(31, 0, 0);

	/*
	 * 7.divide by zero error
	 */
	for(i=0; i<3; i++)
	{
		ret = pkha_divide_by_zero_error(i);
		if(ret != 0x20000a87)
			COND_TRAP(31, 0, 0);
	}
	/*
	 * 8.key size error
	 */
	for(i=0; i<2; i++)
	{
		ret = pkha_key_size_error(i);
		if(ret != 0x20000383)
			COND_TRAP(31, 0, 0);
	}
}
#endif

void bn_test()
{
	unsigned int tmp;

	memset((void *)0xe007c000, 0, 0x1000);
	sec_init();
	rng_impl_init();							//analogy rng sec mode set

	MFSPR(tmp, TBL);//get random seed from TBL
	srand(tmp);
	PSPRINTF("Test start,random seed is 0x%x\r\n", tmp);

#if BN_MOD_ADD_TEST
	bn_mod_add_test();
#endif

#if BN_MOD_SUB_TEST
	bn_mod_sub_test();
#endif

#if BN_MOD_MUL_TEST
	bn_mod_mul_test();
#endif

#if BN_MOD_INV_TEST
	bn_mod_inv_test();
#endif

#if BN_MOD_EXP_TEST
	bn_mod_exp_test();
#endif

#if BN_MOD_MUL_MONT_TEST
	bn_mod_mul_mont_test();
#endif

#if BN_PKHA_FP_MOD_TEST
	bn_pkha_fp_mod_test();
#endif

#if BN_PKHA_FP_MOD_EXP_TEST
	bn_pkha_fp_mod_exp_test();
#endif

#if BN_PKHA_FP_MOD_ADD_TEST
	bn_pkha_fp_mod_add_test();
#endif

#if BN_PKHA_FP_MOD_SUB_TEST
	bn_pkha_fp_mod_sub_test();
#endif

#if BN_PKHA_FP_MOD_MUL_TEST
	bn_pkha_fp_mod_mul_test();
#endif

#if BN_PKHA_FP_MOD_INV_TEST
	bn_pkha_fp_mod_inv_test();
#endif

#if BN_PKHA_FP_MOD_GCD_TEST
	bn_pkha_fp_mod_gcd_test();
#endif

#if BN_PKHA_FP_MOD_R2_TEST
	bn_pkha_fp_mod_r2_test();
#endif

#if BN_PKHA_F2M_ADD_TEST
	bn_pkha_f2m_add_test();
#endif

#if BN_PKHA_F2M_MUL_TEST
	bn_pkha_f2m_mul_test();
#endif

#if BN_PKHA_F2M_EXP_TEST
	bn_pkha_f2m_exp_test();
#endif

#if BN_PKHA_F2M_MOD_TEST
	bn_pkha_f2m_mod_test();
#endif

#if BN_PKHA_F2M_INV_TEST
	bn_pkha_f2m_inv_test();
#endif

#if BN_PKHA_F2M_R2_TEST
	bn_pkha_f2m_r2_test();
#endif

#if BN_PKHA_PRIME_TEST
	bn_pkha_prime_test();
#endif

#if BN_ECC_MOD_ADD_TEST
	bn_ecc_mod_add_test();
#endif

#if BN_ECC_MOD_MUL_TEST
	bn_ecc_mod_mul_test();
#endif

#if BN_ECC_MOD_DBL_TEST
	bn_ecc_mod_dbl_test();
#endif

#if BN_PKHA_FP_MOD_SML_EXP_TEST
	bn_pkha_fp_mod_sml_exp_test();
#endif

#if BN_PKHA_FP_MOD_EXP_TEQ_TEST
	bn_pkha_fp_mod_exp_teq_test();
#endif

#if BN_PKHA_F2M_EXP_TEQ_TEST
	bn_pkha_f2m_exp_teq_test();
#endif

#if BN_ECC_F2M_MUL_TEST
	bn_ecc_f2m_mul_test();
#endif

#if BN_ECC_F2M_DBL_TEST
	bn_ecc_f2m_dbl_test();
#endif

#if BN_ECC_F2M_ADD_TEST
	bn_ecc_f2m_add_test();
#endif

#if BN_PKHA_F2M_GCD_TEST
	bn_pkha_f2m_gcd_test();
#endif

#if BN_PKHA_F2M_MUL_MONT_TEST
	bn_pkha_f2m_mul_mont_test();
#endif

#if BN_PKHA_FP_MOD_EXP_MONT_TEST
	bn_pkha_fp_mod_exp_mont_test();
#endif

#if BN_PKHA_SHIFT_TEST
	bn_pkha_shift_test();
#endif

#if BN_PKHA_BASIS_CONV_TEST
	bn_pkha_basis_conv_test();
#endif

#if BN_PKHA_MASS_RAND_TEST
	bn_pkha_mass_rand_test();
#endif

#if BN_PKHA_FP256_SM2_TEST
	sm2_fp256_test();
#endif

#if BN_PKHA_F2M257_SM2_TEST
	sm2_f2m257_test();
#endif

#if PKHA_FP256_SM2_TEST
	sm2_fp256_test_ex();
#endif

#if PKHA_F2M257_SM2_TEST
	sm2_f2m257_test_ex();
#endif

#if PKHA_SM2_KEY_EXCHANGE_TEST
	sm2_key_exchange_test();
#endif

#if PKHA_SM9_TEST
	sm9_test();
#endif

#if PKHA_ERROR_TEST
	pkha_error_test();
#endif

#if PKHA_RSA_END_BIG_TEST
	rsa_3030_E_BIG();
#endif

#if PKHA_RSA_END_LIT_TEST
	rsa_3030_E_LIT();
#endif
	//sm2_test();
	//test1();
	//test2();
	//test3();
	lib_sm2_test();

	COND_TRAP(31, 0, 0);
}
