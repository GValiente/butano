#include "../include/btn_hw_memory.h"

#include <malloc.h>
#include "tonc.h"
#include "btn_assert.h"

extern unsigned __iwram_start__;
extern unsigned __iwram_end__;
extern unsigned __ewram_start;
extern unsigned __ewram_end;

namespace btn::hw::memory
{

int used_static_iwram()
{
    auto iwram_start = reinterpret_cast<uint8_t*>(&__iwram_start__);
    auto iwram_end = reinterpret_cast<uint8_t*>(&__iwram_end__);
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

int used_malloc_ewram()
{
    return int(mallinfo().uordblks);
}

void copy(const void* source, int bytes, void* destination)
{
    tonccpy(destination, source, unsigned(bytes));
}

void copy16(const void* source, int half_words, void* destination)
{
    memcpy16(destination, source, unsigned(half_words));
}

void copy32(const void* source, int words, void* destination)
{
    memcpy32(destination, source, unsigned(words));
}

void set(uint8_t value, int bytes, void* destination)
{
    toncset(destination, value, unsigned(bytes));
}

void set16(uint16_t value, int half_words, void* destination)
{
    memset16(destination, value, unsigned(half_words));
}

void set32(unsigned value, int words, void* destination)
{
    memset32(destination, value, unsigned(words));
}

}
