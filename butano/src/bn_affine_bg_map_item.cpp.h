/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_map_item.h"

#include "bn_bg_palette_ptr.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_tiles_ptr.h"
#include "../hw/include/bn_hw_decompress.h"

namespace bn
{

affine_bg_map_item affine_bg_map_item::decompress(span<affine_bg_map_cell> decompressed_cells_ref) const
{
    BN_ASSERT(decompressed_cells_ref.size() >= cells_count(),
              "There's not enough space to store the decompressed data: ",
              decompressed_cells_ref.size(), " - ", cells_count());

    affine_bg_map_cell* decompressed_cells_ptr = decompressed_cells_ref.data();
    BN_ASSERT(aligned<4>(decompressed_cells_ptr), "Destination map cells are not aligned");

    affine_bg_map_item result = *this;

    switch(_compression)
    {

    case compression_type::NONE:
        break;

    case compression_type::LZ77:
        hw::decompress::lz77(_cells_ptr, decompressed_cells_ptr);
        result._cells_ptr = decompressed_cells_ptr;
        result._compression = compression_type::NONE;
        break;

    case compression_type::RUN_LENGTH:
        hw::decompress::rl_wram(_cells_ptr, decompressed_cells_ptr);
        result._cells_ptr = decompressed_cells_ptr;
        result._compression = compression_type::NONE;
        break;

    case compression_type::HUFFMAN:
        hw::decompress::huff(_cells_ptr, decompressed_cells_ptr);
        result._cells_ptr = decompressed_cells_ptr;
        result._compression = compression_type::NONE;
        break;

    default:
        BN_ERROR("Unknown compression type: ", int(_compression));
        break;
    }

    return result;
}

affine_bg_map_item affine_bg_map_item::decompress(
        affine_bg_map_cell& decompressed_cells_ref, const size& decompressed_dimensions) const
{
    span<affine_bg_map_cell> decompressed_cells_span(
                &decompressed_cells_ref, decompressed_dimensions.width() * decompressed_dimensions.height());
    return decompress(decompressed_cells_span);
}

optional<affine_bg_map_ptr> affine_bg_map_item::find_map(
        const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const
{
    return affine_bg_map_ptr::find(*this, tiles, palette);
}

optional<affine_bg_map_ptr> affine_bg_map_item::find_map(
        const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette, int map_index) const
{
    return affine_bg_map_ptr::find(*this, tiles, palette, map_index);
}

affine_bg_map_ptr affine_bg_map_item::create_map(affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create(*this, move(tiles), move(palette));
}

affine_bg_map_ptr affine_bg_map_item::create_map(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return affine_bg_map_ptr::create(*this, move(tiles), move(palette), map_index);
}

affine_bg_map_ptr affine_bg_map_item::create_new_map(affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create_new(*this, move(tiles), move(palette));
}

affine_bg_map_ptr affine_bg_map_item::create_new_map(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return affine_bg_map_ptr::create_new(*this, move(tiles), move(palette), map_index);
}

optional<affine_bg_map_ptr> affine_bg_map_item::create_map_optional(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create_optional(*this, move(tiles), move(palette));
}

optional<affine_bg_map_ptr> affine_bg_map_item::create_map_optional(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return affine_bg_map_ptr::create_optional(*this, move(tiles), move(palette), map_index);
}

optional<affine_bg_map_ptr> affine_bg_map_item::create_new_map_optional(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create_new_optional(*this, move(tiles), move(palette));
}

optional<affine_bg_map_ptr> affine_bg_map_item::create_new_map_optional(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return affine_bg_map_ptr::create_new_optional(*this, move(tiles), move(palette), map_index);
}

}
