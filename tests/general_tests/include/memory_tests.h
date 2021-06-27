/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef MEMORY_TESTS_H
#define MEMORY_TESTS_H

#include "bn_memory.h"
#include "bn_cstdlib.h"
#include "tests.h"

class memory_tests : public tests
{

public:
    explicit memory_tests(int initial_used_stack_iwram) :
        tests("memory")
    {
        BN_ASSERT(initial_used_stack_iwram >= 0);

        int current_used_stack_iwram = bn::memory::used_stack_iwram();
        BN_ASSERT(current_used_stack_iwram > initial_used_stack_iwram);

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
    }
};

#endif
