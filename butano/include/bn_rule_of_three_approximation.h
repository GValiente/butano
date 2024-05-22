/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RULE_OF_THREE_APPROXIMATION_H
#define BN_RULE_OF_THREE_APPROXIMATION_H

/**
 * @file
 * bn::rule_of_three_approximation header file.
 *
 * @ingroup math
 */

#include "bn_fixed.h"
#include "bn_assert.h"

namespace bn
{

/**
 * @brief Approximates a rule of three cross-multiplication (x = (value * multiplier) / divisor).
 *
 * rule_of_three_approximation objects should be created at compile time to improve performance.
 *
 * See https://en.wikipedia.org/wiki/Cross-multiplication#Rule_of_Three
 *
 * @ingroup math
 */
class rule_of_three_approximation
{

public:
    /**
     * @brief Constructor.
     * @param divisor Valid divisor (> 0).
     * @param multiplier Valid multiplier (> 0).
     */
    constexpr rule_of_three_approximation(int divisor, int multiplier) :
        _divisor(_build_divisor(divisor)),
        _multiplier(_build_multiplier(multiplier, unsigned(divisor), _divisor))
    {
    }

    /**
     * @brief Returns an approximation of (value * multiplier) / divisor,
     * casting the value to uint64_t to try to avoid integer overflow.
     */
    [[nodiscard]] constexpr unsigned calculate(unsigned value) const
    {
        return unsigned((uint64_t(value) * _multiplier) / _divisor);
    }

    /**
     * @brief Returns an approximation of (value * multiplier) / divisor,
     * without trying to avoid integer overflow.
     */
    [[nodiscard]] constexpr unsigned unsafe_calculate(unsigned value) const
    {
        return (value * _multiplier) / _divisor;
    }

    /**
     * @brief Returns an approximation of (value * multiplier) / divisor,
     * casting the value to int64_t to try to avoid integer overflow.
     */
    [[nodiscard]] constexpr int calculate(int value) const
    {
        return int((int64_t(value) * _multiplier) / _divisor);
    }

    /**
     * @brief Returns an approximation of (value * multiplier) / divisor,
     * without trying to avoid integer overflow.
     */
    [[nodiscard]] constexpr int unsafe_calculate(int value) const
    {
        return (value * int(_multiplier)) / int(_divisor);
    }

    /**
     * @brief Returns an approximation of (value * multiplier) / divisor,
     * casting the value to int64_t to try to avoid integer overflow.
     */
    template<int Precision>
    [[nodiscard]] constexpr fixed_t<Precision> calculate(fixed_t<Precision> value) const
    {
        return fixed_t<Precision>::from_data(calculate(value.data()));
    }

    /**
     * @brief Returns an approximation of (value * multiplier) / divisor,
     * without trying to avoid integer overflow.
     */
    template<int Precision>
    [[nodiscard]] constexpr fixed_t<Precision> unsafe_calculate(fixed_t<Precision> value) const
    {
        return fixed_t<Precision>::from_data(unsafe_calculate(value.data()));
    }

private:
    unsigned _divisor;
    unsigned _multiplier;

    [[nodiscard]] static constexpr unsigned _build_divisor(int divisor)
    {
        BN_ASSERT(divisor > 0, "Invalid divisor: ", divisor);

        unsigned shift_bits = 0;

        while(true)
        {
            unsigned result = unsigned(1) << shift_bits;

            if(result >= unsigned(divisor))
            {
                return result;
            }

            ++shift_bits;
        }
    }

    [[nodiscard]] static constexpr unsigned _build_multiplier(
            int multiplier, unsigned divisor, unsigned internal_divisor)
    {
        BN_ASSERT(multiplier > 0, "Invalid multiplier: ", multiplier);

        return _build_multiplier_impl(unsigned(multiplier), divisor, internal_divisor);
    }

    [[nodiscard]] static constexpr unsigned _build_multiplier_impl(
            unsigned multiplier, unsigned divisor, unsigned internal_divisor)
    {
        if(internal_divisor == divisor)
        {
            return multiplier;
        }

        return unsigned((uint64_t(multiplier) * internal_divisor) / uint64_t(divisor)) + 1;
    }
};

}

#endif
