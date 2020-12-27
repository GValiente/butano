/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_random.h"
#include "bn_profiler.h"
#include "bn_algorithm.h"

#include "../../butano/hw/include/bn_hw_tonc.h"

int main()
{
    bn::core::init();

    bn::random random;
    int integer = 123456789;
    int its = 10000;

    int div_result = 0;
    BN_PROFILER_START("div");

    for(int i = 0; i < its; ++i)
    {
        div_result += integer / (i + 1);
    }

    BN_PROFILER_STOP();

    int bios_div_result = 0;
    BN_PROFILER_START("bios_div");

    for(int i = 0; i < its; ++i)
    {
        bios_div_result += Div(integer, i + 1);
    }

    BN_PROFILER_STOP();

    BN_ASSERT(div_result == bios_div_result, "Invalid division");
    integer += div_result;
    integer += bios_div_result;

    int sqrt_result = 0;
    BN_PROFILER_START("sqrt");

    for(int i = 0; i < its; ++i)
    {
        sqrt_result += bn::sqrt(bn::abs(sqrt_result));
    }

    BN_PROFILER_STOP();

    int bios_sqrt_result = 0;
    BN_PROFILER_START("bios_sqrt");

    for(int i = 0; i < its; ++i)
    {
        bios_sqrt_result += Sqrt(unsigned(bn::abs(bios_sqrt_result)));
    }

    BN_PROFILER_STOP();

    BN_ASSERT(sqrt_result == bios_sqrt_result, "Invalid sqrt");
    integer += sqrt_result;
    integer += bios_sqrt_result;

    BN_PROFILER_START("random");

    for(int i = 0; i < its; ++i)
    {
        integer += random.get();
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("lut_sin");

    for(int i = 0; i < its; ++i)
    {
        integer += bn::lut_sin(i % 2048).data();
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("calculate_sin_lut_value");

    for(int i = 0; i < its; ++i)
    {
        integer += bn::calculate_sin_lut_value(i % 65536);
    }

    BN_PROFILER_STOP();

    [[maybe_unused]] int dummy = bn::sqrt(bn::abs(integer));

    bn::profiler::show();
}
