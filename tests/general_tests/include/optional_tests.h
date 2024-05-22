/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef OPTIONAL_TESTS_H
#define OPTIONAL_TESTS_H

#include "bn_optional.h"
#include "tests.h"

class optional_tests : public tests
{

private:
    class test_type
    {

    public:
        constexpr explicit test_type(int value) :
            _value(value)
        {
        }

        [[nodiscard]] constexpr int value() const
        {
            return _value;
        }

    private:
        const int _value;
    };

    [[nodiscard]] static constexpr bn::optional<test_type> _constexpr_optional(int value)
    {
        bn::optional<test_type> result;
        result.emplace(value);
        BN_ASSERT(result->value() == value);

        return result;
    }

public:
    optional_tests() :
        tests("optional")
    {
        bn::optional<test_type> runtime_optional;
        runtime_optional.emplace(42);
        BN_ASSERT(runtime_optional->value() == 42);

        constexpr bn::optional<test_type> constexpr_optional = _constexpr_optional(24);
        BN_ASSERT(constexpr_optional->value() == 24);
    }
};

#endif
