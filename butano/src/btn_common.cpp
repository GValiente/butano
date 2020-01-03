#include "btn_common.h"

#include <stdlib.h>
#include "btn_assert.h"
#include "btn_alignment.h"
#include "../hw/include/btn_hw_memory.h"

namespace btn
{

void* malloc(size_t bytes)
{
    BTN_ASSERT(bytes, "Size in bytes is 0");

    return ::malloc(bytes);
}

void* malloc(int bytes)
{
    BTN_ASSERT(bytes > 0, "Invalid bytes: ", bytes);

    return ::malloc(unsigned(bytes));
}

void free(void* ptr)
{
    ::free(ptr);
}

void memcpy(void* destination, const void* source, size_t bytes)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(source, "Source is null");

    hw::memcpy(destination, source, int(bytes));
}

void memcpy(void* destination, const void* source, int bytes)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    hw::memcpy(destination, source, bytes);
}

void memcpy16(void* destination, const void* source, int half_words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 2), "Destination is not aligned");
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(aligned(source, 2), "Source is not aligned");
    BTN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);

    hw::memcpy16(destination, source, half_words);
}

void memcpy32(void* destination, const void* source, int words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 4), "Destination is not aligned");
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(aligned(source, 4), "Source is not aligned");
    BTN_ASSERT(words >= 0, "Invalid words: ", words);

    hw::memcpy32(destination, source, words);
}

void memset(void* destination, uint8_t value, size_t bytes)
{
    BTN_ASSERT(destination, "Destination is null");

    hw::memset(destination, value, int(bytes));
}

void memset(void* destination, uint8_t value, int bytes)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    hw::memset(destination, value, bytes);
}

void memset16(void* destination, uint16_t value, int half_words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 2), "Destination is not aligned");
    BTN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);

    hw::memset16(destination, value, half_words);
}

void memset32(void* destination, unsigned value, int words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 4), "Destination is not aligned");
    BTN_ASSERT(words >= 0, "Invalid words: ", words);

    hw::memset32(destination, value, words);
}

}

void* operator new(size_t bytes)
{
    BTN_ASSERT(bytes, "Size in bytes is 0");

    void* ptr = btn::malloc(bytes);
    BTN_ASSERT(ptr, "Allocation failed. Size in bytes: ", bytes);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    btn::free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] size_t bytes) noexcept
{
    btn::free(ptr);
}

void* operator new[](size_t bytes)
{
    BTN_ASSERT(bytes, "Size in bytes is 0");

    void* ptr = btn::malloc(bytes);
    BTN_ASSERT(ptr, "Allocation failed. Size in bytes: ", bytes);
    return ptr;
}

void operator delete[](void* ptr) noexcept
{
    btn::free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] size_t bytes) noexcept
{
    btn::free(ptr);
}
