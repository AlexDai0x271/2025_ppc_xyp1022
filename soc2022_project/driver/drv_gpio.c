/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "drv_gpio.h"
#include "sysclock.h"

/*
功能描述：

名称：g_DeviceGpioDefaultConfig
功能：配置通用输入/输出（GPIO）引脚的默认参数
参数：无
返回值：无
输入：无
输出：无
上层函数：无
下层函数：无

附加说明：
- 此结构体配置了单个GPIO引脚的基本操作模式、初始电平状态以及中断模式
- 主要用于控制硬件设备的数字输入输出

详细字段说明：
- pin: 指定GPIO引脚的编号，这里为0
- mode: 设定GPIO的工作模式，这里设置为输出模式（eGpioMode_Output）
- init_level: 初始电平状态，此设置仅在GPIO为输出模式时有效，这里设为高电平（LEVEL_HIGH）
- int_mode: GPIO的中断模式，此设置仅在GPIO为输入中断模式（eGpioMode_InputInterrupt）时有效，这里设为电平高触发中断（eGpioIntMode_LevelHigh）
- user_data: 用于存储额外的用户定义数据，初始设置为0

注意：
- 确保在使用这些配置之前，正确初始化GPIO硬件，并设置相应的GPIO控制寄存器
- 根据实际应用需求，可能需要调整GPIO的工作模式和其他参数
*/

#ifdef DEBUG
// Note: The default configuration parameters,
//       should not be used unless debug.
DeviceGpioConfig_t g_DeviceGpioDefaultConfig =
    {
        .pin = 0,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_HIGH,           // NOTE: only valid when .mode is 'eGpioMode_Output'
        .int_mode = eGpioIntMode_LevelHigh, // NOTE: only valid when .mode is 'eGpioMode_InputInterrupt'

        .user_data = 0,
};
#endif // #ifdef DEBUG

// Note:
//  1. should add a external pull-up or pull-down resistance when work on 'eGpioMode_Input/eGpioMode_InputInterrupt'

/*
功能描述：

名称：HW_GPIO_InitConfig
功能：初始化GPIO设备的配置
参数：
  - GPIOx: GPIO设备的基地址
  - config: GPIO设备的配置结构体指针
返回值：成功返回0，失败返回-1或-2
输入：GPIO设备基地址和配置结构体
输出：初始化GPIO设备配置
上层函数：可能由需要初始化GPIO设备的功能调用
下层函数：无

附加说明：
- 该函数设置GPIO的模式、初始电平、中断模式等参数
- 配置完成后，GPIO设备会根据设定的模式和参数进行初始化
*/
int HW_GPIO_InitConfig(GPIO_TypeDef *GPIOx, DeviceGpioConfig_t *config)
{
    uint32_t reg_value;

    if ((NULL == GPIOx) || (NULL == config)) {
        return -1;
    }

    if (config->pin > 15) {
        return -2;
    }

    // disable Gpio first
    SET_BIT(GPIOx->INTEN, config->pin, GPIO_DISABLED);
    SET_BIT(GPIOx->INTMASK, config->pin, INTMASK_MASKED);

    reg_value = GPIOx->INTSTATUS;
    UNUSED(reg_value);

    SET_BIT(GPIOx->PORTA_EOI, config->pin, GPIO_ENABLED);

    if (config->mode == eGpioMode_Output) {
        SET_BIT(GPIOx->PORTA_DDR, config->pin, DDR_OUT);
        SET_BIT(GPIOx->PORTA_DR, config->pin, config->init_level);
    } else {
        SET_BIT(GPIOx->PORTA_DDR, config->pin, DDR_IN);
    }

    SET_BIT(GPIOx->PORTA_CTL, config->pin, CTL_SOFTWARE_MODE);

    if (config->mode == eGpioMode_InputInterrupt) {
        SET_BIT(GPIOx->INTEN, config->pin, GPIO_ENABLED);
        SET_BIT(GPIOx->INTMASK, config->pin, INTMASK_UNMASKED);

        if ((config->int_mode == eGpioIntMode_LevelLow) || (config->int_mode == eGpioIntMode_LevelHigh)) {
            SET_BIT(GPIOx->INTTYPE_LEVEL, config->pin, INTTYPE_LEVEL_SENSITIVE);
        } else {
            SET_BIT(GPIOx->INTTYPE_LEVEL, config->pin, INTTYPE_EDGE_SENSITIVE);
        }

        if ((config->int_mode == eGpioIntMode_LevelLow) || (config->int_mode == eGpioIntMode_EdgeFalling)) {
            SET_BIT(GPIOx->INT_BOTHEDGE, config->pin, GPIO_DISABLED);
            SET_BIT(GPIOx->INT_POLARITY, config->pin, INT_POLARITY_ACTIVE_LOW);
        } else if ((config->int_mode == eGpioIntMode_LevelHigh) || (config->int_mode == eGpioIntMode_EdgeRising)) {
            SET_BIT(GPIOx->INT_BOTHEDGE, config->pin, GPIO_DISABLED);
            SET_BIT(GPIOx->INT_POLARITY, config->pin, INT_POLARITY_ACTIVE_HIGH);
        } else {
            SET_BIT(GPIOx->INT_BOTHEDGE, config->pin, GPIO_ENABLED);
        }

        SET_BIT(GPIOx->DEBOUNCE, config->pin, GPIO_ENABLED);

        drv_irq_enable(GPIO0_IRQn - config->pin);
    }

    config->status = 0;
    return 0;
}

/*
功能描述：

名称：HW_GPIO_ClearInterrupt
功能：清除GPIO设备的中断状态
参数：
  - GPIOx: GPIO设备的基地址
  - pin: 要清除中断状态的引脚号
返回值：成功返回0
输入：GPIO设备基地址和引脚号
输出：清除指定引脚的中断状态
上层函数：可能由需要清除GPIO设备中断状态的功能调用
下层函数：无

附加说明：
- 该函数用于清除GPIO设备的特定引脚中断状态，以确保中断处理的正常进行
*/
uint32_t HW_GPIO_ClearInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    SET_BIT(GPIOx->PORTA_EOI, pin, GPIO_ENABLED);
    return 0;
}

/*
功能描述：

名称：HW_GPIO_GetStatus
功能：获取GPIO设备的当前中断状态
参数：
  - GPIOx: GPIO设备的基地址
返回值：GPIO设备的当前中断状态
输入：GPIO设备基地址
输出：GPIO设备的中断状态
上层函数：可能由需要获取GPIO设备中断状态的功能调用
下层函数：无

附加说明：
- 该函数用于获取GPIO设备的当前中断状态寄存器值，以便进行状态检查和调试
*/
uint32_t HW_GPIO_GetStatus(GPIO_TypeDef *GPIOx)
{
    return GPIOx->INTSTATUS;
}

/*
功能描述：

名称：HW_GPIO_GetPreMaskStatus
功能：获取GPIO设备的原始中断状态（未屏蔽状态）
参数：
  - GPIOx: GPIO设备的基地址
返回值：GPIO设备的原始中断状态
输入：GPIO设备基地址
输出：GPIO设备的原始中断状态
上层函数：可能由需要获取GPIO设备原始中断状态的功能调用
下层函数：无

附加说明：
- 该函数用于获取GPIO设备的原始中断状态寄存器值，以便进行详细的中断状态检查和调试
*/
uint32_t HW_GPIO_GetPreMaskStatus(GPIO_TypeDef *GPIOx)
{
    return GPIOx->RAW_INTSTATUS;
}

#ifdef DEBUG_GPIO

#include "xprintf.h"

int selftest_gpio(void)
{
    GPIO_TypeDef *gpio = GPIOA;

    xprintf("GPIO VERSION = %08x\n", gpio->VER_ID_CODE);
    xprintf("GPIO ID = %08x\n", gpio->ID_CODE);

    DeviceGpioConfig_t g_DeviceGpio0Config = {
        .pin = 0,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_HIGH,
    };
    DeviceGpioConfig_t g_DeviceGpio1Config = {
        .pin = 1,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_LOW,
    };
    DeviceGpioConfig_t g_DeviceGpio2Config = {
        .pin = 2,
        .mode = eGpioMode_InputInterrupt,
        .int_mode = eGpioIntMode_EdgeBoth,
    };
    DeviceGpioConfig_t g_DeviceGpio3Config = {
        .pin = 3,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_LOW,
    };
    DeviceGpioConfig_t g_DeviceGpio4Config = {
        .pin = 4,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_HIGH,
    };
    DeviceGpioConfig_t g_DeviceGpio5Config = {
        .pin = 5,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_LOW,
    };
    DeviceGpioConfig_t g_DeviceGpio6Config = {
        .pin = 6,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_LOW,
    };
    DeviceGpioConfig_t g_DeviceGpio7Config = {
        .pin = 7,
        .mode = eGpioMode_Output,
        .init_level = LEVEL_LOW,
    };

    HW_GPIO_InitConfig(gpio, &g_DeviceGpio0Config);
    HW_GPIO_InitConfig(gpio, &g_DeviceGpio1Config); // NOTE: gpio[1] out to gpio[2]
    HW_GPIO_InitConfig(gpio, &g_DeviceGpio2Config);
    HW_GPIO_InitConfig(gpio, &g_DeviceGpio3Config);
    HW_GPIO_InitConfig(gpio, &g_DeviceGpio4Config);
    HW_GPIO_InitConfig(gpio, &g_DeviceGpio5Config);
    HW_GPIO_InitConfig(gpio, &g_DeviceGpio6Config);
    HW_GPIO_InitConfig(gpio, &g_DeviceGpio7Config);

    xprintf("\n");
    return 0;
}

void selftest_gpio_loop(void)
{
    PAout(0, GET_BIT(~PAstate(0), 0));
    PAout(1, GET_BIT(~PAstate(1), 0));
    PAout(3, GET_BIT(~PAstate(3), 0));
    PAout(4, GET_BIT(~PAstate(4), 0));
    PAout(5, GET_BIT(~PAstate(5), 0));
    PAout(6, GET_BIT(~PAstate(6), 0));
    PAout(7, GET_BIT(~PAstate(7), 0));
}

#endif // #ifdef DEBUG_GPIO
