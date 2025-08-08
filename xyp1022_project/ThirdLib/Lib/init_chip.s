/*----------------------------------------------------------------------------+
|       COPYRIGHT   I B M   CORPORATION 2002, 2011
|       LICENSED MATERIAL  -  PROGRAM PROPERTY OF I B M
|       US Government Users Restricted Rights - Use, duplication or
|       disclosure restricted by GSA ADP Schedule Contract with
|       IBM Corp.
+----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------+
| $Author: tyrlik@us.ibm.com $
| $Date: 2011-08-12 09:45:40 -0400 (Fri, 12 Aug 2011) $
| $Revision: 15827 $
+----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------+
| 476FPE BSP for EPOS
| Author: Steve Winiecki
| Component: Boot library.
| File: init_chip.s
| Purpose: Basic 476FPE chip initialization.
| Changes:
| Date:		Comment:
| -----         --------
| 31-Jan-11     Initial port 476FPE					    SAW
| 01-Mar-11     Changed stack invalidate call				    MPT
| 08-Mar-11     Brought in code to initialize PLB6 Arbiter		    MPT
| 09-Mar-11     Corrected PLB6 arbiter initialization values                MPT
| 09-Mar-11     Changed initial stack to use SRAM instead of cache          SAW
| 11-Mar-11     Fix PIR check in spin loop for core1                        SAW
| 21-Mar-11     Add PLL/clock init routines                                 SAW
| 25-Mar-11     Cleanup dealing with second core			    MPT
| 29-Mar-11     Fixed PLB6 bus controller initialization		    MPT
| 13-Apr-11     Final cleanup started					    MPT
| 19-Apr-11     Changes dealing with DDR initialization out of reset	    MPT
| 22-Apr-11     Enable complete processing of DDR memory		    WB
| 25-Apr-11     Another fix for 2MB SRAM support			    MPT
| 26-Apr-11     Add EBC Bank 7 (FPGA) init prior to update_clocks           SAW
| 27-Apr-11     Moved init_misc() up.  Init_misc sets up EBC bank 7	    MPT
| 27-Apr-11     Increased hang detect timeout due to PCI init               MPT
| 11-May-11     Add ddr_scrub to setup ECC handling			    WB
| 22-May-11     Fixed CPU1 release, and fantom SRAM TLB problem		    MPT
| 08-Jun-11     Added code to synchronize time base between two CPU's 	    MPT
| 17-Jul-11     Move DDR ecc init to ddr_init()			 	    ALS
+----------------------------------------------------------------------------*/
	.include "e500mc.i"

	.include "macros.i"
    .include "mpic.i"
    .include "environment.h"

    /* @@ boot from sram,  set it to '1' */
    /* @@ boot from flash, set it to '0' */
    .set BOOT_FROM_SRAM, 1

	.section .text, "ax"
	 	
/*
*********************************************************************
* Init_chip.  Chip initialization code goes here.
*********************************************************************
*/
    .align	2
    .extern mpic_init
    .extern pcie_clkrdy_irqon
    .extern cpr_init
    .extern PCIE_PHYRDY_POLL
    .extern main
    .extern acu_deal
    .extern early_deal
    .extern L3_init
    .extern e500mc_SetTLB
    .global init_chip
init_chip:

/*/////////////////////////////////////////////////////////////////////////
//init r2
//////////////////////////////////////////////////////////////////////////*/
	bl .relative_toc_rom
#Initialize the stack in SRAM for the "C" code that gets called
#__stack_start:CPU0
	lis       r1,  __stack_start@highest
    ori       r1, r1, __stack_start@higher
    sldi      r1, r1, 32
    oris      r1, r1, __stack_start@h
    ori       r1, r1, __stack_start@l
 	mfspr r7,286   
    cmpwi r7,0
    beq core0_branch
/*
	lis       r1,  __stack_start1@highest
    ori       r1, r1, __stack_start1@higher
    sldi      r1, r1, 32
    oris      r1, r1, __stack_start1@h
    ori       r1, r1, __stack_start1@l 
    bl core1_branch
*/
core0_branch:
  	bl axi_law_init
  	bl cc_law_init
  	
  	bl ebc_init
#  	bl gp_branch
.if BOOT_FROM_SRAM /* @@ boot from sram */
  	bl cpr_init
.endif
  	bl pamu_bypass

  	bl acu_deal

	bl mpic_reset
	bl mpic_init
	
.if BOOT_FROM_SRAM /* @@ boot from sram */
	bl ddr_init
.endif

#	bl L3_Init
	bl	init_data
/*
*********************************************************************
 step11 enable all necessary exceptions and interrupts
*********************************************************************
*/
	lis r0,0
	li	r0,0
	lis		r0, (MSR_FP_MASK|MSR_ME_MASK|MSR_EE_MASK|MSR_CE_MASK)@h
	ori		r0, r0, (MSR_FP_MASK|MSR_ME_MASK|MSR_EE_MASK|MSR_CE_MASK)@l
    mtmsr	r0
    
	/* HID0 */	
	lis r0,0
	li	r0,0
	lis		r0, (HID0_EMCP_MASK|HID0_EN_MAS7_UPDATE_MASK|HID0_CIGLSO_MASK)@h
	ori		r0, r0, (HID0_EMCP_MASK|HID0_EN_MAS7_UPDATE_MASK|HID0_CIGLSO_MASK)@l
	mtspr	 0x3f0, r0		

    
# set MSR[CM] = 1 - 64-bit Computation Mode
#    lis    r5, 0x8000
#    mfmsr  r6
#    or     r6, r6, r5
#    mtmsr  r6
#    isync

# set EPCR[ICM] = 1 - Interrupts taken in 64-bit Computation Mode
#    lis    r5, 0x0200
#    mfspr  r6, 307
#    or     r6, r6, r5 
#    mtspr  307, r6 
#    isync    
    
	bl board_init
	bl main
	b .

core1_branch:
	lis r0,0
	li	r0,0
	lis		r0, (MSR_FP_MASK|MSR_ME_MASK|MSR_EE_MASK|MSR_CE_MASK)@h
	ori		r0, r0, (MSR_FP_MASK|MSR_ME_MASK|MSR_EE_MASK|MSR_CE_MASK)@l
    mtmsr	r0

# set MSR[CM] = 1 - 64-bit Computation Mode
#    lis    r5, 0x8000
#    mfmsr  r6
#    or     r6, r6, r5
#    mtmsr  r6
#    isync

# set EPCR[ICM] = 1 - Interrupts taken in 64-bit Computation Mode
#    lis    r5, 0x0200
#    mfspr  r6, 307
#    or     r6, r6, r5 
#    mtspr  307, r6 
#    isync       
    
    #b .
	bl main_cpu1
	b .

/*
*********************************************************************
* Initialize the DATA BSS in SRAM for the "C" code that gets called
*********************************************************************
*/
	.section .text,"ax"
    .align	2
    .global init_data
init_data:

	lis       r3,  __etext@highest
    ori       r3, r3, __etext@higher
    sldi      r3, r3, 32
    oris      r3, r3, __etext@h
    ori       r3, r3, __etext@l

	lis       r4,  __sdata@highest
    ori       r4, r4, __sdata@higher
    sldi      r4, r4, 32
    oris      r4, r4, __sdata@h
    ori       r4, r4, __sdata@l

    lis       r5,  __edata@highest
    ori       r5, r5, __edata@higher
    sldi      r5, r5, 32
    oris      r5, r5, __edata@h
    ori       r5, r5, __edata@l

	sub		r6,r5,r4
	rlwinm  r6,r6,30,2,31
	addi	r6,r6,0x0001

	addi	r7,r3,-4
	addi	r8,r4,-4
	mtspr	CTR,r6
..dag:
	lwzu 	r9,0x0004(r7)
	stwu	r9,0x0004(r8)
	bdnz	..dag

	addi	r5,r5,32
..dcf:
	dcbf	r0,r4
	addi	r4,r4,32
	cmplw	r4,r5
	blt		..dcf

	lis       r4,  __sbss@highest
    ori       r4, r4, __sbss@higher
    sldi      r4, r4, 32
    oris      r4, r4, __sbss@h
    ori       r4, r4, __sbss@l

	lis       r5,  __ebss@highest
    ori       r5, r5, __ebss@higher
    sldi      r5, r5, 32
    oris      r5, r5, __ebss@h
    ori       r5, r5, __ebss@l

	sub		r6,r5,r4
	rlwinm  r6,r6,30,2,31
	addi	r6,r6,0x0001

	addi	r8,r4,-4
	mtspr	CTR,r6
	addi	r9,r0,0x0000
..bag:
	stwu	r9,0x0004(r8)
	bdnz	..bag

..bcf:
	dcbf	r0,r4
	addi	r4,r4,128
	cmplw	r4,r5
	blt	..bcf

	blr
	
/*----------------------------------------------------------------------------+
| PpcMttb.
+----------------------------------------------------------------------------*/
	.section .text,"ax"
    .align	2
    .global .ppcMttb
.ppcMttb:
    addi    r6,r0,0x0000
	mtspr   TBL_W,r6
	lwz 	r6,0x0000(r3)
	lwz	r7,0x0004(r3)
	mtspr	TBU_W,r6
	mtspr   TBL_W,r7
	blr


/*----------------------------------------------------------------------------+
| PpcMftb.
+----------------------------------------------------------------------------*/
	.section .text,"ax"
    .align	2
    .global .ppcMftb
.ppcMftb:
	mfspr	r6,TBU
	mfspr   r7,TBL
	mfspr   r8,TBU
	cmp	cr0,r6,r8
	bne     .ppcMftb
	stw	r6,0x0000(r3)
	stw	r7,0x0004(r3)
	blr

	.section .text,"ax"
    .align	2
    .global .invalidate_dcache_range
.invalidate_dcache_range:
	li	r5,63
	andc	r3,r3,r5
	subf	r4,r3,r4
	add	r4,r4,r5
	rlwinm. r4,r4,26,6,31
	beqlr
	mtctr	r4

1:	dcbi	0,r3
	addi	r3,r3,64
	bdnz	1b
	sync				/* wait for dcbi's to get to ram */
	blr
	
	.section .text,"ax"
    .align	2
	.global .flush_dcache_range
.flush_dcache_range:
	li	r5,63
	andc	r3,r3,r5
	subf	r4,r3,r4
	add	r4,r4,r5
	rlwinm. r4,r4,26,6,31
	beqlr
	mtctr	r4

1:	dcbf	0,r3
	addi	r3,r3,64
	bdnz	1b
	sync				/* wait for dcbst's to get to ram */
	blr	

#define use_lock	
	
	.section .text,"ax"
    .align	2
	.global .spin_lock
.spin_lock:
#ifdef use_lock
	li      r10,1
2:	lwarx   r9,0,r3
	cmpwi   r9,0
	bne-    1f
	stwcx.  r10,0,r3
	bne-    2b
	isync
1:	cmpwi   cr7,r9,0
	beqlr+  cr7
3:	mr      r1,r1
	lwz     r9,0(r3)
	cmpwi   cr7,r9,0
	bne-    cr7,3b
	mr      r2,r2
6:	lwarx   r9,0,r3
	cmpwi   r9,0
	bne-    5f
	stwcx.  r10,0,r3
	bne-    6b
	isync
5:	cmpwi   cr7,r9,0
	bne-    cr7,3b
	blr
#else
	blr
#endif
	nop
	
	.section .text,"ax"
    .align	2
	.global .spin_unlock
.spin_unlock:
#ifdef use_lock
	sync    
	li      r10,0
	li      r4,512
	stw     r10,0(r3)
	mflr    r3
	blr
#else
	blr
#endif
	nop
	nop
	
	.section .text,"ax"
    .align	2
	.global .sync_l1l2cache_range
.sync_l1l2cache_range:
	li	r5,63
	andc	r3,r3,r5
	subf	r4,r3,r4
	add	r4,r4,r5
	rlwinm. r4,r4,26,6,31
	beqlr
	mtctr	r4

1:	icbi	0,r3
	dcbi	0,r3
	addi	r3,r3,64
	bdnz	1b
	sync				/* wait for dcbst's to get to ram */
	blr	
	
	.section .text,"ax"
    .align	2
	.global .relative_toc_rom
.relative_toc_rom:
	mflr	r0
	bcl	20,31,$+4
0:	mflr	r11
	ld	r2,(p_toc_rom - 0b)(r11)
	#add	r2,r2,r11
	mtlr	r0
	blr

.balign 8
p_toc_rom:	.llong	__toc_start_rom + 0x8000

	.section .text,"ax"
    .align	2
	.global .relative_toc_ram
.relative_toc_ram:
	mflr	r0
	bcl	20,31,$+4
0:	mflr	r11
	ld	r2,(p_toc_ram - 0b)(r11)
	#add	r2,r2,r11
	mtlr	r0
	blr

.balign 8
p_toc_ram:	.llong	__toc_start_ram + 0x8000

	.section .text,"ax"
    .align	4
	.global .e500mc_set_tlb_cpu2_linux


	lis       r5,  __ebss@highest
    ori       r5, r5, __ebss@higher
    sldi      r5, r5, 32
    oris      r5, r5, __ebss@h
    ori       r5, r5, __ebss@l

.e500mc_set_tlb_cpu2_linux:

##==================================Following TLB Config should be set for Normal BOOT Begin=================
#ROM 64KB
  mflr r9
  li32  r3,0x10070000 # TLB1
  
  li r4,0
  oris  r4,r4,0xc000
  ori	r4,r4,0x0a00
  
  #li32  r5,0xffff000a # EPN[31:12], X0, X1, WIMGE
  lis r5,0xc000
  sldi r5,r5,32
  ori r5,r5,0x0004@l
 
  
  li  r6,0x003f # RPN[31:12], {U0-U3}=0, {UX, SX, UW, SW, UR, SR}=6'b111111
  
  li  r7,0x00000000 # SGS=0, SLPID=1
  li  r8,0x00000000 # RPN[35:32]
  bl    e500mc_SetTLB
  isync
  mtlr r9
  blr
  
  	.align 4
	.global .JUMP_FUC
.JUMP_FUC:
	mr r10,r3
	li r3,1
	li r4,0
	mtlr r10
	blr
	b .

	.section .text,"ax"
    .align	4
    .global ._ExitProcess
._ExitProcess:
	b .

