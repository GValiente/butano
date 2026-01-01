/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_BITMAP_PIXELS_ITEM_H
#define BN_PALETTE_BITMAP_PIXELS_ITEM_H

/**
 * @file
 * bn::palette_bitmap_pixels_item header file.
 *
 * @ingroup palette_bitmap_bg
 * @ingroup tool
 */

#include "bn_size.h"
#include "bn_span.h"
#include "bn_point.h"
#include "bn_compression_type.h"

namespace bn
{

/**
 * @brief Contains the required information to generate palette bitmap pixels.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `palette_bitmap_pixels` type.
 *
 * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
 * to avoid dangling references.
 *
 * @ingroup palette_bitmap_bg
 * @ingroup tool
 */
class palette_bitmap_pixels_item
{

public:
    /**
     * @brief Constructor.
     * @param pixels_ref Reference to the palette bitmap pixels.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     *
     * @param dimensions Size in pixels of the referenced palette bitmap pixels.
     */
    constexpr palette_bitmap_pixels_item(const uint8_t& pixels_ref, const size& dimensions) :
        palette_bitmap_pixels_item(pixels_ref, dimensions, compression_type::NONE)
    {
    }

    /**
     * @brief Constructor.
     * @param pixels_ref Reference to the palette bitmap pixels.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     *
     * @param dimensions Size in pixels of the referenced palette bitmap pixels.
     * @param compression Compression type.
     */
    constexpr palette_bitmap_pixels_item(
            const uint8_t& pixels_ref, const size& dimensions, compression_type compression) :
        _pixels_ptr(&pixels_ref),
        _dimensions(dimensions),
        _compression(compression)
    {
        BN_ASSERT(dimensions.width() > 0, "Invalid width: ", dimensions.width());
        BN_ASSERT(dimensions.height() > 0, "Invalid height: ", dimensions.height());
    }

    /**
     * @brief Returns a pointer to the referenced palette bitmap pixels.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const uint8_t* pixels_ptr() const
    {
        return _pixels_ptr;
    }

    /**
     * @brief Returns the referenced palette bitmap pixels.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const uint8_t& pixels_ref() const
    {
        return *_pixels_ptr;
    }

    /**
     * @brief Returns the size in pixels of the referenced palette bitmap pixels.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    /**
     * @brief Returns the number of referenced pixels.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr int pixels_count() const
    {
        return _dimensions.width() * _dimensions.height();
    }

    /**
     * @brief Returns the index of the referenced pixel in the specified pixel coordinates.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     *
     * @param x Horizontal position of the palette bitmap pixel [0..dimensions().width()).
     * @param y Vertical position of the palette bitmap pixel [0..dimensions().height()).
     * @return The index of the referenced palette bitmap pixel.
     */
    [[nodiscard]] constexpr int pixel_index(int x, int y) const
    {
        int width = _dimensions.width();
        BN_ASSERT(x >= 0 && x < width, "Invalid x: ", x, " - ", width);
        BN_ASSERT(y >= 0 && y < _dimensions.height(), "Invalid y: ", y, " - ", _dimensions.height());
        BN_BASIC_ASSERT(_compression == compression_type::NONE, "Compressed pixels not supported");

        return (y * width) + x;
    }

    /**
     * @brief Returns the index of the referenced pixel in the specified pixel coordinates.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     *
     * @param position Position of the palette bitmap pixel.
     * @return The index of the referenced palette bitmap pixel.
     */
    [[nodiscard]] constexpr int pixel_index(const point& position) const
    {
        return pixel_index(position.x(), position.y());
    }

    /**
     * @brief Returns the referenced palette bitmap pixel in the specified pixel coordinates.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     *
     * @param x Horizontal position of the palette bitmap pixel [0..dimensions().width()).
     * @param y Vertical position of the palette bitmap pixel [0..dimensions().height()).
     * @return The referenced palette bitmap pixel.
     */
    [[nodiscard]] constexpr uint8_t pixel(int x, int y) const
    {
        return _pixels_ptr[pixel_index(x, y)];
    }

    /**
     * @brief Returns the referenced palette bitmap pixel in the specified pixel coordinates.
     *
     * The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
     * to avoid dangling references.
     *
     * @param position Position of the palette bitmap pixel.
     * @return The referenced palette bitmap pixel.
     */
    [[nodiscard]] constexpr uint8_t pixel(const point& position) const
    {
        return _pixels_ptr[pixel_index(position)];
    }

    /**
     * @brief Returns the compression type.
     */
    [[nodiscard]] constexpr compression_type compression() const
    {
        return _compression;
    }

    /**
     * @brief Decompresses the stored data in the pixels referenced by decompressed_pixels_ref.
     *
     * If the source and destination pixels overlap, the behavior is undefined.
     *
     * @param decompressed_pixels_ref Destination of the decompressed pixels.
     * @return An palette_bitmap_pixels_item pointing to the decompressed pixels.
     */
    [[nodiscard]] palette_bitmap_pixels_item decompress(span<uint8_t> decompressed_pixels_ref) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
            const palette_bitmap_pixels_item& a, const palette_bitmap_pixels_item& b) = default;

private:
    const uint8_t* _pixels_ptr;
    size _dimensions;
    compression_type _compression;
};

}

#endif
