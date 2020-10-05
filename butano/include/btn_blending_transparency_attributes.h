#ifndef BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_H
#define BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_H

#include "btn_fixed.h"

namespace btn
{

class blending_transparency_attributes
{

public:
    constexpr blending_transparency_attributes() = default;

    constexpr blending_transparency_attributes(fixed transparency_alpha, fixed intensity_alpha) :
        _transparency_alpha(transparency_alpha),
        _intensity_alpha(intensity_alpha)
    {
        BTN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
                   "Invalid transparency alpha: ", transparency_alpha);
        BTN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1, "Invalid intensity alpha: ", intensity_alpha);
    }

    [[nodiscard]] constexpr fixed transparency_alpha() const
    {
        return _transparency_alpha;
    }

    constexpr void set_transparency_alpha(fixed transparency_alpha)
    {
        BTN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
                   "Invalid transparency alpha: ", transparency_alpha);

        _transparency_alpha = transparency_alpha;
    }

    [[nodiscard]] constexpr fixed intensity_alpha() const
    {
        return _intensity_alpha;
    }

    constexpr void set_intensity_alpha(fixed intensity_alpha)
    {
        BTN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1, "Invalid intensity alpha: ", intensity_alpha);

        _intensity_alpha = intensity_alpha;
    }

    [[nodiscard]] constexpr friend bool operator==(const blending_transparency_attributes& a,
                                                   const blending_transparency_attributes& b) = default;

private:
    fixed _transparency_alpha = 1;
    fixed _intensity_alpha = 0;
};

}

#endif

