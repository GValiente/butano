/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_RANDOM_H
#define BTN_RANDOM_H

#include "btn_common.h"

namespace btn
{

/**
 * @brief Fast, deterministic random number generator.
 *
 * Its current implementation is a Marsaglia's xorshf generator.
 *
 * See https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
 *
 * @ingroup other
 */
class random
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr random() :
        _x(123456789),
        _y(362436069),
        _z(521288629)
    {
    }

    /**
     * @brief Returns a new random unsigned integer, modifying its internal seed in the process.
     */
    [[nodiscard]] constexpr unsigned get()
    {
        _x ^= _x << 16;
        _x ^= _x >> 5;
        _x ^= _x << 1;

        unsigned t = _x;
        _x = _y;
        _y = _z;
        _z = t ^ _x ^ _y;
        return _z;
    }

private:
    unsigned _x;
    unsigned _y;
    unsigned _z;
};

}

#endif
