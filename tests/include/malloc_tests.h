#ifndef MALLOC_TESTS_H
#define MALLOC_TESTS_H

#include "btn_memory.h"
#include "tests.h"

class malloc_tests : public tests
{

public:
    malloc_tests() :
        tests("malloc")
    {
        int used_ewram = btn::memory::used_malloc_ewram();
        BTN_ASSERT(used_ewram == 0);

        void* ptr = btn::malloc(4);
        BTN_ASSERT(ptr);
        used_ewram = btn::memory::used_malloc_ewram();
        BTN_ASSERT(used_ewram >= 4);

        btn::free(ptr);

        int left_used_ewram = btn::memory::used_malloc_ewram();
        BTN_ASSERT(left_used_ewram >= 0);

        ptr = btn::malloc(0);
        BTN_ASSERT(ptr);
        used_ewram = btn::memory::used_malloc_ewram();
        BTN_ASSERT(used_ewram > left_used_ewram);

        btn::free(ptr);
        used_ewram = btn::memory::used_malloc_ewram();
        BTN_ASSERT(used_ewram == left_used_ewram);

        auto integer = new int(123);
        BTN_ASSERT(integer);
        used_ewram = btn::memory::used_malloc_ewram();
        BTN_ASSERT(used_ewram > left_used_ewram);

        delete integer;
        used_ewram = btn::memory::used_malloc_ewram();
        BTN_ASSERT(used_ewram == left_used_ewram);
    }
};

#endif
