/*******************************************************
 * File: usb_xhci.c
 * Description:eXtensible Host Controller Interface for Universal Serial Bus(xHCI)
 * Date: 2020/07
 * Author: ywang@ccore_ip		gylan@ccore_ip
 *
 ******************************************************/

#include "as_archppc470.h"
#include "ppclib.h"
#include "mpic.h"
#include "uart.h"
#include "common_usb.h"

#include "usb_reg_dwc3.h"
#include "usb_reg_xhci.h"

struct xhci_hcd		ccore_xhci;
struct xhci_hcd		*xhci = &ccore_xhci;

#define XHCI_DEBUG	 	1
volatile UINT8 connect_change = 0;

extern void xhci_queue_ctrl_tx(struct xhci_hcd *xhci,
		struct usb_ctrlrequest *pReq,
		UINT8 *data_stage_buf,
		UINT16 data_buf_len
		);
extern void xhci_queue_bulk_tx(struct xhci_hcd *xhci,
		UINT8 ep_index,
		UINT8 *p_data_buf,
		UINT16 data_buf_len
		);
//================ Bulk only transport =======================================================
//============= Storage/Bulk / SCSI ==========================================================

/* Determine what the maximum LUN supported is */
UINT8 usb_stor_Bulk_max_lun(struct xhci_hcd *xhci, UINT8 *desc,UINT16 des_size  )
{
	struct usb_ctrlrequest *pReq = (struct usb_ctrlrequest*) (Get_Dev_des_request_base);

	pReq->bRequestType  = USB_DIR_IN | USB_TYPE_CLASS |  USB_RECIP_INTERFACE;
	pReq->bRequest		= USB_REQ_GET_MAX_LUN;
	pReq->wValue 		= cpu_to_le16( 0x0000 );
	pReq->wIndex 		= cpu_to_le16( 0x0000 ); //xhci->conn_dev_interface_id
	pReq->wLength		= cpu_to_le16( des_size );
	//A1 FE 00 00

	xhci_queue_ctrl_tx( xhci,pReq,   (UINT8 *) desc , des_size );

	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;

	return ( *(UINT8 *)(desc+0) );
}

void usb_storage_alloc(struct xhci_hcd *xhci)
{
	UINT8 max_lun = 0x5A;

	PSPRINTF(" Now, one %s USB USB Mass Storage device detected\n",
			(xhci->conn_dev_speed >= USB_SPEED_SUPER) ? "3.0":"2.0"		);
	delay(0x40000);
	max_lun = usb_stor_Bulk_max_lun(xhci, (UINT8 *)(Device_Descriptor_Return_base), 0x01 );
#if 1
	PSPRINTF(" get max_lun = %d\n",max_lun	);
#endif
}

UINT8 SCSICmd_Inquiry12[31]=
{
	'U','S','B','C',			//0x55,0x53,0x42,0x43,		//0 dCBWSignature
	0x08,0x20,0xd6,0x81,		//4 dCBWTag
	0x24,0x00,0x00,0x00,		//8 dCBWDataTransferLength
	0x80,						//12 bmCBWFlag
	0x00,						//13 bCBWLUN[3:0]
	0x06,						//14 bCBWCBLength[3:0]

	INQUIRY,					//15 SCSI command code
	0x00,0x00,0x00,0x24,		//16
	0x00,0x00,0x00,0x00,		//20
	0x00,0x00,0x00,0x00,		//24
	0x00,0x00,0x00				//28
};
void usb_stor_Bulk_transport_inqury12(struct xhci_hcd *xhci)
{
#if 1
	//struct bulk_cb_wrap  *bcb = (struct bulk_cb_wrap *)(CBW_Buffer_Base);
	struct bulk_cb_wrap  *bcb = (struct bulk_cb_wrap *)(SCSICmd_Inquiry12);
//	struct bulk_cs_wrap  *bcs = (struct bulk_cs_wrap *)(CSW_Buffer_Base);
#endif
	UINT32 i;


	__memcpy( (UINT8*)CBW_Buffer_Base,SCSICmd_Inquiry12,US_BULK_CB_WRAP_LEN );
#if 1
/*CBW(-->OUT Ep):	alway send out by host
	set up the command wrapper */

	PSPRINTF("CBW: DataL=%d(%s),CBL=%d:'%s'\n",
					 le32_to_cpu( bcb->DataTransferLength ),
					 ((bcb->Flags) &US_BULK_FLAG_IN) ? "IN":"OUT",

					 bcb->Length,
					 SCSI_UFI_OPCODES_string(bcb->CDB[0])
					 );
#endif
//	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpOut_index, (UINT8 *)(bcb),US_BULK_CB_WRAP_LEN );
	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpOut_index, (UINT8 *)(CBW_Buffer_Base),US_BULK_CB_WRAP_LEN );
	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;



#if 1
/* DATA STAGE  <--may be in or out */
	/* send/receive data payload, if there is any */
	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpIn_index, (UINT8 *)(Bulk_Trans_Data_Buffer_Base),0x24 );
	while( xhci->trans_completion ==0);
	xhci->trans_completion =0;

#endif


#if 1
/* get CSW 	<-----always from dev to host
	for device status */
//	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpIn_index, (UINT8 *)(bcs),US_BULK_CS_WRAP_LEN );
	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpIn_index, (UINT8 *)(CSW_Buffer_Base),US_BULK_CS_WRAP_LEN );
	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;

#endif

#if 1
	PSPRINTF("Inquiry12:\n");
	for( i=0; i<0x24; i++) {
		PSPRINTF("0x%x,",	*(UINT8 *)(Bulk_Trans_Data_Buffer_Base+i) );
	}
	PSPRINTF("\n");


	PSPRINTF("CSW:\n");
	for( i=0; i<US_BULK_CS_WRAP_LEN; i++) {
		//PSPRINTF("0x%x,",	*(UINT8 *)(bcs+i) );  err

		PSPRINTF("0x%x,",	*(UINT8 *)(CSW_Buffer_Base +i) );
		//PSPRINTF("0x%x,",	*(UINT8 *)( (UINT8 *)bcs +i) );
	}
	PSPRINTF("\n");
#endif
}


const unsigned char SCSICmd_Read10[31]=
{
	'U','S','B','C',			//0x55,0x53,0x42,0x43,			//0 dCBWSignature
	0xe8,0x03,0x0e,0x85,		//dCBWTag
	0x00,0x02,0x00,0x00,		//dCBWDataTransferLength
	0x80,						//bmCBWFlag
	0x00,						//bCBWLUN 3:0
	0x0a,						//bCBWCBLength 3:0

	READ_10,					//SCSI command code
	0x00,						//LogicalUnitNumber 7:5
	0x00,0x00,0x00,0x00,		//Logical Block Address
	0x00,						//Reserved
	0x00,0x01,					//Transfer Length(sector num)
	0x00,0x00,0x00,				//Reserved
	0x00,0x00,0x00,0x00
};

void usb_stor_Bulk_transport_read10(struct xhci_hcd *xhci)
{
#if 1
	//struct bulk_cb_wrap  *bcb = (struct bulk_cb_wrap *)(CBW_Buffer_Base);
	struct bulk_cb_wrap  *bcb = (struct bulk_cb_wrap *)(SCSICmd_Read10);
	//struct bulk_cs_wrap  *bcs = (struct bulk_cs_wrap *)(CSW_Buffer_Base);
#endif
	UINT32 i;

	__memcpy( (UINT8*)CBW_Buffer_Base,SCSICmd_Read10,US_BULK_CB_WRAP_LEN );
#if 1
/*CBW(-->OUT Ep):	alway send out by host
	set up the command wrapper */

	PSPRINTF("CBW: DataL=%d(%s),CBL=%d:'%s'\n",
					 le32_to_cpu( bcb->DataTransferLength ),
					 ((bcb->Flags) &US_BULK_FLAG_IN) ? "IN":"OUT",

					 bcb->Length,
					 SCSI_UFI_OPCODES_string(bcb->CDB[0])
					 );
#endif
//	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpOut_index, (UINT8 *)(bcb),US_BULK_CB_WRAP_LEN );
	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpOut_index, (UINT8 *)(CBW_Buffer_Base),US_BULK_CB_WRAP_LEN );
	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;


#if 1
/* DATA STAGE  <--may be in or out */
	/* send/receive data payload, if there is any */
	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpIn_index, (UINT8 *)(Bulk_Trans_Data_Buffer_Base),0x200 );
	while( xhci->trans_completion ==0);
	xhci->trans_completion =0;

#endif


#if 1
/* get CSW 	<-----always from dev to host
	for device status */
//	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpIn_index, (UINT8 *)(bcs),US_BULK_CS_WRAP_LEN );
	xhci_queue_bulk_tx( xhci, xhci->conn_dev_EpIn_index, (UINT8 *)(CSW_Buffer_Base),US_BULK_CS_WRAP_LEN );
	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;

#endif

#if 1
	delay(0x200000);

	PSPRINTF("Read10:\n");
	for( i=0; i<0x200; i++) {
		if( (i>0) && (i%16 ==0)  )
			PSPRINTF("\n");

		PSPRINTF("0x%x,",	*(UINT8 *)(Bulk_Trans_Data_Buffer_Base+i) );
	}
	PSPRINTF("\n");

	PSPRINTF("CSW:\n");
	for( i=0; i<US_BULK_CS_WRAP_LEN; i++) {
		//PSPRINTF("0x%x,",	*(UINT8 *)(bcs+i) );  err

		PSPRINTF("0x%x,",	*(UINT8 *)(CSW_Buffer_Base +i) );
		//PSPRINTF("0x%x,",	*(UINT8 *)( (UINT8 *)bcs +i) );
	}
	PSPRINTF("\n");
#endif
}

//============================================================================================
//============= Transfer =====================================================================
/*
 * usb_get_device_descriptor - (re)reads the device descriptor (usbcore)
 * Updates the copy of the device descriptor stored in the device structure,
 * which dedicates space for this purpose.
 *
 * Return: The number of bytes received on success, or else the status code
 * returned by the underlying usb_control_msg() call.
 */
static void usb_get_device_descriptor( struct xhci_hcd *xhci,
								struct usb_device_descriptor *desc,UINT16 des_size )
{
	struct usb_ctrlrequest *pReq = (struct usb_ctrlrequest*) (Get_Dev_des_request_base);

	pReq->bRequestType = USB_DIR_IN;
	pReq->bRequest	 = USB_REQ_GET_DESCRIPTOR;
	pReq->wValue 		= cpu_to_le16( USB_DT_DEVICE << 8 );
	pReq->wIndex 		= cpu_to_le16( 0x0000 );
	pReq->wLength		= cpu_to_le16( des_size );
	//80 06 00 01 00 00 40 00
	//80 06 00 01 00 00 12 00

	xhci_queue_ctrl_tx( xhci,pReq,   (UINT8 *) desc , des_size );
	

	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;

	//copy ?
}


static void usb_get_configuration( struct xhci_hcd *xhci,
						//struct usb_config_descriptor *desc,UINT16 des_size )
		 	 	 	 	 UINT8 *desc,UINT16 des_size )
{
	struct usb_ctrlrequest *pReq = (struct usb_ctrlrequest*) (Get_Dev_des_request_base);

	pReq->bRequestType = USB_DIR_IN;
	pReq->bRequest	   = USB_REQ_GET_DESCRIPTOR;
	pReq->wValue 		= cpu_to_le16( USB_DT_CONFIG << 8 );
	pReq->wIndex 		= cpu_to_le16( 0x0000 );
	pReq->wLength		= cpu_to_le16( des_size );
	//80 06 00 02 00 00 09 00
	//80 06 00 02 00 00 20 00

	xhci_queue_ctrl_tx( xhci,pReq,   (UINT8 *) desc , des_size );

	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;

}
static void usb_get_string( struct xhci_hcd *xhci,
		 	 	 	 	 UINT8 *desc,UINT16 des_size )
{

}

static void usb_set_configuration(struct xhci_hcd *xhci,
								UINT8 bConfigurationValue )
{
	struct usb_ctrlrequest *pReq = (struct usb_ctrlrequest*) (Get_Dev_des_request_base);

	pReq->bRequestType = USB_DIR_OUT;
	pReq->bRequest	   = USB_REQ_SET_CONFIGURATION;
	pReq->wValue 		= cpu_to_le16( bConfigurationValue );
	pReq->wIndex 		= cpu_to_le16( 0x0000 );
	pReq->wLength		= cpu_to_le16( 0x0000 );
	//00 09 01 00 00 00 00 00

	xhci_queue_ctrl_tx( xhci,pReq,   (UINT8 *) 0 , 0x0000 );

	while( xhci->trans_completion ==0);  //->handle_tx_event()
	xhci->trans_completion =0;
}


/**
 * usb_enumerate_device - Read device configs/interface/endpoint/otg (usbcore-internal)
 * @udev: newly addressed device (in ADDRESS state)
 *
 *  This is called with devices which have been detected but not fully
 *  enumerated.  The device descriptor is available, but not descriptors
 *  for any device configuration.
 */
static void usb_enumerate_device(struct xhci_hcd *xhci)
{
#if 1
	struct usb_device_descriptor 	*get_desc;
	struct usb_config_descriptor 	*get_config;
	struct usb_interface_descriptor *get_intf;
	struct usb_endpoint_descriptor  *get_epdesc[2]; //not ep0
	UINT32 config_length,i;
#endif

	//2nd get des:18 bytes-----
	usb_get_device_descriptor( xhci,(struct usb_device_descriptor *)(Device_Descriptor_Return_base),
								USB_DT_DEVICE_SIZE );
	get_desc = (struct usb_device_descriptor *)(Device_Descriptor_Return_base);
	#if XHCI_DEBUG
		PSPRINTF("(2nd get des=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",
				get_desc->bLength,	get_desc->bDescriptorType,	get_desc->bcdUSB,
				get_desc->bDeviceSubClass,	get_desc->bDeviceProtocol,	get_desc->bMaxPacketSize0	);
	#endif
	if( (get_desc->bDescriptorType != USB_DT_DEVICE ) ||
		(get_desc->bLength != USB_DT_DEVICE_SIZE) )	{
		PSPRINTF("get invalid device descriptor");
		COND_TRAP(31, 0, 0);
	}
	if( get_desc->bNumConfigurations > 1 )	{
		PSPRINTF("get_desc has more than one Configurations:%d, you need add more codes!\n",
					get_desc->bNumConfigurations );
		COND_TRAP(31, 0, 0);
	}


/* 1st configuration:
 *  We grab just the first descriptor so we know how long the whole configuration is */
	usb_get_configuration( xhci,(UINT8 *)(Device_Descriptor_Return_base),
							USB_DT_CONFIG_SIZE );
	get_config = (struct usb_config_descriptor *)(Device_Descriptor_Return_base);
	#if (XHCI_DEBUG)
		PSPRINTF("(1st get config=0x%x,0x%x,0x%x,0x%x,0x%x\n",
				get_config->bLength,		get_config->bDescriptorType,
				get_config->wTotalLength,	get_config->bNumInterfaces,	get_config->bConfigurationValue	);
	#endif


/* 2nd configuration:
 * 1st will get the total length: Now that we know the length, get the whole thing */
	config_length = max(  (int)le16_to_cpu(get_config->wTotalLength),  //0x20
						 USB_DT_CONFIG_SIZE);		//0x2C 3.0
	usb_get_configuration( xhci,(UINT8 *)(Device_Descriptor_Return_base),
							config_length );//-2nd-
/* parse/analysis configuration
 * */
	#if (XHCI_DEBUG)
			PSPRINTF("(2nd get config:");
		for( i=0; i<config_length; i++) {
			PSPRINTF("0x%x,",	*(UINT8 *)(Device_Descriptor_Return_base+i) );
		}
		PSPRINTF("\n");
	#endif
	get_config = (struct usb_config_descriptor *)(Device_Descriptor_Return_base);
	if( (get_config->bDescriptorType != USB_DT_CONFIG) ||
		(get_config->bLength != USB_DT_CONFIG_SIZE )
	) {
			PSPRINTF("2nd get invalid config, bLength = %d\n",get_config->bLength  );
			COND_TRAP(31, 0, 0);
	}

	if( le16_to_cpu(get_config->wTotalLength) != config_length
	) {
		PSPRINTF("2nd get invalid config, wTotalLength = %d\n",le16_to_cpu(get_config->wTotalLength)  );
		COND_TRAP(31, 0, 0);
	}
	if(	(le16_to_cpu(get_config->wTotalLength) != 0x20)  &&
		(le16_to_cpu(get_config->wTotalLength) != 0x2C)
	) {
		PSPRINTF("2nd get invalid config, wTotalLength = %d\n",le16_to_cpu(get_config->wTotalLength)  );
		COND_TRAP(31, 0, 0);
	}

/*interface:  checking their length and counting the number of altsettings for each interface
 * */
	if(  get_config->bNumInterfaces > 1 )	{
		PSPRINTF("config %d has too many interfaces: %d, you need add more codes!\n",
							get_config->bConfigurationValue,get_config->bNumInterfaces);
		COND_TRAP(31, 0, 0);
	}

	get_intf = (struct usb_interface_descriptor *)(Device_Descriptor_Return_base + USB_DT_CONFIG_SIZE);
	if( (get_intf->bDescriptorType != USB_DT_INTERFACE ) ||
		(get_intf->bLength != USB_DT_INTERFACE_SIZE) )	{
		PSPRINTF("get invalid interface descriptor, len = %d\n",get_intf->bLength );
		COND_TRAP(31, 0, 0);
	}

	/*** check if it is a mass storage(interface 0x08) bulk-only(interface 0x06) device ***/
	if( (get_intf->bInterfaceClass != bInterfaceClass_mass) ||
		(get_intf->bInterfaceSubClass != bInterfaceSunClass_SCSI)||
		(get_intf->bInterfaceProtocol != bInterfaceProtocol_BBB)
	)	{
		PSPRINTF("Sorry, insert device is not Mass Storage dev, you need add more codes!\n");
		COND_TRAP(31, 0, 0);
	}

/* endpoint */
	if( get_intf->bNumEndpoints != 2 )	{
		PSPRINTF("get invalid num Endpoints = %d\n",get_intf->bNumEndpoints );
		COND_TRAP(31, 0, 0);
	}


//20200805
	if( config_length== 0x20 ){
	get_epdesc[0] = (struct usb_endpoint_descriptor  *)(Device_Descriptor_Return_base
										+ USB_DT_CONFIG_SIZE +USB_DT_INTERFACE_SIZE );
	get_epdesc[1] = (struct usb_endpoint_descriptor  *)(Device_Descriptor_Return_base
										+ USB_DT_CONFIG_SIZE +USB_DT_INTERFACE_SIZE+USB_DT_ENDPOINT_SIZE );
	}
	//usb3.0
	else if( config_length== 0x2C ){
		get_epdesc[0] = (struct usb_endpoint_descriptor  *)(Device_Descriptor_Return_base
											+ USB_DT_CONFIG_SIZE +USB_DT_INTERFACE_SIZE );

		get_epdesc[1] = (struct usb_endpoint_descriptor  *)(Device_Descriptor_Return_base
											+ USB_DT_CONFIG_SIZE +USB_DT_INTERFACE_SIZE+USB_DT_ENDPOINT_SIZE + USB_DT_SS_EP_COMP_SIZE );
	}



	if( (get_epdesc[0]->bDescriptorType != USB_DT_ENDPOINT ) ||
		(get_epdesc[1]->bDescriptorType != USB_DT_ENDPOINT)  ||
		(get_epdesc[0]->bLength != USB_DT_ENDPOINT_SIZE ) ||
		(get_epdesc[1]->bLength != USB_DT_ENDPOINT_SIZE)
	)	{
		PSPRINTF("get invalid Endpoint descriptor, type = 0x%x\n",get_epdesc[0]->bDescriptorType );
		COND_TRAP(31, 0, 0);
	}

	if( (get_epdesc[0]->bEndpointAddress &USB_ENDPOINT_NUMBER_MASK) !=
		(get_epdesc[1]->bEndpointAddress &USB_ENDPOINT_NUMBER_MASK)
	)	{
		PSPRINTF("Be careful: This mass dev, Ep%d=%s,  Ep%d=%s..(diff !!!)\n",
				get_epdesc[0]->bEndpointAddress &USB_ENDPOINT_NUMBER_MASK,
			   (get_epdesc[0]->bEndpointAddress &USB_ENDPOINT_DIR_MASK) ? "In":"Out",
				get_epdesc[1]->bEndpointAddress &USB_ENDPOINT_NUMBER_MASK,
			   (get_epdesc[1]->bEndpointAddress &USB_ENDPOINT_DIR_MASK) ? "In":"Out"
		);
	//	COND_TRAP(31, 0, 0);
	}


/* read the standard strings and cache them if present */
	//udev->product 	=
	//udev->manufacturer=
	//udev->serial 		=

/* important */
	xhci_add_endpoint( xhci, get_epdesc[0] );
	xhci_add_endpoint( xhci, get_epdesc[1] );
#if  XHCI_DEBUG
//test
	PSPRINTF(" after xhci_add_endpoint,Epin_index=%d,EpOut_index=%d\n",
			xhci->conn_dev_EpIn_index ,xhci->conn_dev_EpOut_index );
#endif


/*Configure Endpoint Command
 *  To set a configuration in a device /configure the Device Slot,
 *  software shall issue a Configure Endpoint Command to the xHC
 */
	xhci_configure_endpoint(xhci);


	/* software configure the USB Device itself by issuing a USB SET_CONFIGURATION request
	 *  through the devices?． Default Control Endpoint.
	/* System software selects a specific configuration with a USB SET_CONFIGURATION request. */
	usb_set_configuration(xhci,	get_config->bConfigurationValue  );


/* Configure Endpoint Command shall be used to enable the set of Device Slot endpoints selected by the target
 * 	configuration, and transition a Device Slot from the Addressed to the Configured state.
 */
	//usb_set_device_state(USB_STATE_CONFIGURED)
}

//============== Command =====================================================================
//============= xhci==========================================================================

/* Issue a configure endpoint command
 *   or evaluate context command
 * 		and wait for it to finish.
 */
static void xhci_configure_endpoint(struct xhci_hcd *xhci)
{
	struct xhci_slot_ctx    *slot_ctx;
	struct xhci_ep_ctx		*ep0_ctx,*EpOut_ctx,*EpIn_ctx;
	struct xhci_input_control_ctx *ctrl_ctx;
	UINT32			i,tmp,Ep_entries_in_slot_ctx,ctx_dci;

/* (Input) Context:  init in xhci_setup_device()
 * 'Configure Endpoint' TRB has 'Input Context Pointer' field
 * */


/* update ep ctx <-- already add ep x <--xhci_add_endpoint()
 *
 */


/* update Input Control Context */
#if 1
	ctrl_ctx = xhci_get_input_control_ctx(xhci->in_ctx);

	/* which Device Context data structures be evaluated and/or enabled by a command */
	//ctrl_ctx->add_flags:  already add ep in/ep out <--xhci_add_endpoint()
//	ctrl_ctx->add_flags |= cpu_to_le32(SLOT_FLAG);
//	ctrl_ctx->add_flags &= cpu_to_le32(~EP0_FLAG);  // ep0 already be added ????

//	ctrl_ctx->add_flags |= cpu_to_le32( (UINT32)(EP0_FLAG| SLOT_FLAG)  );
	ctrl_ctx->add_flags |= cpu_to_le32( (UINT32)( SLOT_FLAG)  );
	ctrl_ctx->drop_flags &= cpu_to_le32(~(SLOT_FLAG | EP0_FLAG));
#endif

/* update slot_ctx */
#if 1
	// Context Entries field
	slot_ctx = xhci_get_slot_ctx(xhci, xhci->in_ctx);

	for (i = 31; i >= 1; i--)
	{
	//	tmp = cpu_to_le32( 1 <<i );
	//	if ( ctrl_ctx->add_flags & tmp )  //find which bit is 1 in Add Context flags (A31 - A0).

		tmp = ( 1 <<i );
		if ( cpu_to_le32( ctrl_ctx->add_flags) & tmp )
		{
			slot_ctx->dev_info &= cpu_to_le32(~LAST_CTX_MASK);
			slot_ctx->dev_info |= cpu_to_le32(LAST_CTX(i) );  //i=4
			break;
		}
	}
#endif

#if  0//XHCI_DEBUG
//test
	//Input Control Context:0x0,0x19
	PSPRINTF("In_crtl 0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base+0x00)), le32_to_cpu(readl(Input_Context_Slot1_Base+0x04))  );
	//Slot Context:  0x20300000,0x10000,0x0
	ctx_dci = 0;
	PSPRINTF("In_slot 0x%x,0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x04)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x08))   );
	//ep0 Context
	ctx_dci = 1;
	PSPRINTF("In_ep0 0x%x,0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x04)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x08))   );

	//ep in Context
	ctx_dci = xhci->conn_dev_EpIn_index + 1;
	PSPRINTF("In_EpOut 0x%x,0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x04)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x08))   );

	//ep out Context
	ctx_dci = xhci->conn_dev_EpOut_index + 1;
	PSPRINTF("In_EpIn 0x%x,0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x04)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x08))   );
#endif

#if 1
	/* Queue a 'Configure Endpoint Command (P)' on the command ring */
	queue_command(xhci,
					(UINT32)xhci->in_ctx->bytes,	//Input_Context_Slot1_Base,
					XHCI_BUFFERs_BASE_HI,
					0,
					TRB_TYPE(TRB_CONFIG_EP) | SLOT_ID_FOR_TRB(xhci->cur_slot_id),
					false);

	/* Host Controller Doorbell (0) */
	xhci_ring_cmd_db(xhci);

	/* ctrl tx can take up to 5 sec; XXX: need more time for xHC? */
	while( xhci->cmd_completion ==0);  //->handle_cmd_completion()
	xhci->cmd_completion =0;

#endif


/* analysis event and output CTX(return) */
	/* Zero the input context control for later use */
	//ctrl_ctx->add_flags  = 0;
	//ctrl_ctx->drop_flags = 0;

	// read out_ctx	to software
	slot_ctx = xhci_get_slot_ctx(xhci, xhci->out_ctx);
	ep0_ctx  = xhci_get_ep_ctx(xhci, xhci->out_ctx, 0);
	EpIn_ctx  = xhci_get_ep_ctx(xhci, xhci->out_ctx, xhci->conn_dev_EpIn_index );
	EpOut_ctx = xhci_get_ep_ctx(xhci, xhci->out_ctx, xhci->conn_dev_EpOut_index );
#if 0//XHCI_DEBUG
	PSPRINTF("xhci_configure_endpoint()/return out_ctx->slot_ctx = 0x%x/0x%x/0x%x/0x%x/(slot state='%s')\n",
		le32_to_cpu(slot_ctx->dev_info),	le32_to_cpu(slot_ctx->dev_info2),
		le32_to_cpu(slot_ctx->tt_info),		le32_to_cpu(slot_ctx->dev_state),
		xhci_slot_state_string( GET_SLOT_STATE( le32_to_cpu(slot_ctx->dev_state)) )
		   );

	PSPRINTF("xhci_configure_endpoint()/return out_ctx->ep0_ctx = 0x%x/0x%x/0x%x/0x%x/(ep0 state='%s')\n",
		le32_to_cpu(ep0_ctx->ep_info),		le32_to_cpu(ep0_ctx->ep_info2),
		le32_to_cpu(ep0_ctx->TR_deq_Lo),	le32_to_cpu(ep0_ctx->TR_deq_Hi),
		xhci_ep_state_string( (unsigned char)GET_EP_CTX_STATE(ep0_ctx) )
		   );

//		PSPRINTF("EpIn_ctx addr = 0x%x/EpOut_ctx addr = 0x%x\n",EpIn_ctx,EpOut_ctx );
		//EpIn_ctx addr = 0xe03460c0/EpOut_ctx addr = 0xe0346100

	PSPRINTF("xhci_configure_endpoint()/return out_ctx->EpIn_ctx = 0x%x/0x%x/0x%x/0x%x/(EpIn state='%s')\n",
		le32_to_cpu(EpIn_ctx->ep_info),		le32_to_cpu(EpIn_ctx->ep_info2),
		le32_to_cpu(EpIn_ctx->TR_deq_Lo),	le32_to_cpu(EpIn_ctx->TR_deq_Hi),
		xhci_ep_state_string( (unsigned char)GET_EP_CTX_STATE(EpIn_ctx) )
		   );

	PSPRINTF("xhci_configure_endpoint()/return out_ctx->EpOut_ctx = 0x%x/0x%x/0x%x/0x%x/(EpOut state='%s')\n",
		le32_to_cpu(EpOut_ctx->ep_info),	le32_to_cpu(EpOut_ctx->ep_info2),
		le32_to_cpu(EpOut_ctx->TR_deq_Lo),	le32_to_cpu(EpOut_ctx->TR_deq_Hi),
		xhci_ep_state_string( (unsigned char)GET_EP_CTX_STATE(EpOut_ctx) )
		   );
#endif

}


/* Add an endpoint to a new possible bandwidth configuration for this device.
 * 	for example,add Ep1 in/Ep2 out,
 * 		include add Input Context[EP Context n]/Input Control Context[Add]
 */
static void xhci_add_endpoint(struct xhci_hcd *xhci, struct usb_endpoint_descriptor  *ep_desc)
{
	UINT32 added_ctxs;
	UINT32 ep_index				= xhci_get_endpoint_index(ep_desc); //1/2  //K: 2/3
	UINT32 ep_type_in_Ep_Ctx	= xhci_get_endpoint_type_in_Ep_Ctx(ep_desc);
	UINT32 max_packet			= usb_endpoint_maxp(ep_desc);
	UINT32 tmp;
	struct xhci_ep_ctx *ep_ctx;

	struct xhci_input_control_ctx *ctrl_ctx;
	UINT32 add_which_ctxs_flg;  //in Input Control Context[Add Context flags (A0 - A31).]

	ctrl_ctx = xhci_get_input_control_ctx(xhci->in_ctx);
	add_which_ctxs_flg = 1 << (ep_index + 1); 		//important
	ctrl_ctx->add_flags |= cpu_to_le32(add_which_ctxs_flg);  //-->used in xhci_configure_endpoint()
	ctrl_ctx->drop_flags = 0;

	switch ( ep_desc->bEndpointAddress & USB_ENDPOINT_DIR_MASK )
	{
	case USB_DIR_IN:	//Ep x in
		{
			xhci->conn_dev_EpIn_index = ep_index;  //important

			#if 1
			/* Set up the endpoint n ring
				 *   Set up the Transfer ring (to have one segments for now.)
				 * */
				xhci->ep_ring[ep_index] = (struct xhci_ring*) (Ep_in_Ring_Struct_Base);
				xhci->ep_ring[ep_index]->num_segs = 1;
				xhci->ep_ring[ep_index]->type =  usb_endpoint_type(ep_desc);//TYPE_BULK;

				xhci->ep_ring[ep_index]->first_seg = (struct xhci_segment*) (Ep_in_Ring_Segment_Struct_Base);
				xhci->ep_ring[ep_index]->first_seg->trbs = (union xhci_trb*)(Ep_in_Ring_Segment0_BaseLo);	//useful/=Producer
				xhci->ep_ring[ep_index]->first_seg->next = xhci->ep_ring[ep_index]->first_seg;
				/*	the last TRB in segment is LINK TRB	 */
				if (xhci->ep_ring[ep_index]->type != TYPE_EVENT)
				{
					/* 1. LINK TRB[Ring Segment Pointer],  points to the DMA address of the next segment(= first,only 1) */
					xhci->ep_ring[ep_index]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Lo = cpu_to_le32(Ep_in_Ring_Segment0_BaseLo);
					xhci->ep_ring[ep_index]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Hi = cpu_to_le32(XHCI_BUFFERs_BASE_HI);

					/* 2.LINK TRB[TRB Type],  Set the last TRB in the segment to have a TRB type ID of Link TRB */
					tmp = TRB_TYPE(TRB_LINK) | LINK_TOGGLE ;
					xhci->ep_ring[ep_index]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.control = cpu_to_le32(tmp);
				}

				//if xhci->cmd_ring->num_segs >1, need xhci_segment_alloc() for insert other segment
				xhci->ep_ring[ep_index]->last_seg = xhci->ep_ring[ep_index]->first_seg;

				/* The ring is empty, so the enqueue pointer == dequeue pointer */
				xhci->ep_ring[ep_index]->enqueue = xhci->ep_ring[ep_index]->first_seg->trbs;
				xhci->ep_ring[ep_index]->enq_seg = xhci->ep_ring[ep_index]->first_seg;

				xhci->ep_ring[ep_index]->dequeue = xhci->ep_ring[ep_index]->enqueue;
				xhci->ep_ring[ep_index]->deq_seg = xhci->ep_ring[ep_index]->first_seg;

				xhci->ep_ring[ep_index]->cycle_state = 1;
			#endif

			/* Fill in Endpoint Context */
			#if 1
				ep_ctx = xhci_get_ep_ctx(xhci, xhci->in_ctx, ep_index );

				ep_ctx->ep_info = 0x00000000;

				ep_ctx->ep_info2 = cpu_to_le32(EP_TYPE(ep_type_in_Ep_Ctx));
				/* EP x  Max Burst Size field is ??? */
				ep_ctx->ep_info2 |= cpu_to_le32(  (UINT32)(MAX_BURST(0)  //ep x burst = ???
												| ERROR_COUNT(3) |  MAX_PACKET(max_packet))   );

				ep_ctx->TR_deq_Lo = cpu_to_le32( (UINT32)(Ep_in_Ring_Segment0_BaseLo | xhci->ep_ring[ep_index]->cycle_state) );
				ep_ctx->TR_deq_Hi = cpu_to_le32( XHCI_BUFFERs_BASE_HI  );
			#endif
		}
		break;

	case USB_DIR_OUT: //Ep x out
	default:
		{
			xhci->conn_dev_EpOut_index = ep_index;

			#if 1
			/* Set up the endpoint n ring
				 *   Set up the Transfer ring (to have one segments for now.)
				 * */
				xhci->ep_ring[ep_index] = (struct xhci_ring*) (Ep_out_Ring_Struct_Base);
				xhci->ep_ring[ep_index]->num_segs = 1;
				xhci->ep_ring[ep_index]->type =  usb_endpoint_type(ep_desc);//TYPE_BULK;

				xhci->ep_ring[ep_index]->first_seg = (struct xhci_segment*) (Ep_out_Ring_Segment_Struct_Base);
				xhci->ep_ring[ep_index]->first_seg->trbs = (union xhci_trb*)(Ep_out_Ring_Segment0_BaseLo);	//useful/=Producer
				xhci->ep_ring[ep_index]->first_seg->next = xhci->ep_ring[ep_index]->first_seg;
				/*	the last TRB in segment is LINK TRB	 */
				if (xhci->ep_ring[ep_index]->type != TYPE_EVENT)
				{
					/* 1. LINK TRB[Ring Segment Pointer],  points to the DMA address of the next segment(= first,only 1) */
					xhci->ep_ring[ep_index]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Lo = cpu_to_le32(Ep_out_Ring_Segment0_BaseLo);
					xhci->ep_ring[ep_index]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Hi = cpu_to_le32(XHCI_BUFFERs_BASE_HI);

					/* 2.LINK TRB[TRB Type],  Set the last TRB in the segment to have a TRB type ID of Link TRB */
					tmp = TRB_TYPE(TRB_LINK) | LINK_TOGGLE ;
					xhci->ep_ring[ep_index]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.control = cpu_to_le32(tmp);
				}

				//if xhci->cmd_ring->num_segs >1, need xhci_segment_alloc() for insert other segment
				xhci->ep_ring[ep_index]->last_seg = xhci->ep_ring[ep_index]->first_seg;

				/* The ring is empty, so the enqueue pointer == dequeue pointer */
				xhci->ep_ring[ep_index]->enqueue = xhci->ep_ring[ep_index]->first_seg->trbs;
				xhci->ep_ring[ep_index]->enq_seg = xhci->ep_ring[ep_index]->first_seg;

				xhci->ep_ring[ep_index]->dequeue = xhci->ep_ring[ep_index]->enqueue;
				xhci->ep_ring[ep_index]->deq_seg = xhci->ep_ring[ep_index]->first_seg;

				xhci->ep_ring[ep_index]->cycle_state = 1;
			#endif

			/* Fill in Endpoint Context */
			#if 1
				ep_ctx = xhci_get_ep_ctx(xhci, xhci->in_ctx, ep_index );

				ep_ctx->ep_info = 0x00000000;

				ep_ctx->ep_info2 = cpu_to_le32(EP_TYPE(ep_type_in_Ep_Ctx));
				/* EP x  Max Burst Size field is ??? */
				ep_ctx->ep_info2 |= cpu_to_le32(  (UINT32)(MAX_BURST(0)  //ep x burst = ???
												| ERROR_COUNT(3) |  MAX_PACKET(max_packet))   );

				ep_ctx->TR_deq_Lo = cpu_to_le32( (UINT32)(Ep_out_Ring_Segment0_BaseLo | xhci->ep_ring[ep_index]->cycle_state) );
				ep_ctx->TR_deq_Hi = cpu_to_le32( XHCI_BUFFERs_BASE_HI  );
			#endif
		}
		break;
	}

}


/* Queue a 'TRB_ADDR_DEV (P)' on the command ring */
static void xhci_setup_device(struct xhci_hcd *xhci,
			     enum xhci_setup_dev setup)
{
	const char *act = setup == SETUP_CONTEXT_ONLY ? "context" : "address";
	struct xhci_slot_ctx    *slot_ctx;
	struct xhci_ep_ctx		*ep0_ctx;
	struct xhci_input_control_ctx *ctrl_ctx;
	UINT32			max_packets,tmp;
#if  0//XHCI_DEBUG
	UINT32 ctx_dci ;
#endif

/* init (Input) Context:
 * 'Address Device Command' TRB has 'Input Context Pointer' field
 * */
	slot_ctx = xhci_get_slot_ctx(xhci, xhci->in_ctx);

/*
 * If this is the first Set Address since device plug-in or
 * virt_device realloaction after a resume with an xHCI power loss,
 * 		then set up the slot context.
 */
if (!slot_ctx->dev_info)
{	// 1st-yes

	/* Fill in Slot Context */
#if 1
	slot_ctx->dev_info = cpu_to_le32( (UINT32)(LAST_CTX(1) | (0 <<0)) ); // now,Only the control endpoint is valid
	switch (xhci->conn_dev_speed)
	{
	case USB_SPEED_SUPER:
		slot_ctx->dev_info |= cpu_to_le32(SLOT_SPEED_SS);//---------------
		max_packets = MAX_PACKET(512);
		break;
	case USB_SPEED_HIGH:
		slot_ctx->dev_info |= cpu_to_le32(SLOT_SPEED_HS);//---------------
		max_packets = MAX_PACKET(64);
		break;
	/* USB core guesses at a 64-byte max packet first for FS devices */
	case USB_SPEED_FULL:
		slot_ctx->dev_info |= cpu_to_le32(SLOT_SPEED_FS);
		max_packets = MAX_PACKET(64);
		break;
	case USB_SPEED_LOW:
		slot_ctx->dev_info |= cpu_to_le32(SLOT_SPEED_LS);
		max_packets = MAX_PACKET(8);
		break;
	default:
		/* Speed was set earlier, this shouldn't happen. */
		return;
	}

	/* Find the root hub port this device is under */
	slot_ctx->dev_info2 = cpu_to_le32(ROOT_HUB_PORT(xhci->cur_port_id));
	slot_ctx->tt_info = 0x00000000;
	slot_ctx->dev_state = 0x00000000;
#endif

/* Fill in Endpoint Context */
#if 1
	ep0_ctx = xhci_get_ep_ctx(xhci, xhci->in_ctx, 0);

	ep0_ctx->ep_info = 0x00000000;

	ep0_ctx->ep_info2 = cpu_to_le32(EP_TYPE(CTRL_EP));
	/* EP 0 can handle "burst" sizes of 1, so Max Burst Size field is 0 */
	ep0_ctx->ep_info2 |= cpu_to_le32(  (UINT32)(MAX_BURST(0) | ERROR_COUNT(3) | max_packets)  );

	ep0_ctx->TR_deq_Lo = cpu_to_le32( (UINT32)(Ep0_Ring_Segment0_BaseLo | xhci->ep_ring[0]->cycle_state) ); //1st
	ep0_ctx->TR_deq_Hi = cpu_to_le32( XHCI_BUFFERs_BASE_HI  ); //only pointer to,not use Trans TRB here

#endif
}
/* Otherwise, update the control endpoint ring enqueue pointer. */
else
{
	ep0_ctx = xhci_get_ep_ctx(xhci, xhci->in_ctx, 0);

	//xhci_copy_ep0_dequeue_into_input_ctx(xhci, udev);
	tmp = trb_offset_to_addr( xhci->ep_ring[0]->enq_seg,xhci->ep_ring[0]->enqueue  ); //not 1st

	ep0_ctx->TR_deq_Lo = cpu_to_le32(  (UINT32)(tmp
									  | xhci->ep_ring[0]->cycle_state)
									);
}

/* Fill in Input Control Context */
#if 1
	ctrl_ctx = xhci_get_input_control_ctx(xhci->in_ctx);

	/* which Device Context data structures be evaluated and/or enabled by a command */
	ctrl_ctx->add_flags = cpu_to_le32( (UINT32)(SLOT_FLAG | EP0_FLAG) );
	ctrl_ctx->drop_flags = 0;
#endif



#if  0//XHCI_DEBUG
//test
	//in crtl 	0x0,0x3
	PSPRINTF("in crtl 0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base)), le32_to_cpu(readl(Input_Context_Slot1_Base+0x04))  );
	//slot	0x8300000,0x10000,0x0
	ctx_dci = 0;
	PSPRINTF("in slot 0x%x,0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x04)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x08))   );
	//ep0	0x0,0x400026,0xe0343001
	ctx_dci = 1;
	PSPRINTF("in ep0 0x%x,0x%x,0x%x\n",le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x04)),
			le32_to_cpu(readl(Input_Context_Slot1_Base+64 +ctx_dci*64	+0x08))   );
	//0xe0344800,0xe0344800
	PSPRINTF("in ctx addr 0x%x,0x%x\n",(UINT32)xhci->in_ctx->bytes, Input_Context_Slot1_Base  );
#endif

#if 1
	/* Queue a 'TRB_ADDR_DEV (P)' on the command ring */
	queue_command(xhci,
					(UINT32)xhci->in_ctx->bytes,	//Input_Context_Slot1_Base,
					XHCI_BUFFERs_BASE_HI,
					0,
					TRB_TYPE(TRB_ADDR_DEV) | SLOT_ID_FOR_TRB(xhci->cur_slot_id)
					| (setup == SETUP_CONTEXT_ONLY ? TRB_BSR : 0),
					false);

	/* Host Controller Doorbell (0) */
	xhci_ring_cmd_db(xhci);

	/* ctrl tx can take up to 5 sec; XXX: need more time for xHC? */
	while( xhci->cmd_completion ==0);  //->handle_cmd_completion()
	xhci->cmd_completion =0;

#endif


/* analysis event and output CTX(return) */
	/* Zero the input context control for later use */
	ctrl_ctx->add_flags  = 0;
	ctrl_ctx->drop_flags = 0;

	// read out_ctx	to software
	slot_ctx = xhci_get_slot_ctx(xhci, xhci->out_ctx);
	ep0_ctx  = xhci_get_ep_ctx(xhci, xhci->out_ctx, 0);
#if 0//XHCI_DEBUG
	PSPRINTF("xhci_enable_device()/return out_ctx->slot_ctx = 0x%x/0x%x/0x%x/0x%x/(slot state='%s')\n",
		le32_to_cpu(slot_ctx->dev_info),	le32_to_cpu(slot_ctx->dev_info2),
		le32_to_cpu(slot_ctx->tt_info),		le32_to_cpu(slot_ctx->dev_state),
		xhci_slot_state_string( GET_SLOT_STATE( le32_to_cpu(slot_ctx->dev_state)) )
		   );

	PSPRINTF("xhci_enable_device()/return out_ctx->ep0_ctx = 0x%x/0x%x/0x%x/0x%x/(ep0 state='%s')\n",
		le32_to_cpu(ep0_ctx->ep_info),		le32_to_cpu(ep0_ctx->ep_info2),
		le32_to_cpu(ep0_ctx->TR_deq_Lo),	le32_to_cpu(ep0_ctx->TR_deq_Hi),
		xhci_ep_state_string( (unsigned char)GET_EP_CTX_STATE(ep0_ctx) )
		   );
#endif
//	if( le32_to_cpu(slot_ctx->dev_state) &  DEV_ADDR_MASK ) {
	if( act == "address" ) {

		PSPRINTF("	new SET_ADDRESS request for dev = %d.\n",  le32_to_cpu(slot_ctx->dev_state) &  DEV_ADDR_MASK );
	}

}

/*
 * Issue an Address Device command
 */
static void xhci_address_device(struct xhci_hcd *xhci)
{
	xhci_setup_device(xhci,
					SETUP_CONTEXT_ADDRESS   // 2nd-> BSR=0
					);


/* When in the Enabled state, the slot shall transition to the Addressed state due to the successful completion
 * 	of an Address Device Command with the Block Set Address Request (BSR) flag cleared to ??0?．.
 */
	//Slot States:Addressed state
}


/*
 * Issue an Address Device command
 */
static void xhci_enable_device(struct xhci_hcd *xhci)
{
	//if (udev->state != USB_STATE_DEFAULT)
	//	return -EINVAL;

	xhci_setup_device(xhci,
					SETUP_CONTEXT_ONLY   // 1st-> BSR=1
					);
/*
 * When in the Enabled state, the slot shall transition to the Default state due to the successful completion of
 * an Address Device Command with the Block Set Address Request (BSR) flag set to ??1?．.
 */
	//Slot States:Default state
}


/* TRB_RESET_DEV:
	Queue a Reset Device command TRB */
void xhci_queue_reset_device(struct xhci_hcd *xhci)
{
	queue_command(xhci,
				 0, 	0,
				 0,  	TRB_TYPE(TRB_RESET_DEV) | SLOT_ID_FOR_TRB(xhci->cur_slot_id),
				false);

}


/*
 * A USB2 protocol port requires software to reset the 'port' to advance the port to the Enabled state
 *	 and a USB device from the Powered state to the Default state.
 */
static void port_reset(struct xhci_hcd *xhci, UINT8 flg)
{
	UINT32 portsc;
#if	U2_HOST_TEST
	//--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^66
	portsc = (xhci->cur_port_id == xhci->usb2_port_id)?
				(readl( (UINT32)&xhci->op_regs->port1_status_base )) :  //0x6e1->0x200e03->0x00e03
				(readl( (UINT32)&xhci->op_regs->port2_status_base )) ;  //0x21203->0x1203->...

	portsc = (portsc | PORT_RESET);

	(xhci->cur_port_id == xhci->usb2_port_id)?
			writel( (UINT32)&xhci->op_regs->port1_status_base,portsc ) :
			writel( (UINT32)&xhci->op_regs->port2_status_base,portsc ) ;

	//port_wait_reset
	while( connect_change !=3 ){}	//->xhci_irq()
	connect_change = 0;
#endif

#if 1
	portsc = (xhci->cur_port_id == xhci->usb2_port_id)?
				(readl( (UINT32)&xhci->op_regs->port1_status_base )) :  //0x206e1
				(readl( (UINT32)&xhci->op_regs->port2_status_base )) ;  //0x1203
	if ( xhci->cur_port_id == xhci->usb3_port_id ) {
		if( DEV_SUPERSPEED(portsc)  )
			xhci->conn_dev_speed = USB_SPEED_SUPER;  //tmp ?
	}
	else if( DEV_HIGHSPEED(portsc)  )
		xhci->conn_dev_speed = USB_SPEED_HIGH;
	else if ( DEV_LOWSPEED(portsc) )
		xhci->conn_dev_speed = USB_SPEED_LOW;
	else if ( DEV_FULLSPEED(portsc)  )
		xhci->conn_dev_speed = USB_SPEED_FULL;
	else {
		xhci->conn_dev_speed = USB_SPEED_UNKNOWN;
		COND_TRAP(31, 0, 0);
	}
#endif

#if 1
	if( flg ==2) {
		/* The Reset Device Command
		 * 		TRB is used by software to inform the xHC that a USB device has been Reset.
		 */
		xhci_queue_reset_device(xhci);

		/* Host Controller Doorbell (0) */
		xhci_ring_cmd_db(xhci);

		while( xhci->cmd_completion ==0);  //->xhci_irq()->handle_cmd_completion
		xhci->cmd_completion =0;
	}
#endif


	/*
	 * The reset operation sets the device slot to the Default state,
	 * sets the Device Address to ??0?．, and disables all endpoints except for the Default Control Endpoint.
	 */
	//usb_set_device_state(udev, USB_STATE_DEFAULT);
}


/*	 Enable Slot CMD
 * Returns 0 if the xHC ran out of device slots, the Enable Slot command
 * timed out, or allocating memory failed.  Returns 1 on success.
 */
void xhci_alloc_dev(struct xhci_hcd *xhci)
{
	UINT16 i;

	/* Queue a 'slot enable-TRB_ENABLE_SLOT' on the command ring */
	queue_command(xhci,
					0, 	0,
					0,	TRB_TYPE(TRB_ENABLE_SLOT) | SLOT_ID_FOR_TRB(0),
					false);

	/* Host Controller Doorbell (0) */
	xhci_ring_cmd_db(xhci);

	while( xhci->cmd_completion ==0);  //->xhci_irq()->handle_cmd_completion
	xhci->cmd_completion =0;

	#if XHCI_DEBUG
		PSPRINTF("xhci_alloc_dev()/get slot_id = %d\n",xhci->cur_slot_id  );
	#endif

/*
 * When slot in the Disabled state, the slot shall transition to the Enabled state
 * due to the successful completion of an Enable Slot Command.
 */
	//Slot States:Enabled


/* Once an xHC Device Slot ID has been obtained for a USB device,
 * software shall initialize the data structures associated with the slot.
 */
	for (i = 0; i < MAX_HC_SLOTS; i++)
	{
		/* Set base addr for struct:  Device Context Base Address Array Entries(xhci->dcbaa_entry) */
		xhci->dcbaa_entry[i] = (struct xhci_device_context_Entry*) (
									(DCBAA_Entries_Base_LO & (~(0x40-1)))
									+ i*(sizeof(struct xhci_device_context_Entry))   );

		/* init */
		writel( (UINT32)&xhci->dcbaa_entry[i]->dev_context_ptrs_LO, 0x00000000 );
		writel( (UINT32)&xhci->dcbaa_entry[i]->dev_context_ptrs_Hi, 0x00000000 );
	}
	/* Fill Elements (slot id) for Device Context Base Address Array */
	writel( (UINT32)&xhci->dcbaa_entry[xhci->cur_slot_id]->dev_context_ptrs_LO, cpu_to_le32( Device_Context_Slot1_Base)  );
	writel( (UINT32)&xhci->dcbaa_entry[xhci->cur_slot_id]->dev_context_ptrs_Hi, cpu_to_le32( XHCI_BUFFERs_BASE_HI)  );
#if (XHCI_DEBUG)
	i = xhci->cur_slot_id;
	PSPRINTF( "Device Context Base Address Array: Entry [%d] =0x%x + 0x%x\n",
			i,
			le32_to_cpu( readl( DCBAA_Entries_Base_LO + i*(sizeof(struct xhci_device_context_Entry)) + 0x04 )  ),
			le32_to_cpu( readl( DCBAA_Entries_Base_LO + i*(sizeof(struct xhci_device_context_Entry)) + 0x00 )  )
				);  // =0x80000000 + 0xe0343000
#endif

/* Allocate the (output) device context (struct) that will be used in the HC. */
	xhci->out_ctx = (struct xhci_container_ctx*) (Out_ctx_Struct_Base);
	xhci->out_ctx->type = XHCI_CTX_TYPE_DEVICE;
	xhci->out_ctx->size = HCC_64BYTE_CONTEXT(xhci->hcc_params) ? 2048 : 1024;  //2048 = 64B*  32(DCI)
	xhci->out_ctx->bytes = (UINT8*)(Device_Context_Slot1_Base);

/* Allocate the (input) device context (struct) for xxxxx command */
	xhci->in_ctx = (struct xhci_container_ctx*) (In_ctx_Struct_Base);
	xhci->in_ctx->type = XHCI_CTX_TYPE_INPUT;
	xhci->in_ctx->size = HCC_64BYTE_CONTEXT(xhci->hcc_params) ? 2048 : 1024;
	if ( xhci->in_ctx->type == XHCI_CTX_TYPE_INPUT) {
		xhci->in_ctx->size +=  CTX_SIZE(xhci->hcc_params); //2048+64(Input Control CTX) = 2112
	}
	xhci->in_ctx->bytes = (UINT8*)(Input_Context_Slot1_Base);
}


//============================================================================================
//============= 4.3 USB Device Initialization =====================================================================

static void xhci_port_connect(struct xhci_hcd *xhci)
{
#if 1
	struct usb_device_descriptor 	*get_desc;
#endif

	/* reallocate for each attempt, since references
	 * to the previous one can escape in various ways
	 */
	xhci_alloc_dev(xhci);	//alloc space for inserted dev
	//usb_set_device_state(udev, USB_STATE_POWERED);

	/*
	 * Reset the device; full speed may morph to high speed */
	port_reset(xhci , 1);    //1st port reset
	delay(0x10000);

	if( xhci->conn_dev_speed < USB_SPEED_SUPER  ) {
		PSPRINTF("Insert one new (%s) USB device,xhci port(%d) in (Enable) state.\n",
				speed_names[xhci->conn_dev_speed],
				xhci->cur_port_id	);

		xhci_enable_device(xhci);


		//-------1st time GET_DESCRIPTOR: 64bytes
		usb_get_device_descriptor( xhci,(struct usb_device_descriptor *)(Device_Descriptor_Return_base),   64 );
		#if (XHCI_DEBUG)
			get_desc = (struct usb_device_descriptor *)(Device_Descriptor_Return_base);
			PSPRINTF(" get des=0x%x,0x%x,0x%x,0x%x\n",
					get_desc->bLength,get_desc->bDescriptorType,get_desc->bcdUSB,get_desc->bMaxPacketSize0 );
		#endif

		/* Reset bus again */
		port_reset(xhci ,2);    //2nd port reset
		delay(0x10000);
		
		
	
	}

	xhci_address_device(xhci); //set device address
	if (xhci->conn_dev_speed >= USB_SPEED_SUPER)	{
		PSPRINTF("Insert one new (%s) USB device,xhci port(%d) in (Enable) state.\n",
				speed_names[xhci->conn_dev_speed],
				xhci->cur_port_id	);
	}


	usb_enumerate_device(xhci);


	/* Now that all the interfaces are set up, register them
	 * to trigger binding of drivers to interfaces.
	 * probe() routines may install different altsettings and may
	 * claim() any interfaces not yet bound.  Many class drivers
	 * need that: CDC, audio, video, etc.
	 */
}

//============================================================================================
//?y============= xhci memory basic ===========================================================
/*
 * Reset a halted HC.
 *
 * This resets pipelines, timers, counters, state machines, etc.
 * Transactions will be terminated immediately, and operational registers
 * will be set to their defaults.
 */
char xhci_reset(struct xhci_hcd *xhci)
{
	UINT32 command;
	UINT32 state;
	int ret, i;

	state = readl( (UINT32)&xhci->op_regs->status);
	PSPRINTF("xHCI USBSTS = 0x%x\n", state);
	if (state == ~(UINT32)0) {
		PSPRINTF("Host not accessible, reset failed.\n");
		return -1;
	}

	/* Software shall not reset the host controller,
			when the HCHalted (HCH) bit in the USBSTS register is a '0'
	*/
	if ((state & STS_HALT) == 0) {
		PSPRINTF("Host controller not halted, aborting reset.\n");
		return 0;
	}

	command = readl( (UINT32)&xhci->op_regs->command);
	command |= CMD_RESET;
	writel( (UINT32)&xhci->op_regs->command,command );
	delay(0x1000);
	state = readl( (UINT32)&xhci->op_regs->command);
	while( state & CMD_RESET  ){
		state = readl( (UINT32)&xhci->op_regs->command);
	}

	return 0;
}


/*
 * 4.2 Host Controller Initialization
*/
void xhci_mem_init(struct xhci_hcd *xhci)
{
    UINT32 rddat;
    UINT8 i;

#if 1
    // Issue Light Host controller reset
    //****************************************************************************
    // tmp_data[31:12] = 0; //Reserved
    // tmp_data[11] = 0; 	//Enable U3 MFINDEX Stop
    // tmp_data[10] = 0; 	//Enable Wrap Event
    // tmp_data[9] = 0; 	//Controller Restore State
    // tmp_data[8] = 0; 	//Controller Save States
    // tmp_data[7] = 1; 	//Light Host Controller Reset
    // tmp_data[6:4] = 0; 	//Reserved
    // tmp_data[3] = 0; 	//Host System Error Enable
    // tmp_data[2] = 1; 	//Interrupter Enable
    // tmp_data[1] = 0; 	//Host Controller Reset
    // tmp_data[0] = 0; 	//Run/Stop
    //****************************************************************************
    writel( (UINT32)&xhci->op_regs->command, CMD_LRESET );
    rddat = readl( (UINT32)&xhci->op_regs->command );
    while(rddat & CMD_LRESET) //wait for xHC light host reset to be cleared
    {
    	rddat = readl( (UINT32)&xhci->op_regs->command );
		delay(2000);
    }
#endif

//----------------------------------------------------------------------------
/*	CONFIG:
 * Program the Number of Device Slots Enabled field in the CONFIG
 * register with the max value of slots the HC can handle.
 */
    rddat = HCS_MAX_SLOTS( readl( (UINT32)&xhci->cap_regs->hcs_params1) ); //0x40
#if 0//XHCI_DEBUG
	PSPRINTF( "config_reg default =0x%x\n",  readl( (UINT32)&xhci->op_regs->config_reg)  );//0
#endif
//	writel( (UINT32)&xhci->op_regs->config_reg, rddat );
	writel( (UINT32)&xhci->op_regs->config_reg, MAX_HC_SLOTS );
#if 0//XHCI_DEBUG
	PSPRINTF( "config_reg =0x%x\n",  readl( (UINT32)&xhci->op_regs->config_reg)  ); //4
#endif

//----------------------------------------------------------------------------
/* DCBAAP:	64-byte aligned
 * 	Program the Device Context Base Address Array Pointer (DCBAAP) register with a 64-bit
 * 	address pointing to where the Device Context Base Address Array is located.
 */
	writel( (UINT32)&xhci->op_regs->dcbaa_ptr_Lo ,DCBAA_Entries_Base_LO & (~(0x40-1)) );
	writel( (UINT32)&xhci->op_regs->dcbaa_ptr_Hi ,XHCI_BUFFERs_BASE_HI );
#if (XHCI_DEBUG)
	PSPRINTF( "DCBAA_ENTRY_BASE =0x%x + 0x%x\n",
			readl( (UINT32)&xhci->op_regs->dcbaa_ptr_Hi),readl( (UINT32)&xhci->op_regs->dcbaa_ptr_Lo)  );
#endif

//fill Device Context Base Address Array->xhci_alloc_dev()
//fill context --> 	xhci_setup_device()

//----------------------------------------------------------------------------
 /* Event ring setup:
 *    Allocate a normal Event ring (to have one Event ring segment)
 */
	xhci->event_ring = (struct xhci_ring*) (Event_Ring_Struct_Base);
	xhci->event_ring->num_segs = ERST_NUM_SEGS;
	xhci->event_ring->type = TYPE_EVENT;

	xhci->event_ring->first_seg = (struct xhci_segment*) (Event_Ring_Segment_Struct_Base);
	xhci->event_ring->first_seg->trbs = (union xhci_trb*)(Event_Ring_Segment0_BaseLo);	//useful/ = Producer
	xhci->event_ring->first_seg->next = xhci->event_ring->first_seg;
	 /* Only event ring does not use link TRB */
	xhci->event_ring->last_seg = xhci->event_ring->first_seg;

	/* The ring is empty, so the enqueue pointer == dequeue pointer */
	xhci->event_ring->enqueue = xhci->event_ring->first_seg->trbs;
	xhci->event_ring->enq_seg = xhci->event_ring->first_seg;

	xhci->event_ring->dequeue = xhci->event_ring->enqueue;		//useful/variable
	xhci->event_ring->deq_seg = xhci->event_ring->first_seg;	//useful

	/* The ring is initialized to 0. The producer must write 1 to the cycle
	 * bit to handover ownership of the TRB, so PCS = 1.  The consumer must
	 * compare CCS to the cycle bit to check ownership, so CCS = 1.
	 *
	 * New rings are initialized with cycle state equal to 1; if we are
	 * handling ring expansion, set the cycle state equal to the old ring.
	 */
	xhci->event_ring->cycle_state = 1;


/* Fill Event ring segment table (for xhci->ir_set[0])
 */
	for (i = 0; i < ERST_NUM_SEGS; i++)
	{
		xhci->erst[i] = (struct xhci_erst_entry*) (Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE);

		writel( (UINT32)&xhci->erst[i]->seg_addr_Hi, cpu_to_le32( XHCI_BUFFERs_BASE_HI)  );
		writel( (UINT32)&xhci->erst[i]->seg_addr_Lo, cpu_to_le32( Event_Ring_Segment0_BaseLo)  );

		writel( (UINT32)&xhci->erst[i]->seg_size, cpu_to_le32( TRBS_PER_SEGMENT)  );
		writel( (UINT32)&xhci->erst[i]->rsvd, 0);
	#if (XHCI_DEBUG)
		PSPRINTF( "Event_Ring_SegTable: Entry [%d] =0x%x + 0x%x + 0x%x\n",
				i,
				le32_to_cpu( readl( Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE + 0x04 )  ),
				le32_to_cpu( readl( Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE + 0x00 )  ),
				le32_to_cpu( read16( Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE + 0x0C ) )
					);//0xe0341000 + 0x80000000 + 0x0
	#endif
	}



/* Interrupter Register Set
 *     Event Ring Registers */
//Event Ring Segment Table Size Register (ERSTSZ)
	rddat = readl( (UINT32)&xhci->run_regs->ir_set[0].erst_size);
	rddat &= ERST_SIZE_MASK;
	rddat |= ERST_NUM_SEGS;		//now,only use one segment for Event Ring
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_size, rddat );

//Event Ring Segment Table Base Address Register (ERSTBA)
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Lo, Event_Ring_SegTable_Entries_Base & (~(0x40-1)) );
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Hi, XHCI_BUFFERs_BASE_HI );
	#if (XHCI_DEBUG)
	PSPRINTF( "Event_Ring_SegTable_Entries_Base =0x%x + 0x%x\n",
			readl( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Hi),
			readl( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Lo)  );//=0x80000000 + 0xe0340200
	#endif

//Event Ring Dequeue Pointer Register (ERDP)
	rddat = readl( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Lo );
	rddat &= ERST_PTR_MASK;
	/* Don't clear the EHB bit (which is RW1C) because
	 * there might be more events to service.
	 */
	rddat &= ~ERST_EHB;
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Lo, rddat | (Event_Ring_Segment0_BaseLo & (~ERST_PTR_MASK)) );
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Hi, XHCI_BUFFERs_BASE_HI );  //consumer

//----------------------------------------------------------------------------
 /* Command ring setup:
 *    Set up the command ring (to have one segments for now.)
 */
	xhci->cmd_ring = (struct xhci_ring*) (Cmd_Ring_Struct_Base);
	xhci->cmd_ring->num_segs = 1;
	xhci->cmd_ring->type = TYPE_COMMAND;

	xhci->cmd_ring->first_seg = (struct xhci_segment*) (Cmd_Ring_Segment_Struct_Base);
	xhci->cmd_ring->first_seg->trbs = (union xhci_trb*)(Cmd_Ring_Segment0_BaseLo);	//useful/=Producer
	xhci->cmd_ring->first_seg->next = xhci->cmd_ring->first_seg;
	/*				the last TRB in segment is LINK TRB
	 * Change the last TRB in the prev segment to be a Link TRB which points to the
	 * DMA address of the next segment.  The caller needs to set any Link TRB
	 * related flags, such as End TRB, Toggle Cycle, and no snoop.
	 */
	if (xhci->cmd_ring->type != TYPE_EVENT)
	{
		/* 1. LINK TRB[Ring Segment Pointer],  points to the DMA address of the next segment(= first,only 1) */
	//	xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1] = (union xhci_trb*)(Cmd_Ring_Segment0_BaseLo);
		xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Lo = cpu_to_le32(Cmd_Ring_Segment0_BaseLo);
		xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Hi = cpu_to_le32(XHCI_BUFFERs_BASE_HI);

		/* 2.LINK TRB[TRB Type],  Set the last TRB in the segment to have a TRB type ID of Link TRB */
		rddat = TRB_TYPE(TRB_LINK) | LINK_TOGGLE ;
		xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1].link.control = cpu_to_le32(rddat);
	}

	//if xhci->cmd_ring->num_segs >1, need xhci_segment_alloc() for insert other segment
	xhci->cmd_ring->last_seg = xhci->cmd_ring->first_seg;

	/* The ring is empty, so the enqueue pointer == dequeue pointer */
	xhci->cmd_ring->enqueue = xhci->cmd_ring->first_seg->trbs;
	xhci->cmd_ring->enq_seg = xhci->cmd_ring->first_seg;

	xhci->cmd_ring->dequeue = xhci->cmd_ring->enqueue;
	xhci->cmd_ring->deq_seg = xhci->cmd_ring->first_seg;

	xhci->cmd_ring->cycle_state = 1;

/*
 * Set the address(Command Ring Dequeue Pointer) in the Command Ring Control register
 * =a 64-bit address pointing to the starting address of the first TRB of the Command Ring
 */
	rddat = readl( (UINT32)&xhci->op_regs->cmd_ring_Lo );
	rddat = (rddat & CMD_RING_RSVD_BITS) |	xhci->cmd_ring->cycle_state;
	rddat |= Cmd_Ring_Segment0_BaseLo & (~CMD_RING_RSVD_BITS);
	writel( (UINT32)&xhci->op_regs->cmd_ring_Lo, rddat );		//useful/ Consumer
	writel( (UINT32)&xhci->op_regs->cmd_ring_Hi, XHCI_BUFFERs_BASE_HI );

	xhci->cmd_completion = 0;
	xhci->trans_completion = 0;
	xhci->cur_slot_id = 0;

//----------------------------------------------------------------------------
 /* endpoint 0 ring setup:
 *    Set up the Transfer ring (to have one segments for now.)
 */
	xhci->ep_ring[0] = (struct xhci_ring*) (Ep0_Ring_Struct_Base);
	xhci->ep_ring[0]->num_segs = 1;
	xhci->ep_ring[0]->type = TYPE_CTRL;

	xhci->ep_ring[0]->first_seg = (struct xhci_segment*) (Ep0_Ring_Segment_Struct_Base);
	xhci->ep_ring[0]->first_seg->trbs = (union xhci_trb*)(Ep0_Ring_Segment0_BaseLo);	//useful/=Producer
	xhci->ep_ring[0]->first_seg->next = xhci->ep_ring[0]->first_seg;
	/*	the last TRB in segment is LINK TRB	 */
	if (xhci->ep_ring[0]->type != TYPE_EVENT)
	{
		/* 1. LINK TRB[Ring Segment Pointer],  points to the DMA address of the next segment(= first,only 1) */
		xhci->ep_ring[0]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Lo = cpu_to_le32(Ep0_Ring_Segment0_BaseLo);
		xhci->ep_ring[0]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Hi = cpu_to_le32(XHCI_BUFFERs_BASE_HI);

		/* 2.LINK TRB[TRB Type],  Set the last TRB in the segment to have a TRB type ID of Link TRB */
		rddat = TRB_TYPE(TRB_LINK) | LINK_TOGGLE ;
		xhci->ep_ring[0]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.control = cpu_to_le32(rddat);
	}

	//if xhci->cmd_ring->num_segs >1, need xhci_segment_alloc() for insert other segment
	xhci->ep_ring[0]->last_seg = xhci->ep_ring[0]->first_seg;

	/* The ring is empty, so the enqueue pointer == dequeue pointer */
	xhci->ep_ring[0]->enqueue = xhci->ep_ring[0]->first_seg->trbs;
	xhci->ep_ring[0]->enq_seg = xhci->ep_ring[0]->first_seg;

	xhci->ep_ring[0]->dequeue = xhci->ep_ring[0]->enqueue;
	xhci->ep_ring[0]->deq_seg = xhci->ep_ring[0]->first_seg;

	xhci->ep_ring[0]->cycle_state = 1;

//----------------------------------------------------------------------------
 /* endpoint xx ring setup */
	//->xhci_add_endpoint()
}


void xhci_mem_free(struct xhci_hcd *xhci)
{

    UINT32 rddat;
    UINT8 i;


/* DCBAAP:	64-byte aligned
 * 	Program the Device Context Base Address Array Pointer (DCBAAP) register with a 64-bit
 * 	address pointing to where the Device Context Base Address Array is located.
 */
	writel( (UINT32)&xhci->op_regs->dcbaa_ptr_Lo ,DCBAA_Entries_Base_LO & (~(0x40-1)) );
	writel( (UINT32)&xhci->op_regs->dcbaa_ptr_Hi ,XHCI_BUFFERs_BASE_HI );
#if (XHCI_DEBUG)
	PSPRINTF( "DCBAA_ENTRY_BASE =0x%x + 0x%x\n",
			readl( (UINT32)&xhci->op_regs->dcbaa_ptr_Hi),readl( (UINT32)&xhci->op_regs->dcbaa_ptr_Lo)  );
#endif

//fill Device Context Base Address Array->xhci_alloc_dev()
//fill context --> 	xhci_setup_device()

//----------------------------------------------------------------------------
 /* Event ring setup:
 *    Allocate a normal Event ring (to have one Event ring segment)
 */
	xhci->event_ring = (struct xhci_ring*) (Event_Ring_Struct_Base);
	xhci->event_ring->num_segs = ERST_NUM_SEGS;
	xhci->event_ring->type = TYPE_EVENT;

	xhci->event_ring->first_seg = (struct xhci_segment*) (Event_Ring_Segment_Struct_Base);
	xhci->event_ring->first_seg->trbs = (union xhci_trb*)(Event_Ring_Segment0_BaseLo);	//useful/ = Producer
	xhci->event_ring->first_seg->next = xhci->event_ring->first_seg;
	 /* Only event ring does not use link TRB */
	xhci->event_ring->last_seg = xhci->event_ring->first_seg;

	/* The ring is empty, so the enqueue pointer == dequeue pointer */
	xhci->event_ring->enqueue = xhci->event_ring->first_seg->trbs;
	xhci->event_ring->enq_seg = xhci->event_ring->first_seg;

	xhci->event_ring->dequeue = xhci->event_ring->enqueue;		//useful/variable
	xhci->event_ring->deq_seg = xhci->event_ring->first_seg;	//useful

	/* The ring is initialized to 0. The producer must write 1 to the cycle
	 * bit to handover ownership of the TRB, so PCS = 1.  The consumer must
	 * compare CCS to the cycle bit to check ownership, so CCS = 1.
	 *
	 * New rings are initialized with cycle state equal to 1; if we are
	 * handling ring expansion, set the cycle state equal to the old ring.
	 */
	xhci->event_ring->cycle_state = 1;


/* Fill Event ring segment table (for xhci->ir_set[0])
 */
	for (i = 0; i < ERST_NUM_SEGS; i++)
	{
		xhci->erst[i] = (struct xhci_erst_entry*) (Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE);

		writel( (UINT32)&xhci->erst[i]->seg_addr_Hi, cpu_to_le32( XHCI_BUFFERs_BASE_HI)  );
		writel( (UINT32)&xhci->erst[i]->seg_addr_Lo, cpu_to_le32( Event_Ring_Segment0_BaseLo)  );

		writel( (UINT32)&xhci->erst[i]->seg_size, cpu_to_le32( TRBS_PER_SEGMENT)  );
		writel( (UINT32)&xhci->erst[i]->rsvd, 0);
	#if (XHCI_DEBUG)
		PSPRINTF( "Event_Ring_SegTable: Entry [%d] =0x%x + 0x%x + 0x%x\n",
				i,
				le32_to_cpu( readl( Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE + 0x04 )  ),
				le32_to_cpu( readl( Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE + 0x00 )  ),
				le32_to_cpu( read16( Event_Ring_SegTable_Entries_Base + i*ERST_ONE_ENTRY_SIZE + 0x0C ) )
					);//0xe0341000 + 0x80000000 + 0x0
	#endif
	}



/* Interrupter Register Set
 *     Event Ring Registers */
//Event Ring Segment Table Size Register (ERSTSZ)
	rddat = readl( (UINT32)&xhci->run_regs->ir_set[0].erst_size);
	rddat &= ERST_SIZE_MASK;
	rddat |= ERST_NUM_SEGS;		//now,only use one segment for Event Ring
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_size, rddat );

//Event Ring Segment Table Base Address Register (ERSTBA)
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Lo, Event_Ring_SegTable_Entries_Base & (~(0x40-1)) );
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Hi, XHCI_BUFFERs_BASE_HI );
	#if (XHCI_DEBUG)
	PSPRINTF( "Event_Ring_SegTable_Entries_Base =0x%x + 0x%x\n",
			readl( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Hi),
			readl( (UINT32)&xhci->run_regs->ir_set[0].erst_base_Lo)  );//=0x80000000 + 0xe0340200
	#endif

//Event Ring Dequeue Pointer Register (ERDP)
	rddat = readl( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Lo );
	rddat &= ERST_PTR_MASK;
	/* Don't clear the EHB bit (which is RW1C) because
	 * there might be more events to service.
	 */
	rddat &= ~ERST_EHB;
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Lo, rddat | (Event_Ring_Segment0_BaseLo & (~ERST_PTR_MASK)) );
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Hi, XHCI_BUFFERs_BASE_HI );  //consumer

//----------------------------------------------------------------------------
 /* Command ring setup:
 *    Set up the command ring (to have one segments for now.)
 */
	xhci->cmd_ring = (struct xhci_ring*) (Cmd_Ring_Struct_Base);
	xhci->cmd_ring->num_segs = 1;
	xhci->cmd_ring->type = TYPE_COMMAND;

	xhci->cmd_ring->first_seg = (struct xhci_segment*) (Cmd_Ring_Segment_Struct_Base);
	xhci->cmd_ring->first_seg->trbs = (union xhci_trb*)(Cmd_Ring_Segment0_BaseLo);	//useful/=Producer
	xhci->cmd_ring->first_seg->next = xhci->cmd_ring->first_seg;
	/*				the last TRB in segment is LINK TRB
	 * Change the last TRB in the prev segment to be a Link TRB which points to the
	 * DMA address of the next segment.  The caller needs to set any Link TRB
	 * related flags, such as End TRB, Toggle Cycle, and no snoop.
	 */
	if (xhci->cmd_ring->type != TYPE_EVENT)
	{
		/* 1. LINK TRB[Ring Segment Pointer],  points to the DMA address of the next segment(= first,only 1) */
	//	xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1] = (union xhci_trb*)(Cmd_Ring_Segment0_BaseLo);
		xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Lo = cpu_to_le32(Cmd_Ring_Segment0_BaseLo);
		xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Hi = cpu_to_le32(XHCI_BUFFERs_BASE_HI);

		/* 2.LINK TRB[TRB Type],  Set the last TRB in the segment to have a TRB type ID of Link TRB */
		rddat = TRB_TYPE(TRB_LINK) | LINK_TOGGLE ;
		xhci->cmd_ring->first_seg->trbs[TRBS_PER_SEGMENT-1].link.control = cpu_to_le32(rddat);
	}

	//if xhci->cmd_ring->num_segs >1, need xhci_segment_alloc() for insert other segment
	xhci->cmd_ring->last_seg = xhci->cmd_ring->first_seg;

	/* The ring is empty, so the enqueue pointer == dequeue pointer */
	xhci->cmd_ring->enqueue = xhci->cmd_ring->first_seg->trbs;
	xhci->cmd_ring->enq_seg = xhci->cmd_ring->first_seg;

	xhci->cmd_ring->dequeue = xhci->cmd_ring->enqueue;
	xhci->cmd_ring->deq_seg = xhci->cmd_ring->first_seg;

	xhci->cmd_ring->cycle_state = 1;

/*
 * Set the address(Command Ring Dequeue Pointer) in the Command Ring Control register
 * =a 64-bit address pointing to the starting address of the first TRB of the Command Ring
 */
	rddat = readl( (UINT32)&xhci->op_regs->cmd_ring_Lo );
	rddat = (rddat & CMD_RING_RSVD_BITS) |	xhci->cmd_ring->cycle_state;
	rddat |= Cmd_Ring_Segment0_BaseLo & (~CMD_RING_RSVD_BITS);
	writel( (UINT32)&xhci->op_regs->cmd_ring_Lo, rddat );		//useful/ Consumer
	writel( (UINT32)&xhci->op_regs->cmd_ring_Hi, XHCI_BUFFERs_BASE_HI );

	xhci->cmd_completion = 0;
	xhci->trans_completion = 0;
	xhci->cur_slot_id = 0;

//----------------------------------------------------------------------------
 /* endpoint 0 ring setup:
 *    Set up the Transfer ring (to have one segments for now.)
 */
	xhci->ep_ring[0] = (struct xhci_ring*) (Ep0_Ring_Struct_Base);
	xhci->ep_ring[0]->num_segs = 1;
	xhci->ep_ring[0]->type = TYPE_CTRL;

	xhci->ep_ring[0]->first_seg = (struct xhci_segment*) (Ep0_Ring_Segment_Struct_Base);
	xhci->ep_ring[0]->first_seg->trbs = (union xhci_trb*)(Ep0_Ring_Segment0_BaseLo);	//useful/=Producer
	xhci->ep_ring[0]->first_seg->next = xhci->ep_ring[0]->first_seg;
	/*	the last TRB in segment is LINK TRB	 */
	if (xhci->ep_ring[0]->type != TYPE_EVENT)
	{
		/* 1. LINK TRB[Ring Segment Pointer],  points to the DMA address of the next segment(= first,only 1) */
		xhci->ep_ring[0]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Lo = cpu_to_le32(Ep0_Ring_Segment0_BaseLo);
		xhci->ep_ring[0]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.segment_ptr_Hi = cpu_to_le32(XHCI_BUFFERs_BASE_HI);

		/* 2.LINK TRB[TRB Type],  Set the last TRB in the segment to have a TRB type ID of Link TRB */
		rddat = TRB_TYPE(TRB_LINK) | LINK_TOGGLE ;
		xhci->ep_ring[0]->first_seg->trbs[TRBS_PER_SEGMENT-1].link.control = cpu_to_le32(rddat);
	}

	//if xhci->cmd_ring->num_segs >1, need xhci_segment_alloc() for insert other segment
	xhci->ep_ring[0]->last_seg = xhci->ep_ring[0]->first_seg;

	/* The ring is empty, so the enqueue pointer == dequeue pointer */
	xhci->ep_ring[0]->enqueue = xhci->ep_ring[0]->first_seg->trbs;
	xhci->ep_ring[0]->enq_seg = xhci->ep_ring[0]->first_seg;

	xhci->ep_ring[0]->dequeue = xhci->ep_ring[0]->enqueue;
	xhci->ep_ring[0]->deq_seg = xhci->ep_ring[0]->first_seg;

	xhci->ep_ring[0]->cycle_state = 1;

//----------------------------------------------------------------------------
 /* endpoint xx ring setup */
	//->xhci_add_endpoint()

}



/*
 * Set the run bit and wait for the host to be running.
 */
void xhci_start(struct xhci_hcd *xhci)
{
	/*
	 * allows the xHC to begin accepting doorbell references.
	 * the host controller is up and running and the Root Hub ports will begin reporting device connects, etc.,
	 */
	writel( (UINT32)&xhci->op_regs->command, CMD_RUN |  CMD_EIE ); //0x05
}

/*
 * This function is called by the USB core when the HC driver is added.
 * Setup MSI-X vectors and enable interrupts.
*/
void xhci_run(struct xhci_hcd *xhci)
{
	/* Set the interrupt modulation register */
	writel( (UINT32)&xhci->run_regs->ir_set[0].irq_control, 0x01F401F4);//0x0FA00FA0 );

	/* enable the irqs */
	writel( (UINT32)&xhci->op_regs->command, CMD_EIE );

	/* Set Interrupter Management */
	writel( (UINT32)&xhci->run_regs->ir_set[0].irq_pending, ER_IRQ_ENABLE(0) );

	xhci_start(xhci);
}


//============ Event =========================================================================
//=========== Interrupt ======================================================================

/*		control/bulk/iso common trans fuction
 * If this function returns an error condition, it means it got a Transfer
 * event with a corrupted Slot ID, Endpoint ID, or TRB DMA address.
 * At this point, the host controller is probably hosed and should be reset.
 */
static void handle_tx_event(struct xhci_hcd *xhci,
						struct xhci_transfer_event *event)
{
	UINT32 slot_id;
	UINT32 ep_index;
	UINT32 trb_comp_code;

/* read Transfer Event TRB */
	slot_id  = TRB_TO_SLOT_ID(le32_to_cpu(event->flags));
	ep_index = TRB_TO_EP_ID(le32_to_cpu(event->flags)) - 1;
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));
	/* Trans TRB addr for generate this event  <--Transfer Event TRB [TRB Pointer]   */
//	ep_trb_dma = le64_to_cpu(event->buffer);  //Ep0_Ring_Segment0_BaseLo


/* Look for common error cases */
	switch (trb_comp_code)
	{
		/* Skip codes that require special handling depending on
		 * transfer type
		 */
		case COMP_SUCCESS://-------------
			if ( EVENT_TRB_LEN(le32_to_cpu(event->transfer_len)) == 0)
				xhci->trans_completion = 1;
			else {
				PSPRINTF("WARN: Successful completion, but short packet ??? for ep %d\n",ep_index);
				COND_TRAP(31, 0, 0);
			}

			/* update deq of xhci->ep_ring */
			inc_deq(xhci, xhci->ep_ring[ep_index]);
			break;
		case COMP_SHORT_PACKET:
			/* update deq of xhci->ep_ring */
			inc_deq(xhci, xhci->ep_ring[ep_index]);

			break;   //-------------

		/* Completion codes for endpoint stopped state */
		case COMP_STOPPED:
			PSPRINTF("WARN: Stopped on Transfer TRB for ep %d\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;
		case COMP_STOPPED_LENGTH_INVALID:
			PSPRINTF("WARN: Stopped on No-op or Link TRB for ep %d\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;
		case COMP_STOPPED_SHORT_PACKET:
			PSPRINTF("WARN: Stopped with short packet transfer detected for ep %d\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;

		/* Completion codes for endpoint halted state */
		case COMP_STALL_ERROR:
			PSPRINTF("WARN: Stalled endpoint for ep %d\n",ep_index);
		//	ep->ep_state |= EP_HALTED;
		//	status = -EPIPE;	/* Broken pipe */
			COND_TRAP(31, 0, 0);
			break;
		case COMP_SPLIT_TRANSACTION_ERROR:
		case COMP_USB_TRANSACTION_ERROR:
			PSPRINTF("WARN: Transfer error for ep %d on endpoint\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;
		case COMP_BABBLE_DETECTED_ERROR:
			PSPRINTF("WARN: Babble error for ep %d on endpoint\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;

		/* Completion codes for endpoint error state */
		case COMP_TRB_ERROR:
			PSPRINTF("WARN: TRB error for ep %d on endpoint\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;

		/* completion codes not indicating endpoint state change */
		case COMP_DATA_BUFFER_ERROR:
			PSPRINTF("WARN: HC couldn't access mem fast enough for ep %d\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;
		case COMP_BANDWIDTH_OVERRUN_ERROR:
			PSPRINTF("WARN: bandwidth overrun event for ep %d on endpoint\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;
		case COMP_ISOCH_BUFFER_OVERRUN:
			PSPRINTF("WARN: buffer overrun event for ep %d on endpoint\n",ep_index);
			COND_TRAP(31, 0, 0);
			break;
		case COMP_RING_UNDERRUN:
			PSPRINTF("WARN: underrun event on endpoint\n");
			COND_TRAP(31, 0, 0);
			break;
		case COMP_RING_OVERRUN:
			PSPRINTF("WARN: overrun event on endpoint\n");
			COND_TRAP(31, 0, 0);
			break;
		case COMP_MISSED_SERVICE_ERROR:
		case COMP_NO_PING_RESPONSE_ERROR:
		case COMP_INCOMPATIBLE_DEVICE_ERROR:
		default:
			COND_TRAP(31, 0, 0);
			break;
	}
}




static void handle_cmd_completion(struct xhci_hcd *xhci,
								struct xhci_event_cmd *event)
{
	UINT32 slot_id = TRB_TO_SLOT_ID( le32_to_cpu(event->flags) );
	union xhci_trb *cmd_trb;
	UINT32 cmd_addr1,cmd_addr2;
	UINT32 cmd_comp_code;
	UINT32 cmd_type;

/* event TRB [00:07]
 * 	check the Command TRB addr that generated this event.    */
	cmd_addr1 = le32_to_cpu(event->cmd_trb_ptr_Lo);	//CMD Completion event TRB [00:07]
	cmd_trb = xhci->cmd_ring->dequeue;
	/* Check whether the completion event is for our internal kept command. */
	cmd_addr2 = trb_offset_to_addr( xhci->cmd_ring->deq_seg,cmd_trb );
	if ( (!cmd_addr2) ||  (cmd_addr1 != cmd_addr2)  )
	{
		PSPRINTF( " ERROR: mismatched command completion event\n");
		return;
	}

#if 0//XHCI_DEBUG
	PSPRINTF( "handle_cmd_completion(enq& =0x%x) = 0x%x,0x%x,0x%x,0x%x =(%s)TRB\n",
				cmd_addr1,
			//	readl( (UINT32)&xhci->op_regs->cmd_ring_Lo ),  //(CRCR=0x%x)
				le32_to_cpu( cmd_trb->generic.field[0] ),le32_to_cpu( cmd_trb->generic.field[1] ),
				le32_to_cpu( cmd_trb->generic.field[2] ),le32_to_cpu( cmd_trb->generic.field[3] ),
				xhci_trb_type_string( TRB_FIELD_TO_TYPE( le32_to_cpu(cmd_trb->generic.field[3]))  )
				 );
#endif

/* event TRB [Completion Code] */
	cmd_comp_code = GET_COMP_CODE( le32_to_cpu(event->status) );
	/* If CMD ring stopped we own the trbs between enqueue and dequeue */
	if (cmd_comp_code == COMP_COMMAND_RING_STOPPED) {
		COND_TRAP(31, 0, 0);
		return;
	}
	/* Host aborted the command ring, check if the current command was
	 * supposed to be aborted, otherwise continue normally.
	 * The command ring is stopped now, but the xHC will issue a Command
	 * Ring Stopped event which will cause us to restart it.
	 */
	if (cmd_comp_code == COMP_COMMAND_ABORTED)
	{
		COND_TRAP(31, 0, 0);
		return;
	}


/* CMD TRB [TRB Type]  */
	cmd_type = TRB_FIELD_TO_TYPE( le32_to_cpu(cmd_trb->generic.field[3]) );
	switch (cmd_type)
	{
	case TRB_ENABLE_SLOT:
		/* The Enable Slot Command returns a Slot ID */
		if (cmd_comp_code == COMP_SUCCESS)
			xhci->cur_slot_id = slot_id;
		else {
			COND_TRAP(31, 0, 0);
			xhci->cur_slot_id = 0;
		}
		break;
	case TRB_DISABLE_SLOT:
		COND_TRAP(31, 0, 0);
		break;

	case TRB_CONFIG_EP:
		break;
	case TRB_EVAL_CONTEXT:
		break;
	case TRB_ADDR_DEV:
		//return out_ctx
		//slot_ctx = xhci_get_slot_ctx(xhci, xhci->out_ctx);
		break;


	case TRB_STOP_RING:
		break;
	case TRB_SET_DEQ:
		break;
	case TRB_CMD_NOOP:
		break;

	case TRB_RESET_EP:
		break;
	case TRB_RESET_DEV:
		break;

	case TRB_NEC_GET_FW:
		break;
	default:
		/* Skip over unknown commands on the event ring */
		#if XHCI_DEBUG
			PSPRINTF( "ERR: INFO unknown command type %d\n",cmd_type  );
		#endif
		break;
	}

	/* update deq of xhci->cmd_ring */
	inc_deq(xhci, xhci->cmd_ring);
	xhci->cmd_completion = 1;
}


/*
 * Given a port state, this function returns a value that would result in the
 * port being in the same state, if the value was written to the port status
 * control register.
 * Save Read Only (RO) bits and save read/write bits where
 * writing a 0 clears the bit and writing a 1 sets the bit (RWS).
 * For all other types (RW1S, RW1CS, RW, and RZ), writing a '0' has no effect.
 */
UINT32 xhci_port_state_to_neutral(UINT32 state)
{
	/* Save read-only status and port state */
	return (state & XHCI_PORT_RO) | (state & XHCI_PORT_RWS);
}
static void handle_port_status(struct xhci_hcd *xhci,union xhci_trb *event)
{
	UINT32 port_id,portsc;

/* event TRB[ completion code]:
  *   Port status change events always have a successful completion code */
	if ( GET_COMP_CODE(le32_to_cpu(event->generic.field[2])) != COMP_SUCCESS ) {
		PSPRINTF( "WARN: xHCI returned failed port status event\n");
	}

/* event TRB[ Port ID ]: */
	port_id = GET_PORT_ID(le32_to_cpu(event->generic.field[0]));
#if 0//(XHCI_DEBUG)
//	PSPRINTF( "Port Status Change Event for port %d\n", port_id);	//=1(2.0) / =2(3.0);
#endif
	if ( (port_id <= 0) || (port_id > (xhci->max_ports)) ) {
		PSPRINTF( "Invalid port id %d\n", port_id);
		inc_deq(xhci, xhci->event_ring);
		return;
	}
	xhci->cur_port_id = port_id;

/*
 * Hardware port IDs reported by a Port Status Change Event include USB
 * 3.0 and USB 2.0 ports.  We want to check if the port has reported a
 * resume event, but we first need to translate the hardware port ID
 * into the index into the ports on the correct split roothub, and the
 * correct bus_state structure.
 */
	portsc = (port_id== xhci->usb2_port_id)?
				(readl( (UINT32)&xhci->op_regs->port1_status_base )) :
				(readl( (UINT32)&xhci->op_regs->port2_status_base )) ;
#if 0//(XHCI_DEBUG)
	PSPRINTF( "handle_port_status()/portsc= 0x%x\n", portsc);  //0x206e1
#endif
	if(  (port_id== xhci->usb3_port_id) &&
		 ( (portsc & PORT_PLS_MASK) == XDEV_INACTIVE)    ){
		PSPRINTF( "Err: Port Link is in the Inactive State. \n");
	}
	if ( (portsc & PORT_PLC) && ((portsc & PORT_PLS_MASK) == XDEV_RESUME)   ){
		PSPRINTF( "port resume event for port %d\n", port_id);

		if (DEV_SUPERSPEED_ANY(portsc) ) {
			PSPRINTF( "remote wake SS port %d\n", port_id);
		//	xhci_set_link_state(xhci, XDEV_U0);
		} else {
			PSPRINTF( "remote wake HS port %d\n", port_id);
		}
	}
	if ( (portsc & PORT_PLC) &&
	     ((portsc & PORT_PLS_MASK) == XDEV_U0) &&
		 DEV_SUPERSPEED_ANY(portsc)  ){
		PSPRINTF( "resume SS port %d finished\n", port_id);
	}

#if 1
	(port_id== xhci->usb2_port_id)?
	//err		writel( (UINT32)&xhci->op_regs->port1_status_base,portsc ) :
	//			writel( (UINT32)&xhci->op_regs->port2_status_base,portsc ) ; //clr bit17
	//be careful, write bit 1 cautious
		writel( (UINT32)&xhci->op_regs->port1_status_base,xhci_port_state_to_neutral(portsc) | PORT_CSC | PORT_RC) :
		writel( (UINT32)&xhci->op_regs->port2_status_base,xhci_port_state_to_neutral(portsc) | PORT_CSC | PORT_RC) ;

#endif
#if 0
	PSPRINTF( " starting port polling.  (P0=0x%x,P1=0x%x)\n",
							readl( (UINT32)&xhci->op_regs->port1_status_base ),
							readl( (UINT32)&xhci->op_regs->port2_status_base )
			);
#endif

	if( portsc &  PORT_RC) {
		connect_change = 3;
	}
	else if( portsc &  PORT_CONNECT) {
		connect_change = 1;

	//	PSPRINTF( "usb connect: starting port polling. \n" );
		PSPRINTF( "usb connect: Port(%d) in (%s) state \n",xhci->cur_port_id,
							(portsc & PORT_PE)? "Port Enabled":"Port Disabled" );  //or use PORT_PLS_MASK
	}
	else if( (portsc &  PORT_CONNECT) ==0) {
		connect_change = 2;  //off
	}
}

// Interrupt ***************************************************************************************************************
//********************************************************************************************************************

/* This function handles all OS-owned events on the event ring.
 *
 * Returns >0 for "possibly more events to process" (caller should call again),
 * otherwise 0 if done.
 * In future, <0 returns should indicate error code.
 *				===> return 1= process events over
 */
static int xhci_handle_event(struct xhci_hcd *xhci)
{
	union xhci_trb *event;
	UINT8 update_ptrs = 1;	//in order to update xhci_ring->dequeue one time

	/* Event ring hasn't been allocated yet. */
	if (!xhci->event_ring || !xhci->event_ring->dequeue)
	{
		PSPRINTF( "ERROR event ring not ready\n");
		return -ENOMEM;
	}
	event =  //trb
		xhci->event_ring->dequeue;
#if 0//XHCI_DEBUG
	if(  event->generic.field[3]  != 0x00000000 )
	{
		PSPRINTF( "xhci_handle_event(deq& =0x%x) = 0x%x,0x%x,0x%x,0x%x =(%s)TRB\n",
			(event),
			le32_to_cpu( event->generic.field[0] ),le32_to_cpu( event->generic.field[1] ),
			le32_to_cpu( event->generic.field[2] ),le32_to_cpu( event->generic.field[3] ),
			xhci_trb_type_string( TRB_FIELD_TO_TYPE( le32_to_cpu(event->generic.field[3]))  )
		);
	}
#endif
	/* event TRB[ Cycle bit (C) ]:
	  *     Does the HC or OS own the TRB? */
	if (	(le32_to_cpu(event->generic.field[3]) & TRB_CYCLE) !=
				xhci->event_ring->cycle_state
	   ) {
	//	COND_TRAP(31, 0, 0);
		return 0;
	}

	/* event TRB[ TRB Type ]:
	  *     FIXME: Handle more (event) TRBs types. */
		switch (le32_to_cpu(event->generic.field[3]) & TRB_TYPE_BITMASK)
		{
		case TRB_TYPE(TRB_COMPLETION):
			handle_cmd_completion(xhci, &event->event_cmd);
			break;
		case TRB_TYPE(TRB_PORT_STATUS):
			handle_port_status(xhci, event);
			break;
		case TRB_TYPE(TRB_TRANSFER):
			handle_tx_event(xhci, &event->trans_event);
			break;


		case TRB_TYPE(TRB_DEV_NOTE):
	//		handle_device_notification(xhci, event);
			break;
		default:
			if (  (le32_to_cpu(event->event_cmd.flags) & TRB_TYPE_BITMASK) >=
				  TRB_TYPE(48)  )
			{
		//		handle_vendor_event(xhci, event);
			} else {
				PSPRINTF( "ERROR unknown event type %d\n",
					  TRB_FIELD_TO_TYPE(le32_to_cpu(event->event_cmd.flags)));
			}
		}
		if (update_ptrs)
		{
			/* Update SW event ring dequeue pointer */
			inc_deq(xhci, xhci->event_ring);
		}


/*  again
 * Are there more items on the event ring?  Caller will call us again to check.
 */
	return 1;
}


/*
 * xHCI spec says we can get an interrupt, and if the HC has an error condition,
 * we might get bad data out of the event ring.  Section 4.10.2(Errors) has a list of
 * indicators of an event TRB error, but we check the status *first* to be safe.
 */
void xhci_irq(void)
{
//	PSPRINTF( "###############xhci_irq#################\n");
	volatile UINT32 irq_pending;
	volatile UINT32 status;
	union xhci_trb *event_ring_deq;
	UINT32 deq_rd,trboffset_in_segment,new_deq_addr;
#if 0
//just test
	status = *(volatile unsigned int*)( USB3_HOST_REG_BADDR+DWC3_OEVT );

	if( status & 0x7FFFFFFF ) {
		PSPRINTF( "	----->dwc3_otg_event_isr1,OEVT = 0x%x(%s,%s,%s,%s,%s,)\n",
				status,
				(status & (1 <<31)) ? "B-Device mode":"A-Device mode",
				(status & (1 <<27)) ? "Host CMD.Run":"",
				(status & (1 <<26)) ? "DCTL.Run":"",
				(status & (1 <<24)) ? "ID Change":"",
				(status & (1 <<19)) ? "A-device as host":""
		);
		*(volatile unsigned int*)( USB3_HOST_REG_BADDR+DWC3_OEVT ) = status;
	}
#endif

/*
 *	Check if the xHC generated the interrupt, or the irq is shared
 */
	status = readl( (UINT32)&xhci->op_regs->status);
	if ( (status == ~(UINT32)0)
	   ||(!(status & STS_EINT))
	  )	{
		PSPRINTF( "		Error,USBSTS[Event Interrupt Status] != 1  \n");
		COND_TRAP(31, 0, 0);
	}
	if ( status & STS_FATAL)	{
		PSPRINTF( "		WARNING: Host System Error!!!  \n");
		COND_TRAP(31, 0, 0);
	}
#if 0//(XHCI_DEBUG)
	PSPRINTF( "\n	----------->xhci_irq():\nusb2.0 PORTPMSC=0x%x\n", readl( (UINT32)&xhci->op_regs->port1_status_base) );
	PSPRINTF( "usb3.0 PORTPMSC=0x%x\n", readl( (UINT32)&xhci->op_regs->port2_status_base) );
//	PSPRINTF( "USBCMD=0x%x\n", readl( (UINT32)&xhci->op_regs->command) );  //0x05
//	PSPRINTF( "USBSTS=0x%x\n", readl( (UINT32)&xhci->op_regs->status) );	//-x18
//	PSPRINTF( "IMAN=0x%x\n", readl( (UINT32)&xhci->run_regs->ir_set[0].irq_pending) ); //0x03
	PSPRINTF( "Event Ring Dequeue Pointer(ERDR)=0x%x\n", readl( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Lo) );//0xe0341008
#endif
#if (!XHCI_DEBUG)
	PSPRINTF( "\n	----------->xhci_irq():(S=0x%x,IP=0x%x,P0=0x%x,P1=0x%x)\n",
						status,readl( (UINT32)&xhci->run_regs->ir_set[0].irq_pending ),
						readl( (UINT32)&xhci->op_regs->port1_status_base ),
						readl( (UINT32)&xhci->op_regs->port2_status_base)   );
#endif


/*  Clear the op reg interrupt status first,
* 	so we can receive interrupts from other MSI-X interrupters.
* 	Write 1 to clear the interrupt status.
*/
	status |= STS_EINT;
	writel( (UINT32)&xhci->op_regs->status,status );
/*
 *	Software that uses EINT shall clear it prior to clearing any IP flags */
	irq_pending = readl(  (UINT32)&xhci->run_regs->ir_set[0].irq_pending );
	irq_pending |= IMAN_IP;
	writel( (UINT32)&xhci->run_regs->ir_set[0].irq_pending ,irq_pending );



	event_ring_deq  //=trb
			= xhci->event_ring->dequeue;  //before inc_deq
/* FIXME this should be a delayed service routine that clears the EHB.
 */
	while (xhci_handle_event(xhci) > 0) {} //maybe change dequeue


	deq_rd = readl(  (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Lo ); //==0xe0341008/0xe0341008/=0xe0341028
/* If necessary, update the HW's version of the event ring deq ptr. */
	if (event_ring_deq != xhci->event_ring->dequeue)
	{
	//	trboffset_in_segment = (xhci->event_ring->dequeue - xhci->event_ring->deq_seg->trbs); //1 2 ...
	//	new_deq_addr = ( (UINT32)xhci->event_ring->deq_seg->trbs )
	//			+ (trboffset_in_segment * sizeof(*xhci->event_ring->dequeue) );
		new_deq_addr = trb_offset_to_addr( xhci->event_ring->deq_seg,xhci->event_ring->dequeue  );
	#if 0
		PSPRINTF( "new_deq_addr=0x%x\n",new_deq_addr );//0xe0341010
	#endif

		/* Update HC event ring dequeue pointer */
		deq_rd &= ERST_PTR_MASK;
		deq_rd |= ( new_deq_addr &  (~ERST_PTR_MASK) );
	}
	/* Clear the event handler busy flag (RW1C); event ring is empty. */
	deq_rd |= ERST_EHB;
	writel( (UINT32)&xhci->run_regs->ir_set[0].erst_dequeue_Lo, deq_rd );
}


//============================================================================================
//===============  Main ======================================================================
void __memcpy(UINT8 *dst, UINT8 *src, UINT32 len)
{
	UINT32 i =0;

	for(i=0; i<len; i++)
	{
		*dst++ = *src++;
	}
}

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
#if U2_GADGET_TEST
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


void xhci_main( void )
{
	unsigned int rddat,i,state,reg;

//xHCI:
#if 1
	xhci->cap_regs 	= (struct xhci_cap_regs*) USB3_HOST_REG_BADDR;
	xhci->op_regs 	= (struct xhci_op_regs*) (USB3_HOST_REG_BADDR +
												HC_LENGTH(readl( (UINT32)&xhci->cap_regs->hc_capbase)) ); 		  //+0x20;
	xhci->run_regs 	= (struct xhci_run_regs*)(USB3_HOST_REG_BADDR +
												(readl( (UINT32)&xhci->cap_regs->run_regs_off) & RTSOFF_MASK) ); //+0x440
	xhci->dba 		= (struct xhci_doorbell_array*)(USB3_HOST_REG_BADDR +
												(readl( (UINT32)&xhci->cap_regs->db_off) & DBOFF_MASK) ); //+0x480

	xhci->hcc_params  = readl( (UINT32)&xhci->cap_regs->hc_capbase);
	xhci->hci_version = HC_VERSION(xhci->hcc_params);   //0x0110
	xhci->hcs_params1 = readl( (UINT32)&xhci->cap_regs->hcs_params1);
	xhci->hcs_params2 = readl( (UINT32)&xhci->cap_regs->hcs_params2);
	xhci->hcs_params3 = readl( (UINT32)&xhci->cap_regs->hcs_params3);
	xhci->hcc_params  = readl( (UINT32)&xhci->cap_regs->hcc_params);
	if (xhci->hci_version > 0x0100)
		xhci->hcc_params2 = readl( (UINT32)&xhci->cap_regs->hcc_params2);
	xhci->max_ports = HCS_MAX_PORTS(xhci->hcs_params1); //=2

	#if XHCI_DEBUG
		xhci_print_registers(xhci);
	#endif
	xhci_ext_cap_regs(xhci);
	

//	xhci_reset(xhci);
	xhci_mem_init(xhci);

	xhci_run(xhci);
	
//	reg = readl((UINT32)&xhci->op_regs->port1_status_base);
//	PSPRINTF("port1_status_base:0x%x",reg);
//	reg &= ~(0xf<<5);
//	reg |= (0xa<<5);
//	writel( (UINT32)&xhci->op_regs->port1_status_base, reg );
//	reg = readl((UINT32)&xhci->op_regs->port1_status_base);
//	PSPRINTF("port1_status_base:0x%x",reg);
//	
//	reg = readl( (UINT32)&xhci->op_regs->port2_status_base);
//	PSPRINTF("port2_status_base:0x%x",reg);

//	reg &= ~(0xf<<5);
//	reg |= (0xa<<5);
//	writel( (UINT32)&xhci->op_regs->port2_status_base, reg );
//	reg = readl( (UINT32)&xhci->op_regs->port2_status_base);
//	PSPRINTF("port2_status_base:0x%x",reg);
	
//	reg = readl( (UINT32)&xhci->op_regs->port1_status_base);
//	reg &= ~(1<<9);
//	writel( (UINT32)&xhci->op_regs->port1_status_base, reg );
//	reg = readl( (UINT32)&xhci->op_regs->port1_status_base);
//	PSPRINTF("port1_status_base:0x%x",reg);
//	
//	reg = readl( (UINT32)&xhci->op_regs->port2_status_base);
//	reg &= ~(1<<9);
//	writel( (UINT32)&xhci->op_regs->port2_status_base, reg );
//	reg = readl( (UINT32)&xhci->op_regs->port2_status_base);
//	PSPRINTF("port2_status_base:0x%x",reg);
//	
//	reg = readl( USB30_BASE_ADDR +DWC3_GUSB3PIPECTL(0) );//=0x10c0002
//	PSPRINTF("DWC3_GUSB3PIPECTL:0x%x",reg);
//	reg |=(1<<30); // GUSB3PIPECTL.HstPrtCmpl = 1
//	writel( USB30_BASE_ADDR + DWC3_GUSB3PIPECTL(0),reg );
//	reg = readl( USB30_BASE_ADDR +DWC3_GUSB3PIPECTL(0) );//=0x10c0002
//	PSPRINTF("DWC3_GUSB3PIPECTL:0x%x",reg);
//	while(1)
//	{
//		reg = readl( USB30_BASE_ADDR +DWC3_GUSB3PIPECTL(0) );//=0x10c0002
//		PSPRINTF("DWC3_GUSB3PIPECTL:0x%x",reg);
//		reg &=~(1<<30); // GUSB3PIPECTL.HstPrtCmpl = 1
//		writel( USB30_BASE_ADDR + DWC3_GUSB3PIPECTL(0),reg );
//		
//		reg = readl( USB30_BASE_ADDR +DWC3_DSTS );
//		PSPRINTF("  ltssm_state()/[%s] \n",dwc3_gadget_link_string( DWC3_DSTS_USBLNKST( reg ) ) );
//		
//		reg = readl( USB30_BASE_ADDR +DWC3_GUSB3PIPECTL(0) );//=0x10c0002
//		PSPRINTF("DWC3_GUSB3PIPECTL:0x%x",reg);
//		reg |=(1<<30); // GUSB3PIPECTL.HstPrtCmpl = 1
//		writel( USB30_BASE_ADDR + DWC3_GUSB3PIPECTL(0),reg );
//	}
#endif


#if 1
	PSPRINTF("----------- while(1) test:-----------\n" );
	while( 1 )
	{
//		PSPRINTF("connect_change:%d\n",connect_change);
//		reg = readl( USB30_BASE_ADDR +DWC3_DSTS );
//		PSPRINTF("  ltssm_state()/[%s] \n",dwc3_gadget_link_string( DWC3_DSTS_USBLNKST( reg ) ) );
		if( connect_change == 1 ){   //handle_port_status()
			connect_change = 0;
			xhci_port_connect(xhci);
//			usb_storage_alloc(xhci);
			usb_stor_Bulk_transport_inqury12(xhci);
			usb_stor_Bulk_transport_read10(xhci);
			#if 1
				while(1)
				{
	
						reg = readl(  USB3PIPE_BASE_ADDR + (193*4) );	//
						PSPRINTF( "USB3PIPE_( 193*4 ) (0x%x) = 0x%x\n",(USB3PIPE_BASE_ADDR + ( 193*4 )) , reg  );
						PSPRINTF( "PHY3:%d\n", (reg&(1<<19))>>19  );
						PSPRINTF( "PHY2:%d\n", (reg&(1<<18))>>18  );
						PSPRINTF( "PHY1:%d\n", (reg&(1<<17))>>17  );
						PSPRINTF( "PHY0:%d\n", (reg&(1<<16))>>16  );
					
					usb_stor_Bulk_transport_read10(xhci);
					delay(10000);
				}
			#endif
		}
		if( connect_change ==2 ){

			connect_change = 0;

			PSPRINTF( "\nusb disconnect\n" );
			//COND_TRAP(31, 0, 0);

			delay(0x8000);
			//
			state = readl( (UINT32)&xhci->op_regs->status);
			state &= ~CMD_RUN;
			writel( (UINT32)&xhci->op_regs->status,state );

			xhci_mem_free(xhci);
			#if 1
				//clear used buf
				for( i =0; i <0x8000; i+=4)	{
					writel( (XHCI_BUFFERs_BASE + i)  ,0x00000000);
				}
			#endif

			delay(0x8000);
			xhci_mem_init(xhci);
			xhci_run(xhci);
		}

	}
#endif
	//////////////////////////////////////////////////////////////////////
}

