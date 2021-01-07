/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SRAM_H
#define BN_HW_SRAM_H

#include "bn_common.h"

namespace bn::hw::sram
{
    void init();

    BN_CODE_EWRAM void write(const void* source, int size, int offset);

    BN_CODE_EWRAM void read(void* destination, int size, int offset);
}

#endif
