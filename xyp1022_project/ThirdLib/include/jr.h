/*
 * Copyright 2008-2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */

#ifndef __JR_H
#define __JR_H

#include "sec_common.h"
#include "sec.h"
#include "common.h"

#define CONFIG_PHYS_64BIT

/* We support at most 32 Scatter/Gather Entries.*/
#define MAX_SG_32	32

#define JR_SIZE 4	//need to 2^n
/* Timeout currently defined as 90 sec */
#define CONFIG_SEC_DEQ_TIMEOUT	90000000U

#define DEFAULT_JR_ID		0
#define DEFAULT_JR_LIODN	0
#define DEFAULT_IRQ		0	/* Interrupts not to be configured */

#define MCFGR_SWRST       ((uint32_t)(1)<<31) /* Software Reset */
#define MCFGR_DMA_RST     ((uint32_t)(1)<<28) /* DMA Reset */
#define MCFGR_PS_SHIFT          16
#define MCFGR_AWCACHE_SHIFT	8
#define MCFGR_AWCACHE_MASK	(0xf << MCFGR_AWCACHE_SHIFT)
#define MCFGR_ARCACHE_SHIFT	12
#define MCFGR_ARCACHE_MASK	(0xf << MCFGR_ARCACHE_SHIFT)

#define JR_INTMASK	  0x00000001
#define JRCR_RESET                  0x01
#define JRINT_ERR_HALT_INPROGRESS   0x4
#define JRINT_ERR_HALT_COMPLETE     0x8
#define JRINT_ERR_HALT_MASK         0xc
#define JRNSLIODN_SHIFT		16
#define JRNSLIODN_MASK		0x0fff0000
#define JRSLIODN_SHIFT		0
#define JRSLIODN_MASK		0x00000fff

#define JQ_DEQ_ERR		-1
#define JQ_DEQ_TO_ERR		-2
#define JQ_ENQ_ERR		-3

#pragma pack(4)
struct op_ring {
	phys_addr_t desc;
	uint32_t status;
#ifdef OR4W
	uint32_t reserve;
#endif
} __packed;
#pragma pack

struct jr_info {
	phys_addr_t desc_phys_addr;
	void (*callback)(uint32_t status, void *arg);
	uint32_t desc_len;
	uint32_t op_done;
	void *arg;
};

struct jobring {
	/* Circular Ring of o/p descriptors */
	/* Circula Ring containing info regarding descriptors in i/p
	 * and o/p ring
	 */
	/* This ring can be on the stack */
	struct jr_info info[JR_SIZE];
	struct op_ring *output_ring;

	/* The ip and output rings have to be accessed by SEC. So the
	 * pointers will ahve to point to the housekeeping region provided
	 * by SEC
	 */
	/*Circular  Ring of i/p descriptors */
	dma_addr_t *input_ring;

	int jq_id;
	int irq;
	int liodn;
	/* Head is the index where software would enq the descriptor in
	 * the i/p ring
	 */
	int head;
	/* Tail index would be used by s/w ehile enqueuing to determine if
	 * there is any space left in the s/w maintained i/p rings
	 */
	/* Also in case of deq tail will be incremented only in case of
	 * in-order job completion
	 */
	int tail;
	/* Read index of the output ring. It may not match with tail in case
	 * of out of order completetion
	 */
	int read_idx;
	/* Write index to input ring. Would be always equal to head */
	int write_idx;
	/* Size of the rings. */
	int size;
	/* Op ring size aligned to cache line size */
	int op_size;

	/* Offset in CCSR to the SEC engine to which this JR belongs */
	uint32_t sec_offset;

	struct jr_regs *regs;
};

struct result {
	int done;
	uint32_t status;
};

//#define Ring_Size  0x400
#define Ring_Size JR_SIZE
#define Ring_Num  3

//#define SEC_NUM	2
#define SEC_NUM	1

extern u64 array_data;
///////////////////////////////////////////////////////
#define cpu_relax() asm("nop");

//#define CIRC_CNT(head, tail, size)	(((head) - (tail)) & (size - 1))
#define CIRC_CNT(head, tail, size)	(((head) - (tail)) & (size))

#define array_size (sizeof(dma_addr_t)*Ring_Size + sizeof(struct op_ring)*Ring_Size)

#define JR_MEMOFF	0x200

#if DDR_SEC
#define array_data0  0x800000300037c000	//virt addr for job ring array address
#else
#define array_data0  0x00000000e007c000ull	//virt addr for job ring array address
#endif
#define jobin_array0  array_data0
#define jobout_array0 ((dma_addr_t *)(array_data0 + sizeof(dma_addr_t)*Ring_Size))

#define jobin_array1 ((array_data0 + array_size))
#define jobout_array1 ((jobin_array1 + sizeof(dma_addr_t)*Ring_Size))

#define jobin_array2  (array_data0 + 2*array_size)
#define jobout_array2  (jobin_array2 + sizeof(dma_addr_t)*Ring_Size)

#define jobin_array3  (array_data0 + 3*array_size)
#define jobout_array3  (jobin_array3 + sizeof(dma_addr_t)*Ring_Size)

#define jobin_array4  (array_data0 + 4*array_size)
#define jobout_array4  (jobin_array4 + sizeof(dma_addr_t)*Ring_Size)

#define jobin_array5  (array_data0 + 5*array_size)
#define jobout_array5  (jobin_array5 + sizeof(dma_addr_t)*Ring_Size)

#define jobin_array6  (array_data0 + 6*array_size)
#define jobout_array6  (jobin_array6 + sizeof(dma_addr_t)*Ring_Size)

#define jobin_array7  (array_data0 + 7*array_size)
#define jobout_array7  (jobin_array7 + sizeof(dma_addr_t)*Ring_Size)

#define jobin_array8  (array_data0 + 8*array_size)
#define jobout_array8  (jobin_array8 + sizeof(dma_addr_t)*Ring_Size)

#define VOFFSET (0x0000000400000000UL)

///////////////////////////////////////////////////////

#define SEC_ADDR(idx)	(0xe9200000+idx*0x400000)			//needed set by sys addr configure
//#define SEC_ADDR(idx)	(0xe0500000+idx*0x400000)			//needed set by sys addr configure

#define SEC_JR0_ADDR(idx)	(SEC_ADDR(idx)+0x1000)		//needed set by sys addr configure
#define SEC_JR1_ADDR(idx)	(SEC_ADDR(idx)+0x2000)		//needed set by sys addr configure
#define SEC_JR2_ADDR(idx)	(SEC_ADDR(idx)+0x3000)		//needed set by sys addr configure
#define SEC_JR3_ADDR(idx)	(SEC_ADDR(idx)+0x15000)		//needed set by sys addr configure

#define SEC_JR4_ADDR(idx)	(SEC_ADDR(idx)+0x16000)		//needed set by sys addr configure
#define SEC_JR5_ADDR(idx)	(SEC_ADDR(idx)+0x17000)		//needed set by sys addr configure
#define SEC_JR6_ADDR(idx)	(SEC_ADDR(idx)+0x18000)		//needed set by sys addr configure
#define SEC_JR7_ADDR(idx)	(SEC_ADDR(idx)+0x19000)		//needed set by sys addr configure

#define SEC_JR8_ADDR(idx)	(SEC_ADDR(idx)+0x1a000)		//needed set by sys addr configure


typedef struct jr_total_st{
	struct jobring jr[Ring_Num*SEC_NUM];
	int nr[SEC_NUM];
	int nsec;
}jrt_st;

extern struct jobring *jr_g;
extern jrt_st jr_t;
extern int jr_nr;

void caam_jr_strstatus(u32 status);
int run_descriptor_jr(uint32_t *desc,jrt_st *jrt);

extern int jdkek_gen;

#endif
