/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */

#include "common.h"
#include "desc.h"
#include "jr.h"
#include "fsl_hash.h"
#include "stdlib.h"
#include "string.h"
#include "hash.h"
#include "desc_constr.h"



struct caam_hash_template driver_hash[] = {
	{
		.name = "sha1",
		.digestsize = SHA1_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_SHA1,
	},
	{
		.name = "sha256",
		.digestsize = SHA256_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_SHA256,
	},
	{
		.name = "sha512",
		.digestsize = SHA512_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_SHA512,
	},
	{
		.name = "sm3",
		.digestsize = SM3_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_SM3,
	},
	{
		.name = "sha0",
		.digestsize = SHA0_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_SHA0,
	},
	{
		.name = "sha224",
		.digestsize = SHA224_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_SHA224,
	},
	{
		.name = "sha384",
		.digestsize = SHA384_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_SHA384,
	},
	{
		.name = "md5",
		.digestsize = MD5_DIGEST_SIZE,
		.alg_type = OP_ALG_ALGSEL_MD5,
	},
};

static enum caam_hash_algos get_hash_type(struct hash_algo *algo)
{
	if (!strcmp(algo->name, driver_hash[SHA1].name))
		return SHA1;
	else if (!strcmp(algo->name, driver_hash[SHA256].name))
		return SHA256;
	else
		return SHA512;
}

/* Create the context for progressive hashing using h/w acceleration.
 *
 * @ctxp: Pointer to the pointer of the context for hashing
 * @caam_algo: Enum for SHA1 or SHA256
 * @return 0 if ok, -ENOMEM on error
 */
int caam_hash_init(struct hash_ctx **ctxp)
{
	void *ptr;
	ptr = calloc(1, (sizeof(struct hash_ctx)+DESC_ALIGN));		//for the ctx contain data ptr,so it need data_alian
	if (ptr == NULL) {
		debug("Cannot allocate memory for context\n");
		return -1;
	}
	*ctxp = (struct hash_ctx *)(((unsigned long)ptr+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	(*ctxp)->malloc_ptr = ptr;
	return 0;
}

/*
 * Update sg table for progressive hashing using h/w acceleration
 *
 * The context is freed by this function if an error occurs.
 * We support at most 32 Scatter/Gather Entries.
 *
 * @hash_ctx: Pointer to the context for hashing
 * @buf: Pointer to the buffer being hashed
 * @size: Size of the buffer being hashed
 * @is_last: 1 if this is the last update; 0 otherwise
 * @caam_algo: Enum for SHA1 or SHA256
 * @return 0 if ok, -EINVAL on error
 */
int caam_hash_update(void *hash_ctx_p, const void *buf,
			    unsigned int size, int is_last)
{
	uint32_t final = 0;
	phys_addr_t addr = virt_to_phys((void *)buf);
	struct hash_ctx *ctx = hash_ctx_p;

	if (ctx->sg_num >= MAX_SG_32) {
		free(ctx->malloc_ptr);
		return -2;
	}

#ifdef CONFIG_PHYS_64BIT
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_hi, (uint32_t)(addr >> 32));
#else
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_hi, 0x0);
#endif
	sec_out32(&ctx->sg_tbl[ctx->sg_num].addr_lo, (uint32_t)addr);

	sec_out32(&ctx->sg_tbl[ctx->sg_num].len_flag,
		  (size & SG_ENTRY_LENGTH_MASK));

	ctx->sg_num++;

	if (is_last) {
		final = sec_in32(&ctx->sg_tbl[ctx->sg_num - 1].len_flag) |
			SG_ENTRY_FINAL_BIT;
		sec_out32(&ctx->sg_tbl[ctx->sg_num - 1].len_flag, final);
	}

	return 0;
}

/*
 * Perform progressive hashing on the given buffer and copy hash at
 * destination buffer
 *
 * The context is freed after completion of hash operation.
 *
 * @hash_ctx: Pointer to the context for hashing
 * @dest_buf: Pointer to the destination buffer where hash is to be copied
 * @size: Size of the buffer being hashed
 * @caam_algo: Enum for SHA1 or SHA256
 * @return 0 if ok, -EINVAL on error
 */

int caam_hash_finish(void *hash_ctx_p, void *dest_buf,
			    int size, enum caam_hash_algos caam_algo)
{
	struct hash_ctx *ctx= NULL;
	uint32_t len = 0;
	int i = 0, ret = 0;
	void *fptr=NULL;

	ctx = hash_ctx_p;
	fptr = ctx->malloc_ptr;

	if (size < 1) {
		free(ctx->malloc_ptr);
		return -2;
	}

	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);

	inline_cnstr_jobdesc_hash(ctx->hash_desc, (uint8_t *)ctx->sg_tbl, len,
			ctx->hash,
				  driver_hash[caam_algo].alg_type,
				  driver_hash[caam_algo].digestsize,
				  ctx->sg_num);

	ret = run_descriptor_jr(ctx->hash_desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);
	else
		memcpy_c(dest_buf, ctx->hash, sizeof(ctx->hash));

	free(fptr);
	return ret;
}

int caam_hash_finish_icvcheck(void *hash_ctx_p, void *dest_buf,
			    int size, enum caam_hash_algos caam_algo)
{
	struct hash_ctx *ctx= NULL;
	uint32_t len = 0;
	int i = 0, ret = 0;
	void *fptr=NULL;

	ctx = hash_ctx_p;
	fptr = ctx->malloc_ptr;

	if (size < 1) {
		free(ctx->malloc_ptr);
		return -2;
	}

	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);

	memcpy_c(ctx->hash,dest_buf,driver_hash[caam_algo].digestsize);

	inline_cnstr_jobdesc_hash_icvcheck(ctx->hash_desc, (uint8_t *)ctx->sg_tbl, len,
			ctx->hash,
				  driver_hash[caam_algo].alg_type,
				  driver_hash[caam_algo].digestsize,
				  ctx->sg_num);

	ret = run_descriptor_jr(ctx->hash_desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);

	free(fptr);
	return ret;
}

int caam_hash(const unsigned char *pbuf, unsigned int buf_len,
	      unsigned char *pout, enum caam_hash_algos algo)
{
	int ret = 0;
	uint32_t *desc,*p;

	p = malloc(sizeof(int) * MAX_CAAM_DESCSIZE+DESC_ALIGN);
	desc = (uint32_t *)(((unsigned long)p+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	if (!desc) {
		debug("Not enough memory for descriptor allocation\n");
		return -1;
	}

	inline_cnstr_jobdesc_hash(desc, pbuf, buf_len, pout,
				  driver_hash[algo].alg_type,
				  driver_hash[algo].digestsize,
				  0);

	ret = run_descriptor_jr(desc,&jr_t);

	free(p);
	return ret;
}

int caam_hk_init(struct hash_key_ctx **hctxp,void *key,uint32_t key_size,uint32_t alg,uint32_t digest_size,uint32_t type)
{
	struct hash_ctx *hash_ctx;
	void *p,*raw;
	raw = malloc(sizeof(struct hash_key_ctx)+DESC_ALIGN);
	p = (void *)(((unsigned long)raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	if(!p)
	{
		debug("caam_hk_init malloc error!\n");
		return -1;
	}
	*hctxp = p;
	(*hctxp)->malloc_ptr = raw;
	caam_hash_init(&hash_ctx);
	(*hctxp)->hash_ctx = hash_ctx;
	(*hctxp)->type = type;
	(*hctxp)->alg = alg;
	(*hctxp)->digest_size = digest_size;
	if(alg == OP_ALG_ALGSEL_CRC)
	{
		if(key_size==0)
		{
			;
		}else if(key_size<=32)
		{
			memcpy_c((*hctxp)->key,key,4);
		}else
		{
			memcpy_c((*hctxp)->key,key,(key_size+7)/8);
		}
	}else
	{
		memcpy_c((*hctxp)->key,key,key_size);
	}
	(*hctxp)->key_size = key_size;
	return 0;
}

int caam_hk_sp_init(struct hash_key_ctx **hctxp,void *key,uint32_t key_size,uint32_t alg,uint32_t digest_size,uint32_t type,uint32_t key_type)
{
	struct hash_ctx *hash_ctx;
	void *p,*raw;
	int ret;
	raw = malloc(sizeof(struct hash_key_ctx)+DESC_ALIGN);
	p = (void *)(((unsigned long)raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	if(!p)
	{
		debug("caam_hk_init malloc error!\n");
		return -1;
	}
	*hctxp = p;
	(*hctxp)->malloc_ptr = raw;
	caam_hash_init(&hash_ctx);
	(*hctxp)->hash_ctx = hash_ctx;
	(*hctxp)->type = type;
	(*hctxp)->alg = alg;
	(*hctxp)->digest_size = digest_size;
	if(alg == OP_ALG_ALGSEL_CRC)
	{
		if(key_size==0)
		{
			;
		}else if(key_size<=32)
		{
			memcpy_c((*hctxp)->key,key,4);
		}else
		{
			memcpy_c((*hctxp)->key,key,(key_size+7)/8);
		}
	}else
	{
		memset((*hctxp)->key,0,128);
		memcpy_c((*hctxp)->key,key,key_size);
	}

	inline_cnstr_jobdesc_gspkey(hash_ctx->hash_desc,(*hctxp)->key,key_size,(*hctxp)->iv,alg,key_type);
	ret = run_descriptor_jr_sec0(hash_ctx->hash_desc,&jr_t);
	if (ret)
		debug("Error %x\n", ret);

	return 0;
}

int caam_hk_update(struct hash_key_ctx *hmac_ctx, const void *buf,
			    unsigned int size, int is_last)
{
	struct hash_ctx *hash_ctx;
	hash_ctx = hmac_ctx->hash_ctx;
	caam_hash_update(hash_ctx,buf,size,is_last);
	return 0;
}

caam_hk_setiv(struct hash_key_ctx *hk_ctx,unsigned char *iv,unsigned iv_size)
{
	hk_ctx->iv_size = iv_size;
	memcpy_c(hk_ctx->iv,iv,iv_size);
}

int caam_hk_finish(struct hash_key_ctx *hk_ctx, void *dest_buf,
			    int size)
{
	struct hash_ctx *ctx;
	int i = 0, ret = 0;
	uint32_t len = 0;

	ctx = hk_ctx->hash_ctx;

	if (size < hk_ctx->digest_size) {
		free(ctx);
		return -2;
	}

	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);

	inline_cnstr_jobdesc_hk(ctx->hash_desc,hk_ctx->key,hk_ctx->key_size,hk_ctx->type,hk_ctx->iv,hk_ctx->iv_size,
			(uint8_t *)ctx->sg_tbl, len,
				  ctx->hash,
				  hk_ctx->alg,
				  hk_ctx->digest_size,
				  ctx->sg_num);

	ret = run_descriptor_jr(ctx->hash_desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);
	else
		memcpy_c(dest_buf, ctx->hash, hk_ctx->digest_size);

	free(ctx->malloc_ptr);
	free(hk_ctx->malloc_ptr);
	return ret;
}

int caam_hk_sp_finish(struct hash_key_ctx *hk_ctx, void *dest_buf,
			    int size,unsigned key_type)
{
	struct hash_ctx *ctx;
	int i = 0, ret = 0;
	uint32_t len = 0;

	ctx = hk_ctx->hash_ctx;

	if (size < hk_ctx->digest_size) {
		free(ctx);
		return -2;
	}

	for (i = 0; i < ctx->sg_num; i++)
		len += (sec_in32(&ctx->sg_tbl[i].len_flag) &
			SG_ENTRY_LENGTH_MASK);

	inline_cnstr_jobdesc_hk_sp(ctx->hash_desc,hk_ctx->key,hk_ctx->type,hk_ctx->iv,
			(uint8_t *)ctx->sg_tbl, len,
				  ctx->hash,
				  hk_ctx->alg,
				  hk_ctx->digest_size,
				  ctx->sg_num,key_type);

	ret = run_descriptor_jr_sec0(ctx->hash_desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);
	else
		memcpy_c(dest_buf, ctx->hash, hk_ctx->digest_size);

	free(ctx->malloc_ptr);
	free(hk_ctx->malloc_ptr);
	return ret;
}



