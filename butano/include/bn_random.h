/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RANDOM_H
#define BN_RANDOM_H

/**
 * @file
 * bn::random header file.
 *
 * @ingroup random
 */

#include "bn_math.h"

namespace bn
{

/**
 * @brief Fast, deterministic random number generator.
 *
 * Its current implementation is a Marsaglia's xorshf generator.
 *
 * See https://github.com/raylee/xorshf96/blob/master/xorshf96.c
 *
 * @ingroup random
 */
class random
{

public:
    /**
     * @brief Returns a random unsigned integer greater or equal than 0, updating its internal seed in the process.
     */
    [[nodiscard]] constexpr unsigned get()
    {
        update();
        return _z;
    }

    /**
     * @brief Returns a random boolean, updating its internal seed in the process.
     */
    [[nodiscard]] constexpr bool get_bool()
    {
        return bool(_get_int_impl(2));
    }

    /**
     * @brief Returns a random signed integer greater or equal than 0, updating its internal seed in the process.
     */
    [[nodiscard]] constexpr int get_int()
    {
        return bn::abs(int(get()));
    }

    /**
     * @brief Returns a random bn::fixed greater or equal than 0, updating its internal seed in the process.
     */
    [[nodiscard]] constexpr fixed get_fixed()
    {
        return fixed::from_data(get_int());
    }

    /**
     * @brief Returns a random signed integer in the range [0..limit), updating its internal seed in the process.
     *
     * It's possible that the output will be biased towards smaller numbers due to
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
     *
     * random::get_unbiased_int can be used to avoid modulo bias.
     *
     * @param limit Returned value is lower than this value.
     * @return Random signed integer in the range [0..limit).
     */
    [[nodiscard]] constexpr int get_int(int limit)
    {
        BN_ASSERT(limit > 0, "Invalid limit: ", limit);

        return _get_int_impl(limit);
    }

    /**
     * @brief Returns a random bn::fixed in the range [0..limit), updating its internal seed in the process.
     *
     * It's possible that the output will be biased towards smaller numbers due to
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
     *
     * random::get_unbiased_fixed can be used to avoid modulo bias.
     *
     * @param limit Returned value is lower than this value.
     * @return Random bn::fixed in the range [0..limit).
     */
    [[nodiscard]] constexpr fixed get_fixed(fixed limit)
    {
        BN_ASSERT(limit > 0, "Invalid limit: ", limit);

        return _get_fixed_impl(limit);
    }

    /**
     * @brief Returns a random signed integer in the range [minimum..limit), updating its internal seed in the process.
     *
     * It's possible that the output will be biased towards smaller numbers due to
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
     *
     * random::get_unbiased_int can be used to avoid modulo bias.
     *
     * @param minimum Returned value is greater or equal than this value.
     * @param limit Returned value is lower than this value.
     * @return Random signed integer in the range [minimum..limit).
     */
    [[nodiscard]] constexpr int get_int(int minimum, int limit)
    {
        int range = limit - minimum;
        BN_ASSERT(range > 0, "Invalid range: ", minimum, " - ", limit);

        return minimum + _get_int_impl(range);
    }

    /**
     * @brief Returns a random bn::fixed in the range [minimum..limit), updating its internal seed in the process.
     *
     * It's possible that the output will be biased towards smaller numbers due to
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
     *
     * random::get_unbiased_fixed can be used to avoid modulo bias.
     *
     * @param minimum Returned value is greater or equal than this value.
     * @param limit Returned value is lower than this value.
     * @return Random bn::fixed in the range [minimum..limit).
     */
    [[nodiscard]] constexpr fixed get_fixed(fixed minimum, fixed limit)
    {
        fixed range = limit - minimum;
        BN_ASSERT(range > 0, "Invalid range: ", minimum, " - ", limit);

        return minimum + _get_fixed_impl(range);
    }

    /**
     * @brief Returns a random signed integer in the range [0..limit), updating its internal seed in the process.
     *
     * This implementation avoids
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
     * so it's less likely that the output will be biased towards smaller numbers.
     *
     * @param limit Returned value is lower than this value.
     * @return Random signed integer in the range [0..limit).
     */
    [[nodiscard]] constexpr int get_unbiased_int(int limit)
    {
        BN_ASSERT(limit > 0, "Invalid limit: ", limit);

        return _get_unbiased_int_impl(limit);
    }

    /**
     * @brief Returns a random bn::fixed in the range [0..limit), updating its internal seed in the process.
     *
     * This implementation avoids
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
     * so it's less likely that the output will be biased towards smaller numbers.
     *
     * @param limit Returned value is lower than this value.
     * @return Random bn::fixed in the range [0..limit).
     */
    [[nodiscard]] constexpr fixed get_unbiased_fixed(fixed limit)
    {
        BN_ASSERT(limit > 0, "Invalid limit: ", limit);

        return _get_unbiased_fixed_impl(limit);
    }

    /**
     * @brief Returns a random signed integer in the range [minimum..limit), updating its internal seed in the process.
     *
     * This implementation avoids
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
     * so it's less likely that the output will be biased towards smaller numbers.
     *
     * @param minimum Returned value is greater or equal than this value.
     * @param limit Returned value is lower than this value.
     * @return Random signed integer in the range [minimum..limit).
     */
    [[nodiscard]] constexpr int get_unbiased_int(int minimum, int limit)
    {
        int range = limit - minimum;
        BN_ASSERT(range > 0, "Invalid range: ", minimum, " - ", limit);

        return minimum + _get_unbiased_int_impl(range);
    }

    /**
     * @brief Returns a random bn::fixed in the range [minimum..limit), updating its internal seed in the process.
     *
     * This implementation avoids
     * <a href="https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
     * so it's less likely that the output will be biased towards smaller numbers.
     *
     * @param minimum Returned value is greater or equal than this value.
     * @param limit Returned value is lower than this value.
     * @return Random bn::fixed in the range [minimum..limit).
     */
    [[nodiscard]] constexpr fixed get_unbiased_fixed(fixed minimum, fixed limit)
    {
        fixed range = limit - minimum;
        BN_ASSERT(range > 0, "Invalid range: ", minimum, " - ", limit);

        return minimum + _get_unbiased_fixed_impl(range);
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

protected:
    /// @cond DO_NOT_DOCUMENT

    unsigned _x = 123456789;
    unsigned _y = 362436069;
    unsigned _z = 521288629;

    /// @endcond

private:
    [[nodiscard]] constexpr int _get_int_impl(int limit)
    {
        return int(get() % unsigned(limit));
    }

    [[nodiscard]] constexpr fixed _get_fixed_impl(fixed limit)
    {
        return fixed::from_data(_get_int_impl(limit.data()));
    }

    [[nodiscard]] constexpr int _get_unbiased_int_impl(int limit)
    {
        auto unsigned_limit = unsigned(limit);
        unsigned next_power_of_two = unsigned_limit;
        --next_power_of_two;
        next_power_of_two |= next_power_of_two >> 1;
        next_power_of_two |= next_power_of_two >> 2;
        next_power_of_two |= next_power_of_two >> 4;
        next_power_of_two |= next_power_of_two >> 8;
        next_power_of_two |= next_power_of_two >> 16;
        ++next_power_of_two;

        unsigned and_bits = next_power_of_two - 1;
        unsigned result;

        do
        {
            result = get() & and_bits;
        }
        while(result >= unsigned_limit);

        return int(result);
    }

    [[nodiscard]] constexpr fixed _get_unbiased_fixed_impl(fixed limit)
    {
        return fixed::from_data(_get_unbiased_int_impl(limit.data()));
    }
};

}

#endif
