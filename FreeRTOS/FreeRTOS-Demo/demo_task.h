/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

mTaskDefine(user1_task)
{
    for (;;)
    {
        xprintf("[%s] is running.\n", __func__);
        vTaskDelay( pdMS_TO_TICKS(500) );
    }
}

mTaskDefine(user2_task)
{
    for (;;)
    {
        xprintf("[%s] is running.\n", __func__);
        vTaskDelay( pdMS_TO_TICKS(250) );
    }
}

void uTaskCreate(void)
{
    xprintf("\n");
    xprintf("[INFO]: This is a 'task' demo.\n");
    xprintf("[INFO]: scheduling mode: %s-%s.\n", (configUSE_PREEMPTION ? "PREEMPTION" : ""),
                                                 (configUSE_TIME_SLICING ? "TIME_SLICING" : ""));
    xprintf("\n");

    mTaskCreate(user1_task, "user1_task", 256, 2);
    mTaskCreate(user2_task, "user2_task", 256, 2);
}
