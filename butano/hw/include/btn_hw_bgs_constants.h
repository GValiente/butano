/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_BGS_CONSTANTS_H
#define BTN_HW_BGS_CONSTANTS_H

#include "btn_sort_key.h"

namespace btn::bgs
{
    [[nodiscard]] constexpr int max_priority()
    {
        return 3;
    }

    [[nodiscard]] constexpr int max_z_order()
    {
        return sort_key::max_z_order();
    }

    [[nodiscard]] constexpr int min_z_order()
    {
        return sort_key::min_z_order();
    }
}

#endif
