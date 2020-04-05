#ifndef BTN_SPRITE_AFFINE_MATS_H
#define BTN_SPRITE_AFFINE_MATS_H

#include "../hw/include/btn_hw_sprite_affine_mats_constants.h"

namespace btn::sprite_affine_mats
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();
}

#endif
