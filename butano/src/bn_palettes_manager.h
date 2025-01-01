/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTES_MANAGER_H
#define BN_PALETTES_MANAGER_H

#include "bn_optional_fwd.h"

namespace bn
{

class color;
class palettes_bank;

namespace palettes_manager
{
    void init(const optional<color>& transparent_color);

    [[nodiscard]] palettes_bank& sprite_palettes_bank();

    [[nodiscard]] palettes_bank& bg_palettes_bank();

    void update();

    void commit(bool use_dma);

    void stop();
}

}

#endif
