/*
 * nandflash-mt29f.h
 *
 *  Created on: 2023-7-24
 *      Author: DELL
 */

#ifndef NANDFLASH_MT29F_H_
#define NANDFLASH_MT29F_H_

////////////////////////////////////////////////////////////////////////////
//Command Set
#define	CMD_RESET 		0xFF	
#define	CMD_READ_ID		0x90		

#define	CMD_READ_STATUS 			0x70 //READ STATUS (70h)		
#define CMD_READSTATUS_ENHANCED		0x78 //READ STATUS ENHANCED (78h)


#define	CMD_READ_MODE		0x00 
//#define	CMD_READ_PAGE		0x00 //READ PAGE (00h-30h)
//#define	CMD_READ_PAGE_2		0x30 //READ PAGE (00h-30h)
//#define	CMD_READPAGE_CACHE_RANDOM		0x00 //READ PAGE CACHE RANDOM (00h-31h)
//#define	CMD_READPAGE_CACHE_RANDOM_2		0x31 //READ PAGE CACHE RANDOM (00h-31h)
//#define	CMD_READPAGE_MULTI_PLANE		0x00 //READ PAGE MULTI-PLANE (00h-32h)
//#define	CMD_READPAGE_MULTI_PLANE_2		0x32 //READ PAGE MULTI-PLANE (00h-32h)
#define	CMD_READ_PAGE		0x30 //READ PAGE (00h-30h)
#define	CMD_READPAGE_CACHE_RANDOM		0x31 //READ PAGE CACHE RANDOM (00h-31h)
#define	CMD_READPAGE_MULTI_PLANE		0x32 //READ PAGE MULTI-PLANE (00h-32h)

#define	CMD_READPAGE_CACHE_SEQUENTIAL	0x31 //
#define	CMD_READPAGE_CACHE_LAST			0x3F //


//#define	CMD_PROGRAM_PAGE			0x80 //PROGRAM PAGE (80h-10h)
//#define	CMD_PROGRAM_PAGE_2			0x10 //PROGRAM PAGE (80h-10h)
//#define	CMD_PROGPAGE_CACHE			0x80 //PROGRAM PAGE CACHE (80h-15h)
//#define	CMD_PROGPAGE_CACHE_2		0x15 //PROGRAM PAGE CACHE (80h-15h)
//#define	CMD_PROGPAGE_MULTI_PLANE	0x80 //PROGRAM PAGE MULTI-PLANE 80h-11h
//#define	CMD_PROGPAGE_MULTI_PLANE_2	0x11 //PROGRAM PAGE MULTI-PLANE 80h-11h
#define	CMD_PROGRAM_MODE			0x80 
#define	CMD_PROGRAM_PAGE			0x10 //PROGRAM PAGE (80h-10h)
#define	CMD_PROGPAGE_CACHE			0x15 //PROGRAM PAGE CACHE (80h-15h)
#define	CMD_PROGPAGE_MULTI_PLANE	0x11 //PROGRAM PAGE MULTI-PLANE 80h-11h

//#define	CMD_ERASE_BLOCK					0x60 //ERASE BLOCK (60h-D0h)
//#define	CMD_ERASE_BLOCK_2				0xD0 //ERASE BLOCK (60h-D0h)
//#define	CMD_ERASEBLOCK_MULTI_PLANE		0x60 //ERASE BLOCK MULTI-PLANE (60h-D1h)
//#define	CMD_ERASEBLOCK_MULTI_PLANE_2	0xD1 //ERASE BLOCK MULTI-PLANE (60h-D1h)
#define	CMD_ERASE_MODE				0x60 //ERASE BLOCK (60h-D0h)
#define	CMD_ERASE_BLOCK				0xD0 //ERASE BLOCK (60h-D0h)
#define	CMD_ERASEBLOCK_MULTI_PLANE	0xD1 //ERASE BLOCK MULTI-PLANE (60h-D1h)

////////////////////////////////////////////////////////////////////////////
//Status Register Definition
#define	STS_WPn		(1 <<7) 
#define	STS_RDY		(1 <<6)
#define	STS_ARDY	(1 <<5)
#define	STS_FAILC	(1 <<1)
#define	STS_FAIL	(1 <<0)

////////////////////////////////////////////////////////////////////////////
#if 0
	extern u16 W25q_SpiFlash_Read_MDID();
	extern u32 W25q_SpiFlash_Read_JEDECID();
	
	extern void W25q_SpiFlash_Write_Enable();
	extern void W25q_SpiFlash_VSR_Write_Enable();
	extern void W25q_SpiFlash_Write_Disable();
	extern u8 W25q_SpiFlash_Read_Status(u8 stid);
	extern void W25q_SpiFlash_Write_Status(u8 stid,u8 sta);
	extern void W25q_SpiFlash_Enter_4Baddr_Mode();
	extern void W25q_SpiFlash_Exit_4Baddr_Mode();
	extern void W25q_SpiFlash_ReadData(u8 mode,u32 start_addr,u32 len,u8 *pbuf);
	extern void W25q_SpiFlash_Erase(u8 mode,u32 start_addr);
#endif
////////////////////////////////////////////////////////////////////////////
#endif /* MT29FxG08x_H_ */
