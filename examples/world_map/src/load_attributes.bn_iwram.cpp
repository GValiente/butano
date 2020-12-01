/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "load_attributes.h"

void load_attributes(bn::affine_bg_mat_attributes base_attributes, bn::affine_bg_mat_attributes* attributes)
{
    bn::fixed horizontal_scale = base_attributes.horizontal_scale();

    for(int index = 0; index < bn::display::height(); ++index)
    {
        base_attributes.set_scale(horizontal_scale);
        horizontal_scale += 0.002;
        attributes[index] = base_attributes;
    }
}
