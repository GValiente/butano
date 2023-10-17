/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BLENDING_FADE_ALPHA_H
#define BN_BLENDING_FADE_ALPHA_H

/**
 * @file
 * bn::blending_fade_alpha header file.
 *
 * @ingroup blending
 */

#include "bn_fixed.h"
#include "bn_assert.h"

namespace bn
{

/**
 * @brief Manages the attributes to commit to the GBA fade blending register.
 *
 * @ingroup blending
 */
class blending_fade_alpha
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr blending_fade_alpha() = default;

    /**
     * @brief Constructor.
     * @param value Fade weight in the range [0..1], with 0 being disabled and 1 being fully applied.
     */
    constexpr explicit blending_fade_alpha(fixed value) :
        _value(value)
    {
        BN_ASSERT(value >= 0 && value <= 1, "Invalid value: ", value);
    }

    /**
     * @brief Returns the weight of the fade blending, with 0 being disabled and 1 being fully applied.
     */
    [[nodiscard]] constexpr fixed value() const
    {
        return _value;
    }

    /**
     * @brief Sets the weight of the fade blending, with 0 being disabled and 1 being fully applied.
     * @param value Fade weight in the range [0..1].
     */
    constexpr void set_value(fixed value)
    {
        BN_ASSERT(value >= 0 && value <= 1, "Invalid value: ", value);

        _value = value;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
        const blending_fade_alpha& a, const blending_fade_alpha& b) = default;

private:
    fixed _value;
};

}

#endif

