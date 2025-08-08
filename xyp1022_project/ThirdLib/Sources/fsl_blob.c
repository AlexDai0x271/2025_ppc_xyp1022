/*
 * blob.c
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: ccore
 */

#include "common.h"
#include "sec_common.h"
#include "sec.h"
#include "desc_constr.h"
#include "jobdesc.h"
#include "desc.h"
#include "cipher.h"
#include "hash.h"

#define BLOB_SM4					(0<<10)
#define BLOB_AES					(1<<10)
/*
 * blob.c
 *
 *  Created on: 16/11/2017
 *  Author: YQin
 */

int get_secboot_strap()
{
	volatile unsigned int *addr;
	int val;
	addr = (volatile unsigned int *)0xe807001c;			//strap pin ccm
	val = ((*addr) >> 22 & 1);							//bit 22 sec boot select
	return val;				//val:1 sec boot dis
}

unsigned int get_sec_status()
{
	volatile unsigned int *saddr,*taddr;
	unsigned int val;
	saddr = (volatile unsigned int *)0xeb006010;			//status	sec monitor
	taddr = (volatile unsigned int *)0xeb006004;			//status

	if(get_secboot_strap())
	{
		val = (*saddr) >>20;
		val&=0x0f;
		if(val!=0xb)
		{
			*taddr = (1<<31);
		}

		do{
			val = (*saddr) >>20;
			val&=0x0f;
		}while(val!=0xb);
	}else
	{
		val=0x09;
	}
	return val;			//9 check,0xb no secrue
}

void blob_encap_job_desc(void *desc,UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *src,void *dst,UINT32 slen,UINT32 dlen)
{

	unsigned int opt;

	u32 *jump_cmd;

	dma_addr_t keym_in,src_in,dst_out;

	keym_in = virt_to_phys((void *)keym);
	src_in = virt_to_phys((void *)src);
	dst_out = virt_to_phys((void *)dst);

	// Initialize header command
	init_job_desc(desc, START_INDEX);

	jump_cmd = append_jump(desc, JUMP_CLASS_NONE);
	set_jump_tgt_here(desc, jump_cmd);

	//append_key(desc, keym_in, keym_len, CLASS_2);

	append_cmd_ptr(desc, keym_in,keym_len,CMD_LOAD | LDST_SRCDST_BYTE_KEY2 |
			LDST_CLASS_DECO );

	if (dlen > 0xffff) {
		opt = FIFOLDST_EXT;
		append_seq_in_ptr(desc, src_in, 0, opt);
		append_cmd(desc, slen);
		append_seq_out_ptr(desc, dst_out, 0, opt);
		append_cmd(desc, dlen);
	} else {
		append_seq_in_ptr(desc, src_in, slen, 0);
		append_seq_out_ptr(desc, dst_out, dlen, 0);
	}

	if(type == CCM)
		opt=0x100;
	else
		opt=0;

	if(alg == AES)
		opt|= BLOB_AES;
	else
		opt|= BLOB_SM4;

	append_operation(desc, OP_TYPE_ENCAP_PROTOCOL | OP_PCLID_BLOB |opt);		//0x100 CCM

}

void blob_encap_job_extopt_desc(void *desc,UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *src,void *dst,UINT32 slen,UINT32 dlen,UINT32 exopt)
{

	unsigned int opt;

	u32 *jump_cmd;

	dma_addr_t keym_in,src_in,dst_out;

	keym_in = virt_to_phys((void *)keym);
	src_in = virt_to_phys((void *)src);
	dst_out = virt_to_phys((void *)dst);

	// Initialize header command
	init_job_desc(desc, START_INDEX);

	jump_cmd = append_jump(desc, JUMP_CLASS_NONE);
	set_jump_tgt_here(desc, jump_cmd);

	//append_key(desc, keym_in, keym_len, CLASS_2);

	append_cmd_ptr(desc, keym_in,keym_len,CMD_LOAD | LDST_SRCDST_BYTE_KEY2 |
			LDST_CLASS_DECO );

	if (dlen > 0xffff) {
		opt = FIFOLDST_EXT;
		append_seq_in_ptr(desc, src_in, 0, opt);
		append_cmd(desc, slen);
		append_seq_out_ptr(desc, dst_out, 0, opt);
		append_cmd(desc, dlen);
	} else {
		append_seq_in_ptr(desc, src_in, slen, 0);
		append_seq_out_ptr(desc, dst_out, dlen, 0);
	}

	if(type == CCM)
		opt=0x100;
	else
		opt=0;

	if(alg == AES)
		opt|= BLOB_AES;
	else
		opt|= BLOB_SM4;

	append_operation(desc, OP_TYPE_ENCAP_PROTOCOL | OP_PCLID_BLOB | opt| exopt);		//0x100 CCM

}

void blob_decap_job_desc(void *desc,UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *src,void *dst,UINT32 slen,UINT32 dlen)
{

	unsigned int opt;
	u32 *jump_cmd;

	dma_addr_t keym_in,src_in,dst_out;

	keym_in = virt_to_phys((void *)keym);
	src_in = virt_to_phys((void *)src);
	dst_out = virt_to_phys((void *)dst);

	// Initialize header command
	init_job_desc(desc, START_INDEX);

	jump_cmd = append_jump(desc, JUMP_CLASS_NONE);
	set_jump_tgt_here(desc, jump_cmd);
	//append_key(desc, keym_in, keym_len, CLASS_2);
	append_cmd_ptr(desc, keym_in,keym_len,CMD_LOAD | LDST_SRCDST_BYTE_KEY2 |
			LDST_CLASS_DECO );

	if (slen > 0xffff) {
		opt |= FIFOLDST_EXT;
		append_seq_in_ptr(desc, src_in, 0, opt);
		append_cmd(desc, slen);
		append_seq_out_ptr(desc, dst_out, 0, opt);
		append_cmd(desc, dlen);
	} else {
		append_seq_in_ptr(desc, src_in, slen, 0);
		append_seq_out_ptr(desc, dst_out, dlen, 0);
	}

	if(type == CCM)
		opt=0x100;
	else
		opt=0;

	if(alg == AES)
		opt|= BLOB_AES;
	else
		opt|= BLOB_SM4;

	append_operation(desc, OP_TYPE_DECAP_PROTOCOL | OP_PCLID_BLOB |opt);		//0x100 CCM

}

void blob_decap_job_extopt_desc(void *desc,UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *src,void *dst,UINT32 slen,UINT32 dlen,UINT32 ext_opt)
{

	unsigned int opt;
	u32 *jump_cmd;

	dma_addr_t keym_in,src_in,dst_out;

	keym_in = virt_to_phys((void *)keym);
	src_in = virt_to_phys((void *)src);
	dst_out = virt_to_phys((void *)dst);

	// Initialize header command
	init_job_desc(desc, START_INDEX);

	jump_cmd = append_jump(desc, JUMP_CLASS_NONE);
	set_jump_tgt_here(desc, jump_cmd);
	//append_key(desc, keym_in, keym_len, CLASS_2);
	append_cmd_ptr(desc, keym_in,keym_len,CMD_LOAD | LDST_SRCDST_BYTE_KEY2 |
			LDST_CLASS_DECO );

	if (slen > 0xffff) {
		opt |= FIFOLDST_EXT;
		append_seq_in_ptr(desc, src_in, 0, opt);
		append_cmd(desc, slen);
		append_seq_out_ptr(desc, dst_out, 0, opt);
		append_cmd(desc, dlen);
	} else {
		append_seq_in_ptr(desc, src_in, slen, 0);
		append_seq_out_ptr(desc, dst_out, dlen, 0);
	}

	if(type == CCM)
		opt=0x100;
	else
		opt=0;

	if(alg == AES)
		opt|= BLOB_AES;
	else
		opt|= BLOB_SM4;

	append_operation(desc, OP_TYPE_DECAP_PROTOCOL | OP_PCLID_BLOB |opt |ext_opt);		//0x100 CCM

}

int blob_extopt_encap(UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *plain,void *ctext,UINT32 slen,UINT32 dlen,UINT32 extopt)
{
	int ret = 0;
	UINT32 *desc_raw,*desc;

	desc_raw = malloc(64 + DESC_ALIGN);
	desc = (struct std_data *)(((unsigned long)(desc_raw)+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	blob_encap_job_extopt_desc(desc,alg,type,keym,keym_len,plain,ctext,slen,dlen,extopt);

	// Set job ring to process
	ret = run_descriptor_jr_sec1(desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);

	free(desc_raw);

	return ret;
}

int blob_extopt_decap(UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *plain,volatile unsigned int *ctext,UINT32 slen,UINT32 dlen,UINT32 extopt)
{
	int ret = 0;
	UINT32 *desc_raw,*desc;

	desc_raw = malloc(64 + DESC_ALIGN);
	desc = (struct std_data *)(((unsigned long)(desc_raw)+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	blob_decap_job_extopt_desc(desc,alg,type,keym,keym_len,plain,ctext,slen,dlen,extopt);

	// Set job ring to process
	ret = run_descriptor_jr(desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);

	free(desc_raw);

	return ret;
}

int blob_encap(UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *plain,void *ctext,UINT32 slen,UINT32 dlen)
{
	int ret = 0;
	UINT32 *desc_raw,*desc;

	desc_raw = malloc(64 + DESC_ALIGN);
	desc = (struct std_data *)(((unsigned long)(desc_raw)+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	blob_encap_job_desc(desc,alg,type,keym,keym_len,plain,ctext,slen,dlen);

	// Set job ring to process
	ret = run_descriptor_jr(desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);

	free(desc_raw);

	return ret;
}

int blob_decap(UINT8 alg,UINT8 type,void *keym,UINT32 keym_len,void *src,void *dst,UINT32 slen,UINT32 dlen)
{
	int ret = 0;
	UINT32 *desc_raw,*desc;

	desc_raw = malloc(64 + DESC_ALIGN);
	desc = (struct std_data *)(((unsigned long)(desc_raw)+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	blob_decap_job_desc(desc,alg,type,keym,keym_len,src,dst,slen,dlen);

	// Set job ring to process
	ret = run_descriptor_jr(desc,&jr_t);

	if (ret)
		debug("Error %x\n", ret);

	free(desc_raw);

	return ret;
}

void blob_mk_test(void)
{
	UINT32 *data;
	void *ctx;
	UINT32 *plain_shadow;
	UINT32 *key_modifier;
	UINT32 data_len=0x20;
	int i;
	UINT8 dst_buf[32];
	UINT8 hash_text[54];

 	 //in no secrue mode
	UINT8 hash_nosecrue[] ={
			0x9F,0xB2,0x71,0xC1,0xF2,0xF2,0x46,0x98,0xFB,0xAA,0xAD,0xDB,0x42,0x41,0xEC,0xED,
 	 	 	0x19,0xFD,0x8D,0x72,0xFC,0x05,0xBE,0x54,0x52,0x25,0x5D,0x2B,0xEF,0x0F,0x92,0x8B,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x08,0x2b,
	};

	//in check mode
	UINT8 hash_check[] ={
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x08,0x29,
	};

	debug("blob mk test start\n");

	if(get_sec_status()==0x0b)
		memcpy(hash_text,hash_nosecrue,54);
	else if(get_sec_status()==0x09)
		memcpy(hash_text,hash_check,54);
	else
	{
		debug("blob mk sec status trigger error\n");
		while(1);
	}

	byte_turn(hash_text,54);

	caam_hash_init(&ctx);
	caam_hash_update(ctx,hash_text,54,1);
	caam_hash_finish(ctx,dst_buf,54,SM3);

	data = malloc(data_len+0x40);
	plain_shadow = malloc(data_len+0x40);
	key_modifier = malloc(32);

	for (i=0; i<16; i++)
	{
		data[i] = 0;
	}
	for(i=0;i<32/4;i++)
	{
		key_modifier[i]=0x0;
	}

///////////////////////////////////////////////////////////////////////
	blob_extopt_encap(SM4, CCM, key_modifier,0x10, &(data[8]), &(data[0]),data_len,0x20,PROT_OP_CMD_PROTINFO_MKV_BLOB);

	if(memcmp_c(dst_buf,data,32))
	{
		debug("blob mk test error\n");
		while(1);
	}else{
		debug("blob mk test ok\n");
	}

	free(data);
	free(plain_shadow);
	free(key_modifier);
}

void blob_tf_test(void)
{
	UINT32 *data;
	void *ctx,*ext;
	UINT32 *plain_shadow;
	UINT32 *key_modifier;
	UINT32 data_len=0x20;
	int i;
	UINT8 dst_buf[48],init[32],dmac[16];
	struct cipher_ctx *ctxp;
	UINT8 hash_text[54];

	UINT8 iv[] ={
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	};

	 //in no secrue mode
	UINT8 hash_nosecrue[] ={
			0x9F,0xB2,0x71,0xC1,0xF2,0xF2,0x46,0x98,0xFB,0xAA,0xAD,0xDB,0x42,0x41,0xEC,0xED,
	 	 	0x19,0xFD,0x8D,0x72,0xFC,0x05,0xBE,0x54,0x52,0x25,0x5D,0x2B,0xEF,0x0F,0x92,0x8B,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x08,0x3b,
	};

	//in check mode
	UINT8 hash_check[] ={
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x08,0x39,
	};

	debug("blob tf test start\n");

	if(get_sec_status()==0x0b)
		memcpy(hash_text,hash_nosecrue,54);
	else if(get_sec_status()==0x09)
		memcpy(hash_text,hash_check,54);
	else
	{
		debug("blob mk sec status trigger error\n");
		while(1);
	}

	byte_turn(hash_text,54);

	caam_hash_init(&ctx);
	caam_hash_update(ctx,hash_text,54,1);
	caam_hash_finish(ctx,dst_buf,54,SM3);

	data = malloc(data_len+0x70);
	plain_shadow = malloc(data_len+0x70);
	key_modifier = malloc(32);

	for (i=0; i<24; i++)
	{
		data[i] = 0;
		plain_shadow[i] = data[i];
	}

	for (i=24; i<(24+(data_len/4)); i++)
	{
		data[i] = rand();
		plain_shadow[i] = data[i];
	}
	for(i=0;i<32/4;i++)
	{
		key_modifier[i]=0;
	}

///////////////////////////////////////////////////////////////////////
	blob_extopt_encap(SM4, CCM, key_modifier,0x10, &(data[16]), &(data[0]),data_len,data_len+0x70,PROT_OP_CMD_PROTINFO_TEST_BLOB);

	if(memcmp_c(dst_buf,data,32))
	{
		debug("blob tf test step1 error\n");
		while(1);
	}

	memcpy_c(dst_buf,&(data[8]),32);
	caam_cipher_setkey(&ctx,SM4,ECB,enc,&(data[0]),16);
	caam_cipher_setiv(ctx,dst_buf,16);
	caam_cipher_crypt(ctx,dst_buf,32,1);
	caam_cipher_crypt_finish(ctx,32);

	if(memcmp_c(dst_buf,&(data[16]),32))
	{
		debug("blob tf test step2 error\n");
		while(1);
	}

	if(get_sec_status()==0x09)
	{
		memset_c(dst_buf,0,data_len);
	}else
	{
		memset_c(dst_buf,&(data[24]),data_len);
	}
	//memcpy_c(dst_buf,&(data[24]),32);
	caam_cipher_setkey(&ctx,SM4,CCM,enc,&(data[8]),16);
	ctxp = (struct cipher_ctx *)ctx;
	memset_c(init,0,32);
	ccm_init_format(ctxp,iv,init,init,init,NULL,16,4,0,32);		//set b0
	ext = ctxp->ext;
	caam_cipher_crypt(ctx,dst_buf,32,1);
	caam_cipher_ccm_finish(ctx,dst_buf,dst_buf,dmac,32);
	ccm_free_format(ext);

	if(memcmp_c(dst_buf,&(data[24]),32))
	{
		debug("blob tf test step3 error\n");
		while(1);
	}

	if(memcmp_c(dmac,&(data[32]),16))
	{
		debug("blob tf test step4 error\n");
		while(1);
	}

	debug("blob tf test ok!\n");

	free(data);
	free(plain_shadow);
	free(key_modifier);
}

void blob_nomal_test(void)
{
	UINT32 *data;
	UINT32 *plain_shadow;
	UINT32 *key_modifier;
	UINT32 data_len;
	int i;

	data_len = ( 0x10|(unsigned)(rand()) )&RANDSIZE_MASK;
	//data_len = 0x11000;
	debug("blob test start,rand date len is 0x%x\n",data_len);

	data = malloc(data_len+0x40);
	plain_shadow = malloc(data_len+0x40);
	key_modifier = malloc(16);

	for (i=0; i<8; i++)
	{
		data[i] = 0;
		plain_shadow[i] = data[i];
	}

	for (i=8; i<(8+(data_len/4)); i++)
	{
		data[i] = rand();
		plain_shadow[i] = data[i];
	}
	for(i=0;i<16/4;i++)
	{
		key_modifier[i]=0x05050505;
	}
//////////////////////////////////////////////////////////////////////
	blob_encap(SM4, CCM, key_modifier,0x10, &(data[8]), data,data_len,data_len+0x30);

	for (i=8; i<(8+(data_len/4)); i++)
	{
		if(data[i] == plain_shadow[i])
		{
			debug("blob ccm sm4 encap error!\n");
			while(1);
		}
	}

	blob_decap(SM4, CCM, key_modifier,0x10,data,&(data[8]),data_len+0x30,data_len);

	if(memcmp_c(&(plain_shadow[8]),&(data[8]),data_len)!=0)
	{
		debug("blob ccm sm4 decap error\n");
		while(1);
	}
	debug("blob ccm sm4 ok!\n");

////
	blob_encap(SM4, ECB, key_modifier,0x10, &(data[8]), data,data_len,data_len+0x30);

	for (i=8; i<(8+(data_len/4)); i++)
	{
		if(data[i] == plain_shadow[i])
		{
			debug("blob ecb sm4 encap error!\n");
			while(1);
		}
	}

	blob_decap(SM4, ECB, key_modifier,0x10,data,&(data[8]),data_len+0x30,data_len);

	if(memcmp_c(&(plain_shadow[8]),&(data[8]),data_len)!=0)
	{
		debug("blob ecb sm4 decap error\n");
		while(1);
	}
	debug("blob ecb sm4 ok!\n");

///////////////////////////////////////////////////////////////////////
	blob_encap(AES, CCM, key_modifier,0x10, &(data[8]), data,data_len,data_len+0x30);

	for (i=8; i<(8+(data_len/4)); i++)
	{
		if(data[i] == plain_shadow[i])
		{
			debug("blob ccm aes encap error!\n");
			while(1);
		}
	}

	blob_decap(AES, CCM, key_modifier,0x10,data,&(data[8]),data_len+0x30,data_len);

	if(memcmp_c(&(plain_shadow[8]),&(data[8]),data_len)!=0)
	{
		debug("blob ccm aes decap error\n");
		while(1);
	}
	debug("blob ccm aes ok!\n");

	blob_encap(AES, ECB, key_modifier,0x10, &(data[8]), data,data_len,data_len+0x30);

	for (i=8; i<(8+(data_len/4)); i++)
	{
		if(data[i] == plain_shadow[i])
		{
			debug("blob ecb aes encap error!\n");
			while(1);
		}
	}

	blob_decap(AES, ECB, key_modifier,0x10,data,&(data[8]),data_len+0x30,data_len);

	if(memcmp_c(&(plain_shadow[8]),&(data[8]),data_len)!=0)
	{
		debug("blob ecb aes decap error\n");
		while(1);
	}
	debug("blob ecb aes ok!\n");
//////////////////////////////////////////////////////////////////////////////

	debug("blob ok!\n");
	free(data);
	free(plain_shadow);
	free(key_modifier);
}

blob_c0_test()
{
	unsigned char *key_modifier;
	unsigned int *data_in;
	unsigned int *data_out;
	int i,data_len=0x8000;
	volatile unsigned int *c0_ram;
	int ret;

	c0_ram = (volatile void *)(0xe0250000);

	data_out =(unsigned int *) malloc(data_len+0x30);
	key_modifier = (unsigned char *) malloc(32);
	data_in = &(data_out[8]);
//////////////////////////////////////////////////////////////////////

	for(i=0;i<((data_len + 0x30)>>2);i++)
	{
		data_out[i]=i;
	}

	blob_extopt_encap(SM4, CCM, key_modifier,0x10, data_in, data_out,data_len,data_len+0x30,PROT_OP_CMD_PROTINFO_C0_BLOB);

	blob_extopt_decap(SM4, CCM, key_modifier,0x10, data_out, c0_ram,data_len+0x30,data_len,PROT_OP_CMD_PROTINFO_C0_BLOB);

	for(i=0;i<data_len>>2;i++)
	{
		if(l2b_w(&(c0_ram[i]))!=(i+8) )
		{
			debug("blob c0 ram test step1 error!");
			while(1);
		}
	}

	ret = blob_extopt_decap(SM4, CCM, key_modifier,0x10, data_out, data_in,data_len+0x30,data_len,PROT_OP_CMD_PROTINFO_C0_BLOB);
	if(!ret)
	{
		debug("blob c0 ram test step2 error!");
		while(1);
	}

/////////////////////////////////////////////////////////////////////////////////////////
	for(i=0;i<((data_len + 0x30)>>2);i++)
	{
		data_out[i]=i;
	}

	blob_extopt_encap(AES, CCM, key_modifier,0x10, data_in, data_out,data_len,data_len+0x30,PROT_OP_CMD_PROTINFO_C0_BLOB);

	blob_extopt_decap(AES, CCM, key_modifier,0x10, data_out, c0_ram,data_len+0x30,data_len,PROT_OP_CMD_PROTINFO_C0_BLOB);

	for(i=0;i<data_len>>2;i++)
	{
		if(l2b_w(&(c0_ram[i]))!=(i+8))
		{
			debug("blob c0 ram test step3 error!");
			while(1);
		}
	}

	ret = blob_extopt_decap(AES, CCM, key_modifier,0x10, data_out, data_in,data_len+0x30,data_len,PROT_OP_CMD_PROTINFO_C0_BLOB);
	if(!ret)
	{
		debug("blob c0 ram test step4 error!");
		while(1);
	}

	free(key_modifier);
	free(data_out);
	debug("blob c0 ram test ok!");
}

blob_rscp_test()
{

}

blob_black()
{
	void *ctx;
	struct cipher_ctx *ctxp;
	unsigned int blackkey_len;
	unsigned int *blackkey;
	unsigned int *blackkey_shadow;
	unsigned char key_modifier[32];

	unsigned char iv[16];
	unsigned char data[32],data_shadow[32];
	void *desc_raw;

	int i;

	memset_c(iv,0,16);
	memset_c(key_modifier,0x55,32);

//////////////////////////////////////////////////////////
	blackkey_len=32;
	//blackkey:[0]~[0x1F] blob key,[0x20~0x3F] plain key or cipher key,[0x40~0x4F]mac
	blackkey = (unsigned int *)malloc(blackkey_len+0x30);
	blackkey_shadow = (unsigned int *)malloc(blackkey_len+0x30);

	memset_c(blackkey,0,blackkey_len+0x30);
	memset_c(blackkey_shadow,0,blackkey_len+0x30);
	for(i=0x20>>2;i<(0x20+blackkey_len)>>2;i++)
	{
		blackkey[i]=rand();
		blackkey_shadow[i]= blackkey[i];
	}

	memset_c(data,0xa5,32);
	memset_c(data_shadow,0xa5,32);


	caam_cipher_setkey_enckey(&ctx,AES,ECB,enc,&(blackkey[8]),32,KEY_TYPE_AES_ECB_KEYR);
	ctxp = (struct cipher_ctx *)ctx;
	if(!memcmp_c(&(blackkey[8]),&(blackkey_shadow[8]),blackkey_len))
	{
		debug("blob black test,black key generate error!\n");
		while(1);
	}
	caam_cipher_setiv(ctx,iv,16);
	caam_cipher_crypt(ctx,data,32,1);
	caam_cipher_crypt_enckey_finish(ctx,32,KEY_TYPE_AES_ECB_KEYR);
	if(!memcmp_c(data,data_shadow,0x20))
	{
		debug("blob black test,enc error!\n");
		while(1);
	}

	blob_extopt_encap(AES, ECB, key_modifier,0x10, &(blackkey[8]),blackkey,blackkey_len,blackkey_len+0x30,PROT_OP_CMD_PROTINFO_BLACK_BLOB);
	if(!memcmp_c(blackkey+0x20,blackkey_shadow+0x20,0x20))
	{
		debug("blob black test,black key blob encap error!\n");
		while(1);
	}

	caam_cipher_enckey_from_blob(&ctx,AES,ECB,dec,KEY_TYPE_AES_ECB_KEYR);
	caam_cipher_setiv(ctx,iv,16);
	caam_cipher_crypt(ctx,data,32,1);
	caam_cipher_crypt_enckey_from_blob_finish(ctx,key_modifier,0x10,blackkey,&(blackkey[8]),blackkey_len+0x30,blackkey_len,KEY_TYPE_AES_ECB_KEYR);

	if(!memcmp_c(data,data_shadow,0x20))
	{
		debug("blob black test ok!\n");
	}else
	{
		debug("blob black test,dec error!\n");
		while(1);
	}

	free(blackkey);
	free(blackkey_shadow);
}

blob_test()
{

	blob_nomal_test();

	blob_mk_test();
	blob_tf_test();

	//blob_c0_test();

	blob_rscp_test();

	blob_black();

}

blob_aes_deal(void *dst,void *src,int len)
{
	int i;
	UINT8 key_modifier[16];
	memset_c(key_modifier,0x5a,16);
	blob_decap(AES, CCM, key_modifier,0x10, src, dst,len+0x30,len);
}

blob_sm4_deal(void *dst,void *src,int len)
{
	int i;
	UINT8 key_modifier[16];
	memset_c(key_modifier,0x5a,16);
	blob_decap(SM4, CCM, key_modifier,0x10, src, dst,len+0x30,len);
}
