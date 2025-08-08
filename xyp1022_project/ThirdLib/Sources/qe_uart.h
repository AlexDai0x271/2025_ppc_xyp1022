/*
 * qe_uart.h
 *
 *  Created on: 2021Äê12ÔÂ31ÈÕ
 *      Author: jshi
 */

#ifndef QE_UART_H_
#define QE_UART_H_

/*******************************************************************/

#define	QE_QER_BASE_ADDR			(0xe9580000+0x3c000)

#define QE_QER_ADDR_UART_SEL		(QE_QER_BASE_ADDR+0x44)
#define QE_QER_ADDR_PE_UCC_PC		(QE_QER_BASE_ADDR+0x58)
#define QE_QER_ADDR_PE_UCC_PD		(QE_QER_BASE_ADDR+0x5C)
#define QE_QER_ADDR_PU_PD_UART		(QE_QER_BASE_ADDR+0x19c)
#define QE_QER_ADDR_DATAINV_UART		(QE_QER_BASE_ADDR+0x1A0)
#define QE_QER_ADDR_INTR_MASK_R0	(QE_QER_BASE_ADDR+0x9c)

/*******************************************************************/

/*******************************************************************/
#define QE_UART_BASE				(0xe9580000 + 0x3b400 + (0x100 * 0x2))

typedef struct _qe_uart_regs{
	volatile unsigned int mcr;//offset:0x00
	volatile unsigned int lsr;//offset:0x04
	union			  	      //offset:0x08
	{
		volatile unsigned char rbr;
		volatile unsigned char thr;
	}fifo;
}qe_uart_regs;

#define QE_UART_LSR_TXTHF_POS			(9)
#define QE_UART_LSR_TXTHF_MASK			(0x1<<QE_UART_LSR_TXTHF_POS)

#define QE_UART_LSR_DRDY_POS			(0)
#define QE_UART_LSR_DRDY_MASK			(0x1<<QE_UART_LSR_DRDY_POS)

/*******************************************************************/

#endif /* QE_UART_H_ */
