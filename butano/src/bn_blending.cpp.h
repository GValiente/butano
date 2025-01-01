/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_blending.h"

#include "bn_assert.h"
#include "bn_blending_fade_alpha.h"
#include "bn_blending_transparency_attributes.h"
#include "bn_display_manager.h"

namespace bn::blending
{

fixed transparency_alpha()
{
    return display_manager::blending_transparency_alpha();
}

void set_transparency_alpha(fixed transparency_alpha)
{
    BN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
              "Invalid transparency alpha: ", transparency_alpha);
    BN_BASIC_ASSERT(transparency_alpha == 1 || ! display_manager::blending_fade_enabled(),
                    "Transparency and fade blendings can't be enabled at the same time");

    display_manager::set_blending_transparency_alpha(transparency_alpha);
}

fixed intensity_alpha()
{
    return display_manager::blending_intensity_alpha();
}

void set_intensity_alpha(fixed intensity_alpha)
{
    BN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1,
              "Invalid intensity alpha: ", intensity_alpha);
    BN_BASIC_ASSERT(intensity_alpha == 0 || ! display_manager::blending_fade_enabled(),
                    "Intensity and fade blendings can't be enabled at the same time");

    display_manager::set_blending_intensity_alpha(intensity_alpha);
}

void set_transparency_and_intensity_alpha(fixed transparency_alpha, fixed intensity_alpha)
{
    BN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1,
              "Invalid transparency alpha: ", transparency_alpha);
    BN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1,
              "Invalid intensity alpha: ", intensity_alpha);
    BN_BASIC_ASSERT((transparency_alpha == 1 && intensity_alpha == 0) || ! display_manager::blending_fade_enabled(),
                    "Transparency and fade blendings can't be enabled at the same time");

    display_manager::set_blending_transparency_and_intensity_alpha(transparency_alpha, intensity_alpha);
}

fixed transparency_top_weight()
{
    return display_manager::blending_transparency_top_weight();
}

void set_transparency_top_weight(fixed transparency_top_weight)
{
    BN_ASSERT(transparency_top_weight >= 0 && transparency_top_weight <= 1,
              "Invalid transparency top weight: ", transparency_top_weight);
    BN_BASIC_ASSERT(transparency_top_weight == 1 || ! display_manager::blending_fade_enabled(),
                    "Transparency and fade blendings can't be enabled at the same time");

    display_manager::set_blending_transparency_top_weight(transparency_top_weight);
}

fixed transparency_bottom_weight()
{
    return display_manager::blending_transparency_bottom_weight();
}

void set_transparency_bottom_weight(fixed transparency_bottom_weight)
{
    BN_ASSERT(transparency_bottom_weight >= 0 && transparency_bottom_weight <= 1,
              "Invalid transparency bottom weight: ", transparency_bottom_weight);
    BN_BASIC_ASSERT(transparency_bottom_weight == 0 || ! display_manager::blending_fade_enabled(),
                    "Transparency and fade blendings can't be enabled at the same time");

    display_manager::set_blending_transparency_bottom_weight(transparency_bottom_weight);
}

void set_transparency_weights(fixed transparency_top_weight, fixed transparency_bottom_weight)
{
    BN_ASSERT(transparency_top_weight >= 0 && transparency_top_weight <= 1,
              "Invalid transparency top weight: ", transparency_top_weight);
    BN_ASSERT(transparency_bottom_weight >= 0 && transparency_bottom_weight <= 1,
              "Invalid transparency bottom weight: ", transparency_bottom_weight);
    BN_BASIC_ASSERT(! display_manager::blending_fade_enabled() ||
                    (transparency_top_weight == 1 && transparency_bottom_weight == 0),
                    "Transparency and fade blendings can't be enabled at the same time");

    display_manager::set_blending_transparency_weights(transparency_top_weight, transparency_bottom_weight);
}

blending_transparency_attributes transparency_attributes()
{
    return blending_transparency_attributes(display_manager::blending_transparency_top_weight(),
                                            display_manager::blending_transparency_bottom_weight());
}

void set_transparency_attributes(const blending_transparency_attributes& transparency_attributes)
{
    fixed top_weight = transparency_attributes.transparency_top_weight();
    fixed bottom_weight = transparency_attributes.transparency_bottom_weight();
    BN_BASIC_ASSERT((top_weight == 1 && bottom_weight == 0) || ! display_manager::blending_fade_enabled(),
                    "Transparency and fade blendings can't be enabled at the same time");

    display_manager::set_blending_transparency_weights(top_weight, bottom_weight);
}

fade_color_type fade_color()
{
    return display_manager::blending_fade_to_black() ? fade_color_type::BLACK : fade_color_type::WHITE;
}

void set_fade_color(fade_color_type fade_color)
{
    display_manager::set_blending_fade_to_black(fade_color == fade_color_type::BLACK);
}

void set_black_fade_color()
{
    display_manager::set_blending_fade_to_black(true);
}

void set_white_fade_color()
{
    display_manager::set_blending_fade_to_black(false);
}

fixed fade_alpha()
{
    return display_manager::blending_fade_alpha();
}

void set_fade_alpha(fixed fade_alpha)
{
    BN_ASSERT(fade_alpha >= 0 && fade_alpha <= 1, "Invalid fade alpha: ", fade_alpha);

    display_manager::set_blending_fade_alpha(fade_alpha);

    BN_BASIC_ASSERT(! display_manager::blending_fade_enabled() ||
                    (display_manager::blending_transparency_top_weight() == 1 &&
                    display_manager::blending_transparency_bottom_weight() == 0),
                    "Transparency and fade blendings can't be enabled at the same time");
}

void set_fade_alpha(blending_fade_alpha fade_alpha)
{
    display_manager::set_blending_fade_alpha(fade_alpha.value());

    BN_BASIC_ASSERT(! display_manager::blending_fade_enabled() ||
                    (display_manager::blending_transparency_top_weight() == 1 &&
                    display_manager::blending_transparency_bottom_weight() == 0),
                    "Transparency and fade blendings can't be enabled at the same time");
}

void restore()
{
    display_manager::set_blending_transparency_alpha(1);
    display_manager::set_blending_intensity_alpha(0);
    display_manager::set_blending_fade_to_black(true);
    display_manager::set_blending_fade_alpha(0);
}

}
