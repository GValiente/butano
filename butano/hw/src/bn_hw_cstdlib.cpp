/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_cstdlib.h"

#include <cstdlib>

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
