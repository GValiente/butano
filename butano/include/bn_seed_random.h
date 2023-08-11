/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SEED_RANDOM_H
#define BN_SEED_RANDOM_H

/**
 * @file
 * bn::seed_random header file.
 *
 * @ingroup random
 */

#include "bn_fixed.h"
#include "bn_assert.h"
#include "bn_limits.h"

namespace bn
{

/**
 * @brief Deterministic random number generator with a public, configurable seed.
 *
 * See https://github.com/velipso/whisky
 *
 * @ingroup random
 */
class seed_random
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr seed_random() = default;

    /**
     * @brief Constructor.
     * @param seed Initial value of the seed used to generate random numbers.
     */
    constexpr explicit seed_random(unsigned seed) :
        _seed(seed)
    {
    }

    /**
     * @brief Returns the value of the seed used to generate random numbers.
     */
    [[nodiscard]] constexpr unsigned seed() const
    {
        return _seed;
    }

    /**
     * @brief Sets the value of the seed used to generate random numbers.
     */
    constexpr void set_seed(unsigned seed)
    {
       _seed = seed;
    }

    /**
     * @brief Returns a random unsigned integer greater or equal than 0,
     * updating its seed in the process.
     */
    [[nodiscard]] constexpr unsigned get()
    {
        update();

        unsigned seed = _seed;
        unsigned z0 = (seed * 1831267127) ^ seed;
        unsigned z1 = (z0 * 3915839201) ^ (z0 >> 20);
        unsigned z2 = (z1 * 1561867961) ^ (z1 >> 24);
        return z2;
    }

    /**
     * @brief Returns a random signed integer greater or equal than 0,
     * updating its seed in the process.
     */
    [[nodiscard]] constexpr int get_int()
    {
        unsigned result = get() % unsigned(numeric_limits<int>::max());
        return int(result);
    }

    /**
     * @brief Returns a random bn::fixed greater or equal than 0,
     * updating its seed in the process.
     */
    [[nodiscard]] constexpr fixed get_fixed()
    {
        return fixed::from_data(get_int());
    }

    /**
     * @brief Returns a random signed integer in the range [0..limit),
     * updating its seed in the process.
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
     * updating its seed in the process.
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
     * updating its seed in the process.
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
     * updating its seed in the process.
     * @param minimum Returned value is greater or equal than this value.
     * @param limit Returned value is lower than this value.
     * @return Random bn::fixed in the range [minimum..limit).
     */
    [[nodiscard]] constexpr fixed get_fixed(fixed minimum, fixed limit)
    {
        fixed range = limit - minimum;
        BN_ASSERT(range > 0, "Invalid range: ", minimum, " - ", limit);

        unsigned result = get() % unsigned(range.data());
        return minimum + fixed::from_data(int(result));
    }

    /**
     * @brief Updates the value of the seed.
     */
    constexpr void update()
    {
        ++_seed;
    }

private:
    unsigned _seed = 0;
};

}

#endif
