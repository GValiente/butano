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
        BTN_CONSTEXPR_ASSERT(red >= 0 && red <= 31, "Invalid red component");
        BTN_CONSTEXPR_ASSERT(green >= 0 && green <= 31, "Invalid green component");
        BTN_CONSTEXPR_ASSERT(blue >= 0 && blue <= 31, "Invalid blue component");
    }

    [[nodiscard]] constexpr int value() const
    {
        return _value;
    }

    [[nodiscard]] constexpr bool operator==(color other) const
    {
        return _value == other._value;
    }

    [[nodiscard]] constexpr bool operator!=(color other) const
    {
        return ! (*this == other);
    }

private:
    uint16_t _value = 0;
};


template<>
struct hash<color>
{
    [[nodiscard]] constexpr size_t operator()(color value) const
    {
        return make_hash(value.value());
    }
};

}

#endif
