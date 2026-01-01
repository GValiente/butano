/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_BITMAP_ITEM_H
#define BN_PALETTE_BITMAP_ITEM_H

/**
 * @file
 * bn::palette_bitmap_item header file.
 *
 * @ingroup palette_bitmap_bg
 * @ingroup tool
 */

#include "bn_bg_palette_item.h"
#include "bn_palette_bitmap_pixels_item.h"

namespace bn
{

/**
 * @brief Contains the required information to generate palette bitmaps.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `palette_bitmap` type.
 *
 * Pixels and colors are not copied but referenced, so they should outlive the palette_bitmap_item
 * to avoid dangling references.
 *
 * @ingroup palette_bitmap_bg
 * @ingroup tool
 */
class palette_bitmap_item
{

public:
    /**
     * @brief Constructor.
     * @param pixels_ref Reference to the palette bitmap pixels.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_item
     * to avoid dangling references.
     *
     * @param dimensions Size in pixels of the referenced palette bitmap pixels.
     * @param colors_ref Reference to an array of multiples of 16 colors.
     *
     * The colors are not copied but referenced, so they should outlive the palette_bitmap_item
     * to avoid dangling references.
     */
    constexpr palette_bitmap_item(
            const uint8_t& pixels_ref, const size& dimensions, const span<const color>& colors_ref) :
        _pixels_item(pixels_ref, dimensions),
        _palette_item(colors_ref, bpp_mode::BPP_8)
    {
    }

    /**
     * @brief Constructor.
     * @param pixels_item It references the pixels of the bitmap.
     * @param palette_item It references the colors of the bitmap.
     */
    constexpr palette_bitmap_item(
            const palette_bitmap_pixels_item& pixels_item, const bg_palette_item& palette_item) :
        _pixels_item(pixels_item),
        _palette_item(palette_item)
    {
        BN_ASSERT(palette_item.bpp() == bpp_mode::BPP_8, "BPP_4 palettes are not supported");
    }

    /**
     * @brief Returns the item that references the pixels of the bitmap.
     */
    [[nodiscard]] constexpr const palette_bitmap_pixels_item& pixels_item() const
    {
        return _pixels_item;
    }

    /**
     * @brief Returns the item that references the colors of the bitmap.
     */
    [[nodiscard]] constexpr const bg_palette_item& palette_item() const
    {
        return _palette_item;
    }

    /**
     * @brief Decompresses the stored data in the pixels referenced by decompressed_pixels_ref and
     * the colors referenced by decompressed_colors_ref.
     *
     * If the source and destination pixels or colors overlap, the behavior is undefined.
     *
     * @param decompressed_pixels_ref Destination of the decompressed pixels.
     * @param decompressed_colors_ref Destination of the decompressed colors.
     * @return A sprite_item pointing to the decompressed pixels and colors.
     */
    [[nodiscard]] palette_bitmap_item decompress(
            span<uint8_t> decompressed_pixels_ref, span<color> decompressed_colors_ref) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
            const palette_bitmap_item& a, const palette_bitmap_item& b) = default;

private:
    palette_bitmap_pixels_item _pixels_item;
    bg_palette_item _palette_item;
};

}

#endif

