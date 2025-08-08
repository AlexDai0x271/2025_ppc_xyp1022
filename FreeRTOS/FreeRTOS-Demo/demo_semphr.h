/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "semphr.h"
#include "queue.h"

static uint32_t s_Resource = 0;

SemaphoreHandle_t g_hSemaphore;

mTaskDefine(user1_task)
{
    for (;;)
    {
        xSemaphoreTake(g_hSemaphore, OS_WAIT_FOREVER);

        s_Resource ++;
        xprintf("[%s] take the resource, current value [%08x].\n", __func__, s_Resource);

        xSemaphoreGive(g_hSemaphore);
    }
}

mTaskDefine(user2_task)
{
    for (;;)
    {
        xSemaphoreTake(g_hSemaphore, OS_WAIT_FOREVER);

        s_Resource ++;
        xprintf("[%s] take the resource, current value [%08x].\n", __func__, s_Resource);

        xSemaphoreGive(g_hSemaphore);
    }
}

void uTaskCreate(void)
{
    xprintf("\n");
    xprintf("[INFO]: This is a 'semphr' demo.\n");
    xprintf("\n");

    g_hSemaphore = xSemaphoreCreateCounting(10, 0);
    // g_hSemaphore = xSemaphoreCreateBinary();
    if ( !g_hSemaphore )
    {
        xprintf("[INFO]: Semphr create failed!\n");
        return;
    }  else {
        xSemaphoreGive(g_hSemaphore);
    }

    mTaskCreate(user1_task, "user1_task", 256, 2);
    mTaskCreate(user2_task, "user2_task", 256, 2);
}
