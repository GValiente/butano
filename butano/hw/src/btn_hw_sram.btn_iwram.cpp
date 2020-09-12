#include "../include/btn_hw_sram.h"

namespace btn::hw::sram
{

void read(void* destination, int size, int offset)
{
    // http://problemkaputt.de/gbatek.htm#gbacartbackupsramfram
    // (Reading and Writing section)

    volatile const uint8_t* sram_ptr = reinterpret_cast<const uint8_t*>(MEM_SRAM) + offset;
    volatile uint8_t* destination_ptr = static_cast<uint8_t*>(destination);

    for(int i = 0; i < size; i++)
    {
        destination_ptr[i] = sram_ptr[i];
    }
}

}
