
#
#  PPC460 demonstration interrupt handlers
#
#  The idea here is to let C code handle interrupts.  We do this by
#  providing assembly wrappers for each interrupt that save all the
#  registers, then call the C function.
#

	.file	"intr.s"
	.include "e500mc.i"
	.include "macros.i"
    	.include "mpic.i"	

	.section        .control,"ax"	
	
	.macro mfcpun
	mfspr r3, 286
	.endm
	
# save/restore all necessary registers to/from stack.
	.macro	SAVE_LR	
	#stwu    r1,	-80(r1)  # create a new stack
	# -80 is not enough. max usage of r1 is stw r1+76 (mfxer), will overwrite some regs
	stwu    r1,	-160(r1)  # create a new stack
#	subi    r1,	r1, 80  # create a new stack
	stw     r0,	48(r1)	# save r0 to stack
	mflr	r0
	stw		r0,	52(r1)	# save LR to stack
	stw		r6,	72(r1)	# save r6
	stw		r12,96(r1)
	.endm

	.macro	SAVE_REGS	
	stw		r3,	60(r1)	# save r3~r12 to stack
	stw		r4,	64(r1)
	stw		r5,	68(r1)
#	stw		r6,	40(r1)
	stw		r7,	76(r1)
	stw		r8,	80(r1)
	stw		r9,	84(r1)
	stw		r10,88(r1)
	stw		r11,92(r1)
#stw		r12,64(r1)  #save R12 in SAVE_LR.
	mfcr	r0
	stw		r0,	100(r1)	# save CR to stack
	mfctr	r0
	stw		r0,	104(r1)	# save CTR to stack
	mfxer	r0
	stw		r0,	108(r1)	# save XER to stack
	.endm

	.macro	REST_REGS
	lwz		r0,108(r1)	# restore XER from stack
	mtxer	r0
	lwz		r0,104(r1)	# restore CTR from stack
	mtctr	r0
	lwz		r0,100(r1)	# restore CR from stack
	mtcr	r0
	lwz		r12,96(r1)	# restore r2~r12 from stack
	lwz		r11,92(r1)
	lwz		r10,88(r1)
	lwz		r9,	84(r1)		
	lwz		r8,	80(r1)
	lwz		r7,	76(r1)
	lwz		r6,	72(r1)
	lwz		r5,	68(r1)
	lwz		r4,	64(r1)
	lwz		r3,	60(r1)
	.endm

	.macro	REST_LR
	lwz		r0,52(r1)	# restore LR from stack
	mtlr	r0
	lwz     r0,48(r1)	# restore r0 from stack
	#stwu    r1,80(r1)	# remove current stack
	stwu    r1,160(r1)	# remove current stack -- expand
#	addi    r1,r1,80	# remove current stack
	.endm

	.section .vectors, "ax"
PUSH:
	SAVE_REGS
	blr

POP:
	REST_REGS
	blr
	
## ------- #0: critical interrupt pre-handler -------{{{ 
#	.align 4
#	.global _CINTR_PREHANDLER
#_CINTR_PREHANDLER:
#	CHECK_HEAR
#	SAVE_LR
#    bl PUSH

#    li32   r4,vec_table0

#	lwz	r5,(vecNumCritic<<3)(r4)	 # 172 nonctritical interrupt 
#    	mtctr   r5
#    	bctrl
#    	bl POP
#	REST_LR
#    	rfci 

	.align 4
	.global _CINTR_PREHANDLER
_CINTR_PREHANDLER:
	SAVE_LR
    	bl PUSH
    mfspr r3, 1023
    cmpwi r3,0
    beq   Critical_vec_table0
    cmpwi  r3, 0x1
    beq   Critical_vec_table1
    cmpwi  r3, 0x2
    beq   Critical_vec_table2
    li32   r4, vec_table3
    beq    Critical_jump_over
Critical_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    Critical_jump_over
Critical_vec_table1:
    li32   r4,vec_table1
    beq    Critical_jump_over
Critical_vec_table2:
    li32   r4,vec_table2
    beq    Critical_jump_over
Critical_jump_over:
#	lwz	r5,(vecNumCritic<<3)(r4)	 # 172 nonctritical interrupt 
	li32	r6,0xc0
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
	
	
    	mtctr   r5
    	bctrl
    	bl POP
	REST_LR
    	rfi 
## ------- ------- ------- }}}    
#
## ------- #1: machine check pre-handler ------- {{{

#	CHECK_HEAR
/*
	.align 4 
	.global _MC_PREHANDLER
_MC_PREHANDLER:
	SAVE_LR
    bl	PUSH
 
    li32   r4,vec_table0
 
    lwz	r5,(vecNumMCheck<<3)(r4)       #
    mtctr   r5
    bctrl
	li32	r0, (MCSR_MSC_MASK | MCSR_NMI_MASK)
	mtspr	MCSR,r0 	# clear MCSR all status bits
	#mtspr	MCSR_C,r0	# clear MCSR all status bits
	bl	POP
	REST_LR
	rfmci 
*/
	.align 4 
	.global _MC_PREHANDLER
_MC_PREHANDLER:
	SAVE_LR
    bl	PUSH
    mfspr r3, 1023
    cmpwi r3,0
    beq   MachCheck_vec_table0
    cmpwi  r3, 0x1
    beq   MachCheck_vec_table1
    cmpwi  r3, 0x2
    beq   MachCheck_vec_table2
    li32   r4, vec_table3
    beq    MachCheck_jump_over
MachCheck_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    MachCheck_jump_over
MachCheck_vec_table1:
    li32   r4,vec_table1
    beq    MachCheck_jump_over
MachCheck_vec_table2:
    li32   r4,vec_table2
    beq    MachCheck_jump_over
MachCheck_jump_over:    
#  	lwz	r5,(vecNumMCheck<<3)(r4)      
   	li32	r6,0xc1
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
  
    mtctr   r5
    bctrl
	li32	r0, (MCSR_MSC_MASK | MCSR_NMI_MASK)
	mtspr	MCSR,r0 	# clear MCSR all status bits
	#mtspr	MCSR_C,r0	# clear MCSR all status bits
	bl	POP
	REST_LR
	rfmci 
## ------- ------- ------- }}}

# ------- #2: data storage pre-handler ------- {{{

#	.align 4 
#	.global _DSIDE_PREHANDLER	
#_DSIDE_PREHANDLER:
#	CHECK_HEAR
#	# (MMU permission violation, user mode cache manipulation)
#	SAVE_LR
#	bl PUSH

#    li32   r4,vec_table0

#    lwz	r5,(vecNumDSide<<3)(r4) # get handler entrance addr 
#    mtctr   r5					# jump to handler
#    bctrl
#	bl POP 
#	REST_LR  
#	rfi 

#	.align 4 
#	.global _DSIDE_PREHANDLER	
#_DSIDE_PREHANDLER:
#	# (MMU permission violation, user mode cache manipulation)
#	SAVE_LR
#	bl PUSH

#    mfspr r3, 1023
#    cmpwi r3,0
#    beq   Data_jump_vec_table0
#    cmpwi  r3, 0x1
#    beq   Data_jump_vec_table1
#    cmpwi  r3, 0x2
#    beq   Data_jump_vec_table2
#    li32   r4, vec_table3
#    beq    Data_jump_over
#Data_jump_vec_table0:
#    li32   r4,vec_table0
#	lis 	 r4,	vec_table0@highest 
#    ori 	 r4,	 r4,	vec_table0@higher 
#    sldi  	 r4,	 r4, 32 
#    oris  	 r4,	r4, vec_table0@h 
#    ori	 	 r4,	r4, vec_table0@l    
#    beq    Data_jump_over
#Data_jump_vec_table1:
#    li32   r4,vec_table1
#    beq    Data_jump_over
#Data_jump_vec_table2:
#    li32   r4,vec_table2
#    beq    Data_jump_over
#Data_jump_over:
#    lwz	r5,(vecNumDSide<<3)(r4) # get handler entrance addr 
#   	li32	r6,0xc2
#	slwi	r6,r6,0x3	# vector <<3
#    ldx	r5,r4,r6
#    ld  r5,0(r5)
  
#    mtctr   r5					# jump to handler
#    bctrl
#	bl POP 
#	REST_LR  
#	rfi 
# ------- ------- ------- }}}    

# ------- #3: instruction storage pre-handler ------- {{{
# fetch from non-executable page

#	.align 4 
#	.global _ISIDE_PREHANDLER
#_ISIDE_PREHANDLER:
#	CHECK_HEAR
#	SAVE_LR
#    bl PUSH
#
#    li32   r4,vec_table0

#    lwz	r5,(vecNumISide<<3)(r4)	
#    mtctr   r5
#    bctrl
#    bl POP
#	REST_LR
#	rfi 

#	.align 4 
#	.global _ISIDE_PREHANDLER
#_ISIDE_PREHANDLER:
#	SAVE_LR
#    bl PUSH

#    mfspr r3, 1023
#    cmpwi r3,0
#    beq   Ifetch_jump_vec_table0
#    cmpwi  r3, 0x1
#    beq   Ifetch_jump_vec_table1
#    cmpwi  r3, 0x2
#    beq   Ifetch_jump_vec_table2
#    li32   r4, vec_table3
#    beq    Ifetch_jump_over
#Ifetch_jump_vec_table0:
#    li32   r4,vec_table0
#	lis 	 r4,	vec_table0@highest 
#    ori 	 r4,	 r4,	vec_table0@higher 
#    sldi  	 r4,	 r4, 32 
#    oris  	 r4,	r4, vec_table0@h 
#    ori	 	 r4,	r4, vec_table0@l    
#    beq    Ifetch_jump_over
#Ifetch_jump_vec_table1:
#    li32   r4,vec_table1
#    beq    Ifetch_jump_over
#Ifetch_jump_vec_table2:
#    li32   r4,vec_table2
#    beq    Ifetch_jump_over

#Ifetch_jump_over:   
#    lwz	r5,(vecNumISide<<3)(r4)	
#   	li32	r6,0xc3
#	slwi	r6,r6,0x3	# vector <<3
#    ldx	r5,r4,r6
#    ld  r5,0(r5)
  
#    mtctr   r5
#    bctrl
#    bl POP
#	REST_LR
#	rfi 
# ------- ------- ------- }}}    

# ------- #4: non-critical interrupt pre-handler ------- {{{
	.align 4
	.global _NCINTR_PREHANDLER
_NCINTR_PREHANDLER:
	#CHECK_HEAR
	SAVE_LR
	li32	r12,MPIC_NCIAR0
 	lwz     r6, 0(r12) # read vector, as soon as possible

    SAVE_REGS
 
    li32   r4,vec_table0
  
	mtspr   SPRG6,	r6	# save vector number to SPRG6
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
    mtctr   r5
		isync
    bctrl
#	li32	r5,MPIC_NCEOI
#	li		r4,0x0
#	mtdcrx	r5,r4	# tell MPIC the intr handler has finished
    li32  r5, MPIC_NCEOI0
    li	  r4, 0x0
    stw   r4, 0(r5)

    REST_REGS
	REST_LR
	rfi 
# ------- ------- ------- }}}    

# ------- #5: un-alignment pre-handler ------- {{{
/*
	.align 4
	.global _UNALIGN_PREHANDLER
_UNALIGN_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
	bl	PUSH

    li32   r4,vec_table0

    lwz	r5,(vecNumAlign<<3)(r4)	# get handler entrance addr
    mtctr   r5					# jump to handler
    bctrl
    bl POP
	REST_LR
    rfi 
*/
	.align 4
	.global _UNALIGN_PREHANDLER
_UNALIGN_PREHANDLER:
	SAVE_LR
	bl	PUSH

    mfspr r3, 1023
    cmpwi r3,0
    beq   Unalign_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Unalign_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Unalign_jump_vec_table2
    li32   r4, vec_table3
    beq    Unalign_jump_over
Unalign_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    Unalign_jump_over
Unalign_jump_vec_table1:
    li32   r4,vec_table1
    beq    Unalign_jump_over
Unalign_jump_vec_table2:
    li32   r4,vec_table2
    beq    Unalign_jump_over

Unalign_jump_over:    
#    lwz	r5,(vecNumAlign<<3)(r4)	# get handler entrance addr
    li32	r6,0xc5
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
 
    mtctr   r5					# jump to handler
    bctrl
    bl POP
	REST_LR
    rfi 
# ------- ------- ------- }}}    

# ------- #6: program pre-handler ------- {{{
# illegal instruction or trap
/*
	.align 4
	.global _PROGRAM_PREHANDLER
_PROGRAM_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
    bl PUSH

    li32   r4,vec_table0
 
    lwz	r5,(vecNumProgram<<3)(r4)	
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
*/
	.align 4
	.global _PROGRAM_PREHANDLER
_PROGRAM_PREHANDLER:
	SAVE_LR
    bl PUSH

    mfspr r3, 1023
    cmpwi r3,0
    beq   Illegal_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Illegal_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Illegal_jump_vec_table2
    li32   r4, vec_table3
    beq    Illegal_jump_over
Illegal_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l     
    beq    Illegal_jump_over
Illegal_jump_vec_table1:
    li32   r4,vec_table1
    beq    Illegal_jump_over
Illegal_jump_vec_table2:
    li32   r4,vec_table2
    beq    Illegal_jump_over

Illegal_jump_over:    
#    lwz	r5,(vecNumProgram<<3)(r4)	
    li32	r6,0xc6
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)

    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
# ------- ------- ------- }}}   

# ------- #7: FP unavailable pre-handler ------- {{{
/*
	.align 4
	.global _FPU_PREHANDLER
_FPU_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
    bl PUSH
    mfspr r3, PIR


    li32   r4,vec_table0
 
	lwz	r5,(vecNumFP<<3)(r4)	
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
*/
	.align 4
	.global _FPU_PREHANDLER
_FPU_PREHANDLER:
	SAVE_LR
    bl PUSH
    mfspr r3, PIR

    mfspr r3, 1023
    cmpwi r3,0
    beq   Floating_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Floating_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Floating_jump_vec_table2
    li32   r4, vec_table3
    beq    Floating_jump_over
Floating_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    Floating_jump_over
Floating_jump_vec_table1:
    li32   r4,vec_table1
    beq    Floating_jump_over
Floating_jump_vec_table2:
    li32   r4,vec_table2
    beq    Floating_jump_over

Floating_jump_over:    
#	lwz	r5,(vecNumFP<<3)(r4)	
	li32	r6,0xc7
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
	
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
# ------- ------- ------- }}}   

# ------- #8: system call pre-handler ------- {{{
/*
	.align 4
	.global _SC_PREHANDLER	
_SC_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
    SAVE_REGS

    li32   r4,vec_table0
   
	lwz	r5,(vecNumSCall<<3)(r4)	
    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfi 
*/
	.align 4
	.global _SC_PREHANDLER	
_SC_PREHANDLER:
	SAVE_LR
    SAVE_REGS

    mfspr r3, 1023
    cmpwi r3,0
    beq   SC_jump_vec_table0
    cmpwi  r3, 0x1
    beq   SC_jump_vec_table1
    cmpwi  r3, 0x2
    beq   SC_jump_vec_table2
    li32   r4, vec_table3
    beq    SC_jump_over
SC_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    SC_jump_over
SC_jump_vec_table1:
    li32   r4,vec_table1
    beq    SC_jump_over
SC_jump_vec_table2:
    li32   r4,vec_table2
    beq    SC_jump_over

SC_jump_over:    
#	lwz	r5,(vecNumSCall<<3)(r4)	
	li32	r6,0xc8
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
	
    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfi 

# ------- ------- ------- }}}    

# ------- #9: APU unavailable pre-handler ------- {{{
/*
	.align 4
	.global _APU_PREHANDLER
_APU_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
    bl PUSH

    li32   r4,vec_table0

	lwz	r5,(vecNumAPU<<3)(r4)
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
*/
	.align 4
	.global _APU_PREHANDLER
_APU_PREHANDLER:
	SAVE_LR
    bl PUSH

    mfspr r3, 1023
    cmpwi r3,0
    beq   APU_jump_vec_table0
    cmpwi  r3, 0x1
    beq   APU_jump_vec_table1
    cmpwi  r3, 0x2
    beq   APU_jump_vec_table2
    li32   r4, vec_table3
    beq    APU_jump_over
APU_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    APU_jump_over
APU_jump_vec_table1:
    li32   r4,vec_table1
    beq    APU_jump_over
APU_jump_vec_table2:
    li32   r4,vec_table2
    beq    APU_jump_over

APU_jump_over:    
#	lwz	r5,(vecNumAPU<<3)(r4)
	li32	r6,0xc9
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
	
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
# ------- ------- ------- }}}    

# ------- #10: decrementer timer pre-handler ------- {{{	
/*
	.align 4
	.global _DEC_PREHANDLER
_DEC_PREHANDLER:
	#CHECK_HEAR
	SAVE_LR
    SAVE_REGS

    li32   r4,vec_table0
 
	lwz	r5,(vecNumDEC<<3)(r4)
    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfi 
*/
# ------- ------- ------- }}}  
# ------- #10: decrementer timer pre-handler ------- {{{	
	.align 4
	.global _DEC_PREHANDLER
_DEC_PREHANDLER:
	SAVE_LR
    SAVE_REGS

    mfspr r3, 1023
    cmpwi r3,0
    beq   Decrement_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Decrement_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Decrement_jump_vec_table2
    li32   r4, vec_table3
    beq    Decrement_jump_over 
Decrement_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l 
    beq    Decrement_jump_over
Decrement_jump_vec_table1:
 #   li32   r4,vec_table1
    beq    Decrement_jump_over
Decrement_jump_vec_table2:
 #   li32   r4,vec_table2
    beq    Decrement_jump_over

Decrement_jump_over:    
# 	lwz	r5,(0xca<<3)(r4)
	li32	r6,0xca
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
    
    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfi 
# ------- #11: fixed interval timer pre-handler ------- {{{
/*
	.align 4
	.global _FIT_PREHANDLER
_FIT_PREHANDLER:
	#CHECK_HEAR
	SAVE_LR
    SAVE_REGS

    li32   r4,vec_table0

	lwz	r5,(vecNumFIT<<3)(r4)
    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfi 
*/
# ------- #11: fixed interval timer pre-handler ------- {{{
	.align 4
	.global _FIT_PREHANDLER
_FIT_PREHANDLER:
	SAVE_LR
    SAVE_REGS

    mfspr r3, 1023
    cmpwi r3,0
    beq   FIT_jump_vec_table0
    cmpwi  r3, 0x1
    beq   FIT_jump_vec_table1
    cmpwi  r3, 0x2
    beq   FIT_jump_vec_table2
    li32   r4, vec_table3
    beq    FIT_jump_over
FIT_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l 
    beq    FIT_jump_over
FIT_jump_vec_table1:
 #   li32   r4,vec_table1
    beq    FIT_jump_over
FIT_jump_vec_table2:
 #   li32   r4,vec_table2
    beq    FIT_jump_over

FIT_jump_over:    
#	lwz	r5,(vecNumFIT<<3)(r4)
	li32	r6,0xcb
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)

    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfi
# ------- ------- ------- }}}    

# ------- #12: watchdog timer pre-handler ------- {{{
/*
	.align 4
	.global _WTDOG_PREHANDLER
_WTDOG_PREHANDLER:
	#CHECK_HEAR
	SAVE_LR
    SAVE_REGS

    li32   r4,vec_table0

	lwz	r5,(vecNumWTDog<<3)(r4)	
    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfci 
*/
	.align 4
	.global _WTDOG_PREHANDLER
_WTDOG_PREHANDLER:
	SAVE_LR
    SAVE_REGS

    mfspr r3, 1023
    cmpwi r3,0
    beq   Watchdog_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Watchdog_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Watchdog_jump_vec_table2
    li32   r4, vec_table3
    beq    Watchdog_jump_over
Watchdog_jump_vec_table0:
#   li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l 
    beq    Watchdog_jump_over
Watchdog_jump_vec_table1:
 #   li32   r4,vec_table1
    beq    Watchdog_jump_over
Watchdog_jump_vec_table2:
 #   li32   r4,vec_table2
    beq    Watchdog_jump_over

Watchdog_jump_over:
#	lwz	r5,(vecNumWTDog<<3)(r4)	
	li32	r6,0xcc
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)

    mtctr   r5
    bctrl
    REST_REGS
	REST_LR
    rfci 
# ------- ------- ------- }}}    

# ------- #13: data TLB miss pre-handler ------- {{{
/*
	.align 4
	.global _DTLB_PREHANDLER
_DTLB_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
	bl PUSH	

    li32   r4,vec_table0
 
	lwz	r5,(vecNumDTLB<<3)(r4)
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
*/
	.align 4
	.global _DTLB_PREHANDLER
_DTLB_PREHANDLER:
	SAVE_LR
	bl PUSH	

    mfspr r3, 1023
    cmpwi r3,0
    beq   Data_TLB_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Data_TLB_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Data_TLB_jump_vec_table2
    li32   r4, vec_table3
    beq    Data_TLB_jump_over
Data_TLB_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    Data_TLB_jump_over
Data_TLB_jump_vec_table1:
 #   li32   r4,vec_table1
    beq    Data_TLB_jump_over
Data_TLB_jump_vec_table2:
 #   li32   r4,vec_table2
    beq    Data_TLB_jump_over

Data_TLB_jump_over:    
#	lwz	r5,(vecNumDTLB<<3)(r4)
	li32	r6,0xcd
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)

    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
# ------- ------- ------- }}}   

# ------- #14: instruction TLB miss pre-hanlder ------- {{{
/*
	.align 4 
	.global _ITLB_PREHANDLER
_ITLB_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
    bl PUSH

    li32   r4,vec_table0
  
	lwz	r5,(vecNumITLB<<3)(r4)
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi
*/
	.align 4 
	.global _ITLB_PREHANDLER
_ITLB_PREHANDLER:
	SAVE_LR
    bl PUSH

    mfspr r3, 1023
    cmpwi r3,0
    beq   Instr_TLB_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Instr_TLB_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Instr_TLB_jump_vec_table2
    li32   r4, vec_table3
    beq    Instr_TLB_jump_over
Instr_TLB_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    Instr_TLB_jump_over
Instr_TLB_jump_vec_table1:
 #   li32   r4,vec_table1
    beq    Instr_TLB_jump_over
Instr_TLB_jump_vec_table2:
 #   li32   r4,vec_table2
    beq    Instr_TLB_jump_over

Instr_TLB_jump_over:    
#	lwz	r5,(vecNumITLB<<3)(r4)
	li32	r6,0xce
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)

    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
# ------- ------- ------- }}}  

# ------- #15: internal debug pre-handler ------- {{{
/*
	.align 4
	.global _DEBUG_PREHANDLER
_DEBUG_PREHANDLER:
	CHECK_HEAR
	SAVE_LR
    bl PUSH

    li32   r4,vec_table0
 
	lwz	r5,(vecNumDebug<<3)(r4)		
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfci 
*/
	.align 4
	.global _DEBUG_PREHANDLER
_DEBUG_PREHANDLER:
	SAVE_LR
    bl PUSH

    mfspr r3, 1023
    cmpwi r3,0
    beq   Debug_jump_vec_table0
    cmpwi  r3, 0x1
    beq   Debug_jump_vec_table1
    cmpwi  r3, 0x2
    beq   Debug_jump_vec_table2
    li32   r4, vec_table3
    beq    Debug_jump_over
Debug_jump_vec_table0:
#    li32   r4,vec_table0
	lis 	 r4,	vec_table0@highest 
    ori 	 r4,	 r4,	vec_table0@higher 
    sldi  	 r4,	 r4, 32 
    oris  	 r4,	r4, vec_table0@h 
    ori	 	 r4,	r4, vec_table0@l    
    beq    Debug_jump_over
Debug_jump_vec_table1:
 #   li32   r4,vec_table1
    beq    Debug_jump_over
Debug_jump_vec_table2:
 #   li32   r4,vec_table2
    beq    Debug_jump_over

Debug_jump_over:
#	lwz	r5,(vecNumDebug<<3)(r4)	
	li32	r6,0xcf
	slwi	r6,r6,0x3	# vector <<3
    ldx	r5,r4,r6
    ld  r5,0(r5)
	
    mtctr   r5
    bctrl
    bl POP
	REST_LR
    rfi 
# ------- ------- ------- }}}    


	
