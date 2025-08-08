/*
 * T1022RDB_init.h
 * 64-bit mode
 */

#ifndef T1022RDB_INIT_H_
#define T1022RDB_INIT_H_


#define SET_GROUP_BASE(base) asm("lis  	17, %0" 	: : "n" (((base) >> 48) & 0xFFFF));\
							 asm("ori	17, 17, %0" : : "n" (((base) >> 32) & 0xFFFF));\
							 asm("sldi	17, 17, 32");\
							 asm("oris  17, 17, %0" : : "n" (((base) >> 16) & 0xFFFF));\
							 asm("ori	17, 17, %0" : : "n" (((base) >>  0) & 0xFFFF));

#define LD_VAL_W(value)		 asm("lis	19, %0" : : "n"((value) >> 16));\
							 asm("ori	19, 19, %0" : : "n" ((value) & 0xFFFF));

#define CCSR_SET_W(offset, value)  asm ("li   18, %0" : : "n" ((offset)));\
								   LD_VAL_W((value));\
								   asm ("stwx 19, 18, 17");

#define CCSR_SET_DUP_W(offset)     asm ("li   18, %0" : : "n" ((offset)));\
								   asm ("stwx 19, 18, 17");

#define CCSR_GET_W(offset)		   asm ("li   18, %0" : : "n" ((offset)));\
								   asm ("lwzx 19, 18, 17");



#endif /* T1022RDB_INIT_H_ */
