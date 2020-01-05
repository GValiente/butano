#include "btn_common.h"

#include <stdlib.h>
#include "btn_assert.h"
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

    btn::hw::memory::copy(source, int(bytes), destination);
}

void memcpy(void* destination, const void* source, int bytes)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(source, "Source is null");

    btn::hw::memory::copy(source, bytes, destination);
}

void memset(void* destination, uint8_t value, size_t bytes)
{
    BTN_ASSERT(destination, "Destination is null");

    btn::hw::memory::set(value, int(bytes), destination);
}

void memset(void* destination, uint8_t value, int bytes)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination, "Destination is null");

    btn::hw::memory::set(value, bytes, destination);
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
