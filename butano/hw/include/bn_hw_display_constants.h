/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DISPLAY_CONSTANTS_H
#define BN_HW_DISPLAY_CONSTANTS_H

#include "bn_common.h"

namespace bn::hw::display
{
    [[nodiscard]] constexpr int width()
    {
        return 240;
    }

    [[nodiscard]] constexpr int height()
    {
        return 160;
    }
}

#endif
