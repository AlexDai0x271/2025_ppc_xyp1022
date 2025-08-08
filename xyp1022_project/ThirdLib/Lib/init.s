
	.file	"init.s"

	.include "e500mc.i"
	.include "macros.i"
    .include "mpic.i"
# The boot code must be located from 0xFFFFF000 to 0xFFFFFFFC.
	.section .text_init, "ax"
	.global	init_core
init_core:
# ------- Step 2: reset basic regs ------- {{{
# WARNING: xor r0,r0,r0 doesn't do what you expect if r0==32'bx!
	li	r0,1 
	mtlr	r0
	mtxer	r0
	mtctr	r0
	mtcr	r0
#	mtspr DCRIPR, r0
	mtspr	SRR0,r0
	mtspr	SRR1,r0
	mtspr	CSRR0,r0
	mtspr	CSRR1,r0
	mtspr	MCSRR0,r0
	mtspr	MCSRR1,r0
.ifndef CCP801T
	li32	r2,CCR0_ITE_MASK # enable trace 
	mtspr	CCR0,r2
.else
##	li32	r2,CCR0_BHTI_MASK|CCR0_DBHT_MASK|CCR0_DBTAC_MASK # disable BHT and BTAC
##	mtspr	CCR0,r2
.endif

# Step 9: initial other GPRs
	li	r3,0
	li	r4,0
	li	r5,0
	li	r6,0
	li	r7,0
	li	r8,0
	li	r9,0
	li	r10,0
	li	r11,0
	li	r12,0
	li	r13,0
	li	r14,0
	li	r15,0
	li	r16,0
	li	r17,0
	li	r18,0
	li	r19,0
	li	r20,0
	li	r21,0
	li	r22,0
	li	r23,0
	li	r24,0
	li	r25,0
	li	r26,0
	li	r27,0
	li	r28,0
	li	r29,0
	li	r30,0
	li	r31,0

#-------  Step 11: Initialize exception SPRs ------- {{{
e500mc_set_tlb_jump:
  b e500mc_set_tlb

#   b e500mc_set_tlb
# 
# c9000_set_tlb:
#   li32 r5,TLB0_SIZE_64K
#   li32 r4,0xfffe0000
#   addi r7,r4,0x0
#   li32 r6,TLB2_U_RWX|TLB2_S_RWX|TLB2_I_MASK|TLB2_G_MASK
#   li   r3,1
# 
# # input:
# # r3 = TLB entry number
# # r4 = 32-bit (internal) virtual address
# # r5 = Page size
# # r6 = Attributes including WMIGE
# # r7 = (External) physical address, higher bits wrapped to lower bits
# c9000_SetTlB:
#   or r0,r4,r5
#   ori r0,r0,TLB0_V_MASK
#   tlbwe r0,r3,0
#   tlbwe r7,r3,1
#   tlbwe r6,r3,2

# MAS0-3,5,7 needed
.global .e500mc_SetTLB
.global e500mc_SetTLB
.e500mc_SetTLB:
e500mc_SetTLB:
  mtspr MAS0,r3
  mtspr MAS1,r4
  mtspr MAS2,r5
  mtspr MAS3,r6
  mtspr MAS5,r7
  mtspr MAS7,r8
  sync
  isync
  tlbwe
  #sync  ## will halt
  isync
  blr

.global e500mc_ReadTLB
e500mc_ReadTLB:
  mtspr MAS0,r3
  #sync  ## will halt
  #isync
  tlbre
  #sync  ## will halt
  #isync
  mfspr r7 ,MAS0
  mfspr r8 ,MAS1
  mfspr r9 ,MAS2
  mfspr r10,MAS3
  mfspr r11,MAS4
  mfspr r12,MAS5
  mfspr r13,MAS6
  mfspr r14,MAS7
  mfspr r15,MAS8
  blr

e500mc_dump_l1_ltb1:
  mflr  r4
  li32  r3,0x10000000
  bl    e500mc_ReadTLB
  li32  r3,0x10010000
  bl    e500mc_ReadTLB
  li32  r3,0x10020000
  bl    e500mc_ReadTLB
  li32  r3,0x10030000
  bl    e500mc_ReadTLB
  li32  r3,0x10040000
  bl    e500mc_ReadTLB
  li32  r3,0x10050000
  bl    e500mc_ReadTLB
  li32  r3,0x10060000
  bl    e500mc_ReadTLB
  li32  r3,0x10070000
  bl    e500mc_ReadTLB
  mtlr  r4
  blr

e500mc_set_tlb:
#rom
  li32  r3,0x10000000 # TLB1, ENTRY 1, NEXT VICTIM 2
  li32  r4,0xc0000300 # VALID, TID=FE, SIZE=64K(0030)
  li32  r5,0xffff0004 # EPN[31:12], X0, X1, WIMGE
  li32  r6,0xffff003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000004 # RPN[35:32]
  bl    e500mc_SetTLB


##CCSR  0xe800_0000~0xebff_ffff
##NOTE: Gard bit must be 0x1. If no gard, read more SSI-register will cause access-error exception!
  li32  r3,0x10010000 # TLB1, iENTRY 1, NEXT VICTIM 2
  li32  r4,0xc0000800 # VALID, TID=FE, SIZE=64M(1000)
  li32  r5,0xe800000b # EPN[31:12], X0, X1, WIMGE little-endian
  li32  r6,0xe800003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000004 # RPN[35:32]
  bl    e500mc_SetTLB

##CCSR 0xec00_0000 ~ 0xefff_ffff
  li32  r3,0x10020000 # TLB1, iENTRY 1, NEXT VICTIM 2
  li32  r4,0xc0000800 # VALID, TID=FE, SIZE=64M(1000)
  li32  r5,0xec00000b # EPN[31:12], X0, X1, WIMGE little-endian
  li32  r6,0xec00003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000004 # RPN[35:32]
  bl    e500mc_SetTLB

##SRAM 0xe000_0000 ~ 0xe00f_ffff
  li32  r3,0x10030000 # TLB1, iENTRY 1, NEXT VICTIM 2
  li32  r4,0xc0000500 # VALID, TID=FE, SIZE=1M(0101)
  li32  r5,0xe000000a # EPN[31:12], X0, X1, WIMGE little-endian
  li32  r6,0xe000003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000004 # RPN[35:32]
  bl    e500mc_SetTLB

##ebc flash 0xf800_0000 ~ 0xf83f_ffff
  li32  r3,0x10040000 # TLB1, iENTRY 1, NEXT VICTIM 2
  li32  r4,0xc0000800 # VALID, TID=FE, SIZE=64M
  li32  r5,0xf800000a # EPN[31:12], X0, X1, WIMGE
  li32  r6,0xf800003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000004 # RPN[35:32]
  bl    e500mc_SetTLB

## 0x0000_0000 <=> 0x0000_0000 ~ 0x000f_ffff
##DDR
  li32  r3,0x10050000 # TLB1, iENTRY 1, NEXT VICTIM 2
  li32  r4,0xc0000a00 # VALID, TID=FE, SIZE=1M(0101)
  li32  r5,0x0000000a # EPN[31:12], X0, X1, WIMGE little-endian
  li32  r6,0x0000003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000000 # RPN[35:32]
  bl    e500mc_SetTLB

##0x4000_0000~0x7FFF_FFFF
##DDR
  li32  r3,0x10060000 # TLB1, iENTRY 16, NEXT VICTIM 2
  li32  r4,0xc0000a00 # VALID, TID=FE, SIZE=1G(1010)
  li32  r5,0x4000000A # EPN[31:12], X0, X1, WIMGE
  li32  r6,0x4000003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000000 # RPN[35:32]
  bl    e500mc_SetTLB

## 0xfffe_0000 <=> 0xfffe_0000 ~ 0xfffe_ffff  
  li32  r3,0x10070000 # TLB1, iENTRY 1, NEXT VICTIM 2
  li32  r4,0xc0000300 # VALID, TID=FE, SIZE=64K(0030)
  li32  r5,0xfffe0004 # EPN[31:12], X0, X1, WIMGE
  li32  r6,0xfffe003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  li32  r7,0x00000000 # SGS=0, SLPID=1
  li32  r8,0x00000004 # RPN[35:32]
  bl    e500mc_SetTLB

  bl    e500mc_dump_l1_ltb1

  bl    tlb_clr_other

  isync
  
  li32 r4,0x90
  mtspr HID0,r4
  sync
  isync
.if 0
  bl         l1l2cache_init
 
l1l2cache_init:
# Flash invalidate the L1 caches : page 11-4 of E500MCRM.pdf
# L1 data cache
  xor r4,r4,r4
  ori r5,r4,0x0102
  sync
  isync
  mtspr L1CSR0,r5
  isync
dloop:
  mfspr r4,L1CSR0
  and. r4,r4,r5
  bne dloop
  isync

# L1 instruction cache
  xor r4,r4,r4
  ori r5,r4,0x0102
  sync
  isync
  mtspr L1CSR1,r5
  isync
iloop:
  mfspr r4,L1CSR1
  and. r4,r4,r5
  bne iloop
  isync

# L2 data cache
  xor r4,r4,r4
  ori r5,r4,0x0400
  oris r5,r5,0x0020
  sync
  isync
  mtspr L2CSR0,r5
  isync
l2loop:
  mfspr r4,L2CSR0
  and. r5,r5,r4
  bne l2loop

# Enable branch prediction, copied from page 11-5 of E500MCRM.pdf
  xor r4,r4,r4     # set r4 to 0
  ori r5,r4,0x0201 # set BBFI and BPEN
  mtspr BUCSR,r5   # flash invalidate and enable branch prediction
  isync            # synchronize setting of BUCSR

enable_l1_dcache:
  xor r4,r4,r4
  ori r5,r4,0x0001
  sync
  isync
  mtspr L1CSR0,r5
  isync

#testbench_top.u_h2040_top.u_e500mc_subsys.core0.e500mc_l2size_128_pd.e500mc_l2size_128.elf.mmu.l1csr0_reg0.i000.L2[1]

enable_l1_icache:
  xor r4,r4,r4
  ori r5,r4,0x0001
  sync
  isync
  mtspr L1CSR1,r5
  isync

#testbench_top.u_h2040_top.u_e500mc_subsys.core0.e500mc_l2size_128_pd.e500mc_l2size_128.elf.mmu.l1csr1_reg0.i000.L2[0]  

enable_l2_cache:
  xor r4,r4,r4
  oris r5,r4,0x8000
  sync
  isync
  mtspr L2CSR0,r5
  isync 
.endif
#####_CINTR_PREHANDLER@h --> r2[15:0] --> r3[31:16] --> IVPR[32:47]
#####_CINTR_PREHANDLER@l --> r2[15:0]               --> IVOR0[48:59]
   li      r3,_CINTR_PREHANDLER@high
   rlwinm  r3, r3, 16, 0, 15
   mtspr   IVPR,r3
   li      r3,_CINTR_PREHANDLER@l
   mtspr   IVOR0,r3
   li      r3,_MC_PREHANDLER@l
   mtspr   IVOR1,r3
#   li      r3,_DSIDE_PREHANDLER@l
#   mtspr   IVOR2,r3
#   li      r3,_ISIDE_PREHANDLER@l
#   mtspr   IVOR3,r3
   li      r3,_NCINTR_PREHANDLER@l
   mtspr   IVOR4,r3
   li      r3,_UNALIGN_PREHANDLER@l
   mtspr   IVOR5,r3
   li      r3,_PROGRAM_PREHANDLER@l
   mtspr   IVOR6,r3
   li      r3,_FPU_PREHANDLER@l
   mtspr   IVOR7,r3
   li      r3,_SC_PREHANDLER@l
   mtspr   IVOR8,r3
   li      r3,_APU_PREHANDLER@l
   mtspr   IVOR9,r3
   li      r3,_DEC_PREHANDLER@l
   mtspr   IVOR10,r3
   li      r3,_FIT_PREHANDLER@l
   mtspr   IVOR11,r3
   li      r3,_WTDOG_PREHANDLER@l
   mtspr   IVOR12,r3
   li      r3,_DTLB_PREHANDLER@l
   mtspr   IVOR13,r3
   li      r3,_ITLB_PREHANDLER@l
   mtspr   IVOR14,r3
#   li      r3,_DEBUG_PREHANDLER@l
#   mtspr   IVOR15,r3

    .extern .vPortYieldISR
    .extern .vPortTickISR
    .extern .vPortNormalISR

    li      r3,.vPortNormalISR@l
    mtspr   IVOR4,r3
    li      r3,.vPortYieldISR@l
    mtspr   IVOR8,r3
    li      r3,.vPortTickISR@l
    mtspr   IVOR10,r3

	# Initialize other interrupt-related registers
	li	r0,0
	mtspr	DEAR,r0
	mtspr	ESR,r0
	
	bl init_chip
    b .	

    .section .weakcode, "ax"
    .weak main_cpu1
main_cpu1:
     b .

    .weak main_cpu2
main_cpu2:
     b .

    .weak main_cpu3
main_cpu3:
     b .
     

    #clear all tlb entry ways, excpet entry0
tlb_clr_other:
	mflr	r9	
	li32 	r3, 0x100c0000	#TBL1		need look with the tlb1 used above
	li32	r8, 0x103F0000
tlb1_clr:
	li32 r4,0;	#NO VALID
	bl e500mc_SetTLB
	cmpw	r3,r8
	addis	r3,r3,1
	bne+	tlb1_clr
	
	li32	r3, 0x00000000	#TBL0        all tlb0 no used
	li32	r8, 0x01FF0000
tlb0_clr:
	li32 r4,0;	#NO VALID
	bl e500mc_SetTLB
	cmpw	r3,r8
	addis	r3,r3,1
	bne+	tlb0_clr
	
	mtlr	r9
	blr


