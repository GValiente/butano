/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_memory.h"

#include "btn_alignment.h"
#include "btn_memory_manager.h"
#include "../hw/include/btn_hw_memory.h"

void* operator new(unsigned bytes)
{
    void* ptr = btn::memory_manager::ewram_alloc(bytes);
    BTN_ASSERT(ptr, "Allocation failed. Size in bytes: ", bytes);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    btn::memory_manager::ewram_free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] unsigned bytes) noexcept
{
    btn::memory_manager::ewram_free(ptr);
}

void* operator new[](unsigned bytes)
{
    void* ptr = btn::memory_manager::ewram_alloc(bytes);
    BTN_ASSERT(ptr, "Allocation failed. Size in bytes: ", bytes);
    return ptr;
}

void operator delete[](void* ptr) noexcept
{
    btn::memory_manager::ewram_free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] unsigned bytes) noexcept
{
    btn::memory_manager::ewram_free(ptr);
}

namespace _btn::memory
{

void unsafe_copy(const void* source, int bytes, void* destination)
{
    btn::hw::memory::copy(source, bytes, destination);
}

void unsafe_copy16(const void* source, int half_words, void* destination)
{
    btn::hw::memory::copy16(source, half_words, destination);
}

void unsafe_copy32(const void* source, int words, void* destination)
{
    btn::hw::memory::copy32(source, words, destination);
}

void unsafe_clear(int bytes, void* destination)
{
    btn::hw::memory::set(0, bytes, destination);
}

void unsafe_clear16(int half_words, void* destination)
{
    btn::hw::memory::set16(0, half_words, destination);
}

void unsafe_clear32(int words, void* destination)
{
    btn::hw::memory::set32(0, words, destination);
}

}


namespace btn::memory
{

void* ewram_alloc(int bytes)
{
    return memory_manager::ewram_alloc(bytes);
}

void ewram_free(void* ptr)
{
    memory_manager::ewram_free(ptr);
}

int used_alloc_ewram()
{
    return memory_manager::used_alloc_ewram();
}

int available_alloc_ewram()
{
    return memory_manager::available_alloc_ewram();
}

int used_items_ewram()
{
    return memory_manager::used_items_ewram();
}

int available_items_ewram()
{
    return memory_manager::available_items_ewram();
}

int used_static_iwram()
{
    return hw::memory::used_static_iwram();
}

int used_static_ewram()
{
    return hw::memory::used_static_ewram();
}

void set(uint8_t value, int bytes, void* destination_ptr)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination_ptr, "Destination is null");

    hw::memory::set(value, bytes, destination_ptr);
}

void set16(uint16_t value, int half_words, void* destination_ptr)
{
    BTN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);
    BTN_ASSERT(destination_ptr, "Destination is null");
    BTN_ASSERT(aligned<2>(destination_ptr), "Destination is not aligned");

    hw::memory::set16(value, half_words, destination_ptr);
}

void set32(unsigned value, int words, void* destination_ptr)
{
    BTN_ASSERT(words >= 0, "Invalid words: ", words);
    BTN_ASSERT(destination_ptr, "Destination is null");
    BTN_ASSERT(aligned<4>(destination_ptr), "Destination is not aligned");

    hw::memory::set32(value, words, destination_ptr);
}

}
