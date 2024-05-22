/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_cstdlib.h"

#include <cstdlib>
#include "bn_assert.h"

// fake_heap_end should not be needed:
// https://tbelaire.ca/posts/gba-rust-1.html
__attribute__((weak)) void* fake_heap_end = 0;

void* malloc(std::size_t bytes)
{
    return bn::malloc(int(bytes));
}

void* calloc(std::size_t num, std::size_t bytes)
{
    return bn::calloc(int(num), int(bytes));
}

void* realloc(void* ptr, std::size_t new_bytes)
{
    return bn::realloc(ptr, int(new_bytes));
}

void free(void* ptr)
{
    bn::free(ptr);
}

void abort()
{
    BN_ERROR("Abort call");

    while(true)
    {
    }
}
