/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef SRAM_TESTS_H
#define SRAM_TESTS_H

#include "bn_sram.h"
#include "bn_array.h"
#include "bn_memory.h"
#include "tests.h"

class sram_tests : public tests
{

public:
    sram_tests() :
        tests("sram")
    {
        struct test_data
        {
            bn::array<char, 32> begin = {};
            bn::array<char, 31 * 1024> fill = {};
            bn::array<char, 32> end = {};
        };

        bn::unique_ptr<test_data> loaded = bn::make_unique<test_data>();
        bn::sram::read(*loaded);

        bn::unique_ptr<test_data> expected = bn::make_unique<test_data>();
        bn::istring_base begin_istring(expected->begin._data);
        bn::ostringstream begin_stream(begin_istring);
        begin_stream.append("SRAM test");

        bn::istring_base end_istring(expected->end._data);
        bn::ostringstream end_stream(end_istring);
        end_stream.append("32KB!");

        if(loaded->begin == expected->begin)
        {
            BN_ASSERT(loaded->end == expected->end, "SRAM end array read failed");
        }
        else
        {
            bn::sram::write(*expected);
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
