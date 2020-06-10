#include "../include/btn_hw_memory.h"

#include "btn_assert.h"

extern unsigned __iwram_start__;
extern unsigned __fini_array_end;
extern unsigned __ewram_start;
extern unsigned __ewram_end;
extern char __eheap_start[], __eheap_end[];

namespace btn::hw::memory
{

int used_static_iwram()
{
    auto iwram_start = reinterpret_cast<uint8_t*>(&__iwram_start__);
    auto iwram_end = reinterpret_cast<uint8_t*>(&__fini_array_end);
    auto result = iwram_end - iwram_start;
    BTN_ASSERT(result >= 0, "Invalid used static iwram: ", result);

    return result;
}

int used_static_ewram()
{
    auto ewram_start = reinterpret_cast<uint8_t*>(&__ewram_start);
    auto ewram_end = reinterpret_cast<uint8_t*>(&__ewram_end);
    auto result = ewram_end - ewram_start;
    BTN_ASSERT(result >= 0, "Invalid used static ewram: ", result);

    return result;
}

char* ewram_heap_start()
{
    return __eheap_start;
}

char* ewram_heap_end()
{
    return __eheap_end;
}

}
