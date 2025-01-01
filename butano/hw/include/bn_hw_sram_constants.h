/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SRAM_CONSTANTS_H
#define BN_HW_SRAM_CONSTANTS_H

#include "bn_common.h"

namespace bn::hw::sram
{
    [[nodiscard]] constexpr int size()
    {
        return 32 * 1024;
    }
}

#endif
