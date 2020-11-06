/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_TILES_H
#define BTN_BG_TILES_H

#include "btn_common.h"

namespace btn::bg_tiles
{
    [[nodiscard]] int used_tiles_count();

    [[nodiscard]] int available_tiles_count();

    [[nodiscard]] int used_blocks_count();

    [[nodiscard]] int available_blocks_count();
}

#endif
