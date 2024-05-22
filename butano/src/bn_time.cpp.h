/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_time.h"

#include "bn_optional.h"
#include "bn_gpio_manager.h"

namespace bn
{

bool time::active()
{
    return gpio_manager::rtc_active();
}

optional<time> time::current()
{
    return gpio_manager::current_time();
}

}
