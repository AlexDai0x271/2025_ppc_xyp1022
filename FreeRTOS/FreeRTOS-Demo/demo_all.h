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
#include "event_groups.h"
#include "timers.h"

#define QUEUE_NUM               ( 2 )

#define BIT_1                   ( 1 << 1 )
#define BIT_2                   ( 1 << 2 )

#define GET_VALUE_H16(v)        ( ( (v) >> 16 ) & 0xffff )
#define GET_VALUE_L16(v)        ( ( (v)       ) & 0xffff )

#ifndef NULL
#define NULL                    ( (void *) 0 )
#endif

typedef struct
{
    uint32_t                    item;
    SemaphoreHandle_t           lock;
} ScoreBoard_t;

typedef struct
{
    uint32_t                    depth;
    uint32_t                    size;
    QueueHandle_t               handle;
} Queue_t;

typedef struct
{
    uint32_t                    id;
    uint32_t                    interval;
    TimerHandle_t               handle;
} Timer_t;

ScoreBoard_t g_hScoreBoard;

Queue_t g_hQueue[QUEUE_NUM];
EventGroupHandle_t g_hEventGroup;
Timer_t g_hTimer;

void vTimerCallback(TimerHandle_t xTimer)
{
    xprintf("[INFO]: Soft timer callback, in [%s].\n", __func__);
    xprintf("        Summary: item.a [%d], item.b [%d].\n", GET_VALUE_H16(g_hScoreBoard.item), GET_VALUE_L16(g_hScoreBoard.item));
}

mTaskDefine(user1_task)
{
    uint8_t *pbuf;

    for (;;)
    {
        vTaskDelay( pdMS_TO_TICKS(500) );

        pbuf = NULL;
        pbuf = pvPortMalloc(4);
        if ( !pbuf )
        {
            xprintf("[INFO]: Memory alloc failed!\n");
            continue;
        }

        pbuf[0] = 'X';
        pbuf[1] = 'Y';
        pbuf[2] = 'R';
        pbuf[3] = '\0';
        if ( errQUEUE_FULL == xQueueSend(g_hQueue[0].handle, &pbuf, 0) )
        {
            if ( pbuf ) {
                vPortFree(pbuf);
            }
            xprintf("[INFO]: Queue[0] depth is [%d], but now [%d]!\n", g_hQueue[0].depth, GET_VALUE_H16(g_hScoreBoard.item));
        } else {
            xSemaphoreTake(g_hScoreBoard.lock, OS_WAIT_FOREVER);
            g_hScoreBoard.item += 0x10000; // item_a + 1
            xSemaphoreGive(g_hScoreBoard.lock);

            xEventGroupSetBits(g_hEventGroup, BIT_1);
        }
    }
}

mTaskDefine(user2_task)
{
    uint8_t *pbuf;

    for (;;)
    {
        vTaskDelay( pdMS_TO_TICKS(1000) );

        pbuf = NULL;
        pbuf = pvPortMalloc(5);
        if ( !pbuf )
        {
            xprintf("[INFO]: Memory alloc failed!\n");
            continue;
        }

        pbuf[0] = '2';
        pbuf[1] = '0';
        pbuf[2] = '2';
        pbuf[3] = '2';
        pbuf[4] = '\0';
        if ( errQUEUE_FULL == xQueueSend(g_hQueue[1].handle, &pbuf, 0) )
        {
            if ( pbuf ) {
                vPortFree(pbuf);
            }
            xprintf("[INFO]: Queue[1] depth is [%d], but now [%d]!\n", g_hQueue[1].depth, GET_VALUE_L16(g_hScoreBoard.item));
        } else {
            xSemaphoreTake(g_hScoreBoard.lock, OS_WAIT_FOREVER);
            g_hScoreBoard.item += 0x00001; // item_b + 1
            xSemaphoreGive(g_hScoreBoard.lock);

            xEventGroupSetBits(g_hEventGroup, BIT_2);
        }
    }
}

mTaskDefine(user3_task)
{
    uint8_t *pbuf[QUEUE_NUM];

    for (;;)
    {
        xEventGroupWaitBits(g_hEventGroup, BIT_1|BIT_2, pdFALSE, pdTRUE, OS_WAIT_FOREVER); // .xClearOnExit = pdFALSE, .xWaitForAllBits = pdTRUE
        xEventGroupClearBits(g_hEventGroup, BIT_1|BIT_2);

        for (uint32_t i=0; i<QUEUE_NUM; i++)
        {
            xQueueReceive(g_hQueue[i].handle, &pbuf[i], OS_WAIT_FOREVER);
        }

        xSemaphoreTake(g_hScoreBoard.lock, OS_WAIT_FOREVER);
        g_hScoreBoard.item -= 0x10000; // item_a - 1
        g_hScoreBoard.item -= 0x00001; // item_b - 1
        xSemaphoreGive(g_hScoreBoard.lock);

        xprintf("[INFO]: Recving data [%s %s].\n", pbuf[0], pbuf[1]);

        for (uint32_t i=0; i<QUEUE_NUM; i++)
        {
            if ( pbuf[i] ) {
                vPortFree(pbuf[i]);
            }
        }
    }
}

void uTaskCreate(void)
{
    xprintf("\n");
    xprintf("[INFO]: This is a 'FreeRTOS' demo.\n");
    xprintf("\n");

    vPortInitialiseBlocks();

    g_hScoreBoard.item = 0;
    g_hScoreBoard.lock = xSemaphoreCreateMutex();
    if ( !g_hScoreBoard.lock )
    {
        xprintf("[INFO]: Mutex create failed!\n");
        return;
    } else {
        xprintf("[INFO]: Mutex create success.\n");
    }

    g_hEventGroup = xEventGroupCreate();
    if ( !g_hEventGroup )
    {
        xprintf("[INFO]: Event create failed!\n");
        return;
    } else {
        xprintf("[INFO]: Event create success.\n");
    }

    for (uint32_t i=0; i<QUEUE_NUM; i++)
    {
        g_hQueue[i].depth = 10;
        g_hQueue[i].size = sizeof(uint8_t*);
        g_hQueue[i].handle = xQueueCreate(g_hQueue[i].depth, g_hQueue[i].size);
        if ( !g_hQueue[i].handle )
        {
            xprintf("[INFO]: Message queue [%d] create failed!\n", i);
            return;
        } else {
            xprintf("[INFO]: Message queue [%d] create success.\n", i);
        }
    }

    g_hTimer.id = 0;
    g_hTimer.interval = 10*1000;
    g_hTimer.handle = xTimerCreate("timer0", pdMS_TO_TICKS(g_hTimer.interval), pdTRUE, (void*)g_hTimer.id, vTimerCallback);
    if ( !g_hTimer.handle )
    {
        xprintf("[INFO]: Soft timer create failed!\n");
        return;
    } else {
        if ( pdPASS != xTimerStart(g_hTimer.handle, 0) )
        {
            xprintf("[INFO]: Soft timer start failed!\n");
            return;
        } else {
            xprintf("[INFO]: Soft timer start success.\n");
        }
    }

    xprintf("\n");

    mTaskCreate(user1_task, "user1_task", 256, 3);
    mTaskCreate(user2_task, "user2_task", 256, 3);
    mTaskCreate(user3_task, "user3_task", 256, 2);
}
