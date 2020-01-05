#include "btn_sram.h"

#include "../hw/include/btn_hw_sram.h"

namespace _btn::sram
{

void unsafe_write(const void* source, int size, int offset)
{
    btn::hw::sram::write(source, size, offset);
}

void unsafe_read(void* destination, int size, int offset)
{
    btn::hw::sram::read(destination, size, offset);
}

}

namespace btn::sram
{

int max_size()
{
    return hw::sram::max_size();
}

}
