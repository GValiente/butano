/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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
        bool first_commit = true;
        bool rumble_enabled = false;
        bool commit_rumble = false;
    };

    BN_DATA_EWRAM static_data data;

    void _check_first_commit()
    {
        if(data.first_commit)
        {
            hw::gpio::init();
            data.first_commit = false;
        }
    }
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
        data.commit_rumble = true;
    }
}

void commit()
{
    if(data.commit_rumble)
    {
        _check_first_commit();
        hw::gpio::set_rumble_enabled(data.rumble_enabled);
        data.commit_rumble = false;
    }
}

void sleep()
{
    if(data.rumble_enabled)
    {
        _check_first_commit();
        hw::gpio::set_rumble_enabled(false);
        data.commit_rumble = true;
    }
}

void wake_up()
{
    commit();
}

void stop()
{
    sleep();
}

}
