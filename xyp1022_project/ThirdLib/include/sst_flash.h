#ifndef SST_FLASH_H
#define SST_FLASH_H
#include "common.h"
#include "ebc.h"

//#define  FLASH_BASE_ADDR     0x80000000				 	 //Norflash base address
//#define  FLASH_MAX_ADDR      0x80800000					 //Norflash MAX address

#define  FLASH_BASE_ADDR     0xF0000000				 	 //Norflash base address

#define  FLASH_MAX_ADDR      0xF8000000					 //Norflash MAX address
#define  FLASH_SECTOR_NUM    (2048)//(2032)
#define  FLASH_SECTOR_SIZE   (2048) //halfword
#define  FLASH_BLOCK_NUM     (1) //bottom from 0x0
#define  FLASH_BLOCK_SIZE    (32768) //halfword


#define  SST_FLASH_BASE       ((volatile UINT16*)(FLASH_BASE_ADDR))
#define  SST_FLASH_ADDR(xxx)  (SST_FLASH_BASE+(xxx))
#define  SST_FLASH_MA(xxx)    (*SST_FLASH_ADDR(xxx))
#if 1
#define g_norflash_cmd0 (0x5555)
#define g_norflash_cmd1 (0x2AAA)
#define g_norflash_cmd2 (0x5555)
#define g_norflash_cmd3 (0x5555)
#define g_norflash_cmd4 (0x2AAA)
#define g_norflash_cmd5 (0x5555)
#else
#define g_norflash_cmd0 (0x555<<1)
#define g_norflash_cmd1 (0x2AA<<1)
#define g_norflash_cmd2 (0x555<<1)
#define g_norflash_cmd3 (0x555<<1)
#define g_norflash_cmd4 (0x2AA<<1)
#define g_norflash_cmd5 (0x555<<1)
#endif
#endif
