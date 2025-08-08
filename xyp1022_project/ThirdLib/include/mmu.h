/*
 * mmu.h
 *
 *  Created on: 2020-10-21
 *      Author: ccore
 */

#ifndef MMU_H_
#define MMU_H_

#include "e500mc.h"

/* ------- MMU SPRs ------- */
#define  MAS0		624
#define  MAS1		625
#define  MAS2		626
#define  MAS3		627
#define  MAS4		628
#define  MAS5		339
#define  MAS6		630
#define  MAS7		944
#define	 MAS8		341

#define  LPIDR		338
//#define  PID		48
#define  MMUCSR0	1012
#define  MMUCFG		1015
#define  TLB0CFG	688
#define  TLB1CFG	689
//#define  DEAR		61
#define  GDEAR		381



/* other SPRs */
#define  HID0	1008

#endif /* MMU_H_ */
