/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_BGS_CONSTANTS_H
#define BTN_HW_BGS_CONSTANTS_H

#include "btn_common.h"

namespace btn::hw::bgs
{
    [[nodiscard]] constexpr int min_priority()
    {
        return 0;
    }

    [[nodiscard]] constexpr int max_priority()
    {
        return 3;
    }
}

#endif
