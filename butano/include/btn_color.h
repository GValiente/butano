/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_COLOR_H
#define BTN_COLOR_H

#include "btn_assert.h"
#include "btn_functional.h"

namespace btn
{

/**
 * @brief Defines a color in the GBA format (15 bits BGR).
 *
 * @ingroup color
 */
class color
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr color() = default;

    /**
     * @brief Constructor.
     * @param data Color value in GBA format.
     */
    constexpr explicit color(int data) :
        _data(uint16_t(data))
    {
        BTN_ASSERT(data >= 0 && data <= 0x7FFF, "Invalid data: ", data);
    }

    /**
     * @brief Constructor.
     * @param red Red component, in the range [0..31].
     * @param green Green component, in the range [0..31].
     * @param blue Blue component, in the range [0..31].
     */
    constexpr color(int red, int green, int blue) :
        _data(uint16_t(red + (green << 5) + (blue << 10)))
    {
        BTN_ASSERT(red >= 0 && red <= 31, "Invalid red: ", red);
        BTN_ASSERT(green >= 0 && green <= 31, "Invalid green: ", green);
        BTN_ASSERT(blue >= 0 && blue <= 31, "Invalid blue: ", blue);
    }

    /**
     * @brief Returns the representation of this color in GBA format.
     */
    [[nodiscard]] constexpr int data() const
    {
        return _data;
    }

    /**
     * @brief Sets the representation of this color in GBA format.
     */
    constexpr void set_data(int data)
    {
        BTN_ASSERT(data >= 0 && data <= 0x7FFF, "Invalid data: ", data);

        _data = uint16_t(data);
    }

    /**
     * @brief Sets all the components of this color.
     * @param red Red component, in the range [0..31].
     * @param green Green component, in the range [0..31].
     * @param blue Blue component, in the range [0..31].
     */
    constexpr void set_components(int red, int green, int blue)
    {
        BTN_ASSERT(red >= 0 && red <= 31, "Invalid red: ", red);
        BTN_ASSERT(green >= 0 && green <= 31, "Invalid green: ", green);
        BTN_ASSERT(blue >= 0 && blue <= 31, "Invalid blue: ", blue);

        _data = uint16_t(red + (green << 5) + (blue << 10));
    }

    /**
     * @brief Returns the red component.
     */
    [[nodiscard]] constexpr int red() const
    {
        return _data & 0x001F;
    }

    /**
     * @brief Sets the red component.
     * @param red Red component, in the range [0..31].
     */
    constexpr void set_red(int red)
    {
        BTN_ASSERT(red >= 0 && red <= 31, "Invalid red: ", red);

        _data = uint16_t(red + (_data & 0x03E0) + (_data & 0x7C00));
    }

    /**
     * @brief Returns the green component.
     */
    [[nodiscard]] constexpr int green() const
    {
        return (_data & 0x03E0) >> 5;
    }

    /**
     * @brief Sets the green component.
     * @param green Green component, in the range [0..31].
     */
    constexpr void set_green(int green)
    {
        BTN_ASSERT(green >= 0 && green <= 31, "Invalid green: ", green);

        _data = uint16_t((_data & 0x001F) + (green << 5) + (_data & 0x7C00));
    }

    /**
     * @brief Returns the blue component.
     */
    [[nodiscard]] constexpr int blue() const
    {
        return (_data & 0x7C00) >> 10;
    }

    /**
     * @brief Sets the blue component.
     * @param blue Blue component, in the range [0..31].
     */
    constexpr void set_blue(int blue)
    {
        BTN_ASSERT(blue >= 0 && blue <= 31, "Invalid blue: ", blue);

        _data = uint16_t((_data & 0x001F) + (_data & 0x03E0) + (blue << 10));
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(color a, color b) = default;

private:
    uint16_t _data = 0;
};


/**
 * @brief Hash support for color.
 *
 * @ingroup color
 * @ingroup functional
 */
template<>
struct hash<color>
{
    /**
     * @brief Returns the hash of the given color.
     */
    [[nodiscard]] constexpr unsigned operator()(color value) const
    {
        return make_hash(value.data());
    }
};

}

#endif
