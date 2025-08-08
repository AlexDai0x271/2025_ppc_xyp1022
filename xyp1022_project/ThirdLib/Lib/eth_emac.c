#include "Eth_emac.h"
//#include "common.h"
#include "e500mc_asm.h"

#define MIISPEED (0<<2)

#define VLAN_TAG 0x7788

#undef PSPRINTF
#define PSPRINTF prints

#ifdef XYP1022_DP83867
u32 phy_addr = 6;
#else if
//u32 phy_addr = 0x19;
u32 phy_addr = 0x5;
#endif

u8	vlan_tag[2]={0x77,0x88};
u32  enet_addr[6]= {0x00, 0x22, 0x44, 0x66, 0x88, 0xaa};
//u32  enet_addr[6]= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#ifdef JUMBO
u8	sendbuf[11000]={0};				//JUMBO
#else
u8	sendbuf[16384]={0};
#endif
u32	pmtbuf[128]={0x00,0x01,0x02};

volatile unsigned int *pt_to_last_edge_rxdesc;
volatile unsigned int *pt_to_last_edge_txdesc;

volatile unsigned int g_rx_flag;
volatile unsigned int g_tx_flag;
volatile unsigned int g_rx_num=0;
volatile unsigned int g_rx_err=0;

inline u32 gmac_read(volatile u32 addr);
inline void gmac_write(unsigned int b, volatile unsigned int addr);
void gmac_phy_write( int phyaddr_unused, int reg, int value);
int gmac_phy_read( int phy_reg_unused, int reg);

void init_gmac( int iGmacNum );

/*
 * GMAC0 : 88e1111
 * GMAC1 : DP83867
 */

static int s_iGmacNum = 0;

#define GMAC0_BASE_ADDR			(mac0_cfg_base + 0x4000*2)
#define GMAC1_BASE_ADDR			(mac0_cfg_base + 0x4000*3)

#define GMAC0_PHY_ADDR			0x5
#define GMAC1_PHY_ADDR			0x6

static unsigned int uiGmacBaseAddr[2] = { 0 };

static unsigned int uiGmacPhyAddr[2] = {
		GMAC0_PHY_ADDR,
		GMAC1_PHY_ADDR,
};

unsigned int swap_data(unsigned int val)
{
#if 1
	unsigned int hh,hl,lh,ll;
		ll=(val&0xff000000)>>24;
		lh=(val&0x00ff0000)>>8;
		hl=(val&0x0000ff00)<<8;
		hh=(val&0x000000ff)<<24;
		val = ll|lh|hl|hh;
#endif
	return val;
}


u32 readl_le(volatile u32 addr)
{
	unsigned int b,tmp1,tmp2,tmp3,tmp4;
	b = *((volatile u32 *) addr);
	tmp1 = (b & 0xff000000)>>24; tmp2 = (b & 0x00ff0000)>>16; tmp3 = (b & 0x0000ff00)>>8; tmp4 = (b & 0x000000ff);
	b = ( (tmp1) | (tmp2 << 8) | (tmp3 << 16) | (tmp4 << 24) );
	return b;
}

void writel_le(unsigned int b, volatile unsigned int addr)
{
	unsigned int tmp1,tmp2,tmp3,tmp4;
	tmp1 = (b & 0xff000000)>>24; tmp2 = (b & 0x00ff0000)>>16; tmp3 = (b & 0x0000ff00)>>8; tmp4 = (b & 0x000000ff);
	tmp1 = ( (tmp1) | (tmp2 << 8) | (tmp3 << 16) | (tmp4 << 24) );
	writel(addr,tmp1);
}

u32 gmac_read(volatile u32 addr)
{
	return readl(addr);
}

void gmac_write(unsigned int b, volatile unsigned int addr)
{
	writel(addr,b);
}

u32 desc_read(volatile u32 addr)
{
	return swap_data(readl(addr));
}

void desc_write( unsigned int b,volatile unsigned int addr)
{
	writel(addr,swap_data(b));
}

void sendbuf_fill(u8 *ptr,u32 init,u32 len)
{
	u32 i;
	u32 len16;
	u8 tmp;
	u8 j=init;
	//PSPRINTF("send buf ptr now is 0x%x\r\n",ptr);
	//PSPRINTF("send buf ptr now is 0x%x\r\n",ptr);
	for(i=0;i<6;i++)
	{
		//ptr[i]=enet_addr[i];
		ptr[i] = 0xff;
	}
	for(i=6;i<12;i++)
	{
		ptr[i]=enet_addr[i-6];
	}
//	ptr[12] = (len>>8)&0xff;
//	ptr[13] = (len)&0xff;
	ptr[12] = (len)&0xff;
	ptr[13] = (len>>8)&0xff;
	for(i=14;i<len+14;i++)
	{
		ptr[i]=j++;
	}
#ifdef big_endian
	len16 = (len+16+14)/16;					//head need include(14 byte)
	for(i=0;i<len16;i++)
	{
		for(j=0;j<8;j++)
		{
			tmp = ptr[i*16+j];
			ptr[i*16+j] = ptr[i*16+15-j];
			ptr[i*16+15-j] = tmp;
		}
	}
#endif
}

void pmtbuf_fill(u32 *ptr,u32 init,u32 offset,u32 len)
{
	u32 i;
	PSPRINTF("pmt buf ptr now is 0x%x\r\n",ptr);
	for(i=offset;i<offset+len;i++)
	{
		ptr[i]=init;
	}
}

int gmac_phy_read(int phy_reg_unused, int reg)			// has tested ok!!
{
#if 1
	int ret, i = 0;

	//gmac_dbg("entering gmac_phy_read function!\n");

	while (gmac_read(uiGmacBaseAddr[s_iGmacNum] + rGMIIAddr) & PHYBUSY)
	{
		;		//wait for free`
	}

	/* Fill the phyxcer register into GMII Address register */
	gmac_write((uiGmacPhyAddr[s_iGmacNum]<<11) | (reg<<6) | (0x1<<3) |(0x0<<1) | PHYBUSY | MIISPEED, uiGmacBaseAddr[s_iGmacNum] + rGMIIAddr);
	for (i=0; i<DELAY; i++)
	{
		if ((gmac_read(uiGmacBaseAddr[s_iGmacNum] + rGMIIAddr) & PHYBUSY) == 0)
		{
			break;
		}
	}

	if (i == DELAY)
	{
	    dev_info("phy read timed out\r\n");
	}
	ret = gmac_read(uiGmacBaseAddr[s_iGmacNum] + rGMIIData);

	//gmac_dbg("reg:%x val:%x\n", reg,ret);

	return ret;
#else
	return 0xffffffff;
#endif
}


void gmac_phy_write( int phyaddr_unused, int reg, int value)		//has tested ok!!
{
#if 1
	int i = 0;

	//gmac_dbg( "entering gmac_phy_write fuc\n" );

	while (gmac_read(uiGmacBaseAddr[s_iGmacNum] + rGMIIAddr) & PHYBUSY)						//refer to page of spec 227
	{
		;		//wait for free
	}

	/* Fill the phyxcer register into GMII Address register */
	gmac_write(value, uiGmacBaseAddr[s_iGmacNum] + rGMIIData);
	gmac_write((uiGmacPhyAddr[s_iGmacNum]<<11) | (reg<<6) | (0x1<<3) |(0x1<<1) | PHYBUSY | MIISPEED, uiGmacBaseAddr[s_iGmacNum] + rGMIIAddr );
	for (i=0; i<DELAY; i++)
	{
		if ((gmac_read(uiGmacBaseAddr[s_iGmacNum] + rGMIIAddr) & PHYBUSY) == 0)
		{
			break;
		}
	}

	if (i==DELAY)
	{
		dev_info("phy write timed out\r\n");
	}
#else
	return;
#endif
}

void gmac_phy_write_ext(int phyaddr,int reg,int val)
{
	gmac_phy_write(0,0xd,0x1f);
	gmac_phy_write(0,0xe,reg);
	gmac_phy_write(0,0xd,(1<<14) |0x1f);
	gmac_phy_write(0,0xe,val);
}

int gmac_phy_read_ext(int phyaddr,int reg)
{
	int val;
	gmac_phy_write(0,0xd,0x1f);
	gmac_phy_write(0,0xe,reg);
	gmac_phy_write(0,0xd,(1<<14) |0x1f);
	val = gmac_phy_read(0,0xe);
	return val;
}

void gmac_disable_irq( int iGmacNum )								//閼存绠嶉懘娆掓？閼存宕甸懘娆撳灳閳锋艾鍟犻敓鍊熷壔闁跨喓鐓崑顔炬緬閸愭寧瀚�
{
	gmac_write(0x0, uiGmacBaseAddr[iGmacNum] + rInterruptEnable);
}

void gmac_mask_irq(void)								//閼存绠嶉懘娆掓？閼存宕甸懘娆撳灳閳锋艾鍟犻敓鍊熷壔闁跨喓鐓崑顔炬緬閸愭寧瀚�
{
	gmac_write(RGSMIIIM_DIS_INT, uiGmacBaseAddr[s_iGmacNum] + rInterruptMask);
}

void gmac_enable_irq(void)								//閼存绠嶉懘娆掓？閼存宕甸懘娆撳灳閳锋艾鍟犻敓鍊熷壔闁跨喓鐓崑顔炬緬閸愭寧瀚�
{
	gmac_write(NIE | RIE | TIE, uiGmacBaseAddr[s_iGmacNum] + rInterruptEnable);
}

/*****************************************************************************
 * ** -Function:
 * **   gmac_cre_def_tx_des(volatile unsigned int *tx_des_base_addr)
 * **
 * ** -Description:
 * **   This function create NO_OF_TXDES linked default Tx descriptors, and they form a ring
 * **
 * ** -Input Param
 * **	tx_des_base_addr: point to the physical base address of memory allocated for
 * **	Tx descriptors
 * **
 * ** -Return
 * **   NULL
 * *****************************************************************************/

#ifndef big_endian
void gmac_cre_def_tx_des(unsigned int tx_des_base_addr)
{
	int i = 0;
	unsigned int *pointto_cur_des;

	pointto_cur_des = (unsigned int *)tx_des_base_addr;

	for (i=0; i<NO_OF_TXDES; i++)
	{
		desc_write(0, pointto_cur_des);	//at first the Tx descriptor is not owned by DMA control
//		desc_write((1<<20), pointto_cur_des);	//tch
//		desc_write(swap_data(IC | CIC | DC_T | TCH | DP | TTSE | TBS2), pointto_cur_des + 1);			//Config the TxDES1,if pc not accept tx frame,set DC flag for test

		desc_write(IC | DC_T | TCH | DP | TTSE , pointto_cur_des + 1);				//former
//		desc_write(swap_data(IC | DC_T | DP | TTSE) , pointto_cur_des + 1);						//for test		,point 1############

//		desc_write(((ETH_BASE_TXBUF_PHY+i*RBS1) & 0xffff) | 0xffff0000, pointto_cur_des + 2);		//Physical address for Rx buffer
//		desc_write(((ETH_BASE_TXDES_PHY+(i+1)*SINGLE_TXDES_SIZE) & 0xffff) | 0xffff0000, pointto_cur_des + 3);		//Next Tx descriptor address
		desc_write(ETH_BASE_TXBUF_PHY+i*RBS1, pointto_cur_des + 2);		//Physical address for Rx buffer
		desc_write(ETH_BASE_TXDES_PHY+(i+1)*SINGLE_TXDES_SIZE, pointto_cur_des + 3);		//Next Tx descriptor address

		pointto_cur_des += 4;
	}
	pointto_cur_des -= 4;
//	desc_write((ETH_BASE_TXDES_PHY & 0xffff) | 0xffff0000, pointto_cur_des + 3);	//Next Tx descriptor address of last descriptor is the first one
	desc_write(ETH_BASE_TXDES_PHY, pointto_cur_des + 3);	//Next Tx descriptor address of last descriptor is the first one
}
#else
void gmac_cre_def_tx_des(unsigned int tx_des_base_addr)
{
	int i = 0;
	unsigned int *pointto_cur_des;

	pointto_cur_des = (unsigned int *)tx_des_base_addr;

	for (i=0; i<NO_OF_TXDES; i++)
	{
		desc_write(0, pointto_cur_des+3);	//at first the Tx descriptor is not owned by DMA control
//		desc_write((1<<20), pointto_cur_des+3);	//tch
//		desc_write(swap_data(IC | CIC | DC_T | TCH | DP | TTSE | TBS2), pointto_cur_des + 1);			//Config the TxDES1,if pc not accept tx frame,set DC flag for test

		desc_write(IC | DC_T | TCH | DP | TTSE , pointto_cur_des + 2);				//former
//		desc_write(swap_data(IC | DC_T | DP | TTSE) , pointto_cur_des + 1);						//for test		,point 1############

//		desc_write(((ETH_BASE_TXBUF_PHY+i*RBS1) & 0xffff) | 0xffff0000, pointto_cur_des + 2);		//Physical address for Rx buffer
//		desc_write(((ETH_BASE_TXDES_PHY+(i+1)*SINGLE_TXDES_SIZE) & 0xffff) | 0xffff0000, pointto_cur_des + 3);		//Next Tx descriptor address
		desc_write(ETH_BASE_TXBUF_PHY+i*RBS1, pointto_cur_des + 1);		//Physical address for Rx buffer
		desc_write(ETH_BASE_TXDES_PHY+(i+1)*SINGLE_TXDES_SIZE, pointto_cur_des + 0);		//Next Tx descriptor address

		pointto_cur_des += 4;
	}
	pointto_cur_des -= 4;
//	desc_write((ETH_BASE_TXDES_PHY & 0xffff) | 0xffff0000, pointto_cur_des + 3);	//Next Tx descriptor address of last descriptor is the first one
	desc_write(ETH_BASE_TXDES_PHY, pointto_cur_des + 0);	//Next Tx descriptor address of last descriptor is the first one
}
#endif

/*****************************************************************************
 * ** -Function:
 * **   gmac_cre_def_rx_des(volatile unsigned int *rx_des_base_addr)
 * **
 * ** -Description:
 * **   This function create NO_OF_RXDES linked default Rx descriptors, and they form a ring
 * **
 * ** -Input Param
 * **	rx_des_base_addr: point to the physical base address of memory allocated for
 * **	Rx descriptors
 * **
 * ** -Return
 * **   NULL
 * *****************************************************************************/
#ifndef big_endian
void gmac_cre_def_rx_des(unsigned int rx_des_base_addr)
{
	int i = 0;
	unsigned int *pointto_cur_des;

	pointto_cur_des = (unsigned int *)rx_des_base_addr;

	for (i=0; i<NO_OF_RXDES; i++)
	{
		desc_write(RXOWN, pointto_cur_des);	//at first the Rx descriptor is owned by DMA control

		desc_write(DINT | RCH | RBS2 | RBS1, pointto_cur_des + 1);		//Config the RxDES1,former
//		desc_write(DINT | (1<<14) | RBS1, pointto_cur_des + 1);		//zjjin		next link

//		desc_write(((ETH_BASE_RXBUF_PHY+i*RBS1) & 0xffff) | 0xffff0000, pointto_cur_des + 2);		//Physical address for Rx buffer
//		desc_write(((ETH_BASE_RXDES_PHY+(i+1)*SINGLE_RXDES_SIZE) & 0xffff) | 0xffff0000, pointto_cur_des + 3);	//Next Rx descriptor address
		desc_write(ETH_BASE_RXBUF_PHY+i*RBS1, pointto_cur_des + 2);		//Physical address for Rx buffer
		desc_write(ETH_BASE_RXDES_PHY+(i+1)*SINGLE_RXDES_SIZE, pointto_cur_des + 3);	//Next Rx descriptor address
		pointto_cur_des += 4;
	}
	pointto_cur_des -= 4;
//	desc_write((ETH_BASE_RXDES_PHY & 0xffff) | 0xffff0000, pointto_cur_des + 3);	////Next Rx descriptor address of last descriptor is the first one
	desc_write(ETH_BASE_RXDES_PHY, pointto_cur_des + 3);	////Next Rx descriptor address of last descriptor is the first one
}
#else
void gmac_cre_def_rx_des(unsigned int rx_des_base_addr)
{
	int i = 0;
	unsigned int *pointto_cur_des;

	pointto_cur_des = (unsigned int *)rx_des_base_addr;

	for (i=0; i<NO_OF_RXDES; i++)
	{
		desc_write(RXOWN, pointto_cur_des+3);	//at first the Rx descriptor is owned by DMA control

		desc_write(DINT | RCH | RBS2 | RBS1, pointto_cur_des + 2);		//Config the RxDES1,former
		//desc_write(DINT | (1<<14) | RBS1, pointto_cur_des + 2);		//Config the RxDES1,former

//		desc_write(((ETH_BASE_RXBUF_PHY+i*RBS1) & 0xffff) | 0xffff0000, pointto_cur_des + 2);		//Physical address for Rx buffer
//		desc_write(((ETH_BASE_RXDES_PHY+(i+1)*SINGLE_RXDES_SIZE) & 0xffff) | 0xffff0000, pointto_cur_des + 3);	//Next Rx descriptor address
		desc_write(ETH_BASE_RXBUF_PHY+i*RBS1, pointto_cur_des + 1);		//Physical address for Rx buffer
		desc_write(ETH_BASE_RXDES_PHY+(i+1)*SINGLE_RXDES_SIZE, pointto_cur_des + 0);	//Next Rx descriptor address
		pointto_cur_des += 4;
	}
	pointto_cur_des -= 4;
//	desc_write((ETH_BASE_RXDES_PHY & 0xffff) | 0xffff0000, pointto_cur_des + 3);	////Next Rx descriptor address of last descriptor is the first one
	desc_write(ETH_BASE_RXDES_PHY, pointto_cur_des + 0);	////Next Rx descriptor address of last descriptor is the first one
}
#endif
static void gmac_hash_table(void)
{
	int i;
	u32 hash_val;
	u16 hash_table[4];
	u32 rcr = 0;
	u32 crc=0;
	u32 crc_bit=0;

	gmac_dbg("entering gmac_hash_table function!\r\n");

	gmac_write(enet_addr[5]<<8 | enet_addr[4], uiGmacBaseAddr[s_iGmacNum] + rMACAddr0H);			//mac high addr
	gmac_write(enet_addr[3]<<24 | enet_addr[2]<<16 | enet_addr[1]<<8 | enet_addr[0],
	   uiGmacBaseAddr[s_iGmacNum] + rMACAddr0L);													//mac low addr

	#ifdef MAC_RCVALL
		rcr |=0x80000000;
	#endif

	#ifdef MAC_UDAF					//unicast da filter
		rcr&=~(HUC|HMC);
		rcr|=HPF;
		rcr|=DBF;					//no accept broadcast
	#endif

	#ifdef MAC_UHASH				//unicast hash filter
		rcr&=~(HMC|HPF);
		rcr|=HUC;
		rcr|=DBF;					//no accept broadcast
		initCrcTable_32();
		crc = computeCrc_32(enet_addr,6);

		crc_bit =(crc>>26)&0x1F;
		if(crc&0x80000000)
			gmac_write(1<<crc_bit, uiGmacBaseAddr[s_iGmacNum] + rHashTableH);
		else
			gmac_write(1<<crc_bit, uiGmacBaseAddr[s_iGmacNum] + rHashTableL);
	#endif

	#ifdef VLAN_MATCH
		rcr|=VTFE;
		rcr|=DBF;					//no accept broadcast
		gmac_write((0<<19)|(1<<16)|VLAN_TAG, uiGmacBaseAddr[s_iGmacNum] + rVLANTag);
	#endif

	#ifdef VLAN_HASH
		rcr|=VTFE;
		rcr|=DBF;					//no accept broadcast
		gmac_write((1<<19)|(0<<16)|VLAN_TAG, uiGmacBaseAddr[s_iGmacNum] + rVLANTag);
		initCrcTable_32();
		crc = computeCrc_32(vlan_tag,2);

		crc_bit =(crc>>28)&0x0F;
		gmac_write(1<<crc_bit , uiGmacBaseAddr[s_iGmacNum] + rVlanHash);
	#endif

	gmac_write(rcr, uiGmacBaseAddr[s_iGmacNum] + rMACFrameFilter);
}

void gmac_irq_status(void)
{
	u32 intr_status = gmac_read(Eth_addr + rInterruptStatus);
	u32 tmp;
	/* Not used events (e.g. MMC interrupts) are not handled. */
	if ((intr_status & 0x400))
	{
		dev_dbg("GMAC: LPI entry or exit\r\n");
		tmp = gmac_read(Eth_addr + rLPIcsr);
		dev_dbg("GMAC: LPI status is %x\r\n",tmp);

	}
	if ((intr_status & 0x40))		//mmc_tx_irq
		dev_dbg("GMAC: MMC tx interrupt: 0x%08x\r\n",gmac_read(Eth_addr + GMAC_MMC_TX_INTR));
	if ((intr_status & 0x20))		//mmc_rx_irq
		dev_dbg("GMAC: MMC rx interrupt: 0x%08x\r\n",gmac_read(Eth_addr + GMAC_MMC_RX_INTR));
	if ((intr_status & 0x80))			//mmc_rx_csum_offload_irq
		dev_dbg("GMAC: MMC rx csum offload: 0x%08x\r\n",gmac_read(Eth_addr + GMAC_MMC_RX_CSUM_OFFLOAD));
	if ((intr_status & 0x08)) 		//pmt_irq
	{
		dev_dbg("GMAC: received Magic or remote frame\r\n");
		/* clear the PMT bits 5 and 6 by reading the PMT
		 * status register. */
		gmac_read(Eth_addr + rPMTControl);
	}
}

int dma_irq_handler(void)
{
	int ret = 0;
	/* read the status register (CSR5) */
	u32 intr_status = gmac_read(Eth_addr + rStatus);
	/* ABNORMAL interrupts */
	if ((intr_status & DMA_STATUS_AIS)) {
		dev_dbg("DMA ABNORMAL IRQ: ");
		if ((intr_status & DMA_STATUS_UNF)) {
			dev_dbg("transmit underflow\r\n");
		}
		if ((intr_status & DMA_STATUS_TJT)) {
			dev_dbg( "transmit jabber\r\n");
		}
		if ((intr_status & DMA_STATUS_OVF)) {
			dev_dbg( "recv overflow\r\n");
		}
		if ((intr_status & DMA_STATUS_RU)) {
			dev_dbg( "receive buffer unavailable\r\n");
		}
		if ((intr_status & DMA_STATUS_RPS)) {
			dev_dbg( "receive process stopped\r\n");
		}
		if ((intr_status & DMA_STATUS_RWT)) {
			dev_dbg( "receive watchdog\r\n");
		}
		if ((intr_status & DMA_STATUS_ETI)) {
			dev_dbg( "transmit early interrupt\r\n");
		}
		if ((intr_status & DMA_STATUS_TPS)) {
			dev_dbg( "transmit process stopped\r\n");
		}
		if ((intr_status & DMA_STATUS_FBI)) {
			dev_dbg( "fatal bus error\r\n");
		}
	}
	/* TX/RX NORMAL interrupts */
	if (intr_status & DMA_STATUS_NIS) {
		if ((intr_status & DMA_STATUS_RI))
		{
			g_rx_flag = 1;
		}
		if(intr_status & (DMA_STATUS_TI))
		{
			g_tx_flag = 1;
		}
	}
	/* Optional hardware blocks, interrupts should be disabled */
	if ((intr_status &
		     (DMA_STATUS_GPI | DMA_STATUS_GMI | DMA_STATUS_GLI)))
		dev_dbg("unexpected status %x\r\n", intr_status);
	/* Clear the interrupt by writing a logic 1 to the CSR5[15-0] */
	gmac_write((intr_status & 0x1ffff), Eth_addr + rStatus);

	return ret;
}

void gmac_irq_handler(void)
{
	gmac_irq_status();
	dma_irq_handler();
}

void pmt_irq_handler(void)
{
	u32 intr_status = gmac_read(Eth_addr + rInterruptStatus);
	if ((intr_status & 0x08)) 		//pmt_irq
	{
		dev_dbg("PMT: received Magic or remote frame\r\n");
		/* clear the PMT bits 5 and 6 by reading the PMT
		 * status register. */
		gmac_read(Eth_addr + rPMTControl);
	}
}

void lpi_irq_handler(void)
{
	u32 tmp;
	u32 intr_status = gmac_read(Eth_addr + rInterruptStatus);
	if ((intr_status & 0x400)) 		//lpi_irq
	{
		dev_dbg("LPI: LPI enter or exit\r\n");
		/* clear the PMT bits 5 and 6 by reading the PMT
		 * status register. */
		tmp = gmac_read(Eth_addr + rLPIcsr);
		dev_dbg("LPI: LPI status is %x\r\n",tmp);
	}
}

void rx_poll(u8 *pdata,u32 len)
{
	u32 i,j;
	u32 len16;
	u8 tmp;
	dev_dbg("receive len %d,buffer data is: \r\n",len-4);

#ifdef big_endian
	len16 = (len+16)/16;
	for(i=0;i<len16;i++)
	{
		for(j=0;j<8;j++)
		{
			tmp = pdata[i*16+j];
			pdata[i*16+j] = pdata[i*16+15-j];
			pdata[i*16+15-j] = tmp;
		}
	}
#endif
//	if(len>32)
//		len=32;
	for(i=0;i<len-4;i++)
	{
		if((i%10)==0)
		{
			dev_dbg("\r\n");
		}
		dev_dbg("0x%x ",pdata[i]);
	}
	dev_dbg("\r\n");
}

void rx_poll_check(u8 *pdata,u32 len,u32 islast)
{
	u32 i,j;
	u32 len16;
	u8 tmp;
	u8 *checkdata = (u8 *)((unsigned int)pdata - (SIZE_OF_RXDES + SIZE_OF_TXBUF));
	dev_dbg("receive len %d\r\n",len-4);

//	if(len>32)
//		len=32;
	if(islast)
		len-=4;

	for(i=0;i<len;i++)
	{
		if((i%10)==0)
		{
			dev_dbg("\r\n");
		}
		dev_dbg("0x%x ",pdata[i]);
	}
	dev_dbg("\r\n");

#ifndef big_endian
	for(i=0;i<len;i++)
	{
		if(pdata[i]!=checkdata[i])
		{
			dev_dbg("check error: at offset %d\r\n",i);
			while(1);
		}
	}
#else
	for(i=0;i<len/16;i++)
	{
		for(j=0;j<16;j++)
		{
			if(pdata[i*16 + 15-j]!=checkdata[i*16 + 15-j])
			{
				dev_dbg("check error: at offset %d\r\n",i);
				while(1);
			}
		}
	}
#endif
	dev_dbg("check round\r\n");
}

/*****************************************************************************
 * ** -Function:
 * **   gmac_rx(struct net_device *dev)
 * **
 * ** -Description:
 * **   This function implement passing all received frames to upper layer
 * **
 * ** -Input Param
 * **	dev: point to the struct net_device whose private pointer is struct board_info_t *
 * **
 * ** -Return
 * **   NULL
 * *****************************************************************************/
void gmac_rx()
{
	u8  *rdptr;
	volatile unsigned int *pointto_cur_des;
	u8 *pointto_cur_rx_pkt;		//point to the current RX frame
	u16  pkt_len;
	u32 rx_status;
	u32 tmp;
	u16 seg_last=0;
	static u16 seg_cnt;

	gmac_dbg("entering gmac_rx fuction!!!\r\n");

	tmp=gmac_read(uiGmacBaseAddr[s_iGmacNum] + rStatus);

	gmac_dbg("gmac rstatus is %x\r\n",tmp);

	pointto_cur_des = pt_to_last_edge_rxdesc;

	/* find all received frames in these RX descriptors*/
#ifndef big_endian
	while (!(desc_read(pointto_cur_des) & RXOWN))			//if not owned by dma,it is mean that dma has moved the data to mem
#else
		while (!(desc_read(pointto_cur_des+3) & RXOWN))			//if not owned by dma,it is mean that dma has moved the data to mem
#endif
	{
		g_rx_num++;
#ifndef big_endian
		if(desc_read(pointto_cur_des) & RXLS)			//the RXLS bit means the end of one received frame
#else
		if(desc_read(pointto_cur_des+3) & RXLS)			//the RXLS bit means the end of one received frame
#endif
		{
			seg_last=1;
//			dev_dbg( "The Rx segment is  a frame last one!\r\n");
		}
#ifndef big_endian
		if (desc_read(pointto_cur_des) & RXFS)
#else
		if(desc_read(pointto_cur_des+3) & RXFS)			//the RXLS bit means the end of one received frame
#endif
		{
			seg_cnt = 0;
//			dev_dbg( "The Rx segment is  a frame first one!\r\n");
		}
#ifndef big_endian
		rx_status = desc_read(pointto_cur_des);
#else
		rx_status = desc_read(pointto_cur_des+3);			//the RXLS bit means the end of one received frame
#endif
		if(rx_status & RXES)
		{
			g_rx_err++;
			printk("Rx error!\n" );
			if (rx_status & RXDE)
			{
				printk("Descriptor Error\r\n");
			}
			if (rx_status & RXOE)
			{
				printk("Overflow Error\r\n");
			}
			if (rx_status & RXGF)
			{
				printk("Giant Frame\n");
			}
			if (rx_status & RXLC)
			{
				printk("Late Collision\r\n");
			}
			if (rx_status & RXWT)
			{
				printk("Watchdog Timeout\r\n");
			}
			if (rx_status & RXRE)
			{
				printk("Receive Error\r\n");
			}
			if (rx_status & RXCRCE)
			{
				printk("CRC Error\r\n");
			}
			while(1);
		}

		if (rx_status & RXVLAN)
		{
			printk("VLAN TAG COMPARED\r\n");
		}

		if(!(rx_status & (RXLS|RXES)))
		{
			seg_cnt++;
		}

//		pointto_cur_rx_pkt = (u8 *)(((ETH_BASE_RXBUF_PHY+ (pointto_cur_des - (u32 *)ETH_BASE_RXDES_PHY)/4*RBS1) & 0xffff) | 0x100000);
		pointto_cur_rx_pkt = (u8 *)ETH_BASE_RXBUF_PHY+ (pointto_cur_des - (u32 *)ETH_BASE_RXDES_PHY)/4*RBS1;
#ifndef big_endian
		pkt_len = (desc_read(pointto_cur_des) >> 16) & 0x3fff;		//the length of the frame
#else
		pkt_len = (desc_read(pointto_cur_des+3) >> 16) & 0x3fff;		//the length of the frame
#endif

		/* Packet Status check */
		if (!seg_last)
		{
			pkt_len = MAX_BUF;
		}
		else
		{
			pkt_len -= MAX_BUF*seg_cnt;
		}
//		dev_dbg( "RX Len is:%x\r\n", pkt_len);

#ifdef EXTERNAL_LOOP
		rx_poll_check(pointto_cur_rx_pkt,pkt_len,seg_last);
		if(seg_last)
		{
			dev_dbg( "loopback check ok\n");
//			while(1);
		}
#else
	#ifdef INTERNAL_LOOP
		rx_poll_check(pointto_cur_rx_pkt,pkt_len,seg_last);
		if(seg_last)
		{
			dev_dbg( "loopback check ok\n");
			while(1);
		}
	#else
		rx_poll(pointto_cur_rx_pkt,pkt_len);	//its printf Rx_buf
	#endif

#endif


		/* Now the Rx descriptor could be used by DMA again */
#ifndef big_endian
		desc_write(RXOWN, pointto_cur_des);
		pointto_cur_des = desc_read(pointto_cur_des + 3);		//goto next RxDes
#else
		desc_write(RXOWN, pointto_cur_des+3);
		pointto_cur_des = desc_read(pointto_cur_des + 0);		//goto next RxDes
#endif

	}

	gmac_dbg("====================== a phase is over!!! ======================\r\n");

	pt_to_last_edge_rxdesc = pointto_cur_des;

	return;

}

/*****************************************************************************
 * ** -Function:
 * **   gmac_cre_cur_tx_des(struct board_info *db, uint32_t *pdata_phyaddr, uint32_t len,
 * **	volatile unsigned int *tx_des_base_addr)
 * **
 * ** -Description:
 * **   This function put the Tx frame into current descriptor(only one des is enough
 * **	for one frame).
 * **
 * ** -Input Param
 * **	db: point to the struct board_info
 * **	pdata_phyaddr: point to the physical address of the Tx frame
 * **	len: length of Tx frame
 * **	tx_des_base_addr: point to the physical base address of memory allocated for
 * **	Tx descriptors
 * **
 * ** -Return
 * **   NULL
 * *****************************************************************************/

void gmac_cre_cur_tx_des(unsigned int *pointto_cur_des,unsigned char *pdata,unsigned int pdata_len)
{
	unsigned int i;
	unsigned char *buffer;
	unsigned tmp;

	gmac_dbg("putting the Tx frame into current descriptors\r\n");


	gmac_dbg("pointto_cur_des %x\r\n", pointto_cur_des);
#ifndef big_endian
	tmp = desc_read(pointto_cur_des + 1);

	desc_write(tmp | pdata_len, pointto_cur_des + 1);
//	buffer = (desc_read(pointto_cur_des + 2) & 0xffff) | 0x100000;
	buffer = desc_read(pointto_cur_des + 2);
#else
	tmp = desc_read(pointto_cur_des + 2);

	desc_write(tmp | pdata_len, pointto_cur_des + 2);
//	buffer = (desc_read(pointto_cur_des + 2) & 0xffff) | 0x100000;
	buffer = desc_read(pointto_cur_des + 1);
#endif

#ifdef big_endian
	for(i=0;i<(pdata_len+16);i++)
#else
	for(i=0;i<pdata_len;i++)
#endif
	{
		buffer[i]=pdata[i];
	}

#ifndef big_endian
	tmp = desc_read(pointto_cur_des);
	desc_write(TXOWN|tmp|CMPL_INT, pointto_cur_des);					//this Tx descriptor is owned by DMA control now
#else
	tmp = desc_read(pointto_cur_des+3);
	desc_write(TXOWN|tmp|CMPL_INT, pointto_cur_des+3);					//this Tx descriptor is owned by DMA control now
#endif
}

/*****************************************************************************
 * ** -Function:
 * **   gmac_start_xmit(struct sk_buff *skb, struct net_device *dev)
 * **
 * ** -Description:
 * **   This function implement sending a frame to media from the upper layer.
 * **
 * ** -Input Param
 * **	skb: point to struct sk_buff which contain the frame needed to be sent
 * **	dev: point to the struct net_device whose private pointer is struct board_info_t *
 * **
 * ** -Return
 * **   0 means success, 1 means failure
 * *****************************************************************************/
int gmac_start_xmit(u8* pdata,u32 packet_len)
{
	//由于我这里配置的sram是大端，但是gmac配成小端，所以TxRxdescriptor需要全部swap_data
	volatile unsigned int *pointto_cur_des;
	u8 *pointto_cur_tx_pkt;		//point to the current RX frame
	u32 nr_frag;
	u32 i;
	u32 tmp,mark;

	if(packet_len)
	{
		if(!(packet_len%MAX_BUF))				//if packet_len = n*MAX_BUF
			mark = MAX_BUF;
		else
			mark = packet_len%MAX_BUF;
	}

	gmac_dbg("entering gmac_start_xmit fuction!!!\r\n");

	pointto_cur_des = pt_to_last_edge_txdesc;

	nr_frag = ((packet_len-1)/MAX_BUF)+1;		//in nomal desc buffer can be max to 2K bytes

	for(i=0;i<nr_frag;i++)
	{
#ifndef big_endian
		if(desc_read(pointto_cur_des) & 0x80000000)			//when owned by dma,return
		{
			dev_dbg( "no enough tx desc can be used to xmit this packet,please wait!\r\n");
			return -1;
		}

//		pointto_cur_des = (desc_read(pointto_cur_des+3) & 0xffff) | 0x100000;			//ddr
		pointto_cur_des = desc_read(pointto_cur_des+3);
#else
		if(desc_read(pointto_cur_des+3) & 0x80000000)			//when owned by dma,return
		{
			dev_dbg( "no enough tx desc can be used to xmit this packet,please wait!\r\n");
			return -1;
		}

//		pointto_cur_des = (desc_read(pointto_cur_des+3) & 0xffff) | 0x100000;			//ddr
		pointto_cur_des = desc_read(pointto_cur_des);
#endif
	}

	pointto_cur_des = pt_to_last_edge_txdesc;

	for(i=0;i<nr_frag;i++)
	{
//		gmac_cre_cur_tx_des(pointto_cur_des,pdata,(i==(nr_frag-1))?(packet_len%MAX_BUF):MAX_BUF);
		gmac_cre_cur_tx_des(pointto_cur_des,pdata,(i==(nr_frag-1))?mark:MAX_BUF);						//908_modification
		pdata += MAX_BUF;
#ifndef big_endian
		if(i==0)
		{
			tmp = desc_read(pointto_cur_des+1);
			desc_write(tmp|0x20000000,pointto_cur_des+1);					//first segment
		}

		if(i==(nr_frag-1))
		{
			tmp = desc_read(pointto_cur_des+1);
			desc_write(tmp|0x40000000,pointto_cur_des+1);					//last segment
		}

//		pointto_cur_des = (desc_read(pointto_cur_des+3) & 0xffff) | 0x100000;
		pointto_cur_des = desc_read(pointto_cur_des+3);
#else
		if(i==0)
		{
			tmp = desc_read(pointto_cur_des+2);
			desc_write(tmp|0x20000000,pointto_cur_des+2);					//first segment
		}

		if(i==(nr_frag-1))
		{
			tmp = desc_read(pointto_cur_des+2);
			desc_write(tmp|0x40000000,pointto_cur_des+2);					//last segment
		}

//		pointto_cur_des = (desc_read(pointto_cur_des+3) & 0xffff) | 0x100000;
		pointto_cur_des = desc_read(pointto_cur_des+0);
#endif
	}

	pt_to_last_edge_txdesc = pointto_cur_des;

//	show_desc(ETH_BASE_TXDES_PHY,0x40);

	gmac_write(1,uiGmacBaseAddr[s_iGmacNum]+rTxPollDemand);

	gmac_dbg("out of gmac_start_xmit fuction!\r\n");

	return 0;
}


void gmac_reset(void)									//閼存瑩鏁撻惌顐㈠幗鐠囥儱鍟犻敓鍊熷壔閹煎倽鍓婚柍銉﹀櫞閸忕噥鐓侀崑顔借溅閸嬵喛顕氶崗顖氭灩閸忕噥鍔堕崑顔界ス閸忕喍绠欓崑顔款嚉閸嬵喚鏌曢崗锝囩叓閸嬵喖澶熼崑顕嗚抗閸嬵噮鍎崑顔款嚉閸嬵喚鏌曢崗锝囩叓閸嬵喖澶熼崘锝忔嫹閼存瑩绠嶉懘娆撳灳濠у秴鍠涢幏鐤壒绾板矁鍓婚柍銉э拷mac閼存缍嶉懘娆撳灳閻忚浼勯悿闈涘幗绾崘锝忔嫹閼存瑨鐭�
{
	u32 tmp, i = 0;
	dev_dbg("resetting device\r\n");

	/* reset device */
	tmp = gmac_read(uiGmacBaseAddr[s_iGmacNum] + rBUSMODE);
	tmp |= SWR;
	gmac_write(tmp, uiGmacBaseAddr[s_iGmacNum] + rBUSMODE);         //reset all Gmac registers
	for (i=0; i<DELAY; i++)
	{
		if ((gmac_read(uiGmacBaseAddr[s_iGmacNum] + rBUSMODE) & SWR) == 0)
		{
			break;
		}
	}

	if (i == DELAY)
	{
		dev_info(uiGmacBaseAddr[s_iGmacNum], "phy reset timed out\r\n");
//		while(1);
	}
}

void init_buffer(void)
{
	SIZE_OF_TXDES = (SINGLE_TXDES_SIZE*NO_OF_TXDES);
	SIZE_OF_TXBUF = (SINGLE_TXBUF_SIZE*NO_OF_TXDES);
	SIZE_OF_RXDES = (SINGLE_RXDES_SIZE*NO_OF_RXDES);
	SIZE_OF_RXBUF = (SINGLE_RXBUF_SIZE*NO_OF_RXDES);

//	ETH_BASE_TXDES_PHY = 0x100000;				//the original at ddr
	ETH_BASE_TXDES_PHY = ADDR_START_LOW;

	ETH_BASE_TXBUF_PHY = (ETH_BASE_TXDES_PHY + SIZE_OF_TXDES);
	ETH_BASE_RXDES_PHY = (ETH_BASE_TXBUF_PHY + SIZE_OF_TXBUF);
	ETH_BASE_RXBUF_PHY = (ETH_BASE_RXDES_PHY + SIZE_OF_RXDES);
}

void gmac_addr_init( void )
{
	uiGmacBaseAddr[0] = GMAC0_BASE_ADDR;
	uiGmacBaseAddr[1] = GMAC1_BASE_ADDR;
}

void init_gmac( int iGmacNum )
{
	u32 tmp,eth_speed,datatmp = 0;
	
	s_iGmacNum = iGmacNum;
	
	gmac_addr_init();

	gmac_dbg("entering init_gmac function!\r\n");
	gmac_dbg("Gmac%d BaseAddr = 0x%x\n\r", iGmacNum, GMAC1_BASE_ADDR);
	gmac_dbg("Gmac%d BaseAddr = 0x%x\n\r", iGmacNum, uiGmacBaseAddr[iGmacNum]);

	#ifdef GMII
		eth_speed = FAST_SPEED;						//1000M
	#else
		eth_speed = NORMAL_SPEED;					//100M
	#endif

	if(eth_speed == FAST_SPEED)
		gmac_write(0xf,EMAC_CLK_SEL_CFG);			//select the 125Mhz clock		GMII	by zjjin
	else
		gmac_write(0x0,EMAC_CLK_SEL_CFG);			//select the 25Mhz clock		MII		by zjjin

	gmac_reset();

	gmac_mask_irq();					//mask RGSMIIIM interrupt
//	read_reg();

//#if 1
	tmp = gmac_phy_read(0,MII_BMCR);

	tmp &= ~(BMCR_FULLDPLX | BMCR_SPEED100 | BMCR_SPEED1000 | BMCR_ANENABLE);

	if(eth_speed == FAST_SPEED)
		tmp |= BMCR_SPEED1000;
	else
		tmp |= BMCR_SPEED100;

	#ifdef LOW_SPEED
		tmp &= ~(BMCR_SPEED100|BMCR_SPEED1000);
	#endif

	tmp |= BMCR_FULLDPLX;

	gmac_phy_write(0, MII_BMCR, tmp|BMCR_RESET);				//reset and config PHY

	datatmp = gmac_phy_read(0,MII_BMCR);
	while( gmac_phy_read(0,MII_BMCR)& 0x8000 );


		#ifndef GMII		//100M	&& 88e1119
			gmac_phy_write(0,MII_PAGE, 2);
			delay(200000);
			tmp = gmac_phy_read(0,MII_SP_CTRL_2);
			gmac_phy_write(0,MII_SP_CTRL_2, tmp & 0xfff8 | 0x5);
			delay(200000);
			gmac_phy_write(0,MII_PAGE, 0);
			delay(200000);
		#endif

	#ifdef EXTERNAL_LOOP

	gmac_phy_write(0,MII_BMCR, tmp | 0x4000);

	while( !( gmac_phy_read(0,MII_BMCR)& 0x4000 ));

	delay(10000000);

	#else

//	datatmp = gmac_phy_read(0,MII_AT001_PSCR);
	datatmp = gmac_phy_read(0,0x6E);

	datatmp = gmac_phy_read(0,MII_BMSR);
	while(!(gmac_phy_read(0,MII_BMSR)& 0x4));					//link up

	#endif
	gmac_hash_table();

	// config the DMA BUSMODE register
//	gmac_write(AAL|FBPL|FB|PBL|DSL, Eth_addr + rBUSMODE);		//skip Tx_des
//	gmac_write(AAL|FBPL|FB|PBL, Eth_addr + rBUSMODE);			//no skip
//	gmac_write(0x1201000, Eth_addr + rBUSMODE);					//zjjin:sync with ic code
	gmac_write(0x1100800, uiGmacBaseAddr[s_iGmacNum] + rBUSMODE);					//zjjin:sync with ic code
//	gmac_write(0x1c02000, Eth_addr + rBUSMODE);					//zjjin:test


	//config the DMA TxDescriptorListAddr and RxDescriptorListAddr register */
//	gmac_write((ETH_BASE_TXDES_PHY & 0xffff) | 0xffff0000, Eth_addr + rTxDescriptorListAddr);
//	gmac_write((ETH_BASE_RXDES_PHY & 0xffff) | 0xffff0000, Eth_addr + rRxDescriptorListAddr);
	gmac_write(ETH_BASE_TXDES_PHY, uiGmacBaseAddr[s_iGmacNum] + rTxDescriptorListAddr);
	gmac_write(ETH_BASE_RXDES_PHY, uiGmacBaseAddr[s_iGmacNum] + rRxDescriptorListAddr);

	tmp= gmac_read(uiGmacBaseAddr[s_iGmacNum] + rRxDescriptorListAddr);

	/* config the DMA OperationMode register */
//	gmac_write(TTC, Eth_addr + rOperationMode);
	gmac_write(TTC | (0x1<<7) | RTC, uiGmacBaseAddr[s_iGmacNum] + rOperationMode);
//	gmac_write(TTC | (0x1<<7) | RTC | TSF, Eth_addr + rOperationMode);
//	gmac_write(TTC | RTC , Eth_addr + rOperationMode);
//	gmac_write(TTC | RTC | TSF, Eth_addr + rOperationMode);

	/* config the GMAC MACConfig register */
	//tmp = SARC|PS|FES|DM|TE|RE;
	//tmp = PS|FES|DM|TE|RE;

//	tmp = DM|TE|RE;		//former
	if(eth_speed == FAST_SPEED)
		tmp = DM|TE|RE|SEPPD_1000M;			//908 must set SEPPD_x
	else
	{
		#ifndef LOW_SPEED
			tmp = DM|TE|RE|SEPPD_100M;			//908 must set SEPPD_x
		#else
			tmp = DM|TE|RE|SEPPD_10M;
		#endif
	}
	#ifdef INTERNAL_LOOP			//internal loop need connect lan,because the tx/rx clock is send by phy.
		tmp |= LM;
	#endif

	#ifdef JUMBO
		tmp |= JE;
		tmp |= JD;						//908 add when JUMBO more than 9000byte, need this
	#endif

	#ifdef JABBER
		tmp |= JD;
		tmp |= WD;
	#endif

	gmac_write(tmp, uiGmacBaseAddr[s_iGmacNum] + rMACConfig);

	gmac_write(PT | RFE | TFE, uiGmacBaseAddr[s_iGmacNum] + rFlowControl);

	gmac_write(0xff, uiGmacBaseAddr[s_iGmacNum] + rStatus);	//clear all interrupt bits

	gmac_cre_def_tx_des((u32)ETH_BASE_TXDES_PHY);
	gmac_cre_def_rx_des((u32)ETH_BASE_RXDES_PHY);

	//show_desc(ETH_BASE_TXDES_PHY,0x200);

	tmp = gmac_read(uiGmacBaseAddr[s_iGmacNum]+rInterruptMask);
	gmac_write(tmp|0x01, uiGmacBaseAddr[s_iGmacNum]+rInterruptMask);

	/* enable TI and RI interrupt mask */
	gmac_enable_irq();

	pt_to_last_edge_rxdesc = (u32 *)ETH_BASE_RXDES_PHY;
	pt_to_last_edge_txdesc = (u32 *)ETH_BASE_TXDES_PHY;

	g_rx_flag = 0;
	g_tx_flag = 0;

	/* start Tx and Rx */
	tmp= gmac_read(uiGmacBaseAddr[s_iGmacNum] + rOperationMode);
	tmp |= (ST | SR);						//start Tx RX
	//tmp |= (ST);							//only start Tx; test for Tx to PC
	//tmp |= (SR);							//only start Rx

	gmac_write(tmp, uiGmacBaseAddr[s_iGmacNum] + rOperationMode);

}

u32 gmac_get_irqstatus(void)						//閼煎倿鈹愰梽鍡氬瘶妞规挳绨遍懠鍌炩攼闂勫棜鍔忛棁鑼跺妿閸椼倛瀵曟す鎾閼寸増鍩�閼煎倿鈹愰梽鍡氬瘯妞规挳妾伴懠鍌炩攼闂勫棜鍔忛棁鑼跺瘯妞规挳妾伴懠鍌炩攼闂勫棜鍔忕喊宀冨壘鐠侯垵瀵曟す鎾閼煎倿鈹愰梽鍡氬妼闂勫棜瀵曟す鎾閼煎倿鈹愰梽鍡氬瘯妞规挳妾伴懠鍌炩攼闂勫棜鍔忛棁鑼跺瘯妞规挳妾�
{
	//return (gmac_read(Eth_addr  + rStatus) & (RI | TI | NORINTE));
	return (gmac_read(Eth_addr  + rStatus));
}

void gmac_clear_irqstatus(u32 uValue)			//閼煎倿鈹愰梽鍡氬瘯妞规挳妾伴懠鍌炩攼闂勫棜瀵曟す鎾閼搭偄鈻岄懠鍌炩攼闂勫棜瀵曟す鎾閼煎倿鈹愰梽鍡氬姀闂囪尪鍔庨崡銈堝瘯妞规挳妾伴懘鐗堝焿
{
	gmac_write(uValue, Eth_addr  + rStatus);
}

void magic_wakeup()
{
	gmac_write(0x00000003,Eth_addr  + rPMTControl);			//remote enable&power down
}

void remote_wakeup_anyunicast()
{
	gmac_write(0x00000205,Eth_addr  + rPMTControl);			//remote enable&power down
}

void remote_wakeup()			//packet:6byte DA + 6byte SA + 2byte len + 128byte 0x55
{
	unsigned short crc16;

	pmtbuf_fill(pmtbuf,0x55,0x0,0x80);						//pmtbuf = {0x55,0x55,0x55...}
	initCrcTable_16();
	crc16 = computeCrc_16(pmtbuf,16);						//crc16 = 0xfef8
	//crc16 = computeCrc_16(pmtbuf,3);
	gmac_write(0x80000000,Eth_addr  + rPMTControl);			//reset fifo ptr
	while(gmac_read(Eth_addr  + rPMTControl)&0x80000000);
	//gmac_write(PMT_ByteMask0,Eth_addr  + rRemoteWakeUp);
	gmac_write(0xffff,Eth_addr  + rRemoteWakeUp);
	gmac_write(0,Eth_addr  + rRemoteWakeUp);
	gmac_write(0,Eth_addr  + rRemoteWakeUp);
	gmac_write(0,Eth_addr  + rRemoteWakeUp);
	gmac_write(PMT_Cmd0,Eth_addr  + rRemoteWakeUp);			//PMT_cmd0 = 0x01
	//gmac_write(0x01010101,Eth_addr  + rRemoteWakeUp);
	gmac_write(PMT_OffSet0,Eth_addr  + rRemoteWakeUp);		//PMT_OffSet0 = 0x30
	gmac_write(crc16,Eth_addr  + rRemoteWakeUp);
	gmac_write(0,Eth_addr  + rRemoteWakeUp);
	gmac_write(0x00000005,Eth_addr  + rPMTControl);			//remote enable&power down
}

void lpi_test()
{
	unsigned tmp;
/*	tmp = gmac_read(Eth_addr + rOperationMode);
	tmp &= ~(OP_MODE_Tx_STOP_START);				//stop DMA Tx
	gmac_write(tmp,Eth_addr  + rOperationMode);
	delay(100000);
*/
#ifdef SPEED_1000M
	/*power down the MAC interface*/
	tmp = gmac_phy_read(0,MII_AT001_PSCR);
	tmp &= ~(1<<3);
	gmac_phy_write(0,MII_AT001_PSCR, tmp);

	/*power down the PHY*/
	tmp = gmac_phy_read(0,MII_BMCR);
	tmp |= (1<<11);
	gmac_phy_write(0, MII_BMCR, tmp);
//	gmac_phy_write(0, MII_BMCR, tmp|BMCR_RESET);
//	while( gmac_phy_read(0,MII_BMCR)& 0x8000 );
#endif

	gmac_write(0x30000,Eth_addr  + rLPIcsr);		//Eth_addr = 0x220000,rLPIcsr = 0x30
//	delay(100000);
//	while( (gmac_read(Eth_addr  + rLPIcsr) << 27) & 0x05 != 1 );
	delay(100000);
	tmp = gmac_read(Eth_addr  + rLPIcsr);
	PSPRINTF("lpi status is %x\r\n",tmp);
	gmac_write(0x00000,Eth_addr  + rLPIcsr);
//	while( (gmac_read(Eth_addr  + rLPIcsr) << 27) & 0x0A != 1 );
	delay(100000);
	tmp = gmac_read(Eth_addr  + rLPIcsr);
	PSPRINTF("lpi status is %x\r\n",tmp);

	/* back up */
#ifdef SPEED_1000M
	tmp = gmac_phy_read(0,MII_AT001_PSCR);
	tmp |= (1<<3);
	gmac_phy_write(0,MII_AT001_PSCR, tmp);

	tmp = gmac_phy_read(0,MII_BMCR);
	tmp &= ~(1<<11);
	gmac_phy_write(0, MII_BMCR, tmp);
//	gmac_phy_write(0, MII_BMCR, tmp|BMCR_RESET);
//	while( gmac_phy_read(0,MII_BMCR)& 0x8000 );
#endif

}
