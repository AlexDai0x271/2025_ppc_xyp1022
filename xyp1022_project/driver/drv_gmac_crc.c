/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_gmac_crc.h"
#include "sysclock.h"

static uint32_t _s_crc_table[CRC_TABLE_DEPTH] = {0};
static uint8_t _s_crc_table_flag = 0;

/**
 * @brief   Reverses the bit order of the input data
 * @details Reverses the bits from least significant to most significant.
 *          For example, for 8-bit data:
 *          Input:  0b10110011
 *          Output: 0b11001101
 *
 * @param   data [in]  The original data to be reversed
 * @param   len  [in]  Number of bits to reverse (must be <= 32)
 *
 * @return  uint32_t   The bit-reversed data
 *
 * @note    1. Implements bit reversal through bit-by-bit operations
 *          2. Time complexity is O(len)
 *
 * @see     CRC_CreateTable()
 *          CRC_Calc()
 *
 * @warning len parameter must not exceed 32 as return type is uint32_t
 */
static uint32_t Reflect(uint32_t data, uint32_t len)
{
    uint32_t value = 0;

    for (uint32_t i = 0; i < len; i++) {
        if (data & 0x1) {
            value |= (1 << (len - i - 1));
        }
        data >>= 1;
    }

    return value;
}

/**
 * @brief   Creates the CRC32 lookup table
 * @details Generates CRC32 lookup table using IEEE 802.3 polynomial 0x04C11DB7.
 *          Each table entry is bit-reversed to optimize CRC calculation.
 *
 * @note    1. Table size is 256 entries (all possible 8-bit input values)
 *          2. Table is stored in static array _s_crc_table
 *          3. Each table entry is computed by:
 *             - First reversing 8 input bits
 *             - Performing 8 shifts and XOR operations
 *             - Finally reversing the 32-bit result
 *
 * @see     Reflect()
 *          CRC_Calc()
 */
static void CRC_CreateTable(void)
{
    for (uint32_t i = 0; i < CRC_TABLE_DEPTH; i++) {
        _s_crc_table[i] = (Reflect(i, 8) << 24);
        for (uint32_t j = 0; j < 8; j++) {
            uint32_t t1, t2;
            t1 = (_s_crc_table[i] << 1);
            t2 = (GET_BIT(_s_crc_table[i], 31) ? 0x04c11db7 : 0);
            _s_crc_table[i] = t1 ^ t2;
        }
        _s_crc_table[i] = Reflect(_s_crc_table[i], 32);
    }
}

/**
 * @brief   Calculates CRC32 checksum for a data buffer
 * @details Implements IEEE 802.3 standard CRC32 algorithm using table lookup
 *          method for faster computation. Automatically initializes the lookup
 *          table if not already initialized.
 *
 * @param   buf [in]  Pointer to input data buffer
 * @param   len [in]  Length of data in bytes
 *
 * @return  uint32_t  The calculated CRC32 value
 *
 * @note    1. Initial CRC value is 0xFFFFFFFF
 *          2. Uses table lookup method requiring only one lookup and two XOR
 *             operations per byte
 *          3. Compliant with IEEE 802.3 Ethernet CRC32 standard
 *
 * @see     CRC_CreateTable()
 *
 * @example
 * uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
 * uint32_t crc = CRC_Calc(data, sizeof(data));
 */
uint32_t CRC_Calc(uint8_t *buf, uint32_t len)
{
    uint32_t value = 0xffffffff;

    if (!_s_crc_table_flag) {
        _s_crc_table_flag = 1;
        CRC_CreateTable();
    }

    for (uint32_t i = 0; i < len; i++) {
        value = _s_crc_table[(value ^ buf[i]) & 0xff] ^ (value >> 8);
    }

    return value;
}
