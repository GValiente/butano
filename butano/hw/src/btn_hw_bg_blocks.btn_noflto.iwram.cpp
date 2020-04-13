#include "../include/btn_hw_bg_blocks.h"

namespace btn::hw::bg_blocks
{

void _commit_map_offset(const uint16_t* source_data_ptr, int half_words, int tiles_offset,
                        int palette_offset, uint16_t* destination_vram_ptr)
{
    for(int index = 0; index < half_words; ++index)
    {
        int se = source_data_ptr[index];
        int tile_id = BFN_GET(se, SE_ID);
        int palette_bank = BFN_GET(se, SE_PALBANK);
        BFN_SET(se, tile_id + tiles_offset, SE_ID);
        BFN_SET(se, palette_bank + palette_offset, SE_PALBANK);
        destination_vram_ptr[index] = uint16_t(se);
    }
}

}
