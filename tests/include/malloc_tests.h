#ifndef MALLOC_TESTS_H
#define MALLOC_TESTS_H

#include "btn_memory.h"
#include "btn_cstdlib.h"
#include "tests.h"

class malloc_tests : public tests
{

public:
    malloc_tests() :
        tests("malloc")
    {
        BTN_ASSERT(btn::memory::used_alloc_ewram() == 0);

        void* ptr = btn::malloc(4);
        BTN_ASSERT(ptr);
        BTN_ASSERT(btn::memory::used_alloc_ewram() == 4);

        btn::free(ptr);
        BTN_ASSERT(btn::memory::used_alloc_ewram() == 0);

        ptr = btn::malloc(0);
        BTN_ASSERT(ptr);

        btn::free(ptr);
        BTN_ASSERT(btn::memory::used_alloc_ewram() == 0);

        auto integer = new int(123);
        BTN_ASSERT(integer);
        BTN_ASSERT(btn::memory::used_alloc_ewram() == 4);

        delete integer;
        BTN_ASSERT(btn::memory::used_alloc_ewram() == 0);
    }
};

#endif
