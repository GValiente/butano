#ifndef BTN_HW_SRAM_H
#define BTN_HW_SRAM_H

#include "tonc.h"
#include "btn_common.h"
#include "btn_config_sram.h"

namespace btn::hw::sram
{
    [[nodiscard]] constexpr int unused_bytes()
    {
        return sizeof(int);
    }

    [[nodiscard]] constexpr int max_size()
    {
        return (32 * 1024) - (unused_bytes() * 2);
    }

    inline void init()
    {
        BIT_SET(REG_WAITCNT, BTN_CFG_SRAM_WAIT_STATE);
    }

    inline void write(const void* source, int size, int offset)
    {
        auto sram_ptr = reinterpret_cast<uint8_t*>(MEM_SRAM) + unused_bytes() + offset;
        auto source_ptr = static_cast<const uint8_t*>(source);

        for(int i = 0; i < size; i++)
        {
            sram_ptr[i] = source_ptr[i];
        }
    }

    inline void read(void* destination, int size, int offset)
    {
        auto sram_ptr = reinterpret_cast<const uint8_t*>(MEM_SRAM) + unused_bytes() + offset;
        auto destination_ptr = static_cast<uint8_t*>(destination);

        for(int i = 0; i < size; i++)
        {
            destination_ptr[i] = sram_ptr[i];
        }
    }
}

#endif
