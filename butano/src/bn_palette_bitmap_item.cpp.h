/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_palette_bitmap_item.h"

namespace bn
{

palette_bitmap_item palette_bitmap_item::decompress(
        span<uint8_t> decompressed_pixels_ref, span<color> decompressed_colors_ref) const
{
    palette_bitmap_item result = *this;
    result._pixels_item = _pixels_item.decompress(decompressed_pixels_ref);
    result._palette_item = _palette_item.decompress(decompressed_colors_ref);
    return result;
}

}
