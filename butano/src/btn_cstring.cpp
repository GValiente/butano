#include "btn_cstring.h"

#include "btn_assert.h"
#include "../hw/include/btn_hw_memory.h"

void* memcpy(void* destination, const void* source, unsigned bytes)
{
    btn::hw::memory::copy(source, bytes, destination);
    return destination;
}

void* memset(void* destination, int value, unsigned bytes)
{
    btn::hw::memory::set(value, bytes, destination);
    return destination;
}

namespace btn
{

void memcpy(void* destination, const void* source, int bytes)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(source, "Source is null");

    btn::hw::memory::copy(source, bytes, destination);
}

void memset(void* destination, uint8_t value, int bytes)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination, "Destination is null");

    btn::hw::memory::set(value, bytes, destination);
}

void memclear(void* destination, int bytes)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BTN_ASSERT(destination, "Destination is null");

    btn::hw::memory::set(0, bytes, destination);
}

}
