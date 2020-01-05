#include "btn_memory.h"

#include "btn_alignment.h"
#include "../hw/include/btn_hw_memory.h"

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

int used_static_iwram()
{
    return hw::memory::used_static_iwram();
}

int used_static_ewram()
{
    return hw::memory::used_static_ewram();
}

int used_malloc_ewram()
{
    return hw::memory::used_malloc_ewram();
}

void set(uint8_t value, int bytes, void* destination)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination, "Destination is null");

    hw::memory::set(value, bytes, destination);
}

void set16(uint16_t value, int half_words, void* destination)
{
    BTN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 2), "Destination is not aligned");

    hw::memory::set16(value, half_words, destination);
}

void set32(unsigned value, int words, void* destination)
{
    BTN_ASSERT(words >= 0, "Invalid words: ", words);
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 4), "Destination is not aligned");

    hw::memory::set32(value, words, destination);
}

}
