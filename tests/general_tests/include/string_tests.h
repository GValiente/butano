/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef STRING_TESTS_H
#define STRING_TESTS_H

#include "bn_string.h"
#include "tests.h"

class string_tests : public tests
{

public:
    string_tests() :
        tests("string")
    {
        bn::string<32> string = bn::to_string<32>(50123);
        BN_ASSERT(string == bn::string_view("50123"), string);

        string = bn::to_string<32>(-50123);
        BN_ASSERT(string == bn::string_view("-50123"), string);

        string = bn::to_string<32>(401234567);
        BN_ASSERT(string == bn::string_view("401234567"), string);

        string = bn::to_string<32>(-401234567);
        BN_ASSERT(string == bn::string_view("-401234567"), string);

        string = bn::to_string<32>(9012345678);
        BN_ASSERT(string == bn::string_view("9012345678"), string);

        string = bn::to_string<32>(-9012345678);
        BN_ASSERT(string == bn::string_view("-9012345678"), string);
    }
};

#endif
