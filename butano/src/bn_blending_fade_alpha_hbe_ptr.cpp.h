/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_blending_fade_alpha_hbe_ptr.h"

#include "bn_display.h"
#include "bn_display_manager.h"
#include "bn_blending_fade_alpha.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

blending_fade_alpha_hbe_ptr blending_fade_alpha_hbe_ptr::create(const span<const blending_fade_alpha>& alphas_ref)
{
    BN_BASIC_ASSERT(display_manager::blending_transparency_top_weight() == 1 &&
                    display_manager::blending_transparency_bottom_weight() == 0,
                    "Transparency and fade blendings can't be enabled at the same time");

    int id = hblank_effects_manager::create(alphas_ref.data(), alphas_ref.size(), 0,
                                            hblank_effects_manager::handler_type::BLENDING_FADE_ALPHA);
    return blending_fade_alpha_hbe_ptr(id);
}

optional<blending_fade_alpha_hbe_ptr> blending_fade_alpha_hbe_ptr::create_optional(
        const span<const blending_fade_alpha>& alphas_ref)
{
    BN_BASIC_ASSERT(display_manager::blending_transparency_top_weight() == 1 &&
                    display_manager::blending_transparency_bottom_weight() == 0,
                    "Transparency and fade blendings can't be enabled at the same time");

    int id = hblank_effects_manager::create_optional(alphas_ref.data(), alphas_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::BLENDING_FADE_ALPHA);
    optional<blending_fade_alpha_hbe_ptr> result;

    if(id >= 0)
    {
        result = blending_fade_alpha_hbe_ptr(id);
    }

    return result;
}

span<const blending_fade_alpha> blending_fade_alpha_hbe_ptr::alphas_ref() const
{
    auto alphas_ptr = reinterpret_cast<const blending_fade_alpha*>(hblank_effects_manager::values_ref(id()));
    return span<const blending_fade_alpha>(alphas_ptr, display::height());
}

void blending_fade_alpha_hbe_ptr::set_alphas_ref(const span<const blending_fade_alpha>& alphas_ref)
{
    hblank_effects_manager::set_values_ref(id(), alphas_ref.data(), alphas_ref.size());
}

void blending_fade_alpha_hbe_ptr::reload_alphas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

blending_fade_alpha_hbe_ptr::blending_fade_alpha_hbe_ptr(int id) :
    hbe_ptr(id)
{
}

}
