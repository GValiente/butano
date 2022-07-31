/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_fixed.h"
#include "bn_assert.h"
#include "bn_limits.h"

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
     * @brief Returns a random unsigned integer greater or equal than 0,
     * modifying its internal seed in the process.
     */
    [[nodiscard]] constexpr unsigned get()
    {
        update();
        return _z;
    }

    /**
     * @brief Returns a random signed integer greater or equal than 0,
     * modifying its internal seed in the process.
     */
    [[nodiscard]] constexpr int get_int()
    {
        unsigned result = get() % unsigned(numeric_limits<int>::max());
        return int(result);
    }

    /**
     * @brief Returns a random bn::fixed greater or equal than 0,
     * modifying its internal seed in the process.
     */
    [[nodiscard]] constexpr fixed get_fixed()
    {
        return fixed::from_data(get_int());
    }

    /**
     * @brief Returns a random signed integer in the range [0..limit),
     * modifying its internal seed in the process.
     * @param limit Returned value is lower than this value.
     * @return Random signed integer in the range [0..limit).
     */
    [[nodiscard]] constexpr int get_int(int limit)
    {
        BN_ASSERT(limit > 0, "Invalid limit: ", limit);

        unsigned result = get() % unsigned(limit);
        return int(result);
    }

    /**
     * @brief Returns a random bn::fixed in the range [0..limit),
     * modifying its internal seed in the process.
     * @param limit Returned value is lower than this value.
     * @return Random bn::fixed in the range [0..limit).
     */
    [[nodiscard]] constexpr fixed get_fixed(fixed limit)
    {
        int limit_data = limit.data();
        BN_ASSERT(limit_data > 0, "Invalid limit: ", limit);

        unsigned result = get() % unsigned(limit_data);
        return fixed::from_data(int(result));
    }

    /**
     * @brief Returns a random signed integer in the range [minimum..limit),
     * modifying its internal seed in the process.
     * @param minimum Returned value is greater or equal than this value.
     * @param limit Returned value is lower than this value.
     * @return Random signed integer in the range [minimum..limit).
     */
    [[nodiscard]] constexpr int get_int(int minimum, int limit)
    {
        int range = limit - minimum;
        BN_ASSERT(range > 0, "Invalid range: ", minimum, " - ", limit);

        unsigned result = get() % unsigned(range);
        return minimum + int(result);
    }

    /**
     * @brief Returns a random bn::fixed in the range [minimum..limit),
     * modifying its internal seed in the process.
     * @param minimum Returned value is greater or equal than this value.
     * @param limit Returned value is lower than this value.
     * @return Random bn::fixed in the range [minimum..limit).
     */
    [[nodiscard]] constexpr fixed get_fixed(fixed minimum, fixed limit)
    {
        int range_data = limit.data() - minimum.data();
        BN_ASSERT(range_data > 0, "Invalid range: ", minimum, " - ", limit);

        unsigned result = get() % unsigned(range_data);
        return minimum + fixed::from_data(int(result));
    }

    /**
     * @brief Updates the value of the internal seed.
     */
    constexpr void update()
    {
        _x ^= _x << 16;
        _x ^= _x >> 5;
        _x ^= _x << 1;

        unsigned t = _x;
        _x = _y;
        _y = _z;
        _z = t ^ _x ^ _y;
    }

private:
    unsigned _x = 123456789;
    unsigned _y = 362436069;
    unsigned _z = 521288629;
};

}

#endif
