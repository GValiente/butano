/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_memory_manager.h"

#include "bn_best_fit_allocator.h"
#include "../hw/include/bn_hw_memory.h"

#include "bn_memory.cpp.h"
#include "bn_cstdlib.cpp.h"
#include "bn_cstring.cpp.h"

namespace bn::memory_manager
{

namespace
{
    class static_data
    {

    public:
        best_fit_allocator allocator;
    };

    BN_DATA_EWRAM_BSS static_data data;
}

void init()
{
    new(&data) static_data();

    char* start = hw::memory::ewram_heap_start();
    char* end = hw::memory::ewram_heap_end();
    data.allocator.reset(static_cast<void*>(start), end - start);
}

void* ewram_alloc(int bytes)
{
    return data.allocator.alloc(bytes);
}

void* ewram_calloc(int num, int bytes)
{
    return data.allocator.calloc(num, bytes);
}

void* ewram_realloc(void* ptr, int new_bytes)
{
    return data.allocator.realloc(ptr, new_bytes);
}

void ewram_free(void* ptr)
{
    return data.allocator.free(ptr);
}

int used_alloc_ewram()
{
    return data.allocator.used_bytes();
}

int available_alloc_ewram()
{
    return data.allocator.available_bytes();
}

#if BN_CFG_LOG_ENABLED
    void log_alloc_ewram_status()
    {
        data.allocator.log_status();
    }
#endif

}
