/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DIRECT_BITMAP_ROI_H
#define BN_DIRECT_BITMAP_ROI_H

/**
 * @file
 * bn::direct_bitmap_roi header file.
 *
 * @ingroup direct_bitmap_bg
 */

#include "bn_direct_bitmap_item.h"

namespace bn
{

/**
 * @brief Contains the required information to represent a region of interest (RoI) of a direct bitmap item.
 *
 * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
 *
 * @ingroup direct_bitmap_bg
 */
class direct_bitmap_roi
{

public:
    /**
     * @brief Constructor.
     * @param item_ref Reference to the direct bitmap item.
     *
     * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
     *
     * @param x Horizontal position of the top-left corner of the region of interest.
     * @param y Vertical position of the top-left corner of the region of interest.
     * @param width Width in pixels of the region of interest.
     * @param height Height in pixels of the region of interest.
     */
    constexpr direct_bitmap_roi(const direct_bitmap_item& item_ref, int x, int y, int width, int height) :
        direct_bitmap_roi(item_ref, point(x, y), size(width, height))
    {
    }

    /**
     * @brief Constructor.
     * @param item_ref Reference to the direct bitmap item.
     *
     * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
     *
     * @param position Position of the top-left corner of the region of interest.
     * @param dimensions Size in pixels of the region of interest.
     */
    constexpr direct_bitmap_roi(const direct_bitmap_item& item_ref, const point& position, const size& dimensions) :
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
     * @brief Returns the referenced direct bitmap item.
     *
     * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
     */
    [[nodiscard]] constexpr const direct_bitmap_item& item_ref() const
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
     * @brief Returns the number of referenced colors.
     */
    [[nodiscard]] constexpr int colors_count() const
    {
        return _dimensions.width() * _dimensions.height();
    }

    /**
     * @brief Returns the index of the referenced color in the specified pixel coordinates.
     *
     * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
     *
     * @param x Horizontal position of the direct bitmap color [0..dimensions().width()).
     * @param y Vertical position of the direct bitmap color [0..dimensions().height()).
     * @return The index of the referenced direct bitmap color.
     */
    [[nodiscard]] constexpr int color_index(int x, int y) const
    {
        BN_ASSERT(x >= 0 && x < _dimensions.width(), "Invalid x: ", x, " - ", _dimensions.width());
        BN_ASSERT(y >= 0 && y < _dimensions.height(), "Invalid y: ", y, " - ", _dimensions.height());
        BN_BASIC_ASSERT(_item_ptr->compression() == compression_type::NONE, "Compressed colors not supported");

        x += _position.x();
        y += _position.y();
        return (y * _item_ptr->dimensions().width()) + x;
    }

    /**
     * @brief Returns the index of the referenced color in the specified pixel coordinates.
     *
     * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
     *
     * @param position Position of the direct bitmap color.
     * @return The index of the referenced direct bitmap color.
     */
    [[nodiscard]] constexpr int color_index(const point& position) const
    {
        return color_index(position.x(), position.y());
    }

    /**
     * @brief Returns the referenced direct bitmap color in the specified pixel coordinates.
     *
     * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
     *
     * @param x Horizontal position of the direct bitmap color [0..dimensions().width()).
     * @param y Vertical position of the direct bitmap color [0..dimensions().height()).
     * @return The referenced direct bitmap color.
     */
    [[nodiscard]] constexpr bn::color color(int x, int y) const
    {
        return _item_ptr->colors_ptr()[color_index(x, y)];
    }

    /**
     * @brief Returns the referenced direct bitmap color in the specified pixel coordinates.
     *
     * The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
     *
     * @param position Position of the direct bitmap color.
     * @return The referenced direct bitmap color.
     */
    [[nodiscard]] constexpr bn::color color(const point& position) const
    {
        return _item_ptr->colors_ptr()[color_index(position)];
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const direct_bitmap_roi& a, const direct_bitmap_roi& b) = default;

private:
    const direct_bitmap_item* _item_ptr;
    point _position;
    size _dimensions;
};

}

#endif
