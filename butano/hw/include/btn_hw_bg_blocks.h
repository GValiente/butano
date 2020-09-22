#ifndef BTN_HW_BG_BLOCKS_H
#define BTN_HW_BG_BLOCKS_H

#include "btn_memory.h"
#include "btn_hw_tonc.h"
#include "btn_hw_bg_blocks_constants.h"

namespace btn::hw::bg_blocks
{
    [[nodiscard]] constexpr int max_blocks()
    {
        return 32;
    }

    [[nodiscard]] constexpr int tiles_alignment_blocks_count()
    {
        return 8;
    }

    [[nodiscard]] constexpr int max_bpp_4_tiles_blocks_count()
    {
        return 16;
    }

    [[nodiscard]] constexpr int half_words_per_block()
    {
        return bg_maps::cells_count() / bg_maps::blocks_count();
    }

    namespace
    {
        [[nodiscard]] inline uint16_t* bg_block_vram(int block_index)
        {
            return reinterpret_cast<uint16_t*>(MEM_VRAM) + (block_index * half_words_per_block());
        }
    }

    [[nodiscard]] inline uint16_t* vram(int block_index)
    {
        return bg_block_vram(block_index);
    }

    inline void commit_tiles(const uint16_t* source_data_ptr, int block_index, int half_words)
    {
        uint16_t* destination_vram_ptr = bg_block_vram(block_index);
        memory::copy(*source_data_ptr, half_words, *destination_vram_ptr);
    }

    BTN_CODE_IWRAM void _commit_map_tiles_offset(const uint16_t* source_data_ptr, int half_words, int tiles_offset,
                                                 uint16_t* destination_vram_ptr);

    BTN_CODE_IWRAM void _commit_map_palette_offset(const uint16_t* source_data_ptr, int half_words, int palette_offset,
                                                   uint16_t* destination_vram_ptr);

    BTN_CODE_IWRAM void _commit_map_offset(const uint16_t* source_data_ptr, int half_words, int tiles_offset,
                                           int palette_offset, uint16_t* destination_vram_ptr);

    inline void commit_map(const uint16_t* source_data_ptr, int block_index, int half_words, int tiles_offset,
                           int palette_offset)
    {
        uint16_t* destination_vram_ptr = bg_block_vram(block_index);

        if(tiles_offset)
        {
            if(palette_offset)
            {
                _commit_map_offset(source_data_ptr, half_words, tiles_offset, palette_offset, destination_vram_ptr);
            }
            else
            {
                _commit_map_tiles_offset(source_data_ptr, half_words, tiles_offset, destination_vram_ptr);
            }
        }
        else
        {
            if(palette_offset)
            {
                _commit_map_palette_offset(source_data_ptr, half_words, palette_offset, destination_vram_ptr);
            }
            else
            {
                memory::copy(*source_data_ptr, half_words, *destination_vram_ptr);
            }
        }
    }
}

#endif
