#include "sst_flash.h"
#define disable_interrupt()
#define enable_interrupt()

void sst_wait_flash()
{
	volatile UINT16 *mp;
	UINT16 d1, d2;

	mp=SST_FLASH_BASE;

	//DisableInt();
	d1= (*mp)&0x40;//judge bit6 status
	d2= *mp;
	while( d1 !=(d2&0x40) )
	{
		d1=d2&0x40;
		d2= *mp;
	}
	//EnableInt();
}

void sst_chip_reset()
{
	UINT8 tmp;

	disable_interrupt();

	SST_FLASH_MA(0)=0xF0;

	enable_interrupt();
}

void sst_chip_erase()
{
	disable_interrupt();

#if C0_SMALL_END
	SST_FLASH_MA(g_norflash_cmd0)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd1)=0x5500;
	SST_FLASH_MA(g_norflash_cmd2)=0x8000;
	SST_FLASH_MA(g_norflash_cmd3)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd4)=0x5500;
	SST_FLASH_MA(g_norflash_cmd5)=0x1000;
#else
	SST_FLASH_MA(g_norflash_cmd0)=0xAA;
	SST_FLASH_MA(g_norflash_cmd1)=0x55;
	SST_FLASH_MA(g_norflash_cmd2)=0x80;
	SST_FLASH_MA(g_norflash_cmd3)=0xAA;
	SST_FLASH_MA(g_norflash_cmd4)=0x55;
	SST_FLASH_MA(g_norflash_cmd5)=0x10;
#endif
	delaySysClk(1000);
	enable_interrupt();
	sst_wait_flash();
	delaySysClk(1000);

}

void sst_sector_erase(UINT32 addr)
{
	disable_interrupt();
#if C0_SMALL_END
	SST_FLASH_MA(g_norflash_cmd0)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd1)=0x5500;
	SST_FLASH_MA(g_norflash_cmd2)=0x8000;
	SST_FLASH_MA(g_norflash_cmd3)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd4)=0x5500;
#else
	SST_FLASH_MA(g_norflash_cmd0)=0xAA;
	SST_FLASH_MA(g_norflash_cmd1)=0x55;
	SST_FLASH_MA(g_norflash_cmd2)=0x80;
	SST_FLASH_MA(g_norflash_cmd3)=0xAA;
	SST_FLASH_MA(g_norflash_cmd4)=0x55;
#endif
	*(UINT16*)addr = 0x50;

	enable_interrupt();
	sst_wait_flash();
}

void sst_block_erase(UINT32 addr)
{
	disable_interrupt();
#if C0_SMALL_END
	SST_FLASH_MA(g_norflash_cmd0)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd1)=0x5500;
	SST_FLASH_MA(g_norflash_cmd2)=0x8000;
	SST_FLASH_MA(g_norflash_cmd3)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd4)=0x5500;
#else
	SST_FLASH_MA(g_norflash_cmd0)=0xAA;
	SST_FLASH_MA(g_norflash_cmd1)=0x55;
	SST_FLASH_MA(g_norflash_cmd2)=0x80;
	SST_FLASH_MA(g_norflash_cmd3)=0xAA;
	SST_FLASH_MA(g_norflash_cmd4)=0x55;
#endif
	*(UINT16*)addr = 0x30;

	enable_interrupt();
	sst_wait_flash();
}

void sst_program(UINT32 addr,UINT16 data)
{
	disable_interrupt();
#if C0_SMALL_END
	SST_FLASH_MA(g_norflash_cmd0)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd1)=0x5500;
	SST_FLASH_MA(g_norflash_cmd2)=0xA000;
#else
	SST_FLASH_MA(g_norflash_cmd0)=0xAA;
	SST_FLASH_MA(g_norflash_cmd1)=0x55;
	SST_FLASH_MA(g_norflash_cmd2)=0xA0;
#endif
	delaySysClk(20);

	*(UINT16*)addr = data;

	enable_interrupt();

	sst_wait_flash();
}

UINT16 sst_read_id(UINT32 addr)
{
	UINT16 val=0x0000;

	disable_interrupt();

	sst_chip_reset();	//Exit Read ID mode

/*
	SST_FLASH_MA(g_norflash_cmd0)=0xAA00;
	SST_FLASH_MA(g_norflash_cmd1)=0x5500;
	SST_FLASH_MA(g_norflash_cmd2)=0x9000;
*/
	//C0 LIT_END
	SST_FLASH_MA(g_norflash_cmd0)=0xAA;
	SST_FLASH_MA(g_norflash_cmd1)=0x55;
	SST_FLASH_MA(g_norflash_cmd2)=0x90;

	delaySysClk(1000);
	val = SST_FLASH_MA(addr);
	sst_chip_reset();	//Exit Read ID mode

	delaySysClk(1000);
	enable_interrupt();
	return val;
}
