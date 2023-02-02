/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_rumble.h"

#include "bn_gpio_manager.h"

namespace bn::rumble
{

bool enabled()
{
    return gpio_manager::rumble_enabled();
}

void set_enabled(bool enabled)
{
    gpio_manager::set_rumble_enabled(enabled);
}

}
