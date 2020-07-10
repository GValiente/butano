#ifndef FIXED_TESTS_H
#define FIXED_TESTS_H

#include "btn_math.h"
#include "btn_fixed.h"
#include "btn_string.h"
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

        BTN_ASSERT(btn::fixed_t<4>(btn::fixed(1)).data() == 16);

        BTN_ASSERT(btn::fixed_t<5>(btn::fixed(1)).data() == 32);

        BTN_ASSERT(btn::fixed_t<7>(btn::fixed(-1) + 1).data() == 0);
        BTN_ASSERT(btn::fixed_t<7>(btn::fixed(0) + 1).data() == 128);
        BTN_ASSERT(btn::fixed_t<7>(btn::fixed(1) + 1).data() == 256);

        BTN_ASSERT(btn::fixed_t<8>(btn::fixed(1)).data() == 256);

        BTN_ASSERT(btn::fixed_t<10>(btn::fixed(1)).data() == 1024);
        BTN_ASSERT(btn::fixed_t<10>(btn::fixed(64)).data() == 65536);

        BTN_ASSERT(btn::to_string<8>(btn::fixed(1)) == btn::string_view("1"),
                   "Fixed to string conversion failed: ", btn::to_string<8>(btn::fixed(1)));

        BTN_ASSERT(btn::to_string<8>(btn::fixed(0.5)) == btn::string_view("0.50000"),
                   "Fixed to string conversion failed: ", btn::to_string<8>(btn::fixed(0.5)));

        BTN_ASSERT(btn::to_string<8>(btn::fixed(0.25)) == btn::string_view("0.25000"),
                   "Fixed to string conversion failed: ", btn::to_string<8>(btn::fixed(0.25)));

        BTN_ASSERT(btn::to_string<8>(btn::fixed(0.125)) == btn::string_view("0.12500"),
                   "Fixed to string conversion failed: ", btn::to_string<8>(btn::fixed(0.125)));

        BTN_ASSERT(btn::to_string<8>(btn::fixed(0.0625)) == btn::string_view("0.06250"),
                   "Fixed to string conversion failed: ", btn::to_string<8>(btn::fixed(0.0625)));

        BTN_ASSERT(btn::to_string<8>(btn::fixed(0.03125)) == btn::string_view("0.03125"),
                   "Fixed to string conversion failed: ", btn::to_string<8>(btn::fixed(0.03125)));
    }
};

#endif
