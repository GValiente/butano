/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_CHECKPOINT_H
#define FR_CHECKPOINT_H

#include "bn_fixed.h"
#include "bn_assert.h"

namespace fr
{

class checkpoint
{

public:
    constexpr checkpoint(int x, int y, bn::fixed angle) :
        _x(x),
        _y(y),
        _angle(angle.data() / 8)
    {
        BN_ASSERT(x >= 0 && x < 1024, "Invalid x: ", x);
        BN_ASSERT(y >= 0 && y < 1024, "Invalid y: ", y);
        BN_ASSERT(angle.data() / 8 >= 0 && angle.data() / 8 < 4096, "Invalid angle: ", angle);
    }

    [[nodiscard]] constexpr int x() const
    {
        return _x;
    }

    [[nodiscard]] constexpr int y() const
    {
        return _y;
    }

    [[nodiscard]] constexpr bn::fixed angle() const
    {
        return bn::fixed::from_data(_angle * 8);
    }

private:
    unsigned _x: 10;
    unsigned _y: 10;
    unsigned _angle: 12;
};

}

#endif
