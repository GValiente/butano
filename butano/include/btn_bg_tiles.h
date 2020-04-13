#ifndef BTN_BG_TILES_H
#define BTN_BG_TILES_H

#include "../hw/include/btn_hw_bg_blocks_constants.h"

namespace btn::bg_tiles
{
    [[nodiscard]] int used_tiles_count();

    [[nodiscard]] int available_tiles_count();

    [[nodiscard]] int used_blocks_count();

    [[nodiscard]] int available_blocks_count();
}

#endif
