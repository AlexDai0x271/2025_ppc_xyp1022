
#ifndef CPC_H
#define CPC_H

#define L3_SRAM					0

#define CPC_BASE 				0xffffffffE800F000
	
#define CPCCSR0 				(CPC_BASE+0x0)
#define CPCSRCR1				(CPC_BASE+0x100)
#define CPCSRCR0				(CPC_BASE+0x104)
#define CPCPAR0 				(CPC_BASE+0x208)
#define CPCPAR1					(CPC_BASE+0x218)
#define CPCPAR2					(CPC_BASE+0x228)
#define CPCPAR3					(CPC_BASE+0x238)
#define CPCPAR4					(CPC_BASE+0x248)
#define CPCPAR5					(CPC_BASE+0x258)
#define CPCPAR6					(CPC_BASE+0x268)
#define CPCPAR7					(CPC_BASE+0x278)
#define CPCPAR8					(CPC_BASE+0x288)
#define CPCPAR9					(CPC_BASE+0x298)
#define CPCPAR10				(CPC_BASE+0x2a8)
#define CPCPAR11				(CPC_BASE+0x2b8)
#define CPCPAR12				(CPC_BASE+0x2c8)
#define CPCPAR13				(CPC_BASE+0x2d8)
#define CPCPAR14				(CPC_BASE+0x2e8)
#define CPCPAR15				(CPC_BASE+0x2f8)


#define BIT_CPCLFC    			10
#define BIT_CPCFL				11
#define BIT_CPCFI   			21
#define BIT_CPCPE				30
#define BIT_CPCE      			31

#define BIT_CPCSRCR0_SRBARL		15
#define BIT_CPCSRCR0_INTLVEN	22
#define BIT_CPCSRCR0_SRAMSZ		1
#define BIT_CPCSRCR0_SRAMEN		0
#endif

