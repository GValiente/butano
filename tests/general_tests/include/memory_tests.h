/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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

        void* ptr = bn::malloc(0);
        BN_ASSERT(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 16);

        bn::free(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);

        ptr = bn::malloc(4);
        BN_ASSERT(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 16);

        bn::free(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);

        ptr = bn::malloc(8);
        BN_ASSERT(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 16);

        bn::free(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);

        ptr = bn::malloc(12);
        BN_ASSERT(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 20);

        bn::free(ptr);
        BN_ASSERT(bn::memory::used_alloc_ewram() == 0);

        uint32_t u32_array[3];
        BN_ASSERT(bn::aligned<4>(u32_array));
        BN_ASSERT(bn::aligned<4>(static_cast<const void*>(u32_array)));
        BN_ASSERT(bn::aligned<4>(u32_array + 1));
        BN_ASSERT(bn::aligned<4>(static_cast<const void*>(u32_array + 1)));
        BN_ASSERT(bn::aligned<4>(u32_array + 2));
        BN_ASSERT(bn::aligned<4>(static_cast<const void*>(u32_array + 2)));

        alignas(4) uint16_t u16_array[3];
        BN_ASSERT(bn::aligned<4>(u16_array));
        BN_ASSERT(bn::aligned<4>(static_cast<const void*>(u16_array)));
        BN_ASSERT(! bn::aligned<4>(u16_array + 1));
        BN_ASSERT(! bn::aligned<4>(static_cast<const void*>(u16_array + 1)));
        BN_ASSERT(bn::aligned<4>(u16_array + 2));
        BN_ASSERT(bn::aligned<4>(static_cast<const void*>(u16_array + 2)));
    }
};

#endif
