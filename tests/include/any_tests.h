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
        BTN_ASSERT(btn::type_name<int>() == "int");
        BTN_ASSERT(btn::type_name<int&>() == "int");
        BTN_ASSERT(btn::type_name<any_tests_test_struct>() == "any_tests_test_struct");

        btn::any<16> empty_any;
        BTN_ASSERT(! empty_any.has_value());
        BTN_ASSERT(empty_any.type_name().empty());

        btn::any<sizeof(int)> int_any(1);
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type_name() == "int");
        BTN_ASSERT(int_any.value<int>() == 1);
        BTN_ASSERT(btn::any_cast<int>(int_any) == 1);
        BTN_ASSERT(int_any == 1);
        BTN_ASSERT(int_any != 2);
        BTN_ASSERT(int_any < 2);

        int_any = 2;
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type_name() == "int");
        BTN_ASSERT(int_any.value<int>() == 2);

        int_any.reset();
        BTN_ASSERT(! int_any.has_value());
        BTN_ASSERT(int_any.type_name().empty());

        int_any = 'a';
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type_name() == "char");
        BTN_ASSERT(int_any.value<char>() == 'a');

        int_any.reset();
        BTN_ASSERT(! int_any.has_value());
        BTN_ASSERT(int_any.type_name().empty());

        int_any = any_tests_test_struct();
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type_name() == "any_tests_test_struct");
        BTN_ASSERT(int_any.value<any_tests_test_struct>().value == 3);

        int_any.emplace<any_tests_test_struct>(1, 2, 3);
        BTN_ASSERT(int_any.has_value());
        BTN_ASSERT(int_any.type_name() == "any_tests_test_struct");
        BTN_ASSERT(int_any.value<any_tests_test_struct>().value == 6);

        btn::any<sizeof(any_tests_test_struct)> struct_any(any_tests_test_struct{});
        BTN_ASSERT(struct_any.has_value());
        BTN_ASSERT(struct_any.type_name() == "any_tests_test_struct");
        BTN_ASSERT(struct_any.value<any_tests_test_struct>().value == 3);

        int_any.swap(struct_any);
        BTN_ASSERT(int_any.value<any_tests_test_struct>().value == 3);
        BTN_ASSERT(struct_any.value<any_tests_test_struct>().value == 6);
    }
};

#endif
