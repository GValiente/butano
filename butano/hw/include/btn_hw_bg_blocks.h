#ifndef BTN_HW_BG_BLOCKS_H
#define BTN_HW_BG_BLOCKS_H

#include "btn_common.h"

namespace btn
{

class bg_block;

namespace hw::bg_blocks
{
    [[nodiscard]] constexpr int count()
    {
        return 32;
    }

    [[nodiscard]] constexpr int alignment()
    {
        return 8;
    }

    [[nodiscard]] bg_block& vram(int index);

    void commit(const bg_block& source_blocks_ref, int index, int count);
}

}

#endif
