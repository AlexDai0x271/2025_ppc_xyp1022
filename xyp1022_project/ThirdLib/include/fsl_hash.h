/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */

#ifndef _SHA_H
#define _SHA_H

#include <hash.h>
#include "jr.h"

/*
 * Hash context contains the following fields
 * @sha_desc: Sha Descriptor
 * @sg_num: number of entries in sg table
 * @len: total length of buffer
 * @sg_tbl: sg entry table
 * @hash: index to the hash calculated
 */
struct hash_ctx {
	uint32_t hash_desc[64];
	u8 hash[80];
	u8 dsize_buf[16];
	struct sg_entry sg_tbl[MAX_SG_32];
	void *malloc_ptr;
	uint32_t sg_num;
	uint32_t len;
};

struct hash_key_ctx{
	u8 key[HASH_MAX_DIGEST_SIZE*2];
	u8 iv[HASH_MAX_DIGEST_SIZE+16];
	struct hash_ctx *hash_ctx;
	uint32_t key_size;
	uint32_t iv_size;
	uint32_t type;
	uint32_t alg;
	uint32_t digest_size;
	uint32_t reserve;
	void *malloc_ptr;
};

#define CRYPTO_MAX_ALG_NAME	80
#define SHA1_DIGEST_SIZE        20
#define SHA256_DIGEST_SIZE      32
#define SHA512_DIGEST_SIZE      64
#define SM3_DIGEST_SIZE      32
#define SHA0_DIGEST_SIZE      20
#define SHA224_DIGEST_SIZE      32
#define SHA384_DIGEST_SIZE      64
#define MD5_DIGEST_SIZE      16

#define CRC32_DIGEST_SIZE      4

struct caam_hash_template {
	char name[CRYPTO_MAX_ALG_NAME];
	unsigned int digestsize;
	u32 alg_type;
};

#endif
