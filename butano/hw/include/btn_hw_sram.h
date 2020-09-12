#ifndef BTN_HW_SRAM_H
#define BTN_HW_SRAM_H

#include "btn_common.h"

namespace btn::hw::sram
{
    void init();

    BTN_CODE_EWRAM void write(const void* source, int size, int offset);

    BTN_CODE_EWRAM void read(void* destination, int size, int offset);
}

#endif
