/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_direct_bitmap_item.h"

#include "../hw/include/bn_hw_decompress.h"

namespace bn
{

direct_bitmap_item direct_bitmap_item::decompress(span<bn::color> decompressed_colors_ref) const
{
    int source_colors_count = colors_count();
    BN_ASSERT(decompressed_colors_ref.size() >= source_colors_count,
              "There's not enough space to store the decompressed data: ",
              decompressed_colors_ref.size(), " - ", source_colors_count);

    bn::color* dest_colors_ptr = decompressed_colors_ref.data();
    direct_bitmap_item result = *this;

    switch(_compression)
    {

    case compression_type::NONE:
        break;

    case compression_type::LZ77:
        hw::decompress::lz77(_colors_ptr, dest_colors_ptr);
        result._colors_ptr = dest_colors_ptr;
        result._compression = compression_type::NONE;
        break;

    case compression_type::RUN_LENGTH:
        hw::decompress::rl_wram(_colors_ptr, dest_colors_ptr);
        result._colors_ptr = dest_colors_ptr;
        result._compression = compression_type::NONE;
        break;

    case compression_type::HUFFMAN:
        hw::decompress::huff(_colors_ptr, dest_colors_ptr);
        result._colors_ptr = dest_colors_ptr;
        result._compression = compression_type::NONE;
        break;

    default:
        BN_ERROR("Unknown compression type: ", int(_compression));
        break;
    }

    return result;
}

}
