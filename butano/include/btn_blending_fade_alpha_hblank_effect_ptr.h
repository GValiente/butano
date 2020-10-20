/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_FADE_ALPHA_HBLANK_EFFECT_PTR_H
#define BTN_BLENDING_FADE_ALPHA_HBLANK_EFFECT_PTR_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class blending_fade_alpha;

class blending_fade_alpha_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static blending_fade_alpha_hblank_effect_ptr create(
            const span<const blending_fade_alpha>& values_ref);

    [[nodiscard]] static optional<blending_fade_alpha_hblank_effect_ptr> create_optional(
            const span<const blending_fade_alpha>& values_ref);

    blending_fade_alpha_hblank_effect_ptr(const blending_fade_alpha_hblank_effect_ptr& other) = default;

    blending_fade_alpha_hblank_effect_ptr& operator=(const blending_fade_alpha_hblank_effect_ptr& other) = default;

    blending_fade_alpha_hblank_effect_ptr(blending_fade_alpha_hblank_effect_ptr&& other) = default;

    blending_fade_alpha_hblank_effect_ptr& operator=(blending_fade_alpha_hblank_effect_ptr&& other) = default;

    [[nodiscard]] span<const blending_fade_alpha> values_ref() const;

    void set_values_ref(const span<const blending_fade_alpha>& values_ref);

    void reload_values_ref();

    void swap(blending_fade_alpha_hblank_effect_ptr& other)
    {
        hblank_effect_ptr::swap(other);
    }

    friend void swap(blending_fade_alpha_hblank_effect_ptr& a, blending_fade_alpha_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    blending_fade_alpha_hblank_effect_ptr(int id);
};

}

#endif
