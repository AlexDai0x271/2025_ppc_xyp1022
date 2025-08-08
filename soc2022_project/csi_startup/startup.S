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

/* __Vectors, interrupt vector table */
.section .vectors, "aw", @progbits
    .align  6
    .globl  __Vectors
    .type   __Vectors, @object
__Vectors:
    .long   Default_Handler         // 0
    .long   Default_Handler         // 1
    .long   Default_Handler         // 2
    .long   Default_IRQHandler      // 3, TSPEND_IRQHandler
    .long   Default_Handler         // 4
    .long   Default_Handler         // 5
    .long   Default_Handler         // 6
    .long   Default_IRQHandler      // 7, CORET_IRQHandler
    .long   Default_Handler         // 8
    .long   Default_Handler         // 9
    .long   Default_Handler         // 10
    .long   Default_Handler         // 11
    .long   Default_Handler         // 12
    .long   Default_Handler         // 13
    .long   Default_Handler         // 14
    .long   Default_Handler         // 15

    /* External interrupts */
    .long   Default_IRQHandler      // 16:  1
    .long   Default_IRQHandler      // 17:  2
    .long   Default_IRQHandler      // 18:  3
    .long   Default_IRQHandler      // 19:  4
    .long   Default_IRQHandler      // 20:  5
    .long   Default_IRQHandler      // 21:  6
    .long   Default_IRQHandler      // 22:  7
    .long   Default_IRQHandler      // 23:  8
    .long   Default_IRQHandler      // 24:  9
    .long   Default_IRQHandler      // 25:  10
    .long   Default_IRQHandler      // 26:  11
    .long   Default_IRQHandler      // 27:  12
    .long   Default_IRQHandler      // 28:  13
    .long   Default_IRQHandler      // 29:  14
    .long   Default_IRQHandler      // 30:  15
    .long   Default_IRQHandler      // 31:  16
    .long   Default_IRQHandler      // 32:  17
    .long   Default_IRQHandler      // 33:  18
    .long   Default_IRQHandler      // 34:  19
    .long   Default_IRQHandler      // 35:  20
    .long   Default_IRQHandler      // 36:  21
    .long   Default_IRQHandler      // 37:  22
    .long   Default_IRQHandler      // 38:  23
    .long   Default_IRQHandler      // 39:  24
    .long   Default_IRQHandler      // 40:  25
    .long   Default_IRQHandler      // 41:  26
    .long   Default_IRQHandler      // 42:  27
    .long   Default_IRQHandler      // 43:  28
    .long   Default_IRQHandler      // 44:  29
    .long   Default_IRQHandler      // 45:  30
    .long   Default_IRQHandler      // 46:  31
    .long   Default_IRQHandler      // 47:  32
    .long   Default_IRQHandler      // 48:  33
    .long   Default_IRQHandler      // 49:  34
    .long   Default_IRQHandler      // 50:  35
    .long   Default_IRQHandler      // 51:  36
    .long   Default_IRQHandler      // 52:  37
    .long   Default_IRQHandler      // 53:  38
    .long   Default_IRQHandler      // 54:  39
    .long   Default_IRQHandler      // 55:  40
    .long   Default_IRQHandler      // 56:  41
    .long   Default_IRQHandler      // 57:  42
    .long   Default_IRQHandler      // 58:  43
    .long   Default_IRQHandler      // 59:  44
    .long   Default_IRQHandler      // 60:  45
    .long   Default_IRQHandler      // 61:  46
    .long   Default_IRQHandler      // 62:  47
    .long   Default_IRQHandler      // 63:  48
    .long   Default_IRQHandler      // 64:  49
    .long   Default_IRQHandler      // 65:  50
    .long   Default_IRQHandler      // 66:  51
    .long   Default_IRQHandler      // 67:  52
    .long   Default_IRQHandler      // 68:  53
    .long   Default_IRQHandler      // 69:  54
    .long   Default_IRQHandler      // 70:  55
    .long   Default_IRQHandler      // 71:  56
    .long   Default_IRQHandler      // 72:  57
    .long   Default_IRQHandler      // 73:  58
    .long   Default_IRQHandler      // 74:  59
    .long   Default_IRQHandler      // 75:  60
    .long   Default_IRQHandler      // 76:  61
    .long   Default_IRQHandler      // 77:  62
    .long   Default_IRQHandler      // 78:  63
    .long   Default_IRQHandler      // 79:  64
    .long   Default_IRQHandler      // 80:  65
    .long   Default_IRQHandler      // 81:  66
    .long   Default_IRQHandler      // 82:  67
    .long   Default_IRQHandler      // 83:  68
    .long   Default_IRQHandler      // 84:  69
    .long   Default_IRQHandler      // 85:  70
    .long   Default_IRQHandler      // 86:  71
    .long   Default_IRQHandler      // 87:  72
    .long   Default_IRQHandler      // 88:  73
    .long   Default_IRQHandler      // 89:  74
    .long   Default_IRQHandler      // 90:  75
    .long   Default_IRQHandler      // 91:  76
    .long   Default_IRQHandler      // 92:  77
    .long   Default_IRQHandler      // 93:  78
    .long   Default_IRQHandler      // 94:  79
    .long   Default_IRQHandler      // 95:  80
    .long   Default_IRQHandler      // 96:  81
    .long   Default_IRQHandler      // 97:  82
    .long   Default_IRQHandler      // 98:  83
    .long   Default_IRQHandler      // 99:  84
    .long   Default_IRQHandler      // 100: 85
    .long   Default_IRQHandler      // 101: 86
    .long   Default_IRQHandler      // 102: 87
    .long   Default_IRQHandler      // 103: 88
    .long   Default_IRQHandler      // 104: 89
    .long   Default_IRQHandler      // 105: 90
    .long   Default_IRQHandler      // 106: 91
    .long   Default_IRQHandler      // 107: 92
    .long   Default_IRQHandler      // 108: 93
    .long   Default_IRQHandler      // 109: 94

/* _start, the entry point of program */
.section .text
    .align  2
    .globl  Reset_Handler
    .type   Reset_Handler, %function
_start:
Reset_Handler:
.option push
.option norelax
    la      gp, __global_pointer$
.option pop
    la      a0, Default_Handler
    ori     a0, a0, 3
    csrw    mtvec, a0

    la      a0, __Vectors
    csrw    mtvt, a0

    la      sp, g_top_irqstack
    csrw    mscratch, sp

    /* Load data section */
    la      a0, __rodata_end__
    la      a1, __data_start__
    la      a2, __data_end__
    bgeu    a1, a2, 2f
1:
    lw      t0, (a0)
    sw      t0, (a1)
    addi    a0, a0, 4
    addi    a1, a1, 4
    bltu    a1, a2, 1b
2:

    /* Clear bss section */
    la      a0, __bss_start__
    la      a1, __bss_end__
    bgeu    a0, a1, 2f
1:
    sw      zero, (a0)
    addi    a0, a0, 4
    bltu    a0, a1, 1b
2:

#ifndef __NO_SYSTEM_INIT
    jal     SystemInit
#endif

#ifndef __NO_BOARD_INIT
    jal     board_init
#endif

    call    entry

__exit:
    j       __exit

.weak entry
entry:
    call    main
    ret


.section .bss
    .align  3
    .global g_base_irqstack
    .global g_top_irqstack
g_base_irqstack:
    .space CONFIG_ARCH_INTERRUPTSTACK
g_top_irqstack:
