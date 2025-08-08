/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/********************************************************************
 * Functions: vPortYield
 *
 ********************************************************************/
.global vPortYield
.type   vPortYield, %function
vPortYield:
    li      t0, 0xE080100C
    lb      t1, (t0)
    li      t2, 0x01
    or      t1, t1, t2
    sb      t1, (t0)

    ret

/********************************************************************
 * Functions: vPortStartTask
 *
 ********************************************************************/
.global vPortStartTask
.type   vPortStartTask, %function
vPortStartTask:
    la      a0, pxCurrentTCB
    lw      a0, (a0)
    lw      sp, (a0)

    /* Run in machine mode */
    li      t0, 0x1880
    csrs    mstatus, t0

    lw      t0,  (30*4)(sp)
    csrw    mepc, t0

    lw      x1,  ( 0*4)(sp)
    lw      x4,  ( 2*4)(sp)
    lw      x5,  ( 3*4)(sp)
    lw      x6,  ( 4*4)(sp)
    lw      x7,  ( 5*4)(sp)
    lw      x8,  ( 6*4)(sp)
    lw      x9,  ( 7*4)(sp)
    lw      x10, ( 8*4)(sp)
    lw      x11, ( 9*4)(sp)
    lw      x12, (10*4)(sp)
    lw      x13, (11*4)(sp)
    lw      x14, (12*4)(sp)
    lw      x15, (13*4)(sp)
    lw      x16, (14*4)(sp)
    lw      x17, (15*4)(sp)
    lw      x18, (16*4)(sp)
    lw      x19, (17*4)(sp)
    lw      x20, (18*4)(sp)
    lw      x21, (19*4)(sp)
    lw      x22, (20*4)(sp)
    lw      x23, (21*4)(sp)
    lw      x24, (22*4)(sp)
    lw      x25, (23*4)(sp)
    lw      x26, (24*4)(sp)
    lw      x27, (25*4)(sp)
    lw      x28, (26*4)(sp)
    lw      x29, (27*4)(sp)
    lw      x30, (28*4)(sp)
    lw      x31, (29*4)(sp)
    addi    sp, sp, (31*4)
    
    fld     f31, (0  + 0 )(sp)
    fld     f30, (4  + 4 )(sp)
    fld     f29, (8  + 8 )(sp)
    fld     f28, (12 + 12)(sp)
    fld     f27, (16 + 16)(sp)
    fld     f26, (20 + 20)(sp)
    fld     f25, (24 + 24)(sp)
    fld     f24, (28 + 28)(sp)
    fld     f23, (32 + 32)(sp)
    fld     f22, (36 + 36)(sp)
    fld     f21, (40 + 40)(sp)
    fld     f20, (44 + 44)(sp)
    fld     f19, (48 + 48)(sp)
    fld     f18, (52 + 52)(sp)
    fld     f17, (56 + 56)(sp)
    fld     f16, (60 + 60)(sp)
    fld     f15, (64 + 64)(sp)
    fld     f14, (68 + 68)(sp)
    fld     f13, (72 + 72)(sp)
    fld     f12, (76 + 76)(sp)
    fld     f11, (80 + 80)(sp)
    fld     f10, (84 + 84)(sp)
    fld     f9,  (88 + 88)(sp)
    fld     f8,  (92 + 92)(sp)
    fld     f7,  (96 + 96)(sp)
    fld     f6,  (100+100)(sp)
    fld     f5,  (104+104)(sp)
    fld     f4,  (108+108)(sp)
    fld     f3,  (112+112)(sp)
    fld     f2,  (116+116)(sp)
    fld     f1,  (120+120)(sp)
    fld     f0,  (124+124)(sp)
    addi    sp, sp, (128+128)

    mret

/********************************************************************
 * Functions: TSPEND_IRQHandler
 *
 ********************************************************************/
.global TSPEND_IRQHandler
.type   TSPEND_IRQHandler, %function
TSPEND_IRQHandler:

    la      a1, pxCurrentTCB
    lw      a1, (a1)
    sw      sp, (a1)

	sw      ra, -4(sp)
	csrrw   sp, mscratch, sp
    jal     vTaskSwitchContext
	csrrw   sp, mscratch, sp
	lw      ra, -4(sp)

    la      a1, pxCurrentTCB
    lw      a1, (a1)
    lw      sp, (a1)

    ret
