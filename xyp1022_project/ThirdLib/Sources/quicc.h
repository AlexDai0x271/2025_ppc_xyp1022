/*
 * quicc.h
 *
 *  Created on: 2021-12-15
 *      Author: scgao
 */

#ifndef QUICC_H_
#define QUICC_H_

#define QUICC_HDLC_CHN			(0)
#define QUICC_TDM_CHN			(0)


#define QUICC_BASE				0xe9580000

#define QUICC_SRAM_BASE			(QUICC_BASE+0)
#define QUICC_SRAM_SIZE			0x20000

#define QUICC_HDLC_BASE(n)		(QUICC_BASE+0x20000+n*0x2000)
#define QUICC_HDLC_RXBD_BASE(n)	(QUICC_HDLC_BASE(n)+0)
#define QUICC_HDLC_TXBD_BASE(n)	(QUICC_HDLC_BASE(n)+0x400)
#define QUICC_HDLC_RX_BUFFER(n)	(QUICC_HDLC_BASE(n)+0x800)
#define QUICC_HDLC_TX_BUFFER(n)	(QUICC_HDLC_BASE(n)+0xc00)
#define QUICC_HDLC_REGS_BASE(n)	(QUICC_HDLC_BASE(n)+0x1000)

#define QUICC_TDM_BASE(n)		(QUICC_BASE+0x30000+n*0x1000)

#define QUICC_GPIO_BASE(n)		(QUICC_BASE+0x38000+0x800*n)

#define QUICC_DMA_BASE			(QUICC_BASE+0x3b000)
#define DMA_BLOCK_SIZE			64
#define DMA_LLI_BASE			0x10000
#define QUICC_HDLC_RX_BUFFER_DMA(n)	(0x20000+n*0x2000+0x800)
#define QUICC_HDLC_TX_BUFFER_DMA(n)	(0x20000+n*0x2000+0xc00)
#define E500_HDLC_RX_BUFFER_DMA(n)	(0x10000000+n*0x2000+0x800)
#define E500_HDLC_TX_BUFFER_DMA(n)	(0x10000000+n*0x2000+0xc00)

#define QUICC_UART_BASE(n)		(QUICC_BASE+0x3b400+0x100*n)

#define QUICC_QER_BASE			(QUICC_BASE+0x3c000)

#define QUICC_CPU0_IRAM_BASE	QUICC_SRAM_BASE
#define QUICC_CPU1_IRAM_BASE	(QUICC_SRAM_BASE+0x8000)
#define CCM_C0_EN				*(volatile unsigned int *)0xEB009014

#define QUICC_INT_NUM0			105
#define QUICC_INT_NUM1			106
#define QUICC_INT_NUM2			107

struct hdlc_bd{
	unsigned int len_ctrl;
	unsigned int pointer;
};

#define HDLC_RXBD_E							(1<<0)
#define HDLC_RXBD_W							(1<<2)
#define HDLC_RXBD_I							(1<<3)
#define HDLC_RXBD_L							(1<<4)
#define HDLC_RXBD_F							(1<<5)
#define HDLC_RXBD_LG						(1<<10)
#define HDLC_RXBD_NO						(1<<11)
#define HDLC_RXBD_AB						(1<<12)
#define HDLC_RXBD_CR						(1<<13)
#define HDLC_RXBD_OV						(1<<14)

#define HDLC_TXBD_R							(1<<0)
#define HDLC_TXBD_W							(1<<2)
#define HDLC_TXBD_I							(1<<3)
#define HDLC_TXBD_L							(1<<4)

struct hdlc_regs{
	unsigned int ccr;
	unsigned int modr;
	unsigned int frmr;
	unsigned int haddr12;
	unsigned int haddr34;
	unsigned int isr;
	unsigned int hmask;
	unsigned int reserved0;
	unsigned int para_ram[7];
};

#define HDLC_CCR_RST						(1<<0)
#define HDLC_CCR_NCMDT						(1<<1)
#define HDLC_CCR_OPCODET_MASK				0xfffffff3
#define HDLC_CCR_OPCODET_INIT				0
#define HDLC_CCR_OPCODET_STOP				(1<<2)
#define HDLC_CCR_OPCODET_GRACEFUL_STOP		(2<<2)
#define HDLC_CCR_OPCODET_RESTART			(3<<2)
#define HDLC_CCR_NCMDR						(1<<4)
#define HDLC_CCR_OPCODER_MASK				0xffffff9f
#define HDLC_CCR_OPCODER_INIT				0
#define HDLC_CCR_OPCODER_STOP				(1<<5)
#define HDLC_CCR_OPCODER_START				(2<<5)
#define HDLC_CCR_TCI						(1<<16)
#define HDLC_CCR_RCI						(1<<17)
#define HDLC_CCR_ETCS						(1<<18)
#define HDLC_CCR_ERCS						(1<<19)
#define HDLC_CCR_CLKDIV_MASK				(0xfffff)
#define HDLC_CCR_CLKDIV(n)					(n<<20)//div (n+2)*2

#define HDLC_MODR_DIAG_MASK					0xfffffff3
#define HDLC_MODR_DIAG_NORMAL				0
#define HDLC_MODR_DIAG_LOOPBACK				(1<<2)
#define HDLC_MODR_DIAG_ECHO					(2<<2)
#define HDLC_MODR_DIAG_LOOPBACK_ECHO		(3<<2)
#define HDLC_MODR_NOF_MASK					0xffff87ff
#define HDLC_MODR_NOF(n)					(n<<11)
#define HDLC_MODR_FSE						(1<<15)
#define HDLC_MODR_RTE						(1<<16)
#define HDLC_MODR_BUS						(1<<17)
#define HDLC_MODR_DRT						(1<<19)
#define HDLC_MODR_CRC						(1<<20)
#define HDLC_MODR_CW_MASK					0xff1fffff
#define HDLC_MODR_CW(n)						(n<<21)
#define HDLC_MODR_TOD						(1<<24)
#define HDLC_MODR_ADDRM						(1<<25)

#define HDLC_FRMR_MRBLR						(1<<0)
#define HDLC_FRMR_RFTHR_MASK				0xffffffe1
#define HDLC_FRMR_RFTHR(n)					(n<<1)
#define HDLC_FRMR_TFTHR_MASK				0xfffffe1f
#define HDLC_FRMR_TFTHR(n)					(n<<5)
#define HDLC_FRMR_TSPTR_MASK				0xffff81ff
#define HDLC_FRMR_TSPTR(n)					(n<<9)
#define HDLC_FRMR_MFLR_MASK					0xfc00ffff
#define HDLC_FRMR_MFLR(n)					(n<<16)

#define HDLC_ISR_RXFE						(1<<0)
#define HDLC_ISR_RXBE						(1<<1)
#define HDLC_ISR_FLGE						(1<<2)
#define HDLC_ISR_IDLE						(1<<3)
#define HDLC_ISR_BUFFOVE					(1<<4)
#define HDLC_ISR_TXEE						(1<<5)
#define HDLC_ISR_TXBE						(1<<6)
#define HDLC_ISR_TXFE						(1<<7)
#define HDLC_ISR_GRAE						(1<<8)
#define HDLC_ISR_RINTR						(1<<10)
#define HDLC_ISR_TINTR						(1<<11)
#define HDLC_ISR_FG							(1<<12)
#define HDLC_ISR_ID							(1<<13)
#define HDLC_ISR_CHBSY						(1<<14)
#define HDLC_ISR_CHCD						(1<<15)
#define HDLC_ISR_RXFM						(1<<16)
#define HDLC_ISR_RXBM						(1<<17)
#define HDLC_ISR_FLGM						(1<<18)
#define HDLC_ISR_IDLM						(1<<19)
#define HDLC_ISR_BUFFOVM					(1<<20)
#define HDLC_ISR_TXEM						(1<<21)
#define HDLC_ISR_TXBM						(1<<22)
#define HDLC_ISR_TXFM						(1<<23)
#define HDLC_ISR_GRAM						(1<<24)
#define HDLC_ISR_UDRUN						(1<<29)
#define HDLC_ISR_SHOT						(1<<30)
#define HDLC_ISR_BSYOVT						(1<<31)
#define HDLC_ISR_CLEAR_TX					(HDLC_ISR_TXEE | HDLC_ISR_TXBE | HDLC_ISR_TXFE | HDLC_ISR_GRAE)
#define HDLC_ISR_CLEAR_RX					(HDLC_ISR_RXFE | HDLC_ISR_RXBE | HDLC_ISR_FLGE | HDLC_ISR_IDLE | HDLC_ISR_BUFFOVE)
#define HDLC_ISR_ALL						(HDLC_ISR_RXFM | HDLC_ISR_RXBM | HDLC_ISR_FLGM | HDLC_ISR_IDLM | HDLC_ISR_BUFFOVM | HDLC_ISR_TXEM | HDLC_ISR_TXBM | HDLC_ISR_TXFM | HDLC_ISR_GRAM)
#define HDLC_ISR_ERROR						(HDLC_ISR_BSYOVT | HDLC_ISR_SHOT | HDLC_ISR_UDRUN | HDLC_ISR_CHCD | HDLC_ISR_CHBSY)

struct hdlc_st{
	unsigned int index;
	struct hdlc_regs *p_hdlc_regs;
	struct hdlc_bd *p_hdlc_rxbd;
	struct hdlc_bd *p_hdlc_txbd;
	unsigned int *tx_buffer;
	unsigned int *rx_buffer;
	unsigned int cur_tx;
	unsigned int dirty_tx;
	unsigned int cur_rx;
	unsigned int dirty_rx;
	unsigned int mrblr;
	unsigned int addrm;
	unsigned int crc;
};

struct hdlc_frame_st{
	unsigned int *data;
	unsigned int len;
};

struct hdlc_sg_frame_st{
	struct hdlc_frame_st f;
	struct hdlc_sg_frame_st *next;
};

#define TDM_FLAG							(1<<0)
#define TDM_EN_FIFO_NONE					(0<<1)
#define TDM_EN_FIFO(n)						(n<<1)
#define TDM_EN_FIFO_ALL						(0xf<<1)
#define TDM_ECHO							(1<<5)
#define TDM_LOOPBACK						(1<<6)
#define TDM_RFSD(n)							(n<<7)
#define TDM_RXCLK_INV						(1<<9)
#define TDM_SYN_PSEL_TX(n)					(n<<10)
#define TDM_TXCLK_INV						(1<<11)
#define TDM_TFSD(n)							(n<<13)
#define TDM_SYN_PSEL_RX(n)					(n<<16)
#define TDM_RXCLK_SEL_EXTERN				(1<<17)
#define TDM_TXSYNC_INV						(1<<18)
#define TDM_RXSYNC_INV						(1<<19)

#define TDM_BD_FFC(n)						((n)<<0)
#define TDM_BD_MCSEL(n)						((n)<<3)
#define TDM_BD_CNT(n)						((n)<<11)
#define TDM_BD_LST							(1<<15)

struct gpio_regs{
	unsigned int out;
	unsigned int reserved0[0xff];
	unsigned int dir;
	unsigned int reserved1[0x3];
	unsigned int int_en;
	unsigned int reserved2[0x3];
	unsigned int pue;
	unsigned int reserved3[0x3];
	unsigned int ibe;
	unsigned int reserved4[0x3];
	unsigned int ode;
	unsigned int reserved5[0x3];
	unsigned int in;
	unsigned int reserved6[0x3];
	unsigned int ctl;
};

#define DMA_SARL(n)							(QUICC_DMA_BASE+n*0x58)
#define DMA_SARH(n)							(QUICC_DMA_BASE+n*0x58+0x4)
#define DMA_DARL(n)							(QUICC_DMA_BASE+n*0x58+0x8)
#define DMA_DARH(n)							(QUICC_DMA_BASE+n*0x58+0xc)
#define DMA_LLPL(n)							(QUICC_DMA_BASE+n*0x58+0x10)
#define DMA_LLPH(n)							(QUICC_DMA_BASE+n*0x58+0x14)
#define DMA_CTLL(n)							(QUICC_DMA_BASE+n*0x58+0x18)
#define DMA_CTLH(n)							(QUICC_DMA_BASE+n*0x58+0x1c)
#define DMA_SSTATL(n)						(QUICC_DMA_BASE+n*0x58+0x20)
#define DMA_SSTATH(n)						(QUICC_DMA_BASE+n*0x58+0x24)
#define DMA_DSTATL(n)						(QUICC_DMA_BASE+n*0x58+0x28)
#define DMA_DSTATH(n)						(QUICC_DMA_BASE+n*0x58+0x2c)
#define DMA_SSTATARL(n)						(QUICC_DMA_BASE+n*0x58+0x30)
#define DMA_SSTATARH(n)						(QUICC_DMA_BASE+n*0x58+0x34)
#define DMA_DSTATARL(n)						(QUICC_DMA_BASE+n*0x58+0x38)
#define DMA_DSTATARH(n)						(QUICC_DMA_BASE+n*0x58+0x3c)
#define DMA_CFGL(n)							(QUICC_DMA_BASE+n*0x58+0x40)
#define DMA_CFGH(n)							(QUICC_DMA_BASE+n*0x58+0x44)
#define DMA_SGRL(n)							(QUICC_DMA_BASE+n*0x58+0x48)
#define DMA_SGRH(n)							(QUICC_DMA_BASE+n*0x58+0x4c)
#define DMA_DSRL(n)							(QUICC_DMA_BASE+n*0x58+0x50)
#define DMA_DSRH(n)							(QUICC_DMA_BASE+n*0x58+0x54)
#define DMA_RAWTFRL							(QUICC_DMA_BASE+0x2c0)
#define DMA_RAWTFRH							(QUICC_DMA_BASE+0x2c4)
#define DMA_RAWBLKL							(QUICC_DMA_BASE+0x2c8)
#define DMA_RAWBLKH							(QUICC_DMA_BASE+0x2cc)
#define DMA_RAWSTRANL						(QUICC_DMA_BASE+0x2d0)
#define DMA_RAWSTRANH						(QUICC_DMA_BASE+0x2d4)
#define DMA_RAWDTRANL						(QUICC_DMA_BASE+0x2d8)
#define DMA_RAWDTRANH						(QUICC_DMA_BASE+0x2dc)
#define DMA_RAWERRL							(QUICC_DMA_BASE+0x2e0)
#define DMA_RAWERRH							(QUICC_DMA_BASE+0x2e4)
#define DMA_STATTFRL						(QUICC_DMA_BASE+0x2e8)
#define DMA_STATTFRH						(QUICC_DMA_BASE+0x2ec)
#define DMA_STATBLKL						(QUICC_DMA_BASE+0x2f0)
#define DMA_STATBLKH						(QUICC_DMA_BASE+0x2f4)
#define DMA_STATSTRANL						(QUICC_DMA_BASE+0x2f8)
#define DMA_STATSTRANH						(QUICC_DMA_BASE+0x2fc)
#define DMA_STATDTRANL						(QUICC_DMA_BASE+0x300)
#define DMA_STATDTRANH						(QUICC_DMA_BASE+0x304)
#define DMA_STATERRL						(QUICC_DMA_BASE+0x308)
#define DMA_STATERRH						(QUICC_DMA_BASE+0x30c)
#define DMA_MASKTFRL						(QUICC_DMA_BASE+0x310)
#define DMA_MASKTFRH						(QUICC_DMA_BASE+0x314)
#define DMA_MASKBLKL						(QUICC_DMA_BASE+0x318)
#define DMA_MASKBLKH						(QUICC_DMA_BASE+0x31c)
#define DMA_MASKSTRANL						(QUICC_DMA_BASE+0x320)
#define DMA_MASKSTRANH						(QUICC_DMA_BASE+0x324)
#define DMA_MASKDTRANL						(QUICC_DMA_BASE+0x328)
#define DMA_MASKDTRANH						(QUICC_DMA_BASE+0x32c)
#define DMA_MASKERRL						(QUICC_DMA_BASE+0x330)
#define DMA_MASKERRH						(QUICC_DMA_BASE+0x334)
#define DMA_CLRTFRL							(QUICC_DMA_BASE+0x338)
#define DMA_CLRTFRH							(QUICC_DMA_BASE+0x33c)
#define DMA_CLRBLKL							(QUICC_DMA_BASE+0x340)
#define DMA_CLRBLKH							(QUICC_DMA_BASE+0x344)
#define DMA_CLRSTRANL						(QUICC_DMA_BASE+0x348)
#define DMA_CLRSTRANH						(QUICC_DMA_BASE+0x34c)
#define DMA_CLRDTRANL						(QUICC_DMA_BASE+0x350)
#define DMA_CLRDTRANH						(QUICC_DMA_BASE+0x354)
#define DMA_CLRERRL							(QUICC_DMA_BASE+0x358)
#define DMA_CLRERRH							(QUICC_DMA_BASE+0x35c)
#define DMA_STATINTL						(QUICC_DMA_BASE+0x360)
#define DMA_STATINTH						(QUICC_DMA_BASE+0x364)
#define DMA_REQSRL							(QUICC_DMA_BASE+0x368)
#define DMA_REQSRH							(QUICC_DMA_BASE+0x36c)
#define DMA_REQDRL							(QUICC_DMA_BASE+0x370)
#define DMA_REQDRH							(QUICC_DMA_BASE+0x374)
#define DMA_SGLREQSRL						(QUICC_DMA_BASE+0x378)
#define DMA_SGLREQSRH						(QUICC_DMA_BASE+0x37c)
#define DMA_SGLREQDRL						(QUICC_DMA_BASE+0x380)
#define DMA_SGLREQDRH						(QUICC_DMA_BASE+0x384)
#define DMA_LSTSRL							(QUICC_DMA_BASE+0x388)
#define DMA_LSTSRH							(QUICC_DMA_BASE+0x38c)
#define DMA_LSTDRL							(QUICC_DMA_BASE+0x390)
#define DMA_LSTDRH							(QUICC_DMA_BASE+0x394)
#define DMA_CFGRL							(QUICC_DMA_BASE+0x398)
#define DMA_CFGRH							(QUICC_DMA_BASE+0x39c)
#define DMA_CHENRL							(QUICC_DMA_BASE+0x3a0)
#define DMA_CHENRH							(QUICC_DMA_BASE+0x3a4)
#define DMA_IDRL							(QUICC_DMA_BASE+0x3a8)
#define DMA_IDRH							(QUICC_DMA_BASE+0x3ac)

#define LLP_SRC_EN							(1<<28)
#define LLP_DST_EN							(1<<27)
#define INT_EN								1
#define DST_TR_WIDTH_SHIFT					1
#define SRC_TR_WIDTH_SHIFT					4
#define TT_FC_SHIFT							20

#define DMA_EN								1

#define CH_EN(n)							((n)<<0)
#define CH_EN_WE(n)							((n)<<8)

struct dma_single_para_st{
	unsigned int cn;
	unsigned int sarl;
	//unsigned int sarh;
	unsigned int darl;
	//unsigned int darh;
	unsigned int tt_fc;
	unsigned int block_ts;
	unsigned int src_tr_width;
	unsigned int dst_tr_width;
//	unsigned int src_msize;
//	unsigned int dst_msize;
};

typedef enum{
	MEM_TO_MEM=0,
	MEM_TO_PER,
	PER_TO_MEM,
	PER_TO_PER
}ttfc_val;

typedef enum{
	WIDTH_8=0,
	WIDTH_16,
	WIDTH_32,
	WIDTH_64,
	WIDTH_128,
	WIDTH_256
}trwidth_val;

struct dma_lli_st{
	unsigned int sar;
	unsigned int dar;
	unsigned int llp;
	unsigned int ctl_l;
	unsigned int ctl_h;
	unsigned int sstat;
	unsigned int dstat;
};

#endif /* QUICC_H_ */
