/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_H
#define BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_H

#include "btn_fixed.h"

namespace btn
{

/**
 * @brief Manages the attributes to commit to the GBA transparency and intensity blendings register.
 *
 * @ingroup blending
 */
class blending_transparency_attributes
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr blending_transparency_attributes() = default;

    /**
     * @brief Constructor.
     * @param transparency_alpha Transparency weight in the range [0..1],
     * with 0 being fully transparent and 1 being fully visible.
     * @param intensity_alpha Intensity weight in the range [0..1], with 0 being disabled and 1 being fully applied.
     */
    constexpr blending_transparency_attributes(fixed transparency_alpha, fixed intensity_alpha) :
        _transparency_alpha(transparency_alpha),
        _intensity_alpha(intensity_alpha)
    {
        BTN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
                   "Invalid transparency alpha: ", transparency_alpha);
        BTN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1, "Invalid intensity alpha: ", intensity_alpha);
    }

    /**
     * @brief Returns the weight of the transparency blending,
     * with 0 being fully transparent and 1 being fully visible.
     */
    [[nodiscard]] constexpr fixed transparency_alpha() const
    {
        return _transparency_alpha;
    }

    /**
     * @brief Sets the weight of the transparency blending,
     * with 0 being fully transparent and 1 being fully visible.
     * @param transparency_alpha Transparency weight in the range [0..1].
     */
    constexpr void set_transparency_alpha(fixed transparency_alpha)
    {
        BTN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
                   "Invalid transparency alpha: ", transparency_alpha);

        _transparency_alpha = transparency_alpha;
    }

    /**
     * @brief Returns the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
     */
    [[nodiscard]] constexpr fixed intensity_alpha() const
    {
        return _intensity_alpha;
    }

    /**
     * @brief Sets the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
     * @param intensity_alpha Intensity weight in the range [0..1].
     */
    constexpr void set_intensity_alpha(fixed intensity_alpha)
    {
        BTN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1, "Invalid intensity alpha: ", intensity_alpha);

        _intensity_alpha = intensity_alpha;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const blending_transparency_attributes& a,
                                                   const blending_transparency_attributes& b) = default;

private:
    fixed _transparency_alpha = 1;
    fixed _intensity_alpha = 0;
};

}

#endif

