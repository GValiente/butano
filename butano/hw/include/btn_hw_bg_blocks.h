#ifndef BTN_HW_BG_BLOCKS_H
#define BTN_HW_BG_BLOCKS_H

#include "tonc.h"
#include "btn_memory.h"

namespace btn::hw::bg_blocks
{
    [[nodiscard]] constexpr int count()
    {
        return 32;
    }

    [[nodiscard]] constexpr int tiles_alignment_blocks_count()
    {
        return 8;
    }

    [[nodiscard]] constexpr int half_words_per_block()
    {
        return 1024;
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

    BTN_CODE_IWRAM void _commit_palette_offset_impl(const uint16_t* source_data_ptr, int half_words,
                                                    int palette_offset, uint16_t* destination_vram_ptr);

    inline void commit(const uint16_t* source_data_ptr, int block_index, int half_words, int palette_offset)
    {
        uint16_t* destination_vram_ptr = bg_block_vram(block_index);

        if(palette_offset)
        {
            _commit_palette_offset_impl(source_data_ptr, half_words, palette_offset, destination_vram_ptr);
        }
        else
        {
            memory::copy(*source_data_ptr, half_words, *destination_vram_ptr);
        }
    }
}

#endif
