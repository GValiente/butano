#ifndef BTN_SPRITES_H
#define BTN_SPRITES_H

#include "../hw/include/btn_hw_sprites_constants.h"

namespace btn::sprites
{
    [[nodiscard]] int used_sprites_count();

    [[nodiscard]] int available_sprites_count();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();
}

#endif
