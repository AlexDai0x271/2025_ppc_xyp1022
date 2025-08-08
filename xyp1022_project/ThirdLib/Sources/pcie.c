/*
 * pcie_deal.c
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: ccore
 */

#include "pcie.h"
#include "e500mc_asm.h"
#include "quicc.h"
#include "gpio.h"
#include "ppctimer.h"

#define w64(address, data) 			*(volatile unsigned long *)(address) = data
#define r64(address) 				*(volatile unsigned long *)(address)

#define w32(address, data) 			*(volatile unsigned int *)(address) = data
#define r32(address) 				*(volatile unsigned int *)(address)

#define w16(address, data) 			*(volatile unsigned short *)(address) = data
#define r16(address) 				*(volatile unsigned short *)(address)

#define w8(address, data) 			*(volatile unsigned char *)(address) = data
#define r8(address) 				*(volatile unsigned char *)(address)

#define PCIE_INDEX	0//0-3
#define PCIE_DATAPATH	1//0:corenet;else:axi
#define PCIE_TYPE	1//0:ep;else:rc

#define PCIE_LANE_X4		0
#define PCIE_LANE_X2		0
#define PCIE_LANE_X1		1
#define PCIE_SPEED_GEN2		0
#define PCIE_SPEED_GEN1		1

#if !PCIE_TYPE
#define PCIE_PF_NUM		2

#define PCIE_SRIOV_EN	0
#if PCIE_SRIOV_EN
#define PCIE_VF_NUM		1
#endif

#define PCIE_MSIX_EN	1//0:msi;1:msix
#if PCIE_MSIX_EN
#define PCIE_MSIX_NUM	8
#endif

#define PCIE_DMA_LOCAL_INT	1
#define PCIE_OUTBOUND_MEM_TEST	0
#define PCIE_HOT_RESET_TEST	0
#define PCIE_BUTTON_RESET_TEST	0
#endif

#if !PCIE_DATAPATH
#define PCIE_CACHE_COHERENCE_TEST	1
#endif

volatile unsigned int pcie_ob_index = 0;
volatile unsigned int pcie_ib_index = 0;

volatile unsigned int hot_reset_count = 0;

volatile unsigned int pcie_rc_msi_int_flag = 0;

void w32_check(volatile unsigned int *addr, unsigned int data)
{
	w32(addr, data);
	while(r32(addr) != data);
}

void pcie_rc_msi_int()
{
	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, 0 << PCIE_MISC_PFUNC_NUM_SHIFT);
	pcie_rc_msi_int_flag = r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0STAT);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0STAT, pcie_rc_msi_int_flag);
}

char check_pcie_clk(void);

void pcie_hotrst_int()
{
	unsigned int val;

	uart_printf_buf("hotreset \n");
	hot_reset_count++;

	while(!check_pcie_clk());
	
	stop_ltssm();
	pcie_button_reset();

	cfg_pcie_header();
	cfg_pcie_portlink();

	start_ltssm();

	val = r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_INT_STATUS);
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_INT_STATUS, val);
}

#if PCIE_DMA_LOCAL_INT
volatile unsigned int pcie_dma_int_flag = 0;
void pcie_dma_int()
{
	unsigned int reg, misc;

	pcie_dma_int_flag = 1;

	misc = r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE);
	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, PCIE_MISC_ACCESS_DMA);
	reg = r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_DMAWRINTRSTAT);
	if(reg)
	{
		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_DMAWRINTRCLR, reg);
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX), (reg & 0x1) ? 0xf94e450a : 0x5652cc98);
	}

	reg = r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_DMARDINTRSTAT);
	if(reg)
	{
		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_DMARDINTRCLR, reg);
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX), (reg & 0x1) ? 0x9d28c7c1 : 0xdb66a956);
	}
	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, misc);
}
#endif

void pcie_rc_perst_init(struct gpio_regs *gpio_m)
{
	gpio_m->dir |= 0xc;
	gpio_m->ctl |= 0xc;
	gpio_m->out |= 0xc;
}

void pcie_rc_perst_assert(struct gpio_regs *gpio_m)
{
	gpio_m->out &= 0xfffffff3;
}

void pcie_rc_perst_deassert(struct gpio_regs *gpio_m)
{
	gpio_m->out |= 0xc;
}

/*
 * This function is to configure iATU outbound address match for physical function
 *
 * base:for local access is dbi base;for remote access is slave base
 * ba:slave space address
 * ta:pcie space address
 * size:translation region size
 * pf:physical function number
 * type:TLP type(cfg or mem)
 */
void cfg_ob_iatu_pf(unsigned long base, unsigned long ba, unsigned long ta, unsigned long size, unsigned int pf, unsigned int type)
{
	w32(base+PCIE_CFG_OUTBOUND_IATUBAL(pcie_ob_index), (unsigned int)ba);
    PSPRINTF("PCIE_CFG_OUTBOUND_IATUBAL 0x%x = [0x%x]\n",\
              base+PCIE_CFG_OUTBOUND_IATUBAL(pcie_ob_index),\
              r32(base+PCIE_CFG_OUTBOUND_IATUBAL(pcie_ob_index)));

	w32(base+PCIE_CFG_OUTBOUND_IATUBAH(pcie_ob_index), (unsigned int)(ba>>32));
    PSPRINTF("PCIE_CFG_OUTBOUND_IATUBAH 0x%x = [0x%x]\n",\
        base+PCIE_CFG_OUTBOUND_IATUBAH(pcie_ob_index),\
        r32(base+PCIE_CFG_OUTBOUND_IATUBAH(pcie_ob_index)));

	w32(base+PCIE_CFG_OUTBOUND_IATULMTADDRL(pcie_ob_index), (unsigned int)(ba+size-1));
    PSPRINTF("PCIE_CFG_OUTBOUND_IATULMTADDRL 0x%x = [0x%x]\n",\
        base+PCIE_CFG_OUTBOUND_IATULMTADDRL(pcie_ob_index),\
        r32(base+PCIE_CFG_OUTBOUND_IATULMTADDRL(pcie_ob_index)));

	//w32(base+PCIE_CFG_OUTBOUND_IATULMTADDRH(pcie_ob_index), (unsigned int)((ba+size-1)>>32));
	w32(base+PCIE_CFG_OUTBOUND_IATUTAL(pcie_ob_index), (unsigned int)ta);
    PSPRINTF("PCIE_CFG_OUTBOUND_IATUTAL 0x%x = [0x%x]\n",\
        base+PCIE_CFG_OUTBOUND_IATUTAL(pcie_ob_index),\
        r32(base+PCIE_CFG_OUTBOUND_IATUTAL(pcie_ob_index)));

    w32(base+PCIE_CFG_OUTBOUND_IATUTAH(pcie_ob_index), (unsigned int)(ta>>32));
    PSPRINTF("PCIE_CFG_OUTBOUND_IATUTAH 0x%x = [0x%x]\n",\
        base+PCIE_CFG_OUTBOUND_IATUTAH(pcie_ob_index),\
        r32(base+PCIE_CFG_OUTBOUND_IATUTAH(pcie_ob_index)));

	w32(base+PCIE_CFG_OUTBOUND_IATUCTRL1(pcie_ob_index), type | (pf << PCIE_IATU_CTRL1_FUNC_NUM_SHIFT));
    PSPRINTF("PCIE_CFG_OUTBOUND_IATUCTRL1 0x%x = [0x%x]\n",\
        base+PCIE_CFG_OUTBOUND_IATUCTRL1(pcie_ob_index),\
        r32(base+PCIE_CFG_OUTBOUND_IATUCTRL1(pcie_ob_index)));

	w32(base+PCIE_CFG_OUTBOUND_IATUCTRL2(pcie_ob_index), PCIE_IATU_REGION_ENABLE);
    PSPRINTF("PCIE_CFG_OUTBOUND_IATUCTRL2 0x%x = [0x%x]\n",\
        base+PCIE_CFG_OUTBOUND_IATUCTRL2(pcie_ob_index),\
        r32(base+PCIE_CFG_OUTBOUND_IATUCTRL2(pcie_ob_index)));

	pcie_ob_index++;
    PSPRINTF("pcie_ob_index  = [%d]\n",pcie_ob_index);

}

/*
 * This function is to configure iATU outbound address match for virtual function
 *
 * base:for local access is dbi base;for remote access is slave base
 * ba:slave space address
 * ta:pcie space address
 * size:translation region size
 * pf:physical function number
 * vf:virtual function number
 */
void cfg_ob_iatu_vf(unsigned long base, unsigned long ba, unsigned long ta, unsigned long size, unsigned int pf, unsigned int vf)
{
	w32(base+PCIE_CFG_OUTBOUND_IATUBAL(pcie_ob_index), (unsigned int)ba);
	w32(base+PCIE_CFG_OUTBOUND_IATUBAH(pcie_ob_index), (unsigned int)(ba>>32));
	w32(base+PCIE_CFG_OUTBOUND_IATULMTADDRL(pcie_ob_index), (unsigned int)(ba+size-1));
	//w32(base+PCIE_CFG_OUTBOUND_IATULMTADDRH(pcie_ob_index), (unsigned int)((ba+size-1)>>32));
	w32(base+PCIE_CFG_OUTBOUND_IATUTAL(pcie_ob_index), (unsigned int)ta);
	w32(base+PCIE_CFG_OUTBOUND_IATUTAH(pcie_ob_index), (unsigned int)(ta>>32));
	w32(base+PCIE_CFG_OUTBOUND_IATUCTRL1(pcie_ob_index), PCIE_IATU_CTRL1_TYPE_MEM | (pf << PCIE_IATU_CTRL1_FUNC_NUM_SHIFT));
	w32(base+PCIE_CFG_OUTBOUND_IATUCTRL3(pcie_ob_index), vf | PCIE_IATU_CTRL3_VF_ACTIVE);
	w32(base+PCIE_CFG_OUTBOUND_IATUCTRL2(pcie_ob_index), PCIE_IATU_REGION_ENABLE);
	pcie_ob_index++;
}

/*
 * This function is to configure iATU inbound address match for physical function
 *
 * base:for local access is dbi base;for remote access is slave base
 * ba:pcie space address
 * ta:local space address
 * size:translation region size
 * pf:physical function number
 */
void cfg_ib_iatu_pf(unsigned long base, unsigned long ba, unsigned long ta, unsigned long size, unsigned int pf)
{
	w32(base+PCIE_CFG_INBOUND_IATUBAL(pcie_ib_index), (unsigned int)ba);
	w32(base+PCIE_CFG_INBOUND_IATUBAH(pcie_ib_index), (unsigned int)(ba>>32));
	w32(base+PCIE_CFG_INBOUND_IATULMTADDRL(pcie_ib_index), (unsigned int)(ba+size-1));
	//w32(base+PCIE_CFG_INBOUND_IATULMTADDRH(pcie_ib_index), (unsigned int)((ba+size-1)>>32));
	w32(base+PCIE_CFG_INBOUND_IATUTAL(pcie_ib_index), (unsigned int)ta);
	w32(base+PCIE_CFG_INBOUND_IATUTAH(pcie_ib_index), (unsigned int)(ta>>32));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL1(pcie_ib_index), PCIE_IATU_CTRL1_TYPE_MEM | (pf << PCIE_IATU_CTRL1_FUNC_NUM_SHIFT));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL2(pcie_ib_index), PCIE_IATU_REGION_ENABLE | PCIE_IATU_FUNC_NUM_MATCH_EN);
	pcie_ib_index++;
}

/*
 * This function is to configure iATU inbound address match for virtual function
 *
 * base:for local access is dbi base;for remote access is slave base
 * ba:pcie space address
 * ta:local space address
 * size:translation region size
 * pf:physical function number
 * vf:virtual function number
 */
void cfg_ib_iatu_vf(unsigned long base, unsigned long ba, unsigned long ta, unsigned long size, unsigned int pf, unsigned int vf)
{
	w32(base+PCIE_CFG_INBOUND_IATUBAL(pcie_ib_index), (unsigned int)ba);
	w32(base+PCIE_CFG_INBOUND_IATUBAH(pcie_ib_index), (unsigned int)(ba>>32));
	w32(base+PCIE_CFG_INBOUND_IATULMTADDRL(pcie_ib_index), (unsigned int)(ba+size-1));
	//w32(base+PCIE_CFG_INBOUND_IATULMTADDRH(pcie_ib_index), (unsigned int)((ba+size-1)>>32));
	w32(base+PCIE_CFG_INBOUND_IATUTAL(pcie_ib_index), (unsigned int)ta);
	w32(base+PCIE_CFG_INBOUND_IATUTAH(pcie_ib_index), (unsigned int)(ta>>32));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL1(pcie_ib_index), PCIE_IATU_CTRL1_TYPE_MEM | (pf << PCIE_IATU_CTRL1_FUNC_NUM_SHIFT));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL3(pcie_ib_index), vf);
	w32(base+PCIE_CFG_INBOUND_IATUCTRL2(pcie_ib_index), PCIE_IATU_REGION_ENABLE | PCIE_IATU_VF_MATCH_EN | PCIE_IATU_FUNC_NUM_MATCH_EN);
	pcie_ib_index++;
}

/*
 * This function is to configure iATU inbound bar match for physical function
 *
 * base:for local access is dbi base;for remote access is slave base
 * ta:local space address
 * size:translation region size
 * pf:physical function number
 * bar:bar number
 */
void cfg_ib_bar_iatu_pf(unsigned long base, unsigned long ta, unsigned long size, unsigned int pf, unsigned int bar)
{
	w32(base+PCIE_CFG_INBOUND_IATUBAL(pcie_ib_index), 0);
	w32(base+PCIE_CFG_INBOUND_IATUBAH(pcie_ib_index), 0);
	w32(base+PCIE_CFG_INBOUND_IATULMTADDRL(pcie_ib_index), (unsigned int)(size-1));
	//w32(base+PCIE_CFG_INBOUND_IATULMTADDRH(pcie_ib_index), (unsigned int)((size-1)>>32));
	w32(base+PCIE_CFG_INBOUND_IATUTAL(pcie_ib_index), (unsigned int)ta);
	w32(base+PCIE_CFG_INBOUND_IATUTAH(pcie_ib_index), (unsigned int)(ta>>32));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL1(pcie_ib_index), PCIE_IATU_CTRL1_TYPE_MEM | (pf << PCIE_IATU_CTRL1_FUNC_NUM_SHIFT));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL2(pcie_ib_index), PCIE_IATU_REGION_ENABLE | PCIE_IATU_FUNC_NUM_MATCH_EN | PCIE_IATU_BAR_MATCH_EN | (bar << PCIE_IATU_CTRL2_BAR_NUM_SHIFT));
	pcie_ib_index++;
}

/*
 * This function is to configure iATU inbound bar match for virtual function
 *
 * base:for local access is dbi base;for remote access is slave base
 * ta:local space address
 * size:translation region size
 * pf:physical function number
 * vf:virtual function number
 * bar:bar number
 */
void cfg_ib_bar_iatu_vf(unsigned long base, unsigned long ta, unsigned long size, unsigned int pf, unsigned int vf, unsigned int bar)
{
	w32(base+PCIE_CFG_INBOUND_IATUBAL(pcie_ib_index), 0);
	w32(base+PCIE_CFG_INBOUND_IATUBAH(pcie_ib_index), 0);
	w32(base+PCIE_CFG_INBOUND_IATULMTADDRL(pcie_ib_index), (unsigned int)(size-1));
	//w32(base+PCIE_CFG_INBOUND_IATULMTADDRH(pcie_ib_index), (unsigned int)((size-1)>>32));
	w32(base+PCIE_CFG_INBOUND_IATUTAL(pcie_ib_index), (unsigned int)ta);
	w32(base+PCIE_CFG_INBOUND_IATUTAH(pcie_ib_index), (unsigned int)(ta>>32));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL1(pcie_ib_index), PCIE_IATU_CTRL1_TYPE_MEM | (pf << PCIE_IATU_CTRL1_FUNC_NUM_SHIFT));
	w32(base+PCIE_CFG_INBOUND_IATUCTRL3(pcie_ib_index), vf);
	w32(base+PCIE_CFG_INBOUND_IATUCTRL2(pcie_ib_index), PCIE_IATU_REGION_ENABLE | PCIE_IATU_VF_MATCH_EN | PCIE_IATU_FUNC_NUM_MATCH_EN | PCIE_IATU_BAR_MATCH_EN | (bar << PCIE_IATU_CTRL2_BAR_NUM_SHIFT));
	pcie_ib_index++;
}

/*
 * This function is to drive one DMA write channel for physical function
 *
 * base:for local access is dbi base;for remote access is slave base
 * sar:local memory physical address
 * dar:remote pcie address
 * len:byte length to transfer
 * pf:physical function number
 * ch:dma channel index
 */
void cfg_wr_dma_pf(unsigned long base, unsigned long long sar, unsigned long long dar, unsigned int len, unsigned int pf, unsigned int ch)
{
	unsigned int val;

	w32(base+PCIE_CFG_DMAWREN, PCIE_DMA_ENGINE_DISABLE);
	do{
		val = r32(base+PCIE_CFG_DMAWREN);
	}while(val != PCIE_DMA_ENGINE_DISABLE);
	w32(base+PCIE_CFG_DMAWREN, PCIE_DMA_ENGINE_ENABLE);

	w32(base+PCIE_CFG_DMAWRINTRMASK, PCIE_DMA_DONE_BITMASK | PCIE_DMA_ABORT_BITMASK);

	w32(base+PCIE_CFG_DMAWRCHCTRL1(ch), PCIE_DMA_LOCAL_INT_ENABLE | (pf<<PCIE_DMA_FUNC_NUM_SHIFT));
	w32(base+PCIE_CFG_DMAWRCHTRSFRSZ(ch), len);
	w32(base+PCIE_CFG_DMAWRCHSARL(ch), sar);
	w32(base+PCIE_CFG_DMAWRCHSARH(ch), sar >> 32);
	w32(base+PCIE_CFG_DMAWRCHDARL(ch), dar);
	w32(base+PCIE_CFG_DMAWRCHDARH(ch), dar >> 32);
	w32(base+PCIE_CFG_DMAWRCHCTRL2(ch), 0);

	w32(base+PCIE_CFG_DMAWRDORBEL, PCIE_DMA_DOORBELL(ch));
	while((r32(base+PCIE_CFG_DMAWRINTRSTAT) & PCIE_DMA_DONE_BITMASK) == 0);
	w32(base+PCIE_CFG_DMAWRINTRCLR, PCIE_DMA_DONE_BITMASK);
}

/*
 * This function is to drive one DMA read channel for physical function
 *
 * base:for local access is dbi base;for remote access is slave base
 * sar:remote pcie address
 * dar:local memory physical address
 * len:byte length to transfer
 * pf:physical function number
 * ch:dma channel index
 */
void cfg_rd_dma_pf(unsigned long base, unsigned long long sar, unsigned long long dar, unsigned int len, unsigned int pf, unsigned int ch)
{
	unsigned int val;

	w32(base+PCIE_CFG_DMARDEN, PCIE_DMA_ENGINE_DISABLE);
	do{
		val = r32(base+PCIE_CFG_DMARDEN);
	}while(val != PCIE_DMA_ENGINE_DISABLE);
	w32(base+PCIE_CFG_DMARDEN, PCIE_DMA_ENGINE_ENABLE);

	w32(base+PCIE_CFG_DMARDINTRMASK, PCIE_DMA_DONE_BITMASK | PCIE_DMA_ABORT_BITMASK);

	w32(base+PCIE_CFG_DMARDCHCTRL1(ch), PCIE_DMA_LOCAL_INT_ENABLE | (pf<<PCIE_DMA_FUNC_NUM_SHIFT));
	w32(base+PCIE_CFG_DMARDCHTRSFRSZ(ch), len);
	w32(base+PCIE_CFG_DMARDCHSARL(ch), sar);
	w32(base+PCIE_CFG_DMARDCHSARH(ch), sar >> 32);
	w32(base+PCIE_CFG_DMARDCHDARL(ch), dar);
	w32(base+PCIE_CFG_DMARDCHDARH(ch), dar >> 32);
	w32(base+PCIE_CFG_DMARDCHCTRL2(ch), 0);

	w32(base+PCIE_CFG_DMARDDORBEL, PCIE_DMA_DOORBELL(ch));
	while((r32(base+PCIE_CFG_DMARDINTRSTAT) & PCIE_DMA_DONE_BITMASK) == 0);
	w32(base+PCIE_CFG_DMARDINTRCLR, PCIE_DMA_DONE_BITMASK);
}

void ep_outbound_test()
{
	unsigned int i, val32;
	unsigned short val16;
	unsigned char val8;
	unsigned long val;

	//make sure upstream computer has run test6
	//pcie slave space read&write test
	do{
		val = r64(0xe0070000);
	}while(val != 0x1234567887654321);
	for(i=0; i<0x10000; i++)
	{
		w8(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+i, (unsigned char)(i&0xff));
	}
	for(i=0; i<0x10000; i++)
	{
		val8 = r8(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+i);
		if(val8 != (unsigned char)(i&0xff))
			while(1);
	}

	for(i=0; i<0x10000; i+=2)
	{
		w16(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+i, (unsigned short)(i&0xffff));
	}
	for(i=0; i<0x10000; i+=2)
	{
		val16 = r16(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+i);
		if(val16 != (unsigned short)(i&0xffff))
			while(1);
	}

	for(i=0; i<0x10000; i+=4)
	{
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+i, 0xffffffff-i);
	}
	for(i=0; i<0x10000; i+=4)
	{
		val32 = r32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+i);
		if(val32 != 0xffffffff-i)
			while(1);
	}

	//issue msix interrupt
#if PCIE_MSIX_EN
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_ADDR_LOW, swap_word(r32(0xe0040000)));
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_ADDR_HIGH, swap_word(r32(0xe0040004)));
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_DATA, swap_word(r32(0xe0040008)));
#endif
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSI_CTL, PCIE_MISC_INTR_REQ);
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSI_CTL, 0);
}

void pcie_cache_coherence_test()
{
	unsigned long val, bufad, i;
	unsigned int pf;

	do{
		val = r64(0xe0070000);
	}while(val != 0x5a5a5a5a5a5a5a5a);

	bufad = r64(0xe0070008);
	pf = r32(0xe0070010);

	for(i=0; i<0x10000; i+=8)
	{
		w64(0xe0060000+i, 0x123456789abcdef0+i);
	}

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, PCIE_MISC_ACCESS_DMA);
	cfg_wr_dma_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), (unsigned long long)0x4e0060000, bufad, 0x10000, pf, 0);
	cfg_rd_dma_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), bufad, (unsigned long long)0x4e0070000, 0x10000, pf, 1);

	for(i=0; i<0x10000; i+=8)
	{
		val = r64(0xe0070000+i);
		if(val != 0x123456789abcdef0+i)
		{
			while(1);
		}
	}

	//issue msix interrupt
#if PCIE_MSIX_EN
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_ADDR_LOW, swap_word(r32(0xe0040000+pf*PCIE_EP_MSIX_TABLE_SIZE)));
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_ADDR_HIGH, swap_word(r32(0xe0040004+pf*PCIE_EP_MSIX_TABLE_SIZE)));
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_DATA, swap_word(r32(0xe0040008+pf*PCIE_EP_MSIX_TABLE_SIZE)));
#endif
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSI_CTL, PCIE_MISC_INTR_REQ<<pf);
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSI_CTL, 0);

	do{
		val = r64(0xe0070000);
	}while(val != 0x5a5a5a5a5a5a5a5a);

	bufad = r64(0xe0070008);
	pf = r32(0xe0070010);

	for(i=0; i<0x100000; i+=8)
	{
		w64(0+i, 0x9abcdef012345678+i);
	}

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, PCIE_MISC_ACCESS_DMA);
	cfg_wr_dma_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), (unsigned long long)0, bufad, 0x100000, pf, 2);
	cfg_rd_dma_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), bufad, (unsigned long long)0x100000, 0x100000, pf, 3);

	for(i=0; i<0x100000; i+=8)
	{
		val = r64(0x100000+i);
		if(val != 0x9abcdef012345678+i)
		{
			while(1);
		}
	}

	//issue msix interrupt
#if PCIE_MSIX_EN
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_ADDR_LOW, swap_word(r32(0xe0040000+pf*PCIE_EP_MSIX_TABLE_SIZE)));
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_ADDR_HIGH, swap_word(r32(0xe0040004+pf*PCIE_EP_MSIX_TABLE_SIZE)));
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSIX_DATA, swap_word(r32(0xe0040008+pf*PCIE_EP_MSIX_TABLE_SIZE)));
#endif
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSI_CTL, PCIE_MISC_INTR_REQ<<pf);
	w32(DW_PCIE_MISCE_BASE_ADDR(0)+PCIE_MISCE_MSI_CTL, 0);

	w64(0xe0070000, 0xa5a5a5a5a5a5a5a5);
}

char check_pcie_clk()
{
	return (r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_CLK_FLAG) & 1);
}

char check_pcie_clk_idx(unsigned int index)
{
	return (r32(DW_PCIE_MISCE_BASE_ADDR(index) + PCIE_MISC_CLK_FLAG) & 1);
}

void cfg_pcie_type()
{
	unsigned int val = CCM_PCIE_TYPE;
#if PCIE_TYPE
	CCM_PCIE_TYPE = ~(0xf << (PCIE_INDEX*4)) & val | (4 << (PCIE_INDEX*4));   //RC mode
#else
	CCM_PCIE_TYPE = ~(0xf << (PCIE_INDEX*4)) & val | (0 << (PCIE_INDEX*4));   //EP mode
#endif
}

volatile unsigned int *ltssm_buffer = 0xe0070000;
char check_pcie_link()
{
	unsigned int val;

	val = r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_LINK_STATUS);
	if(val != *ltssm_buffer && ltssm_buffer != 0xe007fffc)
	{
		ltssm_buffer++;
		*ltssm_buffer = val;
	}
#if PCIE_SPEED_GEN1
	if((val & 0xf3f0000) == 0x3110000)
#elif PCIE_SPEED_GEN2
	if((val & 0xf3f0000) == 0x7110000)
#else
	if((val & 0xf3f0000) == 0xb110000)
#endif
		return 1;
	else
		return 0;
}

char check_pcie_link_idx(unsigned int index)
{
	unsigned int val;
	
	val = r32(DW_PCIE_MISCE_BASE_ADDR(index) + PCIE_MISC_LINK_STATUS);
#if PCIE_SPEED_GEN1
	if((val & 0xf3f0000) == 0x3110000)
#elif PCIE_SPEED_GEN2
	if((val & 0xf3f0000) == 0x7110000)	
#else
	if((val & 0xf3f0000) == 0xb110000)	
#endif
		return 1;
	else
		return 0;
}

char check_pcie_link_stable_idx(unsigned int index)
{
	unsigned int val, count;
	
	count = 0;
	
	do{
		val = r32(DW_PCIE_MISCE_BASE_ADDR(index) + PCIE_MISC_LINK_STATUS);
#if PCIE_SPEED_GEN1
		if((val & 0xf3f0000) == 0x3110000)
#elif PCIE_SPEED_GEN2
		if((val & 0xf3f0000) == 0x7110000)	
#else
		if((val & 0xf3f0000) == 0xb110000)	
#endif
			count++;
		else
			break;
	}while(count != 100);
	
	if(count == 100)
		return 1;
	else
		return 0;
}

void monitor_pcie_link()
{
	unsigned int val;
	
	val = r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_LINK_STATUS);
	
#if PCIE_SPEED_GEN1
	if((val & 0xf3f0000) == 0x3110000)
#elif PCIE_SPEED_GEN2
	if((val & 0xf3f0000) == 0x7110000)	
#else
	if((val & 0xf3f0000) == 0xb110000)	
#endif
	{
		
	}else{
		PSPRINTF("ltssm->0x%x\r\n", val);
	}
}

void monitor_pcie_link_idx(unsigned int index)
{
	unsigned int val;
	
	val = r32(DW_PCIE_MISCE_BASE_ADDR(index) + PCIE_MISC_LINK_STATUS);
	
#if PCIE_SPEED_GEN1
	if((val & 0xf3f0000) == 0x3110000)
#elif PCIE_SPEED_GEN2
	if((val & 0xf3f0000) == 0x7110000)	
#else
	if((val & 0xf3f0000) == 0xb110000)	
#endif
	{
		
	}else{
		PSPRINTF("pcie%d:ltssm->0x%x\r\n", index, val);
	}
}

char check_pcie_enum()
{
	if(r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR0) != 0)
		return 1;
	else
	{
		unsigned int val = r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_LINK_STATUS);
		if(val != *ltssm_buffer && ltssm_buffer != 0xe007fffc)
		{
			ltssm_buffer++;
			*ltssm_buffer = val;
		}
		return 0;
	}
}

#if PCIE_MSIX_EN
void pcie_ep_msix_init()
{
	unsigned int pf;

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, PCIE_MISC_ACCESS_IATU);
	for(pf=0; pf<PCIE_PF_NUM; pf++)
	{
		cfg_ib_bar_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), PCIE_EP_MSIX_TABLE_PHYSICAL_ADDR+pf*PCIE_EP_MSIX_TABLE_SIZE, PCIE_EP_MSIX_TABLE_SIZE, pf, PCIE_EP_MSIX_BAR_NUMBER);
#if PCIE_SRIOV_EN

		unsigned int vf;
		for(vf=0; vf<PCIE_VF_NUM; vf++)
		{
			cfg_ib_bar_iatu_vf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), PCIE_EP_MSIX_TABLE_PHYSICAL_ADDR+(PCIE_PF_NUM+pf*PCIE_VF_NUM+vf)*PCIE_EP_MSIX_TABLE_SIZE, PCIE_EP_MSIX_TABLE_SIZE, pf, vf, PCIE_EP_MSIX_BAR_NUMBER);
		}

#endif
	}
/*
	for(pf=0; pf<PCIE_PF_NUM; pf++)
	{
		w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, pf << PCIE_MISC_PFUNC_NUM_SHIFT);

		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXCAP, PCIE_MSIX_ENABLE | PCIE_MSIX_TABLE_SIZE(PCIE_MSIX_NUM-1));
		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXTAOFF, PCIE_MSIX_BIR(PCIE_EP_MSIX_BAR_NUMBER));
		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXPBAOFF, PCIE_MSIX_PBA_OFFSET(PCIE_EP_MSIX_TABLE_SIZE/2) | PCIE_MSIX_PBA_BIR(PCIE_EP_MSIX_BAR_NUMBER));
#if PCIE_SRIOV_EN
		w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, (pf << PCIE_MISC_PFUNC_NUM_SHIFT) | PCIE_MISC_ACCESS_CS2);

		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXCAP, PCIE_MSIX_ENABLE | PCIE_MSIX_TABLE_SIZE(PCIE_MSIX_NUM-1));
		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXTAOFF, PCIE_MSIX_BIR(PCIE_EP_MSIX_BAR_NUMBER));
		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXPBAOFF, PCIE_MSIX_PBA_OFFSET(PCIE_EP_MSIX_TABLE_SIZE/2) | PCIE_MSIX_PBA_BIR(PCIE_EP_MSIX_BAR_NUMBER));
#endif
	}
*/
}
#endif

#if !PCIE_TYPE
//prepare iatu space and dma space to RC
//use bar2
void pcie_view_inbound_pl()
{
	unsigned int pf;

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, PCIE_MISC_ACCESS_IATU);
	for(pf=0; pf<PCIE_PF_NUM; pf++)
	{
		cfg_ib_bar_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), DW_PCIE_DBI_PHYSICAL_ADDR(PCIE_INDEX), 0x1000000, pf, 0x2);

#if PCIE_SRIOV_EN
		unsigned int vf;
		for(vf=0; vf<PCIE_VF_NUM; vf++)
		{
			cfg_ib_bar_iatu_vf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), DW_PCIE_DBI_PHYSICAL_ADDR(PCIE_INDEX), 0x1000000, pf, vf, 0x2);
		}
#endif

	}
}
#endif

void cfg_pcie_phy()
{
#if 1
	//for speed switch
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+239*4, 0);
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+240*4, 0);
#endif
#if 0
#if 0
	//for hcsl
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4) | (3<<29) | (1<<26));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4) | (3<<29) | (1<<26));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4) | (1<<26));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4) | (3<<29) | (1<<26));
#else
	//for lvds
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4) | (3<<29));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4) | (3<<29));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4) | (1<<29));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4) | (1<<29));	
#endif
	
#else
#if 0
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4) | (1<<26));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4) | (1<<26));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4) | (1<<26));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4) | (1<<26));	
#endif
#if 0//force pll lock
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4) | (1<<7));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4) | (1<<7));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4) | (1<<7));
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4) | (1<<7));	
#endif
#endif
#if 0
	unsigned int presetnum = *(volatile unsigned int *)0xf8000004;
	//unsigned int presetnum = 0;
	PSPRINTF("use preset%d\r\n", presetnum);
	
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+18*4, 0);//disable auto equalization
	//cfg de-emphasis
	//w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0xcccccccc);
	//w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0);
	switch(presetnum) {

	case 0:
	//3-P0 //TX De-emphasis/Pre-shoot: -6:0 default

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0xcccccccc);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0);
		break;
	case 1:
		//4 //TX De-emphasis/Pre-shoot: 0:3.5

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0xcccccccc);
		break;
	case 2:
		//5 //TX De-emphasis/Pre-shoot: -3.5:3.5

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0x66666666);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0x66666666);
		break;

	case 3:
		//6 //TX De-emphasis/Pre-shoot: -6:3.5
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0x99999999);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0x55555555);
		break;
	case 4:
		//7 //TX De-emphasis/Pre-shoot: 0:2

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0x55555555);
		break;

	case 5:
		//8 //TX De-emphasis/Pre-shoot: 0:2.5

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0x66666666);
		break;
	case 6:
		//2 //TX De-emphasis/Pre-shoot: -3.5:0

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0x88888888);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0);
		break;
	case 7:
		//10 //TX De-emphasis/Pre-shoot: -4.6:0

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0xaaaaaaaa);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0);
		break;
	case 8:
		//9 //TX De-emphasis/Pre-shoot: -2.5:0

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0x66666666);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0);
		break;
	case 9:
		//1 //TX De-emphasis/Pre-shoot: 0:0
		//phy_tx_cburst_adj = 0x492492;
		//phy_tx_amp = 		0xffffff;
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0);
		break;
	default:
		//4 //TX De-emphasis/Pre-shoot: 0:3.5

		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2cc, 0xcccccccc);
		w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+0x2d4, 0);
		break;
	}
#endif
#if 1
	//for pll lock
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+40*4) | (1<<18) | 0x40);
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+41*4) | (1<<18) | 0x40);
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+42*4) | (1<<18) | 0x40);
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4, r32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX)+0x400+43*4) | (1<<18) | 0x40);
#endif
}

void cfg_pcie_phy_idx(unsigned int index)
{
	
#if 1
	//for speed switch
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+239*4, 0);
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+240*4, 0);
#endif
	
#if 1
	//for pll lock
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+40*4, r32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+40*4) | (1<<18) | 0x40);
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+41*4, r32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+41*4) | (1<<18) | 0x40);
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+42*4, r32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+42*4) | (1<<18) | 0x40);
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+43*4, r32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+43*4) | (1<<18) | 0x40);
#endif
	
#if 0
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+18*4, 0);//disable auto equalization
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+0x2cc, 0);
	w32(DW_PCIE_MISCE_BASE_ADDR(index)+0x400+0x2d4, 0xcccccccc);
#endif
}

void cfg_pcie_header()
{
#if !PCIE_TYPE
	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, 0 << PCIE_MISC_PFUNC_NUM_SHIFT);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_VENID, 0x10229100);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_REVID, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_REVID) & 0xffffff | 0x05000000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffff3ff);
#endif

#if PCIE_LANE_X4
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffc0f | 0x40);
#elif PCIE_LANE_X2
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffc0f | 0x20);
#elif PCIE_LANE_X1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffc0f | 0x10);
#endif

#if PCIE_SPEED_GEN2
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffff0 | 0x2);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2) & 0xffffff00 | 0x6);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2) & 0xfffffff0 | 0x2);
#elif PCIE_SPEED_GEN1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffff0 | 0x1);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2) & 0xffffff00 | 0x2);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2) & 0xfffffff0 | 0x1);
#endif

#if PCIE_MSIX_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXCAP, PCIE_MSIX_TABLE_SIZE(PCIE_MSIX_NUM-1));
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXTAOFF, PCIE_MSIX_BIR(PCIE_EP_MSIX_BAR_NUMBER));
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXPBAOFF, PCIE_MSIX_PBA_OFFSET(PCIE_EP_MSIX_TABLE_SIZE/2) | PCIE_MSIX_PBA_BIR(PCIE_EP_MSIX_BAR_NUMBER));
#endif

#if !PCIE_TYPE
	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, 1 << PCIE_MISC_PFUNC_NUM_SHIFT);

	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_VENID, 0x10229100);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_REVID, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_REVID) & 0xffffff | 0x05000000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffff3ff);

#if PCIE_LANE_X4
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffc0f | 0x40);
#elif PCIE_LANE_X2
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffc0f | 0x20);
#elif PCIE_LANE_X1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffc0f | 0x10);
#endif

#if PCIE_SPEED_GEN2
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffff0 | 0x2);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2) & 0xffffff00 | 0x6);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2) & 0xfffffff0 | 0x2);
#elif PCIE_SPEED_GEN1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP) & 0xfffffff0 | 0x1);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCAP2) & 0xffffff00 | 0x2);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_ECLKCTRL2) & 0xfffffff0 | 0x1);
#endif

#if PCIE_MSIX_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXCAP, PCIE_MSIX_TABLE_SIZE(PCIE_MSIX_NUM-1));
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXTAOFF, PCIE_MSIX_BIR(PCIE_EP_MSIX_BAR_NUMBER));
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIXPBAOFF, PCIE_MSIX_PBA_OFFSET(PCIE_EP_MSIX_TABLE_SIZE/2) | PCIE_MSIX_PBA_BIR(PCIE_EP_MSIX_BAR_NUMBER));
#endif

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, (0 << PCIE_MISC_PFUNC_NUM_SHIFT) | PCIE_MISC_ACCESS_CS2);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR2, 0xffffff);
#if PCIE_MSIX_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR3, 0xfff);
#endif
//	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR5, 0x7ffffff);//set bar5 128M
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR5, 0x0);//set bar5 128M
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_EROMBA0, 0);

#if PCIE_SRIOV_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_SRIOVBAR2REG, 0xffffff);
#if PCIE_MSIX_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_SRIOVBAR3REG, 0xfff);
#endif
#endif

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, (1 << PCIE_MISC_PFUNC_NUM_SHIFT) | PCIE_MISC_ACCESS_CS2);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR2, 0xffffff);
#if PCIE_MSIX_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR3, 0xfff);
#endif
	//w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR5, 0x7ffffff);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR5, 0x0);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_EROMBA0, 0);

#if PCIE_SRIOV_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_SRIOVBAR2REG, 0xffffff);
#if PCIE_MSIX_EN
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_SRIOVBAR3REG, 0xfff);
#endif
#endif
#endif
}

void cfg_pcie_portlink()
{
#if PCIE_LANE_X4
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL) & 0xffc0ffff) | 0x70000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) & 0xffffe0ff) | 0x400);
#elif PCIE_LANE_X2
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL) & 0xffc0ffff) | 0x30000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) & 0xffffe0ff) | 0x200);
#elif PCIE_LANE_X1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL) & 0xffc0ffff) | 0x10000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) & 0xffffe0ff) | 0x100);
#endif

#if PCIE_SPEED_GEN1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) & 0xfffdffff);
#else
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) | 0x20000);
#endif

	//w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL) | 0x30);

	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL) & 0xff0000ff | 0x30);//disable auto equalization
	//w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL) | (0x100<<5));
#if 0
	/*
	 * p5/p6 is ok,p3/p8 is second choice
	 */
	unsigned int preset = *(volatile unsigned int *)0xf8000000 % 11;
	//PSPRINTF("request preset%d\r\n", preset);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL) | (0x100<<preset));
#endif	
#if !PCIE_DATAPATH
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_COHERENCY_CTRL3, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_COHERENCY_CTRL3) | PCIE_COHERENCY_AWCACHE_MODE_FIELD | PCIE_COHERENCY_ARCACHE_MODE_FIELD);
#endif
}

void cfg_pcie_portlink_idx(unsigned int index)
{
#if PCIE_LANE_X4
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL) & 0xffc0ffff) | 0x70000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) & 0xffffe0ff) | 0x400);
#elif PCIE_LANE_X2
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL) & 0xffc0ffff) | 0x30000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) & 0xffffe0ff) | 0x200);	
#elif PCIE_LANE_X1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_LKCTRL) & 0xffc0ffff) | 0x10000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL, (r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN2CTRL) & 0xffffe0ff) | 0x100);
#endif

#if PCIE_SPEED_GEN1
	w32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN2CTRL, r32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN2CTRL) & 0xfffdffff);
#else
	w32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN2CTRL, r32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN2CTRL) | 0x20000);
#endif
	
	//w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL) | 0x30);
	w32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN3_EQ_CTRL) & 0xff0000ff | 0x30);//disable auto equalization
	//w32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_GEN3_EQ_CTRL) | (0x100<<5));
#if 0
	/*
	 * p5/p6 is ok,p3/p8 is second choice
	 */
	unsigned int preset = *(volatile unsigned int *)0xf8000000 % 11;
	//PSPRINTF("request preset%d\r\n", preset);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL, r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_GEN3_EQ_CTRL) | (0x100<<preset));
#endif	
#if !PCIE_DATAPATH
	w32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_COHERENCY_CTRL3, r32(DW_PCIE_DBI_BASE_ADDR(index) + PCIE_CFG_COHERENCY_CTRL3) | PCIE_COHERENCY_AWCACHE_MODE_FIELD | PCIE_COHERENCY_ARCACHE_MODE_FIELD);
#endif
}

void start_ltssm()
{
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX), 1);
}

void start_ltssm_idx(unsigned int index)
{
	w32(DW_PCIE_MISCE_BASE_ADDR(index), 1);
}

void stop_ltssm()
{
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX), 0);
}

void pcie_button_reset()
{
	CPR_PCIE_BUTTON_RESET |= (0x2<<PCIE_INDEX);
	while((CPR_PCIE_BUTTON_RESET & (0x2<<PCIE_INDEX)) == 0);
	CPR_PCIE_BUTTON_RESET &= ~(0x2<<PCIE_INDEX);
}

void pcie_button_reset_idx(unsigned int index)
{
	CPR_PCIE_BUTTON_RESET |= (0x2<<index);
	while((CPR_PCIE_BUTTON_RESET & (0x2<<index)) == 0);
	CPR_PCIE_BUTTON_RESET &= ~(0x2<<index);
}

void pcie_button_reset_test()
{
	pcie_button_reset();
	cfg_pcie_header();

	start_ltssm();
}

#if !PCIE_TYPE
void pcie_ep_test()
{
#if PCIE_DMA_LOCAL_INT
	InstallInterruptHandler0(PCIE_EDMA_INT_NUM(PCIE_INDEX), pcie_dma_int);
	w32(0xe8050200+PCIE_EDMA_INT_NUM(PCIE_INDEX)*0x20, r32(0xe8050200+PCIE_EDMA_INT_NUM(PCIE_INDEX)*0x20) & 0x7fffffff);//enable mpic vector
#endif

#if PCIE_MSIX_EN
	pcie_ep_msix_init();
#endif

	pcie_view_inbound_pl();

#if PCIE_OUTBOUND_MEM_TEST
	ep_outbound_test();
#endif

#if !PCIE_DATAPATH
#if PCIE_CACHE_COHERENCE_TEST
	pcie_cache_coherence_test();
#endif
#endif

#if PCIE_HOT_RESET_TEST
	while(1)
	{
		unsigned int val;
		val = r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX) + PCIE_CFG_BAR2);
	}
#endif

#if PCIE_BUTTON_RESET_TEST
	pcie_button_reset_test();
#endif

	while(1){
		monitor_pcie_link();	
	}
}
#endif

void pcie_rc_cfg_region_setup()
{
	//rc outbound cfg0 tlp setup
	cfg_ob_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), PCIE_SLAVE_PHYSICAL_ADDR(PCIE_INDEX), 0, 0x1000000, 0, PCIE_IATU_CTRL1_TYPE_CFG0);

	//rc outbound cfg1 tlp setup
	cfg_ob_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), PCIE_SLAVE_PHYSICAL_ADDR(PCIE_INDEX)+0x1000000, 0x1000000, 0x7000000, 0, PCIE_IATU_CTRL1_TYPE_CFG1);
}

void pcie_rc_mem_region_setup()
{
	//rc outbound mem tlp setup
	cfg_ob_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), PCIE_SLAVE_PHYSICAL_ADDR(PCIE_INDEX)+0x8000000, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX), 0x8000000, 0, PCIE_IATU_CTRL1_TYPE_MEM);

	//rc inbound mem tlp setup
//	cfg_ib_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), 0xe0000000, 0x4e0000000ull, 0x80000, 0);
	cfg_ib_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), 0, 0, 0x80000000, 0);
/*
	//rc inbound mem tlp setup
	cfg_ib_iatu_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), 0, 0, 0x40000000, 0);
*/
}

void pcie_rc_setup()
{
	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, PCIE_MISC_ACCESS_IATU);
	pcie_rc_cfg_region_setup();
	pcie_rc_mem_region_setup();

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, 0 << PCIE_MISC_PFUNC_NUM_SHIFT);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_RC_CFG_CMD, 0x00100107);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_RC_CFG_PBN, 0x000f0100);
}

void ccp907t_driver()
{
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_CMD, 0x7);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR0, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX));
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR1, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x1000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR2, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x2000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR3, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x3000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR4, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x4000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR5, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x5000000);

	//ep inbound mem tlp setup
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUIDX					, PCIE_IATU_REGION_INBOUND);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAL					, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX));
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAH					, 0);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATULMTADDR				, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x7ffff);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAL					, 0xe0300000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAH					, 0x80000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL1				, PCIE_IATU_CTRL1_TYPE_MEM);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL2				, PCIE_IATU_REGION_ENABLE);
	
	//ep inbound mem tlp setup
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUIDX					, PCIE_IATU_REGION_INBOUND+1);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAL					, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x1000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAH					, 0);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATULMTADDR				, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x100ffff);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAL					, 0xe0210000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAH					, 0x80000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL1				, PCIE_IATU_CTRL1_TYPE_MEM);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL2				, PCIE_IATU_REGION_ENABLE);
}

void raid_driver()
{
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_CMD, 0x7);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR0, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX));
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR1, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x1000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR2, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x2000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR3, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x3000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR4, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x4000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + PCIE_CFG_BAR5, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x5000000);
	
	//ep inbound mem tlp setup
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUIDX					, PCIE_IATU_REGION_INBOUND);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAL					, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX));
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAH					, 0);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATULMTADDR				, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x1ffff);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAL					, 0);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAH					, 0x80000006);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL1				, PCIE_IATU_CTRL1_TYPE_MEM);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL2				, PCIE_IATU_REGION_ENABLE);
	
	//ep inbound mem tlp setup
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUIDX					, PCIE_IATU_REGION_INBOUND+1);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAL					, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x1000000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUBAH					, 0);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATULMTADDR				, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x1000fff);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAL					, 0x224000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUTAH					, 0x80000001);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL1				, PCIE_IATU_CTRL1_TYPE_MEM);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_IATUCTRL2				, PCIE_IATU_REGION_ENABLE);
}

void pcie_rc_dma_test()
{
	unsigned int i, val, seed, count;

	count = 0;
loop:	
	seed = r32(0xe004fffc);

	//srand(seed);

	for(i=0; i<0x400; i++)
	{
		w32(0xe0040000+i*4, seed+i);
		//w32(0xe0020000+i*4, rand());
	}

	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, PCIE_MISC_ACCESS_DMA);
	cfg_wr_dma_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), 0x4e0040000ull, PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX), 0x1000, 0, 0);

	cfg_rd_dma_pf(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX), PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX), 0x4e0050000ull, 0x1000, 0, 0);

	//srand(seed);

	for(i=0; i<0x400; i++)
	{
		val = r32(0xe0050000+i*4);
		if(val != seed+i)
		//if(val != rand())
			//while(1);
		{
			PSPRINTF("[0x%x]:is 0x%x,should be 0x%x\r\n", i*4, val, seed+i);
		}
	}

	PSPRINTF("rc dma test OK[%d]\r\n", count++);
	
	goto loop;
}

void pcie_rc_msi_test()
{
	unsigned int val, msi_data;
	
	InstallInterruptHandler0(PCIE_MSI_CTRL_INT_NUM(PCIE_INDEX), pcie_rc_msi_int);
	w32(0xe8050200+PCIE_MSI_CTRL_INT_NUM(PCIE_INDEX)*0x20, r32(0xe8050200+PCIE_MSI_CTRL_INT_NUM(PCIE_INDEX)*0x20) & 0x7fffffff);//enable mpic vector
	
	w32_check(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_ACCESS_TYPE, 0 << PCIE_MISC_PFUNC_NUM_SHIFT);
	
	// Enable EP MSI & 64-bit address
	val = r32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_MSICAPID);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_MSICAPID, val | (1<<16) | (1<<23));
	// Set EP MSI address
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_MSIADDRL, 0xe0020000);
	w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_MSIADDRH, 0x4);
	// Set RC MSI controller address
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIADDR, 0xe0020000);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIADDRU, 0x4);
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0EN, 0xffffffff);
#if 1
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0MASK, 0);
#else
	w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0MASK, 0xffffffff);
#endif
	
	for(msi_data = 0; msi_data < 0x20; msi_data++)
	{
		// Set EP MSI data
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+PCIE_CFG_MSIDATA, msi_data);

		// EP generate MSI request
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x900e004, 2);

#if 1
		while(pcie_rc_msi_int_flag==0);
		if(pcie_rc_msi_int_flag != (1 << (msi_data & 0x1f)))
			while(1);
		pcie_rc_msi_int_flag = 0;
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x900e004, 0);
		PSPRINTF("msi request:[0x%x] is handled!\r\n", msi_data);
#else
		val = r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0STAT);
		while(val != (1 << (msi_data & 0x1f)))
			val = r32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0STAT);
		w32(DW_PCIE_DBI_BASE_ADDR(PCIE_INDEX)+PCIE_CFG_MSIINT0STAT, val);
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX)+0x900e004, 0);
		PSPRINTF("msi request:[0x%x] is handled!\r\n", msi_data);
#endif
	}
}

void pcie_cfg_write(unsigned int offset, unsigned int len, unsigned int val)
{
	if(len == 4)
		w32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + offset, val);
	else if(len == 2)
		w16(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + offset, val);
	else
		w8(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + offset, val);
}

void pcie_cfg_read(unsigned int offset, unsigned int len, unsigned int *val)
{
	if(len == 4)
		*val = r32(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + offset);
	else if(len == 2)
		*val = r16(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + offset);
	else
		*val = r8(PCIE_SLAVE_VIRTUAL_ADDR(PCIE_INDEX) + offset);
}

unsigned int pcie_cap_find(unsigned int cap_id)
{
	unsigned int cap = 0x34, id;
	pcie_cfg_read(cap, 1, &cap);
	while(1)
	{
		pcie_cfg_read(cap, 1, &id);
		if(id == cap_id)
			break;
		pcie_cfg_read(cap+1, 1, &cap);
	}
	return cap;
}

void pcie_cap_write(unsigned int cap_id, unsigned int off, unsigned int len, unsigned int val)
{
	unsigned int cap = pcie_cap_find(cap_id);
	
	pcie_cfg_write(cap+off, len, val);
}

void pcie_rc_test()
{
	unsigned int pcie_id;


	pcie_cfg_read(PCIE_CFG_VENID, 4, &pcie_id);
	
	PSPRINTF("pcie_cfg_read:the cfg VENDID = [0x%x]\r\n", pcie_id);
	if(pcie_id == 0x00079000)
		ccp907t_driver();
	if(pcie_id == 0x90809000)
		raid_driver();
#if PCIE_RC_DMA_TEST	
	pcie_rc_dma_test();
#endif
	
#if PCIE_RC_MSI_TEST
	pcie_rc_msi_test();
#endif

//	while(1);
}
void pcie_rc_init()
{
#if PCIE_DATAPATH
	CCM_AXI_DATAPATH &= 0xff;
#endif
	cfg_pcie_type();

#if !PCIE_TYPE
	InstallInterruptHandler0(PCIE_MISCE_INT_NUM(PCIE_INDEX), pcie_hotrst_int);
	w32(0xe8050200+PCIE_MISCE_INT_NUM(PCIE_INDEX)*0x20, r32(0xe8050200+PCIE_MISCE_INT_NUM(PCIE_INDEX)*0x20) & 0x7fffffff);//enable mpic vector
	w32(DW_PCIE_MISCE_BASE_ADDR(PCIE_INDEX) + PCIE_MISC_INT_ENABLE, PCIE_MISC_HOTRST_INT_EN);
#endif

	cfg_pcie_phy();
	pcie_button_reset();
	while(!check_pcie_clk());
	uart_printf_buf("PCIE CLK OK. \n");

	cfg_pcie_header();
	cfg_pcie_portlink();

#if PCIE_TYPE
	pcie_rc_perst_init(GPIO_M_BASE_ADDR);
	pcie_rc_perst_assert(GPIO_M_BASE_ADDR);
	delay(10000);
	pcie_rc_perst_deassert(GPIO_M_BASE_ADDR);
#endif
	start_ltssm();

	while(!check_pcie_link());
	uart_printf_buf("PCIE X1 GEN1 Link OK. \n");	
	
	pcie_rc_setup();
}
void pcie_test()
{
	
#if !PCIE_TYPE
	while(!check_pcie_enum());
#endif

#if PCIE_TYPE
	pcie_rc_test();
#else
	pcie_ep_test();
#endif
}

