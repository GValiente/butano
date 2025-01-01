/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_BG_BLOCKS_H
#define BN_HW_BG_BLOCKS_H

#include "bn_hw_tonc.h"
#include "bn_hw_bg_blocks_constants.h"

extern "C"
{
    BN_CODE_IWRAM void bn_hw_bg_blocks_commit_half_words(
            const uint16_t* source_data_ptr, unsigned half_words, uint16_t offset, uint16_t* destination_vram_ptr);

    BN_CODE_IWRAM void bn_hw_bg_blocks_commit_words(
            const unsigned* source_data_ptr, unsigned words, unsigned word_offset, unsigned* destination_vram_ptr);

    BN_CODE_IWRAM void bn_hw_bg_blocks_commit_blocks(
            const unsigned* source_data_ptr, unsigned blocks, unsigned word_offset, unsigned* destination_vram_ptr);
}

namespace bn::hw::bg_blocks
{
    [[nodiscard]] constexpr int max_blocks()
    {
        return 32;
    }

    [[nodiscard]] constexpr int tiles_alignment_blocks_count()
    {
        return 8;
    }

    [[nodiscard]] constexpr int max_bpp_4_regular_tiles_blocks_count()
    {
        return 16;
    }

    [[nodiscard]] constexpr int max_bpp_8_regular_tiles_blocks_count()
    {
        return 32;
    }

    [[nodiscard]] constexpr int max_affine_tiles_blocks_count()
    {
        return 8;
    }

    [[nodiscard]] constexpr int half_words_per_block()
    {
        return bg_maps::cells_count() / bg_maps::blocks_count();
    }

    [[nodiscard]] inline uint16_t* vram(int block_index)
    {
        return reinterpret_cast<uint16_t*>(MEM_VRAM) + (block_index * half_words_per_block());
    }

    [[nodiscard]] inline uint16_t regular_map_cells_offset(unsigned tiles_offset, unsigned palette_offset)
    {
        return uint16_t((palette_offset << 12) + tiles_offset);
    }

    [[nodiscard]] inline uint16_t affine_map_cells_offset(unsigned tiles_offset)
    {
        return uint16_t((tiles_offset << 8) + tiles_offset);
    }
}

#endif
