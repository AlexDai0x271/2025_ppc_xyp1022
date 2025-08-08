/*******************************************************
 * File: sram.c
 * Date: 2013/12/26
 * Author: ccore@ccore_sys
 *
 *This code for sram test
 ******************************************************/
//#include "sram.h"

#include "iomacros.h"

#define SCAN_SRAM_START	0xe0070000
#define SCAN_SRAM_LEN	0x10000

void LED_RED_SHARK4(void)
{
	while(1);
}

byte_wr_test()
{
	unsigned int i;
	
	unsigned int addr = SCAN_SRAM_START;
	unsigned int len = SCAN_SRAM_LEN;
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		*(unsigned char*)(i) = 0x55;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		if( *(unsigned char*)(i) != 0x55)
		{
			LED_RED_SHARK4();
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		*(unsigned char*)(i) = 0xaa;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		if( *(unsigned char*)(i) != 0xaa)
		{
			LED_RED_SHARK4();			
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		*(unsigned char*)(i) = 0x00;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		if( *(unsigned char*)(i) != 0x00)
		{
			LED_RED_SHARK4();			
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		*(unsigned char*)(i) = 0xFF;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		if( *(unsigned char*)(i) != 0xFF)
		{
			LED_RED_SHARK4();
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		*(unsigned char*)(i) = (unsigned char)(i&0xFF);
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i++)
	{
		if( *(unsigned char*)(i) != (unsigned char)(i&0xFF))
		{
			LED_RED_SHARK4();		
		}

	}

	
}

short_wr_test()
{
	unsigned int i;
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		*(unsigned short*)(i) = 0x55aa;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		if( *(unsigned short*)(i) != 0x55aa)
		{
			LED_RED_SHARK4();			
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		*(unsigned short*)(i) = 0xaa55;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		if( *(unsigned short*)(i) != 0xaa55)
		{
			LED_RED_SHARK4();
		}
	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		*(unsigned short*)(i) = 0x0000;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		if( *(unsigned short*)(i) != 0x0000)
		{
			LED_RED_SHARK4();			
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		*(unsigned short*)(i) = 0xFFFF;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		if( *(unsigned short*)(i) != 0xFFFF)
		{
			LED_RED_SHARK4();			
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		*(unsigned short*)(i) = (unsigned short)(i&0xFFFF);
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=2)
	{
		if( *(unsigned short*)(i) != (unsigned short)(i&0xFFFF))
		{
			LED_RED_SHARK4();
		}

	}
}

long_wr_test()
{
	unsigned int i;
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		*(unsigned int*)(i) = 0x55aa55aa;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		if( *(unsigned int*)(i) != 0x55aa55aa)
		{
			LED_RED_SHARK4();
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		*(unsigned int*)(i) = 0xaa55aa55;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		if( *(unsigned int*)(i) != 0xaa55aa55)
		{
			LED_RED_SHARK4();
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		*(unsigned int*)(i) = 0x00000000;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		if( *(unsigned int*)(i) != 0x00000000)
		{
			LED_RED_SHARK4();			
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		*(unsigned int*)(i) = 0xFFFFFFFF;
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		if( *(unsigned int*)(i) != 0xFFFFFFFF)
		{
			LED_RED_SHARK4();	
		}

	}
	
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		*(unsigned int*)(i) = (unsigned short)(i&0xFFFFFFFF);
	}
	for(i=SCAN_SRAM_START;i<SCAN_SRAM_START + SCAN_SRAM_LEN;i+=4)
	{
		if( *(unsigned int*)(i) != (unsigned short)(i&0xFFFFFFFF))
		{
			LED_RED_SHARK4();
		}

	}	

}


/*******************************************************
 * sram test main function.
 *
 ******************************************************/
void sram_test()
{
	int i;

	for(i = 0;i<100;i++)
//	while(1)
	{
		byte_wr_test();
		short_wr_test();
		long_wr_test();
	}


}


