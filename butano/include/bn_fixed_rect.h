/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_RECT_H
#define BN_FIXED_RECT_H

/**
 * @file
 * bn::fixed_rect header file.
 *
 * @ingroup math
 */

#include "bn_rect.h"
#include "bn_fixed_size.h"
#include "bn_fixed_point.h"

namespace bn
{

/**
 * @brief Defines a two-dimensional rectangle using fixed point precision.
 *
 * @ingroup math
 */
class fixed_rect
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr fixed_rect() = default;

    /**
     * @brief Constructor.
     * @param position Position of the center point of the rectangle.
     * @param dimensions Size of the rectangle.
     */
    constexpr fixed_rect(const fixed_point& position, const fixed_size& dimensions) :
        _position(position),
        _dimensions(dimensions)
    {
    }

    /**
     * @brief Constructor.
     * @param x Horizontal position of the center point of the rectangle.
     * @param y Vertical position of the center point of the rectangle.
     * @param width Valid width of the rectangle (>= 0).
     * @param height Valid height of the rectangle (>= 0).
     */
    constexpr fixed_rect(fixed x, fixed y, fixed width, fixed height) :
        _position(x, y),
        _dimensions(width, height)
    {
    }

    /**
     * @brief Constructor.
     * @param rect Integer rectangle.
     */
    constexpr fixed_rect(const rect& rect) :
        _position(rect.position()),
        _dimensions(rect.dimensions())
    {
    }

    /**
     * @brief Returns the position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr const fixed_point& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the position of the center point of the rectangle.
     */
    constexpr void set_position(const fixed_point& position)
    {
        _position = position;
    }

    /**
     * @brief Sets the position of the center point of the rectangle.
     * @param x Horizontal position of the center point of the rectangle.
     * @param y Vertical position of the center point of the rectangle.
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
     * @brief Returns the horizontal position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal position of the center point of the rectangle.
     */
    constexpr void set_x(fixed x)
    {
        _position.set_x(x);
    }

    /**
     * @brief Returns the vertical position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr fixed y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the vertical position of the center point of the rectangle.
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
        return y() - (height() / 2);
    }

    /**
     * @brief Returns the position of the left boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed left() const
    {
        return x() - (width() / 2);
    }

    /**
     * @brief Returns the position of the bottom boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed bottom() const
    {
        return y() + (height() / 2);
    }

    /**
     * @brief Returns the position of the right boundary of the rectangle.
     */
    [[nodiscard]] constexpr fixed right() const
    {
        return x() + (width() / 2);
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
    [[nodiscard]] constexpr bool intersects(const fixed_rect& other) const
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
     * @brief Returns the multiplication of this rect by the given integer value.
     */
    [[nodiscard]] constexpr fixed_rect multiplication(int value) const
    {
        return fixed_rect(_position, _dimensions.multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this rect by the given fixed point value,
     * using half precision to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_rect multiplication(fixed value) const
    {
        return fixed_rect(_position, _dimensions.multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this rect by the given integer value.
     */
    [[nodiscard]] constexpr fixed_rect safe_multiplication(int value) const
    {
        return fixed_rect(_position, _dimensions.safe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this rect by the given fixed point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_rect safe_multiplication(fixed value) const
    {
        return fixed_rect(_position, _dimensions.safe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this rect by the given integer value.
     */
    [[nodiscard]] constexpr fixed_rect unsafe_multiplication(int value) const
    {
        return fixed_rect(_position, _dimensions.unsafe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this rect by the given fixed point value
     * without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_rect unsafe_multiplication(fixed value) const
    {
        return fixed_rect(_position, _dimensions.unsafe_multiplication(value));
    }

    /**
     * @brief Returns the division of this rect by the given integer value.
     */
    [[nodiscard]] constexpr fixed_rect division(int value) const
    {
        return fixed_rect(_position, _dimensions.division(value));
    }

    /**
     * @brief Returns the division of this rect by the given fixed point value,
     * using half precision to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_rect division(fixed value) const
    {
        return fixed_rect(_position, _dimensions.division(value));
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_rect safe_division(int value) const
    {
        return fixed_rect(_position, _dimensions.safe_division(value));
    }

    /**
     * @brief Returns the division of this rect by the given fixed point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_rect safe_division(fixed value) const
    {
        return fixed_rect(_position, _dimensions.safe_division(value));
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_rect unsafe_division(int value) const
    {
        return fixed_rect(_position, _dimensions.unsafe_division(value));
    }

    /**
     * @brief Returns the division of this rect by the given fixed point value
     * without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_rect unsafe_division(fixed value) const
    {
        return fixed_rect(_position, _dimensions.unsafe_division(value));
    }

    /**
     * @brief Multiplies both width and height of the rectangle by the given factor.
     * @param value Valid integer multiplication factor (>= 0).
     * @return Reference to this.
     */
    constexpr fixed_rect& operator*=(int value)
    {
        _dimensions *= value;
        return *this;
    }

    /**
     * @brief Multiplies both width and height of the rectangle by the given factor.
     * @param value Valid fixed point multiplication factor (>= 0).
     * @return Reference to this.
     */
    constexpr fixed_rect& operator*=(fixed value)
    {
        _dimensions *= value;
        return *this;
    }

    /**
     * @brief Divides both width and height of the rectangle by the given divisor.
     * @param value Valid integer divisor (> 0).
     * @return Reference to this.
     */
    constexpr fixed_rect& operator/=(int value)
    {
        _dimensions /= value;
        return *this;
    }

    /**
     * @brief Divides both width and height of the rectangle by the given divisor.
     * @param value Valid fixed point divisor (> 0).
     * @return Reference to this.
     */
    constexpr fixed_rect& operator/=(fixed value)
    {
        _dimensions /= value;
        return *this;
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_rect operator*(const fixed_rect& a, int b)
    {
        return fixed_rect(a._position, a._dimensions * b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_rect operator*(const fixed_rect& a, fixed b)
    {
        return fixed_rect(a._position, a._dimensions * b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_rect operator/(const fixed_rect& a, int b)
    {
        return fixed_rect(a._position, a._dimensions / b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_rect operator/(const fixed_rect& a, fixed b)
    {
        return fixed_rect(a._position, a._dimensions / b);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const fixed_rect& a, const fixed_rect& b) = default;

private:
    fixed_point _position;
    fixed_size _dimensions;
};


/**
 * @brief Hash support for fixed_rect.
 *
 * @ingroup math
 * @ingroup functional
 */
template<>
struct hash<fixed_rect>
{
    /**
     * @brief Returns the hash of the given fixed_rect.
     */
    [[nodiscard]] constexpr unsigned operator()(const fixed_rect& value) const
    {
        unsigned result = make_hash(value.position());
        hash_combine(value.dimensions(), result);
        return result;
    }
};

}

#endif
