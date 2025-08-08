/*
 * usb_reg_dwc3.h
 *
 *  Created on: 2023-7-24
 *      Author: DELL     #ifndef USB_REG_DWC3_H_
 */
#ifndef USB_REG_DWC3_H_
#define USB_REG_DWC3_H_

//===========================================================================================
/* Regs addr/offset */
//===========================================================================================


#if CHIP_S1020

	/* base addr for S1020 */
	#define USB3_REG_BADDR_HIGH    	0x4
	#if USB_CTR0
		#define USB30_BASE_ADDR         0xE9300000
		#define USB2OPHY_BASE_ADDR		0xE9500400  //USB 2.0 OTG PHY for SMIC14SF+/SF++
		#define USB3PIPE_BASE_ADDR		0xE9500400	//PIPE[2]	GSP3S3S14LN
		#define USB30_INT_NUM           39
		#define USB30_OTG_INT_NUM       38
		#define USB30_HST_SMI_INT_NUM   37
	#elif USB_CTR1
		#define USB30_BASE_ADDR         0xE9400000
		#define USB2OPHY_BASE_ADDR		0xE9502400  //USB 2.0 OTG PHY for SMIC14SF+/SF++
		#define USB3PIPE_BASE_ADDR		0xE9502400	//PIPE[2]	GSP3S3S14LN
		#define USB30_INT_NUM           42
		#define USB30_OTG_INT_NUM       41
		#define USB30_HST_SMI_INT_NUM   40
	#endif
#endif


/*offset:  DWC3 registers memory space boundries */
#define DWC3_XHCI_REGS_START		0x0
#define DWC3_XHCI_REGS_END			0x7fff

#define DWC3_GLOBALS_REGS_START		0xc100  //dw3 regs
#define DWC3_GLOBALS_REGS_END		0xc6ff

#define DWC3_DEVICE_REGS_START		0xc700
#define DWC3_DEVICE_REGS_END		0xcbff

#define DWC3_OTG_REGS_START			0xcc00
#define DWC3_OTG_REGS_END			0xccff


/* base addr for DWC3 registers memory  */
#define USB3_GLOBAL_REG_BADDR  (USB30_BASE_ADDR + DWC3_GLOBALS_REGS_START)

#define USB3_HOST_REG_BADDR    (USB30_BASE_ADDR + DWC3_XHCI_REGS_START)
#define USB3_DEV_REG_BADDR     (USB30_BASE_ADDR + DWC3_DEVICE_REGS_START)
#define USB3_OTG_REG_BADDR     (USB30_BASE_ADDR + DWC3_OTG_REGS_START)




//===========================================================================================
/* testcases macro */
//===========================================================================================

//*************************************************
/* testcases macro */


/*
enum usb_dr_mode {
	USB_DR_MODE_UNKNOWN,
	USB_DR_MODE_HOST,
	USB_DR_MODE_PERIPHERAL,
	USB_DR_MODE_OTG,
};
enum usb_dr_mode	g_DWC3_dr_mode;
*/


//===========================================================================================
/* Registers */
//===========================================================================================

//*************************************************
/* DWC_USB3.0 Controller, Global Registers  Map */
#define DWC3_GSBUSCFG0		0xc100
#define DWC3_GSBUSCFG1		0xc104
#define DWC3_GTXTHRCFG		0xc108
#define DWC3_GRXTHRCFG		0xc10c
#define DWC3_GCTL			0xc110 //Global Core Control Regis
#define DWC3_GPMSTS         0xc114
#define DWC3_GSTS			0xc118 //Global Status Register
#define DWC3_GUCTL1         0xc11c
#define DWC3_GSNPSID		0xc120 //Global Synopsys ID Register
#define DWC3_GGPIO			0xc124
#define DWC3_GUID			0xc128
#define DWC3_GUCTL			0xc12c
#define DWC3_GBUSERRADDR0	0xc130
#define DWC3_GBUSERRADDR1	0xc134
#define DWC3_GPRTBIMAP0		0xc138
#define DWC3_GPRTBIMAP1		0xc13c
#define DWC3_GHWPARAMS0		0xc140
#define DWC3_GHWPARAMS1		0xc144
#define DWC3_GHWPARAMS2		0xc148
#define DWC3_GHWPARAMS3		0xc14c
#define DWC3_GHWPARAMS4		0xc150
#define DWC3_GHWPARAMS5		0xc154
#define DWC3_GHWPARAMS6		0xc158
#define DWC3_GHWPARAMS7		0xc15c
#define DWC3_GDBGFIFOSPACE	0xc160
#define DWC3_GDBGLTSSM		0xc164
#define DWC3_GDBGLNMCC      0xc168
#define DWC3_GDBGBMU        0xc16c

#define DWC3_GDBGLSPMUX_DEV  	0xc170
#define DWC3_GDBGLSPMUX_HST 	0xc170

#define DWC3_GDBGLSP		0xc174
#define DWC3_GDBGEPINFO0	0xc178
#define DWC3_GDBGEPINFO1   	0xc17c
#if 0
#define DWC3_GPRTBIMAP_HS0	0xc180
#define DWC3_GPRTBIMAP_HS1	0xc184
#define DWC3_GPRTBIMAP_FS0	0xc188
#define DWC3_GPRTBIMAP_FS1	0xc18c
#else
#define DWC3_GPRTBIMAP_HSLO  0xc180
#define DWC3_GPRTBIMAP_HSHI  0xc184
#define DWC3_GPRTBIMAP_FSLO  0xc188
#define DWC3_GPRTBIMAP_FSHI  0xc18c
#endif
#define DWC3_GERRINJCTL_1    0xc194
#define DWC3_GERRINJCTL_2    0xc198
#define DWC3_GUCTL2			 0xc19c

#define DWC3_GUSB3RMMICTL(n) (0xc1c0 + (n * 0x04))
#define DWC3_GUSB2PHYCFG(n)	 (0xc200 + (n * 0x04)) //Global USB2 PHY Configuration Register
#define DWC3_GUSB2I2CCTL(n)	 (0xc240 + (n * 0x04))
#define DWC3_GUSB2PHYACC(n)	 (0xc280 + (n * 0x04)) //UTMI/ULPI
#define DWC3_GUSB3PIPECTL(n) (0xc2c0 + (n * 0x04))
#define DWC3_GTXFIFOSIZ(n)	 (0xc300 + (n * 0x04))
#define DWC3_GRXFIFOSIZ(n)	 (0xc380 + (n * 0x04))

//Global Event Buffer
#define DWC3_GEVNTADRLO(n)	 (0xc400 + (n * 0x10)) //Global Event Buffer Address (Low) Register
#define DWC3_GEVNTADRHI(n)	 (0xc404 + (n * 0x10))
#define DWC3_GEVNTSIZ(n)	 (0xc408 + (n * 0x10))
#define DWC3_GEVNTCOUNT(n)	 (0xc40c + (n * 0x10))

#define DWC3_GHWPARAMS8      0xc600
#define DWC3_GTXFIFOPRIDEV   0xc610
#define DWC3_GTXFIFOPRIHST   0xc618
#define DWC3_GRXFIFOPRIHST   0xc61c
#define DWC3_GFIFOPRIDBC     0xc620
#define DWC3_GDMAHLRATIO     0xc624
#define DWC3_GFLADJ          0xc630

#define DWC3_DSTS_USBLNKST_MASK		(0x0f << 18)
#define DWC3_DSTS_USBLNKST(n)		( ((n) & DWC3_DSTS_USBLNKST_MASK) >> 18)//RO
#define DWC3_DSTS       0xc70C  //Device Status Register

//*************************************************
/* DWC_USB3.0 Controller, OTG Registers Map */

#define DWC3_OCFG		0xcc00
#define DWC3_OCTL		0xcc04
#define DWC3_OEVT		0xcc08
#define DWC3_OEVTEN		0xcc0c
#define DWC3_OSTS		0xcc10





//===========================================================================================
/* Bit fields */
//===========================================================================================
/********************** OTG *******************/

/* OTG Configuration Register */
#define DWC3_OTG_OCFG_DisPrtPwrCutoff	(1 << 5)
#define DWC3_OTG_OCFG_OTGHibDisMask		(1 << 4)
#define DWC3_OTG_OCFG_OTGSftRstMsk		(1 << 3)
#define DWC3_OTG_OCFG_OTGVersion		(1 << 2)

#define DWC3_OTG_OCFG_HNPCap			(1 << 1)
#define DWC3_OTG_OCFG_SRPCap			(1 << 0)

/* OTG Control Register */
#define DWC3_OTG_OCTL_GOERR			(1 << 7)
#define DWC3_OTG_OCTL_MODE			(1 << 6)
	#define DWC3_OTG_OCTL_HOST_MODE  	0
	#define DWC3_OTG_OCTL_DEVICE_MODE  	1
#define DWC3_OTG_OCTL_PrtPwrCtl		(1 << 5)

#define DWC3_OTG_OCTL_HNPReq		(1 << 4)
#define DWC3_OTG_OCTL_SesReq		(1 << 3)
#define DWC3_OTG_OCTL_TermSelDLPulse	(1 << 2)
#define DWC3_OTG_OCTL_DevSetHNPEn		(1 << 1)
#define DWC3_OTG_OCTL_HstSetHNPEn		(1 << 0)

/* OTG Events Register */
#define DWC3_OTG_OEVT_DeviceMode				(1 << 31)
	#define DWC3_OTG_OEVT_B_DeviceMode		(1 << 31)
	#define DWC3_OTG_OEVT_A_DeviceMode		(0 << 31)

#define DWC3_OTG_OEVT_OTGXhciRunStpSetEvnt		(1 << 27)
#define DWC3_OTG_OEVT_OTGDevRunStpSetEvnt		(1 << 26)

#define DWC3_OTG_OEVT_OTGHibEntryEvnt			(1 << 25)
#define DWC3_OTG_OEVT_OTGConIDStsChngEvnt		(1 << 24)

#define DWC3_OTG_OEVT_HRRConfNotifEvnt			(1 << 23)
#define DWC3_OTG_OEVT_HRRInitNotifEvnt			(1 << 22)

#define DWC3_OTG_OEVT_OTGADevIdleEvnt			(1 << 21)
#define DWC3_OTG_OEVT_OTGADevBHostEndEvnt		(1 << 20)
#define DWC3_OTG_OEVT_OTGADevHostEvnt			(1 << 19)
#define DWC3_OTG_OEVT_OTGADevHNPChngEvnt		(1 << 18)
#define DWC3_OTG_OEVT_OTGADevSRPDetEvnt			(1 << 17)
#define DWC3_OTG_OEVT_OTGADevSessEndDetEvnt		(1 << 16)

#define DWC3_OTG_OEVT_OTGBDevBHostEndEvnt		(1 << 11)
#define DWC3_OTG_OEVT_OTGBDevHNPChngEvnt		(1 << 10)
#define DWC3_OTG_OEVT_OTGBDevSessVldDetEvnt		(1 << 9)
#define DWC3_OTG_OEVT_OTGBDevVBUSChngEvnt		(1 << 8)
#define DWC3_OTG_OEVT_BSesVld					(1 << 3)
#define DWC3_OTG_OEVT_HstNegSts					(1 << 2)
#define DWC3_OTG_OEVT_SesReqSts					(1 << 1)
#define DWC3_OTG_OEVT_OEVTError					(1 << 0)


/* OTG Events Enable Register */
#define DWC3_OTG_OEVTEN_OTGXhciRunStpSetEvntEn	(1 << 27)
#define DWC3_OTG_OEVTEN_OTGDevRunStpSetEvntEn	(1 << 26)

#define DWC3_OTG_OEVTEN_OTGHibEntryEvntEn		(1 << 25)
#define DWC3_OTG_OEVTEN_OTGConIDStsChngEvntEn	(1 << 24)

#define DWC3_OTG_OEVTEN_HRRConfNotifEvntEn		(1 << 23)
#define DWC3_OTG_OEVTEN_HRRInitNotifEvntEn		(1 << 22)

#define DWC3_OTG_OEVTEN_OTGADevIdleEvntEn			(1 << 21)
#define DWC3_OTG_OEVTEN_OTGADevBHostEndEvntEn		(1 << 20)
#define DWC3_OTG_OEVTEN_OTGADevHostEvntEn			(1 << 19)
#define DWC3_OTG_OEVTEN_OTGADevHNPChngEvntEn		(1 << 18)
#define DWC3_OTG_OEVTEN_OTGADevSRPDetEvntEn			(1 << 17)
#define DWC3_OTG_OEVTEN_OTGADevSessEndDetEvntEn		(1 << 16)

#define DWC3_OTG_OEVTEN_OTGBDevBHostEndEvntEn		(1 << 11)
#define DWC3_OTG_OEVTEN_OTGBDevHNPChngEvntEn		(1 << 10)
#define DWC3_OTG_OEVTEN_OTGBDevSessVldDetEvntEn		(1 << 9)
#define DWC3_OTG_OEVTEN_OTGBDevVBUSChngEvntEn		(1 << 8)


/* OTG Status Register */
#define DWC3_OTG_OSTS_DevRunStp			(1 << 13)
#define DWC3_OTG_OSTS_xHciRunStp		(1 << 12)

#define DWC3_OTG_OSTS_OTGstate			(0xf << 8)

#define DWC3_OTG_OSTS_PeripheralState(n)	(  ((n) & (1 << 4))  >>4) //RO
	#define DWC3_OTG_Host_Mode				0
	#define DWC3_OTG_Peripheral_Mode		1

#define DWC3_OTG_OSTS_xHCIPrtPower		(1 << 3)
#define DWC3_OTG_OSTS_BSesVld			(1 << 2)
#define DWC3_OTG_OSTS_ASesVld			(1 << 1)

#define DWC3_OTG_OSTS_IDSts(n)			( ((n) & 0x00000001) ) //RO
	#define DWC3_OTG_OSTS_IDSts_B		1
	#define DWC3_OTG_OSTS_IDSts_A		0


/********************** Global *******************/
/* Global RX Threshold Configuration Register */
#define DWC3_GRXTHRCFG_MAXRXBURSTSIZE(n) 	(((n) & 0x1f) << 19)
#define DWC3_GRXTHRCFG_RXPKTCNT(n)		 	(((n) & 0xf) << 24)
#define DWC3_GRXTHRCFG_PKTCNTSEL 			(1 << 29)

/* Global Configuration Register */
#define DWC3_GCTL_PWRDNSCALE(n)		((n) << 19)
#define DWC3_GCTL_U2RSTECN			(1 << 16)	//only in device mode

//Port Capability Direction
#define DWC3_GCTL_PRTCAP(n)			(((n) & (3 << 12)) >> 12)
#define DWC3_GCTL_PRTCAPDIR(n)		((n) << 12)
	#define DWC3_GCTL_PRTCAP_HOST		1
	#define DWC3_GCTL_PRTCAP_DEVICE		2
	#define DWC3_GCTL_PRTCAP_OTG		3

#define DWC3_GCTL_CORESOFTRESET		(1 << 11)
#define DWC3_GCTL_RAMCLKSEL(x)		(((x) & DWC3_GCTL_CLK_MASK) << 6)
	#define DWC3_GCTL_CLK_BUS			(0)
	#define DWC3_GCTL_CLK_PIPE			(1)
	#define DWC3_GCTL_CLK_PIPEHALF		(2)
	#define DWC3_GCTL_CLK_MASK			(3)

#define DWC3_GCTL_SCALEDOWN(n)		((n) << 4)
#define DWC3_GCTL_SCALEDOWN_MASK 	DWC3_GCTL_SCALEDOWN(3)
	#define DWC3_GCTL_DISSCRAMBLE		(1 << 3)  // 1=dis Controller Scrambling
	#define DWC3_GCTL_U2EXIT_LFPS		(1 << 2)
	#define DWC3_GCTL_GBLHIBERNATIONEN	(1 << 1)
	#define DWC3_GCTL_DSBLCLKGTNG		(1 << 0)  //1=dis

/* Global Synopsys ID Register */
#define DWC3_GSNPSID_MASK	0xffff0000
#define DWC3_GSNPSREV_MASK	0x0000ffff
#define DWC3_REVISION_173A	0x5533173a
#define DWC3_REVISION_175A	0x5533175a
#define DWC3_REVISION_180A	0x5533180a
#define DWC3_REVISION_183A	0x5533183a
#define DWC3_REVISION_185A	0x5533185a
#define DWC3_REVISION_188A	0x5533188a
#define DWC3_REVISION_190A	0x5533190a
#define DWC3_REVISION_290A	0x5533290a
#define DWC3_REVISION_300A	0x5533300a
#define DWC3_REVISION_310A	0x5533310a
#define DWC3_REVISION_320A	0x5533320a

/* Global User Control 1 Register */
#define DWC3_GUCTL1_TX_IPGAP_LINECHECK_DIS	(1 << 28)
#define DWC3_GUCTL1_FORCE_20_CLK_FOR_30_CLK	(1 << 26)
#define DWC3_GUCTL1_DEV_L1_EXIT_BY_HW		(1 << 24)

/* Global USB2 PHY Configuration Register */
#define DWC3_GUSB2PHYCFG_PHYSOFTRST 		(1 << 31)
#define DWC3_GUSB2PHYCFG_U2_FREECLK_EXISTS	(1 << 30)
#define DWC3_GUSB2PHYCFG_USBTRDTIM(n)		(n << 10)
#define DWC3_GUSB2PHYCFG_USBTRDTIM_MASK		DWC3_GUSB2PHYCFG_USBTRDTIM(0xf)
#define USBTRDTIM_UTMI_8_BIT		9
#define USBTRDTIM_UTMI_16_BIT		5

#define DWC3_GUSB2PHYCFG_ENBLSLPM		(1 << 8)
#define DWC3_GUSB2PHYCFG_SUSPHY			(1 << 6)
#define DWC3_GUSB2PHYCFG_ULPI_UTMI		(1 << 4)

#define DWC3_GUSB2PHYCFG_PHYIF(n)		(n << 3)
#define DWC3_GUSB2PHYCFG_PHYIF_MASK		DWC3_GUSB2PHYCFG_PHYIF(1)
#define UTMI_PHYIF_16_BIT			1
#define UTMI_PHYIF_8_BIT			0

/* Global USB3 PIPE Control Register */
#define DWC3_GUSB3PIPECTL_PHYSOFTRST 	(1 << 31)
#define DWC3_GUSB3PIPECTL_U2SSINP3OK	(1 << 29)
#define DWC3_GUSB3PIPECTL_DISRXDETINP3	(1 << 28)
#define DWC3_GUSB3PIPECTL_UX_EXIT_PX	(1 << 27)
#define DWC3_GUSB3PIPECTL_REQP1P2P3		(1 << 24)
#define DWC3_GUSB3PIPECTL_DEP1P2P3(n)	((n) << 19)
#define DWC3_GUSB3PIPECTL_DEP1P2P3_MASK	DWC3_GUSB3PIPECTL_DEP1P2P3(7)
#define DWC3_GUSB3PIPECTL_DEP1P2P3_EN	DWC3_GUSB3PIPECTL_DEP1P2P3(1)
#define DWC3_GUSB3PIPECTL_DEPOCHANGE	(1 << 18)
#define DWC3_GUSB3PIPECTL_SUSPHY 		(1 << 17)
#define DWC3_GUSB3PIPECTL_LFPSFILT		(1 << 9)
#define DWC3_GUSB3PIPECTL_RX_DETOPOLL	(1 << 8)
#define DWC3_GUSB3PIPECTL_TX_DEEPH_MASK	DWC3_GUSB3PIPECTL_TX_DEEPH(3)
#define DWC3_GUSB3PIPECTL_TX_DEEPH(n)	((n) << 1)

/* Global TX Fifo Size Register */
#define DWC3_GTXFIFOSIZ_TXFDEF(n) 		((n) & 0xffff)
#define DWC3_GTXFIFOSIZ_TXFSTADDR(n) 	((n) & 0xffff0000)

/* Global Event Size Registers */
#define DWC3_GEVNTSIZ_INTMASK			(1 << 31)
#define DWC3_GEVNTSIZ_SIZE(n)			((n) & 0xffff)

/* Global Event Buffer Count Register */
#define DWC3_GEVNTCOUNT_MASK			0xfffc
#define DWC3_GEVNTCOUNT_EHB				(1 << 31)
/************ Global Hardware Parameters ************/
/* HWPARAMS0 */
#define DWC3_GHWPARAMS0_MODE(n)		((n) & 0x7)
#define DWC3_GHWPARAMS0_MODE_GADGET	0
#define DWC3_GHWPARAMS0_MODE_HOST	1
#define DWC3_GHWPARAMS0_MODE_DRD	2

#define DWC3_GHWPARAMS0_MBUS_TYPE(n)	(((n) >> 3) & 0x7)
#define DWC3_GHWPARAMS0_SBUS_TYPE(n)	(((n) >> 6) & 0x3)
#define DWC3_GHWPARAMS0_MDWIDTH(n)		(((n) >> 8) & 0xff)
#define DWC3_GHWPARAMS0_SDWIDTH(n)		(((n) >> 16) & 0xff)
#define DWC3_GHWPARAMS0_AWIDTH(n)		(((n) >> 24) & 0xff)

/* Global HWPARAMS1 Register */
#define DWC3_GHWPARAMS1_EN_PWROPT(n)	(((n) & (3 << 24)) >> 24)
#define DWC3_GHWPARAMS1_EN_PWROPT_NO	0
#define DWC3_GHWPARAMS1_EN_PWROPT_CLK	1
#define DWC3_GHWPARAMS1_EN_PWROPT_HIB	2
#define DWC3_GHWPARAMS1_PWROPT(n)		((n) << 24)
#define DWC3_GHWPARAMS1_PWROPT_MASK		DWC3_GHWPARAMS1_PWROPT(3)

#define DWC3_NUM_INT(n)		(((n) & (0x3f << 15)) >> 15)

/* Global HWPARAMS3 Register */
#define DWC3_GHWPARAMS3_SSPHY_IFC(n)		((n) & 3)
#define DWC3_GHWPARAMS3_SSPHY_IFC_DIS		0
#define DWC3_GHWPARAMS3_SSPHY_IFC_GEN1		1
#define DWC3_GHWPARAMS3_SSPHY_IFC_GEN2		2 /* DWC_usb31 only */

#define DWC3_GHWPARAMS3_HSPHY_IFC(n)		(((n) & (3 << 2)) >> 2)
#define DWC3_GHWPARAMS3_HSPHY_IFC_DIS		0
#define DWC3_GHWPARAMS3_HSPHY_IFC_UTMI		1
#define DWC3_GHWPARAMS3_HSPHY_IFC_ULPI		2
#define DWC3_GHWPARAMS3_HSPHY_IFC_UTMI_ULPI	3

#define DWC3_GHWPARAMS3_FSPHY_IFC(n)		(((n) & (3 << 4)) >> 4)
#define DWC3_GHWPARAMS3_FSPHY_IFC_DIS		0
#define DWC3_GHWPARAMS3_FSPHY_IFC_ENA		1

#define DWC3_NUM_IN_EPS_MASK	(0x1f << 18)
#define DWC3_NUM_EPS_MASK		(0x3f << 12)
#define DWC3_NUM_IN_EPS(n)		( ( n & DWC3_NUM_IN_EPS_MASK) >> 18)
#define DWC3_NUM_EPS(n)			( ( n & DWC3_NUM_EPS_MASK) >> 12)


/* Global HWPARAMS4 Register */
#define DWC3_GHWPARAMS4_HIBER_SCRATCHBUFS(n)	(((n) & (0x0f << 13)) >> 13)
#define DWC3_MAX_HIBER_SCRATCHBUFS			15

/* Global HWPARAMS6 Register */
#define DWC3_GHWPARAMS6_BCSUPPORT			(1 << 14)
#define DWC3_GHWPARAMS6_OTG3SUPPORT			(1 << 13)
#define DWC3_GHWPARAMS6_ADPSUPPORT			(1 << 12)
#define DWC3_GHWPARAMS6_HNPSUPPORT			(1 << 11)
#define DWC3_GHWPARAMS6_SRPSUPPORT			(1 << 10)
#define DWC3_GHWPARAMS6_EN_FPGA				(1 << 7)

/* HWPARAMS7 */
#define DWC3_GHWPARAMS7_RAM1_DEPTH(n)		((n) & 0xffff)
#define DWC3_GHWPARAMS7_RAM2_DEPTH(n)		(((n) >> 16) & 0xffff)

/* Global User Control Register 2 */
#define DWC3_GUCTL2_RST_ACTBITLATER			(1 << 14)

/* Global Frame Length Adjustment Register */
#define DWC3_GFLADJ_30MHZ_SDBND_SEL			(1 << 7)
#define DWC3_GFLADJ_30MHZ_MASK				0x3f


//===========================================================================================
/* Global constants */
//===========================================================================================


//===========================================================================================
/* others */
//===========================================================================================
/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#if 1

#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 	5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */

#define	EINPROGRESS	36	/* Operation now in progress */
#define	EALREADY	37	/* Operation already in progress */
#define	ENOTSOCK	38	/* Socket operation on non-socket */
#define	EDESTADDRREQ	39	/* Destination address required */
#define	EMSGSIZE	40	/* Message too long */
#define	EPROTOTYPE	41	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	42	/* Protocol not available */
#define	EPROTONOSUPPORT	43	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	44	/* Socket type not supported */
#define	EOPNOTSUPP	45	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	46	/* Protocol family not supported */
#define	EAFNOSUPPORT	47	/* Address family not supported by protocol */
#define	EADDRINUSE	48	/* Address already in use */
#define	EADDRNOTAVAIL	49	/* Cannot assign requested address */
#define	ENETDOWN	50	/* Network is down */
#define	ENETUNREACH	51	/* Network is unreachable */
#define	ENETRESET	52	/* Network dropped connection because of reset */
#define	ECONNABORTED	53	/* Software caused connection abort */
#define	ECONNRESET	54	/* Connection reset by peer */
#define	ENOBUFS		55	/* No buffer space available */
#define	EISCONN		56	/* Transport endpoint is already connected */
#define	ENOTCONN	57	/* Transport endpoint is not connected */
#define	ESHUTDOWN	58	/* Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	59	/* Too many references: cannot splice */
#define	ETIMEDOUT	60	/* Connection timed out */
#define	ECONNREFUSED	61	/* Connection refused */
#define	ELOOP		62	/* Too many symbolic links encountered */
#define	ENAMETOOLONG	63	/* File name too long */
#define	EHOSTDOWN	64	/* Host is down */
#define	EHOSTUNREACH	65	/* No route to host */
#define	ENOTEMPTY	66	/* Directory not empty */

#define	EUSERS		68	/* Too many users */
#define	EDQUOT		69	/* Quota exceeded */
#define	ESTALE		70	/* Stale file handle */
#define	EREMOTE		71	/* Object is remote */

#define	ENOLCK		77	/* No record locks available */
#define	ENOSYS		78	/* Function not implemented */

#define	ENOMSG		80	/* No message of desired type */
#define	EIDRM		81	/* Identifier removed */
#define	ENOSR		82	/* Out of streams resources */
#define	ETIME		83	/* Timer expired */
#define	EBADMSG		84	/* Not a data message */
#define	EPROTO		85	/* Protocol error */
#define	ENODATA		86	/* No data available */
#define	ENOSTR		87	/* Device not a stream */

#define	ENOPKG		92	/* Package not installed */

#define	EILSEQ		116	/* Illegal byte sequence */

/* The following are just random noise.. */
#define	ECHRNG		88	/* Channel number out of range */
#define	EL2NSYNC	89	/* Level 2 not synchronized */
#define	EL3HLT		90	/* Level 3 halted */
#define	EL3RST		91	/* Level 3 reset */

#define	ELNRNG		93	/* Link number out of range */
#define	EUNATCH		94	/* Protocol driver not attached */
#define	ENOCSI		95	/* No CSI structure available */
#define	EL2HLT		96	/* Level 2 halted */
#define	EBADE		97	/* Invalid exchange */
#define	EBADR		98	/* Invalid request descriptor */
#define	EXFULL		99	/* Exchange full */
#define	ENOANO		100	/* No anode */
#define	EBADRQC		101	/* Invalid request code */
#define	EBADSLT		102	/* Invalid slot */

#endif

//===========================================================================================
/* end */
//===========================================================================================
#endif /* __DRIVERS_USB_DWC3_CORE_H */
                       



