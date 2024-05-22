/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "load_attributes.h"

void load_attributes(bn::affine_bg_mat_attributes base_attributes, bn::affine_bg_mat_attributes* attributes)
{
    constexpr bn::fixed scale_inc = 0.003;

    bn::fixed scale = base_attributes.horizontal_scale() + (scale_inc * attributes_window_height);

    for(int index = attributes_window_height; index < bn::display::height() - attributes_window_height; ++index)
    {
        base_attributes.set_scale(scale);
        scale += scale_inc;
        attributes[index] = base_attributes;
    }
}
