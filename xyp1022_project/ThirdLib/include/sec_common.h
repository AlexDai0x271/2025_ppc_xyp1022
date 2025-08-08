/*
 * common.h
 *
 *  Created on: Nov 20, 2012
 *      Author: jzj
 */

#ifndef SEC_COMMON_H_
#define SEC_COMMON_H_

#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8	unsigned char
#define s32 int
#define s16 short
#define s8 	char

#define U64 unsigned long long
#define U32 unsigned int
#define U16 unsigned short
//#define U8	unsigned char
#define S32 int
#define S16 short
//#define S8 	char

#define __u32 u32

#define int64_t long long

#define uint32_t u32
#define uint	u32
#define uint8_t u8

#ifndef NULL
	#define NULL 0
#endif

#define debug PSPRINTF

extern void writel(u32 addr,u32 value);
extern u32 readl(u32 addr);
extern void write16(volatile u32 addr,u16 value);
extern volatile u16 read16(volatile u32 addr);

extern void delay(u32 val);

#define setbits32(_addr, _v) writel(_addr, readl(_addr) |  (_v))
#define clrbits32(_addr, _v) writel(_addr, readl(_addr) & ~(_v))

#define DESC_ALIGN	4		//byte
#define DATA_ALIGN	4	//byte	zjjin 18-3-30
//#define DATA_ALIGN	16	//byte

#ifdef MEM_BIGENDIA
#define cpu_to_le32(v32) ( ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8))
#define le32_to_cpu(v32) ( ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8))
#else
#define cpu_to_le32(v32) (v32)
#define le32_to_cpu(v32) (v32)
#endif

#define b2l l2b

#define SG_LE
//#define ALG_BIG
//#define OR4W
#define INITFINAL_OK
#define RAND_CNT 1
//#define RANDSIZE_MASK (0x11FF0)
#define RANDSIZE_MASK (0x3FF0)
//#define RANDSIZE_MASK (0x7F0)		//for bug 4-20


#define CONST_ST const

#endif /* COMMON_H_ */
