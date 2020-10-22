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

void unsafe_copy_bytes(const void* source, int bytes, void* destination)
{
    btn::hw::memory::copy_bytes(source, bytes, destination);
}

void unsafe_copy_half_words(const void* source, int half_words, void* destination)
{
    btn::hw::memory::copy_half_words(source, half_words, destination);
}

void unsafe_copy_words(const void* source, int words, void* destination)
{
    btn::hw::memory::copy_words(source, words, destination);
}

void unsafe_clear_bytes(int bytes, void* destination)
{
    btn::hw::memory::set_bytes(0, bytes, destination);
}

void unsafe_clear_half_words(int half_words, void* destination)
{
    btn::hw::memory::set_half_words(0, half_words, destination);
}

void unsafe_clear_words(int words, void* destination)
{
    btn::hw::memory::set_words(0, words, destination);
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

void set_bytes(uint8_t value, int bytes, void* destination_ptr)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination_ptr, "Destination is null");

    hw::memory::set_bytes(value, bytes, destination_ptr);
}

void set_half_words(uint16_t value, int half_words, void* destination_ptr)
{
    BTN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);
    BTN_ASSERT(destination_ptr, "Destination is null");
    BTN_ASSERT(aligned<2>(destination_ptr), "Destination is not aligned");

    hw::memory::set_half_words(value, half_words, destination_ptr);
}

void set_words(unsigned value, int words, void* destination_ptr)
{
    BTN_ASSERT(words >= 0, "Invalid words: ", words);
    BTN_ASSERT(destination_ptr, "Destination is null");
    BTN_ASSERT(aligned<4>(destination_ptr), "Destination is not aligned");

    hw::memory::set_words(value, words, destination_ptr);
}

}
