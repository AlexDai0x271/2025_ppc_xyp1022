/*
 * sec.c
 *
 *  Created on: Mar 23, 2017
 *      Author: jzj
 */

#include "sec_common.h"
#include "hash.h"
#include "cipher.h"
#include "std_data.h"
#include "fsl_hash.h"
#include "desc.h"

#include <stdlib.h>

#define AES_TEST
//#define SM1_TEST
#define DES_TEST
//#define DES2_TEST
//#define DES3_TEST
//#define SSF33_TEST
#define SM4_TEST
//#define SM6_TEST

extern struct caam_hash_template driver_hash[];

void sec_cmd_test()
{
	//load_store_cmd_test();
	 math_add_cmd_test();
}

void sec_cmd_single()
{

	deco_single_test();

}

void sec_hash_std_test()
{
	void *ctx;
	unsigned char dst_buf[80];
	struct std_data *sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw;
	struct std_data *sha0_raw,*sha224_raw,*sha384_raw,*md5_raw;
	STD_HASH(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	STD_HASH2(&md5_raw,&sha224_raw,&sha384_raw,&sha0_raw);

#define MD5_TEST
#define SHA0_TEST
#define SHA1_TEST
#define SHA224_TEST
#define SHA256_TEST
#define SHA384_TEST
#define SHA512_TEST
#define SM3_TEST

/////////////////////////////////////////////////////////////////
//md5
/////////////////////////////////////////////////////////////////
#ifdef MD5_TEST
		//l2b(std_sha1->data,32);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_md5->data),32,1);
	caam_hash_finish(ctx,dst_buf,16,MD5);
	//free(((struct hash_ctx *)ctx)->malloc_ptr);

	if(!memcmp_c(dst_buf,std_md5->stdout,16))
	{
		debug("std md5 test ok!\n");
	}
	else
	{
		debug("std error md5 test!\n");
		while(1);
	}

#endif

/////////////////////////////////////////////////////////////////
//sha0
/////////////////////////////////////////////////////////////////
#ifdef SHA0_TEST
	//l2b(std_sha1->data,32);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha0->data),32,1);
	caam_hash_finish(ctx,dst_buf,20,SHA0);

	if(!memcmp_c(dst_buf,std_sha0->stdout,20))
	{
		debug("std sha0 test ok!\n");
	}
	else
	{
		debug("std error sha0 test!\n");
		while(1);
	}
#endif

/////////////////////////////////////////////////////////////////
//sha1
/////////////////////////////////////////////////////////////////
#ifdef SHA1_TEST
	//l2b(std_sha1->data,32);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha1->data),32,1);
	caam_hash_finish(ctx,dst_buf,32,SHA1);

	if(!memcmp_c(dst_buf,std_sha1->stdout,20))
	{
		debug("std sha1 test ok!\n");
	}
	else
	{
		debug("std error sha1 test!\n");
		while(1);
	}
#endif
/////////////////////////////////////////////////////////////////
//sha224
/////////////////////////////////////////////////////////////////
#ifdef SHA224_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha224->data),32,1);
	caam_hash_finish(ctx,dst_buf,28,SHA224);

	if(!memcmp_c(dst_buf,std_sha224->stdout,28))
	{
		debug("std sha224 test ok!\n");
	}
	else
	{
		debug("std error sha224 test!\n");
		while(1);
	}
#endif

/////////////////////////////////////////////////////////////////
//sha256
/////////////////////////////////////////////////////////////////
#ifdef SHA256_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha256->data),32,1);
	caam_hash_finish(ctx,dst_buf,32,SHA256);

	if(!memcmp_c(dst_buf,std_sha256->stdout,32))
	{
		debug("std sha256 test ok!\n");
	}
	else
	{
		debug("std error sha256 test!\n");
		while(1);
	}
#endif

/////////////////////////////////////////////////////////////////
//sha384
/////////////////////////////////////////////////////////////////
#ifdef SHA384_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha384->data),32,1);
	caam_hash_finish(ctx,dst_buf,48,SHA384);

	if(!memcmp_c(dst_buf,std_sha384->stdout,48))
	{
		debug("std sha384 test ok!\n");
	}
	else
	{
		debug("std error sha384 test!\n");
		while(1);
	}
#endif

/////////////////////////////////////////////////////////////////
//sha512
/////////////////////////////////////////////////////////////////
#ifdef SHA512_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha512->data),32,1);
	caam_hash_finish(ctx,dst_buf,64,SHA512);

	if(!memcmp_c(dst_buf,std_sha512->stdout,64))
	{
		debug("std sha512 test ok!\n");
	}
	else
	{
		debug("std error sha512 test!\n");
		while(1);
	}
#endif

/////////////////////////////////////////////////////////////////
//sm3
/////////////////////////////////////////////////////////////////
#ifdef SM3_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sm3->data),32,1);
	caam_hash_finish(ctx,dst_buf,32,SM3);

	if(!memcmp_c(dst_buf,std_sm3->stdout,32))
	{
		debug("std sm3 test ok!\n");
	}
	else
	{
		debug("std error sm3 test!\n");
		while(1);
	}


#endif

	#undef MD5_TEST
	#undef SHA0_TEST
	#undef SHA1_TEST
	#undef SHA224_TEST
	#undef SHA256_TEST
	#undef SHA384_TEST
	#undef SHA512_TEST
	#undef SM3_TEST

	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);
	free(md5_raw);
	free(sha0_raw);
	free(sha224_raw);
	free(sha384_raw);
}

void sec_hash_std_icvcheck_test()
{
	void *ctx;
	unsigned char dst_buf[80];
	int ret;
	struct std_data *sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw;
	struct std_data *sha0_raw,*sha224_raw,*sha384_raw,*md5_raw;
	STD_HASH(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	STD_HASH2(&md5_raw,&sha224_raw,&sha384_raw,&sha0_raw);

#define MD5_TEST
#define SHA0_TEST
#define SHA1_TEST
//#define SHA224_TEST
#define SHA256_TEST
//#define SHA384_TEST
#define SHA512_TEST
#define SM3_TEST

/////////////////////////////////////////////////////////////////
//md5
/////////////////////////////////////////////////////////////////
#ifdef MD5_TEST
		//l2b(std_sha1->data,32);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_md5->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_md5->stdout,16,MD5);

	if(ret)
	{
		debug("std md5 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std md5 icvcheck test ok!\n");
	}

//////////////////////////////////////////////////////////////
	*(std_md5->data)=~*(std_md5->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_md5->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_md5->stdout,16,MD5);

	if(!ret)
	{
		debug("std md5 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std emd5 icvcheck test,ok Num.2!\n");
	}

	*(std_md5->data)=~*(std_md5->data);

#endif

/////////////////////////////////////////////////////////////////
//sha0
/////////////////////////////////////////////////////////////////
#ifdef SHA0_TEST
	//l2b(std_sha1->data,32);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha0->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha0->stdout,20,SHA0);

	if(ret)
	{
		debug("std sha0 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std sha0 icvcheck test ok!\n");
	}
//////////////////////////////////////////////////////////////
	*(std_sha0->data)=~*(std_sha0->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha0->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha0->stdout,20,SHA0);

	if(!ret)
	{
		debug("std sha0 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std sha0 icvcheck test,ok Num.2!\n");
	}

	*(std_sha0->data)=~*(std_sha0->data);
#endif

/////////////////////////////////////////////////////////////////
//sha1
/////////////////////////////////////////////////////////////////
#ifdef SHA1_TEST
	//l2b(std_sha1->data,32);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha1->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha1->stdout,32,SHA1);

	if(ret)
	{
		debug("std sha1 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std sha1 icvcheck test ok!\n");
	}

//////////////////////////////////////////////////////////////
	*(std_sha1->data)=~*(std_sha1->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha1->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha1->stdout,32,SHA1);

	if(!ret)
	{
		debug("std sha1 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std sha1 icvcheck test,ok Num.2!\n");
	}

	*(std_sha1->data)=~*(std_sha1->data);
#endif
/////////////////////////////////////////////////////////////////
//sha224
/////////////////////////////////////////////////////////////////
#ifdef SHA224_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha224->data),28,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha224->stdout,28,SHA224);

	if(ret)
	{
		debug("std sha224 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std sha224 icvcheck test ok!\n");
	}
//////////////////////////////////////////////////////////////
	*(std_sha224->data)=~*(std_sha224->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha224->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha224->stdout,32,SHA224);

	if(!ret)
	{
		debug("std sha224 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std sha224 icvcheck test,ok Num.2!\n");
	}

	*(std_sha224->data)=~*(std_sha224->data);

#endif

/////////////////////////////////////////////////////////////////
//sha256
/////////////////////////////////////////////////////////////////
#ifdef SHA256_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha256->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha256->stdout,32,SHA256);

	if(ret)
	{
		debug("std sha256 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std sha256 icvcheck test ok!\n");
	}
/////////////////////////////////////////////////////////////////
	*(std_sha256->data)=~*(std_sha256->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha256->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha256->stdout,32,SHA256);

	if(!ret)
	{
		debug("std sha256 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std sha256 icvcheck test,ok Num.2!\n");
	}

	*(std_sha256->data)=~*(std_sha256->data);
#endif

/////////////////////////////////////////////////////////////////
//sha384
/////////////////////////////////////////////////////////////////
#ifdef SHA384_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha384->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha384->stdout,48,SHA384);

	if(ret)
	{
		debug("std sha384 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std sha384 icvcheck test ok!\n");
	}
/////////////////////////////////////////////////////////////////
	*(std_sha384->data)=~*(std_sha384->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha384->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha384->stdout,48,SHA384);

	if(!ret)
	{
		debug("std sha384 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std sha384 icvcheck test,ok Num.2!\n");
	}

	*(std_sha384->data)=~*(std_sha384->data);

#endif

/////////////////////////////////////////////////////////////////
//sha512
/////////////////////////////////////////////////////////////////
#ifdef SHA512_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha512->data),32,1);
	ret = caam_hash_finish(ctx,std_sha512->stdout,64,SHA512);

	if(ret)
	{
		debug("std sha512 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std sha512 icvcheck test ok!\n");
	}
/////////////////////////////////////////////////////////////////
	*(std_sha512->data)=~*(std_sha512->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sha512->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sha512->stdout,64,SHA512);

	if(!ret)
	{
		debug("std sha512 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std sha512 icvcheck test,ok Num.2!\n");
	}

	*(std_sha512->data)=~*(std_sha512->data);
#endif

/////////////////////////////////////////////////////////////////
//sm3
/////////////////////////////////////////////////////////////////
#ifdef SM3_TEST
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sm3->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sm3->stdout,32,SM3);

	if(ret)
	{
		debug("std sm3 icvcheck test error!\n");
		while(1);
	}else
	{
		debug("std sm3 icvcheck test ok!\n");
	}
/////////////////////////////////////////////////////////////////////////
	*(std_sm3->data)=~*(std_sm3->data);
	caam_hash_init(&ctx);
	caam_hash_update(ctx,(std_sm3->data),32,1);
	ret = caam_hash_finish_icvcheck(ctx,std_sm3->stdout,32,SM3);

	if(!ret)
	{
		debug("std sm3 icvcheck test,error Num。2!\n");
		while(1);
	}else
	{
		debug("std sm3 icvcheck test,ok Num.2!\n");
	}

	*(std_sm3->data)=~*(std_sm3->data);

#endif

	#undef MD5_TEST
	#undef SHA0_TEST
	#undef SHA1_TEST
	#undef SHA224_TEST
	#undef SHA256_TEST
	#undef SHA384_TEST
	#undef SHA512_TEST
	#undef SM3_TEST

	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);
	free(md5_raw);
	free(sha0_raw);
	free(sha224_raw);
	free(sha384_raw);
}

void sec_hash_separate_test(void)
{
	struct hash_ctx *ctx;
	int ret,i;
	unsigned char dst_buf[80];
	struct std_data *sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw,*stdp;
	struct std_data *sha0_raw,*sha224_raw,*sha384_raw,*md5_raw;
	enum caam_hash_algos caam_algo = SHA1;
	int cmp_size;
	STD_HASH(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	STD_HASH2(&md5_raw,&sha224_raw,&sha384_raw,&sha0_raw);
for(i=0;i<HASH_MAX;i++)		//sizeof driver_hash is the array size of driver_hash?
{
	switch(caam_algo)
	{
		case SHA0:
			stdp = std_sha0; break;
		case SHA1:
			stdp = std_sha1; break;
		case SHA224:
			stdp = std_sha224; break;
		case SHA256:
			stdp = std_sha256; break;
		case SHA384:
			stdp = std_sha384; break;
		case SHA512:
			stdp = std_sha512; break;
		case SM3:
			stdp = std_sm3; break;
		case MD5:
		default:
			stdp = std_md5; break;
	}
///////////////////////////////////////////////
//init
///////////////////////////////////////////////
	caam_hash_init(&ctx);
	inline_cnstr_jobdesc_init_hash(ctx->hash_desc, (unsigned char *)(stdp->data),128,
				 ctx->dsize_buf ,ctx->hash,
				driver_hash[caam_algo].alg_type,
				driver_hash[caam_algo].digestsize,
				  0);


	ret = run_descriptor_jr(ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash, sizeof(ctx->hash));
	free(ctx->malloc_ptr);
/////////////////////////////////////////////////
//updata
/////////////////////////////////////////////////
	caam_hash_init(&ctx);
	memcpy_c(ctx->hash, dst_buf, sizeof(ctx->hash));
	inline_cnstr_jobdesc_update_hash(ctx->hash_desc,ctx->hash,driver_hash[caam_algo].digestsize,
			(unsigned char *)(stdp->data), 128,ctx->dsize_buf, ctx->hash,
			driver_hash[caam_algo].alg_type, driver_hash[caam_algo].digestsize, 0);
	ret = run_descriptor_jr(ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash, sizeof(ctx->hash));
	free(ctx->malloc_ptr);

///////////////////////////////////////////////////
//final
///////////////////////////////////////////////////
	caam_hash_init(&ctx);
	memcpy_c(ctx->hash, dst_buf, sizeof(ctx->hash));
	inline_cnstr_jobdesc_final_hash(ctx->hash_desc,ctx->hash,driver_hash[caam_algo].digestsize,
			(unsigned char *)(stdp->data),128,ctx->dsize_buf,ctx->hash,
				  driver_hash[caam_algo].alg_type,driver_hash[caam_algo].digestsize,0);

	ret = run_descriptor_jr(ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash, sizeof(ctx->hash));
	free(ctx->malloc_ptr);

	cmp_size = driver_hash[caam_algo].digestsize;
	if(caam_algo == SHA224) cmp_size = 28;
	if(caam_algo == SHA384) cmp_size = 48;
	if(!memcmp_c(dst_buf,&(stdp->stdout[cmp_size]),cmp_size))
		debug("std emu %x(sha1,sha256,sha512,sm3,sha0,sha224,sha384,md5) separate test ok!\n",i);
	else
	{
		debug("std error emu %x(sha1,sha256,sha512,sm3,sha0,sha224,sha384,md5) separate test!\n",i);
		while(1);
	}
	caam_algo ++;
}
	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);

	free(md5_raw);
	free(sha224_raw);
	free(sha384_raw);
	free(sha0_raw);
}

void sec_hmac_std_test()		//note that the std_sha1 now is just for hash,not for hmac;so the stdout is not really right
{
	struct hash_ctx *ctx;
	int ret,i;
	unsigned char dst_buf[80];
	struct std_data *sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw,*stdp;
	struct std_data *sha0_raw,*sha224_raw,*sha384_raw,*md5_raw;

	int cmp_size,key_size;
	STD_HMAC(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	STD_HMAC2(&md5_raw,&sha224_raw,&sha384_raw,&sha0_raw);

#define HMAC_SHA1
#define HMAC_SHA256
#define HMAC_SHA512
#define HMAC_SM3
#define HMAC_MD5
#define HMAC_SHA0
#define HMAC_SHA224
#define HMAC_SHA384

#ifdef HMAC_SHA1
	cmp_size = 20; key_size = 6;
	caam_hk_init(&ctx,std_sha1->key,key_size,OP_ALG_ALGSEL_SHA1,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha1->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sha1->stdout,cmp_size))
		debug("std hmac(sha1) test ok!\n");
	else
	{
		debug("std error hmac(sha1) test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA256
	cmp_size = 32; key_size = 6;
	caam_hk_init(&ctx,std_sha256->key,key_size,OP_ALG_ALGSEL_SHA256,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha256->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sha256->stdout,cmp_size))
		debug("std hmac(sha256) test ok!\n");
	else
	{
		debug("std error hmac(sha256) test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA512
	cmp_size = 64; key_size = 6;
	caam_hk_init(&ctx,std_sha512->key,key_size,OP_ALG_ALGSEL_SHA512,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha512->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sha512->stdout,cmp_size))
		debug("std hmac(sha512) test ok!\n");
	else
	{
		debug("std error hmac(sha512) test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SM3
	cmp_size = 32; key_size = 6;
	caam_hk_init(&ctx,std_sm3->key,key_size,OP_ALG_ALGSEL_SM3,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sm3->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sm3->stdout,cmp_size))
		debug("std hmac(sm3) test ok!\n");
	else
	{
		debug("std error hmac(sm3) test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_MD5
	cmp_size = 16; key_size = 6;
	caam_hk_init(&ctx,std_md5->key,key_size,OP_ALG_ALGSEL_MD5,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_md5->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_md5->stdout,cmp_size))
		debug("std hmac(md5) test ok!\n");
	else
	{
		debug("std error hmac(md5) test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA0
	cmp_size = 20; key_size = 6;
	caam_hk_init(&ctx,std_sha0->key,key_size,OP_ALG_ALGSEL_SHA0,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha0->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sha0->stdout,cmp_size))
		debug("std hmac(sha0) test ok!\n");
	else
	{
		debug("std error hmac(sha0) test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA224
	cmp_size = 28; key_size = 6;
	caam_hk_init(&ctx,std_sha224->key,key_size,OP_ALG_ALGSEL_SHA224,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha224->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sha224->stdout,cmp_size))
		debug("std hmac(sha224) test ok!\n");
	else
	{
		debug("std error hmac(sha224) test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA384
	cmp_size = 48; key_size = 6;
	caam_hk_init(&ctx,std_sha384->key,key_size,OP_ALG_ALGSEL_SHA384,cmp_size,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha384->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sha384->stdout,cmp_size))
		debug("std hmac(sha384) test ok!\n");
	else
	{
		debug("std error hmac(sha384) test!\n");
		asm("trap");
	}
#endif


#undef HMAC_SHA1
#undef HMAC_SHA256
#undef HMAC_SHA512
#undef HMAC_SM3
#undef HMAC_MD5
#undef HMAC_SHA0
#undef HMAC_SHA224
#undef HMAC_SHA384

	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);

	free(md5_raw);
	free(sha224_raw);
	free(sha384_raw);
	free(sha0_raw);
}

void sec_hmac_sperate_test()		//note that the std_sha1 now is just for hash,not for hmac;so the stdout is not really right
{
	struct hash_key_ctx *ctx;
	int ret,i;
	unsigned char dst_buf[80];
	struct std_data *sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw,*stdp;
	struct std_data *sha0_raw,*sha224_raw,*sha384_raw,*md5_raw;
	enum caam_hash_algos caam_algo = SHA1;
	unsigned algo_sel = 0;
	int cmp_size;
	STD_HMAC(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	STD_HMAC2(&md5_raw,&sha224_raw,&sha384_raw,&sha0_raw);
	unsigned key_size = 6;

for(i=0;i<HASH_MAX;i++)		//sizeof driver_hash is the array size of driver_hash(HMAC)
{
	switch(caam_algo)
	{
		case SHA0:
			stdp = std_sha0; algo_sel = OP_ALG_ALGSEL_SHA0; break;
		case SHA1:
			stdp = std_sha1; algo_sel = OP_ALG_ALGSEL_SHA1;  break;
		case SHA224:
			stdp = std_sha224; algo_sel = OP_ALG_ALGSEL_SHA224;  break;
		case SHA256:
			stdp = std_sha256; algo_sel = OP_ALG_ALGSEL_SHA256;  break;
		case SHA384:
			stdp = std_sha384; algo_sel = OP_ALG_ALGSEL_SHA384;  break;
		case SHA512:
			stdp = std_sha512; algo_sel = OP_ALG_ALGSEL_SHA512;  break;
		case SM3:
			stdp = std_sm3; algo_sel = OP_ALG_ALGSEL_SM3;  break;
		case MD5:
		default:
			stdp = std_md5; algo_sel = OP_ALG_ALGSEL_MD5;  break;
	}
///////////////////////////////////////////////
//init
///////////////////////////////////////////////
	caam_hk_init(&ctx,stdp->key,key_size,algo_sel,cmp_size,OP_ALG_AAI_HMAC);
	inline_cnstr_jobdesc_init_hmac(ctx->hash_ctx->hash_desc,ctx->key,key_size, (unsigned char *)(stdp->data),128,
				 ctx->hash_ctx->dsize_buf ,ctx->hash_ctx->hash,
				driver_hash[caam_algo].alg_type,
				driver_hash[caam_algo].digestsize,
				  0);


	ret = run_descriptor_jr(ctx->hash_ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash_ctx->hash, sizeof(ctx->hash_ctx->hash));
	free(ctx->hash_ctx->malloc_ptr);
	free(ctx->malloc_ptr);
/////////////////////////////////////////////////
//updata
/////////////////////////////////////////////////
	caam_hk_init(&ctx,stdp->key,key_size,algo_sel,cmp_size,OP_ALG_AAI_HMAC);
	memcpy_c(ctx->hash_ctx->hash, dst_buf, sizeof(ctx->hash_ctx->hash));
	inline_cnstr_jobdesc_update_hmac(ctx->hash_ctx->hash_desc,ctx->key,key_size, ctx->hash_ctx->hash,driver_hash[caam_algo].digestsize,
			(unsigned char *)(stdp->data), 128,ctx->hash_ctx->dsize_buf, ctx->hash_ctx->hash,
			driver_hash[caam_algo].alg_type, driver_hash[caam_algo].digestsize, 0);
	ret = run_descriptor_jr(ctx->hash_ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash_ctx->hash, sizeof(ctx->hash_ctx->hash));
	free(ctx->hash_ctx->malloc_ptr);
	free(ctx->malloc_ptr);

///////////////////////////////////////////////////
//final
///////////////////////////////////////////////////
	caam_hk_init(&ctx,stdp->key,key_size,algo_sel,cmp_size,OP_ALG_AAI_HMAC);
	memcpy_c(ctx->hash_ctx->hash, dst_buf, sizeof(ctx->hash_ctx->hash));
	inline_cnstr_jobdesc_final_hmac(ctx->hash_ctx->hash_desc,ctx->key,key_size,ctx->hash_ctx->hash,driver_hash[caam_algo].digestsize,
			(unsigned char *)(stdp->data),128,ctx->hash_ctx->dsize_buf,ctx->hash_ctx->hash,
				  driver_hash[caam_algo].alg_type,driver_hash[caam_algo].digestsize,0);

	ret = run_descriptor_jr(ctx->hash_ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash_ctx->hash, sizeof(ctx->hash_ctx->hash));
	free(ctx->hash_ctx->malloc_ptr);
	free(ctx->malloc_ptr);

	cmp_size = driver_hash[caam_algo].digestsize;
	if(caam_algo == SHA224) cmp_size = 28;
	if(caam_algo == SHA384) cmp_size = 48;
	if(!memcmp_c(dst_buf,&(stdp->stdout[cmp_size]),cmp_size))
		debug("std emu %x hmac (sha1,sha256,sha512,sm3,sha0,sha224,sha384,md5) separate test ok!\n",i);
	else
	{
		debug("std error emu %x hmac (sha1,sha256,sha512,sm3,sha0,sha224,sha384,md5) separate test!\n",i);
		while(1);
	}
	caam_algo ++;
}
	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);

	free(md5_raw);
	free(sha224_raw);
	free(sha384_raw);
	free(sha0_raw);
}

void sec_hmac_split_sperate_test()		//note that the std_sha1 now is just for hash,not for hmac;so the stdout is not really right
{
	struct hash_key_ctx *ctx;
	int ret,i;
	unsigned char dst_buf[80];
	struct std_data *sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw,*stdp;
	struct std_data *sha0_raw,*sha224_raw,*sha384_raw,*md5_raw;
	enum caam_hash_algos caam_algo = SHA1;
	unsigned algo_sel = 0;
	int cmp_size;
	STD_HMAC(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	STD_HMAC2(&md5_raw,&sha224_raw,&sha384_raw,&sha0_raw);
	unsigned key_size ;

for(i=0;i<HASH_MAX;i++)		//sizeof driver_hash is the array size of driver_hash(HMAC)
{
	switch(caam_algo)
	{
		case SHA0:
			stdp = std_sha0; algo_sel = OP_ALG_ALGSEL_SHA0; break;
		case SHA1:
			stdp = std_sha1; algo_sel = OP_ALG_ALGSEL_SHA1;  break;
		case SHA224:
			stdp = std_sha224; algo_sel = OP_ALG_ALGSEL_SHA224;  break;
		case SHA256:
			stdp = std_sha256; algo_sel = OP_ALG_ALGSEL_SHA256;  break;
		case SHA384:
			stdp = std_sha384; algo_sel = OP_ALG_ALGSEL_SHA384;  break;
		case SHA512:
			stdp = std_sha512; algo_sel = OP_ALG_ALGSEL_SHA512;  break;
		case SM3:
			stdp = std_sm3; algo_sel = OP_ALG_ALGSEL_SM3;  break;
		case MD5:
		default:
			stdp = std_md5; algo_sel = OP_ALG_ALGSEL_MD5;  break;
	}
///////////////////////////////////////////////
//init
///////////////////////////////////////////////
	key_size = 6;				//orige key size
 	caam_hk_sp_init(&ctx,stdp->key,key_size,algo_sel,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_AES_ECB_SKEYR);
	key_size = 128;				//split key size
	inline_cnstr_jobdesc_init_hmac_split(ctx->hash_ctx->hash_desc,ctx->key,key_size, (unsigned char *)(stdp->data),128,
				 ctx->hash_ctx->dsize_buf ,ctx->hash_ctx->hash,
				driver_hash[caam_algo].alg_type,
				driver_hash[caam_algo].digestsize,
				KEY_TYPE_AES_ECB_SKEYR);


	ret = run_descriptor_jr(ctx->hash_ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash_ctx->hash, sizeof(ctx->hash_ctx->hash));
	free(ctx->hash_ctx->malloc_ptr);
	free(ctx->malloc_ptr);
/////////////////////////////////////////////////
//updata
/////////////////////////////////////////////////
	key_size = 6;				//orige key size
 	caam_hk_sp_init(&ctx,stdp->key,key_size,algo_sel,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_ECB_SKEYR);
	key_size = 128;				//split key size
	memcpy_c(ctx->hash_ctx->hash, dst_buf, sizeof(ctx->hash_ctx->hash));
	inline_cnstr_jobdesc_update_hmac_split(ctx->hash_ctx->hash_desc,ctx->key,key_size, ctx->hash_ctx->hash,driver_hash[caam_algo].digestsize,
			(unsigned char *)(stdp->data), 128,ctx->hash_ctx->dsize_buf, ctx->hash_ctx->hash,
			driver_hash[caam_algo].alg_type, driver_hash[caam_algo].digestsize, 0,KEY_TYPE_SM4_ECB_SKEYR);
	ret = run_descriptor_jr(ctx->hash_ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash_ctx->hash, sizeof(ctx->hash_ctx->hash));
	free(ctx->hash_ctx->malloc_ptr);
	free(ctx->malloc_ptr);

///////////////////////////////////////////////////
//final
///////////////////////////////////////////////////
	key_size = 6;				//orige key size
 	caam_hk_sp_init(&ctx,stdp->key,key_size,algo_sel,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_CCM_SKEYR);
	key_size = 128;				//split key size
	memcpy_c(ctx->hash_ctx->hash, dst_buf, sizeof(ctx->hash_ctx->hash));
	inline_cnstr_jobdesc_final_hmac_split(ctx->hash_ctx->hash_desc,ctx->key,key_size,ctx->hash_ctx->hash,driver_hash[caam_algo].digestsize,
			(unsigned char *)(stdp->data),128,ctx->hash_ctx->dsize_buf,ctx->hash_ctx->hash,
				  driver_hash[caam_algo].alg_type,driver_hash[caam_algo].digestsize,0,KEY_TYPE_SM4_CCM_SKEYR);

	ret = run_descriptor_jr(ctx->hash_ctx->hash_desc,&jr_t);
	if(!ret)
		memcpy_c(dst_buf, ctx->hash_ctx->hash, sizeof(ctx->hash_ctx->hash));
	free(ctx->hash_ctx->malloc_ptr);
	free(ctx->malloc_ptr);

	cmp_size = driver_hash[caam_algo].digestsize;
	if(caam_algo == SHA224) cmp_size = 28;
	if(caam_algo == SHA384) cmp_size = 48;
	if(!memcmp_c(dst_buf,&(stdp->stdout[cmp_size]),cmp_size))
		debug("std emu %x hmac split (sha1,sha256,sha512,sm3,sha0,sha224,sha384,md5) separate test ok!\n",i);
	else
	{
		debug("std error emu %x hmac split (sha1,sha256,sha512,sm3,sha0,sha224,sha384,md5) separate test!\n",i);
		while(1);
	}
	caam_algo ++;
}
	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);

	free(md5_raw);
	free(sha224_raw);
	free(sha384_raw);
	free(sha0_raw);
}

void sec_hmac_splitkey_test()		//note that the std_sha1 now is just for hash,not for hmac;so the stdout is not really right
{
	struct hash_key_ctx *ctx;
	int ret,i;
	unsigned char dst_buf[80];
	struct std_data *sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw,*stdp;
	struct std_data *sha0_raw,*sha224_raw,*sha384_raw,*md5_raw;

	int cmp_size,key_size;
	STD_HMAC(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	STD_HMAC2(&md5_raw,&sha224_raw,&sha384_raw,&sha0_raw);

#define HMAC_SHA1
#define HMAC_SHA256
#define HMAC_SHA512
#define HMAC_SM3
#define HMAC_MD5
#define HMAC_SHA0
#define HMAC_SHA224
#define HMAC_SHA384

#ifdef HMAC_SHA1
	cmp_size = 20; key_size = 6;
	caam_hk_sp_init(&ctx,std_sha1->key,key_size,OP_ALG_ALGSEL_SHA1,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_AES_ECB_SKEYR);
	caam_hk_update(ctx,std_sha1->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_AES_ECB_SKEYR);
	if(!memcmp_c(dst_buf,std_sha1->stdout,cmp_size))
		debug("std hmac(sha1) splitkey test ok!\n");
	else
	{
		debug("std error hmac(sha1) splitkey test!\n");
		asm("trap");
	}

#endif

#ifdef HMAC_SHA256
	cmp_size = 32; key_size = 6;
	caam_hk_sp_init(&ctx,std_sha256->key,key_size,OP_ALG_ALGSEL_SHA256,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_AES_CCM_SKEYR);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha256->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_AES_CCM_SKEYR);
	if(!memcmp_c(dst_buf,std_sha256->stdout,cmp_size))
		debug("std hmac(sha256) splitkey test ok!\n");
	else
	{
		debug("std error hmac(sha256) splitkey test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA512
	cmp_size = 64; key_size = 6;
	caam_hk_sp_init(&ctx,std_sha512->key,key_size,OP_ALG_ALGSEL_SHA512,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_ECB_SKEYR);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha512->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_SM4_ECB_SKEYR);
	if(!memcmp_c(dst_buf,std_sha512->stdout,cmp_size))
		debug("std hmac(sha512) splitkey test ok!\n");
	else
	{
		debug("std error hmac(sha512) splitkey test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SM3
	cmp_size = 32; key_size = 6;
	caam_hk_sp_init(&ctx,std_sm3->key,key_size,OP_ALG_ALGSEL_SM3,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_CCM_SKEYR);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sm3->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_SM4_CCM_SKEYR);
	if(!memcmp_c(dst_buf,std_sm3->stdout,cmp_size))
		debug("std hmac(sm3) test splitkey ok!\n");
	else
	{
		debug("std error hmac(sm3) splitkey test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_MD5
	cmp_size = 16; key_size = 6;
	caam_hk_sp_init(&ctx,std_md5->key,key_size,OP_ALG_ALGSEL_MD5,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_CCM_SKEYR);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_md5->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_SM4_CCM_SKEYR);
	if(!memcmp_c(dst_buf,std_md5->stdout,cmp_size))
		debug("std hmac(md5) splitkey test ok!\n");
	else
	{
		debug("std error hmac(md5) splitkey test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA0
	cmp_size = 20; key_size = 6;
	caam_hk_sp_init(&ctx,std_sha0->key,key_size,OP_ALG_ALGSEL_SHA0,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_CCM_SKEYR);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha0->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_SM4_CCM_SKEYR);
	if(!memcmp_c(dst_buf,std_sha0->stdout,cmp_size))
		debug("std hmac(sha0) splitkey test ok!\n");
	else
	{
		debug("std error hmac(sha0) splitkey test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA224
	cmp_size = 28; key_size = 6;
	caam_hk_sp_init(&ctx,std_sha224->key,key_size,OP_ALG_ALGSEL_SHA224,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_CCM_SKEYR);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha224->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_SM4_CCM_SKEYR);
	if(!memcmp_c(dst_buf,std_sha224->stdout,cmp_size))
		debug("std hmac(sha224) splitkey test ok!\n");
	else
	{
		debug("std error hmac(sha224) splitkey test!\n");
		asm("trap");
	}
#endif

#ifdef HMAC_SHA384
	cmp_size = 48; key_size = 6;
	caam_hk_sp_init(&ctx,std_sha384->key,key_size,OP_ALG_ALGSEL_SHA384,cmp_size,OP_ALG_AAI_HMAC_PRECOMP,KEY_TYPE_SM4_CCM_SKEYR);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha384->data,32,1);
	caam_hk_sp_finish(ctx,dst_buf,cmp_size,KEY_TYPE_SM4_CCM_SKEYR);
	if(!memcmp_c(dst_buf,std_sha384->stdout,cmp_size))
		debug("std hmac(sha384) splitkey test ok!\n");
	else
	{
		debug("std error hmac(sha384) splitkey test!\n");
		asm("trap");
	}
#endif

#undef HMAC_SHA1
#undef HMAC_SHA256
#undef HMAC_SHA512
#undef HMAC_SM3
#undef HMAC_MD5
#undef HMAC_SHA0
#undef HMAC_SHA224
#undef HMAC_SHA384


	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);

	free(md5_raw);
	free(sha224_raw);
	free(sha384_raw);
	free(sha0_raw);
}

void sec_smac_std_test()		//note that the std_sha1 now is just for hash,not for hmac;so the stdout is not really right
{
	struct hash_ctx *ctx;
	int ret,i;
	unsigned char dst_buf[64];
	struct std_data *sha1_raw;
	struct std_data *md5_raw;

	int cmp_size,key_size;
	STD_SMAC(&sha1_raw,&md5_raw);

#define SMAC_SHA1
#define SMAC_MD5

#ifdef SMAC_SHA1
	cmp_size = 20; key_size = 6;
	caam_hk_init(&ctx,std_sha1->key,key_size,OP_ALG_ALGSEL_SHA1,cmp_size,OP_ALG_AAI_SMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_sha1->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_sha1->stdout,cmp_size))
		debug("std smac(sha1) test ok!\n");
	else
	{
		debug("std error smac(sha1) test!\n");
		asm("trap");
	}
#endif

#ifdef SMAC_MD5
	cmp_size = 32; key_size = 6;
	caam_hk_init(&ctx,std_md5->key,key_size,OP_ALG_ALGSEL_MD5,cmp_size,OP_ALG_AAI_SMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_md5->data,32,1);
	caam_hk_finish(ctx,dst_buf,cmp_size);
	if(!memcmp_c(dst_buf,std_md5->stdout,cmp_size))
		debug("std smac(md5) test ok!\n");
	else
	{
		debug("std error smac(md5) test!\n");
		asm("trap");
	}
#endif

}
////////////////////////////////////////////////////////////////
//	AES
////////////////////////////////////////////////////////////////
#ifdef AES_TEST
void sec_cipher_std_ecb_aes_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[16];
	STD_AES(&aes_raw);
	memcpy_c(dt,std_aes->data,16);
#ifdef ALG_BIG
	l2b(std_aes->key,16);
	l2b(std_aes->data,16);
#endif

	caam_cipher_setkey(&ctx,AES,ECB,enc,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	l2b(std_aes->stdout,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout,16))
		debug("std ecb(aes) test ok!\n");
	else
	{
		debug("std error ecb(aes) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,ECB,dec,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std ecb(aes) dec test ok!\n");
	else
	{
		debug("std error ecb(aes) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(aes) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,ECB,enc,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,ECB,dec,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(aes) enc dec test ok!\n");
		else
		{
			debug("rand error ecb(aes) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(aes_raw);
}

void sec_cipher_std_ecb_aes192_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[16];
	STD_AES(&aes_raw);
	memcpy_c(dt,std_aes->data,16);
#ifdef ALG_BIG
	l2b(std_aes->key+16,24);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,ECB,enc,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	l2b(std_aes->stdout+16,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+16,16))
		debug("std ecb(aes192) test ok!\n");
	else
	{
		debug("std error ecb(aes192) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,ECB,dec,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std ecb(aes192) dec test ok!\n");
	else
	{
		debug("std error ecb(aes192) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(aes192) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,ECB,enc,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,ECB,dec,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(aes192) enc dec test ok!\n");
		else
		{
			debug("rand error ecb(aes192) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(aes_raw);
}

void sec_cipher_std_ecb_aes256_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[16];
	STD_AES(&aes_raw );
	memcpy_c(dt,std_aes->data,16);
#ifdef ALG_BIG
	l2b(std_aes->key+40,32);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,ECB,enc,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	l2b(std_aes->stdout+32,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+32,16))
		debug("std ecb(aes256) test ok!\n");
	else
	{
		debug("std error ecb(aes256) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,ECB,dec,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std ecb(aes256) dec test ok!\n");
	else
	{
		debug("std error ecb(aes256) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(aes256) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,ECB,enc,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,ECB,dec,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(aes256) enc dec test ok!\n");
		else
		{
			debug("rand error ecb(aes256) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(aes_raw);
}

void sec_cipher_std_cfb_aes_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES2(&aes_raw );
	memcpy_c(dt,std_aes->data,32);
#ifdef ALG_BIG
	l2b(std_aes->key,16);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,CFB,enc,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	l2b(std_aes->stdout+48,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+48,32))
		debug("std cfb(aes) test ok!\n");
	else
	{
		debug("std error cfb(aes) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CFB,dec,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);
	if(!memcmp_c(std_aes->data,dt,32))
		debug("std cfb(aes) dec test ok!\n");
	else
	{
		debug("std error cfb(aes) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(aes) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CFB+i,enc,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CFB+i,dec,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(aes) enc dec test ok!\n");
		else
		{
			debug("rand error cfb(aes) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(aes_raw);
}

sec_cipher_jdkek_init()
{
	void *ctx;
	caam_cipher_jdkek_gen(&ctx);
}

void sec_cipher_std_cfb_1_aes_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES4(&aes_raw );
	memcpy_c(dt,std_aes->data,16);
#ifdef ALG_BIG
	l2b(std_aes->key,16);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,CFB1,enc,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	l2b(std_aes->stdout+48,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout,16))
		debug("std cfb_1(aes) test ok!\n");
	else
	{
		debug("std error cfb_1(aes) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CFB1,dec,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std cfb_1(aes) dec test ok!\n");
	else
	{
		debug("std error cfb_1(aes) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb_1(aes) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CFB1,enc,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CFB1,dec,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb_1(aes) enc dec test ok!\n");
		else
		{
			debug("rand error cfb_1(aes) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(aes_raw);
}

void sec_cipher_std_cfb_aes192_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES2(&aes_raw );
	memcpy_c(dt,std_aes->data,32);
#ifdef ALG_BIG
	l2b(std_aes->key+16,24);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,CFB,enc,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	l2b(std_aes->stdout+64,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+80,32))
		debug("std cfb(aes192) test ok!\n");
	else
	{
		debug("std error cfb(aes192) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CFB,dec,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);
	if(!memcmp_c(std_aes->data,dt,32))
		debug("std cfb(aes192) dec test ok!\n");
	else
	{
		debug("std error cfb(aes192) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(aes192) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CFB+i,enc,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CFB+i,dec,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(aes192) enc dec test ok!\n");
		else
		{
			debug("rand error cfb(aes) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(aes_raw);
}

void sec_cipher_std_cfb_aes256_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES2(&aes_raw );
	memcpy_c(dt,std_aes->data,32);
#ifdef ALG_BIG
	l2b(std_aes->key+40,32);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,CFB,enc,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	l2b(std_aes->stdout+80,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+112,32))
		debug("std cfb(aes256) test ok!\n");
	else
	{
		debug("std error cfb(aes256) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CFB,dec,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);
	if(!memcmp_c(std_aes->data,dt,32))
		debug("std cfb(aes256) dec test ok!\n");
	else
	{
		debug("std error cfb(aes256) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(aes256) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CFB+i,enc,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CFB+i,dec,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(aes256) enc dec test ok!\n");
		else
		{
			debug("rand error cfb(aes256) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}


	free(aes_raw);
}


void sec_cipher_std_ofb_aes_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES3(&aes_raw );
	memcpy_c(dt,std_aes->data,32);
#ifdef ALG_BIG
	l2b(std_aes->key,16);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,OFB,enc,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	l2b(std_aes->stdout+48,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout,32))
		debug("std ofb(aes) test ok!\n");
	else
	{
		debug("std error ofb(aes) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,OFB,dec,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);
	if(!memcmp_c(std_aes->data,dt,32))
		debug("std ofb(aes) dec test ok!\n");
	else
	{
		debug("std error ofb(aes) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(aes) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,OFB+i,enc,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,OFB+i,dec,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(aes) enc dec test ok!\n");
		else
		{
			debug("rand error ofb(aes) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(aes_raw);
}

void sec_cipher_std_ofb_aes192_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES3(&aes_raw );
	memcpy_c(dt,std_aes->data,32);
#ifdef ALG_BIG
	l2b(std_aes->key+16,24);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,OFB,enc,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	l2b(std_aes->stdout+64,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+32,32))
		debug("std ofb(aes192) test ok!\n");
	else
	{
		debug("std error ofb(aes192) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,OFB,dec,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);
	if(!memcmp_c(std_aes->data,dt,32))
		debug("std ofb(aes192) dec test ok!\n");
	else
	{
		debug("std error ofb(aes192) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(aes192) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,OFB+i,enc,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,OFB+i,dec,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(aes192) enc dec test ok!\n");
		else
		{
			debug("rand error ofb(aes) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(aes_raw);
}

void sec_cipher_std_ofb_aes256_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES3(&aes_raw );
	memcpy_c(dt,std_aes->data,32);
#ifdef ALG_BIG
	l2b(std_aes->key+40,32);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,OFB,enc,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	l2b(std_aes->stdout+80,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+64,32))
		debug("std ofb(aes256) test ok!\n");
	else
	{
		debug("std error ofb(aes256) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,OFB,dec,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,32,1);
	caam_cipher_crypt_finish(ctx,32);
	if(!memcmp_c(std_aes->data,dt,32))
		debug("std ofb(aes256) dec test ok!\n");
	else
	{
		debug("std error ofb(aes256) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(aes256) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,OFB+i,enc,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,OFB+i,dec,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(aes256) enc dec test ok!\n");
		else
		{
			debug("rand error ofb(aes256) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}


	free(aes_raw);
}



void sec_cipher_std_cbc_aes_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[16];
	STD_AES2(&aes_raw );
	memcpy_c(dt,std_aes->data,16);

#ifdef ALG_BIG
	l2b(std_aes->key,16);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,CBC,enc,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	l2b(std_aes->stdout,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout,16))
		debug("std cbc(aes) test ok!\n");
	else
	{
		debug("std error cbc(aes) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CBC,dec,std_aes->key,16);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std cbc(aes) dec test ok!\n");
	else
	{
		debug("std error cbc(aes) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(aes) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CBC,enc,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);

		if(rand_size>0x1000)
		{
			caam_cipher_crypt(ctx,rand_data,0x800,0);
			caam_cipher_crypt(ctx,rand_data+0x900,rand_size-0x900,1);
		}
		else

			caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CBC,dec,std_aes->key,16);
		caam_cipher_setiv(ctx,std_aes->iv,16);

		if(rand_size>0x1000)
		{
			caam_cipher_crypt(ctx,rand_data,0x800,0);
			caam_cipher_crypt(ctx,rand_data+0x900,rand_size-0x900,1);
		}
		else

			caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(aes) enc dec test ok!\n");
		else
		{
			debug("rand error cbc(aes) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(aes_raw);
}

void sec_cipher_std_cbc_aes192_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[16];
	STD_AES2(&aes_raw );
	memcpy_c(dt,std_aes->data,16);
#ifdef ALG_BIG
	l2b(std_aes->key+16,24);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,CBC,enc,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	l2b(std_aes->stdout+16,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+16,16))
		debug("std cbc(aes192) test ok!\n");
	else
	{
		debug("std error cbc(aes192) test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CBC,dec,std_aes->key+16,24);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std cbc(aes192) dec test ok!\n");
	else
	{
		debug("std error cbc(aes192) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(aes192) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CBC,enc,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CBC,dec,std_aes->key+16,24);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(aes192) enc dec test ok!\n");
		else
		{
			debug("rand error cbc(aes192) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(aes_raw);
}

void sec_cipher_std_cbc_aes256_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[16];
	STD_AES2(&aes_raw );
	memcpy_c(dt,std_aes->data,16);
#ifdef ALG_BIG
	l2b(std_aes->key+40,32);
	l2b(std_aes->iv,16);
	l2b(std_aes->data,16);
#endif
	caam_cipher_setkey(&ctx,AES,CBC,enc,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	l2b(std_aes->stdout+32,16);
#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout+32,16))
		debug("std cbc(aes256) test ok!\n");
	else
	{
		debug("std error cbc(aes256) test!\n");
		while(1);
	}


	caam_cipher_setkey(&ctx,AES,CBC,dec,std_aes->key+40,32);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std cbc(aes256) dec test ok!\n");
	else
	{
		debug("std error cbc(aes256) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(aes256) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CBC,enc,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CBC,dec,std_aes->key+40,32);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(aes256) enc dec test ok!\n");
		else
		{
			debug("rand error cbc(aes256) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}


	free(aes_raw);
}

#endif //AES_TEST

////////////////////////////////////////////////////////////////
//	DES
////////////////////////////////////////////////////////////////
#ifdef DES_TEST
void sec_cipher_std_ecb_des_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES,ECB,enc,std_des->key,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_des->data,std_des->stdout,16))
		debug("std ecb(des) enc test ok!\n");
	else
	{
		debug("std error ecb(des) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES,ECB,dec,std_des->key,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_des->data,dt,16))
		debug("std ecb(des) dec test ok!\n");
	else
	{
		debug("std error ecb(des) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES,ECB,enc,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES,ECB,dec,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(des) enc dec test ok!\n");
		else
		{
			debug("rand error ecb(des) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(des_raw);
	free(des3_raw);

}

void sec_cipher_std_cbc_des_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data+32,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES,CBC,enc,std_des->key+16,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_des->data+32,std_des->stdout+32,16))
		debug("std cbc(des) enc test ok!\n");
	else
	{
		debug("std error cbc(des) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES,CBC,dec,std_des->key+16,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_des->data+32,dt,16))
		debug("std cbc(des) dec test ok!\n");
	else
	{
		debug("std error cbc(des) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES,CBC,enc,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES,CBC,dec,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(des) enc dec test ok!\n");
		else
		{
			debug("rand error cbc(des) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(des_raw);
	free(des3_raw);
}

void sec_cipher_std_cfb_des_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V2(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data+32,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES,CFB,enc,std_des->key+16,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_des->data+32,std_des->stdout+32,16))
		debug("std cfb64(des) enc test ok!\n");
	else
	{
		debug("std error cfb64(des) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES,CFB,dec,std_des->key+16,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_des->data+32,dt,16))
		debug("cfb64(des) dec test ok!\n");
	else
	{
		debug("std error cfb64(des) dec test!\n");
		while(1);
	}

	for(i=0;i<5;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES,CFB+i,enc,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES,CFB+i,dec,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(des) enc dec test ok!\n");
		else
		{
			debug("rand error cfb(des) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(des_raw);
	free(des3_raw);
}
void sec_cipher_std_ofb_des_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V2(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data+32,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES,OFB,enc,std_des->key+16,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_des->data+32,std_des->stdout+48,16))
		debug("std ofb64(des) enc test ok!\n");
	else
	{
		debug("std error ofb64(des) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES,OFB,dec,std_des->key+16,8);
	caam_cipher_setiv(ctx,std_des->iv,8);
	caam_cipher_crypt(ctx,std_des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_des->data+32,dt,16))
		debug("std ofb64(des) dec test ok!\n");
	else
	{
		debug("std error ofb64(des) dec test!\n");
		while(1);
	}

	for(i=0;i<5;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb64(des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES,OFB+i,enc,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES,OFB+i,dec,std_des->key,8);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(des) enc dec test ok!\n");
		else
		{
			debug("rand error ofb(des) enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(des_raw);
	free(des3_raw);
}
#endif //DES_TEST
////////////////////////////////////////////////////////////////
//	2DES
////////////////////////////////////////////////////////////////
#ifdef DES2_TEST
void sec_cipher_std_ecb_des2_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V1(&des_raw,&des3_raw);
	memcpy_c(dt,std_3des->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,ECB,enc,std_3des->key,16);
	caam_cipher_setiv(ctx,std_3des->iv,16);
	caam_cipher_crypt(ctx,std_3des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data,std_3des->stdout,16))
		debug("std ecb(des3) key128 enc test ok!\n");
	else
	{
		debug("std error ecb(des3) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,ECB,dec,std_3des->key,16);
	caam_cipher_setiv(ctx,std_3des->iv,16);
	caam_cipher_crypt(ctx,std_3des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_3des->data,dt,16))
		debug("std ecb(des3) key128 dec test ok!\n");
	else
	{
		debug("std error ecb(des3) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(des3) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,ECB,enc,std_3des->key,16);
		caam_cipher_setiv(ctx,std_3des->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,ECB,dec,std_3des->key,16);
		caam_cipher_setiv(ctx,std_3des->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(des3) key128 enc dec test ok!\n");
		else
		{
			debug("rand error ecb(des3) key128 enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(des_raw);
	free(des3_raw);

}

void sec_cipher_std_cbc_des2_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V1(&des_raw,&des3_raw);
	memcpy_c(dt,std_3des->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,CBC,enc,std_3des->key,16);
	caam_cipher_setiv(ctx,std_3des->iv,16);
	caam_cipher_crypt(ctx,std_3des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data,std_3des->stdout+16,16))
		debug("std cbc(des3) key128 enc test ok!\n");
	else
	{
		debug("std error cbc(des3) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,CBC,dec,std_3des->key,16);
	caam_cipher_setiv(ctx,std_3des->iv,16);
	caam_cipher_crypt(ctx,std_3des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_3des->data,dt,16))
		debug("std cbc(des3) key128 dec test ok!\n");
	else
	{
		debug("std error cbc(des3) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(des3) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,CBC,enc,std_3des->key,16);
		caam_cipher_setiv(ctx,std_3des->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,CBC,dec,std_3des->key,16);
		caam_cipher_setiv(ctx,std_3des->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(des3) key128 enc dec test ok!\n");
		else
		{
			debug("rand error cbc(des3) key128 enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(des_raw);
	free(des3_raw);

}

void sec_cipher_std_cfb_des2_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V1(&des_raw,&des3_raw);
	memcpy_c(dt,std_3des->data+16,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,CFB,enc,std_3des->key+16,16);
	caam_cipher_setiv(ctx,std_3des->iv+16,8);
	caam_cipher_crypt(ctx,std_3des->data+16,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data+16,std_3des->stdout+32,16))
		debug("std cfb(des3) key128 enc test ok!\n");
	else
	{
		debug("std error cfb(des3) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,CFB,dec,std_3des->key+16,16);
	caam_cipher_setiv(ctx,std_3des->iv+16,8);
	caam_cipher_crypt(ctx,std_3des->data+16,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_3des->data+16,dt,16))
		debug("std cfb(des3) key128 dec test ok!\n");
	else
	{
		debug("std error cfb(des3) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(des3) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,CFB,enc,std_3des->key+16,16);
		caam_cipher_setiv(ctx,std_3des->iv+16,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,CFB,dec,std_3des->key+16,16);
		caam_cipher_setiv(ctx,std_3des->iv+16,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(des3) key128 enc dec test ok!\n");
		else
		{
			debug("rand error cfb(des3) key128 enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(des_raw);
	free(des3_raw);

}

void sec_cipher_std_ofb_des2_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V1(&des_raw,&des3_raw);
	memcpy_c(dt,std_3des->data+32,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,OFB,enc,std_3des->key+32,16);
	caam_cipher_setiv(ctx,std_3des->iv+16,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data+32,std_3des->stdout+48,16))
		debug("std ofb(des3) key128 enc test ok!\n");
	else
	{
		debug("std error ofb(des3) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,OFB,dec,std_3des->key+32,16);
	caam_cipher_setiv(ctx,std_3des->iv+16,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_3des->data+32,dt,16))
		debug("std ofb(des3) key128 dec test ok!\n");
	else
	{
		debug("std error ofb(des3) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(des3) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,OFB,enc,std_3des->key+32,16);
		caam_cipher_setiv(ctx,std_3des->iv+16,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,OFB,dec,std_3des->key+32,16);
		caam_cipher_setiv(ctx,std_3des->iv+16,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(des3) key128 enc dec test ok!\n");
		else
		{
			debug("rand error ofb(des3) key128 enc dec test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(des_raw);
	free(des3_raw);

}
#endif //DES3_TEST

////////////////////////////////////////////////////////////////
//	3DES
////////////////////////////////////////////////////////////////
#ifdef DES3_TEST
void sec_cipher_std_ecb_des3_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,ECB,enc,std_3des->key,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data,std_3des->stdout,16))
		debug("std ecb(des3) enc test ok!\n");
	else
	{
		debug("std error ecb(des3) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,ECB,dec,std_3des->key,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data,16,1);
	caam_cipher_crypt_finish(ctx,16);
	if(!memcmp_c(std_3des->data,dt,16))
		debug("std ecb(des3) dec test ok!\n");
	else
	{
		debug("std error ecb(des3) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(3des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,ECB,enc,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,ECB,dec,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(des3) enc dec test ok!\n");
		else
		{
			debug("rand ecb(des3) enc dec test  error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(des_raw);
	free(des3_raw);
}

void sec_cipher_std_cbc_des3_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned dt[16];
	STD_DES(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data+32,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,CBC,enc,std_3des->key+48,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data+32,std_3des->stdout+32,16))
		debug("std cbc(des3) enc test ok!\n");
	else
	{
		debug("std error cbc(des3) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,CBC,dec,std_3des->key+48,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_3des->data+32,dt,16))
		debug("std cbc(des3) dec test ok!\n");
	else
	{
		debug("std error cbc(des3) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(3des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,CBC,enc,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,CBC,dec,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(des3) enc dec test ok!\n");
		else
		{
			debug("rand cbc(des3) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(des_raw);
	free(des3_raw);
}

void sec_cipher_std_cfb_des3_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V2(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data+32,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,CFB,enc,std_3des->key+48,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data+32,std_3des->stdout+32,16))
		debug("std cfb64(des3) enc test ok!\n");
	else
	{
		debug("std error cfb64(des3) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,CFB,dec,std_3des->key+48,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_3des->data+32,dt,16))
		debug("std cfb64(des3) dec test ok!\n");
	else
	{
		debug("std error cfb64(des3) dec test!\n");
		while(1);
	}
	for(i=0;i<5;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(3des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,CFB+i,enc,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,CFB+i,dec,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(des3) enc dec test ok!\n");
		else
		{
			debug("rand cfb(des3) enc dec test  error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(des_raw);
	free(des3_raw);
}

void sec_cipher_std_ofb_des3_test()
{
	void *ctx;
	void *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_DES_V2(&des_raw,&des3_raw);
	memcpy_c(dt,std_des->data+32,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,DES3,OFB,enc,std_3des->key+48,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_3des->data+32,std_3des->stdout+48,16))
		debug("std ofb64(des3) enc test ok!\n");
	else
	{
		debug("std error ofb64(des3) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,DES3,OFB,dec,std_3des->key+48,24);
	caam_cipher_setiv(ctx,std_3des->iv,8);
	caam_cipher_crypt(ctx,std_3des->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_3des->data+32,dt,16))
		debug("std ofb64(des3) dec test ok!\n");
	else
	{
		debug("std error ofb64(des3) dec test!\n");
		while(1);
	}

	for(i=0;i<5;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(3des) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,OFB+i,enc,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,OFB+i,dec,std_des->key,24);
		caam_cipher_setiv(ctx,std_des->iv,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(des3) enc dec test ok!\n");
		else
		{
			debug("rand ofb(des3) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(des_raw);
	free(des3_raw);
}
#endif //DES3_TEST
////////////////////////////////////////////////////////////////
//	SM1
////////////////////////////////////////////////////////////////
#ifdef SM1_TEST
void sec_cipher_std_ecb_sm1_key256_test0()
{
	void *ctx;
	void *sm1,*sm1_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SM1(&sm1_raw);
	memcpy_c(dt,std_sm1->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM1,ECB,enc,std_sm1->key,32);
	//caam_cipher_setkey(&ctx,SM1,ECB,enc,std_sm1->key,16);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm1->data,std_sm1->stdout,16))
		debug("std ecb(sm1) enc test ok!\n");
	else
	{
		debug("std error ecb(sm1) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM1,ECB,dec,std_sm1->key,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_sm1->data,dt,16))
		debug("std ecb(sm1) dec test ok!\n");
	else
	{
		debug("std error ecb(sm1) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(sm1) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM1,ECB,enc,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM1,ECB,dec,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(sm1) enc dec test ok!\n");
		else
		{
			debug("rand ecb(sm1) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm1_raw);
}

void sec_cipher_std_ecb_sm1_key256_test()
{
	void *ctx;
	void *sm1,*sm1_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SM1_V2(&sm1_raw);
	memcpy_c(dt,std_sm1->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM1,ECB,enc,std_sm1->key,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm1->data,std_sm1->stdout,16))
		debug("std ecb(sm1) key256 enc test ok!\n");
	else
	{
		debug("std error ecb(sm1) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM1,ECB,dec,std_sm1->key,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_sm1->data,dt,16))
		debug("std ecb(sm1) key256 dec test ok!\n");
	else
	{
		debug("std error ecb(sm1) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(sm1) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM1,ECB,enc,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM1,ECB,dec,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(sm1) key256 enc dec test ok!\n");
		else
		{
			debug("rand ecb(sm1) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm1_raw);
}

void sec_cipher_std_ecb_sm1_key384_test()
{
	void *ctx;
	void *sm1,*sm1_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SM1_V2(&sm1_raw);
	memcpy_c(dt,std_sm1->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM1,ECB|AAI_SK_SL,enc,std_sm1->key,48);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm1->data,std_sm1->stdout+32,16))
		debug("std ecb(sm1) key384 enc test ok!\n");
	else
	{
		debug("std error ecb(sm1) key384 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM1,ECB|AAI_SK_SL,dec,std_sm1->key,48);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_sm1->data,dt,16))
		debug("std ecb(sm1) key384 dec test ok!\n");
	else
	{
		debug("std error ecb(sm1) key384 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(sm1) key384 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM1,ECB|AAI_SK_SL,enc,std_sm1->key,48);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM1,ECB|AAI_SK_SL,dec,std_sm1->key,48);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(sm1) key384 enc dec test ok!\n");
		else
		{
			debug("rand ecb(sm1) key384 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm1_raw);
}


void sec_cipher_std_cbc_sm1_key256_test0()
{
	void *ctx;
	void *sm1,*sm1_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM1(&sm1_raw);
	memcpy_c(dt,std_sm1->data+16,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM1,CBC,enc,std_sm1->key+32,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm1->data+16,std_sm1->stdout+16,32))
		debug("std cbc(sm1) enc test ok!\n");
	else
	{
		debug("std error cbc(sm1) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM1,CBC,dec,std_sm1->key+32,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm1->data+16,dt,32))
		debug("std cbc(sm1) dec test ok!\n");
	else
	{
		debug("std error cbc(sm1) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(sm1) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM1,CBC,enc,std_sm1->key+32,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM1,CBC,dec,std_sm1->key+32,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
		{
			debug("rand cbc(sm1) enc dec test ok!\n");
		}
		else
		{
			debug("rand cbc(sm1) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm1_raw);
}

void sec_cipher_std_cfb128_sm1_key256_test()
{
	void *ctx;
	void *sm1,*sm1_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM1_V2(&sm1_raw);
	memcpy_c(dt,std_sm1->data+16,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM1,CFB,enc,std_sm1->key,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm1->data+16,std_sm1->stdout,32))
		debug("std cfb128(sm1) enc test ok!\n");
	else
	{
		debug("std error cfb128(sm1) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM1,CFB,dec,std_sm1->key,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm1->data+16,dt,32))
		debug("std cfb128(sm1) dec test ok!\n");
	else
	{
		debug("std error cfb128(sm1) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(sm1) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM1,CFB+i,enc,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM1,CFB+i,dec,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
		{
			debug("rand cfb(sm1) enc dec test ok!\n");
		}
		else
		{
			debug("rand cfb(sm1) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm1_raw);
}

void sec_cipher_std_ofb128_sm1_key256_test()
{
	void *ctx;
	void *sm1,*sm1_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM1_V2(&sm1_raw);
	memcpy_c(dt,std_sm1->data+16,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM1,OFB,enc,std_sm1->key,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm1->data+16,std_sm1->stdout+48,32))
		debug("std ofb128(sm1) enc test ok!\n");
	else
	{
		debug("std error ofb128(sm1) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM1,OFB,dec,std_sm1->key,32);
	caam_cipher_setiv(ctx,std_sm1->iv,16);
	caam_cipher_crypt(ctx,std_sm1->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm1->data+16,dt,32))
		debug("std ofb128(sm1) dec test ok!\n");
	else
	{
		debug("std error ofb128(sm1) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(sm1) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM1,OFB+i,enc,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM1,OFB+i,dec,std_sm1->key,32);
		caam_cipher_setiv(ctx,std_sm1->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
		{
			debug("rand ofb(sm1) enc dec test ok!\n");
		}
		else
		{
			debug("rand ofb(sm1) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm1_raw);
}
#endif //SM1_TEST
////////////////////////////////////////////////////////////////
//	SSF33
////////////////////////////////////////////////////////////////
#ifdef SSF33_TEST
void sec_cipher_std_ecb_ssf33_key128_test0()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SSF33(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SSF33,ECB,enc,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_ssf33->data,std_ssf33->stdout,16))
		debug("std ecb(ssf33) key128 enc test ok!\n");
	else
	{
		debug("std error ecb(ssf33) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,ECB,dec,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_ssf33->data,dt,16))
		debug("std ecb(ssf33) key128 dec test ok!\n");
	else
	{
		debug("std error ecb(ssf33) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(ssf33) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,ECB,enc,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,ECB,dec,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(ssf33) key128 enc dec test ok!\n");
		else
		{
			debug("rand ecb(ssf33) key128 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_cbc_ssf33_key128_test0()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SSF33(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data+32,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,32);
#endif
	caam_cipher_setkey(&ctx,SSF33,CBC,enc,std_ssf33->key+32,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,32);
#endif

	if(!memcmp_c(std_ssf33->data+32,std_ssf33->stdout+32,32))
		debug("std cbc(ssf33) key128 enc test ok!\n");
	else
	{
		debug("std error cbc(ssf33) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,CBC,dec,std_ssf33->key+32,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_ssf33->data+32,dt,32))
		debug("std cbc(ssf33) key128 dec test ok!\n");
	else
	{
		debug("std error cbc(ssf33) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(ssf33) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,CBC,enc,std_ssf33->key+32,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,CBC,dec,std_ssf33->key+32,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(ssf33) key128 enc dec test ok!\n");
		else
		{
			debug("rand cbc(ssf33) key128 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_cbc_ssf33_key128()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SSF33_V2(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,32);
#endif
	caam_cipher_setkey(&ctx,SSF33,CBC,enc,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,32);
#endif

	if(!memcmp_c(std_ssf33->data,std_ssf33->stdout+32,16))
		debug("std cbc(ssf33) key128 enc test ok!\n");
	else
	{
		debug("std error cbc(ssf33) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,CBC,dec,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_ssf33->data,dt,16))
		debug("std cbc(ssf33) key128 dec test ok!\n");
	else
	{
		debug("std error cbc(ssf33) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(ssf33) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,CBC,enc,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,CBC,dec,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(ssf33) key128 enc dec test ok!\n");
		else
		{
			debug("rand cbc(ssf33) key128 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_cfb_ssf33_key128()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SSF33_V2(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,32);
#endif
	caam_cipher_setkey(&ctx,SSF33,CFB,enc,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,32);
#endif

	if(!memcmp_c(std_ssf33->data,std_ssf33->stdout,32))
		debug("std cfb(ssf33) key128 enc test ok!\n");
	else
	{
		debug("std error cfb(ssf33) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,CFB,dec,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_ssf33->data,dt,32))
		debug("std cfb(ssf33) key128 dec test ok!\n");
	else
	{
		debug("std error cfb(ssf33) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(ssf33) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,CFB+i,enc,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,CFB+i,dec,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(ssf33) key128 enc dec test ok!\n");
		else
		{
			debug("rand cfb(ssf33) key128 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_ofb_ssf33_key128()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SSF33_V2(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,32);
#endif
	caam_cipher_setkey(&ctx,SSF33,OFB,enc,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,32);
#endif

	if(!memcmp_c(std_ssf33->data,std_ssf33->stdout+48,32))
		debug("std ofb(ssf33) key128 enc test ok!\n");
	else
	{
		debug("std error ofb(ssf33) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,OFB,dec,std_ssf33->key,16);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_ssf33->data,dt,32))
		debug("std ofb(ssf33) dec key128 test ok!\n");
	else
	{
		debug("std error ofb(ssf33) key128 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(ssf33) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,OFB+i,enc,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,OFB+i,dec,std_ssf33->key,16);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(ssf33) key128 enc dec test ok!\n");
		else
		{
			debug("rand ofb(ssf33) key128 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_ecb_ssf33_key256()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SSF33_V3(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data,32);

#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SSF33,ECB|AAI_SK_SL,enc,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_ssf33->data,std_ssf33->stdout,32))
		debug("std ecb(ssf33) key256 enc test ok!\n");
	else
	{
		debug("std error ecb(ssf33) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,ECB|AAI_SK_SL,dec,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_ssf33->data,dt,32))
		debug("std ecb(ssf33) key256 dec test ok!\n");
	else
	{
		debug("std error ecb(ssf33) key256 dec test!\n");
		while(1);
	}

#if 1
	caam_cipher_setkey(&ctx,SSF33,ECB|AAI_SK_SL,enc,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_ssf33->data,std_ssf33->stdout,32))
		debug("std ecb(ssf33) key256 enc test ok!\n");
	else
	{
		debug("std error ecb(ssf33) key256 enc test!\n");
		while(1);
	}

#endif

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(ssf33) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,ECB|AAI_SK_SL,enc,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,ECB|AAI_SK_SL,dec,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(ssf33) key256 enc dec test ok!\n");
		else
		{
			debug("rand ecb(ssf33) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_cbc_ssf33_key256()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SSF33_V3(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SSF33,CBC|AAI_SK_SL,enc,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_ssf33->data,std_ssf33->stdout+32,16))
		debug("std cbc(ssf33) key256 enc test ok!\n");
	else
	{
		debug("std error cbc(ssf33) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,CBC|AAI_SK_SL,dec,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_ssf33->data,dt,16))
		debug("std cbc(ssf33) key256 dec test ok!\n");
	else
	{
		debug("std error cbc(ssf33) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(ssf33) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,CBC|AAI_SK_SL,enc,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,CBC|AAI_SK_SL,dec,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(ssf33) key256 enc dec test ok!\n");
		else
		{
			debug("rand cbc(ssf33) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_cfb_ssf33_key256()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SSF33_V3(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data+32,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SSF33,CFB|AAI_SK_SL,enc,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_ssf33->data+32,std_ssf33->stdout+48,32))
		debug("std cfb(ssf33) key256 enc test ok!\n");
	else
	{
		debug("std error cfb(ssf33) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,CFB|AAI_SK_SL,dec,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_ssf33->data+32,dt,32))
		debug("std cfb(ssf33) key256 dec test ok!\n");
	else
	{
		debug("std error cfb(ssf33) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(ssf33) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,(CFB+1)|AAI_SK_SL,enc,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,(CFB+1)|AAI_SK_SL,dec,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(ssf33) key256 enc dec test ok!\n");
		else
		{
			debug("rand cfb(ssf33) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}

void sec_cipher_std_ofb_ssf33_key256()
{
	void *ctx;
	void *ssf33,*ssf33_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SSF33_V3(&ssf33_raw);
	memcpy_c(dt,std_ssf33->data+32,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SSF33,OFB|AAI_SK_SL,enc,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_ssf33->data+32,std_ssf33->stdout+80,32))
		debug("std ofb(ssf33) key256 enc test ok!\n");
	else
	{
		debug("std error ofb(ssf33) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SSF33,OFB|AAI_SK_SL,dec,std_ssf33->key,32);
	caam_cipher_setiv(ctx,std_ssf33->iv,16);
	caam_cipher_crypt(ctx,std_ssf33->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_ssf33->data+32,dt,32))
		debug("std ofb(ssf33) key256 dec test ok!\n");
	else
	{
		debug("std error cfb(ssf33) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(ssf33) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SSF33,(OFB+1)|AAI_SK_SL,enc,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SSF33,(OFB+1)|AAI_SK_SL,dec,std_ssf33->key,32);
		caam_cipher_setiv(ctx,std_ssf33->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(ssf33) key256 enc dec test ok!\n");
		else
		{
			debug("rand ofb(ssf33) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(ssf33_raw);
}
#endif //SSF33_TEST

////////////////////////////////////////////////////////////////
//	SM4
////////////////////////////////////////////////////////////////
#ifdef SM4_TEST
void sec_cipher_std_ecb_sm4_test()
{
	void *ctx;
	void *sm4,*sm4_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SMS4(&sm4_raw);
	memcpy_c(dt,std_sm4->data,16);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM4,ECB,enc,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm4->data,std_sm4->stdout,16))
		debug("std ecb(sm4) enc test ok!\n");
	else
	{
		debug("std error ecb(sm4) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM4,ECB,dec,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_sm4->data,dt,16))
		debug("std ecb(sm4) dec test ok!\n");
	else
	{
		debug("std error ecb(sm4) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(sm4) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM4,ECB,enc,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM4,ECB,dec,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(sm4) enc dec test ok!\n");
		else
		{
			debug("rand ecb(sm4) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm4_raw);
}

void sec_cipher_std_cbc_sm4_test()
{
	void *ctx;
	void *sm4,*sm4_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SMS4(&sm4_raw);
	memcpy_c(dt,std_sm4->data+16,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM4,CBC,enc,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm4->data+16,std_sm4->stdout+16,32))
		debug("std cbc(sm4) enc test ok!\n");
	else
	{
		debug("std error cbc(sm4) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM4,CBC,dec,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm4->data+16,dt,32))
		debug("std cbc(sm4) dec test ok!\n");
	else
	{
		debug("std error cbc(sm4) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(sm4) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM4,CBC,enc,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM4,CBC,dec,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(sm4) enc dec test ok!\n");
		else
		{
			debug("rand cbc(sm4) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm4_raw);
}

void sec_cipher_std_cfb128_sm4_test()
{
	void *ctx;
	void *sm4,*sm4_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SMS4(&sm4_raw);
	memcpy_c(dt,std_sm4->data+16,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM4,CFB,enc,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm4->data+16,std_sm4->stdout+48,32))
		debug("std cfb(sm4) enc test ok!\n");
	else
	{
		debug("std error cfb(sm4) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM4,CFB,dec,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm4->data+16,dt,32))
		debug("std cfb(sm4) dec test ok!\n");
	else
	{
		debug("std error cfb(sm4) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(sm4) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM4,CFB+i,enc,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM4,CFB+i,dec,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(sm4) enc dec test ok!\n");
		else
		{
			debug("rand cfb(sm4) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm4_raw);
}

void sec_cipher_std_ofb_sm4_test()
{
	void *ctx;
	void *sm4,*sm4_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SMS4(&sm4_raw);
	memcpy_c(dt,std_sm4->data+16,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM4,OFB,enc,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm4->data+16,std_sm4->stdout+80,32))
		debug("std ofb(sm4) enc test ok!\n");
	else
	{
		debug("std error ofb(sm4) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM4,OFB,dec,std_sm4->key,16);
	caam_cipher_setiv(ctx,std_sm4->iv,16);
	caam_cipher_crypt(ctx,std_sm4->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm4->data+16,dt,32))
		debug("std ofb(sm4) dec test ok!\n");
	else
	{
		debug("std error ofb(sm4) dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(sm4) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM4,OFB+i,enc,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM4,OFB+i,dec,std_sm4->key,16);
		caam_cipher_setiv(ctx,std_sm4->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(sm4) enc dec test ok!\n");
		else
		{
			debug("rand ofb(sm4) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm4_raw);
}
#endif //SM4_TEST

////////////////////////////////////////////////////////////////
//	SM6
////////////////////////////////////////////////////////////////
#ifdef SM6_TEST
void sec_cipher_std_ecb_sm6_test()				//AK is default 0,so key is 128bit
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SM6(&sm6_raw);
	memcpy_c(dt,std_sm6->data,16);

	caam_cipher_setkey(&ctx,SM6,ECB,enc,std_sm6->key,16);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,16,1);
	caam_cipher_crypt_finish(ctx,16);


	if(!memcmp_c(std_sm6->data,std_sm6->stdout,16))
		debug("std ecb(sm6) enc test ok!\n");
	else
	{
		debug("std error ecb(sm6) enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,ECB,dec,std_sm6->key,16);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_sm6->data,dt,16))
		debug("std ecb(sm6) dec test ok!\n");
	else
	{
		debug("std error ecb(sm6) dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(sm6) enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,ECB,enc,std_sm6->key,16);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,ECB,dec,std_sm6->key,16);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(sm6) enc dec test ok!\n");
		else
		{
			debug("rand ecb(sm6) enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_ecb_sm6_key384()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SM6(&sm6_raw);
	memcpy_c(dt,std_sm6->data+16,16);

	caam_cipher_setkey(&ctx,SM6,ECB|AAI_SK_SL,enc,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+16,16,1);
	caam_cipher_crypt_finish(ctx,16);


	if(!memcmp_c(std_sm6->data+16,std_sm6->stdout+16,16))
		debug("std ecb(sm6) key384 enc test ok!\n");
	else
	{
		debug("std error ecb(sm6) key384 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,ECB|AAI_SK_SL,dec,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+16,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_sm6->data+16,dt,16))
		debug("std ecb(sm6) key384 dec test ok!\n");
	else
	{
		debug("std error ecb(sm6) key384 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(sm6) key384 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,ECB|AAI_SK_SL,enc,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,ECB|AAI_SK_SL,dec,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(sm6) key384 enc dec test ok!\n");
		else
		{
			debug("rand ecb(sm6) key384 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_ecb_sm6_key256()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[16];
	STD_SM6(&sm6_raw);
	memcpy_c(dt,std_sm6->data+32,16);

	caam_cipher_setkey(&ctx,SM6,ECB,enc,std_sm6->key,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);


	if(!memcmp_c(std_sm6->data+32,std_sm6->stdout+32,16))
		debug("std ecb(sm6) key256 enc test ok!\n");
	else
	{
		debug("std error ecb(sm6) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,ECB,dec,std_sm6->key,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+32,16,1);
	caam_cipher_crypt_finish(ctx,16);

	if(!memcmp_c(std_sm6->data+32,dt,16))
		debug("std ecb(sm6) key256 dec test ok!\n");
	else
	{
		debug("std error ecb(sm6) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ecb(sm6) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,ECB,enc,std_sm6->key,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,ECB,dec,std_sm6->key,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(sm6) key256 enc dec test ok!\n");
		else
		{
			debug("rand ecb(sm6) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_cbc_sm6_key256()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6(&sm6_raw);
	memcpy_c(dt,std_sm6->data+32,32);

	caam_cipher_setkey(&ctx,SM6,CBC,enc,std_sm6->key+48,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data+32,std_sm6->stdout+48,32))
		debug("std cbc(sm6) key256 enc test ok!\n");
	else
	{
		debug("std error cbc(sm6) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,CBC,dec,std_sm6->key+48,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data+32,dt,32))
		debug("std cbc(sm6) key256 dec test ok!\n");
	else
	{
		debug("std error cbc(sm6) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(sm6) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,CBC,enc,std_sm6->key+48,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,CBC,dec,std_sm6->key+48,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(sm6) key256 enc dec test ok!\n");
		else
		{
			debug("rand cbc(sm6) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_cbc_sm6_key384()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6(&sm6_raw);
	memcpy_c(dt,std_sm6->data+32,32);

	caam_cipher_setkey(&ctx,SM6,CBC|AAI_SK_SL,enc,std_sm6->key+48,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data+32,std_sm6->stdout+80,32))
		debug("std cbc(sm6) key384 enc test ok!\n");
	else
	{
		debug("std error cbc(sm6) key384 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,CBC|AAI_SK_SL,dec,std_sm6->key+48,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data+32,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data+32,dt,32))
		debug("std cbc(sm6) key384 dec test ok!\n");
	else
	{
		debug("std error cbc(sm6) key384 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cbc(sm6) key384 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,CBC|AAI_SK_SL,enc,std_sm6->key+48,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,CBC|AAI_SK_SL,dec,std_sm6->key+48,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(sm6) key384 enc dec test ok!\n");
		else
		{
			debug("rand cbc(sm6) key384 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}


void sec_cipher_std_cfb128_sm6_key256()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6_V2(&sm6_raw);
	memcpy_c(dt,std_sm6->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM6,CFB,enc,std_sm6->key,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm6->data,std_sm6->stdout+32,32))
		debug("std cfb(sm6) key256 enc test ok!\n");
	else
	{
		debug("std error cfb(sm6) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,CFB,dec,std_sm6->key,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data,dt,32))
		debug("std cfb(sm6) key256 dec test ok!\n");
	else
	{
		debug("std error cfb(sm6) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(sm6) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,CFB+1,enc,std_sm6->key,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,CFB+1,dec,std_sm6->key,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(sm6) key256 enc dec test ok!\n");
		else
		{
			debug("rand cfb(sm6) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_cfb128_sm6_key384()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6_V2(&sm6_raw);
	memcpy_c(dt,std_sm6->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM6,CFB|AAI_SK_SL,enc,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm6->data,std_sm6->stdout,32))
		debug("std cfb(sm6) key384 enc test ok!\n");
	else
	{
		debug("std error cfb(sm6) key384 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,CFB|AAI_SK_SL,dec,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data,dt,32))
		debug("std cfb(sm6) key384 dec test ok!\n");
	else
	{
		debug("std error cfb(sm6) key384 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("cfb(sm6) key384 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,(CFB+1)|AAI_SK_SL,enc,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,(CFB+1)|AAI_SK_SL,dec,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cfb(sm6) key384 enc dec test ok!\n");
		else
		{
			debug("rand cfb(sm6) key384 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_ofb_sm6_key256()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6_V2(&sm6_raw);
	memcpy_c(dt,std_sm6->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM6,OFB,enc,std_sm6->key,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm6->data,std_sm6->stdout+64,32))
		debug("std ofb(sm6) key256 enc test ok!\n");
	else
	{
		debug("std error ofb(sm6) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,OFB,dec,std_sm6->key,32);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data,dt,32))
		debug("std ofb(sm6) dec key256 test ok!\n");
	else
	{
		debug("std error ofb(sm6) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(sm6) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,OFB+i,enc,std_sm6->key,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,OFB+i,dec,std_sm6->key,32);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(sm6) key256 enc dec test ok!\n");
		else
		{
			debug("rand ofb(sm6) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_ofb_sm6_key384()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6_V2(&sm6_raw);
	memcpy_c(dt,std_sm6->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM6,OFB|AAI_SK_SL,enc,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm6->data,std_sm6->stdout+96,32))
		debug("std ofb(sm6) key384 enc test ok!\n");
	else
	{
		debug("std error ofb(sm6) key384 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,OFB|AAI_SK_SL,dec,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,16);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data,dt,32))
		debug("std ofb(sm6) dec key384 test ok!\n");
	else
	{
		debug("std error ofb(sm6) key384 dec test!\n");
		while(1);
	}

	for(i=0;i<6;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ofb(sm6) key384 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,(OFB+1)|AAI_SK_SL,enc,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,(OFB+1)|AAI_SK_SL,dec,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ofb(sm6) key384 enc dec test ok!\n");
		else
		{
			debug("rand ofb(sm6) key384 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_ctr_sm6_key256()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6_V3(&sm6_raw);
	memcpy_c(dt,std_sm6->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM6,CTR,enc,std_sm6->key+48,32);
	caam_cipher_setiv(ctx,std_sm6->iv,32);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm6->data,std_sm6->stdout+32,32))
		debug("std ctr(sm6) key256 enc test ok!\n");
	else
	{
		debug("std error ctr(sm6) key256 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,CTR,dec,std_sm6->key+48,32);
	caam_cipher_setiv(ctx,std_sm6->iv,32);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data,dt,32))
		debug("std ctr(sm6) dec key256 test ok!\n");
	else
	{
		debug("std error ctr(sm6) key256 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ctr(sm6) key256 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,OFB,enc,std_sm6->key+48,32);
		caam_cipher_setiv(ctx,std_sm6->iv,32);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,OFB,dec,std_sm6->key+48,32);
		caam_cipher_setiv(ctx,std_sm6->iv,32);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ctr(sm6) key256 enc dec test ok!\n");
		else
		{
			debug("rand ctr(sm6) key256 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

void sec_cipher_std_ctr_sm6_key384()
{
	void *ctx;
	void *sm6,*sm6_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size,i;
	unsigned char dt[32];
	STD_SM6_V3(&sm6_raw);
	memcpy_c(dt,std_sm6->data,32);
#ifdef ALG_BIG
	//l2b(std_des->key,16);
	//l2b(std_des->data,16);
#endif
	caam_cipher_setkey(&ctx,SM6,CTR|AAI_SK_SL,enc,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,32);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

#ifdef ALG_BIG
	//l2b(std_des->stdout,16);
#endif

	if(!memcmp_c(std_sm6->data,std_sm6->stdout,32))
		debug("std ctr(sm6) key384 enc test ok!\n");
	else
	{
		debug("std error ctr(sm6) key384 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,SM6,CTR|AAI_SK_SL,dec,std_sm6->key,48);
	caam_cipher_setiv(ctx,std_sm6->iv,32);
	caam_cipher_crypt(ctx,std_sm6->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_sm6->data,dt,32))
		debug("std ctr(sm6) dec key384 test ok!\n");
	else
	{
		debug("std error ctr(sm6) key384 dec test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ctr(sm6) key384 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM6,CTR|AAI_SK_SL,enc,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,32);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,SM6,CTR|AAI_SK_SL,dec,std_sm6->key,48);
		caam_cipher_setiv(ctx,std_sm6->iv,32);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ctr(sm6) key384 enc dec test ok!\n");
		else
		{
			debug("rand ctr(sm6) key384 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(sm6_raw);
}

#endif //SM6_TEST

sec_aes128_ctr_nomal_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES(&aes_raw);
	memcpy_c(dt,std_aes->data,16);

	caam_cipher_setkey(&ctx,AES,CTR,enc,std_aes->key,16);
	//caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_setctr0(ctx,std_aes->iv,16,16);
	caam_cipher_crypt(ctx,std_aes->data+16,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_aes->data+16,std_aes->stdout+48,32))
		debug("std ctr(aes) nomal test ok!\n");
	else
	{
		debug("std error ctr(aes) nomal test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ctr(aes) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,AES,CTR,enc,std_aes->key,16);
		caam_cipher_setctr0(ctx,std_aes->iv,16,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,AES,CTR,dec,std_aes->key,16);
		caam_cipher_setctr0(ctx,std_aes->iv,16,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ctr(aes) key128 enc dec test ok!\n");
		else
		{
			debug("rand ctr(aes) key128 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	free(aes_raw);

}

sec_3des_ctr_test()
{
	void *ctx;
	struct std_data *des_raw,*des3_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_DES_V3(&des_raw,&des3_raw);
	memcpy_c(dt,std_3des->data,32);

	caam_cipher_setkey(&ctx,DES3,CTR,enc,std_3des->key,24);
	caam_cipher_setctr0(ctx,std_3des->iv,8,8);
	caam_cipher_crypt(ctx,std_3des->data,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(!memcmp_c(std_3des->data,std_3des->stdout,32))
		debug("std ctr(3des) nomal test ok!\n");
	else
	{
		debug("std error ctr(3des) nomal test!\n");
		while(1);
	}

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("ctr(3des) key128 enc dec rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,DES3,CTR,enc,std_3des->key,24);
		caam_cipher_setctr0(ctx,std_3des->iv,8,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		caam_cipher_setkey(&ctx,DES3,CTR,dec,std_3des->key,24);
		caam_cipher_setctr0(ctx,std_3des->iv,8,8);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_finish(ctx,rand_size);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ctr(3des) key192 enc dec test ok!\n");
		else
		{
			debug("rand ctr(3des) key192 enc dec test error!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}


	free(des_raw);
	free(des3_raw);
}


sec_aes128_ctr_chunks_test()
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[32];
	STD_AES(&aes_raw);
	memcpy_c(dt,std_aes->data,16);

	caam_cipher_setkey(&ctx,AES,CTR,enc,std_aes->key,16);
	//caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_setctr0(ctx,std_aes->iv,16,16);
	caam_cipher_crypt(ctx,std_aes->data+16,16,1);
	caam_cipher_crypt_chunks_update(ctx,16);

	caam_cipher_crypt2(ctx,std_aes->data+32,16,1);
	caam_cipher_crypt_chunks_final(ctx,16);

	if(!memcmp_c(std_aes->data+16,std_aes->stdout+48,32))
		debug("std ctr(aes) chunks test ok!\n");
	else
	{
		debug("std error ctr(aes) chunks test!\n");
		while(1);
	}

	free(aes_raw);

}

sec_aes128_ctr_test()
{
	//sec_aes128_ctr_nomal_test();
	sec_aes128_ctr_chunks_test();		//need to check if ok 9-18

}

sec_cipher_ctr_test()
{
	sec_aes128_ctr_test();
//	sec_3des_ctr_test();
}

void sec_cipher_std_test()
{
///////////////////////////////////////////
//	AES
///////////////////////////////////////////
#ifdef AES_TEST
	uart_printf_buf("AES test start!\r\n");
	sec_cipher_std_ecb_aes_test();
	sec_cipher_std_ecb_aes192_test();
	sec_cipher_std_ecb_aes256_test();


	sec_cipher_std_cbc_aes_test();
	sec_cipher_std_cbc_aes192_test();
	sec_cipher_std_cbc_aes256_test();

	sec_cipher_std_cfb_aes_test();
	sec_cipher_std_cfb_aes192_test();
	sec_cipher_std_cfb_aes256_test();

	sec_cipher_std_cfb_1_aes_test();

	sec_cipher_std_ofb_aes_test();
	sec_cipher_std_ofb_aes192_test();
	sec_cipher_std_ofb_aes256_test();


#endif
///////////////////////////////////////////
//	DES
///////////////////////////////////////////
#ifdef DES_TEST
	uart_printf_buf("DES test start!\r\n");
	sec_cipher_std_ecb_des_test();
	sec_cipher_std_cbc_des_test();
	sec_cipher_std_cfb_des_test();
	sec_cipher_std_ofb_des_test();
#endif
///////////////////////////////////////////
//	2DES
///////////////////////////////////////////
#ifdef DES2_TEST
	sec_cipher_std_ecb_des2_test();
	sec_cipher_std_cbc_des2_test();
	sec_cipher_std_cfb_des2_test();
	sec_cipher_std_ofb_des2_test();

#endif
///////////////////////////////////////////
//	3DES
///////////////////////////////////////////
#ifdef DES3_TEST
	sec_cipher_std_ecb_des3_test();
	sec_cipher_std_cbc_des3_test();
	sec_cipher_std_cfb_des3_test();
	sec_cipher_std_ofb_des3_test();
#endif
///////////////////////////////////////////
//	SM1
///////////////////////////////////////////
#ifdef SM1_TEST

	sec_cipher_std_ecb_sm1_key256_test0();
	sec_cipher_std_cbc_sm1_key256_test0();
	sec_cipher_std_ecb_sm1_key256_test();
	sec_cipher_std_ecb_sm1_key384_test();

	sec_cipher_std_cfb128_sm1_key256_test();
	sec_cipher_std_ofb128_sm1_key256_test();
#endif
///////////////////////////////////////////
//	SSF33
///////////////////////////////////////////
#ifdef SSF33_TEST
/*
	//for key size not sync,key128 not use here,if use may be error.
	sec_cipher_std_ecb_ssf33_key128_test0();
	//sec_cipher_std_cbc_ssf33_key128_test0();

	sec_cipher_std_cbc_ssf33_key128();
	sec_cipher_std_cfb_ssf33_key128();
	sec_cipher_std_ofb_ssf33_key128();
*/
	sec_cipher_std_ecb_ssf33_key256();
	sec_cipher_std_cbc_ssf33_key256();
	sec_cipher_std_cfb_ssf33_key256();
	sec_cipher_std_ofb_ssf33_key256();
#endif

///////////////////////////////////////////
//	SM4
///////////////////////////////////////////
#ifdef SM4_TEST
	sec_cipher_std_ecb_sm4_test();
	sec_cipher_std_cbc_sm4_test();
	sec_cipher_std_cfb128_sm4_test();
	sec_cipher_std_ofb_sm4_test();
#endif

///////////////////////////////////////////
//	SM6
///////////////////////////////////////////
#ifdef SM6_TEST

	//sec_cipher_std_ecb_sm6_test();			//for key size not sync,key128 not use here,if use may be error.

	sec_cipher_std_ecb_sm6_key384();
	sec_cipher_std_ecb_sm6_key256();
	sec_cipher_std_cbc_sm6_key256();
	sec_cipher_std_cbc_sm6_key384();
	sec_cipher_std_cfb128_sm6_key256();
	sec_cipher_std_cfb128_sm6_key384();
	sec_cipher_std_ofb_sm6_key256();
	sec_cipher_std_ofb_sm6_key384();

	sec_cipher_std_ctr_sm6_key256();
	sec_cipher_std_ctr_sm6_key384();

#endif
}

void sec_cipher_std_enckey_test(void)
{
	void *ctx;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	unsigned char dt[16];
	unsigned char key_tmp[32];
	STD_AES(&aes_raw);
	memcpy_c(key_tmp,std_aes->key,16);
	memcpy_c(dt,std_aes->data,16);
	#ifdef ALG_BIG
	l2b(std_aes->key,16);
	l2b(std_aes->data,16);
	#endif
//aes_ecb

	caam_cipher_setkey_enckey(&ctx,AES,ECB,enc,std_aes->key,16,KEY_TYPE_AES_ECB_KEYR);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_enckey_finish(ctx,16,KEY_TYPE_AES_ECB_KEYR);

	#ifdef ALG_BIG
	l2b(std_aes->stdout,16);
	#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout,16))
		debug("std ecb(aes) enckey(ecb_aes) test ok!\n");
	else
	{
		debug("std error ecb(aes) enckey(ecb_aes) test!\n");
		while(1);
	}

//aes_ccm
		memcpy_c(std_aes->key,key_tmp,16);
		memcpy_c(std_aes->data,dt,16);
		caam_cipher_setkey_enckey(&ctx,AES,ECB,enc,std_aes->key,16,KEY_TYPE_AES_CCM_KEYR);
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,std_aes->data,16,1);
		caam_cipher_crypt_enckey_finish(ctx,16,KEY_TYPE_AES_CCM_KEYR);

		#ifdef ALG_BIG
		l2b(std_aes->stdout,16);
		#endif

		if(!memcmp_c(std_aes->data,std_aes->stdout,16))
			debug("std ecb(aes) enckey(ccm_aes) test ok!\n");
		else
		{
			debug("std error ecb(aes) enckey(ccm_aes) test!\n");
			while(1);
		}

//sm4_ecb
	memcpy_c(std_aes->key,key_tmp,16);
	memcpy_c(std_aes->data,dt,16);
	caam_cipher_setkey_enckey(&ctx,AES,ECB,enc,std_aes->key,16,KEY_TYPE_SM4_ECB_KEYR);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_enckey_finish(ctx,16,KEY_TYPE_SM4_ECB_KEYR);

	#ifdef ALG_BIG
	l2b(std_aes->stdout,16);
	#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout,16))
		debug("std ecb(aes) enckey(sm4_ecb) ok!\n");
	else
	{
		debug("std error ecb(aes) enckey(sm4_ecb) test!\n");
		while(1);
	}
//
	memcpy_c(std_aes->key,key_tmp,16);
	memcpy_c(std_aes->data,dt,16);
	caam_cipher_setkey_enckey(&ctx,AES,ECB,enc,std_aes->key,16,KEY_TYPE_SM4_CCM_KEYR);
	caam_cipher_setiv(ctx,std_aes->iv,16);
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_crypt_enckey_finish(ctx,16,KEY_TYPE_SM4_CCM_KEYR);

	#ifdef ALG_BIG
	l2b(std_aes->stdout,16);
	#endif

	if(!memcmp_c(std_aes->data,std_aes->stdout,16))
		debug("std ecb(aes) enckey(sm4_ccm) ok!\n");
	else
	{
		debug("std error ecb(aes) enckey(sm4_ccm) test!\n");
		while(1);
	}

	free(aes_raw);
}

void sec_cipher_random_enckey_test(void)
{
	void *ctx;
	struct cipher_ctx *ctxp;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int i,rand_size;
	u8 dk[32];

	STD_AES(&aes_raw);
	memcpy_c(dk,std_aes->key,32);

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("enckey(aes ecb mode) rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		//aes_ecb
		memcpy_c(std_aes->key,dk,32);
		caam_cipher_setkey_enckey(&ctx,AES,ECB,enc,std_aes->key,32,KEY_TYPE_AES_ECB_KEYR);
		ctxp = (struct cipher_ctx *)ctx;
		if(!memcmp_c(dk,ctxp->key,32))
		{
			debug("rand ecb(aes) enckey(aes ecb) black key generate error!\n");
			while(1);
		}
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_enckey_finish(ctx,rand_size,KEY_TYPE_AES_ECB_KEYR);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
		{
			debug("rand ecb(aes) enckey(aes ecb) test error!\n");
			while(1);
		}
		//aes_ccm
		memcpy_c(std_aes->key,dk,32);
		caam_cipher_setkey_enckey(&ctx,AES,ECB,dec,std_aes->key,32,KEY_TYPE_AES_CCM_KEYR);
		ctxp = (struct cipher_ctx *)ctx;
		if(!memcmp_c(dk,ctxp->key,32))
		{
			debug("rand ecb(aes) enckey(aes ccm) black key generate error!\n");
			while(1);
		}
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_enckey_finish(ctx,rand_size,KEY_TYPE_AES_CCM_KEYR);
		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ecb(aes) enckey(ccm_aes) test ok!\n");
		else
		{
			debug("rand error ecb(aes) enckey(ecb_aes/ccm_aes) test!\n");
			while(1);
		}

		//sm4_ecb
		memcpy_c(std_aes->key,dk,32);
		caam_cipher_setkey_enckey(&ctx,SM4,CBC,enc,std_aes->key,16,KEY_TYPE_SM4_ECB_KEYR);
		ctxp = (struct cipher_ctx *)ctx;
		if(!memcmp_c(dk,ctxp->key,16))
		{
			debug("rand cbc(sm4) enckey(sm4 ecb) black key generate error!\n");
			while(1);
		}
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_enckey_finish(ctx,rand_size,KEY_TYPE_SM4_ECB_KEYR);
		if(!memcmp_c(rand_data,rand_cmp,rand_size))
		{
			debug("rand cbc(sm4) enckey(sm4 ecb) test error!\n");
			while(1);
		}

		//sm4_ccm
		memcpy_c(std_aes->key,dk,32);
		caam_cipher_setkey_enckey(&ctx,SM4,CBC,dec,std_aes->key,16,KEY_TYPE_SM4_CCM_KEYR);
		ctxp = (struct cipher_ctx *)ctx;
		if(!memcmp_c(dk,ctxp->key,16))
		{
			debug("rand cbc(sm4) enckey(sm4 ccm) black key generate error!\n");
			while(1);
		}
		caam_cipher_setiv(ctx,std_aes->iv,16);
		caam_cipher_crypt(ctx,rand_data,rand_size,1);
		caam_cipher_crypt_enckey_finish(ctx,rand_size,KEY_TYPE_SM4_CCM_KEYR);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand cbc(sm4) enckey(ccm_sm4) test ok!\n");
		else
		{
			debug("rand error cbc(sm4) enckey(ecb_sm4/ccm_sm4) test!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(aes_raw);
}


void sec_blackkey_test()
{
	sec_cipher_std_enckey_test();
	sec_cipher_random_enckey_test();
}

len_2_mem(void *mptr,unsigned len,unsigned len_size)
{
	int i;
	unsigned long long len_long = (unsigned long long)len;
	unsigned char *mp = (unsigned char *)mptr;
	for(i=0;i<len_size;i++)
	{
		mp[i]=(len_long>>((7-i)*8)) & 0xff;
	}
}

ccm_init_format(struct cipher_ctx *ctxp,void *randp,u8 *cntp,u8 *macp,u8 *cbcivp,u8 *aad,unsigned auth_len,unsigned ml_len,unsigned  aad_size,unsigned message_len)
{
	unsigned char *fp,*rp;
	unsigned int i,j;
	fp =(unsigned char *)(&(ctxp->iv[16]));
	rp = (unsigned char *)randp;
	unsigned long long *cntull;

	ctxp->auth_len=auth_len;
	ctxp->ml_len = ml_len;
	ctxp->aad_size = aad_size;
	ctxp->message_len = message_len;

	ctxp->iv_size = 64;

	ctxp->ext = malloc(0x10+0x10+ctxp->aad_size+0x10);
	//b0 size 0x10,next 0x10(val) for aead size,and the end 0x10 is resever for padding

	for(i=0;i<16;i++)
	{
		ctxp->iv[i]=macp[i];
	}

	//A0 fp[0]=ml_len-1;	for enc/dec
	fp[0] = (auth_len -2)<<2 | (ml_len-1);	//A0 type
	//fp[0] = (ml_len-1);
	for(i=0;i<15-ml_len;i++)
	{
		fp[i+1]=rp[i];	//noise,infact need rand data,now just test
	}
	//len_2_mem(&(fp[8]),message_len,ml_len);
	for(i=16-ml_len;i<16;i++)
	{
		fp[i]=cntp[i];
	}

	cntull = ((unsigned long long *)cntp);
	*cntull+=1;
	for(i=16,j=0;i<16+ml_len;i++,j++)
	{
		fp[i]=cntp[j+8-ml_len];
	}

	for(i=64;i<80;i++)
	{
		ctxp->iv[i]=cbcivp[i];
	}
//////ctx end

	fp = ctxp->ext;
	//6bit Adata,5~3bit auth len,(auth_len-2)/2,2~0bit len area (L-1)
	fp[0] = (aad_size?1:0)<<6 | (auth_len -2)<<2 | (ml_len-1);	//now just set adata & auth len & message len
	for(i=0;i<15-ml_len;i++)
	{
		fp[i+1]=rp[i];	//noise,infact need rand data,now just test
	}
	for(i=16-ml_len,j=0;i<16;i++,j++)
	{
		fp[i]=(message_len>>((ml_len-j-1)*8)) & 0xff;
	}
	j=16;

	if(aad_size>0)
	{
		if((aad_size<0xFEFF))
		{
			fp[16]=(aad_size>>8)&0xff;
			fp[17]=aad_size&0xff;
			j=18;
		}else if(aad_size<0xffffffff)
		{
			fp[16]=0xFF;
			fp[17]=0xFE;
			for(i=0;i<4;i++)
			{
				fp[18+i]=(aad_size>>((3-i)*8))&0xff;
			}
			j=22;
		}else
		{
			fp[16]=0xFF;
			fp[17]=0xFF;
			for(i=0;i<8;i++)
			{
				fp[18+i]=(aad_size>>((7-i)*8))&0xff;
			}
			j=26;
		}
	}
	for(i=0;i<aad_size;i++)
	{
		fp[j]=aad[i];
		j++;
	}

	if(aad_size)
	{
		ctxp->aad_size = (j+0x10)&0xfffffff0;		//maybe,it will be usefull for fifo load cmd
		for(i=j;i<ctxp->aad_size;i++)
		{
			fp[i]=0;
		}
	}else
	{
		ctxp->aad_size = 0x10;
	}
}

ccm_free_format(void *fp)
{
	free(fp);
}

sec_aes128_ccm_test()
{
	void *ctx;
	struct cipher_ctx *ctxp;
	struct std_data *aes_raw;
	void *ext;
	u8 init[32];
	u8 dt[64];
	u8 dmac[16];

	STD_AES_CCM(&aes_raw);

	memcpy_c(dt,std_aes->data+32,32);

	caam_cipher_setkey(&ctx,AES,CCM,enc,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,8,32,32);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+32,32,1);
	caam_cipher_ccm_finish(ctx,std_aes->data+32,std_aes->data+32,dmac,32);
	ccm_free_format(ext);


	if(!memcmp_c(std_aes->data+32,std_aes->stdout+32,32))
		debug("std ccm(aes) key128 enc test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 enc test!\n");
		while(1);
	}

	if(!memcmp_c(dmac,std_aes->stdout,16))
		debug("std ccm(aes) key128 mac test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 mac test!\n");
		while(1);
	}

	memset_c(dmac,0,16);

	caam_cipher_setkey(&ctx,AES,CCM,dec,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,8,32,32);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+32,32,1);
	caam_cipher_ccm_finish(ctx,std_aes->data+32,std_aes->data+32,dmac,32);
	ccm_free_format(ext);

	if(!memcmp_c(std_aes->data+32,dt,32))
		debug("std ccm(aes) key128 dec test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 dec test!\n");
		while(1);
	}

	if(!memcmp_c(dmac,std_aes->stdout,16))
		debug("std ccm(aes) key128 mac test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 mac test!\n");
		while(1);
	}

	free(aes_raw);
}

sec_sm4_ccm_random_test()
{
	void *ctx;
	struct cipher_ctx *ctxp;
	struct std_data *aes_raw;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned rand_size;
	unsigned aad_size;
	unsigned ml_len,auth_len,ret;
	void *ext;
	u8 init[32];
	u8 dmac[16],dmac2[16];
	int i;

	STD_AES_CCM(&aes_raw);			//not mater the key and iv value,just rand test, so use aes std for sm4

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp2(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		aad_size = (rand_size-0x10)& rand();
		ml_len =  ((rand()&7)+1)&0x0e ? :2;
		auth_len = ( (((rand()&7)+1)&0x0e)?:2  )<<1 ;
		//auth_len = 16;

		memset_c(dmac,0,16);
		memset_c(dmac2,0,16);

		debug("enckey(aes ecb mode) rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);

		caam_cipher_setkey(&ctx,SM4,CCM,enc,std_aes->key,16);
		ctxp = (struct cipher_ctx *)ctx;
		memset_c(init,0,32);
		ccm_init_format(ctxp,std_aes->iv,init,init,init,rand_data,auth_len,ml_len,aad_size,rand_size-aad_size);		//set b0
		ext = ctxp->ext;
		caam_cipher_crypt(ctx,rand_data+aad_size,rand_size-aad_size,1);
		caam_cipher_ccm_finish(ctx,rand_data+aad_size,rand_data+aad_size,dmac,rand_size-aad_size);
		ccm_free_format(ext);

		caam_cipher_setkey(&ctx,SM4,CCM,dec,std_aes->key,16);
		ctxp = (struct cipher_ctx *)ctx;
		memset_c(init,0,32);
		ccm_init_format(ctxp,std_aes->iv,init,init,init,rand_data,auth_len,ml_len,aad_size,rand_size-aad_size);		//set b0
		ext = ctxp->ext;
		caam_cipher_crypt(ctx,rand_data+aad_size,rand_size-aad_size,1);
		ret = caam_cipher_ccm_icv_finish(ctx,rand_data+aad_size,rand_data+aad_size,dmac,rand_size-aad_size);
		ccm_free_format(ext);

		if(!memcmp_c(rand_data,rand_cmp,rand_size))
			debug("rand ccm(sm4) key128 enc test ok!\n");
		else
		{
			debug("rand error ccm(sm4) key128 enc test!\n");
			while(1);
		}
		if(ret!=0)
		{
			debug("rand error ccm(sm4) key128 mac test!\n");
			while(1);
		}
		// next mac error

		if(aad_size>1)
		{
			caam_cipher_setkey(&ctx,SM4,CCM,dec,std_aes->key,16);
			ctxp = (struct cipher_ctx *)ctx;
			memset_c(init,0,32);
			*(unsigned char *)rand_data = ~(*(unsigned char *)rand_data);
			ccm_init_format(ctxp,std_aes->iv,init,init,init,rand_data,16,ml_len,aad_size,rand_size-aad_size);		//set b0
			ext = ctxp->ext;
			caam_cipher_crypt(ctx,rand_data+aad_size,rand_size-aad_size,1);
			ret = caam_cipher_ccm_icv_finish(ctx,rand_data+aad_size,rand_data+aad_size,dmac,rand_size-aad_size);
			ccm_free_format(ext);

			if(ret==0)
			{
				debug("rand error ccm(sm4) key128 icv detect error!\n");
				while(1);
			}
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}
	free(aes_raw);
}

sec_aes128_ccm_rfc3610_test()
{
	void *ctx;
	struct cipher_ctx *ctxp;
	struct std_data *aes_raw;
	void *ext;
	u8 init[32];
	u8 dt[64];
	u8 dmac[16];

	STD_AES_CCM4(&aes_raw);

	memcpy_c(dt,std_aes->data,31);

	caam_cipher_setkey(&ctx,AES,CCM,enc,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,8,2,8,23);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+8,23,1);
	caam_cipher_ccm_finish(ctx,std_aes->data+8,std_aes->data+8,dmac,23);
	ccm_free_format(ext);


	if(!memcmp_c(std_aes->data+8,std_aes->stdout+8,23))
		debug("std ccm(aes) key128 rfc3601 enc test ok!\n");
	else
	{
		debug("std error ccm(aes) rfc3601 key128 enc test!\n");
		while(1);
	}

	if(!memcmp_c(std_aes->stdout,dmac,8))
		debug("std ccm(aes) key128 rfc3601 enc mac ok!\n");
	else
	{
		debug("std error ccm(aes) key128  rfc3601 mac test!\n");
		while(1);
	}

	memset_c(dmac,0,16);
	caam_cipher_setkey(&ctx,AES,CCM,dec,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,8,2,8,23);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+8,23,1);
	caam_cipher_ccm_finish(ctx,std_aes->data+8,std_aes->data+8,dmac,23);
	ccm_free_format(ext);

	if(!memcmp_c(std_aes->data+8,dt+8,23))
		debug("std ccm(aes) key128 rfc3601 dec test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 rfc3601 dec test!\n");
		while(1);
	}
	if(!memcmp_c(std_aes->stdout,dmac,8))
		debug("std ccm(aes) key128 rfc3601 dec mac ok!\n");
	else
	{
		debug("std error ccm(aes) key128 rfc3601 dec mac test!\n");
		while(1);
	}

	free(aes_raw);
}

sec_aes256_ccm_test()
{
	void *ctx;
	struct cipher_ctx *ctxp;
	struct std_data *aes_raw;
	void *ext;
	u8 init[32];
	u8 dt[64];
	u8 dmac[16];

	STD_AES_CCM2(&aes_raw);

	memcpy_c(dt,std_aes->data,16);

	caam_cipher_setkey(&ctx,AES,CCM,enc,std_aes->key,32);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,4,0,16);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_ccm_finish(ctx,std_aes->data,std_aes->data,dmac,16);
	ccm_free_format(ext);


	if(!memcmp_c(std_aes->data,std_aes->stdout+16,16))
		debug("std ccm(aes) key256 enc ok!\n");
	else
	{
		debug("std error ccm(aes) key256 enc test!\n");
		while(1);
	}
	if(!memcmp_c(dmac,std_aes->stdout,16))
		debug("std ccm(aes) key256 mac ok!\n");
	else
	{
		debug("std error ccm(aes) key256 mac test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CCM,dec,std_aes->key,32);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,4,0,16);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data,16,1);
	caam_cipher_ccm_finish(ctx,std_aes->data,std_aes->data,dmac,16);
	ccm_free_format(ext);
	if(!memcmp_c(std_aes->data,dt,16))
		debug("std ccm(aes) key256 enc ok!\n");
	else
	{
		debug("std error ccm(aes) key256 enc test!\n");
		while(1);
	}
	if(!memcmp_c(dmac,std_aes->stdout,16))
		debug("std ccm(aes) key256 mac ok!\n");
	else
	{
		debug("std error ccm(aes) key256 mac test!\n");
		while(1);
	}

	free(aes_raw);
}

sec_aes128_ccm_icv_test()
{
	void *ctx;
	struct cipher_ctx *ctxp;
	struct std_data *aes_raw;
	void *ext;
	u8 init[32];
	u8 dt[64];
	u8 dmac[16];
	u64 ret;

	STD_AES_CCM(&aes_raw);

	memcpy_c(dt,std_aes->data+32,32);
	caam_cipher_setkey(&ctx,AES,CCM,enc,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,8,32,32);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+32,32,1);
	caam_cipher_ccm_icv_finish(ctx,std_aes->data+32,std_aes->data+32,dmac,32);
	ccm_free_format(ext);


	if(!memcmp_c(std_aes->data+32,std_aes->stdout+32,32))
		debug("std ccm(aes) key128 enc test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 enc test!\n");
		while(1);
	}

	caam_cipher_setkey(&ctx,AES,CCM,dec,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,8,32,32);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+32,32,1);
	ret = caam_cipher_ccm_icv_finish(ctx,std_aes->data+32,std_aes->data+32,dmac,32);
	//ret = caam_cipher_ccm_finish(ctx,std_aes->data+32,std_aes->data+32,dmac,32);
	ccm_free_format(ext);

	if(!memcmp_c(std_aes->data+32,dt,32))
		debug("std ccm(aes) key128 dec test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 dec test!\n");
		while(1);
	}

	if(ret!=0)
	{
		debug("std ccm(aes) key128 icv mac error!\n");
	}

//

	caam_cipher_setkey(&ctx,AES,CCM,enc,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,8,32,32);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+32,32,1);
	caam_cipher_ccm_icv_finish(ctx,std_aes->data+32,std_aes->data+32,dmac,32);
	ccm_free_format(ext);


	if(!memcmp_c(std_aes->data+32,std_aes->stdout+32,32))
		debug("std ccm(aes) key128 enc test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 enc test!\n");
		while(1);
	}

	*(u8 *)(std_aes->data) = ~*(u8 *)(std_aes->data);
	caam_cipher_setkey(&ctx,AES,CCM,dec,std_aes->key,16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,std_aes->iv,init,init,init,std_aes->data,16,8,32,32);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,std_aes->data+32,32,1);
	ret = caam_cipher_ccm_icv_finish(ctx,std_aes->data+32,std_aes->data+32,dmac,32);
	ccm_free_format(ext);

	if(!memcmp_c(std_aes->data+32,dt,32))
		debug("std ccm(aes) key128 dec test ok!\n");
	else
	{
		debug("std error ccm(aes) key128 dec test!\n");
		while(1);
	}

	if(ret==0)
	{
		debug("std ccm(aes) key128 icv check error!\n");
	}

	free(aes_raw);
}


void sec_ccm_test()
{
	sec_aes128_ccm_test();
	sec_aes128_ccm_rfc3610_test();
	sec_aes128_ccm_icv_test();
	sec_aes256_ccm_test();

	sec_sm4_ccm_random_test();
}

void sec_pkha_test()
{

}

void sec_shared_test()
{
	load_store_shared_reo_simple();		//this fuc to see if the job share desc can nomally run to while(1)
	fifo_load_cipher_shared_test();
}

void sec_crc32_ieee802()
{
	int i;
	void *crc_raw,*ctx;
	unsigned crc_sret,crc_hret;
	unsigned char *crc_sp,*crc_hp;
	crc_sp = (unsigned char *)(&crc_sret);
	crc_hp = (unsigned char *)(&crc_hret);
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size;
	STD_CRC(&crc_raw);
	crc_sret = crc_32_ieee802(std_crc->data,32);

	caam_hk_init(&ctx,NULL,0,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_802);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_crc->data,32,1);
	caam_hk_finish(ctx,crc_hp,4);

	for(i=0;i<0x04;i++)
	{
		if(crc_sp[i]!=std_crc->stdout[i])
		{
			debug("std crc32 ieee802 std soft test  err!\n");
			while(1);
		}
	}

	for(i=0;i<0x04;i++)
	{
		if(crc_hp[i]!=std_crc->stdout[i])
		{
			debug("std crc32 ieee802 std hw sec test  err!\n");
			while(1);
		}
	}

	debug("std crc32 ieee802 std test ok!\n");

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("crc32 ieee802 rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);
		crc_sret = crc_32_ieee802(rand_data,rand_size);

		caam_hk_init(&ctx,NULL,0,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_802);
		caam_hk_setiv(ctx,NULL,0);
		caam_hk_update(ctx,rand_data,rand_size,1);
		caam_hk_finish(ctx,crc_hp,4);

		if(crc_sret != crc_hret)
		{
			debug("std crc32 ieee802 rand test err!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	debug("std crc32 ieee802 rand test ok!\n");

	free(crc_raw);
}

void sec_crc32_ietf3385()
{
	int i;
	void *crc_raw,*ctx;
	unsigned crc_sret,crc_hret;
	unsigned char *crc_sp,*crc_hp;
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size;
	crc_sp = (unsigned char *)(&crc_sret);
	crc_hp = (unsigned char *)(&crc_hret);
	STD_CRC(&crc_raw);
	crc_sret = crc_32_ietf3385(std_crc->data,32);


	caam_hk_init(&ctx,NULL,0,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_3385);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_crc->data,32,1);
	caam_hk_finish(ctx,crc_hp,4);

	for(i=0;i<0x04;i++)
	{
		if(crc_sp[i]!=std_crc->stdout[i+0x04])
		{
			debug("std crc32 ietf3385 std soft test  err!\n");
			while(1);
		}
	}

	for(i=0;i<0x04;i++)
	{
		if(crc_hp[i]!=std_crc->stdout[i+0x04])
		{
			debug("std crc32 ietf3385 std hw sec test  err!\n");
			while(1);
		}
	}

	debug("std crc32 ietf3385 std test ok!\n");

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("crc32 ietf3385 rand_size is 0x%x,rand cnt is 0x%x\n",rand_size,i);
		crc_sret = crc_32_ietf3385(rand_data,rand_size);

		caam_hk_init(&ctx,NULL,0,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_3385);
		caam_hk_setiv(ctx,NULL,0);
		caam_hk_update(ctx,rand_data,rand_size,1);
		caam_hk_finish(ctx,crc_hp,4);

		if(crc_sret != crc_hret)
		{
			debug("std crc32 ietf3385 rand test err!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	debug("std crc32 ietf3385 rand test ok!\n");

	free(crc_raw);
}

void crc_custom_16()
{
	unsigned char data[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	unsigned int poly = 0x1021;
	unsigned int poly_l = 16;
	poly = ( *(unsigned int *)bswap(&poly,32)>>16 );
	unsigned crc_sret;
	crc_sret = crc_16_custom(data,8,poly);
	if( (crc_sret&0xffff0000)!=0x4AF70000)
	{
		debug("crc_custom_16 test err!\n");
		while(1);
	}
	else
	{
		debug("crc_custom_16 test ok!\n");
	}
}

void sec_crc32_custom()
{
	int i;
	void *crc_raw,*ctx;
	unsigned crc_sret,crc_hret;
	unsigned char *crc_sp,*crc_hp;
	crc_sp = (unsigned char *)(&crc_sret);
	crc_hp = (unsigned char *)(&crc_hret);
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size;
	unsigned int poly,poly_l;

//	poly = 0x04c11db7;
	poly = 0x04c11db7;
	poly_l = 32;

	STD_CRC(&crc_raw);
	crc_sret = crc_32_custom(std_crc->data,poly_l,*(unsigned int *)(bswap(&poly,32)));

	poly = 0x04c11db7;
	b2l(&poly,4);
	caam_hk_init(&ctx,&poly,poly_l,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_CUST_POLY);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_crc->data,32,1);
	caam_hk_finish(ctx,crc_hp,4);

	for(i=0;i<0x04;i++)
	{
		if(crc_sp[i]!=std_crc->stdout[i])
		{
			debug("std crc32 custom std soft test  err!\n");
			while(1);
		}
	}

	debug("std crc32 custom std test ok!\n");

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		debug("crc32 custom rand_size is 0x%x,rand cnt is 0x%x,poly is %x\n",rand_size,i,poly);

		poly = (unsigned int)(rand());
		poly_l = 32;
		caam_hk_init(&ctx,&poly,poly_l,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_CUST_POLY);
		caam_hk_setiv(ctx,NULL,0);
		caam_hk_update(ctx,rand_data,32,1);
		caam_hk_finish(ctx,crc_hp,4);

		b2l(&poly,4);
		crc_sret = crc_32_custom(rand_data,32,*(unsigned int *)(bswap(&poly,32)));

		if(crc_sret != crc_hret)
		{
			debug("crc32 custom rand test err!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	debug("crc32 custom rand test ok!\n");

	free(crc_raw);
}

void sec_crc16_custom()
{
	int i;
	void *crc_raw,*ctx;
	unsigned crc_sret,crc_hret;
	unsigned char *crc_sp,*crc_hp;
	crc_sp = (unsigned char *)(&crc_sret);
	crc_hp = (unsigned char *)(&crc_hret);
	void *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size;
	unsigned int poly,poly_l,poly_t;

	poly = 0x1021;
	poly_l = 16;
	poly = *(unsigned int *)bswap(&poly,32)>>16;

	STD_CRC(&crc_raw);
	crc_sret = crc_16_custom(std_crc->data,32,poly);

	poly = 0x1021;	// 9-25,hardware key input format,left alian change to right alian,so mask this line
	b2l(&poly,4);
	caam_hk_init(&ctx,&poly,poly_l,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_CUST_POLY);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_crc->data,32,1);
	caam_hk_finish(ctx,crc_hp,4);

	if( (crc_sret&0xffff0000)!=(crc_hret&0xffff0000))
	{
		debug("std crc16 custom std soft test  err!\n");
		while(1);
	}

	debug("std crc16 custom std test ok!\n");

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		if(!rand_size)
			continue;
		poly = (unsigned int)(rand());			//high 16 bit is useful

		debug("crc16 custom rand_size is 0x%x,rand cnt is 0x%x,poly is %x\n",rand_size,i,poly);

		poly_t = poly;
		poly_l = 16;
		poly = *(unsigned int *)bswap(&poly,32)>>16;	// 9-25,hardware key input format,left alian change to right alian,so mask this line


		crc_sret = crc_16_custom(rand_data,rand_size,poly);

		poly = poly_t;
		b2l(&poly,4);
		caam_hk_init(&ctx,&poly,poly_l,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_CUST_POLY);
		caam_hk_setiv(ctx,NULL,0);
		caam_hk_update(ctx,rand_data,rand_size,1);
		caam_hk_finish(ctx,crc_hp,4);

		if( (crc_sret&0xffff0000)!=(crc_hret&0xffff0000) )
		{
			debug("crc16 custom rand test err!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	debug("crc16 custom rand test ok!\n");

	free(crc_raw);
}

sec_crcx_custom_soft()
{
	unsigned int poly,poly_l;
	void *crc_raw;
	unsigned crc_sret;
	poly = 0x04c11db7;
	poly_l = 32;
	poly = *(unsigned int *)bswap(&poly,32)>>(32-poly_l);

	STD_CRC(&crc_raw);
	set_sinit();
	//crc_sret = crc_x_e(std_crc->data,32,poly,poly_l,0xffffffff,0,0,0);	//ibs=0;obs=0;c=0;
	crc_sret = crc_x_e(std_crc->data,8,poly,poly_l,0xffffffff,0,1,1);	//ibs=0;obs=0;c=0;
	crc_sret = crc_x_e(std_crc->data+8,8,poly,poly_l,0,0,1,1);	//ibs=0;obs=0;c=0;
	//crc_sret = crc_x_e(std_crc->data+16,16,poly,poly_l,0,0,0,0);	//ibs=0;obs=0;c=0;
	debug("sec_crcx_custom_soft ok!\n");
	free(crc_raw);
}

void sec_crcx_custom()
{
	int i;
	void *crc_raw,*ctx;
	unsigned crc_sret,crc_hret;
	unsigned char *crc_sp,*crc_hp;
	crc_sp = (unsigned char *)(&crc_sret);
	crc_hp = (unsigned char *)(&crc_hret);
	unsigned int *rand_raw,*rand_data,*rand_cmp;
	unsigned int rand_size;
	unsigned int poly,poly_l,poly_t;
	unsigned int dflag;

	poly = 0x1021;
	poly_t = poly;
	poly_l = 16;
	poly = *(unsigned int *)bswap(&poly,32)>>(32-poly_l);

	STD_CRC(&crc_raw);
	set_sinit();
	crc_sret = crc_x_e(std_crc->data,32,poly,poly_l,0xffffffff,0,0,0);	//ibs=0;obs=0;c=0;

	poly = poly_t;
	//poly <<=(32-poly_l);	// 9-25,hardware key input format,left alian change to right alian,so mask this line
	b2l(&poly,4);
	caam_hk_init(&ctx,&poly,poly_l,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_CUST_POLY);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,std_crc->data,32,1);
	caam_hk_finish(ctx,crc_hp,4);

	crc_sret = ( (crc_sret<<(32-poly_l))>>(32-poly_l));
	l2b(&crc_hret,4);
	//crc_hret = crc_hret>>(32-poly_l);	// 9-25,hardware key input format,left alian change to right alian,so mask this line

	if(crc_sret != crc_hret)
	{
		debug("std crcx custom std soft test  err!\n");
		while(1);
	}

	debug("std crcx custom std test ok!\n");

	for(i=0;i<RAND_CNT;i++)
	{
		alloc_rand_dp(&rand_raw,&rand_data,&rand_cmp,&rand_size);
		if(!rand_size)
			continue;

		poly = (unsigned int)(rand());			//high 16 bit is useful
		dflag = (unsigned int)(rand());			//bit0~5 poly_len,is_ibs bit6,is_obs bit7,is_c bit8
		if(!(dflag&0x3f))
			continue;

		poly_t = poly;
		poly_l = (dflag &0x20)?0x20:(dflag &0x1f);		//9-14 for test
		poly_l = poly_l?:0x20;
		debug("crcx custom rand_size is 0x%x,rand cnt is 0x%x,poly is %x,poly_l is %x,dflag & 0x1c0 is %x\n",rand_size,i,poly,poly_l,dflag&0x1c0);
		poly = *(unsigned int *)bswap(&poly,32);
		poly >>= (32-poly_l);
		debug("crcx custom poly after process is %x\n",poly);	//9-14 for test

		set_sinit();
		crc_sret = crc_x_e(rand_cmp,rand_size,poly,poly_l,0xffffffff,dflag&0x40,dflag&0x80,dflag&0x100);

		poly = poly_t;
		//poly <<=(32-poly_l);			// 9-25,hardware key input format,left alian change to right alian,so mask this line

		b2l(&poly,4);
		//dflag &= 0xffffffbf;
		caam_hk_init(&ctx,&poly,poly_l,OP_ALG_ALGSEL_CRC,4,OP_ALG_AAI_CUST_POLY|( (dflag&0x1c0)<<2 ));
		caam_hk_setiv(ctx,NULL,0);
		caam_hk_update(ctx,rand_data,rand_size,1);
		caam_hk_finish(ctx,crc_hp,4);

		crc_sret = ( (crc_sret<<(32-poly_l))>>(32-poly_l));
		l2b(&crc_hret,4);
		//crc_hret = crc_hret>>(32-poly_l);		// 9-25,hardware key input format,left alian change to right alian,so mask this line

		if(crc_sret != crc_hret)
		{
			debug("crcx custom rand test err!\n");
			while(1);
		}

		free_rand_dp(&rand_raw,&rand_cmp);
	}

	debug("crcx custom rand test ok!\n");

	free(crc_raw);
}

void sec_crcx_test()
{
#if 0
	unsigned buf[2]={0x12345678,0x9abcdef0};
	debug("buf is 0x%x,0x%x\n",buf[0],buf[1]);
#endif

	sec_crc32_ieee802();
	sec_crc32_ietf3385();
	sec_crc32_custom();
	crc_custom_16();
	sec_crc16_custom();

	sec_crcx_custom();

#if 0
	bswap((unsigned char *)buf,64);
	debug("buf is 0x%x,0x%x\n",buf[0],buf[1]);
#endif
}

void sec_out_snoop_test()	//note that no std aes enc out for hmac,so need to complete the snoop std data next
{
	int i;
	void *cipher_ctx;
	void *hmac_ctx;
	unsigned char dst_buf[80];
	void *sm4_raw,*sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw;
	STD_SMS4(&sm4_raw);
	STD_HMAC(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	caam_snoop_init(&cipher_ctx,SM4,CBC,enc,std_sm4->key,16, \
			&hmac_ctx,std_sha1->key,6,OP_ALG_ALGSEL_SHA1,OP_ALG_AAI_HMAC);
	caam_snoop_setiv(cipher_ctx,std_sm4->iv,16);	//which iv
	caam_snoop_updata(cipher_ctx,std_sm4->data+16,32,1);	//which data?
	caam_out_snoop_finish(cipher_ctx,hmac_ctx,dst_buf);
	for(i=0;i<20;i++)
	{
		if(dst_buf[i]!=std_sha1->stdout[i+40])
		{
			debug("sec_out_snoop_test err!\n");
			while(1);
		}
	}
	free(sm4_raw);
	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);
	debug("sec_out_snoop_test ok!\n");
}

void sec_in_snoop_test()	//note that no std aes enc out for hmac,so need to complete the snoop std data next
{
	int i;
	void *cipher_ctx;
	void *hmac_ctx;
	unsigned char dst_buf[80];
	void *sm4_raw,*sha1_raw,*sha256_raw,*sha512_raw,*sm3_raw;
	STD_SMS4(&sm4_raw);
	STD_HMAC(&sha1_raw,&sha256_raw,&sha512_raw,&sm3_raw);
	caam_snoop_init(&cipher_ctx,SM4,CBC,enc,std_sm4->key,16, \
			&hmac_ctx,std_sha1->key,6,OP_ALG_ALGSEL_SHA1,OP_ALG_AAI_HMAC);
	caam_snoop_setiv(cipher_ctx,std_sm4->iv,16);	//which iv
	caam_snoop_updata(cipher_ctx,std_sm4->data+16,32,1);	//which data?
	caam_in_snoop_finish(cipher_ctx,hmac_ctx,dst_buf);
	for(i=0;i<20;i++)
	{
		if(dst_buf[i]!=std_sha1->stdout[i+60])
		{
			debug("sec_in_snoop_test err!\n");
			while(1);
		}
	}
	free(sm4_raw);
	free(sha1_raw);
	free(sha256_raw);
	free(sha512_raw);
	free(sm3_raw);
	debug("sec_in_snoop_test ok!\n");

}

void sec_jbring_test()
{
	sec_init();
	jbring_pipeline_test();
}

sec_hmac_large_nocompare_test()
{
	struct hash_ctx *ctx;
	int ret,i;
	unsigned char dst_buf[80];
	void *data_raw,*key_raw,*data,*key;
	int data_size = 0x1000;
	int key_size = 6;
	data_raw = malloc(data_size+DATA_ALIGN);
	key_raw = malloc(6+DATA_ALIGN);
	data=(void *)(((unsigned long)data_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	key = (void *)(((unsigned long)key_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	caam_hk_init(&ctx,key,key_size,OP_ALG_ALGSEL_MD5,16,OP_ALG_AAI_HMAC);
	caam_hk_setiv(ctx,NULL,0);
	caam_hk_update(ctx,data,data_size,1);
	caam_hk_finish(ctx,dst_buf,16);

	free(data_raw);
	free(key_raw);
}

void sec_test()
{
//	sec_tlb_add();			//add 64bit VA SRAM_TLB for SEC
	sec_init();

	rng_impl_init();

	//do
	//{
#if 1
	uart_printf_buf("SHA TEST start!\n");
	sec_hash_std_test();		//test not ok,need to test again next fpga version
	//sec_hash_std_icvcheck_test();

//	sec_hmac_std_test();
//	sec_hmac_large_nocompare_test();				//just test large hmac can or cannot do complete

//	sec_hmac_splitkey_test();	//split key must with black key now
//	sec_smac_std_test();		//not aready

//	sec_hash_separate_test();

//	sec_shared_test();

//	sec_crcx_test();

//	sec_crcx_custom_soft();		//just for soft crc test
#endif
	
	sec_cipher_std_test();

	//sec_cipher_ctr_test();

#if 0
	blob_test();

	sec_ccm_test();

	sec_blackkey_test();

	sec_hmac_sperate_test();

	sec_out_snoop_test();

	sec_in_snoop_test();
#endif
	//sec_hmac_split_sperate_test();		//no use,for split now combine with blackkey,but no complete this use case now!

	//}while(1);

}
