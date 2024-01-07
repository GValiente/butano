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

    alignas(int) constexpr array<uint16_t, reciprocal_16_lut_size> reciprocal_16_lut_impl = []{
        array<uint16_t, reciprocal_16_lut_size> result = {};

        for(int index = 1; index < reciprocal_16_lut_size; ++index)
        {
            result[index] = uint16_t(calculate_reciprocal_lut_value<16>(index).data());
        }

        return result;
    }();
}

const array<fixed_t<20>, reciprocal_lut_size>& reciprocal_lut = reciprocal_lut_impl;

alignas(int) const array<uint16_t, reciprocal_16_lut_size>& reciprocal_16_lut = reciprocal_16_lut_impl;

}
