/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FORMAT_TESTS_H
#define FORMAT_TESTS_H

#include "bn_format.h"
#include "tests.h"

class format_tests : public tests
{

public:
    format_tests() :
        tests("format")
    {
        BN_ASSERT(bn::format<32>("Hello world!") == bn::string<32>("Hello world!"));
        BN_ASSERT(bn::format<32>("Hello {}!", "world") == bn::string<32>("Hello world!"));
        BN_ASSERT(bn::format<32>("Hello!", "world") == bn::string<32>("Hello!"));
        BN_ASSERT(bn::format<32>("Hello {{!", "world") == bn::string<32>("Hello {!"));
        BN_ASSERT(bn::format<32>("Hello }}!", "world") == bn::string<32>("Hello }!"));
        BN_ASSERT(bn::format<32>("We have {} {}", 4, "apples") == bn::string<32>("We have 4 apples"));
    }
};

#endif
