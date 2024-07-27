/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_circle_generator.h"

#include "bn_cstring.h"
#include "bn_display.h"

namespace bf
{

void circle_generator::set_radius(bn::fixed radius)
{
    BN_ASSERT(radius >= 0, "Invalid radius: ", radius);

    _radius = radius;
}

void circle_generator::generate(bn::span<bn::pair<bn::fixed, bn::fixed>> values) const
{
    // https://gbadev.net/tonc/dma.html#sec-demo

    int values_count = values.size();

    if(! values_count)
    {
        return;
    }

    bn::fixed y0 = (bn::display::height() / 2) + _origin_y;
    bn::fixed x = 0;
    bn::fixed y = _radius;
    bn::fixed d = 1 - _radius;

    bn::pair<bn::fixed, bn::fixed>* values_data = values.data();
    bn::memclear(values_data, values_count * int(sizeof(bn::pair<bn::fixed, bn::fixed>)));

    while(y >= x)
    {
        int top = (y0 - x).right_shift_integer();
        int bottom = (y0 + x).right_shift_integer();

        if(top >= 0 && top < values_count)
        {
            values_data[top] = bn::make_pair(-y, y);
        }

        if(bottom >= 0 && bottom < values_count)
        {
            values_data[bottom] = bn::make_pair(-y, y);
        }

        if(d >= 0)
        {
            top = (y0 - y).right_shift_integer();
            bottom = (y0 + y).right_shift_integer();

            if(top >= 0 && top < values_count)
            {
                values_data[top] = bn::make_pair(-x, x);
            }

            if(bottom >= 0 && bottom < values_count)
            {
                values_data[bottom] = bn::make_pair(-x, x);
            }

            y -= 1;
            d -= 2 * y;
        }

        d += 2 * x + 3;
        x += 1;
    }
}

}
