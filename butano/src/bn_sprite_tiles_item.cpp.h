/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_tiles_item.h"

#include "bn_optional.h"
#include "bn_sprite_tiles_ptr.h"
#include "../hw/include/bn_hw_uncompress.h"

namespace bn
{

sprite_tiles_item sprite_tiles_item::uncompress(span<tile> uncompressed_tiles_ref) const
{
    int source_tiles_count = _tiles_ref.size();
    BN_ASSERT(uncompressed_tiles_ref.size() >= source_tiles_count,
              "There's not enough space to store the uncompressed data: ",
              uncompressed_tiles_ref.size(), " - ", source_tiles_count);

    tile* dest_tiles_ptr = uncompressed_tiles_ref.data();
    sprite_tiles_item result = *this;

    switch(_compression)
    {

    case compression_type::NONE:
        break;

    case compression_type::LZ77:
        hw::uncompress::lz77_wram(_tiles_ref.data(), dest_tiles_ptr);
        result._tiles_ref = span<const tile>(dest_tiles_ptr, source_tiles_count);
        result._compression = compression_type::NONE;
        break;

    case compression_type::RUN_LENGTH:
        hw::uncompress::rl_wram(_tiles_ref.data(), dest_tiles_ptr);
        result._tiles_ref = span<const tile>(dest_tiles_ptr, source_tiles_count);
        result._compression = compression_type::NONE;
        break;

    default:
        BN_ERROR("Unknown compression type: ", int(_compression));
        break;
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_item::find_tiles() const
{
    return sprite_tiles_ptr::find(*this);
}

optional<sprite_tiles_ptr> sprite_tiles_item::find_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::find(*this, graphics_index);
}

sprite_tiles_ptr sprite_tiles_item::create_tiles() const
{
    return sprite_tiles_ptr::create(*this);
}

sprite_tiles_ptr sprite_tiles_item::create_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::create(*this, graphics_index);
}

sprite_tiles_ptr sprite_tiles_item::create_new_tiles() const
{
    return sprite_tiles_ptr::create_new(*this);
}

sprite_tiles_ptr sprite_tiles_item::create_new_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::create_new(*this, graphics_index);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_tiles_optional() const
{
    return sprite_tiles_ptr::create_optional(*this);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_tiles_optional(int graphics_index) const
{
    return sprite_tiles_ptr::create_optional(*this, graphics_index);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_new_tiles_optional() const
{
    return sprite_tiles_ptr::create_new_optional(*this);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_new_tiles_optional(int graphics_index) const
{
    return sprite_tiles_ptr::create_new_optional(*this, graphics_index);
}

}
