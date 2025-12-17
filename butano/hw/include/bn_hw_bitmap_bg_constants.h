/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_BITMAP_BG_CONSTANTS_H
#define BN_HW_BITMAP_BG_CONSTANTS_H

#include "bn_common.h"

namespace bn::hw::bitmap_bg
{
    [[nodiscard]] constexpr int palette_width()
    {
        return 240;
    }

    [[nodiscard]] constexpr int palette_height()
    {
        return 160;
    }

    [[nodiscard]] constexpr int dp_direct_width()
    {
        return 160;
    }

    [[nodiscard]] constexpr int dp_direct_height()
    {
        return 128;
    }
}

#endif
