/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_gpio_manager.h"

#include "../hw/include/bn_hw_gpio.h"

#include "bn_date.cpp.h"
#include "bn_time.cpp.h"
#include "bn_rumble.cpp.h"

namespace bn::gpio_manager
{

namespace
{
    enum class rtc_status_type : uint8_t
    {
        INITIAL,
        FAIL,
        OK
    };

    class static_data
    {

    public:
        rtc_status_type rtc_status = rtc_status_type::INITIAL;
        bool rumble_enabled = false;
    };

    BN_DATA_EWRAM static_data data;

    void _check_init_rtc()
    {
        if(data.rtc_status == rtc_status_type::INITIAL)
        {
            if(hw::gpio::init_rtc())
            {
                data.rtc_status = rtc_status_type::OK;
            }
            else
            {
                data.rtc_status = rtc_status_type::FAIL;
            }
        }
    }

    void _check_rumble_enabled()
    {
        if(data.rumble_enabled)
        {
            hw::gpio::set_rumble_enabled(true);
        }
    }
}

bool rtc_active()
{
    _check_init_rtc();
    _check_rumble_enabled();
    return data.rtc_status == rtc_status_type::OK;
}

optional<date> current_date()
{
    optional<date> result;
    _check_init_rtc();

    if(data.rtc_status == rtc_status_type::OK)
    {
            result = hw::gpio::rtc_date();
    }

    _check_rumble_enabled();
    return result;
}

optional<time> current_time()
{
    optional<time> result;
    _check_init_rtc();

    if(data.rtc_status == rtc_status_type::OK)
    {
        result = hw::gpio::rtc_time();
    }

    _check_rumble_enabled();
    return result;
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
    _check_rumble_enabled();
}

}
