/*
 * common_usb.h
 *
 *  Created on: 2023-7-24
 *      Author: DELL
 */

#ifndef COMMON_USB_H_
#define COMMON_USB_H_

//--------------------------------------------------------------------------------------
#define	CHIP_S1020		1
#define CHIP_STRING   		"CHIP_S1020"

#define USB_CTR0  1
#define USB_CTR1  0
//--------------------------------------------------------------------------------------
#define true 	1
#define false 	0

//| 0| 1| 2| 3| 4| 5| 6| 7|
//| 8| 9|10|11|12|13|14|15|
//|16|17|18|19|20|21|22|23|
//|24|25|26|27|28|29|30|31|

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
#define U8	unsigned char
#define S32 int
#define S16 short
#define S8 	char

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


#define  uint8_t	unsigned char  ;
#define  uint16_t	unsigned short ;
#define  uint32_t	unsigned int   ;
#define  int8_t		signed char	;
#define  int16_t	signed short;
#define  int32_t	signed int  ;


#define  bool	unsigned char		;

#if 0
#define PSPRINTF
#else
#define PSPRINTF uart_printf
#endif

#define debug PSPRINTF
//--------------------------------------------------------------------------------------
extern void delay(UINT32 val);

//extern void writel( u32 addr,u32 value);
//extern u32 readl( u32 addr);
//extern void write16( u32 addr,u16 value);
//extern u16 	read16( u32 addr);
//extern void write8( u32 addr,u8 value);
//extern u8 	read8( u32 addr);

//#define  	writel(addr,val)   ( *(volatile unsigned int *)(addr) =  val)
#define  	write16(addr,val)  ( *(volatile unsigned short *)(addr) =  val)
#define  	write8(addr,val)   ( *(volatile unsigned char *)(addr) =  val)

//#define  	readl(addr)   ( *(volatile unsigned int *)(addr) )
//#define  	read16(addr)  ( *(volatile unsigned short *)(addr) )
#define  	read8(addr)   ( *(volatile unsigned char *)(addr) )


#define setbits32(_addr, _v) writel(_addr, readl(_addr) |  (_v))
#define clrbits32(_addr, _v) writel(_addr, readl(_addr) & ~(_v))

//1 bit
#define SET_ONEBIT(data,bitnum) (data | (1<<bitnum) )
//2 bits
#define SET_TWOBIT(data,bitnum0,bitnum1) (data | (1<<bitnum0) | (1<<bitnum1) )
//3 bits
#define SET_THRBIT(data,bitnum0,bitnum1,bitnum2) (data | (1<<bitnum0) | (1<<bitnum1) | (1<<bitnum2) )
//4 bits
#define SET_FORBIT(data,bitnum0,bitnum1,bitnum2,bitnum3) (data | (1<<bitnum0) | (1<<bitnum1) | (1<<bitnum2) | (1<<bitnum3) )



#define W32(address, data) 			(*(volatile unsigned int *)(address) = (unsigned int)data )
#define R32(address) 				(*(volatile unsigned int *)(address) )
#define W16(address, data) 			(*(volatile unsigned short *)(address) = (unsigned short)data )
#define R16(address) 				(*(volatile unsigned short *)(address) )
#define W8(address, data) 			(*(volatile unsigned char *)(address) = (unsigned char)data )
#define R8(address) 				(*(volatile unsigned char *)(address) )

#if 1//def MEM_BIGENDIA
#define cpu_to_le32(v32)	 ( ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8) ) //
#define le32_to_cpu(v32) 	 ( ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8) )

#define cpu_to_le16(v16)	 ( ((v16 &0xff)<<8)| ((v16 &0xff00)>>8) ) //
#define le16_to_cpu(v16) 	 ( ((v16 &0xff)<<8)| ((v16 &0xff00)>>8) )


#else
#define cpu_to_le32(v32) (v32)
#define le32_to_cpu(v32) (v32)
#endif

//#define be_to_le(v32)  	(  ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8)    )
//#define le_to_be(v32)  	(  ((v32&0xff)<<24)| ((v32&0xff000000)>>24)|((v32&0xff00)<<8) | ((v32 & 0xff0000) >>8)    )

#define min(x,y) 	((x) < (y) ? x : y)
#define max(x, y) 	(((int)(x) > (int)(y)) ? x : y)

void __memcpy(UINT8 *dst, UINT8 *src, UINT32 len);

//--------------------------------------------------------------------------------------
#endif /* COMMON_H_ */
