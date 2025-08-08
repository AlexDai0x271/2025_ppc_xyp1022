/*
 * jr.c
 *
 *  Created on: Mar 20, 2017
 *      Author: jzj
 */
/*
 * CAAM/SEC 4.x transport/backend driver
 * JobR backend functionality
 *
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
 */


#include "jr.h"
#include "desc.h"
#include "sec.h"
#include "common.h"
#include "desc_constr.h"

jrt_st jr_t;
int jr_nr;
struct jobring *jr_g;

phys_addr_t virt_to_phys(uint32_t *desc_addr)
{
	phys_addr_t phys_addr;
	#if DDR_SEC
		if(desc_addr > 0xE0000000)
		{
			phys_addr = ((phys_addr_t )desc_addr &0xffffffff) + VOFFSET+0x3000000000UL;
		}else
		{
			phys_addr = ((phys_addr_t )desc_addr &0xffffffff) + VOFFSET+0x0000000000UL;
		}
	#else
		phys_addr = ((phys_addr_t )desc_addr &0xffffffff) + VOFFSET;
	#endif
	return phys_addr;
}

static int caam_reset_hw_jr(struct jobring *jr,uint8_t sec_idx)
{
	unsigned int timeout = 100000;
	struct jr_regs *regs;
	regs = jr->regs;
	/*
	 * mask interrupts since we are going to poll
	 * for reset completion status
	 */
	//setbits32(&regs->jrcfg1, JR_INTMASK);

	/* initiate flush (required prior to reset) */
	jr_out32(&regs->jrcr, JRCR_RESET);
	while (((jr_in32(&regs->jrint) & JRINT_ERR_HALT_MASK) ==
		JRINT_ERR_HALT_INPROGRESS) && --timeout)
		cpu_relax();

	if ((jr_in32(&regs->jrint) & JRINT_ERR_HALT_MASK) !=
	    JRINT_ERR_HALT_COMPLETE || timeout == 0) {
		dev_err("failed to flush job ring\n");
		return -1;
	}

	/* initiate reset */
	timeout = 100000;
	jr_out32(&regs->jrcr, JRCR_RESET);
	while ((jr_in32(&regs->jrcr) & JRCR_RESET) && --timeout)
		cpu_relax();

	if (timeout == 0) {
		dev_err("failed to reset job ring\n");
		return -1;
	}

	/* unmask interrupts */
	//clrbits32(&regs->jrcfg1, JR_INTMASK);

	return 0;
}

static inline void start_jr(uint8_t sec_idx)
{
	ccsr_sec_t *sec = (void *)SEC_ADDR(sec_idx);
	//u32 ctpr_ms = sec_in32(&sec->ctpr_ms);
	//u32 scfgr = sec_in32(&sec->scfgr);
#if 0
	if (ctpr_ms & SEC_CTPR_MS_VIRT_EN_INCL) {
		/* VIRT_EN_INCL = 1 & VIRT_EN_POR = 1 or
		 * VIRT_EN_INCL = 1 & VIRT_EN_POR = 0 & SEC_SCFGR_VIRT_EN = 1
		 */
		if ((ctpr_ms & SEC_CTPR_MS_VIRT_EN_POR) ||
		    (!(ctpr_ms & SEC_CTPR_MS_VIRT_EN_POR) &&
					(scfgr & SEC_SCFGR_VIRT_EN)))
			sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR0);
	} else {
		/* VIRT_EN_INCL = 0 && VIRT_EN_POR_VALUE = 1 */
		if (ctpr_ms & SEC_CTPR_MS_VIRT_EN_POR)
			sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR0);
	}
#else
	//sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR0);
	//sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR1);
	//sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR2);
	//sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR3);
#endif
}

static inline void jr_disable_irq(uint8_t sec_idx)
{
	struct jr_regs *regs = (struct jr_regs *)SEC_JR0_ADDR(sec_idx);
	uint32_t jrcfg = sec_in32(&regs->jrcfg1);

	jrcfg = jrcfg | JR_INTMASK;

	sec_out32(&regs->jrcfg1, jrcfg);
}


static void jr_initregs(uint8_t sec_idx,struct jobring *jr_in)
{
	struct jr_regs *regs;
	//struct jobring *jr = &jr0[sec_idx];
	struct jobring *jr = jr_in;

	regs = jr->regs;

	phys_addr_t ip_base = virt_to_phys((void *)jr->input_ring);
	phys_addr_t op_base = virt_to_phys((void *)jr->output_ring);

	//op_base = 0x1234567891234567UL;
	sec_out32(&regs->irba_l, (uint32_t)ip_base);
#ifdef CONFIG_PHYS_64BIT
	sec_out32(&regs->irba_h, ip_base >> 32);
#else
	sec_out32(&regs->irba_h, 0x0);
#endif
	sec_out32(&regs->orba_l, (uint32_t)op_base);
#ifdef CONFIG_PHYS_64BIT
	sec_out32(&regs->orba_h, op_base >> 32);
#else
	sec_out32(&regs->orba_h, 0x0);
#endif

	sec_out32(&regs->jrcfg0, 0x3030);

	sec_out32(&regs->jrcfg1, 0x01);			//disable int

	sec_out32(&regs->irs, JR_SIZE);
	sec_out32(&regs->ors, JR_SIZE);


	//if (!jr->irq)
		//jr_disable_irq(sec_idx);

}

static int jr_init(uint8_t sec_idx,struct jobring *jr)
{
	memset(jr, 0, sizeof(struct jobring));

	jr->jq_id = DEFAULT_JR_ID;
	jr->irq = DEFAULT_IRQ;

#ifdef CONFIG_FSL_CORENET
	jr->liodn = DEFAULT_JR_LIODN;
#endif
	jr->size = JR_SIZE;
	if(jr == &(jr_g[0+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array0+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR0_ADDR(sec_idx);
	}else if(jr == &(jr_g[1+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array1+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR1_ADDR(sec_idx);
	}else if(jr == &(jr_g[2+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array2+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR2_ADDR(sec_idx);
	}else if(jr == &(jr_g[3+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array3+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR3_ADDR(sec_idx);
	}else if(jr == &(jr_g[4+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array4+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR4_ADDR(sec_idx);
	}else if(jr == &(jr_g[5+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array5+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR5_ADDR(sec_idx);
	}else if(jr == &(jr_g[6+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array6+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR6_ADDR(sec_idx);
	}else if(jr == &(jr_g[7+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array7+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR7_ADDR(sec_idx);
	}else if(jr == &(jr_g[8+sec_idx*Ring_Num]))
	{
		jr->input_ring = jobin_array8+JR_MEMOFF*sec_idx;
		jr->regs = (struct jr_regs *)SEC_JR8_ADDR(sec_idx);
	}else
	{
		return -1;
	}

	if (!jr->input_ring)
		return -1;

	jr->op_size = JR_SIZE * sizeof(struct op_ring);
	if(jr == &(jr_g[0+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array0+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[1+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array1+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[2+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array2+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[3+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array3+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[4+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array4+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[5+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array5+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[6+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array6+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[7+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array7+JR_MEMOFF*sec_idx;
	}else if(jr == &(jr_g[8+sec_idx*Ring_Num]))
	{
		jr->output_ring = jobout_array8+JR_MEMOFF*sec_idx;
	}else
	{
		return -1;
	}

	if (!jr->output_ring)
		return -1;

	memset(jr->input_ring, 0, JR_SIZE * sizeof(dma_addr_t));
	memset(jr->output_ring, 0, jr->op_size);

	caam_reset_hw_jr(jr,sec_idx);

//	start_jr(sec_idx);		//for ccore sec,maybe not need

	jr_initregs(sec_idx,jr);


	return 0;
}

/* -1 --- error, can't enqueue -- no space available */
int jr_enqueue(uint32_t *desc_addr,
	       void (*callback)(uint32_t status, void *arg),
	       void *arg, struct jobring *jr_in)
{
	struct jr_regs *regs;
	//struct jobring *jr = &jr0[sec_idx];
	struct jobring *jr = jr_in;

	regs = jr->regs;

	int head = jr->head;
	int length = desc_len(desc_addr);
	int i;
#ifdef CONFIG_PHYS_64BIT
	uint32_t *addr_hi, *addr_lo;
#endif

	/* The descriptor must be submitted to SEC block as per endianness
	 * of the SEC Block.
	 * So, if the endianness of Core and SEC block is different, each word
	 * of the descriptor will be byte-swapped.
	 */
	if (!sec_in32(&regs->irsa) ||
	    CIRC_CNT(jr->head + 1, jr->tail,jr->size-1) <= 0)
	{
		return -1;
	}
/*
	for (i = 0; i < length; i++) {
		desc_addr[i] = cpu_to_le32(desc_addr[i]);
	}
*/
	phys_addr_t desc_phys_addr = virt_to_phys(desc_addr);
	desc_phys_addr = (desc_phys_addr>>32)|((desc_phys_addr&0xffffffff)<<32);

	jr->info[head].desc_phys_addr = desc_phys_addr;
	jr->info[head].callback = (void *)callback;
	jr->info[head].arg = arg;
	jr->info[head].op_done = 0;

#ifdef CONFIG_PHYS_64BIT
	/* Write the 64 bit Descriptor address on Input Ring.
	 * The 32 bit hign and low part of the address will
	 * depend on endianness of SEC block.
	 */

	addr_hi = (uint32_t *)(&jr->input_ring[head])+1;
	addr_lo = (uint32_t *)(&jr->input_ring[head]);

	//desc_phys_addr = 0x0123456789abcdefUL;

	sec_out32(addr_hi, (uint32_t)cpu_to_le32(desc_phys_addr ));
	sec_out32(addr_lo, (uint32_t)cpu_to_le32(desc_phys_addr>> 32));

#else
	/* Write the 32 bit Descriptor address on Input Ring. */
	sec_out32(&jr->input_ring[head], desc_phys_addr);
#endif /* ifdef CONFIG_PHYS_64BIT */

	jr->head = (head + 1) & (jr->size-1);

	sec_out32(&regs->irja, 1);

	return 0;
}

int jr_dequeue(struct jobring *jr_in)
{
	struct jr_regs *regs;
	//struct jobring *jr = &jr0[sec_idx];
	struct jobring *jr = jr_in;

	regs = jr->regs;

	int head = jr->head;
	int tail = jr->tail;
	int idx, i, found;
	void (*callback)(uint32_t status, void *arg);
	void *arg = NULL;
#ifdef CONFIG_PHYS_64BIT
	uint32_t *addr_hi, *addr_lo;
#else
	uint32_t *addr;
#endif

	while (sec_in32(&regs->orsf) && CIRC_CNT(jr->head, jr->tail,
						 jr->size-1)) {

//	while (sec_in32(&regs->orsf)){
		found = 0;

		phys_addr_t op_desc;
	#ifdef CONFIG_PHYS_64BIT
		/* Read the 64 bit Descriptor address from Output Ring.
		 * The 32 bit hign and low part of the address will
		 * depend on endianness of SEC block.
		 */

		op_desc = jr->output_ring[jr->tail].desc;
//		op_desc = (le32_to_cpu((op_desc >> 32)&0xffffffff)<<32) | (le32_to_cpu(op_desc &0xfffffffff)<<32);

	#else
		/* Read the 32 bit Descriptor address from Output Ring. */
		addr = (uint32_t *)&jr->output_ring[jr->tail].desc;
		op_desc = sec_in32(addr);
	#endif /* ifdef CONFIG_PHYS_64BIT */

		uint32_t status = le32_to_cpu(sec_in32(&jr->output_ring[jr->tail].status));

		for (i = 0; CIRC_CNT(head, tail + i, jr->size-1) >= 1; i++) {
			idx = (tail + i) & (jr->size-1);
			if (op_desc == ((jr->info[idx].desc_phys_addr)&ORING_DESC_MASK)) {
				found = 1;
				break;
			}
		}

		/* Error condition if match not found */
		if (!found)
			return -1;

		jr->info[idx].op_done = 1;
		callback = (void *)jr->info[idx].callback;
		arg = jr->info[idx].arg;

		/* When the job on tail idx gets done, increment
		 * tail till the point where job completed out of oredr has
		 * been taken into account
		 */
		if (idx == tail)
			do {
				tail = (tail + 1) & (jr->size-1);
			} while (jr->info[tail].op_done);

		jr->tail = tail;
		jr->read_idx = (jr->read_idx + 1) & (jr->size - 1);

		sec_out32(&regs->orjr, 1);

		while(sec_in32(&regs->orjr)!=1);

		if(sec_in32(&regs->orjr)!=0)
		{
			while(1);
		}

		jr->info[idx].op_done = 0;

		callback(status, arg);
	}

	return 0;
}

int sec_init_idx(uint8_t sec_idx,struct jobring *jr)
{
	ccsr_sec_t *sec = (void *)SEC_ADDR(sec_idx);
	//uint32_t mcr = sec_in32(&sec->mcfgr);
	int ret = 0;

	//mcr = (mcr & ~MCFGR_AWCACHE_MASK) | (0x2 << MCFGR_AWCACHE_SHIFT);		//is it needed?

#ifdef CONFIG_PHYS_64BIT
	//mcr |= (1 << MCFGR_PS_SHIFT);
#endif
	//sec_out32(&sec->mcfgr, mcr);

	ret = jr_init(sec_idx,jr);
	if (ret < 0) {
		printf("SEC initialization failed\n");
		return -1;
	}
/*
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x74c) = 0xf;
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x700) = 0x040f0100;
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x750) = 0xffffffff;
	*(unsigned int *)(SEC_ADDR(sec_idx) + 0x630) = 2;

	if (get_rng_vid(sec_idx) >= 4) {
		if (rng_init(sec_idx) < 0) {
			printf("SEC%u: RNG instantiation failed\n", sec_idx);
			return -1;
		}
		printf("SEC%u: RNG instantiated\n", sec_idx);
	}
*/
	return ret;
}

int sec_init(void)
{
	int i,j,ret;
	jr_g = jr_t.jr;

	for(j=0;j<SEC_NUM;j++)
	{
		for(i=0;i<Ring_Num;i++)
		{
			ret = sec_init_idx(j,&jr_g[i+Ring_Num*j]);
			if(ret)
				return ret;
		}
		jr_t.nr[j] = j*Ring_Num;
	}

	jr_nr =0;
	//sec_cipher_jdkek_init();
	return 0;
}

static void desc_done(uint32_t status, void *arg)
{
	struct result *x = arg;
	x->status = status;
	//caam_jr_strstatus(status);
	x->done = 1;
}

static inline int do_run_descriptor_jr(uint32_t *desc,struct jobring *jr)
{
	//unsigned long long timeval = get_ticks();
	//unsigned long long timeout = usec2ticks(CONFIG_SEC_DEQ_TIMEOUT);
	struct result op;
	int ret = 0;

	memset(&op, 0, sizeof(op));

	ret = jr_enqueue(desc,desc_done, &op,jr);
	if (ret) {
		debug("Error in SEC enq\n");
		ret = JQ_ENQ_ERR;
		goto out;
	}

	//timeval = get_ticks();
	//timeout = usec2ticks(CONFIG_SEC_DEQ_TIMEOUT);
	while (op.done != 1) {
		ret = jr_dequeue(jr);
		if (ret) {
			debug("Error in SEC deq\n");
			ret = JQ_DEQ_ERR;
			goto out;
		}
/*
		if ((get_ticks() - timeval) > timeout) {
			debug("SEC Dequeue timed out\n");
			ret = JQ_DEQ_TO_ERR;
			goto out;
		}
*/
	}

	if (op.status & 0x0fffffff) {
		debug("RUN Error %x\n", op.status);
		ret = op.status;
	}
out:
	return ret;
}

int run_descriptor_jr_index(uint32_t index, uint32_t *desc)
{
	int ret;
	ret = do_run_descriptor_jr(desc, &(jr_g[index]));

	if(ret!=0)
	{
		return ret;
	}

	return 0;
	
}

int run_descriptor_jr(uint32_t *desc,jrt_st *jrt)
{
	int ret;
	ret = do_run_descriptor_jr(desc, &(jr_g[jr_nr]));
	if(ret)
	{
		return ret;
	}
	if(jr_nr < (Ring_Num*SEC_NUM-1))
		jr_nr ++;
	else
		jr_nr = 0;

	return 0;
}

#if 0	
int run_descriptor_jr_sec0(uint32_t *desc,jrt_st *jrt)
{
	int ret;
	ret = do_run_descriptor_jr(desc, &(jr_g[jrt->nr[0]]));
	if(ret)
	{
		return ret;
	}
	if(jrt->nr < (Ring_Num-1))
		jrt->nr[0] ++;
	else
		jrt->nr[0] = 0;

	return 0;
}
													
int run_descriptor_jr_sec1(uint32_t *desc,jrt_st *jrt)				//908 2sec use
{
	int ret;
	ret = do_run_descriptor_jr(desc, &(jr_g[jrt->nr[1]]));
	if(ret)
	{
		return ret;
	}
	if(jrt->nr[1] < (Ring_Num*SEC_NUM-1))
		jrt->nr[1] ++;
	else
		jrt->nr[1] = Ring_Num;

	return 0;
}
#else
int run_descriptor_jr_sec0(uint32_t *desc,jrt_st *jrt)
{
	int ret;
	ret = do_run_descriptor_jr(desc, &(jr_g[jr_nr]));
	if(ret)
	{
		return ret;
	}
	if(jr_nr < (Ring_Num*SEC_NUM-1))
		jr_nr ++;
	else
		jr_nr = 0;

	return 0;
}

int run_descriptor_jr_sec1(uint32_t *desc,jrt_st *jrt)				//908 2sec use
{
	int ret;
	ret = do_run_descriptor_jr(desc, &(jr_g[jr_nr]));
	if(ret)
	{
		return ret;
	}
	if(jr_nr < (Ring_Num*SEC_NUM-1))
		jr_nr ++;
	else
		jr_nr = 0;

	return 0;
}
#endif

