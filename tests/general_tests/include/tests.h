/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef TESTS_H
#define TESTS_H

#include "bn_log.h"
#include "bn_string_view.h"

class tests
{

public:
    explicit tests(const bn::string_view& tag) :
        _tag(tag)
    {
        BN_LOG("Running ", tag, " tests...");
    }

    ~tests()
    {
        BN_LOG(_tag, " tests passed");
    }

private:
    bn::string_view _tag;
};

#endif
