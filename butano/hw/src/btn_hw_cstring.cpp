#include "btn_assert.h"
#include "../hw/include/btn_hw_memory.h"

extern "C"
{
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
}
