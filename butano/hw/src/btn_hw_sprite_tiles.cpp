#include "../include/btn_hw_sprite_tiles.h"

#include "tonc.h"
#include "btn_tile.h"
#include "btn_memory.h"

namespace btn::hw::sprite_tiles
{

namespace
{
    tile& tile_vram(int tile_index)
    {
        auto sprite_vram = reinterpret_cast<tile*>(MEM_VRAM_OBJ);
        return sprite_vram[tile_index];
    }
}

void copy_tiles(const tile& source_tiles_ref, int tiles_count, tile& destination_tiles_ref)
{
    memory::copy(source_tiles_ref, tiles_count, destination_tiles_ref);
}

void clear_tiles(int tiles_count, tile& tiles_ref)
{
    memory::clear(tiles_count, tiles_ref);
}

tile& vram(int tile_index)
{
    return tile_vram(tile_index);
}

void commit(const tile& source_tiles_ref, int tile_index, int tiles_count)
{
    memory::copy(source_tiles_ref, tiles_count, tile_vram(tile_index));
}

}
