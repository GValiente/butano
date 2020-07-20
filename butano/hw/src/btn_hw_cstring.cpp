#include "../hw/include/btn_hw_memory.h"

extern "C"
{
    void* memcpy(void* destination, const void* source, unsigned bytes)
    {
        btn::hw::memory::copy(source, bytes, destination);
        return destination;
    }
}
