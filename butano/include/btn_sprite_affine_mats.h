/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_AFFINE_MATS_H
#define BTN_SPRITE_AFFINE_MATS_H

#include "../hw/include/btn_hw_sprite_affine_mats_constants.h"

namespace btn::sprite_affine_mats
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();
}

#endif
