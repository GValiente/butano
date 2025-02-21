/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DMA_H
#define BN_HW_DMA_H

#include "bn_hw_audio.h"

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
    audio::start_dma(3, unsigned(half_words) | DMA_CPY16, source, destination);
}

inline void copy_words(const void* source, int words, void* destination)
{
    audio::start_dma(3, unsigned(words) | DMA_CPY32, source, destination);
}

inline void start_hdma(int channel, const uint16_t* source, int half_words, uint16_t* destination)
{
    audio::start_dma(channel, unsigned(half_words) | DMA_HDMA, source, destination);
}

inline void stop_hdma(int channel)
{
    audio::stop_dma(channel);
}

}

#endif
