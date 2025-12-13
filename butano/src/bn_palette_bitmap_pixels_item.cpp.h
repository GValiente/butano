/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_palette_bitmap_pixels_item.h"

#include "../hw/include/bn_hw_decompress.h"

namespace bn
{

palette_bitmap_pixels_item palette_bitmap_pixels_item::decompress(span<uint8_t> decompressed_pixels_ref) const
{
    int source_pixels_count = pixels_count();
    BN_ASSERT(decompressed_pixels_ref.size() >= source_pixels_count,
              "There's not enough space to store the decompressed data: ",
              decompressed_pixels_ref.size(), " - ", source_pixels_count);

    uint8_t* dest_pixels_ptr = decompressed_pixels_ref.data();
    palette_bitmap_pixels_item result = *this;

    switch(_compression)
    {

    case compression_type::NONE:
        break;

    case compression_type::LZ77:
        hw::decompress::lz77(_pixels_ptr, dest_pixels_ptr);
        result._pixels_ptr = dest_pixels_ptr;
        result._compression = compression_type::NONE;
        break;

    case compression_type::RUN_LENGTH:
        hw::decompress::rl_wram(_pixels_ptr, dest_pixels_ptr);
        result._pixels_ptr = dest_pixels_ptr;
        result._compression = compression_type::NONE;
        break;

    case compression_type::HUFFMAN:
        hw::decompress::huff(_pixels_ptr, dest_pixels_ptr);
        result._pixels_ptr = dest_pixels_ptr;
        result._compression = compression_type::NONE;
        break;

    default:
        BN_ERROR("Unknown compression type: ", int(_compression));
        break;
    }

    return result;
}

}
