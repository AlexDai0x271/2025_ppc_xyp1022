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

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the PPC440 port.
 *----------------------------------------------------------*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Library includes. */

/*-----------------------------------------------------------*/

/* Definitions to set the initial MSR of each task. */
#define portCRITICAL_64BIT_MODE_ENABLE (1UL << 31UL)
#define portCRITICAL_32BIT_MODE_ENABLE (0UL << 31UL)
#define portCRITICAL_INTERRUPT_ENABLE  (1UL << 17UL)
#define portEXTERNAL_INTERRUPT_ENABLE  (1UL << 15UL)
#define portMACHINE_CHECK_ENABLE       (1UL << 12UL)

#define portINITIAL_MSR                (portCRITICAL_32BIT_MODE_ENABLE | portCRITICAL_INTERRUPT_ENABLE | portEXTERNAL_INTERRUPT_ENABLE | portMACHINE_CHECK_ENABLE)

typedef struct
{
    void (*entry)(void);
    void *toc_base;
    void *env_ptr;
} opd_t;

/*-----------------------------------------------------------*/

/* Function to start the scheduler running by starting the highest
priority task that has thus far been created. */
extern void vPortStartFirstTask(void);

/*
 * Initialise the stack of a task to look exactly as if the task had been
 * interrupted.
 *
 * See the header file portable.h.
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                   TaskFunction_t pxCode,
                                   void *pvParameters)
{
    opd_t *pcDesc = (opd_t *)pxCode;
    opd_t *lrDesc = (opd_t *)vPortEndScheduler;

    portSTACK_TYPE *pxBackchain;

    /* Place a known value at the bottom of the stack for debugging. */
    *pxTopOfStack-- = 0xDEADBEEF;
    *pxTopOfStack-- = 0xDEADBEEF;

    /* Create a root frame header */
    *pxTopOfStack-- = 0x0L; /* Root lr save word */
    *pxTopOfStack-- = 0x0L; /* Root backchain */

    pxBackchain = pxTopOfStack;

    *pxTopOfStack-- = 0x94949494L;                  /* r31  - 0x94 */
    *pxTopOfStack-- = 0x90909090L;                  /* r30  - 0x90 */
    *pxTopOfStack-- = 0x8C8C8C8CL;                  /* r29  - 0x8C */
    *pxTopOfStack-- = 0x88888888L;                  /* r28  - 0x88 */
    *pxTopOfStack-- = 0x84848484L;                  /* r27  - 0x84 */
    *pxTopOfStack-- = 0x80808080L;                  /* r26  - 0x80 */
    *pxTopOfStack-- = 0x7C7C7C7CL;                  /* r25  - 0x7C */
    *pxTopOfStack-- = 0x78787878L;                  /* r24  - 0x78 */
    *pxTopOfStack-- = 0x74747474L;                  /* r23  - 0x74 */
    *pxTopOfStack-- = 0x70707070L;                  /* r22  - 0x70 */
    *pxTopOfStack-- = 0x6C6C6C6CL;                  /* r21  - 0x6C */
    *pxTopOfStack-- = 0x68686868L;                  /* r20  - 0x68 */
    *pxTopOfStack-- = 0x64646464L;                  /* r19  - 0x64 */
    *pxTopOfStack-- = 0x60606060L;                  /* r18  - 0x60 */
    *pxTopOfStack-- = 0x5C5C5C5CL;                  /* r17  - 0x5C */
    *pxTopOfStack-- = 0x58585858L;                  /* r16  - 0x58 */
    *pxTopOfStack-- = 0x54545454L;                  /* r15  - 0x54 */
    *pxTopOfStack-- = 0x50505050L;                  /* r14  - 0x50 */
    *pxTopOfStack-- = 0x4C4C4C4CL;                  /* r12  - 0x4C */
    *pxTopOfStack-- = 0x48484848L;                  /* r11  - 0x48 */
    *pxTopOfStack-- = 0x44444444L;                  /* r10  - 0x44 */
    *pxTopOfStack-- = 0x40404040L;                  /* r09  - 0x40 */
    *pxTopOfStack-- = 0x3C3C3C3CL;                  /* r08  - 0x3C */
    *pxTopOfStack-- = 0x38383838L;                  /* r07  - 0x38 */
    *pxTopOfStack-- = 0x34343434L;                  /* r06  - 0x34 */
    *pxTopOfStack-- = 0x30303030L;                  /* r05  - 0x30 */
    *pxTopOfStack-- = 0x2C2C2C2CL;                  /* r04  - 0x2C */
    *pxTopOfStack-- = (portSTACK_TYPE)pvParameters; /* r03  - 0x28 */
    *pxTopOfStack-- = 0x0L;                         /* r00  - 0x24 */
    *pxTopOfStack-- = 0x0L;                         /* XER  - 0x20 */
    *pxTopOfStack-- = 0x0L;                         /* CTR  - 0x1C */
    *pxTopOfStack-- = (StackType_t)lrDesc->entry;   /* LR   - 0x18 */
    *pxTopOfStack-- = 0x0L;                         /* CR   - 0x14 */
    *pxTopOfStack-- = portINITIAL_MSR;              /* SRR1 - 0x10 */
    *pxTopOfStack-- = (StackType_t)pcDesc->entry;   /* SRR0 - 0x0C */
    *pxTopOfStack-- = 0x0L;                         /* Pad  - 0x08 */
    *pxTopOfStack-- = 0x0L;                         /* LR save word - 0x04 */

    *pxTopOfStack = (portSTACK_TYPE)pxBackchain;    /* SP(r1) - 0x00 */

    return pxTopOfStack;
}

/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler(void)
{
    asm volatile("b vPortStartFirstTask");

    /* Should not get here as the tasks are now running! */
    return pdFALSE;
}

void vPortEndScheduler(void)
{
    /* Not implemented. */
    for (;;) {
    }
}

void xPortSysTickHandler(void)
{
    if (taskSCHEDULER_NOT_STARTED != xTaskGetSchedulerState()) {
        if (xTaskIncrementTick() != pdFALSE) {
#if configUSE_PREEMPTION == 1
            portYIELD_FROM_ISR(pdTRUE);
#endif
        }
    }
}

__attribute__((weak)) void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    xprintf("Stack Overflow!\n");
    for (;;)
        ;
}

__attribute__((weak)) void vApplicationMallocFailedHook(void)
{
    xprintf("Malloc Failed!\n");
    for (;;)
        ;
}

__attribute__((weak)) void vApplicationIdleHook(void)
{
    xprintf(".");
}
