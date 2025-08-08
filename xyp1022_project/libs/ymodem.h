/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#ifndef YMODEM_H
#define YMODEM_H

#include <stdio.h>
#include <string.h>

#define RYM_PACKET_HEADER    (3)
#define RYM_PACKET_TRAILER   (2)
#define RYM_PACKET_OVERHEAD  (RYM_PACKET_HEADER + RYM_PACKET_TRAILER)

#define RYM_PACKET_SIZE      (128)
#define RYM_PACKET_1K_SIZE   (1024)

#define RYM_FILE_NAME_LENGTH (256)
#define RYM_FILE_SIZE_LENGTH (16)

#define RYM_CODE_NULL        (0x00)
#define RYM_CODE_SOH         (0x01) /* start of 128-byte data packet */
#define RYM_CODE_STX         (0x02) /* start of 1024-byte data packet */
#define RYM_CODE_EOT         (0x04) /* end of transmission */
#define RYM_CODE_ACK         (0x06) /* acknowledge */
#define RYM_CODE_NAK         (0x15) /* negative acknowledge */
#define RYM_CODE_CAN         (0x18) /* aborts transfer */
#define RYM_CODE_CRC         (0x43) /* 'C' == 0x43, request 16-bit CRC, use in place of first NAK for CRC mode */

uint32_t Ymodem_ReceiveTask(void);

#endif /* YMODEM_H */
