/*******************************************************
 * File: usb_xhci_ring.c
 * Description:eXtensible Host Controller Interface for Universal Serial Bus(xHCI)/Data Structure
 * Date: 2020/07
 * Author: ywang@ccore_ip		gylan@ccore_ip
 *
 ******************************************************/

#include "as_archppc470.h"
#include "ppclib.h"
#include "e500mc_asm.h"
#include "mpic.h"
#include "common_usb.h"
#include "uart.h"

#include "usb_reg_dwc3.h"
#include "usb_reg_xhci.h"

//============================================================================================
//?y============= xhci-dbg functions ============================================================
static void xhci_print_cap_regs(struct xhci_hcd *xhci)
{
	UINT32 temp;
	UINT32 hci_version;
	unsigned int i;

	PSPRINTF("\n xHCI capability registers begin at 0x%x:\n", xhci->cap_regs);

	temp = readl( (UINT32)&xhci->cap_regs->hc_capbase);
	hci_version = HC_VERSION(temp);
	PSPRINTF("  CAPLENGTH AND HCIVERSION 0x%x:\n",(unsigned int) temp);//0x1100020
	PSPRINTF("    CAPLENGTH: 0x%x\n",  (unsigned int) HC_LENGTH(temp));//0x20
	PSPRINTF("    HCIVERSION: 0x%x\n", hci_version);//0x110

	temp = readl( (UINT32)&xhci->cap_regs->hcs_params1);
	PSPRINTF("  HCSPARAMS 1: 0x%x\n",   (unsigned int) temp);//0x2000140
	PSPRINTF("    Max device slots: %d\n", (unsigned int) HCS_MAX_SLOTS(temp));// 64
	PSPRINTF("    Max interrupters: %d\n", (unsigned int) HCS_MAX_INTRS(temp));// 1
	PSPRINTF("    Max ports: %d\n",	     (unsigned int) HCS_MAX_PORTS(temp));  // 2

	temp = readl( (UINT32)&xhci->cap_regs->hcs_params2);
	PSPRINTF("  HCSPARAMS 2: 0x%x\n",		  (unsigned int) temp);	//0x0c0000f1
	PSPRINTF("    Isoc scheduling threshold: %d\n",(unsigned int) HCS_IST(temp));//1
	PSPRINTF("    Maximum allowed segments in event ring: %d\n",
												   (unsigned int) HCS_ERST_MAX(temp));//15

	temp = readl( (UINT32)&xhci->cap_regs->hcs_params3);
	PSPRINTF("  HCSPARAMS 3 0x%x:\n",				(unsigned int) temp); //0x7ff000a
	PSPRINTF("    Worst case U1 device exit latency: %d\n",(unsigned int) HCS_U1_LATENCY(temp));
	PSPRINTF("    Worst case U2 device exit latency: %d\n",	(unsigned int) HCS_U2_LATENCY(temp));

	temp = readl( (UINT32)&xhci->cap_regs->hcc_params);
	PSPRINTF("  HCC PARAMS 0x%x:\n", 		   (unsigned int) temp); //0x220fe6d
	PSPRINTF("    HC generates %s bit addresses\n",HCC_64BIT_ADDR(temp) ? "64" : "32"); //64
	PSPRINTF("    HC %s Contiguous Frame ID Capability\n",
							HCC_CFC(temp) ? "has" : "hasn't"); //hasn't
	PSPRINTF("    HC %s generate Stopped - Short Package event\n",
							HCC_SPC(temp) ? "can" : "can't"); //can


	temp = readl( (UINT32)&xhci->cap_regs->db_off);
	PSPRINTF("  DBOFF  0x%x:\n", temp & DBOFF_MASK); //0x480

	temp = readl( (UINT32)&xhci->cap_regs->run_regs_off);
	PSPRINTF("  RTSOFF 0x%x:\n", temp & RTSOFF_MASK); //0x440

	/* xhci 1.1 controllers have the HCCPARAMS2 register */
	if (hci_version > 0x100)
	{
		temp = readl( (UINT32)&xhci->cap_regs->hcc_params2);
		PSPRINTF("  HCC PARAMS2 0x%x:\n", (unsigned int) temp);
		PSPRINTF("    HC %s Force save context capability\n",
			 				HCC2_FSC(temp) ? "supports" : "doesn't support");
		PSPRINTF("    HC %s Large ESIT Payload Capability\n",
			 				HCC2_LEC(temp) ? "supports" : "doesn't support");
		PSPRINTF("    HC %s Extended TBC capability\n",
			 				HCC2_ETC(temp) ? "supports" : "doesn't support");
	}
}
static void xhci_print_op_regs(struct xhci_hcd *xhci)
{
	UINT32 temp;
	PSPRINTF(" xHCI operational registers begin at 0x%x:\n", xhci->op_regs  ); //

//	xhci_print_command_reg(xhci);
	temp = readl( (UINT32)&xhci->op_regs->command);
	PSPRINTF("  USBCMD 0x%x:\n", temp);	//=0
	PSPRINTF("    HC is %s\n",
						(temp & CMD_RUN) ? "running" : "being stopped");
	PSPRINTF("    HC has %sfinished hard reset\n",
						(temp & CMD_RESET) ? "not " : "");
	PSPRINTF("    Event Interrupts %s\n",
						(temp & CMD_EIE) ? "enabled " : "disabled");
	PSPRINTF("    Host System Error Interrupts %s\n",
						(temp & CMD_HSEIE) ? "enabled " : "disabled");
	PSPRINTF("    HC has %sfinished light reset\n",
						(temp & CMD_LRESET) ? "not " : "");


//	xhci_print_status(xhci);
	temp = readl( (UINT32)&xhci->op_regs->status);
	PSPRINTF("  USBSTS 0x%x:\n", temp);	//0x1
	PSPRINTF("    Event ring is %sempty\n",
			(temp & STS_EINT) ? "not " : "");
	PSPRINTF("    %sHost System Error\n",
			(temp & STS_FATAL) ? "WARNING: " : "No ");
	PSPRINTF("    HC is %s\n",
			(temp & STS_HALT) ? "halted" : "running");
}


static void xhci_print_ports(struct xhci_hcd *xhci)
{
	UINT32 *addr;
	int i, j;
//	int max_ports;
	char *names[NUM_PORT_REGS] = {
		"status",
		"power",
		"link",
		"reserved",
	};

//	max_ports = HCS_MAX_PORTS(xhci->hcs_params1);	// =2
	addr = &xhci->op_regs->port1_status_base;
	PSPRINTF(" xHCI port_status_base begin at 0x%x:\n",addr   );

//	for (i = 0; i < (xhci->max_ports); i++)
	for (i = 0; i < (MAX_HC_PORTS); i++)
	{
		for (j = 0; j < NUM_PORT_REGS; j++)
		{
			PSPRINTF("  (0x%x)/ port %s reg = 0x%x\n",
							addr, names[j],
						//	readl( (UINT32)addr)
							*addr
							);
			addr++;
		}
	}
}

void xhci_print_registers(struct xhci_hcd *xhci)
{
	xhci_print_cap_regs(xhci);
	xhci_print_op_regs(xhci);
	xhci_print_ports(xhci);
}

//Scan the Extended Capabilities
void xhci_ext_cap_regs(struct xhci_hcd *xhci)
{
	UINT8 i;
	UINT32 rddat;
    UINT32 ext_cap_start;
    UINT32 next_cap_offset = 0x00;
    UINT8 major_revision, minor_revision;
    UINT16 port_count,port_offset;

    ext_cap_start = readl( (UINT32)&xhci->cap_regs->hcc_params);
    ext_cap_start = HCC_EXT_CAPS(ext_cap_start) << 2;
#if XHCI_DEBUG
    PSPRINTF("----------- xHCI Extended Capabilities:(offset=0x%x)-----------\n",ext_cap_start );
	for( i=0; i<=0x80; i+=4 )
	{
		LDW( rddat,(USB3_HOST_REG_BADDR + ext_cap_start+i) );
		if( rddat )
			PSPRINTF( "xHCI offset(0x%x) =0x%x\n",  ext_cap_start+i ,   rddat );
	}
//	PSPRINTF( "\n" );
#endif


//find bus protocol
	PSPRINTF("	-----------------------important:---------------------------------\n" );
	do{
		LDW( rddat,(USB3_HOST_REG_BADDR + ext_cap_start + next_cap_offset) );
		if( (XHCI_EXT_CAPS_ID( rddat ) == XHCI_EXT_CAPS_PROTOCOL)
		  &&( readl( USB3_HOST_REG_BADDR + ext_cap_start +next_cap_offset+4) == 0x20425355 ) //USB
		  )
		{
			major_revision = XHCI_EXT_PORT_MAJOR(rddat); //0x02 / 0x03
			minor_revision = XHCI_EXT_PORT_MINOR(rddat);
			port_count  = XHCI_EXT_PORT_COUNT( readl( USB3_HOST_REG_BADDR + ext_cap_start +next_cap_offset+8) );
			port_offset = XHCI_EXT_PORT_OFF( readl( USB3_HOST_REG_BADDR + ext_cap_start +next_cap_offset+8) );

			if (major_revision == 0x02) {
				xhci->total_usb2_ports = port_count;
				xhci->usb2_port_id = port_offset;

				PSPRINTF("	%s(Root Hub) has (%d) USB2.0 port%s:starting from port(%d).\n",
						CHIP_STRING,
						port_count, (port_count == 1) ? "" : "s",	port_offset );
			}
			if (major_revision == 0x03) {
				xhci->total_usb3_ports = port_count;
				xhci->usb3_port_id = port_offset;

				PSPRINTF("	%s(Root Hub) has (%d) USB3.0 port%s:starting from port(%d).\n",
						CHIP_STRING,
						port_count, (port_count == 1) ? "" : "s",	port_offset );
			}
		}

		next_cap_offset += XHCI_EXT_CAPS_NEXT(rddat) <<2;
	}while( XHCI_EXT_CAPS_NEXT(rddat) );
	PSPRINTF("	------------------------------------------------------------------\n\n" );
}




//============================================================================================
//?y=========== Xhci ring basic functions =======================================================
//?y===========  CMD ring basic functions =======================================================

/*    Host Controller Doorbell (0) for CMD Ring , not (1-255)
 * Ring the host controller doorbell after placing a command on the ring
 */
void xhci_ring_cmd_db(struct xhci_hcd *xhci)
{
	//if (!(xhci->cmd_ring_state & CMD_RING_STATE_RUNNING))
	//	return;

#if 0//XHCI_DEBUG
	PSPRINTF("Ding dong!\n");
#endif

	writel( (UINT32)&xhci->dba->doorbell[0], DB_VALUE_HOST  );
	/* Flush PCI posted writes */
	readl( (UINT32)&xhci->dba->doorbell[0] );
}


// Common TRB/Ring Function **********
//************************************

/*	return DMA address
 * Returns zero if the TRB isn't in this segment, otherwise it returns the DMA
 * address of the TRB.
 */
UINT32 trb_offset_to_addr(struct xhci_segment *seg,	union xhci_trb *trb)
{
	UINT32 trboffset_in_segment;

	if (!seg || !trb || (trb < seg->trbs)   )
		return 0;

	/* offset in TRBs */
	trboffset_in_segment = trb - seg->trbs;
	if (trboffset_in_segment >= TRBS_PER_SEGMENT)
		return 0;

	return ((UINT32) seg->trbs) + (trboffset_in_segment * sizeof(*trb))  ;
}

//return 1 = link trb
static UINT8 trb_is_link(union xhci_trb *trb)
{
	return TRB_TYPE_LINK_LE32(trb->link.control);
}

//if trb = 256th TRB in one seg ?
static UINT8 last_trb_on_seg(struct xhci_segment *seg, union xhci_trb *trb)
{
	return (trb == &seg->trbs[TRBS_PER_SEGMENT - 1] );
}

//if trb = 256th TRB in one seg ?
//if seg = last seg in one ring ?
static UINT8 last_trb_on_ring(struct xhci_ring *ring,
			struct xhci_segment *seg, union xhci_trb *trb)
{
	return (last_trb_on_seg(seg, trb) &&
			(seg->next == ring->first_seg)
				);
}

static UINT8 link_trb_toggles_cycle(union xhci_trb *trb)
{
	return le32_to_cpu(trb->link.control) & LINK_TOGGLE;
}


/* 	  most time,for cmd/trans ring
 * See Cycle bit rules. Don't make a ring full of link TRBs.  That would be dumb and this would loop.
 * If we've just enqueued a TRB that is in the middle of a TD (meaning the
 * chain bit is set), then set the chain bit in all the following link TRBs.
 * If we've enqueued the last TRB in a TD, make sure the following link TRBs
 * have their chain bit cleared (so that each Link TRB is a separate TD).
 */
static void inc_enq(struct xhci_hcd *xhci, struct xhci_ring *ring,
					UINT8 more_trbs_coming	)
{
	u32 chain;
	union xhci_trb *next;

	chain = le32_to_cpu(ring->enqueue->generic.field[3]) & TRB_CHAIN;

	/* If this is not event ring, there is one less usable TRB */
//	if (!trb_is_link(ring->enqueue))
//		ring->num_trbs_free--;

	next = ++(ring->enqueue);			//-------
	/* Update the dequeue pointer further if that was a link TRB */
	while (trb_is_link(next)) {
	//	COND_TRAP(31, 0, 0);

		/*
		 * If the caller doesn't plan on enqueueing more TDs before
		 * ringing the doorbell, then we don't want to give the link TRB
		 * to the hardware just yet. We'll give the link TRB back in
		 * prepare_ring() just before we enqueue the TD at the top of
		 * the ring.
		 */
		if (!chain && !more_trbs_coming)
			break;

		next->link.control ^= cpu_to_le32(TRB_CYCLE);

		/* Toggle the cycle bit after the last ring segment. */
		if (link_trb_toggles_cycle(next))
			ring->cycle_state ^= 1;

		ring->enq_seg = ring->enq_seg->next;
		ring->enqueue = ring->enq_seg->trbs;
		next = ring->enqueue;
	}
}



/*	  most time,for event ring
 * See Cycle bit rules. SW is the consumer for the event ring only.
 * Don't make a ring full of link TRBs.  That would be dumb and this would loop.
 */
//static
void inc_deq(struct xhci_hcd *xhci, struct xhci_ring *ring)
{
	/* event ring doesn't have link trbs, check for last trb */
	if (ring->type == TYPE_EVENT) {
		if ( !last_trb_on_seg(ring->deq_seg, ring->dequeue)  ){
			ring->dequeue ++;				//-------
			return;
		}

		if (last_trb_on_ring(ring, ring->deq_seg, ring->dequeue)  ){
		//	COND_TRAP(31, 0, 0);

			ring->cycle_state ^= 1;
		}
		ring->deq_seg = ring->deq_seg->next;
		ring->dequeue = ring->deq_seg->trbs;

		return;
	}

	/* All other rings have link trbs */
	if (!trb_is_link(ring->dequeue)) {
		ring->dequeue++;
	//	ring->num_trbs_free++;
	}
	while (trb_is_link(ring->dequeue)) {
		ring->deq_seg = ring->deq_seg->next;
		ring->dequeue = ring->deq_seg->trbs;
	}

	return;
}



/****  Basic function, for fill Trans/CMD Ring Operations	****/
/* Generic function for queueing a TRB on a ring.
 * The caller must have checked to make sure there's room on the ring.
 *
 * @more_trbs_coming:	Will you enqueue more TRBs before calling prepare_transfer()?
 */
static void queue_trb(struct xhci_hcd *xhci,
			struct xhci_ring *ring,
			UINT8 more_trbs_coming,
			UINT32 field1, UINT32 field2, UINT32 field3, UINT32 field4)
{//<--cmd or trans will call this

	struct xhci_generic_trb *trb;

	trb = &ring->enqueue->generic;
	trb->field[0] = cpu_to_le32(field1);
	trb->field[1] = cpu_to_le32(field2);
	trb->field[2] = cpu_to_le32(field3);
	trb->field[3] = cpu_to_le32(field4);

	inc_enq(xhci, ring, more_trbs_coming);

#if 0//XHCI_DEBUG
	PSPRINTF( " queue_trb()(enq& =0x%x) = 0x%x,0x%x,0x%x,0x%x =(%s)TRB\n",
		(trb),
		le32_to_cpu(trb->field[0] ),le32_to_cpu(trb->field[1] ),
		le32_to_cpu(trb->field[2] ),le32_to_cpu(trb->field[3] ),
		xhci_trb_type_string( TRB_FIELD_TO_TYPE( le32_to_cpu(trb->field[3]))  )
	);
#endif

}


/****	Generic function, for Command Ring Operations		****/
/* Generic function for queueing a command TRB on the command ring.
 * Check to make sure there's room on the command ring for one command TRB.
 */
void queue_command(struct xhci_hcd *xhci,
						 UINT32 field1, UINT32 field2,
						 UINT32 field3, UINT32 field4,
						 UINT8 command_must_succeed)
{
	/* Software -> Write */
	queue_trb(xhci,
				xhci->cmd_ring,
			//	false,------
				true,
				field1, field2, field3,
				field4 | xhci->cmd_ring->cycle_state);
}



//============================================================================================
//?y============= Transfer (ring) basic functions ==============================================

static void xhci_ring_ep_doorbell( struct xhci_hcd *xhci,
							UINT32 ep_index,	//-> DB Target
							UINT32 start_cycle,
							struct xhci_generic_trb *start_trb
							)
{
	UINT32 stream_id = 0;

	/* 1.write TRB[Cycle bit (C).]
	 * 2.Ring the Doorbell[slot_id] <--DB Target = Control EP 0 Enqueue Pointer Update
	 */
	if (start_cycle)
		start_trb->field[3] |= cpu_to_le32(start_cycle);
	else
		start_trb->field[3] &= cpu_to_le32(~TRB_CYCLE);

	/* Don't ring the doorbell for this endpoint if there are pending
	 * cancellations because we don't want to interrupt processing.
	 * We don't want to restart any stream rings if there's a set dequeue
	 * pointer command pending because the device can choose to start any
	 * stream once the endpoint is on the HW schedule.
	 */


	writel( (UINT32)&xhci->dba->doorbell[xhci->cur_slot_id], 	/* relation: slot_id */
			DB_VALUE(ep_index, stream_id) );
	/* The CPU has better things to do at this point than wait for a
	 * write-posting flush.  It'll get there soon enough.
	 */
}

/* just for Ep0 control Trans */
void xhci_queue_ctrl_tx(struct xhci_hcd *xhci,
								struct usb_ctrlrequest *pReq,
								UINT8 *data_stage_buf,
								UINT16 data_buf_len
								)
{
//	UINT8 num_trbs;
	UINT32 field;  // TRB field4
	UINT32 start_cycle;
	struct xhci_generic_trb *start_trb;

	/* 1 TRB for setup, 1 for status */
//	num_trbs = 2;
	/* 		have data stage
	 * Don't need to check if we need additional event data and normal TRBs,
	 * since data in control transfers will never get bigger than 16MB
	 * XXX: can we get a buffer that crosses 64KB boundaries?
	 */
//	if (pReq->wLength > 0)
//		num_trbs++;

	/*
	 * Don't give the first TRB to the hardware (by toggling the cycle bit)
	 * until we've finished creating all the other TRBs.  The ring's cycle
	 * state may change as we enqueue the other TRBs, so save it too.
	 */
	start_trb = &xhci->ep_ring[0]->enqueue->generic;		// save setup TRB 1st
	start_cycle = xhci->ep_ring[0]->cycle_state;//

#if 1
	field = 0;
	field |= TRB_IDT | TRB_TYPE(TRB_SETUP) ;//	|TRB_CYCLE;
	if (start_cycle == 0)
		field |= TRB_CYCLE;
	if (data_buf_len > 0)
	{//if has Data Stage
		if (pReq->bRequestType & USB_DIR_IN)
			field |= TRB_TX_TYPE(TRB_DATA_IN);
		else
			field |= TRB_TX_TYPE(TRB_DATA_OUT);
	}

/* Queue setup TRB - Setup Stage TRB */
	queue_trb(xhci,
			xhci->ep_ring[0],
			true,  //not last

			pReq->bRequestType | (pReq->bRequest << 8) |  (le16_to_cpu(pReq->wValue) << 16),
			le16_to_cpu(pReq->wIndex) | (le16_to_cpu(pReq->wLength) << 16),
			TRB_LEN(8) | TRB_INTR_TARGET(0),
			/* Immediate data in pointer */
			field
		);	//->inc enqueue


	/* update deq of xhci->ep_ring */
	inc_deq(xhci, xhci->ep_ring[0]);
#endif

#if 1
/* If there's data, queue data TRBs -Data Stage TRB */

	/* Only set interrupt on short packet for IN endpoints */
	if ( pReq->bRequestType & USB_DIR_IN )
		field = TRB_ISP 		//Interrupt-on Short Packet (ISP). 1st get dev,gen short event; other, no
			| TRB_TYPE(TRB_DATA);
	else
		field = TRB_TYPE(TRB_DATA);

	if (data_buf_len > 0)
	{
		if (pReq->bRequestType & USB_DIR_IN)
				field |= TRB_DIR_IN;

		queue_trb(xhci,
				xhci->ep_ring[0],
				true, //not last

				(UINT32)data_stage_buf,
				XHCI_BUFFERs_BASE_HI,
			//	TRB_LEN(pReq->wLength)   err 0x4000
				TRB_LEN(data_buf_len)
				| TRB_INTR_TARGET(0) | TRB_TD_SIZE(0),
				field | xhci->ep_ring[0]->cycle_state);
		//->inc enqueue
	}
#endif

#if 1
/* Queue status TRB - Status Stage TRB */

	/* If the device sent data, the status stage is an OUT transfer */
	if ( (data_buf_len > 0) &&
			(pReq->bRequestType & USB_DIR_IN) )
		field = 0;	//get des
	else
		field = TRB_DIR_IN;

	queue_trb(xhci,xhci->ep_ring[0],
		//	false,  //last,not more------
			true,
			0,	0,
			TRB_INTR_TARGET(0),
			/* Event(Interrupt) on completion (IOC) */
			field | TRB_IOC | TRB_TYPE(TRB_STATUS) | xhci->ep_ring[0]->cycle_state);
	//->inc enqueue
#endif

#if 0
//print Ctrl-Setup Stage TRB
//recommendation:   do not use dequeue
	//PSPRINTF("xhci_queue_ctrl_tx(Setup TRB& =0x%x)=0x%x,0x%x,0x%x,0x%x\n",
	//		xhci->ep_ring[0]->dequeue,
	//		le32_to_cpu(xhci->ep_ring[0]->dequeue->generic.field[0]),le32_to_cpu(xhci->ep_ring[0]->dequeue->generic.field[1]),
	//		le32_to_cpu(xhci->ep_ring[0]->dequeue->generic.field[2]),le32_to_cpu(xhci->ep_ring[0]->dequeue->generic.field[3])   );

	//PSPRINTF("xhci_queue_ctrl_tx(data TRB& =0x%x)=0x%x,0x%x,0x%x,0x%x\n",
	//		(xhci->ep_ring[0]->dequeue+1),
	//		le32_to_cpu((xhci->ep_ring[0]->dequeue+1)->generic.field[0]),le32_to_cpu((xhci->ep_ring[0]->dequeue+1)->generic.field[1]),
	//		le32_to_cpu((xhci->ep_ring[0]->dequeue+1)->generic.field[2]),le32_to_cpu((xhci->ep_ring[0]->dequeue+1)->generic.field[3])   );

	PSPRINTF("xhci_queue_ctrl_tx(Setup TRB& =0x%x)=0x%x,0x%x,0x%x,0x%x\n",
			start_trb,
			le32_to_cpu(start_trb->field[0]),le32_to_cpu(start_trb->field[1]),
			le32_to_cpu(start_trb->field[2]),le32_to_cpu(start_trb->field[3])   );

	PSPRINTF("xhci_queue_ctrl_tx(data TRB& =0x%x)=0x%x,0x%x,0x%x,0x%x\n",
			(start_trb+1),
			le32_to_cpu((start_trb+1)->field[0]),le32_to_cpu((start_trb+1)->field[1]),
			le32_to_cpu((start_trb+1)->field[2]),le32_to_cpu((start_trb+1)->field[3])   );
#endif

	xhci_ring_ep_doorbell(xhci , 0,
							start_cycle, start_trb);
}



/* for Ep n Bulk Trans */
void xhci_queue_bulk_tx(struct xhci_hcd *xhci,
								UINT8 ep_index,
								UINT8 *p_data_buf,
								UINT16 data_buf_len
								)
{
	UINT32 field;  // TRB field4
	UINT32 start_cycle;
	struct xhci_generic_trb *start_trb;
//	struct xhci_ring 	*ring;	//ep[x] ring;  diff from ctrl-ep0-ring
	UINT8 need_zero_pkt = false;

//xhci to transfer_ring
//	ring =  &xhci->ep_ring[ep_index];  //dir


	/*
	 * Don't give the first TRB to the hardware (by toggling the cycle bit)
	 * until we've finished creating all the other TRBs.  The ring's cycle
	 * state may change as we enqueue the other TRBs, so save it too.
	 */
	start_trb = &xhci->ep_ring[ep_index]->enqueue->generic;		// save setup TRB 1st
	start_cycle = xhci->ep_ring[ep_index]->cycle_state;//

#if 1
/* Queue the Transfer TRBs[Normal TRB], even if they are zero-length
*/
	field = TRB_TYPE(TRB_NORMAL);//	|TRB_CYCLE;
	/* Don't change the cycle bit of the first TRB until later */
	if (start_cycle == 0)
		field |= TRB_CYCLE;
	/* Only set interrupt on short packet for IN endpoints */
	if ( ep_index == xhci->conn_dev_EpIn_index )
		field |= TRB_ISP;

	//TRB_CHAIN ?

	queue_trb(xhci,
			xhci->ep_ring[ep_index],
		//	false, //last ??------
			true,

			(UINT32)p_data_buf,
			XHCI_BUFFERs_BASE_HI,

			TRB_LEN(data_buf_len) | TRB_INTR_TARGET(0) | TRB_TD_SIZE(0),
			field | TRB_IOC );
	//->inc enqueue
#endif


#if 0
//print Bulk Normal TRB (not ep0 ring)

//recommendation:   do not use dequeue

	PSPRINTF("xhci_queue_bulk_tx(Nor TRB& =0x%x)=0x%x,0x%x,0x%x,0x%x\n",
			start_trb,
			le32_to_cpu(start_trb->field[0]),	le32_to_cpu(start_trb->field[1]),
			le32_to_cpu(start_trb->field[2]),	le32_to_cpu(start_trb->field[3])   );
#endif

	xhci_ring_ep_doorbell(xhci , ep_index,
							start_cycle, start_trb);
}


//============================================================================================
//?y=========== Context basic functions ========================================================
struct xhci_input_control_ctx *xhci_get_input_control_ctx(
					      struct xhci_container_ctx *ctx)
{
	if (ctx->type != XHCI_CTX_TYPE_INPUT) {
		COND_TRAP(31, 0, 0);
		return 0;
	}

	return (struct xhci_input_control_ctx *)ctx->bytes;
}

struct xhci_slot_ctx *xhci_get_slot_ctx(struct xhci_hcd *xhci,
								struct xhci_container_ctx *ctx)
{
	//Device Context Data Structure
	if (ctx->type == XHCI_CTX_TYPE_DEVICE)
		return (struct xhci_slot_ctx *)ctx->bytes;

	//Input Context
	return (struct xhci_slot_ctx *)
				(ctx->bytes + CTX_SIZE(xhci->hcc_params)) ;
}

struct xhci_ep_ctx *xhci_get_ep_ctx(struct xhci_hcd *xhci,
				    struct xhci_container_ctx *ctx,
				    unsigned int ep_index)
{
	/* increment ep index by offset of start of ep ctx array */
	ep_index++;	//DCI			//xhci_slot_ctx
	if (ctx->type == XHCI_CTX_TYPE_INPUT)
		ep_index++;			//xhci_input_control_ctx

	return (struct xhci_ep_ctx *)
				(ctx->bytes +
				(ep_index * CTX_SIZE(xhci->hcc_params))
				);
}


//Slot Context[31:27 Slot State.]
inline char *xhci_slot_state_string(u32 state)
{
	switch (state) {
	case SLOT_STATE_ENABLED:
		return "enabled/disabled";
	case SLOT_STATE_DEFAULT:
		return "default";
	case SLOT_STATE_ADDRESSED:
		return "addressed";
	case SLOT_STATE_CONFIGURED:
		return "configured";
	default:
		return "reserved";
	}
}

//Endpoint Context[2:0 Endpoint State (EP State).]
inline const char *xhci_ep_state_string( UINT8 state)
{
	switch (state) {
	case EP_STATE_DISABLED:
		return "disabled";
	case EP_STATE_RUNNING:
		return "running";
	case EP_STATE_HALTED:
		return "halted";
	case EP_STATE_STOPPED:
		return "stopped";
	case EP_STATE_ERROR:
		return "error";
	default:
		return "INVALID";
	}
}
//end?จน=========== Context basic functions ========================================================


//============================================================================================
//=========== USB 2.0/3.0 common functions ===================================================
/**
 * usb_endpoint_num - get the endpoint's number
 * @epd: endpoint to be checked
 *
 * Returns @epd's number: 0 to 15.
 */
static inline int usb_endpoint_num(const struct usb_endpoint_descriptor *epd)
{
	return epd->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
}
/**
 * usb_endpoint_type - get the endpoint's transfer type
 * @epd: endpoint to be checked
 *
 * Returns one of USB_ENDPOINT_XFER_{CONTROL, ISOC, BULK, INT} according
 * to @epd's transfer type.
 */
inline int usb_endpoint_type(const struct usb_endpoint_descriptor *epd)
{
	return epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;
}

/**
 * usb_endpoint_dir_in - check if the endpoint has IN direction
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type IN, otherwise it returns false.
 */
inline int usb_endpoint_dir_in(const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN);
}
/**
 * usb_endpoint_dir_out - check if the endpoint has OUT direction
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type OUT, otherwise it returns false.
 */
inline int usb_endpoint_dir_out(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT);
}


/**
 * usb_endpoint_xfer_bulk - check if the endpoint has bulk transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type bulk, otherwise it returns false.
 */
inline int usb_endpoint_xfer_bulk(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		USB_ENDPOINT_XFER_BULK);
}
/**
 * usb_endpoint_xfer_control - check if the endpoint has control transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type control, otherwise it returns false.
 */
inline int usb_endpoint_xfer_control(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		USB_ENDPOINT_XFER_CONTROL);
}

/**
 * usb_endpoint_maxp - get endpoint's max packet size
 * @epd: endpoint to be checked
 *
 * Returns @epd's max packet bits [10:0]
 */
inline int usb_endpoint_maxp(const struct usb_endpoint_descriptor *epd)
{
	return le16_to_cpu( epd->wMaxPacketSize)  & USB_ENDPOINT_MAXP_MASK;
}

UINT32 xhci_get_endpoint_type_in_Ep_Ctx(struct usb_endpoint_descriptor *epd)
{
	int in;

	in = usb_endpoint_dir_in(epd);

	switch (usb_endpoint_type(epd))
	{
	case USB_ENDPOINT_XFER_CONTROL:
		return CTRL_EP;

	case USB_ENDPOINT_XFER_BULK:
		return in ? BULK_IN_EP : BULK_OUT_EP;

	case USB_ENDPOINT_XFER_ISOC:
		return in ? ISOC_IN_EP : ISOC_OUT_EP;
	case USB_ENDPOINT_XFER_INT:
		return in ? INT_IN_EP : INT_OUT_EP;
	}
	return 0;
}


/**
 * xhci_get_endpoint_index - Used for passing endpoint bitmasks between the core and
 * HCDs.  Find the index for an endpoint given its descriptor.  Use the return
 * value to right shift 1 for the bitmask.
 *
 * 			Index  = (epnum * 2) + direction - 1, (0=OUT/i=IN)
 * 		where direction = 0 for OUT, 1 for IN.

 * For control endpoints, the IN index is used (OUT index is unused), so
 * index = (epnum * 2) + direction - 1 = (epnum * 2) + 1 - 1 = (epnum * 2) <===== control
 */
UINT32 xhci_get_endpoint_index(struct usb_endpoint_descriptor *desc)
{
	UINT32 index;
	if (usb_endpoint_xfer_control(desc))
		index = (UINT32) (usb_endpoint_num(desc)*2);
							//ep_index=0->EP Context 0 ->DCI=1
	else
		index = (UINT32) (usb_endpoint_num(desc)*2) +
							(usb_endpoint_dir_in(desc) ? 1 : 0) - 1; //Index  = (epnum * 2) + direction - 1, (0=OUT/1=IN)
							//ep_index=1(EP 1 OUT)
							//ep_index=2(EP 1 IN)   ->DCI=3
							//ep_index=3(EP 2 OUT)	->DCI=4
							//ep_index=4(EP 2 IN)

	return index;  //index +1=DCI
}
//end ?จน=========== USB 2.0/3.0 common functions ===================================================







//============================================================================================
//bulk============================================================================================
const char  *SCSI_UFI_OPCODES_string(unsigned char op_core)
{
	switch (op_core)
	{
	case TEST_UNIT_READY:      return "TEST_UNIT_READY";
	case REZERO_UNIT	:      return "REZERO_UNIT";
	case REQUEST_SENSE	:      return "REQUEST_SENSE";
	case FORMAT_UNIT	:      return "FORMAT_UNIT";
	case READ_BLOCK_LIMITS:  return "READ_BLOCK_LIMITS";
	case REASSIGN_BLOCKS:     return "REASSIGN_BLOCKS";
	case READ_6			:      return "READ_6";
	case WRITE_6			:      return "WRITE_6";
	case SEEK_6			:      return "SEEK_6";
	case READ_REVERSE	:      return "READ_REVERSE";
	case WRITE_FILEMARKS:      return "WRITE_FILEMARKS";
	case SPACE			:      return "SPACE";
	case INQUIRY			:      return "INQUIRY";
	case RECOVER_BUFFERED_DATA:      return "RECOVER_BUFFERED_DATA";
	case MODE_SELECT	:      return "MODE_SELECT";
	case RESERVE			:      return "RESERVE";
	case RELEASE			:      return "RELEASE";
	case COPY			:      return "COPY";
	case ERASE			:      return "ERASE";
	case MODE_SENSE		:      return "MODE_SENSE";
	case START_STOP		:      return "START_STOP";
	case RECEIVE_DIAGNOSTIC:      return "RECEIVE_DIAGNOSTIC";
	case SEND_DIAGNOSTIC:      return "SEND_DIAGNOSTIC";
	case ALLOW_MEDIUM_REMOVAL:      return "ALLOW_MEDIUM_REMOVAL";
	case SET_WINDOW	:      return "SET_WINDOW";
	case READ_CAPACITY	:      return "READ_CAPACITY";
	case READ_10		:      return "READ_10";
	case WRITE_10		:      return "WRITE_10";
	case SEEK_10			:      return "SEEK_10";
	case WRITE_VERIFY	:      return "WRITE_VERIFY";
	case VERIFY			:      return "VERIFY";
	case SEARCH_HIGH	:      return "SEARCH_HIGH";
	case SEARCH_EQUAL	:      return "SEARCH_EQUAL";
	case SEARCH_LOW		:      return "SEARCH_LOW";
	case SET_LIMITS		:      return "SET_LIMITS";
	case READ_POSITION	:      return "READ_POSITION";
	case SYNCHRONIZE_CACHE:      return "SYNCHRONIZE_CACHE";
	case LOCK_UNLOCK_CACHE:      return "LOCK_UNLOCK_CACHE";
	case READ_DEFECT_DATA:      return "READ_DEFECT_DATA";
	case MEDIUM_SCAN	:      return "MEDIUM_SCAN";
	case COMPARE		:      return "COMPARE";
	case COPY_VERIFY		:      return "COPY_VERIFY";
	case WRITE_BUFFER	:      return "WRITE_BUFFER";
	case READ_BUFFER	:      return "READ_BUFFER";
	case UPDATE_BLOCK	:      return "UPDATE_BLOCK";
	case READ_LONG		:      return "READ_LONG";
	case WRITE_LONG		:      return "WRITE_LONG";
	case CHANGE_DEFINITION:      return "CHANGE_DEFINITION";
	case WRITE_SAME		:      return "WRITE_SAME";
	//case GPCMD_READ_SUBCHANNEL:      return "READ SUBCHANNEL";
	case READ_TOC		:      return "READ_TOC";
	//case GPCMD_READ_HEADER:      return "READ HEADER";
	//case GPCMD_PLAY_AUDIO_10:      return "PLAY AUDIO (10)";
	//case GPCMD_PLAY_AUDIO_MSF:      return "PLAY AUDIO MSF";
	//case GPCMD_GET_EVENT_STATUS_NOTIFICATION:
		     return "GET EVENT/STATUS NOTIFICATION";
	//case GPCMD_PAUSE_RESUME:      return "PAUSE/RESUME";
	case LOG_SELECT		:      return "LOG_SELECT";
	case LOG_SENSE		:      return "LOG_SENSE";
	//case GPCMD_STOP_PLAY_SCAN:      return "STOP PLAY/SCAN";
	//case GPCMD_READ_DISC_INFO:      return "READ DISC INFORMATION";
	//case GPCMD_READ_TRACK_RZONE_INFO:
		     return "READ TRACK INFORMATION";
	//case GPCMD_RESERVE_RZONE_TRACK:      return "RESERVE TRACK";
	//case GPCMD_SEND_OPC:      return "SEND OPC";
	case MODE_SELECT_10	:      return "MODE_SELECT_10";
	//case GPCMD_REPAIR_RZONE_TRACK:      return "REPAIR TRACK";
	case 0x59			:      return "READ MASTER CUE";
	case MODE_SENSE_10	:      return "MODE_SENSE_10";
	//case GPCMD_CLOSE_TRACK:      return "CLOSE TRACK/SESSION";
	case 0x5C			:      return "READ BUFFER CAPACITY";
	case 0x5D			:      return "SEND CUE SHEET";
	//case GPCMD_BLANK	:      return "BLANK";
	case REPORT_LUNS	:      return "REPORT LUNS";
	case MOVE_MEDIUM	:      return "MOVE_MEDIUM or PLAY AUDIO (12)";
	case READ_12		:      return "READ_12";
	case WRITE_12		:      return "WRITE_12";
	case WRITE_VERIFY_12	:      return "WRITE_VERIFY_12";
	case SEARCH_HIGH_12	:      return "SEARCH_HIGH_12";
	case SEARCH_EQUAL_12:      return "SEARCH_EQUAL_12";
	case SEARCH_LOW_12	:      return "SEARCH_LOW_12";
	case SEND_VOLUME_TAG:      return "SEND_VOLUME_TAG";
	case READ_ELEMENT_STATUS:      return "READ_ELEMENT_STATUS";
	//case GPCMD_READ_CD_MSF:      return "READ CD MSF";
	//case GPCMD_SCAN		:      return "SCAN";
	//case GPCMD_SET_SPEED:      return "SET CD SPEED";
	//case GPCMD_MECHANISM_STATUS:      return "MECHANISM STATUS";
	//case GPCMD_READ_CD	:      return "READ CD";
	case 0xE1			:      return "WRITE CONTINUE";
	case WRITE_LONG_2	:      return "WRITE_LONG_2";

	default:
		return "Unknown";
	}
}

