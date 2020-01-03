#include "../include/btn_hw_memory.h"

#include <malloc.h>
#include "tonc.h"
#include "btn_alignment.h"

extern unsigned __iwram_start__;
extern unsigned __iwram_end__;
extern unsigned __ewram_start;
extern unsigned __ewram_end;

namespace btn::hw
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
    auto result = int(mallinfo().uordblks);
    BTN_ASSERT(result >= 0, "Invalid used malloc ewram: ", result);

    return result;
}

void memcpy(void* destination, const void* source, int bytes)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    tonccpy(destination, source, unsigned(bytes));
}

void memcpy16(void* destination, const void* source, int half_words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 2), "Destination is not aligned");
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(aligned(source, 2), "Source is not aligned");
    BTN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);

    ::memcpy16(destination, source, unsigned(half_words));
}

void memcpy32(void* destination, const void* source, int words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 4), "Destination is not aligned");
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(aligned(source, 4), "Source is not aligned");
    BTN_ASSERT(words >= 0, "Invalid words: ", words);

    ::memcpy32(destination, source, unsigned(words));
}

void memset(void* destination, uint8_t value, int bytes)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    toncset(destination, value, unsigned(bytes));
}

void memset16(void* destination, uint16_t value, int half_words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 2), "Destination is not aligned");
    BTN_ASSERT(half_words >= 0, "Invalid half words: ", half_words);

    ::memset16(destination, value, unsigned(half_words));
}

void memset32(void* destination, unsigned value, int words)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(aligned(destination, 4), "Destination is not aligned");
    BTN_ASSERT(words >= 0, "Invalid words: ", words);

    ::memset32(destination, value, unsigned(words));
}

}
