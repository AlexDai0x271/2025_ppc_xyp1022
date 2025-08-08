/*
 * drng.h
 *
 *  Created on: 2019Äê9ÔÂ21ÈÕ
 *      Author: sytu
 */

#include "jr.h"

#ifndef DRNG_H_
#define DRNG_H_

/********* Digital RNG Configuration & Status Registers *********/
#define	DRNG_SEC_EN_REG			SEC_ADDR(0) + 0xc

#define DRNG_BASE_ADDR			SEC_ADDR(0) + 0xa00

#define RNG_CTRL_REG			(0x00 + DRNG_BASE_ADDR)
#define RNG_STAT_REG			(0x04 + DRNG_BASE_ADDR)
#define RNG_MODE_REG			(0x08 + DRNG_BASE_ADDR)
#define RNG_SMODE_REG			(0x0c + DRNG_BASE_ADDR)
#define RNG_IE_REG				(0x10 + DRNG_BASE_ADDR)
#define RNG_ISTAT_REG			(0x14 + DRNG_BASE_ADDR)
#define RNG_FEATURES_REG		(0x1c + DRNG_BASE_ADDR)
#define RNG_RAND0_REG			(0x20 + DRNG_BASE_ADDR)
#define RNG_RAND1_REG			(0x24 + DRNG_BASE_ADDR)
#define RNG_RAND2_REG			(0x28 + DRNG_BASE_ADDR)
#define RNG_RAND3_REG			(0x2c + DRNG_BASE_ADDR)
#define RNG_RAND4_REG			(0x30 + DRNG_BASE_ADDR)
#define RNG_RAND5_REG			(0x34 + DRNG_BASE_ADDR)
#define RNG_RAND6_REG			(0x38 + DRNG_BASE_ADDR)
#define RNG_RAND7_REG			(0x3c + DRNG_BASE_ADDR)
#define RNG_SEED0_REG			(0x40 + DRNG_BASE_ADDR)
#define RNG_SEED1_REG			(0x44 + DRNG_BASE_ADDR)
#define RNG_SEED2_REG			(0x48 + DRNG_BASE_ADDR)
#define RNG_SEED3_REG			(0x4c + DRNG_BASE_ADDR)
#define RNG_SEED4_REG			(0x50 + DRNG_BASE_ADDR)
#define RNG_SEED5_REG			(0x54 + DRNG_BASE_ADDR)
#define RNG_SEED6_REG			(0x58 + DRNG_BASE_ADDR)
#define RNG_SEED7_REG			(0x5c + DRNG_BASE_ADDR)
#define RNG_AUTO_RQSTS_REG		(0x60 + DRNG_BASE_ADDR)
#define RNG_AUTO_AGE_REG		(0x64 + DRNG_BASE_ADDR)


#endif /* DRNG_H_ */
