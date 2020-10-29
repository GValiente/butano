/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_SPRITES_CONSTANTS_H
#define BTN_HW_SPRITES_CONSTANTS_H

#include "btn_common.h"

namespace btn::hw::sprites
{
    [[nodiscard]] constexpr int count()
    {
        return 128;
    }

    [[nodiscard]] constexpr int min_bg_priority()
    {
        return 0;
    }

    [[nodiscard]] constexpr int max_bg_priority()
    {
        return 3;
    }
}

#endif
