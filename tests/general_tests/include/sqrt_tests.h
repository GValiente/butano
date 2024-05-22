/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef SQRT_TESTS_H
#define SQRT_TESTS_H

#include <cmath>
#include "bn_math.h"
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
        }

        for(bn::fixed i = bn::fixed(0.25); i < 25; i += bn::fixed(0.25))
        {
            bn::fixed bn_sqrt = bn::sqrt(i);
            bn::fixed std_sqrt = std::sqrt(i.to_float());
            BN_ASSERT(bn::abs(bn_sqrt - std_sqrt) < bn::fixed(0.0175),
                      "Invalid bn::sqrt(bn::fixed): ", i, " - ", bn_sqrt, " - ", std_sqrt);

            bn::fixed bn_unsafe_sqrt = bn::unsafe_sqrt(i);
            BN_ASSERT(bn::abs(bn_unsafe_sqrt - std_sqrt) < bn::fixed(0.0005),
                      "Invalid bn::unsafe_sqrt(bn::fixed): ", i, " - ", bn_unsafe_sqrt, " - ", std_sqrt);
        }

        constexpr bn::fixed a = 0.25;
        constexpr bn::fixed a_constexpr = bn::sqrt(a);
        bn::fixed a_runtime = bn::sqrt(a);
        BN_ASSERT(bn::abs(a_constexpr - a_runtime) < bn::fixed(0.01),
                  "Invalid constexpr bn::sqrt(): ", a, " - ", a_constexpr, " - ", a_runtime);

        constexpr bn::fixed b = 0.5;
        constexpr bn::fixed b_constexpr = bn::sqrt(b);
        bn::fixed b_runtime = bn::sqrt(b);
        BN_ASSERT(bn::abs(b_constexpr - b_runtime) < bn::fixed(0.01),
                  "Invalid constexpr bn::sqrt(): ", b, " - ", b_constexpr, " - ", b_runtime);

        constexpr bn::fixed c = 0.75;
        constexpr bn::fixed c_constexpr = bn::sqrt(c);
        bn::fixed c_runtime = bn::sqrt(c);
        BN_ASSERT(bn::abs(c_constexpr - c_runtime) < bn::fixed(0.01),
                  "Invalid constexpr bn::sqrt(): ", c, " - ", c_constexpr, " - ", c_runtime);

        constexpr bn::fixed d = 1;
        constexpr bn::fixed d_constexpr = bn::sqrt(c);
        bn::fixed d_runtime = bn::sqrt(d);
        BN_ASSERT(c_constexpr == c_runtime,
                  "Invalid constexpr bn::sqrt(): ", d, " - ", d_constexpr, " - ", d_runtime);
    }
};

#endif
