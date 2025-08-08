/*
 * cipher.h
 *
 *  Created on: Mar 23, 2017
 *      Author: jzj
 */

#ifndef CIPHER_H_
#define CIPHER_H_

#include "jr.h"

#define CIPHER_MAX_IV_SIZE	128
#define CIPHER_MAX_KEY_SIZE 64

enum cipher_algos
{
	AES = 0x10,
	DES = 0x20,
	DES3 = 0x21,
	ARC4 = 0x30,
	RNG = 0x50,
	SNOW_3G_f8 = 0x60,
	Kasumi = 0x70,
	SM1 = 0x80,
	SM4 = 0x90,
	SSF33 = 0xA0,
	SM6 = 0xB0
};	//need to compate with csec define

enum encdec{
	dec = 0,
	enc
};	//need to compate with csec define

enum icvonoff{
	icvon = 0,
	icvoff
};

enum atype{
	CBC = 0x10,
	ECB = 0x20,
	CFB = 0x30,
	CFB1 = 0x31,
	CFB8 = 0x32,
	CFB16 = 0x33,
	CFB32 = 0x34,
	CFB64 = 0x35,
	OFB = 0x40,
	OFB1 = 0x41,
	OFB8 = 0x42,
	OFB16 = 0x43,
	OFB32 = 0x44,
	OFB64 = 0x45,
	CTR = 0x00,
	CCM = 0x80,
};	//need to compate with csec define

#define AAI_SK_SL	(0x800)

struct cipher_ctx {
	uint32_t cipher_desc[64];
	struct sg_entry sg_tbl[MAX_SG_32];
	u8 iv[CIPHER_MAX_IV_SIZE];
	u8 key[CIPHER_MAX_KEY_SIZE];
	u32 aad_size;
	u32 ml_len;
	u32 message_len;
	u32 auth_len;

	uint32_t sg_num;
	uint32_t len;
	u32 iv_size;
	u32 key_size;

	u8 algo;
	u16 algo_type;
	u8 dir;
	void *ext;
	void *malloc_ptr;
};

#endif /* CIPHER_H_ */
