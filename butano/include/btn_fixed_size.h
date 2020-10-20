/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_FIXED_SIZE_H
#define BTN_FIXED_SIZE_H

#include "btn_size.h"
#include "btn_fixed.h"

namespace btn
{

class fixed_size
{

public:
    constexpr fixed_size() = default;

    constexpr fixed_size(fixed width, fixed height) :
        _width(width),
        _height(height)
    {
        BTN_ASSERT(width >= 0, "Invalid width: ", width);
        BTN_ASSERT(height >= 0, "Invalid height: ", height);
    }

    constexpr explicit fixed_size(size size) :
        _width(size.width()),
        _height(size.height())
    {
    }

    [[nodiscard]] constexpr fixed width() const
    {
        return _width;
    }

    constexpr void set_width(fixed width)
    {
        BTN_ASSERT(width >= 0, "Invalid width: ", width);

        _width = width;
    }

    [[nodiscard]] constexpr fixed height() const
    {
        return _height;
    }

    constexpr void set_height(fixed height)
    {
        BTN_ASSERT(height >= 0, "Invalid height: ", height);

        _height = height;
    }

    constexpr fixed_size& operator+=(const fixed_size& other)
    {
        _width += other._width;
        _height += other._height;
        return *this;
    }

    constexpr fixed_size& operator-=(const fixed_size& other)
    {
        _width -= other._width;
        BTN_ASSERT(_width >= 0, "Invalid width: ", _width);

        _height -= other._height;
        BTN_ASSERT(_height >= 0, "Invalid height: ", _height);

        return *this;
    }

    constexpr fixed_size& operator*=(int value)
    {
        BTN_ASSERT(value >= 0, "Invalid value: ", value);

        _width *= value;
        _height *= value;
        return *this;
    }

    constexpr fixed_size& operator*=(fixed value)
    {
        BTN_ASSERT(value >= 0, "Invalid value: ", value);

        _width *= value;
        _height *= value;
        return *this;
    }

    constexpr fixed_size& operator/=(int value)
    {
        BTN_ASSERT(value > 0, "Invalid value: ", value);

        _width /= value;
        _height /= value;
        return *this;
    }

    constexpr fixed_size& operator/=(fixed value)
    {
        BTN_ASSERT(value > 0, "Invalid value: ", value);

        _width /= value;
        _height /= value;
        return *this;
    }

    [[nodiscard]] constexpr friend fixed_size operator+(const fixed_size& a, const fixed_size& b)
    {
        return fixed_size(a._width + b._width, a._height + b._height);
    }

    [[nodiscard]] constexpr friend fixed_size operator-(const fixed_size& a, const fixed_size& b)
    {
        return fixed_size(a._width - b._width, a._height - b._height);
    }

    [[nodiscard]] constexpr friend fixed_size operator*(const fixed_size& a, int b)
    {
        BTN_ASSERT(b >= 0, "Invalid value: ", b);

        return fixed_size(a._width * b, a._height * b);
    }

    [[nodiscard]] constexpr friend fixed_size operator*(const fixed_size& a, fixed b)
    {
        BTN_ASSERT(b >= 0, "Invalid value: ", b);

        return fixed_size(a._width * b, a._height * b);
    }

    [[nodiscard]] constexpr friend fixed_size operator/(const fixed_size& a, int b)
    {
        BTN_ASSERT(b > 0, "Invalid value: ", b);

        return fixed_size(a._width / b, a._height / b);
    }

    [[nodiscard]] constexpr friend fixed_size operator/(const fixed_size& a, fixed b)
    {
        BTN_ASSERT(b > 0, "Invalid value: ", b);

        return fixed_size(a._width / b, a._height / b);
    }

    [[nodiscard]] constexpr friend bool operator==(const fixed_size& a, const fixed_size& b) = default;

private:
    fixed _width;
    fixed _height;
};


template<>
struct hash<fixed_size>
{
    [[nodiscard]] constexpr unsigned operator()(const fixed_size& value) const
    {
        unsigned result = make_hash(value.width());
        hash_combine(value.height(), result);
        return result;
    }
};

}

#endif
