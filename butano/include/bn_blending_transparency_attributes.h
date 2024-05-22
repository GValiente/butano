/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BLENDING_TRANSPARENCY_ATTRIBUTES_H
#define BN_BLENDING_TRANSPARENCY_ATTRIBUTES_H

/**
 * @file
 * bn::blending_transparency_attributes header file.
 *
 * @ingroup blending
 */

#include "bn_fixed.h"
#include "bn_assert.h"

namespace bn
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
     * @param transparency_top_weight Transparency weight in the range [0..1] for the top layer,
     * with 0 being fully transparent and 1 being fully visible.
     * @param transparency_bottom_weight Transparency weight in the range [0..1] for the bottom layer,
     * with 0 being fully transparent and 1 being fully visible.
     */
    constexpr blending_transparency_attributes(fixed transparency_top_weight, fixed transparency_bottom_weight) :
        _transparency_top_weight(transparency_top_weight),
        _transparency_bottom_weight(transparency_bottom_weight)
    {
        BN_ASSERT(transparency_top_weight >= 0 && transparency_top_weight <= 1,
                  "Invalid transparency top weight: ", transparency_top_weight);
        BN_ASSERT(transparency_bottom_weight >= 0 && transparency_bottom_weight <= 1,
                  "Invalid transparency bottom weight: ", transparency_bottom_weight);
    }

    /**
     * @brief Returns the weight of the transparency blending for the top layer,
     * with 0 being fully transparent and 1 being fully visible.
     */
    [[nodiscard]] constexpr fixed transparency_top_weight() const
    {
        return _transparency_top_weight;
    }

    /**
     * @brief Sets the weight of the transparency blending for the top layer,
     * with 0 being fully transparent and 1 being fully visible.
     * @param transparency_top_weight Transparency weight in the range [0..1].
     */
    constexpr void set_transparency_top_weight(fixed transparency_top_weight)
    {
        BN_ASSERT(transparency_top_weight >= 0 && transparency_top_weight <= 1,
                  "Invalid transparency top weight: ", transparency_top_weight);

        _transparency_top_weight = transparency_top_weight;
    }

    /**
     * @brief Returns the weight of the transparency blending for the bottom layer,
     * with 0 being fully transparent and 1 being fully visible.
     */
    [[nodiscard]] constexpr fixed transparency_bottom_weight() const
    {
        return _transparency_bottom_weight;
    }

    /**
     * @brief Sets the weight of the transparency blending for the bottom layer,
     * with 0 being fully transparent and 1 being fully visible.
     * @param transparency_bottom_weight Transparency weight in the range [0..1].
     */
    constexpr void set_transparency_bottom_weight(fixed transparency_bottom_weight)
    {
        BN_ASSERT(transparency_bottom_weight >= 0 && transparency_bottom_weight <= 1,
                  "Invalid transparency bottom weight: ", transparency_bottom_weight);

        _transparency_bottom_weight = transparency_bottom_weight;
    }

    /**
     * @brief Sets the weight of the transparency blending,
     * with 0 being fully transparent and 1 being fully visible.
     * @param transparency_alpha Transparency weight in the range [0..1].
     */
    constexpr void set_transparency_alpha(fixed transparency_alpha)
    {
        BN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
                  "Invalid transparency alpha: ", transparency_alpha);

        _transparency_top_weight = transparency_alpha;
        _transparency_bottom_weight = 1 - transparency_alpha;
    }

    /**
     * @brief Sets the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
     * @param intensity_alpha Intensity weight in the range [0..1].
     */
    constexpr void set_intensity_alpha(fixed intensity_alpha)
    {
        BN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1,
                  "Invalid intensity alpha: ", intensity_alpha);

        _transparency_top_weight = 1;
        _transparency_bottom_weight = intensity_alpha;
    }

    /**
     * @brief Sets the weights of the transparency and the the intensity blendings.
     * @param transparency_alpha Transparency weight in the range [0..1],
     * with 0 being fully transparent and 1 being fully visible.
     * @param intensity_alpha Intensity weight in the range [0..1],
     * with 0 being disabled and 1 being fully applied.
     */
    constexpr void set_transparency_and_intensity_alphas(fixed transparency_alpha, fixed intensity_alpha)
    {
        BN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
                  "Invalid transparency alpha: ", transparency_alpha);
        BN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1,
                  "Invalid intensity alpha: ", intensity_alpha);

        _transparency_top_weight = transparency_alpha;
        _transparency_bottom_weight = max(1 - transparency_alpha, intensity_alpha);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const blending_transparency_attributes& a,
                                                   const blending_transparency_attributes& b) = default;

private:
    fixed _transparency_top_weight = 1;
    fixed _transparency_bottom_weight = 0;
};

}

#endif

