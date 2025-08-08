/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

    .include "e500mc.i"
    .include "macros.i"
    .include "mpic.i"

    .extern pxCurrentTCB
    .extern xPortSysTickHandler

    .extern systick_reinit

    .extern vec_table0

    .global vPortStartFirstTask
    .global .vPortYieldISR
    .global .vPortTickISR
    .global .vPortNormalISR

.macro portINIT_IRQ_STACK
    lis     r1, g_top_irqstack@highest
    ori     r1, r1, g_top_irqstack@higher
    sldi    r1, r1, 32
    oris    r1, r1, g_top_irqstack@h
    ori     r1, r1, g_top_irqstack@l

    lis     r11, g_sp_irqstack@highest
    ori     r11, r11, g_sp_irqstack@higher
    sldi    r11, r11, 32
    oris    r11, r11, g_sp_irqstack@h
    ori     r11, r11, g_sp_irqstack@l
    std     r1, 0(r11)
.endm

.macro portSWITCH_IN_STACK
    lis     r11, g_sp_irqstack@highest
    ori     r11, r11, g_sp_irqstack@higher
    sldi    r11, r11, 32
    oris    r11, r11, g_sp_irqstack@h
    ori     r11, r11, g_sp_irqstack@l
    ld      r1, 0(r11)

    addi    r1, r1, -32
.endm

.macro portSWITCH_OUT_STACK
    addi    r1, r1,  32

    lis     r11, g_sp_irqstack@highest
    ori     r11, r11, g_sp_irqstack@higher
    sldi    r11, r11, 32
    oris    r11, r11, g_sp_irqstack@h
    ori     r11, r11, g_sp_irqstack@l
    std     r1, 0(r11)
.endm


.macro portSAVE_CONTEXT
    addi    r1, r1, -0x98
    stw     r0,  0x24(r1)

    stmw    r14, 0x50(r1)
    stw     r12, 0x4c(r1)
    stw     r11, 0x48(r1)
    stw     r10, 0x44(r1)
    stw     r9,  0x40(r1)
    stw     r8,  0x3c(r1)
    stw     r7,  0x38(r1)
    stw     r6,  0x34(r1)
    stw     r5,  0x30(r1)
    stw     r4,  0x2c(r1)
    stw     r3,  0x28(r1)

    mfxer   r0
    stw     r0,  0x20(r1)

    mfcr    r0
    stw     r0,  0x1c(r1)

    mflr    r0
    stw     r0,  0x18(r1)

    mfctr   r0
    stw     r0,  0x14(r1)

    mfsrr1  r0
    stw     r0,  0x10(r1)

    mfsrr0  r0
    stw     r0,  0x0c(r1)
.endm

.macro portRESTORE_CONTEXT
    lwz     r0,  0x0c(r1)
    mtsrr0  r0

    lwz     r0,  0x10(r1)
    mtsrr1  r0

    lwz     r0,  0x14(r1)
    mtctr   r0

    lwz     r0,  0x18(r1)
    mtlr    r0

    lwz     r0,  0x1c(r1)
    mtcr    r0

    lwz     r0,  0x20(r1)
    mtxer   r0

    lwz     r3,  0x28(r1)
    lwz     r4,  0x2c(r1)
    lwz     r5,  0x30(r1)
    lwz     r6,  0x34(r1)
    lwz     r7,  0x38(r1)
    lwz     r8,  0x3c(r1)
    lwz     r9,  0x40(r1)
    lwz     r10, 0x44(r1)
    lwz     r11, 0x48(r1)
    lwz     r12, 0x4c(r1)
    lmw     r14, 0x50(r1)

    lwz     r0,  0x24(r1)
    addi    r1, r1, 0x98
 .endm

 .macro portPUSH_TASK
    /* Get the address of the TCB. */
    lis     r11, pxCurrentTCB@highest
    ori     r11, r11, pxCurrentTCB@higher
    sldi    r11, r11, 32
    oris    r11, r11, pxCurrentTCB@h
    ori     r11, r11, pxCurrentTCB@l
    ld      r11, 0(r11)

    /* Store the stack pointer into the TCB */
    std     r1, 0(r11)
.endm

.macro portPOP_TASK
    /* Get the address of the TCB. */
    lis     r11, pxCurrentTCB@highest
    ori     r11, r11, pxCurrentTCB@higher
    sldi    r11, r11, 32
    oris    r11, r11, pxCurrentTCB@h
    ori     r11, r11, pxCurrentTCB@l
    ld      r11, 0(r11)

    /* Get the task stack pointer from the TCB. */
    ld      r1, 0(r11)
.endm


    .section .text
    .align 4

vPortStartFirstTask:

    portINIT_IRQ_STACK

    portPOP_TASK
    portRESTORE_CONTEXT

    addi r1, r1, -128

    bl systick_reinit

    /* Return into the first task */
    rfi


    .section .vectors, "ax"
    .align 4

.vPortYieldISR:

    portSAVE_CONTEXT
    portPUSH_TASK

    portSWITCH_IN_STACK

    bl vTaskSwitchContext

    portSWITCH_OUT_STACK

    portPOP_TASK
    portRESTORE_CONTEXT

    rfi


    .section .vectors, "ax"
    .align 4

.vPortTickISR:

    portSAVE_CONTEXT
    portPUSH_TASK

    /* Clear the interrupt */
    lis r0, 2048
    mttsr r0

    portSWITCH_IN_STACK

    bl xPortSysTickHandler
    bl systick_reinit

    portSWITCH_OUT_STACK

    portPOP_TASK
    portRESTORE_CONTEXT

    rfi


    .section .vectors, "ax"
    .align 4

.vPortNormalISR:

    portSAVE_CONTEXT
    portPUSH_TASK

    portSWITCH_IN_STACK

    li32    r12, MPIC_NCIAR0
    lwz     r6, 0(r12)

    mtspr   SPRG6, r6
    slwi    r6, r6, 0x3
    li32    r4, vec_table0
    ldx     r5, r4, r6
    ld      r5, 0(r5)
    mtctr   r5
    isync

    bctrl

    li32    r5, MPIC_NCEOI0
    li      r4, 0x0
    stw     r4, 0(r5)

    portSWITCH_OUT_STACK

    portPOP_TASK
    portRESTORE_CONTEXT

    rfi


    .section .bss
    .align 3

    .global g_sp_irqstack
    .global g_base_irqstack
    .global g_top_irqstack
g_sp_irqstack:
    .skip 8
g_base_irqstack:
    .skip 1024
g_top_irqstack:
