#include "../include/btn_hw_bg_blocks.h"

#include "tonc.h"
#include "btn_memory.h"

namespace btn::hw::bg_blocks
{

namespace
{
    uint16_t& bg_block_vram(int block_index)
    {
        return reinterpret_cast<uint16_t*>(MEM_VRAM)[block_index * half_words_per_block()];
    }
}

uint16_t& vram(int block_index)
{
    return bg_block_vram(block_index);
}

void commit(const uint16_t& source_data_ref, int block_index, int half_words)
{
    memory::copy(source_data_ref, half_words, bg_block_vram(block_index));
}

}
