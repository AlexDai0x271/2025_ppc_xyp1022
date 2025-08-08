.extern InterruptHandler	# void InterruptHandler(unsigned long exceptNumber)
.extern __reset

/***************************************************************************
 * Register names
 */
.equ r0,	0
.equ r1,	1
.equ rsp,	1
.equ r2,	2
.equ r3,	3
.equ r4,	4
.equ r5,	5
.equ r6,	6
.equ r7,	7
.equ r8,	8
.equ r9,	9
.equ r10,	10
.equ r11,	11
.equ r12,	12
.equ r13,	13
.equ r14,	14
.equ r15,	15
.equ r16,	16
.equ r17,	17
.equ r18,	18
.equ r19,	19
.equ r20,	20
.equ r21,	21
.equ r22,	22
.equ r23,	23
.equ r24,	24
.equ r25,	25
.equ r26,	26
.equ r27,	27
.equ r28,	28
.equ r29,	29
.equ r30,	30
.equ r31,	31

.globl gInterruptVectorTable, gInterruptVectorTableEnd	#, reset

.section	.intvec	,"awx"
set_msr: 	.macro				
				mfmsr r0
				ori r0, r0, 0x2000
				mtmsr r0
				.endm
isr_prologue_: 	.macro

				stdu	rsp,	-200(rsp)
				
				
				std		 r0,	 48(rsp)
				mfctr	 r0
				std		 r0,	 56(rsp)
				mfxer	 r0
				std		 r0,	 64(rsp)
				mfcr	 r0
				std		 r0,	 72(rsp)
				mflr	 r0
				std		 r0,	 80(rsp)
				std		 r3,	112(rsp)
				std		 r4,	120(rsp)
				std		 r5,	128(rsp)
				std		 r6,	136(rsp)
				std		 r7,	144(rsp)
				std		 r8,	160(rsp)
				std		 r9,	168(rsp)
				std		r10,	176(rsp)
				std		r11,	184(rsp)
				std		r12,	192(rsp)				
				
				.endm

isr_epilogue_: 	.macro

				ld		 r3,	112(rsp)
				ld		 r4,	120(rsp)
				ld		 r5,	128(rsp)
				ld		 r6,	136(rsp)
				ld		 r7,	144(rsp)
				ld		 r8,	160(rsp)
				ld		 r9,	168(rsp)
				ld		r10,	176(rsp)
				ld		r11,	184(rsp)
				ld		r12,	192(rsp)
				ld		 r0,	 80(rsp)				
				mtlr	 r0
				ld       r0,	 72(rsp)
				mtcrf    0xff,	 r0
				ld       r0,	 64(rsp)
				mtxer    r0
				ld       r0,	 56(rsp)
				mtctr    r0							
				ld       r0,	 48(rsp)
				
				addi	rsp,	 rsp,	200
				.endm
##############################################################################
#
#	isr_prologue / isr_epilogue
#
#	Saves / Restores the necessary registers for an interrupt service routine
#
##############################################################################
								
isr_prologue: 	.macro
				isr_prologue_								
				mfsrr0    r0
				std       r0,	 88(rsp)
				mfsrr1    r0
				std       r0,	 96(rsp)
				set_msr
				.endm

isr_epilogue: 	.macro
				ld       r0,	96(rsp)
				mtsrr1   r0
				ld       r0,	88(rsp)
				mtsrr0   r0
				isr_epilogue_
				rfi
				.endm

##############################################################################
#
#	cisr_prologue / cisr_epilogue
#
#	Saves / Restores the necessary registers for a critical interrupt service routine
#
##############################################################################

cisr_prologue: 	.macro
				isr_prologue_
				mfspr    r0,	 58
				std      r0,	 88(rsp)
				mfspr    r0,	 59
				std      r0,	 96(rsp)
				set_msr
				.endm


cisr_epilogue: 	.macro
				ld       r0,	 96(rsp)
				mtspr    59,	 r0
				ld       r0,	 88(rsp)
				mtspr    58,	 r0
				isr_epilogue_
				rfci
				.endm


##############################################################################
#
#	disr_prologue / disr_epilogue
#
#	Saves / Restores the necessary registers for a debug interrupt service routine
#
##############################################################################

disr_prologue: 	.macro
				isr_prologue_
				mfspr    r0,	574
				std      r0,	 88(rsp)
				mfspr    r0,	575
				std      r0,	 96(rsp)
				set_msr
				.endm

disr_epilogue: 	.macro
				ld       r0,	 96(rsp)
				mtspr    575,	 r0
				ld       r0,	 88(rsp)
				mtspr    574,	 r0
				isr_epilogue_
				rfdi
				.endm

##############################################################################
#
#	mcisr_prologue / misr_epilogue
#
#	Saves / Restores the necessary registers for a machine check interrupt service routine
#
##############################################################################

mcisr_prologue: 	.macro
				isr_prologue_
				mfspr	 r0,	570
				std      r0,	 88(rsp)
				mfspr    r0,	571
				std      r0,	 96(rsp)
				set_msr
				.endm

mcisr_epilogue: 	.macro
				ld       r0,	 96(rsp)
				mtspr    571,	 r0
				ld       r0,	 88(rsp)
				mtspr    570,	 r0
				isr_epilogue_
				rfmci
				.endm

gInterruptVectorTable:


##############################################################################
#
#	IVOR0 - 0x0100 critical

#
##############################################################################
		.org	0x100

		cisr_prologue

		li		r3,	0x0100
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
	    ld		r4, 0(r4)
		mtlr	r4
		blrl

		cisr_epilogue


##############################################################################
#
#	IVOR1 - 0x0200 Machine Check
#
##############################################################################
		.org 	0x200

        mcisr_prologue

		li		r3,	0x0200
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

        mcisr_epilogue


##############################################################################
#
#	IVOR2 - 0x0300 Data Storage
#
##############################################################################
		.org	0x300

		isr_prologue

		li		r3,	0x0300
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue

##############################################################################
#
#	IVOR3 - 0x0400 Instruction Storage
#
##############################################################################
		.org	0x400

		isr_prologue

		li		r3,	0x0400
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue

##############################################################################
#
#	IVOR4 - 0x0500 External Interrupt
#
##############################################################################
		.org	0x500

		isr_prologue

		li		r3,	0x0500
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue

##############################################################################
#
#	IVOR5 - 0x0600 Alignment
#
##############################################################################
		.org	0x600

		isr_prologue

		li		r3,	0x0600
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue

##############################################################################
#
#	IVOR6 - 0x0700 Program
#
##############################################################################
		.org	0x700

        nop
		isr_prologue

		li		r3,	0x0700
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue
		
##############################################################################
#
#	IVOR7 - 0x0800 Floating point unavailable
#
##############################################################################
		.org	0x800

		isr_prologue

		li		r3,	0x0800
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue
				
##############################################################################
#
#	IVOR10 - 0x0900 Decrementer
#
##############################################################################
		.org	0x900

		isr_prologue

		li		r3,	0x0900
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue

##############################################################################
#
#	IVOR12 - 0x0B00 Watchdog timer interrupt
#
##############################################################################
		.org	0xB00

		cisr_prologue

		li		r3,	0x0B00
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		cisr_epilogue
		
##############################################################################
#
#	IVOR8 - 0x0C00 System Call
#
##############################################################################
		.org	0xC00

		isr_prologue

		li		r3,	0x0C00
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue		

##############################################################################
#
#	IVOR11 - 0x0F00 Fixed interval timer
#
##############################################################################
		.org	0xF00

		isr_prologue

		li		r3,	0x0F00
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue

		
##############################################################################
#
#	IVOR14 - 0x1000 	Instruction TLB Miss
#
##############################################################################
		.org	0x1000

		isr_prologue

		li		r3,	0x1000
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue
		
##############################################################################
#
#	IVOR13 - 0x1100 	Data TLB Miss
#
##############################################################################
		.org	0x1100

		isr_prologue

		li		r3,	0x1100
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		isr_epilogue

##############################################################################
#
#	IVOR15 - 0x1500	Debug
#
##############################################################################
		.org	0x1500

        nop
		disr_prologue

		li		r3,	0x1500
		lis 	r4,	InterruptHandler@highest
		ori 	r4, r4,	InterruptHandler@higher
		sldi  	r4,	r4, 32
		oris  	r4,	r4, InterruptHandler@h
	    ori		r4,	r4, InterruptHandler@l
		ld		r4, 0(r4)	    
		mtlr	r4
		blrl

		disr_epilogue

gInterruptVectorTableEnd:
