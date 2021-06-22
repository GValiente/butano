#include "bn_reciprocal_lut.h"

#include "bn_array.h"

namespace bn
{

namespace
{
    constexpr array<fixed_t<20>, reciprocal_lut_size> reciprocal_lut_impl = []{
        array<fixed_t<20>, reciprocal_lut_size> result;

        for(int index = 1; index < reciprocal_lut_size; ++index)
        {
            result[index] = calculate_reciprocal_lut_value(index);
        }

        return result;
    }();
}

const array<fixed_t<20>, reciprocal_lut_size>& reciprocal_lut = reciprocal_lut_impl;

}
