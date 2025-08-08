#ifndef __SECMON_H__
#define __SECMON_H__

#define SECMON_BASE_ADDR				0xEB006000ull
#define SECMON_INT_NUM					90

#define SECMON_HPLR_REG					(SECMON_BASE_ADDR+0x00)
#define SECMON_HPCMDR_REG				(SECMON_BASE_ADDR+0x04)
#define SECMON_HPSICR_REG				(SECMON_BASE_ADDR+0x08) //interrupt control
#define SECMON_HPSVCR_REG				(SECMON_BASE_ADDR+0x0C) //sv control
#define SECMON_HPSR_REG					(SECMON_BASE_ADDR+0x10) //ssm st status
#define SECMON_HPSVSR_REG				(SECMON_BASE_ADDR+0x14) //sv status
#define SECMON_HPHACIVR_REG				(SECMON_BASE_ADDR+0x18) //counter init
#define SECMON_HPHACR_REG				(SECMON_BASE_ADDR+0x1C)
#define SECMON_HPMISCR_REG				(SECMON_BASE_ADDR+0x20)
#define SECMON_HPFILTR_REG				(SECMON_BASE_ADDR+0x24)
#define SECMON_HPVIDR0_REG				(SECMON_BASE_ADDR+0x28)
#define SECMON_HPVIDR1_REG				(SECMON_BASE_ADDR+0x2C)

#define HPLR_HAC_LOCK					(1<<13)
#define HPLR_HPSICR_LOCK				(1<<14)
#define HPLR_HPSVCR_LOCK				(1<<15)
#define HPLR_MISC_LOCK					(1<<24)

#define HPCMDR_HAC_STOP					(1<<12)
#define HPCMDR_HAC_CLEAR				(1<<13)
#define HPCMDR_HAC_LOAD					(1<<14)
#define HPCMDR_HAC_EN					(1<<15)
#define HPCMDR_SW_SV					(1<<23)
#define HPCMDR_SFNS_DIS					(1<<29)
#define HPCMDR_ST_DIS					(1<<30)
#define HPCMDR_SSM_ST					(1<<31)

#define HPSICR_SVI_EN_OP				(1<<19)
#define HPSICR_SVI_EN_LD				(1<<20)
#define HPSICR_SVI_EN_TD				(1<<21)
#define HPSICR_SVI_EN_VD				(1<<22)
#define HPSICR_SVI_EN_FD				(1<<23)
#define HPSICR_SVI_EN_PGD				(1<<24)
#define HPSICR_SVI_EN_MESH				(1<<25)
#define HPSICR_SVI_EN_RTIC_h20x8			(1<<26)
#define HPSICR_SVI_EN_RTIC				(1<<27)
#define HPSICR_SVI_EN_ACU_JR			(1<<27)
#define HPSICR_SVI_EN_ACU				(1<<28)
#define HPSICR_SVI_EN_TMP				(1<<29)
#define HPSICR_SVI_EN_SFM				(1<<30)
#define HPSICR_SVI_EN_BAD_KEY			(1<<31)

#define HPSVCR_CFG_OP					(1<<19)
#define HPSVCR_CFG_LD					(1<<20)
#define HPSVCR_CFG_TD					(1<<21)
#define HPSVCR_CFG_VD					(1<<22)
#define HPSVCR_CFG_FD					(1<<23)
#define HPSVCR_CFG_PGD					(1<<24)
#define HPSVCR_CFG_MESH					(1<<25)
#define HPSVCR_CFG_RTIC_h20x8				(1<<26)
#define HPSVCR_CFG_RTIC					(1<<27)
#define HPSVCR_CFG_ACU_JR				(1<<27)
#define HPSVCR_CFG_ACU					(1<<28)
#define HPSVCR_CFG_TMP					(1<<29)
#define HPSVCR_CFG_SFM					(1<<30)
#define HPSVCR_CFG_BAD_KEY				(1<<31)

/*
#define HPSICR_SVI_EN_LP_L				(1<<0)
#define HPSICR_SVI_EN_LP_H				(2<<0)
#define HPSICR_SVI_EN_RTIC				(1<<28)
#define HPSICR_SVI_EN_ACU				(1<<29)
#define HPSICR_SVI_EN_TMP				(1<<30)
#define HPSICR_SVI_EN_FUSE				(1<<31)

#define HPSVCR_CFG_LP					(1<<0)
#define HPSVCR_CFG_RTIC					(1<<28)
#define HPSVCR_CFG_ACU					(1<<29)
#define HPSVCR_CFG_TMP					(1<<30)
#define HPSVCR_CFG_FUSE					(1<<31)
*/
#define HPSVSR_SEC_VIO_OP				(1<<19)
#define HPSVSR_SEC_VIO_LD				(1<<20)
#define HPSVSR_SEC_VIO_TD				(1<<21)
#define HPSVSR_SEC_VIO_VD				(1<<22)
#define HPSVSR_SEC_VIO_FD				(1<<23)
#define HPSVSR_SEC_VIO_PGD				(1<<24)
#define HPSVSR_SEC_VIO_MESH				(1<<25)
#define HPSVSR_SEC_VIO_RTIC_h20x8			(1<<26)
#define HPSVSR_SEC_VIO_RTIC				(1<<27)
#define HPSVSR_SEC_VIO_ACU_JR			(1<<27)
#define HPSVSR_SEC_VIO_ACU				(1<<28)
#define HPSVSR_SEC_VIO_TMP				(1<<29)
#define HPSVSR_SEC_VIO_SFM				(1<<30)
#define HPSVSR_SEC_VIO_BAD_KEY			(1<<31)

#define HPSR_SSM_ST_VALUE(n)			((n>>20)&0x0000000F)
#define HPSR_SSM_ST_HARD_FAIL			(0x1)
#define HPSR_SSM_ST_SOFT_FAIL			(0x3)
#define HPSR_SSM_ST_CHECK				(0x9)
#define HPSR_SSM_ST_NON_SECURE			(0xb)
#define HPSR_SSM_ST_TRUSTED				(0xd)
#define HPSR_SSM_ST_SECURE				(0xf)

#define HPMISR_HRST_REQ_DIS				(1<<31)
#endif
