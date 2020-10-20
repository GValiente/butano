/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_RULE_OF_THREE_APPROXIMATION_H
#define BTN_RULE_OF_THREE_APPROXIMATION_H

#include "btn_assert.h"
#include "btn_fixed_fwd.h"

namespace btn
{

class rule_of_three_approximation
{

public:
    constexpr rule_of_three_approximation(int divisor, int multiplier) :
        _divisor(_build_divisor(divisor)),
        _multiplier(_build_multiplier(multiplier, divisor, _divisor))
    {
    }

    [[nodiscard]] constexpr unsigned calculate(unsigned value) const
    {
        return unsigned((uint64_t(value) * _multiplier) / _divisor);
    }

    [[nodiscard]] constexpr unsigned unsafe_calculate(unsigned value) const
    {
        return (value * _multiplier) / _divisor;
    }

    [[nodiscard]] constexpr int calculate(int value) const
    {
        return int((int64_t(value) * _multiplier) / _divisor);
    }

    [[nodiscard]] constexpr int unsafe_calculate(int value) const
    {
        return (value * int(_multiplier)) / int(_divisor);
    }

    template<int Precision>
    [[nodiscard]] constexpr fixed_t<Precision> calculate(fixed_t<Precision> value) const
    {
        return fixed_t<Precision>::from_data(calculate(value.data()));
    }

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
        BTN_ASSERT(divisor > 0, "Invalid divisor: ", divisor);

        unsigned shift_bits = 0;

        while(true)
        {
            unsigned result = 1u << shift_bits;

            if(result >= unsigned(divisor))
            {
                return result;
            }

            ++shift_bits;
        }
    }

    [[nodiscard]] static constexpr unsigned _build_multiplier(int multiplier, int divisor, unsigned internal_divisor)
    {
        BTN_ASSERT(multiplier > 0, "Invalid multiplier: ", multiplier);

        if(internal_divisor == unsigned(divisor))
        {
            return multiplier;
        }

        return ((unsigned(multiplier) * internal_divisor) / divisor) + 1;
    }
};

}

#endif
