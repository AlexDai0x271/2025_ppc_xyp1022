/*
 * iic.h
 *
 *  Created on: 2014-6-23
 *      Author: bbwang
 */

#ifndef IIC_H_
#define IIC_H_

#define IIC_TLB_BAS  0xe9604000


#define IIC0_INT_NUM  33
#define IIC1_INT_NUM  34
//IIC0
#define IIC0_BASE  	IIC_TLB_BAS+0x00000000
#define IIC1_BASE  	IIC_TLB_BAS+0x00001000

#define IIC0_MDB  	*(volatile unsigned char *)(IIC0_BASE+0x00)
#define IIC0_SDB  	*(volatile unsigned char *)(IIC0_BASE+0x02)
#define IIC0_LMA  	*(volatile unsigned char *)(IIC0_BASE+0x04)
#define IIC0_HMA  	*(volatile unsigned char *)(IIC0_BASE+0x05)
#define IIC0_CR   	*(volatile unsigned char *)(IIC0_BASE+0x06)
#define IIC0_MCR  	*(volatile unsigned char *)(IIC0_BASE+0x07)
#define IIC0_SR   	*(volatile unsigned char *)(IIC0_BASE+0x08)
#define IIC0_ESR  	*(volatile unsigned char *)(IIC0_BASE+0x09)
#define IIC0_LSA  	*(volatile unsigned char *)(IIC0_BASE+0x0A)
#define IIC0_HSA  	*(volatile unsigned char *)(IIC0_BASE+0x0B)
#define IIC0_CDR  	*(volatile unsigned char *)(IIC0_BASE+0x0C)
#define IIC0_IER  	*(volatile unsigned char *)(IIC0_BASE+0x0D)
#define IIC0_TCR  	*(volatile unsigned char *)(IIC0_BASE+0x0E)
#define IIC0_ECSSR  	*(volatile unsigned char *)(IIC0_BASE+0x0F)
#define IIC0_DCR    	*(volatile unsigned char *)(IIC0_BASE+0x10)
#define IIC0_IR     	*(volatile unsigned char *)(IIC0_BASE+0x11)
//IIC1

#define IIC1_MDB  	*(volatile unsigned char *)(IIC1_BASE+0x00)
#define IIC1_SDB  	*(volatile unsigned char *)(IIC1_BASE+0x02)
#define IIC1_LMA  	*(volatile unsigned char *)(IIC1_BASE+0x04)
#define IIC1_HMA  	*(volatile unsigned char *)(IIC1_BASE+0x05)
#define IIC1_CR   	*(volatile unsigned char *)(IIC1_BASE+0x06)
#define IIC1_MCR  	*(volatile unsigned char *)(IIC1_BASE+0x07)
#define IIC1_SR   	*(volatile unsigned char *)(IIC1_BASE+0x08)
#define IIC1_ESR  	*(volatile unsigned char *)(IIC1_BASE+0x09)
#define IIC1_LSA  	*(volatile unsigned char *)(IIC1_BASE+0x0A)
#define IIC1_HSA  	*(volatile unsigned char *)(IIC1_BASE+0x0B)
#define IIC1_CDR  	*(volatile unsigned char *)(IIC1_BASE+0x0C)
#define IIC1_IER  	*(volatile unsigned char *)(IIC1_BASE+0x0D)
#define IIC1_TCR  	*(volatile unsigned char *)(IIC1_BASE+0x0E)
#define IIC1_ECSSR  	*(volatile unsigned char *)(IIC1_BASE+0x0F)
#define IIC1_DCR    	*(volatile unsigned char *)(IIC1_BASE+0x10)
#define IIC1_IR     	*(volatile unsigned char *)(IIC1_BASE+0x11)

//Control Register
#define IIC_CR_HMT_MASK   	0x80
#define IIC_CR_AMC_MASK   	0x40
#define IIC_CR_XFCM_MASK   	0x20
#define IIC_CR_XFCL_MASK   	0x10
#define IIC_CR_BCL_MASK   	0x08
#define IIC_CR_XFC_MASK   	0x04
#define IIC_CR_RNW_MASK   	0x02
#define IIC_CR_PXF_MASK   	0x01

//Mode Control Register
#define IIC_MCR_FSB_MASK    0x80
#define IIC_MCR_FMB_MASK    0x40
#define IIC_MCR_GCE_MASK    0x20
#define IIC_MCR_SPMD_MASK   0x10
#define IIC_MCR_SMD_MASK    0x08
#define IIC_MCR_INTE_MASK   0x04
#define IIC_MCR_EUS_MASK    0x02
#define IIC_MCR_PCL_MASK    0x01

//Status Register
#define IIC_SR_SSET_MASK	0x80
#define IIC_SR_SLR_MASK		0x40
#define IIC_SR_MDH_MASK		0x20
#define IIC_SR_MDF_MASK		0x10
#define IIC_SR_HLT_MASK		0x08
#define IIC_SR_ERR_MASK		0x04
#define IIC_SR_IRQ_MASK		0x02
#define IIC_SR_PXF_MASK		0x01

//Extended Status Register
#define IIC_ESR_ILLS_MASK	0x80
#define IIC_ESR_BCS6_MASK	0x40
#define IIC_ESR_BCS5_MASK	0x20
#define IIC_ESR_BCS4_MASK	0x10
#define IIC_ESR_ILL_MASK	0x08
#define IIC_ESR_LARB_MASK	0x04
#define IIC_ESR_IXF_MASK	0x02
#define IIC_ESR_ANAK_MASK	0x01

//Interrupt Enable Register
#define IIC_IER_IRQSVC_MASK	0x80
#define IIC_IER_IRQSVR_MASK	0x40
#define IIC_IER_IRQSW_MASK	0x20
#define IIC_IER_IRQSVW_MASK	0x10
#define IIC_IER_IRQH_MASK	0x08
#define IIC_IER_IRQI_MASK	0x04
#define IIC_IER_IRQA_MASK	0x02
#define IIC_IER_IRQM_MASK	0x01

//Transfer Count Register
#define IIC_TCR_STC6_MASK	0x40
#define IIC_TCR_STC5_MASK	0x20
#define IIC_TCR_STC4_MASK	0x10
#define IIC_TCR_MTC2_MASK	0x04
#define IIC_TCR_MTC1_MASK	0x02
#define IIC_TCR_MTC0_MASK	0x01

//Extended Control and Slave Status Register
#define IIC_ECSSR_COMR_MASK	0x80
#define IIC_ECSSR_SRVR_MASK	0x40
#define IIC_ECSSR_COMW_MASK	0x20
#define IIC_ECSSR_SRVW_MASK	0x10
#define IIC_ECSSR_SHD_MASK	0x08
#define IIC_ECSSR_SFUL_MASK	0x04
#define IIC_ECSSR_PIRQ_MASK	0x02
#define IIC_ECSSR_SRST_MASK	0x01

//Direct Control Register
#define IIC_DCR_CSDA_MASK	0x08
#define IIC_DCR_CSDL_MASK	0x04
#define IIC_DCR_SDA_MASK	0x02
#define IIC_DCR_SDL_MASK	0x01

//Interrupt Register
#define IIC_IR_IRQP_MASK	0x02
#define IIC_IR_IRQD_MASK	0x01


/*****************************************************************/
#define IIC_REG_INIT_TEST		0
#define IIC_REG_RESET_TEST		0
#define IIC_MS_RW_TEST			1
#define IIC_INT_TEST			0
#define IIC_RTC_DRIVER_TEST		0
/*****************************************************************/
#if IIC_MS_RW_TEST
	#define MASTER_READ_OP      1
	#define MASTER_WRITE_OP     1
#if MASTER_READ_OP
		#define MR_STANDARD_MODE       0
        #define MR_FAST_MODE           0
        #define MR_ADR_7_BIT           0
	    #define MR_ADR_10_BIT          0
        #define MR_CNT_4_BYTE          0
        #define MR_CHAIN_FUN           1
#endif
#if MASTER_WRITE_OP
		#define MW_STANDARD_MODE       0
        #define MW_FAST_MODE           0
        #define MW_ADR_7_BIT           0
	    #define MW_ADR_10_BIT          0
        #define MW_CNT_4_BYTE          0
        #define MW_CHAIN_FUN           1
#endif

#endif
/*************************************************************/
#if IIC_INT_TEST
	#define	MASTER_INTER_EN	1
	#define	SLAVE_INTER_EN	1
	#define IRQSVC_EN   1
	#define IRQSVR_EN   1
	#define IRQSW_EN    1
	#define IRQSVW_EN   1
	#define IRQH_EN   	1
	#define IRQI_EN   	1
	#define IRQA_EN   	1
	#define IRQM_EN   	1
#endif

#define MTREG(addr, val)   		(*(volatile char *)addr) = val
#define MFREG(val, addr)   		val = (*(volatile char *)addr)
/*****************************************************************/

// typedef unsigned char		Uint8;
// typedef unsigned char		UINT8;
// typedef unsigned char		uint8;
// typedef char			int8;
// typedef unsigned short		Uint16;
// typedef unsigned short		UINT16;
// typedef unsigned short		uint16;
// typedef short			int16;
// typedef unsigned long		Uint32;
// typedef unsigned long		UINT32;
// typedef unsigned long		uint32;


void iic_reg_init_test();
void iic_reg_reset_test();
void iic_ms_rw_test();
void iic_int_test();


#endif /* IIC_H_ */
