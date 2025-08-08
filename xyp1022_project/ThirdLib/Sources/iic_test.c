/*

 * iic_test.c
 *
 *  Created on: 2014��8��17��
 *      Author: bbwang
 */

#include "iic.h"

#include "e500mc_asm.h"
#include "mpic.h"
#include "types.h"

#define uart_printf(...) xprintf(__VA_ARGS__)
#define uart_printf_buf(...)

#if  1
volatile unsigned char iic_irqh_int_flag;
volatile unsigned char iic_irqi_int_flag;
volatile unsigned char iic_irqa_int_flag;
volatile unsigned char iic_irqm_int_flag;
volatile unsigned char iic_irqsvc_int_flag;
volatile unsigned char iic_irqsvr_int_flag;
volatile unsigned char iic_irqsw_int_flag;
volatile unsigned char iic_irqsvw_int_flag;

#if IIC_REG_INIT_TEST
/*
 * iic_reg_init_test:�Ĵ�����ʼֵ����
 */
void iic_reg_init_test()
{
 	unsigned char value0 = 0;
 	unsigned char value1 = 0;

 	value0 = IIC0_MDB;
 	value1 = IIC1_MDB;
// 	printf("IIC0_MDB:%x, IIC1_MDB:%x", value0, value1);

 	value0 = IIC0_SDB;
 	value1 = IIC1_SDB;
 //	printf("IIC0_SDB:%x, IIC1_SDB:%x", value0, value1);

 	value0 = IIC0_LMA;
 	value1 = IIC1_LMA;
// 	printf("IIC0_LMA:%x, IIC1_LMA:%x", value0, value1);

 	value0 = IIC0_HMA;
 	value1 = IIC1_HMA;
 //	printf("IIC0_HMA:%x, IIC1_HMA:%x", value0, value1);

	value0 = IIC0_LSA;
 	value1 = IIC1_LSA;
 //	printf("IIC0_MDB:%x, IIC1_MDB:%x", value0, value1);

	value0 = IIC0_HSA;
 	value1 = IIC1_HSA;
 //	printf("IIC0_MDB:%x, IIC1_MDB:%x", value0, value1);

	value0 = IIC0_DCR;
 	value1 = IIC1_DCR;
// 	printf("IIC0_MDB:%x, IIC1_MDB:%x", value0, value1);

 	value0 = IIC0_CR;
 	value1 = IIC1_CR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	value0 = IIC0_MCR;
 	value1 = IIC1_MCR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	value0 = IIC0_SR;
 	value1 = IIC1_SR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	value0 = IIC0_ESR;
 	value1 = IIC1_ESR;
 	if((value0!=0x00)||(value1!=0x00))  //0x60
 	{
 		//while(1);
 	}

 	value0 = IIC0_CDR;
 	value1 = IIC1_CDR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	value0 = IIC0_IER;
 	value1 = IIC1_IER;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	value0 = IIC0_TCR;
 	value1 = IIC1_TCR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	value0 = IIC0_ECSSR;
 	value1 = IIC1_ECSSR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	value0 = IIC0_IR;
 	value1 = IIC1_IR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}
}
#endif
#if IIC_REG_RESET_TEST
/*
 * iic_reg_reset_test:�Ĵ�����λ����
 */
void iic_reg_reset_test()
{
 	volatile unsigned char value0 = 0;
 	volatile unsigned char value1 = 0;
 	value0 = IIC0_IR;
 	printf("IIC0_IR:%x", value0);

 	 	 	value0 = IIC0_DCR;
 	 	 	printf("IIC0_DCR:%x", value0);

 	    IIC0_ECSSR = 0x00;
 		delay(0x40);
 		//IIC0_IR = 0x5A;

 	    value0 = 0;

 	 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;

 	 	delay(0x40);
 	 	value0 = IIC0_IR;
 	 	printf("IIC0_IR:%x\r\n", value0);

 		IIC0_ECSSR = 0x00;
 		delay(0x40);
 		//IIC0_DCR = 0x5A;

 	    value0 = 0;

 	 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;

 	 	delay(0x40);
 	 	value0 = IIC0_DCR;
 	 	printf("IIC0_DCR:%x\r\n", value0);

 	while(1);

 	IIC0_MDB = 0x5A;
 	IIC1_MDB = 0x5A;
 	value0 = IIC0_MDB;
 	value1 = IIC1_MDB;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_MDB;
 	value1 = IIC1_MDB;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_SDB = 0x5A;
 	IIC1_SDB = 0x5A;
 	value0 = IIC0_SDB;
 	value1 = IIC1_SDB;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_SDB;
 	value1 = IIC1_SDB;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_LMA = 0x5A;
 	IIC1_LMA = 0x5A;
 	value0 = IIC0_LMA;
 	value1 = IIC1_LMA;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_LMA;
 	value1 = IIC1_LMA;
 	if((value0!=0x5A)||(value1!=0x5A))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_HMA = 0x5A;
 	IIC1_HMA = 0x5A;
 	value0 = IIC0_HMA;
 	value1 = IIC1_HMA;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_HMA;
 	value1 = IIC1_HMA;
 	if((value0!=0x5A)||(value1!=0x5A))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_CR = 0x5A;
 	IIC1_CR = 0x5A;
	value0 = IIC0_CR;
 	value1 = IIC1_CR;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_CR;
 	value1 = IIC1_CR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_MCR = 0x5A;
 	IIC1_MCR = 0x5A;
	value0 = IIC0_MCR;
 	value1 = IIC1_MCR;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_MCR;
 	value1 = IIC1_MCR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_LSA = 0x5A;
 	IIC1_LSA = 0x5A;
	value0 = IIC0_LSA;
 	value1 = IIC1_LSA;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_LSA;
 	value1 = IIC1_LSA;
 	if((value0!=0x5A)||(value1!=0x5A))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_HSA = 0x5A;
 	IIC1_HSA = 0x5A;
 	value0 = IIC0_HSA;
 	value1 = IIC1_HSA;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_HSA;
 	value1 = IIC1_HSA;
 	if((value0!=0x5A)||(value1!=0x5A))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_CDR = 0x5A;
 	IIC1_CDR = 0x5A;
 	value0 = IIC0_CDR;
 	value1 = IIC1_CDR;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_CDR;
 	value1 = IIC1_CDR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_IER = 0x5A;
 	IIC1_IER = 0x5A;
 	value0 = IIC0_IER;
 	value1 = IIC1_IER;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_IER;
 	value1 = IIC1_IER;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
 	IIC1_ECSSR = 0x00;
 	IIC0_TCR = 0x5A;
 	IIC1_TCR = 0x5A;
 	value0 = IIC0_TCR;
 	value1 = IIC1_TCR;
 	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
 	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
 	value0 = IIC0_TCR;
 	value1 = IIC1_TCR;
 	if((value0!=0x00)||(value1!=0x00))
 	{
 		while(1);
 	}

 	IIC0_ECSSR = 0x00;
	IIC1_ECSSR = 0x00;
	IIC0_DCR = 0x5A;
	IIC0_DCR = 0x5A;
	value0 = IIC0_DCR;
	value1 = IIC0_DCR;
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
	value0 = IIC0_DCR;
	value1 = IIC0_DCR;
	if((value0!=0x00)||(value1!=0x00))
	{
		while(1);
	}

 	IIC0_ECSSR = 0x00;
	IIC1_ECSSR = 0x00;
	IIC0_IR = 0x5A;
	IIC0_IR = 0x5A;
	value0 = IIC0_IR;
	value1 = IIC0_IR;
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
	value0 = IIC0_IR;
	value1 = IIC0_IR;
	if((value0!=0x00)||(value1!=0x00))
	{
		while(1);
	}

	IIC0_ECSSR = 0x00;
	IIC1_ECSSR = 0x00;
	IIC0_SR = 0x5A;
	IIC0_SR = 0x5A;
	value0 = IIC0_SR;
	value1 = IIC0_SR;
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;
	value0 = IIC0_SR;
	value1 = IIC0_SR;
	if((value0!=0x00)||(value1!=0x00))
	{
		while(1);
	}

	IIC0_ECSSR = 0x00;
	IIC1_ECSSR = 0x00;
 	while(1);
}
#endif
#if MR_STANDARD_MODE
/*
 * iic_ms_rw_mr_sm_test:master read, standard mode����׼ģʽ
 */
void iic_ms_rw_mr_sm_test()
{
	unsigned char cr = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0x5a;
	unsigned char transfer_cnt=0;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x07;
	IIC1_CDR = 0x07;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;


	IIC1_SDB = send_data;
	cr = IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
//	cr |= (transfer_cnt<<4);
	IIC0_CR = cr;
    cr = IIC0_CR;
 //   uart_printf("i2c0_master_read: IIC0_CR 2= %x\r\n", cr);	
	while((IIC0_SR&IIC_SR_PXF_MASK));

	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		uart_printf("i2c0_master_read: IIC0_SR = %x,IIC0_ESR = %x\r\n", IIC0_SR, IIC0_ESR);
	}
	recv_data = IIC0_MDB;
	uart_printf("i2c0_master_read: recv_data = %x,send_data = %x\r\n", recv_data,send_data);
	if(recv_data != send_data)
	{
		uart_printf("i2c0_master_read: IIC0_MDB = %x\r\n", recv_data);
		while(1);
	}
//	while(1);
}
#endif
#if MR_FAST_MODE
/*
 * iic_ms_rw_mr_fm_test:master read, fast mode������ģʽ
 */

void iic_ms_rw_mr_fm_test1()
{
	unsigned char cr = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0xa5;

	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC1_CDR = 0x03;
	IIC0_CDR = 0x03;
	IIC1_IER = 0x00;
	IIC0_IER = 0x00;

	IIC1_LMA = 0x5A;
	IIC1_HMA = 0xF6;
	IIC0_LSA = 0x5A;
	IIC0_HSA = 0xF6;

	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_SDB = send_data;
	cr = IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC1_CR = cr;

	while((IIC1_SR&IIC_SR_PXF_MASK));

	if(IIC1_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}
	recv_data = IIC1_MDB;
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);
	if(recv_data != send_data)
	{
		while(1);
	}
	while(1);
}
void iic_ms_rw_mr_fm_test()
{
	unsigned char cr = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0xa5;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC1_SDB = send_data;
	cr = IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));


	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		uart_printf("i2c0_master_read: IIC0_SR = %x,IIC0_ESR = %x\r\n", IIC0_SR, IIC0_ESR);
	}	
	/*
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}*/
	recv_data = IIC0_MDB;
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);
	if(recv_data != send_data)
	{
		while(1);
	}
	//while(1);
}
#endif
#if MR_ADR_7_BIT
/*
 * iic_ms_rw_mr_a7_test: 7bit��ַ����
 */
void iic_ms_rw_mr_a7_test()
{

	unsigned char cr = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0x5a;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;


	IIC0_CDR = 0x07;
	IIC1_CDR = 0x07;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC1_SDB = send_data;
	cr = IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));


	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		uart_printf("i2c0_master_read: IIC0_SR = %x,IIC0_ESR = %x\r\n", IIC0_SR, IIC0_ESR);
	}	
	/*
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}*/
	recv_data = IIC0_MDB;
#if 0
	IIC0_SR = 0x08;
	IIC0_MCR|=IIC_MCR_FMB_MASK;
	IIC1_MCR|=IIC_MCR_FSB_MASK;
	IIC1_SDB = 0xde;
	//cr = IIC_CR_RNW_MASK;
	//cr |= IIC_CR_PXF_MASK;
	//IIC0_CR = cr;
	IIC0_CR = IIC_CR_PXF_MASK;

	while((IIC0_SR&IIC_SR_PXF_MASK));
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}
	recv_data = IIC0_MDB;
#endif
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);
	if(recv_data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif
#if MR_ADR_10_BIT
/*
 * iic_ms_rw_mr_a10_test: 10bit��ַ����
 */
void iic_ms_rw_mr_a10_test()
{

	unsigned char cr = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0x5a;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;


	IIC0_CDR = 0x07;
	IIC1_CDR = 0x07;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC1_SDB = send_data;
	cr = IIC_CR_RNW_MASK;
	cr |= IIC_CR_AMC_MASK;
	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));


	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		uart_printf("i2c0_master_read: IIC0_SR = %x,IIC0_ESR = %x\r\n", IIC0_SR, IIC0_ESR);
	}	
/*	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}*/
	uart_printf("i2c0_master_read: i2c0 transfer cnt = %x, i2c1 transfer cnt = %x\r\n", IIC0_TCR,IIC1_TCR);
	
	recv_data = IIC0_MDB;
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);
	if(recv_data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif
#if MR_CNT_4_BYTE
/*
 * iic_ms_rw_mr_4b_test:4 byte��ݴ���
 */
void iic_ms_rw_mr_4b_test(unsigned int txData)
{
	unsigned char cr = 0;
	unsigned int data = 0x00;
	unsigned int recv_data[4] = {0};
	unsigned int send_data = txData;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;


	IIC0_CDR = 0x07;
	IIC1_CDR = 0x07;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC1_SDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC1_SDB = (unsigned char)(send_data>>16);
	IIC1_SDB = (unsigned char)(send_data>>8);
	IIC1_SDB = (unsigned char)(send_data>>0);

	cr = IIC_CR_RNW_MASK | IIC_CR_XFCL_MASK | IIC_CR_XFCM_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));

	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		uart_printf("i2c0_master_read: IIC0_SR = %x,IIC0_ESR = %x\r\n", IIC0_SR, IIC0_ESR);
	}	
	/*
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}*/
	recv_data[0] = IIC0_MDB;
	recv_data[1] = IIC0_MDB;
	recv_data[2] = IIC0_MDB;
	recv_data[3] = IIC0_MDB;

	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}
	//while(1);
}

void iic_ms_rw_mr_fm_4b_test(unsigned int txData)
{
	unsigned char cr = 0;
	unsigned int data = 0x00;
	unsigned int recv_data[4] = {0};
	unsigned int send_data = txData;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC1_SDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC1_SDB = (unsigned char)(send_data>>16);
	IIC1_SDB = (unsigned char)(send_data>>8);
	IIC1_SDB = (unsigned char)(send_data>>0);

	cr = IIC_CR_RNW_MASK | IIC_CR_XFCL_MASK | IIC_CR_XFCM_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));


	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		uart_printf("i2c0_master_read: IIC0_SR = %x,IIC0_ESR = %x\r\n", IIC0_SR, IIC0_ESR);
	}	
	/*
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}*/
	recv_data[0] = IIC0_MDB;
	recv_data[1] = IIC0_MDB;
	recv_data[2] = IIC0_MDB;
	recv_data[3] = IIC0_MDB;

	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif
#if MR_CHAIN_FUN
/*
 * iic_ms_rw_mr_cf_test: chain function ����
 * ˵������һ�δ���͵ڶ��δ���֮��û��stop�źţ�ͨ��ʾ�����۲�
 */
void iic_ms_rw_mr_cf_test()
{
	unsigned char cr = 0;
	unsigned char chain_s_data,chain_r_data;
	unsigned int data;
	unsigned int recv_data[4] = {0};
	unsigned int send_data = 0x12345678;
	unsigned char mcr0, mcr1;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	data = 0x00;
	chain_s_data = 0x5A;
	chain_r_data = 0x00;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0x00;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

//��һ����ݴ��䣺���ֽ�0x12345678
	IIC1_SDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC1_SDB = (unsigned char)(send_data>>16);
	IIC1_SDB = (unsigned char)(send_data>>8);
	IIC1_SDB = (unsigned char)(send_data>>0);
	cr = IIC_CR_RNW_MASK|IIC_CR_XFCL_MASK|IIC_CR_XFCM_MASK;
	cr |= IIC_CR_XFC_MASK | IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;

	while((IIC0_SR&IIC_SR_PXF_MASK));
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}
	recv_data[0] = IIC0_MDB;
	recv_data[1] = IIC0_MDB;
	recv_data[2] = IIC0_MDB;
	recv_data[3] = IIC0_MDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		COND_TRAP(31, 0, 0);
	}
#if 0
//�ڶ�����ݴ��䣺1�ֽ�0x5A
	IIC1_SDB = chain_s_data;
//	IIC1_ECSSR = 0xf0;
	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

//    cr = 0;
	cr = IIC_CR_RNW_MASK;
	cr |= IIC_CR_XFC_MASK | IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}

	chain_r_data = IIC0_MDB;
//	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", chain_r_data,chain_s_data);
	if(chain_s_data!=chain_r_data)
	{
		while(1);
	}
#endif
	
#if 0
//	�������ݴ��䣺1�ֽ�0x5A
	chain_s_data = 0xa5;
	IIC1_SDB = chain_s_data;
//	IIC1_ECSSR = 0xf0;
	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	cr = IIC_CR_RNW_MASK;
	cr |= IIC_CR_XFC_MASK | IIC_CR_PXF_MASK;
	IIC0_CR = cr;

	while((IIC0_SR&IIC_SR_PXF_MASK));
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}

	chain_r_data = IIC0_MDB;

	if(chain_s_data!=chain_r_data)
	{
		while(1);
	}

//	���Ĵ���ݴ��䣺1�ֽ�0x5A
	chain_s_data = 0x77;
	IIC1_SDB = chain_s_data;
//	IIC1_ECSSR = 0xf0;
//	IIC0_SR = IIC_SR_HLT_MASK;
//	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	cr = IIC_CR_RNW_MASK;
	cr |= IIC_CR_XFC_MASK | IIC_CR_PXF_MASK;
	IIC0_CR = cr;

	while((IIC0_SR&IIC_SR_PXF_MASK));
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}

	chain_r_data = IIC0_MDB;

	if(chain_s_data!=chain_r_data)
	{
		while(1);
	}
#else
//�������ݴ��䣺���ֽ�0x9abcdef0
	send_data = 0x9abcdef0;
	IIC1_SDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC1_SDB = (unsigned char)(send_data>>16);
	IIC1_SDB = (unsigned char)(send_data>>8);
	IIC1_SDB = (unsigned char)(send_data>>0);

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	cr = IIC_CR_RNW_MASK|IIC_CR_XFCL_MASK|IIC_CR_XFCM_MASK;
	cr |= IIC_CR_XFC_MASK | IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}
	recv_data[0] = IIC0_MDB;
	recv_data[1] = IIC0_MDB;
	recv_data[2] = IIC0_MDB;
	recv_data[3] = IIC0_MDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);
	
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", data,send_data);

	if(data != send_data)
	{
		while(1);
	}
#endif
//���Ĵ���ݴ��䣺���ֽ�0xa55a55aa
	send_data = 0xa55a55aa;
	IIC1_SDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC1_SDB = (unsigned char)(send_data>>16);
	IIC1_SDB = (unsigned char)(send_data>>8);
	IIC1_SDB = (unsigned char)(send_data>>0);

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	cr = IIC_CR_RNW_MASK|IIC_CR_XFCL_MASK|IIC_CR_XFCM_MASK;
	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	 cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));
	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}
	recv_data[0] = IIC0_MDB;
	recv_data[1] = IIC0_MDB;
	recv_data[2] = IIC0_MDB;
	recv_data[3] = IIC0_MDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);
	uart_printf("i2c0_master_read: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}
	//while(1);
}
#endif
#if MASTER_READ_OP
/*
 * iic_ms_rw_mr_test():master read ����
 */
void iic_ms_rw_mr_test()
{
	unsigned int i = 0;

#if MR_STANDARD_MODE
	iic_ms_rw_mr_sm_test();
#endif
#if MR_FAST_MODE
	iic_ms_rw_mr_fm_test();
#endif
#if MR_ADR_7_BIT
	iic_ms_rw_mr_a7_test();
#endif
#if MR_ADR_10_BIT
	iic_ms_rw_mr_a10_test();
#endif
#if MR_CNT_4_BYTE
	
	for(i = 0; i < 100; i++)
	{
		iic_ms_rw_mr_4b_test(0x12345678);
		iic_ms_rw_mr_4b_test(0x9abcdef0);
		iic_ms_rw_mr_4b_test(0xa55a55aa);
		iic_ms_rw_mr_fm_4b_test(0x12345678);
		iic_ms_rw_mr_fm_4b_test(0x9abcdef0);
		iic_ms_rw_mr_fm_4b_test(0xa55a55aa);
	}
#endif
#if MR_CHAIN_FUN
	iic_ms_rw_mr_cf_test();
#endif
//	while(1);
}
#endif

#if MW_STANDARD_MODE
/*
 * iic_ms_rw_mw_sm_test:standard mode ����
 */
void iic_ms_rw_mw_sm_test()
{
	unsigned char cr = 0, sr0 = 0, ext_sr0 = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0xa5;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x04;
	IIC1_CDR = 0x04;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_MDB = send_data;      //1-4�ֽ����
	cr &= ~IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		uart_printf("i2c0_master_write: IIC0_SR = %x,IIC0_ESR = %x\r\n", sr0,ext_sr0);			
		while(1);
	}
	recv_data = IIC1_SDB;
	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);

	if(recv_data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif
#if MW_FAST_MODE
/*
 * iic_ms_rw_mw_fm_test:fast mode ����
 */
void iic_ms_rw_mw_fm_test()
{
	unsigned char cr = 0, sr0 = 0, ext_sr0 = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0x52;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_MDB = send_data;      //1-4�ֽ����
	cr &= ~IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	recv_data = IIC1_SDB;
	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);

	if(recv_data != send_data)
	{
		while(1);
	}
//	while(1);
}

#endif
#if MW_ADR_7_BIT
/*
 * iic_ms_rw_mw_a7_test:7 bit����
 */
/*
void iic_ms_rw_mw_a7_test()
{
	unsigned char cr = 0, sr1 = 0, ext_sr1 = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0x29;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC1_LMA = 0x60;
	IIC1_HMA = 0xF6;
	IIC0_LSA = 0x5A;
	IIC0_HSA = 0xF6;

	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC1_MDB = send_data;      //1-4�ֽ����
	cr &= ~IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC1_CR = cr;

	do
	{
		sr1= IIC0_SR;
	}while(sr1 &IIC_SR_PXF_MASK);

	if(sr1 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr1 = IIC0_ESR;
		while(1);
	}
	recv_data = IIC0_SDB;

	if(recv_data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif
*/

void iic_ms_rw_mw_a7_test()
{
	unsigned char cr = 0, sr0 = 0, ext_sr0 = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0x29;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_MDB = send_data;      //1-4�ֽ����
	cr &= ~IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	recv_data = IIC1_SDB;

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);
	if(recv_data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif

#if MW_ADR_10_BIT
/*
 * iic_ms_rw_mw_a10_test:10 bit����
 */
void iic_ms_rw_mw_a10_test()
{
	unsigned char cr = 0, sr0 = 0, ext_sr0 = 0;
	unsigned char recv_data = 0x00;
	unsigned char send_data = 0x14;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_MDB = send_data;      //1-4�ֽ����
	cr &= ~IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK | IIC_CR_AMC_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	uart_printf("i2c0_master_read: i2c0 transfer cnt = %x, i2c1 transfer cnt = %x\r\n", IIC0_TCR,IIC1_TCR);
	recv_data = IIC1_SDB;

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", recv_data,send_data);
	if(recv_data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif
#if MW_CNT_4_BYTE
/*
 * iic_ms_rw_mw_4b_test:4 byte��ݲ���
 */
void iic_ms_rw_mw_4b_test(unsigned int txData)
{
	unsigned char cr = 0, sr0 = 0, sr1 = 0, ext_sr0 = 0, ext_sr1 = 0;
	unsigned int data = 0x00;
	unsigned int recv_data[4] = {0};
	unsigned int send_data = txData;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_MDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC0_MDB = (unsigned char)(send_data>>16);
	IIC0_MDB = (unsigned char)(send_data>>8);
	IIC0_MDB = (unsigned char)(send_data>>0);

	cr &= ~IIC_CR_RNW_MASK;
	cr |= IIC_CR_PXF_MASK | IIC_CR_XFCL_MASK | IIC_CR_XFCM_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
//	while((IIC0_SR&IIC_SR_PXF_MASK));
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}

	recv_data[0] = IIC1_SDB;
	recv_data[1] = IIC1_SDB;
	recv_data[2] = IIC1_SDB;
	recv_data[3] = IIC1_SDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}

	do
	{
		cr = IIC0_CR;
	}while(cr &IIC_CR_PXF_MASK);

	sr0 = IIC0_SR;
	sr1 = IIC1_SR;
//	while(1);
}

void iic_ms_rw_mw_4b_test2(void)
{
	unsigned char cr = 0, sr0 = 0, sr1 = 0, ext_sr0 = 0, ext_sr1 = 1;
	unsigned char mcr0, mcr1, ecssr0, ecssr1;
	unsigned int data = 0x00;
	unsigned int recv_data[4] = {0};
	unsigned int send_data = 0x9abcdef0;

#if 0
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.
#endif

	IIC0_SR = sr0 | IIC_SR_HLT_MASK;
	IIC1_SR = sr1 | IIC_SR_HLT_MASK;
#if 1
	ecssr0 = IIC0_ECSSR;
	ecssr1 = IIC1_ECSSR;

	IIC0_ECSSR = ecssr0 | IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = ecssr1 | IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
#endif
#if 1
	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0xAC;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0xAC;
	IIC1_HSA = 0xF6;
#endif
	sr0 = IIC0_SR;
	sr1 = IIC1_SR;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FSB_MASK | IIC_MCR_FMB_MASK;

	mcr0 = IIC0_MCR;
	mcr1 = IIC1_MCR;

	IIC0_MDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC0_MDB = (unsigned char)(send_data>>16);
	IIC0_MDB = (unsigned char)(send_data>>8);
	IIC0_MDB = (unsigned char)(send_data>>0);

	cr &= ~IIC_CR_RNW_MASK;
	cr |= IIC_CR_PXF_MASK|IIC_CR_XFCL_MASK|IIC_CR_XFCM_MASK;
	IIC0_CR = cr;

	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	recv_data[0] = IIC1_SDB;
	recv_data[1] = IIC1_SDB;
	recv_data[2] = IIC1_SDB;
	recv_data[3] = IIC1_SDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}
//	while(1);
}

void iic_ms_rw_mw_fm_4b_test(unsigned int txData)
{
	unsigned char cr = 0, sr0 = 0, ext_sr0 = 0;
	unsigned char recv_data[4] = {0};
	unsigned int send_data = txData, data;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SPMD_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_MDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC0_MDB = (unsigned char)(send_data>>16);
	IIC0_MDB = (unsigned char)(send_data>>8);
	IIC0_MDB = (unsigned char)(send_data>>0);
	cr &= ~IIC_CR_RNW_MASK;

	cr |= IIC_CR_PXF_MASK | IIC_CR_XFCL_MASK | IIC_CR_XFCM_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}

	recv_data[0] = IIC1_SDB;
	recv_data[1] = IIC1_SDB;
	recv_data[2] = IIC1_SDB;
	recv_data[3] = IIC1_SDB;
	data = (recv_data[0] << 24) | (recv_data[1] << 16) | (recv_data[2] << 8) | (recv_data[3]);

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}
//	while(1);
}
#endif
#if MW_CHAIN_FUN
/*
 * iic_ms_rw_mw_cf_test: chain function����
 * ˵������һ�δ���͵ڶ��δ���֮��û��stop�źţ�ͨ��ʾ�����۲�
 */
void iic_ms_rw_mw_cf_test()
{
	unsigned char cr = 0, sr0 = 0, ext_sr0 = 0;
	unsigned char chain_s_data,chain_r_data;
	unsigned int data;
	unsigned int recv_data[4] = {0};
	unsigned int send_data = 0x12345678;

	IIC0_SR = IIC_SR_HLT_MASK;
	IIC1_SR = IIC_SR_HLT_MASK;
	IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	data = 0;
	chain_s_data = 0x5A;
	chain_r_data = 0x00;

	IIC0_CDR = 0x03;
	IIC1_CDR = 0x03;
	IIC0_IER = 0x00;
	IIC1_IER = 0x00;

	IIC0_LMA = 0x5A;
	IIC0_HMA = 0xF6;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0xF6;

	IIC0_MCR = IIC_MCR_EUS_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = IIC_MCR_EUS_MASK | IIC_MCR_SMD_MASK | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

//��һ����ݴ���
	IIC0_MDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC0_MDB = (unsigned char)(send_data>>16);
	IIC0_MDB = (unsigned char)(send_data>>8);
	IIC0_MDB = (unsigned char)(send_data>>0);

	cr = IIC_CR_XFC_MASK | IIC_CR_PXF_MASK | IIC_CR_XFCL_MASK | IIC_CR_XFCM_MASK;
	cr &= ~IIC_CR_RNW_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	recv_data[0] = IIC1_SDB;
	recv_data[1] = IIC1_SDB;
	recv_data[2] = IIC1_SDB;
	recv_data[3] = IIC1_SDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}

//�ڶ�����ݴ���
	IIC0_MDB = chain_s_data;
	cr = 0;

	cr =  IIC_CR_XFC_MASK | IIC_CR_PXF_MASK ;
	cr &= ~IIC_CR_RNW_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	chain_r_data = IIC1_SDB;

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", chain_r_data,chain_s_data);
	if(chain_s_data!=chain_r_data)
	{
		while(1);
	}
//	while(1);

//�������ݴ���
	send_data = 0x9abcdef0;
	IIC0_MDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC0_MDB = (unsigned char)(send_data>>16);
	IIC0_MDB = (unsigned char)(send_data>>8);
	IIC0_MDB = (unsigned char)(send_data>>0);

	cr = IIC_CR_XFC_MASK | IIC_CR_PXF_MASK | IIC_CR_XFCL_MASK | IIC_CR_XFCM_MASK;
	cr &= ~IIC_CR_RNW_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	recv_data[0] = IIC1_SDB;
	recv_data[1] = IIC1_SDB;
	recv_data[2] = IIC1_SDB;
	recv_data[3] = IIC1_SDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}

//���Ĵ���ݴ���
	send_data = 0xa55a55aa;
	IIC0_MDB = (unsigned char)(send_data>>24);      //1-4�ֽ����
	IIC0_MDB = (unsigned char)(send_data>>16);
	IIC0_MDB = (unsigned char)(send_data>>8);
	IIC0_MDB = (unsigned char)(send_data>>0);

	cr = IIC_CR_PXF_MASK | IIC_CR_XFCL_MASK | IIC_CR_XFCM_MASK;
	cr &= ~IIC_CR_RNW_MASK;
	IIC0_CR = cr;
	cr = IIC0_CR;
	do
	{
		sr0 = IIC0_SR;
	}while(sr0 &IIC_SR_PXF_MASK);

	if(sr0 & 0x04)		//corresponding to write without stop signal
	{
		ext_sr0 = IIC0_ESR;
		while(1);
	}
	recv_data[0] = IIC1_SDB;
	recv_data[1] = IIC1_SDB;
	recv_data[2] = IIC1_SDB;
	recv_data[3] = IIC1_SDB;
	data = (recv_data[0]<<24)|(recv_data[1]<<16)|(recv_data[2]<<8)|(recv_data[3]);

	uart_printf("i2c0_master_write: recv_data = %x�� send_data = %x\r\n", data,send_data);
	if(data != send_data)
	{
		while(1);
	}
}
#endif
#if MASTER_WRITE_OP
/*
 * iic_ms_rw_mw_test: master write ����
 */
void iic_ms_rw_mw_test()
{
	unsigned int i = 0;

#if MW_STANDARD_MODE
	iic_ms_rw_mw_sm_test();
#endif
#if MW_FAST_MODE
	iic_ms_rw_mw_fm_test();
#endif
#if MW_ADR_7_BIT
	iic_ms_rw_mw_a7_test();
#endif
#if MW_ADR_10_BIT
	iic_ms_rw_mw_a10_test();
#endif
#if MW_CNT_4_BYTE
	for(i = 0; i < 100; i++)
	{
		iic_ms_rw_mw_4b_test(0x12345678);
		iic_ms_rw_mw_4b_test(0x9abcdef0);
		iic_ms_rw_mw_4b_test(0xa55a55aa);
		iic_ms_rw_mw_fm_4b_test(0x12345678);
		iic_ms_rw_mw_fm_4b_test(0x9abcdef0);
		iic_ms_rw_mw_fm_4b_test(0xa55a55aa);
	}
#endif
#if MW_CHAIN_FUN
	iic_ms_rw_mw_cf_test();
#endif
	//while(1);
}
#endif
#if IIC_MS_RW_TEST
/*
 * iic_ms_rw_test:Master/Slave��д����
 */
void iic_ms_rw_test()
{
	unsigned int i = 0;


#if 0
		IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
		IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

		IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
		IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.
#endif
#if MASTER_READ_OP
		iic_ms_rw_mr_test();
#endif

#if MASTER_WRITE_OP
		iic_ms_rw_mw_test();
#endif

}
#endif


/*
 * iic_isr
 */
void iic_isr()
{
	unsigned char sr  = 0;
	unsigned char esr = 0;
	unsigned char ecssr = 0;

#if IRQH_EN
	if(IIC0_IER & 0x08)
	{
		sr 	= IIC0_SR;
		if((sr&IIC_SR_HLT_MASK)==IIC_SR_HLT_MASK)
		{
			iic_irqh_int_flag = 1;
			//uart_printf_buf("Enter IRQH inter!\n");
			IIC0_SR = 0xff;
		}
	}
#endif

#if IRQI_EN
	esr = IIC0_ESR;
	if((esr&IIC_ESR_IXF_MASK)==IIC_ESR_IXF_MASK)
	{
		iic_irqi_int_flag = 1;
		//uart_printf_buf("Enter IRQI inter!\n");
	}
#endif

#if IRQA_EN
	esr = IIC0_ESR;
	if((esr&IIC_ESR_ANAK_MASK)==IIC_ESR_ANAK_MASK)
	{
		iic_irqa_int_flag = 1;
	//	uart_printf_buf("Enter IRQA inter!\n");
	}
#endif

#if IRQM_EN
	if(IIC0_IER & 0x01)
	{
		sr 	= IIC0_SR;
		if((sr&IIC_SR_HLT_MASK)==IIC_SR_HLT_MASK)
		{
			iic_irqm_int_flag = 1;
		//	uart_printf_buf("Enter IRQM inter!\n");
		}
	}
#endif

#if IRQSVC_EN
	ecssr = IIC1_ECSSR;
	if((ecssr&IIC_ECSSR_COMR_MASK)==IIC_ECSSR_COMR_MASK)
	{
		iic_irqsvc_int_flag = 1;
	//	uart_printf_buf("Enter IRQSVC inter!\n");
	}
#endif

#if IRQSVR_EN
	ecssr = IIC1_ECSSR;
	if((ecssr&IIC_ECSSR_SRVR_MASK)==IIC_ECSSR_SRVR_MASK)
	{
		iic_irqsvr_int_flag = 1;
		//uart_printf_buf("Enter IRQSVR inter!\n");
	}
#endif

#if IRQSW_EN
	ecssr = IIC1_ECSSR;
	if((ecssr&IIC_ECSSR_COMW_MASK)==IIC_ECSSR_COMW_MASK)
	{
		iic_irqsw_int_flag = 1;
		//uart_printf_buf("Enter IRQSW inter!\n");
	}
#endif

#if IRQSVW_EN
	ecssr = IIC1_ECSSR;
	if((ecssr&IIC_ECSSR_SRVW_MASK)==IIC_ECSSR_SRVW_MASK)
	{
		iic_irqsvw_int_flag = 1;
		//uart_printf_buf("Enter IRQSVW inter!\n");
	}
#endif

	IIC0_ECSSR = 0xf0;
	IIC1_ECSSR = 0xf0;
	IIC0_ESR = 0xf0;
	IIC1_ESR = 0xf0;
	IIC0_SR = 0xff;
	IIC1_SR = 0xff;
}
#if IIC_INT_TEST
/*
 * iic_int_test:�жϲ���
 */
void iic_int_test()
{
	unsigned char i;
	unsigned char cr = 0;
	unsigned char recv_data = 0;
	unsigned char send_data = 0xFF;

#if MASTER_INTER_EN
	InstallInterruptHandler0(IIC0_INT_NUM,iic_isr);
#if IRQH_EN
	iic_irqh_int_flag = 0;

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x08;
	IIC1_IER = 0x00;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x06;
	IIC1_MCR = 0x0A;

	IIC0_MDB = send_data;
	IIC0_CR = 0x01;
	cr = IIC0_CR;
	while(IIC0_SR&IIC_SR_PXF_MASK)
	{
		IIC0_CR |= IIC_CR_HMT_MASK;
		while(iic_irqh_int_flag==0);
		iic_irqh_int_flag=0;
	}

#endif

#if IRQI_EN
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.

	iic_irqi_int_flag = 0;
//	InstallInterruptHandler0(IIC0_INT_NUM,iic_isr);

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x04;
	IIC1_IER = 0x00;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x68;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x06;
	IIC1_MCR = 0x0A;

	IIC0_MDB = send_data;
	IIC0_CR = 0x01;
	cr = IIC0_CR;
	while(!iic_irqi_int_flag);
	iic_irqi_int_flag = 0;

#endif

#if 0
#if IRQA_EN
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.

	iic_irqa_int_flag = 0;
//	InstallInterruptHandler0(IIC0_INT_NUM,iic_isr);

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x02;
	IIC1_IER = 0x00;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x68;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x06;
	IIC1_MCR = 0x0A;

	IIC0_MDB = send_data;
	IIC0_CR = 0x01;

	while(!iic_irqa_int_flag);
	iic_irqa_int_flag = 0;
#endif
#endif

#if IRQM_EN
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.

	iic_irqm_int_flag = 0;
//	InstallInterruptHandler0(IIC0_INT_NUM,iic_isr);

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x01;
	IIC1_IER = 0x00;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x06 | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;
	IIC1_MCR = 0x0A | IIC_MCR_FMB_MASK | IIC_MCR_FSB_MASK;

	IIC0_MDB = send_data;
	IIC0_CR = 0x01;
	cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));

	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}

	while(!iic_irqm_int_flag);
	iic_irqm_int_flag = 0;

	recv_data = IIC1_SDB;
	if(recv_data != send_data)
	{
		while(1);
	}

#endif
#endif

#if SLAVE_INTER_EN
	InstallInterruptHandler0(IIC1_INT_NUM,iic_isr);
#if IRQSVC_EN
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.

	iic_irqsvc_int_flag = 0;

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x01;
	IIC1_IER = 0x80;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x02;
	IIC1_MCR = 0x0E;

	IIC1_SDB = send_data;
	IIC0_CR = 0x03;
	cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));

	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}

	while(iic_irqsvc_int_flag==0);
	iic_irqsvc_int_flag=0;

	recv_data = IIC0_MDB;
	if(recv_data != send_data)
	{
		while(1);
	}
#endif

#if IRQSVR_EN
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.

	iic_irqsvr_int_flag = 0;
//	InstallInterruptHandler0(IIC1_INT_NUM,iic_isr);

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x00;
	IIC1_IER = 0x40;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x02;
	IIC1_MCR = 0x0E;

	IIC1_SDB = send_data;

	IIC0_CR = 0x33;
	cr = IIC0_CR;
	while(!iic_irqsvr_int_flag);
	iic_irqsvr_int_flag = 0;

#endif

#if IRQSW_EN
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.

	iic_irqsw_int_flag = 0;
//	InstallInterruptHandler0(IIC1_INT_NUM,iic_isr);

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x01;
	IIC1_IER = 0x20;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x02;
	IIC1_MCR = 0x0E;

	IIC0_MDB = send_data;

	IIC0_CR = 0x01;
	cr = IIC0_CR;
	while((IIC0_SR&IIC_SR_PXF_MASK));

	if(IIC0_SR & 0x04)		//corresponding to write without stop signal
	{
		while(1);
	}

	while(!iic_irqsw_int_flag);
	iic_irqm_int_flag = 0;

	recv_data = IIC1_SDB;
	if(recv_data != send_data)
	{
		while(1);
	}
#endif

#if IRQSVW_EN
	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers
	IIC1_ECSSR &= ~IIC_ECSSR_SRST_MASK; //should be programmed before this register is programmed.

	iic_irqsvw_int_flag = 0;
//	InstallInterruptHandler0(IIC1_INT_NUM,iic_isr);

	IIC0_CDR = 0x02;
	IIC1_CDR = 0x02;
	IIC0_IER = 0x00;
	IIC1_IER = 0x10;
	IIC0_LMA = 0x5A;
	IIC0_HMA = 0x00;
	IIC1_LSA = 0x5A;
	IIC1_HSA = 0x00;

	IIC0_MCR = 0x02;
	IIC1_MCR = 0x0E;

	IIC0_MDB = send_data;
	for(i=0;i<4;i++)
	{
		IIC1_SDB = i+0xAA;
	}

	IIC0_CR = 0x01;
	cr = IIC0_CR;
	while(!iic_irqsvw_int_flag);
	iic_irqsvw_int_flag = 0;

#endif
#endif
}
#endif
/*
 * iic_write_one_byte:дһ���ֽ�
 */
unsigned char iic_write_one_byte(unsigned char *pbyte)
{
	int rc ;
	int Done_Polling ;
	unsigned char DataByte ;
	unsigned char IICstatus ;
	unsigned int Timeout_Count ;
	unsigned int MaxWaitTime ;

	rc = 0 ;
	Done_Polling = 0 ;
	Timeout_Count = 0 ;
	MaxWaitTime = 5000;

	DataByte = *pbyte ;        //DataByte is the byte to be written on the I2C bus
	IIC1_SR = 0x08;
	IIC1_MDB = DataByte;
	IIC1_CR = 0x01;

	while(!Done_Polling)
	{
		IICstatus = IIC1_SR;  //Read the Status register

		Timeout_Count++ ;
		if(!(IICstatus & 0x01))     //Look for the Pending Transfer bit to be a 0
		{
			Done_Polling = 1 ;       //The transfer is done, stop polling
		}
		if (Timeout_Count == MaxWaitTime)   //Check the amount of time we have waited
		{
			return(8) ;         //Time-out occurred. Exit immediately
		}
	}

	if(!(IICstatus == 0x08))  //Look for just the Halted or Stopped bit to be set
	{
		return(4) ;  //The status is wrong, exit
	}
	if(!(IIC1_TCR == 0x01))  //Look for one byte transferred by the master
	{
		return(4) ;  //The wrong number of bytes were sent, exit
	}
	return(rc);
}
/*
 * iic_rtc_driver_test:RTC��
 */

void i2c0_init()
{
#if 0
	MTREG(IIC0_CDR, 29);	//system clock for i2c is 300Mhz
	MTREG(IIC0_IER, 0x00);
	MTREG(IIC0_LMA, 0xa2);
	MTREG(IIC0_HMA, 0x00);
	MTREG(IIC0_LSA, 0x00);
	MTREG(IIC0_HSA, 0x00);
	MTREG(IIC0_MCR, 0x12);
#endif
	IIC0_CDR = 9;
	IIC0_IER = 0;
	IIC0_LMA = 0xa2;
	IIC0_HMA = 0x00;
	IIC0_LSA = 0x00;
	IIC0_HSA = 0x00;
	IIC0_MCR = 0x12;
}

void i2c0_master_write(uint8 byte_num, uint32 data)
{
	uint8 i;
	uint8 done_polling;
	uint16 time_cnt;
	uint8 stat_val;
	uint8 ctr_val;
	uint8 xfr_cnt;
	uint8 e_stat_val;
	uint8 mcr_val;
	uint32 max_wait_time;

	IIC0_SR = 0x08;

	mcr_val = IIC0_MCR;
	mcr_val |= (3 << 6);	//flush master&slave data buffer
	IIC0_MCR = mcr_val;

	for(i = 0; i < byte_num; i++)
	{
		IIC0_MDB = (uint8)((data >> i * 8) & 0xFF);
	}

	ctr_val = 0x01 + ((byte_num - 1) << 4);	//set PXF, XFCL, XFCM&BCL(disable stop signal) bits of CR
	ctr_val |= (1 << 3);
	IIC0_CR = ctr_val;

	done_polling = 0;
	time_cnt = 0;
	max_wait_time = 1000;

#if 1
	stat_val = IIC0_SR;
	e_stat_val = IIC0_ESR;
#endif
	while(!done_polling) 
	{
		time_cnt++;

		stat_val = IIC0_SR;
		if(!(stat_val & 0x01))
		{
			done_polling = 1;
		}
		if(time_cnt == max_wait_time)
		{
			uart_printf_buf("i2c0_master_write: Time out!\n");
			return;
		}
	}
#if 0
	if(!(stat_val & 0x08))	//corresponding to write with stop signal
	{
		e_stat_val = IIC0_ESR;
		uart_printf_buf("i2c0_master_write: Transfer completed with an error(%x, %x)!\n", stat_val, e_stat_val);
		return;
	}
#endif
	if(stat_val & 0x04)		//corresponding to write without stop sign
	{
		e_stat_val = IIC0_ESR;
		uart_printf("i2c0_master_write: Transfer completed with an error, not all was transferred(%x, %x)!\n", stat_val, e_stat_val);
		return;
	}
	
	xfr_cnt = IIC0_TCR;
	if(!(xfr_cnt == byte_num))
	{
		uart_printf("i2c0_master_write: The number of bytes sent is wrong(%x)!\n", xfr_cnt);
		return;
	}
//	uart_printf_buf("i2c0_master_write: Write data completed!\n");
}

uint8 tmpData = 0;
void i2c0_master_read(uint8 byte_num, uint32 data)
{
	uint8 i;
	uint8 done_polling;
	uint16 time_cnt;
	uint8 stat_val;
	uint8 ctr_val;
	uint8 xfr_cnt;
	uint8 rdDataVal;
	uint8 secondVal;
	uint8 e_stat_val;
	uint8 mcr_val;
	uint8 minuteVal;
	uint8 hourVal;
	uint32 max_wait_time;

	IIC0_SR = 0x08;
	
	mcr_val = IIC0_MCR;
	mcr_val |= (3 << 6);	//flush master&slave data buffer
	IIC0_MCR = mcr_val;
	
	ctr_val = IIC0_CR;
	ctr_val =  (ctr_val & 0xcf) | ((byte_num - 1) << 4);//|IIC_CR_XFC_MASK;		//set XFCL&XFCM bits of CR, i.e. bytes are in the requested master transfer.
	ctr_val |= ((1 << 1) + 1);	//set RNW(R)&PXF bits of CR
	ctr_val &= 0xf7;	//clear BCL of CR, enable stop signal;set BCL bit enable a restart signal
//	uart_printf_buf("ctr_val = %x\n", ctr_val);
	IIC0_CR = ctr_val;

	done_polling = 0;
	time_cnt = 0;
	max_wait_time = 1000;

	while(!done_polling) 
	{
		time_cnt++;

		stat_val = IIC0_SR;
		if(!(stat_val & 0x01))
		{
			done_polling = 1;
		}

		if(time_cnt == max_wait_time)
		{
			uart_printf_buf("i2c0_master_read: Time-out!\n");
			return;
		}
	}
#if 0	
	if(!(stat_val & 0x28))	//corresponding to write with stop signal 
	{
		e_stat_val = IIC0_ESR;
		uart_printf_buf("i2c0_master_write: Transfer completed with an error(%x, %x)!\n", stat_val, e_stat_val);
		return;
	}
#endif	    
	if(stat_val & 0x04)		//corresponding to write without stop signal
	{
		e_stat_val = IIC0_ESR;
		uart_printf("i2c0_master_write: Transfer completed with an error, not all was transferred(%x, %x)!\n", stat_val, e_stat_val);
		return;
	}
	
	xfr_cnt = IIC0_TCR;
	if(!(xfr_cnt == byte_num))
	{
		uart_printf("i2c0_master_write: The number of bytes sent is wrong(%x)!\n", xfr_cnt);
		return;
	}

	for(i = 0; i < byte_num - 1; i++)
	{
		rdDataVal = IIC0_MDB;

		secondVal = rdDataVal & 0x7f;
		if(tmpData != secondVal)
		{
			uart_printf("s:%x, raw:%x\n", secondVal, rdDataVal);
			tmpData = secondVal;

			if(!secondVal)
			{
				rdDataVal = IIC0_MDB;
				
				minuteVal = rdDataVal & 0x7f;
				uart_printf("m:%x\n", minuteVal);
			}
			else
			{
				rdDataVal = IIC0_MDB;
			}
		}
	}
/*
	IIC0_SR = IIC_SR_HLT_MASK;

		IIC0_ECSSR = IIC_ECSSR_COMR_MASK | IIC_ECSSR_SRVR_MASK | IIC_ECSSR_COMW_MASK | IIC_ECSSR_SRVW_MASK;

	//    cr = 0;
		ctr_val = IIC_CR_RNW_MASK;
		ctr_val |=  IIC_CR_PXF_MASK;
		IIC0_CR =ctr_val;

		while((IIC0_SR&IIC_SR_PXF_MASK));
		if(IIC0_SR & 0x04)		//corresponding to write without stop signal
		{
			while(1);
		}

		rdDataVal = IIC0_MDB;
		printf("rrr raw:%x\n", rdDataVal);
		*/
//	uart_printf_buf("i2c0_master_read: Read data completed\n.");
}

void iic_rtc_driver_test()
{
#if 0
	unsigned char recv_data ;
	unsigned char send_data ;
	unsigned char cr = 0;

	//iic init
	IIC1_CDR = 0x0E;
	IIC1_IER = 0x00;
	IIC1_LMA = 0xD0;        //RTC ADDRESS:11010000
	IIC1_HMA = 0x00;
	IIC1_LSA = 0x00;        //not used
	IIC1_HSA = 0x00;
	IIC1_MCR = IIC_MCR_EUS_MASK ;

	IIC1_CR = IIC_CR_PXF_MASK;
	//cr = iic_write_one_byte(0x55);
	while(1);
#endif

	IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
	IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;	//The clock divide register, lo and hi slave address registers

	i2c0_init();

	while(1)
	{
		i2c0_master_write(1, 0x0002);

		i2c0_master_read(2, 0xa2);
	}
}
/******************************************************************************/
/*
 * iic_dev_init:iic��ʼ��
 */
void iic_dev_init( )
{
//step 1: clear master address
	IIC1_LMA = 0xD0;
	IIC1_HMA = 0x00;
//step 2: clear slave address
	IIC1_LSA = 0x00;
	IIC1_HSA = 0x00;
//step 3: clear status,extended status
	IIC1_SR = IIC_SR_HLT_MASK | IIC_SR_IRQ_MASK;
	IIC1_ESR = IIC_ESR_ILLS_MASK|IIC_ESR_ILL_MASK|IIC_ESR_LARB_MASK|IIC_ESR_IXF_MASK|IIC_ESR_ANAK_MASK;
//step 4: set clock divider
	IIC1_CDR = 0x0E;
//step 5: clear transfer count
	IIC1_TCR = 0x00;
//step 6: clear extended control and status:
	IIC1_ECSSR = IIC_ECSSR_COMR_MASK|IIC_ECSSR_SRVR_MASK|IIC_ECSSR_COMW_MASK|IIC_ECSSR_SRVW_MASK;
//step 7: clear control register
	IIC1_CR = 0x00;
//step 8: disable/enable interrupts
	IIC1_IER = 0x00;
//step 9: set mode control register
	IIC1_MCR = IIC_MCR_EUS_MASK|IIC_MCR_FMB_MASK;
}
/*
 * undelay
 */
void undelay(unsigned int cnt)
{
	while(cnt--);
}
/*
 * iic_dev_rst:iic reset
 */
void iic_dev_rst( )
{
	unsigned char dc;
	unsigned int i;

	IIC1_ECSSR = IIC_ECSSR_SRST_MASK;

	for(i=0;i<100;i++)
	{
		dc = IIC1_DCR;
		if(!((dc&0x0f)==0x0f))
		{
			IIC1_DCR = IIC_DCR_CSDA_MASK|IIC_DCR_CSDL_MASK;

			for(i=0;i<100;i++)
			{
				dc = IIC1_DCR;
				if(((dc&0x0f)==0x0f))
					break;
	             /** Toggle SCL line */
	             dc ^= IIC_DCR_CSDL_MASK;
	             dc = IIC1_DCR;
	             undelay(10);
	             dc ^= IIC_DCR_CSDL_MASK;
	             dc = IIC1_DCR;
			}
		}
	}

	IIC0_ECSSR = 0x00;
	iic_dev_init();
}

void iic_flash_wt(int add ,int data)
{

	        IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
		    IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;
		    IIC0_CDR = 0x02;
			IIC0_IER = 0;
			IIC0_LMA = 0xa0;
			IIC0_HMA = 0x00;
			IIC0_LSA = 0x00;
			IIC0_HSA = 0x00;
			IIC0_MCR = 0x12;

			uint16 time_cnt;
			uint8 stat_val;
			uint8 ctr_val;
            uint8 mcr_val;

			unsigned int recv_data[4] = {0};

			IIC0_SR = 0x08;
			mcr_val = IIC0_MCR;
			mcr_val |= (3 << 6);	//flush master&slave data buffer
			IIC0_MCR = mcr_val;


			IIC0_MDB =0x09;
			IIC0_MDB =add;
			IIC0_MDB =data;

			ctr_val = 0x01 + ((2) << 4);	//set PXF, XFCL, XFCM&BCL(disable stop signal) bits of CR
			//ctr_val |= (1 << 3);
			IIC0_CR = ctr_val;

			if(IIC0_SR & 0x04)		//corresponding to write without stop signal
			{
			    while(1);
			}
            delay(5000);

}
				void iic_flash_rt()
				{

				    IIC0_ECSSR = IIC_ECSSR_SRST_MASK;
					IIC0_ECSSR &= ~IIC_ECSSR_SRST_MASK;
					IIC0_CDR = 0x02;
					IIC0_IER = 0;
					IIC0_LMA = 0xa0;
					IIC0_HMA = 0x00;
					IIC0_LSA = 0x00;
					IIC0_HSA = 0x00;
					IIC0_MCR = 0x12;

					uint16 time_cnt;
					uint8 stat_val;
					uint8 ctr_val;
                    uint8 mcr_val;

					unsigned int recv_data[4] = {0};
				    IIC0_SR = 0x08;

					mcr_val = IIC0_MCR;
					mcr_val |= (3 << 6);	//flush master&slave data buffer
					IIC0_MCR = mcr_val;

					ctr_val = IIC0_CR;
					ctr_val =  (ctr_val & 0xcf) | ((0) << 4);		//set XFCL&XFCM bits of CR, i.e. bytes are in the requested master transfer.
					ctr_val |= ((1 << 1) + 1);	//set RNW(R)&PXF bits of CR
					ctr_val &= 0xf7;	//clear BCL of CR, enable stop signal;set BCL bit enable a restart signal
				//	uart_printf_buf("ctr_val = %x\n", ctr_val);
					IIC0_CR = ctr_val;
					while((IIC0_SR&IIC_SR_PXF_MASK));
					if(IIC0_SR & 0x04)		//corresponding to write without stop signal
						{
						while(1);
						}
					recv_data[0] = IIC0_MDB;
				//	rdDataVal = IIC0_MDB;

					printf("rdDataVal=%d\n",recv_data[0]);
	}

/******************************************************************************/

void iic_test(void)
{
//	ClrTLB(IIC_TLB_BAS, TLB0_SIZE_4K);
//	SetTLB(0,IIC_TLB_BAS,TLB0_SIZE_4K,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,IIC_TLB_BAS|0x00000022);
#if IIC_MS_RW_TEST    // 1
	IIC0_DCR = 0xf;

//	uart_printf("IIC0_DCR = %x\r\n",IIC0_DCR);			yww
//	i2c0_init();
//	i2c0_master_read(1,0x11);
//	i2c0_master_read(1,0x22);
	
	
	iic_ms_rw_test();
#endif

#if IIC_INT_TEST
	unsigned int i,u;
	
	  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	
	  for(i=MPIC_IIVPR33;i<=MPIC_IIVPR36;i=i+0x20) {       //umask iic int
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  }
	iic_int_test();
#endif
	//while(1);
}
#endif
