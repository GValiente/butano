/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_map_item.h"

#include "bn_optional.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "../hw/include/bn_hw_uncompress.h"

namespace bn
{

regular_bg_map_item regular_bg_map_item::uncompress(regular_bg_map_cell& uncompressed_cells_ref,
                                                    [[maybe_unused]] const size& uncompressed_dimensions) const
{
    BN_ASSERT(uncompressed_dimensions.width() >= _dimensions.width() &&
              uncompressed_dimensions.height() >= _dimensions.height(),
              "There's not enough space to store the uncompressed data: ",
              uncompressed_dimensions.width(), " - ", _dimensions.width(), " - ",
              uncompressed_dimensions.height(), " - ", _dimensions.height());

    BN_ASSERT(aligned<alignof(int)>(&uncompressed_cells_ref), "Destination map cells are not aligned");

    regular_bg_map_item result = *this;

    switch(_compression)
    {

    case compression_type::NONE:
        break;

    case compression_type::LZ77:
        hw::uncompress::lz77_wram(_cells_ptr, &uncompressed_cells_ref);
        result._cells_ptr = &uncompressed_cells_ref;
        result._compression = compression_type::NONE;
        break;

    case compression_type::RUN_LENGTH:
        hw::uncompress::rl_wram(_cells_ptr, &uncompressed_cells_ref);
        result._cells_ptr = &uncompressed_cells_ref;
        result._compression = compression_type::NONE;
        break;

    default:
        BN_ERROR("Unknown compression type: ", int(_compression));
        break;
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_item::find_map(
        const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const
{
    return regular_bg_map_ptr::find(*this, tiles, palette);
}

regular_bg_map_ptr regular_bg_map_item::create_map(regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create(*this, move(tiles), move(palette));
}

regular_bg_map_ptr regular_bg_map_item::create_new_map(regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create_new(*this, move(tiles), move(palette));
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_map_optional(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create_optional(*this, move(tiles), move(palette));
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_new_map_optional(
        regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create_new_optional(*this, move(tiles), move(palette));
}

}
