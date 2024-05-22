/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_cstdlib.h"

#include "bn_memory_manager.h"

namespace bn
{

void* malloc(int bytes)
{
    return memory_manager::ewram_alloc(bytes);
}

void* calloc(int num, int bytes)
{
    return memory_manager::ewram_calloc(num, bytes);
}

void* realloc(void* ptr, int new_bytes)
{
    return memory_manager::ewram_realloc(ptr, new_bytes);
}

void free(void* ptr)
{
    memory_manager::ewram_free(ptr);
}

}
