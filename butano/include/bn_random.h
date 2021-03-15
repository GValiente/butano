/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RANDOM_H
#define BN_RANDOM_H

/**
 * @file
 * bn::random header file.
 *
 * @ingroup other
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Fast, deterministic random number generator.
 *
 * Its current implementation is a Marsaglia's xorshf generator.
 *
 * See https://github.com/raylee/xorshf96/blob/master/xorshf96.c
 *
 * @ingroup other
 */
class random
{

public:
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
    unsigned _x = 123456789;
    unsigned _y = 362436069;
    unsigned _z = 521288629;
};

}

#endif
