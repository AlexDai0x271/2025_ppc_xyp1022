/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "queue.h"

#define QUEUE_LENGTH    10
#define QUEUE_ITEM_SIZE sizeof(uint32_t)

QueueHandle_t g_hQueue;

/* ----- ----- ----- ----- USER TASK DEFINE ----- ----- ----- ----- */

mTaskDefine(user1_task)
{
    uint32_t offset = 0;

    for (;;) {
        uint32_t data = 0xfadec0de;

        vTaskDelay(pdMS_TO_TICKS(500));

        xQueueSend(g_hQueue, &data, 0);
        xprintf("[%s] sending a data, value [%08x].\n", __func__, data);

        offset = (offset + 1) % QUEUE_LENGTH;
    }
}

/* ----- ----- ----- ----- USER TASK 2 DEFINE ----- ----- ----- ----- */

mTaskDefine(user2_task)
{
    uint32_t uData;

    for (;;) {
        xQueueReceive(g_hQueue, &uData, OS_WAIT_FOREVER);
        xprintf("[%s] recving a data, value [%08x].\n", __func__, uData);
        xprintf("\n");
    }
}

/* ----- ----- ----- ----- USER TASK START ----- ----- ----- ----- */

void uTaskCreate(void)
{
    xprintf("\n");
    xprintf("[INFO]: This is a 'message queue' demo.\n");
    xprintf("\n");

    g_hQueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    if (!g_hQueue) {
        xprintf("[INFO]: Message queue create failed!\n");
        return;
    }

    mTaskCreate(user1_task, "user1_task", 256, 2);
    mTaskCreate(user2_task, "user2_task", 256, 2);
}
