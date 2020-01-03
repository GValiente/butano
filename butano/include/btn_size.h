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

    [[nodiscard]] constexpr size operator+(const size& other) const
    {
        return size(_width + other._width, _height + other._height);
    }

    [[nodiscard]] constexpr size operator-(const size& other) const
    {
        return size(_width - other._width, _height - other._height);
    }

    [[nodiscard]] constexpr bool operator==(const size& other) const
    {
        return _width == other._width && _height == other._height;
    }

    [[nodiscard]] constexpr bool operator!=(const size& other) const
    {
        return ! (*this == other);
    }

private:
    int _width = 0;
    int _height = 0;
};


template<>
struct hash<size>
{
    [[nodiscard]] constexpr size_t operator()(const size& value) const
    {
        size_t result = make_hash(value.width());
        hash_combine(value.height(), result);
        return result;
    }
};

}

#endif
