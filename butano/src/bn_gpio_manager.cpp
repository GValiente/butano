/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_gpio_manager.h"

#include "../hw/include/bn_hw_gpio.h"

#include "bn_rumble.cpp.h"

namespace bn::gpio_manager
{

namespace
{
    class static_data
    {

    public:
        bool rumble_enabled = false;
    };

    BN_DATA_EWRAM static_data data;
}

bool rumble_enabled()
{
    return data.rumble_enabled;
}

void set_rumble_enabled(bool enabled)
{
    if(data.rumble_enabled != enabled)
    {
        data.rumble_enabled = enabled;
        hw::gpio::set_rumble_enabled(enabled);
    }
}

void sleep()
{
    if(data.rumble_enabled)
    {
        hw::gpio::set_rumble_enabled(false);
    }
}

void wake_up()
{
    if(data.rumble_enabled)
    {
        hw::gpio::set_rumble_enabled(true);
    }
}

}
