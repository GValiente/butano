#ifndef BTN_HW_BG_BLOCKS_H
#define BTN_HW_BG_BLOCKS_H

#include "btn_common.h"

namespace btn::hw::bg_blocks
{
    [[nodiscard]] constexpr int count()
    {
        return 32;
    }

    [[nodiscard]] constexpr int alignment()
    {
        return 8;
    }

    [[nodiscard]] constexpr int half_words_per_block()
    {
        return 1024;
    }

    [[nodiscard]] uint16_t& vram(int block_index);

    void commit(const uint16_t& source_data_ref, int block_index, int half_words);
}

#endif
