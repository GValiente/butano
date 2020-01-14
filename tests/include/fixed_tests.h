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

        BTN_ASSERT(btn::fixed_t<4>(btn::fixed(1)).value() == 16);

        BTN_ASSERT(btn::fixed_t<5>(btn::fixed(1)).value() == 32);

        BTN_ASSERT(btn::fixed_t<7>(btn::fixed(-1) + 1).value() == 0);
        BTN_ASSERT(btn::fixed_t<7>(btn::fixed(0) + 1).value() == 128);
        BTN_ASSERT(btn::fixed_t<7>(btn::fixed(1) + 1).value() == 256);

        BTN_ASSERT(btn::fixed_t<8>(btn::fixed(1)).value() == 256);

        BTN_ASSERT(btn::fixed_t<10>(btn::fixed(1)).value() == 1024);
        BTN_ASSERT(btn::fixed_t<10>(btn::fixed(64)).value() == 65536);
    }
};

#endif
