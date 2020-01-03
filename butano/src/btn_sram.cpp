#include "btn_sram.h"

#include "btn_assert.h"
#include "../hw/include/btn_hw_sram.h"

namespace btn::sram
{

int max_size()
{
    return hw::sram::max_size();
}

void write(const void* source, int size)
{
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(size >= 0, "Invalid size: ", size);
    BTN_ASSERT(size <= max_size(), "Size is too high: ", size);

    hw::sram::write(source, size, 0);
}

void write_offset(const void* source, int size, int offset)
{
    BTN_ASSERT(source, "Source is null");
    BTN_ASSERT(size >= 0, "Invalid size: ", size);
    BTN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BTN_ASSERT(size + offset <= max_size(), "Size and offset are too high: ", size, " - ", offset);

    hw::sram::write(source, size, offset);
}

void read(void* destination, int size)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(size >= 0, "Invalid size: ", size);
    BTN_ASSERT(size <= max_size(), "Size is too high: ", size);

    hw::sram::read(destination, size, 0);
}

void read_offset(void* destination, int size, int offset)
{
    BTN_ASSERT(destination, "Destination is null");
    BTN_ASSERT(size >= 0, "Invalid size: ", size);
    BTN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BTN_ASSERT(size + offset <= max_size(), "Size and offset are too high: ", size, " - ", offset);

    hw::sram::read(destination, size, offset);
}

}
