/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_BGS_CONSTANTS_H
#define BN_HW_BGS_CONSTANTS_H

#include "bn_common.h"

namespace bn::hw::bgs
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
