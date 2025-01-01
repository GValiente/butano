/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_GPIO_H
#define BN_HW_GPIO_H

#include "bn_date.h"
#include "bn_time.h"
#include "bn_optional.h"
#include "../3rd_party/agbabi/include/agbabi.h"

namespace bn::hw::gpio
{

namespace
{
    [[nodiscard]] int _bcd_decode(unsigned x)
    {
        return int(x & 0xf) + int(x >> 4) * 10;
    }
}

[[nodiscard]] const char* init();

[[nodiscard]] inline bool init_rtc()
{
    return __agbabi_rtc_init() == 0;
}

[[nodiscard]] inline optional<date> rtc_date()
{
    optional<date> result;
    unsigned hw_date = __agbabi_rtc_datetime()[0];
    int year = _bcd_decode(hw_date & 0xff);
    int month = _bcd_decode(hw_date >> 8 & 0xff);
    int month_day = _bcd_decode(hw_date >> 16 & 0xff);
    int week_day = _bcd_decode(hw_date >> 24 & 0xff);

    if(date::valid(year, month, month_day, week_day))
    {
        result.emplace(year, month, month_day, week_day);
    }

    return result;
}

[[nodiscard]] inline optional<time> rtc_time()
{
    optional<time> result;
    unsigned hw_time = __agbabi_rtc_time();
    int hour = _bcd_decode(hw_time & 0xff);
    int minute = _bcd_decode(hw_time >> 8 & 0xff);
    int second = _bcd_decode(hw_time >> 16 & 0xff);

    if(hour >= 80)
    {
        hour -= 80;
    }

    if(time::valid(hour, minute, second))
    {
        result.emplace(hour, minute, second);
    }

    return result;
}

inline void set_rumble_enabled(bool enabled)
{
    volatile uint16_t& gpio_direction_register = *reinterpret_cast<uint16_t*>(0x80000C6);
    uint16_t gpio_direction = gpio_direction_register;
    gpio_direction_register = gpio_direction | (1 << 3);

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
