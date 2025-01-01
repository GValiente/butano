/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_gpio.h"

namespace bn::hw::gpio
{

namespace
{
    alignas(int) const char rtc_string[] = "SIIRTC_V001";
}

const char* init()
{
    return rtc_string;
}

}
