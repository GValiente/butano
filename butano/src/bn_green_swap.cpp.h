/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_green_swap.h"

#include "bn_display_manager.h"

namespace bn::green_swap
{

bool enabled()
{
    return display_manager::green_swap_enabled();
}

void set_enabled(bool enabled)
{
    display_manager::set_green_swap_enabled(enabled);
}

}
