#ifndef FIXED_TESTS_H
#define FIXED_TESTS_H

#include <cmath>
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

        _truncate_and_round_test(-2.5);
        _truncate_and_round_test(-2);
        _truncate_and_round_test(-1.5);
        _truncate_and_round_test(-1);
        _truncate_and_round_test(-0.5);
        _truncate_and_round_test(0);
        _truncate_and_round_test(0.5);
        _truncate_and_round_test(1);
        _truncate_and_round_test(1.5);
        _truncate_and_round_test(2);
        _truncate_and_round_test(2.5);
    }

private:
    static void _truncate_and_round_test(float value)
    {
        BTN_ASSERT(btn::fixed(value).integer() == int(value), "Fixed truncate failed: ",
                   btn::fixed(value), " - ", btn::fixed(value).integer(), " - ", int(value));

        BTN_ASSERT(btn::fixed(value).round_integer() == int(std::round(value)), "Fixed round failed: ",
                   btn::fixed(value), " - ", btn::fixed(value).round_integer(), " - ", int(std::round(value)));
    }
};

#endif
