/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DIRECT_BITMAP_ITEM_H
#define BN_DIRECT_BITMAP_ITEM_H

/**
 * @file
 * bn::direct_bitmap_item header file.
 *
 * @ingroup direct_bitmap_bg
 * @ingroup tool
 */

#include "bn_size.h"
#include "bn_span.h"
#include "bn_color.h"
#include "bn_point.h"
#include "bn_compression_type.h"

namespace bn
{

/**
 * @brief Contains the required information to generate direct bitmaps.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `direct_bitmap` type.
 *
 * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
 * to avoid dangling references.
 *
 * @ingroup direct_bitmap_bg
 * @ingroup tool
 */
class direct_bitmap_item
{

public:
    /**
     * @brief Constructor.
     * @param colors_ref Reference to the bitmap colors.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     *
     * @param dimensions Size in pixels of the referenced bitmap.
     */
    constexpr direct_bitmap_item(const bn::color& colors_ref, const size& dimensions) :
        direct_bitmap_item(colors_ref, dimensions, compression_type::NONE)
    {
    }

    /**
     * @brief Constructor.
     * @param colors_ref Reference to the bitmap colors.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     *
     * @param dimensions Size in pixels of the referenced bitmap.
     * @param compression Compression type.
     */
    constexpr direct_bitmap_item(
            const bn::color& colors_ref, const size& dimensions, compression_type compression) :
        _colors_ptr(&colors_ref),
        _dimensions(dimensions),
        _compression(compression)
    {
        BN_ASSERT(dimensions.width() > 0, "Invalid width: ", dimensions.width());
        BN_ASSERT(dimensions.height() > 0, "Invalid height: ", dimensions.height());
    }

    /**
     * @brief Returns a pointer to the referenced bitmap colors.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const bn::color* colors_ptr() const
    {
        return _colors_ptr;
    }

    /**
     * @brief Returns the referenced bitmap colors.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const bn::color& colors_ref() const
    {
        return *_colors_ptr;
    }

    /**
     * @brief Returns the size in pixels of the referenced bitmap.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    /**
     * @brief Returns the number of referenced colors.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr int colors_count() const
    {
        return _dimensions.width() * _dimensions.height();
    }

    /**
     * @brief Returns the index of the referenced color in the specified pixel coordinates.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     *
     * @param x Horizontal position of the bitmap color [0..dimensions().width()).
     * @param y Vertical position of the bitmap color [0..dimensions().height()).
     * @return The index of the referenced bitmap color.
     */
    [[nodiscard]] constexpr int color_index(int x, int y) const
    {
        int width = _dimensions.width();
        BN_ASSERT(x >= 0 && x < width, "Invalid x: ", x, " - ", width);
        BN_ASSERT(y >= 0 && y < _dimensions.height(), "Invalid y: ", y, " - ", _dimensions.height());
        BN_BASIC_ASSERT(_compression == compression_type::NONE, "Compressed colors not supported");

        return (y * width) + x;
    }

    /**
     * @brief Returns the index of the referenced color in the specified pixel coordinates.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     *
     * @param position Position of the bitmap color.
     * @return The index of the referenced bitmap color.
     */
    [[nodiscard]] constexpr int color_index(const point& position) const
    {
        return color_index(position.x(), position.y());
    }

    /**
     * @brief Returns the referenced bitmap color in the specified pixel coordinates.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     *
     * @param x Horizontal position of the bitmap color [0..dimensions().width()).
     * @param y Vertical position of the bitmap color [0..dimensions().height()).
     * @return The referenced bitmap color.
     */
    [[nodiscard]] constexpr bn::color color(int x, int y) const
    {
        return _colors_ptr[color_index(x, y)];
    }

    /**
     * @brief Returns the referenced bitmap color in the specified pixel coordinates.
     *
     * The colors are not copied but referenced, so they should outlive the direct_bitmap_item
     * to avoid dangling references.
     *
     * @param position Position of the bitmap color.
     * @return The referenced bitmap color.
     */
    [[nodiscard]] constexpr bn::color color(const point& position) const
    {
        return _colors_ptr[color_index(position)];
    }

    /**
     * @brief Returns the compression type.
     */
    [[nodiscard]] constexpr compression_type compression() const
    {
        return _compression;
    }

    /**
     * @brief Decompresses the stored data in the colors referenced by decompressed_colors_ref.
     *
     * If the source and destination colors overlap, the behavior is undefined.
     *
     * @param decompressed_colors_ref Destination of the decompressed colors.
     * @return An direct_bitmap_item pointing to the decompressed colors.
     */
    [[nodiscard]] direct_bitmap_item decompress(span<bn::color> decompressed_colors_ref) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
            const direct_bitmap_item& a, const direct_bitmap_item& b) = default;

private:
    const bn::color* _colors_ptr;
    size _dimensions;
    compression_type _compression;
};

}

#endif
