/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_DISPLAY_H
#define BTN_DISPLAY_H

#include "../hw/include/btn_hw_display_constants.h"

namespace btn::display
{
    [[nodiscard]] constexpr int width()
    {
        return hw::display::width();
    }

    [[nodiscard]] constexpr int height()
    {
        return hw::display::height();
    }
}

#endif
