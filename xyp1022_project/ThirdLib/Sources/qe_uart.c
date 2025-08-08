/*
 * qe_uart.c
 *
 *  Created on: 2021Äê12ÔÂ31ÈÕ
 *      Author: jshi
 */
//#include "iomacros.h"
#include "qe_uart.h"
#define IO_WRITE32(address, data) 			*(volatile unsigned int *)(address) = data
#define r32(address) 				*(volatile unsigned int *)(address)

#define IO_WRITE16(address, data) 			*(volatile unsigned short *)(address) = data
#define r16(address) 				*(volatile unsigned short *)(address)

#define IO_WRITE8(address, data) 			*(volatile unsigned char *)(address) = data
#define r8(address) 				*(volatile unsigned char *)(address)


qe_uart_regs *p_uart_regs = (qe_uart_regs *)QE_UART_BASE;
//qe_uart_regs *p_uart_regs = (qe_uart_regs *)0xe95a0000;

#define CCM_IO_MULTIPLEX		(0xEB009000+0x28)

void qe_uart_pinio_init(void)
{
	IO_WRITE32(CCM_IO_MULTIPLEX,r32(CCM_IO_MULTIPLEX)|(1<<17)|(1<<16));//CCM SWAP QUICC UART
	
	//UART1
	IO_WRITE32(QE_QER_ADDR_PE_UCC_PC,0x0000000f);//other PC[0-3] = UCC MODDE
	IO_WRITE32(QE_QER_ADDR_UART_SEL,0x00000001);//uart change to HTU ver
	IO_WRITE32(QE_QER_ADDR_PU_PD_UART,0x0000aaa2);//cts pull down(no, long x,try no pu/pd)
	//IO_WRITE32(QE_QER_ADDR_PU_PD_UART,0x0000aaa6);//cts pull down(no, long x,try no pu/pd)
	//IO_WRITE32(QE_QER_ADDR_PU_PD_UART,0x0000aaaa);//cts pull down(no, long x,try no pu/pd)

//	IO_WRITE32(QE_QER_ADDR_DATAINV_UART,0x00000000);
//	
	IO_WRITE32(QE_QER_ADDR_DATAINV_UART,0x00000002);
	
	IO_WRITE32(QE_QER_ADDR_INTR_MASK_R0,0x00000000);//disbale intr for post sim

	//UART2
	IO_WRITE32(QE_QER_ADDR_PE_UCC_PD,0xF0000000);//other PD[0-3] = UCC MODDE
	IO_WRITE32(QE_QER_ADDR_UART_SEL,0x00000003);//uart change to HTU ver
	IO_WRITE32(QE_QER_ADDR_PU_PD_UART,0x0000a2a2);//cts pull down(no, long x,try no pu/pd)
	
	IO_WRITE32(QE_QER_ADDR_DATAINV_UART,0x00000022);
}

void qe_uart_init(void)
{
	unsigned int reg;
//	p_uart_regs->mcr = 0x1817fc43;
//	p_uart_regs->mcr = 0x18075c43;
	p_uart_regs->mcr = 0x01025c43;
	reg = p_uart_regs->mcr;

	//p_uart_regs->mcr = 0x98175c43;
//	p_uart_regs->mcr = 0x80025c43;//normal
	p_uart_regs->mcr = 0x80025cc3;//loopback
	reg = p_uart_regs->mcr;
	reg = p_uart_regs->mcr;
	reg = p_uart_regs->lsr;//clear E bit
}

unsigned char tx_data[]=
{
	0x30,
	0x31,
	0x4f,
};


#if 0

void qe_uart_test(void)
{
	unsigned int tx_cnt = 0;
	unsigned int rx_cnt = 0;
	unsigned int data_cnt = 0;

	qe_uart_pinio_init();
	qe_uart_init();

	data_cnt = sizeof(tx_data)/sizeof(unsigned char);
	while(1)
	{
		
		//transmit
		if(   ( (QE_UART_LSR_TXTHF_MASK)&(p_uart_regs->lsr) )
			&&( tx_cnt < data_cnt ) )
		{
			p_uart_regs->fifo.thr = tx_data[tx_cnt];
			tx_cnt += 1;
		}

		delay(0x1000);
		
		//receive
		if( (QE_UART_LSR_DRDY_MASK)&(p_uart_regs->lsr) )
		{
			if( p_uart_regs->fifo.rbr != tx_data[rx_cnt] )
			{
				while(1);//failed
			}
			rx_cnt += 1;
		}

		//complete
		if(rx_cnt >= data_cnt )
		{
			break;
		}
	}

}

#else


void qe_uart_test(void)
{
	unsigned int tx_cnt = 0;
	unsigned int rx_cnt = 0;
	unsigned int data_cnt = 0;

	qe_uart_pinio_init();
	
	extern int quicc_uart_testmain(void);
	
	quicc_uart_testmain();

}

#endif

