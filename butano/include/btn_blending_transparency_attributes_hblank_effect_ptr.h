/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_HBLANK_EFFECT_PTR_H
#define BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_HBLANK_EFFECT_PTR_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class blending_transparency_attributes;

class blending_transparency_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static blending_transparency_attributes_hblank_effect_ptr create(
            const span<const blending_transparency_attributes>& attributes_ref);

    [[nodiscard]] static optional<blending_transparency_attributes_hblank_effect_ptr> create_optional(
            const span<const blending_transparency_attributes>& attributes_ref);

    [[nodiscard]] span<const blending_transparency_attributes> attributes_ref() const;

    void set_attributes_ref(const span<const blending_transparency_attributes>& attributes_ref);

    void reload_attributes_ref();

    void swap(blending_transparency_attributes_hblank_effect_ptr& other)
    {
        hblank_effect_ptr::swap(other);
    }

    friend void swap(blending_transparency_attributes_hblank_effect_ptr& a,
                     blending_transparency_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    blending_transparency_attributes_hblank_effect_ptr(int id);
};

}

#endif
