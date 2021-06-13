/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RECT_H
#define BN_RECT_H

/**
 * @file
 * bn::rect header file.
 *
 * @ingroup math
 */

#include "bn_size.h"
#include "bn_point.h"

namespace bn
{

/**
 * @brief Defines a two-dimensional rectangle using integer precision.
 *
 * @ingroup math
 */
class rect
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr rect() = default;

    /**
     * @brief Constructor.
     * @param position Position of the center point of the rectangle.
     * @param dimensions Size of the rectangle.
     */
    constexpr rect(const point& position, const size& dimensions) :
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
    constexpr rect(int x, int y, int width, int height) :
        _position(x, y),
        _dimensions(width, height)
    {
    }

    /**
     * @brief Returns the position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr const point& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the position of the center point of the rectangle.
     */
    constexpr void set_position(const point& position)
    {
        _position = position;
    }

    /**
     * @brief Sets the position of the center point of the rectangle.
     * @param x Horizontal position of the center point of the rectangle.
     * @param y Vertical position of the center point of the rectangle.
     */
    constexpr void set_position(int x, int y)
    {
        _position = bn::point(x, y);
    }

    /**
     * @brief Returns the size of the rectangle.
     */
    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    /**
     * @brief Sets the size of the rectangle.
     */
    constexpr void set_dimensions(const size& dimensions)
    {
        _dimensions = dimensions;
    }

    /**
     * @brief Sets the size of the rectangle.
     * @param width Valid width of the rectangle (>= 0).
     * @param height Valid height of the rectangle (>= 0).
     */
    constexpr void set_dimensions(int width, int height)
    {
        _dimensions = bn::size(width, height);
    }

    /**
     * @brief Returns the horizontal position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr int x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal position of the center point of the rectangle.
     */
    constexpr void set_x(int x)
    {
        _position.set_x(x);
    }

    /**
     * @brief Returns the vertical position of the center point of the rectangle.
     */
    [[nodiscard]] constexpr int y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the vertical position of the center point of the rectangle.
     */
    constexpr void set_y(int y)
    {
        _position.set_y(y);
    }

    /**
     * @brief Returns the width of the rectangle.
     */
    [[nodiscard]] constexpr int width() const
    {
        return _dimensions.width();
    }

    /**
     * @brief Sets the width of the rectangle.
     * @param width Valid width (>= 0).
     */
    constexpr void set_width(int width)
    {
        _dimensions.set_width(width);
    }

    /**
     * @brief Returns the height of the rectangle.
     */
    [[nodiscard]] constexpr int height() const
    {
        return _dimensions.height();
    }

    /**
     * @brief Sets the height of the rectangle.
     * @param height Valid height (>= 0).
     */
    constexpr void set_height(int height)
    {
        return _dimensions.set_height(height);
    }

    /**
     * @brief Returns the position of the top boundary of the rectangle.
     */
    [[nodiscard]] constexpr int top() const
    {
        return y() - (height() / 2);
    }

    /**
     * @brief Returns the position of the left boundary of the rectangle.
     */
    [[nodiscard]] constexpr int left() const
    {
        return x() - (width() / 2);
    }

    /**
     * @brief Returns the position of the top-left of the rectangle.
     */
    [[nodiscard]] constexpr point top_left() const
    {
        return point(left(), top());
    }

    /**
     * @brief Returns the position of the bottom boundary of the rectangle.
     */
    [[nodiscard]] constexpr int bottom() const
    {
        return y() + (height() / 2);
    }

    /**
     * @brief Returns the position of the right boundary of the rectangle.
     */
    [[nodiscard]] constexpr int right() const
    {
        return x() + (width() / 2);
    }

    /**
     * @brief Returns the position of the bottom-right of the rectangle.
     */
    [[nodiscard]] constexpr point bottom_right() const
    {
        return point(right(), bottom());
    }

    /**
     * @brief Returns the position of the top-right of the rectangle.
     */
    [[nodiscard]] constexpr point top_right() const
    {
        return point(right(), top());
    }

    /**
     * @brief Returns the position of the bottom-left of the rectangle.
     */
    [[nodiscard]] constexpr point bottom_left() const
    {
        return point(left(), bottom());
    }

    /**
     * @brief Indicates if the given point is inside the rectangle or not.
     *
     * If the point is in the edge of the rectangle, it returns `false`.
     */
    [[nodiscard]] constexpr bool contains(const point& point) const
    {
        int this_left = left();

        if(this_left < point.x() && this_left + width() > point.x())
        {
            int this_top = top();
            return this_top < point.y() && this_top + height() > point.y();
        }

        return false;
    }

    /**
     * @brief Indicates if this rectangle intersects with the given one or not.
     *
     * Two rectangles intersect if there is at least one point that is within both rectangles.
     */
    [[nodiscard]] constexpr bool intersects(const rect& other) const
    {
        int this_left = left();
        int other_left = other.left();

        if(this_left < other_left + other.width() && this_left + width() > other_left)
        {
            int this_top = top();
            int other_top = other.top();
            return this_top < other_top + other.height() && height() + this_top > other_top;
        }

        return false;
    }

    /**
     * @brief Multiplies both width and height of the rectangle by the given factor.
     * @param value Integer multiplication factor.
     * @return Reference to this.
     */
    constexpr rect& operator*=(int value)
    {
        _dimensions *= value;
        return *this;
    }

    /**
     * @brief Multiplies both width and height of the rectangle by the given factor.
     * @param value Unsigned integer multiplication factor.
     * @return Reference to this.
     */
    constexpr rect& operator*=(unsigned value)
    {
        _dimensions *= value;
        return *this;
    }

    /**
     * @brief Divides both width and height of the rectangle by the given divisor.
     * @param value Valid integer divisor (> 0).
     * @return Reference to this.
     */
    constexpr rect& operator/=(int value)
    {
        _dimensions /= value;
        return *this;
    }

    /**
     * @brief Divides both width and height of the rectangle by the given divisor.
     * @param value Valid unsigned integer divisor (> 0).
     * @return Reference to this.
     */
    constexpr rect& operator/=(unsigned value)
    {
        _dimensions /= value;
        return *this;
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend rect operator*(const rect& a, int b)
    {
        return rect(a._position, a._dimensions * b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend rect operator*(const rect& a, unsigned b)
    {
        return rect(a._position, a._dimensions * b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend rect operator/(const rect& a, int b)
    {
        return rect(a._position, a._dimensions / b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend rect operator/(const rect& a, unsigned b)
    {
        return rect(a._position, a._dimensions / b);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const rect& a, const rect& b) = default;

private:
    point _position;
    size _dimensions;
};


/**
 * @brief Hash support for rect.
 *
 * @ingroup math
 * @ingroup functional
 */
template<>
struct hash<rect>
{
    /**
     * @brief Returns the hash of the given rect.
     */
    [[nodiscard]] constexpr unsigned operator()(const rect& value) const
    {
        unsigned result = make_hash(value.position());
        hash_combine(value.dimensions(), result);
        return result;
    }
};

}

#endif
