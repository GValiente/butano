#ifndef BTN_BG_PALETTES_HBLANK_EFFECTS_H
#define BTN_BG_PALETTES_HBLANK_EFFECTS_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class color;

class bg_palettes_transparent_color_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static bg_palettes_transparent_color_hblank_effect_ptr create(
            const span<const color>& colors_ref);

    [[nodiscard]] static optional<bg_palettes_transparent_color_hblank_effect_ptr> optional_create(
            const span<const color>& colors_ref);

    bg_palettes_transparent_color_hblank_effect_ptr(
            const bg_palettes_transparent_color_hblank_effect_ptr& other);

    bg_palettes_transparent_color_hblank_effect_ptr& operator=(
            const bg_palettes_transparent_color_hblank_effect_ptr& other);

    bg_palettes_transparent_color_hblank_effect_ptr(
            bg_palettes_transparent_color_hblank_effect_ptr&& other) noexcept;

    bg_palettes_transparent_color_hblank_effect_ptr& operator=(
            bg_palettes_transparent_color_hblank_effect_ptr&& other) noexcept;

    [[nodiscard]] span<const color> colors_ref() const;

    void set_colors_ref(const span<const color>& colors_ref);

    void reload_colors_ref();

    void swap(bg_palettes_transparent_color_hblank_effect_ptr& other);

    friend void swap(bg_palettes_transparent_color_hblank_effect_ptr& a,
                     bg_palettes_transparent_color_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    bg_palettes_transparent_color_hblank_effect_ptr(int id);
};

}

#endif
