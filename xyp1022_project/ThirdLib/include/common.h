/*
 * common.h
 *
 *  Created on: Nov 20, 2012
 *      Author: jzj
 */

#ifndef COMMON_H_
#define COMMON_H_

//| 0| 1| 2| 3| 4| 5| 6| 7|
//| 8| 9|10|11|12|13|14|15|
//|16|17|18|19|20|21|22|23|
//|24|25|26|27|28|29|30|31|

#define UINT64  unsigned long long 
#define UINT32  unsigned int
#define UINT16  unsigned short
#define UINT8	unsigned char
#define INT32   int
#define INT16   short
#define INT8 	char

typedef volatile unsigned       char  V_UINT8;
typedef volatile signed         char  V_INT8;
typedef volatile unsigned short int   V_UINT16;
typedef volatile signed   short int   V_INT16;
typedef volatile unsigned       int   V_UINT32;
typedef volatile signed         int   V_INT32;
typedef volatile float                V_FP32;

extern void writel(UINT32 addr,UINT32 value);
extern UINT32 readl(UINT32 addr);

#define IO_READ8(p)      (UINT8)(*(volatile UINT8 *)(p))
#define IO_WRITE8(p,c)   asm volatile("stbx %0,0,%1"::"r"(c),"r"(p))
//#define IO_WRITE8(p,c)   (*((volatile UINT8 *)(p)) = ((UINT8)(c)))

#define IO_READ16(p)     (UINT16)(*(volatile UINT16 *)(p))
#define IO_WRITE16(p,v)  asm volatile("sthx %0,0,%1"::"r"(v),"r"(p))
//#define IO_WRITE16(p,v)  (*((volatile UINT16 *)(p)) = (UINT16)(v))

#define IO_READ16_SPI(p) (UINT16)(((*(volatile UINT8 *)(p+1))<<8) | (*(volatile UINT8 *)(p)))
#define IO_WRITE16_SPI(p,v)  asm volatile("sthx %0,0,%1"::"r"((v<<8|v>>8)),"r"(p))
//#define IO_WRITE16_SPI(p,v)  (*((volatile UINT16 *)(p)) = (UINT16)(v<<8|v>>8))

#define IO_READ32(p)     (UINT32)(*(volatile UINT32 *)(p))
#define IO_WRITE32(p,v)  asm volatile("stwx %0,0,%1"::"r"(v),"r"(p))
//#define IO_WRITE32(p,v)  (*((volatile UINT32 *)(p)) = (UINT32)(v))
 
#define W64(address, data) 			*(volatile unsigned long long *)(address) = (data)
#define R64(address) 				*(volatile unsigned long long *)(address)
#define W32(address, data) 			*(volatile unsigned int *)(address) = (data)
#define R32(address) 				*(volatile unsigned int *)(address)
#define w32(address, data) 			*(volatile unsigned int *)(address) = (data)
#define r32(address) 				*(volatile unsigned int *)(address)
//#define W8(address, data) 			*(volatile unsigned char *)(address) = (data)
#define W8(address, data) 			(*((volatile unsigned char *)(address)) = data )
#define R8(address) 				*(volatile unsigned char *)(address)
#define W16(address, data) 			*(volatile unsigned short *)(address) = (data)
#define R16(address) 				*(volatile unsigned short *)(address)


#define U64 unsigned long long
#define U32 unsigned int
#define U16 unsigned short
#define U8	unsigned char
#define S32 int
#define S16 short
#define S8 char

#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned 
#define u8 unsigned char
#define s64 long long
#define s32 int
#define s16 short
#define s8 char


/************************** Constant Definitions *****************************/

#ifndef TRUE
#  define TRUE		1
#endif

#ifndef FALSE
#  define FALSE		0
#endif

#ifndef NULL
#define NULL		0
#endif

/***************** Macros (In-line Functions) Definitions *********************/

#if 0 // @@
#define UINT64_MSW(x) ((x).Upper)

#define UINT64_LSW(x) ((x).Lower)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))


#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

extern unsigned char _ctype[];
extern char _ctmp;

#define isalnum(c) ((_ctype+1)[c]&(_U|_L|_D))
#define isalpha(c) ((_ctype+1)[c]&(_U|_L))
#define iscntrl(c) ((_ctype+1)[c]&(_C))
#define isdigit(c) ((_ctype+1)[c]&(_D))
#define isgraph(c) ((_ctype+1)[c]&(_P|_U|_L|_D))
#define islower(c) ((_ctype+1)[c]&(_L))
#define isprint(c) ((_ctype+1)[c]&(_P|_U|_L|_D|_SP))
#define ispunct(c) ((_ctype+1)[c]&(_P))
#define isspace(c) ((_ctype+1)[c]&(_S))
#define isupper(c) ((_ctype+1)[c]&(_U))
#define isxdigit(c) ((_ctype+1)[c]&(_D|_X))

#define isascii(c) (((unsigned) c)<=0x7f)
#define toascii(c) (((unsigned) c)&0x7f)

#define tolower(c) (_ctmp=c,isupper(_ctmp)?_ctmp-('A'-'a'):_ctmp)
#define toupper(c) (_ctmp=c,islower(_ctmp)?_ctmp-('a'-'A'):_ctmp)
#endif // @@

extern void udelay( volatile unsigned int us );
#define delay(us) udelay(us)

extern void xprintf(const char *fmt, ...);

#define uart_printf_buf(...) //xprintf(__VA_ARGS__)
#define uart_printf(...) //xprintf(__VA_ARGS__)

//#define PSPRINTF 
#define PSPRINTF uart_printf
//#define PSPRINTF uart_printf_buf

#endif /* COMMON_H_ */
