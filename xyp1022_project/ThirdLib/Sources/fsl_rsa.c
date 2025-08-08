/*
 * (C) Copyright 2014 Freescale Semiconductor, Inc.
 * Author: Ruchika Gupta <ruchika.gupta@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include "common.h"
#include "stdlib.h"
#include "jobdesc.h"
#include "desc.h"
#include "jr.h"
#include "rsa_caam.h"
#include "rsa-mod-exp.h"

int fsl_mod_exp(struct udevice *dev, const uint8_t *sig, uint32_t sig_len,
		struct key_prop *prop, uint8_t *out)
{
	uint32_t keylen;
	struct pk_in_params pkin;
	uint32_t desc[MAX_CAAM_DESCSIZE];
	int ret;

	/* Length in bytes */
	keylen = prop->num_bits / 8;

	pkin.a = sig;
	pkin.a_siz = sig_len;
	pkin.n = prop->modulus;
	pkin.n_siz = keylen;
	pkin.e = prop->public_exponent;
	pkin.e_siz = prop->exp_len;

	inline_cnstr_jobdesc_pkha_rsaexp(desc, &pkin, out, sig_len);

	ret = run_descriptor_jr(desc,&jr_t);
	if (ret) {
		debug("%s: RSA failed to verify: %d\n", __func__, ret);
		return -5;
	}

	return 0;
}
