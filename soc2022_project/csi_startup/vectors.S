/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "csi_config.h"

.section .bss
    .align  3
    .global g_base_trapstack
    .global g_top_trapstack
g_base_trapstack:
    .space CONFIG_ARCH_TRAPSTACK
g_top_trapstack:


/* __trig_trap, trig trap. */
.section .text
    .align  2
    .global __trig_trap
    .type   __trig_trap, %function
__trig_trap:
    li      t0, 0xE080100C
    lb      t1, (t0)
    li      t2, 0x01
    or      t1, t1, t2
    sb      t1, (t0)

    ret

/* Default_Handler, default exception service function. */
.section .text
    .align  6
    .weak   Default_Handler
    .global Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    addi    sp, sp, -8
    sw      t0, 0x0(sp)
    sw      t1, 0x4(sp)

    csrr    t0, mcause
    andi    t0, t0, 0x3FF
    li      t1, 24
    beq     t0, t1, .NMI_Handler

    lw      t0, 0x0(sp)
    lw      t1, 0x4(sp)
    addi    sp, sp, 8

    j       trap // NOTE: WON'T RETURN.

.NMI_Handler:
    lw      t0, 0x0(sp)
    lw      t1, 0x4(sp)
    addi    sp, sp, 8
    
    addi    sp, sp, -64
    sw      ra, 0(sp)
    sw      t0, 4(sp)
    sw      t1, 8(sp)
    sw      t2, 12(sp)
    sw      a0, 16(sp)
    sw      a1, 20(sp)
    sw      a2, 24(sp)
    sw      a3, 28(sp)
    sw      a4, 32(sp)
    sw      a5, 36(sp)
    sw      a6, 40(sp)
    sw      a7, 44(sp)
    sw      t3, 48(sp)
    sw      t4, 52(sp)
    sw      t5, 56(sp)
    sw      t6, 60(sp)

    addi    sp,  sp, -160
    fsd     ft0, (0 +0 )(sp)
    fsd     ft1, (4 +4 )(sp)
    fsd     ft2, (8 +8 )(sp)
    fsd     ft3, (12+12)(sp)
    fsd     ft4, (16+16)(sp)
    fsd     ft5, (20+20)(sp)
    fsd     ft6, (24+24)(sp)
    fsd     ft7, (28+28)(sp)
    fsd     fa0, (32+32)(sp)
    fsd     fa1, (36+36)(sp)
    fsd     fa2, (40+40)(sp)
    fsd     fa3, (44+44)(sp)
    fsd     fa4, (48+48)(sp)
    fsd     fa5, (52+52)(sp)
    fsd     fa6, (56+56)(sp)
    fsd     fa7, (60+60)(sp)
    fsd     ft8, (64+64)(sp)
    fsd     ft9, (68+68)(sp)
    fsd     ft10,(72+72)(sp)
    fsd     ft11,(76+76)(sp)

    la      t0, g_nmivector
    lw      t0, (t0)
    jalr    t0

    fld     ft0, (0 +0 )(sp)
    fld     ft1, (4 +4 )(sp)
    fld     ft2, (8 +8 )(sp)
    fld     ft3, (12+12)(sp)
    fld     ft4, (16+16)(sp)
    fld     ft5, (20+20)(sp)
    fld     ft6, (24+24)(sp)
    fld     ft7, (28+28)(sp)
    fld     fa0, (32+32)(sp)
    fld     fa1, (36+36)(sp)
    fld     fa2, (40+40)(sp)
    fld     fa3, (44+44)(sp)
    fld     fa4, (48+48)(sp)
    fld     fa5, (52+52)(sp)
    fld     fa6, (56+56)(sp)
    fld     fa7, (60+60)(sp)
    fld     ft8, (64+64)(sp)
    fld     ft9, (68+68)(sp)
    fld     ft10,(72+72)(sp)
    fld     ft11,(76+76)(sp)
    addi    sp, sp, 160

    lw      ra, 0(sp)
    lw      t0, 4(sp)
    lw      t1, 8(sp)
    lw      t2, 12(sp)
    lw      a0, 16(sp)
    lw      a1, 20(sp)
    lw      a2, 24(sp)
    lw      a3, 28(sp)
    lw      a4, 32(sp)
    lw      a5, 36(sp)
    lw      a6, 40(sp)
    lw      a7, 44(sp)
    lw      t3, 48(sp)
    lw      t4, 52(sp)
    lw      t5, 56(sp)
    lw      t6, 60(sp)
    addi    sp, sp, 64

    mret

/* Default_IRQHandler, default non-vector interrupt service function. */
.section .text
    .align  2
    .global Default_IRQHandler
    .weak   Default_IRQHandler
    .type   Default_IRQHandler, %function
Default_IRQHandler:
    addi    sp, sp, -(128+128)
    fsd     f31, (0  + 0 )(sp)
    fsd     f30, (4  + 4 )(sp)
    fsd     f29, (8  + 8 )(sp)
    fsd     f28, (12 + 12)(sp)
    fsd     f27, (16 + 16)(sp)
    fsd     f26, (20 + 20)(sp)
    fsd     f25, (24 + 24)(sp)
    fsd     f24, (28 + 28)(sp)
    fsd     f23, (32 + 32)(sp)
    fsd     f22, (36 + 36)(sp)
    fsd     f21, (40 + 40)(sp)
    fsd     f20, (44 + 44)(sp)
    fsd     f19, (48 + 48)(sp)
    fsd     f18, (52 + 52)(sp)
    fsd     f17, (56 + 56)(sp)
    fsd     f16, (60 + 60)(sp)
    fsd     f15, (64 + 64)(sp)
    fsd     f14, (68 + 68)(sp)
    fsd     f13, (72 + 72)(sp)
    fsd     f12, (76 + 76)(sp)
    fsd     f11, (80 + 80)(sp)
    fsd     f10, (84 + 84)(sp)
    fsd     f9,  (88 + 88)(sp)
    fsd     f8,  (92 + 92)(sp)
    fsd     f7,  (96 + 96)(sp)
    fsd     f6,  (100+100)(sp)
    fsd     f5,  (104+104)(sp)
    fsd     f4,  (108+108)(sp)
    fsd     f3,  (112+112)(sp)
    fsd     f2,  (116+116)(sp)
    fsd     f1,  (120+120)(sp)
    fsd     f0,  (124+124)(sp)

    addi    sp, sp, -(31*4)
    sw      x1,  ( 0*4)(sp)
    sw      x3,  ( 1*4)(sp)
    sw      x4,  ( 2*4)(sp)
    sw      x5,  ( 3*4)(sp)
    sw      x6,  ( 4*4)(sp)
    sw      x7,  ( 5*4)(sp)
    sw      x8,  ( 6*4)(sp)
    sw      x9,  ( 7*4)(sp)
    sw      x10, ( 8*4)(sp)
    sw      x11, ( 9*4)(sp)
    sw      x12, (10*4)(sp)
    sw      x13, (11*4)(sp)
    sw      x14, (12*4)(sp)
    sw      x15, (13*4)(sp)
    sw      x16, (14*4)(sp)
    sw      x17, (15*4)(sp)
    sw      x18, (16*4)(sp)
    sw      x19, (17*4)(sp)
    sw      x20, (18*4)(sp)
    sw      x21, (19*4)(sp)
    sw      x22, (20*4)(sp)
    sw      x23, (21*4)(sp)
    sw      x24, (22*4)(sp)
    sw      x25, (23*4)(sp)
    sw      x26, (24*4)(sp)
    sw      x27, (25*4)(sp)
    sw      x28, (26*4)(sp)
    sw      x29, (27*4)(sp)
    sw      x30, (28*4)(sp)
    sw      x31, (29*4)(sp)
    csrr    t0, mepc
    sw      t0,  (30*4)(sp)

    csrr    t0, mnxti

    csrr    t1, mcause
    andi    t1, t1, 0x3FF
    slli    t1, t1, 2
    la      t0, g_irqvector
    add     t0, t0, t1
    lw      t2, (t0)
    jalr    t2
    
    lw      t0,  (30*4)(sp)
    csrw    mepc, t0
    lw      x1,  ( 0*4)(sp)
    lw      x3,  ( 1*4)(sp)
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

/* trap, save the value of the register to memory. */
.section .text
    .align  2
    .global trap
    .type   trap, %function
    .global trap_dump
    .type   trap_dump, %function
trap:
    sw      t0, -4(sp)

    csrr    t0, mcause
    blt     t0, x0, .Lirq

trap_dump:
    la      t0, g_top_trapstack
    addi    t0, t0, -34*4

    sw      x1,   0*4(t0)
    sw      x2,   1*4(t0)
    sw      x3,   2*4(t0)
    sw      x4,   3*4(t0)
    sw      x6,   5*4(t0)
    sw      x7,   6*4(t0)
    sw      x8,   7*4(t0)
    sw      x9,   8*4(t0)
    sw      x10,  9*4(t0)
    sw      x11, 10*4(t0)
    sw      x12, 11*4(t0)
    sw      x13, 12*4(t0)
    sw      x14, 13*4(t0)
    sw      x15, 14*4(t0)
    sw      x16, 15*4(t0)
    sw      x17, 16*4(t0)
    sw      x18, 17*4(t0)
    sw      x19, 18*4(t0)
    sw      x20, 19*4(t0)
    sw      x21, 20*4(t0)
    sw      x22, 21*4(t0)
    sw      x23, 22*4(t0)
    sw      x24, 23*4(t0)
    sw      x25, 24*4(t0)
    sw      x26, 25*4(t0)
    sw      x27, 26*4(t0)
    sw      x28, 27*4(t0)
    sw      x29, 28*4(t0)
    sw      x30, 29*4(t0)
    sw      x31, 30*4(t0)

    csrr    a0, mepc
    sw      a0, 31*4(t0)
    csrr    a0, mtval
    sw      a0, 32*4(t0)
    csrr    a0, mstatus
    sw      a0, 33*4(t0)

    mv      t1, t0

    lw      t0, -4(sp)
    sw      t0,  4*4(t1)

    mv      a0, t1
    j       trap_c // NOTE: WON'T RETURN.

.Lirq:
    lw      t0, -4(sp)
    j       Default_IRQHandler // NOTE: WON'T RETURN.

/* TSPEND_IRQHandler, software interrupt service function. */
.section .text
    .align  6
    .weak   TSPEND_IRQHandler
    .global TSPEND_IRQHandler
    .type   TSPEND_IRQHandler, %function
TSPEND_IRQHandler:
    sw      t0,  -4(sp)
    li      t0, 0xE080100C
    sb      zero, (t0)
    lw      t0,  -4(sp)

    mret

/* CORET_IRQHandler, core timer interrupt service function. */
.section .text
    .align  6
    .weak   CORET_IRQHandler
    .global CORET_IRQHandler
    .type   CORET_IRQHandler, %function
CORET_IRQHandler:
    call    idling
    mret

/* idling, just dead loops. */
.section .text
    .align  2
    .global idling
    .type   idling, %function
idling:
    j       idling

/*
 *  Macro to define default handlers. Default handler will be weak symbol and just dead loops. 
 *  They can be overwritten by other handlers.
 */
.macro  _def_irq_handler handler_name
.section .text
    .align  2
    .globl  \handler_name
    .weak   \handler_name
    .type   \handler_name, %function
    .set    \handler_name, idling
.endm

    _def_irq_handler BUS_1e_IRQHandler          // 16
    _def_irq_handler AMC_1e_IRQHandler          // 17
    _def_irq_handler SRAM_1e_IRQHandler         // 18
    _def_irq_handler IRAM_1e_IRQHandler         // 19
    _def_irq_handler SRIO1_IRQHandler           // 20
    _def_irq_handler SRIO0_IRQHandler           // 21
    _def_irq_handler DMAC_IRQHandler            // 22
    _def_irq_handler DDR_IRQHandler             // 23
    _def_irq_handler GMAC1_IRQHandler           // 24
    _def_irq_handler GMAC0_IRQHandler           // 25
    _def_irq_handler PWM5_IRQHandler            // 26
    _def_irq_handler PWM4_IRQHandler            // 27
    _def_irq_handler PWM3_IRQHandler            // 28
    _def_irq_handler PWM2_IRQHandler            // 29
    _def_irq_handler PWM1_IRQHandler            // 30
    _def_irq_handler PWM0_IRQHandler            // 31
    _def_irq_handler EPWM7_IRQHandler           // 32
    _def_irq_handler EPWM6_IRQHandler           // 33
    _def_irq_handler EPWM5_IRQHandler           // 34
    _def_irq_handler EPWM4_IRQHandler           // 35
    _def_irq_handler EPWM3_IRQHandler           // 36
    _def_irq_handler EPWM2_IRQHandler           // 37
    _def_irq_handler EPWM1_IRQHandler           // 38
    _def_irq_handler EPWM0_IRQHandler           // 39
    _def_irq_handler RTC_IRQHandler             // 40
    _def_irq_handler SMC_IRQHandler             // 41
    _def_irq_handler I2C1_IRQHandler            // 42
    _def_irq_handler I2C0_IRQHandler            // 43
    _def_irq_handler SPI7_IRQHandler            // 44
    _def_irq_handler SPI6_IRQHandler            // 45
    _def_irq_handler SPI5_IRQHandler            // 46
    _def_irq_handler SPI4_IRQHandler            // 47
    _def_irq_handler SPI3_IRQHandler            // 48
    _def_irq_handler SPI2_IRQHandler            // 49
    _def_irq_handler SPI1_IRQHandler            // 50
    _def_irq_handler SPI0_IRQHandler            // 51
    _def_irq_handler UART21_IRQHandler          // 52
    _def_irq_handler UART20_IRQHandler          // 53
    _def_irq_handler UART19_IRQHandler          // 54
    _def_irq_handler UART18_IRQHandler          // 55
    _def_irq_handler UART17_IRQHandler          // 56
    _def_irq_handler UART16_IRQHandler          // 57
    _def_irq_handler UART15_IRQHandler          // 58
    _def_irq_handler UART14_IRQHandler          // 59
    _def_irq_handler UART13_IRQHandler          // 60
    _def_irq_handler UART12_IRQHandler          // 61
    _def_irq_handler UART11_IRQHandler          // 62
    _def_irq_handler UART10_IRQHandler          // 63
    _def_irq_handler UART9_IRQHandler           // 64
    _def_irq_handler UART8_IRQHandler           // 65
    _def_irq_handler UART7_IRQHandler           // 66
    _def_irq_handler UART6_IRQHandler           // 67
    _def_irq_handler UART5_IRQHandler           // 68
    _def_irq_handler UART4_IRQHandler           // 69
    _def_irq_handler UART3_IRQHandler           // 70
    _def_irq_handler UART2_IRQHandler           // 71
    _def_irq_handler UART1_IRQHandler           // 72
    _def_irq_handler UART0_IRQHandler           // 73
    _def_irq_handler QSPI_IRQHandler            // 74
    _def_irq_handler AMC_IRQHandler             // 75
    _def_irq_handler TIMER1_IRQHandler          // 76
    _def_irq_handler TIMER0_IRQHandler          // 77
    _def_irq_handler SPW3_IRQHandler            // 78
    _def_irq_handler SPW2_IRQHandler            // 79
    _def_irq_handler SPW1_IRQHandler            // 80
    _def_irq_handler SPW0_IRQHandler            // 81
    _def_irq_handler CAN1_IRQHandler            // 82
    _def_irq_handler CAN0_IRQHandler            // 83
    _def_irq_handler B1553B1_IRQHandler         // 84
    _def_irq_handler B1553B0_IRQHandler         // 85
    _def_irq_handler GPIO15_IRQHandler          // 86
    _def_irq_handler GPIO14_IRQHandler          // 87
    _def_irq_handler GPIO13_IRQHandler          // 88
    _def_irq_handler GPIO12_IRQHandler          // 89
    _def_irq_handler GPIO11_IRQHandler          // 90
    _def_irq_handler GPIO10_IRQHandler          // 91
    _def_irq_handler GPIO9_IRQHandler           // 92
    _def_irq_handler GPIO8_IRQHandler           // 93
    _def_irq_handler GPIO7_IRQHandler           // 94
    _def_irq_handler GPIO6_IRQHandler           // 95
    _def_irq_handler GPIO5_IRQHandler           // 96
    _def_irq_handler GPIO4_IRQHandler           // 97
    _def_irq_handler GPIO3_IRQHandler           // 98
    _def_irq_handler GPIO2_IRQHandler           // 99
    _def_irq_handler GPIO1_IRQHandler           // 100
    _def_irq_handler GPIO0_IRQHandler           // 101
    _def_irq_handler APBG1_Bus_2e_IRQHandler    // 102
    _def_irq_handler APBG0_Bus_2e_IRQHandler    // 103
    _def_irq_handler AHB_2e_IRQHandler          // 104
    _def_irq_handler AXI_2e_IRQHandler          // 105
    _def_irq_handler SMC_2e_IRQHandler          // 106
    _def_irq_handler AMC_2e_IRQHandler          // 107
    _def_irq_handler SRAM_2e_IRQHandler         // 108
    _def_irq_handler IRAM_2e_IRQHandler         // 109
