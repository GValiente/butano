#include "bn_sin_lut.h"

#include "bn_array.h"

namespace bn
{

namespace
{
    constexpr array<int16_t, sin_lut_size> sin_lut_impl = []{
        array<int16_t, sin_lut_size> result;
        int lut_angle = 0;

        for(int index = 0; index < sin_lut_size; ++index)
        {
            result[index] = int16_t(calculate_sin_lut_value(lut_angle));
            lut_angle += 65536 / (sin_lut_size - 1);
        }

        return result;
    }();
}

const array<int16_t, sin_lut_size>& sin_lut = sin_lut_impl;

}
