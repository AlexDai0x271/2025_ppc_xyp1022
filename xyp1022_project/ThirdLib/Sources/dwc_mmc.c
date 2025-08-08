/*******************************************************
 * File: dwc_mmc.c
 * 	Synopsys DesignWare Multimedia Card Interface driver
 * 	DesignWare Cores Mobile Storage Host
 * Date: 2017
 * Author: ccore sys
 ******************************************************/

//#include "as_archppc470.h"
//#include "ppclib.h"
#include "uart.h"
#include "common.h"
#include "gpio.h"
#include "e500mc_asm.h"

#include "sd_mmc_card.h"
#include "dwc_mmc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Hardware Configuration Register
 * */
void SDIO_HCON(void)
{
	HCON_Reg_t hcon_reg;

	(*(U32*)(&hcon_reg)) = R32(SDMMC_HCON);

#if 0
//test LSB
	uart_printf("SDIO_HCON() = %d...\r\n", hcon_reg.card_type ); //0
	uart_printf("SDIO_HCON() = %d...\r\n", hcon_reg.num_card );  //2
	uart_printf("SDIO_HCON() = %d...\r\n", hcon_reg.h_bus_type );//0
#endif

	if( hcon_reg.card_type != CARD_TYPE_SD_MMC )
	{
		while(1){}  //asm("trap");
	}

	/*
	* Get the host data width - this assumes that HCON has been set with the correct values.
	*/
	uart_printf("SDMMC_HCON read =0x%x=%s,NUM_CARDS(%d),%s,[**]H_DATA(%dbits),H_ADDR(%dbits),%s,%s\r\n",
			R32(SDMMC_HCON),
			hcon_reg.card_type	?"CARD_TYPE_SD_MMC":"CARD_TYPE_MMC_ONLY",
			hcon_reg.num_card +1,
			hcon_reg.h_bus_type	?"H_BUS_TYPE_AHB":"H_BUS_TYPE_APB",
			hcon_reg.h_data_width*32,
			hcon_reg.h_addr_width +1,
			hcon_reg.fifo_ram_inside?"FIFO_RAM_INSIDE":"FIFO_RAM_OUTSIDE",
			hcon_reg.addr_config	?"Support_ADDR_64BIT":"Support_ADDR_32BIT"
			);
}

/*
 * Polling Status Register
 * 	wait one flag or some flags come.
 */
U8 SDIO_PollStatus(U32 flag_s)
{
	U32 status = 0;
	U32 poll_cnt = 0;

	status = R32(SDMMC_RINTSTS);
	while((status & flag_s)!= flag_s)
	{
		status = R32(SDMMC_RINTSTS);
		if (flag_s == SDMMC_INT_CMD_DONE) //only CMDDONE is expected
		{
			poll_cnt++;
			if (poll_cnt == RESP_TIMEOUT){ //software detect Response timeout
				while(1){}  //asm("trap");
				return 1;
			}
		}
	}

	//clear
	W32(SDMMC_RINTSTS, flag_s);
	return 0;
}


/*
 * Polling CMD[31]
 * 	Once command is taken by CIU, bit is cleared.
 */
void SDIO_Start_CMD(void)
{
	U32 status = 0;

	status = R32(SDMMC_CMD);
	while ( status & SDMMC_CMD_START )
	{
		status = R32(SDMMC_CMD);  //Hardware
	}
}

/* Clock Divider & Clock Source for slot_N */
void SDIO_SetClk(U32 slot_num, U32 div_val)
{
	U8 Clk_Source_n;
#if 1//DEBUG_PRINT
	U32 tmp;
#endif

	/*
	 * Before disabling the clocks,
	 *  ensure that the card is not busy due to any previous data command.
	 * To determine this, check for 0 in bit 9 of the STATUS register.
	 */
	while( R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY );


	/* disable clock to CIU */
	W32( SDMMC_CLKENA, 0x00000000 );
	/* inform CIU */
	W32( SDMMC_CMD, SDMMC_CMD_START
					|SDMMC_CMD_UPD_CLK
					|SDMMC_CMD_PRV_DAT_WAIT
					|( (slot_num-1) <<16) );
	SDIO_Start_CMD();


	/* set clock to desired speed */
	Clk_Source_n = SDMMC_CLK_SOURCE_0;
//	Clk_Source_n = SDMMC_CLK_SOURCE_1;
//	Clk_Source_n = SDMMC_CLK_SOURCE_2;
//	Clk_Source_n = SDMMC_CLK_SOURCE_3;
	W32(SDMMC_CLKSRC, Clk_Source_n << ((slot_num-1)*2)  );
	W32(SDMMC_CLKDIV, div_val << (Clk_Source_n * 8)  );
	/* inform CIU */
	W32(SDMMC_CMD, SDMMC_CMD_START
					|SDMMC_CMD_UPD_CLK
					|SDMMC_CMD_PRV_DAT_WAIT
					|( (slot_num-1) <<16) );
	SDIO_Start_CMD();


	/* enable clock */
#if 1
	//continuous
	W32(SDMMC_CLKENA, (0x0000<<SDMMC_CLKEN_LOW_PWR) | (slot_num <<SDMMC_CLKEN_ENABLE));
#else
	//stop clock when card in IDLE
	W32(SDMMC_CLKENA, (slot_num<<SDMMC_CLKEN_LOW_PWR) | (slot_num <<SDMMC_CLKEN_ENABLE));
#endif
	/* inform CIU */
	W32(SDMMC_CMD, SDMMC_CMD_START |SDMMC_CMD_UPD_CLK
					|SDMMC_CMD_PRV_DAT_WAIT
					|( (slot_num-1) <<16) );
	SDIO_Start_CMD();

#if 1
	delay( 0x5000 ) ;
#endif
#if 1//DEBUG_PRINT
	W32(SDMMC_CLKDIV, div_val << (Clk_Source_n * 8)  );
	tmp = R32(SDMMC_CLKDIV);
	tmp = (tmp >> (Clk_Source_n * 8) ) & 0xff;
	uart_printf(" [######### set clk: div=%d (%d)MHz-(%d)KHz]\r\n" ,
			tmp,
			(tmp==0)? (SDIO_CLK_IN/1000) : (SDIO_CLK_IN/tmp/2000),
			(tmp==0)? (SDIO_CLK_IN/1) : (SDIO_CLK_IN/tmp/2)
			);
#endif
}

/*
 *  for Voltage Switching
 */
void SDIO_SwitchClk( U8 flag,U8 card_number )
{
	if(flag == 0) //stop clock
	{
		/* disable clock to CIU */
		W32(SDMMC_CLKENA, 0);

		/* inform CIU */
		W32(SDMMC_CMD, SDMMC_CMD_START
					|SDMMC_CMD_UPD_CLK
					|SDMMC_CMD_PRV_DAT_WAIT
					|SDMMC_CMD_VOLT_SWITCH
					|(card_number <<16)   );
		SDIO_Start_CMD();
	}
	else
	{
		/* enable clock */
		W32(SDMMC_CLKENA, (1<<card_number) <<SDMMC_CLKEN_ENABLE);

		/* inform CIU */
		W32(SDMMC_CMD, SDMMC_CMD_START
					|SDMMC_CMD_UPD_CLK
					|SDMMC_CMD_PRV_DAT_WAIT
					|SDMMC_CMD_VOLT_SWITCH
					|(card_number <<16)   );
		SDIO_Start_CMD();
	}
}


//----------------------------interrupt-----------------------------------------------------
#if SDMMC_HOST_INT_TEST

void SD_enable_sdio_irq( U32 slot_num, U8 enb )
{
	U32 int_mask;

	/* Enable/disable Slot Specific SDIO interrupt
	 * */
	int_mask = R32(SDMMC_INTMASK);
	if (enb) {
		W32( SDMMC_INTMASK,  (int_mask | SDMMC_INT_SDIO(slot_num))   );

	} else {
		W32( SDMMC_INTMASK,  (int_mask & ~(SDMMC_INT_SDIO(slot_num)))   );
	}
}



//volatile U8 g_intr_RTO = 0;
volatile U8 g_intr_RXDR = 0;
volatile U8 g_intr_TXDR = 0;
volatile U8 g_intr_DATA_OVER = 0;
volatile U8 g_intr_CMD_DONE = 0;
volatile U8 g_intr_CD = 0;
#if SDMMC_HOST_SD30_1D8V_TEST
	volatile U8 g_intr_VOIT_SWITCH = 0;
#endif
#if USE_INTERNAL_DMAC
	//volatile U8 g_intr_DMA = 0;
#endif
void SD_interrupt_isr( void )
{
	U8 pass_count = 0;
	volatile U32 pending;
	U8 i;

	do
	{
		pending = R32(SDMMC_MINTSTS); /* read-only mask reg */
		#if DEBUG_PRINT
			uart_printf(" isr()/M=0x%x/R_S=0x%x/S=0x%x: ",
					pending,R32(SDMMC_RINTSTS),R32(SDMMC_STATUS) );
		#endif

		if (!pending)
			break;

		#if SDMMC_HOST_SD30_1D8V_TEST
			if (pending & SDMMC_INT_VOLT_SWITCH) {
				g_intr_VOIT_SWITCH = 1;
				W32( SDMMC_RINTSTS,  SDMMC_INT_VOLT_SWITCH  );
				pending &= ~SDMMC_INT_VOLT_SWITCH;

				#if 0//DEBUG_PRINT
				uart_printf(" ---------VOLT_SWITCH.." );
				#endif
			}
		#endif

		if (pending & DWC_MSH_CMD_ERROR_FLAGS) {
		//	g_intr_RTO = 1;
			W32( SDMMC_RINTSTS,  DWC_MSH_CMD_ERROR_FLAGS  );

			#if DEBUG_PRINT
			uart_printf(" ---------CMD_ERROR.." );
			#endif
		}
		if (pending & DWC_MSH_DATA_ERROR_FLAGS) {
			W32( SDMMC_RINTSTS,  DWC_MSH_DATA_ERROR_FLAGS  );

			#if DEBUG_PRINT
			uart_printf(" ---------DATA_ERROR.." );
			#endif
		}
	#if USE_INTERNAL_DMAC
		if (pending & SDMMC_INT_FRUN) {
			W32( SDMMC_RINTSTS,  SDMMC_INT_FRUN  );

			#if DEBUG_PRINT
			uart_printf(" ---------underrun/overrun.." );
			#endif
		}
	#endif

		if (pending & SDMMC_INT_DATA_OVER) {
			g_intr_DATA_OVER = 1;
			W32( SDMMC_RINTSTS,  SDMMC_INT_DATA_OVER  );

			#if DEBUG_PRINT
			uart_printf(" DATA_OVER." );
			#endif
		}

		if (pending & SDMMC_INT_RXDR) {
			g_intr_RXDR = 1;
			W32( SDMMC_RINTSTS,  SDMMC_INT_RXDR  );

			#if DEBUG_PRINT
			uart_printf("RXDR:FIFO >(Receive_Threshold).." ); //Word
			#endif
		}
		if (pending & SDMMC_INT_TXDR) {
			g_intr_TXDR = 1;
			W32( SDMMC_RINTSTS,  SDMMC_INT_TXDR  );

			#if DEBUG_PRINT
			uart_printf("TXDR:FIFO <=(Transmit-Threshold).." );
			#endif
		}


		if (pending & SDMMC_INT_CMD_DONE) {
			g_intr_CMD_DONE = 1;
			W32( SDMMC_RINTSTS,  SDMMC_INT_CMD_DONE  );

			#if 0//DEBUG_PRINT
			uart_printf(" CMD_DONE.." );
			#endif
		}
		if (pending & SDMMC_INT_CD) {
			g_intr_CD = 1;
			W32( SDMMC_RINTSTS,  SDMMC_INT_CD  );

			#if DEBUG_PRINT
			uart_printf(" Card detect..(SDMMC_CDETECT =0x%x)",R32(SDMMC_CDETECT) );
			#endif
		}
	#if 1
		/* Handle SDIO Interrupts */
		for (i = 0; i < 2; i++) {
			if ( pending & SDMMC_INT_SDIO(i) ) {
				W32( SDMMC_RINTSTS,  SDMMC_INT_SDIO(i)  );

				#if DEBUG_PRINT
				uart_printf(" Interrupt from SDIO card/slot(%d).." ,i);
				#endif
			}
		}
	#endif

		#if DEBUG_PRINT
		uart_printf("\r\n");
		#endif
	}
	while (pass_count++ < 5);

#if USE_INTERNAL_DMAC
	/* Handle DMA interrupts */
	pending = R32(SDMMC_IDSTS);
	if (pending & (SDMMC_IDMAC_INT_TI | SDMMC_IDMAC_INT_RI) )
	{
	//	g_intr_DMA = 1;
		W32( SDMMC_IDSTS, SDMMC_IDMAC_INT_TI | SDMMC_IDMAC_INT_RI );
		W32( SDMMC_IDSTS, SDMMC_IDMAC_INT_NI );
	}

	#if DEBUG_PRINT
//	uart_printf(" SD_interrupt()/IDSTS=0x%x.\r\n" ,pending);
	#endif
#endif
}

void SD_interrupt_isr_DTO( void )
{
	U8 pass_count = 0;
	volatile U32 pending;
	U8 i;

	do
	{
		pending = R32(SDMMC_MINTSTS); /* read-only mask reg */

		if (!pending)
			break;

		while((pending & SDMMC_INT_DATA_OVER) != SDMMC_INT_DATA_OVER) 
		{
			pending = R32(SDMMC_MINTSTS); 
		}
		
		g_intr_DATA_OVER = 1;
		W32( SDMMC_RINTSTS,  SDMMC_INT_DATA_OVER  );

	}
	while (pass_count++ < 5);

#if USE_INTERNAL_DMAC
	/* Handle DMA interrupts */
	pending = R32(SDMMC_IDSTS);
	if (pending & (SDMMC_IDMAC_INT_TI | SDMMC_IDMAC_INT_RI) )
	{
	//	g_intr_DMA = 1;
		W32( SDMMC_IDSTS, SDMMC_IDMAC_INT_TI | SDMMC_IDMAC_INT_RI );
		W32( SDMMC_IDSTS, SDMMC_IDMAC_INT_NI );
	}

	#if DEBUG_PRINT
//	uart_printf(" SD_interrupt()/IDSTS=0x%x.\r\n" ,pending);
	#endif
#endif
}
#endif

//----------------------------init-----------------------------------------------------
/*
 * Reset all blocks */
U8 SDIO_Reset(void)
{
	U32 ctrl,timeout = 1000;

	W32(SDMMC_CTRL, SDMMC_CTRL_RESET | SDMMC_CTRL_FIFO_RESET | SDMMC_CTRL_DMA_RESET);
	/* wait till resets clear */
	do{
		ctrl = R32(SDMMC_CTRL);
		if( !(ctrl & (SDMMC_CTRL_RESET | SDMMC_CTRL_FIFO_RESET | SDMMC_CTRL_DMA_RESET)) )
			return 0;
	}while(timeout--);

	return 1;
}
U8 SDIO_FIFO_Reset(void)
{
	U32 ctrl,timeout = 1000,data;

	data = R32(SDMMC_CTRL);
	data |= SDMMC_CTRL_FIFO_RESET;
	W32(SDMMC_CTRL, data);
	/* wait till resets clear */
	do{
		ctrl = R32(SDMMC_CTRL);
		if( !(ctrl & SDMMC_CTRL_FIFO_RESET) )
			return 0;
	}while(timeout--);

	return 1;
}

/*
 *  Interrupt + FIFO
 *  */
void SDIO_Init(void)
{
	U32 fifo_size,tmp;

	/* Clear(Wr 1) the interrupts for the host controller */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);

	/* 0=disable all sdio/mmc interrupt first */
	W32(SDMMC_INTMASK, 0x00000000);

	/* Put in max timeout */
	W32(SDMMC_TMOUT, 0xFFFFFFFF);

	/* init 0:1-bit mode */
#if SD_HOST_TEST
	W32(SDMMC_CTYPE, SDMMC_CTYPE_1BIT <<SD_SLOT1 );
#else
	W32(SDMMC_CTYPE, SDMMC_CTYPE_1BIT <<EMMC_SLOT0 );
#endif

#if 1
	/* debounce filter */
	W32(SDMMC_DEBNCE, 0x00ffffff);
#endif
	
#if 0
	//shift: drive(write timing),
	//sample(read timing)  --0,1,2,3
	W32(SDMMC_UHS_EXT, 1<<23|3<<16);
#endif

#if 1
	/*
	 * FIFO threshold settings  RxMark  = fifo_size / 2 - 1,
	 *                          Tx Mark = fifo_size / 2 DMA Size = 8
	 */
	#if 1
		/*
		 * Power-on value of RX_WMark is FIFO_DEPTH-1, but this may
		 * have been overwritten by the bootloader, just like we're
		 * about to do, so if you know the value for your hardware, you
		 * should put it in the platform data.
		 */
		fifo_size = R32( SDMMC_FIFOTH );
		fifo_size = 1 + ((fifo_size >> 16) & 0xfff);
	#else
		fifo_size = FIFO_DEPTH;
	#endif

//	if( CHIP_CCP908T )
//		fifo_size = 0x400;

#if 0
		//former
	if( CHIP_S1020 )
		fifo_size = 0x400;
	W32( SDMMC_FIFOTH ,	(0x2 << 28)	| ((fifo_size/2 - 1) << 16)	| ((fifo_size/2) << 0)  );
#else
	if( CHIP_S1020 )
		fifo_size = 0x400;
	W32( SDMMC_FIFOTH ,	(0x6 << 28)	| ((fifo_size/2 - 1) << 16)	| ((fifo_size/2) << 0)  );

#endif
/*test
	W32( SDMMC_FIFOTH ,( (0x2 << 28)| ((2/2 - 1) << 16)| ((fifo_size/2) << 0) ) );  0x20000080->RX_WMark+1=1W=4bytes
	W32( SDMMC_FIFOTH ,( (0x2 << 28)| ((258/2 - 1) << 16)| ((fifo_size/2) << 0) ) ); //
*/
	#if DEBUG_PRINT
		fifo_size = R32( SDMMC_FIFOTH );//0x27ff0800  //0x21ff0200
		uart_printf( " SDIO_Init()/SDMMC_FIFOTH = 0x%x\r\n",fifo_size );
	#endif
#endif


#if 0
	uart_printf( " DWC_MSH_ERROR_FLAGS = 0x%x\r\n",DWC_MSH_ERROR_FLAGS ); //0xb7c2
#endif

#if SDMMC_HOST_INT_TEST
	InstallInterruptHandler0( SDMMC_HOST_INT_NUM,SD_interrupt_isr );

	tmp = 0x0;

	//test bit 0 ：C Card detect (CD)
	tmp |= SDMMC_INT_CD;

	//test bit 2：C Command done (CD)
	tmp |= SDMMC_INT_CMD_DONE;

	//bit 8 ：C Response timeout (RTO)
	tmp |= SDMMC_INT_RTO;

	#if 0
	//test
	//bit 5 ：C Receive FIFO data request (RXDR)
//	tmp |= SDMMC_INT_RXDR;
	//bit 4 - Transmit FIFO data request (TXDR)
//	tmp |= SDMMC_INT_TXDR;
	#endif

	//bit 3 ：C Data transfer over (DTO)
	tmp |= SDMMC_INT_DATA_OVER;

	#if SDMMC_HOST_SD30_1D8V_TEST
	//bit 10 ：C Volt_switch_int
	tmp |= SDMMC_INT_VOLT_SWITCH;
	#endif

	//bit 11 ：C FIFO underrun/overrun error (FRUN)
	tmp |= SDMMC_INT_FRUN;

	#if USE_INTERNAL_DMAC
		//3.1.4
		tmp &= ~SDMMC_INT_RXDR;
		tmp &= ~SDMMC_INT_TXDR;
	#endif
	W32(SDMMC_INTMASK, tmp );


	#if USE_INTERNAL_DMAC
//	tmp = R32(SDMMC_IDINTEN);
//	tmp |= SDMMC_IDMAC_INT_RI | SDMMC_IDMAC_INT_TI;
//	W32(SDMMC_IDINTEN, tmp );
	#endif

	//Global interrupt enable
	tmp = R32(SDMMC_CTRL);
	W32(SDMMC_CTRL,  tmp |SDMMC_CTRL_INT_ENABLE);
	#if 1
		uart_printf( " SDIO_Init()/INTMASK = 0x%x/CTRL = 0x%x\r\n",
				R32(SDMMC_INTMASK),
				R32(SDMMC_CTRL) );
	#endif
#endif
}

/* card detection for slot_N:
 * polling any changes in the card-detect signals for card insertion
 * */
U8 SDIO_CardDetect(void)
{
	U32 card_det;

	card_det = R32(SDMMC_CDETECT);
#if 0
	uart_printf(" 1.SDMMC_CDETECT = 0x%x.\r\n",card_det );  //0x03
#endif

#if EMMC_HOST_TEST
	//CARD_DETECT_N[0]
	#if EMMC_HOST_TEST
		card_det &= ~(1 <<EMMC_SLOT0);
	#endif
#endif
	if ( (card_det & (1 <<EMMC_SLOT0)) == SDMMC_CDETECT_CARD_PRESENT )
	{
		uart_printf(" [######### (DETECT=0x%x) Now,Detected one eMMC Card on SD_HOST EVB slot(%d)]\r\n\r\n",
				card_det,EMMC_SLOT0 );
		#if 1
			//905t/908t:NC
			//fpga:pin086-(VDDQ)
			SDIO_PowerUp( 1<<EMMC_SLOT0, 1 );

			#if	SDMMC_HOST_EMMC_1D8V_TEST
			SDIO_Card_Volt( 1<<EMMC_SLOT0, UHS_1D8 );
			#else
			SDIO_Card_Volt( 1<<EMMC_SLOT0, SD2_3D3 );
			#endif
			delay(0x5000);
		#endif

		#if FPGA_TEST
			//use pin088-for (VDD)
			SDIO_PowerUp( 1 <<SD_SLOT1,1 );
		#elif EVB_903T_TEST
			//en eMMC_VDDQ-use eMMC_POWER_EN1
			SDIO_PowerUp( 1 <<SD_SLOT1,1 );
			//en U19
		//	config GPIO M[0]
		#elif EVB_905T_TEST
			//CARD_VDD 	3.3
		#elif EVB_908T_TEST
			//SD0_VDD 	3.3
			//or GPIO_M4
		#endif

	  	//return SDIO_PollStatus(SDMMC_INT_CD);
		return 0;
	}



	if( (card_det & (1 <<SD_SLOT1)) == SDMMC_CDETECT_CARD_PRESENT )
	{
		uart_printf(" [######### (DETECT=0x%x) Now,Detected one SD/TF Card on SD_HOST EVB slot(%d)]\r\n\r\n",
				card_det,SD_SLOT1 );

	#if 1
		//905t/908t:NC
		//fpga:pin088-(VDD)/not used for SD.
		SDIO_PowerUp( 1<<SD_SLOT1, 1 );

		SDIO_Card_Volt( 1<<SD_SLOT1, SD2_3D3 );
		delay(0x5000);
	#endif

	#if FPGA_TEST
		//pin8=0/U10 en/default at 3.3V
	//	SDIO_Card_Volt( 1<<SD_SLOT1, SD2_3D3 );
	#endif
	#if EVB_903T_TEST
		//
	//	config GPIO M[4]/M[8]
		//En U13
	//	config GPIO M[13]

		//dis U15
		SDIO_Card_Volt( 1<<SD_SLOT1, 0 );
	#endif
	#if EVB_905T_TEST
		//905T EVB-CARD_VDD- use BIU_VOLT_REG[0]
		SDIO_Card_Volt( 1<<EMMC_SLOT0, SD2_3D3 );
	#endif
	#if EVB_908T_TEST
		//SD1_VDD 	3.3
		//or GPIO_M5
	#endif

	  	//return SDIO_PollStatus(SDMMC_INT_CD);
		return 0;
	}
	if ( ((card_det & (1 <<EMMC_SLOT0)) == (1 <<EMMC_SLOT0))
	  && ((card_det & (1 <<SD_SLOT1)) == (1 <<SD_SLOT1))  )
	{
		uart_printf(" [######### (DETECT=0x%x) No Card, Wait you to Insert one Card to SD_HOST...]\r\n\r\n",
				card_det);

		#if SDMMC_HOST_INT_TEST
			//SD_interrupt_isr
			while( g_intr_CD == 0 ) {}
			g_intr_CD = 0;
		#else
			/* wait until detect status change */
			SDIO_PollStatus( SDMMC_INT_CD );
		#endif

		return 1;
	}

	return 0;
}

//----------------------------power-----------------------------------------------------
/*
 * UHS_REG[VOLT_REG]
 */
void SDIO_Card_Volt(U32 slot_num, U8 flag)
{
	U32 uhs_1_reg;

	uhs_1_reg = R32(SDMMC_UHS_REG);
	if( flag == SD2_3D3 )
	{
		uhs_1_reg &= ~slot_num;
	}
	else
	{
		uhs_1_reg |= slot_num;  //1 ：C 1.8V
	}

	W32(SDMMC_UHS_REG, uhs_1_reg);
}

/*
 *  Power Enable Register
 */
void SDIO_PowerUp(U32 slot_num, U8 flag)
{
	U32 power;

	power = R32(SDMMC_PWREN);
	if(flag == 0) //off
	{
		power &= ~slot_num;
	}
	else //on
	{
		power |= slot_num;
	}

	W32(SDMMC_PWREN, power);
}

U8 SDIO_GetPowerState(U32 slot_num)
{
	U32 power;

	power = R32(SDMMC_PWREN) & slot_num;
	return (power == 0) ? 0 : 1;
}



	#if USE_INTERNAL_DMAC
void SDIO_Internal_Dmac_Ctrl(U8 flag)
{
	U32 data;

#if 1
	/* issue a IDMAC software reset */
	data = R32(SDMMC_BMOD);
	W32( SDMMC_BMOD, data|SDMMC_BMOD_SWR );
	data = R32(SDMMC_BMOD);
	while( data &  SDMMC_BMOD_SWR) {
		data = R32(SDMMC_BMOD);
	}
#endif

	data = R32(SDMMC_CTRL);
	if(flag == 1) //enable
	{
		data |= SDMMC_CTRL_USE_IDMAC;
		W32(SDMMC_CTRL, data);

		W32(SDMMC_BMOD, SDMMC_BMOD_IDMAC_EN| SDMMC_BMOD_FIXED_BURST );

		#if 0//DEBUG_PRINT
		uart_printf("SDIO_Internal_Dmac_Ctrl(): CTRL=0x%x,BMOD = 0x%x\r\n",
				R32(SDMMC_CTRL),R32(SDMMC_BMOD) );
		//CTRL=0x2000000,BMOD = 0x282
		#endif
	}
	else //disable
	{
		data &= ~SDMMC_CTRL_USE_IDMAC;
		W32(SDMMC_CTRL, data);

		data = R32(SDMMC_BMOD);
		data &= ~(SDMMC_BMOD_IDMAC_EN);
		W32(SDMMC_BMOD, data);
	}
}


/*	
 *  Dual buffer Descriptor
 *  one basic idma function:
 *  IDMAC DES0~DES7 for 64-bit addressing configuration
 */
void SDIO_Config_Idma_Desc_DUAL( U32 des_start_addr,
							U32 dbuf_start_addr,U32 total_len )
{
	U32 buf_len;
	struct idmac_desc_64addr *desc_first, *desc_last, *desc;

	desc_first = desc_last = desc = (struct idmac_desc_64addr *)des_start_addr;
	for ( ; total_len ; desc++) {
		desc->des0 = 0;
		desc->des1 = 0;
		desc->des2 = 0;
		desc->des3 = 0;
		desc->des4 = 0;
		desc->des5 = 0;
		desc->des6 = 0;
		desc->des7 = 0;

		buf_len = (total_len <= (FIFO_DEPTH <<2) ) ?
					total_len : (FIFO_DEPTH <<2);
		total_len -= buf_len;

		/* Set the OWN bit and disable interrupts for this descriptor */
		desc->des0 = IDMAC_DES0_OWN
					| IDMAC_DES0_DIC
					| IDMAC_DES0_CH;

		/* Buffer length */
		desc->des2 = (buf_len & (IDMAC_DESC2_BUFSIZE1 -1)) | (buf_len & (IDMAC_DESC2_BUFSIZE1 -1)<<12 );

		/* Physical address to DMA to/from */
		desc->des4 = dbuf_start_addr;
		desc->des5 = SDMMC_BUFFERs_BASE_HI;

		/* Forward link the descriptor list */
		des_start_addr += sizeof(struct idmac_desc_64addr);
		desc->des6 = des_start_addr;
		desc->des7 = SDMMC_BUFFERs_BASE_HI;

		/* Update physical address for the next desc */
		dbuf_start_addr += buf_len;

		/* Save pointer to the last descriptor */
		desc_last = desc;
	}

	/* Set first descriptor */
	desc_first->des0 |= IDMAC_DES0_FD;

	/* Set last descriptor */
	desc_last->des0 &= ~(IDMAC_DES0_CH | IDMAC_DES0_DIC);
	desc_last->des0 |= IDMAC_DES0_LD;

	#if DEBUG_PRINT
		uart_printf("SDIO_Config_Idma_Desc():from (0x%x)des = 0x%x,0x%x, 0x%x,0x%x, 0x%x\r\n",
				(U32)desc_first,
				desc_first->des0,	desc_first->des2,
				desc_first->des4,	desc_first->des5,
				desc_first->des6
				);
		//0x8000000c,0x200, 0xe0301000,0x80000030, 0xe0300020
		//0x8000000c,0x200, 0x1001000,0x80000000, 0x1000020
	#endif
}


/*	
 * 	one basic idma function:
 *  IDMAC DES0~DES7 for 64-bit addressing configuration
 */
void SDIO_Config_Idma_Desc( U32 des_start_addr,
							U32 dbuf_start_addr,U32 total_len )
{
	U32 buf_len;
	struct idmac_desc_64addr *desc_first, *desc_last, *desc;

	desc_first = desc_last = desc = (struct idmac_desc_64addr *)des_start_addr;
	for ( ; total_len ; desc++) {
		desc->des0 = 0;
		desc->des1 = 0;
		desc->des2 = 0;
		desc->des3 = 0;
		desc->des4 = 0;
		desc->des5 = 0;
		desc->des6 = 0;
		desc->des7 = 0;

		buf_len = (total_len <= (FIFO_DEPTH <<2) ) ?
					total_len : (FIFO_DEPTH <<2);
		total_len -= buf_len;

		/* Set the OWN bit and disable interrupts for this descriptor */
		desc->des0 = IDMAC_DES0_OWN
					| IDMAC_DES0_DIC
					| IDMAC_DES0_CH;

		/* Buffer length */
		desc->des2 = buf_len & (IDMAC_DESC2_BUFSIZE1 -1);

		/* Physical address to DMA to/from */
		desc->des4 = dbuf_start_addr;
		desc->des5 = SDMMC_BUFFERs_BASE_HI;

		/* Forward link the descriptor list */
		des_start_addr += sizeof(struct idmac_desc_64addr);
		desc->des6 = des_start_addr;
		desc->des7 = SDMMC_BUFFERs_BASE_HI;

		/* Update physical address for the next desc */
		dbuf_start_addr += buf_len;

		/* Save pointer to the last descriptor */
		desc_last = desc;
	}

	/* Set first descriptor */
	desc_first->des0 |= IDMAC_DES0_FD;

	/* Set last descriptor */
	desc_last->des0 &= ~(IDMAC_DES0_CH | IDMAC_DES0_DIC);
	desc_last->des0 |= IDMAC_DES0_LD;

	#if DEBUG_PRINT
		uart_printf("SDIO_Config_Idma_Desc():from (0x%x)des = 0x%x,0x%x, 0x%x,0x%x, 0x%x\r\n",
				(U32)desc_first,
				desc_first->des0,	desc_first->des2,
				desc_first->des4,	desc_first->des5,
				desc_first->des6
				);
		//0x8000000c,0x200, 0xe0301000,0x80000030, 0xe0300020
		//0x8000000c,0x200, 0x1001000,0x80000000, 0x1000020
	#endif
}
	#endif  //dma
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const char *R1_CurState_string( U8 val )
{
	switch (val) {
		case R1_STATE_IDLE:		return "IDLE";
		case R1_STATE_READY:	return "READY";
		case R1_STATE_IDENT:	return "IDENT";
		case R1_STATE_STBY:		return "STBY";
		case R1_STATE_TRAN:		return "TRAN";
		case R1_STATE_DATA:		return "DATA";
		case R1_STATE_RCV:		return "RCV";
		case R1_STATE_PRG:		return "PRG";
		case R1_STATE_DIS:		return "DIS";
	}
	return "UNKNOWN state";
}

#if 1//SD_HOST_TEST
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMD0_SD_tsk( U8 card_number )
{
	W32(SDMMC_CMDARG, 0x00000000 );

	/* No CMD response required */
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD		//?
			|SDMMC_CMD_INIT			//Send initialization sequence before sending this command
			|(card_number <<16)|SDMMC_CMD_INDX(0));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}	//SD_interrupt_isr()
	g_intr_CMD_DONE = 0;
#else
	//wait cmd done
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}


/* SEND_IF_COND (CMD8) :VHS   /same to ACMD41-OCR/ Send CMD8 to verify SD card interface operating condition
 *  Checks whether the card can operate on the host supply voltage.
 *   SD Version 2.00/valid when the card is in Idle state/issue CMD8 before ACMD41.
 * Argument: - [31:12]: Reserved (shall be set to '0')
		     - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
		     - [7:0]: Check Pattern (recommended 0xAA) */
void CMD8_SD_tsk( U32 arg,U8 card_number )
{
	W32(SDMMC_CMDARG, arg);

	//R7 +Res index=001000'b
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			|SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(8));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

#if SDMMC_HOST_SD30_1D8V_TEST
void CMD11_SD_tsk( U8 card_number )
{
	W32(SDMMC_CMDARG, 0);

	//R1.+Res CMD index.
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			|SDMMC_CMD_VOLT_SWITCH  //
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(11));

//	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
	/*
	 * the host controller does not generate cmd_done interrupt on receiving R1 response.
	 * The host controller generates interrupt (VOLT_SWITCH_INT) once the CMD or DAT [3:0] line goes low.
	 */
#if SDMMC_HOST_INT_TEST
	while( g_intr_VOIT_SWITCH == 0 ){}
	g_intr_VOIT_SWITCH = 0;
#else
	SDIO_PollStatus(SDMMC_INT_VOLT_SWITCH);
#endif
}
#endif

void CMD55_SD_tsk( U32 rca,U8 card_number )
{
	W32(SDMMC_CMDARG, (rca <<16));

	//R1.+Res CMD index.
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(55));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* SD_SEND_OP_COND(ACMD41) :OCR */
void ACMD41_SD_tsk( U32 arg,U8 card_number )
{
	//CMD55 in idle_state shall be the card?．s default RCA=0x0000.
	CMD55_SD_tsk( 0x0000,card_number );

	//the host should set HCS to 0 if the card returns no response to CMD8.
	W32(SDMMC_CMDARG, arg);
	//R3 has no CRC. +Res index=111111'b
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			|(card_number <<16)|SDMMC_CMD_INDX(41));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* CMD02:ALL_SEND_CID */
void CMD2_SD_tsk( U8 card_number )
{
	W32(SDMMC_CMDARG, 0x00000000 );

	//136 bits R2 has CRC. + Res index=111111'b
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			|SDMMC_CMD_RESP_LONG
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(2));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* CMD03:
 * SD:SEND_RELATIVE_ADDR
 * 	 Send CMD3 SET_REL_ADDR with argument 0/ SD Card publishes its RCA.
 */
void CMD3_SD_tsk( U8 card_number )
{
	W32(SDMMC_CMDARG, 0x00000000 );

	//SD: R6.+Res CMD index. + new RCA+ status
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(3));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* CMD09:SEND_CSD
 *  Send CMD9 SEND_CSD with argument as card's RCA
 * */
void CMD9_SD_tsk( U32 rca,U8 card_number )
{
	W32(SDMMC_CMDARG, (rca <<16));

	//136 bits R2 has CRC. + Res index=111111'b
	W32(SDMMC_CMD, SDMMC_CMD_START
				|SDMMC_CMD_USE_HOLD
				| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_RESP_LONG
				| SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(9));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}



/* CMD07:SELECT/DESELECT_CARD */
void CMD7_SD_tsk( U32 rca,U8 card_number )
{
	W32(SDMMC_CMDARG, (rca <<16));
	//R1/R1b.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				| SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(7));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* CMD13:SEND_STATUS */
void CMD13_SD_tsk( U32 rca,U8 card_number )
{
	W32(SDMMC_CMDARG, (rca <<16));
	//R1.
	W32(SDMMC_CMD, SDMMC_CMD_START |SDMMC_CMD_USE_HOLD
				| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				| SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(13));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* ACMD42:SET_CLR_CARD_DETECT, arg: bit0, 0:disconnect 1:connect */
void ACMD42_SD_tsk( U32 rca, U32 set_cd,U8 card_number )
{
	CMD55_SD_tsk( rca,card_number );

	W32(SDMMC_CMDARG, set_cd & 0x00000001 );
	//R1.
	W32(SDMMC_CMD, SDMMC_CMD_START |SDMMC_CMD_USE_HOLD
				| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				| SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(42));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* ACMD51:SEND_SCR:  Reads the SD Configuration Register(64bits) */
void ACMD51_SD_tsk( U32 rca,U32 byte_num,U8 card_number )
{
	CMD55_SD_tsk( rca,card_number );

	W32(SDMMC_CMDARG, 0);
	W32(SDMMC_BLKSIZ, byte_num);
	W32(SDMMC_BYTCNT, byte_num);
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_RD
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
				|(card_number <<16)|SDMMC_CMD_INDX(51));


#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){} //SD_interrupt_isr
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
#endif
}

/* Defines the data bus width (?．00?．=1bit or ?．10?．=4 bits bus) to be used for data transfer. */
void ACMD6_SD_tsk( U32 rca,U32 busw,U8 card_number )
{
	CMD55_SD_tsk( rca,card_number );

	W32(SDMMC_CMDARG, busw & 0x00000003 );
	//R1
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(6));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* High Capacity SD Memory Card: fixed 512 Bytes*/
void CMD16_SD_tsk( U16 blocklen,U8 card_number )
{
	W32(SDMMC_CMDARG, blocklen);
	//R1
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(16));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* CMD6: SWITCH_FUNC
 * 	Checks switchable function(mode 0) and
 * 	 switch card function(mode 1). */
void CMD6_SD_tsk( U32 arg,U32 byte_num,U8 card_number )
{
	W32(SDMMC_CMDARG, arg);
	W32(SDMMC_BLKSIZ, byte_num);
	W32(SDMMC_BYTCNT, byte_num);

	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START
				|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_RD
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
				|(card_number <<16)|SDMMC_CMD_INDX(6));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
#endif
}

/* CMD17:Read Single Block */
void CMD17_SD_tsk( U32 arg,U32 byte_num,U8 card_number )
{
	W32(SDMMC_BYTCNT, byte_num);
	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_RDTHR_EN );
#if USE_INTERNAL_DMAC
	W32(SDMMC_DBADDRL, IDMA_DESCRIPTION_ADDR);
	W32(SDMMC_DBADDRU, SDMMC_BUFFERs_BASE_HI ); //Upper

	SDIO_Internal_Dmac_Ctrl( 1 );
#endif

	W32(SDMMC_CMDARG, arg); //Data address
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START
				|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP
				|SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_RD
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
				|(card_number <<16)|SDMMC_CMD_INDX(17));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
#endif
}

/* CMD18:Read Multi Block */
void CMD18_SD_tsk( U32 arg,U32 byte_num,U8 card_number )
{
	W32(SDMMC_BYTCNT, byte_num);
	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_RDTHR_EN );
#if USE_INTERNAL_DMAC
	W32(SDMMC_DBADDRL, IDMA_DESCRIPTION_ADDR);
	W32(SDMMC_DBADDRU, SDMMC_BUFFERs_BASE_HI);

	SDIO_Internal_Dmac_Ctrl( 1 );
#endif

	W32(SDMMC_CMDARG, arg);
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START
				|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_RD
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
				|(card_number <<16)|SDMMC_CMD_INDX(18));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
#endif
}

/* STOP_TRANSMISSION(CMD12) */
void CMD12_SD_tsk( U8 card_number )
{
	//the host should set HCS to 0 if the card returns no response to CMD8.
	W32(SDMMC_CMDARG, 0);

	//R3 has no CRC. +Res index=111111'b
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			| SDMMC_CMD_STOP
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(12));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* CMD24:Write Single Block */
void CMD24_SD_tsk( U32 arg,U32 byte_num,U8 card_number )
{
	W32(SDMMC_BYTCNT, byte_num);
	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );
#if USE_INTERNAL_DMAC
	W32(SDMMC_DBADDRL, IDMA_DESCRIPTION_ADDR);
	W32(SDMMC_DBADDRU, SDMMC_BUFFERs_BASE_HI ); //Upper

	SDIO_Internal_Dmac_Ctrl(1);
#endif

	W32(SDMMC_CMDARG, arg); //Data address
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_WR
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
				|(card_number <<16)|SDMMC_CMD_INDX(24));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER|SDMMC_INT_BCI);

#endif
}

/* CMD25:Write Multi Block */
void CMD25_SD_tsk( U32 arg,U32 byte_num,U8 card_number )
{
	W32(SDMMC_BYTCNT, byte_num);
	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );
#if USE_INTERNAL_DMAC
	W32(SDMMC_DBADDRL, IDMA_DESCRIPTION_ADDR);
	W32(SDMMC_DBADDRU, SDMMC_BUFFERs_BASE_HI );

	SDIO_Internal_Dmac_Ctrl(1);
#endif

	W32(SDMMC_CMDARG, arg);
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_WR
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
//				|SDMMC_CMD_SEND_AUTO_STOP		//S1020 add for test
				|(card_number <<16)|SDMMC_CMD_INDX(25));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* CMD35:Erase Start Block */
void CMD35_SD_tsk( U32 arg, U8 card_number )
{
//	W32(SDMMC_BYTCNT, byte_num);
//	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

//	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );

	W32(SDMMC_CMDARG, arg); //Data address
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(35));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER|SDMMC_INT_BCI);
#endif
}

/* CMD36:Erase End Block */
void CMD36_SD_tsk( U32 arg,U32 byte_num,U8 card_number )
{
//	W32(SDMMC_BYTCNT, byte_num);
//	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

//	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );

	W32(SDMMC_CMDARG, arg); //Data address
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(36));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER|SDMMC_INT_BCI);
#endif
}


/* CMD38:Erase Enable */
void CMD38_SD_tsk(U8 card_number)
{
//	W32(SDMMC_BYTCNT, byte_num);
//	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

//	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );

	W32(SDMMC_CMDARG, 0); //Data address
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(38));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER|SDMMC_INT_BCI);
#endif
}


void SD_DISCARD_tsk(U8 card_number)
{
	SD_Error errorstatus = SD_OK;
	W32(SDMMC_CMDARG, (1<<0 | 1<<1) ); //Data address
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(38));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER|SDMMC_INT_BCI);
#endif
	
	errorstatus = MMC_Switch( 0,
						EXT_CSD_SANITIZE_START,
						EXT_CSD_SANNTIZE_ENABLE
					);//CMD6

}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //SD_HOST_TEST


#if EMMC_HOST_TEST
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMD0_MMC_tsk( U32 arg,U8 card_number )
{
	W32(SDMMC_CMDARG, arg );

	/* No CMD response required */
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD		//?
			|SDMMC_CMD_INIT			//Send initialization sequence before sending this command
			|(card_number <<16)|SDMMC_CMD_INDX(0));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}	//SD_interrupt_isr()
	g_intr_CMD_DONE = 0;
#else
	//wait cmd done
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}

/* SEND_IF_COND (CMD1) :OCR
 *  [30:29] two bits for the indication of the supported access mode of the memory.
 */
void CMD1_MMC_tsk( U32 arg,U8 card_number )
{
	W32(SDMMC_CMDARG, arg);

	//R3 has no CRC. +Res index=111111'b
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
		//	| SDMMC_CMD_RESP_CRC	no
			|(card_number <<16)|SDMMC_CMD_INDX(1) );

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}


/* CMD03:
 * eMMC:SET_RELATIVE_ ADDR
 * 	 Assigns relative address to the Device
 */
void CMD3_MMC_tsk( U32 rca,U8 card_number )
{
	W32(SDMMC_CMDARG, (rca <<16) );

	//eMMC: R1.+Res CMD index.
	W32(SDMMC_CMD, SDMMC_CMD_START
			|SDMMC_CMD_USE_HOLD
			| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
			| SDMMC_CMD_RESP_CRC
			|(card_number <<16)|SDMMC_CMD_INDX(3));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}


/* CMD8:SEND_EXT_CSD:  The Device sends its EXT_CSD register as a block of data. */
void CMD8_MMC_tsk( U32 byte_num,U8 card_number )
{
	W32(SDMMC_CMDARG, 0);
	W32(SDMMC_BLKSIZ, byte_num);
	W32(SDMMC_BYTCNT, byte_num);

	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				| SDMMC_CMD_RESP_CRC
				| SDMMC_CMD_DAT_EXP
				|(card_number <<16)|SDMMC_CMD_INDX(8) );


#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){} //SD_interrupt_isr
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
#endif
}

/* CMD21:SEND_TUNING_ BLOCK */
void CMD21_MMC_tsk( U32 byte_num,U8 card_number )
{
	W32(SDMMC_CMDARG, 0);
	W32(SDMMC_BLKSIZ, byte_num);
	W32(SDMMC_BYTCNT, byte_num);

	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				| SDMMC_CMD_RESP_CRC
				| SDMMC_CMD_DAT_EXP
				|(card_number <<16)|SDMMC_CMD_INDX(21) );


#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){} //SD_interrupt_isr
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
#endif
}

/* CMD6:SWITCH
 *  Switches the mode or modifies the EXT_CSD */
void CMD6_MMC_tsk( U32 arg,U8 card_number )
{
	W32(SDMMC_CMDARG, arg);

	//R1b
	W32(SDMMC_CMD, SDMMC_CMD_START
				|SDMMC_CMD_USE_HOLD
				| SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				| SDMMC_CMD_RESP_CRC
				|(card_number <<16)|SDMMC_CMD_INDX(6));

#if SDMMC_HOST_INT_TEST
	while( g_intr_CMD_DONE == 0 ){}
	g_intr_CMD_DONE = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //EMMC_HOST_TEST
