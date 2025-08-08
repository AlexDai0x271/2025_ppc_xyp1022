/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

void uTaskCreate(void)
{
    xprintf("\n");
    xprintf("[INFO]: This is a 'memory pool' demo.\n");
    xprintf("\n");

    uint8_t *mempool[16];

    vPortInitialiseBlocks();

    for (int i=0; i<sizeof(mempool)/sizeof(mempool[0]); i++)
    {
        if ( mempool[i] = pvPortMalloc(256) )
        {
            xprintf("[INFO]: Memory alloc success. addr [%08x]\n", mempool[i]);
            xprintf("[INFO]: Memory pool total: %d Byte.\n", xPortGetFreeHeapSize());
            xprintf("\n");
        } else {
            xprintf("[INFO]: Memory alloc failed!\n");
        }
    }

    for (int i=0; i<sizeof(mempool)/sizeof(mempool[0]); i++)
    {
        if ( mempool[i] ) {
            vPortFree(mempool[i]);
        }
    }

    xprintf("[INFO]: Memory free success.\n");
}
