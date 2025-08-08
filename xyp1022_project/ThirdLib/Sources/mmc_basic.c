/*******************************************************
 * File: mmc_basic.c
 * Date:
 * Author: ccore_sys
 ******************************************************/

#include "e500mc_asm.h"
#include "ppclib.h"
#include "uart.h"
#include "gpio.h"
#include "common.h"

#include "sd_mmc_card.h"
#include "dwc_mmc.h"

extern void SD_interrupt_isr_DTO( void );
/*
 *  return DesignWare Cores Mobile Storage Host
 *  'chapter 6 Registers' register string.
 */
inline const char *DWC_RegsMemMap_string( U32 Offset )
{
	switch (Offset) {
		case SDMMC_CTRL:		return "SDMMC_CTRL";
		case SDMMC_PWREN:		return "SDMMC_PWREN";
		case SDMMC_CLKDIV:		return "SDMMC_CLKDIV";
		case SDMMC_CLKSRC:		return "SDMMC_CLKSRC";
		case SDMMC_CLKENA:		return "SDMMC_CLKENA";
		case SDMMC_TMOUT:		return "SDMMC_TMOUT";

		case SDMMC_CTYPE:		return "SDMMC_CTYPE";
		case SDMMC_BLKSIZ:		return "SDMMC_BLKSIZ";
		case SDMMC_BYTCNT:		return "SDMMC_BYTCNT";
		case SDMMC_INTMASK:		return "SDMMC_INTMASK";
		case SDMMC_CMDARG:		return "SDMMC_CMDARG";
		case SDMMC_CMD:			return "SDMMC_CMD";
		case SDMMC_RESP0:		return "SDMMC_RESP0";
		case SDMMC_RESP1:		return "SDMMC_RESP1";
		case SDMMC_RESP2:		return "SDMMC_RESP2";
		case SDMMC_RESP3:		return "SDMMC_RESP3";
		case SDMMC_MINTSTS:		return "SDMMC_MINTSTS";
		case SDMMC_RINTSTS:		return "SDMMC_RINTSTS";
		case SDMMC_STATUS:		return "SDMMC_STATUS";
		case SDMMC_FIFOTH:		return "SDMMC_FIFOTH";
		case SDMMC_CDETECT:		return "SDMMC_CDETECT";
		case SDMMC_WRTPRT:		return "SDMMC_WRTPRT";
		case SDMMC_GPIO:		return "SDMMC_GPIO";
		case SDMMC_TCBCNT:		return "SDMMC_TCBCNT";
		case SDMMC_TBBCNT:		return "SDMMC_TBBCNT";
		case SDMMC_DEBNCE:		return "SDMMC_DEBNCE";
		case SDMMC_USRID:		return "SDMMC_USRID";
		case SDMMC_VERID:		return "SDMMC_VERID";
		case SDMMC_HCON:		return "SDMMC_HCON";
		case SDMMC_UHS_REG:		return "SDMMC_UHS";
		case SDMMC_RST_N:		return "SDMMC_RST_N";

		case SDMMC_BMOD:		return "SDMMC_BMOD";
		case SDMMC_PLDMND:		return "SDMMC_PLDMND";
//		case SDMMC_DBADDR:		return "SDMMC_DBADDR";
		case SDMMC_DBADDRL:		return "SDMMC_DBADDRL";
		case SDMMC_DBADDRU:		return "SDMMC_DBADDRU";

		case SDMMC_IDSTS:		return "SDMMC_IDSTS";
		case SDMMC_IDINTEN:		return "SDMMC_IDINTEN";
//		case SDMMC_DSCADDR:		return "SDMMC_DSCADDR";
		case SDMMC_DSCADDRL:	return "SDMMC_DSCADDRL";
		case SDMMC_DSCADDRU:	return "SDMMC_DSCADDRU";
//		case SDMMC_BUFADDR:		return "SDMMC_BUFADDR";
		case SDMMC_BUFADDRL:	return "SDMMC_BUFADDRL";
		case SDMMC_BUFADDRU:	return "SDMMC_BUFADDRU";

		case SDMMC_CARDTHRCTL:	return "SDMMC_CARDTHRCTL";
		case SDMMC_BACK_END_POWER:return "SDMMC_BACK_END_POWER";
		case SDMMC_UHS_EXT:		return "SDMMC_UHS_EXT";
		case SDMMC_EMMC_DDR:	return "SDMMC_EMMC_DDR";
		case SDMMC_ENABLE_SHIFT:return "SDMMC_ENABLE_SHIFT";
	}
	return "SDMMC_UNKNOWN";
}
/*
 *  return DesignWare Cores Mobile Storage Host 'chapter 6 Registers' register string.
 */
inline const char *DWC_CtypeReg_string( U32 reg,U8 card_number )
{
	if ( reg & (SDMMC_CTYPE_8BIT <<card_number) )
			return "BUS_WIDTH_8BIT";
	if ( reg & (SDMMC_CTYPE_4BIT <<card_number) )
			return "BUS_WIDTH_4BIT";

	return "BUS_WIDTH_1BIT";
}

/*
 * for DesignWare Cores Mobile Storage Host 'chapter 6 Registers' read test.
 */
void DWC_SD_Host_RegTest(void)
{
	U16 i = 0;
	uart_printf_buf("\nDWC_MSH reg test start .............................. \n");

#if 1
	uart_printf(" SDMMC_USRID read =0x%x\r\n",R32(SDMMC_USRID) );//0x0ccc-xxxx
	//test LSB
	uart_printf(" SDMMC_USRID read 0=0x%x\r\n",R8(SDMMC_USRID+0) );
	uart_printf(" SDMMC_USRID read 1=0x%x\r\n",R8(SDMMC_USRID+1) );
	uart_printf(" SDMMC_USRID read 2=0x%x\r\n",R8(SDMMC_USRID+2) );//0xcc
	uart_printf(" SDMMC_USRID read 3=0x%x\r\n",R8(SDMMC_USRID+3) );//0x0c

	uart_printf(" SDMMC_VERID read =0x%x\r\n",R32(SDMMC_VERID) );
#endif

	for( i=0; i<0x114; i+=4 )
	{
		if( R32( SD_EMMC_HOST_BASE +i ) != 0x00000000 )
		{
			uart_printf(" Reg not 0x0,0x%x=0x%x    [%s]\r\n",i,
					R32( SD_EMMC_HOST_BASE +i ),
					DWC_RegsMemMap_string(SD_EMMC_HOST_BASE +i) );
		}
	}

	uart_printf_buf("DWC_MSH reg test end .................. \r\n\r\n");
}

//-------------------------------------------------------------------------------------------------------------------------

SD_CardInfo SDCardInfo;
static U32 CSD_Tab[4], CID_Tab[4], g_RCA = 0;
//static U32 SCR[2];
static U32 gExt_CSD[128];
static U32 gCardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;

SD_Error SD_EnumCard(void)
{
	SD_Error errorstatus = SD_OK;
	U32 tmp; 

	/* SD interface init
	 * 	reset/init/dectect/clk,CMD0/1/2/3/9
	 * 	*/
	errorstatus = SD_Init();
	if(errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}

	/* get & save SD info */
    errorstatus = SD_GetCardInfo( &SDCardInfo );
    errorstatus = SD_SelectDeselect( SDCardInfo.RCA );	//CMD7/13
    if(errorstatus != SD_OK) {
    	while(1){}   //asm("trap");
    	return(errorstatus);
    }


	SDIO_FIFO_Reset();
/*****
 *  device into the DATA Transfer State.
 *****/

	errorstatus = MMC_Send_ExtendedCSD( gExt_CSD ); //CMD8
	if(errorstatus != SD_OK) {
		return(errorstatus);
	}

	
	/* SET Data Bus Width  */
	#if (SDMMC_HOST_DWIDTH_4b_TEST || SDMMC_HOST_DWIDTH_8b_TEST)
		errorstatus = MMC_Switch( 0,
								EXT_CSD_BUS_WIDTH,
							#if SDMMC_HOST_DWIDTH_4b_TEST
								EXT_CSD_BUS_WIDTH_4
							#endif
							#if SDMMC_HOST_DWIDTH_8b_TEST
								EXT_CSD_BUS_WIDTH_8
							#endif
						);//CMD6

		/* CTYPE: Card Type Register */
		tmp = R32( SDMMC_CTYPE );
		#if SDMMC_HOST_DWIDTH_4b_TEST
			tmp |= ( SDMMC_CTYPE_4BIT <<EMMC_SLOT0 );
		#endif
		#if SDMMC_HOST_DWIDTH_8b_TEST
			tmp |= ( SDMMC_CTYPE_8BIT <<EMMC_SLOT0 );
		#endif
		W32(SDMMC_CTYPE, tmp );
	#endif
	#if 1//DEBUG_PRINT
		uart_printf(" [######### (C6-CTYPE=0x%x): slot%d %s]\r\n",
					tmp = R32(SDMMC_CTYPE) ,EMMC_SLOT0,
					DWC_CtypeReg_string( tmp,EMMC_SLOT0 )   );
		//

#if	(!SDMMC_HOST_EMMC_1D8V_TEST)
	errorstatus = MMC_Switch( 0,
							EXT_CSD_HS_TIMING,
							EXT_CSD_HS_TIMING_HSPEED );//CMD6
#endif
#if	SDMMC_HOST_EMMC_1D8V_TEST
	errorstatus = MMC_Switch( 0,
							EXT_CSD_HS_TIMING,
							EXT_CSD_HS_TIMING_HS200 );

#endif
#if 0
	//test CMD6 result
	 MMC_Send_ExtendedCSD( gExt_CSD ); 
#endif
#endif

	 
#if 1
	#if FPGA_TEST
		//	SDIO_SetClk( 1<<EMMC_SLOT0, FPGA_CLKOUT_1MHz_DIV);
		//	SDIO_SetClk( 1<<EMMC_SLOT0, FPGA_CLKOUT_3MHz_DIV);
			SDIO_SetClk( 1<<EMMC_SLOT0, FPGA_CLKOUT_6MHz_DIV);
		//	SDIO_SetClk( 1<<EMMC_SLOT0, FPGA_CLKOUT_12MHz_DIV);
	#else
		#if	(!SDMMC_HOST_EMMC_1D8V_TEST)
	//	SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_20MHz_DIV );
		SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_33d3MHz_DIV );
		#endif
		#if	(SDMMC_HOST_EMMC_1D8V_TEST)
	//	SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_25MHz_DIV );
		SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_50MHz_DIV );
	//	SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_100MHz_DIV );
	//	SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_200MHz_DIV );

		#endif
	#endif
#endif
#if 1
	delay( 0x5000 ) ;
#endif
	return(errorstatus);
}

/*******************************************************************************
* Function Name  : SD_Init(card identification mode)
* Description    : Card Initialization and Identification Process.
* 				   Initializes the SD Card and put it into StandBy State (Ready
*                  for data transfer).
* Input          : None
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
SD_Error SD_Init(void)
{
	SD_Error errorstatus = SD_OK;

#if FPGA_TEST

#endif
#if EVB_903T_TEST
	//
	GPIO_SD_HOST_Configuration(0);  //Q2 eMMC_VDDQ
#endif

	/* Assert Reset - active-low */
	if( SDIO_Reset() )
	{
	/* (wait for hw rest flag) Reset TimeOut */
		errorstatus = SD_ERROR;
		while(1){}   //asm("trap");
		return(errorstatus);
	}
	/* inter/fifo */
	SDIO_Init();

	while( SDIO_CardDetect() )
	{}

	#if FPGA_TEST
		SDIO_SetClk( 1<<EMMC_SLOT0, FPGA_CLKOUT_400kHz_DIV );
	//	SDIO_SetClk( 1<<EMMC_SLOT0, FPGA_CLKOUT_100kHz_DIV ); //pin38
	#else
		SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_400kHz_DIV );

	//	SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_500kHz_DIV );//cpr=0x30
	//	SDIO_SetClk( 1<<EMMC_SLOT0, EVB_CLKOUT_700kHz_DIV );//cpr=0x20
	#endif


	errorstatus = SD_PowerON();//CMD0/1
	if (errorstatus != SD_OK)
	{
		while(1){}   //asm("trap");

		/* CMD Response TimeOut (wait for CMDSENT flag) */
		return(errorstatus);
	}

	errorstatus = SD_InitializeCards();//CMD2/3/9
	if (errorstatus != SD_OK)
	{
		while(1){}   //asm("trap");

		/* CMD Response TimeOut (wait for CMDSENT flag) */
		return(errorstatus);
	}

	return(errorstatus);
}

/* Card Reset:
 *  sets each card into Idle State regardless of the current card state.
 * Operating Condition Validation:
 *  verify SD Memory Card interface operating condition.
 * */
SD_Error SD_PowerON(void)
{
	SD_Error errorstatus = SD_OK;
	U32 rstatus,response;
#if DEBUG_PRINT
	U32 status,response_c11;;
#endif
	U32 count = 0;

/* CMD0: GO_IDLE_STATE */
	CMD0_MMC_tsk( CMD0ARG_GO_IDLE_STATE,EMMC_SLOT0 );
	delay(0x8000);
#if DEBUG_PRINT
	uart_printf(" C0:R_S=0x%x/S=0x%x..\r\n",R32(SDMMC_RINTSTS),
									R32(SDMMC_STATUS) );
	//0/S=0x106.
#endif



/* CMD1: SEND_IF_COND */
	/* Access mode validation (higher than 2GB of densities).
	 * The host must repeat CMD1 until the busy bit is cleared.
	 */
	do
	{
		//host support 2.7V-3.6V/ 1.70¡§C1.95V
		CMD1_MMC_tsk( 0x40FF8080,EMMC_SLOT0 );	//host> 2GB
	//	CMD1_MMC_tsk( 0x00FF8080,EMMC_SLOT0 );	//host<=2GB

		rstatus = R32(SDMMC_RINTSTS);
		if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)
		{
			W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
			while(1){}   //asm("trap");
			return SD_ERROR;
		}

		/*
		 * device is busy or entering Ready state
		 */
		response = R32(SDMMC_RESP0);
		if( (response & OCR_BUSY_Bit31) != OCR_POWER_BUSY ) {
			break;
		}

		count++;
	}while(count < SD_MAX_VOLT_TRIAL);
#if DEBUG_PRINT
	uart_printf(" C1.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=0x%x)..\r\n",
			response,
				R32(SDMMC_RINTSTS),
				status=R32(SDMMC_STATUS),
				SDMMC_GET_R_IDX(status) //see R3
			);
	//0xc0ff8080. 0x0/0x1f906(R_idx=0x3f)..
#endif
/*****
 * Device entering Ready state
 *****/

	if(count >= SD_MAX_VOLT_TRIAL)
	{
		errorstatus = SD_INVALID_VOLTRANGE;
		while(1){}   //asm("trap");
		return(errorstatus);
	}

	if( (response & SD_HIGH_CAPACITY) == SD_HIGH_CAPACITY )	{
		/* sector mode */
		gCardType = SDIO_HIGH_CAPACITY_SD_CARD;
	}
	else{
		/* byte mode */
		uart_printf(" [######### (C1 response):eMMC devices capacity <= 2GB\r\n" );
	//	while(1){}   //asm("trap");
	}

	return(errorstatus);
}

/* Device Initialization and Identification Process
 *  */
SD_Error SD_InitializeCards(void)
{
	SD_Error errorstatus = SD_OK;
	U32 rstatus,response;
#if DEBUG_PRINT
	U32 status;
#endif

	if( SDIO_GetPowerState(1 <<EMMC_SLOT0) == SDIO_PowerState_OFF) {
		errorstatus = SD_REQUEST_NOT_APPLICABLE;
		while(1){}   //asm("trap");
		return(errorstatus);
	}

/* CMD2:ALL_SEND_CID */
	CMD2_SD_tsk( EMMC_SLOT0 );
	rstatus = R32(SDMMC_RINTSTS);
	if( rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
//	CID_Tab[0] = R32(SDMMC_RESP0);
//	CID_Tab[1] = R32(SDMMC_RESP1);
//	CID_Tab[2] = R32(SDMMC_RESP2);
//	CID_Tab[3] = R32(SDMMC_RESP3);
	//R2
	CID_Tab[0] = R32(SDMMC_RESP3);
	CID_Tab[1] = R32(SDMMC_RESP2);
	CID_Tab[2] = R32(SDMMC_RESP1);
	CID_Tab[3] = R32(SDMMC_RESP0);
#if DEBUG_PRINT
	uart_printf(" C2-CID:MSB-0x%x/0x%x/0x%x/0x%x..\r\n",
			CID_Tab[0],CID_Tab[1],CID_Tab[2],CID_Tab[3] );
	//0x11010030/0x30384737/0x30004154/0x2f04a697..
	uart_printf(" C2.R_S=0x%x/S=0x%x(R_idx=0x%x)..\r\n",
			rstatus,
			status=R32(SDMMC_STATUS),
			SDMMC_GET_R_IDX(status) //see R2
			);
	//0x0/0x1f906(R_idx=0x3f)..
#endif
/*****
 * This Device then goes into Identification State.
 *****/


/* CMD03:SET_RELATIVE_ADDR */
//	g_RCA = 0x5a5a;
	g_RCA = 0x1234;
	CMD3_MMC_tsk( g_RCA,EMMC_SLOT0 );
	rstatus = R32(SDMMC_RINTSTS);
	if( rstatus & DWC_MSH_CMD_ERROR_FLAGS)
	{
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}
#if DEBUG_PRINT
	uart_printf(" C3.Res=0x%x(cur(%s)sate). R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
			response = R32(SDMMC_RESP0),
			R1_CurState_string( (U8)R1_CURRENT_STATE(response) ),
				rstatus,
				status=R32(SDMMC_STATUS),
				SDMMC_GET_R_IDX(status) //see R1
			);
	//0x500(cur(IDENT)sate). 0x0/0x1906(R_idx=3)..
#endif
/*****
 * Once the RCA is received the Device state changes to the Stand-by State
 *****/


/* CMD09:SEND_CSD, after Stand-by State */
	CMD9_SD_tsk( g_RCA,EMMC_SLOT0 );
	rstatus = R32(SDMMC_RINTSTS);
	if( rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	//R2
	CSD_Tab[0] = R32(SDMMC_RESP3);
	CSD_Tab[1] = R32(SDMMC_RESP2);
	CSD_Tab[2] = R32(SDMMC_RESP1);
	CSD_Tab[3] = R32(SDMMC_RESP0);
#if DEBUG_PRINT
	uart_printf(" C9-CSD:MSB-0x%x/0x%x/0x%x/0x%x..\r\n",
			CSD_Tab[0],CSD_Tab[1],CSD_Tab[2],CSD_Tab[3] );
	//0xd0270032/0xf5903ff/0xffffffe7/0x8640009b..
	//...
	uart_printf(" C9.R_S=0x%x/S=0x%x(R_idx=0x%x)..\r\n",
			rstatus,
			status=R32(SDMMC_STATUS),
			SDMMC_GET_R_IDX(status) //see R2
			);
	//0x0/0x1f906(R_idx=0x3f)..
#endif

	errorstatus = SD_OK; /* All cards get intialized */
	return(errorstatus);
}

/*******************************************************************************
* Function Name  : SD_GetCardInfo
* Description    : Returns information about specific card.
* Input          : cardinfo : pointer to a SD_CardInfo structure
*                  that contains all SD card information.
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
  U8 tmp = 0;

  cardinfo->CardType = (U8)gCardType; //=1
  cardinfo->RCA = (U16)g_RCA; //
  /////////////////////CSD///////////////////////////////////////////
  /* Byte 0 */
  tmp = (U8)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;
#if EMMC_HOST_TEST
	#if DEBUG_PRINT
  	  	uart_printf_buf(" #########." );

		//CSD_STRUCTURE [127:126]
		switch( (tmp & 0xC0) >> 6 )
		{
			case 0:
			case 1:
				uart_printf(" unrecognised CSD structure version. \r\n" );
				break;
			case 2:
				uart_printf(" CSD version No. 1.2 \r\n" );
				break;
			case 3:
			default:
				uart_printf(" CSD version: see EXT_CSD register below \r\n");
				break;
		}

		//SPEC_VERS [125:122]:System specification version
		switch( (tmp & 0x3C) >> 2 )
		{
			case 4:
				uart_printf(" eMMC Sys Speci ver:4.1-4.2-4.3-4.4-4.41-4.5-4.51-5.0 \r\n" );
			break;
			default:
				uart_printf(" unrecognised eMMC System Specification version. \r\n" );
				break;
		}
	#endif
#endif

  /* Byte 1 */
  tmp = (U8)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /* Byte 2 */
  tmp = (U8)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /* Byte 3 */
  tmp = (U8)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /* Byte 4 */
  tmp = (U8)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /* Byte 5 */
  tmp = (U8)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /* Byte 6 */
  tmp = (U8)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /* Reserved */

  if(gCardType == SDIO_STD_CAPACITY_SD_CARD_V2_0)
  {
    cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    /* Byte 7 */
    tmp = (U8)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    /* Byte 8 */
    tmp = (U8)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /* Byte 9 */
    tmp = (U8)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    /* Byte 10 */
    tmp = (U8)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if(gCardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    /* Byte 7 */
    tmp = (U8)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /* Byte 8 */
    tmp = (U8)((CSD_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    /* Byte 9 */
    tmp = (U8)((CSD_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SD_csd.DeviceSize |= (tmp);

    /* Byte 10 */
    tmp = (U8)((CSD_Tab[2] & 0x0000FF00) >> 8);

    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    cardinfo->CardBlockSize = 512;
  }


  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /* Byte 11 */
  tmp = (U8)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /* Byte 12 */
  tmp = (U8)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /* Byte 13 */
  tmp = (U8)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /* Byte 14 */
  tmp = (U8)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /* Byte 15 */
  tmp = (U8)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;

  /////////////////////CID///////////////////////////////////////////
  /* Byte 0 */
  tmp = (U8)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;  //0xaa

  /* Byte 1 */
  tmp = (U8)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /* Byte 2 */
  tmp = (U8)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /* Byte 3 */
  tmp = (U8)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /* Byte 4 */
  tmp = (U8)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /* Byte 5 */
  tmp = (U8)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /* Byte 6 */
  tmp = (U8)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /* Byte 7 */
  tmp = (U8)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /* Byte 8 */
  tmp = (U8)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /* Byte 9 */
  tmp = (U8)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /* Byte 10 */
  tmp = (U8)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /* Byte 11 */
  tmp = (U8)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /* Byte 12 */
  tmp = (U8)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /* Byte 13 */
  tmp = (U8)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /* Byte 14 */
  tmp = (U8)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /* Byte 15 */
  tmp = (U8)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;

  return(errorstatus);
}

/*******************************************************************************
* Function Name  : SD_SelectDeselect
* Description    : Selects od Deselects the corresponding card.
* Input          : addr: Address of the Card to be selected.
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
SD_Error SD_SelectDeselect( U32 rca )
{
	SD_Error errorstatus = SD_OK;
	U32 rstatus;
#if DEBUG_PRINT
	U32 status,response;
#endif
#if FPGA_TEST
//	U8 phase_shift_value= 0;
#endif

/* CMD7:SELECT/DESELECT_CARD */
	CMD7_SD_tsk( rca,EMMC_SLOT0 );
	rstatus = R32(SDMMC_RINTSTS);

#if DEBUG_PRINT
//	uart_printf(" C7.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
	uart_printf(" C7.Res=0x%x(cur(%s)sate,%s_FOR_DATA). R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
			response=R32(SDMMC_RESP0),
			R1_CurState_string( (U8)R1_CURRENT_STATE(response) ),
			(response & R1_READY_FOR_DATA) ?"READY":"not READY",
					rstatus,
					status=R32(SDMMC_STATUS),
					SDMMC_GET_R_IDX(status) //see R1b
			);
	//Res=0x700(cur(STBY)sate,READY_FOR_DATA). 0x0/0x3906(R_idx=7)..
#endif
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)
	{
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
	if(errorstatus != SD_OK)
	{
		while(1){}   //asm("trap");
		return errorstatus;
	}



/* CMD13:SEND_STATUS */
	CMD13_SD_tsk( rca,EMMC_SLOT0 );
	rstatus = R32(SDMMC_RINTSTS);

#if DEBUG_PRINT
//	uart_printf(" C13.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
	uart_printf(" C13.Res=0x%x(cur(%s)sate,%s_FOR_DATA). R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
			response=R32(SDMMC_RESP0),
			R1_CurState_string( (U8)R1_CURRENT_STATE(response) ),
			(response & R1_READY_FOR_DATA) ?"READY":"not READY",
					rstatus,
					status=R32(SDMMC_STATUS),
					SDMMC_GET_R_IDX(status) //see R1
			);
	//0x900(cur(TRAN)sate,READY_FOR_DATA). 0x0/0x6906(R_idx=13)..
#endif
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)
	{
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}

	return(errorstatus);
}


/* Data Read
 * */
SD_Error MMC_Send_ExtendedCSD(U32 *ext_csd)
{
	SD_Error errorstatus = SD_OK;
	U32 rstatus,tmp,i;
#if DEBUG_PRINT
	U32 status,response;
#endif

/* CMD8:SEND_EXT_CSD */
	CMD8_MMC_tsk( 512,EMMC_SLOT0  );

	rstatus = R32(SDMMC_RINTSTS);
#if DEBUG_PRINT
	uart_printf(" C8.Res=0x%x(cur(%s)sate). R_S=0x%x/S=0x%x(R_idx=%d,c=%dW)..\r\n",
			response=R32(SDMMC_RESP0),
			R1_CurState_string( (U8)R1_CURRENT_STATE(response) ),
				rstatus,
				status=R32(SDMMC_STATUS),
				SDMMC_GET_R_IDX(status), //see R1
				SDMMC_GET_FCNT(status)
			);
	//0x900(cur(TRAN)sate). R_S=0x0/S=0x1004102(R_idx=8,c=128W)..
	uart_printf(" C8:TCBCNT=%d/TBBCNT=%d..\r\n",
			R32(SDMMC_TCBCNT),R32(SDMMC_TBBCNT) );
	//512/0
#endif
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)
	{
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();

	for(i=0; i<128; i++) {
		tmp = R32(SDMMC_FIFO_DATA);
		ext_csd[i] = le32_to_cpu_emmc( tmp );
	}
#if DEBUG_PRINT
	uart_printf(" C8-2 TCBCNT=%d/TBBCNT=%d/S=0x%x(c=%dW)..\r\n",
			R32(SDMMC_TCBCNT),R32(SDMMC_TBBCNT),
			status=R32(SDMMC_STATUS),
			SDMMC_GET_FCNT(status)  );
	//TCBCNT=512/TBBCNT=512/S=0x4106(c=0W)..
#endif

#if 1//DEBUG_PRINT
//	for(i=0; i<128; i++) {
//		uart_printf(" ext_csd[%d] = 0x%x.\r\n",i,ext_csd[i] );
//	}
	uart_printf(" ext_csd[48] = 0x%x.\r\n",ext_csd[48] );
	uart_printf(" ext_csd[49] = 0x%x.\r\n",ext_csd[49] );
	uart_printf(" ext_csd[50] = 0x%x.\r\n",ext_csd[50] );
	//0x7000200./0x571f0a0a./0xaaaa4444.

//	uart_printf(" ext_csd[47] = 0x%x.\r\n",ext_csd[47] );
//	uart_printf(" ext_csd[46] = 0x%x.\r\n",ext_csd[46] );
//	uart_printf(" ext_csd[45] = 0x%x.\r\n",ext_csd[45] );
#endif
#if 1//DEBUG_PRINT
	tmp = ext_csd[48];

	uart_printf_buf(" #########." );
	//EXT_CSD_REV [192]
	switch( (tmp & 0xFF000000) >> 24 )
	{
		case 0:
			uart_printf(" Extended CSD revisions 1.0 (for MMC v4.0). \r\n" );
			break;
		case 1:
			uart_printf(" Extended CSD revisions 1.1 (for MMC v4.1). \r\n" );
			break;
		case 2:
			uart_printf(" Extended CSD revisions 1.2 (for MMC v4.2). \r\n" );
			break;
		case 3:
			uart_printf(" Extended CSD revisions 1.3 (for MMC v4.3). \r\n" );
			break;
		case 4:
			uart_printf(" Extended CSD revisions 1.4 (Obsolete). \r\n" );
			break;
		case 5:
			uart_printf(" Extended CSD revisions 1.5 (for MMC v4.41). \r\n" );
			break;
		case 6:
			uart_printf(" Extended CSD revisions 1.6 (for MMC v4.5, v4.51). \r\n" );
			break;
		case 7:
			uart_printf(" Extended CSD revisions 1.7 (for MMC v5.0). \r\n" );//--
			break;
		default:
			uart_printf(" unrecognised Extended CSD structure version. \r\n" );
			break;
	}

	//CSD_STRUCTURE [194]
//	switch( (tmp & 0x0000FF00) >> 8 )
//	{
//		case 2:
//			uart_printf(" CSD version No. 1.2 \r\n" );
//			break;
//		default:
//			uart_printf(" unrecognised CSD structure version. \r\n" );
//			break;
//	}
#endif
#if 1//DEBUG_PRINT
	tmp = ext_csd[49];
	//DEVICE_TYPE [196]
	tmp = (tmp & 0xFF000000) >> 24;
	uart_printf(" DEVICE_TYPE:eMMC- %s,%s%s%s ,%s%s ,%s%s. \r\n",
				(tmp& (EXT_CSD_CARD_TYPE_26)) ? "26MHz":"",

				(tmp& EXT_CSD_CARD_TYPE_52) ? "52MHz":"",
				(tmp& EXT_CSD_CARD_TYPE_DDR_1_8V) ? "-1.8V or 3V":"",
				(tmp& EXT_CSD_CARD_TYPE_DDR_1_2V) ? "+ 1.2V":"",

				(tmp& EXT_CSD_CARD_TYPE_SDR_1_8V) ? "HS200 Single 200MHz-1.8V":"",//-
				(tmp& EXT_CSD_CARD_TYPE_SDR_1_2V) ? "+200MHz-1.2V":"",
				(tmp& EXT_CSD_CARD_TYPE_DDR_1_8V) ? "HS400 Dual 200MHz-1.8V":"",//-
				(tmp& EXT_CSD_CARD_TYPE_DDR_1_2V) ? "+200MHz-1.2V":""
				);

#endif

	return(errorstatus);
}

/**
 *	mmc_switch - modify EXT_CSD register
 *	@card: the MMC card associated with the data transfer
 *	@set: cmd set values
 *	@index: EXT_CSD register index
 *	@value: value to program into EXT_CSD register
 *	@timeout_ms: timeout (ms) for operation performed by register write,
 *                   timeout of zero implies maximum possible timeout
 *
 *	only Modifies the EXT_CSD register for selected card.
 */
SD_Error MMC_Switch( U8 set, U8 index, U8 value )
{
	SD_Error errorstatus = SD_OK;
	U32 rstatus,tmp,response;
	U8 i;
#if DEBUG_PRINT
	U32 status;
#endif

/* CMD6:SWITCH_FUNC */
	CMD6_MMC_tsk( MMC_CMD6_ACCESSMODE_WRITE_BYTE
					|MMC_CMD6_INDEX( index )
					|MMC_CMD6_VALUE( value )
					|MMC_CMD6_CMDSET(set),
					EMMC_SLOT0  );
	rstatus = R32(SDMMC_RINTSTS);
#if 1
	while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ){} //1: card data busy
#endif
	#if DEBUG_PRINT
	//	uart_printf(" C6.1.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
		uart_printf(" C6.1.Res=0x%x(cur(%s)sate,%s_FOR_DATA). R_S=0x%x/S=0x%x..\r\n",
				response=R32(SDMMC_RESP0),
				R1_CurState_string( (U8)R1_CURRENT_STATE(response) ),
				(response & R1_READY_FOR_DATA) ?"READY":"not READY",
				rstatus,
				status=R32(SDMMC_STATUS),
				SDMMC_GET_R_IDX(status) //see R1b
				);
		// 0x800(cur(TRAN)sate,not READY_FOR_DATA). 0x0/0x3306 or 0x3106..(R_idx=6)
	#endif
	if( rstatus & DWC_MSH_CMD_ERROR_FLAGS)
	{
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();

#if 1
	/* CMD13:SEND_STATUS */
	CMD13_SD_tsk( g_RCA,EMMC_SLOT0 );
	rstatus = R32(SDMMC_RINTSTS);
	response = R32(SDMMC_RESP0);
	#if DEBUG_PRINT
	//	uart_printf(" C13.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
		uart_printf(" C13(-C6).Res=0x%x(cur(%s)sate,%s_FOR_DATA). R_S=0x%x/S=0x%x(R_idx=%d)..\r\n",
				response,
				R1_CurState_string( (U8)R1_CURRENT_STATE(response) ),
				(response & R1_READY_FOR_DATA) ?"READY":"not READY",
						rstatus,
						status=R32(SDMMC_STATUS),
						SDMMC_GET_R_IDX(status) //see R1
				);
		//C13(-C6).0x900(cur(TRAN)sate,READY_FOR_DATA). 0x0/0x6906(R_idx=13)..
	#endif
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)
	{
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	if( response & R1_SWITCH_ERROR ){
		uart_printf(" 	C13(-C6)...switch error...\r\n");
		while(1){}   //asm("trap");
	}
#endif

	return(errorstatus);
}


SD_Error SD_CmdResp1Error(void)
{
	SD_Error errorstatus = SD_OK;
	U32 response_r1;

	/* We have received response, retrieve it for analysis  */
	response_r1 = R32(SDMMC_RESP0);

	if ((response_r1 & SD_R1_ERRORBITS) == 0)
	{
	//	while(1){}   //asm("trap");
		return(errorstatus);
	}

	if (response_r1 & SD_R1_ADDR_OUT_OF_RANGE)
	{
		while(1){}   //asm("trap");
		return(SD_ADDR_OUT_OF_RANGE);
	}

	if (response_r1 & SD_R1_ADDR_MISALIGNED)
	{
		while(1){}   //asm("trap");
		return(SD_ADDR_MISALIGNED);
	}

	if (response_r1 & SD_R1_BLOCK_LEN_ERR)
	{
		while(1){}   //asm("trap");
		return(SD_BLOCK_LEN_ERR);
	}

	if (response_r1 & SD_R1_ERASE_SEQ_ERR)
	{
		while(1){}   //asm("trap");
		return(SD_ERASE_SEQ_ERR);
	}

	if (response_r1 & SD_R1_BAD_ERASE_PARAM)
	{
		while(1){}   //asm("trap");
		return(SD_BAD_ERASE_PARAM);
	}

	if (response_r1 & SD_R1_WRITE_PROT_VIOLATION)
	{
		while(1){}   //asm("trap");
		return(SD_WRITE_PROT_VIOLATION);
	}

	if (response_r1 & SD_R1_LOCK_UNLOCK_FAILED)
	{
		while(1){}   //asm("trap");
		return(SD_LOCK_UNLOCK_FAILED);
	}

	if (response_r1 & SD_R1_COM_CRC_FAILED)
	{
		while(1){}   //asm("trap");
		return(SD_COM_CRC_FAILED);
	}

	if (response_r1 & SD_R1_ILLEGAL_CMD)
	{
		while(1){}   //asm("trap");
		return(SD_ILLEGAL_CMD);
	}

	if (response_r1 & SD_R1_CARD_ECC_FAILED)
	{
		while(1){}   //asm("trap");
		return(SD_CARD_ECC_FAILED);
	}

	if (response_r1 & SD_R1_CC_ERROR)
	{
		while(1){}   //asm("trap");
		return(SD_CC_ERROR);
	}

	if (response_r1 & SD_R1_GENERAL_UNKNOWN_ERROR)
	{
		while(1){}   //asm("trap");
		return(SD_GENERAL_UNKNOWN_ERROR);
	}

	if (response_r1 & SD_R1_STREAM_READ_UNDERRUN)
	{
		while(1){}   //asm("trap");
		return(SD_STREAM_READ_UNDERRUN);
	}

	if (response_r1 & SD_R1_STREAM_WRITE_OVERRUN)
	{
		while(1){}   //asm("trap");
		return(SD_STREAM_WRITE_OVERRUN);
	}

	if (response_r1 & SD_R1_CID_CSD_OVERWRIETE)
	{
		while(1){}   //asm("trap");
		return(SD_CID_CSD_OVERWRITE);
	}

	if (response_r1 & SD_R1_WP_ERASE_SKIP)
	{
		while(1){}   //asm("trap");
		return(SD_WP_ERASE_SKIP);
	}

	if (response_r1 & SD_R1_CARD_ECC_DISABLED)
	{
		while(1){}   //asm("trap");
		return(SD_CARD_ECC_DISABLED);
	}

	if (response_r1 & SD_R1_ERASE_RESET)
	{
		while(1){}   //asm("trap");
		return(SD_ERASE_RESET);
	}

	if (response_r1 & SD_R1_AKE_SEQ_ERROR)
	{
		while(1){}   //asm("trap");
		return(SD_AKE_SEQ_ERROR);
	}

	return(errorstatus);
}

/*******************************************************************************
* Function Name  : SD_ReadBlock
* Description    : Allows to read blocks from a specified address  in a card.
* Input          : - addr: Address from where data are to be read.
*                  - readbuff: pointer to the buffer that will contain the
*                    received data.
*                  - BlockSize: the SD card Data block size.
*                  - NumberOfBlocks: number of blocks to be read.
* Output         : None
* Return         : SD_Error: SD Card Error code.
*
*******************************************************************************/

SD_Error SD_ReadBlock(U32 card_addr,   //from
						U32 *readbuff, //to
						U16 BlockSize, U32 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	U32 TotalNumberOfBytes = 0;
	U32 *p_databuff = readbuff;
	U32 rstatus;
	U32 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;
#if DEBUG_PRINT
	U32 status,response;
#endif
#if (USE_INTERNAL_DMAC==0)
	U32 i,word_len;
#endif
	MFSPR(tmp1, 268);
	if (0 == readbuff) {
		errorstatus = SD_INVALID_PARAMETER;
		while(1){}   //asm("trap");
		return(errorstatus);
	}

	TotalNumberOfBytes = NumberOfBlocks * BlockSize;
/*
	if( TotalNumberOfBytes > (FIFO_DEPTH <<2) ) {
		TotalNumberOfBytes = (FIFO_DEPTH <<2);
		while(1){}   //asm("trap");
	}
*/
#if USE_INTERNAL_DMAC
	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR,
							(U32)p_databuff, TotalNumberOfBytes );

	#if 0
		uart_printf_buf("before dma\r\n");
		DWC_SD_Host_RegTest();
	#endif
#else
	//
#endif
	
	MFSPR(tmp2, 268);
	if( NumberOfBlocks > 1 ) {
		CMD18_SD_tsk( card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	}
	else {
		CMD17_SD_tsk( card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	}

	MFSPR(tmp3, 268);
#if 0
	rstatus = R32(SDMMC_RINTSTS);
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	if(rstatus & DWC_MSH_DATA_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_DATA_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}
#endif
	if(NumberOfBlocks > 1)	{
		errorstatus = SD_StopTransfer();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
	}


#if DEBUG_PRINT
	rstatus = R32(SDMMC_RINTSTS);

	if( NumberOfBlocks > 1 ) {
		uart_printf(" C18-C12.1.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d,c=%dW)..\r\n",
				response=R32(SDMMC_RESP0),
				rstatus,
				status=R32(SDMMC_STATUS),
				SDMMC_GET_R_IDX(status),
				SDMMC_GET_FCNT(status)
				);
		//
		uart_printf(" C18-C12.1:TCBCNT=%d/TBBCNT=%d..\r\n",
				R32(SDMMC_TCBCNT),R32(SDMMC_TBBCNT) );
	}
	else{
	//	uart_printf(" C17.1.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d,c=%dW)..\r\n",
		uart_printf(" C17.1.Res=0x%x(cur(%s)sate,%s_FOR_DATA). R_S=0x%x/S=0x%x(c=%dW)..\r\n",
				response=R32(SDMMC_RESP0),
				R1_CurState_string( (U8)R1_CURRENT_STATE(response) ),
				(response & R1_READY_FOR_DATA) ?"READY":"not READY",
				rstatus,
				status=R32(SDMMC_STATUS),
			//	SDMMC_GET_R_IDX(status), //see R1
				SDMMC_GET_FCNT(status)
				);
		// 0x900(cur(TRAN)sate,READY_FOR_DATA). 0x0/0x1008902(idx=17,c=128W)..

		uart_printf(" C17:TCBCNT=%d/TBBCNT=%d..\r\n",
				R32(SDMMC_TCBCNT),R32(SDMMC_TBBCNT) );
		//512/0

		#if USE_INTERNAL_DMAC
		//C17.1.0x900(cur(TRAN)sate,READY_FOR_DATA). 0x0/0x8906(c=0W)
		//512/512
		#endif
	}
#endif
	MFSPR(tmp4, 268);
#if USE_INTERNAL_DMAC
	//dis
	SDIO_Internal_Dmac_Ctrl( 0 );
#else

	word_len = SDMMC_GET_FCNT( R32(SDMMC_STATUS) );
	for(i=0; i<word_len; i++) {
		*p_databuff =  R32(SDMMC_FIFO_DATA);
		p_databuff++;
	}

	while( !(R32(SDMMC_STATUS) & SDMMC_FIFO_EMPTY) )
	{
		while(1){}   //asm("trap");

		*p_databuff = R32(SDMMC_FIFO_DATA);
		p_databuff++;
	}
#endif

	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);

#if DEBUG_PRINT
	rstatus = R32(SDMMC_RINTSTS);

	if( NumberOfBlocks > 1 ) {
		uart_printf(" C18-C12.2.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d,c=%dW)..\r\n",
				response=R32(SDMMC_RESP0),
				rstatus,
				status=R32(SDMMC_STATUS),
				SDMMC_GET_R_IDX(status),
				SDMMC_GET_FCNT(status)
				);
		//
		uart_printf(" C18-C12.2:TCBCNT=%d/TBBCNT=%d..\r\n",
				R32(SDMMC_TCBCNT),R32(SDMMC_TBBCNT) );
	}
	else{
		uart_printf(" C17.2.Res=0x%x. R_S=0x%x/S=0x%x(R_idx=%d,c=%dW)..\r\n",
				response=R32(SDMMC_RESP0),
				rstatus,
				status=R32(SDMMC_STATUS),
				SDMMC_GET_R_IDX(status), //see R1
				SDMMC_GET_FCNT(status)
				);
		//0x900. 0x0/0x8906(R_idx=17,c=0W)

		uart_printf(" C17:TCBCNT=%d/TBBCNT=%d..\r\n",
				R32(SDMMC_TCBCNT),R32(SDMMC_TBBCNT) );//=512/512
	}
#endif
	MFSPR(tmp5, 268);
	tmp6=tmp5-tmp1;
	return(errorstatus);
}

SD_Error SD_ReadBlock_simple(U32 card_addr,   //from
						U32 *readbuff, //to
						U16 BlockSize, U32 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	U32 cardstate = 0;
	U32 TotalNumberOfBytes = 0;
	U32 *p_databuff = readbuff;
	U32 rstatus;
	U32 data, status;
	struct idmac_desc_64addr *desc;
	U32 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
#if (USE_INTERNAL_DMAC==0)
	U32 i,word_len;
#endif

#if 0
	if (writebuff == 0) {
		errorstatus = SD_INVALID_PARAMETER;
		while(1){}   //asm("trap");
		return(errorstatus);
	}
#endif
	TotalNumberOfBytes = NumberOfBlocks * BlockSize;
#if 0
	if( TotalNumberOfBytes > (FIFO_DEPTH <<2) ) {
		TotalNumberOfBytes = (FIFO_DEPTH <<2);
		while(1){}   //asm("trap");
	}
#endif
	

	MFSPR(tmp1, 268);
#if 0
	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR, (U32)p_databuff, TotalNumberOfBytes );
#else
	desc = (struct idmac_desc_64addr *)IDMA_DESCRIPTION_ADDR;
	desc->des0 = IDMAC_DES0_OWN
						| IDMAC_DES0_DIC
						| IDMAC_DES0_CH;
	
	desc->des4 = (U32)p_databuff;
#endif
	

#if 0
	if(NumberOfBlocks > 1)	{
		CMD25_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	} else {
		CMD24_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	}
#else
	W32(SDMMC_BYTCNT, TotalNumberOfBytes);
	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );
#if USE_INTERNAL_DMAC
	W32(SDMMC_DBADDRL, IDMA_DESCRIPTION_ADDR);
	W32(SDMMC_DBADDRU, SDMMC_BUFFERs_BASE_HI );

	
#if 0
	SDIO_Internal_Dmac_Ctrl(1);
#else
	/* issue a IDMAC software reset */
	data = R32(SDMMC_BMOD);
	W32( SDMMC_BMOD, data|SDMMC_BMOD_SWR );
	data = R32(SDMMC_BMOD);
	while( data &  SDMMC_BMOD_SWR) {
		data = R32(SDMMC_BMOD);
	}
#endif
	
#endif
	
	status = R32(SDMMC_CARDTHRCTL);
	W32(SDMMC_CARDTHRCTL, (status | 1<<1));		//Enable BsyClrIntEn
	MFSPR(tmp2, 268);
	W32(SDMMC_CMDARG, card_addr);
	//R1.+data.

	W32(SDMMC_CMD, SDMMC_CMD_START
				|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP
				|SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_RD
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
				|SDMMC_CMD_SEND_AUTO_STOP		//S1020 add for test
				|(EMMC_SLOT0 <<16)
				|SDMMC_CMD_INDX(18));			//Multi
//				|SDMMC_CMD_INDX(17));			//Single


#if SDMMC_HOST_INT_TEST
//	InstallInterruptHandler0( SDMMC_HOST_INT_NUM, SD_interrupt_isr_DTO );

	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	MFSPR(tmp3, 268);

	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER|SDMMC_INT_BCI);

#endif
	
	MFSPR(tmp4, 268);
//	errorstatus = SD_StopTransfer();
//	while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY );
	MFSPR(tmp5, 268);
#endif
	
#if 0	
	rstatus = R32(SDMMC_RINTSTS);
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	if(rstatus & DWC_MSH_DATA_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_DATA_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}

	if(NumberOfBlocks > 1)	{
		errorstatus = SD_StopTransfer();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
	}
#endif
	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);
#if 0
	/* Wait till the card is in programming state */
	do	{
		while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy

		CMD13_SD_tsk( g_RCA,EMMC_SLOT0 );
/*		rstatus = R32(SDMMC_RINTSTS);
		if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)	{
			W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
			while(1){}   //asm("trap");
			return SD_ERROR;
		}

		errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
*/
		cardstate = (R32(SDMMC_RESP0)>>9) & 0xf;
	}while((errorstatus == SD_OK)
			&& ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING))
			);
	MFSPR(tmp4, 268);
	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);
#else
//	while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy
#endif
#if USE_INTERNAL_DMAC
	//dis
//	SDIO_Internal_Dmac_Ctrl( 0 );
#endif

	MFSPR(tmp6, 268);
	tmp7 = tmp6 - tmp1;
	return(errorstatus);
}

/*******************************************************************************
* Function Name  : SD_WriteBlock
* Description    : Allows to write blocks starting from a specified address in
*                  a card.
* Input          : - addr: Address from where data are to be read.
*                  - writebuff: pointer to the buffer that contain the data to be
*                    transferred.
*                  - BlockSize: the SD card Data block size.
*                  - NumberOfBlocks: number of blocks to be written.
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
SD_Error SD_WriteBlock(U32 card_addr,
						U32 *writebuff,
						U16 BlockSize, U32 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	U32 cardstate = 0;
	U32 TotalNumberOfBytes = 0;
	U32 *p_databuff = writebuff;
	U32 rstatus;
	U32 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
#if (USE_INTERNAL_DMAC==0)
	U32 i,word_len;
#endif
	MFSPR(tmp1, 268);
#if 0
	if (writebuff == 0) {
		errorstatus = SD_INVALID_PARAMETER;
		while(1){}   //asm("trap");
		return(errorstatus);
	}
#endif
	TotalNumberOfBytes = NumberOfBlocks * BlockSize;
/*
		if( TotalNumberOfBytes > (FIFO_DEPTH <<2) ) {
		TotalNumberOfBytes = (FIFO_DEPTH <<2);
		while(1){}   //asm("trap");
	}
*/
#if USE_INTERNAL_DMAC
	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR,
							(U32)p_databuff, TotalNumberOfBytes );
#else
	word_len = (TotalNumberOfBytes >>2);
	for(i=0; i< word_len; i++)
	{
		W32(SDMMC_FIFO_DATA, *p_databuff);
		p_databuff++;
	}
#endif
	MFSPR(tmp2, 268);
	if(NumberOfBlocks > 1)	{
		CMD25_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	} else {
		CMD24_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	}
	MFSPR(tmp3, 268);
	rstatus = R32(SDMMC_RINTSTS);
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	if(rstatus & DWC_MSH_DATA_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_DATA_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}
	MFSPR(tmp4, 268);
	if(NumberOfBlocks > 1)	{
		errorstatus = SD_StopTransfer();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
	}

	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);
	MFSPR(tmp5, 268);
	/* Wait till the card is in programming state */
	do	{
		while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy

		CMD13_SD_tsk( g_RCA,EMMC_SLOT0 );
		rstatus = R32(SDMMC_RINTSTS);
		if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)	{
			W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
			while(1){}   //asm("trap");
			return SD_ERROR;
		}

		errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}

		cardstate = (R32(SDMMC_RESP0)>>9) & 0xf;
	}while((errorstatus == SD_OK)
			&& ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING))
			);
	MFSPR(tmp6, 268);
	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);

#if USE_INTERNAL_DMAC
	//dis
	SDIO_Internal_Dmac_Ctrl( 0 );
#endif
	MFSPR(tmp7, 268);
	tmp8 = tmp7-tmp1;
	return(errorstatus);
}

SD_Error SD_WriteBlock_simple(U32 card_addr,
						U32 *writebuff,
						U16 BlockSize, U32 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	U32 cardstate = 0;
	U32 TotalNumberOfBytes = 0;
	U32 *p_databuff = writebuff;
	U32 rstatus;
	U32 data, status;
	struct idmac_desc_64addr *desc;
	U32 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
#if (USE_INTERNAL_DMAC==0)
	U32 i,word_len;
#endif

#if 0
	if (writebuff == 0) {
		errorstatus = SD_INVALID_PARAMETER;
		while(1){}   //asm("trap");
		return(errorstatus);
	}
#endif
	TotalNumberOfBytes = NumberOfBlocks * BlockSize;
#if 0
	if( TotalNumberOfBytes > (FIFO_DEPTH <<2) ) {
		TotalNumberOfBytes = (FIFO_DEPTH <<2);
		while(1){}   //asm("trap");
	}
#endif
	
	MFSPR(tmp1, 268);
#if 1
	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR, (U32)p_databuff, TotalNumberOfBytes );
#else
	desc = IDMA_DESCRIPTION_ADDR;
	desc->des0 = IDMAC_DES0_OWN
						| IDMAC_DES0_DIC
						| IDMAC_DES0_CH;
	
	desc->des4 = p_databuff;
#endif
	
#if 0
	if(NumberOfBlocks > 1)	{
		CMD25_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	} else {
		CMD24_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	}
#else
	W32(SDMMC_BYTCNT, TotalNumberOfBytes);
	W32(SDMMC_BLKSIZ, BLOCK_SIZE);

	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );
#if USE_INTERNAL_DMAC
	W32(SDMMC_DBADDRL, IDMA_DESCRIPTION_ADDR);
	W32(SDMMC_DBADDRU, SDMMC_BUFFERs_BASE_HI );

	
#if 0
	SDIO_Internal_Dmac_Ctrl(1);
#else
	/* issue a IDMAC software reset */
	data = R32(SDMMC_BMOD);
	W32( SDMMC_BMOD, data|SDMMC_BMOD_SWR );
	data = R32(SDMMC_BMOD);
	while( data &  SDMMC_BMOD_SWR) {
		data = R32(SDMMC_BMOD);
	}
#endif
	
#endif
	
//	status = R32(SDMMC_CARDTHRCTL);
//	W32(SDMMC_CARDTHRCTL, (status | 1<<1));		//Enable BsyClrIntEn
	
	MFSPR(tmp2, 268);
	W32(SDMMC_CMDARG, card_addr);
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START				//
				|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP				//
				| SDMMC_CMD_PRV_DAT_WAIT		//
				|SDMMC_CMD_DAT_WR				//
				|SDMMC_CMD_RESP_CRC				//
				|SDMMC_CMD_DAT_EXP				//
				|SDMMC_CMD_SEND_AUTO_STOP		//S1020 add for test
				|(EMMC_SLOT0 <<16)				
				|SDMMC_CMD_INDX(25));			//Multi
//				|SDMMC_CMD_INDX(24));			//Single		


#if SDMMC_HOST_INT_TEST
//	InstallInterruptHandler0( SDMMC_HOST_INT_NUM, SD_interrupt_isr_DTO );

	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	MFSPR(tmp3, 268);

	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER|SDMMC_INT_BCI);

#endif
	
	MFSPR(tmp4, 268);
//	errorstatus = SD_StopTransfer();
	while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY );
	MFSPR(tmp5, 268);
#endif
	
#if 0	
	rstatus = R32(SDMMC_RINTSTS);
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	if(rstatus & DWC_MSH_DATA_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_DATA_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}

	if(NumberOfBlocks > 1)	{
		errorstatus = SD_StopTransfer();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
	}
#endif
	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);
#if 0
	/* Wait till the card is in programming state */
	do	{
		while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy

		CMD13_SD_tsk( g_RCA,EMMC_SLOT0 );
/*		rstatus = R32(SDMMC_RINTSTS);
		if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)	{
			W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
			while(1){}   //asm("trap");
			return SD_ERROR;
		}

		errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
*/
		cardstate = (R32(SDMMC_RESP0)>>9) & 0xf;
	}while((errorstatus == SD_OK)
			&& ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING))
			);
	MFSPR(tmp4, 268);
	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);
#else
//	while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy
#endif
#if USE_INTERNAL_DMAC
	//dis
//	SDIO_Internal_Dmac_Ctrl( 0 );
#endif

	MFSPR(tmp6, 268);
	tmp7 = tmp6 - tmp1;
	return(errorstatus);
}

/*******************************************************************************
* Function Name  : SD_WriteBlock
* Description    : Allows to write blocks starting from a specified address in
*                  a card.
* Input          : - addr: Address from where data are to be read.
*                  - writebuff: pointer to the buffer that contain the data to be
*                    transferred.
*                  - BlockSize: the SD card Data block size.
*                  - NumberOfBlocks: number of blocks to be written.
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
SD_Error SD_WriteBlock_2G(U32 card_addr,
						U32 *writebuff,
						U16 BlockSize, U32 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	U32 cardstate = 0;
	U32 TotalNumberOfBytes = 0;
	U32 *p_databuff = writebuff;
	U32 rstatus;

	TotalNumberOfBytes = NumberOfBlocks * BlockSize;

//	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR,(U32)p_databuff, TotalNumberOfBytes );

#if 1
	struct idmac_desc_64addr *desc;
	desc = (struct idmac_desc_64addr *)IDMA_DESCRIPTION_ADDR;
	desc->des0 = IDMAC_DES0_OWN
						| IDMAC_DES0_DIC
						| IDMAC_DES0_CH;
	
	desc->des4 = (U32)p_databuff;
#endif
	
#if 0
	if(NumberOfBlocks > 1)	{
		CMD25_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	} else {
		CMD24_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	}
#else	

	W32(SDMMC_CARDTHRCTL, (BLOCK_SIZE <<16)|SDMMC_WRTHR_EN );
#if USE_INTERNAL_DMAC
	W32(SDMMC_DBADDRL, IDMA_DESCRIPTION_ADDR);
#endif
	SDIO_Internal_Dmac_Ctrl(1);			//Enable DMA
	
	W32(SDMMC_CMDARG, card_addr);
	//R1.+data.
	W32(SDMMC_CMD, SDMMC_CMD_START|SDMMC_CMD_USE_HOLD
				|SDMMC_CMD_RESP_EXP| SDMMC_CMD_PRV_DAT_WAIT
				|SDMMC_CMD_DAT_WR
				|SDMMC_CMD_RESP_CRC
				|SDMMC_CMD_DAT_EXP
				|SDMMC_CMD_SEND_AUTO_STOP					//add for test, not need CMD12
				|(EMMC_SLOT0 <<16)|SDMMC_CMD_INDX(25));

#if SDMMC_HOST_INT_TEST
	while( g_intr_DATA_OVER == 0 ){}
	g_intr_DATA_OVER = 0;
#else
	SDIO_PollStatus(SDMMC_INT_CMD_DONE| SDMMC_INT_DATA_OVER);
//	SDIO_PollStatus(SDMMC_INT_CMD_DONE);					//S1020 test

#endif
#endif	

#if 0
	rstatus = R32(SDMMC_RINTSTS);
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
/*	if(rstatus & DWC_MSH_DATA_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_DATA_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}
	*/
#endif
		
#if 0
	if(NumberOfBlocks > 1)	{
		errorstatus = SD_StopTransfer();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
	}
#else
//	CMD12_SD_tsk( EMMC_SLOT0 );			//StopTransfer
#endif
	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);
#if 0
	/* Wait till the card is in programming state */
	do	{
		while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy

		CMD13_SD_tsk( g_RCA,EMMC_SLOT0 );
		rstatus = R32(SDMMC_RINTSTS);
		if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)	{
			W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
			while(1){}   //asm("trap");
			return SD_ERROR;
		}

		errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}

		cardstate = (R32(SDMMC_RESP0)>>9) & 0xf;
	}while((errorstatus == SD_OK)
			&& ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING))
			);
#endif
	
	//add point
	while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy
	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);

#if USE_INTERNAL_DMAC
	//dis
	SDIO_Internal_Dmac_Ctrl( 0 );
#endif

	return(errorstatus);
}


SD_Error SD_WriteBlock_DDR(U32 card_addr,
						U32 *writebuff,
						U16 BlockSize, U32 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	U32 cardstate = 0;
	U32 TotalNumberOfBytes = 0;
	U32 *p_databuff = writebuff;
	U32 rstatus;
#if (USE_INTERNAL_DMAC==0)
	U32 i,word_len;
#endif

	TotalNumberOfBytes = NumberOfBlocks * BlockSize;
	if( TotalNumberOfBytes > (FIFO_DEPTH <<2) ) {
		TotalNumberOfBytes = (FIFO_DEPTH <<2);
		while(1){}   //asm("trap");
	}

#if USE_INTERNAL_DMAC
	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR,
							(U32)p_databuff, TotalNumberOfBytes );
#else
	word_len = (TotalNumberOfBytes >>2);
	for(i=0; i< word_len; i++)
	{
		W32(SDMMC_FIFO_DATA, *p_databuff);
		p_databuff++;
	}
#endif

	if(NumberOfBlocks > 1)	{
		CMD25_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	} else {
		CMD24_SD_tsk(card_addr, TotalNumberOfBytes, EMMC_SLOT0 );
	}

	rstatus = R32(SDMMC_RINTSTS);
	if(rstatus & DWC_MSH_CMD_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	if(rstatus & DWC_MSH_DATA_ERROR_FLAGS) {
		W32(SDMMC_RINTSTS, DWC_MSH_DATA_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}
	errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK) {
		while(1){}   //asm("trap");
		return(errorstatus);
	}

	if(NumberOfBlocks > 1)	{
		errorstatus = SD_StopTransfer();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}
	}

	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);

	/* Wait till the card is in programming state */
	do	{
		while(R32(SDMMC_STATUS) & SDMMC_STATUS_BUSY ); //1: card data busy

		CMD13_SD_tsk( g_RCA,EMMC_SLOT0 );
		rstatus = R32(SDMMC_RINTSTS);
		if(rstatus & DWC_MSH_CMD_ERROR_FLAGS)	{
			W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
			while(1){}   //asm("trap");
			return SD_ERROR;
		}

		errorstatus = SD_CmdResp1Error();
		if (errorstatus != SD_OK)	{
			while(1){}   //asm("trap");
			return(errorstatus);
		}

		cardstate = (R32(SDMMC_RESP0)>>9) & 0xf;
	}while((errorstatus == SD_OK)
			&& ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING))
			);

	/* Clear all the static flags */
	W32(SDMMC_RINTSTS, 0xFFFFFFFF);

#if USE_INTERNAL_DMAC
	//dis
	SDIO_Internal_Dmac_Ctrl( 0 );
#endif

	return(errorstatus);
}

/*******************************************************************************
* Function Name  : SD_StopTransfer
* Description    : Aborts an ongoing data transfer.
* Input          : None
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
SD_Error SD_StopTransfer(void)
{
	SD_Error errorstatus = SD_OK;
	U32 status;

	CMD12_SD_tsk( EMMC_SLOT0 );
	status = R32(SDMMC_RINTSTS);
	if(status & DWC_MSH_CMD_ERROR_FLAGS)
	{
		W32(SDMMC_RINTSTS, DWC_MSH_CMD_ERROR_FLAGS);
		while(1){}   //asm("trap");
		return SD_ERROR;
	}

	errorstatus = SD_CmdResp1Error();
	return(errorstatus);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
	U32 g_databuf[128] ={0};
#else
	U32 g_databuf = (SDMMC_BUFFERs_BASE + 0x1000);
//	U32 g_databuf = (SDMMC_BUFFERs_BASE + 0x100);
#endif


/*	EMMC device test demo
 * Synopsys DesignWare Cores Mobile Storage Host Driver
 */
void emmc_main( void )
{
	SD_Error status;
	U32 i;//, j,
	U32 lba, secCnt;
	U32 *ddr_ptr, *check_ptr, *p_databuff;
	U32 tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4=0, tmp5=0, tmp6=0;

#if 0
	SetTLB( 0x0,
			SD_EMMC_HOST_BASE,
			TLB0_SIZE_1M,				//TLB0_SIZE_64K //TLB0_SIZE_4K
			TLB2_I_MASK|TLB2_S_MASK|TLB2_U_MASK|TLB2_G_MASK|TLB2_E_LIT,
			SD_EMMC_HOST_BASE|0x30);
#endif

#if DEBUG_PRINT
	/* reg test
	 * */
	DWC_SD_Host_RegTest();
	SDIO_HCON();
#endif

	status = SD_EnumCard();

#if SINGLE_BLOCK_TEST
	/* read one block test, secCnt = 1 */
#if 1
	if(status == SD_OK)
	{
		lba = 0;
		secCnt = 1;

		/* 128 word */
		for( i=0; i<(secCnt <<7); i++ ) { //word
			*( (U32*)g_databuf+i ) = 0;
		}

		/* in block (512 Byte) units in a High Capacity SD Memory Card */
		status = SD_ReadBlock( lba, (U32*)g_databuf, BLOCK_SIZE, secCnt);
		if(status != SD_OK) {
			while(1){}   //asm("trap");
		}

		#if 0
			DWC_SD_Host_RegTest();
		#endif
			uart_printf("\r\n--SD_ReadBlock test(lba=%d):\r\n",lba );
		#if 1//DEBUG_PRINT
			for( i=0; i<(secCnt <<7); i++ ) {
				uart_printf(" [%d] = 0x%x, ",i,
					le32_to_cpu_emmc(  *((U32*)g_databuf+i) )   );
			}
			uart_printf_buf("\r\n");
		#endif
		#if 0
		if( (le32_to_cpu( *((U32*)g_databuf+127)) & 0x0000FFFF)
			!= 0x000055AA) {
			while(1){}   //asm("trap");
		}
		#endif
	}
	else {
		while(1){}   //asm("trap");
	}
#endif

	lba = 0;//256;
	/* write one block test, secCnt = 1 */
#if 1
	secCnt = 1;

	#if 1
		for( i=0; i<(secCnt <<7); i++ ) {
			*((U32*)g_databuf+i) = 0x24681357 + i;
		}

		status = SD_WriteBlock( lba+0, (U32*)g_databuf, BLOCK_SIZE, secCnt );
		if(status != SD_OK) {
			while(1){}   //asm("trap");
		}

		//lba+1, next block
		for(i=0; i<(secCnt <<7); i++) {
			*((U32*)g_databuf+i) = 0x5a5a5a5a + i;
		}
		status = SD_WriteBlock( lba+1, (U32*)g_databuf, BLOCK_SIZE, secCnt );
	#endif
	#if 1
		//clear used buf
		for( i=0; i<(secCnt <<7); i++ ) {
			*((U32*)g_databuf+i)  = 0;
		}
		SD_ReadBlock( lba, (U32*)g_databuf, BLOCK_SIZE, secCnt);
		for(i=0; i<(secCnt <<7); i++) {
			if (  (*((U32*)g_databuf+i)  ) != (0x24681357 + i)  ){
				while(1){}   //asm("trap");
			}
		}
		uart_printf("--SD_ReadBlock after Write test(lba=%d):\r\n",lba );
		#if 1//DEBUG_PRINT
			for( i=0; i<(secCnt <<7); i++ ) {
				uart_printf(" [%d] = 0x%x, ",i,
						//	le32_to_cpu(  *((U32*)g_databuf+i) )   );
						*((U32*)g_databuf+i)  );
			}
			uart_printf_buf("\r\n");
		#endif

		//lba+1
		//clear used buf
		for( i =0; i <(secCnt <<7); i++) {
			*((U32*)g_databuf+i)  = 0;
		}
		SD_ReadBlock( lba+1, (U32*)g_databuf, BLOCK_SIZE, secCnt);
		for(i=0; i<(secCnt <<7); i++) {
			if (  (*((U32*)g_databuf+i)  ) != (0x5a5a5a5a + i)  ){
				while(1){}   //asm("trap");
			}
		}
		uart_printf("--SD_ReadBlock after Write test(lba=%d):\r\n",lba+1 );
		#if 1//DEBUG_PRINT
			for( i=0; i<(secCnt <<7); i++ ) {
				uart_printf(" [%d] = 0x%x, ",i,
						//	le32_to_cpu(  *((U32*)g_databuf+i) )   );
						*((U32*)g_databuf+i)  );
			}
			uart_printf_buf("\r\n");
		#endif
	#endif
#endif
#endif

#if MULTI_BLOCK_TEST
/* multi block write & read test */
#if 1
	lba += 2;
//	secCnt = 4; //2K
	secCnt = 8; //4K

	#if 1
		for( i=0; i<(secCnt <<7); i++ ) {
			*((U32*)g_databuf+i) = 0x55aaabcd + i;
		}
		uart_printf("---WRITE_MULTIPLE_BLOCK from lba%d - 0x%xbytes:\r\n",
						lba,secCnt*BLOCK_SIZE );

		status = SD_WriteBlock( lba+0, (U32*)g_databuf, BLOCK_SIZE, secCnt );
		if(status != SD_OK) {
			while(1){}   //asm("trap");
		}
	#endif
	#if 1
		//clear used buf
		for( i=0; i<(secCnt <<7); i++ ) {
			*((U32*)g_databuf+i)  = 0;
		}
		SD_ReadBlock( lba+0, (U32*)g_databuf, BLOCK_SIZE, secCnt);
		for(i=0; i<(secCnt<<7); i++) {
			if (  (*((U32*)g_databuf+i)  ) != (0x55aaabcd + i)  ){
				while(1){}   //asm("trap");
			}
		}

		uart_printf("---READ_MULTIPLE_BLOCK from lba%d - 0x%xbytes:\r\n",
						lba,secCnt*BLOCK_SIZE );
		#if 1//DEBUG_PRINT
			for( i=0; i<(secCnt <<7); i++ ) {
				if( i %(1 <<7) ==0 )
					uart_printf_buf("\r\n");

				uart_printf(" [%d] = 0x%x, ",i,
						//	le32_to_cpu(  *((U32*)g_databuf+i) )   );
						*((U32*)g_databuf+i)  );
			}
			uart_printf_buf("\r\n");
		#endif
	#endif
#endif
#endif
			
#if WHOLE_BLOCK_TEST
#if 1			
	/* 2GB eMMC multi block write&read test */	
	for(lba=0; lba<0x8000; lba+=200)				//0x80000*4K=2G	
	{
		secCnt = 8; //4K

	#if 1
		for( i=0; i<(secCnt <<7); i++ ) {
			*((U32*)g_databuf+i) = 0x55aabbcc + i*3 + lba*5;
		}
		status = SD_WriteBlock( lba+0, (U32*)g_databuf, BLOCK_SIZE, secCnt );
//		status = SD_WriteBlock_simple( lba+0, (U32*)g_databuf, BLOCK_SIZE, secCnt );		
		if(status != SD_OK) {
			while(1){}   //asm("trap");
		}
	#endif
		
	#if 1
		//clear used buf
		for( i=0; i<(secCnt <<7); i++ ) {
			*((U32*)g_databuf+i)  = 0;
		}
		SD_ReadBlock( lba+0, (U32*)g_databuf, BLOCK_SIZE, secCnt);
		for(i=0; i<(secCnt<<7); i++) {
			if (  (*((U32*)g_databuf+i)  ) != (0x55aabbcc + i*3 + lba*5)  ){
				while(1){}   //asm("trap");
			}
		}
	
		uart_printf("---READ_MULTIPLE_BLOCK from lba%d - 0x%xbytes:\r\n",
						lba,secCnt*BLOCK_SIZE );
		uart_printf_buf("\r\n");
	#endif

	}
#else
	/*enable cache*/
	set_e0_tlb(0x10050000,0xc0000a00,0x0000000a,0x0000003f,0x00000000,0x00000000);
	set_e0_tlb(0x10060000,0xc0000a00,0x4000000a,0x4000003f,0x00000000,0x00000000);	
	set_e0_tlb(0x10030000,0xc0000500,0xe0000004,0xe000003f,0x00000000,0x00000004);

	/*cover all 2G DDR*/
	secCnt = 8*0x10;	//64K
//	secCnt = 8;		 	//4K
//	secCnt = 4;		 	//2K
//	secCnt = 1;		 	//512B
	
	ddr_ptr = 0x00040000;	//data start addr
//	ddr_ptr = 0xE0062000;	//data start addr
	
	for(lba=0; lba<0x4000; lba++)	
		*((U32*)ddr_ptr+lba) = 0x12345678+lba;
		
	/*disable cache
	set_e0_tlb(0x10050000,0xc0000a00,0x0000000a,0x0000003f,0x00000000,0x00000000);
	set_e0_tlb(0x10060000,0xc0000a00,0x4000000a,0x4000003f,0x00000000,0x00000000);	
	set_e0_tlb(0x10030000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);
	 */

	p_databuff=ddr_ptr;
	
//	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR, (U32)p_databuff, secCnt*BLOCK_SIZE );
//	W32(SDMMC_BYTCNT, 0x1000);			//4K
	
	SDIO_Config_Idma_Desc( IDMA_DESCRIPTION_ADDR, (U32)p_databuff, secCnt*BLOCK_SIZE );
	W32(SDMMC_BYTCNT, secCnt*BLOCK_SIZE );	//32M
	
//	W32(SDMMC_BYTCNT, 0x1000);			//4K
//	W32(SDMMC_BYTCNT, 0x800);			//2K
//	W32(SDMMC_BYTCNT, 0x200);			//512
	SDIO_Internal_Dmac_Ctrl(1);

	MFSPR(tmp1, 268);	
	/*write 2G eMMC*/ 
	for(lba=0; lba<0x80; lba++)		//0x80*512 size	
//	for(lba=0; lba<0x400000; lba+=0x80)	//0x400000*512 size, 2G	
	{
//		status = SD_WriteBlock_2G( lba+0, ((U32*)ddr_ptr+lba*BLOCK_SIZE/4), BLOCK_SIZE, secCnt );
//		status = SD_WriteBlock( lba+0, ((U32*)ddr_ptr+lba*BLOCK_SIZE/4), BLOCK_SIZE, secCnt );
		status = SD_WriteBlock_simple( lba, ((U32*)ddr_ptr), BLOCK_SIZE, secCnt );
	}

//	status = SD_WriteBlock_simple( lba, ((U32*)ddr_ptr), BLOCK_SIZE, secCnt );
//	status = SD_WriteBlock( lba, ((U32*)ddr_ptr), BLOCK_SIZE, secCnt );

	MFSPR(tmp2, 268);
	tmp3 = tmp2 - tmp1;
	
	//disable cache
	set_e0_tlb(0x10030000,0xc0000500,0xe000000a,0xe000003f,0x00000000,0x00000004);

	/*read and check 2G eMMC*/
	check_ptr = 0x80000;	
	for(lba=0; lba<0x80; lba++)			//64K	
//	for(lba=0; lba<0x400000; lba+=0x80)	//0x400000*512 size, 2G			
	{
#if 1		
		for(i=0; i<0x4000; i++)	
			*((U32*)check_ptr+i) = 0;
#endif
	MFSPR(tmp4, 268);
	SD_ReadBlock( lba, (U32*)check_ptr, BLOCK_SIZE, secCnt);
//	SD_ReadBlock_simple( lba+7, (U32*)check_ptr, BLOCK_SIZE, secCnt);
	MFSPR(tmp5, 268);
	tmp6 = tmp5 - tmp4;

#if 1		
	for(i=0; i<0x4000; i++)				//64K	
		if( *((U32*)ddr_ptr+i) != *((U32*)check_ptr+i) )
			while(1);
#endif	
	}


#endif
#endif
//	while(1){}   //asm("trap");
//	while( 1 );
}

