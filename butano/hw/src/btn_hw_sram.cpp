#include "../include/btn_hw_sram.h"

#include "tonc.h"
#include "btn_assert.h"
#include "btn_config_sram.h"

namespace btn::hw::sram
{

namespace
{
    // https://forum.gbadev.org/viewtopic.php?f=4&t=2825
    [[maybe_unused]] const char* save_type = "SRAM_V113";
}

void init()
{
    BIT_SET(REG_WAITCNT, BTN_CFG_SRAM_WAIT_STATE);
}

void write(const void* source, int size, int offset)
{
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(size >= 0, "Invalid size: ", size);
    BTN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BTN_ASSERT(size + offset <= max_size(), "Size and offset are too high: ", size, " - ", offset);

    auto sram_ptr = reinterpret_cast<uint8_t*>(MEM_SRAM + unused_bytes() + offset);
    auto source_ptr = static_cast<const uint8_t*>(source);

    for(int i = 0; i < size; i++)
    {
        sram_ptr[i] = source_ptr[i];
    }
}

void read(void* destination, int size, int offset)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(size >= 0, "Invalid size: ", size);
    BTN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BTN_ASSERT(size + offset <= max_size(), "Size and offset are too high: ", size, " - ", offset);

    auto sram_ptr = reinterpret_cast<const uint8_t*>(MEM_SRAM + unused_bytes() + offset);
    auto destination_ptr = static_cast<uint8_t*>(destination);

    for(int i = 0; i < size; i++)
    {
        destination_ptr[i] = sram_ptr[i];
    }
}

}
