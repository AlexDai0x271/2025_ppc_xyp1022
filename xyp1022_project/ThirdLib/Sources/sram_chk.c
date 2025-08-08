
#include "uart.h"
#include "common.h"
#include "e500mc_asm.h"

#define NEED_DELAY_10CLK	//ISYNC//NOP

extern invalidate_dcache_range(unsigned int start, unsigned int end);
extern flush_dcache_range(unsigned int start, unsigned int end);

unsigned char sram_chk(unsigned int memAddr, unsigned int memSize)
{
	unsigned int addr;
	unsigned int data;
	int i = 0;
	char chkFlg = 0;
	
	uart_printf("Memory check: Start address %x;length %x \n", memAddr, memSize);
	for(addr = memAddr;addr<(memAddr+memSize);addr=addr+4)
	{
	    W32(addr,0x00000000);
	}
	
	for(addr = memAddr;addr<(memAddr+memSize);addr=addr+4)
	{
	    if((data=R32(addr))!=0x00000000)
	    {
	    	PSPRINTF("Memory check error1: 0x%x must be 0x00000000,but be 0x%x \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok1: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0xFFFFFFFF);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0xFFFFFFFF)
	    {
	    	PSPRINTF("Memory check error1: 0x%x must be 0xFFFFFFFF,but be 0x%x \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok1: %08X must be 0xFFFFFFFF, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	for(addr = memAddr;addr<(memAddr+memSize);addr=addr+4)
	{
	    if((data=R32(addr))!=0xFFFFFFFF)
	    {
	    	PSPRINTF("Memory check error2: 0x%x must be 0xFFFFFFFF,but be 0x%x \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok2: %08X must be 0xFFFFFFFF, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0x00000000);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0x00000000)
	    {
	    	PSPRINTF("Memory check error2: 0x%x must be 0x00000000,but be 0x%x \n", addr, data);
	    }else{
	    	//printf("Memory check ok2: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
	    if((data=R32(addr))!=0x00000000)
	    {
	    	PSPRINTF("Memory check error3: 0x%x must be 0x00000000,but be 0x%x \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok3: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0x5a5a5a5a);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0x5a5a5a5a)
	    {
	    	PSPRINTF("Memory check error3: 0x%x must be 0x5a5a5a5a,but be 0x%x \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok3: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
	    if((data=R32(addr))!=0x5a5a5a5a)
	    {
	    	PSPRINTF("Memory check error4: 0x%x must be 0x5a5a5a5a,but be 0x%x \n", addr, data);
	       	while(1);
	    }else{
	    	//printf("Memory check ok4: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0xa5a5a5a5);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0xa5a5a5a5)
	    {
	    	PSPRINTF("Memory check error4: 0x%x must be 0xa5a5a5a5,but be 0x%x \n", addr, data);
	      	while(1);
	    }else{
	    	//printf("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}

	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
		W32(addr,addr);
		NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
		if((data=R32(addr))!=addr)
		{
			PSPRINTF("Memory check error4: %x must be 0x%x ,but be 0x%x \n", addr, addr, data);
		   	while(1);
		}else{
			   	//printf("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
		}
	}
	return 1;
}

unsigned char sram_chk_byte(unsigned int memAddr, unsigned int memSize)
{
	int addr;
	unsigned int data;
	int i = 0;
	char chkFlg = 0;
	
	PSPRINTF("Memory check: Start address %x;length %x \n", memAddr, memSize);
	for(addr = memAddr;addr<(memAddr+memSize);addr++)
	{
		W8(addr,0x00);
	}
	for(addr = memAddr;addr<(memAddr+memSize);addr++)
	{
	    if((data=R8(addr))!=0x00)
	    {
	    	PSPRINTF("Memory check error1: %d must be 0x00,but be %08d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok1: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W8(addr,0xFF);
	    NEED_DELAY_10CLK;
	    if((data=R8(addr))!=0xFF)
	    {
	    	PSPRINTF("Memory check error1: %d must be 0xFF,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok1: %08X must be 0xFFFFFFFF, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	for(addr = memAddr;addr<(memAddr+memSize);addr++)
	{
	    if((data=R8(addr))!=0xFF)
	    {
	    	PSPRINTF("Memory check error2: %d must be 0xFF,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok2: %08X must be 0xFFFFFFFF, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W8(addr,0x00);
	    NEED_DELAY_10CLK;
	    if((data=R8(addr))!=0x00)
	    {
	    	PSPRINTF("Memory check error2: %d must be 0x00,but be %d \n", addr, data);
	    }else{
	    	//printf("Memory check ok2: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-1;addr>(memAddr);addr=addr-1)
	{
	    if((data=R8(addr))!=0x00)
	    {
	    	PSPRINTF("Memory check error3: %d must be 0x00,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok3: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W8(addr,0x5a);
	    NEED_DELAY_10CLK;
	    if((data=R8(addr))!=0x5a)
	    {
	    	PSPRINTF("Memory check error3: %d must be 0x5a,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok3: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-1;addr>(memAddr);addr=addr-1)
	{
	    if((data=R8(addr))!=0x5a)
	    {
	    	PSPRINTF("Memory check error4: %d must be 0x5a,but be %d \n", addr, data);
	       	while(1);
	    }else{
	    	//printf("Memory check ok4: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W8(addr,0xa5);
	    NEED_DELAY_10CLK;
	    if((data=R8(addr))!=0xa5)
	    {
	    	PSPRINTF("Memory check error4: %d must be 0xa5,but be %d \n", addr, data);
	      	while(1);
	    }else{
	    	//printf("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}

	addr = (memAddr+memSize);
	for(addr = addr-1;addr>(memAddr);addr=addr-1)
	{
		W8(addr,(addr&0xff));
		NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-1;addr>(memAddr);addr=addr-1)
	{
		if((data=R8(addr))!=(addr&0xff))
		{
			PSPRINTF("Memory check error4: %d must be %d ,but be %d \n", addr, addr, data);
		   	while(1);
		}else{
			   	//printf("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
		}
	}
	PSPRINTF("============================================================================= \n");
	return 1;
}

unsigned char sram_chk_with_code(unsigned int memAddr, unsigned int memSize, unsigned int tmp_mem, unsigned int tmp_size)
{
	int addr;
	unsigned int data;
	int i = 0, cmp_addr = 0;
	char chkFlg = 0;
	for(i=0x2000; i<tmp_size; i+=4)
		W32(tmp_mem+i, R32(i));
	
	for(i=0x2000; i<tmp_size; i+=4){
		if(R32(tmp_mem+i) != R32(i)){
			PSPRINTF("Memory check error1:src:%x, data:%x, dst:%x, data:%x \n", i, R32(i), tmp_mem+i, R32(tmp_mem+i));
			while(1);
		}
	}
	
	PSPRINTF("Memory check: Start address %d;length %d \n", memAddr, memSize);
	for(addr = memAddr;addr<(memAddr+memSize);addr=addr+4)
	{
	    W32(addr,0x00000000);
	}

	for(addr = memAddr;addr<(memAddr+memSize);addr=addr+4)
	{
	    if((data=R32(addr))!=0x00000000)
	    {
	    	PSPRINTF("Memory check error3: %d must be 0x00000000,but be %08d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok1: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0xFFFFFFFF);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0xFFFFFFFF)
	    {
	    	PSPRINTF("Memory check error4: %d must be 0xFFFFFFFF,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok1: %08X must be 0xFFFFFFFF, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}

	for(addr = memAddr;addr<(memAddr+memSize);addr=addr+4)
	{
	    if((data=R32(addr))!=0xFFFFFFFF)
	    {
	    	PSPRINTF("Memory check error6: %d must be 0xFFFFFFFF,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok2: %08X must be 0xFFFFFFFF, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0x00000000);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0x00000000)
	    {
	    	PSPRINTF("Memory check error7: %d must be 0x00000000,but be %d \n", addr, data);
			while(1);
	    }else{
//	    	PSPRINTF("Memory check ok2: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}

	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
	    if((data=R32(addr))!=0x00000000)
	    {
	    	PSPRINTF("Memory check error9: %d must be 0x00000000,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok3: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0x5a5a5a5a);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0x5a5a5a5a)
	    {
	    	PSPRINTF("Memory check error10: %d must be 0x5a5a5a5a,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//PSPRINTF("Memory check ok3: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}

	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
	    if((data=R32(addr))!=0x5a5a5a5a)
	    {
	    	PSPRINTF("Memory check error12: %d must be 0x5a5a5a5a,but be %d \n", addr, data);
	       	while(1);
	    }else{
	    	//printf("Memory check ok4: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0xa5a5a5a5);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0xa5a5a5a5)
	    {
	    	PSPRINTF("Memory check error13: %d must be 0xa5a5a5a5,but be %d \n", addr, data);
	      	while(1);
	    }else{
	    	//PSPRINTF("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}

	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
		W32(addr,addr);
		NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
		if((data=R32(addr))!=addr)
		{
			PSPRINTF("Memory check error15: %d must be %d ,but be %d \n", addr, addr, data);
		   	while(1);
		}else{
//			PSPRINTF("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
		}
	}
	for(i=0x2000; i<tmp_size; i+=4){
		if(R32(tmp_mem+i) != R32(i)){
			PSPRINTF("Memory check error16:src:%x, data:%x, dst:%x, data:%x \n", i, R32(i), tmp_mem+i, R32(tmp_mem+i));
//			while(1);
		}
	}
	PSPRINTF("============================================================================= \n");
	
	cmp_addr = i;
	DCBF(cmp_addr);
	return 1;
}

unsigned char sram_chk_simple(unsigned int memAddr, unsigned int memSize)
{
	int addr;
	unsigned int data;
	int i = 0;
	char chkFlg = 0;
	
	PSPRINTF("Memory check: Start address %x;length %d \n", memAddr, memSize);
	for(addr = memAddr;addr<(memAddr+memSize);addr=addr+4)
	{
	    if((data=R32(addr))!=0xFFFFFFFF)
	    {
	    	PSPRINTF("Memory check error2: %d must be 0xFFFFFFFF,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok2: %08X must be 0xFFFFFFFF, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0x00000000);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0x00000000)
	    {
	    	PSPRINTF("Memory check error2: %d must be 0x00000000,but be %d \n", addr, data);
	    }else{
	    	//printf("Memory check ok2: %08X must be 0x00000000, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	
	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
	    W32(addr,0x5a5a5a5a);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0x5a5a5a5a)
	    {
	    	PSPRINTF("Memory check error3: %d must be 0x5a5a5a5a,but be %d \n", addr, data);
	    	while(1);
	    }else{
	    	//printf("Memory check ok3: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
	    if((data=R32(addr))!=0x5a5a5a5a)
	    {
	    	PSPRINTF("Memory check error4: %d must be 0x5a5a5a5a,but be %d \n", addr, data);
	       	while(1);
	    }else{
	    	//printf("Memory check ok4: %08X must be 0x5a5a5a5a, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	    W32(addr,0xa5a5a5a5);
	    NEED_DELAY_10CLK;
	    if((data=R32(addr))!=0xa5a5a5a5)
	    {
	    	PSPRINTF("Memory check error4: %d must be 0xa5a5a5a5,but be %d \n", addr, data);
	      	while(1);
	    }else{
	    	//printf("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
	    }
	    NEED_DELAY_10CLK;
	}

	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
		W32(addr,addr);
		NEED_DELAY_10CLK;
	}
	addr = (memAddr+memSize);
	for(addr = addr-4;addr>(memAddr);addr=addr-4)
	{
		if((data=R32(addr))!=addr)
		{
			PSPRINTF("Memory check error4: %d must be %d ,but be %d \n", addr, addr, data);
		   	while(1);
		}else{
			   	//printf("Memory check ok4: %08X must be 0xa5a5a5a5, be %08X \n", addr, data);
		}
	}
	PSPRINTF("============================================================================= \n");
	return 1;
}

//#define LOOP_NUM 0x1000
#define LOOP_NUM 0x20
mem_scan_test(unsigned start,unsigned mem_len)
{
	volatile unsigned int *mem_start_addr32;
	volatile unsigned short *mem_start_addr16;
	volatile unsigned char	*mem_start_addr8;
	int i, count;
	for(count=0; count<LOOP_NUM; count++){
	//32bit
		mem_start_addr32 = (volatile unsigned int *)start;
		for(i=0;i<mem_len/4;i++)
		{
			*(mem_start_addr32++) =0x5a5a5a5a;
		}
		flush_dcache_range(start,start+mem_len);
		invalidate_dcache_range(start,start+mem_len);
		mem_start_addr32 = (volatile unsigned int *)start;
		for(i=0;i<mem_len/4;i++)
		{
			if(*(mem_start_addr32++)!=0x5a5a5a5a)
				while(1);
		}
		
		mem_start_addr32 = (volatile unsigned int *)start;
		for(i=0;i<mem_len/4;i++)
		{
			*(mem_start_addr32++) =i;
		}
		flush_dcache_range(start,start+mem_len);
		invalidate_dcache_range(start,start+mem_len);
		mem_start_addr32 = (volatile unsigned int *)start;
		for(i=0;i<mem_len/4;i++)
		{
			if(*(mem_start_addr32++)!=i)
				while(1);
		}
		uart_printf_buf("ddr 32bit test ok. \n");
	//16bit	
		mem_start_addr16 = (volatile unsigned short *)start;
		for(i=0;i<mem_len/2;i++)
		{
			*(mem_start_addr16++) =0xa5a5;
		}
		flush_dcache_range(start,start+mem_len);
		invalidate_dcache_range(start,start+mem_len);
		mem_start_addr16 = (volatile unsigned short *)start;
		for(i=0;i<mem_len/2;i++)
		{
			if(*(mem_start_addr16++)!=0xa5a5)
				while(1);
		}
	
		mem_start_addr16 = (volatile unsigned short *)start;
		for(i=0;i<mem_len/2;i++)
		{
			*(mem_start_addr16++) =(i&0xffff);
		}
		flush_dcache_range(start,start+mem_len);
		invalidate_dcache_range(start,start+mem_len);
		mem_start_addr16 = (volatile unsigned short *)start;
		for(i=0;i<mem_len/2;i++)
		{
			if(*(mem_start_addr16++)!=(i&0xffff))
				while(1);
		}
		uart_printf_buf("ddr 16bit test ok. \n");
	//8bit	
		mem_start_addr8 = (volatile unsigned char *)start;
		for(i=0;i<mem_len;i++)
		{
			*(mem_start_addr8++) =0x55;
		}
		flush_dcache_range(start,start+mem_len);
		invalidate_dcache_range(start,start+mem_len);
		mem_start_addr8 = (volatile unsigned char *)start;
		for(i=0;i<mem_len;i++)
		{
			if(*(mem_start_addr8++)!=0x55)
				while(1);
		}
		mem_start_addr8 = (volatile unsigned char *)start;
		for(i=0;i<mem_len;i++)
		{
			*(mem_start_addr8++) =(i&0xff);
		}
		flush_dcache_range(start,start+mem_len);
		invalidate_dcache_range(start,start+mem_len);
		mem_start_addr8 = (volatile unsigned char *)start;
		for(i=0;i<mem_len;i++)
		{
			if(*(mem_start_addr8++)!=(i&0xff))
				while(1);
		}
		uart_printf_buf("ddr 8bit test ok. \n");
	}	
}
