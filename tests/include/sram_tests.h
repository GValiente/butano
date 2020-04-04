#ifndef SRAM_TESTS_H
#define SRAM_TESTS_H

#include "btn_sram.h"
#include "btn_array.h"
#include "btn_memory.h"
#include "tests.h"

class sram_tests : public tests
{

public:
    sram_tests() :
        tests("sram")
    {
        struct test_data
        {
            btn::array<char, 32> begin = {};
            btn::array<char, 31 * 1024> fill = {};
            btn::array<char, 32> end = {};
        };

        btn::unique_ptr<test_data> loaded = btn::make_unique<test_data>();
        btn::sram::read(*loaded);

        btn::unique_ptr<test_data> expected = btn::make_unique<test_data>();
        btn::istring_base begin_istring(expected->begin._data);
        btn::input_string_stream begin_stream(begin_istring);
        begin_stream.append("butano SRAM test");

        btn::istring_base end_istring(expected->end._data);
        btn::input_string_stream end_stream(end_istring);
        end_stream.append("32KB!");

        if(loaded->begin == expected->begin)
        {
            BTN_ASSERT(loaded->end == expected->end, "SRAM end array read failed");
        }
        else
        {
            btn::sram::write(*expected);
            _again = true;
        }
    }

    [[nodiscard]] bool again() const
    {
        return _again;
    }

private:
    bool _again = false;
};

#endif
