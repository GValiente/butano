/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_tiles_item.h"

#include "bn_affine_bg_tiles_ptr.h"
#include "../hw/include/bn_hw_decompress.h"

namespace bn
{

affine_bg_tiles_item affine_bg_tiles_item::decompress(span<tile> decompressed_tiles_ref) const
{
    int source_tiles_count = _tiles_ref.size();
    BN_ASSERT(decompressed_tiles_ref.size() >= source_tiles_count,
              "There's not enough space to store the decompressed data: ",
              decompressed_tiles_ref.size(), " - ", source_tiles_count);

    tile* dest_tiles_ptr = decompressed_tiles_ref.data();
    affine_bg_tiles_item result = *this;

    switch(_compression)
    {

    case compression_type::NONE:
        break;

    case compression_type::LZ77:
        hw::decompress::lz77(_tiles_ref.data(), dest_tiles_ptr);
        result._tiles_ref = span<const tile>(dest_tiles_ptr, source_tiles_count);
        result._compression = compression_type::NONE;
        break;

    case compression_type::RUN_LENGTH:
        hw::decompress::rl_wram(_tiles_ref.data(), dest_tiles_ptr);
        result._tiles_ref = span<const tile>(dest_tiles_ptr, source_tiles_count);
        result._compression = compression_type::NONE;
        break;

    case compression_type::HUFFMAN:
        hw::decompress::huff(_tiles_ref.data(), dest_tiles_ptr);
        result._tiles_ref = span<const tile>(dest_tiles_ptr, source_tiles_count);
        result._compression = compression_type::NONE;
        break;

    default:
        BN_ERROR("Unknown compression type: ", int(_compression));
        break;
    }

    return result;
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_item::find_tiles() const
{
    return affine_bg_tiles_ptr::find(*this);
}

affine_bg_tiles_ptr affine_bg_tiles_item::create_tiles() const
{
    return affine_bg_tiles_ptr::create(*this);
}

affine_bg_tiles_ptr affine_bg_tiles_item::create_new_tiles() const
{
    return affine_bg_tiles_ptr::create(*this);
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_item::create_tiles_optional() const
{
    return affine_bg_tiles_ptr::create_optional(*this);
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_item::create_new_tiles_optional() const
{
    return affine_bg_tiles_ptr::create_optional(*this);
}

}
