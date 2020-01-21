#include "../include/btn_hw_sprite_tiles.h"

#include "tonc.h"
#include "btn_tile.h"
#include "btn_memory.h"

namespace btn::hw::sprite_tiles
{

namespace
{
    static_assert(sizeof(TILE) == sizeof(tile));
    static_assert(alignof(TILE) == alignof(tile));

    tile& tile_vram(int index)
    {
        return reinterpret_cast<tile*>(MEM_VRAM_OBJ)[index];
    }
}

void copy_tiles(const tile& source_tiles_ref, int count, tile& destination_tiles_ref)
{
    memory::copy(source_tiles_ref, count, destination_tiles_ref);
}

void clear_tiles(int count, tile& tiles_ref)
{
    memory::clear(count, tiles_ref);
}

tile& vram(int index)
{
    return tile_vram(index);
}

void commit(const tile& source_tiles_ref, int index, int count)
{
    memory::copy(source_tiles_ref, count, tile_vram(index));
}

}
