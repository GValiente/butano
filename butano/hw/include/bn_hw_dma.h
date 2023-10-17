/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DMA_H
#define BN_HW_DMA_H

#include "bn_hw_tonc.h"

namespace bn::hw::dma
{

[[nodiscard]] constexpr int low_priority_channel()
{
    return 3;
}

[[nodiscard]] constexpr int high_priority_channel()
{
    return 0;
}

inline void copy_half_words(const void* source, int half_words, void* destination)
{
    REG_DMA[3].cnt = 0;
    REG_DMA[3].src = source;
    REG_DMA[3].dst = destination;
    REG_DMA[3].cnt = unsigned(half_words) | DMA_CPY16;
}

inline void copy_words(const void* source, int words, void* destination)
{
    REG_DMA[3].cnt = 0;
    REG_DMA[3].src = source;
    REG_DMA[3].dst = destination;
    REG_DMA[3].cnt = unsigned(words) | DMA_CPY32;
}

inline void start_hdma(int channel, const uint16_t* source, int half_words, uint16_t* destination)
{
    REG_DMA[channel].cnt = 0;
    REG_DMA[channel].src = source;
    REG_DMA[channel].dst = destination;
    REG_DMA[channel].cnt = unsigned(half_words) | DMA_HDMA;
}

inline void stop_hdma(int channel)
{
    REG_DMA[channel].cnt = 0;
}

}

#endif
