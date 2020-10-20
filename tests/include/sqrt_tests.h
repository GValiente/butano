/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef SQRT_TESTS_H
#define SQRT_TESTS_H

#include <cmath>
#include "btn_math.h"
#include "btn_fixed.h"
#include "tests.h"

class sqrt_tests : public tests
{

public:
    sqrt_tests() :
        tests("sqrt")
    {
        for(int i = 0; i < 100; ++i)
        {
            int btn_sqrt = btn::sqrt(i);
            btn::fixed btn_fixed_sqrt = btn::sqrt(btn::fixed(i));
            auto std_sqrt = int(std::floor(std::sqrt(i)));
            BTN_ASSERT(btn_sqrt == std_sqrt,
                       "Invalid btn::sqrt(int): ", i, " - ", btn_sqrt, " - ", std_sqrt);
            BTN_ASSERT(btn_fixed_sqrt.integer() == std_sqrt,
                       "Invalid btn::sqrt(btn::fixed): ", i, " - ", btn_fixed_sqrt, " - ", std_sqrt);

            if(i == 0)
            {
                constexpr int nr_sqrt = btn::newton_raphson_sqrt(0);
                BTN_ASSERT(nr_sqrt == std_sqrt, "Invalid btn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
            else if(i == 1)
            {
                constexpr int nr_sqrt = btn::newton_raphson_sqrt(1);
                BTN_ASSERT(nr_sqrt == std_sqrt, "Invalid btn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
            else if(i == 4)
            {
                constexpr int nr_sqrt = btn::newton_raphson_sqrt(4);
                BTN_ASSERT(nr_sqrt == std_sqrt, "Invalid btn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
            else if(i == 50)
            {
                constexpr int nr_sqrt = btn::newton_raphson_sqrt(50);
                BTN_ASSERT(nr_sqrt == std_sqrt, "Invalid btn::newton_raphson_sqrt: ", i, " - ", nr_sqrt, " - ", std_sqrt);
            }
        }
    }
};

#endif
