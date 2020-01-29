#ifndef BTN_SPRITES_H
#define BTN_SPRITES_H

#include "btn_common.h"

namespace btn::sprites
{
    [[nodiscard]] int used_sprites_count();

    [[nodiscard]] int available_sprites_count();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();
}

#endif
