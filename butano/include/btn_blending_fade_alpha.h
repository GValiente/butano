/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_FADE_ALPHA_H
#define BTN_BLENDING_FADE_ALPHA_H

#include "btn_fixed.h"

namespace btn
{

class blending_fade_alpha
{

public:
    constexpr blending_fade_alpha() = default;

    constexpr explicit blending_fade_alpha(fixed value) :
        _value(value)
    {
        BTN_ASSERT(value >= 0 && value <= 1, "Invalid value: ", value);
    }

    [[nodiscard]] constexpr fixed value() const
    {
        return _value;
    }

    constexpr void set_value(fixed value)
    {
        BTN_ASSERT(value >= 0 && value <= 1, "Invalid value: ", value);

        _value = value;
    }

    [[nodiscard]] constexpr friend bool operator==(const blending_fade_alpha& a, const blending_fade_alpha& b) = default;

private:
    fixed _value;
};

}

#endif

