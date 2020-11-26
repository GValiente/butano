/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_BG_BLOCKS_H
#define BN_HW_BG_BLOCKS_H

#include "bn_hw_tonc.h"
#include "bn_hw_bg_blocks_constants.h"

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

    inline void copy_regular_bg_map_cell_tiles_offset(unsigned source_cell, unsigned tiles_offset,
                                                      uint16_t& destination_cell)
    {
        unsigned tile_id = BFN_GET(source_cell, SE_ID);
        BFN_SET(source_cell, tile_id + tiles_offset, SE_ID);
        destination_cell = uint16_t(source_cell);
    }

    inline void copy_affine_bg_map_cells_tiles_offset(unsigned first_source_cell, unsigned second_source_cell,
                                                      unsigned tiles_offset, uint16_t& destination_cell)
    {
        first_source_cell += tiles_offset;
        second_source_cell += tiles_offset;
        destination_cell = uint16_t((second_source_cell << 8) + first_source_cell);
    }

    inline void copy_regular_bg_map_cell_palette_offset(unsigned source_cell, unsigned palette_offset,
                                                        uint16_t& destination_cell)
    {
        unsigned palette_bank = BFN_GET(source_cell, SE_PALBANK);
        BFN_SET(source_cell, palette_bank + palette_offset, SE_PALBANK);
        destination_cell = uint16_t(source_cell);
    }

    inline void copy_regular_bg_map_cell_offset(unsigned source_cell, unsigned tiles_offset,
                                                unsigned palette_offset, uint16_t& destination_cell)
    {
        unsigned tile_id = BFN_GET(source_cell, SE_ID);
        unsigned palette_bank = BFN_GET(source_cell, SE_PALBANK);
        BFN_SET(source_cell, tile_id + tiles_offset, SE_ID);
        BFN_SET(source_cell, palette_bank + palette_offset, SE_PALBANK);
        destination_cell = uint16_t(source_cell);
    }
}

#endif
