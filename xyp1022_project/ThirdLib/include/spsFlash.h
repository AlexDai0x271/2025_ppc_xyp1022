/*
*****************************************************************************
* Copyright (c) 2012 CCore, Inc.  All rights reserved.
* File	: 	spsFlash.h
* Author: 	ccore_sys
* Date	: 	
*****************************************************************************
* Description:
* S29GL01GP, S29GL512P, S29GL256P, S29GL128P norflash flash(Spansion) driver
*****************************************************************************
* Changes log:
* Date:		Comment:
*
*
*****************************************************************************
*/

#ifndef __SPANSION_FLASH_H__
#define __SPANSION_FLASH_H__

#include "common.h"
#include "ebc.h"
//----------------------------------------------------------------------------------

///SPANSION
#define  SPANSION_FLASH_BASE       (BANK0_ROM_ADDR)
#define  SPANSION_FLASH_ADDR(xxx)  (SPANSION_FLASH_BASE + xxx)

#define spansion_1st_addr 		(SPANSION_FLASH_BASE + (0x555<<1))
#define spansion_2nd_addr 		(SPANSION_FLASH_BASE + (0x2AA<<1))
#define spansion_3rd_addr 		(SPANSION_FLASH_BASE + (0x555<<1))
#define spansion_4th_addr 		(SPANSION_FLASH_BASE + (0x555<<1))
#define spansion_5th_addr 		(SPANSION_FLASH_BASE + (0x2AA<<1))
#define spansion_6th_addr 		(SPANSION_FLASH_BASE + (0x555<<1))

//#define spansion_1st_addr (0x555)
//#define spansion_2nd_addr (0x2AA)
//#define spansion_3rd_addr (0x555)
//#define spansion_4th_addr (0x555)
//#define spansion_5th_addr (0x2AA)
//#define spansion_6th_addr (0x555)

#define BANK0_ADDR 	BANK0_ROM_ADDR

#define  SPANSION_SECTOR_NUM    (31)    //spansion
#define  SPANSION_SECTOR_SIZE   (65536)  //spansion x16mode: 64 Kword/128 Kb

//----------------------------------------------------------------------------------
extern unsigned int spansionReadID(unsigned int addr);
extern void spansionChipReset();
extern void spansionChipErase();
extern void spansionSectorErase(unsigned int  addr);
extern void spansionProgram(unsigned int  addr,unsigned int data);
extern void spansionWaitFlash();
//----------------------------------------------------------------------------------

#endif
