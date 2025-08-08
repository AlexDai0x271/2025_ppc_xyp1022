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
#include "sysclock.h"

void drv_irq_enable(uint32_t irq_num)
{
    if (NMI_EXPn != irq_num) {
#ifdef CONFIG_SYSTEM_SECURE
        csi_vic_enable_sirq(irq_num);
#else
        csi_vic_enable_irq(irq_num);
#endif
    }
}

void drv_irq_disable(uint32_t irq_num)
{
    if (NMI_EXPn != irq_num) {
#ifdef CONFIG_SYSTEM_SECURE
        csi_vic_disable_sirq(irq_num);
#else
        csi_vic_disable_irq(irq_num);
#endif
    }
}

static void _system_init_for_baremetal(void)
{
    /* disable mexstatus SPUSHEN and SPSWAPEN  */
#if ((CONFIG_CPU_E906 == 1) || (CONFIG_CPU_E906F == 1) || (CONFIG_CPU_E906FD == 1))
    uint32_t mexstatus = __get_MEXSTATUS();
    mexstatus &= ~(0x3 << 16);
    __set_MEXSTATUS(mexstatus);
#endif

    extern void drv_irq_vectors_init(void);
    drv_irq_vectors_init();

    uint32_t freq = 1000 / CONFIG_SYSTICK_MS;
    CORET->MTIMECMP = CORET->MTIME + (drv_get_sys_freq() / freq);

    drv_irq_enable(CORET_IRQn);
    drv_irq_enable(Machine_Software_IRQn);

#ifdef USE_OS_FREERTOS
    __disable_irq();
#else
    __enable_irq();
#endif // #ifdef USE_OS_FREERTOS
}

int SystemIsLittleEndian(void)
{
    volatile uint32_t dw = 0x00000001;
    return (*(uint8_t *)&dw == 0x01);
}

int SystemIsStructRightAlign(void)
{
    volatile union
    {
        struct
        {
            uint32_t reserved : 31;
            uint32_t bit0     : 1;
        };
        uint32_t value;
    } u;

    u.bit0 = 1;

    return (u.value & BIT(0));
}

void SystemInit(void)
{
    /* set system clock */
    extern uint32_t g_system_clock;
    g_system_clock = CPU_CLOCK_HZ;

    /* enable mstatus FS */
#if ((CONFIG_CPU_E906F == 1) || (CONFIG_CPU_E906FD == 1))
    uint32_t mstatus = __get_MSTATUS();
    mstatus |= (1 << 13);
    __set_MSTATUS(mstatus);
#endif

    /* enable mxstatus THEADISAEE */
    uint32_t mxstatus = __get_MXSTATUS();
    mxstatus |= (1 << 22);
    /* enable mxstatus MM */
#if ((CONFIG_CPU_E906 == 1) || (CONFIG_CPU_E906F == 1) || (CONFIG_CPU_E906FD == 1))
    mxstatus |= (1 << 15);
#endif
    __set_MXSTATUS(mxstatus);

    /* get interrupt level from info */
    CLIC->CLICCFG = (((CLIC->CLICINFO & CLIC_INFO_CLICINTCTLBITS_Msk) >> CLIC_INFO_CLICINTCTLBITS_Pos) << CLIC_CLICCFG_NLBIT_Pos);

    for (int i = 0; i < IRQn_Max; i++) {
        CLIC->CLICINT[i].IP = 0;
        CLIC->CLICINT[i].ATTR = 0x1; /* use vector interrupt */
    }

    CLIC->CLICINT[Machine_Software_IRQn].ATTR = 0x3; /* use vector interrupt */

    csi_dcache_enable();
    csi_icache_enable();

    _system_init_for_baremetal();
}
