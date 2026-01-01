/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
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

    alignas(static_data) BN_DATA_EWRAM_BSS char data_buffer[sizeof(static_data)];

    [[nodiscard]] static_data& data_ref()
    {
        return *reinterpret_cast<static_data*>(data_buffer);
    }
}

void init()
{
    ::new(static_cast<void*>(data_buffer)) static_data();

    char* start = hw::memory::ewram_heap_start();
    char* end = hw::memory::ewram_heap_end();
    data_ref().allocator.reset(static_cast<void*>(start), end - start);
}

void* ewram_alloc(int bytes)
{
    return data_ref().allocator.alloc(bytes);
}

void* ewram_calloc(int num, int bytes)
{
    return data_ref().allocator.calloc(num, bytes);
}

void* ewram_realloc(void* ptr, int new_bytes)
{
    return data_ref().allocator.realloc(ptr, new_bytes);
}

void ewram_free(void* ptr)
{
    return data_ref().allocator.free(ptr);
}

int used_alloc_ewram()
{
    return data_ref().allocator.used_bytes();
}

int available_alloc_ewram()
{
    return data_ref().allocator.available_bytes();
}

#if BN_CFG_LOG_ENABLED
    void log_alloc_ewram_status()
    {
        data_ref().allocator.log_status();
    }
#endif

}
