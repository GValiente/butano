/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef ANY_TESTS_H
#define ANY_TESTS_H

#include "btn_any.h"
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
        btn::any<16> empty_any;
        BTN_ASSERT(! empty_any.has_value());
        BTN_ASSERT(empty_any.type() == btn::type_id_t());
        BTN_ASSERT(btn::any_cast<int>(&empty_any) == nullptr);

        btn::any<sizeof(int)> int_any(1);
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type() == btn::type_id<int>());
        BTN_ASSERT(int_any.value<int>() == 1);

        int* any_cast_ptr = btn::any_cast<int>(&int_any);
        BTN_ASSERT(any_cast_ptr != nullptr);
        BTN_ASSERT(*any_cast_ptr == 1);

        int_any = 2;
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type() == btn::type_id<int>());
        BTN_ASSERT(int_any.value<int>() == 2);

        int_any.reset();
        BTN_ASSERT(! int_any.has_value());
        BTN_ASSERT(empty_any.type() == btn::type_id_t());

        int_any = 'a';
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type() == btn::type_id<char>());
        BTN_ASSERT(int_any.value<char>() == 'a');

        int_any.reset();
        BTN_ASSERT(! int_any.has_value());
        BTN_ASSERT(empty_any.type() == btn::type_id_t());

        int_any = any_tests_test_struct();
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type() == btn::type_id<any_tests_test_struct>());
        BTN_ASSERT(int_any.value<any_tests_test_struct>().value == 3);

        int_any.emplace<any_tests_test_struct>(1, 2, 3);
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type() == btn::type_id<any_tests_test_struct>());
        BTN_ASSERT(int_any.value<any_tests_test_struct>().value == 6);

        btn::any<sizeof(any_tests_test_struct)> struct_any(any_tests_test_struct{});
        BTN_ASSERT(struct_any.has_value());
        BTN_ASSERT(int_any.type() == btn::type_id<any_tests_test_struct>());
        BTN_ASSERT(struct_any.value<any_tests_test_struct>().value == 3);

        int_any.swap(struct_any);
        BTN_ASSERT(int_any.value<any_tests_test_struct>().value == 3);
        BTN_ASSERT(struct_any.value<any_tests_test_struct>().value == 6);
    }
};

#endif
