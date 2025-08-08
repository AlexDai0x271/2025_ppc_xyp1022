/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "XYR3201.h"
#include "xprintf.h"

void trap_c(uint32_t *regs)
{
#if (CONFIG_ENABLE_TRAP_C == 1)
    uint32_t vec = 0;

    vec = __get_MCAUSE() & 0x3FF;

    xprintf("CPU Exception: NO.%d", vec);
    xprintf("\n");

    xprintf("x1(alias: ra): %08x\n", regs[0]); // regs[0] records the value of x1, and so on
    xprintf("x2(alias: sp): %08x\n", regs[1]);
    xprintf("x3(alias: gp): %08x\n", regs[2]);
    xprintf("x4(alias: tp): %08x\n", regs[3]);

    for (int i = 0; i < 31; i++) {
        xprintf("x%d: %08x\t", i + 1, regs[i]);

        if ((i % 4) == 3) {
            xprintf("\n");
        }
    }

    xprintf("\n");
    xprintf("mepc   : %08x\n", regs[31]);
    xprintf("mtval  : %08x\n", regs[32]);
    xprintf("mstatus: %08x\n", regs[33]);

    for (;;)
        ;
        // NOTE: WON'T EXECUTE HERE.
#endif // #if ( CONFIG_ENABLE_TRAP_C == 1 )
}
