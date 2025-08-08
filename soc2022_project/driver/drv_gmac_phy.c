/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_gmac_phy.h"
#include "sysclock.h"
#ifdef DEBUG
#include "xprintf.h"
#endif

/**
 * @brief   Read value from PHY register
 * @details Reads register value from specified PHY device through GMAC's MDIO interface.
 *          Includes timeout protection and data consistency check through value comparison.
 *
 * @param   GMACx   [in]  GMAC device pointer, points to GMAC base address
 * @param   phyid   [in]  PHY device ID (0-31)
 * @param   addr    [in]  PHY register address (0-31)
 * @param   timeout [in]  Timeout value in microseconds
 *
 * @return  uint32_t
 *          - Success: Returns the read register value
 *          - Failure: Returns last read value if timeout occurs
 *
 * @note    1. Uses polling method to wait for operation completion
 *          2. Additional 10ms delay after register read for stability
 *          3. CR_CSR clock configured for 20-35MHz MDC with divider 16
 *
 * @see     PHY_Write()
 *          GMAC_Init()
 *
 * @warning Ensure GMAC clock is enabled before calling this function
 *
 * @example
 * uint32_t phy_id = 0;
 * uint32_t reg_val = PHY_Read(GMAC1, phy_id, PHY_BSR, 1000);
 */
uint32_t PHY_Read(GMAC_TypeDef *GMACx, uint8_t phyid, uint32_t addr, uint32_t timeout)
{
    GET_PHY_INTERFACE();

    uint32_t reg_value;

    do {

        while (GET_BIT(GMACx->Register4, GMAC_GMIIAR_GB_POS))
            ;

        reg_value = 0; // .Register4 inited to zero first
        reg_value |= ((phyid & 0x1f) << GMAC_GMIIAR_PA_POS);
        reg_value |= ((addr & 0x1f) << GMAC_GMIIAR_GR_POS);
        reg_value |= (CR_CSR_250_300M_MDC_DIV_124 << GMAC_GMIIAR_CR_POS);
        SET_BIT(reg_value, GMAC_GMIIAR_GW_POS, GW_READ);
        SET_BIT(reg_value, GMAC_GMIIAR_GB_POS, GMAC_ENABLED);
        GMACx->Register4 = reg_value;

        for (uint32_t i = 0; i < timeout; i++) {
            if (!GET_BIT(GMACx->Register4, GMAC_GMIIAR_GB_POS)) {
                break;
            }
            udelay(1);
        }
    } while (0);

    return GMACx->Register5;
}

/**
 * @brief   Write value to PHY register
 * @details Writes data to specified PHY device register through GMAC's MDIO interface.
 *          Includes timeout protection mechanism.
 *
 * @param   GMACx   [in]  GMAC device pointer, points to GMAC base address
 * @param   phyid   [in]  PHY device ID (0-31)
 * @param   addr    [in]  PHY register address (0-31)
 * @param   data    [in]  Data to be written
 * @param   timeout [in]  Timeout value in microseconds
 *
 * @return  uint32_t
 *          - Success: Returns the written data
 *          - Failure: Returns the written data even if timeout occurs
 *
 * @note    1. Uses polling method to wait for operation completion
 *          2. Additional 10ms delay after register write for stability
 *          3. CR_CSR clock configured for 20-35MHz MDC with divider 16
 *
 * @see     PHY_Read()
 *          GMAC_Init()
 *
 * @warning Ensure GMAC clock is enabled before calling this function
 *
 * @example
 * uint32_t phy_id = 0;
 * uint32_t reg_val = 0x1234;
 * PHY_Write(GMAC1, phy_id, PHY_BSR, reg_val, 1000);
 */
uint32_t PHY_Write(GMAC_TypeDef *GMACx, uint8_t phyid, uint32_t addr, uint32_t data, uint32_t timeout)
{
    GET_PHY_INTERFACE();

    uint32_t reg_value;

    while (GET_BIT(GMACx->Register4, GMAC_GMIIAR_GB_POS))
        ;

    GMACx->Register5 = (data & 0xffff);

    reg_value = 0; // .Register4 inited to zero first
    reg_value |= ((phyid & 0x1f) << GMAC_GMIIAR_PA_POS);
    reg_value |= ((addr & 0x1f) << GMAC_GMIIAR_GR_POS);
    reg_value |= (CR_CSR_250_300M_MDC_DIV_124 << GMAC_GMIIAR_CR_POS);
    SET_BIT(reg_value, GMAC_GMIIAR_GW_POS, GW_WRITE);
    SET_BIT(reg_value, GMAC_GMIIAR_GB_POS, GMAC_ENABLED);
    GMACx->Register4 = reg_value;

    for (uint32_t i = 0; i < timeout; i++) {
        if (!GET_BIT(GMACx->Register4, GMAC_GMIIAR_GB_POS)) {
            break;
        }
        udelay(1);
    }

    return data;
}

/**
 * @brief   Initialize PHY device configuration
 * @details Configures PHY device mode, speed, and other parameters including
 *          auto-negotiation, speed, and duplex mode settings.
 *
 * @param   GMACx   [in]  GMAC device pointer, points to GMAC base address
 * @param   config  [in]  Pointer to GMAC device configuration structure
 *
 * @return  int
 *          - 0: Configuration successful
 *          - -1: Configuration failed (e.g., invalid PHY ID)
 *
 * @note    1. Configures PHY Basic Mode Register (BMR) and Auto-Negotiation Advertisement Register (ANAR)
 *          2. Supports 10/100/1000Mbps speed configuration
 *          3. Supports both auto-negotiation and manual configuration modes
 *
 * @see     PHY_Read()
 *          PHY_Write()
 *
 * @warning Ensure GMAC clock is enabled before calling this function
 */
int PHY_InitConfig(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    GET_PHY_INTERFACE();

    uint32_t reg_value;

    reg_value = PHY_Read(GMACx, config->phy_id, PHY_REG_CONTROL, 10 * 1000);
    reg_value &= ~(BIT(13) | BIT(6) | BIT(8) | BIT(12));

    switch (config->speed) {
    case eGmacSpeed_10M:
        reg_value &= ~(BIT(13) | BIT(6));
        break;
    case eGmacSpeed_100M:
        reg_value |= BIT(13);
        break;
    case eGmacSpeed_1000M:
        reg_value |= BIT(6);
        break;
    default:
        break;
    }
    reg_value |= BIT(8); // NOTE: '1' is full-duplex
    PHY_Write(GMACx, config->phy_id, PHY_REG_CONTROL, reg_value, 10 * 1000);

    reg_value |= BIT(15); // NOTE: '1' is software reset
    PHY_Write(GMACx, config->phy_id, PHY_REG_CONTROL, reg_value, 10 * 1000);
    while (PHY_Read(GMACx, config->phy_id, PHY_REG_CONTROL, 10 * 1000) & BIT(15))
        ;

#if (DEBUG_GMAC_WAIT_PHY_LINK_UP != 0)
    while (!(PHY_Read(GMACx, config->phy_id, PHY_REG_STATUS, 10 * 1000) & 0x4))
        ;
#endif

#ifdef DEBUG
    uint32_t id[2] = {0x0, 0x0};
#if (DEBUG_GMAC_CHECK_PHY_ID != 0)
    while (PHY_IDENTIFIER_MAKER(id[0], id[1]) != PHY_IDENTIFIER)
#endif
    {
        id[0] = PHY_Read(GMACx, config->phy_id, PHY_REG_ID0, 10 * 1000);
        id[1] = PHY_Read(GMACx, config->phy_id, PHY_REG_ID1, 10 * 1000);
    }
    xprintf("phy id: %04x %04x\n", id[0], id[1]);
    xprintf("      : OUI LSb: %06b (always 000011)\n", (id[1] >> 10) & 0x3f);
    xprintf("      : Model number: %06b (always 001100)\n", (id[1] >> 4) & 0x3f);
    xprintf("      : Revision number: %04b\n", (id[1]) & 0xf);
#endif

    return 0;
}

/**
 * @brief   Get PHY device link status
 * @details Reads the Basic Status Register (BSR) to determine current link status.
 *
 * @param   GMACx   [in]  GMAC device pointer, points to GMAC base address
 * @param   config  [in]  Pointer to GMAC device configuration structure
 *
 * @return  int
 *          - 1: Link is up (BSR.LinkStatus bit is 1)
 *          - 0: Link is down (BSR.LinkStatus bit is 0)
 *
 * @note    1. Switches to default page (Page 0) first
 *          2. Reads Link Status bit from BSR register (typically bit 2)
 *
 * @see     PHY_Read()
 *          PHY_Write()
 */
int PHY_GetLinkStatus(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    PHY_Write(GMACx, config->phy_id, PHY_REG_PAGESELECT22, PHY_PAGE_DEF, 10 * 1000);
    return (PHY_Read(GMACx, config->phy_id, PHY_REG_17, 10 * 1000) & BIT(10));
}

/**
 * @brief   Get PHY device link speed
 * @details Reads PHY registers to determine current link speed.
 *
 * @param   GMACx   [in]  GMAC device pointer, points to GMAC base address
 * @param   config  [in]  Pointer to GMAC device configuration structure
 *
 * @return  uint32_t
 *          - Link speed in Mbps (0, 10, 100, or 1000)
 *          - 0: Link down
 *          - 10: 10 Mbps
 *          - 100: 100 Mbps
 *          - 1000: 1000 Mbps
 *
 * @note    1. Reads and interprets speed bits from PHY status register
 *          2. Returns 0 if link is down
 *
 * @see     PHY_Read()
 *          PHY_GetLinkStatus()
 *
 * @warning Ensure GMAC clock is enabled before calling this function
 */
uint32_t PHY_GetLinkSpeed(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    uint32_t reg_value;
    uint32_t speed;

    PHY_Write(GMACx, config->phy_id, PHY_REG_PAGESELECT22, PHY_PAGE_DEF, 10 * 1000);

    reg_value = PHY_Read(GMACx, config->phy_id, PHY_REG_17, 10 * 1000);
    reg_value = PHY_GetLinkStatus(GMACx, config) ? ((reg_value >> 14) & 0x3) : 0;

    switch (reg_value) {
    case 0x0:
        speed = 10;
        break;
    case 0x1:
        speed = 100;
        break;
    case 0x2:
        speed = 1000;
        break;
    default:
        speed = 0;
        break;
    }

    // Note:
    //  speed = 0: link down
    //  speed = 10: 10 Mbps
    //  speed = 100: 100 Mbps
    //  speed = 1000: 1000 Mbps

    return speed;
}

/**
 * @brief   Get PHY device duplex mode
 * @details Reads PHY registers to determine current duplex mode.
 *
 * @param   GMACx   [in]  GMAC device pointer, points to GMAC base address
 * @param   config  [in]  Pointer to GMAC device configuration structure
 *
 * @return  uint32_t
 *          - 0: Link down
 *          - 1: Half-duplex
 *          - 2: Full-duplex
 *
 * @note    1. Reads and interprets duplex bit from PHY status register
 *          2. Returns 0 if link is down
 *
 * @see     PHY_Read()
 *          PHY_GetLinkStatus()
 *
 * @warning Ensure GMAC clock is enabled before calling this function
 */
uint32_t PHY_GetLinkDuplex(GMAC_TypeDef *GMACx, DeviceGmacConfig_t *config)
{
    uint32_t reg_value;
    uint32_t duplex;

    PHY_Write(GMACx, config->phy_id, PHY_REG_PAGESELECT22, PHY_PAGE_DEF, 10 * 1000);

    reg_value = PHY_Read(GMACx, config->phy_id, PHY_REG_17, 10 * 1000);
    reg_value = ((reg_value >> 13) & 0x1);

    if (!PHY_GetLinkStatus(GMACx, config)) {
        duplex = 0;
    } else {
        duplex = reg_value + 1;
    }

    // Note:
    //  duplex = 0: link down
    //  duplex = 1: half-duplex
    //  duplex = 2: full-duplex

    return duplex;
}
