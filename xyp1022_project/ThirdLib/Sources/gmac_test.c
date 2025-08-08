#include "Eth_emac.h"
//#include "common.h"
//#include "interrupt.h"
//#include "sec_common.h"
//#include "ppclib.h"
#include "e500mc_asm.h"

extern u32 phy_addr;

extern int gmac_phy_read(int phy_reg_unused, int reg);
extern void gmac_phy_write( int phyaddr_unused, int reg, int value);

extern volatile unsigned int g_rx_flag;
extern volatile unsigned int g_tx_flag;

#undef PSPRINTF
#define PSPRINTF

u32 SINGLE_TXDES_SIZE = 16;
u32 SINGLE_RXDES_SIZE = 16;

u32 SINGLE_TXBUF_SIZE = MAX_BUF;
u32 SINGLE_RXBUF_SIZE = MAX_BUF;
u32 SIZE_OF_TXDES;
u32 SIZE_OF_TXBUF;
u32 SIZE_OF_RXDES;
u32 SIZE_OF_RXBUF;

u32 ETH_BASE_TXDES_PHY;
u32 ETH_BASE_TXBUF_PHY;
u32 ETH_BASE_RXDES_PHY;
u32 ETH_BASE_RXBUF_PHY;

unsigned int mac_reg_num[]={0x1000,0x100C,0x1010,0x1014,0x1018,0x101C,0,0x4,0x8,0xc,0x40,0x44};
unsigned int phy_reg_num[]={1,2,3,4,5,6,10,15,16,17,18,19,20,21,22,23,24,25,50,110};

void read_cfg_phyreg()
{
	unsigned int tmp32,i,reg,j;
	j=0;

	gmac_dbg("mac regs are:!!!\r\n");
	for(i=0;i<12;i++)
	{
		reg = mac_reg_num[i];
		tmp32 = readl((Eth_addr+reg));
		gmac_dbg("the mac reg: %x,value is %x!\r\n",reg,tmp32);
	}

	gmac_dbg("mii regs are:!!!\r\n");
	//for(j=0;j<32;j++)
	//j=5;
	{
		//phy_addr = j;
		gmac_dbg("the phy addr is %d\r\n",phy_addr);
#if 0
		for(i=0;i<20;i++)
		{
			reg = phy_reg_num[i];
			tmp32 = gmac_phy_read(Eth_addr,reg);
			gmac_dbg("the phy reg: %d,value is %x!\r\n",reg,tmp32);
		}
#else
		for(i=0;i<32;i++)
		{
			reg = i;
			tmp32 = gmac_phy_read(Eth_addr,reg);
			gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);
		}
#endif

#ifdef DP83867
#ifndef rgmii
		reg = 0x6e;
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);
		reg = 0x2c;
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);
		reg = 0x32;
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);
		tmp32 &= ~0x80;					// change to gmii
		gmac_phy_write_ext(Eth_addr,reg,tmp32);
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);

		reg = 0x10;
		tmp32 = gmac_phy_read(Eth_addr,reg);
		tmp32 &=0x1;			//enable jammber
		gmac_phy_write(Eth_addr,reg,tmp32);
#endif
		reg = 0x6e;
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);
		reg = 0x2c;
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);
		reg = 0x32;
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);

		reg = 0x86;
		tmp32 = gmac_phy_read_ext(Eth_addr,reg);
		gmac_dbg("the phy reg: 0x%x,value is 0x%x!\r\n",reg,tmp32);

		reg = 0x10;
		tmp32 = gmac_phy_read(Eth_addr,reg);
		tmp32 &=0x1;			//enable jammber
		gmac_phy_write(Eth_addr,reg,tmp32);
#endif

	}

}

extern u8 sendbuf[16384];

show_desc(unsigned *pointto,unsigned len)
{
	gmac_dbg("the addr %x value is:\r\n",pointto);
	while(len--)
	{
		gmac_dbg("%x ",*(pointto++));
		if(len%8 == 0)
		gmac_dbg("\r\n");
	}
}

void sendmsg()
{
	u32 msg_len, i, j;
	msg_len = 64;
//	msg_len = 46;		//46+14+4 = 64	= 16*4
//	msg_len = 61;		//61+14+4 = 79 < 16*5
//	msg_len = 400;		//110+14+4 = 128 = 16*8
//	msg_len = 1024-18;
//	msg_len = 1500;
//	msg_len = 2048-14-4;		//the max data limit
//	msg_len = 2049-14-4;		//more than the max data limit, cause CRC error at RDES0 and JT at TDES0
	#ifdef JUMBO
	msg_len = 3000;
//	msg_len = 4000;
//	msg_len = 9000;
//	msg_len = 10240-14-4;		//the max data limit,need open tmp |= JD;
//	msg_len = 10241-14-4;
	#endif
	#ifdef JABBER
		msg_len = 12000-14-4;
//		msg_len = 15024-14-4;
	#endif
/*
		u32 Rx_buffer_addr;
		Rx_buffer_addr = gmac_read(Eth_addr+rCurrHostRxBufferAddr);			//for memcmp test, need read before gmac_start_xmit
		memset(ETH_BASE_TXBUF_PHY, 0, 0x4000);
		memset(ETH_BASE_RXBUF_PHY, 0, 0x7000);
*/
	sendbuf_fill(sendbuf,0,msg_len);

	gmac_start_xmit(sendbuf,msg_len+14);

	dev_dbg("send len %d,buffer data is: \r\n",msg_len+14);

			for(i=0;i<msg_len+14;i++)
			{
				if((i%10)==0)
				{
					dev_dbg("\r\n");
				}
				dev_dbg("0x%x ",sendbuf[i]);
			}
			dev_dbg("\r\n");

//	while(g_tx_flag!=1);
	dev_dbg("msg has been send\r\n");
	g_tx_flag = 0;
}

void revmsg()
{
#if 0
	while(1)
	{
	  if(g_rx_flag == 1)
	  {
		  gmac_rx();
		  g_rx_flag = 0;
	  }
	}
#else
//	while(g_rx_flag == 0);
	gmac_rx();
	g_rx_flag = 0;
#endif
}

extern void gmac_irq_handler();
extern void pmt_irq_handler();
extern void lpi_irq_handler();

#undef GMAC_DS_CFG
#define GMAC_DS_CFG (0xEB009000 + 0x108)

void ext_cfg( int iGmacNum )
{
	unsigned int data_read;
	int i;
	for(i=0;i<16;i++)
	{
		STW(i|(i<<4)|(i<<8)|(i<<12),GMAC_DS_CFG);
		MSYNC;
    }

#ifdef GMII
	STW(0xf,(EMAC_CLK_SEL_CFG_OFFSET | CPR_BASE_ADDR));	//select the 125Mhz clock
	STW(0x0,0xeb009118);				//ccm dpaa ctl
#else
	STW(0x0,(EMAC_CLK_SEL_CFG_OFFSET | CPR_BASE_ADDR));	//select the 125Mhz clock
	STW(0x3,0xeb009118);				//ccm dpaa ctl
#endif
	
	if ( iGmacNum == 1 )
	{
		//dpaa chanel enable standalone mode & phy_sel=001:rgmii & big endian
		#ifdef big_endian
		STW((0x100 | 0x10 | 0x1), 0xea4820a8);
		STW((0x100 | 0x10 | 0x1), 0xea4824a8);
		STW((0x100 | 0x10 | 0x1), 0xea4828a8);
		STW((0x100 | 0x10 | 0x1), 0xea482ca8);
		STW((0x100 | 0x10 | 0x1), 0xea4840a8);
		#else
		/*
		STW((0x000 | 0x10 | 0x1), 0xea4820a8);
		STW((0x000 | 0x10 | 0x1), 0xea4824a8);
		STW((0x000 | 0x10 | 0x1), 0xea4828a8);
		STW((0x000 | 0x10 | 0x1), 0xea482ca8);
		STW((0x000 | 0x10 | 0x1), 0xea4840a8);
		*/
		STW((0x000 | 0x10 | 0x0), 0xea4820a8);
		STW((0x000 | 0x10 | 0x0), 0xea4824a8);
		STW((0x000 | 0x10 | 0x0), 0xea4828a8);
		STW((0x000 | 0x10 | 0x0), 0xea482ca8);
		STW((0x000 | 0x10 | 0x0), 0xea4840a8);

		#endif
	}
	else
	{
		//dpaa chanel enable standalone mode & phy_sel=000:gmii & big endian
		#ifdef big_endian
		STW((0x100 | 0x00 | 0x1), 0xea4820a8);
		STW((0x100 | 0x00 | 0x1), 0xea4824a8);
		STW((0x100 | 0x00 | 0x1), 0xea4828a8);
		STW((0x100 | 0x00 | 0x1), 0xea482ca8);
		STW((0x100 | 0x00 | 0x1), 0xea4840a8);
		#else
		STW((0x00 | 0x00 | 0x0), 0xea4820a8);
		STW((0x00 | 0x00 | 0x0), 0xea4824a8);
		STW((0x00 | 0x00 | 0x0), 0xea4828a8);
		STW((0x00 | 0x00 | 0x0), 0xea482ca8);
		STW((0x00 | 0x00 | 0x0), 0xea4840a8);
		#endif	
	}

	STW(ADDR_START_HIGH, 0xea4820ac);
	STW(ADDR_START_HIGH, 0xea4824ac);
	STW(ADDR_START_HIGH, 0xea4828ac);
	STW(ADDR_START_HIGH, 0xea482cac);
	STW(ADDR_START_HIGH, 0xea4840ac);
}

#define w32(address, data) 			*(volatile unsigned int *)(address) = data
#define r32(address) 				*(volatile unsigned int *)(address)

void regDMAShow(u32 num)
{
    unsigned int i=0,data;
    for(i=0;i< num;i++)
    {
        data = r32(0xEA4D9000 + 4*i);
        dev_dbg("(REG_ADDR + 0x%x) = 0x%x\n\r", (0xEA4D9000 + 4*i),data);
    }
}

void regGMACShow(u32 num)
{
    unsigned int i=0,data;
    for(i=0;i< num;i++)
    {
        data = r32(0xEA4D8000 + 4*i);
        dev_dbg("(REG_ADDR + 0x%x) = 0x%x\n\r", (0xEA4D8000 + 4*i),data);
    }
}

void gmac_init( int iGmacNum )
{
	  init_buffer();

	  //read_cfg_phyreg();
	  dev_dbg("Gmac%d BaseAddr = 0x%x\n\r", iGmacNum, (mac0_cfg_base + 0x4000*3) );
	  ext_cfg( iGmacNum );

	  gmac_disable_irq( iGmacNum );

	#if 0
	  memset(ETH_BASE_TXDES_PHY,0x00,(SIZE_OF_TXDES+SIZE_OF_TXBUF+SIZE_OF_RXDES+SIZE_OF_RXBUF));
	//  show_desc(ETH_BASE_TXDES_PHY,0x200);

	  PSPRINTF("\n*******************ETHMAC test starting now!!!**************************\r\n");

	  InstallInterruptHandler0(120, gmac_irq_handler);
	  w32(0xe8050200+120*0x20, r32(0xe8050200+120*0x20) & 0x7fffffff);//enable mpic vector
	  InstallInterruptHandler0(121, gmac_irq_handler);
	  w32(0xe8050200+121*0x20, r32(0xe8050200+121*0x20) & 0x7fffffff);//enable mpic vector
	  InstallInterruptHandler0(122, gmac_irq_handler);
	  w32(0xe8050200+122*0x20, r32(0xe8050200+122*0x20) & 0x7fffffff);//enable mpic vector
	  InstallInterruptHandler0(123, gmac_irq_handler);
	  w32(0xe8050200+123*0x20, r32(0xe8050200+123*0x20) & 0x7fffffff);//enable mpic vector
	  InstallInterruptHandler0(124, gmac_irq_handler);
	  w32(0xe8050200+124*0x20, r32(0xe8050200+124*0x20) & 0x7fffffff);//enable mpic vector
	#endif

	  init_gmac( iGmacNum );	
}

void gmac_test()

{
  u16 val=0x1100;
  int intval=0, i;
  u16 ret;

//  read_reg();
//  sendmsg();

#ifdef INTERNAL_LOOP
  intval = 1;
#endif
#ifdef EXTERNAL_LOOP
  intval = 1;
#endif

  //while(1)
//  {

#if GMAC0
	  gmac_init(0);
	  
	  for ( i = 0; i < 4; i++ )
	  {
		  sendmsg();
		  delay(0x10000);
		  revmsg();
		  delay(0x10000);
	  }
	  dev_dbg( "\n*******************GMAC0 test ok!!!**************************\r\n");
#endif
  //gmac1-3, loopback

#if GMAC1
	  gmac_init(1);
	  	  
	  for ( i = 0; i < 4; i++ )
	  {
		  sendmsg();
		  delay(0x10000);
		  revmsg();
		  delay(0x10000);
	  }
	  dev_dbg( "\n*******************GMAC1 test ok!!!**************************\r\n");
#endif

#if GMAC2
	sendmsg();
	revmsg();
	dev_dbg( "\n*******************GMAC2 test ok!!!**************************\r\n");
#endif
#if GMAC3
	sendmsg();
	revmsg();
	dev_dbg( "\n*******************GMAC3 test ok!!!**************************\r\n");
#endif

 // }

}
/*
void exit(int exit_code)
{
  while(1);
}
*/
