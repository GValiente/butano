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

    constexpr explicit color(int value) :
        _value(uint16_t(value))
    {
        BTN_CONSTEXPR_ASSERT(value >= 0 && value <= 0x7FFF, "Invalid value");
    }

    constexpr color(int red, int green, int blue) :
        _value(uint16_t(red + (green << 5) + (blue << 10)))
    {
        BTN_CONSTEXPR_ASSERT(red >= 0 && red <= 31, "Invalid red");
        BTN_CONSTEXPR_ASSERT(green >= 0 && green <= 31, "Invalid green");
        BTN_CONSTEXPR_ASSERT(blue >= 0 && blue <= 31, "Invalid blue");
    }

    [[nodiscard]] constexpr int value() const
    {
        return _value;
    }

    constexpr void set_value(int value)
    {
        BTN_CONSTEXPR_ASSERT(value >= 0 && value <= 0x7FFF, "Invalid value");

        _value = uint16_t(value);
    }

    constexpr void set_value(int red, int green, int blue)
    {
        BTN_CONSTEXPR_ASSERT(red >= 0 && red <= 31, "Invalid red");
        BTN_CONSTEXPR_ASSERT(green >= 0 && green <= 31, "Invalid green");
        BTN_CONSTEXPR_ASSERT(blue >= 0 && blue <= 31, "Invalid blue");

        _value = uint16_t(red + (green << 5) + (blue << 10));
    }

    [[nodiscard]] constexpr int red() const
    {
        return _value & 0x001F;
    }

    constexpr void set_red(int red)
    {
        BTN_CONSTEXPR_ASSERT(red >= 0 && red <= 31, "Invalid red");

        _value = uint16_t(red + (_value & 0x03E0) + (_value & 0x7C00));
    }

    [[nodiscard]] constexpr int green() const
    {
        return (_value & 0x03E0) >> 5;
    }

    constexpr void set_green(int green)
    {
        BTN_CONSTEXPR_ASSERT(green >= 0 && green <= 31, "Invalid green");

        _value = uint16_t((_value & 0x001F) + (green << 5) + (_value & 0x7C00));
    }

    [[nodiscard]] constexpr int blue() const
    {
        return (_value & 0x7C00) >> 10;
    }

    constexpr void set_blue(int blue)
    {
        BTN_CONSTEXPR_ASSERT(blue >= 0 && blue <= 31, "Invalid blue");

        _value = uint16_t((_value & 0x001F) + (_value & 0x03E0) + (blue << 10));
    }

    [[nodiscard]] constexpr friend bool operator==(color a, color b)
    {
        return a._value == b._value;
    }

    [[nodiscard]] constexpr friend bool operator!=(color a, color b)
    {
        return ! (a == b);
    }

private:
    uint16_t _value = 0;
};


template<>
struct hash<color>
{
    [[nodiscard]] constexpr unsigned operator()(color value) const
    {
        return make_hash(value.value());
    }
};

}

#endif
