/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_random.h"
#include "bn_profiler.h"
#include "bn_seed_random.h"

int main()
{
    bn::core::init();

    bn::random random;
    bn::seed_random seed_random;
    int integer = 0;
    int its_sqrt = 100;
    int its = its_sqrt * its_sqrt;

    BN_PROFILER_START("random");

    for(int i = 0; i < its; ++i)
    {
        integer += random.get();
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("seed_random");

    for(int i = 0; i < its; ++i)
    {
        integer += seed_random.get();
    }

    BN_PROFILER_STOP();

    if(integer)
    {
        bn::profiler::show();
    }
    else
    {
        while(true)
        {
        }
    }
}
