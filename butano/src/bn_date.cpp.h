/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_date.h"

#include "bn_optional.h"
#include "bn_gpio_manager.h"

namespace bn
{

bool date::active()
{
    return gpio_manager::rtc_active();
}

optional<date> date::current()
{
    return gpio_manager::current_date();
}

}
