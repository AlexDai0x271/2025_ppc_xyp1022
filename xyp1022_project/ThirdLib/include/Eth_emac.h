#ifndef ETH_EMAC
#define ETH_EMAC

//#include "common.h"
//#include "sec_common.h"
#include "dpaa.h"
#include "ccm_axi.h"
#include "cpr.h"

#define GMAC0	1	//default gmac0;jianding test£ºgmac0:net wire;gmac1-3:ex-loopback
#define GMAC1	0
#define GMAC2	0
#define GMAC3	0

#if GMAC0

#else
//	#define EXTERNAL_LOOP
#endif
#ifdef GMAC1
	#define XYP1022_DP83867     //enable DP83867,disable 88e1111 GMAC1

#endif

#define GMII			//or MII TODO
#define MAC_RCVAL
//#define LOW_SPEED		//10Mbps
//#define INTERNAL_LOOP


//#define MAC_UDAF
//#define MAC_UHASH
//#define VLAN_MATCH			//no filter cability,but show bit change in desc0
//#define VLAN_HASH				//no filter cability,but show bit change in desc0
//#define JUMBO
//#define JABBER
#ifdef XYP1022_DP83867
#define rgmii
#define DP83867
#endif

#if 1
#define gmac_dbg prints
#define dev_info prints
#define dev_dbg prints
#define printk prints
#else
#define gmac_dbg
#define dev_info
#define dev_dbg
#define printk
#endif

//ec channel
#ifdef XYP1022_DP83867
#define MAC_NUM			3			//dp83867
#else if
#define MAC_NUM			2			//88e1111
#endif

#define Eth_addr		(mac0_cfg_base + 0x4000*MAC_NUM)

#define PHYBUSY		0x1
#define DELAY		1000000

#define UCHAR unsigned char
#define UINT	unsigned int

#define U8 unsigned char
#define U32 unsigned int
#define S8 char
#define S32 int

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define s8 char
#define s32 int

#define DBF			(0x1<<5)
#define PM			(0x1<<4)
#define DAIF		(0x1<<3)
#define HMC			(0x1<<2)
#define HPF			(0x1<<10)
#define VTFE		(0x1<<16)
#define HUC			(0x1<<1)
#define PRM			(0x1<<0)

#define NORMAL_SPEED		0x0
#define FAST_SPEED			0x1

//BUS MODE
#define AAL			(0x1<<25)
#define FBPL		(0x1<<24)
#define USP			(0x1<<23)
#define RXPBL		(0x8<<17)
#define FB			(0x1<<16)
#define PR			(0x0<<14)
//#define PBL			(0x8<<8)
#define PBL			(0x10<<8)
#define DSL			(0x4<<2)
#define DA			(0x1<<1)
#define SWR			(0x1<<0)

//Operation mode register
#define DT			(0x1<<26)
#define RSF			(0x1<<25)
#define DFF			(0x1<<24)
#define TSF			(0x1<<21)
#define FTF			(0x1<<20)
#define TTC			(0x1<<14)
#define ST			(0x1<<13)
#define RFD			(0x1<<11)
#define RFA			(0x3<<9)
#define EFC			(0x1<<8)
#define FEF			(0x1<<7)
#define FUF			(0x1<<6)
#define RTC			(0x3<<3)
//#define RTC			(0x1<<3)
#define OSF			(0x1<<2)
#define SR			(0x1<<1)

//#define IFG		(0x4<<17)  //100: 64bit times
#define SARC		(2<<28)		//0b10 insert sa
#define WD			(1<<23)
#define JD			(1<<22)
#define JE			(1<<20)
#define IFG			(0x7<<17)  //111: 40bit times
#define DCRS		(0x0<<16)
#define PS			(0x1<<15) //MII (10/100 Mbps)
#define FES			(0x1<<14)
#define DO			(0x0<<13)
#define LM			(0x1<<12) //not use loopback mode
#define DM			(0x1<<11)
#define IPC			(0x0<<10)
#define DR			(0x0<<9)
#define LUD			(0x0<<8)
#define ACS			(0x1<<7)
#define BL			(0x1<<5)
#define DC			(0x0<<4)
#define TE			(0x1<<3)
#define RE			(0x1<<2)
#define SEPPD_10M	(0x2<<14)
#define SEPPD_100M	(0x3<<14)
#define SEPPD_1000M	(0x0<<14)

//FLOW Control register
#define PT			(0x200<<16)
#define TFE			(0x1<<2)
#define	RFE			(0x1<<1)

//Relation with Buffer
#define NO_OF_TXDES			8
#define NO_OF_RXDES			8

#define MIN_SIZE			(0x40)
//#define MAX_BUF				(512)
#define MAX_BUF				(1024)
//#define MAX_BUF				(1536)
//#define MAX_BUF				(2048)

extern u32 SINGLE_TXDES_SIZE;
extern u32 SINGLE_RXDES_SIZE;

extern u32 SINGLE_TXBUF_SIZE;
extern u32 SINGLE_RXBUF_SIZE;
extern u32 SIZE_OF_TXDES;
extern u32 SIZE_OF_TXBUF;
extern u32 SIZE_OF_RXDES;
extern u32 SIZE_OF_RXBUF;

extern u32 ETH_BASE_TXDES_PHY;
extern u32 ETH_BASE_TXBUF_PHY;
extern u32 ETH_BASE_RXDES_PHY;
extern u32 ETH_BASE_RXBUF_PHY;

//TXDES0
#define TXOWN				(0x1<<31)
#define CMPL_INT			(0x1<<30)
#define TXES				(0x1<<15)
#define TXJT 				(0x1<<14)
#define TXFF				(0x1<<13)
//#define TXLC				(0x1<<11)
#define TXNC				(0x1<<10)
#define TXLC				(0x1<<9)
#define TXEC				(0x1<<8)
#define TXED				(0x1<<2)
#define TXUE				(0x1<<1)

//TXDES1
#define IC					(0x1<<31)
#define TXLS				(0x1<<30)
#define FS					(0x1<<29)
#define CIC					(0x3<<27)
#define DC_T				(0x0<<26)
#define DC					(0x1<<26)
#define TER					(0x1<<25)
#define TCH					(0x1<<24) //not second address but next descriptor address
#define DP					(0x0<<23)
#define TTSE				(0x1<<22)
#define TBS2				((SINGLE_TXBUF_SIZE&0x7ff)<<11)
#define TBS1				((SINGLE_TXBUF_SIZE&0x7ff)<<0)

//RXDES0
#define RXOWN				(0x1<<31)
#define RXLS				(0x1<<8)
#define RXFS				(0x1<<9)
#define RXES				(0x1<<15)
#define RXDE				(0x1<<14)
#define RXOE				(0x1<<11)
#define RXVLAN				(0x1<<10)
#define RXGF				(0x1<<7)
#define RXLC				(0x1<<6)
#define RXWT				(0x1<<4)
#define RXRE				(0x1<<3)
#define RXCRCE				(0x1<<1)

//RXDES1
#define DINT				(0x0<<31)
#define RER					(0x1<<25)
#define RCH					(0x1<<24)
#define RBS2				((SINGLE_RXBUF_SIZE&0xfff)<<11)
#define RBS1				((SINGLE_RXBUF_SIZE&0xfff)<<0)

/***************************** NET definition **************************************/
#define MII_JAB             16
#define MII_OutputStatus    24

#define BMCR_FULLDPLX       0x0100    // Full duplex
#define BMCR_ANRESTART      0x0200    // Auto negotiation restart
#define BMCR_ANENABLE       0x1000    // Enable auto negotiation
#define BMCR_SPEED100       0x2000    // Select 100Mbps
#define BMCR_SPEED1000       0x0040    // Select 100Mbps
#define BMSR_LSTATUS        0x0004    // Link status
#define PHY_16_JAB_ENB      0x1000
#define PHY_16_PORT_ENB     0x1

#define BMCR_RESET			0x8000  /* 0 = normal, 1 = PHY reset */

//DMA status register
#define TTI			(0x1<<29)
#define GPI			(0x1<<28)
#define GMI			(0x1<<27)
#define GLI			(0x1<<26)
#define TSSTOP		(0x0<<20)
#define TSFET		(0x1<<20)
#define TSWAIT		(0x2<<20)
#define TSTRAN		(0x3<<20)
#define TSSPEND		(0x6<<20)
#define TSCLOSE		(0x7<<20)
#define RSSTOP		(0x0<<17)
#define RSFET		(0x1<<17)
#define RSWAIT		(0x3<<17)
#define RSSPEND		(0x4<<17)
#define RSCLOSE		(0x5<<17)
#define RSTRAN		(0x7<<17)
#define NORINTE		(0x1<<16)
#define ABNORINTE	(0x1<<15)
#define ERI			(0x1<<14)
#define FBI			(0x1<<13)
#define ETI			(0x1<<10)
#define RWT			(0x1<<9)
#define RPS			(0x1<<8)
#define RU			(0x1<<7)
#define RI			(0x1<<6)
#define UNF			(0x1<<5)
#define OVF			(0x1<<4)
#define TJT			(0x1<<3)
#define TU			(0x1<<2)
#define TPS			(0x1<<1)
#define TI			(0x1<<0)

//DMA interrupt enable register
#define NIE			(0x1<<16)
#define AIE			(0x1<<15)
#define ERE			(0x1<<14)
#define FBE			(0x1<<13)
#define ETE			(0x1<<10)
#define RWE			(0x1<<9)
#define RSE			(0x1<<8)
#define RUE			(0x1<<7)
#define RIE			(0x1<<6)
#define UNE			(0x1<<5)
#define OVE			(0x1<<4)
#define TJE			(0x1<<3)
#define TUE			(0x1<<2)
#define TSE			(0x1<<1)
#define TIE			(0x1<<0)

/***************************** MII definition ***************************************/
/* PHY Common Register */
#define MII_BMCR                        0x00
#define MII_BMSR                        0x01
#define MII_PHYSID1                     0x02
#define MII_PHYSID2                     0x03
#define MII_ADVERTISE                   0x04
#define MII_LPA                         0x05
#define MII_EXPANSION                   0x06
#define MII_AT001_CR                    0x09
#define MII_AT001_SR                    0x0A
#define MII_AT001_ESR                   0x0F
#define MII_AT001_PSCR                  0x10
#define MII_AT001_PSSR                  0x11
#define MII_INT_CTRL                    0x12
#define MII_INT_STATUS                  0x13
#define MII_SMARTSPEED                  0x14
#define MII_RERRCOUNTER                 0x15
#define MII_SREVISION                   0x16
#define MII_RESV1                       0x17
#define MII_LBRERROR                    0x18
#define MII_PHYADDR                     0x19
#define MII_RESV2                       0x1a
#define MII_TPISTATUS                   0x1b
#define MII_NCONFIG                     0x1c

#define MII_PAGE                   22
#define MII_SP_CTRL_2              21

#define MII_DBG_ADDR			0x1D
#define MII_DBG_DATA			0x1E


/* PHY Control Register */
#define MII_CR_SPEED_SELECT_MSB                  0x0040  /* bits 6,13: 10=1000, 01=100, 00=10 */
#define MII_CR_COLL_TEST_ENABLE                  0x0080  /* Collision test enable */
#define MII_CR_FULL_DUPLEX                       0x0100  /* FDX =1, half duplex =0 */
#define MII_CR_RESTART_AUTO_NEG                  0x0200  /* Restart auto negotiation */
#define MII_CR_ISOLATE                           0x0400  /* Isolate PHY from MII */
#define MII_CR_POWER_DOWN                        0x0800  /* Power down */
#define MII_CR_AUTO_NEG_EN                       0x1000  /* Auto Neg Enable */
#define MII_CR_SPEED_SELECT_LSB                  0x2000  /* bits 6,13: 10=1000, 01=100, 00=10 */
#define MII_CR_LOOPBACK                          0x4000  /* 0 = normal, 1 = loopback */
#define MII_CR_RESET                             0x8000  /* 0 = normal, 1 = PHY reset */
#define MII_CR_SPEED_MASK                        0x2040
#define MII_CR_SPEED_1000                        0x0040
#define MII_CR_SPEED_100                         0x2000
#define MII_CR_SPEED_10                          0x0000


/* PHY Status Register */
#define MII_SR_EXTENDED_CAPS                     0x0001  /* Extended register capabilities */
#define MII_SR_JABBER_DETECT                     0x0002  /* Jabber Detected */
#define MII_SR_LINK_STATUS                       0x0004  /* Link Status 1 = link */
#define MII_SR_AUTONEG_CAPS                      0x0008  /* Auto Neg Capable */
#define MII_SR_REMOTE_FAULT                      0x0010  /* Remote Fault Detect */
#define MII_SR_AUTONEG_COMPLETE                  0x0020  /* Auto Neg Complete */
#define MII_SR_PREAMBLE_SUPPRESS                 0x0040  /* Preamble may be suppressed */
#define MII_SR_EXTENDED_STATUS                   0x0100  /* Ext. status info in Reg 0x0F */
#define MII_SR_100T2_HD_CAPS                     0x0200  /* 100T2 Half Duplex Capable */
#define MII_SR_100T2_FD_CAPS                     0x0400  /* 100T2 Full Duplex Capable */
#define MII_SR_10T_HD_CAPS                       0x0800  /* 10T   Half Duplex Capable */
#define MII_SR_10T_FD_CAPS                       0x1000  /* 10T   Full Duplex Capable */
#define MII_SR_100X_HD_CAPS                      0x2000  /* 100X  Half Duplex Capable */
#define MII_SR_100X_FD_CAPS                      0x4000  /* 100X  Full Duplex Capable */
#define MII_SR_100T4_CAPS                        0x8000  /* 100T4 Capable */

/* Link partner ability register. */
#define MII_LPA_SLCT                             0x001f  /* Same as advertise selector  */
#define MII_LPA_10HALF                           0x0020  /* Can do 10mbps half-duplex   */
#define MII_LPA_10FULL                           0x0040  /* Can do 10mbps full-duplex   */
#define MII_LPA_100HALF                          0x0080  /* Can do 100mbps half-duplex  */
#define MII_LPA_100FULL                          0x0100  /* Can do 100mbps full-duplex  */
#define MII_LPA_100BASE4                         0x0200  /* 100BASE-T4  */
#define MII_LPA_PAUSE                            0x0400  /* PAUSE */
#define MII_LPA_ASYPAUSE                         0x0800  /* Asymmetrical PAUSE */
#define MII_LPA_RFAULT                           0x2000  /* Link partner faulted        */
#define MII_LPA_LPACK                            0x4000  /* Link partner acked us       */
#define MII_LPA_NPAGE                            0x8000  /* Next page bit               */

/* Autoneg Advertisement Register */
#define MII_AR_SELECTOR_FIELD                   0x0001  /* indicates IEEE 802.3 CSMA/CD */
#define MII_AR_10T_HD_CAPS                      0x0020  /* 10T   Half Duplex Capable */
#define MII_AR_10T_FD_CAPS                      0x0040  /* 10T   Full Duplex Capable */
#define MII_AR_100TX_HD_CAPS                    0x0080  /* 100TX Half Duplex Capable */
#define MII_AR_100TX_FD_CAPS                    0x0100  /* 100TX Full Duplex Capable */
#define MII_AR_100T4_CAPS                       0x0200  /* 100T4 Capable */
#define MII_AR_PAUSE                            0x0400  /* Pause operation desired */
#define MII_AR_ASM_DIR                          0x0800  /* Asymmetric Pause Direction bit */
#define MII_AR_REMOTE_FAULT                     0x2000  /* Remote Fault detected */
#define MII_AR_NEXT_PAGE                        0x8000  /* Next Page ability supported */
#define MII_AR_SPEED_MASK                       0x01E0
#define MII_AR_DEFAULT_CAP_MASK                 0x0DE0

/* 1000BASE-T Control Register */
#define MII_AT001_CR_1000T_HD_CAPS              0x0100  /* Advertise 1000T HD capability */
#define MII_AT001_CR_1000T_FD_CAPS              0x0200  /* Advertise 1000T FD capability  */
#define MII_AT001_CR_1000T_REPEATER_DTE         0x0400  /* 1=Repeater/switch device port */
/* 0=DTE device */
#define MII_AT001_CR_1000T_MS_VALUE             0x0800  /* 1=Configure PHY as Master */
/* 0=Configure PHY as Slave */
#define MII_AT001_CR_1000T_MS_ENABLE            0x1000  /* 1=Master/Slave manual config value */
/* 0=Automatic Master/Slave config */
#define MII_AT001_CR_1000T_TEST_MODE_NORMAL     0x0000  /* Normal Operation */
#define MII_AT001_CR_1000T_TEST_MODE_1          0x2000  /* Transmit Waveform test */
#define MII_AT001_CR_1000T_TEST_MODE_2          0x4000  /* Master Transmit Jitter test */
#define MII_AT001_CR_1000T_TEST_MODE_3          0x6000  /* Slave Transmit Jitter test */
#define MII_AT001_CR_1000T_TEST_MODE_4          0x8000  /* Transmitter Distortion test */
#define MII_AT001_CR_1000T_SPEED_MASK           0x0300
#define MII_AT001_CR_1000T_DEFAULT_CAP_MASK     0x0300

/* 1000BASE-T Status Register */
#define MII_AT001_SR_1000T_LP_HD_CAPS           0x0400  /* LP is 1000T HD capable */
#define MII_AT001_SR_1000T_LP_FD_CAPS           0x0800  /* LP is 1000T FD capable */
#define MII_AT001_SR_1000T_REMOTE_RX_STATUS     0x1000  /* Remote receiver OK */
#define MII_AT001_SR_1000T_LOCAL_RX_STATUS      0x2000  /* Local receiver OK */
#define MII_AT001_SR_1000T_MS_CONFIG_RES        0x4000  /* 1=Local TX is Master, 0=Slave */
#define MII_AT001_SR_1000T_MS_CONFIG_FAULT      0x8000  /* Master/Slave config fault */
#define MII_AT001_SR_1000T_REMOTE_RX_STATUS_SHIFT   12
#define MII_AT001_SR_1000T_LOCAL_RX_STATUS_SHIFT    13

/* Extended Status Register */
#define MII_AT001_ESR_1000T_HD_CAPS             0x1000  /* 1000T HD capable */
#define MII_AT001_ESR_1000T_FD_CAPS             0x2000  /* 1000T FD capable */
#define MII_AT001_ESR_1000X_HD_CAPS             0x4000  /* 1000X HD capable */
#define MII_AT001_ESR_1000X_FD_CAPS             0x8000  /* 1000X FD capable */

/* AT001 PHY Specific Control Register */
#define MII_AT001_PSCR_JABBER_DISABLE           0x0001  /* 1=Jabber Function disabled */
#define MII_AT001_PSCR_POLARITY_REVERSAL        0x0002  /* 1=Polarity Reversal enabled */
#define MII_AT001_PSCR_SQE_TEST                 0x0004  /* 1=SQE Test enabled */
#define MII_AT001_PSCR_MAC_POWERDOWN            0x0008
#define MII_AT001_PSCR_CLK125_DISABLE           0x0010  /* 1=CLK125 low,
							 * 0=CLK125 toggling
							 */
#define MII_AT001_PSCR_MDI_MANUAL_MODE          0x0000  /* MDI Crossover Mode bits 6:5 */
/* Manual MDI configuration */
#define MII_AT001_PSCR_MDIX_MANUAL_MODE         0x0020  /* Manual MDIX configuration */
#define MII_AT001_PSCR_AUTO_X_1000T             0x0040  /* 1000BASE-T: Auto crossover,
							 *  100BASE-TX/10BASE-T:
							 *  MDI Mode
							 */
#define MII_AT001_PSCR_AUTO_X_MODE              0x0060  /* Auto crossover enabled
							 * all speeds.
							 */
#define MII_AT001_PSCR_10BT_EXT_DIST_ENABLE     0x0080
/* 1=Enable Extended 10BASE-T distance
 * (Lower 10BASE-T RX Threshold)
 * 0=Normal 10BASE-T RX Threshold */
#define MII_AT001_PSCR_MII_5BIT_ENABLE          0x0100
/* 1=5-Bit interface in 100BASE-TX
 * 0=MII interface in 100BASE-TX */
#define MII_AT001_PSCR_SCRAMBLER_DISABLE        0x0200  /* 1=Scrambler disable */
#define MII_AT001_PSCR_FORCE_LINK_GOOD          0x0400  /* 1=Force link good */
#define MII_AT001_PSCR_ASSERT_CRS_ON_TX         0x0800  /* 1=Assert CRS on Transmit */
#define MII_AT001_PSCR_POLARITY_REVERSAL_SHIFT    1
#define MII_AT001_PSCR_AUTO_X_MODE_SHIFT          5
#define MII_AT001_PSCR_10BT_EXT_DIST_ENABLE_SHIFT 7
/* AT001 PHY Specific Status Register */
#define MII_AT001_PSSR_SPD_DPLX_RESOLVED        0x0800  /* 1=Speed & Duplex resolved */
#define MII_AT001_PSSR_DPLX                     0x2000  /* 1=Duplex 0=Half Duplex */
#define MII_AT001_PSSR_SPEED                    0xC000  /* Speed, bits 14:15 */
#define MII_AT001_PSSR_10MBS                    0x0000  /* 00=10Mbs */
#define MII_AT001_PSSR_100MBS                   0x4000  /* 01=100Mbs */
#define MII_AT001_PSSR_1000MBS                  0x8000  /* 10=1000Mbs */

/***************************** Reg definition **************************************/
#define rMACConfig     	  		 	 (0x00 ) //This is the operation mode register for the MAC.
#define rMACFrameFilter   		 	 (0x04 ) //Contains the frame filtering controls.
#define rHashTableH     			 (0x08 ) //Contains the higher 32 bits of the Multicast Hash table.This register is present only when the Hash filter function is selected in coreConsultant.
#define rHashTableL     			 (0x0c ) //Contains the lower 32 bits of the Multicast Hash table.This register is present only when the Hash filter function is selected in coreConsultant.
#define rGMIIAddr		     		 (0x10 ) //Controls the management cycles to an external PHY. This register is present only when the Station Management (MDIO) feature is selected in coreConsultant.
#define rGMIIData		     		 (0x14 ) //Contains the data to be written to or read from the PHY register. This register is present only when the Station Management (MDIO) feature is selected in coreConsultant.
#define rFlowControl     			 (0x18 ) //Controls the generation of control frames.
#define rVLANTag		    		 (0x1c ) //Identifies IEEE 802.1Q VLAN type frames.

#define rIdeftifiesVersion  	 		 (0x20 ) //Identifies the version of the Core
#define rReserved	   			 (0x24 ) //Reserved
#define rRemoteWakeUp   			 (0x28 ) //This is the address through which the remote Wake-up Frame Filter registers
#define rPMTControl     			 (0x2c ) //This register is present only when the PMT module is selected in coreConsultant.
#define rLPIcsr						(0x30)
#define rInterruptStatus   		 	 (0x38 ) //Contains the interrupt status.
#define rInterruptMask     		 	 (0x3c ) //Contains the masks for generating the interrupts.
#define rMACAddr0H     				 (0x40 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr0L	    			 (0x44 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr1H    				 (0x48 ) //Contains the interrupt status.
#define rMACAddr1L     				 (0x4c ) //Contains the masks for generating the interrupts.
#define rMACAddr2H      			 (0x50 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr2L 	    			 (0x54 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr3H    				 (0x58 ) //Contains the interrupt status.
#define rMACAddr3L      			 (0x5c ) //Contains the masks for generating the interrupts.
#define rMACAddr4H     				 (0x60 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr4L 	    			 (0x64 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr5H     				 (0x68 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr5L 	    			 (0x6C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr6H     				 (0x70 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr6L 	    			 (0x74 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr7H     				 (0x78 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr7L 	    			 (0x7C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr8H     				 (0x80 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr8L 	    			 (0x84 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr9H     				 (0x88 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr9L 	    			 (0x8C ) //Contains the lower 32 bits of the first MAC address.

#define rMACAddr10H     			 (0x90 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr10L 	    		 	 (0x94 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr11H     			 (0x98 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr11L 	    		 	 (0x9C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr12H     			 (0xA0 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr12L 	    		 	 (0xA4 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr13H     			 (0xA8 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr13L 	    		 	 (0xAC ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr14H     			 (0xB0 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr14L 	    		 	 (0xB4 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr15H     			 (0xB8 ) //Contains the higher 16 bits of the first MAC address.
#define rMACAddr15L 	    		 	 (0xBC ) //Contains the lower 32 bits of the first MAC address.

#define rANControl 	    			 (0xC0 ) //Enables and/or restarts auto-negotiation. It also enables PCS loopback.
#define rANStatus		    		 (0xC4 ) //Indicates the link and auto-negotiation status.
#define rANAdvertise    			 (0xC8 ) //This register is configured before auto-negotiation begins. It contains the advertised ability of the GMAC.
#define rANegoLink  				 (0xCC ) //Contains the advertised ability of the link partner.
#define rANExpansion 	    		 	 (0xD0 ) //Indicates whether a new base page has been received from the link partner.
#define rTBIExStatus	    		 	 (0xD4 ) //Indicates all modes of operation of the GMAC.
#define rSGRGStatus	    			 (0xD8 ) //Indicates the status signals received from the PHY through the SGMII/RGMII interface.

#define rVlanHash					(0x588)

/* MMC registers offset */
#define GMAC_MMC_CTRL      			(0x100)
#define GMAC_MMC_RX_INTR   			(0x104)
#define GMAC_MMC_TX_INTR   			(0x108)
#define GMAC_MMC_RX_CSUM_OFFLOAD   		(0x208)

#define rTimeStampControl			 (0x700 ) //Controls the time stamp generation and update logic.
#define rSubSecIncrement			 (0x704 ) //Contains the 8-bit value by which the Sub-Second register is incremented.
#define rTimeStampHigh    		 	 (0x708 ) //Contains the most significant (higher) 32 time bits.
#define rTimeStampLow  				 (0x70C ) //Contains the least significant (lower) 32 time bits.
#define rTimeStampHighUp 			 (0x710 ) //Contains the most significant (higher) 32 bits of the time to be written to, added to, or subtracted from the System Time value.
#define rTimeStampLowUp	    	 	  	 (0x714 ) //Contains the least significant (lower) 32 bits of the time to be written to, added to, or subtracted from the System Time value.
#define rTimeStampAddend			 (0x718 ) //This register is used by the software to readjust the clock frequency linearly to match the master clock frequency.
#define rTargetTimeHigh	    	 	  	 (0x71C ) //This register is used by the software to readjust the clock frequency linearly to match the master clock frequency.
#define rTargetTimeLow	    	 		 (0x720 ) //Contains the lower 32 bits of time to be compared with the system time for interrupt event generation.

#define rMACAddr16H	    			 (0x800 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr16L				 (0x804 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr17H    				 (0x808 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr17L  				 (0x80C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr18H 				 (0x810 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr18L	    			 (0x814 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr19H	    			 (0x818 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr19L	    			 (0x81C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr20H	    			 (0x820 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr20L	    			 (0x824 ) //Contains the lower 32 bits of the first MAC address.

#define rMACAddr21H	    			 (0x828 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr21L				 (0x82C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr22H    				 (0x830 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr22L  				 (0x834 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr23H 				 (0x838 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr23L	    			 (0x83C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr24H	    			 (0x840 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr24L	    			 (0x844 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr25H	    			 (0x848 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr25L	    			 (0x84C ) //Contains the lower 32 bits of the first MAC address.

#define rMACAddr26H	    			 (0x850 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr26L				 (0x854 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr27H    				 (0x858 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr27L  				 (0x85C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr28H 				 (0x860 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr28L	    			 (0x864 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr29H	    			 (0x868 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr29L	    			 (0x86C ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr30H	    			 (0x870 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr30L	    			 (0x874 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr31H	    			 (0x878 ) //Contains the lower 32 bits of the first MAC address.
#define rMACAddr31L	    			 (0x87C ) //Contains the lower 32 bits of the first MAC address.


//Ethernet DMA Register
#define rBUSMODE     	       			 (0x1000 )    //Controls the Host Interface Mode
#define rTxPollDemand      			 (0x1004 )    //Used by the host to instruct the DMA to poll the Transmit Descriptor List
#define rRxPollDemand     			 (0x1008 )    //Used by the host to instruct the DMA to poll the Receive Descriptor List
#define rRxDescriptorListAddr     		 (0x100c )    //Points the DMA to the start of the Receive Descriptor list
#define rTxDescriptorListAddr     	 	 (0x1010 )    //Points the DMA to the start of the Transmit Descriptor list
#define rStatus                			 (0x1014 )    //The Software driver reads this register during interrupt service routine or polling to determint the status of the DMA
#define rOperationMode    			 (0x1018 )    //Establishes the Receive and Transmit operating modes and command
#define rInterruptEnable   			 (0x101c )    //Enables the interrupt reported by the Status Register
#define rMissOverflow     			 (0x1020 )    //Contains the counters for discarded frames because no host Receive Descriptor was available,and discarded frames because of Receive FIFO Overflow
#define rCurrHostTxDescriptor     		 (0x1048 )    //Points to the start of current Transmit Descriptor read by the DMA.
#define rCurrHostRxDescriptor     	 	 (0x104c )	  //Points to the start of current Receive Descriptor read by the DMA.
#define rCurrHostTxBufferAddr     	 	 (0x1050 )   //Points to the current Transmit Buffer address read by the DMA.
#define rCurrHostRxBufferAddr     	 	 (0x1054 )   //Points to the current Receive Buffer address read by the DMA.

/* DMA Status register defines */
#define DMA_STATUS_GPI		0x10000000	/* PMT interrupt */
#define DMA_STATUS_GMI		0x08000000	/* MMC interrupt */
#define DMA_STATUS_GLI		0x04000000	/* GMAC Line interface int */
#define DMA_STATUS_GMI		0x08000000
#define DMA_STATUS_GLI		0x04000000
#define DMA_STATUS_EB_MASK	0x00380000	/* Error Bits Mask */
#define DMA_STATUS_EB_TX_ABORT	0x00080000	/* Error Bits - TX Abort */
#define DMA_STATUS_EB_RX_ABORT	0x00100000	/* Error Bits - RX Abort */
#define DMA_STATUS_TS_MASK	0x00700000	/* Transmit Process State */
#define DMA_STATUS_TS_SHIFT	20
#define DMA_STATUS_RS_MASK	0x000e0000	/* Receive Process State */
#define DMA_STATUS_RS_SHIFT	17
#define DMA_STATUS_NIS	0x00010000	/* Normal Interrupt Summary */
#define DMA_STATUS_AIS	0x00008000	/* Abnormal Interrupt Summary */
#define DMA_STATUS_ERI	0x00004000	/* Early Receive Interrupt */
#define DMA_STATUS_FBI	0x00002000	/* Fatal Bus Error Interrupt */
#define DMA_STATUS_ETI	0x00000400	/* Early Transmit Interrupt */
#define DMA_STATUS_RWT	0x00000200	/* Receive Watchdog Timeout */
#define DMA_STATUS_RPS	0x00000100	/* Receive Process Stopped */
#define DMA_STATUS_RU	0x00000080	/* Receive Buffer Unavailable */
#define DMA_STATUS_RI	0x00000040	/* Receive Interrupt */
#define DMA_STATUS_UNF	0x00000020	/* Transmit Underflow */
#define DMA_STATUS_OVF	0x00000010	/* Receive Overflow */
#define DMA_STATUS_TJT	0x00000008	/* Transmit Jabber Timeout */
#define DMA_STATUS_TU	0x00000004	/* Transmit Buffer Unavailable */
#define DMA_STATUS_TPS	0x00000002	/* Transmit Process Stopped */
#define DMA_STATUS_TI	0x00000001	/* Transmit Interrupt */

#define PMT_ByteMask0	(0xffff)
#define PMT_Cmd0		(1)
#define PMT_OffSet0		(0x30)

#define RGSMIIIM_DIS_INT	(0x01)

//#define rgmii
//#define big_endian

//#define DDR_ADDR
//#define DDR_ADDR

#ifdef DDR_ADDR
	#define ADDR_START_LOW		0x1000000 //0x100000
	#define ADDR_START_HIGH		0x0000000
#else
	#define ADDR_START_LOW		0xe0070000
	#define ADDR_START_HIGH		0x00000004
#endif
#endif

