/*
 * secmon.c
 *
 *  Created on: 2017Äê11ÔÂ20ÈÕ
 *      Author: yyang
 */
#include "secmon.h"
#include "ppclib.h"

#define W32(address, data) 			*(volatile unsigned int *)(address) = (unsigned int)data
#define R32(address) 				*(volatile unsigned int *)(address)
#define W8(address, data) 			*(volatile unsigned char *)(address) = (unsigned char)data
#define R8(address) 				*(volatile unsigned char *)(address)

void secmon_svi_enable(unsigned int value)
{
	W32(SECMON_HPSICR_REG, value);
}

void secmon_sv_enable(unsigned int value)
{
	W32(SECMON_HPSVCR_REG, value);
}

void secmon_svi_state_clear(unsigned int value)
{
	W32(SECMON_HPSVSR_REG, value);
}

void secmon_disable_hrst_req(void)
{
	W32(SECMON_HPMISCR_REG, HPMISR_HRST_REQ_DIS);
}
