/*******************************************************
 * File: dspi_test.c
 * Date:
 * Author: ccore_sys
 ******************************************************/

//#include "as_archppc470.h"
//#include "ppclib.h"
#include "uart.h"
#include "gpio.h"
#include "common.h"
#include "dspi.h"
#include "w25q.h"
#include "e500mc_asm.h"
#include "axidma.h"
//#include "types.h"

#undef  PSPRINTF
#define PSPRINTF xprintf

#define DEBUG_PRINT		1

#define SPI_FLASH_TEST	1

#define SPI_INTR_TEST	0

#define SPI_AXIDMA_TEST	0
#define DSPI_DMA_INTC_TEST  	0

//sram
#define SPI_TEST_BUF_BASE		0xE0001000
//ddr
//#define SPI_TEST_BUF_BASE		0x01000000


/*
 * for Deserial Serial Peripheral Interface (DSPI)
 */
void DSPI_RegTest(void)
{
//	U16 i = 0;
	unsigned long tmp;
	uart_printf_buf("\nDeserial Serial Peripheral Interface (DSPI) reg test start .....\n");
	
#if 0
	for( i=0; i<=0x88; i+=4 )
	{
		if( *((volatile unsigned int *)(SPI_TLB_BASE +i)) != 0x00000000 )
		{
			PSPRINTF(" Reg not 0x0,0x%x=0x%x\r\n",i,
					*((volatile unsigned int *)(SPI_TLB_BASE +i))
					);
		}
	}
#endif
	
	
	PSPRINTF("DSPI_RegTest: %s\n", 
			(0x78 == (*(volatile unsigned char*)0xE960000C)) ? "MSB" : "LSB");  // 
	
#if 0
	(tmp = DSPI_A.MCR.R,tmp) 	 ? 	(PSPRINTF(" Reg DSPI_A.MCR 	 =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.TCR.R,tmp)	 ?  (PSPRINTF(" Reg DSPI_A.TCR 	 =0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = DSPI_A.CTAR[0].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.CTAR[0] =0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = DSPI_A.CTAR[1].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.CTAR[1] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.CTAR[2].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.CTAR[2] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.CTAR[3].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.CTAR[3] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.CTAR[4].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.CTAR[4] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.CTAR[5].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.CTAR[5] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.SR.R,tmp) 	 ? 	(PSPRINTF(" Reg DSPI_A.SR         =0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = DSPI_A.RSER.R,tmp)    ?	(PSPRINTF(" Reg DSPI_A.RSER 	 =0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = DSPI_A.PUSHR.R,tmp)   ? 	(PSPRINTF(" Reg DSPI_A.PUSHR 	 =0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = DSPI_A.POPR.R,tmp )   ?	(PSPRINTF(" Reg DSPI_A.POPR 	 =0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = DSPI_A.TXFR[0].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.TXFR[0] =0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = DSPI_A.TXFR[1].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.TXFR[1] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.TXFR[2].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.TXFR[2] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.TXFR[3].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.TXFR[3] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.RXFR[0].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.RXFR[0] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.RXFR[1].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.RXFR[1] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.RXFR[2].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.RXFR[2] =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = DSPI_A.RXFR[3].R,tmp) ? 	(PSPRINTF(" Reg DSPI_A.RXFR[3] =0x%x\r\n",tmp ))  : (tmp=0);	
#endif
	
	
	uart_printf_buf("DSPI reg test end .................. \r\n\r\n");
}

//-------------------------------------------------------------------------------------------------------------------------

void initDSPI_A(UINT32 regmcr, UINT32 regctar) {
	DSPI_A.MCR.R = regmcr;     		/* Configure DSPI_A as master */
	DSPI_A.CTAR[0].R = regctar; 	/* Configure CTARn  */
			
	DSPI_A.MCR.B.FRZ = 0x0;	    
	DSPI_A.MCR.B.HALT = 0x0;	/* Exit HALT mode: go from STOPPED to RUNNING state*/
	DSPI_A.SR.B.EOQF = 1;

//	SIU.PCR[95].R = 0x0600 |MAXIMUM_SLEW_RATE;       	/* MPC555x: Config pad as DSPI_A SOUT output */
//	SIU.PCR[94].R = 0x0500;       						/* MPC555x: Config pad as DSPI_A SIN input */
//	SIU.PCR[93].R = 0x0600 |MAXIMUM_SLEW_RATE;//master	/* MPC555x: Config pad as DSPI_A SCK output */
//	SIU.PCR[96].R = 0x0600 |MAXIMUM_SLEW_RATE;       	/* MPC555x: Config pad as DSPI_A PCS0 output */
}



UINT16 ReadDataDSPI_A(void) {
	UINT16 RecData_A;
	
	#if 1
		while ( !(DSPI_A.SR.R & SR_RFDF) ){}  	/* Wait for Receive FIFO Drain Flag = 1 */
	#else
		while (g_dspi_test_timeout--) {	
			if( DSPI_A.SR.B.RFDF == 1) {
				break; 
			}
		}
	#endif

//PSPRINTF("SR-1=0x%x\n", DSPI_A.SR.R); 
		
	RecData_A = DSPI_A.POPR.R;   		/* Read data received by master SPI */
	DSPI_A.SR.R = 0x90020000;        	/* Clear TCF, RDRF, EOQ flags by writing 1 */
	
//PSPRINTF("SR-2=0x%x\n", DSPI_A.SR.R);  
	return RecData_A;
}

/*
 *  return DSPI Status Register (DSPIx_SR).
 */
inline void spi_statusReg_String( UINT32 sts )
{
	PSPRINTF(" SR=0x%x: %s%s%s%s%s\r\n",
				sts,
				(sts & SR_TCF) ? "Frame TCF," : "",
				(sts & SR_EOQF) ? "Queue EOQF," : "",
				(sts & SR_TFFF) ? "" : "TX FIFO full-TFFF",
				(sts & SR_RFDF) ? "" : "RX FIFO empty-RFDF",
						
				(sts & (SR_TFUF|SR_RFOF) ) ? "TFUF or RFOF," : ""
			 );
}


//-------------------------------------------------------------------------------------------------------------------------
UINT8 gINT_TXFLG=0;
void spi_tx_isr( void )
{
	UINT32 sts = DSPI_A.SR.R; 

#if DEBUG_PRINT 	
	PSPRINTF(" spi_tx_isr():");
	spi_statusReg_String( sts );
#endif
	
	DSPI_A.SR.R = sts;
	gINT_TXFLG++;
}

UINT16 RecData_A;
UINT8 gINT_RXFLG=0;
void spi_rx_isr( void )
{
	UINT32 sts = DSPI_A.SR.R; 

#if 1//DEBUG_PRINT 	
	PSPRINTF(" spi_rx_isr():");
	spi_statusReg_String( sts );
#endif
	
	if( sts & SR_RFDF )  /* Wait for Receive FIFO Drain Flag = 1 */
	{
		RecData_A = DSPI_A.POPR.R;  	/* Read data received by master SPI */
		DSPI_A.SR.R = 0x90020000;      	/* Clear TCF, RDRF, EOQ flags by writing 1 */	
		
		gINT_RXFLG++;
	}
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
/*
 * external pins external spi module
 */
void spi_ext_ext_test( void ) 
{
	UINT16* pdata = (UINT16*)(SPI_TEST_BUF_BASE +0x0);
	
#if 0
	/* Initialize DSPI_A as master SPI and init CTARn*/
	initDSPI_A( Master_Mode 
			|Not_Cont_SCK 
		//	|Continuous_SCK
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI ,
				
				CTARn_DBR_0
	//	|CTARn_FMSZ(8)
		|CTARn_FMSZ(16)
			|CTARn_CPOL_0|CTARn_CPHA_0
		|CTARn_MSB
	//	|CTARn_LSB
	//	|CTARn_PBR_5|CTARn_BR_16 //12M/(PBR_5 *(1+0)/BR_16) = 150kHz	
		|CTARn_PBR_3|CTARn_BR_8 //500kHz
			|0x87720 );

//TCF interrupt requests
#if	SPI_INTR_TEST
	InstallInterruptHandler0( SPI_TCF_INT_NUM,spi_tx_isr);
	DSPI_A.RSER.B.TCFRE =1;
#endif
	DSPI_A.PUSHR.R =//(0x55)
					(0xabcd)//(0x1234)	
				//	|Master_CONT 
					|Master_not_CONT 
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);	
//	while( !(DSPI_A.SR.R & SR_TCF) ){}  
//	DSPI_A.SR.R = DSPI_A.SR.R;
	while( gINT_TXFLG == 0){}
	gINT_TXFLG = 0;
	pdata[0] = ReadDataDSPI_A();
	
	
	/* ext slave trans to DSPI_A master */
	#if 1	
		#if 0
			DSPI_A.MCR.B.CLR_RXF = 1;
		#endif
					
		//RFDF interrupt requests	
		#if	SPI_INTR_TEST
			DSPI_A.RSER.B.TCFRE =0;
			
			InstallInterruptHandler0( SPI_RFDF_INT_NUM,spi_rx_isr);
			DSPI_A.RSER.B.RFDFRE =1;
			DSPI_A.RSER.B.RFDFDIRS =0;
		#endif		
	
		DSPI_A.PUSHR.R =(DUMMY)
				//	|Master_CONT 
					|Master_not_CONT 
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);	
	//	while( !(DSPI_A.SR.R & SR_TCF) ){} 
		
		#if	SPI_INTR_TEST
			while( gINT_RXFLG == 0){}
			pdata[0] = RecData_A;	/* Read data on master DSPI */
			gINT_RXFLG = 0;
		#else
			pdata[0] = ReadDataDSPI_A();	/* Read data on master DSPI */
		#endif
		
		PSPRINTF("spi_ext_ext_test()/RecData_A = 0x%x\n", pdata[0]); 
		DSPI_RegTest();
	#endif
		
	
	while(1){}
#endif
	
#if 1
	/* Initialize DSPI_A as slave SPI and init CTARn*/
	initDSPI_A( Slave_Mode 
			|Not_Cont_SCK 
		//	|Continuous_SCK
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI ,
				
	//	CTARn_FMSZ(8)
		CTARn_FMSZ(16)
			|CTARn_CPOL_0|CTARn_CPHA_0
		 );
	pdata[0] = ReadDataDSPI_A();
	
	/* slave trans to ext master */
	DSPI_A.PUSHR.R =//(0x55)
					(0x1234)	
						|Master_CTASn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
		
	PSPRINTF("spi_ext_ext_test()/RecData_A = 0x%x\n", pdata[0]); 
	DSPI_RegTest();
	
	while(1){}
#endif
}


/*
 * external pins external spi module
 */
void reg_mcr_disfifo( UINT8 flag ) 
{
//	UINT16* pdata = (UINT16*)(SPI_TEST_BUF_BASE +0x0);
	
	UINT32 RxBuf_0[4] = {0}, TxBuf_0[4] = {0};
	UINT32 i;
	
	/* Initialize DSPI_A as master SPI and init CTARn*/
	initDSPI_A( Master_Mode 
				|Not_Cont_SCK 
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI ,
				
				CTARn_DBR_0
				|CTARn_FMSZ(16)
				|CTARn_CPOL_0|CTARn_CPHA_0
				|CTARn_MSB
			//	|CTARn_PBR_5|CTARn_BR_16 //12M/(PBR_5 *(1+0)/BR_16) = 150kHz	
				|CTARn_PBR_3|CTARn_BR_8 //500kHz
				|0x87720 );
	
	/* Stop transfers,the data can't be transmitted. */
 	DSPI_A.MCR.B.HALT = 1;
#if 1 	
 	/* 0 RX/TX FIFO is enabled
 	 * 1 RX/TX FIFO is disabled
 	 */
 	DSPI_A.MCR.B.DIS_TXF = flag;
  	DSPI_A.MCR.B.DIS_RXF = flag;
#endif
  	DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
  	DSPI_A.MCR.B.CLR_RXF = 1;  	
	
#if 1  	
	for(i = 0; i < 4; i++)	{
		DSPI_A.PUSHR.R = (i != 3) ?
			( (0x0000aaaa|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0) ) + 0x00001111 * i ) 
		  : ( (0x0000aaaa|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0)|Master_EOQ ) + 0x00001111 * i ) ; 
		
		TxBuf_0[i] = DSPI_A.TXFR[i].R;
	}
	for(i = 0; i < 4; i++)	{
		PSPRINTF("DSPI_A.TXFR[%d].R = 0x%x\n",i, TxBuf_0[i]); 
	}	
	
	//Start transfers,the data startes to  be transmitted.
	DSPI_A.MCR.B.HALT = 0;	
	
	while( !(DSPI_A.SR.R & SR_TCF) ){} 
#endif
	
	DSPI_RegTest();
	while(1){}
}
void reg_spi_tcr( void ) 
{
//	UINT16* pdata = (UINT16*)(SPI_TEST_BUF_BASE +0x0);
	
	UINT32 count = 0;
//	UINT32 RxBuf_0[4] = {0}, TxBuf_0[4] = {0};
	UINT32 i;
	
	/* Initialize DSPI_A as master SPI and init CTARn*/
	initDSPI_A( Master_Mode 
				|Not_Cont_SCK 
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI ,
				
				CTARn_DBR_0
				|CTARn_FMSZ(16)
				|CTARn_CPOL_0|CTARn_CPHA_0
				|CTARn_MSB
			//	|CTARn_PBR_5|CTARn_BR_16 //12M/(PBR_5 *(1+0)/BR_16) = 150kHz	
				|CTARn_PBR_3|CTARn_BR_8 //500kHz
				|0x87720 );
	
  	DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
  	DSPI_A.MCR.B.CLR_RXF = 1;  	
	
#if 1  	
	for(i = 0; i < 4; i++)	{
		DSPI_A.PUSHR.R = (0x0000aaaa|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0)|Master_EOQ ) 
						- 0x00001111 * i ; 
	
//		DSPI_A.PUSHR.R = (0x0000aaaa|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0)|Master_EOQ
//							|Master_CTCNT ) 
//						- 0x00001111 * i ; 
		
		while( !(DSPI_A.SR.R & SR_TCF) ){} 
		DSPI_A.SR.R = DSPI_A.SR.R;
		
		count = DSPI_A.TCR.R;
		PSPRINTF("DSPIx_TCR = 0x%x\n", DSPI_A.TCR.R ); 
	}
#endif
	
	DSPI_RegTest();
	while(1){}
}
void reg_mcr_clrfifo( void ) 
{
//	UINT16* pdata = (UINT16*)(SPI_TEST_BUF_BASE +0x0);
	
	UINT32 RxBuf_0[4] = {0}, TxBuf_0[4] = {0};
	UINT32 i;
	
	/* Initialize DSPI_A as master SPI and init CTARn*/
	initDSPI_A( Master_Mode 
				|Not_Cont_SCK 
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI ,
				
				CTARn_DBR_0
				|CTARn_FMSZ(16)
				|CTARn_CPOL_0|CTARn_CPHA_0
				|CTARn_MSB
			//	|CTARn_PBR_5|CTARn_BR_16 //12M/(PBR_5 *(1+0)/BR_16) = 150kHz	
				|CTARn_PBR_3|CTARn_BR_8 //500kHz
				|0x87720 );
	
	/* Stop transfers,the data can't be transmitted. */
 	DSPI_A.MCR.B.HALT = 1;	
  	DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
  	DSPI_A.MCR.B.CLR_RXF = 1;  	

//STOPPED state  	
#if 1  	
	DSPI_A.PUSHR.R = 0x000055aa|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x00005a5a|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x00001234|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x0800579a|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	
	PSPRINTF(" 1.(STOPPED):DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);
	for(i = 0; i < 4; i++)	{
		TxBuf_0[i] = DSPI_A.TXFR[i].R;
		PSPRINTF("DSPI_A.TXFR[%d].R = 0x%x\n",i, TxBuf_0[i]); 
	}
	
 	DSPI_A.MCR.B.CLR_TXF = 1; //clear FIFO
//	DSPI_A.MCR.B.CLR_RXF = 1;
	PSPRINTF(" 2.(STOPPED):DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);
	for(i = 0; i < 4; i++)	{
		TxBuf_0[i] = DSPI_A.TXFR[i].R;
		PSPRINTF("DSPI_A.TXFR[%d].R = 0x%x\n",i, TxBuf_0[i]); 
	}	
#endif
	
	

//RUNNING state
#if 1  
	#if	0//SPI_INTR_TEST
		InstallInterruptHandler0( SPI_TFFF_INT_NUM,spi_tx_isr);
		DSPI_A.RSER.B.TFFFRE =1;
		DSPI_A.RSER.B.TFFFDIRS =0;
	#endif	
	DSPI_A.PUSHR.R = 0x00005555|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x00006789|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x0000abcd|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x00005a5a|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0)|Master_EOQ; 

	//EOQF interrupt requests
	#if	SPI_INTR_TEST
		InstallInterruptHandler0( SPI_EOQF_INT_NUM,spi_tx_isr);
		DSPI_A.RSER.B.EOQFRE =1;
	#endif
	DSPI_A.MCR.B.HALT = 0;
	#if	SPI_INTR_TEST
		while( gINT_TXFLG == 0){}
		gINT_TXFLG = 0;
	#else
	//	while( !(DSPI_A.SR.R & SR_TCF) ){}
		while( !(DSPI_A.SR.R & SR_EOQF) ){}
	#endif
		
		
	PSPRINTF(" 1.(RUNING):DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);
	for(i = 0; i < 4; i++)	{
		TxBuf_0[i] = DSPI_A.TXFR[i].R;
		PSPRINTF("DSPI_A.TXFR[%d].R = 0x%x\n",i, TxBuf_0[i]); 
	}
	
 	DSPI_A.MCR.B.CLR_TXF = 1; //clear FIFO
//	DSPI_A.MCR.B.CLR_RXF = 1;
	PSPRINTF(" 2.(RUNING):DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);
	for(i = 0; i < 4; i++)	{
		TxBuf_0[i] = DSPI_A.TXFR[i].R;
		PSPRINTF("DSPI_A.TXFR[%d].R = 0x%x\n",i, TxBuf_0[i]); 
	}	
#endif	
	
	
	
	DSPI_RegTest();
	while(1){}
}
void fifo_attribute_test()
{
//	reg_mcr_disfifo(0);	//enable fifo
//	reg_mcr_disfifo(1);	//disable fifo
	
//	reg_spi_tcr();		//TCR reg test
	
	reg_mcr_clrfifo();	//clear fifo
}

//-------------------------------------------------------------------------------------------------------------------------

volatile UINT8 g_axidma_intc_num;
void axidma_intc_isr(void)
{
#if 1
//	uart_printf_buf("----axidma_intc_isr()\n" );
	PSPRINTF("--axidma_intc_isr()/0x%x\n",R32( CH_INT_STATUS_REG(1)) );
	
	
	W32(CH1_INTCLEARREG,0xFFFFFFFF);
	
	g_axidma_intc_num = 0x1;
	//g_axidma_intc_num++;
#endif
}
void set_dma_intc()
{
#if CHIP_CCP907T  
	InstallInterruptHandler0(18,ssi_int_dma);
	InstallInterruptHandler0(105,ssi_int_isr);
#endif
#if CHIP_H2040  
	InstallInterruptHandler0(AXI_DMAC_0_INTR,ssi_int_dma);
	InstallInterruptHandler0(AXI_DMAC_1_INTR,ssi_int_dma);
#endif
#if CHIP_H20x8  
	InstallInterruptHandler0(AXI_DMAC_0_INTR,axidma_intc_isr);
	InstallInterruptHandler0(AXI_DMAC_1_INTR,axidma_intc_isr);
#endif	
}

	#define TX_TRANS_SIZE 	1  //SRC_TR_WIDTH_128
//	#define TX_TRANS_SIZE 	4  //SRC_TR_WIDTH_32
void spi_axidma_tx_test( void ) 
{
	UINT16* pdata = (UINT16*)(SPI_TEST_BUF_BASE +0x0);
	
	UINT32 TxBuf_0[4] = {0}, TxBuf_1[4] = {0};
	UINT32 i,read_val = 0,read_val1 = 0;
	
	/* Initialize DSPI_A as master SPI and init CTARn*/
	initDSPI_A( Master_Mode 
				|Not_Cont_SCK 
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI ,
				
				CTARn_DBR_0
				|CTARn_FMSZ(16)
				|CTARn_CPOL_0|CTARn_CPHA_0
				|CTARn_MSB
			//	|CTARn_PBR_5|CTARn_BR_16 //10M/(PBR_5 *(1+0)/BR_16) = 125kHz	
				|CTARn_PBR_3|CTARn_BR_8 //400kHz
				|0x87720 );
	
	/* Stop transfers,the data can't be transmitted. */
 	DSPI_A.MCR.B.HALT = 1;	
  	DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
  	DSPI_A.MCR.B.CLR_RXF = 1;  	
  	
//STOPPED state  
	TxBuf_0[0] = 0x000055aa|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	TxBuf_0[1] = 0x00005a5a|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	TxBuf_0[2] = 0x00001234|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
	TxBuf_0[3] = 0x0800579a|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0);
#if (!SPI_AXIDMA_TEST)  	
	DSPI_A.PUSHR.R = TxBuf_0[0]; 
	DSPI_A.PUSHR.R = TxBuf_0[1];  
	DSPI_A.PUSHR.R = TxBuf_0[2];  
	DSPI_A.PUSHR.R = TxBuf_0[3];  
#else
	TxBuf_0[0] = cpu_to_le32( TxBuf_0[0] ); 
	TxBuf_0[1] = cpu_to_le32( TxBuf_0[1] ); 
	TxBuf_0[2] = cpu_to_le32( TxBuf_0[2] ); 
	TxBuf_0[3] = cpu_to_le32( TxBuf_0[3] ); 
	
	{
	/* enable dma  */
	W32( DMA_CfgReg,0x1 );
	#if DSPI_DMA_INTC_TEST
		W32(DMA_CfgReg,0x3);
	#endif
	read_val = R32(DMA_CfgReg);	
	
	/* starting source address */
	W32(CH1_SAR_H,DMA_SRAM_ADDR_H);
	read_val = R32(CH1_SAR_H);
	W32(CH1_SAR,TxBuf_0 );
	read_val = R32(CH1_SAR);
	
	/* starting destination address */
	W32(CH1_DAR_H,SPI_DMA_ADDR_H);
	read_val = R32(CH1_DAR_H);	//0x0c
	W32(CH1_DAR,  &(DSPI_A.PUSHR.R) ); //0xe9600034
	read_val = R32(CH1_DAR);	//+0x08
	
	W32(CH1_BLOCK_TS,(TX_TRANS_SIZE - 1) );
	read_val = R32(CH1_BLOCK_TS);	
	
//	W32(CH1_CTL,DST_TR_WIDTH_32|SRC_TR_WIDTH_32  |D_NO_INC|SINC  |DMS_1|SMS_1);
	W32(CH1_CTL,DST_MSIZE_4| 
				DST_TR_WIDTH_32|SRC_TR_WIDTH_128  
				|D_NO_INC|SINC  |DMS_1|SMS_1);
	read_val = R32(CH1_CTL);	//+0x18
	W32(CH1_CTL_H,0 );			//IOC_BLKTFR------
	read_val = R32(CH1_CTL_H);	//+0x1c
	
	read_val = R32(CH1_CFG);
	read_val1 = R32(CH1_CFG_H);
	W32(CH1_CFG_H, HARD_HAND_SEL_DST| MEM_NOHAND_SEL_SRC| TT_FC_MEM2PER  |SPI_DMA_DST_PER);	
	read_val1 = R32(CH1_CFG_H);
	W32(CH1_CFG,(read_val & SRC_MULTBLK_TYPE_MASK & DST_MULTBLK_TYPE_MASK) 
				|DST_MULTBLK_TYPE_CONTIGUOUS
					|SRC_MULTBLK_TYPE_CONTIGUOUS ); 
	read_val = R32(CH1_CFG);
	
	#if 1
		//TFFF interrupt requests or DMA requests are enabled
		DSPI_A.RSER.B.TFFFRE =1;
		//DMA request will be generated
		DSPI_A.RSER.B.TFFFDIRS =1;
	#endif

	#if DSPI_DMA_INTC_TEST
		InstallInterruptHandler0( AXI_DMAC_0_INTR,axidma_intc_isr );
		InstallInterruptHandler0( AXI_DMAC_1_INTR,axidma_intc_isr );
		
	//	W32(CH1_INTSIGNAL_EN_REG,0x3);	//0x7c08007
		W32(CH1_INTSIGNAL_EN_REG,Enable_DMA_TFR_DONE_IntSignal
								|Enable_BLOCK_TFR_DONE_IntSignal
								);
	//	read_val = R32(CH1_CFG);
	#endif
		W32(DMA_ChEnReg,0x00000101);	//en ch1
		read_val = R32(DMA_ChEnReg);
		
	#if DSPI_DMA_INTC_TEST
		while(g_axidma_intc_num != 0x1)
		{	
			PSPRINTF(" ->dma status 0x%x/ch1 status 0x%x\n",
							(R32(DMAC_INT_STATUS_REG)), (R32(CH_INT_STATUS_REG(1))) );
			
			delay(0x10000);
		}
		g_axidma_intc_num = 0;
	#endif
	#if (!DSPI_DMA_INTC_TEST)	
	//	PSPRINTF("DMA_CfgReg = 0x%x\n",R32(DMA_CfgReg) );	
	//	PSPRINTF("DMA_ChEnReg = 0x%x\n",R32(DMA_ChEnReg) );
	//	PSPRINTF("DMAC_INT_STATUS_REG = 0x%x-----\n",R32(DMAC_INT_STATUS_REG) );	
		
		while( (R32(CH_INT_STATUS_REG(1)) &(DMA_TFR_DONE_INTSTAT))
				!= DMA_TFR_DONE_INTSTAT
			 )
		{
			PSPRINTF(" ->dma status 0x%x/ch1 status 0x%x\n",
							(R32(DMAC_INT_STATUS_REG)), (R32(CH_INT_STATUS_REG(1))) );
			
			delay(0x10000);	
		}
		W32( CH_INT_STATUS_REG(1),	R32(CH_INT_STATUS_REG(1)) );
	#endif	
	}
#endif
		
	PSPRINTF(" 1.(STOPPED):DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);
	for(i = 0; i < 4; i++)	{
		TxBuf_1[i] = DSPI_A.TXFR[i].R;
		PSPRINTF("DSPI_A.TXFR[%d].R = 0x%x\n",i, TxBuf_1[i]); 
	}
	
#if 1
	DSPI_A.MCR.B.HALT = 0;	//tx
//	while( !(DSPI_A.SR.R & SR_TCF) ){}
	while( !(DSPI_A.SR.R & SR_EOQF) ){}	
#endif
	DSPI_RegTest();
	while(1){}
}

	#define RX_TRANS_SIZE 	4  //SRC_TR_WIDTH_32
void spi_axidma_rx_test( void ) 
{
	UINT16* pdata = (UINT16*)(SPI_TEST_BUF_BASE +0x0);
	
	UINT32 RxBuf_0[4] = {0}, RxBuf_1[4] = {0};
	UINT32 i,read_val = 0,read_val1 = 0;
	
	/* Initialize DSPI_A as master SPI and init CTARn*/
	initDSPI_A( Master_Mode 
				|Not_Cont_SCK 
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI ,
				
				CTARn_DBR_0
				|CTARn_FMSZ(16)
				|CTARn_CPOL_0|CTARn_CPHA_0
				|CTARn_MSB
			//	|CTARn_PBR_5|CTARn_BR_16 //10M/(PBR_5 *(1+0)/BR_16) = 125kHz	
				|CTARn_PBR_3|CTARn_BR_8 //400kHz
				|0x87720 );
	
	/* Stop transfers,the data can't be transmitted. */
 	DSPI_A.MCR.B.HALT = 1;	
  	DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
  	DSPI_A.MCR.B.CLR_RXF = 1;  	
  	
//STOPPED state  
  	DSPI_A.PUSHR.R = 0x000055aa|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
  	DSPI_A.PUSHR.R = 0x00005a5a|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
  	DSPI_A.PUSHR.R = 0x00001234|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0); 
  	DSPI_A.PUSHR.R = 0x0800579a|Master_not_CONT|Master_CTASn(0)|Master_PCSn(0);
	PSPRINTF(" 1.(STOPPED):DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);	
	for(i = 0; i < 4; i++)	{
		RxBuf_0[i] = DSPI_A.RXFR[i].R;
		PSPRINTF("DSPI_A.RXFR[%d].R = 0x%x\n",i, RxBuf_0[i]); 
	}	

	DSPI_A.MCR.B.HALT = 0;
//	while( !(DSPI_A.SR.R & SR_TCF) ){}
	while( !(DSPI_A.SR.R & SR_EOQF) ){}	
	
#if (!SPI_AXIDMA_TEST)  
	//read pop below
#else
	
	{
	/* enable dma  */
	W32( DMA_CfgReg,0x1 );
	#if DSPI_DMA_INTC_TEST
		W32(DMA_CfgReg,0x3);
	#endif
	read_val = R32(DMA_CfgReg);	
	
	/* starting source address */
	W32(CH1_SAR_H,DMA_SRAM_ADDR_H);
	read_val = R32(CH1_SAR_H);
	W32(CH1_SAR,&(DSPI_A.POPR.R) );
	read_val = R32(CH1_SAR);
	
	/* starting destination address */
	W32(CH1_DAR_H,SPI_DMA_ADDR_H);
	read_val = R32(CH1_DAR_H);	//0x0c
	W32(CH1_DAR,  RxBuf_1 ); 
	read_val = R32(CH1_DAR);	//+0x08
	
	//the total number of data of width CHx_CTL.SRC_TR_WIDTH
	W32(CH1_BLOCK_TS,(RX_TRANS_SIZE - 1) );
	read_val = R32(CH1_BLOCK_TS);	
	
	W32(CH1_CTL,SRC_MSIZE_4|
				DST_TR_WIDTH_128|SRC_TR_WIDTH_32
				|DINC|S_NO_INC  |DMS_1|SMS_1);
	
	read_val = R32(CH1_CTL);	//+0x18
	W32(CH1_CTL_H,0 );			//IOC_BLKTFR------
	read_val = R32(CH1_CTL_H);	//+0x1c
	
	read_val = R32(CH1_CFG);
	read_val1 = R32(CH1_CFG_H);
	W32(CH1_CFG_H, HARD_HAND_SEL_SRC| MEM_NOHAND_SEL_DST| TT_FC_PER2MEM  |SPI_DMA_SRC_PER);
	read_val1 = R32(CH1_CFG_H);
	W32(CH1_CFG,(read_val & SRC_MULTBLK_TYPE_MASK & DST_MULTBLK_TYPE_MASK) 
				|DST_MULTBLK_TYPE_CONTIGUOUS
					|SRC_MULTBLK_TYPE_CONTIGUOUS ); 
	read_val = R32(CH1_CFG);
	
	//rx 
	#if 1
		//RFDF interrupt requests or DMA requests are enabled
		DSPI_A.RSER.B.RFDFRE =1;
		//DMA request will be generated
		DSPI_A.RSER.B.RFDFDIRS =1;
	#endif
	
	#if DSPI_DMA_INTC_TEST
		InstallInterruptHandler0( AXI_DMAC_0_INTR,axidma_intc_isr );
		InstallInterruptHandler0( AXI_DMAC_1_INTR,axidma_intc_isr );
		
	//	W32(CH1_INTSIGNAL_EN_REG,0x3);	//0x7c08007
		W32(CH1_INTSIGNAL_EN_REG,Enable_DMA_TFR_DONE_IntSignal
								|Enable_BLOCK_TFR_DONE_IntSignal
								);
	//	read_val = R32(CH1_CFG);
	#endif
		W32(DMA_ChEnReg,0x00000101);	//en ch1
		read_val = R32(DMA_ChEnReg);
		
	#if DSPI_DMA_INTC_TEST
		while(g_axidma_intc_num != 0x1)
		{	
			PSPRINTF(" ->dma status 0x%x/ch1 status 0x%x\n",
							(R32(DMAC_INT_STATUS_REG)), (R32(CH_INT_STATUS_REG(1))) );
			
			delay(0x10000);
		}
		g_axidma_intc_num = 0;
	#endif
	#if (!DSPI_DMA_INTC_TEST)	
	//	PSPRINTF("DMA_CfgReg = 0x%x\n",R32(DMA_CfgReg) );	
	//	PSPRINTF("DMA_ChEnReg = 0x%x\n",R32(DMA_ChEnReg) );
	//	PSPRINTF("DMAC_INT_STATUS_REG = 0x%x-----\n",R32(DMAC_INT_STATUS_REG) );	
		
		while( (R32(CH_INT_STATUS_REG(1)) &(DMA_TFR_DONE_INTSTAT))
				!= DMA_TFR_DONE_INTSTAT
			 )
		{
			PSPRINTF(" ->dma status 0x%x/ch1 status 0x%x\n",
							(R32(DMAC_INT_STATUS_REG)), (R32(CH_INT_STATUS_REG(1))) );
			
			delay(0x10000);	
		}
		W32( CH_INT_STATUS_REG(1),	R32(CH_INT_STATUS_REG(1)) );
	#endif	
	}
#endif
		
	PSPRINTF(" 2.(RUNING):DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);	
	for(i = 0; i < 4; i++)	{
		RxBuf_1[i] = DSPI_A.RXFR[i].R;
		PSPRINTF("DSPI_A.RXFR[%d].R = 0x%x\n",i, RxBuf_1[i]); 
	}	

	
	DSPI_RegTest();
	while(1){}
}
//-------------------------------------------------------------------------------------------------------------------------
void spi_CTAR_sck_format_test( void ) 
{
//	UINT16* pdata = (UINT16*)(SPI_TEST_BUF_BASE +0x0);
	
	UINT32 i = 0;
	UINT32 RxBuf_0[4] = {0}, TxBuf_0[4] = {0};
	UINT32 tmp;
	UINT32 bit[13]=
	{
	0b1111,
	0b11111,
	0b111111,
	0b1111111,
	0b11111111,
	0b111111111,
	0b1111111111,
	0b11111111111,
	0b111111111111,
	0b1111111111111,
	0b11111111111111,
	0b111111111111111,
	0b1111111111111111,
	};	
	UINT8 size;
	UINT8 ctar_n;
	
	/* Initialize DSPI_A as master SPI and init CTARn*/
#if 1	
	size = 16;
#else
//	size = 4;
	size = 8;
//	size = 5;
//	size = 11;
//	size = 15;
#endif
	
#if 0	
	initDSPI_A(
#else		
	DSPI_A.MCR.R = 	/* Configure DSPI_A as master */
#endif
				Master_Mode 
				|Not_Cont_SCK 
				|SPI_Config
				|PCSn(0) 
				|HALT_DSPI;
	
#if 0
	DSPI_A.CTAR[0].R =  	/* Configure CTARn  */
#else
	ctar_n = 1;
	ctar_n = 2;
	ctar_n = 3;
	ctar_n = 4;
	ctar_n = 5;
	DSPI_A.CTAR[ctar_n].R =  			
#endif
			#if 1
				CTARn_DBR_0
			#else
			//Double Baud Rate
			//DSPI SCK Duty Cycle
				CTARn_DBR_1 	//12M/(CTARn_PBR_3 *(1+1)/CTARn_BR_8) = 1MHz  //300KHz
			#endif			
				
				|CTARn_FMSZ( size )
				
			//Polarity/Phase	
			#if 1
				|CTARn_CPOL_0|CTARn_CPHA_0
			#else
			//	|CTARn_CPOL_1|CTARn_CPHA_1
				|CTARn_CPOL_0|CTARn_CPHA_1
			//	|CTARn_CPOL_1|CTARn_CPHA_0
			#endif
			#if 1	
				|CTARn_MSB
			#else
				|CTARn_LSB
			#endif
				
			//Baud Rate	
			#if 0 	
				|CTARn_PBR_5|CTARn_BR_16 //12M/(PBR_5 *(1+0)/BR_16) = 150kHz
			#else
			//	|CTARn_PBR_3|CTARn_BR_8 //500kHz
				|CTARn_PBR_2|CTARn_BR_2 //3MHz
			//	|CTARn_PBR_2|CTARn_BR_4 //1.5MHz
			//	|CTARn_PBR_2|CTARn_BR_6 //1MHz
			#endif
			#if 1 	
				|0x87720 //21.3us/21.3us/3.3us
			#else
				|0x83330 //1.3us/1.3us/6.6us
			#endif
				;
	DSPI_A.MCR.B.FRZ = 0x0;	    
//	DSPI_A.MCR.B.HALT = 0x0;	/* Exit HALT mode: go from STOPPED to RUNNING state*/
	DSPI_A.SR.B.EOQF = 1;

	
	/* Stop transfers,the data can't be transmitted. */
 	DSPI_A.MCR.B.HALT = 1;	
  	DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
  	DSPI_A.MCR.B.CLR_RXF = 1;  	
	
	DSPI_A.PUSHR.R = 0x00002468|Master_not_CONT|Master_CTASn(ctar_n)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x00001234|Master_not_CONT|Master_CTASn(ctar_n)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x0000abcd|Master_not_CONT|Master_CTASn(ctar_n)|Master_PCSn(0); 
	DSPI_A.PUSHR.R = 0x00001369|Master_not_CONT|Master_CTASn(ctar_n)|Master_PCSn(0)|Master_EOQ; 
#if 1 
	PSPRINTF(" DSPI0_MCR = %x, DSPI0_SR = %x\n", DSPI_A.MCR.R, DSPI_A.SR.R);
	for(i = 0; i < 4; i++)	{
		TxBuf_0[i] = DSPI_A.TXFR[i].R;
		
		PSPRINTF("DSPI_A.TXFR[%d].R = 0x%x---will send 0x%x\n",
				i, TxBuf_0[i],
				    tmp= TxBuf_0[i]  & bit[size-4]    ); 
	}	
#endif  	
	
	DSPI_A.MCR.B.HALT = 0;
	while( !(DSPI_A.SR.R & SR_EOQF) ){}
	DSPI_A.SR.R = DSPI_A.SR.R;
	
//	DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
	DSPI_A.MCR.B.CLR_RXF = 1; 	
	for(i = 0; i< 4; i++) {
		DSPI_A.PUSHR.R = DUMMY|Master_not_CONT|Master_CTASn(ctar_n)|Master_PCSn(0); 
		while( !(DSPI_A.SR.R & SR_TCF) ){} 
		
		RxBuf_0[i] = ReadDataDSPI_A();	
	}
	for(i = 0; i< 4; i++) {
		PSPRINTF("RxBuf_0[%d] = 0x%x\n",i, RxBuf_0[i]); 
	}	
	
	
	DSPI_RegTest();
	while(1){}
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
#if SPI_FLASH_TEST

void Dspi_Init_for_W25q_SpiFlash( void )
{
	/* Initialize DSPI_A as master SPI and init CTARn*/
	initDSPI_A( Master_Mode 
			|Not_Cont_SCK 
		//	|Continuous_SCK
				|SPI_Config
				|PCSn(1) 
				|HALT_DSPI ,
				
				CTARn_DBR_0
				|CTARn_FMSZ(8)
				|CTARn_CPOL_0|CTARn_CPHA_0
				|CTARn_MSB
		//	|CTARn_PBR_5|CTARn_BR_16 //12M/(PBR_5 *(1+0)/BR_16) = 150kHz
				|CTARn_PBR_5|CTARn_BR_2 //100M/(PBR_5 *(1+0)/BR_2) = 10MHz
		//		|CTARn_DBR_1 //20M/(1 *(1+1)/1) = 10MHz
		//	|CTARn_PBR_3|CTARn_BR_16 //250kHz
		//		|CTARn_PBR_7|CTARn_BR_32 
				|0x87720 );
							
}

void W25q_SpiFlash_Read_MDID( UINT8* pdata )
{
	UINT32 pop_val;
	Dspi_Init_for_W25q_SpiFlash();
	
	DSPI_A.PUSHR.R = (CMD_W25Q_MANUFACTDEVICEID)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);/* Transmit data from master to slave SPI with EOQ */
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
//	DSPI_RegTest();
	
	
	DSPI_A.PUSHR.R = (0x00)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
	
	
	DSPI_A.PUSHR.R = (0x00)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
	
	
	DSPI_A.PUSHR.R = (0x00)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
#if 1
//test	
//	DSPI_A.MCR.B.CLR_RXF = 1; 
//	DSPI_A.MCR.R = 0x80013400;MSYNC;	//DSPI_A.MCR.R = 0x80013400;MSYNC;
//	while(1){PSPRINTF("s3-0x%x\n", DSPI_A.SR.R); }
//	DSPI_A.MCR.B.CLR_RXF = 1;PSPRINTF("s4-0x%x\n", DSPI_A.SR.R);
//	ReadDataDSPI_A();PSPRINTF("s5-0x%x\n", DSPI_A.SR.R);
//	ReadDataDSPI_A();PSPRINTF("s5-0x%x\n", DSPI_A.SR.R);
#endif
	
	
	DSPI_A.PUSHR.R = (DUMMY)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	pdata[0] = ReadDataDSPI_A();	/* Read data on master DSPI */
//	PSPRINTF("RecData_A = 0x%x\n", RecData_A); 
//	DSPI_RegTest();
	
	
	DSPI_A.PUSHR.R = (DUMMY)
					//	|Master_CONT --
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);	
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	pdata[1] = ReadDataDSPI_A();	/* Read data on master DSPI */
//	PSPRINTF("RecData_A = 0x%x\n", RecData_A); 	
//	DSPI_RegTest();

#if DEBUG_PRINT
	PSPRINTF("read MANUFACTURER ID =0x%x + Device ID =0x%x...\n", 
			pdata[0], //W25Q_MANUID_WINBOND
			pdata[1]  //W25Q_DEV_ID
			);
#endif
}

void W25q_SpiFlash_Read_JEDECID( UINT8* pdata )
{
	UINT32 pop_val;
//	Dspi_Init_for_W25q_SpiFlash();
	
	DSPI_A.PUSHR.R = (CMD_W25Q_JEDECDEVICEID)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	
	
#if 0
	DSPI_A.MCR.B.CLR_RXF = 1;
#endif
	DSPI_A.PUSHR.R = (DUMMY)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	pdata[0]  = ReadDataDSPI_A();	/* Read data on master DSPI */
	
	
	DSPI_A.PUSHR.R = (DUMMY)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
					//	|Master_CTCNT	test
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	pdata[1] = ReadDataDSPI_A();	/* Read data on master DSPI */
	
	
	DSPI_A.PUSHR.R = (DUMMY)
					//	|Master_CONT --
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	pdata[2] = ReadDataDSPI_A();	/* Read data on master DSPI */
	
	
#if DEBUG_PRINT
	PSPRINTF("read MANUFACTURER ID =0x%x + JEDEC ID =0x%x...\n", 
			pdata[0], //W25Q_MANUID_WINBOND
			*(UINT16*)(pdata + 1)  //W25Q_JEDEC_ID_SPI_MODE
			);
#endif
}


/* Read Data instruction 
 * allows one or more data bytes to be sequentially read from the memory
 * */
UINT16 W25q_SpiFlash_ReadStatus( UINT8 sts_num )
{
	UINT16 cmd;
	UINT32 pop_val;
	
	switch( sts_num )
	{
		case 2:cmd = CMD_W25Q_READSTATUS_2;	break;
		case 3:cmd = CMD_W25Q_READSTATUS_3;	break;
		case 1:
		default:cmd = CMD_W25Q_READSTATUS_1;
			break;
	}
	
	DSPI_A.PUSHR.R = (cmd)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
	
#if 0
	DSPI_A.MCR.B.CLR_RXF = 1;
#endif	
	DSPI_A.PUSHR.R = (DUMMY)
					//	|Master_CONT 
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	return( ReadDataDSPI_A() );	/* Read data on master DSPI */
}


/* Read Data instruction 
 * allows one or more data bytes to be sequentially read from the memory
 * */
void W25q_SpiFlash_ReadData( UINT32 addr,UINT32 size,UINT8* pdata )
{
	UINT32 i = 0;
	UINT32 pop_val;
	
	DSPI_A.PUSHR.R = (CMD_W25Q_READDATA)
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
	
	DSPI_A.PUSHR.R = ( (addr>>16)&0xff )
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
	
	DSPI_A.PUSHR.R = ( (addr>>8)&0xff )
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
	
	DSPI_A.PUSHR.R = ( (addr>>0)&0xff )
						|Master_CONT
						|Master_CTASn(0)
						|Master_EOQ  
						|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;
	
	
#if 0
	DSPI_A.MCR.B.CLR_RXF = 1;
#endif
	i = 0;
	while( (size>1) && (i< (size-1))   ) 
	{
		DSPI_A.PUSHR.R = ( DUMMY )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
		while( !(DSPI_A.SR.R & SR_TCF) ){}
		
		pdata[i] = ReadDataDSPI_A();	/* Read data on master DSPI */
		i++;
	}
	{
		DSPI_A.PUSHR.R = ( DUMMY )
						//	|Master_CONT  
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
		while( !(DSPI_A.SR.R & SR_TCF) ){}
		
		pdata[i] = ReadDataDSPI_A();	/* Read data on master DSPI */
	}
}

#if 0
/* Fast Read instruction 
 * 	is similar to the Read Data instruction 
 * 	except that it can operate at the highest possible frequency of FR.
 * */
void W25q_SpiFlash_FastReadData( UINT32 addr,UINT32 size,UINT8* pdata )
{}
#endif
		

void W25q_SpiFlash_WriteEnable( void )
{
	UINT32 pop_val;
	
	DSPI_A.PUSHR.R = (CMD_W25Q_WRITEENABLE)
						//	|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;	
	pop_val = DSPI_A.POPR.R;	
}
/* Page Program instruction 
 *  allows from one byte to 256 bytes (a page) of data to be programmed at
 * 	previously erased (FFh) memory locations.
 */
void W25q_SpiFlash_PageProgram( UINT32 addr,UINT32 size,UINT8* pdata )
{
	UINT16 sts;
	UINT32 pop_val;
	UINT32 i = 0;
	
	if( size > 256 ){
		#if DEBUG_PRINT
			PSPRINTF("	W25Q 256 bytes a page...\n" );
		#endif			
		return;
	}
	
#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-before prog,sts1 = 0x%x.\n", sts );
	#endif	
	
	W25q_SpiFlash_WriteEnable();
	
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("2-before prog,sts1 = 0x%x.\n", sts );
	#endif	
	if( !(sts & 0x02) ){
		//Write Enable Latch (WEL)
		while(1) {}
		return;		
	}
#endif	

	DSPI_A.PUSHR.R = (CMD_W25Q_PAGE_PROGRAM)
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>16)&0xff )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>8)&0xff )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>0)&0xff )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	
	i = 0;
	while( (size>1) && (i< (size-1))   ) 
	{
		DSPI_A.PUSHR.R = ( pdata[i] )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
		while( !(DSPI_A.SR.R & SR_TCF) ){}
		DSPI_A.SR.R = DSPI_A.SR.R;
		pop_val = DSPI_A.POPR.R;	
		
		i++;
	}
	{
		DSPI_A.PUSHR.R = ( pdata[i] )
						//	|Master_CONT 
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
		while( !(DSPI_A.SR.R & SR_TCF) ){}
		DSPI_A.SR.R = DSPI_A.SR.R;
		pop_val = DSPI_A.POPR.R;	
	}
	
	
#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-after prog,sts1 = 0x%x.\n", sts );
	#endif	
	while( sts & 0x01 )
	{
		//Erase/Write In Progress (BUSY)
		sts = W25q_SpiFlash_ReadStatus(1);
	}
	#if DEBUG_PRINT		
		PSPRINTF("2-after prog,sts1 = 0x%x.\n", sts );
	#endif	
#endif	
}

/* Sector Erase instruction 
 * sets all memory within a specified sector (4K-bytes) to the erased state of all 1s (FFh). 
 */
void W25q_SpiFlash_SectorEarse( UINT32 addr )
{
	UINT16 sts;
	UINT32 pop_val;
	
#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-before erase,sts1 = 0x%x.\n", sts );
	#endif	
	
	W25q_SpiFlash_WriteEnable();
	
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("2-before erase,sts1 = 0x%x.\n", sts );
	#endif	
	if( !(sts & 0x02) ){
		//Write Enable Latch (WEL)
		while(1) {}
		return;		
	}
#endif
	
	DSPI_A.PUSHR.R = (CMD_W25Q_SECTOR_ERASE)
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>16)&0xff )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>8)&0xff )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>0)&0xff )
						//	|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-after erase,sts1 = 0x%x.\n", sts );
	#endif	
	while( sts & 0x01 )
	{
		//Erase/Write In Progress (BUSY)
		sts = W25q_SpiFlash_ReadStatus(1);
	}
	#if DEBUG_PRINT		
		PSPRINTF("2-after erase,sts1 = 0x%x.\n", sts );
	#endif	
#endif
}


/* Block Erase instruction 
 * sets all memory within a specified block (64K-bytes) to the erased state of all 1s (FFh). 
 * */
void W25q_SpiFlash_64KBlockErase( UINT32 addr )
{
	UINT16 sts;
	UINT32 pop_val;
	
#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-before erase,sts1 = 0x%x.\n", sts );
	#endif	
	
	W25q_SpiFlash_WriteEnable();
	
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("2-before erase,sts1 = 0x%x.\n", sts );
	#endif	
	if( !(sts & 0x02) ){
		//Write Enable Latch (WEL)
		while(1) {}
		return;		
	}
#endif
	
	DSPI_A.PUSHR.R = (CMD_W25Q_BLOCK_64K_ERASE)
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>16)&0xff )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>8)&0xff )
							|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
	DSPI_A.PUSHR.R = ( (addr>>0)&0xff )
						//	|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-after erase,sts1 = 0x%x.\n", sts );
	#endif	
	while( sts & 0x01 )
	{
		//Erase/Write In Progress (BUSY)
		sts = W25q_SpiFlash_ReadStatus(1);
	}
	#if DEBUG_PRINT		
		PSPRINTF("2-after erase,sts1 = 0x%x.\n", sts );
	#endif	
#endif
}

/* Chip Erase instruction 
 * sets all memory within the device to the erased state of all 1s (FFh).
 * */
void W25q_SpiFlash_ChipErase( void )
{
	UINT16 sts;
	UINT32 pop_val;

#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-before erase,sts1 = 0x%x.\n", sts );
	#endif	
	
	W25q_SpiFlash_WriteEnable();
	
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("2-before erase,sts1 = 0x%x.\n", sts );
	#endif	
	if( !(sts & 0x02) ){
		//Write Enable Latch (WEL)
		while(1) {}
		return;		
	}
#endif
	
	DSPI_A.PUSHR.R = (CMD_W25Q_CHIP_ERASE)
						//	|Master_CONT
							|Master_CTASn(0)
							|Master_EOQ  
							|Master_PCSn(0);
	while( !(DSPI_A.SR.R & SR_TCF) ){}  
	DSPI_A.SR.R = DSPI_A.SR.R;
	pop_val = DSPI_A.POPR.R;	
	
#if 1
	sts = W25q_SpiFlash_ReadStatus(1);
	#if DEBUG_PRINT		
		PSPRINTF("1-after erase,sts1 = 0x%x.\n", sts );
	#endif	
	while( sts & 0x01 )
	{
		//Erase/Write In Progress (BUSY)
		sts = W25q_SpiFlash_ReadStatus(1);
	}
	#if DEBUG_PRINT		
		PSPRINTF("2-after erase,sts1 = 0x%x.\n", sts );
	#endif	
#endif
}


/* compatibility test/spi flash
* */
void W25q_SpiFlash_Test( void )
{
	UINT32 i, j;
	UINT32 test_addr,test_size;

	Dspi_Init_for_W25q_SpiFlash();
	
#if 1//DEBUG_PRINT		
	PSPRINTF("init,sts1 = 0x%x.\n", W25q_SpiFlash_ReadStatus(1) );
	PSPRINTF("init,sts2 = 0x%x.\n", W25q_SpiFlash_ReadStatus(2) );
	PSPRINTF("init,sts3 = 0x%x.\n", W25q_SpiFlash_ReadStatus(3) );
#endif	
	
				#if 0
				//dis fifo
					
					/* Stop transfers,the data can't be transmitted. */
					DSPI_A.MCR.B.HALT = 1;
					#if 1 	
					/* 0 RX/TX FIFO is enabled
					 * 1 RX/TX FIFO is disabled
					 */
					DSPI_A.MCR.B.DIS_TXF = 1;
					DSPI_A.MCR.B.DIS_RXF = 1;
					#endif
					
					DSPI_A.MCR.B.CLR_TXF = 1;	 //clear FIFO
					DSPI_A.MCR.B.CLR_RXF = 1; 
					
				//	DSPI_RegTest();
				#endif
				#if 0
					//Start transfers,the data startes to  be transmitted.
					DSPI_A.MCR.B.HALT = 0;	
					DSPI_RegTest();
				#endif	
 	
#if 1
	W25q_SpiFlash_Read_MDID( (UINT8*)(SPI_TEST_BUF_BASE +0x0) );
//	DSPI_RegTest();
	
	W25q_SpiFlash_Read_JEDECID( (UINT8*)(SPI_TEST_BUF_BASE +0x10) );
//	DSPI_RegTest();
#endif
	

	test_addr = 0x00000000;
	test_size = 32;//256
#if 0	
	W25q_SpiFlash_SectorEarse( test_addr );
//	W25q_SpiFlash_64KBlockErase( test_addr );
//	W25q_SpiFlash_ChipErase( );
	delay(0x100);
	W25q_SpiFlash_ReadData( test_addr,test_size,(UINT8*)(SPI_TEST_BUF_BASE +0x0) );
	#if DEBUG_PRINT
		for( i=0; i<(test_size/4);i++ ){
			PSPRINTF("0x%x read = 0x%x.\n",  (UINT32*)SPI_TEST_BUF_BASE +i,
					*( (UINT32*)SPI_TEST_BUF_BASE +i )  );
		}
	#endif
#endif
		
#if 1		
	for( i=0; i<(test_size/4);i++ ){
		*( (UINT32*)SPI_TEST_BUF_BASE +0x0 +i ) = 0x1234abcd+i;//0x789a3456+i;
	}	
	W25q_SpiFlash_PageProgram( test_addr,test_size,(UINT8*)(SPI_TEST_BUF_BASE +0x0) );
//	delay(0x4000);
	W25q_SpiFlash_ReadData( test_addr,test_size,(UINT8*)(SPI_TEST_BUF_BASE +0x0) );
	#if DEBUG_PRINT	
		for( i=0; i<(test_size/4);i++ ){
			PSPRINTF("after Prog 0x%x read = 0x%x.\n",  (UINT32*)SPI_TEST_BUF_BASE +i,
					*( (UINT32*)SPI_TEST_BUF_BASE +i )  );
		}
	#endif		
#endif	
}

#endif //SPI_FLASH_TEST	
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

/*
 */
void dspi_main( void )
{
	UINT32 i, j;
	UINT32 test_addr,test_size;
	
#if 1
	/* reg test
	 * */
	DSPI_RegTest();
#endif
	
	
#if SPI_FLASH_TEST
	W25q_SpiFlash_Test();
#endif
	
//	spi_ext_ext_test(); 
//	fifo_attribute_test();
//	spi_CTAR_sck_format_test();
	
#if 0//SPI_AXIDMA_TEST
	#if 0
	//test dmac0: axi path
	
		i = R32(0xEB009000); //ccm_sys_axi
		PSPRINTF(" 0.0xEB009000 = 0x%x \n", i );//0x3ff/  corenet path
		i &= ~ (1 <<2);//cir02上的master M5 -DMAC0
		i &= ~ (1 <<7);//cir13上的master M10
		W32(0xEB009000, i );
		PSPRINTF(" 1.0xEB009000 = 0x%x \n", R32(0xEB009000) );//0x37b
	#endif
	
	spi_axidma_tx_test();
	spi_axidma_rx_test();
#endif
	
#if 0
	DSPI_RegTest();
#endif
	
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
