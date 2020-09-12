#ifndef BTN_HW_SRAM_H
#define BTN_HW_SRAM_H

#include "btn_config_sram.h"
#include "btn_hw_tonc.h"

namespace btn::hw::sram
{
    inline void init()
    {
        BIT_SET(REG_WAITCNT_NV, BTN_CFG_SRAM_WAIT_STATE);
    }

    inline void write(const void* source, int size, int offset)
    {
        volatile const uint8_t* source_ptr = static_cast<const uint8_t*>(source);
        volatile uint8_t* sram_ptr = reinterpret_cast<uint8_t*>(MEM_SRAM) + offset;

        for(int i = 0; i < size; i++)
        {
            sram_ptr[i] = source_ptr[i];
        }
    }

    BTN_CODE_IWRAM void read(void* destination, int size, int offset);
}

#endif
