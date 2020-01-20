#include "../include/btn_hw_bg_blocks.h"

#include "tonc.h"
#include "btn_memory.h"
#include "btn_bg_block.h"

namespace btn::hw::bg_blocks
{

namespace
{
    static_assert(sizeof(SCREENBLOCK) == sizeof(bg_block));
    static_assert(alignof(SCREENBLOCK) == alignof(bg_block));

    bg_block& bg_block_vram(int index)
    {
        return reinterpret_cast<bg_block*>(MEM_VRAM)[index];
    }
}

bg_block& vram(int index)
{
    return bg_block_vram(index);
}

void commit(const bg_block& source_blocks_ref, int index, int count)
{
    memory::copy(source_blocks_ref, count, bg_block_vram(index));
}

}
