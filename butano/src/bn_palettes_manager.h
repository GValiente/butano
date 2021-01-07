/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTES_MANAGER_H
#define BN_PALETTES_MANAGER_H

#include "bn_common.h"

namespace bn
{

class palettes_bank;

namespace palettes_manager
{
    [[nodiscard]] palettes_bank& sprite_palettes_bank();

    [[nodiscard]] palettes_bank& bg_palettes_bank();

    void update();

    void commit();

    void stop();
}

}

#endif
