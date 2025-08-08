/*
 * quicc.c
 *
 *  Created on: 2021-12-15
 *      Author: scgao
 */
#include "quicc.h"
#include "cpu0_rom.h"
#include "cpu1_rom.h"
#include "e500mc_asm.h"

#define w32(address, data) 			*(volatile unsigned int *)(address) = data
#define r32(address) 				*(volatile unsigned int *)(address)

#define w16(address, data) 			*(volatile unsigned short *)(address) = data
#define r16(address) 				*(volatile unsigned short *)(address)

#define w8(address, data) 			*(volatile unsigned char *)(address) = data
#define r8(address) 				*(volatile unsigned char *)(address)

#define QUICC_SRAM_TEST	1
#define QUICC_HDLC_TEST	0
#define QUICC_HDLC_BUS_TEST	0
#define QUICC_TDM_TEST	0
#define QUICC_GPIO_TEST	0
#define QUICC_UART_TEST	0
#define QUICC_C0_TEST	0
#define QUICC_DMA_TEST	0

#if QUICC_HDLC_TEST
#define HDLC_SIMPLE_TRANSFER_TEST	0
#define HDLC_LOOPBACK_TEST			0
#define HDLC_TRANSECIVE_TEST		0
#define HDLC_RX_FILTER_TEST			0
#define HDLC_ECHO_TEST				0
#define HDLC_MASS_TRANSECIVE_TEST	1
#endif

#if QUICC_HDLC_TEST || QUICC_HDLC_BUS_TEST || QUICC_TDM_TEST
#define ADDR_16BIT		1
#define CRC_32BIT		1
#define RX_BUFFER_64BYTE	1
#endif

#if QUICC_HDLC_BUS_TEST
#define AUTO_RETRANSMIT_ENABLE	1
#endif

#if QUICC_GPIO_TEST
#define GPIO_OUTPUT_TEST			1
#define GPIO_INPUT_TEST				0
#define GPIO_INTERCONNECT_TEST		0
#endif

#if QUICC_TDM_TEST
#define TDM_SIMPLE_TEST				1
#define TDM_EQUAL_TRANSECIVE_TEST	0
#endif

#if QUICC_UART_TEST
#define UART_TEST_0					0
#define UART_TEST_1					1
#endif
//void delay(volatile unsigned int time)
//{
//	while(time)
//		time--;
//}

//unsigned int swap_word(unsigned int data)
//{
//	return (((data & 0x000000ff) << 24) | ((data & 0x0000ff00) << 8) | ((data & 0x00ff0000) >> 8) | ((data & 0xff000000) >> 24));
//}

#if QUICC_SRAM_TEST
void quicc_sram_test()
{
	unsigned int i, val;
	
	for(i=0; i<QUICC_SRAM_SIZE; i++)
	{
		w8(QUICC_SRAM_BASE+i, i&0xff);
	}
	for(i=0; i<QUICC_SRAM_SIZE; i++)
	{
		val = r8(QUICC_SRAM_BASE+i);
		if(val != (i&0xff))
			while(1);
	}
	
	for(i=0; i<QUICC_SRAM_SIZE; i+=2)
	{
		w16(QUICC_SRAM_BASE+i, i&0xffff);
	}
	for(i=0; i<QUICC_SRAM_SIZE; i+=2)
	{
		val = r16(QUICC_SRAM_BASE+i);
		if(val != (i&0xffff))
			while(1);
	}
	
	for(i=0; i<QUICC_SRAM_SIZE; i+=4)
	{
		w32(QUICC_SRAM_BASE+i, 0x5a5a5a5a+i);
	}
	for(i=0; i<QUICC_SRAM_SIZE; i+=4)
	{
		val = r32(QUICC_SRAM_BASE+i);
		if(val != i+0x5a5a5a5a)
			while(1);
	}
}
#endif

void hdlc_reset(struct hdlc_regs *p_hdlc_regs)
{
	unsigned int val;
	
	//w32(&p_hdlc_regs->ccr, HDLC_CCR_RST);
	p_hdlc_regs->ccr = HDLC_CCR_RST;
	do{
		//val = r32(&p_hdlc_regs->ccr);
		val = p_hdlc_regs->ccr;
	}while(val & HDLC_CCR_RST);
}

void qe_ucc_init()
{
#if 1//2M clock
	w32(QUICC_QER_BASE+0x28, 1);
	w32(QUICC_QER_BASE+0x2c, 75);
	w32(QUICC_QER_BASE+0x30, 1);
	w32(QUICC_QER_BASE+0x34, 75);	
#endif
#if 1
	//enable all hdlc pins
	w32(QUICC_QER_BASE+0x50, 0xd8000000 | 0x007e6c00);
	w32(QUICC_QER_BASE+0x54, 0x1fe6c000 | 0x11fc0 | 0xc0000000);
	w32(QUICC_QER_BASE+0x58, 0xdbf00000 | 0x1ffb0);
	w32(QUICC_QER_BASE+0x5c, 0xc00000 | 0x3fe780);
	w32(QUICC_QER_BASE+0x60, 0x3cf6f780);
	w32(QUICC_QER_BASE+0x64, 0x3cf6f780);

#endif
	w32(QUICC_QER_BASE+0x8, 0xaaaa9999);//tx clock select,0xa:E1,0x9:T1
	w32(QUICC_QER_BASE+0xc, 0x88888888);//rx clock select,0x8:input
	w32(QUICC_QER_BASE+0x10, 0x88888888);//set rx data input
	//w32(QUICC_QER_BASE+0x14, 0x33333377);
	w32(QUICC_QER_BASE+0x14, 0);//set data x1
	//set data pin pullup disable
	w32(QUICC_QER_BASE+0x154, 0);
	w32(QUICC_QER_BASE+0x158, 0);
	w32(QUICC_QER_BASE+0x15c, 0);
	w32(QUICC_QER_BASE+0x160, 0);
	w32(QUICC_QER_BASE+0x164, 0);
	w32(QUICC_QER_BASE+0x168, 0);
	w32(QUICC_QER_BASE+0x16c, 0);
	w32(QUICC_QER_BASE+0x170, 0);
}

/*
 * hdlc6 tx and rx interconnect
 */
void hdlc_simple_transfer_test()
{
	struct hdlc_regs *p_hdlc0_regs = QUICC_HDLC_REGS_BASE(QUICC_HDLC_CHN);
	struct hdlc_bd *p_hdlc0_txbd = QUICC_HDLC_TXBD_BASE(QUICC_HDLC_CHN);
	unsigned int i, val;
	
	hdlc_reset(p_hdlc0_regs);
	p_hdlc0_regs->isr = HDLC_ISR_ALL;
	
	for(i=0; i<0x400; i+=4)
	{
		w32(QUICC_HDLC_TX_BUFFER(QUICC_HDLC_CHN)+i, 0xa5a5a5a5);
	}
	
	p_hdlc0_txbd[0].len_ctrl = (0x8<<16) | HDLC_TXBD_W | HDLC_TXBD_L | HDLC_TXBD_I;
	p_hdlc0_txbd[0].pointer = 0xc00;
	p_hdlc0_txbd[0].len_ctrl |= HDLC_TXBD_R;
	
	p_hdlc0_regs->ccr |= HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
	
	do{
		val = p_hdlc0_regs->isr;
	}while((val & (HDLC_ISR_TXBE | HDLC_ISR_TXFE)) != (HDLC_ISR_TXBE | HDLC_ISR_TXFE));
	
	//pass
}

void malloc_hdlc(struct hdlc_st *hdlc, unsigned int index)
{
	memset(hdlc, 0, sizeof(struct hdlc_st));
	
	hdlc->index = index;
	hdlc->p_hdlc_regs = QUICC_HDLC_REGS_BASE(index);
	hdlc->p_hdlc_rxbd = QUICC_HDLC_RXBD_BASE(index);
	hdlc->p_hdlc_txbd = QUICC_HDLC_TXBD_BASE(index);
	hdlc->tx_buffer = QUICC_HDLC_TX_BUFFER(index);
	hdlc->rx_buffer = QUICC_HDLC_RX_BUFFER(index);
	hdlc->mrblr = 0;
	hdlc->addrm = 0;
}

void init_rx_bds(struct hdlc_st *hdlc)
{
	unsigned int b_size = hdlc->mrblr ? 64: 32;
	unsigned int i;
	
	for(i=0; i<0x400/b_size; i++)
	{
		hdlc->p_hdlc_rxbd[i].len_ctrl = HDLC_RXBD_E | HDLC_RXBD_I;
	}
	hdlc->p_hdlc_rxbd[i-1].len_ctrl |= HDLC_RXBD_W;
}

void init_tx_bds(struct hdlc_st *hdlc)
{
	unsigned int b_size = 32;
	unsigned int i;	
	
	for(i=0; i<0x400/b_size; i++)
	{
		hdlc->p_hdlc_txbd[i].len_ctrl = HDLC_TXBD_I;
	}
	hdlc->p_hdlc_txbd[i-1].len_ctrl |= HDLC_TXBD_W;
}

void hdlc_loopback_test()
{
	struct hdlc_st hdlc0;
	unsigned int i, val;
	
	malloc_hdlc(&hdlc0, 0);
	
	hdlc_reset(hdlc0.p_hdlc_regs);
	
	hdlc0.p_hdlc_regs->modr = HDLC_MODR_DIAG_LOOPBACK;
	hdlc0.p_hdlc_regs->isr = HDLC_ISR_ALL;
	
	init_rx_bds(&hdlc0);
	hdlc0.p_hdlc_regs->ccr |= HDLC_CCR_OPCODER_INIT | HDLC_CCR_NCMDR;
	hdlc0.p_hdlc_regs->ccr |= HDLC_CCR_OPCODER_START | HDLC_CCR_NCMDR;
	hdlc0.p_hdlc_regs->ccr &= (~HDLC_CCR_NCMDR);
	
	for(i=0; i<0x400; i+=4)
	{
		w32(QUICC_HDLC_TX_BUFFER(0)+i, 0xa5a5a5a5+i);
	}
	
	hdlc0.p_hdlc_txbd[0].len_ctrl = (0x20<<16) | HDLC_TXBD_W | HDLC_TXBD_L | HDLC_TXBD_I;
	hdlc0.p_hdlc_txbd[0].pointer = 0xc00;
	hdlc0.p_hdlc_txbd[0].len_ctrl |= HDLC_TXBD_R;
	
	hdlc0.p_hdlc_regs->ccr |= HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
	
	do{
		val = hdlc0.p_hdlc_regs->isr;
	}while((val & (HDLC_ISR_RXBE | HDLC_ISR_RXFE)) != (HDLC_ISR_RXBE | HDLC_ISR_RXFE));
	
	//fail
}

void hdlc_init(struct hdlc_st *hdlc)
{
	//hdlc->p_hdlc_regs->ccr = 0;
#if ADDR_16BIT
	hdlc->addrm = 1;
#endif
#if CRC_32BIT
	hdlc->crc = 1;
#endif
#if RX_BUFFER_64BYTE
	hdlc->mrblr = 1;
#endif
	if(hdlc->mrblr)
		hdlc->p_hdlc_regs->frmr |= HDLC_FRMR_MRBLR;
	if(hdlc->addrm)
		hdlc->p_hdlc_regs->modr |= HDLC_MODR_ADDRM;
	if(hdlc->crc)
		hdlc->p_hdlc_regs->modr |= HDLC_MODR_CRC;
#if AUTO_RETRANSMIT_ENABLE
	hdlc->p_hdlc_regs->modr |= HDLC_MODR_RTE;
#endif
	hdlc->p_hdlc_regs->isr = HDLC_ISR_ALL;
	//hdlc->p_hdlc_regs->ccr |= HDLC_CCR_ETCS | HDLC_CCR_ERCS;
	
	init_tx_bds(hdlc);
	init_rx_bds(hdlc);
	hdlc->p_hdlc_regs->ccr |= HDLC_CCR_OPCODER_INIT | HDLC_CCR_NCMDR;
	hdlc->p_hdlc_regs->ccr |= HDLC_CCR_OPCODER_START | HDLC_CCR_NCMDR;
	hdlc->p_hdlc_regs->ccr &= (~HDLC_CCR_NCMDR);	
}

/*
 * hdlc6 tx rx interconnect
 */
void hdlc_transecive_test()
{
	struct hdlc_st hdlc6;
	unsigned int i, val, len;
	
	malloc_hdlc(&hdlc6, QUICC_HDLC_CHN);
	
	hdlc_reset(hdlc6.p_hdlc_regs);
	
	hdlc_init(&hdlc6);
	
	for(i=0; i<0x400; i+=4)
	{
		w32(QUICC_HDLC_TX_BUFFER(QUICC_HDLC_CHN)+i, 0xa5a5a5a5+i);
	}
	
	hdlc6.p_hdlc_txbd[0].len_ctrl = (0x8<<16) | HDLC_TXBD_W | HDLC_TXBD_L | HDLC_TXBD_I;
	hdlc6.p_hdlc_txbd[0].pointer = 0xc00;
	hdlc6.p_hdlc_txbd[0].len_ctrl |= HDLC_TXBD_R;
	
	hdlc6.p_hdlc_regs->ccr |= HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
	
	do{
		val = hdlc6.p_hdlc_regs->isr;
	}while((val & (HDLC_ISR_RXBE | HDLC_ISR_RXFE)) != (HDLC_ISR_RXBE | HDLC_ISR_RXFE));
	
	len = hdlc6.p_hdlc_rxbd[0].len_ctrl >> 16;
	for(i=0; i<(len-(hdlc6.crc ? 4 : 2)); i+=4)
	//for(i=0; i<(len-4); i+=4)
	{
		val = r32(QUICC_HDLC_RX_BUFFER(QUICC_HDLC_CHN)+(hdlc6.p_hdlc_rxbd[0].pointer&0x3ff)+i);
		if(val != 0xa5a5a5a5+i)
			while(1);
	}
}

/*
 * 0 < num <= 4
 */
void hdlc_rx_set_filter(struct hdlc_st *hdlc, unsigned short *addr, unsigned int num, unsigned int mask)
{
	unsigned int i, tmp, tmp1;
	
	for(i=0; i<num; i++)
	{
		if(i%2 == 0)
			tmp = addr[i];
		else
		{
			tmp1 = addr[i];
			tmp |= (tmp1<<16);
			if(i != 3)
				hdlc->p_hdlc_regs->haddr12 = tmp;
			else
				hdlc->p_hdlc_regs->haddr34 = tmp;
		}
	}
	if(i%2)
	{
		if(i != 3)
			hdlc->p_hdlc_regs->haddr12 = tmp;
		else
			hdlc->p_hdlc_regs->haddr34 = tmp;
	}
	
	hdlc->p_hdlc_regs->hmask = mask;
}

void hdlc_xmit_one(struct hdlc_st *hdlc, struct hdlc_frame_st *f)
{
	unsigned int i, tmp;
	
	for(i=0; i<f->len/4; i++)
	{
		hdlc->tx_buffer[i] = f->data[i];
	}

	if(f->len%4)
	{
		hdlc->tx_buffer[i] = f->data[i];
	}
	
	hdlc->p_hdlc_txbd[hdlc->cur_tx].len_ctrl = (f->len<<16) | HDLC_TXBD_W | HDLC_TXBD_L | HDLC_TXBD_I;
	hdlc->p_hdlc_txbd[hdlc->cur_tx].pointer = (unsigned int)hdlc->tx_buffer & 0x3ff;
	hdlc->p_hdlc_txbd[hdlc->cur_tx].len_ctrl |= HDLC_TXBD_R;
	
	hdlc->p_hdlc_regs->ccr |= HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
	
	do{
		tmp = hdlc->p_hdlc_regs->isr;
	}while((tmp & HDLC_ISR_TXBE) != HDLC_ISR_TXBE);
	hdlc->p_hdlc_regs->isr = (hdlc->p_hdlc_regs->isr & 0xfffffe00) | HDLC_ISR_CLEAR_TX;
}

void hdlc_xmit(struct hdlc_st *hdlc, struct hdlc_frame_st *f)
{
	unsigned int i, tmp;
	unsigned int b_size = 32;
	unsigned int bd_size = 0x400/b_size;
	unsigned int entry = hdlc->cur_tx % bd_size;
	struct hdlc_bd *first_bd = hdlc->p_hdlc_txbd + entry;
	
	//must use word access
	for(i=0; i<f->len/4; i++)
	{
		hdlc->tx_buffer[(entry*b_size/4+i)%0x100] = swap_word(f->data[i]);
	}

	if(f->len%4)
	{
		hdlc->tx_buffer[(entry*b_size/4+i)%0x100] = swap_word(f->data[i]);
	}
	
	//one bd buffer is 32 bytes at most
	for(i=0; i<f->len/b_size; i++)
	{
		if(i==0)
			hdlc->p_hdlc_txbd[entry].len_ctrl |= (b_size<<16);
		else
			hdlc->p_hdlc_txbd[entry].len_ctrl |= (b_size<<16) | HDLC_TXBD_R;
		hdlc->p_hdlc_txbd[entry].pointer = entry*b_size;
		
		hdlc->cur_tx++;
		entry = hdlc->cur_tx % bd_size;
	}
	if(f->len%b_size)
	{
		if(i==0)
			hdlc->p_hdlc_txbd[entry].len_ctrl |= ((f->len%b_size)<<16) | HDLC_TXBD_L;
		else
			hdlc->p_hdlc_txbd[entry].len_ctrl |= ((f->len%b_size)<<16) | HDLC_TXBD_R | HDLC_TXBD_L;
		hdlc->p_hdlc_txbd[entry].pointer = entry*b_size;
		
		hdlc->cur_tx++;		
	}
	else
	{
		entry = (hdlc->cur_tx-1) % bd_size;
		hdlc->p_hdlc_txbd[entry].len_ctrl |= HDLC_TXBD_L;
	}
	first_bd->len_ctrl |= HDLC_TXBD_R;
	
	while(hdlc->p_hdlc_regs->isr & HDLC_ISR_ERROR);
#if !AUTO_RETRANSMIT_ENABLE
	hdlc->p_hdlc_regs->ccr = (hdlc->p_hdlc_regs->ccr & HDLC_CCR_OPCODET_MASK) | HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
#endif
}

void hdlc_xmit_padding(struct hdlc_st *hdlc, unsigned int len)
{
	unsigned int i, tmp;
	unsigned int b_size = 32;
	unsigned int bd_size = 0x400/b_size;
	unsigned int entry = hdlc->cur_tx % bd_size;
	struct hdlc_bd *first_bd = hdlc->p_hdlc_txbd + entry;
	
	for(i=0; i<len/4; i++)
	{
		hdlc->tx_buffer[(entry*b_size/4+i)%0x100] = hdlc->index+i;
	}

	if(len%4)
	{
		hdlc->tx_buffer[(entry*b_size/4+i)%0x100] = hdlc->index+i;
	}
	
	for(i=0; i<len/b_size; i++)
	{
		if(i==0)
			hdlc->p_hdlc_txbd[entry].len_ctrl |= (b_size<<16);
		else
			hdlc->p_hdlc_txbd[entry].len_ctrl |= (b_size<<16) | HDLC_TXBD_R;
		hdlc->p_hdlc_txbd[entry].pointer = entry*b_size;
		
		hdlc->cur_tx++;
		entry = hdlc->cur_tx % bd_size;
	}
	if(len%b_size)
	{
		if(i==0)
			hdlc->p_hdlc_txbd[entry].len_ctrl |= ((len%b_size)<<16) | HDLC_TXBD_L;
		else
			hdlc->p_hdlc_txbd[entry].len_ctrl |= ((len%b_size)<<16) | HDLC_TXBD_R | HDLC_TXBD_L;
		hdlc->p_hdlc_txbd[entry].pointer = entry*b_size;
		
		hdlc->cur_tx++;		
	}
	else
	{
		entry = (hdlc->cur_tx-1) % bd_size;
		hdlc->p_hdlc_txbd[entry].len_ctrl |= HDLC_TXBD_L;
	}
	first_bd->len_ctrl |= HDLC_TXBD_R;
	
	hdlc->p_hdlc_regs->ccr = (hdlc->p_hdlc_regs->ccr & HDLC_CCR_OPCODET_MASK) | HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
}

/*
 * recycle tx bds
 */
void hdlc_tx_clean(struct hdlc_st *hdlc)
{
	unsigned int i, tmp;
	unsigned int b_size = 32;
	unsigned int bd_size = 0x400/b_size;
	
	while(hdlc->dirty_tx < hdlc->cur_tx)
	{
		unsigned int entry = hdlc->dirty_tx % bd_size;
		
		if(hdlc->p_hdlc_txbd[entry].len_ctrl & HDLC_TXBD_R)
			break;
		
		hdlc->p_hdlc_txbd[entry].len_ctrl = HDLC_TXBD_I;
		if(entry == bd_size-1)
		{
			hdlc->p_hdlc_txbd[entry].len_ctrl |= HDLC_TXBD_W;
		}
		
		hdlc->dirty_tx++;
	}
}

unsigned int hdlc_rx(struct hdlc_st *hdlc, unsigned int limit)
{
	unsigned int b_size = hdlc->mrblr ? 64: 32;
	unsigned int bd_size = 0x400/b_size;
	unsigned int count = 0;
	
	while(count < limit)
	{
		unsigned int entry = hdlc->cur_rx % bd_size;
		
		if(hdlc->p_hdlc_rxbd[entry].len_ctrl & HDLC_RXBD_E)
			break;
		if(hdlc->p_hdlc_rxbd[entry].len_ctrl & HDLC_RXBD_L)
			count++;
		
		hdlc->cur_rx++;
	}
	
	return count;
}

/*
 * recycle rx bds
 */
void hdlc_rx_refill(struct hdlc_st *hdlc)
{
	unsigned int b_size = hdlc->mrblr ? 64: 32;
	unsigned int bd_size = 0x400/b_size;
	
	while(hdlc->dirty_rx < hdlc->cur_rx)
	{
		unsigned int entry = hdlc->dirty_rx % bd_size;
		
		hdlc->p_hdlc_rxbd[entry].len_ctrl = HDLC_RXBD_E | HDLC_RXBD_I;
		if(entry == bd_size - 1)
			hdlc->p_hdlc_rxbd[entry].len_ctrl |= HDLC_RXBD_W;
		
		hdlc->dirty_rx++;
	}
}

void hdlc_rx_one(struct hdlc_st *hdlc, struct hdlc_frame_st *rf)
{
	unsigned int b_size = hdlc->mrblr ? 64: 32;
	unsigned int bd_size = 0x400/b_size;
	unsigned int j, tmp, entry, off, f_entry;
	
	do{
		tmp = hdlc->p_hdlc_regs->isr;
	}while((tmp & HDLC_ISR_RXFE) != HDLC_ISR_RXFE);
	hdlc->p_hdlc_regs->isr = (hdlc->p_hdlc_regs->isr & 0xfffffe00) | HDLC_ISR_CLEAR_RX;
	
	f_entry = hdlc->cur_rx%bd_size;
	tmp = hdlc_rx(hdlc, 1);
	if(tmp == 0)
		while(1);
	//last bd contains the length of frame and crc
	entry = (hdlc->cur_rx-1)%bd_size;
	rf->len = hdlc->p_hdlc_rxbd[entry].len_ctrl >> 16;
#if CRC_32BIT
	rf->len -= 4;
#else
	rf->len -= 2;
#endif
	off = hdlc->p_hdlc_rxbd[f_entry].pointer & 0x3ff;
	for(j=0; j<rf->len/4; j++)
		rf->data[j] = swap_word(hdlc->rx_buffer[(off/4+j)%0x100]);
	if(rf->len%4)
		rf->data[j] = swap_word(hdlc->rx_buffer[(off/4+j)%0x100]);

	hdlc_rx_refill(hdlc);
}

void hdlc_rx_compare_padding(struct hdlc_st *hdlc, unsigned int len)
{
	unsigned int b_size = hdlc->mrblr ? 64: 32;
	unsigned int bd_size = 0x400/b_size;
	unsigned int j, tmp, entry, off, f_entry;
	
	do{
		tmp = hdlc->p_hdlc_regs->isr;
	}while((tmp & HDLC_ISR_RXFE) != HDLC_ISR_RXFE);
	hdlc->p_hdlc_regs->isr = (hdlc->p_hdlc_regs->isr & 0xfffffe00) | HDLC_ISR_CLEAR_RX;
	
	f_entry = hdlc->cur_rx%bd_size;
	tmp = hdlc_rx(hdlc, 1);
	if(tmp == 0)
		while(1);
	entry = (hdlc->cur_rx-1)%bd_size;
	tmp = hdlc->p_hdlc_rxbd[entry].len_ctrl >> 16;
#if CRC_32BIT
	tmp -= 4;
#else
	tmp -= 2;
#endif

	if(tmp != len)
		while(1);
	
	off = hdlc->p_hdlc_rxbd[f_entry].pointer & 0x3ff;
	for(j=0; j<len/4; j++)
	{
		if(hdlc->index+j != hdlc->rx_buffer[(off/4+j)%0x100])
			while(1);
	}
	if(len%4)
	{
		tmp = (4 - len%4)*8;
		if(((hdlc->index+j)<<tmp) != (hdlc->rx_buffer[(off/4+j)%0x100] << tmp))
			while(1);
	}

	hdlc_rx_refill(hdlc);
}

unsigned short addr[4] = {
		0x1111, 0x2222, 0x3333, 0x4444
};

void hdlc_rx_filter_test()
{
	struct hdlc_st hdlc6;
	struct hdlc_frame_st tf, rf;
	unsigned int i, val, len;

	unsigned int f1[2], f2[2];
	
	malloc_hdlc(&hdlc6, 6);
	
	hdlc_reset(hdlc6.p_hdlc_regs);
	
	hdlc_rx_set_filter(&hdlc6, addr, 4, 0xffffffff);
	hdlc_init(&hdlc6);
	
	//this frame should be received
	f1[0] = 0x11111111;
	f1[1] = 0x11111111;
	tf.data = f1;
	tf.len = 8;
	hdlc_xmit_one(&hdlc6, &tf);
	
	rf.data = f2;
	rf.len = 0;
	hdlc_rx_one(&hdlc6, &rf);

	if(tf.len != rf.len || memcmp(f1, f2, rf.len))
		while(1);
	
	//this frame should be received
	f1[0] = 0x22222222;
	tf.data = f1;
	tf.len = 8;
	hdlc_xmit_one(&hdlc6, &tf);
	
	rf.data = f2;
	rf.len = 0;
	hdlc_rx_one(&hdlc6, &rf);

	if(tf.len != rf.len || memcmp(f1, f2, rf.len))
		while(1);
	
	//this frame should be dropped
	f1[0] = 0x55555555;
	tf.data = f1;
	tf.len = 8;
	hdlc_xmit_one(&hdlc6, &tf);
	
	rf.data = f2;
	rf.len = 0;
	hdlc_rx_one(&hdlc6, &rf);

	if(tf.len != rf.len || memcmp(f1, f2, rf.len))
		while(1);
}

/*
 * hdlc4 tx is connected to hdlc6 rx
 * hdlc4 rx is connected to hdlc6 tx
 */
void hdlc_echo_test()
{
	struct hdlc_st hdlc4, hdlc6;
	struct hdlc_frame_st tf, rf1, rf2;
	unsigned int f1[10], f2[10], f3[10];
	
	malloc_hdlc(&hdlc4, 4);
	malloc_hdlc(&hdlc6, 6);
	
	hdlc_reset(hdlc4.p_hdlc_regs);
	hdlc_reset(hdlc6.p_hdlc_regs);
	
	hdlc_init(&hdlc4);
	hdlc6.p_hdlc_regs->modr = HDLC_MODR_DIAG_ECHO;
	hdlc_init(&hdlc6);
	
	memset(f1, 0xff, 40);
	tf.data = f1;
	tf.len = 40;
	hdlc_xmit_one(&hdlc4, &tf);

	rf2.data = f3;
	rf2.len = 0;
	hdlc_rx_one(&hdlc4, &rf2);

	if(tf.len != rf2.len || memcmp(f1, f3, rf2.len))
		while(1);
}

void hdlc_mass_transecive_test()
{
	//struct gpio_regs *gpio_m = 0xe9646000;
	unsigned int b_size;
	unsigned int bd_size;
	struct hdlc_st hdlc6;
	struct hdlc_frame_st tf, rf;
	unsigned char f1[1024];
	unsigned int f2[256];
	unsigned int i, j, tmp, entry, off, f_entry;
	
	//gpio_m->dir |= 0x80;
	//gpio_m->ctl |= 0x80;
	//gpio_m->out = 0x80;
	
	malloc_hdlc(&hdlc6, QUICC_HDLC_CHN);
	
	hdlc_reset(hdlc6.p_hdlc_regs);
	
	hdlc_init(&hdlc6);
	b_size = hdlc6.mrblr ? 64: 32;
	bd_size = 0x400/b_size;
	
	for(i=(hdlc6.addrm ? 2 : 1) + 1; i<=1024 - (hdlc6.crc ? 4 : 2); i++)
	{
		//gpio_m->out = 0x80;
		//delay(0x100000);
		//sgpio_m->out = 0x0;
		//delay(0x100000);
		
		for(j=0; j<i; j++)
			f1[j] = j;
		
		tf.len = i;
		tf.data = f1;
		
		hdlc_xmit(&hdlc6, &tf);
		uart_printf("\nHDLC Send Data! cnt : %d\n",i);
#if 1		
		do{
			tmp = hdlc6.p_hdlc_regs->isr;
		}while((tmp & HDLC_ISR_TXFE) != HDLC_ISR_TXFE);
		hdlc6.p_hdlc_regs->isr = (hdlc6.p_hdlc_regs->isr & 0xfffffe00) | HDLC_ISR_CLEAR_TX;
#endif	
		hdlc_tx_clean(&hdlc6);
		
		uart_printf_buf("\nHDLC Recv Data!\n");
		do{
			tmp = hdlc6.p_hdlc_regs->isr;
		}while((tmp & HDLC_ISR_RXFE) != HDLC_ISR_RXFE);
		hdlc6.p_hdlc_regs->isr = (hdlc6.p_hdlc_regs->isr & 0xfffffe00) | HDLC_ISR_CLEAR_RX;
		
		f_entry = hdlc6.cur_rx%bd_size;
		tmp = hdlc_rx(&hdlc6, 1);
		if(tmp == 0)
			while(1);
		entry = (hdlc6.cur_rx-1)%bd_size;
		rf.len = hdlc6.p_hdlc_rxbd[entry].len_ctrl >> 16;
#if CRC_32BIT
		rf.len -= 4;
#else
		rf.len -= 2;
#endif
		off = hdlc6.p_hdlc_rxbd[f_entry].pointer & 0x3ff;
		for(j=0; j<rf.len/4; j++)
			f2[j] = swap_word(hdlc6.rx_buffer[(off/4+j)%0x100]);
		if(rf.len%4)
			f2[j] = swap_word(hdlc6.rx_buffer[(off/4+j)%0x100]);
		
		if(tf.len != rf.len || memcmp(f1, f2, tf.len))
		{
			uart_printf_buf("\nHDLC HDLC Check Data error!\n");
			while(1);
		}else{
			uart_printf_buf("\nHDLC HDLC Check Data OK!\n");
		}
		hdlc_rx_refill(&hdlc6);
	}
}

/*
 * configuration for hdlc bus mode
 */
void qe_hdlc_bus_init()
{
	unsigned int i, val;
/*	
	w32(QUICC_QER_BASE+0x28, 1);
	w32(QUICC_QER_BASE+0x2c, 40);
	w32(QUICC_QER_BASE+0x30, 1);
	w32(QUICC_QER_BASE+0x34, 40);
*/
	w32(QUICC_QER_BASE+0x50, 0xd8000000 | 0x007e6c00);
	w32(QUICC_QER_BASE+0x54, 0x1fe6c000 | 0x11fc0 | 0xc0000000);
	w32(QUICC_QER_BASE+0x58, 0xdbf00000 | 0x1ffb0);
	w32(QUICC_QER_BASE+0x5c, 0xc00000 | 0x3fe780);
	w32(QUICC_QER_BASE+0x60, 0x3cf7f780);
	w32(QUICC_QER_BASE+0x64, 0x3cf7f780);
/*
	w32(QUICC_QER_BASE+0x8, 0xaaaaaaaa);//all tx clocks should output first
	do{
		val = r32(QUICC_QER_BASE+0x8);
	}while(val != 0xaaaaaaaa);
	for(i=0; i<1000; i++);
*/
	//set hdlc7 tx clock to output only,the rest is input
	w32(QUICC_QER_BASE+0x8, 0x88888889);
	w32(QUICC_QER_BASE+0xc, 0x88888888);
	w32(QUICC_QER_BASE+0x10, 0x88888888);
	w32(QUICC_QER_BASE+0x14, 0);	
	w32(QUICC_QER_BASE+0x1c, 0x100);
}

void hdlc_handle_tx(struct hdlc_st *hdlc)
{
	unsigned int val;

wait:
	do{
		val = hdlc->p_hdlc_regs->isr;
#if AUTO_RETRANSMIT_ENABLE
	}while((val & (HDLC_ISR_TXFE | HDLC_ISR_TXEE | HDLC_ISR_CHCD)) == 0);	
#else
	}while((val & (HDLC_ISR_TXFE | HDLC_ISR_TXEE)) == 0);
#endif
	
	if(val & HDLC_ISR_TXFE)
		hdlc_tx_clean(hdlc);
#if AUTO_RETRANSMIT_ENABLE
	else if(val & HDLC_ISR_CHCD)
	{
		goto wait;
	}
#endif
	else//maybe collide
	{
#if !AUTO_RETRANSMIT_ENABLE
		hdlc->p_hdlc_regs->ccr = (hdlc->p_hdlc_regs->ccr & HDLC_CCR_OPCODET_MASK) | HDLC_CCR_OPCODER_STOP | HDLC_CCR_NCMDT;
		delay(100);
		hdlc->p_hdlc_regs->ccr = (hdlc->p_hdlc_regs->ccr & HDLC_CCR_OPCODET_MASK) | HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
#endif
		goto wait;
	}
	hdlc->p_hdlc_regs->isr = (hdlc->p_hdlc_regs->isr & 0xfffffe00) | HDLC_ISR_CLEAR_TX;
}

/*
 * hdlc5\6\7 interconnect
 */
void hdlc_bus_transecive_test()
{
	struct hdlc_st hdlc0, hdlc1, hdlc2;
	unsigned short addr0[4], addr1[4], addr2[4];
	unsigned int i;
	struct hdlc_frame_st tf0, tf1, tf2, rf;
	unsigned char f0[1024], f1[1024], f2[1024];
	unsigned int f3[256];
	
	qe_hdlc_bus_init();
	
	malloc_hdlc(&hdlc0, 0);
	malloc_hdlc(&hdlc1, 1);
	//malloc_hdlc(&hdlc2, 2);
	
	hdlc_reset(hdlc0.p_hdlc_regs);
	hdlc_reset(hdlc1.p_hdlc_regs);
	//hdlc_reset(hdlc2.p_hdlc_regs);
	
	for(i=0; i<4; i++)
	{
		addr0[i] = 0+i;
		addr1[i] = 0x1111+i;
		addr2[i] = 0x2222+i;
	}
	hdlc_rx_set_filter(&hdlc0, addr2, 4, 0xffffffff);
	hdlc_rx_set_filter(&hdlc1, addr1, 4, 0xffffffff);
	//hdlc_rx_set_filter(&hdlc2, addr2, 4, 0xffffffff);
	hdlc0.p_hdlc_regs->modr |= HDLC_MODR_BUS;//| HDLC_MODR_TOD;
	hdlc1.p_hdlc_regs->modr |= HDLC_MODR_BUS;//| HDLC_MODR_TOD;
	//hdlc2.p_hdlc_regs->modr |= HDLC_MODR_BUS;//| HDLC_MODR_TOD;
	hdlc0.p_hdlc_regs->modr = (hdlc0.p_hdlc_regs->modr & HDLC_MODR_CW_MASK) | HDLC_MODR_CW(1);
	hdlc1.p_hdlc_regs->modr = (hdlc1.p_hdlc_regs->modr & HDLC_MODR_CW_MASK) | HDLC_MODR_CW(1);
	//hdlc2.p_hdlc_regs->modr = (hdlc2.p_hdlc_regs->modr & HDLC_MODR_CW_MASK) | HDLC_MODR_CW(1);
	hdlc_init(&hdlc0);
	hdlc_init(&hdlc1);
	//hdlc_init(&hdlc2);
	
	for(i=0; i<2; i++)
	{
		f0[i] = 0x11;
		f1[i] = 0x22;
		f2[i] = 0;		
	}
	
	for(i=2; i<1024; i++)
	{
		f0[i] = i;
		f1[i] = 1+i;
		f2[i] = 2+i;
	}
	tf0.data = f0;
	tf1.data = f1;
	tf2.data = f2;
	rf.data = f3;
	for(i=(hdlc0.addrm ? 2 : 1) + 1; i<=1024 - (hdlc0.crc ? 4 : 2); i++)
	{
		//i=43;
		tf0.len = tf1.len = tf2.len = i;
		hdlc_xmit(&hdlc0, &tf0);
		hdlc_xmit(&hdlc1, &tf1);
		//hdlc_xmit(&hdlc2, &tf2);
#if AUTO_RETRANSMIT_ENABLE
		hdlc0.p_hdlc_regs->ccr = (hdlc0.p_hdlc_regs->ccr & HDLC_CCR_OPCODET_MASK) | HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
		hdlc1.p_hdlc_regs->ccr = (hdlc1.p_hdlc_regs->ccr & HDLC_CCR_OPCODET_MASK) | HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
		hdlc_handle_tx(&hdlc1);
		hdlc_handle_tx(&hdlc0);
#else
		hdlc_handle_tx(&hdlc0);
		hdlc_handle_tx(&hdlc1);
		//hdlc_handle_tx(&hdlc2);
#endif
		
		hdlc_rx_one(&hdlc0, &rf);
		if(rf.len != i || memcmp(rf.data, tf1.data, rf.len))
			while(1);
		
		hdlc_rx_one(&hdlc1, &rf);
		if(rf.len != i || memcmp(rf.data, tf0.data, rf.len))
			while(1);
/*		
		hdlc_rx_one(&hdlc2, &rf);
		if(rf.len != i || memcmp(rf.data, tf1.data, rf.len))
			while(1);
*/
	}
}

void hdlc_test()
{
	qe_ucc_init();
#if HDLC_SIMPLE_TRANSFER_TEST
	hdlc_simple_transfer_test();
#endif
#if HDLC_LOOPBACK_TEST
	hdlc_loopback_test();//fail
#endif
#if HDLC_TRANSECIVE_TEST
	hdlc_transecive_test();
#endif
#if HDLC_RX_FILTER_TEST
	hdlc_rx_filter_test();
#endif
#if HDLC_ECHO_TEST
	hdlc_echo_test();
#endif
#if HDLC_MASS_TRANSECIVE_TEST
	hdlc_mass_transecive_test();
#endif
}

void gpio_output_test()
{
	struct gpio_regs *gpio_a = QUICC_GPIO_BASE(0);
	unsigned int i, j;
#if 0	
	gpio_a->dir = 0xffffffff;
	gpio_a->ctl = 0xffffffff;
	
	while(1){
		gpio_a->out = 0xffffffff;
		delay(0x100000);
		gpio_a->out = 0;
		delay(0x100000);		
	}
#else
	for(i=2; i<6; i++)
	{
		gpio_a = QUICC_GPIO_BASE(i);
		gpio_a->dir = 0xffffffff;
		gpio_a->ctl = 0xffffffff;
		
		for(j=0; j<32; j++)
		{
			gpio_a->out = 1<<j;
		}
	}
#endif
}

void gpio_input_test()
{
	struct gpio_regs *gpio_a = QUICC_GPIO_BASE(1);
	unsigned int val;
	
	gpio_a->dir = 0;
	gpio_a->ibe = 0xffffffff;
	gpio_a->ctl = 0xffffffff;
	
	while(1)
		val = gpio_a->in;
}

//gpio_a and b interconnect, a:output, b:input
void gpio_interconnect_test()
{
	struct gpio_regs *gpio_a = QUICC_GPIO_BASE(0);
	struct gpio_regs *gpio_b = QUICC_GPIO_BASE(1);
	
	gpio_a->dir |= 0x3;
	gpio_a->ctl |= 0x3;
	
	gpio_b->dir &= 0xffffffcf;
	gpio_b->ibe |= 0x30;
	gpio_b->ctl |= 0x30;
	
	gpio_a->out |= 0x3;
	if((gpio_b->in & 0x30) != 0x30)
		while(1);
	
	gpio_a->out &= 0xfffffffc;
	if((gpio_b->in & 0x30) != 0)
		while(1);
}

void gpio_test()
{
#if GPIO_OUTPUT_TEST
	gpio_output_test();
#endif
#if GPIO_INPUT_TEST
	gpio_input_test();
#endif
#if GPIO_INTERCONNECT_TEST
	gpio_interconnect_test();
#endif
}

void uart_transceive_test()
{
	unsigned char i, val;
	
	for(i=0; i<0xff; i++)
	{
		uart0_16750_TX(i);
		val = uart0_16750_RX();
		if(val != i)
			while(1);
	}
}

void uart_test()
{
	w32(QUICC_QER_BASE+0x58, 0xf);
	uart0_16750_init(5000000, 9600);
#if 0
	uart_printf_buf("01234567\n");
#endif
	uart_transceive_test();
}

void cpu0_launch_test()
{
	unsigned int i;
	
	for(i=0; i<cpu0_bin_total_bytes; i++)
	{
		w8(QUICC_CPU0_IRAM_BASE+i, cpu0_bin_data[i]);
	}
	
	CCM_C0_EN |= (1<<0);
}

void cpu1_launch_test()
{
	unsigned int i;
	
	for(i=0; i<cpu1_bin_total_bytes; i++)
	{
		w8(QUICC_CPU1_IRAM_BASE+i, cpu1_bin_data[i]);
	}
	
	CCM_C0_EN |= (1<<1);
}

volatile unsigned int quicc_int_flag[3] = {0, 0, 0};
void quicc_isr0()
{
	quicc_int_flag[0] = r32(QUICC_QER_BASE+0xc4);
	
	w32(QUICC_QER_BASE+0xa8, 0);

}

void quicc_isr1()
{
	quicc_int_flag[1] = r32(QUICC_QER_BASE+0xcc);
	
	w32(QUICC_QER_BASE+0xb0, 0);

}

void quicc_isr2()
{
	quicc_int_flag[2] = r32(QUICC_QER_BASE+0xd8);
	
	w32(QUICC_QER_BASE+0xbc, 0);

}

void quicc_multicore_sync_test()
{
	unsigned int i;
	
	InstallInterruptHandler0(QUICC_INT_NUM0, quicc_isr0);
	InstallInterruptHandler0(QUICC_INT_NUM1, quicc_isr1);
	InstallInterruptHandler0(QUICC_INT_NUM2, quicc_isr2);
	*(volatile unsigned int *)(0xe8050200+QUICC_INT_NUM0*0x20) &= 0x7fffffff;//enable mpic vector
	*(volatile unsigned int *)(0xe8050200+QUICC_INT_NUM1*0x20) &= 0x7fffffff;//enable mpic vector
	*(volatile unsigned int *)(0xe8050200+QUICC_INT_NUM2*0x20) &= 0x7fffffff;//enable mpic vector
	
	//interrupt self
	for(i=0; i<32; i++)
	{
		w32(QUICC_QER_BASE+0xd8, 1<<i);
		w32(QUICC_QER_BASE+0xbc, 1<<i);
		while(quicc_int_flag[2] == 0);
		if(quicc_int_flag[2] != (1<<i))
			while(1);
		quicc_int_flag[2] = 0;
	}
	
	//interrupt risc0,then wait risc0's interrupt
	for(i=0; i<32; i++)
	{
		w32(QUICC_QER_BASE+0xd0, 0x5a5a5a5a);
		w32(QUICC_QER_BASE+0xb4, 1<<i);
		while(quicc_int_flag[0] == 0);
		if(quicc_int_flag[0] != (1<<i)+0x5a5a5a5a)
			while(1);
		quicc_int_flag[0] = 0;
	}
	
	//interrupt risc1,then wait risc1's interrupt
	for(i=0; i<32; i++)
	{
		w32(QUICC_QER_BASE+0xd4, 0xa5a5a5a5);
		w32(QUICC_QER_BASE+0xb8, 1<<i);
		while(quicc_int_flag[1] == 0);
		if(quicc_int_flag[1] != (1<<i)+0xa5a5a5a5)
			while(1);
		quicc_int_flag[1] = 0;
	}
}

void quicc_c0_test()
{
#if 1
	cpu0_launch_test();
#endif
#if 1
	cpu1_launch_test();
#endif
	quicc_multicore_sync_test();
}

void qe_tdm_init()
{
	w32(QUICC_QER_BASE+0x68, 0x38e);
	w32(QUICC_QER_BASE+0x50, ~0x38e);
	w32(QUICC_QER_BASE+0x74, 0x3e40);
	w32(QUICC_QER_BASE+0x5c, ~0x3e40);
}

void init_tdm(unsigned int index)
{
	unsigned int val;
	
	w32(QUICC_TDM_BASE(index)+0x800, TDM_RXCLK_SEL_EXTERN | TDM_SYN_PSEL_RX(1) | TDM_SYN_PSEL_TX(1) | TDM_EN_FIFO_ALL);
	w32(QUICC_TDM_BASE(index)+0, TDM_BD_LST | TDM_BD_CNT(0) | TDM_BD_MCSEL(0) | TDM_BD_FFC(0));
	w32(QUICC_TDM_BASE(index)+0x400, TDM_BD_LST | TDM_BD_CNT(0) | TDM_BD_MCSEL(0) | TDM_BD_FFC(0));
	w32(QUICC_TDM_BASE(index)+0x800, TDM_RXCLK_SEL_EXTERN | TDM_SYN_PSEL_RX(1) | TDM_SYN_PSEL_TX(1) | TDM_EN_FIFO_ALL | TDM_FLAG);
}

void tdm_4hdlc_equal_init(unsigned int index)
{
	unsigned int i;
	
	w32(QUICC_TDM_BASE(index)+0x800, TDM_RXCLK_SEL_EXTERN | TDM_SYN_PSEL_RX(1) | TDM_SYN_PSEL_TX(1) | TDM_EN_FIFO_ALL);
	for(i=0; i<8; i+=2)
	{
		w32(QUICC_TDM_BASE(index)+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(0) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(0))<<16));
		w32(QUICC_TDM_BASE(index)+0x400+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(0) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(0))<<16));
	}
	for(i=8; i<16; i+=2)
	{
		w32(QUICC_TDM_BASE(index)+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(1) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(1))<<16));
		w32(QUICC_TDM_BASE(index)+0x400+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(1) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(1))<<16));
	}
	for(i=16; i<24; i+=2)
	{
		w32(QUICC_TDM_BASE(index)+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(2) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(2))<<16));
		w32(QUICC_TDM_BASE(index)+0x400+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(2) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(2))<<16));
	}
	for(i=24; i<32; i+=2)
	{
		if(i==30)
		{
			w32(QUICC_TDM_BASE(index)+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(3) | ((TDM_BD_LST | TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(3))<<16));
			w32(QUICC_TDM_BASE(index)+0x400+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(3) | ((TDM_BD_LST | TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(3))<<16));
		}
		else
		{
			w32(QUICC_TDM_BASE(index)+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(3) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(3))<<16));
			w32(QUICC_TDM_BASE(index)+0x400+i*2, TDM_BD_CNT(7) | TDM_BD_MCSEL(i*8) | TDM_BD_FFC(3) | ((TDM_BD_CNT(7) | TDM_BD_MCSEL((i+1)*8) | TDM_BD_FFC(3))<<16));
		}
			
	}
	w32(QUICC_TDM_BASE(index)+0x800, TDM_RXCLK_SEL_EXTERN | TDM_SYN_PSEL_RX(1) | TDM_SYN_PSEL_TX(1) | TDM_EN_FIFO_ALL | TDM_FLAG);	
}

void tdm_simple_test()
{
	struct hdlc_st hdlc0;
	unsigned int i, val, len;
	
	qe_tdm_init();
	init_tdm(QUICC_TDM_CHN);
	
	malloc_hdlc(&hdlc0, QUICC_TDM_CHN);
	
	hdlc_reset(hdlc0.p_hdlc_regs);
	
	hdlc_init(&hdlc0);
	
	for(i=0; i<0x400; i+=4)
	{
		w32(QUICC_HDLC_TX_BUFFER(QUICC_TDM_CHN)+i, 0xa5a5a5a5+i);
	}
	
	hdlc0.p_hdlc_txbd[0].len_ctrl = (0x8<<16) | HDLC_TXBD_W | HDLC_TXBD_L | HDLC_TXBD_I;
	hdlc0.p_hdlc_txbd[0].pointer = 0xc00;
	hdlc0.p_hdlc_txbd[0].len_ctrl |= HDLC_TXBD_R;
	
	hdlc0.p_hdlc_regs->ccr |= HDLC_CCR_OPCODET_INIT | HDLC_CCR_NCMDT;
	
	do{
		val = hdlc0.p_hdlc_regs->isr;
	}while((val & (HDLC_ISR_RXBE | HDLC_ISR_RXFE)) != (HDLC_ISR_RXBE | HDLC_ISR_RXFE));
	
	len = hdlc0.p_hdlc_rxbd[0].len_ctrl >> 16;
	for(i=0; i<(len-(hdlc0.crc ? 4 : 2)); i+=4)
	{
		val = r32(QUICC_HDLC_RX_BUFFER(QUICC_TDM_CHN)+(hdlc0.p_hdlc_rxbd[0].pointer&0x3ff)+i);
		if(val != 0xa5a5a5a5+i)
			while(1);
	}
}

void tdm_equal_transecive_test()
{
	struct hdlc_st hdlc0, hdlc1, hdlc2, hdlc3;
	unsigned int i;

	qe_tdm_init();
	tdm_4hdlc_equal_init(0);
	
	malloc_hdlc(&hdlc0, 0);
	malloc_hdlc(&hdlc1, 1);
	malloc_hdlc(&hdlc2, 2);
	malloc_hdlc(&hdlc3, 3);
	
	hdlc_reset(hdlc0.p_hdlc_regs);
	hdlc_reset(hdlc1.p_hdlc_regs);
	hdlc_reset(hdlc2.p_hdlc_regs);
	hdlc_reset(hdlc3.p_hdlc_regs);
	
	hdlc_init(&hdlc0);
	hdlc_init(&hdlc1);
	hdlc_init(&hdlc2);
	hdlc_init(&hdlc3);
	
	for(i=(hdlc0.addrm ? 3 : 2); i<=1024 - (hdlc0.crc ? 4 : 2); i++)
	{
		hdlc_xmit_padding(&hdlc0, i);
		hdlc_xmit_padding(&hdlc1, i);
		hdlc_xmit_padding(&hdlc2, i);
		hdlc_xmit_padding(&hdlc3, i);
		
		hdlc_handle_tx(&hdlc0);
		hdlc_handle_tx(&hdlc1);
		hdlc_handle_tx(&hdlc2);
		hdlc_handle_tx(&hdlc3);
		
		hdlc_rx_compare_padding(&hdlc0, i);
		hdlc_rx_compare_padding(&hdlc1, i);
		hdlc_rx_compare_padding(&hdlc2, i);
		hdlc_rx_compare_padding(&hdlc3, i);
	}
}

void quicc_tdm_test()
{
#if TDM_SIMPLE_TEST
	tdm_simple_test();
#endif
#if TDM_EQUAL_TRANSECIVE_TEST
	tdm_equal_transecive_test();
#endif
}

void qe_dma_init()
{
/*
 * configure quicc memory translation
 * quicc address			e500 address
 * 0x10000000    <<==>>     0x0
 * 0x20000000	 <<==>>		0x10000000
 * 0x30000000	 <<==>>		0x20000000
 * 0x40000000	 <<==>>		0x30000000
 * 0x50000000	 <<==>>		0x40000000
 * 0x60000000	 <<==>>		0x50000000
 * 0x70000000	 <<==>>		0x60000000
 * 0x80000000	 <<==>>		0x4e0000000
 */
	w32(QUICC_QER_BASE+0xdc, 0);
	w32(QUICC_QER_BASE+0xe0, 0);
	w32(QUICC_QER_BASE+0xe4, 0);
	w32(QUICC_QER_BASE+0xe8, 0);
	w32(QUICC_QER_BASE+0xec, 0);
	w32(QUICC_QER_BASE+0xf0, 0);
	w32(QUICC_QER_BASE+0xf4, 0);
	w32(QUICC_QER_BASE+0xf8, 4);

	w32(QUICC_QER_BASE+0xfc, 0);
	w32(QUICC_QER_BASE+0x100, 1);
	w32(QUICC_QER_BASE+0x104, 2);
	w32(QUICC_QER_BASE+0x108, 3);
	w32(QUICC_QER_BASE+0x10c, 4);
	w32(QUICC_QER_BASE+0x110, 5);
	w32(QUICC_QER_BASE+0x114, 6);
	w32(QUICC_QER_BASE+0x118, 0xe);
}

void dma_single_transmit(struct dma_single_para_st *p)
{
	unsigned int val;
	
	do{
		val = r32(DMA_CHENRL);
	}while(val & CH_EN(p->cn));
	
	w32(DMA_SARL(p->cn), p->sarl);
	//w32(DMA_SARH(p->cn), p->sarh);
	w32(DMA_DARL(p->cn), p->darl);
	//w32(DMA_DARH(p->cn), p->darh);
	w32(DMA_CTLL(p->cn),(p->tt_fc << TT_FC_SHIFT) | (p->src_tr_width << SRC_TR_WIDTH_SHIFT) | (p->dst_tr_width << DST_TR_WIDTH_SHIFT) | INT_EN);
	w32(DMA_CTLH(p->cn), p->block_ts);
	w32(DMA_CFGRL, DMA_EN);
	w32(DMA_CHENRL, CH_EN_WE(1<<p->cn) | CH_EN(1<<p->cn));
	
	do{
		val = r32(DMA_RAWTFRL);
	}while((val & (1<<p->cn)) == 0);
	
	val = r32(DMA_RAWERRL);
	if(val)
		while(1);
	
	w32(DMA_CLRTFRL, 1<<p->cn);
	w32(DMA_CLRBLKL, 1<<p->cn);
	w32(DMA_CLRSTRANL, 1<<p->cn);
	w32(DMA_CLRDTRANL, 1<<p->cn);
	w32(DMA_CLRERRL, 1<<p->cn);	
}

void dma_linked_list_transmit(unsigned int cn, unsigned int lli_base)
{
	unsigned int val;
	
	do{
		val = r32(DMA_CHENRL);
	}while(val & CH_EN(cn));
	
	w32(DMA_CTLL(cn),(MEM_TO_MEM << TT_FC_SHIFT) | (WIDTH_32 << SRC_TR_WIDTH_SHIFT) | (WIDTH_32 << DST_TR_WIDTH_SHIFT) | LLP_SRC_EN | LLP_DST_EN | INT_EN);
	w32(DMA_LLPL(cn), lli_base);
	w32(DMA_CFGRL, DMA_EN);
	w32(DMA_CHENRL, CH_EN_WE(1<<cn) | CH_EN(1<<cn));
	
	do{
		val = r32(DMA_RAWTFRL);
	}while((val & (1<<cn)) == 0);
	
	val = r32(DMA_RAWERRL);
	if(val)
		while(1);
	
	w32(DMA_CLRTFRL, 1<<cn);
	w32(DMA_CLRBLKL, 1<<cn);
	w32(DMA_CLRSTRANL, 1<<cn);
	w32(DMA_CLRDTRANL, 1<<cn);
	w32(DMA_CLRERRL, 1<<cn);	
}

//void buf_init(unsigned int addr, unsigned int size, unsigned char data)
//{
//	unsigned int i;
//
//	for(i=0; i<size; i++)
//		w8(addr+i, data++);
//}

void dma_single_block_transmit_test()
{
	struct dma_single_para_st p;
	
	p.sarl = 0x80040000;
	p.darl = 0;//QUICC_SRAM_BASE;
	p.tt_fc = MEM_TO_MEM;
	for(p.cn=0; p.cn<8; p.cn++)
	{
		for(p.src_tr_width=2; p.src_tr_width<3; p.src_tr_width++)
		{
			for(p.dst_tr_width=2; p.dst_tr_width<3; p.dst_tr_width++)
			{
				for(p.block_ts=1; p.block_ts<(p.cn < 2 ? 256 : 32); p.block_ts++)
				{
					buf_init(0xe0040000, p.block_ts<<p.src_tr_width, p.block_ts);
					
					dma_single_transmit(&p);
					
					if(memcmp(0xe0040000, QUICC_SRAM_BASE, p.block_ts<<p.src_tr_width))
						while(1);
				}
			}
		}
	}
	
	p.sarl = 0x10000000;
	p.darl = 0x10000;//QUICC_SRAM_BASE;
	p.tt_fc = MEM_TO_MEM;
	for(p.cn=0; p.cn<8; p.cn++)
	{
		for(p.src_tr_width=2; p.src_tr_width<3; p.src_tr_width++)
		{
			for(p.dst_tr_width=2; p.dst_tr_width<3; p.dst_tr_width++)
			{
				for(p.block_ts=1; p.block_ts<(p.cn < 2 ? 256 : 32); p.block_ts++)
				{
					buf_init(0, p.block_ts<<p.src_tr_width, p.block_ts);
					
					dma_single_transmit(&p);
					
					if(memcmp(0, QUICC_SRAM_BASE+0x10000, p.block_ts<<p.src_tr_width))
						while(1);
				}
			}
		}
	}
	
	p.sarl = 0x10000;
	p.darl = 0x80060000;
	p.tt_fc = MEM_TO_MEM;
	for(p.cn=0; p.cn<8; p.cn++)
	{
		for(p.src_tr_width=2; p.src_tr_width<3; p.src_tr_width++)
		{
			for(p.dst_tr_width=2; p.dst_tr_width<3; p.dst_tr_width++)
			{
				for(p.block_ts=1; p.block_ts<(p.cn < 2 ? 256 : 32); p.block_ts++)
				{
					buf_init(QUICC_SRAM_BASE+0x10000, p.block_ts<<p.src_tr_width, p.block_ts);
					
					dma_single_transmit(&p);
					
					if(memcmp(0xe0060000, QUICC_SRAM_BASE+0x10000, p.block_ts<<p.src_tr_width))
						while(1);
				}
			}
		}
	}
	
	p.sarl = 0;
	p.darl = 0x20000000;
	p.tt_fc = MEM_TO_MEM;
	for(p.cn=0; p.cn<8; p.cn++)
	{
		for(p.src_tr_width=2; p.src_tr_width<3; p.src_tr_width++)
		{
			for(p.dst_tr_width=2; p.dst_tr_width<3; p.dst_tr_width++)
			{
				for(p.block_ts=1; p.block_ts<(p.cn < 2 ? 256 : 32); p.block_ts++)
				{
					buf_init(QUICC_SRAM_BASE, p.block_ts<<p.src_tr_width, p.block_ts);
					
					dma_single_transmit(&p);
					
					if(memcmp(0x10000000, QUICC_SRAM_BASE, p.block_ts<<p.src_tr_width))
						while(1);
				}
			}
		}
	}
}

void dma_linked_list_transmit_test()
{
	struct dma_lli_st *lli = QUICC_SRAM_BASE+0x10000;
	unsigned int i, j;
	
	for(i=0; i<0x20; i+=4)
	{
		w32(QUICC_SRAM_BASE+i, 0x5a5a5a5a+i);
	}
	
	for(i=0; i<1; i++)
	{
		lli->sar = 0+i*0x20;
		lli->dar = 0x10000000+i*0x20;
		lli->ctl_l = (MEM_TO_MEM << TT_FC_SHIFT) | (WIDTH_32 << SRC_TR_WIDTH_SHIFT) | (WIDTH_32 << DST_TR_WIDTH_SHIFT) | LLP_SRC_EN | LLP_DST_EN | INT_EN;
		lli->ctl_h = 8;
		lli->llp = ++lli;
	}
	lli--;
	lli->llp = 0;
	dma_linked_list_transmit(0, DMA_LLI_BASE);
	
	if(memcmp(QUICC_SRAM_BASE, 0, 32))
		while(1);
	
	//fail,do not support
}

void quicc_dma_test()
{
	qe_dma_init();
#if 0
	dma_single_block_transmit_test();
#endif
#if 1
	dma_linked_list_transmit_test();
#endif
}

void quicc_test()
{
#if 0
	(*(volatile unsigned int *)0xeb009000) &= 0x3bf;//axi datapath
#endif
#if QUICC_SRAM_TEST
	quicc_sram_test();
#endif
#if QUICC_HDLC_TEST || QUICC_HDLC_BUS_TEST
	(*(volatile unsigned int *)0xeb009028) |= (1 << 14) | (1 << 15);//hdlc enable
#endif
#if QUICC_HDLC_TEST
	uart_printf_buf("\n\n############ QUICC HDLC TEST BEGIN #############!\n");
	hdlc_test();
	uart_printf_buf("\n\n############ QUICC HDLC TEST END #############!\n");
#endif
#if QUICC_HDLC_BUS_TEST
	hdlc_bus_transecive_test();
#endif
#if QUICC_GPIO_TEST
	gpio_test();
#endif
#if QUICC_UART_TEST
#if UART_TEST_0
	uart_test();
#endif
#if UART_TEST_1
	qe_uart_test();
#endif
#endif
#if QUICC_C0_TEST
	(*(volatile unsigned int *)0xeb009028) |= (1 << 3) | (1 << 4);//c0 swd enable
	quicc_c0_test();
#endif
#if QUICC_TDM_TEST
	uart_printf_buf("\n\n############ QUICC TDM TEST BEGIN #############!\n");
	quicc_tdm_test();
	uart_printf_buf("\n\n############ QUICC TDM TEST END #############!\n");
#endif
#if QUICC_DMA_TEST
	quicc_dma_test();
#endif
//	while(1);
}
