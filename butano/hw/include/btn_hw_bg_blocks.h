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

    [[nodiscard]] constexpr int alignment_blocks_count()
    {
        return 8;
    }

    [[nodiscard]] constexpr int half_words_per_block()
    {
        return 1024;
    }

    namespace
    {
        uint16_t& bg_block_vram(int block_index)
        {
            return reinterpret_cast<uint16_t*>(MEM_VRAM)[block_index * half_words_per_block()];
        }
    }

    [[nodiscard]] inline uint16_t& vram(int block_index)
    {
        return bg_block_vram(block_index);
    }

    inline void commit(const uint16_t& source_data_ref, int block_index, int half_words, int palette_offset)
    {
        if(palette_offset)
        {
            const uint16_t* source_data_ptr = &source_data_ref;
            uint16_t* destination_vram_ptr = &bg_block_vram(block_index);

            for(int index = 0; index < half_words; ++index)
            {
                uint16_t se = source_data_ptr[index];
                BFN_SET(se, palette_offset, SE_PALBANK);
                destination_vram_ptr[index] = se;
            }
        }
        else
        {
            memory::copy(source_data_ref, half_words, bg_block_vram(block_index));
        }
    }
}

#endif
