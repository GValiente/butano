#ifndef MATH_TESTS_H
#define MATH_TESTS_H

#include "btn_math.h"
#include "btn_fixed.h"
#include "btn_rule_of_three_approximation.h"
#include "tests.h"

class math_tests : public tests
{

public:
    math_tests() :
        tests("math")
    {
        BTN_ASSERT(btn::rule_of_three_approximation(360, 65536).calculate(0) == 0);
        BTN_ASSERT(btn::rule_of_three_approximation(360, 65536).calculate(90) == 16384);
        BTN_ASSERT(btn::rule_of_three_approximation(360, 65536).calculate(180) == 32768);
        BTN_ASSERT(btn::rule_of_three_approximation(360, 65536).calculate(270) == 49152);
        BTN_ASSERT(btn::rule_of_three_approximation(360, 65536).calculate(360) == 65536);

        BTN_ASSERT(btn::degrees_sin(0) == 0);
        BTN_ASSERT(btn::degrees_sin(90) == 1);
        BTN_ASSERT(btn::degrees_sin(180) == 0);
        BTN_ASSERT(btn::degrees_sin(270) == -1);
        BTN_ASSERT(btn::degrees_sin(360) == 0);
    }
};

#endif
