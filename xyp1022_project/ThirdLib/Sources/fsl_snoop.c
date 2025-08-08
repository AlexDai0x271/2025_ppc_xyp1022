/*
 * fsl_snoop.c
 *
 *  Created on: Apr 1, 2017
 *      Author: jzj
 */
#include "desc_constr.h"
#include "common.h"
#include "desc.h"
#include "sec.h"
#include "cipher.h"
#include "hash.h"
#include "stdlib.h"
#include "fsl_hash.h"

extern struct caam_hash_template *driver_hash;


int caam_snoop_init(struct cipher_ctx **ctxp,enum cipher_algos cipher_algo,enum atype alg_type,enum encdec dir,unsigned char *ckey,unsigned ckey_size, \
		struct hmac_ctx **hctxp,void *hkey,uint32_t hkey_size,unsigned int h_alg,unsigned int h_alg_type)
{
	caam_cipher_setkey(ctxp,cipher_algo,alg_type,dir,ckey,ckey_size);
	caam_hk_init(hctxp,hkey,hkey_size,h_alg,80,h_alg_type);
}

int caam_snoop_setiv(struct cipher_ctx *ctxp,unsigned char *iv,unsigned iv_size)
{
	caam_cipher_setiv(ctxp,iv,iv_size);
}



int caam_snoop_updata(struct cipher_ctx *ctx,unsigned char *data,unsigned data_size,int is_last)
{
	caam_cipher_crypt(ctx,data,data_size,is_last);	//note,int snoop fuction,the input data is in cipher ctx
}
/*
(uint32_t *desc,uint8_t *ckey,uint32_t ckey_size,uint8_t* hkey,uint32_t hkey_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *src, uint32_t src_size, uint8_t *dst,uint8_t *icv_out,
			  int sg_tbl,u32 calg_type,u32 calg,u32 halg,u32 dir)
			  */

int caam_out_snoop_finish(struct cipher_ctx *c_ctx,struct hash_key_ctx *h_ctx,
	    unsigned char *dest_buf)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < c_ctx->sg_num; i++)
		len += (sec_in32(&c_ctx->sg_tbl[i].len_flag) &
				SG_ENTRY_LENGTH_MASK);

	inline_cnstr_jobdesc_cipher_hash_snoop(c_ctx->cipher_desc,c_ctx->key,c_ctx->key_size,
			h_ctx->key,h_ctx->key_size,c_ctx->iv,c_ctx->iv_size,(uint8_t *)c_ctx->sg_tbl,
			len,(uint8_t *)c_ctx->sg_tbl,h_ctx->hash_ctx->hash,c_ctx->sg_num,c_ctx->algo_type,
			(u32)(c_ctx->algo),h_ctx->alg,h_ctx->type,(u32)(c_ctx->dir),FIFOLD_TYPE_MSG1OUT2);

	ret = run_descriptor_jr(c_ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
	}
	else
	{
		memcpy_c(dest_buf, h_ctx->hash_ctx->hash, sizeof(h_ctx->hash_ctx->hash));
	}

	free(c_ctx->malloc_ptr);
	free(h_ctx->hash_ctx->malloc_ptr);
	free(h_ctx);
	return ret;
}

int caam_in_snoop_finish(struct cipher_ctx *c_ctx,struct hash_key_ctx *h_ctx,
	    unsigned char *dest_buf)
{
	uint32_t len = 0;
	int i = 0, ret = 0;
	for (i = 0; i < c_ctx->sg_num; i++)
		len += (sec_in32(&c_ctx->sg_tbl[i].len_flag) &
				SG_ENTRY_LENGTH_MASK);

	inline_cnstr_jobdesc_cipher_hash_snoop(c_ctx->cipher_desc,c_ctx->key,c_ctx->key_size,
			h_ctx->key,h_ctx->key_size,c_ctx->iv,c_ctx->iv_size,(uint8_t *)c_ctx->sg_tbl,
			len,(uint8_t *)c_ctx->sg_tbl,h_ctx->hash_ctx->hash,c_ctx->sg_num,c_ctx->algo_type,
			(u32)(c_ctx->algo),h_ctx->alg,h_ctx->type,(u32)(c_ctx->dir),FIFOLD_TYPE_MSG	);

	ret = run_descriptor_jr(c_ctx->cipher_desc,&jr_t);

	if (ret)
	{
		debug("Error %x\n", ret);
	}
	else
	{
		memcpy_c(dest_buf, h_ctx->hash_ctx->hash, sizeof(h_ctx->hash_ctx->hash));
	}

	free(c_ctx->malloc_ptr);
	free(h_ctx->hash_ctx->malloc_ptr);
	free(h_ctx);
	return ret;
}

