/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef MATH_TESTS_H
#define MATH_TESTS_H

#include "bn_math.h"
#include "bn_fixed.h"
#include "bn_rule_of_three_approximation.h"
#include "tests.h"

class math_tests : public tests
{

public:
    math_tests() :
        tests("math")
    {
        BN_ASSERT(bn::rule_of_three_approximation(360, 65536).calculate(0) == 0);
        BN_ASSERT(bn::rule_of_three_approximation(360, 65536).calculate(90) == 16384);
        BN_ASSERT(bn::rule_of_three_approximation(360, 65536).calculate(180) == 32768);
        BN_ASSERT(bn::rule_of_three_approximation(360, 65536).calculate(270) == 49152);
        BN_ASSERT(bn::rule_of_three_approximation(360, 65536).calculate(360) == 65536);

        BN_ASSERT(bn::degrees_sin(0) == 0);
        BN_ASSERT(bn::degrees_sin(90) == 1);
        BN_ASSERT(bn::degrees_sin(180) == 0);
        BN_ASSERT(bn::degrees_sin(270) == -1);
        BN_ASSERT(bn::degrees_sin(360) == 0);
    }
};

#endif
