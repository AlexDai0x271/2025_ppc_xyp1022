#include "desc_constr.h"
#include "common.h"
#include "desc.h"
#include "sec.h"
#include "cipher.h"
#include "stdlib.h"

int jdkek_gen = 0;

caam_cipher_setkey(struct cipher_ctx **ctxp,enum cipher_algos cipher_algo,enum atype alg_type,enum encdec dir,unsigned char *key,unsigned key_size)
{
	void *ptr;
	ptr = calloc(1, (sizeof(struct cipher_ctx)+DESC_ALIGN));	//need data_alian for it contain data ptr
	if (ptr == NULL) {
		debug("Cannot allocate memory for context\n");
		return -1;
	}
	*ctxp  = (uint32_t *)(((unsigned long)ptr+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	(*ctxp)->algo = cipher_algo;
	(*ctxp)->dir = dir;
	(*ctxp)->algo_type = alg_type;
	(*ctxp)->malloc_ptr = ptr;
	(*ctxp)->key_size = key_size;

	memcpy_c((*ctxp)->key,key,key_size);

	return 0;
}

caam_cipher_jdkek_gen(struct cipher_ctx **ctxp)
{
	void *ptr;
	int ret;
	ptr = calloc(1, (sizeof(struct cipher_ctx)+DESC_ALIGN));	//need data_alian for it contain data ptr
	if (ptr == NULL) {
		debug("Cannot allocate memory for context\n");
		return -1;
	}
	*ctxp  = (uint32_t *)(((unsigned long)ptr+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	(*ctxp)->malloc_ptr = ptr;

	inline_jdkek_gen((*ctxp)->cipher_desc);

	ret = run_descriptor_jr((*ctxp)->cipher_desc,&jr_t);
	if (ret)
		debug("Error %x\n", ret);

	return 0;
}

caam_cipher_setkey_enckey(struct cipher_ctx **ctxp,enum cipher_algos cipher_algo,enum atype alg_type,enum encdec dir,unsigned char *key,unsigned key_size,unsigned ek_type)
{
	void *ptr;
	int ret;
	ptr = calloc(1, (sizeof(struct cipher_ctx)+DESC_ALIGN));	//need data_alian for it contain data ptr
	if (ptr == NULL) {
		debug("Cannot allocate memory for context\n");
		return -1;
	}
	*ctxp  = (uint32_t *)(((unsigned long)ptr+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	(*ctxp)->algo = cipher_algo;
	(*ctxp)->dir = dir;
	(*ctxp)->algo_type = alg_type;
	(*ctxp)->malloc_ptr = ptr;
	(*ctxp)->key_size = key_size;

	memcpy_c((*ctxp)->key,key,key_size);

	//if(ek_type<6) //if 0 ecb_aes,else ccm_aes
		(*ctxp)->key_size = (key_size+15)&0xfffffff0;

	inline_cnstr_jobdesc_cipher_enckey_gen((*ctxp)->cipher_desc,(*ctxp)->key,key_size,ek_type);
/*
	if(ek_type>=6)
		(*ctxp)->key_size = (key_size+12+7)&0xfffffff8;
*/
	ret = run_descriptor_jr_sec1((*ctxp)->cipher_desc,&jr_t);
	if (ret)
		debug("Error %x\n", ret);

	memcpy_c(key,(*ctxp)->key,key_size);

	return 0;
}

caam_cipher_enckey_from_blob(struct cipher_ctx **ctxp,enum cipher_algos cipher_algo,enum atype alg_type,enum encdec dir,unsigned ek_type)
{
	void *ptr;
	int ret;
	ptr = calloc(1, (sizeof(struct cipher_ctx)+DESC_ALIGN));	//need data_alian for it contain data ptr
	if (ptr == NULL) {
		debug("Cannot allocate memory for context\n");
		return -1;
	}
	*ctxp  = (uint32_t *)(((unsigned long)ptr+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	(*ctxp)->algo = cipher_algo;
	(*ctxp)->dir = dir;
	(*ctxp)->algo_type = alg_type;
	(*ctxp)->malloc_ptr = ptr;

	return 0;
}

caam_cipher_setiv(struct cipher_ctx *ctxp,unsigned char *iv,unsigned iv_size)
{
	ctxp->iv_size = iv_size;
	memcpy_c(ctxp->iv,iv,iv_size);
}

caam_cipher_setctr0(struct cipher_ctx *ctxp,unsigned char *ctr0,unsigned iv_size,unsigned ctr0_size)
{
	ctxp->iv_size = iv_size+ctr0_size;
	memcpy_c(ctxp->iv+iv_size,ctr0,ctr0_size);
}


caam_cipher_crypt(struct cipher_ctx *ctx,unsigned char *data,unsigned data_size,int is_last)
{
	uint32_t final = 0;
	phys_addr_t addr = virt_to_phys((void *)data);
	if (ctx->sg_num >= MAX_SG_32) {
		free(ctx->malloc_ptr);
		while(1);
	}

#ifdef CONFIG_PHYS_64BIT
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_hi, (uint32_t)(addr >> 32));
#else
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_hi, 0x0);
#endif
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_lo, (uint32_t)addr);

	sec_out32(&ctx->sg_tbl[ctx->sg_num].len_flag,
		  (data_size & SG_ENTRY_LENGTH_MASK));

	ctx->sg_num++;

	if (is_last) {
		final = sec_in32(&ctx->sg_tbl[ctx->sg_num - 1].len_flag) |
			SG_ENTRY_FINAL_BIT;
		sec_out32(&ctx->sg_tbl[ctx->sg_num - 1].len_flag, final);
	}
}

caam_cipher_crypt2(struct cipher_ctx *ctx,unsigned char *data,unsigned data_size,int is_last)	//sg_num not add by before
{
	uint32_t final = 0;
	phys_addr_t addr = virt_to_phys((void *)data);
	if (ctx->sg_num >= MAX_SG_32) {
		free(ctx->malloc_ptr);
		while(1);
	}

	ctx->sg_num = 0;

#ifdef CONFIG_PHYS_64BIT
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_hi, (uint32_t)(addr >> 32));
#else
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_hi, 0x0);
#endif
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_lo, (uint32_t)addr);

	sec_out32(&ctx->sg_tbl[ctx->sg_num].len_flag,
		  (data_size & SG_ENTRY_LENGTH_MASK));

	ctx->sg_num++;

	if (is_last) {
		final = sec_in32(&ctx->sg_tbl[ctx->sg_num - 1].len_flag) |
			SG_ENTRY_FINAL_BIT;
		sec_out32(&ctx->sg_tbl[ctx->sg_num - 1].len_flag, final);
	}
}

int caam_cipher_crypt_finish(struct cipher_ctx *ctx,
	    int size)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);
	inline_cnstr_jobdesc_cipher(ctx->cipher_desc,ctx->key,ctx->key_size,ctx->iv,ctx->iv_size,(uint8_t *)ctx->sg_tbl,
			len,(uint8_t *)ctx->sg_tbl,ctx->sg_num, ctx->algo_type,ctx->algo,ctx->dir);
	ret = run_descriptor_jr(ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
		while(1);
	}

	free(ctx->malloc_ptr);
	return ret;

}

int caam_cipher_ccm_finish(struct cipher_ctx *ctx,void *src,void *dst,u8 *dmac,
	    int size)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);
	#if 0
	inline_cnstr_jobdesc_ccm_icv(ctx->cipher_desc,ctx->key,ctx->key_size,ctx->iv,ctx->iv_size,
			ctx->ext,ctx->aad_size,dmac,ctx->auth_len,(uint8_t *)ctx->sg_tbl,
			len,(uint8_t *)ctx->sg_tbl,ctx->sg_num, ctx->algo_type,ctx->algo,ctx->dir,icvoff);	//icv off
	#else
	inline_cnstr_jobdesc_ccm_icv(ctx->cipher_desc,ctx->key,ctx->key_size,ctx->iv,ctx->iv_size,
			ctx->ext,ctx->aad_size,dmac,ctx->auth_len,src,
			len,dst,0, ctx->algo_type,ctx->algo,ctx->dir,icvoff);	//icv off
	#endif
	ret = run_descriptor_jr(ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
		while(1);
	}

	free(ctx->malloc_ptr);
	return ret;

}

int caam_cipher_ccm_icv_finish(struct cipher_ctx *ctx,void *src,void *dst,u8 *dmac,
	    int size)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);
	inline_cnstr_jobdesc_ccm_icv(ctx->cipher_desc,ctx->key,ctx->key_size,ctx->iv,ctx->iv_size,
			ctx->ext,ctx->aad_size,dmac,ctx->auth_len,src,
			len,dst,0, ctx->algo_type,ctx->algo,ctx->dir,icvon);	//icv off
	ret = run_descriptor_jr(ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
		//while(1);
	}

	free(ctx->malloc_ptr);
	return ret;

}

int caam_cipher_crypt_chunks_update(struct cipher_ctx *ctx,
	    int size)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);
	inline_cnstr_jobdesc_cipher_chunks_final(ctx->cipher_desc,ctx->key,ctx->key_size,ctx->iv,ctx->iv_size,(uint8_t *)ctx->sg_tbl,
			len,(uint8_t *)ctx->sg_tbl,ctx->sg_num, ctx->algo_type,ctx->algo,ctx->dir);
	ret = run_descriptor_jr(ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
		while(1);
	}

	return ret;

}

extern void inline_cnstr_jobdesc_cipher_chunks_final(uint32_t *desc,uint8_t *key,uint32_t key_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *src, uint32_t src_size, uint8_t *dst,
			  int sg_tbl,u32 alg_type,u32 alg,u32 dir);

int caam_cipher_crypt_chunks_final(struct cipher_ctx *ctx,
	    int size)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);
	inline_cnstr_jobdesc_cipher_chunks_final(ctx->cipher_desc,ctx->key,ctx->key_size,ctx->iv,ctx->iv_size,(uint8_t *)ctx->sg_tbl,
			len,(uint8_t *)ctx->sg_tbl,ctx->sg_num, ctx->algo_type,ctx->algo,ctx->dir);
	ret = run_descriptor_jr(ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
		while(1);
	}

	free(ctx->malloc_ptr);
	return ret;

}

int caam_cipher_crypt_enckey_finish(struct cipher_ctx *ctx,
	    int size,unsigned key_type)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);
	inline_cnstr_jobdesc_cipher_enckey_do(ctx->cipher_desc,ctx->key,ctx->key_size,ctx->iv,ctx->iv_size,(uint8_t *)ctx->sg_tbl,
			len,(uint8_t *)ctx->sg_tbl,ctx->sg_num, ctx->algo_type,ctx->algo,ctx->dir,key_type);
	ret = run_descriptor_jr_sec1(ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
		while(1);
	}

	free(ctx->malloc_ptr);
	return ret;

}

int caam_cipher_crypt_enckey_from_blob_finish(struct cipher_ctx *ctx,void *keym,unsigned int keym_len,
		void *src,void *dst,unsigned int slen,unsigned int dlen,unsigned key_type)
{
	uint32_t len = 0;
	UINT8 alg,type;
	void *desc;

	int i = 0, ret = 0;
	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);

	switch(key_type)
	{
		case KEY_TYPE_AES_ECB_KEYR:
		{
			alg = AES;	type = ECB;
			break;
		}
		case KEY_TYPE_AES_CCM_KEYR:
		{
			alg = AES;	type = CCM;
			break;
		}
		case KEY_TYPE_SM4_ECB_KEYR:
		{
			alg = SM4;	type = ECB;
			break;
		}
		case KEY_TYPE_SM4_CCM_KEYR:
		{
			alg = SM4;	type = CCM;
			break;
		}
	}

	blob_decap_job_extopt_desc(ctx->cipher_desc,alg,type,keym,keym_len,src,dst,slen,dlen,PROT_OP_CMD_PROTINFO_BLACK_BLOB);
	//desc = (void *)(ctx->cipher_desc) + desc_bytes(ctx->cipher_desc);
	inline_cnstr_jobdesc_cipher_enckey_do2(ctx->cipher_desc,dst,dlen,ctx->iv,ctx->iv_size,(uint8_t *)ctx->sg_tbl,
			len,(uint8_t *)ctx->sg_tbl,ctx->sg_num, ctx->algo_type,ctx->algo,ctx->dir,key_type);
	ret = run_descriptor_jr_sec1(ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
		while(1);
	}

	free(ctx->malloc_ptr);
	return ret;

}

