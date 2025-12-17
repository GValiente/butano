/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_BITMAP_ROI_H
#define BN_PALETTE_BITMAP_ROI_H

/**
 * @file
 * bn::palette_bitmap_roi header file.
 *
 * @ingroup palette_bitmap_bg
 */

#include "bn_palette_bitmap_pixels_item.h"

namespace bn
{

/**
 * @brief Contains the required information to represent a region of interest (RoI) of a palette bitmap pixels item.
 *
 * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
 *
 * @ingroup palette_bitmap_bg
 */
class palette_bitmap_roi
{

public:
    /**
     * @brief Constructor.
     * @param item_ref Reference to the palette bitmap pixels item.
     *
     * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
     *
     * @param x Horizontal position of the top-left corner of the region of interest.
     * @param y Vertical position of the top-left corner of the region of interest.
     * @param width Width in pixels of the region of interest.
     * @param height Height in pixels of the region of interest.
     */
    constexpr palette_bitmap_roi(
            const palette_bitmap_pixels_item& item_ref, int x, int y, int width, int height) :
        palette_bitmap_roi(item_ref, point(x, y), size(width, height))
    {
    }

    /**
     * @brief Constructor.
     * @param item_ref Reference to the palette bitmap pixels item.
     *
     * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
     *
     * @param position Position of the top-left corner of the region of interest.
     * @param dimensions Size in pixels of the region of interest.
     */
    constexpr palette_bitmap_roi(
            const palette_bitmap_pixels_item& item_ref, const point& position, const size& dimensions) :
        _item_ptr(&item_ref),
        _position(position),
        _dimensions(dimensions)
    {
        BN_ASSERT(position.x() >= 0, "Invalid x: ", position.x());
        BN_ASSERT(position.y() >= 0, "Invalid y: ", position.y());
        BN_ASSERT(dimensions.width() > 0, "Invalid width: ", dimensions.width());
        BN_ASSERT(dimensions.height() > 0, "Invalid height: ", dimensions.height());
        BN_ASSERT(position.x() + dimensions.width() <= item_ref.dimensions().width(), "Invalid x or width: ",
                  position.x(), " - ", dimensions.width(), " - ", item_ref.dimensions().width());
        BN_ASSERT(position.y() + dimensions.height() <= item_ref.dimensions().height(), "Invalid y or height: ",
                  position.y(), " - ", dimensions.height(), " - ", item_ref.dimensions().height());
    }

    /**
     * @brief Returns the referenced palette bitmap pixels item.
     *
     * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
     */
    [[nodiscard]] constexpr const palette_bitmap_pixels_item& item_ref() const
    {
        return *_item_ptr;
    }

    /**
     * @brief Returns the horizontal position of the top-left corner of the region of interest.
     */
    [[nodiscard]] constexpr int x() const
    {
        return _position.x();
    }

    /**
     * @brief Returns the vertical position of the top-left corner of the region of interest.
     */
    [[nodiscard]] constexpr int y() const
    {
        return _position.y();
    }

    /**
     * @brief Returns the position of the top-left corner of the region of interest.
     */
    [[nodiscard]] constexpr const point& position() const
    {
        return _position;
    }

    /**
     * @brief Returns the width in pixels of the region of interest.
     */
    [[nodiscard]] constexpr int width() const
    {
        return _dimensions.width();
    }

    /**
     * @brief Returns the height in pixels of the region of interest.
     */
    [[nodiscard]] constexpr int height() const
    {
        return _dimensions.height();
    }

    /**
     * @brief Returns the size in pixels of the region of interest.
     */
    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    /**
     * @brief Returns the number of referenced pixels.
     */
    [[nodiscard]] constexpr int pixels_count() const
    {
        return _dimensions.width() * _dimensions.height();
    }

    /**
     * @brief Returns the index of the referenced pixel in the specified pixel coordinates.
     *
     * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
     *
     * @param x Horizontal position of the palette bitmap pixel [0..dimensions().width()).
     * @param y Vertical position of the palette bitmap pixel [0..dimensions().height()).
     * @return The index of the referenced palette bitmap pixel.
     */
    [[nodiscard]] constexpr int pixel_index(int x, int y) const
    {
        BN_ASSERT(x >= 0 && x < _dimensions.width(), "Invalid x: ", x, " - ", _dimensions.width());
        BN_ASSERT(y >= 0 && y < _dimensions.height(), "Invalid y: ", y, " - ", _dimensions.height());
        BN_BASIC_ASSERT(_item_ptr->compression() == compression_type::NONE, "Compressed pixels not supported");

        x += _position.x();
        y += _position.y();
        return (y * _item_ptr->dimensions().width()) + x;
    }

    /**
     * @brief Returns the index of the referenced pixel in the specified pixel coordinates.
     *
     * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
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
     * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
     *
     * @param x Horizontal position of the palette bitmap pixel [0..dimensions().width()).
     * @param y Vertical position of the palette bitmap pixel [0..dimensions().height()).
     * @return The referenced palette bitmap pixel.
     */
    [[nodiscard]] constexpr uint8_t pixel(int x, int y) const
    {
        return _item_ptr->pixels_ptr()[pixel_index(x, y)];
    }

    /**
     * @brief Returns the referenced palette bitmap pixel in the specified pixel coordinates.
     *
     * The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
     *
     * @param position Position of the palette bitmap pixel.
     * @return The referenced palette bitmap pixel.
     */
    [[nodiscard]] constexpr uint8_t pixel(const point& position) const
    {
        return _item_ptr->pixels_ptr()[pixel_index(position)];
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
            const palette_bitmap_roi& a, const palette_bitmap_roi& b) = default;

private:
    const palette_bitmap_pixels_item* _item_ptr;
    point _position;
    size _dimensions;
};

}

#endif
