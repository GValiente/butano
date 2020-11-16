/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_HDMA_H
#define BN_HW_HDMA_H

#include "bn_hw_tonc.h"

namespace bn::hw::hdma
{

namespace
{
    constexpr const int channel = 3;
}

inline void start(const uint16_t* source_ptr, int half_words, uint16_t* destination_ptr)
{
    DMA_TRANSFER(destination_ptr, source_ptr, half_words, channel, DMA_HDMA);
}

inline void stop()
{
    REG_DMA[channel].cnt = 0;
}

}

#endif
