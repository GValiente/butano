#ifndef BTN_SPRITE_AFFINE_MAT_SCALE_LUT_H
#define BTN_SPRITE_AFFINE_MAT_SCALE_LUT_H

#include "btn_array.h"
#include "btn_fixed.h"

namespace btn
{

[[nodiscard]] constexpr array<uint16_t, 1025> create_sprite_affine_mat_scale_lut()
{
    array<uint16_t, 1025> result = {};
    int one = fixed_t<8>(1).data() * fixed_t<8>::scale();

    for(int index = 1; index < 1025; ++index)
    {
        result[index] = uint16_t(one / index);
    }

    return result;
}

constexpr const array<uint16_t, 1025> sprite_affine_mat_scale_lut = create_sprite_affine_mat_scale_lut();

}

#endif
