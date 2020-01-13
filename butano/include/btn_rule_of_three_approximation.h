#ifndef BTN_RULE_OF_THREE_APPROXIMATION_H
#define BTN_RULE_OF_THREE_APPROXIMATION_H

#include "btn_assert.h"
#include "btn_fixed_fwd.h"

namespace btn
{

class rule_of_three_approximation
{

public:
    constexpr rule_of_three_approximation(unsigned divisor, unsigned multiplicator) :
        _divisor(_build_divisor(divisor)),
        _multiplicator(_build_multiplicator(multiplicator, divisor))
    {
    }

    [[nodiscard]] constexpr unsigned calculate(unsigned value) const
    {
        return unsigned((uint64_t(value) * _multiplicator) / _divisor);
    }

    [[nodiscard]] constexpr unsigned unsafe_calculate(unsigned value) const
    {
        return (value * _multiplicator) / _divisor;
    }

    [[nodiscard]] constexpr int calculate(int value) const
    {
        return int((int64_t(value) * _multiplicator) / _divisor);
    }

    [[nodiscard]] constexpr int unsafe_calculate(int value) const
    {
        return (value * int(_multiplicator)) / int(_divisor);
    }

    template<size_t Precision>
    [[nodiscard]] constexpr fixed_t<Precision> calculate(fixed_t<Precision> fixed) const
    {
        return fixed_t<Precision>::create(calculate(fixed.value()));
    }

    template<size_t Precision>
    [[nodiscard]] constexpr fixed_t<Precision> unsafe_calculate(fixed_t<Precision> fixed) const
    {
        return fixed_t<Precision>::create(unsafe_calculate(fixed.value()));
    }

private:
    unsigned _divisor;
    unsigned _multiplicator;

    [[nodiscard]] static constexpr unsigned _build_divisor(unsigned divisor)
    {
        BTN_CONSTEXPR_ASSERT(divisor, "Divisor is zero");

        unsigned shift_bits = 0;

        while(true)
        {
            unsigned result = 1u << shift_bits;

            if(result >= divisor)
            {
                return result;
            }

            ++shift_bits;
        }
    }

    [[nodiscard]] constexpr unsigned _build_multiplicator(unsigned multiplicator, unsigned divisor)
    {
        BTN_CONSTEXPR_ASSERT(divisor, "Divisor is zero");

        if(_divisor == divisor)
        {
            return multiplicator;
        }

        return ((multiplicator * _divisor) / divisor) + 1;
    }
};

}

#endif
