/*
 * common.c
 *
 *  Created on: Nov 20, 2012
 *      Author: jzj
 */

#include "sec_common.h"

void writel(volatile u32 addr,u32 value)
{
	*( (volatile u32 *)addr) = value;
}

volatile u32 readl(volatile u32 addr)
{
	return *( (volatile u32 *)addr);
}

void write16(volatile u32 addr,u16 value)
{
	*( (volatile u16 *)addr) = value;
}

volatile u16 read16(volatile u32 addr)
{
	return *( (volatile u16 *)addr);
}

void delay(volatile u32 val)
{
	while(val--);
}

int memcmp_c(unsigned char *src,unsigned char *dst,int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		if(*src > *dst)
			return 1;
		else if(*src < *dst)
			return -1;
		else
		{
			src++;
			dst++;
		}
	}
	return 0;
}

void *memcpy_c(unsigned char *dst,unsigned char *src,int size)
{
	int i;
	void *xdst = (void *)dst;
	for(i=0;i<size;i++)
	{
		*dst = *src;
		src ++;
		dst ++;
	}
	return xdst;
}

memset_c2(volatile void *addr,unsigned char val,int len)
{
	int i;
	volatile unsigned char *addr_in = (volatile unsigned char *)addr;
	for(i=0;i<len;i++)
	{
		*addr_in = val;
		addr_in++;
	}
}

memset_c(volatile void *addr,unsigned char val,int len)
{
	int i,j;
	unsigned int tmp;
	j= len%4;
	tmp = val<<24 | val<<16 | val<<8 | val;
	volatile unsigned int *addr_in = (volatile unsigned char *)addr;
	for(i=0;i<len-j;i+=4)
	{
		*addr_in = tmp;
		addr_in++;
	}
	memset_c2((volatile void *)addr_in,val,j);
}

unsigned char *l2b(unsigned char *in,int len)
{
	unsigned char tmp;
	int i;
	for(i=0;i<len;i+=4)
	{
		tmp = *in;
		*in = *(in+3);
		*(in+3) = tmp;
		tmp = *(in+1);
		*(in+1)=*(in+2);
		*(in+2)=tmp;
		in+=4;
	}

}

unsigned int l2b_w(volatile unsigned int *in,int len)
{
	unsigned char tmp;
	unsigned int val;
	val = *in;
	*in = ((val&0xff000000)>>24)|((val&0xff)<<24)|((val&0xff0000)>>8)|((val&0xff00)<<8);
	return *in;
}

unsigned char *byte_turn(unsigned char *in,int len)
{
	unsigned char tmp;
	int i;
	for(i=0;i<(len>>1);i++)
	{
		tmp=in[i];
		in[i]=in[len-1-i];
		in[len-1-i]=tmp;
	}
	return in;
}


