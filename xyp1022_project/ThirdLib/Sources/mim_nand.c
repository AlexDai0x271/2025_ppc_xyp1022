/*******************************************************
 * File: mim_test.c
 * Date:
 * Author: ccore_sys
 ******************************************************/

//#include "as_archppc470.h"
//#include "ppclib.h"
#include "uart.h"
#include "gpio.h"
#include "common.h"
#include "mim.h"
#include "nandflash-mt29f.h"
#include "gpio.h"


#define DEBUG_PRINT		1

#define BCH_MIM_NONECC_TEST		1
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

/*
 * for Memory Integration Module (MIM)
 */
void MIM_NFC_RegTest(void)
{
//	U16 i = 0;
	U32 tmp;
	uart_printf_buf("\n (MIM) reg test start .....\n");
	
#if 0
	for( i=0; i<=0x40; i+=4 )
	{
		tmp = R32(MIM_REG_BASE_ADDR +i);
		if( tmp != 0x00000000 )
		{
			PSPRINTF(" Reg not 0x0,0x%x=0x%x\r\n", i,tmp);
		}
	}
#endif
#if 0
	PSPRINTF("MIM reg %s\n", (0x27 == R8(MIM_REG_BASE_ADDR+2)) ? "MSB" : "LSB"); //
#endif
#if 1
	(tmp = R32(SCSCR0)) 	? 	(PSPRINTF(" Reg SCSCR0 	=0x%x\r\n",tmp ))  : (tmp=0);		
	(tmp = R32(SCSCR1))		?   (PSPRINTF(" Reg SCSCR1 	=0x%x\r\n",tmp ))  : (tmp=0);		
	(tmp = R32(NFCSCR)) 	? 	(PSPRINTF(" Reg NFCSCR	=0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = R32(NPCR)) 		? 	(PSPRINTF(" Reg NPCR		=0x%x\r\n",tmp ))  : (tmp=0);
	
	(tmp = R16(IO_CTRL)) 	? 	(PSPRINTF(" Reg IO_CTRL 	=0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = R16(ROM_WS)) 	? 	(PSPRINTF(" Reg ROM_WS 	=0x%x\r\n",tmp ))  : (tmp=0);	
	
	(tmp = R32(GPIO_DO)) 	? 	(PSPRINTF(" Reg GPIO_DO 	=0x%x\r\n",tmp ))  : (tmp=0);
	(tmp = R32(GPIO_DIR)) 	? 	(PSPRINTF(" Reg GPIO_DIR 	=0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = R32(GPIO_DI)) 	? 	(PSPRINTF(" Reg GPIO_DI 	=0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = R32(ONFI30SCR)) 	? 	(PSPRINTF(" Reg ONFI30SCR =0x%x\r\n",tmp ))  : (tmp=0);	
	(tmp = R32(ONFI30PH90CR)) ? (PSPRINTF(" Reg ONFI30PH90CR =0x%x\r\n",tmp ))  : (tmp=0);
/*
 Reg SCSCR0 	=0x102702
 Reg SCSCR1 	=0x11102706
 Reg NFCSCR		=0x11105702
 Reg NPCR		=0x8fff00
 Reg ONFI30SCR  =0x80080
*/
#endif
	
	uart_printf_buf("(MIM) reg test end ..... \r\n\r\n");
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

inline void Nand_Write_Cmd( UINT8 cmd_val)
{
	W8( NFC_CMD,cmd_val );
}
inline void Nand_Write_Addr( UINT32 addrLow, UINT32 addrHigh, UINT8 cycle )
{
	UINT8 AddrFlash[5];
	UINT8 i;

	for(i=0; i<sizeof(addrLow); i++) {
		AddrFlash[i] = (addrLow >> (i << 3));
	}
	
	if(cycle > 4) {
		AddrFlash[4] = addrHigh;
	}

	for(i=0; i<cycle; i++)
	{
		W8(NFC_ADDR, AddrFlash[i]);
	}
}

//#define CS_USE_GPIO_OUT_NUM		6 //GPIO_A[1]
//#define CS_USE_GPIO_OUT_NUM		2 //GPIO_A[1]		//test board CE use M2
#define CS_USE_GPIO_OUT_NUM			7 //GPIO_A[1]		//use board CE use M7
//#define RB_USE_GPIO_IN_NUM		0					//test board RD use M0
#define RB_USE_GPIO_IN_NUM			6					//use board RD use M6
/* gpio or eport to nand CE# 
 * */
void Nand_use_gpio_out_config( void )
{
	UINT8 gpio_num = CS_USE_GPIO_OUT_NUM;
	
 	GPIO_IBE_REG &= ~ (1 <<gpio_num);
 	
 	//DIR out
 	GPIO_DIR_REG |= (1 <<gpio_num);
 	
    //dis open-drain
 	GPIO_ODE_REG   = 0x00000000;
 	GPIO_INTEN_REG = 0x00000000;
 	
 	//GPIO mode
 	GPIO_CTL_REG |= (1 <<gpio_num);

	GPIO_OUT_REG |= (1 <<gpio_num);
//	GPIO_OUT_REG &= ~(1 <<gpio_num);
}
/* gpio or eport to nand R/B# 
 * */
void Nand_use_gpio_in_config( void )
{
	UINT8 gpio_num = RB_USE_GPIO_IN_NUM;
	
 	GPIO_IBE_REG |= (1 <<gpio_num);
 	
 	//DIR in
 	GPIO_DIR_REG &= ~ (1 <<gpio_num);
 	
    //dis open-drain
 	GPIO_ODE_REG   = 0x00000000;
 	GPIO_INTEN_REG = 0x00000000;
 	
 	//GPIO mode
 	GPIO_CTL_REG |= (1 <<gpio_num);
 	
#if 0 	
 	while(1){
		PSPRINTF("GPIO_IN_REG = 0x%x\n", GPIO_IN_REG);
		delay(0xa000);
		//0x0/0x20000
 	}
#endif
}

inline void Nand_Chip_En( void )
{
	UINT8 gpio_num = CS_USE_GPIO_OUT_NUM;
	
	//Nand_Select_Chip
	GPIO_OUT_REG &= ~ (1 <<gpio_num);
}
inline void Nand_Chip_Dis( void )
{
	UINT8 gpio_num = CS_USE_GPIO_OUT_NUM;
	
	GPIO_OUT_REG |= (1 <<gpio_num);
}

/* RB# Waiting by interrupt or polling GPIO/EPORT flag 
 * */
void WaitforReady_pin( void )
{
	UINT8 gpio_num = RB_USE_GPIO_IN_NUM;
	UINT32 in_dat;
	UINT32 m_count = 0;
	
#if 1	
	//read pin
	//R/B# is HIGH when the	target is ready
	do{
		in_dat = GPIO_IN_REG;
		if( in_dat & (1 <<gpio_num) )
			break;
		m_count++;
	}
	while( m_count < 0x40000 );
	
	if( m_count == 0x40000)
	{
		uart_printf_buf("\n...WaitforReady_pin busy time out... \n");
		while(1) {}
	}
#endif
#if INTERRUPT		//int
	while(0 != g_flashState[chipNo])
	{
		m_count++;
		if(m_count > LimitRB)
		{
			g_flashState[chipNo] = 0;
			return RB_TIMEOUT;
		}
	}
	return RB_READY;
#endif	
}

/* returns the status of the most recently selected die (LUN).
 * */
void WaitForReady_sts( UINT8 flg )
{
	UINT8 status;
	UINT32 m_count = 0;
	
	do{
		status = 0;
		
		Nand_Write_Cmd( CMD_READ_STATUS );	
		status = R8( NFC_D_NONECC );
		
		if ( !(status & STS_WPn) 
			&& flg	
			){
			PSPRINTF("\n...READ STATUS(0x%x): Write Protect(WP#=GND),can not Erase or Program... \n",status );
			while(1) {}
		}		
		if ( status & STS_RDY ){
			if ( status & STS_FAILC ){
			//	return RB_TIMEOUT;		
			}else{
				break;
			}
		}
			
		m_count++;
	}
	while( m_count < 0x100 );
	
	if( m_count == 0x100)
	{
		PSPRINTF("\n...WaitForReady_sts(0x%x) busy time out... \n",status );
		while(1) {}
	}
}
//-------------------------------------------------------------------------------------------------------------------------

void Nand_Init(void)
{
	UINT32 i, j;
	UINT32 tmp;
	UINT8 buf[10]={0,0,0,0,0};

#if 1
	Nand_use_gpio_out_config();
//	Nand_use_eport_out_config();
	
	Nand_use_gpio_in_config();
//	Nand_use_eport_in_config();
#endif	
	/*
	 * 20M:CS_WATS1|CS_WNTS2|CS_RATS1|CS_RNTS0|CS_WS9
	 * 37.5M:CS_WATS0|CS_WNTS1|CS_RATS0|CS_RNTS1|CS_WS8
	 */
	/* Nand-Flash Chip Select Control */
	W32( NFCSCR,
			CS_WATS0|CS_WNTS1|CS_RATS0|CS_RNTS1
		#if 1		
			|CS_ENDIAN  //D[7:0]
		#else
		//	|(0 <<14)	//D[31:24]
		#endif
			//8 bit port
			|CS_PS8
			|CS_WS8
			|CS_CSEN );
	
	/* Nand Flash Ports Control */
	tmp = R32( NPCR );
	W32( NPCR,tmp
		#if 1
			|(0 <<24)
		#else
		//	| NDINV	//Data Invert
		#endif
			|CS_PULLUPEN3
			|(1<<21) 
			|FWP_DIS
			);	
	
#if 1
	MIM_NFC_RegTest();
#endif
}

void Nand_Reset( void )
{
	Nand_Chip_En();
	
	Nand_Write_Cmd( CMD_RESET );	

#if 1
	WaitforReady_pin();
#else
	WaitForReady_sts( 0 );
#endif
}

void Nand_Read_ID( UINT8 *p_ID_Buff )
{
	UINT8 i=0;
			
	Nand_Chip_En();
	
	//set reading ID command
	Nand_Write_Cmd( CMD_READ_ID );	

	Nand_Write_Addr( 0x0,0x0,1 );
//	W8( NFC_ADDR, 0x00 );
	
	delay(10);
	
	for(i=0;i<8;i++)
	{
		p_ID_Buff[i] = R8( NFC_D_NONECC );
	}
}
void Nand_Read_ONFI_ID( UINT8 *p_ID_Buff )
{
	UINT8 i=0;
		
	Nand_Chip_En();
	
	//set reading ID command
	Nand_Write_Cmd( CMD_READ_ID );	

	Nand_Write_Addr( 0x20,0x0,1 );
//	W8( NFC_ADDR, 0x00 );
	
	delay(10);
	
	for(i=0;i<5;i++)
	{
		p_ID_Buff[i] = R8( NFC_D_NONECC );
	}
}

void Nand_Page_Read( UINT8 *DataBuf,UINT32 Blen,
					UINT32 NandLowAddr,UINT32 NandHighAddr )
{
	UINT32 i=0;
#if MIM_DMA_TEST	
	
	UINT32 sts=0;
	
	mim_dma_config( 
				#if 1	
					(U32)DataBuf + 0x0,
				#else 
					//test sram decrement
					(U32)DataBuf + Blen, 
				#endif			
					 NFC_D_NONECC,
					 Blen,
				//	 1,   test
					 
						 DCRn_TDIR_P2B //---
					#if DMA_CHANNELn_DONE_INTC_TEST
						|DCRn_DONEIE
					#endif
					#if 1 
						|DCRn_WEN_FINI
					#else
						|DCRn_WEN_INFI
					#endif
					#if 1
						|DCRn_BNUM_SRAM 	|DCRn_BINC 
					#else
						//test sram decrement
						|DCRn_BNUM_SRAM 	|DCRn_BDEC 
					#endif	

					#if BYTE_TEST
						|DCRn_TSIZ_BYTE 
					#endif						
					#if HALFWORD_TEST
						|DCRn_TSIZ_HALF
					#endif
					#if WORD_TEST
						|DCRn_TSIZ_WORD
					#endif		
					#if 1//AUTO_TEST
						|DCRn_TSIZ_AUTO
					#endif							
						|DCRn_P_FIX,
				#if DMA_CHANNEL_0_TEST	
					MIM_DMA_CH0
				#endif
				#if DMA_CHANNEL_1_TEST	
					MIM_DMA_CH1
				#endif
					);
	
//	MIM_DMA_RegTest();
#endif		
	
	Nand_Chip_En();
	
	//read mode
	Nand_Write_Cmd( CMD_READ_MODE );	

	Nand_Write_Addr( NandLowAddr,NandHighAddr,5 );
	
	Nand_Write_Cmd( CMD_READ_PAGE );
	
	delay(100);
	
#if 1
	WaitforReady_pin();
#else
	WaitForReady_sts( 0 );
#endif
	
	delay(1000);
	
#if (!MIM_DMA_TEST)
	for(i=0;i<Blen;i++)
	{
		DataBuf[i] = R8( NFC_D_NONECC );
	}
#else
	#if DMA_CHANNEL_0_TEST		
		mim_dma_ch_start( MIM_DMA_CH0 ); 
		sts = R16( DMA_DSR );
		
		#if DMA_CHANNELn_DONE_INTC_TEST
			while( !g_mim_dma_done_intc_flg ){}
			g_mim_dma_done_intc_flg = 0;
		#else
			while( !(sts & DSR_DONE0) ) // wait done
			{
				sts = R16( DMA_DSR ); 
			}
			W16( DMA_DSR, sts| DSR_DONE0 ); // clear done	
		#endif
	#endif		
	#if DMA_CHANNEL_1_TEST	
		mim_dma_ch_start( MIM_DMA_CH1 );
		sts = R16( DMA_DSR );
		
		#if DMA_CHANNELn_DONE_INTC_TEST
			while( !g_mim_dma_done_intc_flg ){}
			g_mim_dma_done_intc_flg = 0;
		#else	
			while( !(sts & DSR_DONE1) ) // wait done
			{
				sts = R16( DMA_DSR );
			}
			W16( DMA_DSR, sts| DSR_DONE1 ); // clear done
		#endif	
	#endif

#endif
}

void Nand_Page_Write( UINT8 *DataBuf,UINT32 Blen,
					UINT32 NandLowAddr,UINT32 NandHighAddr )
{
	UINT32 i=0;
#if MIM_DMA_TEST	
	
	UINT32 sts=0;
	
	mim_dma_config( 
				#if 1	
					(U32)DataBuf + 0x0,
				#else 
					//test sram decrement
					(U32)DataBuf + Blen, 
				#endif		
					 NFC_D_NONECC,
					 Blen,
					 
					 	 DCRn_TDIR_B2P //---
					#if DMA_CHANNELn_DONE_INTC_TEST
						|DCRn_DONEIE
					#endif
						|DCRn_WEN_FINI
					#if 1
						|DCRn_BNUM_SRAM 	|DCRn_BINC 
					#else
						//test sram decrement
						|DCRn_BNUM_SRAM 	|DCRn_BDEC 
					#endif	

					#if BYTE_TEST
						|DCRn_TSIZ_BYTE 
					#endif						
					#if HALFWORD_TEST
						|DCRn_TSIZ_HALF
					#endif
					#if WORD_TEST
						|DCRn_TSIZ_WORD
					#endif		
					#if 1//AUTO_TEST
						|DCRn_TSIZ_AUTO
					#endif							
						|DCRn_P_FIX,
					
				#if DMA_CHANNEL_0_TEST	
					MIM_DMA_CH0
				#endif
				#if DMA_CHANNEL_1_TEST	
					MIM_DMA_CH1
				#endif
					);	
//	MIM_DMA_RegTest();
#endif
	
	Nand_Chip_En();
	
	Nand_Write_Cmd( CMD_PROGRAM_MODE );	

	Nand_Write_Addr( NandLowAddr,NandHighAddr,5 );
	
	delay(1000);
	
#if (!MIM_DMA_TEST)
	for(i=0;i<Blen;i++)
	{
		W8( NFC_D_NONECC,DataBuf[i] );
	}	
#else
	#if DMA_CHANNEL_0_TEST		
		mim_dma_ch_start( MIM_DMA_CH0 ); 
		sts = R16( DMA_DSR );
		
		#if DMA_CHANNELn_DONE_INTC_TEST
			while( !g_mim_dma_done_intc_flg ){}
			g_mim_dma_done_intc_flg = 0;
		#else
			while( !(sts & DSR_DONE0) ) // wait done
			{
				sts = R16( DMA_DSR ); 
			}
			W16( DMA_DSR, sts| DSR_DONE0 ); // clear done	
		#endif
	#endif		
	#if DMA_CHANNEL_1_TEST	
		mim_dma_ch_start( MIM_DMA_CH1 );
		sts = R16( DMA_DSR );
		
		#if DMA_CHANNELn_DONE_INTC_TEST
			while( !g_mim_dma_done_intc_flg ){}
			g_mim_dma_done_intc_flg = 0;
		#else	
			while( !(sts & DSR_DONE1) ) // wait done
			{
				sts = R16( DMA_DSR );
			}
			W16( DMA_DSR, sts| DSR_DONE1 ); // clear done
		#endif	
	#endif
#endif	
	
    delay(1000);
			
	Nand_Write_Cmd( CMD_PROGRAM_PAGE );
	
    delay(100);
	
#if 1
	WaitforReady_pin();
#else
	WaitForReady_sts( 1 );
#endif
		
}

void Nand_Block_Erase( UINT32 NandLowAddr,UINT32 NandHighAddr )
{
	Nand_Chip_En();
	
	Nand_Write_Cmd( CMD_ERASE_MODE );	

	Nand_Write_Addr( NandLowAddr,NandHighAddr,3 );
	
	Nand_Write_Cmd( CMD_ERASE_BLOCK );
	
#if 1
	WaitforReady_pin();
#else
	WaitForReady_sts( 1 );
#endif
		
}

void mim_Nand_test( void )
{
	UINT32 test_data;
#if DEBUG_PRINT
	UINT32 i;
#endif
	
#if 1
	/* reg test
	 * */
	MIM_NFC_RegTest();
#endif
	
//	Nand_Init();
	Nand_Reset();
	
	for( i=0;i<16;i++ )
		*(UINT8*)(MIM_TEST_BUF_BASE +i) = 0;
			
#if 1
	Nand_Read_ID( (UINT8*)(MIM_TEST_BUF_BASE +0) );
	#if DEBUG_PRINT
		//0x2c,0x64,0x44,0x4b,0xa9,
		PSPRINTF(" read id =0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x...\r\n",
				*(UINT8*)(MIM_TEST_BUF_BASE +0),*(UINT8*)(MIM_TEST_BUF_BASE +1),
				*(UINT8*)(MIM_TEST_BUF_BASE +2),*(UINT8*)(MIM_TEST_BUF_BASE +3),
				*(UINT8*)(MIM_TEST_BUF_BASE +4),*(UINT8*)(MIM_TEST_BUF_BASE +5),
				*(UINT8*)(MIM_TEST_BUF_BASE +6),*(UINT8*)(MIM_TEST_BUF_BASE +7) );
	#endif
	delay(1000);
	
	Nand_Read_ONFI_ID( (UINT8*)(MIM_TEST_BUF_BASE +10) );
	#if DEBUG_PRINT
		//0x4f,0x4e,0x46,0x49
		PSPRINTF(" read ONFI ID =0x%x,0x%x,0x%x,0x%x...\r\n",
				*(UINT8*)(MIM_TEST_BUF_BASE +10),*(UINT8*)(MIM_TEST_BUF_BASE +11),
				*(UINT8*)(MIM_TEST_BUF_BASE +12),*(UINT8*)(MIM_TEST_BUF_BASE +13) );
	#endif
#endif
	
#if 1
	Nand_Block_Erase( 0,0 );	

	
	for( i=0; i<(512/4); i++) {
			*( (UINT32*)(MIM_TEST_BUF_BASE +0x0) + i) = 0x00000000; 
	}
	Nand_Page_Read( (UINT8*)(MIM_TEST_BUF_BASE +0x0),512,  0,0  );	
	#if DEBUG_PRINT
	for( i=0; i<(512/4); i++) {
		PSPRINTF(" (0x%x) =0x%x.", (UINT32*)(MIM_TEST_BUF_BASE +0x0)  +i,
			*( (UINT32*)(MIM_TEST_BUF_BASE +0x0)  +i) );
	}
	uart_printf_buf("\n\n");
	#endif	
#endif
#if 1
	test_data = 0x12345678; //0x12345678 0x13572468 0x5a5a55aa
	for( i=0; i<(512/4); i++) {
			*( (UINT32*)MIM_TEST_BUF_BASE +0x0 + i) = test_data + i;
	}
	Nand_Page_Write( (UINT8*)(MIM_TEST_BUF_BASE +0x0),512,  0,0  );	
	
	delay(10000);
	
	for( i=0; i<(512/4); i++) {
			*( (UINT32*)(MIM_TEST_BUF_BASE +0x0) + i) = 0xFFFFFFFF; 
	}	
	Nand_Page_Read( (UINT8*)(MIM_TEST_BUF_BASE +0x0),512,  0,0  );	
	#if DEBUG_PRINT
	for( i=0; i<(512/4); i++) {
		PSPRINTF(" (0x%x) =0x%x.", (UINT32*)(MIM_TEST_BUF_BASE +0x0)  +i,
			*( (UINT32*)(MIM_TEST_BUF_BASE +0x0)  +i)  );
	}
	uart_printf_buf("\n\n");
	#endif	
	for( i=0; i<(512/4); i++){
		if( *( (UINT32*)MIM_TEST_BUF_BASE+i) != (test_data + i) )  {
			PSPRINTF(" read (0x%x) = 0x%x (!= 0x%x ?).", (UINT32*)(MIM_TEST_BUF_BASE +0x0)  +i,
				*( (UINT32*)(MIM_TEST_BUF_BASE +0x0)  +i),
				(test_data + i)  );
			while(1) {}
		}
	}
#endif		
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

/* Memory Integration Module
 */
void mim_main( void )
{
	unsigned int val;
	//ccm_sys_axi  IO_MULTIPLEX	0x028
		PSPRINTF("CCM-[AXIDP_EN] 	= 0x%x\n", val=R32( (0xEB009000+0x000)) );
		PSPRINTF("CCM-[ENDIAN] 		= 0x%x\n", val=R32( (0xEB009000+0x008)) );
		#if 0
			W32( (0xEB009000 + 0x008),val 
										| (1 <<11) 
									//	| (1 <<10) 
										);
		#endif
		PSPRINTF("CCM-[ENDIAN] 2 = 0x%x(MIM_AHB2CLB_ENDIAN = %s)(MIM_AHB2OPB_ENDIAN = %s)\n", 
									val=R32( (0xEB009000+0x008)),
									(val & (1 <<11)) ? "MSB...":"LSB...",
									(val & (1 <<10)) ? "MSB...":"LSB..."
									);	
		
		PSPRINTF("CCM-[PCIE_TYPE]	= 0x%x\n", val=R32( (0xEB009000+0x00C)) );
		PSPRINTF("CCM-[CPU_EN] 		= 0x%x\n", val=R32( (0xEB009000+0x018)) );
		PSPRINTF("CCM-[STRAP_PIN] 	= 0x%x\n", val=R32( (0xEB009000+0x01C)) );
		PSPRINTF("CCM-[BOOT_STAT] 	= 0x%x\n", val=R32( (0xEB009000+0x024)) );
		
		val = R32( (0xEB009000 + 0x028) );
		PSPRINTF("CCM-[IO_MULTIPLEX] = 0x%x\n", val );
		#if 1
			//NFC_EN
			W32( (0xEB009000 + 0x028),val | (1 <<7) );
		#endif
		PSPRINTF("CCM-[IO_MULTIPLEX] 2 = 0x%x(%s)\n",
									val=R32( (0xEB009000+0x028)),
									(val & (1 <<7)) ? "NFC IO EN...":"NFC IO DIS..."
									);		

	Nand_Init();
	
#if BCH_MIM_NONECC_TEST	
	mim_Nand_test();
	
	Nand_Chip_Dis();
#endif
	
#if MIM_DMA_TEST
	MIM_DMA_RegTest();
	
	mim_dma_disable();
#endif	
	
//	MIM_NFC_RegTest();
	
#if 0	
	bch_test_main();
#endif
	//NFC_not_EN
	val = R32( (0xEB009000 + 0x028) );
	val &= ~ (1 <<7);
	W32( (0xEB009000 + 0x028),val);
}


 
 





