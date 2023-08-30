/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_TOP_LEFT_FIXED_RECT_H
#define BN_TOP_LEFT_FIXED_RECT_H

/**
 * @file
 * bn::top_left_fixed_rect header file.
 *
 * @ingroup math
 */

#include "bn_fixed_size.h"
#include "bn_fixed_point.h"
#include "bn_top_left_rect.h"

namespace bn
{

/**
 * @brief Defines a two-dimensional rectangle using fixed point precision and a top-left point as position.
 *
 * @ingroup math
 */
class top_left_fixed_rect
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr top_left_fixed_rect() = default;

    /**
     * @brief Constructor.
     * @param position Position of the top-left point of the rectangle.
     * @param dimensions Size of the rectangle.
     */
    constexpr top_left_fixed_rect(const fixed_point& position, const fixed_size& dimensions) :
        _position(position),
        _dimensions(dimensions)
    {
    }

    /**
     * @brief Constructor.
     * @param x Horizontal position of the top-left point of the rectangle.
     * @param y Vertical position of the top-left point of the rectangle.
     * @param width Valid width of the rectangle (>= 0).
     * @param height Valid height of the rectangle (>= 0).
     */
    constexpr top_left_fixed_rect(fixed x, fixed y, fixed width, fixed height) :
        _position(x, y),
        _dimensions(width, height)
    {
    }

    /**
     * @brief Constructor.
     * @param rect Integer rectangle.
     */
    constexpr top_left_fixed_rect(const top_left_rect& rect) :
        _position(rect.position()),
        _dimensions(rect.dimensions())
    {
    }

    /**
     * @brief Returns the position of the top-left point of the rectangle.
     */
    [[nodiscard]] constexpr const fixed_point& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the position of the top-left point of the rectangle.
     */
    constexpr void set_position(const fixed_point& position)
    {
        _position = position;
    }

    /**
     * @brief Sets the position of the top-left point of the rectangle.
     * @param x Horizontal position of the top-left point of the rectangle.
     * @param y Vertical position of the top-left point of the rectangle.
     */
    constexpr void set_position(fixed x, fixed y)
    {
        _position = fixed_point(x, y);
    }

    /**
     * @brief Returns the size of the rectangle.
     */
    [[nodiscard]] constexpr const fixed_size& dimensions() const
    {
        return _dimensions;
    }

    /**
     * @brief Sets the size of the rectangle.
     */
    constexpr void set_dimensions(const fixed_size& dimensions)
    {
        _dimensions = dimensions;
    }

    /**
     * @brief Sets the size of the rectangle.
     * @param width Valid width of the rectangle (>= 0).
     * @param height Valid height of the rectangle (>= 0).
     */
    constexpr void set_dimensions(fixed width, fixed height)
    {
        _dimensions = fixed_size(width, height);
    }

    /**
     * @brief Returns the horizontal position of the top-left point of the rectangle.
     */
    [[nodiscard]] constexpr fixed x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal position of the top-left point of the rectangle.
     */
    constexpr void set_x(fixed x)
    {
        _position.set_x(x);
    }

    /**
     * @brief Returns the vertical position of the top-left point of the rectangle.
     */
    [[nodiscard]] constexpr fixed y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the vertical position of the top-left point of the rectangle.
     */
    constexpr void set_y(fixed y)
    {
        _position.set_y(y);
    }

    /**
     * @brief Returns the width of the rectangle.
     */
    [[nodiscard]] constexpr fixed width() const
    {
        return _dimensions.width();
    }

    /**
     * @brief Sets the width of the rectangle.
     * @param width Valid width (>= 0).
     */
    constexpr void set_width(fixed width)
    {
        _dimensions.set_width(width);
    }

    /**
     * @brief Returns the height of the rectangle.
     */
    [[nodiscard]] constexpr fixed height() const
    {
        return _dimensions.height();
    }

    /**
     * @brief Sets the height of the rectangle.
     * @param height Valid height (>= 0).
     */
    constexpr void set_height(fixed height)
    {
        return _dimensions.set_height(height);
    }

    /**
     * @brief Returns the position of the top boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed top() const
    {
        return y();
    }

    /**
     * @brief Returns the position of the left boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed left() const
    {
        return x();
    }

    /**
     * @brief Returns the position of the bottom boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed bottom() const
    {
        return y() + height();
    }

    /**
     * @brief Returns the position of the right boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed right() const
    {
        return x() + width();
    }

    /**
     * @brief Returns the position of the top-left corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point top_left() const
    {
        return fixed_point(left(), top());
    }

    /**
     * @brief Returns the position of the top-right corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point top_right() const
    {
        return fixed_point(right(), top());
    }

    /**
     * @brief Returns the position of the bottom-left corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point bottom_left() const
    {
        return fixed_point(left(), bottom());
    }

    /**
     * @brief Returns the position of the bottom-right corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point bottom_right() const
    {
        return fixed_point(right(), bottom());
    }

    /**
     * @brief Returns the horizontal position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed center_x() const
    {
        return x() + (width() / 2);
    }

    /**
     * @brief Returns the vertical position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed center_y() const
    {
        return y() + (height() / 2);
    }

    /**
     * @brief Returns the position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point center() const
    {
        return fixed_point(center_x(), center_y());
    }

    /**
     * @brief Indicates if the given point is inside the rectangle or not.
     *
     * If the point is in the edge of the rectangle, it returns `false`.
     */
    [[nodiscard]] constexpr bool contains(const fixed_point& point) const
    {
        fixed this_left = left();

        if(this_left < point.x() && this_left + width() > point.x())
        {
            fixed this_top = top();
            return this_top < point.y() && this_top + height() > point.y();
        }

        return false;
    }

    /**
     * @brief Indicates if this rectangle intersects with the given one or not.
     *
     * Two rectangles intersect if there is at least one point that is within both rectangles.
     */
    [[nodiscard]] constexpr bool intersects(const top_left_fixed_rect& other) const
    {
        fixed this_left = left();
        fixed other_left = other.left();

        if(this_left < other_left + other.width() && this_left + width() > other_left)
        {
            fixed this_top = top();
            fixed other_top = other.top();
            return this_top < other_top + other.height() && height() + this_top > other_top;
        }

        return false;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
            const top_left_fixed_rect& a, const top_left_fixed_rect& b) = default;

private:
    fixed_point _position;
    fixed_size _dimensions;
};


/**
 * @brief Hash support for top_left_fixed_rect.
 *
 * @ingroup math
 * @ingroup functional
 */
template<>
struct hash<top_left_fixed_rect>
{
    /**
     * @brief Returns the hash of the given top_left_fixed_rect.
     */
    [[nodiscard]] constexpr unsigned operator()(const top_left_fixed_rect& value) const
    {
        unsigned result = make_hash(value.position());
        hash_combine(value.dimensions(), result);
        return result;
    }
};

}

#endif
