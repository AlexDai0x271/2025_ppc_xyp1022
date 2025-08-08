/*******************************************************
 * File: usb_dwc3_core.c
 * Description:DesignWare Cores SuperSpeed USB 3.0 Controller/Global Register/Basic Configuration
 * Date: 2020/07
 * Author: ywang@ccore_ip		gylan@ccore_ip
 *
 ******************************************************/


#include "ppclib.h"
#include "e500mc_asm.h"
#include "mpic.h"
#include "common_usb.h"
#include "usb30.h"
#include "uart.h"
#include "as_archppc470.h"

#include "usb_reg_dwc3.h"
#include "usb_reg_xhci.h"


#define DEBUG_PRINT 1

enum DWC3_link_state {
	DWC3_LINK_STATE_U0		= 0x00, /* in HS, means ON */
	DWC3_LINK_STATE_U1		= 0x01,
	DWC3_LINK_STATE_U2		= 0x02, /* in HS, means SLEEP */
	DWC3_LINK_STATE_U3		= 0x03, /* in HS, means SUSPEND */
	DWC3_LINK_STATE_SS_DIS		= 0x04,
	DWC3_LINK_STATE_RX_DET		= 0x05, /* in HS, means Early Suspend */
	DWC3_LINK_STATE_SS_INACT	= 0x06,
	DWC3_LINK_STATE_POLL		= 0x07,
	DWC3_LINK_STATE_RECOV		= 0x08,
	DWC3_LINK_STATE_HRESET		= 0x09,
	DWC3_LINK_STATE_CMPLY		= 0x0a,
	DWC3_LINK_STATE_LPBK		= 0x0b,
	DWC3_LINK_STATE_RESUME		= 0x0f, /* in HS */

	DWC3_LINK_STATE_MASK		= 0x0f,
};

static inline const char *
dwc3_gadget_link_string(enum DWC3_link_state   link_state)
{
	switch (link_state)
	{
#if 0
	case DWC3_LINK_STATE_ON:
		return "On";
	case DWC3_LINK_STATE_SLEEP_L1:
		return "Sleep";
	case DWC3_LINK_STATE_SUSPEND_L2:
		return "Suspend";
	case DWC3_LINK_STATE_DISCONN:
		return "Dis-Conn";
	case DWC3_LINK_STATE_EARLY_SUSPEND:
		return "Early-Suspend";
	case DWC3_LINK_STATE_RESET:
		return "Reset";
	case DWC3_LINK_STATE_RESUME:
		return "Resume";
#endif
//#if U3_GADGET_TEST
#if 1
	case DWC3_LINK_STATE_U0:
		return "U0";
	case DWC3_LINK_STATE_U1:
		return "U1";
	case DWC3_LINK_STATE_U2:
		return "U2";
	case DWC3_LINK_STATE_U3:
		return "U3";
	case DWC3_LINK_STATE_SS_DIS:
		return "SS.Disabled";
	case DWC3_LINK_STATE_RX_DET:
		return "RX.Detect";
	case DWC3_LINK_STATE_SS_INACT:
		return "SS.Inactive";
	case DWC3_LINK_STATE_POLL:
		return "Polling";
	case DWC3_LINK_STATE_RECOV:
		return "Recovery";
	case DWC3_LINK_STATE_HRESET:
		return "Hot Reset";
	case DWC3_LINK_STATE_CMPLY:
		return "Compliance";
	case DWC3_LINK_STATE_LPBK:
		return "Loopback";
	case DWC3_LINK_STATE_RESUME:
		return "Resume";
#endif

	default:
		return "UNKNOWN link state\n";
	}
}


//============= Controller ===================================================================
//============= Initiation ===================================================================
static void dw3_core_reg_rd(void)
{
    U32 reg,i;
    PSPRINTF("dw3_core_reg_rd():\n" );


	#if 1
    	PSPRINTF("----------- Global Registers Map:-----------\n" );

	//	for( i=0; i<=0x50; i+=4 )
	//	for( i=0; i<=0x100; i+=4 )
    	for( i=0; i<=0x200; i+=4 )
		{
    		reg = readl(USB30_BASE_ADDR + DWC3_GSBUSCFG0 + i );
			if( reg )
				PSPRINTF( "offset(0x%x) =0x%x\n", i + DWC3_GSBUSCFG0,reg );
		}
	#endif
	#if 1
		PSPRINTF("----------- OTG Registers Map:-----------\n" );

		for( i=0; i<=0x10; i+=4 )
		{
			reg = readl(USB30_BASE_ADDR + DWC3_OCFG + i );
			if( reg )
				PSPRINTF( "offset(0x%x) =0x%x\n", i + DWC3_OCFG,reg );
		}
	#endif

	#if 1
		PSPRINTF("----------- Device Registers Map:-----------\n" );
		for( i=0; i<=0x14; i+=4 )
		{
			LDW( reg,(USB30_BASE_ADDR + 0xc700 + i) );//DWC3_DCFG
			if( reg )
				PSPRINTF( "offset(0x%x) =0x%x\n", i + 0xc700,reg );
		}
	#endif
	#if 1
		PSPRINTF("----------- Global Hardware Parameters Register 0~7 test:-----------\n" );
		for( i=0; i<32; i+=4 )
		{
			reg = readl(USB30_BASE_ADDR + DWC3_GHWPARAMS0 + i );
			if( reg )
				PSPRINTF( "offset(0x%x) =0x%x\n", i + DWC3_GHWPARAMS0,reg );
		}
	#endif
	#if 1
		//
		PSPRINTF( "GSNPSID=0x%x\n",     readl( USB30_BASE_ADDR+ DWC3_GSNPSID ) );
		//0xa
		PSPRINTF( "GSNPSID(0)=0x%x\n",  read8( USB30_BASE_ADDR+ DWC3_GSNPSID+0 ) );
		//
		PSPRINTF( "GSNPSID(1)=0x%x\n",  read8( USB30_BASE_ADDR+ DWC3_GSNPSID+1 ) );
		//0x33
		PSPRINTF( "GSNPSID(2)=0x%x\n",  read8( USB30_BASE_ADDR+ DWC3_GSNPSID+2 ) );
		//0x55
		PSPRINTF( "GSNPSID(3)=0x%x\n",  read8( USB30_BASE_ADDR+ DWC3_GSNPSID+3 ) );


		/* This should read as U3 followed by revision number */
		reg = readl(USB30_BASE_ADDR + DWC3_GSNPSID );
		if ( (reg & DWC3_GSNPSID_MASK) != 0x55330000)
		{
			PSPRINTF( "this is not a DesignWare USB3 (DRD) Core !!!\n");
			COND_TRAP(31, 0, 0);
		//	while(1){}
		}
		/*
		 * WORKAROUND: DWC3 revisions <1.90a have a bug
		 * where the device can fail to connect at SuperSpeed
		 * and falls back to high-speed mode which causes
		 * the device to enter a Connect/Disconnect loop
		 */
		if ( reg < DWC3_REVISION_190A)
		{
			PSPRINTF( "no,DWC3 revisions <1.90a !!!\n");
			COND_TRAP(31, 0, 0);
		//	while(1){}
		}
	#endif

	PSPRINTF( "\n" );
}


/**
 * DWC3_core_soft_reset - Issues core soft reset and PHY reset
 */
//static
void dwc3_core_soft_reset( void )
{
	U32 reg;

	/* Before Resetting PHY, [put] Core in Reset */
	reg = readl(USB30_BASE_ADDR + DWC3_GCTL );
	reg |= DWC3_GCTL_CORESOFTRESET;
	writel( USB30_BASE_ADDR + DWC3_GCTL,reg );

	/* Assert USB3 PHY reset */
	reg = readl(USB30_BASE_ADDR +  DWC3_GUSB3PIPECTL(0) );
	reg |= DWC3_GUSB3PIPECTL_PHYSOFTRST;
	writel( USB30_BASE_ADDR + DWC3_GUSB3PIPECTL(0),reg );

	/* Assert USB2 PHY reset */
	reg = readl(USB30_BASE_ADDR +  DWC3_GUSB2PHYCFG(0) );
	reg |= DWC3_GUSB2PHYCFG_PHYSOFTRST;
	writel( USB30_BASE_ADDR + DWC3_GUSB2PHYCFG(0),reg );

	delay(1000);
//	delay(0x100000);
//	delay(0x100000);
	/* (software needs to clear this bit):   Clear USB3 PHY reset */
	reg = readl(USB30_BASE_ADDR +  DWC3_GUSB3PIPECTL(0) );
	reg &= ~DWC3_GUSB3PIPECTL_PHYSOFTRST;
	writel( USB30_BASE_ADDR + DWC3_GUSB3PIPECTL(0),reg );

	/* Clear USB2 PHY reset */
	reg = readl(USB30_BASE_ADDR +  DWC3_GUSB2PHYCFG(0) );
	reg &= ~DWC3_GUSB2PHYCFG_PHYSOFTRST;
	writel( USB30_BASE_ADDR + DWC3_GUSB2PHYCFG(0),reg );

	/* keep the core in reset state until PHY clocks are stable. */
	reg = readl(USB30_BASE_ADDR + DWC3_GCTL );
	reg &= ~DWC3_GCTL_CORESOFTRESET;
	writel( USB30_BASE_ADDR + DWC3_GCTL,reg );
	delay(0x100000);

}


/**
 * 	config DWC3_GCTL register
 */
static void dwc3_core_setup_global_control( void )
{
	U32 reg;
	reg = readl(USB30_BASE_ADDR + DWC3_GCTL );  //0x30c13004


	reg &= ~DWC3_GCTL_SCALEDOWN_MASK;	//Actual timing values are used
	reg &= ~DWC3_GCTL_U2EXIT_LFPS;

//	reg |= DWC3_GCTL_DISSCRAMBLE;
	reg &= ~DWC3_GCTL_DISSCRAMBLE;

	#if 0
		reg |= DWC3_GCTL_U2RSTECN;	//?
	#endif

	switch ( DWC3_GHWPARAMS1_EN_PWROPT( readl(USB30_BASE_ADDR + DWC3_GHWPARAMS1))  ){
	case DWC3_GHWPARAMS1_EN_PWROPT_CLK:
		reg &= ~DWC3_GCTL_DSBLCLKGTNG;
		break;
	case DWC3_GHWPARAMS1_EN_PWROPT_HIB:
		reg |= DWC3_GCTL_GBLHIBERNATIONEN;
		break;
	default:
	//	PSPRINTF( "No power optimization available !!! \n");
		break;
	}

	//dwc3_core_init_mode
	reg &= ~ (DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG));
	switch (  DWC3_GHWPARAMS0_MODE( readl( USB30_BASE_ADDR+ DWC3_GHWPARAMS0 ))   )
	{
		case DWC3_GHWPARAMS0_MODE_GADGET:
			reg |= DWC3_GCTL_PRTCAPDIR( DWC3_GCTL_PRTCAP_DEVICE );
			break;
		case DWC3_GHWPARAMS0_MODE_HOST:
			reg |= DWC3_GCTL_PRTCAPDIR( DWC3_GCTL_PRTCAP_HOST );
			break;
		case DWC3_GHWPARAMS0_MODE_DRD:
			#if 1
				reg |= DWC3_GCTL_PRTCAPDIR( DWC3_GCTL_PRTCAP_OTG );
			#else
				/* for some static Host-only/Device-only applications */
				#if 0
					PSPRINTF( "Note: DWC3 acts as a DRD, but only use host mode !!!\n");
					reg |= DWC3_GCTL_PRTCAPDIR( DWC3_GCTL_PRTCAP_HOST );
				#endif
				#if 0
					PSPRINTF( "Note: DWC3 acts as a DRD, but only use device mode !!!\n");
					reg |= DWC3_GCTL_PRTCAPDIR( DWC3_GCTL_PRTCAP_DEVICE );
				#endif
			#endif
			break;
		default:
			PSPRINTF( "read GPRTBIMAP0 is Hub mode !!!\n");
			COND_TRAP(31, 0, 0);
		//	while(1){}
			break;
	}


	writel( USB30_BASE_ADDR + DWC3_GCTL,reg );// 0x30c13008
	//IP simu 0x1d493008
#if DEBUG_PRINT
	PSPRINTF( "cfg_GCTL = 0x%x\n",	readl(USB30_BASE_ADDR + DWC3_GCTL ) );
#endif
}


static void dwc3_core_num_eps( void )
{
	U32 reg;

	reg = readl(USB30_BASE_ADDR + DWC3_GHWPARAMS3 );

	PSPRINTF("	%s DWC_USB3_NUM_IN_EPS=(%d)/NUM_EPS=(%d)..\n",
						CHIP_STRING,DWC3_NUM_IN_EPS(reg), DWC3_NUM_EPS(reg) ); //8 16
}
//============================================================================================
//================= PHY ======================================================================
/**
 * dwc3_phy_setup - Configure USB PHY Interface of DWC3 Core
 */
static void dwc3_phy_setup( void )
{
	U32 reg;


/* Set the SS PHY interface */
	reg = readl( USB30_BASE_ADDR +DWC3_GUSB3PIPECTL(0) );//=0x10c0002

	if ( DWC3_GHWPARAMS3_SSPHY_IFC( readl(USB30_BASE_ADDR + DWC3_GHWPARAMS3) )
		==  DWC3_GHWPARAMS3_SSPHY_IFC_DIS )
	{
		PSPRINTF( "Waring,SSPHY_IFC_DIS,USB2.0-only core configuration,maximum_speed=2.0\n");
		while(1){}
	}else
	if( DWC3_GHWPARAMS3_SSPHY_IFC( readl(USB30_BASE_ADDR + DWC3_GHWPARAMS3) )
		==  DWC3_GHWPARAMS3_SSPHY_IFC_GEN2 )
	{
		PSPRINTF( "Waring,USB3.1 \n");
		while(1){}
	}

//.sv
	//reg = 0x01240002;
//	reg = 0x01240002;
	#if 0
		reg |= DWC3_GUSB3PIPECTL_SUSPHY;
	#endif
		
	writel( USB30_BASE_ADDR + DWC3_GUSB3PIPECTL(0),reg );

#if DEBUG_PRINT
	PSPRINTF("function{dwc3_phy_setup()}¡êoDWC3_GUSB3PIPECTL= 0x%x \r\n", readl(USB30_BASE_ADDR+DWC3_GUSB3PIPECTL(0))  );
#endif




/* Set the HS PHY interface */
#if 1
	reg = readl( USB30_BASE_ADDR +DWC3_GUSB2PHYCFG(0) );  //0x40102400

	if ( DWC3_GHWPARAMS3_SSPHY_IFC( readl(USB30_BASE_ADDR + DWC3_GHWPARAMS3) )
		==  DWC3_GHWPARAMS3_SSPHY_IFC_DIS )
	{
		PSPRINTF( "Waring,SSPHY_IFC_DIS,USB2.0-only core configuration,maximum_speed=2.0\n");
		while(1){}
	}
	switch ( DWC3_GHWPARAMS3_HSPHY_IFC( readl(USB30_BASE_ADDR + DWC3_GHWPARAMS3))  )
	{
	case DWC3_GHWPARAMS3_HSPHY_IFC_DIS:
		PSPRINTF( "Waring,HSPHY_IFC_DIS\n");
		while(1){}
		break;
	case DWC3_GHWPARAMS3_HSPHY_IFC_UTMI_ULPI:
		PSPRINTF( "HSPHY_IFC: UTMI+ & ULPI\n");
	//	reg &= ~ DWC3_GUSB2PHYCFG_ULPI_UTMI;
	//	reg |= DWC3_GUSB2PHYCFG_ULPI_UTMI;
		while(1){}
		break;
	case DWC3_GHWPARAMS3_HSPHY_IFC_ULPI:
		PSPRINTF( "HSPHY_IFC: ULPI\n");
		while(1){}
		break;
	case DWC3_GHWPARAMS3_HSPHY_IFC_UTMI:
		break;
	}
	#if 0 //USBPHY_INTERFACE_MODE_UTMI
		reg &= ~(DWC3_GUSB2PHYCFG_PHYIF_MASK |
			   DWC3_GUSB2PHYCFG_USBTRDTIM_MASK);
		reg |= DWC3_GUSB2PHYCFG_PHYIF(UTMI_PHYIF_8_BIT) |
			   DWC3_GUSB2PHYCFG_USBTRDTIM(USBTRDTIM_UTMI_8_BIT);
	#else //USBPHY_INTERFACE_MODE_UTMIW
		reg &= ~(DWC3_GUSB2PHYCFG_PHYIF_MASK |
		       DWC3_GUSB2PHYCFG_USBTRDTIM_MASK);
		reg |= DWC3_GUSB2PHYCFG_PHYIF(UTMI_PHYIF_16_BIT) |
		       DWC3_GUSB2PHYCFG_USBTRDTIM(USBTRDTIM_UTMI_16_BIT);
	#endif
	#if 0
		reg |= DWC3_GUSB2PHYCFG_SUSPHY;
	#endif

	#if 0
		reg &= ~ DWC3_GUSB2PHYCFG_U2_FREECLK_EXISTS;  
	#endif

	writel( USB30_BASE_ADDR + DWC3_GUSB2PHYCFG(0),reg );
	//IP simu 0x102408
#if DEBUG_PRINT
	PSPRINTF("function{dwc3_phy_setup()}¡êoGUSB2PHYCFG= 0x%x \r\n", readl(USB30_BASE_ADDR+DWC3_GUSB2PHYCFG(0))  );//0x40101408
#endif	
#endif
}


#if 0
	#define PHR_ADDR_OFFSET		( (22+2)*4 )
	#define PHR_WRDATA_OFFSET	( (22+2)*4 )
	#define PHR_RDDATA_OFFSET	( (185+2)*4 )

	#define PHR_CTRL_OFFSET		( (56+2)*4 )
	#define PHR_CTRL_RD			(5 <<18)
	#define PHR_CTRL_WR			(6 <<18)

	#define PHR_CFGREG_NUM		( 0x0B )
#endif

#if 0
//static
void USB2_NaNeng_OTG_PHY_Config(void)
{
    U32 reg;
    U8 addr;

    /*
     * test USB2_OTG_PHY(U2OPHYS14SF) Register Configuration Interface
     */
#if 0
    PSPRINTF( "************** Read U2OPHYS14SF regs test!!! ************** \n" );

//	for( addr=1; addr<= PHR_CFGREG_NUM; addr++ )
    addr=11;
    {
    	writel( USB2OPHY_BASE_ADDR + PHR_ADDR_OFFSET,(addr & 0x7F) <<15 );//bit[20:15]
		delay(0x1000);
		writel( USB2OPHY_BASE_ADDR + PHR_CTRL_OFFSET,PHR_CTRL_RD ); //bit20 19 18
		delay(0x1000);

		reg = readl(USB2OPHY_BASE_ADDR + PHR_RDDATA_OFFSET );
		PSPRINTF( "PHR_RDDATA_OFFSET (0x%x) = 0x%x\n",addr, (reg >> 9) & 0x000000FF );

		delay(0x100000);
    }
#endif

    /*
     * test PLL_EN
     */
#if 0
	reg = readl(USB2OPHY_BASE_ADDR + PHR_CTRL_OFFSET );
	PSPRINTF( "reg = 0x%x\n",reg );  //=0 -->~ -> PLL_EN=1

	writel( USB2OPHY_BASE_ADDR + PHR_CTRL_OFFSET,reg | (1<<17) ); //bit17
	delay(0x100000);
	reg = readl(USB2OPHY_BASE_ADDR + PHR_CTRL_OFFSET );
	PSPRINTF( "reg-2 = 0x%x\n",reg );  //0x20000

	writel( USB2OPHY_BASE_ADDR + PHR_CTRL_OFFSET,reg & 0xFFFDFFFF ); //bit17
	delay(0x100000);
	reg = readl(USB2OPHY_BASE_ADDR + PHR_CTRL_OFFSET );
	PSPRINTF( "reg-3 = 0x%x\n",reg ); //0x0
#endif


#if OTG_HOST_TEST
	/*
	 * config U2OPHYS14SF   [OTG_SUSPENDM_BYPS:OTG_SUSPENDM]
	 */
	reg = readl(USB2OPHY_BASE_ADDR + ( (2+3)*4 )   ); //=0x0
	PSPRINTF( "PHR_( (2+3)*4 ) (0x%x) = 0x%x\n",(USB2OPHY_BASE_ADDR + ( (2+3)*4 )) , reg  );

	writel( USB2OPHY_BASE_ADDR + ( (2+3)*4 ),0x00030000   );
	delay(0x100000);

	writel( USB2OPHY_BASE_ADDR + ( (2+3)*4 ),0x00030003   );
	delay(0x100000);

	reg = readl(USB2OPHY_BASE_ADDR + ( (2+3)*4 )   );
	PSPRINTF( "2.  PHR_( (2+3)*4 ) (0x%x) = 0x%x\n",(USB2OPHY_BASE_ADDR + ( (2+3)*4 )) , reg  );
#endif
#if OTG_GADGET_TEST
	//U2OPHYS14SF   [OTG_SUSPENDM_BYPS:OTG_SUSPENDM]

	reg = readl(USB2OPHY_BASE_ADDR + ( (2+3)*4 )  );
//	PSPRINTF( "PHR_( (2+3)*4 ) (0x%x) = 0x%x\n",(USB2OPHY_BASE_ADDR + ( (2+3)*4 )) , reg  );

	writel( USB2OPHY_BASE_ADDR + ( (2+3)*4 ),0x00030000  );
	delay(0x1000);
	reg = readl(USB2OPHY_BASE_ADDR + ( (2+3)*4 )   );
//	PSPRINTF( "2.  PHR_( (2+3)*4 ) (0x%x) = 0x%x\n",(USB2OPHY_BASE_ADDR + ( (2+3)*4 )) , reg  );
#endif
}

static void USB3_NaNeng_PIPE_Config(void)
{
    U32 reg;
    U8 addr;

    /*
     * config PIPE detect bypass
     */

	reg = readl(USB3PIPE_BASE_ADDR + ( (2+174)*4 )   );	//0x0
	PSPRINTF( "\nUSB3PIPE_( (2+174)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+174)*4 )) , reg  );
	writel( USB3PIPE_BASE_ADDR + ( (2+174)*4 ),0x21084210   );
	delay(0x1000);

	writel( USB3PIPE_BASE_ADDR + ( (2+175)*4 ),0x21084210   );
	delay(0x1000);

	writel( USB3PIPE_BASE_ADDR + ( (2+176)*4 ),0x21084210   );
	delay(0x1000);


	reg = readl(USB3PIPE_BASE_ADDR + ( (2+174)*4 )   );
	PSPRINTF( "2. USB3PIPE_( (2+174)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+174)*4 )) , reg  );
	reg = readl(USB3PIPE_BASE_ADDR + ( (2+175)*4 )   );
	PSPRINTF( "2. USB3PIPE_( (2+175)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+175)*4 )) , reg  );
	reg = readl(USB3PIPE_BASE_ADDR + ( (2+176)*4 )   );
	PSPRINTF( "2. USB3PIPE_( (2+176)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+176)*4 )) , reg  );
}

static void USB3_NaNeng_PIPE_ClkMode_Config(void)
{
    U32 reg;
    U8 addr;

#if 1
    /*
     * config pll_set[25] CKRCV Mode Select
     */
    //difference receive mode
 reg = readl(USB3PIPE_BASE_ADDR + ( (2+38)*4 )   ); //0
 PSPRINTF( "\nUSB3PIPE_( (2+38)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+38)*4 )) , reg  );
 
 //signal receive mode
 //writel( USB3PIPE_BASE_ADDR + ( (2+38)*4 ),1 <<25   ); //signal mode 25bit 1,difference mode 25bit is 0,default is difference
 writel( USB3PIPE_BASE_ADDR + ( (2+38)*4 ),1 <<23   ); //signal mode 25bit 1,difference mode 25bit is 0,default is difference
 delay(0x1000);

 reg = readl(USB3PIPE_BASE_ADDR + ( (2+38)*4 )   ); //
 PSPRINTF( "\nUSB3PIPE_( (2+38)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+38)*4 )) , reg  );
#endif

#if 0
    /* if 25MHz:
     * config pll_ckref_div[3:0]
     */
 reg = readl(USB3PIPE_BASE_ADDR + ( (2+64)*4 )   ); //0x33333333
 PSPRINTF( "\nUSB3PIPE_( (2+64)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+64)*4 )) , reg  );

 //writel( USB3PIPE_BASE_ADDR + ( (2+64)*4 ),0x00000000  );
 writel( USB3PIPE_BASE_ADDR + ( (2+64)*4 ),0x3  );
 delay(0x1000);

 reg = readl(USB3PIPE_BASE_ADDR + ( (2+64)*4 )   ); //
 PSPRINTF( "\nUSB3PIPE_( (2+64)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+64)*4 )) , reg  );
#endif

#if 1
    /*
     * pll_div[14:0]
     */
 reg = readl(USB3PIPE_BASE_ADDR + ( (2+22)*4 )   ); // 0x32006400
 PSPRINTF( "\nUSB3PIPE_( (2+22)*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( (2+22)*4 )) , reg  );
#endif
}


//test
#if 0
void USB3_NaNeng_PIPE_BIST_test( void )
{
    U32 reg;

//usb30_phy_bist.c
	writel( USB3PIPE_BASE_ADDR + 4*6  ,0x0000FFFF);   //USB Sel the config rx_term_en;
	writel( USB3PIPE_BASE_ADDR + 4*8  ,0xFFFF0000);   //PCS Lane0 bist enable
	writel( USB3PIPE_BASE_ADDR + 4*23 ,0xFFFF0000);  //PHY rx0 bist enable
	writel( USB3PIPE_BASE_ADDR + 4*80 ,0xFFFF0000);  //The configed rx_term_en is high
	writel( USB3PIPE_BASE_ADDR + 4*81 ,0x0000FFFF);  //PHY lpbk enable
	writel( USB3PIPE_BASE_ADDR + 4*82 ,0x0000FFFF);  //PHY tx0 bist enable
	writel( USB3PIPE_BASE_ADDR + 4*134,0x00013013);  //PHY tx0/tx1 byp eidle and byp tx/rx_pd
	writel( USB3PIPE_BASE_ADDR + 4*135,0x00013013);  //PHY tx2/tx3 byp eidle and byp tx/rx_pd
	writel( USB3PIPE_BASE_ADDR + 4*136,0x00013013);  //PHY tx4/tx5 byp eidle and byp tx/rx_pd
	writel( USB3PIPE_BASE_ADDR + 4*137,0x00013013);  //PHY tx6/tx7 byp eidle and byp tx/rx_pd

	delay(0x1000);
	reg = readl( USB3PIPE_BASE_ADDR + 4*194);     //PHY Bist out
	while( (reg & 0xFFFF0000) != 0x00030000 )
	{
		reg = readl( USB3PIPE_BASE_ADDR + 4*194);     //PHY Bist out
	}

//	passed();
	PSPRINTF( "USB3_PIPE_BIST_test reg= 0x%x\n",reg  );
	while(1){}
}
#endif
#endif
//============================================================================================
//============= Interrupt ====================================================================
static void dwc3_otg_event_isr(void)
{
//	PSPRINTF( "###############dwc3_otg_event_isr#################\n");
	volatile U32 reg;
	reg = readl( USB30_BASE_ADDR +DWC3_OEVT );


	PSPRINTF( "\n	->otg_isr,OEVT = 0x%x(%s,%s,%s,%s,%s,)\n",
			reg,
			(reg & DWC3_OTG_OEVT_DeviceMode) 			? "B-Dev mode":"A-Dev mode",
			(reg & DWC3_OTG_OEVT_OTGXhciRunStpSetEvnt)  ? "Xhci.Run":"",
			(reg & DWC3_OTG_OEVT_OTGConIDStsChngEvnt)   ? "IDStsChng":"",
			(reg & DWC3_OTG_OEVT_OTGADevHostEvnt) 		? "ADevHost":"",
			(reg & DWC3_OTG_OEVT_OTGADevSessEndDetEvnt) ? "ADevSessEnd":""
	);

	writel(  USB30_BASE_ADDR +DWC3_OEVT ,reg );

#if 1
	if( reg & DWC3_OTG_OEVT_OTGConIDStsChngEvnt )
	{
		switch( reg & DWC3_OTG_OEVT_DeviceMode ){
		case DWC3_OTG_OEVT_B_DeviceMode:
			PSPRINTF( "\n[ID L->H(B-Dev mode),maybe just unplugged the OTG cable ?]\n");
			break;
		case DWC3_OTG_OEVT_A_DeviceMode:
			PSPRINTF( "\n[ID H->L(A-Dev mode),maybe just plugged the OTG cable ?]\n");
			break;
		}
	}
#endif
}

static void dwc3_host_smi_isr(void)
{
//	PSPRINTF( "###############dwc3_host_smi_isr#################\n");
	PSPRINTF( "	->dwc3_host_smi_isr ? \n"	);
}

//============================================================================================
//============= OTG Controller================================================================

/**		DWC_usb3_block_otg
 */
static void dwc3_otg_reg_rd( void )
{
	U32 reg;
	U8 i;

#if 1
	PSPRINTF("----------- OTG Registers Map:-----------\n" );
	for( i=0; i<=0x10; i+=4 )
	{
		reg = readl(USB30_BASE_ADDR + DWC3_OCFG + i );
		if( reg )
			PSPRINTF( "offset(0x%x) =0x%x\n", i + DWC3_OCFG,reg );
	}
#endif
#if 1
	reg = readl(USB30_BASE_ADDR + DWC3_OSTS );
	switch ( DWC3_OTG_OSTS_IDSts( reg)  ){
		case DWC3_OTG_OSTS_IDSts_A:
			PSPRINTF( "IDDIG signal = L /");
			break;
		case DWC3_OTG_OSTS_IDSts_B:
			PSPRINTF( "IDDIG signal = H /");
			break;
	}

	reg = readl(USB30_BASE_ADDR + DWC3_OEVT );
	switch (  reg & DWC3_OTG_OEVT_DeviceMode  ){
		case DWC3_OTG_OEVT_A_DeviceMode:
			PSPRINTF( "DWC3( DRD Controller) = A-Device mode+");
			break;
		case DWC3_OTG_OEVT_B_DeviceMode:
			PSPRINTF( "DWC3( DRD Controller) = B-Device mode+");
			break;
	}

	reg = readl(USB30_BASE_ADDR + DWC3_OSTS );
	switch ( DWC3_OTG_OSTS_PeripheralState( reg)  ){
		case DWC3_OTG_Host_Mode:
			PSPRINTF( "acts as a host.\n");
			break;
		case DWC3_OTG_Peripheral_Mode:
			PSPRINTF( "acts as a peripheral.\n");
			break;
	}
	PSPRINTF( "\n");
#endif
}


void dwc3_drd_host_init( void )
{
	U32 reg;

/*******************************
 *  OTG & host Events Enable
 *******************************/
	writel( USB30_BASE_ADDR + DWC3_OEVTEN,
		 DWC3_OTG_OEVTEN_OTGXhciRunStpSetEvntEn	//OTG Host Run Stop Set Event
		|DWC3_OTG_OEVTEN_OTGConIDStsChngEvntEn	//change in connector ID status
		|DWC3_OTG_OEVTEN_OTGADevHostEvntEn	  	//only for OTG 2.0 mode;   when A-device enters host role
		|DWC3_OTG_OEVTEN_OTGADevSessEndDetEvntEn   //end of a session
		 ); 	//0x9090000
#if 0
	reg = readl( USB30_BASE_ADDR +DWC3_DSTS );
	PSPRINTF("  ltssm_state()/[%s] \n",dwc3_gadget_link_string( DWC3_DSTS_USBLNKST( reg ) ) );
#endif

	InstallInterruptHandler0(USB30_INT_NUM, 		xhci_irq);
	InstallInterruptHandler0(USB30_OTG_INT_NUM, 	dwc3_otg_event_isr);
	InstallInterruptHandler0(USB30_HST_SMI_INT_NUM, dwc3_host_smi_isr);
	
//	*(volatile unsigned int *)(0xe8050200+USB30_INT_NUM*0x20) &= 0x7fffffff;//enable mpic vector
//	*(volatile unsigned int *)(0xe8050200+USB30_OTG_INT_NUM*0x20) &= 0x7fffffff;//enable mpic vector
//	*(volatile unsigned int *)(0xe8050200+USB30_HST_SMI_INT_NUM*0x20) &= 0x7fffffff;//enable mpic vector

	if( readl(USB30_BASE_ADDR + DWC3_GSNPSID) <=  DWC3_REVISION_310A )
	{
		//Device Address is equal to Slot ID.used in host mode operation only
		reg = readl(USB30_BASE_ADDR + DWC3_GUCTL );  //=0x2000010
		if(  reg & 0x00008000 ) {
			reg &= ~(1 <<15 );
			writel( USB30_BASE_ADDR + DWC3_GUCTL,reg );

			COND_TRAP(31, 0, 0);
		//	while(1){}
		}
	}


/*******************************
 *  OTG: host config
 *******************************/
//Id status
	reg = readl(USB30_BASE_ADDR + DWC3_OSTS ); //read otg id status
	if( !!DWC3_OTG_OSTS_IDSts(reg) == DWC3_OTG_OSTS_IDSts_B ) {
		#if DEBUG_PRINT
			reg = readl(  USB3PIPE_BASE_ADDR + (193*4) );	//
			PSPRINTF( "USB3PIPE_( 193*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( 193*4 )) , reg  );
			PSPRINTF( "PHY3:%d\n", (reg&(1<<19))>>19  );
			PSPRINTF( "PHY2:%d\n", (reg&(1<<18))>>18  );
			PSPRINTF( "PHY1:%d\n", (reg&(1<<17))>>17  );
			PSPRINTF( "PHY0:%d\n", (reg&(1<<16))>>16  );
			PSPRINTF( "\n		err,DWC3 should act as A-device mode,but IDDIG signal = H.\n");
		#endif
		
		COND_TRAP(31, 0, 0);
		while(1){}
	}

//A-Device mode
	reg = readl(USB30_BASE_ADDR + DWC3_OEVT );
	switch (  reg & DWC3_OTG_OEVT_DeviceMode  )
	{
		case DWC3_OTG_OEVT_A_DeviceMode:
			
			#if DEBUG_PRINT
				PSPRINTF( "[DWC3 = A-Device mode.]\n");
			#endif
			/* Set Port Power to enable VBUS when it is an A-device */
			reg = readl(USB30_BASE_ADDR + DWC3_OCTL );
			reg |= DWC3_OTG_OCTL_PrtPwrCtl;
			writel( USB30_BASE_ADDR + DWC3_OCTL,reg ); //set 5v to device
			delay(0x100);

			//wait A-session valid
			reg = readl(USB30_BASE_ADDR + DWC3_OSTS ); //wait host session valid
			while( (reg & DWC3_OTG_OSTS_ASesVld) == 0 )
			{
				reg = readl(USB30_BASE_ADDR + DWC3_OSTS );
			//	COND_TRAP(31, 0, 0);
				delay(2000);
			}
			break;

		case DWC3_OTG_OEVT_B_DeviceMode:
			PSPRINTF( "Err:ID Status & Device Mode(now B-Device mode) not match ??\n");
			COND_TRAP(31, 0, 0);
			while(1){}
			break;
	}
	//set OTG Role
	reg = readl(USB30_BASE_ADDR + DWC3_OCTL );
	reg &= ~DWC3_OTG_OCTL_MODE;
	reg |= DWC3_OTG_OCTL_HOST_MODE << 6;
	writel( USB30_BASE_ADDR + DWC3_OCTL,reg );   //set the core to act as host
	delay(0x100);
	
	reg = readl(USB30_BASE_ADDR + DWC3_OSTS ); //Indicates whether the core is acting as a peripheral or host
	switch ( DWC3_OTG_OSTS_PeripheralState(reg)  ){
		case DWC3_OTG_Host_Mode:
		#if DEBUG_PRINT
			reg = readl(  USB3PIPE_BASE_ADDR + (193*4) );	//
			PSPRINTF( "USB3PIPE_( 193*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( 193*4 )) , reg  );
			PSPRINTF( "PHY3:%d\n", (reg&(1<<19))>>19  );
			PSPRINTF( "PHY2:%d\n", (reg&(1<<18))>>18  );
			PSPRINTF( "PHY1:%d\n", (reg&(1<<17))>>17  );
			PSPRINTF( "PHY0:%d\n", (reg&(1<<16))>>16  );
			
			PSPRINTF( "[DWC3( DRD Controller) acts as a host mode.]\n");
		#endif
			break;
		case DWC3_OTG_Peripheral_Mode:
			PSPRINTF( "[*** DWC3( DRD Controller) acts as a peripheral mode ? **]\n");
			PSPRINTF( "\n		err,just test host mode,but PeriMode=1 ?\n");
			PSPRINTF( "\n		If want to test peripheral mdoe,need RSP/HNP Capability !!\n");
			COND_TRAP(31, 0, 0);
		//	while(1){}
			break;
	}
}



//============================================================================================
//===============  Main ======================================================================
/**
 * dwc3_core_init - Low-level initialization of DWC3 Core
 * @dwc: Pointer to our controller context structure
 */
#if 0
void bist()
{
	unsigned int retval;
	STW(0x00000003,USB30_MISCE_BASE_ADDR+0x0400+4*6);//PHY term_en select
	STW(0xffff0000,USB30_MISCE_BASE_ADDR+0x0400+4*23);//PHT rx0 bist enable
	STW(0x0000ffff,USB30_MISCE_BASE_ADDR+0x0400+4*81);//PHY lpbk enable
	STW(0x0000ffff,USB30_MISCE_BASE_ADDR+0x0400+4*82);//PHY tx0 bist enable
	STW(0x00002002,USB30_MISCE_BASE_ADDR+0x0400+4*134);// PHY tx0/tx1 byp eidle and byp tx/rx_pd
	LDW(retval,USB30_MISCE_BASE_ADDR+0x0400+4*194);//PHY Bist out
	while((retval&0x00010000)==0x00000000)
	{
		LDW(retval,USB30_MISCE_BASE_ADDR+0x0400+4*194);//PHY Bist out
		PSPRINTF( "Bist testing\n");
	}
}
#endif
void usb_main(void)
{

	U32 reg,i;
#if 0 // close serdes
	W32(0xe954c45c,0xffffffff);
	W32(0xe954d45c,0xffffffff);
	reg = R32(0xe954c45c);
	PSPRINTF("0xe954c45c:0x%x\n",reg );
	reg = R32(0xe954d45c);
	PSPRINTF("0xe954d45c:0x%x\n",reg );
	reg = R32(0xeb009000+0x01c);
	PSPRINTF("0xeb009000+0x01c:0x%x\n",reg );
#endif 
	
#if 1
	//clear used buf
	for( i =0; i <0x8000; i+=4)	{
		writel( XHCI_BUFFERs_BASE + i,0x00000000 );
	}
#endif
#if 1
	//dw3_core_reg_rd();
	reg = readl( USB3PIPE_BASE_ADDR + (40*4));
	writel( USB3PIPE_BASE_ADDR + (40*4),reg | (1<<18)|0x60);

	reg = readl( USB3PIPE_BASE_ADDR + (41*4));
	writel( USB3PIPE_BASE_ADDR + (41*4),reg | (1<<18)|0x60);
	
	reg = readl( USB3PIPE_BASE_ADDR + (42*4));
	writel( USB3PIPE_BASE_ADDR + (42*4),reg | (1<<18)|0x60);
	
	reg = readl( USB3PIPE_BASE_ADDR + (43*4));
	writel( USB3PIPE_BASE_ADDR + (43*4),reg | (1<<18)|0x60);
	
	reg = readl( USB3PIPE_BASE_ADDR + (44*4));
	writel( USB3PIPE_BASE_ADDR + (44*4),reg | (1<<18)|0x60);
	
	reg = readl( USB3PIPE_BASE_ADDR + (45*4));
	writel( USB3PIPE_BASE_ADDR + (45*4),reg | (1<<18)|0x60);
	
	reg = readl( USB3PIPE_BASE_ADDR + (46*4));
	writel( USB3PIPE_BASE_ADDR + (46*4),reg | (1<<18)|0x60);
	
	reg = readl( USB3PIPE_BASE_ADDR + (47*4));
	writel( USB3PIPE_BASE_ADDR + (47*4),reg | (1<<18)|0x60);
#endif
#if 1
	dwc3_core_soft_reset();
	dwc3_phy_setup();
#endif

#if DEBUG_PRINT
	reg = readl(  USB3PIPE_BASE_ADDR + (193*4) );	//
	PSPRINTF( "USB3PIPE_( 193*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( 193*4 )) , reg  );
	PSPRINTF( "pll_PHY3:%d\n", (reg&(1<<19))>>19  );
	PSPRINTF( "pll_PHY2:%d\n", (reg&(1<<18))>>18  );
	PSPRINTF( "pll_PHY1:%d\n", (reg&(1<<17))>>17  );
	PSPRINTF( "pll_PHY0:%d\n", (reg&(1<<16))>>16  );
#endif	

//	dw3_core_reg_rd();
	dwc3_core_setup_global_control();

#if DEBUG_PRINT
	dwc3_core_num_eps();
#endif
	
	/* dwc3_core_init_mode / dwc3_get_dr_mode */
	switch( DWC3_GHWPARAMS0_MODE( readl( USB30_BASE_ADDR+ DWC3_GHWPARAMS0 )) )
	{
	case DWC3_GHWPARAMS0_MODE_GADGET:
		PSPRINTF("\n[%s DWC3 is configured as Device Only mode by IP Ower !]\n",CHIP_STRING );
	//	dwc3_gadget_init();
		break;
	case DWC3_GHWPARAMS0_MODE_HOST:
		PSPRINTF("\n[%s DWC3 is configured as Host Only mode by IP Ower !]\n",CHIP_STRING );
	//	dwc3_host_init();
		break;
	case DWC3_GHWPARAMS0_MODE_DRD:
#if DEBUG_PRINT
		PSPRINTF("\n[%s DWC3 is configured as Device and Host (DRD) mode by IP Ower !]\n",CHIP_STRING );
#endif
		
#if DEBUG_PRINT
		dwc3_otg_reg_rd();
#endif

#if 0
		reg = readl(  USB3PIPE_BASE_ADDR + (40*4) );	//
		PSPRINTF( "USB3PIPE_( 40*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( 40*4 )) , reg  );
		reg |=(1<<18);
		reg |=(1<<5);
		writel( USB3PIPE_BASE_ADDR + (40*4),reg );
		reg = readl(  USB3PIPE_BASE_ADDR + (40*4) );	//
		PSPRINTF( "USB3PIPE_( 40*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( 40*4 )) , reg  );
	
#endif
		
		dwc3_drd_host_init();
		xhci_main();

		break;
//	default:
//		PSPRINTF("\n%s DWC3 is configured as Hub mode by IP Ower !\n",CHIP_STRING );
//		break;
	}

	//////////////////////////////////////////////////////////////////////
}

//============================================================================================
//===============  End =======================================================================
