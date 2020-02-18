#ifndef BTN_SIZE_H
#define BTN_SIZE_H

#include "btn_assert.h"
#include "btn_functional.h"

namespace btn
{

class size
{

public:
    constexpr size() = default;

    constexpr size(int width, int height) :
        _width(width),
        _height(height)
    {
        BTN_CONSTEXPR_ASSERT(width >= 0, "Invalid width");
        BTN_CONSTEXPR_ASSERT(height >= 0, "Invalid height");
    }

    [[nodiscard]] constexpr int width() const
    {
        return _width;
    }

    constexpr void set_width(int width)
    {
        BTN_CONSTEXPR_ASSERT(width >= 0, "Invalid width");

        _width = width;
    }

    [[nodiscard]] constexpr int height() const
    {
        return _height;
    }

    constexpr void set_height(int height)
    {
        BTN_CONSTEXPR_ASSERT(height >= 0, "Invalid height");

        _height = height;
    }

    constexpr size& operator+=(const size& other)
    {
        _width += other._width;
        _height += other._height;
        return *this;
    }

    constexpr size& operator-=(const size& other)
    {
        _width -= other._width;
        BTN_CONSTEXPR_ASSERT(_width >= 0, "Invalid width");

        _height -= other._height;
        BTN_CONSTEXPR_ASSERT(_height >= 0, "Invalid height");

        return *this;
    }

    constexpr size& operator*=(int value)
    {
        BTN_CONSTEXPR_ASSERT(value >= 0, "Invalid value");

        _width *= value;
        _height *= value;
        return *this;
    }

    constexpr size& operator/=(int value)
    {
        BTN_CONSTEXPR_ASSERT(value > 0, "Invalid value");

        _width /= value;
        _height /= value;
        return *this;
    }

    [[nodiscard]] constexpr friend size operator+(const size& a, const size& b)
    {
        return size(a._width + b._width, a._height + b._height);
    }

    [[nodiscard]] constexpr friend size operator-(const size& a, const size& b)
    {
        return size(a._width - b._width, a._height - b._height);
    }

    [[nodiscard]] constexpr friend size operator*(const size& a, int b)
    {
        BTN_CONSTEXPR_ASSERT(b >= 0, "Invalid value");

        return size(a._width * b, a._height * b);
    }

    [[nodiscard]] constexpr friend size operator/(const size& a, int b)
    {
        BTN_CONSTEXPR_ASSERT(b > 0, "Invalid value");

        return size(a._width / b, a._height / b);
    }

    [[nodiscard]] constexpr friend bool operator==(const size& a, const size& b)
    {
        return a._width == b._width && a._height == b._height;
    }

    [[nodiscard]] constexpr friend bool operator!=(const size& a, const size& b)
    {
        return ! (a == b);
    }

private:
    int _width = 0;
    int _height = 0;
};


template<>
struct hash<size>
{
    [[nodiscard]] constexpr unsigned operator()(const size& value) const
    {
        unsigned result = make_hash(value.width());
        hash_combine(value.height(), result);
        return result;
    }
};

}

#endif
