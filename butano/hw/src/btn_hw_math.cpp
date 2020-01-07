#include "../include/btn_hw_math.h"

#include "tonc.h"

extern "C"
{
    unsigned isqrt32(unsigned x) BTN_CODE_IWRAM;
}

namespace btn::hw
{

int sqrt(int value)
{
    return int(isqrt32(unsigned(value)));
}

int lut_sin(int s16_angle)
{
    return lu_sin(unsigned(s16_angle));
}

int lut_cos(int s16_angle)
{
    return lu_cos(unsigned(s16_angle));
}

}
