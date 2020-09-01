#ifndef BTN_COLOR_H
#define BTN_COLOR_H

#include "btn_assert.h"
#include "btn_functional.h"

namespace btn
{

class color
{

public:
    constexpr color() = default;

    constexpr explicit color(int data) :
        _data(uint16_t(data))
    {
        BTN_CONSTEXPR_ASSERT(data >= 0 && data <= 0x7FFF, "Invalid data");
    }

    constexpr color(int red, int green, int blue) :
        _data(uint16_t(red + (green << 5) + (blue << 10)))
    {
        BTN_CONSTEXPR_ASSERT(red >= 0 && red <= 31, "Invalid red");
        BTN_CONSTEXPR_ASSERT(green >= 0 && green <= 31, "Invalid green");
        BTN_CONSTEXPR_ASSERT(blue >= 0 && blue <= 31, "Invalid blue");
    }

    [[nodiscard]] constexpr int data() const
    {
        return _data;
    }

    constexpr void set_data(int data)
    {
        BTN_CONSTEXPR_ASSERT(data >= 0 && data <= 0x7FFF, "Invalid data");

        _data = uint16_t(data);
    }

    constexpr void set_components(int red, int green, int blue)
    {
        BTN_CONSTEXPR_ASSERT(red >= 0 && red <= 31, "Invalid red");
        BTN_CONSTEXPR_ASSERT(green >= 0 && green <= 31, "Invalid green");
        BTN_CONSTEXPR_ASSERT(blue >= 0 && blue <= 31, "Invalid blue");

        _data = uint16_t(red + (green << 5) + (blue << 10));
    }

    [[nodiscard]] constexpr int red() const
    {
        return _data & 0x001F;
    }

    constexpr void set_red(int red)
    {
        BTN_CONSTEXPR_ASSERT(red >= 0 && red <= 31, "Invalid red");

        _data = uint16_t(red + (_data & 0x03E0) + (_data & 0x7C00));
    }

    [[nodiscard]] constexpr int green() const
    {
        return (_data & 0x03E0) >> 5;
    }

    constexpr void set_green(int green)
    {
        BTN_CONSTEXPR_ASSERT(green >= 0 && green <= 31, "Invalid green");

        _data = uint16_t((_data & 0x001F) + (green << 5) + (_data & 0x7C00));
    }

    [[nodiscard]] constexpr int blue() const
    {
        return (_data & 0x7C00) >> 10;
    }

    constexpr void set_blue(int blue)
    {
        BTN_CONSTEXPR_ASSERT(blue >= 0 && blue <= 31, "Invalid blue");

        _data = uint16_t((_data & 0x001F) + (_data & 0x03E0) + (blue << 10));
    }

    [[nodiscard]] constexpr friend bool operator==(color a, color b) = default;

private:
    uint16_t _data = 0;
};


template<>
struct hash<color>
{
    [[nodiscard]] constexpr unsigned operator()(color value) const
    {
        return make_hash(value.data());
    }
};

}

#endif
