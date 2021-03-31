/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
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
        bool commit_rumble = false;
    };

    BN_DATA_EWRAM static_data data;
}

void init()
{
    hw::gpio::init();
}

bool rumble_enabled()
{
    return data.rumble_enabled;
}

void set_rumble_enabled(bool enabled)
{
    data.rumble_enabled = enabled;
    data.commit_rumble = true;
}

void commit()
{
    if(data.commit_rumble)
    {
        hw::gpio::set_rumble_enabled(data.rumble_enabled);
        data.commit_rumble = false;
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
        data.commit_rumble = false;
    }
}

void stop()
{
    sleep();
}

}
