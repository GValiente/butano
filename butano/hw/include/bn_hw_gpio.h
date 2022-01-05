/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_GPIO_H
#define BN_HW_GPIO_H

#include "bn_common.h"

namespace bn::hw::gpio
{

inline void init()
{
    volatile uint16_t& gpio_direction_register = *reinterpret_cast<uint16_t*>(0x80000C6);
    uint16_t gpio_direction = gpio_direction_register;
    gpio_direction_register = gpio_direction | (1 << 3);
}

inline void set_rumble_enabled(bool enabled)
{
    volatile uint16_t& gpio_data_register = *reinterpret_cast<uint16_t*>(0x80000C4);
    uint16_t gpio_data = gpio_data_register;

    if(enabled)
    {
        gpio_data_register = gpio_data | (1 << 3);
    }
    else
    {
        gpio_data_register = gpio_data & ~(1 << 3);
    }
}

}

#endif
