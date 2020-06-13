#ifndef BTN_SPRITE_PALETTE_HBLANK_EFFECTS_H
#define BTN_SPRITE_PALETTE_HBLANK_EFFECTS_H

#include "btn_hblank_effect_ptr.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

class color;

class sprite_palette_color_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_palette_color_hblank_effect_ptr create(
            sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    [[nodiscard]] static optional<sprite_palette_color_hblank_effect_ptr> create_optional(
            sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    sprite_palette_color_hblank_effect_ptr(const sprite_palette_color_hblank_effect_ptr& other) = default;

    sprite_palette_color_hblank_effect_ptr& operator=(const sprite_palette_color_hblank_effect_ptr& other) = default;

    sprite_palette_color_hblank_effect_ptr(sprite_palette_color_hblank_effect_ptr&& other) noexcept;

    sprite_palette_color_hblank_effect_ptr& operator=(sprite_palette_color_hblank_effect_ptr&& other) noexcept;

    [[nodiscard]] const sprite_palette_ptr& palette() const
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

    void swap(sprite_palette_color_hblank_effect_ptr& other);

    friend void swap(sprite_palette_color_hblank_effect_ptr& a, sprite_palette_color_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_palette_ptr _palette;
    int16_t _color_index;

    sprite_palette_color_hblank_effect_ptr(int id, int color_index, sprite_palette_ptr&& palette);
};

}

#endif
