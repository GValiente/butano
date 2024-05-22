/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef ANY_TESTS_H
#define ANY_TESTS_H

#include "bn_any.h"
#include "tests.h"

struct any_tests_test_struct
{
    int value = 3;

    any_tests_test_struct() = default;

    any_tests_test_struct(int a, int b, int c) :
        value(a + b + c)
    {
    }
};

class any_tests : public tests
{

public:
    any_tests() :
        tests("any")
    {
        bn::any<16> empty_any;
        BN_ASSERT(! empty_any.has_value());
        BN_ASSERT(empty_any.type() == bn::type_id_t());
        BN_ASSERT(bn::any_cast<int>(&empty_any) == nullptr);

        bn::any<sizeof(int)> int_any(1);
        BN_ASSERT(int_any.has_value());
        BN_ASSERT(int_any.type() == bn::type_id<int>());
        BN_ASSERT(int_any.value<int>() == 1);

        int* any_cast_ptr = bn::any_cast<int>(&int_any);
        BN_ASSERT(any_cast_ptr != nullptr);
        BN_ASSERT(*any_cast_ptr == 1);

        int_any = 2;
        BN_ASSERT(int_any.has_value());
        BN_ASSERT(int_any.type() == bn::type_id<int>());
        BN_ASSERT(int_any.value<int>() == 2);

        int_any.reset();
        BN_ASSERT(! int_any.has_value());
        BN_ASSERT(empty_any.type() == bn::type_id_t());

        int_any = 'a';
        BN_ASSERT(int_any.has_value());
        BN_ASSERT(int_any.type() == bn::type_id<char>());
        BN_ASSERT(int_any.value<char>() == 'a');

        int_any.reset();
        BN_ASSERT(! int_any.has_value());
        BN_ASSERT(empty_any.type() == bn::type_id_t());

        int_any = any_tests_test_struct();
        BN_ASSERT(int_any.has_value());
        BN_ASSERT(int_any.type() == bn::type_id<any_tests_test_struct>());
        BN_ASSERT(int_any.value<any_tests_test_struct>().value == 3);

        int_any.emplace<any_tests_test_struct>(1, 2, 3);
        BN_ASSERT(int_any.has_value());
        BN_ASSERT(int_any.type() == bn::type_id<any_tests_test_struct>());
        BN_ASSERT(int_any.value<any_tests_test_struct>().value == 6);

        bn::any<sizeof(any_tests_test_struct)> struct_any(any_tests_test_struct{});
        BN_ASSERT(struct_any.has_value());
        BN_ASSERT(int_any.type() == bn::type_id<any_tests_test_struct>());
        BN_ASSERT(struct_any.value<any_tests_test_struct>().value == 3);

        int_any.swap(struct_any);
        BN_ASSERT(int_any.value<any_tests_test_struct>().value == 3);
        BN_ASSERT(struct_any.value<any_tests_test_struct>().value == 6);
    }
};

#endif
