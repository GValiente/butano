/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BGS_H
#define BTN_BGS_H

#include "btn_sort_key.h"
#include "../hw/include/btn_hw_bgs_constants.h"

namespace btn::bgs
{
    [[nodiscard]] constexpr int min_priority()
    {
        return hw::bgs::min_priority();
    }

    [[nodiscard]] constexpr int max_priority()
    {
        return hw::bgs::max_priority();
    }

    [[nodiscard]] constexpr int min_z_order()
    {
        return sort_key::min_z_order();
    }

    [[nodiscard]] constexpr int max_z_order()
    {
        return sort_key::max_z_order();
    }

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();
}

#endif
