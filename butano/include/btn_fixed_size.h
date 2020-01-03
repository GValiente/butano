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
        BTN_CONSTEXPR_ASSERT(width >= 0, "Invalid width");
        BTN_CONSTEXPR_ASSERT(height >= 0, "Invalid height");
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
        BTN_CONSTEXPR_ASSERT(width >= 0, "Invalid width");

        _width = width;
    }

    [[nodiscard]] constexpr fixed height() const
    {
        return _height;
    }

    constexpr void set_height(fixed height)
    {
        BTN_CONSTEXPR_ASSERT(height >= 0, "Invalid height");

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
        BTN_CONSTEXPR_ASSERT(_width >= 0, "Invalid width");

        _height -= other._height;
        BTN_CONSTEXPR_ASSERT(_height >= 0, "Invalid height");

        return *this;
    }

    constexpr fixed_size& operator*=(int value)
    {
        BTN_CONSTEXPR_ASSERT(value >= 0, "Invalid value");

        _width *= value;
        _height *= value;
        return *this;
    }

    constexpr fixed_size& operator*=(fixed value)
    {
        BTN_CONSTEXPR_ASSERT(value >= 0, "Invalid value");

        _width *= value;
        _height *= value;
        return *this;
    }

    constexpr fixed_size& operator/=(int value)
    {
        BTN_CONSTEXPR_ASSERT(value > 0, "Invalid value");

        _width /= value;
        _height /= value;
        return *this;
    }

    constexpr fixed_size& operator/=(fixed value)
    {
        BTN_CONSTEXPR_ASSERT(value > 0, "Invalid value");

        _width /= value;
        _height /= value;
        return *this;
    }

    [[nodiscard]] constexpr fixed_size operator+(const fixed_size& other) const
    {
        return fixed_size(_width + other._width, _height + other._height);
    }

    [[nodiscard]] constexpr fixed_size operator-(const fixed_size& other) const
    {
        return fixed_size(_width - other._width, _height - other._height);
    }

    [[nodiscard]] constexpr fixed_size operator*(int value) const
    {
        BTN_CONSTEXPR_ASSERT(value >= 0, "Invalid value");

        return fixed_size(_width * value, _height * value);
    }

    [[nodiscard]] constexpr fixed_size operator*(fixed value) const
    {
        BTN_CONSTEXPR_ASSERT(value >= 0, "Invalid value");

        return fixed_size(_width * value, _height * value);
    }

    [[nodiscard]] constexpr fixed_size operator/(int value) const
    {
        BTN_CONSTEXPR_ASSERT(value > 0, "Invalid value");

        return fixed_size(_width / value, _height / value);
    }

    [[nodiscard]] constexpr fixed_size operator/(fixed value) const
    {
        BTN_CONSTEXPR_ASSERT(value > 0, "Invalid value");

        return fixed_size(_width / value, _height / value);
    }

    [[nodiscard]] constexpr bool operator==(const fixed_size& other) const
    {
        return _width == other._width && _height == other._height;
    }

    [[nodiscard]] constexpr bool operator!=(const fixed_size& other) const
    {
        return ! (*this == other);
    }

private:
    fixed _width;
    fixed _height;
};


template<>
struct hash<fixed_size>
{
    [[nodiscard]] constexpr size_t operator()(const fixed_size& value) const
    {
        size_t result = make_hash(value.width());
        hash_combine(value.height(), result);
        return result;
    }
};

}

#endif
