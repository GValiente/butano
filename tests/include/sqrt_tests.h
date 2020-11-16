/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef SQRT_TESTS_H
#define SQRT_TESTS_H

#include <cmath>
#include "bn_math.h"
#include "bn_fixed.h"
#include "tests.h"

class sqrt_tests : public tests
{

public:
    sqrt_tests() :
        tests("sqrt")
    {
        for(int i = 0; i < 100; ++i)
        {
            int bn_sqrt = bn::sqrt(i);
            bn::fixed bn_fixed_sqrt = bn::sqrt(bn::fixed(i));
            auto std_sqrt = int(std::floor(std::sqrt(i)));
            BN_ASSERT(bn_sqrt == std_sqrt,
                       "Invalid bn::sqrt(int): ", i, " - ", bn_sqrt, " - ", std_sqrt);
            BN_ASSERT(bn_fixed_sqrt.integer() == std_sqrt,
                       "Invalid bn::sqrt(bn::fixed): ", i, " - ", bn_fixed_sqrt, " - ", std_sqrt);

            if(i == 0)
            {
                constexpr int nr_sqrt = bn::newton_raphson_sqrt(0);
                BN_ASSERT(nr_sqrt == std_sqrt, "Invalid bn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
            else if(i == 1)
            {
                constexpr int nr_sqrt = bn::newton_raphson_sqrt(1);
                BN_ASSERT(nr_sqrt == std_sqrt, "Invalid bn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
            else if(i == 4)
            {
                constexpr int nr_sqrt = bn::newton_raphson_sqrt(4);
                BN_ASSERT(nr_sqrt == std_sqrt, "Invalid bn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
            else if(i == 50)
            {
                constexpr int nr_sqrt = bn::newton_raphson_sqrt(50);
                BN_ASSERT(nr_sqrt == std_sqrt, "Invalid bn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
        }
    }
};

#endif
