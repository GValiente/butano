#ifndef FIXED_TESTS_H
#define FIXED_TESTS_H

#include "btn_math.h"
#include "btn_fixed.h"
#include "tests.h"

class fixed_tests : public tests
{

public:
    fixed_tests() :
        tests("fixed")
    {
        BTN_ASSERT(btn::fixed(4) + 4 == 8);
        BTN_ASSERT(4 + btn::fixed(4) == 8);

        BTN_ASSERT(btn::abs(btn::fixed(-4)) == 4);
    }
};

#endif
