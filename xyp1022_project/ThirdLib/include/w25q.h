/*
 * w25q.h
 *	W25Q128FV/SERIAL FLASH MEMORY WITH standard Serial Peripheral Interface (SPI) & DUAL/QUAD SPI & QPI
 *  Created on: 
 *      Author: ccore sys
 */

#ifndef W25Q_H_
#define W25Q_H_

////////////////////////////////////////////////////////////////////////////
#define CMD_W25Q_MANUFACTDEVICEID     		0x90
#define CMD_W25Q_JEDECDEVICEID        		0x9F

#define CMD_W25Q_WRITEENABLE          		0x06
#define CMD_W25Q_VSR_WRITEENABLE      		0x50
#define CMD_W25Q_WRITEDISABLE         		0x04

#define CMD_W25Q_READSTATUS_1         		0x05
#define CMD_W25Q_WRITESTATUS_1        		0x01
#define CMD_W25Q_READSTATUS_2         		0x35
#define CMD_W25Q_WRITESTATUS_2        		0x31
#define CMD_W25Q_READSTATUS_3         		0x15
#define CMD_W25Q_WRITESTATUS_3        		0x11

//#define CMD_W25Q_READ_EXTADDR         		0xC8
//#define CMD_W25Q_WRIT_EXTADDR         		0xC5
//#define CMD_W25Q_ENTER_4BADDR_MODE    	0xB7
//#define CMD_W25Q_EXIT_4BADDR_MODE     		0xE9

#define CMD_W25Q_READDATA             			0x03
//#define CMD_W25Q_READDATA_WITH_4BADDR   		0x13
#define CMD_W25Q_FASTREADDATA           			0x0B
//#define CMD_W25Q_FASTREADDATA_WITH_4BADDR   	0x0C
#define CMD_W25Q_BURST_READ_WITH_WARP	    	0x0C
#define CMD_W25Q_FASTREADDATA_DUAL 				0x3B
#define CMD_W25Q_FASTREADDATA_QUARD				0x6B //Fast Read Quad Output
#define CMD_W25Q_FASTREADDATA_QUARD_ALL			0xEB //Fast Read Quad I/O (EBh)

#define CMD_W25Q_SECTOR_ERASE               	0x20
#define CMD_W25Q_BLOCK_32K_ERASE            	0x52
#define CMD_W25Q_BLOCK_64K_ERASE           		0xD8 //SECTOR_ERASE
#define CMD_W25Q_CHIP_ERASE            			0xC7

#define CMD_W25Q_PAGE_PROGRAM   		 		0x02 //Page Program 
#define CMD_W25Q_QUARD_Page_PROGRAM				0x32

////////////////////////////////////////////////////////////////////////////
#define W25Q_MANUID_WINBOND     			0xEF
#define W25Q_DEV_ID    						0x17

#define W25Q_JEDEC_ID_SPI_MODE     			0x4018
#define W25Q_JEDEC_ID_QPI_MODE     			0x6018
////////////////////////////////////////////////////////////////////////////
#define DUMMY      	        					0xFF
#define MODE_4B_ADDR        					0x01
#define MODE_FAST_READ      					0x02

#define STATUS3_ADS_MASK    					0x01
#define STATUS2_Quad_Enable_MASK				0x02

#define STATUS1_BUSY_MASK   					0x01 //0 state indicating the device is ready for further instructions
#define STATUS1_WEL_MASK    					0x02
////////////////////////////////////////////////////////////////////////////
#define SECTOR_ERASE_MODE       				0x0
#define BLOCK_32K_ERASE_MODE   					0x1
#define BLOCK_64K_ERASE_MODE    				0x2
#define CHIP_ERASE_MODE         				0x3
////////////////////////////////////////////////////////////////////////////
#define CPU_MODE      							0
#define PAGE_LEN      							0x100
#define SECTOR_LEN    							0x1000
#define BLOCK_LEN     							0x10000
////////////////////////////////////////////////////////////////////////////
#if 0
	void W25q_SpiFlash_Read_MDID( UINT8* );
	void W25q_SpiFlash_Read_JEDECID( UINT8* );
	
	UINT16 W25q_SpiFlash_ReadStatus( UINT8 );
	void W25q_SpiFlash_ReadData( UINT32 addr,UINT32 size,UINT8* );
	void W25q_SpiFlash_WriteEnable( void );
	void W25q_SpiFlash_PageProgram( UINT32 addr,UINT32 size,UINT8* );
	void W25q_SpiFlash_SectorEarse( UINT32 addr );
	void W25q_SpiFlash_64KBlockErase( UINT32 addr );
	void W25q_SpiFlash_ChipErase( void );
#endif
////////////////////////////////////////////////////////////////////////////
#endif /* W25Q_H_ */

