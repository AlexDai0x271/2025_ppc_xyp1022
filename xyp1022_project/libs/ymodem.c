/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "xprintf.h"
#include "sysclock.h"
#include "drv_common.h"
#include "ymodem.h"

/* FatFS core includes */
#include "ff.h"

// Note:
//  1. There are some bug of SD driver, we do not modify them now
//  2. We let 'fobj' link into SRAM to avoid SD timeout occured by DDR access

static FIL m_fobj;

typedef struct YMODEM_CTX_TAG
{
    uint8_t ui8Code;
    uint8_t ui8Index;
    uint8_t aui8Data[RYM_PACKET_1K_SIZE + RYM_PACKET_OVERHEAD];
    uint16_t ui16PacketLength;
    enum
    {
        eRymInit = 0,
        eRymRecv,
        eRymDone,
        eRymFail,
    } eRymState;
    uint16_t ui16WrPosition;
    uint16_t ui16RdPosition;
} YMODEM_CTX_T;

static YMODEM_CTX_T mRymCtx = {
    .eRymState = eRymInit,
};

static uint16_t
YmodemCrc16(
    uint8_t *pui8Data,
    uint32_t ui32Length)
{
    uint16_t ui16Dummy;
    uint16_t ui16Crc = 0;

    while (ui32Length--) {
        ui16Dummy = (ui16Crc >> 8) ^ *pui8Data++;
        ui16Dummy ^= (ui16Dummy >> 4);
        ui16Crc = (ui16Crc << 8) ^ (ui16Dummy << 12) ^ (ui16Dummy << 5) ^ ui16Dummy;
    }

    return ui16Crc;
}

inline __attribute__((always_inline)) void YmodemSendOneByte(
    uint8_t ui8Data)
{
    HW_UART_Write(UART, ui8Data);
}

inline __attribute__((always_inline)) void YmodemRecvOneByte(
    uint8_t *pui8Data)
{
    *pui8Data = HW_UART_Read(UART);
}

static uint8_t
YmodemRecvPacket(
    YMODEM_CTX_T *pRymCtx,
    uint8_t ui8NextAckData)
{
    int bFinished = FALSE;

    pRymCtx->ui16WrPosition = 0;
    pRymCtx->ui16RdPosition = 0;

    YmodemSendOneByte(ui8NextAckData);

    while (!bFinished) {
        YmodemRecvOneByte(&pRymCtx->aui8Data[pRymCtx->ui16WrPosition]);
        pRymCtx->ui16WrPosition++;

        while (pRymCtx->ui16WrPosition > pRymCtx->ui16RdPosition) {
            if (pRymCtx->ui16RdPosition == 0) {
                pRymCtx->ui8Code = pRymCtx->aui8Data[pRymCtx->ui16RdPosition];
                switch (pRymCtx->ui8Code) {
                case RYM_CODE_SOH:
                    pRymCtx->ui16PacketLength = RYM_PACKET_SIZE;
                    pRymCtx->eRymState = eRymRecv;
                    break;
                case RYM_CODE_STX:
                    pRymCtx->ui16PacketLength = RYM_PACKET_1K_SIZE;
                    pRymCtx->eRymState = eRymRecv;
                    break;
                default:
                    pRymCtx->ui16PacketLength = 0;
                    pRymCtx->eRymState = eRymDone;
                    bFinished = TRUE;
                    break;
                }
                pRymCtx->ui16RdPosition++;
                continue;
            }

            if (pRymCtx->ui16RdPosition == 1) {
                pRymCtx->ui8Index = pRymCtx->aui8Data[pRymCtx->ui16RdPosition];
                pRymCtx->ui16RdPosition++;
                continue;
            }

            if (pRymCtx->ui16RdPosition == 2) {
                if (pRymCtx->ui8Index & pRymCtx->aui8Data[pRymCtx->ui16RdPosition]) {
                    pRymCtx->ui16PacketLength = 0;
                    pRymCtx->eRymState = eRymFail;
                    bFinished = TRUE;
                    break;
                } else {
                    pRymCtx->ui16RdPosition++;
                    continue;
                }
            }

            if (pRymCtx->ui16RdPosition == (RYM_PACKET_HEADER + pRymCtx->ui16PacketLength + 1)) {
                uint16_t ui16RecvCrc = 0;
                uint16_t ui16CalcCrc = 0;

                ui16RecvCrc |= (pRymCtx->aui8Data[RYM_PACKET_HEADER + pRymCtx->ui16PacketLength + 0] << 0);
                ui16RecvCrc |= (pRymCtx->aui8Data[RYM_PACKET_HEADER + pRymCtx->ui16PacketLength + 1] << 1);

                ui16CalcCrc = YmodemCrc16(&pRymCtx->aui8Data[RYM_PACKET_HEADER], pRymCtx->ui16PacketLength);

                pRymCtx->eRymState = (ui16CalcCrc != ui16RecvCrc) ? eRymFail : eRymDone;

                bFinished = TRUE;
                break;
            }

            pRymCtx->ui16RdPosition++;
        }
    }

    return 0;
}

uint32_t
Ymodem_ReceiveTask(void)
{
    FRESULT res;
    UINT bw;

    mRymCtx.eRymState = eRymInit;

    uint8_t ui8NextAckData = RYM_CODE_CRC;

    int bHaveRecvedHeader = FALSE;
    int bHaveRecvedEOT = FALSE;

    while (1) {
        YmodemRecvPacket(&mRymCtx, ui8NextAckData);

        switch (mRymCtx.ui8Code) {
        case RYM_CODE_SOH:
        case RYM_CODE_STX:
            if (!bHaveRecvedHeader && (mRymCtx.ui8Index == 0)) {
                bHaveRecvedHeader = TRUE;
                res = f_open(&m_fobj, (void *)&mRymCtx.aui8Data[RYM_PACKET_HEADER], FA_CREATE_ALWAYS | FA_WRITE);
                if (res != FR_OK) {
                    bHaveRecvedEOT = TRUE;
                }
            } else {
                f_write(&m_fobj, &mRymCtx.aui8Data[RYM_PACKET_HEADER], mRymCtx.ui16PacketLength, &bw);
            }
            ui8NextAckData = RYM_CODE_ACK;
            break;
        case RYM_CODE_EOT:
            bHaveRecvedEOT = TRUE;
            for (uint32_t i = 0; i < 6; i++) {
                YmodemSendOneByte(RYM_CODE_CAN);
            }
            for (uint32_t i = 0; i < 10; i++) {
                uint8_t ui8DummyData;
                YmodemRecvOneByte(&ui8DummyData);
                UNUSED(ui8DummyData);
            }
            xprintf("\033[1A");
            xprintf("\033[K");
            xprintf("recv finished.\n");
            break;
        default:
            break;
        }

        if (bHaveRecvedEOT) {
            f_close(&m_fobj);
            break;
        }
    }

    return 0;
}
