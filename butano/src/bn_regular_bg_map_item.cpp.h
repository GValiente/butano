/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_map_item.h"

#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "../hw/include/bn_hw_decompress.h"

namespace bn
{

regular_bg_map_item regular_bg_map_item::decompress(span<regular_bg_map_cell> decompressed_cells_ref) const
{
    BN_ASSERT(decompressed_cells_ref.size() >= cells_count(),
              "There's not enough space to store the decompressed data: ",
              decompressed_cells_ref.size(), " - ", cells_count());

    regular_bg_map_cell* decompressed_cells_ptr = decompressed_cells_ref.data();
    BN_ASSERT(aligned<4>(decompressed_cells_ptr), "Destination map cells are not aligned");

    regular_bg_map_item result = *this;

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

regular_bg_map_item regular_bg_map_item::decompress(
        regular_bg_map_cell& decompressed_cells_ref, const size& decompressed_dimensions) const
{
    span<regular_bg_map_cell> decompressed_cells_span(
                &decompressed_cells_ref, decompressed_dimensions.width() * decompressed_dimensions.height());
    return decompress(decompressed_cells_span);
}

optional<regular_bg_map_ptr> regular_bg_map_item::find_map(
        const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const
{
    return regular_bg_map_ptr::find(*this, tiles, palette);
}

optional<regular_bg_map_ptr> regular_bg_map_item::find_map(
        const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette, int map_index) const
{
    return regular_bg_map_ptr::find(*this, tiles, palette, map_index);
}

regular_bg_map_ptr regular_bg_map_item::create_map(regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create(*this, move(tiles), move(palette));
}

regular_bg_map_ptr regular_bg_map_item::create_map(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return regular_bg_map_ptr::create(*this, move(tiles), move(palette), map_index);
}

regular_bg_map_ptr regular_bg_map_item::create_new_map(regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create_new(*this, move(tiles), move(palette));
}

regular_bg_map_ptr regular_bg_map_item::create_new_map(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return regular_bg_map_ptr::create_new(*this, move(tiles), move(palette), map_index);
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_map_optional(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create_optional(*this, move(tiles), move(palette));
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_map_optional(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return regular_bg_map_ptr::create_optional(*this, move(tiles), move(palette), map_index);
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_new_map_optional(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create_new_optional(*this, move(tiles), move(palette));
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_new_map_optional(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const
{
    return regular_bg_map_ptr::create_new_optional(*this, move(tiles), move(palette), map_index);
}

}
