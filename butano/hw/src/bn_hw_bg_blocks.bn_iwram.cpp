/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_bg_blocks.h"

namespace bn::hw::bg_blocks
{

void _commit_offset_impl(const unsigned* source_data_ptr, unsigned words, unsigned word_offset,
                         unsigned* destination_vram_ptr)
{
    for(unsigned index = 0; index < words; index += 2)
    {
        destination_vram_ptr[index] = source_data_ptr[index] + word_offset;
        destination_vram_ptr[index + 1] = source_data_ptr[index + 1] + word_offset;
    }
}

}
