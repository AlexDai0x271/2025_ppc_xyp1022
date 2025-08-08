/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "timers.h"

TimerHandle_t g_hTimer;
uint32_t g_pvTimerID;

void vTimerCallback(TimerHandle_t xTimer)
{
    static uint32_t s_count = 0;

    s_count ++;

    xprintf("[INFO]: Soft timer callback, in [%s], count [%d].\n", __func__, s_count);

    if ( s_count >= 27 )
    {
        if ( pdPASS != xTimerStop(g_hTimer, 0) )
        {
            xprintf("[INFO]: Soft timer stop failed!\n");
        } else {
            xprintf("[INFO]: Soft timer stop success.\n");
        }
    }
}

void uTaskCreate(void)
{
    xprintf("\n");
    xprintf("[INFO]: This is a 'soft timer' demo.\n");
    xprintf("\n");

    g_pvTimerID = 0;
    g_hTimer = xTimerCreate("timer0", pdMS_TO_TICKS(1000), pdTRUE, (void*)g_pvTimerID, vTimerCallback);

    if ( !g_hTimer )
    {
        xprintf("[INFO]: Soft timer create failed!\n");
        return;
    } else {
        if ( pdPASS != xTimerStart(g_hTimer, 0) )
        {
            xprintf("[INFO]: Soft timer start failed!\n");
            return;
        } else {
            xprintf("[INFO]: Soft timer start success.\n");
        }
    }
}
