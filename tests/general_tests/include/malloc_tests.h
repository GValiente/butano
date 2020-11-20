/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef MALLOC_TESTS_H
#define MALLOC_TESTS_H

#include "bn_memory.h"
#include "bn_cstdlib.h"
#include "tests.h"

class malloc_tests : public tests
{

public:
    malloc_tests() :
        tests("malloc")
    {
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);

        void* ptr = bn::malloc(4);
        BN_ASSERT(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 8);

        bn::free(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);

        ptr = bn::malloc(0);
        BN_ASSERT(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 4);

        bn::free(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);

        auto integer = new int(123);
        BN_ASSERT(integer);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 8);

        delete integer;
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);
    }
};

#endif
