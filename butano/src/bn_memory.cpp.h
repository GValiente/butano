/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_memory.h"

#include "bn_compression_type.h"
#include "bn_memory_manager.h"
#include "../hw/include/bn_hw_memory.h"
#include "../hw/include/bn_hw_decompress.h"

void* operator new(unsigned bytes)
{
    void* ptr = bn::memory_manager::ewram_alloc(int(bytes));
    BN_BASIC_ASSERT(ptr, "Allocation failed. Size in bytes: ", bytes);

    return ptr;
}

void operator delete(void* ptr) noexcept
{
    bn::memory_manager::ewram_free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] unsigned bytes) noexcept
{
    bn::memory_manager::ewram_free(ptr);
}

void* operator new[](unsigned bytes)
{
    void* ptr = bn::memory_manager::ewram_alloc(int(bytes));
    BN_BASIC_ASSERT(ptr, "Allocation failed. Size in bytes: ", bytes);

    return ptr;
}

void operator delete[](void* ptr) noexcept
{
    bn::memory_manager::ewram_free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] unsigned bytes) noexcept
{
    bn::memory_manager::ewram_free(ptr);
}

namespace _bn::memory
{

void unsafe_copy_bytes(const void* source, int bytes, void* destination)
{
    bn::hw::memory::copy_bytes(source, bytes, destination);
}

void unsafe_copy_half_words(const void* source, int half_words, void* destination)
{
    bn::hw::memory::copy_half_words(source, half_words, destination);
}

void unsafe_copy_words(const void* source, int words, void* destination)
{
    bn::hw::memory::copy_words(source, words, destination);
}

void unsafe_clear_bytes(int bytes, void* destination)
{
    bn::hw::memory::set_bytes(0, bytes, destination);
}

void unsafe_clear_half_words(int half_words, void* destination)
{
    bn::hw::memory::set_half_words(0, half_words, destination);
}

void unsafe_clear_words(int words, void* destination)
{
    bn::hw::memory::set_words(0, words, destination);
}

}


namespace bn::memory
{

void* ewram_alloc(int bytes)
{
    return memory_manager::ewram_alloc(bytes);
}

void* ewram_calloc(int num, int bytes)
{
    return memory_manager::ewram_calloc(num, bytes);
}

void* ewram_realloc(void* ptr, int new_bytes)
{
    return memory_manager::ewram_realloc(ptr, new_bytes);
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

void log_alloc_ewram_status()
{
    #if BN_CFG_LOG_ENABLED
        memory_manager::log_alloc_ewram_status();
    #endif
}

int used_stack_iwram()
{
    return hw::memory::used_stack_iwram(hw::memory::stack_address());
}

int used_static_iwram()
{
    return hw::memory::used_static_iwram();
}

int used_static_ewram()
{
    return hw::memory::used_static_ewram();
}

int used_rom()
{
    return hw::memory::used_rom();
}

void set_bytes(uint8_t value, int bytes, void* destination_ptr)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_BASIC_ASSERT(destination_ptr, "Destination is null");

    hw::memory::set_bytes(value, bytes, destination_ptr);
}

void set_half_words(uint16_t value, int half_words, void* destination_ptr)
{
    BN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);
    BN_BASIC_ASSERT(destination_ptr, "Destination is null");
    BN_ASSERT(aligned<2>(destination_ptr), "Destination is not aligned");

    hw::memory::set_half_words(value, half_words, destination_ptr);
}

void set_words(unsigned value, int words, void* destination_ptr)
{
    BN_ASSERT(words >= 0, "Invalid words: ", words);
    BN_BASIC_ASSERT(destination_ptr, "Destination is null");
    BN_ASSERT(aligned<4>(destination_ptr), "Destination is not aligned");

    hw::memory::set_words(value, words, destination_ptr);
}

void decompress(compression_type compression, const void* source_ptr, int bytes, void* destination_ptr)
{
    BN_BASIC_ASSERT(source_ptr, "Source is null");
    BN_BASIC_ASSERT(destination_ptr, "Destination is null");

    switch(compression)
    {

    case compression_type::NONE:
        copy(*static_cast<const char*>(source_ptr), bytes, *static_cast<char*>(destination_ptr));
        break;

    case compression_type::LZ77:
        hw::decompress::lz77(source_ptr, destination_ptr);
        break;

    case compression_type::RUN_LENGTH:
        if(hw::memory::in_vram(destination_ptr))
        {
            hw::decompress::rl_vram(source_ptr, destination_ptr);
        }
        else
        {
            hw::decompress::rl_wram(source_ptr, destination_ptr);
        }
        break;

    case compression_type::HUFFMAN:
        hw::decompress::huff(source_ptr, destination_ptr);
        break;

    default:
        BN_ERROR("Unknown compression type: ", int(compression));
        break;
    }
}

}
