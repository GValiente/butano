/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef TESTS_H
#define TESTS_H

#include "btn_log.h"
#include "btn_string_view.h"

class tests
{

public:
    explicit tests(const btn::string_view& tag) :
        _tag(tag)
    {
        BTN_LOG("Running ", tag, " tests...");
    }

    ~tests()
    {
        BTN_LOG(_tag, " tests passed");
    }

private:
    btn::string_view _tag;
};

#endif
