/*
 * xHCI host controller driver
 *
 * Copyright © 2008-2010 Intel Corporation. All rights reserved.
 *
 * Author: Intel
 * Some code borrowed from the Linux EHCI driver.
 *
 */
#ifndef __DRIVERS_XHCI_HCD_H
#define __DRIVERS_XHCI_HCD_H

#include "common_usb.h"
#include "usb3_0.h"


/* Max number of USB devices for any host controller - limit in section 6.1 */
#define MAX_HC_SLOTS		4 //0x10	//256
/* Section 5.3.3 - MaxPorts */
#define MAX_HC_PORTS		2	//127

//	Sec 5. xHCI register interface ******************************************************************************************
//********************************************************************************************************************

/**		0x00+
 * xHCI Host Controller Capability Registers.
 */
struct xhci_cap_regs {
	UINT32	hc_capbase;		//length of the capabilities register and HC version number
	UINT32	hcs_params1;	//Structural Parameters 1
	UINT32	hcs_params2;
	UINT32	hcs_params3;

	UINT32	hcc_params;		//Capability Parameters
	UINT32	db_off;			//Doorbell Offset Register   	 0x480
	UINT32	run_regs_off;	//Runtime Register Space Offset  0x440

	UINT32	hcc_params2;	 /* xhci 1.1 only */
	/* Reserved up to (CAPLENGTH - 0x1C) */
};
/* hc_capbase bitmasks */
	/* bits 7:0 - how long is the Capabilities register */
	#define HC_LENGTH(p)		( ((p)>>00)&0x00ff )
	/* bits 31:16	*/
	#define HC_VERSION(p)		(((p) >> 16) & 0xffff)

/* HCSPARAMS1 - hcs_params1 - bitmasks */
	/* bits 0:7, Max Device Slots */
	#define HCS_MAX_SLOTS(p)	(((p) >> 0) & 0xff)
	#define HCS_SLOTS_MASK		0xff
	/* bits 8:18, Max Interrupters */
	#define HCS_MAX_INTRS(p)	(((p) >> 8) & 0x7ff)
	/* bits 24:31, Max Ports - max value is 0x7F = 127 ports */
	#define HCS_MAX_PORTS(p)	(((p) >> 24) & 0x7f)    //=MaxPorts

/* HCSPARAMS2 - hcs_params2 - bitmasks */
	/* bits 0:3, frames or uframes that SW needs to queue transactions
	 * ahead of the HW to meet periodic deadlines */
	#define HCS_IST(p)		(((p) >> 0) & 0xf)
	/* bits 4:7, max number of Event Ring segments */
	#define HCS_ERST_MAX(p)		(((p) >> 4) & 0xf)
	/* bits 21:25 Hi 5 bits of Scratchpad buffers SW must allocate for the HW */
	/* bit 26 Scratchpad restore - for save/restore HW state - not used yet */
	/* bits 27:31 Lo 5 bits of Scratchpad buffers SW must allocate for the HW */
	#define HCS_MAX_SCRATCHPAD(p)   ((((p) >> 16) & 0x3e0) | (((p) >> 27) & 0x1f))

/* HCSPARAMS3 - hcs_params3 - bitmasks */
	/* bits 0:7, Max U1 to U0 latency for the roothub ports */
	#define HCS_U1_LATENCY(p)	(((p) >> 0) & 0xff)
	/* bits 16:31, Max U2 to U0 latency for the roothub ports */
	#define HCS_U2_LATENCY(p)	(((p) >> 16) & 0xffff)

/* HCCPARAMS - hcc_params - bitmasks */
	/* true: HC can use 64-bit address pointers */
	#define HCC_64BIT_ADDR(p)	((p) & (1 << 0))
	/* true: HC can do bandwidth negotiation */
	#define HCC_BANDWIDTH_NEG(p)	((p) & (1 << 1))
	/* true: HC uses 64-byte Device Context structures
	 *  64-byte context structures aren't supported yet.
	 */
	#define HCC_64BYTE_CONTEXT(p)	((p) & (1 << 2))
	/* true: HC has port power switches */
	#define HCC_PPC(p)		((p) & (1 << 3))
	/* true: HC has port indicators */
	#define HCS_INDICATOR(p)	((p) & (1 << 4))
	/* true: HC has Light HC Reset Capability */
	#define HCC_LIGHT_RESET(p)	((p) & (1 << 5))
	/* true: HC supports latency tolerance messaging */
	#define HCC_LTC(p)		((p) & (1 << 6))
	/* true: no secondary Stream ID Support */
	#define HCC_NSS(p)		((p) & (1 << 7))
	/* true: HC supports Stopped - Short Packet */
	#define HCC_SPC(p)		((p) & (1 << 9))
	/* true: HC has Contiguous Frame ID Capability */
	#define HCC_CFC(p)		((p) & (1 << 11))
	/* Max size for Primary Stream Arrays - 2^(n+1), where n is bits 12:15 */
	#define HCC_MAX_PSA(p)		(1 << ((((p) >> 12) & 0xf) + 1))
	/* Extended Capabilities pointer from PCI base - section 5.3.6 */
	#define HCC_EXT_CAPS(p)		(((p)>>16)&0xffff)

/* db_off bitmask - bits 0:1 reserved */
	#define	DBOFF_MASK	(~0x3)

/* run_regs_off bitmask - bits 0:4 reserved */
	#define	RTSOFF_MASK	(~0x1f)

/* HCCPARAMS2 - hcc_params2 - bitmasks */
	/* true: HC supports U3 entry Capability */
	#define	HCC2_U3C(p)		((p) & (1 << 0))
	/* true: HC supports Configure endpoint command Max exit latency too large */
	#define	HCC2_CMC(p)		((p) & (1 << 1))
	/* true: HC supports Force Save context Capability */
	#define	HCC2_FSC(p)		((p) & (1 << 2))
	/* true: HC supports Compliance Transition Capability */
	#define	HCC2_CTC(p)		((p) & (1 << 3))
	/* true: HC support Large ESIT payload Capability > 48k */
	#define	HCC2_LEC(p)		((p) & (1 << 4))
	/* true: HC support Configuration Information Capability */
	#define	HCC2_CIC(p)		((p) & (1 << 5))
	/* true: HC support Extended TBC Capability, Isoc burst count > 65535 */
	#define	HCC2_ETC(p)		((p) & (1 << 6))




//====================================================================================================================
/**		0x20+
 * Host Controller Operational Registers
 */
#define	NUM_PORT_REGS	4	//Number of registers per port
#define PORTSC			0
#define PORTPMSC		1
#define PORTLI			2
#define PORTHLPMC		3

struct xhci_op_regs {
	UINT32	command;			//xHC command register
	UINT32	status;				//xHC status register
	UINT32	page_size;			//This indicates the page size that the host controller supports
	UINT32	reserved1;
	UINT32	reserved2;

	UINT32	dev_notification; 	//+(14h)
	UINT32	cmd_ring_Lo;		//+(18h) 64-bit Command Ring Pointer/CRCR
	UINT32	cmd_ring_Hi;
	/* rsvd: offset 0x20-2F */
	UINT32	reserved3[4];

	UINT32	dcbaa_ptr_Lo;		//+(30h) 64-bit Device Context Base Address Array Pointer
	UINT32	dcbaa_ptr_Hi;
	UINT32	config_reg;			//+(38h) Configure Register
	/* rsvd: offset 0x3C-3FF */
	UINT32	reserved4[241];

#if 0
	/* port 1 registers, which serve as a base address for other ports */
	UINT32	port_status_base;	//0x420  PORTPMSCn
	UINT32	port_power_base;
	UINT32	port_link_base;
	UINT32	reserved5;
	/* registers for ports 2~n */
//	UINT32	reserved6[NUM_PORT_REGS*254];  //0x430+
	UINT32	reserved6[NUM_PORT_REGS* (MAX_HC_PORTS-1) ];
#else
	UINT32	port1_status_base;	//0x420  PORTPMSCn
	UINT32	port1_power_base;
	UINT32	port1_link_base;
	UINT32	reserved5;

	UINT32	port2_status_base;	//0x440
	UINT32	port2_power_base;
	UINT32	port2_link_base;
	UINT32	reserved6;
#endif
};

/* (USBCMD)USB command - command bitmasks */
	/* start/stop HC execution - do not write unless HC is halted*/
	#define CMD_RUN		(1 << 0)
	/* Reset HC - resets internal HC state machine and all registers (except
	 * PCI config regs).  HC does NOT drive a USB reset on the downstream ports.
	 * The xHCI driver must reinitialize the xHC after setting this bit.
	 */
	#define CMD_RESET	(1 << 1)
	/* Event Interrupt Enable - a '1' allows interrupts from the host controller */
	#define CMD_EIE		(1 << 2)
	/* Host System Error Interrupt Enable - get out-of-band signal for HC errors */
	#define CMD_HSEIE	(1 << 3)
	/* bits 4:6 are reserved (and should be preserved on writes). */
	/* light reset (port status stays unchanged) - reset completed when this is 0 */
	#define CMD_LRESET	(1 << 7)
	/* host controller save/restore state. */
	#define CMD_CSS		(1 << 8)
	#define CMD_CRS		(1 << 9)
	/* Enable Wrap Event - '1' means xHC generates an event when MFINDEX wraps. */
	#define CMD_EWE		(1 << 10)
	/* MFINDEX power management - '1' means xHC can stop MFINDEX counter if all root
	 * hubs are in U3 (selective suspend), disconnect, disabled, or powered-off.
	 * '0' means the xHC can power it off if all ports are in the disconnect,
	 * disabled, or powered-off state.
	 */
	#define CMD_PM_INDEX	(1 << 11)
	/* bit 14 Extended TBC Enable, changes Isoc TRB fields to support larger TBC */
	#define CMD_ETE		(1 << 14)
	/* bits 15:31 are reserved (and should be preserved on writes). */

/* (IMAN) - Interrupt Management Register */
	#define IMAN_IE		(1 << 1)
	#define IMAN_IP		(1 << 0)

/* (USBSTS) - USB status - status bitmasks */
	/* HC not running - set to 1 when run/stop bit is cleared. */
	#define STS_HALT	(1<<0)
	/* serious error, e.g. PCI parity error.  The HC will clear the run/stop bit. */
	#define STS_FATAL	(1 << 2)
	/* event interrupt - clear this prior to clearing any IP flags in IR set*/
	#define STS_EINT	(1 << 3)
	/* port change detect */
	#define STS_PORT	(1 << 4)
	/* bits 5:7 reserved and zeroed */
	/* save state status - '1' means xHC is saving state */
	#define STS_SAVE	(1 << 8)
	/* restore state status - '1' means xHC is restoring state */
	#define STS_RESTORE	(1 << 9)
	/* true: save or restore error */
	#define STS_SRE		(1 << 10)
	/* true: Controller Not Ready to accept doorbell or op reg writes after reset */
	#define STS_CNR		(1 << 11)
	/* true: internal Host Controller Error - SW needs to reset and reinitialize */
	#define STS_HCE		(1 << 12)
	/* bits 13:31 reserved and should be preserved */

/*(	DNCTRL) - Device Notification Control Register - dev_notification bitmasks
 * Generate a device notification event when the HC sees a transaction with a
 * notification type that matches a bit set in this bit field.
 */
	#define	DEV_NOTE_MASK		(0xffff)
	#define ENABLE_DEV_NOTE(x)	(1 << (x))
	/* Most of the device notification types should only be used for debug.
	 * SW does need to pay attention to function wake notifications.
	 */
	#define	DEV_NOTE_FWAKE		ENABLE_DEV_NOTE(1)

/* (CRCR) - Command Ring Control Register - cmd_ring bitmasks */
	/* bit 0 is the command ring cycle state */
	/* stop ring operation after completion of the currently executing command */
	#define CMD_RING_PAUSE		(1 << 1)
	/* stop ring immediately - abort the currently executing command */
	#define CMD_RING_ABORT		(1 << 2)
	/* true: command ring is running */
	#define CMD_RING_RUNNING	(1 << 3)
	/* bits 4:5 reserved and should be preserved */
	/* Command Ring pointer - bit mask for the lower 32 bits. */
	#define CMD_RING_RSVD_BITS	(0x3f)

/* (CONFIG) - Configure Register - config_reg bitmasks */
	/* bits 0:7 - maximum number of device slots enabled (NumSlotsEn) */
	#define MAX_DEVS(p)	((p) & 0xff)
	/* bit 8: U3 Entry Enabled, assert PLC when root port enters U3, xhci 1.1 */
	#define CONFIG_U3E		(1 << 8)
	/* bit 9: Configuration Information Enable, xhci 1.1 */
	#define CONFIG_CIE		(1 << 9)
	/* bits 10:31 - reserved and should be preserved */

#if 1
/* (PORTSC) - Port Status and Control Register - port_status_base bitmasks */
	/* true: device connected */
	#define PORT_CONNECT	(1 << 0)
	/* true: port enabled */
	#define PORT_PE		(1 << 1)
	/* bit 2 reserved and zeroed */
	/* true: port has an over-current condition */
	#define PORT_OC		(1 << 3)
	/* true: port reset signaling asserted */
	#define PORT_RESET	(1 << 4)

	/* Port Link State - bits 5:8
	 * A read gives the current link PM state of the port,
	 * a write with Link State Write Strobe set sets the link state.
	 */
	#define PORT_PLS_MASK	(0xf << 5)
	#define XDEV_U0			(0x0 << 5)
	#define XDEV_U1			(0x1 << 5)
	#define XDEV_U2			(0x2 << 5)
	#define XDEV_U3			(0x3 << 5)
	#define XDEV_DISABLED	(0x4 << 5)
	#define XDEV_RXDETECT	(0x5 << 5)
	#define XDEV_INACTIVE	(0x6 << 5)
	#define XDEV_POLLING	(0x7 << 5)
	#define XDEV_RECOVERY	(0x8 << 5)
	#define XDEV_HOT_RESET	(0x9 << 5)
	#define XDEV_COMP_MODE	(0xa << 5)
	#define XDEV_TEST_MODE	(0xb << 5)
	#define XDEV_RESUME		(0xf << 5)

	/* true: port has power (see HCC_PPC) */
	#define PORT_POWER	(1 << 9)

	/* bits 10:13 indicate device speed:
	 * 0 - undefined speed - port hasn't be initialized by a reset yet
	 * 1 - full speed
	 * 2 - low speed
	 * 3 - high speed
	 * 4 - super speed
	 * 5-15 reserved
	*/
	#define DEV_SPEED_MASK		(0xf << 10)
	#define	XDEV_FS				(0x1 << 10)
	#define	XDEV_LS				(0x2 << 10)
	#define	XDEV_HS				(0x3 << 10)
	#define	XDEV_SS				(0x4 << 10)
	#define	XDEV_SSP			(0x5 << 10)
	#define DEV_UNDEFSPEED(p)		(((p) & DEV_SPEED_MASK) == (0x0<<10))
	#define DEV_FULLSPEED(p)		(((p) & DEV_SPEED_MASK) == XDEV_FS)
	#define DEV_LOWSPEED(p)			(((p) & DEV_SPEED_MASK) == XDEV_LS)
	#define DEV_HIGHSPEED(p)		(((p) & DEV_SPEED_MASK) == XDEV_HS)
	#define DEV_SUPERSPEED(p)		(((p) & DEV_SPEED_MASK) == XDEV_SS)

	#define DEV_SUPERSPEEDPLUS(p)	(((p) & DEV_SPEED_MASK) == XDEV_SSP)
	#define DEV_SUPERSPEED_ANY(p)	(((p) & DEV_SPEED_MASK) >= XDEV_SS)
	#define DEV_PORT_SPEED(p)		(((p) >> 10) & 0x0f)
  #if 1
	/* Slot Context[Bits 20:23]
		in the Slot Context are the speed for the device */
	#define	SLOT_SPEED_FS		(XDEV_FS << 10)
	#define	SLOT_SPEED_LS		(XDEV_LS << 10)
	#define	SLOT_SPEED_HS		(XDEV_HS << 10)
	#define	SLOT_SPEED_SS		(XDEV_SS << 10)
	#define	SLOT_SPEED_SSP		(XDEV_SSP << 10)
  #endif
	/* Port Indicator Control */
	#define PORT_LED_OFF	(0 << 14)
	#define PORT_LED_AMBER	(1 << 14)
	#define PORT_LED_GREEN	(2 << 14)
	#define PORT_LED_MASK	(3 << 14)
	/* Port Link State Write Strobe - set this when changing link state */
	#define PORT_LINK_STROBE	(1 << 16)
	/* true: connect status change */
	#define PORT_CSC	(1 << 17)
	/* true: port enable change */
	#define PORT_PEC	(1 << 18)
	/* true: warm reset for a USB 3.0 device is done.  A "hot" reset puts the port
	 * into an enabled state, and the device into the default state.  A "warm" reset
	 * also resets the link, forcing the device through the link training sequence.
	 * SW can also look at the Port Reset register to see when warm reset is done.
	 */
	#define PORT_WRC	(1 << 19)
	/* true: over-current change */
	#define PORT_OCC	(1 << 20)
	/* true: reset change - 1 to 0 transition of PORT_RESET */
	#define PORT_RC		(1 << 21)

	/* port link status change
				- set on some port link state transitions:
	 *  Transition				Reason
	 *  ------------------------------------------------------------------------------
	 *  - U3 to Resume			Wakeup signaling from a device
	 *  - Resume to Recovery to U0		USB 3.0 device resume
	 *  - Resume to U0			USB 2.0 device resume
	 *  - U3 to Recovery to U0		Software resume of USB 3.0 device complete
	 *  - U3 to U0				Software resume of USB 2.0 device complete
	 *  - U2 to U0				L1 resume of USB 2.1 device complete
	 *  - U0 to U0 (???)			L1 entry rejection by USB 2.1 device
	 *  - U0 to disabled			L1 entry error with USB 2.1 device
	 *  - Any state to inactive		Error on USB 3.0 port  */
	#define PORT_PLC	(1 << 22)

	/* port configure error change - port failed to configure its link partner */
	#define PORT_CEC	(1 << 23)
	/* Cold Attach Status - xHC can set this bit to report device attached during
	 * Sx state. Warm port reset should be perfomed to clear this bit and move port
	 * to connected state.
	 */
	#define PORT_CAS		(1 << 24)
	/* wake on connect (enable) */
	#define PORT_WKCONN_E	(1 << 25)
	/* wake on disconnect (enable) */
	#define PORT_WKDISC_E	(1 << 26)
	/* wake on over-current (enable) */
	#define PORT_WKOC_E		(1 << 27)
	/* bits 28:29 reserved */
	/* true: device is non-removable - for USB 3.0 roothub emulation */
	#define PORT_DEV_REMOVE	(1 << 30)
	/* Initiate a warm port reset - complete when PORT_WRC is '1' */
	#define PORT_WR			(1 << 31)


/*
 * These bits are Read Only (RO) and should be saved and written to the
 * registers: 0, 3, 10:13, 30
 * connect status, over-current status, port speed, and device removable.
 * connect status and port speed are also sticky - meaning they're in
 * the AUX well and they aren't changed by a hot, warm, or cold reset.
 */
#define	XHCI_PORT_RO	((1<<0) | (1<<3) | (0xf<<10) | (1<<30))
/*
 * These bits are RW; writing a 0 clears the bit, writing a 1 sets the bit:
 * bits 5:8, 9, 14:15, 25:27
 * link state, port power, port indicator state, "wake on" enable state
 */
#define XHCI_PORT_RWS	((0xf<<5) | (1<<9) | (0x3<<14) | (0x7<<25))
/*
 * These bits are RW; writing a 1 sets the bit, writing a 0 has no effect:
 * bit 4 (port reset)
 */
#define	XHCI_PORT_RW1S	((1<<4))
/*
 * These bits are RW; writing a 1 clears the bit, writing a 0 has no effect:
 * bits 1, 17, 18, 19, 20, 21, 22, 23
 * port enable/disable, and
 * change bits: connect, PED, warm port reset changed (reserved zero for USB 2.0 ports),
 * over-current, reset, link state, and L1 change
 */
#define XHCI_PORT_RW1CS	((1<<1) | (0x7f<<17))
/*
 * Bit 16 is RW, and writing a '1' to it causes the link state control to be
 * latched in
 */
#define	XHCI_PORT_RW	((1<<16))
/*
 * These bits are Reserved Zero (RsvdZ) and zero should be written to them:
 * bits 2, 24, 28:31
 */
#define	XHCI_PORT_RZ	((1<<2) | (1<<24) | (0xf<<28))


#endif //PORTSC


//====================================================================================================================
/**		Interrupter Register Set
 * 		Event Ring Registers
 */
struct xhci_intr_reg {
	UINT32	irq_pending;	//IMAN	Interrupt Management Register
	UINT32	irq_control;	//IMOD	Interrupt Moderation Register

	UINT32	erst_size;		//ERSTSZ	Number of segments in the Event Ring Segment Table (ERST)
	UINT32	rsvd;
	UINT32	erst_base_Lo;	//ERSTBA	ERST base address
	UINT32	erst_base_Hi;
	UINT32	erst_dequeue_Lo;//ERDP		Event ring dequeue pointer
	UINT32	erst_dequeue_Hi;
};
/* irq_pending bitmasks */
	 #define	ER_IRQ_PENDING(p)	((p) & 0x1)
	 /* bits 2:31 need to be preserved */
	 /* THIS IS BUGGY - FIXME - IP IS WRITE 1 TO CLEAR */
	 #define	ER_IRQ_CLEAR(p)		((p) & 0xfffffffe)
	 #define	ER_IRQ_ENABLE(p)	((ER_IRQ_CLEAR(p)) | 0x2)
	 #define	ER_IRQ_DISABLE(p)	((ER_IRQ_CLEAR(p)) & ~(0x2))

/* irq_control bitmasks */
	 /* Minimum interval between interrupts (in 250ns intervals).  The interval
	  * between interrupts will be longer if there are no events on the event ring.
	  * Default is 4000 (1 ms).
	  */
	 #define ER_IRQ_INTERVAL_MASK	(0xffff)
	 /* Counter used to count down the time to the next interrupt - HW use only */
	 #define ER_IRQ_COUNTER_MASK	(0xffff << 16)

/* erst_size bitmasks */
	 /* Preserve bits 16:31 of erst_size */
	 #define	ERST_SIZE_MASK		(0xffff << 16)

/* erst_dequeue bitmasks */
	 /* Dequeue ERST Segment Index (DESI) - Segment number (or alias)
	  * where the current dequeue pointer lies.  This is an optional HW hint.
	  */
	 #define ERST_DESI_MASK		(0x7)
	 /* Event Handler Busy (EHB) - is the event ring scheduled to be serviced by
	  * a work queue (or delayed service routine)?
	  		//This flag shall be set to '1' (by xhci) when the IP bit is set to '1'
	  */
	 #define ERST_EHB		(1 << 3) //cleared to '0' by software when the Dequeue Pointer register is written
	 #define ERST_PTR_MASK	(0xf)


/**	0x440+
 * Host Controller Runtime Registers	(32 bit)
 */
struct xhci_run_regs {
	UINT32			microframe_index;  //current microframe number
	UINT32			rsvd[7];

	/* Our HCD's current interrupter register set */
//	struct xhci_intr_reg	ir_set[128];
	struct xhci_intr_reg	ir_set[1];
};


/**	0x480+
 * Doorbell Registers
 *
 * Bits  0 -  7: Endpoint target
 * Bits  8 - 15: RsvdZ
 * Bits 16 - 31: Stream ID
 */
struct xhci_doorbell_array {	//array,all
//	UINT32	doorbell[256];
	UINT32	doorbell[MAX_HC_SLOTS];
};
#define DB_VALUE(ep, stream)	( (((ep) + 1) & 0xff) | ((stream) << 16) )  // DB Target | stream
#define DB_VALUE_HOST			0x00000000


//====================================================================================================================
/*
 * xHCI Extended Capability Pointer Register
 * Bits  0 -  7: Capability ID
 * Bits  8 - 15: Next xHCI Extended Capability Pointer
 * Bits 16 - 31: Capability Specific
 */
//see spec

/* Extended capability register bit fields */
	#define XHCI_EXT_CAPS_ID(p)		(((p)>>0)&0xff)
	#define XHCI_EXT_CAPS_NEXT(p)	(((p)>>8)&0xff)
	#define	XHCI_EXT_CAPS_VAL(p)	((p)>>16)

	/* Extended capability IDs - ID 0 reserved */
	#define XHCI_EXT_CAPS_LEGACY	1
	#define XHCI_EXT_CAPS_PROTOCOL	2
	#define XHCI_EXT_CAPS_PM		3
	#define XHCI_EXT_CAPS_VIRT		4
	#define XHCI_EXT_CAPS_ROUTE		5


/* USB Legacy Support Capability
 * */
//see spec

/**
 * xHCI Supported Protocol Capability
 * @revision:		major revision, minor revision, capability ID,
 *			and next capability pointer.
 * @name_string:	Four ASCII characters to say which spec this xHC
 *			follows, typically "USB ".
 * @port_info:		Port offset, count, and protocol-defined information.
 */
struct xhci_protocol_caps {
	UINT32	revision;
	#define	XHCI_EXT_PORT_MAJOR(x)	(((x) >> 24) & 0xff)
	#define	XHCI_EXT_PORT_MINOR(x)	(((x) >> 16) & 0xff)

	UINT32	name_string;

	UINT32	port_info;
	#define	XHCI_EXT_PORT_OFF(x)	((x) & 0xff)
	#define	XHCI_EXT_PORT_COUNT(x)	(((x) >> 8) & 0xff)
	#define	XHCI_EXT_PORT_PSIC(x)	(((x) >> 28) & 0x0f)
};






// Sec 6. Data structures ****************************************************************************************************
//********************************************************************************************************************

//def xx_ctx==================================================================

#define CTX_SIZE(_hcc) (HCC_64BYTE_CONTEXT(_hcc) ? 64 : 32)

/** Sec6.1 Device Context Base Address Array
 *
 * 	Element 0/ Element 1-n
 */
struct xhci_device_context_Entry {    //one Element

	/* 64-bit device addresses; we only write 32-bit addresses */
//	__le64		dev_context_ptrs[MAX_HC_SLOTS];
	UINT32	dev_context_ptrs_LO;
	UINT32	dev_context_ptrs_Hi;

	/* private xHCD pointers */
//	dma_addr_t	dma;
};
//struct xhci_device_context_array {
//	struct xhci_device_context_Entry*	dcbaa_entry[MAX_HC_SLOTS];
//};




/**		Sec6.2.1 Device Context + Sec6.2.5 Input Context
 *
 * @type: Type of context.  Used to calculated offsets to contained contexts.
 * @size: Size of the context data
 * @bytes: The raw context data given to HW
 * @dma: dma address of the bytes
 *
 * Represents either a Device or Input context.  Holds a pointer to the raw
 * memory used for the context (bytes) and dma address of it (dma).
 */
struct xhci_container_ctx {	//inlcude struct xhci_slot_ctx + struct xhci_ep_ctx  + struct xhci_input_control_ctx
	unsigned type;

	int size; 	//one device/input CTX size(bytes) 2048/2112s

	UINT8 *bytes;	// The raw context data????

//	dma_addr_t dma;
//	UINT32 Ctx_Base_Lo;
};
/* type bits*/
	#define XHCI_CTX_TYPE_DEVICE  0x1
	#define XHCI_CTX_TYPE_INPUT   0x2


/**	Sec 6.2.2 Slot Context
 *
 * @dev_info:	Route string, device speed, hub info, and last valid endpoint
 * @dev_info2:	Max exit latency for device number, root hub port number
 * @tt_info:	tt_info is used to construct split transaction tokens
 * @dev_state:	slot state and device address
 *
 * This assumes the HC uses 32-byte context structures.
 * If the HC uses 64-byte contexts, there is an additional 32 bytes
 * reserved at the end of the slot context for HC internal use.
 */
struct xhci_slot_ctx {
	UINT32	dev_info;
	UINT32	dev_info2;

	UINT32	tt_info;
	UINT32	dev_state;

	/* offset 0x10 to 0x1f reserved for HC internal use */
	UINT32	reserved[4];
};
/* dev_info bitmasks */
	/* Route String - 0:19 */
	#define ROUTE_STRING_MASK	(0xfffff)
	/* Device speed - values defined by PORTSC Device Speed field - 20:23 */
	#define DEV_SPEED		  	(0xf << 20)
	#define GET_DEV_SPEED(n) 	(((n) & DEV_SPEED) >> 20)
	/* bit 24 reserved */

	/* Is this LS/FS device connected through a HS hub? - bit 25 */
	#define DEV_MTT			(0x1 << 25)
	/* Set if the device is a hub - bit 26 */
	#define DEV_HUB			(0x1 << 26)
	/* Index of the last valid endpoint context in this device context - 27:31 */
	#define LAST_CTX_MASK	(0x1f << 27)
	#define LAST_CTX(p)		((p) << 27)
	#define LAST_CTX_TO_EP_NUM(p)	(((p) >> 27) - 1)

	#define SLOT_FLAG		(1 << 0)
	#define EP0_FLAG		(1 << 1)

/* dev_info2 bitmasks */
	/* Max Exit Latency (ms) - worst case time to wake up all links in dev path */
	#define MAX_EXIT			(0xffff)
	/* Root hub port number that is needed to access the USB device */
	#define ROOT_HUB_PORT(p)			(((p) & 0xff) << 16)
	#define DEVINFO_TO_ROOT_HUB_PORT(p)	(((p) >> 16) & 0xff)
	/* Maximum number of ports under a hub device */
	#define XHCI_MAX_PORTS(p)			(((p) & 0xff) << 24)
	#define DEVINFO_TO_MAX_PORTS(p)		(((p) & (0xff << 24)) >> 24)

/* tt_info bitmasks */
	/*
	* TT Hub Slot ID - for low or full speed devices attached to a high-speed hub
	* The Slot ID of the hub that isolates the high speed signaling from
	* this low or full-speed device.  '0' if attached to root hub port.
	*/
	#define TT_SLOT		(0xff)
	/*
	* The number of the downstream facing port of the high-speed hub
	* '0' if the device is not low or full speed.
	*/
	#define TT_PORT					(0xff << 8)
	#define TT_THINK_TIME(p)		(((p) & 0x3) << 16)
	#define GET_TT_THINK_TIME(p)	(((p) & (0x3 << 16)) >> 16)

/* dev_state bitmasks */
	/* USB device address - assigned by the HC */
	#define DEV_ADDR_MASK		(0xff)
	/* bits 8:26 reserved */
	/* Slot state */
	#define SLOT_STATE			(0x1f << 27)
	#define GET_SLOT_STATE(p)	(((p) & (0x1f << 27)) >> 27)
	/* 4.5.3.1 Device Slot State Codes    Slot Context[31:27 Slot State.]*/
	#define SLOT_STATE_DISABLED		0
	#define SLOT_STATE_ENABLED		SLOT_STATE_DISABLED
	#define SLOT_STATE_DEFAULT		1
	#define SLOT_STATE_ADDRESSED	2
	#define SLOT_STATE_CONFIGURED	3




/**	Sec6.2.3 Endpoint Context
 *
 * @ep_info:	endpoint state, streams, mult, and interval information.
 * @ep_info2:	information on endpoint type, max packet size, max burst size,
 * 		error count, and whether the HC will force an event for all
 * 		transactions.
 * @deq:	64-bit ring dequeue pointer address.  If the endpoint only
 * 		defines one stream, this points to the endpoint transfer ring.
 * 		Otherwise, it points to a stream context array, which has a
 * 		ring pointer for each flow.
 * @tx_info:
 * 		Average TRB lengths for the endpoint ring and
 * 		max payload within an Endpoint Service Interval Time (ESIT).
 *
 * Endpoint Context, This assumes the HC uses 32-byte context structures.
 * If the HC uses 64-byte contexts, there is an additional 32 bytes
 * reserved at the end of the endpoint context for HC internal use.
 */
struct xhci_ep_ctx {
	UINT32	ep_info;
	UINT32	ep_info2;

	UINT32	TR_deq_Lo;
	UINT32	TR_deq_Hi;

	UINT32	tx_info;

	/* offset 0x14 - 0x1f reserved for HC internal use */
	UINT32	reserved[3];
};

/* ep_info bitmasks */
	/*
	* Endpoint State - bits 0:2
	* 0 - disabled
	* 1 - running
	* 2 - halted due to halt condition - ok to manipulate endpoint ring
	* 3 - stopped
	* 4 - TRB error
	* 5-7 - reserved
	*/
	#define EP_STATE_MASK		(0xf)
	#define EP_STATE_DISABLED	0
	#define EP_STATE_RUNNING	1
	#define EP_STATE_HALTED		2
	#define EP_STATE_STOPPED	3
	#define EP_STATE_ERROR		4
	#define GET_EP_CTX_STATE(ctx)	(le32_to_cpu((ctx)->ep_info) & EP_STATE_MASK)

	/* Mult - Max number of burtst within an interval, in EP companion desc. */
	#define EP_MULT(p)			(((p) & 0x3) << 8)
	#define CTX_TO_EP_MULT(p)	(((p) >> 8) & 0x3)
	/* bits 10:14 are Max Primary Streams */
	/* bit 15 is Linear Stream Array */
	/* Interval - period between requests to an endpoint - 125u increments. */
	#define EP_INTERVAL(p)				(((p) & 0xff) << 16)
	#define EP_INTERVAL_TO_UFRAMES(p)	(1 << (((p) >> 16) & 0xff))
	#define CTX_TO_EP_INTERVAL(p)		(((p) >> 16) & 0xff)
	#define EP_MAXPSTREAMS_MASK			(0x1f << 10)
	#define EP_MAXPSTREAMS(p)			(((p) << 10) & EP_MAXPSTREAMS_MASK)
	#define CTX_TO_EP_MAXPSTREAMS(p)	(((p) & EP_MAXPSTREAMS_MASK) >> 10)
	/* Endpoint is set up with a Linear Stream Array (vs. Secondary Stream Array) */
	#define	EP_HAS_LSA						(1 << 15)
	/* hosts with LEC=1 use bits 31:24 as ESIT high bits. */
	#define CTX_TO_MAX_ESIT_PAYLOAD_HI(p)	(((p) >> 24) & 0xff)

/* ep_info2 bitmasks */
	/*
	* Force Event - generate transfer events for all TRBs for this endpoint
	* This will tell the HC to ignore the IOC and ISP flags (for debugging only).
	*/
	#define	FORCE_EVENT		(0x1)
	#define ERROR_COUNT(p)	(((p) & 0x3) << 1)
	#define CTX_TO_EP_TYPE(p)	(((p) >> 3) & 0x7)
	#define EP_TYPE(p)		((p) << 3)
	#define ISOC_OUT_EP		1
	#define BULK_OUT_EP		2
	#define INT_OUT_EP		3
	#define CTRL_EP			4
	#define ISOC_IN_EP		5
	#define BULK_IN_EP		6
	#define INT_IN_EP		7
	/* bit 6 reserved */
	/* bit 7 is Host Initiate Disable - for disabling stream selection */
	#define MAX_BURST(p)		(((p)&0xff) << 8)
	#define CTX_TO_MAX_BURST(p)	(((p) >> 8) & 0xff)
	#define MAX_PACKET(p)		(((p)&0xffff) << 16)
	#define MAX_PACKET_MASK		(0xffff << 16)
	#define MAX_PACKET_DECODED(p)	(((p) >> 16) & 0xffff)

/* tx_info bitmasks */
	#define EP_AVG_TRB_LENGTH(p)		((p) & 0xffff)
	#define EP_MAX_ESIT_PAYLOAD_LO(p)	(((p) & 0xffff) << 16)
	#define EP_MAX_ESIT_PAYLOAD_HI(p)	((((p) >> 16) & 0xff) << 24)
	#define CTX_TO_MAX_ESIT_PAYLOAD(p)	(((p) >> 16) & 0xffff)

/* deq bitmasks */
	#define EP_CTX_CYCLE_MASK		(1 << 0) //DCS
	#define SCTX_DEQ_MASK			(~0xfL)


/**6.2.5.1 Input Control Context
 *
 * @drop_context:	set the bit of the endpoint context you want to disable
 * @add_context:	set the bit of the endpoint context you want to enable
 */
struct xhci_input_control_ctx {
	UINT32	drop_flags;
	UINT32	add_flags;
	UINT32	rsvd2[6];
};
/* bit */
	#define	EP_IS_ADDED(ctrl_ctx, i) \
		(le32_to_cpu(ctrl_ctx->add_flags) & (1 << (i + 1)))
	#define	EP_IS_DROPPED(ctrl_ctx, i)       \
		(le32_to_cpu(ctrl_ctx->drop_flags) & (1 << (i + 1)))

//end xx_ctx==================================================================




//============================================================================
/* Sec 6.4 Transfer Request Block (TRB) */
struct xhci_generic_trb {
	UINT32 field[4];
};

//============================================================================
/* Sec6.4.1 Transfer TRBs */

/* Sec6.4.1.1 Normal TRB fields */
	/* transfer_len bitmasks - bits 0:16 */
	#define	TRB_LEN(p)			((p) & 0x1ffff)
	/* TD Size, packets remaining in this TD, bits 21:17 (5 bits, so max 31) */
	#define TRB_TD_SIZE(p)      (min((p), (u32)31) << 17)
	#define GET_TD_SIZE(p)		(((p) & 0x3e0000) >> 17)
	/* xhci 1.1 uses the TD_SIZE field for TBC if Extended TBC is enabled (ETE) */
	#define TRB_TD_SIZE_TBC(p)   (min((p), (u32)31) << 17)
	/* Interrupter Target - which MSI-X vector to target the completion event at */
	#define TRB_INTR_TARGET(p)	(((p) & 0x3ff) << 22)
	#define GET_INTR_TARGET(p)	(((p) >> 22) & 0x3ff)
	/* Total burst count field, Rsvdz on xhci 1.1 with Extended TBC enabled (ETE) */
	#define TRB_TBC(p)			(((p) & 0x3) << 7)
	#define TRB_TLBPC(p)		(((p) & 0xf) << 16)

	/* Cycle bit - indicates TRB ownership by HC or HCD */
	#define TRB_CYCLE		(1<<0)
	/*
	 * Force next event data TRB to be evaluated before task switch.
	 * Used to pass OS data back after a TD completes.
	 */
	#define TRB_ENT			(1<<1)
	/* Interrupt on short packet */
	#define TRB_ISP			(1<<2)
	/* Set PCIe no snoop attribute */
	#define TRB_NO_SNOOP	(1<<3)
	/* Chain multiple TRBs into a TD */
	#define TRB_CHAIN		(1<<4)
	/* Interrupt on completion */
	#define TRB_IOC			(1<<5)
	/* The buffer pointer contains immediate data */
	#define TRB_IDT			(1<<6)

	/* Block Event Interrupt */
	#define	TRB_BEI			(1<<9)

/* Sec6.4.1.2 Control transfer TRB specific fields */
	/* [17:16] Transfer Type (TRT). */
	#define TRB_DIR_IN		(1<<16)
	#define	TRB_TX_TYPE(p)	((p) << 16)
	#define	TRB_DATA_OUT	2
	#define	TRB_DATA_IN		3

/* Sec6.4.1.3  Isochronous TRB specific fields */
	#define TRB_SIA				(1<<31)
	#define TRB_FRAME_ID(p)		(((p) & 0x7ff) << 20)


//============================================================================
/* Sec6.4.2 Event TRBs */
	/* bits 24:31 are the slot ID */
	#define TRB_TO_SLOT_ID(p)		(((p) & (0xff<<24)) >> 24)
	#define SLOT_ID_FOR_TRB(p)		(((p) & 0xff) << 24)


/* Sec6.4.2.1 Transfer Event TRB */
struct xhci_transfer_event {
	/* 64-bit buffer address, or immediate data */
	UINT32	buffer_Lo;	//TRB Pointer
	UINT32	buffer_Hi;

	UINT32	transfer_len;

	/* This field is interpreted differently based on the type of TRB */
	UINT32	flags;
};
/* Transfer event TRB length bit mask */
	/* bits 0:23 */
	#define	EVENT_TRB_LEN(p)	((p) & 0xffffff)

	/** Transfer Event bit fields **/
	#define	TRB_TO_EP_ID(p)		(((p) >> 16) & 0x1f)
	#define EVENT_DATA			(1 << 2)


/* Sec6.4.2.2 Command completion event TRB */
struct xhci_event_cmd {
	/* Pointer to command TRB, or the value passed by the event data trb */
	UINT32 cmd_trb_ptr_Lo;
	UINT32 cmd_trb_ptr_Hi;

	UINT32 status;
	UINT32 flags;
};
/* flags bitmasks */
	/* Sec6.4.3.4  Address device Command TRB- disable SetAddress */
	#define TRB_BSR		(1<<9)
	enum xhci_setup_dev {
		SETUP_CONTEXT_ONLY,		//BSR=1
		SETUP_CONTEXT_ADDRESS,	//BSR=0 not generate
	};

	/* Sec6.4.3.5  Configure Endpoint - Deconfigure */
	#define TRB_DC		(1<<9)
	/* Sec6.4.3.7  Reset Endpoint/Stop Ring - Transfer State Preserve */
	#define TRB_TSP		(1<<9)
	enum xhci_ep_reset_type {
		EP_HARD_RESET,
		EP_SOFT_RESET,
	};

	/* Sec6.4.3.11  Force Event */
	#define TRB_TO_VF_INTR_TARGET(p)	(((p) & (0x3ff << 22)) >> 22)
	#define TRB_TO_VF_ID(p)				(((p) & (0xff << 16)) >> 16)
	/* Sec6.4.3.12	Set Latency Tolerance Value */
	#define TRB_TO_BELT(p)				(((p) & (0xfff << 16)) >> 16)
	/* Sec6.4.3.14	Get Port Bandwidth */
	#define TRB_TO_DEV_SPEED(p)			(((p) & (0xf << 16)) >> 16)
	/* Sec6.4.3.15	Force Header */
	#define TRB_TO_PACKET_TYPE(p)		((p) & 0x1f)
	#define TRB_TO_ROOTHUB_PORT(p)		(((p) & (0xff << 24)) >> 24)
	/* Sec6.4.3.8  Stop Endpoint TRB - ep_index to endpoint ID for this TRB */
	#define TRB_TO_EP_INDEX(p)			((((p) & (0x1f << 16)) >> 16) - 1)
	#define	EP_ID_FOR_TRB(p)			((((p) + 1) & 0x1f) << 16)

	#define SUSPEND_PORT_FOR_TRB(p)		(((p) & 1) << 23)
	#define TRB_TO_SUSPEND_PORT(p)		(((p) & (1 << 23)) >> 23)
	#define LAST_EP_INDEX				30

	/* Sec6.4.3.9  Set TR Dequeue Pointer command TRB fields, */
	#define TRB_TO_STREAM_ID(p)			((((p) & (0xffff << 16)) >> 16))
	#define STREAM_ID_FOR_TRB(p)		((((p)) & 0xffff) << 16)
	#define SCT_FOR_TRB(p)				(((p) << 1) & 0x7)


/* Sec6.4.2.3 Port Status Change Event TRB fields */
	/* Port ID - bits 31:24 */
	#define GET_PORT_ID(p)		(((p) & (0xff << 24)) >> 24)



//============================================================================
/* Sec 6.4.4.1 Link TRB */
struct xhci_link_trb {
	/* 64-bit segment pointer*/
	UINT32 segment_ptr_Lo;
	UINT32 segment_ptr_Hi;

	UINT32 intr_target;

	UINT32 control;
};
/* control bitfields */
	#define LINK_TOGGLE		(0x1<<1)
	#define TRB_TC			(1<<1)


	#define TRB_TYPE_LINK(x)	(((x) & TRB_TYPE_BITMASK) == TRB_TYPE(TRB_LINK))
	/* Above, but for __le32 types -- can avoid work by swapping constants: */
	#define TRB_TYPE_LINK_LE32(x)	(((x) & cpu_to_le32(TRB_TYPE_BITMASK)) == \
				 cpu_to_le32(TRB_TYPE(TRB_LINK)))
	#define TRB_TYPE_NOOP_LE32(x)	(((x) & cpu_to_le32(TRB_TYPE_BITMASK)) == \
				 cpu_to_le32(TRB_TYPE(TRB_TR_NOOP)))

	#define NEC_FW_MINOR(p)		(((p) >> 0) & 0xff)
	#define NEC_FW_MAJOR(p)		(((p) >> 8) & 0xff)


//============================================================================
union xhci_trb {
	struct xhci_link_trb		link;

//Sec6.4.2 Event TRBs
	struct xhci_transfer_event	trans_event; //Sec6.4.2.1
	struct xhci_event_cmd		event_cmd;	 //Sec6.4.2.2

//Sec6.4 Transfer Request Block (TRB)
	struct xhci_generic_trb		generic;
};


//============================================================================
/* Sec 6.4.5 Completion Code
 * - only applicable for some types of (event) TRBs
 */
#define	COMP_CODE_MASK		(0xff << 24)
#define GET_COMP_CODE(p)	(((p) & COMP_CODE_MASK) >> 24)

#define COMP_INVALID					0
#define COMP_SUCCESS					1
#define COMP_DATA_BUFFER_ERROR			2
#define COMP_BABBLE_DETECTED_ERROR		3
#define COMP_USB_TRANSACTION_ERROR		4
#define COMP_TRB_ERROR					5
#define COMP_STALL_ERROR				6
#define COMP_RESOURCE_ERROR				7
#define COMP_BANDWIDTH_ERROR				8
#define COMP_NO_SLOTS_AVAILABLE_ERROR		9
#define COMP_INVALID_STREAM_TYPE_ERROR		10
#define COMP_SLOT_NOT_ENABLED_ERROR			11
#define COMP_ENDPOINT_NOT_ENABLED_ERROR		12
#define COMP_SHORT_PACKET					13
#define COMP_RING_UNDERRUN					14
#define COMP_RING_OVERRUN					15
#define COMP_VF_EVENT_RING_FULL_ERROR		16
#define COMP_PARAMETER_ERROR				17
#define COMP_BANDWIDTH_OVERRUN_ERROR		18
#define COMP_CONTEXT_STATE_ERROR			19
#define COMP_NO_PING_RESPONSE_ERROR		20
#define COMP_EVENT_RING_FULL_ERROR		21
#define COMP_INCOMPATIBLE_DEVICE_ERROR	22
#define COMP_MISSED_SERVICE_ERROR		23
#define COMP_COMMAND_RING_STOPPED		24
#define COMP_COMMAND_ABORTED			25
#define COMP_STOPPED					26
#define COMP_STOPPED_LENGTH_INVALID		27
#define COMP_STOPPED_SHORT_PACKET		28
#define COMP_MAX_EXIT_LATENCY_TOO_LARGE_ERROR	29
#define COMP_ISOCH_BUFFER_OVERRUN				31
#define COMP_EVENT_LOST_ERROR					32
#define COMP_UNDEFINED_ERROR					33
#define COMP_INVALID_STREAM_ID_ERROR			34
#define COMP_SECONDARY_BANDWIDTH_ERROR			35
#define COMP_SPLIT_TRANSACTION_ERROR			36

static inline const char *xhci_trb_comp_code_string(UINT8 status)
{
	switch (status) {
	case COMP_INVALID:
		return "Invalid";
	case COMP_SUCCESS:
		return "Success";
	case COMP_DATA_BUFFER_ERROR:
		return "Data Buffer Error";
	case COMP_BABBLE_DETECTED_ERROR:
		return "Babble Detected";
	case COMP_USB_TRANSACTION_ERROR:
		return "USB Transaction Error";
	case COMP_TRB_ERROR:
		return "TRB Error";
	case COMP_STALL_ERROR:
		return "Stall Error";
	case COMP_RESOURCE_ERROR:
		return "Resource Error";
	case COMP_BANDWIDTH_ERROR:
		return "Bandwidth Error";
	case COMP_NO_SLOTS_AVAILABLE_ERROR:
		return "No Slots Available Error";
	case COMP_INVALID_STREAM_TYPE_ERROR:
		return "Invalid Stream Type Error";
	case COMP_SLOT_NOT_ENABLED_ERROR:
		return "Slot Not Enabled Error";
	case COMP_ENDPOINT_NOT_ENABLED_ERROR:
		return "Endpoint Not Enabled Error";
	case COMP_SHORT_PACKET:
		return "Short Packet";
	case COMP_RING_UNDERRUN:
		return "Ring Underrun";
	case COMP_RING_OVERRUN:
		return "Ring Overrun";
	case COMP_VF_EVENT_RING_FULL_ERROR:
		return "VF Event Ring Full Error";
	case COMP_PARAMETER_ERROR:
		return "Parameter Error";
	case COMP_BANDWIDTH_OVERRUN_ERROR:
		return "Bandwidth Overrun Error";
	case COMP_CONTEXT_STATE_ERROR:
		return "Context State Error";
	case COMP_NO_PING_RESPONSE_ERROR:
		return "No Ping Response Error";
	case COMP_EVENT_RING_FULL_ERROR:
		return "Event Ring Full Error";
	case COMP_INCOMPATIBLE_DEVICE_ERROR:
		return "Incompatible Device Error";
	case COMP_MISSED_SERVICE_ERROR:
		return "Missed Service Error";
	case COMP_COMMAND_RING_STOPPED:
		return "Command Ring Stopped";
	case COMP_COMMAND_ABORTED:
		return "Command Aborted";
	case COMP_STOPPED:
		return "Stopped";
	case COMP_STOPPED_LENGTH_INVALID:
		return "Stopped - Length Invalid";
	case COMP_STOPPED_SHORT_PACKET:
		return "Stopped - Short Packet";
	case COMP_MAX_EXIT_LATENCY_TOO_LARGE_ERROR:
		return "Max Exit Latency Too Large Error";
	case COMP_ISOCH_BUFFER_OVERRUN:
		return "Isoch Buffer Overrun";
	case COMP_EVENT_LOST_ERROR:
		return "Event Lost Error";
	case COMP_UNDEFINED_ERROR:
		return "Undefined Error";
	case COMP_INVALID_STREAM_ID_ERROR:
		return "Invalid Stream ID Error";
	case COMP_SECONDARY_BANDWIDTH_ERROR:
		return "Secondary Bandwidth Error";
	case COMP_SPLIT_TRANSACTION_ERROR:
		return "Split Transaction Error";
	default:
		return "Unknown!!";
	}
}
//============================================================================
/*
 * Sec6.4.6 TRB Types IDs
 */

/* TRB bit mask */
#define	TRB_TYPE_BITMASK		(0xfc00)
#define TRB_TYPE(p)				((p) << 10)
#define TRB_FIELD_TO_TYPE(p)	(((p) & TRB_TYPE_BITMASK) >> 10)

/* bulk, interrupt, isoc scatter/gather, and control data stage */
#define TRB_NORMAL		1
/* setup stage for control transfers */
#define TRB_SETUP		2
/* data stage for control transfers */
#define TRB_DATA		3
/* status stage for control transfers */
#define TRB_STATUS		4
/* isoc transfers */
#define TRB_ISOC		5
/* TRB for linking ring segments */
#define TRB_LINK		6
#define TRB_EVENT_DATA	7

/* Transfer Ring No-op (not for the command ring) */
#define TRB_TR_NOOP		8
/* Command TRBs */
/* Enable Slot Command */
#define TRB_ENABLE_SLOT	9	//no Slot ID
/* Disable Slot Command */
#define TRB_DISABLE_SLOT	10
/* Address Device Command */
#define TRB_ADDR_DEV		11	//Input Context Pointer/BSR
/* Configure Endpoint Command */
#define TRB_CONFIG_EP		12	//Input Context Pointer/DC
/* Evaluate Context Command */
#define TRB_EVAL_CONTEXT	13	//Input Context Pointer
/* Reset Endpoint Command */
#define TRB_RESET_EP		14
/* Stop Transfer Ring Command */
#define TRB_STOP_RING		15
/* Set Transfer Ring Dequeue Pointer Command */
#define TRB_SET_DEQ			16
/* Reset Device Command */
#define TRB_RESET_DEV		17
/* Force Event Command (opt) */
#define TRB_FORCE_EVENT		18
/* Negotiate Bandwidth Command (opt) */
#define TRB_NEG_BANDWIDTH	19
/* Set Latency Tolerance Value Command (opt) */
#define TRB_SET_LT			20
/* Get port bandwidth Command */
#define TRB_GET_BW			21
/* Force Header Command - generate a transaction or link management packet */
#define TRB_FORCE_HEADER	22
/* No-op Command - not for transfer rings */
#define TRB_CMD_NOOP		23

/* TRB IDs 24-31 reserved */
/* Event TRBS */
/* Transfer Event */
#define TRB_TRANSFER		32
/* Command Completion Event */
#define TRB_COMPLETION		33
/* Port Status Change Event */
#define TRB_PORT_STATUS		34 //0x22
/* Bandwidth Request Event (opt) */
#define TRB_BANDWIDTH_EVENT	35
/* Doorbell Event (opt) */
#define TRB_DOORBELL		36
/* Host Controller Event */
#define TRB_HC_EVENT		37
/* Device Notification Event - device sent function wake notification */
#define TRB_DEV_NOTE		38
/* MFINDEX Wrap Event - microframe counter wrapped */
#define TRB_MFINDEX_WRAP	39
/* TRB IDs 40-47 reserved, 48-63 is vendor-defined */

/* Nec vendor-specific command completion event. */
#define	TRB_NEC_CMD_COMP	48
/* Get NEC firmware revision. */
#define	TRB_NEC_GET_FW		49

static inline const char *xhci_trb_type_string(UINT8 type)
{
	switch (type) {
	case TRB_NORMAL:
		return "Normal";
	case TRB_SETUP:
		return "Setup Stage";
	case TRB_DATA:
		return "Data Stage";
	case TRB_STATUS:
		return "Status Stage";
	case TRB_ISOC:
		return "Isoch";
	case TRB_LINK:
		return "Link";
	case TRB_EVENT_DATA:
		return "Event Data";
	case TRB_TR_NOOP:
		return "No-Op";
	case TRB_ENABLE_SLOT:
		return "Enable Slot Command";
	case TRB_DISABLE_SLOT:
		return "Disable Slot Command";
	case TRB_ADDR_DEV:
		return "Address Device Command";
	case TRB_CONFIG_EP:
		return "Configure Endpoint Command";
	case TRB_EVAL_CONTEXT:
		return "Evaluate Context Command";
	case TRB_RESET_EP:
		return "Reset Endpoint Command";
	case TRB_STOP_RING:
		return "Stop Ring Command";
	case TRB_SET_DEQ:
		return "Set TR Dequeue Pointer Command";
	case TRB_RESET_DEV:
		return "Reset Device Command";
	case TRB_FORCE_EVENT:
		return "Force Event Command";
	case TRB_NEG_BANDWIDTH:
		return "Negotiate Bandwidth Command";
	case TRB_SET_LT:
		return "Set Latency Tolerance Value Command";
	case TRB_GET_BW:
		return "Get Port Bandwidth Command";
	case TRB_FORCE_HEADER:
		return "Force Header Command";
	case TRB_CMD_NOOP:
		return "No-Op Command";
	case TRB_TRANSFER:
		return "Transfer Event";
	case TRB_COMPLETION:
		return "Command Completion Event";
	case TRB_PORT_STATUS:
		return "Port Status Change Event";
	case TRB_BANDWIDTH_EVENT:
		return "Bandwidth Request Event";
	case TRB_DOORBELL:
		return "Doorbell Event";
	case TRB_HC_EVENT:
		return "Host Controller Event";
	case TRB_DEV_NOTE:
		return "Device Notification Event";
	case TRB_MFINDEX_WRAP:
		return "MFINDEX Wrap Event";
	case TRB_NEC_CMD_COMP:
		return "NEC Command Completion Event";
	case TRB_NEC_GET_FW:
		return "NET Get Firmware Revision Command";
	default:
		return "UNKNOWN";
	}
}


//============================================================================
/*  Sec6.5 Event Ring Segment Table Entry
*/
struct xhci_erst_entry {	//one Entry
	/* 64-bit event ring segment address */
	UINT32	seg_addr_Lo;
	UINT32	seg_addr_Hi;

	UINT32	seg_size;

	/* Set to zero */
	UINT32	rsvd;
};
	/*
	 * Each segment table entry is 4*32bits long.  1K seems like an ok size:
	 * (1K bytes) / (4*32 bits) = 64 segment entries in the table,
	 * meaning 64 ring segments.
	 * Initial allocated size of the ERST, in number of entries */
	#define	ERST_NUM_SEGS	1
	/* Initial allocated size of the ERST, in number of entries */
	#define	ERST_SIZE		64			//64 entries is enough; in fact,only use 1 entry.
	/* Initial number of event segment rings allocated */
	#define	ERST_ENTRIES	1
	/* Figure 108: Event Ring Segment Table Entry */
	#define	ERST_ONE_ENTRY_SIZE		0x10




// Sec4.9 TRB Ring ************************************************************************************************************
//********************************************************************************************************************
/*
 * TRBS_PER_SEGMENT must be a multiple of 4,
 * since the command ring is 64-byte aligned.
 * It must also be greater than 16.
 */
#define TRBS_PER_SEGMENT	256			//256 * TRBs  in one segment		4K
/* Allow two commands + a link TRB, along with any reserved command TRBs */
//#define MAX_RSVD_CMD_TRBS	(TRBS_PER_SEGMENT - 3)
//#define TRB_SEGMENT_SIZE	(TRBS_PER_SEGMENT*16)	//16=one TRB size
//#define TRB_SEGMENT_SHIFT	(ilog2(TRB_SEGMENT_SIZE))
/* TRB buffer pointers can't cross 64KB boundaries */
//#define TRB_MAX_BUFF_SHIFT		16
//#define TRB_MAX_BUFF_SIZE	(1 << TRB_MAX_BUFF_SHIFT)
/* How much data is left before the 64KB boundary? */
//#define TRB_BUFF_LEN_UP_TO_BOUNDARY(addr)	(TRB_MAX_BUFF_SIZE - \
//					(addr & (TRB_MAX_BUFF_SIZE - 1)))

/* Sec4.9.2.1 Segmented Rings */
struct xhci_segment {

	union xhci_trb		*trbs;

	/* private to HCD */
	struct xhci_segment	*next;

//	dma_addr_t		dma;
//	UINT32	seg_addr_Lo;
//	UINT32	seg_addr_Hi;

	/* Max packet sized bounce buffer for td-fragmant alignment */
//	dma_addr_t		bounce_dma;
//	void			*bounce_buf;
//	unsigned int		bounce_offs;
//	unsigned int		bounce_len;
};

struct xhci_dequeue_state {
	struct xhci_segment *new_deq_seg;
	union  xhci_trb *new_deq_ptr;
	int 			new_cycle_state;
	unsigned int 	stream_id;
};

//============================================================================
enum xhci_ring_type {
	// 1 Transfer Ring
	TYPE_CTRL = 0,
	TYPE_ISOC,
	TYPE_BULK,  //same to usb_endpoint_descriptor->bmAttributes
	TYPE_INTR,
	TYPE_STREAM,

	// 2
	TYPE_COMMAND,

	// 3
	TYPE_EVENT,
};

static inline const char *xhci_ring_type_string(enum xhci_ring_type type)
{
	switch (type) {
	case TYPE_CTRL:
		return "CTRL";
	case TYPE_ISOC:
		return "ISOC";
	case TYPE_BULK:
		return "BULK";
	case TYPE_INTR:
		return "INTR";
	case TYPE_STREAM:
		return "STREAM";
	case TYPE_COMMAND:
		return "CMD";
	case TYPE_EVENT:
		return "EVENT";
	}

	return "UNKNOWN";
}

struct xhci_ring {
	struct xhci_segment	*first_seg;			//fixed
	struct xhci_segment	*last_seg;			//

	union  xhci_trb		*enqueue;			//no
	struct xhci_segment	*enq_seg;			//no

	union  xhci_trb		*dequeue;			//------useful/variable
	struct xhci_segment	*deq_seg;

//	struct list_head	td_list;

	/*
	 * Write the cycle state into the TRB cycle field to give ownership of
	 * the TRB to the host controller (if we are the producer), or to check
	 * if we own the TRB (if we are the consumer).  See section 4.9.2.
	 */
	UINT32				cycle_state;

//	unsigned int		stream_id;
	unsigned int		num_segs;			//--

//	unsigned int		num_trbs_free;
//	unsigned int		num_trbs_free_temp;
//	unsigned int		bounce_buf_len;
	enum xhci_ring_type	type;			//--
//	bool				last_td_was_short;
//	struct radix_tree_root	*trb_address_map;
};

//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================




// Buffers offset ****************************************************************************************************
//********************************************************************************************************************
/* buffers offset */
#define XHCI_BUFFERs_BASE		0xE0070000   	//AXI SRAM
#define XHCI_BUFFERs_BASE_HI    0x4


/* Device Context Base Address Array (DCBAA) */
	#define DCBAA_Entries_Base_LO   (XHCI_BUFFERs_BASE + 0x0000)
	//64 slots * 64bit = 512B 0x200

/* Event Ring Segment Table & Entry */
	#define Event_Ring_SegTable_Entries_Base	(XHCI_BUFFERs_BASE + 0x0200)
//	#define Event_Ring_SegTable_Entries_Size	(ERST_SIZE * ERST_ONE_ENTRY_SIZE)	//1K
	#define Event_Ring_SegTable_Entries_Size	(ERST_NUM_SEGS * ERST_ONE_ENTRY_SIZE) //0x200~0x210

//struct
	#define  Event_Ring_Segment_Struct_Base	(Event_Ring_SegTable_Entries_Base + 0x10 ) //struct xhci_segment
	#define  Event_Ring_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x30 ) //struct xhci_ring  0x230~0x27F

	#define  Cmd_Ring_Segment_Struct_Base	(Event_Ring_SegTable_Entries_Base + 0x80 ) //struct xhci_segment
	#define  Cmd_Ring_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x90 ) //struct xhci_ring  0x290~0x2E0

	#define  Out_ctx_Struct_Base		(Event_Ring_SegTable_Entries_Base + 0x100 ) //struct xhci_container_ctx  0x300
	#define  In_ctx_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x110 ) //

	#define  Ep0_Ring_Segment_Struct_Base	(Event_Ring_SegTable_Entries_Base + 0x120 ) //struct xhci_segment
	#define  Ep0_Ring_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x130 ) //struct xhci_ring

//	#define  Ep_in_Ring_Segment_Struct_Base	(Event_Ring_SegTable_Entries_Base + 0x160 ) //struct xhci_segment
//	#define  Ep_in_Ring_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x170 ) //struct xhci_ring

	#define  Ep_in_Ring_Segment_Struct_Base	(Event_Ring_SegTable_Entries_Base + 0x170 ) //struct xhci_segment
	#define  Ep_in_Ring_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x180 ) //struct xhci_ring

//	#define  Ep_out_Ring_Segment_Struct_Base	(Event_Ring_SegTable_Entries_Base + 0x1A0 ) //struct xhci_segment
//	#define  Ep_out_Ring_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x1B0 ) //struct xhci_ring
	#define  Ep_out_Ring_Segment_Struct_Base	(Event_Ring_SegTable_Entries_Base + 0x1E0 ) //struct xhci_segment
	#define  Ep_out_Ring_Struct_Base			(Event_Ring_SegTable_Entries_Base + 0x1F0 ) //struct xhci_ring
//Event Ring Segment(s)
#define Event_Ring_Segment0_BaseLo	(XHCI_BUFFERs_BASE + 0x1000) //~4K
#if (ERST_NUM_SEGS > 1)
	#define Event_Ring_Segment1_BaseLo	(XHCI_BUFFERs_BASE + 0x2000)
	#define Event_Ring_Segment2_BaseLo	(XHCI_BUFFERs_BASE + 0x3000)
	#define Event_Ring_Segment3_BaseLo	(XHCI_BUFFERs_BASE + 0x4000)
#endif

/* Command Ring Segment(s) */
#define Cmd_Ring_Segment0_BaseLo	(XHCI_BUFFERs_BASE + 0x2000) //~4K

/* Ep0 Ring Segment(s) */
#define Ep0_Ring_Segment0_BaseLo	(XHCI_BUFFERs_BASE + 0x3000) //~4K
/* Ep n in Ring Segment(s) */
#define Ep_in_Ring_Segment0_BaseLo	(XHCI_BUFFERs_BASE + 0x4000) //~4K
/* Ep n out Ring Segment(s) */
#define Ep_out_Ring_Segment0_BaseLo	(XHCI_BUFFERs_BASE + 0x5000) //~4K

/* Device Context */
	#define Device_Context_Slot1_Base	(XHCI_BUFFERs_BASE + 0x6000)
//	#define Device_Context_Slot2_Base	(XHCI_BUFFERs_BASE + 0x6800)

/*Input Context */
	#define Input_Context_Slot1_Base	(XHCI_BUFFERs_BASE + 0x6800) //~0x840
//	#define Input_Context_Slot2_Base	(XHCI_BUFFERs_BASE + 0x6040)

/* USB */
	#define Get_Dev_des_request_base		(XHCI_BUFFERs_BASE + 0x7000+0)
	#define Device_Descriptor_Return_base	(XHCI_BUFFERs_BASE + 0x7000+64)

/* BULK */
	#define CBW_Buffer_Base					(XHCI_BUFFERs_BASE + 0x7000+0x200)
	#define CSW_Buffer_Base					(XHCI_BUFFERs_BASE + 0x7000+0x200+0x40)
	#define Bulk_Trans_Data_Buffer_Base		(XHCI_BUFFERs_BASE + 0x7000+0x300)

// Controller ********************************************************************************************************
//********************************************************************************************************************
/* Represents everything that is needed to issue a command on the command ring.
 * It's useful to pre-allocate these for commands that cannot fail due to
 * out-of-memory errors, like freeing streams.
 */
struct xhci_command {
	/*   [Input context]      for changing device state */
	struct xhci_container_ctx	*in_ctx;
				//only 3 CMDs(Address Device/Configure Endpoint/Evaluate Context) has 'Input Context Pointer' field.

	UINT32				status;
	UINT32				slot_id;

	/* If completion is null, no one is waiting on this command
	 * and the structure can be freed after the command completes.
	 */
	UINT32				completion;
	union xhci_trb		*command_trb;

//	struct list_head		cmd_list;
};



/* There is one xhci_hcd structure per controller */
struct xhci_hcd {
//	struct usb_hcd *main_hcd;
//	struct usb_hcd *shared_hcd;

	/* glue to PCI and HCD framework */
	struct xhci_cap_regs	 	*cap_regs;
	struct xhci_op_regs 		*op_regs;
	struct xhci_run_regs 		*run_regs;
	struct xhci_doorbell_array 	*dba;

	/* save register copies of read-only HC data */
	UINT32		hcs_params1;
	UINT32		hcs_params2;
	UINT32		hcs_params3;
	UINT32		hcc_params;
	UINT32		hcc_params2;

	/* packed release number */
	UINT16		hci_version;

	UINT8		max_ports; //2
	UINT8		max_slots;
	UINT8		total_usb2_ports; //1
	UINT8		usb2_port_id; //1
	UINT8		total_usb3_ports; //1
	UINT8		usb3_port_id; //2

	UINT8		cur_slot_id;
	UINT8		cur_port_id;
	enum usb_device_speed 	conn_dev_speed;
	UINT8		conn_dev_EpIn_index;
	UINT8		conn_dev_EpOut_index;

/* data structures */
	// 1st use
	struct xhci_erst_entry	*erst[ERST_NUM_SEGS];
	struct xhci_ring	*event_ring;

	// 2nd use
	struct xhci_ring	*cmd_ring;
	UINT32            	cmd_ring_state;
//	struct xhci_command	*command;
	UINT32				cmd_completion;
	UINT32				trans_completion;

	//3rd use
	struct xhci_ring	*ep_ring[31];

	/* Device Context Base Address Array */
//	struct xhci_device_context_array *dcbaa;
	struct xhci_device_context_Entry *dcbaa_entry[MAX_HC_SLOTS];

	/*
	 * Commands to the hardware are passed an "input context" that
	 * tells the hardware what to change in its data structures.
	 *
	 * The hardware will return changes in an "output context" that
	 * software must allocate for the hardware.  We need to keep
	 * track of input and output contexts separately because
	 * these commands might fail and we don't trust the hardware.
	 */
	struct xhci_container_ctx       *out_ctx;	//return
	/* Used for addressing devices and configuration changes */
	struct xhci_container_ctx       *in_ctx;	//Commands


};


// Functions******************************************************************************************************
//********************************************************************************************************************
void xhci_irq(void);
void xhci_main(void);
static void xhci_configure_endpoint(struct xhci_hcd *);
static void xhci_add_endpoint(struct xhci_hcd *xhci, struct usb_endpoint_descriptor  *ep);

void xhci_print_registers(struct xhci_hcd *xhci);
void xhci_ext_cap_regs(struct xhci_hcd *xhci);
void xhci_ring_cmd_db(struct xhci_hcd *xhci);
UINT32 trb_offset_to_addr(struct xhci_segment *seg,	union xhci_trb *trb);
void inc_deq(struct xhci_hcd *xhci, struct xhci_ring *ring);
void queue_command(struct xhci_hcd *xhci,
						 UINT32 field1, UINT32 field2,
						 UINT32 field3, UINT32 field4,
						 UINT8 command_must_succeed);
struct xhci_slot_ctx *xhci_get_slot_ctx(struct xhci_hcd *xhci,
								struct xhci_container_ctx *ctx);
struct xhci_input_control_ctx *xhci_get_input_control_ctx(
					      struct xhci_container_ctx *ctx);
struct xhci_ep_ctx *xhci_get_ep_ctx(struct xhci_hcd *xhci,
				    struct xhci_container_ctx *ctx,
				    unsigned int ep_index);
inline const char *xhci_ep_state_string( UINT8 state);
inline char *xhci_slot_state_string(u32 state);


#endif /* __DRIVERS_XHCI_HCD_H */
                       



