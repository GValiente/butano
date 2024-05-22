/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_CIRCLE_GENERATOR_H
#define BF_CIRCLE_GENERATOR_H

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_memory.h"

namespace bf
{

class circle_generator
{

public:
    [[nodiscard]] bn::fixed origin_y() const
    {
        return _origin_y;
    }

    void set_origin_y(bn::fixed origin_y)
    {
        _origin_y = origin_y;
    }

    [[nodiscard]] bn::fixed radius() const
    {
        return _radius;
    }

    void set_radius(bn::fixed radius);

    void generate(bn::span<bn::pair<bn::fixed, bn::fixed>> values) const;

private:
    bn::fixed _origin_y;
    bn::fixed _radius = 1;
};

}

#endif
