/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_TOP_LEFT_FIXED_RECT_H
#define BN_TOP_LEFT_FIXED_RECT_H

/**
 * @file
 * bn::top_left_fixed_rect_t and bn::top_left_fixed_rect implementation header file.
 *
 * @ingroup math
 */

#include "bn_fixed_size.h"
#include "bn_fixed_point.h"
#include "bn_top_left_rect.h"
#include "bn_top_left_fixed_rect_fwd.h"

namespace bn
{

template<int Precision>
class top_left_fixed_rect_t
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr top_left_fixed_rect_t() = default;

    /**
     * @brief Constructor.
     * @param position Position of the top-left point of the rectangle.
     * @param dimensions Size of the rectangle.
     */
    constexpr top_left_fixed_rect_t(
            const fixed_point_t<Precision>& position, const fixed_size_t<Precision>& dimensions) :
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
    constexpr top_left_fixed_rect_t(
            fixed_t<Precision> x, fixed_t<Precision> y, fixed_t<Precision> width, fixed_t<Precision> height) :
        _position(x, y),
        _dimensions(width, height)
    {
    }

    /**
     * @brief Constructor.
     * @param rect Integer rectangle.
     */
    constexpr top_left_fixed_rect_t(const top_left_rect& rect) :
        _position(rect.position()),
        _dimensions(rect.dimensions())
    {
    }

    /**
     * @brief Returns the position of the top-left point of the rectangle.
     */
    [[nodiscard]] constexpr const fixed_point_t<Precision>& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the position of the top-left point of the rectangle.
     */
    constexpr void set_position(const fixed_point_t<Precision>& position)
    {
        _position = position;
    }

    /**
     * @brief Sets the position of the top-left point of the rectangle.
     * @param x Horizontal position of the top-left point of the rectangle.
     * @param y Vertical position of the top-left point of the rectangle.
     */
    constexpr void set_position(fixed_t<Precision> x, fixed_t<Precision> y)
    {
        _position = fixed_point_t<Precision>(x, y);
    }

    /**
     * @brief Returns the size of the rectangle.
     */
    [[nodiscard]] constexpr const fixed_size_t<Precision>& dimensions() const
    {
        return _dimensions;
    }

    /**
     * @brief Sets the size of the rectangle.
     */
    constexpr void set_dimensions(const fixed_size_t<Precision>& dimensions)
    {
        _dimensions = dimensions;
    }

    /**
     * @brief Sets the size of the rectangle.
     * @param width Valid width of the rectangle (>= 0).
     * @param height Valid height of the rectangle (>= 0).
     */
    constexpr void set_dimensions(fixed_t<Precision> width, fixed_t<Precision> height)
    {
        _dimensions = fixed_size_t<Precision>(width, height);
    }

    /**
     * @brief Returns the horizontal position of the top-left point of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal position of the top-left point of the rectangle.
     */
    constexpr void set_x(fixed_t<Precision> x)
    {
        _position.set_x(x);
    }

    /**
     * @brief Returns the vertical position of the top-left point of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the vertical position of the top-left point of the rectangle.
     */
    constexpr void set_y(fixed_t<Precision> y)
    {
        _position.set_y(y);
    }

    /**
     * @brief Returns the width of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> width() const
    {
        return _dimensions.width();
    }

    /**
     * @brief Sets the width of the rectangle.
     * @param width Valid width (>= 0).
     */
    constexpr void set_width(fixed_t<Precision> width)
    {
        _dimensions.set_width(width);
    }

    /**
     * @brief Returns the height of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> height() const
    {
        return _dimensions.height();
    }

    /**
     * @brief Sets the height of the rectangle.
     * @param height Valid height (>= 0).
     */
    constexpr void set_height(fixed_t<Precision> height)
    {
        return _dimensions.set_height(height);
    }

    /**
     * @brief Returns the position of the top boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> top() const
    {
        return y();
    }

    /**
     * @brief Returns the position of the left boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> left() const
    {
        return x();
    }

    /**
     * @brief Returns the position of the bottom boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> bottom() const
    {
        return y() + height();
    }

    /**
     * @brief Returns the position of the right boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> right() const
    {
        return x() + width();
    }

    /**
     * @brief Returns the position of the top-left corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> top_left() const
    {
        return fixed_point_t<Precision>(left(), top());
    }

    /**
     * @brief Returns the position of the top-right corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> top_right() const
    {
        return fixed_point_t<Precision>(right(), top());
    }

    /**
     * @brief Returns the position of the bottom-left corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> bottom_left() const
    {
        return fixed_point_t<Precision>(left(), bottom());
    }

    /**
     * @brief Returns the position of the bottom-right corner of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> bottom_right() const
    {
        return fixed_point_t<Precision>(right(), bottom());
    }

    /**
     * @brief Returns the horizontal position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> center_x() const
    {
        return x() + (width() / 2);
    }

    /**
     * @brief Returns the vertical position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed_t<Precision> center_y() const
    {
        return y() + (height() / 2);
    }

    /**
     * @brief Returns the position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> center() const
    {
        return fixed_point_t<Precision>(center_x(), center_y());
    }

    /**
     * @brief Indicates if the given point is inside the rectangle or not.
     *
     * If the point is on the edge of the rectangle, it returns `false`.
     */
    [[nodiscard]] constexpr bool contains(const fixed_point_t<Precision>& point) const
    {
        fixed_t<Precision> this_left = left();

        if(this_left < point.x() && this_left + width() > point.x())
        {
            fixed_t<Precision> this_top = top();
            return this_top < point.y() && this_top + height() > point.y();
        }

        return false;
    }

    /**
     * @brief Indicates if the given point is inside the rectangle or not.
     *
     * If the point is on the edge of the rectangle, it returns `true`.
     */
    [[nodiscard]] constexpr bool edge_contains(const fixed_point_t<Precision>& point) const
    {
        fixed_t<Precision> this_left = left();

        if(this_left <= point.x() && this_left + width() >= point.x())
        {
            fixed_t<Precision> this_top = top();
            return this_top <= point.y() && this_top + height() >= point.y();
        }

        return false;
    }

    /**
     * @brief Indicates if the given rectangle is inside this one or not.
     *
     * If the given rectangle is on the edge of this one, it returns `false`.
     */
    [[nodiscard]] constexpr bool contains(const top_left_fixed_rect_t& other) const
    {
        return left() < other.left() && right() > other.right() &&
               top() < other.top() && bottom() > other.bottom();
    }

    /**
     * @brief Indicates if the given rectangle is inside this one or not.
     *
     * If the given rectangle is on the edge of this one, it returns `true`.
     */
    [[nodiscard]] constexpr bool edge_contains(const top_left_fixed_rect_t& other) const
    {
        return left() <= other.left() && right() >= other.right() &&
               top() <= other.top() && bottom() >= other.bottom();
    }

    /**
     * @brief Indicates if this rectangle intersects with the given one or not.
     *
     * Two rectangles intersect if there is at least one point that is within both rectangles,
     * excluding their edges.
     */
    [[nodiscard]] constexpr bool intersects(const top_left_fixed_rect_t& other) const
    {
        fixed_t<Precision> this_left = left();
        fixed_t<Precision> other_left = other.left();

        if(this_left < other_left + other.width() && this_left + width() > other_left)
        {
            fixed_t<Precision> this_top = top();
            fixed_t<Precision> other_top = other.top();
            return this_top < other_top + other.height() && height() + this_top > other_top;
        }

        return false;
    }

    /**
     * @brief Indicates if this rectangle touches the given one or not.
     *
     * Two rectangles touch each other if there is at least one point that is within both rectangles,
     * including their edges.
     */
    [[nodiscard]] constexpr bool touches(const top_left_fixed_rect_t& other) const
    {
        fixed_t<Precision> this_left = left();
        fixed_t<Precision> other_left = other.left();

        if(this_left <= other_left + other.width() && this_left + width() >= other_left)
        {
            fixed_t<Precision> this_top = top();
            fixed_t<Precision> other_top = other.top();
            return this_top <= other_top + other.height() && height() + this_top >= other_top;
        }

        return false;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
            const top_left_fixed_rect_t& a, const top_left_fixed_rect_t& b) = default;

private:
    fixed_point_t<Precision> _position;
    fixed_size_t<Precision> _dimensions;
};


/**
 * @brief Hash support for top_left_fixed_rect_t.
 *
 * @ingroup math
 * @ingroup functional
 */
template<int Precision>
struct hash<top_left_fixed_rect_t<Precision>>
{
    /**
     * @brief Returns the hash of the given top_left_fixed_rect_t.
     */
    [[nodiscard]] constexpr unsigned operator()(const top_left_fixed_rect_t<Precision> value) const
    {
        unsigned result = make_hash(value.position());
        hash_combine(value.dimensions(), result);
        return result;
    }
};

}

#endif
