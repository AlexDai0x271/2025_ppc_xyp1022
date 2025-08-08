#include <stdarg.h>
//#include "as_archppc470.h"
//#include "ppclib.h"
#include "uart.h"
#include "e500mc_asm.h"
#include "mpic.h"


char uart0_16750_RX( void)
{
	while((UART0_LSR & UART_LSR_RXDR_IND) == 0);
	return UART0_RBR;
}

void UART0_INT_HANDLER()
{
	int i = 0;
	char RXBuf[64];
	for(i = 0; i < 64; i++)
	{
		RXBuf[i] = uart0_16750_RX();
		UART0_THR = RXBuf[i];
	}
#if 0
	unsigned char iir, rbr, msr, lsr;
	int i;

	iir = UART0_IIR;
	//do
	//{
		if((iir & 0x0F) == UART_IIR_RXDA_INT_MASK)
		{
			lsr = UART0_LSR;
			for(i=0;i<7;i++)
				rbr = UART0_RBR;
			if(lsr & UART_LSR_RXDR_IND)
			{
				//COMMENT("An Receive data ready intr!");
			}
			UART0_IER = UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
			for(i=0;i<30;i++)
				UART0_THR = i;
		}

		if((iir & 0x0F) == UART_IIR_RXLS_INT_MASK)
		{
			lsr = UART0_LSR;
			//COMMENT("An Receiver line state interrupt!");
			if(lsr & UART_LSR_OVRUN_ERR_IND)
			{
				//COMMENT("An Over run intr!");
			}
			if(lsr & UART_LSR_PAR_ERR_IND)
			{
				//COMMENT("An Parity intr!");
			}
			if(lsr & UART_LSR_FRAM_ERR_IND)
			{
				//COMMENT("An Frame intr!");
			}
			if(lsr & UART_LSR_RX_BREAK_INT)
			{
				//COMMENT("An Break intr!");
			}
			if(lsr & UART_LSR_TX_HOLDREG_EMP)
			{
				//COMMENT("An Hold reg empty intr!");
			}
			if(lsr & UART_LSR_TXEMP)
			{
				//COMMENT("An empty intr!");
			}
			if(lsr & UART_LSR_RXFIFO_ERR)
			{
				//COMMENT("An Fifo intr!");
			}
		}

		if((iir & 0x0F) == UART_IIR_CHTOUT_INT_MASK)
		{
			//COMMENT("AUTO Time out interrupt!");
		}

		if((iir & 0x0F) == UART_IIR_THREMP_INT_MASK)
		{
			//COMMENT("UART0 Transmiter holding register empty!");
		}

		if((iir & 0x0F) == UART_IIR_MS_INT_MASK)
		{
			msr = UART0_MSR;
			//COMMENT("UART0 Modem status interrupts!");
			if(msr & UART_MSR_DDCD)
			{
				//COMMENT("An Detect DCD intr!");
			}
			if(msr & UART_MSR_TERI)
			{
				//COMMENT("An Detect RI  intr!");
			}
			if(msr & UART_MSR_DDSR)
			{
				//COMMENT("An Detect DSR intr!");
			}
			if(msr & UART_MSR_DCTS)
			{
				//COMMENT("An Detect CTS intr!");
			}
			if(msr & (UART_MSR_DDCD | UART_MSR_TERI | UART_MSR_DDSR | UART_MSR_DCTS))
			{

			}
		}
		iir = UART0_IIR;
	//}while(!(iir & UART_IIR_NO_PENDING_INT));
#endif
}
void uart1_16750_init(int xtalClk, int baudrate)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	// install the int handler
//	InstallInterruptHandler0(70,UART0_INT_HANDLER);
//	InstallInterruptHandler0(71,UART1_INT_HANDLER);

	//enable write to dll,diab=1
	UART1_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = xtalClk / (16 * baudrate);
//	dl = xtalClk / (16 * 9600);
	if(dl <= 0)
	{
		dl = 1;
	}
	UART1_DLL = (dl & 0xff);
	UART1_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	//UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	UART1_LCR =0x03;
	//disable fifo
	//UART0_FCR = 0x00;
	UART1_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_RE_RXFIFO_MASK | UART_FCR_RE_TXFIFO_MASK;
	//enable int
	UART1_IER = UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;

	UART1_MCR = UART_MCR_OUT2N_CLEAR | UART_MCR_OUT1N_CLEAR | UART_MCR_RTSN_CLEAR | UART_MCR_DTRN_CLEAR;
	//read lsr msr to clear any error;
	tmp = UART1_DLL;
	tmp = UART1_RBR;
	tmp = UART1_RBR;
	tmp = UART1_RBR;
	tmp = UART1_RBR;
	tmp = UART1_LSR;
	tmp = UART1_MSR;
	
}


//The UART1 interrupt handler
void UART1_INT_HANDLER()
{

}

void uart0_16750_init(int xtalClk, int baudrate)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	// install the int handler
//	InstallInterruptHandler0(121,UART0_INT_HANDLER);
//	InstallInterruptHandler0(71,UART1_INT_HANDLER);

	//enable write to dll,diab=1
	UART0_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = xtalClk / (16 * baudrate);
//	dl = xtalClk / (16 * 9600);
	if(dl <= 0)
	{
		dl = 1;
	}
	UART0_DLL = (dl & 0xff);
	UART0_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	//UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	UART0_LCR =0x03;
	//disable fifo
	//UART0_FCR = 0x00;
	UART0_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_RE_RXFIFO_MASK | UART_FCR_RE_TXFIFO_MASK;
	//enable int  所有中断全开
	//UART0_IER = UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
	UART0_IER = 0;
	UART0_MCR = UART_MCR_OUT2N_CLEAR | UART_MCR_OUT1N_CLEAR | UART_MCR_RTSN_CLEAR | UART_MCR_DTRN_CLEAR;
	//read lsr msr to clear any error;
	tmp = UART0_DLL;
	tmp = UART0_RBR;
	tmp = UART0_RBR;
	tmp = UART0_RBR;
	tmp = UART0_RBR;
	tmp = UART0_LSR;
	tmp = UART0_MSR;
	
}

void uart3_16750_init(int xtalClk, int baudrate)
{
	unsigned short dl = 0;
	unsigned char tmp;

	// install the int handler
	//InstallInterruptHandler0(70,UART0_INT_HANDLER);
	//InstallInterruptHandler0(71,UART1_INT_HANDLER);

	//enable write to dll,diab=1
	UART3_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = xtalClk / (16 * baudrate);
//	dl = xtalClk / (16 * 9600);
	if(dl <= 0)
	{
		dl = 1;
	}
	UART3_DLL = (dl & 0xff);
	UART3_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	//UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	UART3_LCR =0x03;
	//disable fifo
	//UART0_FCR = 0x00;
	UART3_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_RE_RXFIFO_MASK | UART_FCR_RE_TXFIFO_MASK;
	//enable int
	//UART0_IER = UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;

	UART3_MCR = UART_MCR_OUT2N_CLEAR | UART_MCR_OUT1N_CLEAR | UART_MCR_RTSN_CLEAR | UART_MCR_DTRN_CLEAR;
	//read lsr msr to clear any error;
	tmp = UART3_RBR;
	tmp = UART3_LSR;
	tmp = UART3_MSR;

}

void uart2_16750_init(int xtalClk, int baudrate)
{
	unsigned short dl = 0;
	unsigned char tmp;

	// install the int handler
	//InstallInterruptHandler0(70,UART0_INT_HANDLER);
	//InstallInterruptHandler0(71,UART1_INT_HANDLER);

	//enable write to dll,diab=1
	UART2_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = xtalClk / (16 * baudrate);
//	dl = xtalClk / (16 * 9600);
	if(dl <= 0)
	{
		dl = 1;
	}
	UART2_DLL = (dl & 0xff);
	UART2_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	//UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	UART2_LCR =0x03;
	//disable fifo
	//UART0_FCR = 0x00;
	UART2_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_RE_RXFIFO_MASK | UART_FCR_RE_TXFIFO_MASK;
	//enable int
	//UART0_IER = UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;

	UART2_MCR = UART_MCR_OUT2N_CLEAR | UART_MCR_OUT1N_CLEAR | UART_MCR_RTSN_CLEAR | UART_MCR_DTRN_CLEAR;
	//read lsr msr to clear any error;
	tmp = UART2_RBR;
	tmp = UART2_LSR;
	tmp = UART2_MSR;

}










void uart1_16750_TX(char TXData)
{
	//while((UART0_LSR & UART_LSR_TX_HOLDREG_EMP) == 0);
	while((UART1_LSR & UART_LSR_TXEMP) == 0);
	UART1_THR = TXData;
}

void uart0_16750_TX(char TXData)
{
	//while((UART0_LSR & UART_LSR_TX_HOLDREG_EMP) == 0);
	while((UART0_LSR & UART_LSR_TXEMP) == 0);
	UART0_THR = TXData;
}
char uart1_16750_RX( void)
{
	while((UART1_LSR & UART_LSR_RXDR_IND) == 0);
	return UART1_RBR;
}



void uart3_16750_TX(char TXData)
{
	//while((UART0_LSR & UART_LSR_TX_HOLDREG_EMP) == 0);
	while((UART3_LSR & UART_LSR_TXEMP) == 0);
	UART3_THR = TXData;
}

char uart3_16750_RX( void)
{
	while((UART3_LSR & UART_LSR_RXDR_IND) == 0);
	return UART3_RBR;
}




void uart2_16750_TX(char TXData)
{
	//while((UART0_LSR & UART_LSR_TX_HOLDREG_EMP) == 0);
	while((UART2_LSR & UART_LSR_TXEMP) == 0);
	UART2_THR = TXData;
}

char uart2_16750_RX( void)
{
	while((UART2_LSR & UART_LSR_RXDR_IND) == 0);
	return UART2_RBR;
}




//return == 1; rec data ok
//return == 0; rec data timeout
char uart0_16750_RX_time( volatile char *dataAddr, int cpuClk)
{
	int i = 0;

	for(i = 0; i < (cpuClk/4); i++)
	{
		if((UART0_LSR & UART_LSR_RXDR_IND) == UART_LSR_RXDR_IND)
		{
				*(volatile char *)dataAddr = UART0_RBR;
				return 1;
		}
	}

	return 0;
}


//uart0和uart1对接，回环测试
//uart0发 uart1收
void uart_test1(void)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	unsigned short readdata[64],senddata[64];
	int i = 0;
	long j = 0;
	//Set TLB
	//SetTLB(0,UART_TLB_BAS,TLB0_SIZE_4K,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,UART_TLB_BAS|0x00000052);
	ISYNC;
	//set uart0 in loop mode
	UART0_MCR = 0;
	//enable write to dll,diab=1
	UART0_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = UART_XTAL / (16 * 115200);
	UART0_DLL = (dl & 0xff);
	UART0_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	//disable fifo
	//UART0_FCR = 0x00;
	UART0_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_8OR32B_TGL_MASK | UART_FCR_RE_TXFIFO_MASK | UART_FCR_RE_RXFIFO_MASK;
	//enable int
	UART0_IER = 0x00;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
	//read lsr msr to clear any error;
	tmp = UART0_LSR;
	tmp = UART0_MSR;

	//set uart1 in loop mode
	UART2_MCR = 0;
	//enable write to dll,diab=1
	UART2_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	UART2_DLL = (dl & 0xff);
	UART2_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	UART2_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	//disable fifo
	//UART1_FCR = 0x00;
	UART2_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_8OR32B_TGL_MASK | UART_FCR_RE_TXFIFO_MASK | UART_FCR_RE_RXFIFO_MASK;
	//enable int
	UART2_IER = 0x00;
	//UART1_IER = UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
	//read lsr msr to clear any error;
	tmp = UART2_LSR;
	tmp = UART2_MSR;

	for(i=0;i<64;i++)
	{
		senddata[i]=i;
		readdata[i]=0x0;
	}
	for(i=0;i<64;i++)
	{
		while((UART0_LSR & UART_LSR_TX_HOLDREG_EMP) != 0x20);
		uart0_16750_TX(senddata[i]);
//		while((UART1_LSR & UART_LSR_RXDR_IND) != 0x0);
		readdata[i]=uart2_16750_RX();
		while(readdata[i] !=senddata[i]);		
	}
//	while(1);
}
//uart1发 uart0收
void uart_test2(void)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	unsigned short readdata[64],senddata[64];
	int i = 0;
	long j = 0;
	//Set TLB
	//SetTLB(0,UART_TLB_BAS,TLB0_SIZE_4K,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,UART_TLB_BAS|0x00000052);
	ISYNC;
	//set uart0 in loop mode
	UART0_MCR = 0;
	//enable write to dll,diab=1
	UART0_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = UART_XTAL / (16 * 115200);
	UART0_DLL = (dl & 0xff);
	UART0_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	//disable fifo
	//UART0_FCR = 0x00;
	UART0_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_8OR32B_TGL_MASK | UART_FCR_RE_TXFIFO_MASK | UART_FCR_RE_RXFIFO_MASK;
	//enable int
	UART0_IER = 0x00;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
	//read lsr msr to clear any error;
	tmp = UART0_LSR;
	tmp = UART0_MSR;

	//set uart1 in loop mode
	UART2_MCR = 0;
	//enable write to dll,diab=1
	UART2_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	UART2_DLL = (dl & 0xff);
	UART2_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	UART2_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	//disable fifo
	//UART1_FCR = 0x00;
	UART2_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_8OR32B_TGL_MASK | UART_FCR_RE_TXFIFO_MASK | UART_FCR_RE_RXFIFO_MASK;
	//enable int
	UART2_IER = 0x00;
	//UART1_IER = UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
	//read lsr msr to clear any error;
	tmp = UART2_LSR;
	tmp = UART2_MSR;

	for(i=0;i<64;i++)
	{
		senddata[i]=i;
		readdata[i]=0x0;
	}
	for(i=0;i<64;i++)
	{
		while((UART2_LSR & UART_LSR_TX_HOLDREG_EMP) != 0x20);
		uart2_16750_TX(senddata[i]);
//		while((UART1_LSR & UART_LSR_RXDR_IND) != 0x0);
		readdata[i]=uart0_16750_RX();
		while(readdata[i] !=senddata[i]);		
	}
//	while(1);
}

void uart_test3(void)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	unsigned short readdata[64],senddata[64];
	int i = 0;
	long j = 0;
	//Set TLB
	//SetTLB(0,UART_TLB_BAS,TLB0_SIZE_4K,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,UART_TLB_BAS|0x00000052);
	ISYNC;
	//set uart0 in loop mode
	

	for(i=0;i<64;i++)
	{
		senddata[i]=i;
		readdata[i]=0x0;
	}
	for(i=0;i<64;i++)
	{
		while((UART1_LSR & UART_LSR_TX_HOLDREG_EMP) != 0x20);
		uart1_16750_TX(senddata[i]);
//		while((UART1_LSR & UART_LSR_RXDR_IND) != 0x0);
		readdata[i]=uart3_16750_RX();
		while(readdata[i] !=senddata[i]);		
	}
//	while(1);
}




void uart_test4(void)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	unsigned short readdata[64],senddata[64];
	int i = 0;
	long j = 0;
	//Set TLB
	//SetTLB(0,UART_TLB_BAS,TLB0_SIZE_4K,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,UART_TLB_BAS|0x00000052);
	ISYNC;
	//set uart0 in loop mode
	

	for(i=0;i<64;i++)
	{
		senddata[i]=i;
		readdata[i]=0x0;
	}
	for(i=0;i<64;i++)
	{
		while((UART3_LSR & UART_LSR_TX_HOLDREG_EMP) != 0x20);
		uart3_16750_TX(senddata[i]);
//		while((UART1_LSR & UART_LSR_RXDR_IND) != 0x0);
		readdata[i]=uart1_16750_RX();
		while(readdata[i] !=senddata[i]);		
	}
//	while(1);
}



void uart0_16750_TX_test( void)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	unsigned short readdata = 0;
	int i = 0;
	long j = 0;

	//Set TLB
//	SetTLB(0,UART_TLB_BAS,TLB0_SIZE_4K,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,UART_TLB_BAS|0x00000052);
	ISYNC;
	//set uart0 in loop mode
	UART0_MCR = 0;
	//enable write to dll,diab=1
	UART0_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = UART_XTAL / (16 * 9600);
	UART0_DLL = (dl & 0xff);
	UART0_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	//disable fifo
	UART0_FCR = 0x00;
	//UART0_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_8OR32B_TGL_MASK;
	//enable int
	UART0_IER = 0x00;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
	//read lsr msr to clear any error;
	tmp = UART0_LSR;
	tmp = UART0_MSR;

	i = 0;
//	while(1)
	{
		uart0_16750_TX(0x55);
		//while((UART0_LSR & UART_LSR_TX_HOLDREG_EMP) == 0);
		for(j = 0; j < 0x1000; j++);
		readdata = uart0_16750_RX();
		while((readdata-0x55)!=0);
	}
	while(1);
}

void uart0_16750_RX_test( void)
{
	unsigned short dl = 0;
	unsigned char tmp = 0;
	int i = 0;
	int j = 0;
	char RXBuf[64];

	//Set TLB
//	SetTLB(0,UART_TLB_BAS,TLB0_SIZE_4K,TLB2_U_RW|TLB2_S_RW|TLB2_I_MASK,UART_TLB_BAS|0x00000052);
	ISYNC;
	//set uart0 in loop mode
	UART0_MCR = 0;
	//enable write to dll,diab=1
	UART0_LCR = UART_LCR_DLAB_EN_MASK;
	//set baudrate
	//baud rate(bps) = XTAL CLOCK / (16 * divisor)
	dl = UART_XTAL / (16 * 9600);
	UART0_DLL = (dl & 0xff);
	UART0_DLM = (dl >> 8);
	//8bit data; 1stop bit;no check
	UART0_LCR = UART_LCR_8BIT_CHAR_LENGTH;
	//disable fifo
	UART0_FCR = 0x00;
	//UART0_FCR = UART_FCR_EN_BFIFO_MASK | UART_FCR_8OR32B_TGL_MASK;
	//enable int
	UART0_IER = 0x00;//UART_IER_ERDAI_MASK | UART_IER_EMSI_MASK | UART_IER_ERLSI_MASK | UART_IER_ETHREI_MASK;
	//read lsr msr to clear any error;
	tmp = UART0_LSR;
	tmp = UART0_MSR;
//	while((UART0_LSR & UART_LSR_RXDR_IND) == 0);
	for(i = 0; i < 64; i++)
	{
		RXBuf[i] = uart0_16750_RX();
		UART0_THR = RXBuf[i];
	}
}


void uart0_16750_RXINT_test( void)
{
	int i = 0;
	int u = 0;
	char RXBuf[64];

	  for(i=MPIC_IILR119;i<=MPIC_IILR121;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	
	  for(i=MPIC_IIVPR119;i<=MPIC_IIVPR121;i=i+0x20) {       //umask uart int
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  }
	  
	  
	InstallInterruptHandler0(121,UART0_INT_HANDLER);
	uart0_16750_init(UART_XTAL, 115200);
	UART0_IER = UART_IER_ERDAI_MASK;//enable receive data available interrupt

	//for(i = 0; i < 64; i++)
	//{
	//	uart0_16750_TX(0x55);
	//}

	//for(i = 0; i < 64; i++)
	//{
	//	RXBuf[i] = uart0_16750_RX();
	//}
	while(1);
}

void uart_printf_buf( char * buf)
{
	int i = 0;

	while(buf[i] != '\0')
	{
		if(buf[i] == '\n')
		{
			uart0_16750_TX('\r');
		}
		uart0_16750_TX(buf[i]);
		i++;
	}
}
void uart1_printf_buf( char * buf)
{
	int i = 0;

	while(buf[i] != '\0')
	{
		if(buf[i] == '\n')
		{
			uart1_16750_TX('\r');
		}
		uart1_16750_TX(buf[i]);
		i++;
	}
}
char printbuffer[256];

int uart_printf(const char *fmt, ...)
{
    const char *s;
    int d;
	float f;
    char buf[16];
    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
    		//delay_nms(100);//temp solution ?
        if (*fmt != '%') {
            //putchar(*fmt++);
            //SerialOutChar(*fmt++);
        	uart0_16750_TX(*fmt++);
            continue;
        }
        switch (*++fmt) {
            case 's':
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart0_16750_TX(*s);
                }
                break;
            case 'd':
                d = va_arg(ap, int);
                myitoa(d, buf, 10);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart0_16750_TX(*s);
                }
                break;
			
			case 'f':
            case 'F':
            	//serial_putc_dev(0,'a');
            	f = va_arg(ap, double);
            	//serial_putc_dev(0,'b');
            	gcvt(f,15,buf);
            	//serial_putc_dev(0,'c');
                for (s = buf; *s; s++) {
                   // putchar(*s);
                	uart0_16750_TX(*s);
                }
                break;
            
            case 'x':
            case 'X':
                d = va_arg(ap, int);
                myitoa(d, buf, 16);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart0_16750_TX(*s);
                }
                break;
            /* Add other specifiers here... */
            default:
                //putchar(*fmt);
                //SerialOutChar(*fmt);
            	uart0_16750_TX(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    uart0_16750_TX('\r');
    uart0_16750_TX('\n');
    return 1;   /* Dummy return value */
}

int uart1_printf(const char *fmt, ...)
{
    const char *s;
    int d;
	float f;
    char buf[16];
    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
    		//delay_nms(100);//temp solution ?
        if (*fmt != '%') {
            //putchar(*fmt++);
            //SerialOutChar(*fmt++);
        	uart1_16750_TX(*fmt++);
            continue;
        }
        switch (*++fmt) {
            case 's':
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart1_16750_TX(*s);
                }
                break;
            case 'd':
                d = va_arg(ap, int);
                myitoa(d, buf, 10);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart1_16750_TX(*s);
                }
                break;
			/*
			case 'f':
            case 'F':
            	//serial_putc_dev(0,'a');
            	f = va_arg(ap, double);
            	//serial_putc_dev(0,'b');
            	gcvt(f,15,buf);
            	//serial_putc_dev(0,'c');
                for (s = buf; *s; s++) {
                   // putchar(*s);
                	SerialOutChar(*s);
                }
                break;
            */
            case 'x':
            case 'X':
                d = va_arg(ap, int);
                myitoa(d, buf, 16);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart1_16750_TX(*s);
                }
                break;
            /* Add other specifiers here... */
            default:
                //putchar(*fmt);
                //SerialOutChar(*fmt);
            	uart1_16750_TX(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    uart1_16750_TX('\r');
    uart1_16750_TX('\n');
    return 1;   /* Dummy return value */
}

int uart2_printf(const char *fmt, ...)
{
    const char *s;
    int d;
	float f;
    char buf[16];
    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
    		//delay_nms(100);//temp solution ?
        if (*fmt != '%') {
            //putchar(*fmt++);
            //SerialOutChar(*fmt++);
        	uart2_16750_TX(*fmt++);
            continue;
        }
        switch (*++fmt) {
            case 's':
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart2_16750_TX(*s);
                }
                break;
            case 'd':
                d = va_arg(ap, int);
                myitoa(d, buf, 10);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart2_16750_TX(*s);
                }
                break;
			/*
			case 'f':
            case 'F':
            	//serial_putc_dev(0,'a');
            	f = va_arg(ap, double);
            	//serial_putc_dev(0,'b');
            	gcvt(f,15,buf);
            	//serial_putc_dev(0,'c');
                for (s = buf; *s; s++) {
                   // putchar(*s);
                	SerialOutChar(*s);
                }
                break;
            */
            case 'x':
            case 'X':
                d = va_arg(ap, int);
                myitoa(d, buf, 16);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart2_16750_TX(*s);
                }
                break;
            /* Add other specifiers here... */
            default:
                //putchar(*fmt);
                //SerialOutChar(*fmt);
            	uart2_16750_TX(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    uart2_16750_TX('\r');
    uart2_16750_TX('\n');
    return 1;   /* Dummy return value */
}

int uart3_printf(const char *fmt, ...)
{
    const char *s;
    int d;
	float f;
    char buf[16];
    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
    		//delay_nms(100);//temp solution ?
        if (*fmt != '%') {
            //putchar(*fmt++);
            //SerialOutChar(*fmt++);
        	uart3_16750_TX(*fmt++);
            continue;
        }
        switch (*++fmt) {
            case 's':
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart3_16750_TX(*s);
                }
                break;
            case 'd':
                d = va_arg(ap, int);
                myitoa(d, buf, 10);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart3_16750_TX(*s);
                }
                break;
			/*
			case 'f':
            case 'F':
            	//serial_putc_dev(0,'a');
            	f = va_arg(ap, double);
            	//serial_putc_dev(0,'b');
            	gcvt(f,15,buf);
            	//serial_putc_dev(0,'c');
                for (s = buf; *s; s++) {
                   // putchar(*s);
                	SerialOutChar(*s);
                }
                break;
            */
            case 'x':
            case 'X':
                d = va_arg(ap, int);
                myitoa(d, buf, 16);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart3_16750_TX(*s);
                }
                break;
            /* Add other specifiers here... */
            default:
                //putchar(*fmt);
                //SerialOutChar(*fmt);
            	uart3_16750_TX(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    uart3_16750_TX('\r');
    uart3_16750_TX('\n');
    return 1;   /* Dummy return value */
}








