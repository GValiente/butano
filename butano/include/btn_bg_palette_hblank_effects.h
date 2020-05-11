#ifndef BTN_BG_PALETTE_HBLANK_EFFECTS_H
#define BTN_BG_PALETTE_HBLANK_EFFECTS_H

#include "btn_bg_palette_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class color;

class bg_palette_color_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static bg_palette_color_hblank_effect_ptr create(
            bg_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    [[nodiscard]] static optional<bg_palette_color_hblank_effect_ptr> optional_create(
            bg_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    bg_palette_color_hblank_effect_ptr(const bg_palette_color_hblank_effect_ptr& other) = default;

    bg_palette_color_hblank_effect_ptr& operator=(const bg_palette_color_hblank_effect_ptr& other) = default;

    bg_palette_color_hblank_effect_ptr(bg_palette_color_hblank_effect_ptr&& other) noexcept;

    bg_palette_color_hblank_effect_ptr& operator=(bg_palette_color_hblank_effect_ptr&& other) noexcept;

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return _palette;
    }

    [[nodiscard]] int color_index() const
    {
        return _color_index;
    }

    [[nodiscard]] span<const color> colors_ref() const;

    void set_colors_ref(const span<const color>& colors_ref);

    void reload_colors_ref();

    void swap(bg_palette_color_hblank_effect_ptr& other);

    friend void swap(bg_palette_color_hblank_effect_ptr& a, bg_palette_color_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    bg_palette_ptr _palette;
    int16_t _color_index;

    bg_palette_color_hblank_effect_ptr(int id, int color_index, bg_palette_ptr&& palette);
};

}

#endif
