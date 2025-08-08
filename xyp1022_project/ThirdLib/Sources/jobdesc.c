/*
 * SEC Descriptor Construction Library
 * Basic job descriptor construction
 *
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */

#include "common.h"
#include "sec.h"
#include "desc_constr.h"
#include "jobdesc.h"
#include "rsa_caam.h"
#include "desc.h"
#include "fsl_hash.h"
//#include "ppc_asm.h"
#include "std_data.h"
#include "cipher.h"

extern struct caam_hash_template *driver_hash;

#if defined(CONFIG_MX6) || defined(CONFIG_MX7)
/*!
 * Secure memory run command
 *
 * @param   sec_mem_cmd  Secure memory command register
 * @return  cmd_status  Secure memory command status register
 */
uint32_t secmem_set_cmd(uint32_t sec_mem_cmd)
{
	uint32_t temp_reg;

	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	uint32_t sm_vid = SM_VERSION(sec_in32(&sec->smvid));
	uint32_t jr_id = 0;

	sec_out32(CAAM_SMCJR(sm_vid, jr_id), sec_mem_cmd);

	do {
		temp_reg = sec_in32(CAAM_SMCSJR(sm_vid, jr_id));
	} while (temp_reg & CMD_COMPLETE);

	return temp_reg;
}

/*!
 * CAAM page allocation:
 * Allocates a partition from secure memory, with the id
 * equal to partition_num. This will de-allocate the page
 * if it is already allocated. The partition will have
 * full access permissions. The permissions are set before,
 * running a job descriptor. A memory page of secure RAM
 * is allocated for the partition.
 *
 * @param   page  Number of the page to allocate.
 * @param   partition  Number of the partition to allocate.
 * @return  0 on success, ERROR_IN_PAGE_ALLOC otherwise
 */
int caam_page_alloc(uint8_t page_num, uint8_t partition_num)
{
	uint32_t temp_reg;

	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	uint32_t sm_vid = SM_VERSION(sec_in32(&sec->smvid));
	uint32_t jr_id = 0;

	/*
	 * De-Allocate partition_num if already allocated to ARM core
	 */
	if (sec_in32(CAAM_SMPO_0) & PARTITION_OWNER(partition_num)) {
		temp_reg = secmem_set_cmd(PARTITION(partition_num) |
						CMD_PART_DEALLOC);
		if (temp_reg & SMCSJR_AERR) {
			printf("Error: De-allocation status 0x%X\n", temp_reg);
			return ERROR_IN_PAGE_ALLOC;
		}
	}

	/* set the access rights to allow full access */
	sec_out32(CAAM_SMAG1JR(sm_vid, jr_id, partition_num), 0xF);
	sec_out32(CAAM_SMAG2JR(sm_vid, jr_id, partition_num), 0xF);
	sec_out32(CAAM_SMAPJR(sm_vid, jr_id, partition_num), 0xFF);

	/* Now need to allocate partition_num of secure RAM. */
	/* De-Allocate page_num by starting with a page inquiry command */
	temp_reg = secmem_set_cmd(PAGE(page_num) | CMD_INQUIRY);

	/* if the page is owned, de-allocate it */
	if ((temp_reg & SMCSJR_PO) == PAGE_OWNED) {
		temp_reg = secmem_set_cmd(PAGE(page_num) | CMD_PAGE_DEALLOC);
		if (temp_reg & SMCSJR_AERR) {
			printf("Error: Allocation status 0x%X\n", temp_reg);
			return ERROR_IN_PAGE_ALLOC;
		}
	}

	/* Allocate page_num to partition_num */
	temp_reg = secmem_set_cmd(PAGE(page_num) | PARTITION(partition_num)
						| CMD_PAGE_ALLOC);
	if (temp_reg & SMCSJR_AERR) {
		printf("Error: Allocation status 0x%X\n", temp_reg);
		return ERROR_IN_PAGE_ALLOC;
	}
	/* page inquiry command to ensure that the page was allocated */
	temp_reg = secmem_set_cmd(PAGE(page_num) | CMD_INQUIRY);

	/* if the page is not owned => problem */
	if ((temp_reg & SMCSJR_PO) != PAGE_OWNED) {
		printf("Allocation of page %d in partition %d failed 0x%X\n",
		       temp_reg, page_num, partition_num);

		return ERROR_IN_PAGE_ALLOC;
	}

	return 0;
}

int inline_cnstr_jobdesc_blob_dek(uint32_t *desc, const uint8_t *plain_txt,
				       uint8_t *dek_blob, uint32_t in_sz)
{
	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	uint32_t sm_vid = SM_VERSION(sec_in32(&sec->smvid));
	uint32_t jr_id = 0;

	uint32_t ret = 0;
	u32 aad_w1, aad_w2;
	/* output blob will have 32 bytes key blob in beginning and
	 * 16 byte HMAC identifier at end of data blob */
	uint32_t out_sz = in_sz + KEY_BLOB_SIZE + MAC_SIZE;
	/* Setting HDR for blob */
	uint8_t wrapped_key_hdr[8] = {HDR_TAG, 0x00, WRP_HDR_SIZE + out_sz,
			     HDR_PAR, HAB_MOD, HAB_ALG, in_sz, HAB_FLG};

	/* initialize the blob array */
	memset(dek_blob, 0, out_sz + 8);
	/* Copy the header into the DEK blob buffer */
	memcpy(dek_blob, wrapped_key_hdr, sizeof(wrapped_key_hdr));

	/* allocating secure memory */
	ret = caam_page_alloc(PAGE_1, PARTITION_1);
	if (ret)
		return ret;

	/* Write DEK to secure memory */
	memcpy((uint32_t *)SEC_MEM_PAGE1, (uint32_t *)plain_txt, in_sz);

	unsigned long start = (unsigned long)SEC_MEM_PAGE1 &
				~(ARCH_DMA_MINALIGN - 1);
	unsigned long end = ALIGN(start + 0x1000, ARCH_DMA_MINALIGN);
	flush_dcache_range(start, end);

	/* Now configure the access rights of the partition */
	sec_out32(CAAM_SMAG1JR(sm_vid, jr_id, PARTITION_1), KS_G1);
	sec_out32(CAAM_SMAG2JR(sm_vid, jr_id, PARTITION_1), 0);
	sec_out32(CAAM_SMAPJR(sm_vid, jr_id, PARTITION_1), PERM);

	/* construct aad for AES */
	aad_w1 = (in_sz << OP_ALG_ALGSEL_SHIFT) | KEY_AES_SRC | LD_CCM_MODE;
	aad_w2 = 0x0;

	init_job_desc(desc, 0);

	append_cmd(desc, CMD_LOAD | CLASS_2 | KEY_IMM | KEY_ENC |
				(0x0c << LDST_OFFSET_SHIFT) | 0x08);

	append_u32(desc, aad_w1);

	append_u32(desc, aad_w2);

	append_cmd_ptr(desc, (dma_addr_t)SEC_MEM_PAGE1, in_sz, CMD_SEQ_IN_PTR);

	append_cmd_ptr(desc, (dma_addr_t)dek_blob + 8, out_sz, CMD_SEQ_OUT_PTR);

	append_operation(desc, OP_TYPE_ENCAP_PROTOCOL | OP_PCLID_BLOB |
						OP_PCLID_SECMEM);

	return ret;
}
#endif

void inline_cnstr_jobdesc_hash(uint32_t *desc,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 OP_ALG_AAI_HASH | OP_ALG_AS_INITFINAL |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	append_store(desc, dma_addr_out, storelen,
		     LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);

}

void inline_cnstr_jobdesc_hash_icvcheck(uint32_t *desc,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 OP_ALG_AAI_HASH | OP_ALG_AS_INITFINAL |
			 OP_ALG_ICV_ON | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	options =  LDST_CLASS_2_CCB | FIFOLD_TYPE_ICV |FIFOLD_TYPE_LAST2;
	append_fifo_load(desc, dma_addr_out, alg_size, options);

}

void inline_cnstr_jobdesc_init_hash(uint32_t *desc,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *dsize_buf,uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out,dsize_addr;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	dsize_addr = virt_to_phys((void *)dsize_buf);

	init_job_desc(desc, 0);
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 OP_ALG_AAI_HASH | OP_ALG_AS_INIT |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	//append_store(desc, dma_addr_out, storelen, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_store(desc, dma_addr_out, 80, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	//append_store(desc, dsize_addr, 8, LDST_CLASS_2_CCB | LDST_SRCDST_WORD_DATASZ_REG);
}

void inline_cnstr_jobdesc_init_hmac(uint32_t *desc,void *key,unsigned key_size,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *dsize_buf,uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out,dsize_addr,key_addr;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	dsize_addr = virt_to_phys((void *)dsize_buf);
	key_addr = virt_to_phys((void *)key);

	init_job_desc(desc, 0);

	append_key(desc, key_addr, key_size, CLASS_2 | KEY_DEST_CLASS_REG);

	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 OP_ALG_AAI_HMAC | OP_ALG_AS_INIT |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	//append_store(desc, dma_addr_out, storelen, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_store(desc, dma_addr_out, 80, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	//append_store(desc, dsize_addr, 8, LDST_CLASS_2_CCB | LDST_SRCDST_WORD_DATASZ_REG);
}

void inline_cnstr_jobdesc_init_hmac_split(uint32_t *desc,void *key,unsigned key_size,
			  const uint8_t *msg, uint32_t msgsz,uint8_t *digest,
			  u32 alg_type, int sg_tbl,u32 key_type)
{
	u32 options,key_opt;
	dma_addr_t dma_addr_in, dma_addr_out,dsize_addr,key_addr,iv_addr;

	unsigned int clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	key_addr = virt_to_phys((void *)key);

	init_job_desc(desc, 0);

	////////////////////////// //add at 2018-5-15
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////

	switch(key_type)
	{
		case KEY_TYPE_AES_ECB_KEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_ECB_SKEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_ECB_KEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_ECB_SKEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_AES_CCM_KEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_CCM_SKEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_CCM_KEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_CCM_SKEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
	}


	append_key(desc, key_addr, 128, CLASS_2 | key_opt | KEY_ENC);

	append_operation(desc, OP_TYPE_CLASS2_ALG |
			OP_ALG_AAI_HMAC_PRECOMP | OP_ALG_AS_INIT |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	append_store(desc, dma_addr_out, 80, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}

void inline_cnstr_jobdesc_update_hash(uint32_t *desc,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *dsize_buf,uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr,dsize_addr;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	iv_addr = virt_to_phys((void *)iv);
	dsize_addr = virt_to_phys((void *)dsize_buf);

	init_job_desc(desc, 0);
	//append_cmd_ptr(desc, dsize_addr,8,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_REG |LDST_CLASS_2_CCB );
	//append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_cmd_ptr(desc, iv_addr,80,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 OP_ALG_AAI_HASH | OP_ALG_AS_UPDATE |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	//append_store(desc, dma_addr_out, storelen,LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_store(desc, dma_addr_out, 80,LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	//append_store(desc, dsize_addr, 8, LDST_CLASS_2_CCB | LDST_SRCDST_WORD_DATASZ_REG);
}

void inline_cnstr_jobdesc_update_hmac(uint32_t *desc,void *key,unsigned key_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *dsize_buf,uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr,dsize_addr,key_addr;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	iv_addr = virt_to_phys((void *)iv);
	dsize_addr = virt_to_phys((void *)dsize_buf);
	key_addr = virt_to_phys((void *)key);

	init_job_desc(desc, 0);
	//append_cmd_ptr(desc, dsize_addr,8,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_REG |LDST_CLASS_2_CCB );
	//append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_key(desc, key_addr, key_size, CLASS_2 | KEY_DEST_CLASS_REG);
	append_cmd_ptr(desc, iv_addr,80,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 OP_ALG_AAI_HMAC | OP_ALG_AAI_HMAC_SUB | OP_ALG_AS_UPDATE |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	//append_store(desc, dma_addr_out, storelen,LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_store(desc, dma_addr_out, 80,LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	//append_store(desc, dsize_addr, 8, LDST_CLASS_2_CCB | LDST_SRCDST_WORD_DATASZ_REG);
}

void inline_cnstr_jobdesc_update_hmac_split(uint32_t *desc,void *key,unsigned key_size,uint8_t *iv,
			  const uint8_t *msg, uint32_t msgsz,uint8_t *digest,
			  u32 alg_type,int sg_tbl,u32 key_type)
{
	/* SHA 256 , output is of length 32 words */
	u32 options,key_opt;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr,dsize_addr,key_addr;
	unsigned int clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	iv_addr = virt_to_phys((void *)iv);
	key_addr = virt_to_phys((void *)key);

	init_job_desc(desc, 0);
	////////////////////////// //add at 2018-5-15
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////
	switch(key_type)
	{
		case KEY_TYPE_AES_ECB_KEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_ECB_SKEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_ECB_KEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_ECB_SKEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_AES_CCM_KEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_CCM_SKEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_CCM_KEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_CCM_SKEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
	}


	append_key(desc, key_addr, 128, CLASS_2 | key_opt | KEY_ENC);

	append_cmd_ptr(desc, iv_addr,80,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			OP_ALG_AAI_HMAC_PRECOMP | OP_ALG_AAI_HMAC_SUB | OP_ALG_AS_UPDATE |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	append_store(desc, dma_addr_out, 80,LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}

void inline_cnstr_jobdesc_final_hash(uint32_t *desc,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *msg, uint32_t msgsz,uint8_t *dsize_buf, uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr,dsize_addr;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	iv_addr = virt_to_phys((void *)iv);
	dsize_addr = virt_to_phys((void *)dsize_buf);

	init_job_desc(desc, 0);
	//append_cmd_ptr(desc, dsize_addr,8,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_REG | LDST_CLASS_2_CCB );
	//append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_cmd_ptr(desc, iv_addr,80,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 OP_ALG_AAI_HASH | OP_ALG_AS_FINALIZE |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	//append_store(desc, dma_addr_out, storelen, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_store(desc, dma_addr_out, 80, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}

void inline_cnstr_jobdesc_final_hmac(uint32_t *desc,void *key,unsigned key_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *msg, uint32_t msgsz,uint8_t *dsize_buf, uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr,dsize_addr,key_addr;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	iv_addr = virt_to_phys((void *)iv);
	dsize_addr = virt_to_phys((void *)dsize_buf);

	key_addr = virt_to_phys((void *)key);

	init_job_desc(desc, 0);
	//append_cmd_ptr(desc, dsize_addr,8,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_REG |LDST_CLASS_2_CCB );
	//append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_key(desc, key_addr, key_size, CLASS_2 | KEY_DEST_CLASS_REG);
	//append_cmd_ptr(desc, dsize_addr,8,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_REG | LDST_CLASS_2_CCB );
	//append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_cmd_ptr(desc, iv_addr,80,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			OP_ALG_AAI_HMAC | OP_ALG_AAI_HMAC_SUB | OP_ALG_AS_FINALIZE |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	//append_store(desc, dma_addr_out, storelen, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_store(desc, dma_addr_out, 80, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}

void inline_cnstr_jobdesc_final_hmac_split(uint32_t *desc,void *key,unsigned key_size,uint8_t *iv,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *digest,
			  u32 alg_type, int sg_tbl,u32 key_type)
{

	u32 options,key_opt;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr,dsize_addr,key_addr;
	unsigned int clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);
	iv_addr = virt_to_phys((void *)iv);

	key_addr = virt_to_phys((void *)key);

	init_job_desc(desc, 0);
	////////////////////////// //add at 2018-5-15
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////
	switch(key_type)
	{
		case KEY_TYPE_AES_ECB_KEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_ECB_SKEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_ECB_KEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_ECB_SKEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_AES_CCM_KEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_CCM_SKEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_CCM_KEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_CCM_SKEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
	}


	append_key(desc, key_addr, 128, CLASS_2 | key_opt | KEY_ENC);

	append_cmd_ptr(desc, iv_addr,80,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );
	append_operation(desc, OP_TYPE_CLASS2_ALG |
			OP_ALG_AAI_HMAC_PRECOMP | OP_ALG_AAI_HMAC_SUB | OP_ALG_AS_FINALIZE |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}
	append_store(desc, dma_addr_out, 80, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}


void inline_cnstr_jobdesc_hk(uint32_t *desc,uint8_t *key,uint32_t key_size,uint32_t hash_type,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl)
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t key_addr,iv_addr;

	unsigned long  clear_data = 0x01;
	b2l(&clear_data,4);

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);

	iv_addr = virt_to_phys(iv);
	key_addr = virt_to_phys(key);

	init_job_desc(desc, 0);

////////////////////////// //add at 2017-8-28
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

//	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
//////////////////////////

	if(key_size)
	{
		append_key(desc, key_addr, key_size, CLASS_2 | KEY_DEST_CLASS_REG);
		//append_cmd_ptr(desc, key_addr,(key_size+7)/8,CMD_LOAD | LDST_SRCDST_BYTE_KEY | LDST_CLASS_2_CCB );
		//b2l(&key_size,4);
		//append_load_as_imm(desc, &key_size,4,CMD_LOAD | LDST_SRCDST_WORD_KEYSZ_REG | LDST_CLASS_2_CCB );
	}

	if(iv_size)
	{
		append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_2_CCB );
	}

	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 hash_type | OP_ALG_AS_INITFINAL |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	append_store(desc, dma_addr_out, storelen,
		     LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}

unsigned int hash_size(unsigned int alg)
{
	unsigned int hs;
	switch(alg)
	{
		case MD5:
			hs = 16;
			break;
		case SHA0:
		case SHA1:
			hs = 20;
			break;
		case SHA224:
			hs = 28;
			break;
		case SHA256:
		case SM3:
			hs = 32;
			break;
		case SHA384:
			hs = 48;
			break;
		case SHA512:
			hs = 64;
			break;
		default:
			hs = 16;
			break;

	}
	return hs;
}

inline_cnstr_jobdesc_gspkey(uint32_t *desc,uint8_t *key,uint32_t key_size,uint8_t *iv,u32 alg_type,u32 key_type)
{
	dma_addr_t key_addr,iv_addr;
	u32 *jump_cmd;

	key_addr = virt_to_phys(key);
	iv_addr = virt_to_phys(iv);

	unsigned long clear_data = 0xffffffff;
	unsigned long clear_data2 = 0x60;

	unsigned sk_size;

	init_job_desc(desc, 0);

	append_load_as_imm(desc,&clear_data2,4,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_RNG_REG | LDST_CLASS_1_CCB);
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 OP_ALG_ALGSEL_RNG|			//OP_ALG
			 OP_ALG_AAI_RNG_JDKEK 		//OP_ALG_AAI
				);

	/////////////////////////
	jump_cmd = append_jump(desc, JUMP_CLASS_CLASS1);
	set_jump_tgt_here(desc, jump_cmd);
////////////////////////// //add at 2017-8-28
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
//////////////////////////
	append_key(desc, key_addr, key_size, CLASS_2 |  KEY_DEST_CLASS_REG);

	append_operation(desc, OP_TYPE_CLASS2_ALG | OP_ALG_AAI_HMAC_SPKEY_GEN | OP_ALG_AS_INIT| alg_type);

	//append_fifo_load_as_imm(desc, NULL, 0, LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2);

	key_size = 128;

	if(key_type<6)
	{
		if(key_type == KEY_TYPE_AES_ECB_KEYR)
		append_fifo_store(desc, key_addr, key_size,
			   LDST_CLASS_2_CCB | FIFOST_TYPE_ECB | FIFOST_TYPE_AES_TO_KEYR);
		if(key_type == KEY_TYPE_AES_ECB_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
				LDST_CLASS_2_CCB |FIFOST_TYPE_ECB | FIFOST_TYPE_AES_TO_SKEYR );
		if(key_type == KEY_TYPE_SM4_ECB_KEYR)
		append_fifo_store(desc, key_addr, key_size,
				LDST_CLASS_2_CCB |FIFOST_TYPE_ECB | FIFOST_TYPE_SM4_TO_KEYR );
		if(key_type == KEY_TYPE_SM4_ECB_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
				LDST_CLASS_2_CCB |FIFOST_TYPE_ECB | FIFOST_TYPE_SM4_TO_SKEYR );
	}
	else
	{
		//key_size = (key_size+12+7)&0xfffffff8;
		if(key_type == KEY_TYPE_AES_CCM_KEYR)
		append_fifo_store(desc, key_addr, key_size,
				LDST_CLASS_2_CCB |FIFOST_TYPE_CCM | FIFOST_TYPE_AES_TO_KEYR );
		if(key_type == KEY_TYPE_AES_CCM_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
				LDST_CLASS_2_CCB |FIFOST_TYPE_CCM | FIFOST_TYPE_AES_TO_SKEYR );
		if(key_type == KEY_TYPE_SM4_CCM_KEYR)
		append_fifo_store(desc, key_addr, key_size,
				LDST_CLASS_2_CCB |FIFOST_TYPE_CCM | FIFOST_TYPE_SM4_TO_KEYR );
		if(key_type == KEY_TYPE_SM4_CCM_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
				LDST_CLASS_2_CCB |FIFOST_TYPE_CCM | FIFOST_TYPE_SM4_TO_SKEYR );
	}

}

void inline_cnstr_jobdesc_hk_sp(uint32_t *desc,uint8_t *key,uint32_t hash_type,uint8_t *iv,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl,uint32_t key_type)	//split key
{
	/* SHA 256 , output is of length 32 words */
	uint32_t storelen = alg_size;
	u32 options;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t key_addr,iv_addr;

	unsigned key_opt;

	unsigned int clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)msg);
	dma_addr_out = virt_to_phys((void *)digest);

	iv_addr = virt_to_phys(iv);
	key_addr = virt_to_phys(key);

	init_job_desc(desc, 0);

	////////////////////////// //add at 2017-8-28
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////

	switch(key_type)
	{
		case KEY_TYPE_AES_ECB_KEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_ECB_SKEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_ECB_KEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_ECB_SKEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_AES_CCM_KEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_CCM_SKEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_CCM_KEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_CCM_SKEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
	}

	append_key(desc, key_addr, 128, CLASS_2 |
			KEY_ENC|key_opt);
	//append_cmd_ptr(desc, key_addr,128,CMD_LOAD | LDST_SRCDST_BYTE_KEY |
	//		   LDST_CLASS_2_CCB );


	//append_cmd_ptr(desc, iv_addr,80,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT | LDST_CLASS_2_CCB );

	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 hash_type | OP_ALG_AS_INITFINAL |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | alg_type);

	options = LDST_CLASS_2_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2;
	if (sg_tbl)
		options |= FIFOLDST_SGF;
	if (msgsz > 0xffff) {
		options |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, msgsz);
	} else {
		append_fifo_load(desc, dma_addr_in, msgsz, options);
	}

	//append_store(desc, dma_addr_out, storelen, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_store(desc, dma_addr_out, 80, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}

void inline_cnstr_jobdesc_cipher(uint32_t *desc,uint8_t *key,uint32_t key_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *src, uint32_t src_size, uint8_t *dst,
			  int sg_tbl,u32 alg_type,u32 alg,u32 dir)
{
	/* SHA 256 , output is of length 32 words */
	u32 options;
	u32 store_ops;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr;
	dma_addr_t key_addr;

//	unsigned long clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)src);
	dma_addr_out = virt_to_phys((void *)dst);

	iv_addr = virt_to_phys(iv);
	key_addr = virt_to_phys(key);

	init_job_desc(desc, START_INDEX);

	////////////////////////// //add at 2017-8-28
//	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

//	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////

	append_key(desc, key_addr, key_size, CLASS_1 |
			KEY_DEST_CLASS_REG);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB );

#ifdef INITFINAL_OK
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INITFINAL |
			 dir );
#else
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INIT |
			 dir );
#endif

	options = LDST_CLASS_1_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1;
	store_ops = FIFOST_TYPE_MESSAGE_DATA;
	if (sg_tbl)
	{
		options |= FIFOLDST_SGF;
		store_ops |= FIFOLDST_SGF;
	}
	if (src_size > 0xffff) {
		options |= FIFOLDST_EXT;
		store_ops |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, src_size);
		append_fifo_store(desc, dma_addr_out, 0, store_ops);
		append_cmd(desc, src_size);
	} else {
		append_fifo_load(desc, dma_addr_in, src_size, options);
		append_fifo_store(desc, dma_addr_out, src_size, store_ops);
	}
}


void inline_cnstr_jobdesc_ccm_icv(uint32_t *desc,uint8_t *key,uint32_t key_size,
			uint8_t *iv,uint32_t iv_size,uint8_t *aad,uint32_t aad_size,
			uint8_t *mac,uint32_t mac_len,const uint8_t *src, uint32_t src_size, uint8_t *dst,
			  int sg_tbl,u32 alg_type,u32 alg,u32 dir,u32 icv_on)
{
	/* SHA 256 , output is of length 32 words */
	u32 options;
	u32 store_ops;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr;
	dma_addr_t key_addr;
	dma_addr_t aad_in;
	dma_addr_t mac_addr;

	u32 *jump_cmd;

	unsigned long clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)src);
	dma_addr_out = virt_to_phys((void *)dst);

	iv_addr = virt_to_phys(iv);
	key_addr = virt_to_phys(key);

	aad_in = virt_to_phys(aad);
	mac_addr = virt_to_phys(mac);

	init_job_desc(desc, START_INDEX);

	////////////////////////// //add at 2017-8-28
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	jump_cmd = append_jump(desc, JUMP_CLASS_NONE);
	set_jump_tgt_here(desc, jump_cmd);
	//////////////////////////

	append_key(desc, key_addr, key_size, CLASS_1 |
			KEY_DEST_CLASS_REG);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB );



#ifdef INITFINAL_OK
	if(dir==dec)
	{
		append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INITFINAL |((icv_on==icvoff)?0:OP_ALG_ICV_ON) |
			 dir );
	}else
	{
		append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INITFINAL |
			 dir );
	}
#else
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INIT |
			 dir );
#endif

	if(aad_size > 0xffff)
	{
		options = LDST_CLASS_1_CCB | FIFOLD_TYPE_AAD |FIFOLDST_EXT;
		append_fifo_load(desc, aad_in,0, options);
		append_cmd(desc, aad_size);
	}else
	{
		options = LDST_CLASS_1_CCB | FIFOLD_TYPE_AAD ;
		append_fifo_load(desc, aad_in,aad_size, options);
	}

	options = LDST_CLASS_1_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1;
	store_ops = FIFOST_TYPE_MESSAGE_DATA;
	if (sg_tbl)
	{
		options |= FIFOLDST_SGF;
		store_ops |= FIFOLDST_SGF;
	}
	if (src_size > 0xffff) {
		options |= FIFOLDST_EXT;
		store_ops |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, src_size);
		append_fifo_store(desc, dma_addr_out, 0, store_ops);
		append_cmd(desc, src_size);
	} else {
		append_fifo_load(desc, dma_addr_in, src_size, options);
		append_fifo_store(desc, dma_addr_out, src_size, store_ops);
	}
	//note load mac to icv
	if(dir==dec)
	{
		if(icv_on == icvon)
		{
			//append_cmd_ptr(desc, mac_addr,mac_len,CMD_FIFO_LOAD | FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_LAST1 | FIFOLD_TYPE_ICV);
			append_cmd_ptr(desc, mac_addr,mac_len,CMD_FIFO_LOAD | FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_LAST1 | FIFOLD_TYPE_ICV);
		}else
			//append_store(desc, mac_addr, mac_len,LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
		append_store(desc, mac_addr,mac_len,LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
	}
	else
	{
		//append_store(desc, mac_addr, mac_len, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
		append_store(desc, mac_addr, mac_len, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
	}

}

void inline_cnstr_jobdesc_cipher_chunks_final(uint32_t *desc,uint8_t *key,uint32_t key_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *src, uint32_t src_size, uint8_t *dst,
			  int sg_tbl,u32 alg_type,u32 alg,u32 dir)
{
	/* SHA 256 , output is of length 32 words */
	u32 options;
	u32 store_ops;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr;
	dma_addr_t key_addr;

//	unsigned long clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)src);
	dma_addr_out = virt_to_phys((void *)dst);

	iv_addr = virt_to_phys(iv);
	key_addr = virt_to_phys(key);

	init_job_desc(desc, START_INDEX);

	////////////////////////// //add at 2017-8-28
//	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

//	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////

	append_key(desc, key_addr, key_size, CLASS_1 |
			KEY_DEST_CLASS_REG);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB );

#ifdef INITFINAL_OK
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INITFINAL |
			 dir );
#else
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INIT |
			 dir );
#endif

	options = LDST_CLASS_1_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1;
	store_ops = FIFOST_TYPE_MESSAGE_DATA;
	if (sg_tbl)
	{
		options |= FIFOLDST_SGF;
		store_ops |= FIFOLDST_SGF;
	}
	if (src_size > 0xffff) {
		options |= FIFOLDST_EXT;
		store_ops |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, src_size);
		append_fifo_store(desc, dma_addr_out, 0, store_ops);
		append_cmd(desc, src_size);
	} else {
		append_fifo_load(desc, dma_addr_in, src_size, options);
		append_fifo_store(desc, dma_addr_out, src_size, store_ops);
	}
	append_store(desc, iv_addr, iv_size, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
}

void inline_jdkek_gen(uint32_t *desc)
{

	unsigned long dsize_data = 0x60000000;

	init_job_desc(desc, START_INDEX);

	append_load_as_imm(desc,&dsize_data,4,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_RNG_REG| LDST_CLASS_1_CCB);

	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_SK);
}

void inline_cnstr_jobdesc_cipher_enckey_gen(uint32_t *desc,uint8_t *key,uint32_t key_size,u32 key_type)	//key type:0==> AES ECB,key type:1==> AES CCM
{
	u32 options;
	u32 store_ops;
	dma_addr_t key_addr;
	u32* jump_cmd;

	unsigned  clear_data1 = 0xffffffff;
	unsigned  clear_data2 = 0x60;

	key_addr = virt_to_phys(key);

	init_job_desc(desc, START_INDEX);

	////////////////////////// //add at 2017-8-28
	append_load_as_imm(desc,&clear_data1,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);
	//////////////////////////
	///////////////////////// //add at 2018-4-16
	append_load_as_imm(desc,&clear_data2,4,CMD_LOAD | LDST_SRCDST_WORD_DATASZ_RNG_REG | LDST_CLASS_1_CCB);
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 OP_ALG_ALGSEL_RNG|			//OP_ALG
			 OP_ALG_AAI_RNG_JDKEK 		//OP_ALG_AAI
				);

	/////////////////////////
	jump_cmd = append_jump(desc, JUMP_CLASS_NONE);
	set_jump_tgt_here(desc, jump_cmd);

	append_key(desc, key_addr, key_size, CLASS_1 |
			KEY_DEST_CLASS_REG);

	if(key_type<6)
	{
		if(key_type == KEY_TYPE_AES_ECB_KEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_ECB | FIFOST_TYPE_AES_TO_KEYR |MOVE_AUX_LS);
		if(key_type == KEY_TYPE_AES_ECB_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_ECB | FIFOST_TYPE_AES_TO_SKEYR |MOVE_AUX_LS);
		if(key_type == KEY_TYPE_SM4_ECB_KEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_ECB | FIFOST_TYPE_SM4_TO_KEYR |MOVE_AUX_LS);
		if(key_type == KEY_TYPE_SM4_ECB_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_ECB | FIFOST_TYPE_SM4_TO_SKEYR |MOVE_AUX_LS);
	}
	else
	{
		//key_size = (key_size+12+7)&0xfffffff8;
		if(key_type == KEY_TYPE_AES_CCM_KEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_CCM | FIFOST_TYPE_AES_TO_KEYR |MOVE_AUX_LS);
		if(key_type == KEY_TYPE_AES_CCM_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_CCM | FIFOST_TYPE_AES_TO_SKEYR |MOVE_AUX_LS);
		if(key_type == KEY_TYPE_SM4_CCM_KEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_CCM | FIFOST_TYPE_SM4_TO_KEYR |MOVE_AUX_LS);
		if(key_type == KEY_TYPE_SM4_CCM_SKEYR)
		append_fifo_store(desc, key_addr, key_size,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_CCM | FIFOST_TYPE_SM4_TO_SKEYR |MOVE_AUX_LS);
	}

}

void inline_cnstr_jobdesc_cipher_enckey_do(uint32_t *desc,uint8_t *key,uint32_t key_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *src, uint32_t src_size, uint8_t *dst,
			  int sg_tbl,u32 alg_type,u32 alg,u32 dir,u32 key_type)	//key type:0==> AES ECB,key type:1==> AES CCM
{
	/* SHA 256 , output is of length 32 words */
	u32 options;
	u32 store_ops;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr;
	dma_addr_t key_addr;

	unsigned key_opt=0;

	unsigned long clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)src);
	dma_addr_out = virt_to_phys((void *)dst);

	iv_addr = virt_to_phys(iv);
	key_addr = virt_to_phys(key);

	switch(key_type)
	{
		case KEY_TYPE_AES_ECB_KEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_ECB_SKEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_ECB_KEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_ECB_SKEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_AES_CCM_KEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_CCM_SKEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_CCM_KEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_CCM_SKEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
	}

	init_job_desc(desc, START_INDEX);

	////////////////////////// //add at 2017-8-28
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////

	append_key(desc, key_addr, key_size, CLASS_1 |
			KEY_ENC|key_opt);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB );

#ifdef INITFINAL_OK
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INITFINAL |
			 dir );
#else
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INIT |
			 dir );
#endif

	options = LDST_CLASS_1_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1;
	store_ops = FIFOST_TYPE_MESSAGE_DATA;
	if (sg_tbl)
	{
		options |= FIFOLDST_SGF;
		store_ops |= FIFOLDST_SGF;
	}
	if (src_size > 0xffff) {
		options |= FIFOLDST_EXT;
		store_ops |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, src_size);
		append_fifo_store(desc, dma_addr_out, 0, store_ops);
		append_cmd(desc, src_size);
	} else {
		append_fifo_load(desc, dma_addr_in, src_size, options);
		append_fifo_store(desc, dma_addr_out, src_size, store_ops);
	}
}

void inline_cnstr_jobdesc_cipher_enckey_do2(uint32_t *desc,uint8_t *key,uint32_t key_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *src, uint32_t src_size, uint8_t *dst,
			  int sg_tbl,u32 alg_type,u32 alg,u32 dir,u32 key_type)	//key type:0==> AES ECB,key type:1==> AES CCM
{
	/* SHA 256 , output is of length 32 words */
	u32 options;
	u32 store_ops;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr;
	dma_addr_t key_addr;

	unsigned key_opt=0;

	unsigned long clear_data = 0xffffffff;

	dma_addr_in = virt_to_phys((void *)src);
	dma_addr_out = virt_to_phys((void *)dst);

	iv_addr = virt_to_phys(iv);
	key_addr = virt_to_phys(key);

	switch(key_type)
	{
		case KEY_TYPE_AES_ECB_KEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_ECB_SKEYR:
		{
			key_opt = KEY_AES|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_ECB_KEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_ECB_SKEYR:
		{
			key_opt = KEY_SM4|KEY_ECB|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_AES_CCM_KEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_AES_CCM_SKEYR:
		{
			key_opt = KEY_AES|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
		case KEY_TYPE_SM4_CCM_KEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_CLASS_REG;
			break;
		}
		case KEY_TYPE_SM4_CCM_SKEYR:
		{
			key_opt = KEY_SM4|KEY_CCM|KEY_DEST_MDHA_SPLIT;
			break;
		}
	}

	//init_job_desc(desc, START_INDEX);

	////////////////////////// //add at 2017-8-28
	append_load_as_imm(desc,&clear_data,4,CMD_LOAD | LDST_SRCDST_WORD_CLRW | LDST_CLASS_IND_CCB);

	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	//////////////////////////

	append_key(desc, key_addr, key_size, CLASS_1 |
			KEY_ENC|key_opt);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB );

#ifdef INITFINAL_OK
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INITFINAL |
			 dir );
#else
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (alg_type<<4) | //OP_ALG_AAI
			 (alg<<16)|		//OP_ALG
			 OP_ALG_AS_INIT |
			 dir );
#endif

	options = LDST_CLASS_1_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1;
	store_ops = FIFOST_TYPE_MESSAGE_DATA;
	if (sg_tbl)
	{
		options |= FIFOLDST_SGF;
		store_ops |= FIFOLDST_SGF;
	}
	if (src_size > 0xffff) {
		options |= FIFOLDST_EXT;
		store_ops |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, src_size);
		append_fifo_store(desc, dma_addr_out, 0, store_ops);
		append_cmd(desc, src_size);
	} else {
		append_fifo_load(desc, dma_addr_in, src_size, options);
		append_fifo_store(desc, dma_addr_out, src_size, store_ops);
	}
}

void inline_cnstr_jobdesc_cipher_hash_snoop(uint32_t *desc,uint8_t *ckey,uint32_t ckey_size,uint8_t* hkey,uint32_t hkey_size,uint8_t *iv,uint32_t iv_size,
			  const uint8_t *src, uint32_t src_size, uint8_t *dst,uint8_t *icv_out,
			  int sg_tbl,u32 calg_type,u32 calg,u32 h_alg,u32 h_alg_type,u32 dir,u32 snoop_type)
{
	/* SHA 256 , output is of length 32 words */
	u32 options;
	u32 store_ops;
	dma_addr_t dma_addr_in, dma_addr_out;
	dma_addr_t iv_addr,icv_addr;
	dma_addr_t ckey_addr,hkey_addr;

	dma_addr_in = virt_to_phys((void *)src);
	dma_addr_out = virt_to_phys((void *)dst);

	iv_addr = virt_to_phys(iv);
	ckey_addr = virt_to_phys(ckey);
	hkey_addr = virt_to_phys(hkey);
	icv_addr = virt_to_phys(icv_out);

	init_job_desc(desc, 0);

	append_key(desc, ckey_addr, ckey_size, CLASS_1 |
			KEY_DEST_CLASS_REG);

	append_key(desc, hkey_addr, hkey_size, CLASS_2 |
			KEY_DEST_CLASS_REG);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB );

	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (calg_type<<4) | //OP_ALG_AAI
			 (calg<<16)|		//OP_ALG
			 OP_ALG_AS_INITFINAL |
			 dir );

	append_operation(desc, OP_TYPE_CLASS2_ALG |
			 h_alg | OP_ALG_AS_INITFINAL |
			 OP_ALG_ENCRYPT | OP_ALG_ICV_OFF | h_alg_type);

	append_move(desc, MOVE_SRC_CLASS1CTX | MOVE_DEST_CLASS2INFIFO | iv_size);

	options = FIFOLD_CLASS_BOTH| FIFOLD_TYPE_MSG | FIFOLD_TYPE_LASTBOTH| snoop_type;
	store_ops = FIFOST_TYPE_MESSAGE_DATA;
	if (sg_tbl)
	{
		options |= FIFOLDST_SGF;
		store_ops |= FIFOLDST_SGF;
	}
	if (src_size > 0xffff) {
		options |= FIFOLDST_EXT;
		store_ops |= FIFOLDST_EXT;
		append_fifo_load(desc, dma_addr_in, 0, options);
		append_cmd(desc, src_size);
		append_fifo_store(desc, dma_addr_out, 0, store_ops);
		append_cmd(desc, src_size);
	} else {
		append_fifo_load(desc, dma_addr_in, src_size, options);
		append_fifo_store(desc, dma_addr_out, src_size, store_ops);
	}
	append_store(desc, icv_addr, 80,
		     LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);
}



#if 1
void inline_cnstr_jobdesc_blob_encap(uint32_t *desc, uint8_t *key_idnfr,
				     uint8_t *plain_txt, uint8_t *enc_blob,
				     uint32_t in_sz)
{
	dma_addr_t dma_addr_key_idnfr, dma_addr_in, dma_addr_out;
	uint32_t key_sz = KEY_IDNFR_SZ_BYTES;
	/* output blob will have 32 bytes key blob in beginning and
	 * 16 byte HMAC identifier at end of data blob */
	uint32_t out_sz = in_sz + KEY_BLOB_SIZE + MAC_SIZE;

	dma_addr_key_idnfr = virt_to_phys((void *)key_idnfr);
	dma_addr_in	= virt_to_phys((void *)plain_txt);
	dma_addr_out	= virt_to_phys((void *)enc_blob);

	init_job_desc(desc, 0);

	append_key(desc, dma_addr_key_idnfr, key_sz, CLASS_2);

	append_seq_in_ptr(desc, dma_addr_in, in_sz, 0);

	append_seq_out_ptr(desc, dma_addr_out, out_sz, 0);

	append_operation(desc, OP_TYPE_ENCAP_PROTOCOL | OP_PCLID_BLOB);
}

void inline_cnstr_jobdesc_blob_decap(uint32_t *desc, uint8_t *key_idnfr,
				     uint8_t *enc_blob, uint8_t *plain_txt,
				     uint32_t out_sz)
{
	dma_addr_t dma_addr_key_idnfr, dma_addr_in, dma_addr_out;
	uint32_t key_sz = KEY_IDNFR_SZ_BYTES;
	uint32_t in_sz = out_sz + KEY_BLOB_SIZE + MAC_SIZE;

	dma_addr_key_idnfr = virt_to_phys((void *)key_idnfr);
	dma_addr_in	= virt_to_phys((void *)enc_blob);
	dma_addr_out	= virt_to_phys((void *)plain_txt);

	init_job_desc(desc, 0);

	append_key(desc, dma_addr_key_idnfr, key_sz, CLASS_2);

	append_seq_in_ptr(desc, dma_addr_in, in_sz, 0);

	append_seq_out_ptr(desc, dma_addr_out, out_sz, 0);

	append_operation(desc, OP_TYPE_DECAP_PROTOCOL | OP_PCLID_BLOB);
}
#endif

/*
 * Descriptor to instantiate RNG State Handle 0 in normal mode and
 * load the JDKEK, TDKEK and TDSK registers
 */
#if 0
void inline_cnstr_jobdesc_rng_instantiation(uint32_t *desc)
{
	u32 *jump_cmd;

	init_job_desc(desc, 0);

	/* INIT RNG in non-test mode */
	append_operation(desc, OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG |
			 OP_ALG_AS_INIT);

	/* wait for done */
	jump_cmd = append_jump(desc, JUMP_CLASS_CLASS1);
	set_jump_tgt_here(desc, jump_cmd);

	/*
	 * load 1 to clear written reg:
	 * resets the done interrrupt and returns the RNG to idle.
	 */
	append_load_imm_u32(desc, 1, LDST_SRCDST_WORD_CLRW);

	/* generate secure keys (non-test) */
	append_operation(desc, OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG |
			 OP_ALG_RNG4_SK);
}
#endif

/* Change key size to bytes form bits in calling function*/
void inline_cnstr_jobdesc_pkha_rsaexp(uint32_t *desc,
				      struct pk_in_params *pkin, uint8_t *out,
				      uint32_t out_siz)
{
	dma_addr_t dma_addr_e, dma_addr_a, dma_addr_n, dma_addr_out;

	dma_addr_e = virt_to_phys((void *)pkin->e);
	dma_addr_a = virt_to_phys((void *)pkin->a);
	dma_addr_n = virt_to_phys((void *)pkin->n);
	dma_addr_out = virt_to_phys((void *)out);

	init_job_desc(desc, 0);
	append_key(desc, dma_addr_e, pkin->e_siz, KEY_DEST_PKHA_E | CLASS_1);

	append_fifo_load(desc, dma_addr_a,
			 pkin->a_siz, LDST_CLASS_1_CCB | FIFOLD_TYPE_PK_A);

	append_fifo_load(desc, dma_addr_n,
			 pkin->n_siz, LDST_CLASS_1_CCB | FIFOLD_TYPE_PK_N);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);

	append_fifo_store(desc, dma_addr_out, out_siz,
			  LDST_CLASS_1_CCB | FIFOST_TYPE_PKHA_B);
}

void load_store_cmd_test()
{
	uint32_t *desc,*desc_raw;
	uint8_t *iv_raw,*dst_raw,*iv,*dst;
	uint32_t iv_size=16;
	dma_addr_t iv_addr,dst_addr;
	desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
	iv_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	dst_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);

	iv = (uint8_t *)(((unsigned long)iv_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	dst = (uint8_t *)(((unsigned long)dst_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	iv_addr = virt_to_phys(iv);
	dst_addr = virt_to_phys(dst);
	memset(iv,0x5a,iv_size);
	memset(dst,0x37,iv_size);
	init_job_desc(desc, START_INDEX);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
			   LDST_CLASS_2_CCB );
	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
			   LDST_CLASS_2_CCB );
	append_store(desc, dst_addr, iv_size,
		     LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);

	run_descriptor_jr(desc,&jr_t);

	if(memcmp_c(iv,dst,iv_size))		//if equ !=0	error
	{
		debug("compared error!");
		while(1);
	}

	free(iv_raw);
	free(dst_raw);
	free(desc_raw);
	while(1);
}

load_store_shared_reo_simple()		//need to edit for share desc
{
	uint32_t *sh_desc,*sh_desc_raw,*job_desc,*job_desc_raw;
	uint8_t *iv_raw,*dst_raw,*iv,*dst;
	uint32_t iv_size=16;
	dma_addr_t iv_addr,dst_addr;
	unsigned int sh_len;
	int i;

	sh_desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
	job_desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
	iv_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	dst_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);

	iv = (uint8_t *)(((unsigned long)iv_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	dst = (uint8_t *)(((unsigned long)dst_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	sh_desc = (uint32_t *)(((unsigned long)sh_desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	job_desc = (uint32_t *)(((unsigned long)job_desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	iv_addr = virt_to_phys(iv);
	dst_addr = virt_to_phys(dst);
	memset(iv,0x5a,iv_size);
	memset(dst,0x37,iv_size);

	init_sh_desc(sh_desc, HDR_SHARE_NEVER);

	append_cmd_ptr(sh_desc, dst_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
			   LDST_CLASS_2_CCB );
	append_move(sh_desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));

	append_cmd_ptr(sh_desc, iv_addr,iv_size,CMD_STORE | LDST_SRCDST_BYTE_CONTEXT |
			   LDST_CLASS_2_CCB );

	sh_len = desc_len(sh_desc);

	init_job_desc_shared(job_desc, virt_to_phys(sh_desc), sh_len, HDR_SHARE_DEFER | HDR_REVERSE);

	append_cmd_ptr(job_desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
			   LDST_CLASS_2_CCB );
	append_move(job_desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));

	append_cmd_ptr(job_desc, dst_addr,iv_size,CMD_STORE | LDST_SRCDST_BYTE_CONTEXT |
			   LDST_CLASS_2_CCB );

	ccore_job_desc_len(job_desc);

	run_descriptor_jr(job_desc,&jr_t);
	//run_descriptor_jr(sh_desc,&jr_t);

	for(i=0;i<iv_size;i++)
	{
		if(dst[i]!=0x5a)
		{
			PSPRINTF("load_store_shared_reo_simple error!\n");
			asm("trap");
		}
	}

	PSPRINTF("load_store_shared_reo_simple ok!\n");

	free(iv_raw);
	free(dst_raw);
	free(sh_desc_raw);
	free(job_desc_raw);
}

fifo_load_cipher_shared_test()
{
	int i;
	uint32_t *sh_desc,*sh_desc_raw,*job_desc,*job_desc_raw;
	uint8_t *src_raw,*dst_raw,*key_raw,*src,*dst,*key;
	uint32_t src_size=64;
	dma_addr_t src_addr,dst_addr,key_addr;
	unsigned int sh_len;
	struct std_data *aes_raw;
	sh_desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
	job_desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
	src_raw = (uint8_t *)malloc(src_size+DATA_ALIGN);
	dst_raw = (uint8_t *)malloc(src_size+DATA_ALIGN);		//for src_size = dst_size for cipher
	key_raw = (uint8_t *)malloc(16+DATA_ALIGN);

	STD_AES(&aes_raw);

	key = (uint8_t *)(((unsigned long)key_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	src = (uint8_t *)(((unsigned long)src_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	dst = (uint8_t *)(((unsigned long)dst_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	sh_desc = (uint32_t *)(((unsigned long)sh_desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	job_desc = (uint32_t *)(((unsigned long)job_desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	src_addr = virt_to_phys(src);
	dst_addr = virt_to_phys(dst);
	key_addr = virt_to_phys(key);

	memcpy_c(key,std_aes->key,16);
	memcpy_c(src,std_aes->iv,16);		//16 is aes iv size
	memcpy_c(src+16,std_aes->data,16);	//16 is aes data size

	init_sh_desc(sh_desc, HDR_SHARE_NEVER);
	append_seq_in_ptr(sh_desc, src_addr, 32, 0);
	append_seq_out_ptr(sh_desc, dst_addr, 16, 0);

	sh_len = desc_len(sh_desc);

	init_job_desc_shared(job_desc, virt_to_phys(sh_desc), sh_len, HDR_SHARE_DEFER );

	append_math_sub_imm_u32(job_desc, REG3, SEQINLEN, IMM, 16);	//reg3=seqinlen-datalen = 32-16 =16
	append_math_sub_imm_u32(job_desc, REG2, SEQINLEN, IMM, 0);	//reg2=seqinlen = 32
	//16 is aes data size, SEQOUTLEN is 32 include iv size & data_size
	append_math_add(job_desc, VARSEQINLEN, REG3, REG0, CAAM_CMD_SZ);	// first REG0 is 0,iv len = vlen = reg3
	append_math_add(job_desc, VARSEQOUTLEN, REG3, REG0, CAAM_CMD_SZ);	// first REG0 is 0,iv len = vlen = reg3

	append_key(job_desc, key_addr,
		   16, CLASS_1 | KEY_DEST_CLASS_REG);

	append_cmd(job_desc, CMD_SEQ_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB | 16);

	append_operation(job_desc, OP_TYPE_CLASS1_ALG |
			 (0x20<<4) | //OP_ALG_AAI		ECB
			 (0x10<<16)|		//OP_ALG	AES
			 OP_ALG_AS_INITFINAL |
			 //OP_ALG_AS_INIT |
			 1);

	append_math_sub(job_desc, VARSEQINLEN, REG2, REG3, CAAM_CMD_SZ);	//datalen = vlen = seqoutlen-reg3
	append_seq_fifo_load(job_desc, 0, FIFOLD_CLASS_CLASS1 | \
			     KEY_VLF |  FIFOLD_TYPE_MSG	 | FIFOLD_TYPE_LAST1);

	append_seq_fifo_store(job_desc, 0, FIFOST_TYPE_MESSAGE_DATA | KEY_VLF);

	ccore_job_desc_len(job_desc);

	run_descriptor_jr(job_desc,&jr_t);

	for(i=0;i<16;i++)
	{
		if(dst[i]!=std_aes->stdout[i])
			asm("trap");
	}

	PSPRINTF("fifo_load_cipher_shared_test ok!\n");

	free(key_raw);
	free(src_raw);
	free(dst_raw);
	free(sh_desc_raw);
	free(job_desc_raw);
	free(aes_raw);
}

deco_single_test()
{
	uint32_t *desc;
	uint8_t *iv_raw,*dst_raw,*iv,*dst;
	uint32_t iv_size=16;
	dma_addr_t iv_addr,dst_addr;
	int i,length;
	desc = (uint32_t *)DECO_DxDESB;
	iv_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	dst_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	iv = (uint8_t *)(((unsigned long)iv_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	dst = (uint8_t *)(((unsigned long)dst_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	iv_addr = virt_to_phys(iv);
	dst_addr = virt_to_phys(dst);
	memset(iv,0x5a,iv_size);
	memset(dst,0x37,iv_size);
	init_job_desc(desc, START_INDEX);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
				   LDST_CLASS_2_CCB );
	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
				   LDST_CLASS_2_CCB );
	append_store(desc, dst_addr, iv_size,
			     LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);


	#if 0
	length = desc_len(desc);
	for (i = 0; i < length; i++) {
	desc[i] = cpu_to_le32(desc[i]);
	}
	#endif
	//because in the sys set before,the sec reg region tlb is set to little endian,so not changle endian
	*(uint32_t *)DECO_DxJQCRH = DECO_DxJQCRH_SINGLE;
	while(!(*(uint32_t *)DECO_DxJQCRH & DECO_DxJQCRH_DONE));
	//*(uint32_t *)DECO_DxJQCRH = DECO_DxJQCRH_DONE;

	while(1);

}

deco_single_step_test()
{
	uint32_t *desc;
	uint8_t *iv_raw,*dst_raw,*iv,*dst;
	uint32_t iv_size=16;
	dma_addr_t iv_addr,dst_addr;
	int i,length;
	uint32_t view_cmd1,view_cmd2,view_cmd3;
	desc = (uint32_t *)DECO_DxDESB;
	iv_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	dst_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	iv = (uint8_t *)(((unsigned long)iv_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	dst = (uint8_t *)(((unsigned long)dst_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	iv_addr = virt_to_phys(iv);
	//iv_addr = 0xF0000000UL;
	dst_addr = virt_to_phys(dst);
	memset(iv,0x5a,iv_size);
	memset(dst,0x37,iv_size);
	init_job_desc(desc, START_INDEX);

	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
				   LDST_CLASS_2_CCB );
	append_cmd_ptr(desc, iv_addr,iv_size,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
				   LDST_CLASS_2_CCB );
	append_store(desc, dst_addr, iv_size,
			     LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT);

	//because in the sys set before,the sec reg region tlb is set to little endian,so not changle endian
	#if 0
	length = desc_len(desc);
	for (i = 0; i < length; i++) {
		desc[i] = cpu_to_le32(desc[i]);
	}
	#endif
	*(uint32_t *)DECO_DxJQCRH = DECO_DxJQCRH_SINGLE|DECO_DxJQCRH_STEP;
	view_cmd1 = *(uint32_t *)DECO_DxJQCRL;
	view_cmd2 = *(uint32_t *)DECO_DxJQCRL;
	view_cmd3 = *(uint32_t *)DECO_DxJQCRL;
	view_cmd1 = *(uint32_t *)DECO_DxJQCRL;
	view_cmd2 = *(uint32_t *)DECO_DxJQCRL;
	view_cmd3 = *(uint32_t *)DECO_DxJQCRL;
	while(!(*(uint32_t *)DECO_DxJQCRH & DECO_DxJQCRH_DONE));
	*(uint32_t *)DECO_DxJQCRH = DECO_DxJQCRH_DONE;
    while(1);
}

deco_single_step_test2()
{
	uint32_t *desc;
	uint8_t *iv_raw,*dst_raw,*iv,*dst,*key,*key_raw,*src,*src_raw;
	uint32_t iv_size=16;
	uint32_t options,store_ops;
	dma_addr_t iv_addr,dst_addr,key_addr,src_addr;
	int i,length;
	uint32_t view_cmd1,view_cmd2,view_cmd3;
	int cmdcnt=5;

	uint8_t keyN[] = {0xAF,0x86,0x18,0x23,0x8C,0x94,0xA1,0x19,0xAE,0x6D,0xE9,0x22,0xDB,0xB9,0x35,0x4D};
	uint8_t ivN[] = {0x38,0x32,0xF2,0x7E,0x0A,0x14,0x51,0xAF,0x4B,0x97,0x47,0xA0,0x17,0x7C,0xDF,0xC2};
	uint8_t dataN[] = {0xF7,0xAF,0x83,0xD4,0x98,0x7A,0x94,0x83,0x98,0xEE,0x24,0x8A,0xD8,0xB7,0x09,0x95};
	uint8_t outN[] = {0xa9,0x18,0x0e,0x6c,0xf0,0x2c,0x4a,0xad,0xe2,0x57,0x3b,0xc1,0x50,0x9e,0x26,0x41};

	desc = (uint32_t *)DECO_DxDESB;
	iv_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	dst_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	key_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	src_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	dst_raw = (uint8_t *)malloc(iv_size+DATA_ALIGN);
	iv = (uint8_t *)(((unsigned long)iv_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	dst = (uint8_t *)(((unsigned long)dst_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	key = (uint8_t *)(((unsigned long)key_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	src = (uint8_t *)(((unsigned long)src_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	dst = (uint8_t *)(((unsigned long)dst_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));

	iv_addr = virt_to_phys(iv);
	dst_addr = virt_to_phys(dst);
	key_addr = virt_to_phys(key);
	src_addr = virt_to_phys(src);
	dst_addr = virt_to_phys(dst);

	memcpy_c(key,keyN,16);
	memcpy_c(iv,ivN,16);
	memcpy_c(src,dataN,16);

	init_job_desc(desc, START_INDEX);

	append_key(desc, key_addr, 16, CLASS_1 |
			KEY_DEST_CLASS_REG);

	append_cmd_ptr(desc, iv_addr,16,CMD_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB );


	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (0x20<<4) | //OP_ALG_AAI		ECB
			 (0x10<<16)|		//OP_ALG	AES
			 //OP_ALG_AS_INITFINAL |
			 OP_ALG_AS_INIT |
			 1 );		//enc
/*
	append_operation(desc, OP_TYPE_CLASS1_ALG |
			 (0x10<<4) | //OP_ALG_AAI		CBC
			 (0x10<<16)|		//OP_ALG	AES
			 //OP_ALG_AS_INITFINAL |
			 OP_ALG_AS_INIT |
			 1 );		//enc
*/
	options = LDST_CLASS_1_CCB | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1;
	//options = LDST_CLASS_1_CCB | FIFOLD_TYPE_MSG ;
	store_ops = FIFOST_TYPE_MESSAGE_DATA;

	append_fifo_load(desc, src_addr, 16, options);
	append_fifo_store(desc, dst_addr, 16, store_ops);


	//because in the sys set before,the sec reg region tlb is set to little endian,so not changle endian
	#if 0
	length = desc_len(desc);
	for (i = 0; i < length; i++) {
		desc[i] = cpu_to_le32(desc[i]);
	}
	#endif
	*(uint32_t *)DECO_DxJQCRH = DECO_DxJQCRH_SINGLE|DECO_DxJQCRH_STEP;
	for(i=0;i<cmdcnt;i++)
	{
		view_cmd1 = *(uint32_t *)DECO_DxJQCRL;
	}
	while(!(*(uint32_t *)DECO_DxJQCRH & DECO_DxJQCRH_DONE));
	*(uint32_t *)DECO_DxJQCRH = DECO_DxJQCRH_DONE;
	while(1);
}

struct presult{
	int index;
	void *desc;
	unsigned char *ptr;
};

pl_desc_done(uint32_t status, struct presult *result)
{
	unsigned char *regval,*regval_raw;
	int i,index;
	regval_raw = result->ptr;
	index = result->index;
	regval = (uint32_t *)(((unsigned long)regval_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	for(i=0;i<8;i++)
	{
		if(*regval == 2*index)
		{

		}else
		{
			debug("virt desc raw %x,val raw %x,index %x del ERROR!\n",result->desc,result->ptr,index);
			while(1);
		}
	}
	debug("virt desc raw %x,val raw %x,index %x del OK!\n",result->desc,result->ptr,index);
	free(result->desc);
	free(result->ptr);
}

void math_add_cmd_test()
{
	int i,j;
	struct result *op;
	uint32_t *desc,*desc_raw;

	unsigned char *regval,*regval_raw;
	dma_addr_t regval_addr;
	unsigned int reg_size=8;
	unsigned char ret[8];

	desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
	desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	regval_raw = (uint32_t *)malloc(16+DATA_ALIGN);
	regval = (uint32_t *)(((unsigned long)regval_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));

	regval_addr = virt_to_phys(regval);

	memset(regval, 0x11, 8);
	memset(ret, 0x22, 8);

	init_job_desc(desc, START_INDEX);

	append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH0 |
					   LDST_CLASS_DECO );

	append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH1 |
							   LDST_CLASS_DECO );
/*
	append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH1 |
							   LDST_CLASS_DECO );
*/
	append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	append_math_add(desc, REG3, REG2, REG0, sizeof(unsigned long long));

	//

	append_cmd_ptr(desc, regval_addr,reg_size,CMD_STORE | LDST_SRCDST_WORD_DECO_MATH3 |
							   LDST_CLASS_DECO );

	run_descriptor_jr(desc,&jr_t);

	if(memcmp_c(regval,ret,8))
	{
		debug("math add error");
		while(1);
	}

	free(desc_raw);
	free(regval_raw);
	//COND_TRAP(31,0,0);
}

void jbring_pipeline_test()
{
	int i,j;
	struct result *op;
	uint32_t *desc,*desc_raw;
	struct presult *result;

	unsigned char *regval,*regval_raw;
	dma_addr_t regval_addr;
	unsigned int reg_size=8;

	int t_cnt=10;

	for(i=1;i<=t_cnt;i++){								//jb ring 0

		result = (uint32_t *)malloc(sizeof(struct presult));

		desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
		desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
		regval_raw = (uint32_t *)malloc(16+DATA_ALIGN);
		regval = (uint32_t *)(((unsigned long)regval_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));

		regval_addr = virt_to_phys(regval);

		memset(regval, i, 8);
		memset(result, 0, sizeof(struct presult));

		init_job_desc(desc, START_INDEX);

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH0 |
					   LDST_CLASS_DECO );

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH1 |
							   LDST_CLASS_DECO );

		append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));

		append_math_add(desc, REG3, REG2, REG0, sizeof(unsigned long long));

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_STORE | LDST_SRCDST_WORD_DECO_MATH3 |
							   LDST_CLASS_DECO );

		result->desc = desc_raw;
		result->ptr = regval_raw;
		result->index = i;

		while(jr_enqueue(desc, pl_desc_done, result, 0,&jr_g[0])!=0)	//no valid slot
		{
				jr_dequeue(0,&jr_g[0]);
		}

	}
	for(i=1;i<=t_cnt;i++){		//jb ring 1

		result = (uint32_t *)malloc(sizeof(struct presult));

		desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
		desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
		regval_raw = (uint32_t *)malloc(16+DATA_ALIGN);
		regval = (uint32_t *)(((unsigned long)regval_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));

		regval_addr = virt_to_phys(regval);

		memset(regval, i, 8);
		memset(result, 0, sizeof(struct presult));

		init_job_desc(desc, START_INDEX);

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH0 |
					   LDST_CLASS_DECO );

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH1 |
							   LDST_CLASS_DECO );

		append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));

		append_math_add(desc, REG3, REG2, REG0, sizeof(unsigned long long));

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_STORE | LDST_SRCDST_WORD_DECO_MATH3 |
							   LDST_CLASS_DECO );

		result->desc = desc_raw;
		result->ptr = regval_raw;
		result->index = i;

		while(jr_enqueue(desc, pl_desc_done, result, 0,&jr_g[1])!=0)	//no valid slot
		{
				jr_dequeue(0,&jr_g[1]);
		}

	}
#if 0
	for(i=1;i<=t_cnt;i++){							//jb ring 2

		result = (uint32_t *)malloc(sizeof(struct presult));

		desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
		desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
		regval_raw = (uint32_t *)malloc(16+DATA_ALIGN);
		regval = (uint32_t *)(((unsigned long)regval_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));

		regval_addr = virt_to_phys(regval);

		memset(regval, i, 8);
		memset(result, 0, sizeof(struct presult));

		init_job_desc(desc, START_INDEX);

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH0 |
					   LDST_CLASS_DECO );

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH1 |
							   LDST_CLASS_DECO );

		append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));

		append_math_add(desc, REG3, REG2, REG0, sizeof(unsigned long long));

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_STORE | LDST_SRCDST_WORD_DECO_MATH3 |
							   LDST_CLASS_DECO );

		result->desc = desc_raw;
		result->ptr = regval_raw;
		result->index = i;

		while(jr_enqueue(desc, pl_desc_done, result, 0,&jr_g[2])!=0)	//no valid slot
		{
				jr_dequeue(0,&jr_g[2]);
		}

	}
	for(i=1;i<=t_cnt;i++){						//jb ring 3

		result = (uint32_t *)malloc(sizeof(struct presult));

		desc_raw = (uint32_t *)malloc(64*4+DESC_ALIGN);
		desc = (uint32_t *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
		regval_raw = (uint32_t *)malloc(16+DATA_ALIGN);
		regval = (uint32_t *)(((unsigned long)regval_raw+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));

		regval_addr = virt_to_phys(regval);

		memset(regval, i, 8);
		memset(result, 0, sizeof(struct presult));

		init_job_desc(desc, START_INDEX);

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH0 |
					   LDST_CLASS_DECO );

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_LOAD | LDST_SRCDST_WORD_DECO_MATH1 |
							   LDST_CLASS_DECO );

		append_move(desc,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));

		append_math_add(desc, REG3, REG2, REG0, sizeof(unsigned long long));

		append_cmd_ptr(desc, regval_addr,reg_size,CMD_STORE | LDST_SRCDST_WORD_DECO_MATH3 |
							   LDST_CLASS_DECO );

		result->desc = desc_raw;
		result->ptr = regval_raw;
		result->index = i;

		while(jr_enqueue(desc, pl_desc_done, result, 0,&jr_g[3])!=0)	//no valid slot
		{
				jr_dequeue(0,&jr_g[3]);
		}

	}
#endif

	jr_dequeue(0,&jr_g[0]);
	jr_dequeue(0,&jr_g[1]);

}

