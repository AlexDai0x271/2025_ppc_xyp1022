/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "event_groups.h"

#define BIT_0                   ( 1 << 0 )
#define BIT_4                   ( 1 << 4 )

EventGroupHandle_t g_hEventGroup;

mTaskDefine(user1_task)
{
    for (;;)
    {
        vTaskDelay( pdMS_TO_TICKS(500) );
        xEventGroupSetBits(g_hEventGroup, BIT_0);
    }
}

mTaskDefine(user2_task)
{
    for (;;)
    {
        vTaskDelay( pdMS_TO_TICKS(250) );
        xEventGroupSetBits(g_hEventGroup, BIT_4);
    }
}

mTaskDefine(user3_task)
{
    EventBits_t uxBits;

    for (;;)
    {
        uxBits = xEventGroupWaitBits(g_hEventGroup, BIT_0|BIT_4, pdTRUE, pdFALSE, OS_WAIT_FOREVER); // .xClearOnExit = pdTRUE, .xWaitForAllBits = pdFALSE
        xprintf("[%s] recving a event, value [%016b]\n", __func__, uxBits);
    }
}

void uTaskCreate(void)
{
    xprintf("\n");
    xprintf("[INFO]: This is a 'event' demo.\n");
    xprintf("\n");

    g_hEventGroup = xEventGroupCreate();
    if ( !g_hEventGroup )
    {
        xprintf("[INFO]: Event create failed!\n");
        return;
    }

    mTaskCreate(user1_task, "user1_task", 256, 2);
    mTaskCreate(user2_task, "user2_task", 256, 2);
    mTaskCreate(user3_task, "user3_task", 256, 2);
}
