/*
 * 	Universal Serial Bus 3.0 Specification
 *
 */
#ifndef __USB_3_0_H
#define __USB_3_0_H

#include "common_usb.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------*/
/* CONTROL REQUEST SUPPORT */

/*	bRequestType */
	 /*		<U3.0.pdf Table 9-2.>
	 * USB directions
	 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
	 * It's also one of three fields in control requests bRequestType.
	 */
	#define USB_DIR_OUT			0			/* to device */
	#define USB_DIR_IN			0x80		/* to host */
	/*
	 * USB types, the second of three bRequestType fields
	 */
	#define USB_TYPE_MASK			(0x03 << 5)
	#define USB_TYPE_STANDARD		(0x00 << 5)
	#define USB_TYPE_CLASS			(0x01 << 5)
	#define USB_TYPE_VENDOR			(0x02 << 5)
	#define USB_TYPE_RESERVED		(0x03 << 5)
	/*
	 * USB recipients, the third of three bRequestType fields
	 */
	#define USB_RECIP_MASK			0x1f
	#define USB_RECIP_DEVICE		0x00
	#define USB_RECIP_INTERFACE		0x01
	#define USB_RECIP_ENDPOINT		0x02
	#define USB_RECIP_OTHER			0x03
	/* From Wireless USB 1.0 */
	#define USB_RECIP_PORT			0x04
	#define USB_RECIP_RPIPE			0x05

/*	bRequest */
	/* Standard requests, for the bRequest field of a SETUP packet.
	 *
	 * These are qualified by the bRequestType field, so that for example
	 * TYPE_CLASS or TYPE_VENDOR specific feature flags could be retrieved
	 * by a GET_STATUS request.
	 */
	#define USB_REQ_GET_STATUS			0x00
	#define USB_REQ_CLEAR_FEATURE		0x01
	#define USB_REQ_SET_FEATURE			0x03
	#define USB_REQ_SET_ADDRESS			0x05
	#define USB_REQ_GET_DESCRIPTOR		0x06
	#define USB_REQ_SET_DESCRIPTOR		0x07
	#define USB_REQ_GET_CONFIGURATION	0x08
	#define USB_REQ_SET_CONFIGURATION	0x09
	#define USB_REQ_GET_INTERFACE		0x0A
	#define USB_REQ_SET_INTERFACE		0x0B
	#define USB_REQ_SYNCH_FRAME			0x0C
	#define USB_REQ_SET_SEL				0x30
	#define USB_REQ_SET_ISOCH_DELAY		0x31

	#define USB_REQ_SET_ENCRYPTION		0x0D	/* Wireless USB */
	#define USB_REQ_GET_ENCRYPTION		0x0E
	#define USB_REQ_RPIPE_ABORT			0x0E
	#define USB_REQ_SET_HANDSHAKE		0x0F
	#define USB_REQ_RPIPE_RESET			0x0F
	#define USB_REQ_GET_HANDSHAKE		0x10
	#define USB_REQ_SET_CONNECTION		0x11
	#define USB_REQ_SET_SECURITY_DATA	0x12
	#define USB_REQ_GET_SECURITY_DATA	0x13
	#define USB_REQ_SET_WUSB_DATA		0x14
	#define USB_REQ_LOOPBACK_DATA_WRITE	0x15
	#define USB_REQ_LOOPBACK_DATA_READ	0x16
	#define USB_REQ_SET_INTERFACE_DS	0x17

	/* specific requests for USB Power Delivery */
	#define USB_REQ_GET_PARTNER_PDO		20
	#define USB_REQ_GET_BATTERY_STATUS	21
	#define USB_REQ_SET_PDO				22
	#define USB_REQ_GET_VDM				23
	#define USB_REQ_SEND_VDM			24

	/** 0x08--Mass Storage
	 *   bulk-only class specific requests
	 *				control transfer   */
	#define USB_REQ_GET_MAX_LUN				(0xFE)
	#define USB_REQ_MASS_STORAGE_RESET		(0xFF)

	/* The Link Power Management (LPM) ECN defines USB_REQ_TEST_AND_SET command,
	 * used by hubs to put ports into a new L1 suspend state, except that it
	 * forgot to define its number ...
	 */


/*	wValue */
/* Feature Selector---Clear Feature/Set Feature */
	 /* USB feature flags are written using USB_REQ_{CLEAR,SET}_FEATURE, and
	 * are read as a bit array returned by USB_REQ_GET_STATUS.  (So there
	 * are at most sixteen features of each type.)  Hubs may also support a
	 * new USB_REQ_TEST_AND_SET_FEATURE to put ports into L1 suspend.
	 */
	#define USB_DEVICE_SELF_POWERED		0	/* (read only) */
	#define USB_DEVICE_REMOTE_WAKEUP	1	/* dev may initiate wakeup */
	#define USB_DEVICE_TEST_MODE		2	/* (wired high speed only) */
	#define USB_DEVICE_BATTERY			2	/* (wireless) */
	#define USB_DEVICE_B_HNP_ENABLE		3	/* (otg) dev may initiate HNP */
	#define USB_DEVICE_WUSB_DEVICE		3	/* (wireless)*/
	#define USB_DEVICE_A_HNP_SUPPORT		4	/* (otg) RH port supports HNP */
	#define USB_DEVICE_A_ALT_HNP_SUPPORT	5	/* (otg) other RH port does */
	#define USB_DEVICE_DEBUG_MODE			6	/* (special devices only) */
	 /** New Feature Selectors as added by USB 3.0
	 * See USB 3.0 spec Table 9-6
	 */
	#define USB_DEVICE_U1_ENABLE		48	/* dev may initiate U1 transition */
	#define USB_DEVICE_U2_ENABLE		49	/* dev may initiate U2 transition */
	#define USB_DEVICE_LTM_ENABLE		50	/* dev may send LTM */

	#define USB_INTRF_FUNC_SUSPEND		0	/* Interface/function suspend */
	#define USB_ENDPOINT_HALT			0	/* IN/OUT will STALL */

	/*
	 * Feature selectors from Table 9-8 USB Power Delivery spec
	 */
	#define USB_DEVICE_BATTERY_WAKE_MASK	40
	#define USB_DEVICE_OS_IS_PD_AWARE	41
	#define USB_DEVICE_POLICY_MODE		42
	#define USB_PORT_PR_SWAP		43
	#define USB_PORT_GOTO_MIN		44
	#define USB_PORT_RETURN_POWER		45
	#define USB_PORT_ACCEPT_PD_REQUEST	46
	#define USB_PORT_REJECT_PD_REQUEST	47
	#define USB_PORT_PORT_PD_RESET		48
	#define USB_PORT_C_PORT_PD_CHANGE	49
	#define USB_PORT_CABLE_PD_RESET		50
	#define USB_DEVICE_CHARGING_POLICY	54


	#define USB_INTR_FUNC_SUSPEND_OPT_MASK	0xFF00
/* wIndex-MSB */
	/* Suspend Options, Table 9-7 USB 3.0 spec
	 */
	#define USB_INTRF_FUNC_SUSPEND_LP	(1 << (8 + 0))
	#define USB_INTRF_FUNC_SUSPEND_RW	(1 << (8 + 1))

/* wIndex-LSB
	 * Test Mode Selectors
	 * See USB 2.0 spec Table 9-7
	 */
	#define	TEST_J			1
	#define	TEST_K			2
	#define	TEST_SE0_NAK	3
	#define	TEST_PACKET		4
	#define	TEST_FORCE_EN	5

/* DATA */
	/* Bit array elements as returned by the USB_REQ_GET_STATUS request. */
	#define USB_DEV_STAT_U1_ENABLED		2	/* transition into U1 state */
	#define USB_DEV_STAT_U2_ENABLED		3	/* transition into U2 state */
	#define USB_DEV_STAT_LTM_ENABLED	4	/* Latency tolerance messages */
	 /* Interface status, Figure 9-6 USB 3.0 spec
	 */
	#define USB_INTRF_STAT_FUNC_RW_CAP     	1
	#define USB_INTRF_STAT_FUNC_RW        	2




/**
 * struct usb_ctrlrequest - SETUP data for a USB device control request
 * @bRequestType: matches the USB bmRequestType field
 * @bRequest: matches the USB bRequest field
 * @wValue: matches the USB wValue field (le16 byte order)
 * @wIndex: matches the USB wIndex field (le16 byte order)
 * @wLength: matches the USB wLength field (le16 byte order)
 *
 * This structure is used to send control requests to a USB device.  It matches
 * the different fields of the USB 2.0 Spec section 9.3, table 9-2.  See the
 * USB spec for a fuller description of the different fields, and what they are
 * used for.
 *
 * Note that the driver for any interface can issue control requests.
 * For most devices, interfaces don't coordinate with each other, so
 * such requests may be made at any time.
 */
struct usb_ctrlrequest {
	U8 bRequestType;
	U8 bRequest;

#if 1
	U16 wValue;
#else
	U8 wValue_index; //string index
	U8 wValue_type;  //descriptor type
#endif

	U16 wIndex;
	U16 wLength;
} __attribute__ ((packed));

/*-------------------------------------------------------------------------*/

/*
 * STANDARD DESCRIPTORS ... as returned by GET_DESCRIPTOR, or
 * (rarely) accepted by SET_DESCRIPTOR.
 *
 * Note that all multi-byte values here are encoded in little endian
 * byte order "on the wire".  Within the kernel and when exposed
 * through the Linux-USB APIs, they are not converted to cpu byte
 * order; it is the responsibility of the client code to do this.
 * The single exception is when device and configuration descriptors (but
 * not other descriptors) are read from character devices
 * (i.e. /dev/bus/usb/BBB/DDD);
 * in this case the fields are converted to host endianness by the kernel.
 */

/*
 * Descriptor types ... USB 2.0 spec table 9.5
 */
#define USB_DT_DEVICE				0x01
#define USB_DT_CONFIG				0x02
#define USB_DT_STRING				0x03
#define USB_DT_INTERFACE			0x04
#define USB_DT_ENDPOINT				0x05
#define USB_DT_DEVICE_QUALIFIER		0x06
#define USB_DT_OTHER_SPEED_CONFIG	0x07
#define USB_DT_INTERFACE_POWER		0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define USB_DT_OTG					0x09
#define USB_DT_DEBUG				0x0a
#define USB_DT_INTERFACE_ASSOCIATION	0x0b
/* these are from the Wireless USB spec */
#define USB_DT_SECURITY				0x0c
#define USB_DT_KEY					0x0d
#define USB_DT_ENCRYPTION_TYPE		0x0e
#define USB_DT_BOS					0x0f
#define USB_DT_DEVICE_CAPABILITY	0x10
#define USB_DT_WIRELESS_ENDPOINT_COMP	0x11
#define USB_DT_WIRE_ADAPTER			0x21
#define USB_DT_RPIPE				0x22
#define USB_DT_CS_RADIO_CONTROL		0x23
/* From the T10 UAS specification */
#define USB_DT_PIPE_USAGE			0x24
/* From the USB 3.0 spec */
#define	USB_DT_SS_ENDPOINT_COMP			0x30
/* From the USB 3.1 spec */
#define	USB_DT_SSP_ISOC_ENDPOINT_COMP	0x31

/* Conventional codes for class-specific descriptors.  The convention is
 * defined in the USB "Common Class" Spec (3.11).  Individual class specs
 * are authoritative for their usage, not the "common class" writeup.
 */
#define USB_DT_CS_DEVICE		(USB_TYPE_CLASS | USB_DT_DEVICE)
#define USB_DT_CS_CONFIG		(USB_TYPE_CLASS | USB_DT_CONFIG)
#define USB_DT_CS_STRING		(USB_TYPE_CLASS | USB_DT_STRING)
#define USB_DT_CS_INTERFACE		(USB_TYPE_CLASS | USB_DT_INTERFACE)
#define USB_DT_CS_ENDPOINT		(USB_TYPE_CLASS | USB_DT_ENDPOINT)

/* All standard descriptors have these 2 fields at the beginning */
struct usb_descriptor_header {
	U8  bLength;
	U8  bDescriptorType;
} __attribute__ ((packed));



/*-------------------------------------------------------------------------*/

/* USB_DT_DEVICE: Device descriptor */
struct usb_device_descriptor {
	U8  bLength;
	U8  bDescriptorType;

	U16 bcdUSB;
	U8  bDeviceClass;
	U8  bDeviceSubClass;
	U8  bDeviceProtocol;
	U8  bMaxPacketSize0;
	U16 idVendor;
	U16 idProduct;
	U16 bcdDevice;
	U8  iManufacturer;
	U8  iProduct;
	U8  iSerialNumber;
	U8  bNumConfigurations;
} __attribute__ ((packed));

#define USB_DT_DEVICE_SIZE		18


/*
 * Device and/or Interface Class codes
 * as found in bDeviceClass or bInterfaceClass
 * and defined by www.usb.org documents
 */
#define USB_CLASS_PER_INTERFACE		0	/* for DeviceClass */
#define USB_CLASS_AUDIO				1
#define USB_CLASS_COMM				2
#define USB_CLASS_HID				3   //
#define USB_CLASS_PHYSICAL			5
#define USB_CLASS_STILL_IMAGE		6
#define USB_CLASS_PRINTER			7
#define USB_CLASS_MASS_STORAGE		8   //
#define USB_CLASS_HUB				9
#define USB_CLASS_CDC_DATA			0x0a
#define USB_CLASS_CSCID				0x0b	/* chip+ smart card */
#define USB_CLASS_CONTENT_SEC		0x0d	/* content security */
#define USB_CLASS_VIDEO				0x0e
#define USB_CLASS_WIRELESS_CONTROLLER	0xe0
#define USB_CLASS_MISC				0xef
#define USB_CLASS_APP_SPEC			0xfe
#define USB_CLASS_VENDOR_SPEC		0xff

#define USB_SUBCLASS_VENDOR_SPEC	0xff

#if U2_GADGET_TEST
	#define b_MAX_PACKET_SIZE_0   	0x40   //64B  bMaxPacketSize0
	#define b_MAX_PACKET_SIZE_BULK  512    //bMaxPacketSize
#endif
#if U3_GADGET_TEST
	#define b_MAX_PACKET_SIZE_0   	0x09   //512B
	#define b_MAX_PACKET_SIZE_BULK	1024
#endif


/*-------------------------------------------------------------------------*/
/* USB_DT_CONFIG: Configuration descriptor information.
 *
 * USB_DT_OTHER_SPEED_CONFIG is the same descriptor, except that the
 * descriptor type is different.  Highspeed-capable devices can look
 * different depending on what speed they're currently running.  Only
 * devices with a USB_DT_DEVICE_QUALIFIER have any OTHER_SPEED_CONFIG
 * descriptors.
 */
struct usb_config_descriptor {
	U8  bLength;
	U8  bDescriptorType;

	U16 wTotalLength;
	U8  bNumInterfaces;
	U8  bConfigurationValue;
	U8  iConfiguration;
	U8  bmAttributes;
	U8  bMaxPower;
} __attribute__ ((packed));

#define USB_DT_CONFIG_SIZE		9

/* from config descriptor bmAttributes */
#define USB_CONFIG_ATT_ONE			(1 << 7)	/* must be set */
#define USB_CONFIG_ATT_SELFPOWER	(1 << 6)	/* self powered */
#define USB_CONFIG_ATT_WAKEUP		(1 << 5)	/* can wakeup */
#define USB_CONFIG_ATT_BATTERY		(1 << 4)	/* battery powered */


/*-------------------------------------------------------------------------*/
/* USB_DT_STRING: String descriptor */
struct usb_string_descriptor {
	U8  bLength;
	U8  bDescriptorType;

	U16 wData[1];		/* UTF-16LE encoded */
} __attribute__ ((packed));

/* note that "string" zero is special, it holds language codes that
 * the device supports, not Unicode characters.
 */


/*-------------------------------------------------------------------------*/
/* USB_DT_INTERFACE: Interface descriptor */
struct usb_interface_descriptor {  //basic struct, 9bytes <---in struct usb_host_interface{
	U8  bLength;
	U8  bDescriptorType;

	U8  bInterfaceNumber;
	U8  bAlternateSetting;
	U8  bNumEndpoints;
	U8  bInterfaceClass;
	U8  bInterfaceSubClass;
	U8  bInterfaceProtocol;
	U8  iInterface;
} __attribute__ ((packed));

#define USB_DT_INTERFACE_SIZE		9



/*-------------------------------------------------------------------------*/

/* USB_DT_ENDPOINT: Endpoint descriptor */
struct usb_endpoint_descriptor {
	U8  bLength;
	U8  bDescriptorType;

	U8  bEndpointAddress;
	U8  bmAttributes;
	U16 wMaxPacketSize;
	U8  bInterval;

	/* NOTE:  these two are _only_ in audio endpoints. */
	/* use USB_DT_ENDPOINT*_SIZE in bLength, not sizeof. */
	U8  bRefresh;
	U8  bSynchAddress;
} __attribute__ ((packed));

#define USB_DT_ENDPOINT_SIZE			7
#define USB_DT_ENDPOINT_AUDIO_SIZE		9	/* Audio extension */

/*
 * Endpoints
 */
#define USB_ENDPOINT_NUMBER_MASK		0x0f	/* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK			0x80

#define USB_ENDPOINT_XFERTYPE_MASK		0x03	/* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL		0
#define USB_ENDPOINT_XFER_ISOC			1
#define USB_ENDPOINT_XFER_BULK			2
#define USB_ENDPOINT_XFER_INT			3
#define USB_ENDPOINT_MAX_ADJUSTABLE		0x80

#define USB_ENDPOINT_MAXP_MASK		0x07ff
#define USB_EP_MAXP_MULT_SHIFT		11
#define USB_EP_MAXP_MULT_MASK		(3 << USB_EP_MAXP_MULT_SHIFT)
#define USB_EP_MAXP_MULT(m) \
					(((m) & USB_EP_MAXP_MULT_MASK) >> USB_EP_MAXP_MULT_SHIFT)

/* The USB 3.0 spec redefines bits 5:4 of bmAttributes as interrupt ep type. */
#define USB_ENDPOINT_INTRTYPE				0x30
#define USB_ENDPOINT_INTR_PERIODIC			(0 << 4)
#define USB_ENDPOINT_INTR_NOTIFICATION		(1 << 4)

#define USB_ENDPOINT_SYNCTYPE			0x0c
#define USB_ENDPOINT_SYNC_NONE			(0 << 2)
#define USB_ENDPOINT_SYNC_ASYNC			(1 << 2)
#define USB_ENDPOINT_SYNC_ADAPTIVE		(2 << 2)
#define USB_ENDPOINT_SYNC_SYNC			(3 << 2)

#define USB_ENDPOINT_USAGE_MASK			0x30
#define USB_ENDPOINT_USAGE_DATA			0x00
#define USB_ENDPOINT_USAGE_FEEDBACK		0x10
#define USB_ENDPOINT_USAGE_IMPLICIT_FB	0x20	/* Implicit feedback Data endpoint */

/*-------------------------------------------------------------------------*/

struct usb_SetFeature_TestMode {
	U8			test_mode_en;
	U8			test_mode;
}__attribute__ ((packed));


/*-------------------------------------------------------------------------*/
//3.0 des
/* USB_DT_SS_ENDPOINT_COMP: SuperSpeed Endpoint Companion descriptor --3.0 P339 */
struct usb_ss_ep_comp_descriptor {  //basic config struct ,6 bytes <---in struct usb_host_endpoint
	U8  bLength;
	U8  bDescriptorType;  //<--usb_parse_ss_endpoint_companion()

	U8  bMaxBurst;
	U8  bmAttributes;
	U16 wBytesPerInterval;
} __attribute__ ((packed));

#define USB_DT_SS_EP_COMP_SIZE		6


			#if 1
/*-------------------------------------------------------------------------*/
enum usb_device_speed {
	USB_SPEED_UNKNOWN = 0,			/* enumerating */

	USB_SPEED_LOW, USB_SPEED_FULL,	/* usb 1.1 */
	USB_SPEED_HIGH,					/* usb 2.0 */
	USB_SPEED_WIRELESS,				/* wireless (usb 2.5) */
	USB_SPEED_SUPER,				/* usb 3.0 */
	USB_SPEED_SUPER_PLUS,			/* usb 3.1 */
};

static const char *const speed_names[] = {
	[USB_SPEED_UNKNOWN] = "UNKNOWN",
	[USB_SPEED_LOW] 	= "low-speed",
	[USB_SPEED_FULL] 	= "full-speed",
	[USB_SPEED_HIGH] 	= "high-speed",
	[USB_SPEED_WIRELESS]	= "wireless",
	[USB_SPEED_SUPER] 		= "super-speed",
	[USB_SPEED_SUPER_PLUS] 	= "super-speed-plus"
};



enum usb_device_state {
	/* NOTATTACHED isn't in the USB spec, and this state acts
	 * the same as ATTACHED ... but it's clearer this way.
	 */
	USB_STATE_NOTATTACHED = 0,

	/* chapter 9 and authentication (wireless) device states */
	USB_STATE_ATTACHED,
	USB_STATE_POWERED,			/* wired */
	USB_STATE_RECONNECTING,		/* auth */
	USB_STATE_UNAUTHENTICATED,	/* auth */
	USB_STATE_DEFAULT,			/* limited function */
	USB_STATE_ADDRESS,
	USB_STATE_CONFIGURED,		/* most functions -----*/

	USB_STATE_SUSPENDED

	/* NOTE:  there are actually four different SUSPENDED
	 * states, returning to POWERED, DEFAULT, ADDRESS, or
	 * CONFIGURED respectively when SOF tokens flow again.
	 * At this level there's no difference between L1 and L2
	 * suspend states.  (L2 being original USB 1.1 suspend.)
	 */
};
enum usb3_link_state {
	USB3_LPM_U0 = 0,
	USB3_LPM_U1,
	USB3_LPM_U2,
	USB3_LPM_U3
};

/*
 * A U1 timeout of 0x0 means the parent hub will reject any transitions to U1.
 * 0xff means the parent hub will accept transitions to U1, but will not
 * initiate a transition.
 *
 * A U1 timeout of 0x1 to 0x7F also causes the hub to initiate a transition to
 * U1 after that many microseconds.  Timeouts of 0x80 to 0xFE are reserved
 * values.
 *
 * A U2 timeout of 0x0 means the parent hub will reject any transitions to U2.
 * 0xff means the parent hub will accept transitions to U2, but will not
 * initiate a transition.
 *
 * A U2 timeout of 0x1 to 0xFE also causes the hub to initiate a transition to
 * U2 after N*256 microseconds.  Therefore a U2 timeout value of 0x1 means a U2
 * idle timer of 256 microseconds, 0x2 means 512 microseconds, 0xFE means
 * 65.024ms.
 */
#define USB3_LPM_DISABLED			0x0
#define USB3_LPM_U1_MAX_TIMEOUT		0x7F
#define USB3_LPM_U2_MAX_TIMEOUT		0xFE
#define USB3_LPM_DEVICE_INITIATED	0xFF

/*
 * The Set System Exit Latency control transfer provides one byte each for
 * U1 SEL and U1 PEL, so the max exit latency is 0xFF.  U2 SEL and U2 PEL each
 * are two bytes long.
 */
#define USB3_LPM_MAX_U1_SEL_PEL		0xFF
#define USB3_LPM_MAX_U2_SEL_PEL		0xFFFF

			#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//===================== UFI Commands  .x ========================
#define bInterfaceClass_mass				0x08		//Mass Storage Class
#define bInterfaceSunClass_SCSI				0x06
#define bInterfaceProtocol_CBIInterrupt		0x00
#define bInterfaceProtocol_CBICommand		0x01
#define bInterfaceProtocol_BBB				0x50		//Bulk-Only Transport

#define CSW_STATUS_CommandPassed	0
#define CSW_STATUS_Command_Failed	1
#define CSW_STATUS_Phase_Error		2


/*
 * Bulk only data structures
 */
/* command block wrapper				CBW    */
struct bulk_cb_wrap {
	U32	Signature;			/* contains 'USBC' */
	U32	Tag;				/* unique per command id */
	U32	DataTransferLength;	/* size of data */
	U8	Flags;			/* direction in bit 0 */
	U8	Lun;			/* LUN normally 0 ----------> high 4 bit = target */
	U8	Length;			/* length of the CDB */

	U8	CDB[16];		/* max command ----------------------- =us->srb->cmnd[0~xx]   */
};
#define US_BULK_CB_WRAP_LEN		31
#define US_BULK_CB_SIGN			0x43425355	/* spells out 'USBC=0x55534243' */
#define US_BULK_FLAG_IN			(1 << 7)
#define US_BULK_FLAG_OUT		0


/* command status wrapper 				CSW  */
struct bulk_cs_wrap {
	U32	Signature;	/* contains 'USBS' */
	U32	Tag;		/* same as original command */
	U32	Residue;	/* amount not transferred */
	U8	Status;		/* see below */
};
#define US_BULK_CS_WRAP_LEN		13
#define US_BULK_CS_SIGN			0x53425355      /* spells out 'USBS' */
#define US_BULK_STAT_OK			0
#define US_BULK_STAT_FAIL		1
#define US_BULK_STAT_PHASE		2

/* states for Bulk Transfer */
enum USB_Bulk_state {
	BULK_COMMAND_STAGE	= 0xFF,
	BULK_DATA_STAGE   	= 0x00,
	BULK_STATUS_STAGE 	= 0x01
};


/*
 * These definitions mirror those in pci.h, so they can be used
 * interchangeably with their PCI_ counterparts.
 */
enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};


/*
 *      SCSI opcodes
 */
#define TEST_UNIT_READY      	 	0x00
#define REZERO_UNIT           		0x01
#define REQUEST_SENSE         		0x03
#define FORMAT_UNIT           		0x04
#define READ_BLOCK_LIMITS     		0x05
#define REASSIGN_BLOCKS      		0x07
#define INITIALIZE_ELEMENT_STATUS 	0x07
#define READ_6                		0x08
#define WRITE_6               		0x0a
#define SEEK_6                		0x0b
#define READ_REVERSE         		0x0f
#define WRITE_FILEMARKS     		0x10
#define SPACE                 		0x11
#define INQUIRY               		0x12
#define RECOVER_BUFFERED_DATA 		0x14
#define MODE_SELECT           		0x15
#define RESERVE              		0x16
#define RELEASE               		0x17
#define COPY                  		0x18
#define ERASE                 		0x19
#define MODE_SENSE            		0x1a
#define START_STOP            		0x1b
#define RECEIVE_DIAGNOSTIC    		0x1c
#define SEND_DIAGNOSTIC       		0x1d
#define ALLOW_MEDIUM_REMOVAL  		0x1e

#define READ_FORMAT_CAPACITIES 		0x23
#define SET_WINDOW            		0x24
#define READ_CAPACITY         		0x25
#define READ_10               		0x28
#define WRITE_10              		0x2a

#define SEEK_10               		0x2b
#define POSITION_TO_ELEMENT   		0x2b
#define WRITE_VERIFY          		0x2e
#define VERIFY               		0x2f
#define SEARCH_HIGH           		0x30
#define SEARCH_EQUAL          		0x31
#define SEARCH_LOW            		0x32
#define SET_LIMITS            		0x33
#define PRE_FETCH             		0x34
#define READ_POSITION         		0x34
#define SYNCHRONIZE_CACHE     		0x35
#define LOCK_UNLOCK_CACHE     		0x36
#define READ_DEFECT_DATA      		0x37
#define MEDIUM_SCAN           		0x38
#define COMPARE               		0x39
#define COPY_VERIFY           		0x3a
#define WRITE_BUFFER          		0x3b
#define READ_BUFFER           		0x3c
#define UPDATE_BLOCK          		0x3d
#define READ_LONG            		0x3e
#define WRITE_LONG            		0x3f
#define CHANGE_DEFINITION     		0x40
#define WRITE_SAME            		0x41
#define UNMAP		      			0x42
#define READ_TOC              		0x43
#define READ_HEADER           		0x44
#define GET_EVENT_STATUS_NOTIFICATION 0x4a
#define LOG_SELECT            		0x4c
#define LOG_SENSE             		0x4d
#define XDWRITEREAD_10        		0x53
#define MODE_SELECT_10        		0x55
#define RESERVE_10            		0x56
#define RELEASE_10            		0x57
#define MODE_SENSE_10         		0x5a
#define PERSISTENT_RESERVE_IN 		0x5e
#define PERSISTENT_RESERVE_OUT 		0x5f
#define VARIABLE_LENGTH_CMD   		0x7f
#define REPORT_LUNS           		0xa0
#define SECURITY_PROTOCOL_IN  		0xa2
#define MAINTENANCE_IN       		0xa3
#define MAINTENANCE_OUT       		0xa4
#define MOVE_MEDIUM           		0xa5
#define EXCHANGE_MEDIUM       		0xa6
#define READ_12               		0xa8
#define SERVICE_ACTION_OUT_12 		0xa9
#define WRITE_12              		0xaa
#define READ_MEDIA_SERIAL_NUMBER 	0xab /* Obsolete with SPC-2 */
#define SERVICE_ACTION_IN_12  		0xab
#define WRITE_VERIFY_12       		0xae
#define VERIFY_12	      			0xaf
#define SEARCH_HIGH_12        		0xb0
#define SEARCH_EQUAL_12       		0xb1
#define SEARCH_LOW_12         		0xb2
#define SECURITY_PROTOCOL_OUT 		0xb5
#define READ_ELEMENT_STATUS   		0xb8
#define SEND_VOLUME_TAG       		0xb6
#define WRITE_LONG_2          		0xea
#define EXTENDED_COPY        		0x83
#define RECEIVE_COPY_RESULTS  		0x84
#define ACCESS_CONTROL_IN     		0x86
#define ACCESS_CONTROL_OUT    		0x87
#define READ_16               		0x88
#define COMPARE_AND_WRITE     		0x89
#define WRITE_16              		0x8a
#define READ_ATTRIBUTE        		0x8c
#define WRITE_ATTRIBUTE	      		0x8d
#define WRITE_VERIFY_16	      		0x8e
#define VERIFY_16	      			0x8f
#define SYNCHRONIZE_CACHE_16  		0x91
#define WRITE_SAME_16	      		0x93
#define ZBC_OUT		      			0x94
#define ZBC_IN		      			0x95
#define SERVICE_ACTION_BIDIRECTIONAL 0x9d
#define SERVICE_ACTION_IN_16  		0x9e
#define SERVICE_ACTION_OUT_16 		0x9f
const char  *SCSI_UFI_OPCODES_string(unsigned char op_core);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* _USB3_H_ */    
                       



