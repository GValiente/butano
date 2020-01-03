#include "../include/btn_hw_sprite_tiles.h"

#include "tonc.h"
#include "btn_tile.h"
#include "../include/btn_hw_memory.h"

namespace btn::hw::sprite_tiles
{

namespace
{
    tile* tile_vram(int tile_index)
    {
        auto sprite_vram = reinterpret_cast<tile*>(MEM_VRAM_OBJ);
        return sprite_vram + tile_index;
    }
}

void copy_tiles(const tile* source_tiles_ptr, int tiles_count, tile* destination_tiles_ptr)
{
    BTN_ASSERT(source_tiles_ptr, "Source tiles pointer is null");
    BTN_ASSERT(tiles_count >= 0, "Invalid tiles count: ", tiles_count);
    BTN_ASSERT(destination_tiles_ptr, "Destination tiles pointer is null");

    hw::memcpy32(destination_tiles_ptr, span<const tile>(source_tiles_ptr, size_t(tiles_count)));
}

void clear_tiles(int tiles_count, tile* tiles_ptr)
{
    BTN_ASSERT(tiles_count >= 0, "Invalid tiles count: ", tiles_count);
    BTN_ASSERT(tiles_ptr, "Tiles pointer is null");

    hw::memset32(tiles_ptr, 0, tiles_count * int(sizeof(tile) / 4));
}

tile* vram(int tile_index)
{
    BTN_ASSERT(tile_index >= 0 && tile_index < count_per_bank() * 2, "Invalid tile index: ", tile_index);

    return tile_vram(tile_index);
}

void commit(const tile* source_tiles_ptr, int tile_index, int tiles_count)
{
    BTN_ASSERT(source_tiles_ptr, "Source tiles pointer is null");
    BTN_ASSERT(tile_index >= 0, "Invalid tile index: ", tile_index);
    BTN_ASSERT(tiles_count > 0, "Invalid tiles count: ", tiles_count);
    BTN_ASSERT(tile_index + tiles_count <= count_per_bank() * 2, "Too much tiles: ", tile_index, " - ", tiles_count);

    hw::memcpy32(tile_vram(tile_index), span<const tile>(source_tiles_ptr, size_t(tiles_count)));
}

}
