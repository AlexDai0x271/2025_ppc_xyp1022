/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"         /* Obtains integer types */
#include "diskio.h"     /* Declarations of disk functions */

#include "environment.h"

/* Definitions of physical drive number for each drive */
#define DEV_SD_CARD             0

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;

    switch (pdrv) {
    case DEV_SD_CARD :
        stat = 0;
        break;

    default:
        stat = STA_NODISK;
        break;
    }

    return stat;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;

    switch (pdrv) {
    case DEV_SD_CARD :
        if (SD_EnumCard() == SD_OK) {
            stat = 0;
        } else {
            stat = STA_NOINIT;
        }
        break;

    default:
        stat = STA_NODISK;
        break;
    }

    return stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    BYTE *buff,         /* Data buffer to store read data */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to read */
)
{
    DSTATUS stat;

    pdrv = pdrv;
    if (count == 0) return RES_PARERR;

    switch (pdrv) {
    case DEV_SD_CARD :
        if (SD_ReadBlock(sector, (U32*)buff, BLOCK_SIZE, count) == SD_OK) {
            stat = RES_OK;
        } else {
            stat = RES_PARERR;
        }
        break;

    default:
        stat = RES_PARERR;
        break;
    }

    return stat;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DSTATUS stat;

    DWORD addr;
    DWORD offset;

    switch (pdrv) {
    case DEV_SD_CARD :
        if (SD_WriteBlock(sector, (U32*)buff, BLOCK_SIZE, count) == SD_OK) {
            stat = RES_OK;
        } else {
            stat = RES_PARERR;
        }
        break;

    default:
        stat = RES_PARERR;
        break;
    }

    return stat;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,          /* Physical drive nmuber (0..) */
    BYTE cmd,           /* Control code */
    void *buff          /* Buffer to send/receive control data */
)
{
    DSTATUS stat;

    switch (pdrv) {
    case DEV_SD_CARD :
        switch (cmd) {
        case CTRL_SYNC:
            break;
        case GET_SECTOR_COUNT:
            *(DWORD *)buff = 40960;
            break;
        case GET_SECTOR_SIZE:
            *(DWORD *)buff = BLOCK_SIZE;
            break;
        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            break;
        case CTRL_TRIM:
            break;

        default:
            break;
        }

        stat = RES_OK;
        break;

    default:
        stat = RES_PARERR;
        break;
    }

    return stat;
}

DWORD get_fattime (void)
{
    return ((1997ul - 1980ul) << 25) | (9ul << 21) | (1ul << 16) | (13u << 11) | (14u << 5) | (21u >> 1);
}
