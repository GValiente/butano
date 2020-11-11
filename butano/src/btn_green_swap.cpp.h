/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_green_swap.h"

#include "btn_display_manager.h"

namespace btn::green_swap
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
