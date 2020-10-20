/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_math.h"
#include "btn_random.h"
#include "btn_profiler.h"
#include "btn_algorithm.h"

#include "../../butano/hw/include/btn_hw_tonc.h"

int main()
{
    btn::core::init();

    btn::random random;
    int integer = 123456789;
    int its = 10000;

    int div_result = 0;
    BTN_PROFILER_START("div");

    for(int i = 0; i < its; ++i)
    {
        div_result += integer / (i + 1);
    }

    BTN_PROFILER_STOP();

    int bios_div_result = 0;
    BTN_PROFILER_START("bios_div");

    for(int i = 0; i < its; ++i)
    {
        bios_div_result += Div(integer, i + 1);
    }

    BTN_PROFILER_STOP();

    BTN_ASSERT(div_result == bios_div_result, "Invalid division");
    integer += div_result;
    integer += bios_div_result;

    int sqrt_result = 0;
    BTN_PROFILER_START("sqrt");

    for(int i = 0; i < its; ++i)
    {
        sqrt_result += btn::sqrt(btn::abs(sqrt_result));
    }

    BTN_PROFILER_STOP();

    int bios_sqrt_result = 0;
    BTN_PROFILER_START("bios_sqrt");

    for(int i = 0; i < its; ++i)
    {
        bios_sqrt_result += Sqrt(unsigned(btn::abs(bios_sqrt_result)));
    }

    BTN_PROFILER_STOP();

    BTN_ASSERT(sqrt_result == bios_sqrt_result, "Invalid sqrt");
    integer += sqrt_result;
    integer += bios_sqrt_result;

    BTN_PROFILER_START("random");

    for(int i = 0; i < its; ++i)
    {
        integer += random.get();
    }

    BTN_PROFILER_STOP();

    BTN_PROFILER_START("sin");

    for(int i = 0; i < its; ++i)
    {
        integer += btn::lut_sin(i % 512).data();
    }

    BTN_PROFILER_STOP();

    [[maybe_unused]] int dummy = btn::sqrt(btn::abs(integer));

    btn::profiler::show();
}
