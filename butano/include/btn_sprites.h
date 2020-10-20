/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITES_H
#define BTN_SPRITES_H

#include "../hw/include/btn_hw_sprites_constants.h"

namespace btn::sprites
{
    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();
}

#endif
