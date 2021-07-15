/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_bg_blocks.h"

namespace bn::hw::bg_blocks
{

void commit_offset(const uint16_t* source_data_ptr, int half_words, uint16_t offset, uint16_t* destination_vram_ptr)
{
    for(int index = 0; index < half_words; ++index)
    {
        destination_vram_ptr[index] = source_data_ptr[index] + offset;
    }
}

}
