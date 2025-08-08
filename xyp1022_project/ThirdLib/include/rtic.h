/*
 * File: rtic.h
 * Date: 2017/11/15
 * Author: sytu@ccore_sys
 */

#ifndef RTIC_H_
#define RTIC_H_

typedef unsigned char 	uint8;
typedef unsigned short  uint16;
typedef unsigned int  	uint32;


/********* RTIC_Configuration & Status_Registers *********/
#define RTIC_BASE_ADDR			0xe0104000

#define RTIC_RSTA_REG				(0x00 + RTIC_BASE_ADDR)
#define RTIC_RCMD_REG				(0x04 + RTIC_BASE_ADDR)
#define RTIC_RCTL_REG				(0x08 + RTIC_BASE_ADDR)
#define RTIC_RTHR_MS_REG			(0x0C + RTIC_BASE_ADDR)
#define RTIC_RTHR_LS_REG			(0x10 + RTIC_BASE_ADDR)
#define RTIC_RWDOG_MS_REG			(0x14 + RTIC_BASE_ADDR)
#define RTIC_RWDOG_LS_REG			(0x18 + RTIC_BASE_ADDR)
#define RTIC_REND_0_REG				(0x1C + RTIC_BASE_ADDR)
#define RTIC_REND_1_REG				(0x20 + RTIC_BASE_ADDR)
#define RTIC_RMAA_MS_REG			(0x24 + RTIC_BASE_ADDR)
#define RTIC_RMAA_LS_REG			(0x28 + RTIC_BASE_ADDR)
#define RTIC_RMAL_REG				(0x2C + RTIC_BASE_ADDR)
#define RTIC_RMBA_MS_REG			(0x30 + RTIC_BASE_ADDR)
#define RTIC_RMBA_LS_REG			(0x34 + RTIC_BASE_ADDR)
#define RTIC_RMBL_REG				(0x38 + RTIC_BASE_ADDR)
#define RTIC_RMCA_MS_REG			(0x3C + RTIC_BASE_ADDR)
#define RTIC_RMCA_LS_REG			(0x40 + RTIC_BASE_ADDR)
#define RTIC_RMCL_REG				(0x44 + RTIC_BASE_ADDR)
#define RTIC_RMDA_MS_REG			(0x48 + RTIC_BASE_ADDR)
#define RTIC_RMDA_LS_REG			(0x4C + RTIC_BASE_ADDR)
#define RTIC_RMDL_REG				(0x50 + RTIC_BASE_ADDR)
#define RTIC_HASH_RAMD			(0x54 + RTIC_BASE_ADDR)
#define RTIC_HASH_RBMD			(0x74 + RTIC_BASE_ADDR)
#define RTIC_HASH_RCMD			(0x94 + RTIC_BASE_ADDR)
#define RTIC_HASH_RDMD			(0xB4 + RTIC_BASE_ADDR)


/*******************   Mask Bits   *******************/
//RSTA
#define RSTA_CS_SHIFT			5
#define RSTA_CS_IDLE			(0x00 << RTIC_CS_SHIFT)
#define RSTA_CS_HASH_ONCE		(0x01 << RTIC_CS_SHIFT)
#define RSTA_CS_RUN				(0x10 << RTIC_CS_SHIFT)
#define RSTA_CS_ERROR			(0x11 << RTIC_CS_SHIFT)

#define RSTA_RTD_RUN_TIME_DISABLE	(1 << 12)
#define RSTA_HOD_HASH_ONCE_DISABLE	(1 << 13)
#define RSTA_WE_ERROR				(1 << 15)
//#define RTIC_ABH

#define RSTA_AE_SHIFT			8
#define AE_MEM_0_ADD_ERROR		(1 << RTIC_AE_SHIFT)
#define AE_MEM_1_ADD_ERROR		(2 << RTIC_AE_SHIFT)
#define AE_MEM_2_ADD_ERROR		(4 << RTIC_AE_SHIFT)
#define AE_MEM_3_ADD_ERROR		(8 << RTIC_AE_SHIFT)

#define RSTA_MIS_SHIFT			4
#define MIS_MEM_0_HASH_ERROR	(1 << RTIC_MIS_SHIFT)
#define MIS_MEM_1_HASH_ERROR	(2 << RTIC_MIS_SHIFT)
#define MIS_MEM_2_HASH_ERROR	(4 << RTIC_MIS_SHIFT)
#define MIS_MEM_3_HASH_ERROR	(8 << RTIC_MIS_SHIFT)

#define RSTA_HE_SHIFT			3
#define HE_HASH_AUTHENTICATED	(0 << RSTA_HE_SHIFT)
#define HE_HASH_ERROR			(1 << RSTA_HE_SHIFT)

#define RSTA_SV_SHIFT			2
#define SV_AUTHENTICATED		(0 << RSTA_SV_SHIFT)
#define SV_ERROR				(1 << RSTA_SV_SHIFT)

#define RSTA_HD_SHIFT			1
#define HD_HASH_ONCE_DISABLE	(0 << RSTA_HD_SHIFT)
#define HD_HASH_ONCE_DONE		(1 << RSTA_HD_SHIFT)

#define RSTA_BSY_IDLE			0
#define RSTA_BSY_BUSY			1

//RCMD
#define RCMD_RTD_SHIFT			28
#define RCMD_RTD_ALLOW			(0 << RCMD_RTD_SHIFT)
#define RCMD_RTD_PREVENT		(1 << RCMD_RTD_SHIFT)

#define RCMD_RTC_SHIFT			29
#define RCMD_RTC_NONE			(0 << RCMD_RTC_SHIFT)
#define RCMD_RTC_CONTINUALLY	(1 << RCMD_RTC_SHIFT)

#define RCMD_HO_SHIFT			30
#define RCMD_HO_NONE				(0 << RCMD_HO_SHIFT)
#define RCMD_HO_HASH_ONCE_ENABLE	(1 << RCMD_HO_SHIFT)

#define RCMD_CLEAR_SHIFT		31
#define RCMD_CLEAR_NONE			(0 << RCMD_CLEAR_SHIFT)
#define RCMD_CLEAR				(1 << RCMD_CLEAR_SHIFT)

//RCTL
#define RCTL_RALG_SHIFT			12
#define SHA_256_MEM_0			(0 << RCTL_RALG_SHIFT)
#define SHA_256_MEM_1			(0 << RCTL_RALG_SHIFT)
#define SHA_256_MEM_2			(0 << RCTL_RALG_SHIFT)
#define SHA_256_MEM_3			(0 << RCTL_RALG_SHIFT)
#define SM3_MEM_0			(1 << RCTL_RALG_SHIFT)
#define SM3_MEM_1			(2 << RCTL_RALG_SHIFT)
#define SM3_MEM_2			(4 << RCTL_RALG_SHIFT)
#define SM3_MEM_3			(8 << RCTL_RALG_SHIFT)

#define RCTL_RTME_SHIFT			20
#define RTME_MEM_0_ENANLE		(1 << RCTL_RTME_SHIFT)
#define RTME_MEM_1_ENANLE		(2 << RCTL_RTME_SHIFT)
#define RTME_MEM_2_ENANLE		(4 << RCTL_RTME_SHIFT)
#define RTME_MEM_3_ENANLE		(8 << RCTL_RTME_SHIFT)

#define RCTL_HOME_SHIFT			24
#define HOME_MEM_0_ENABLE		(1 << RCTL_HOME_SHIFT)
#define HOME_MEM_1_ENABLE		(2 << RCTL_HOME_SHIFT)
#define HOME_MEM_2_ENABLE		(4 << RCTL_HOME_SHIFT)
#define HOME_MEM_3_ENABLE		(8 << RCTL_HOME_SHIFT)

#define RCTL_RREQS_SHIFT		29
#define RREQS_1					(0 << RCTL_RREQS_SHIFT)
#define RREQS_2					(1 << RCTL_RREQS_SHIFT)
#define RREQS_4					(2 << RCTL_RREQS_SHIFT)
#define RREQS_8					(3 << RCTL_RREQS_SHIFT)

#define RCTL_IE_disabled		0	<< 31
#define RCTL_IE_enabled			1	<< 31

#endif /* RTIC_H_ */
