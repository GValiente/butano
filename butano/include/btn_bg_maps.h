/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_MAPS_H
#define BTN_BG_MAPS_H

#include "../hw/include/btn_hw_bg_blocks_constants.h"

namespace btn::bg_maps
{
    [[nodiscard]] int used_cells_count();

    [[nodiscard]] int available_cells_count();

    [[nodiscard]] int used_blocks_count();

    [[nodiscard]] int available_blocks_count();
}

#endif
