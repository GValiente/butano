#ifndef BTN_SPRITE_PALETTE_ACTIONS_H
#define BTN_SPRITE_PALETTE_ACTIONS_H

#include "btn_fixed.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_template_actions.h"

namespace btn
{

// fade

class sprite_palette_fade_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.fade_intensity();
    }

    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_fade_intensity(intensity);
    }
};


class sprite_palette_fade_to_action : public to_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    template<class SpritePalettePtr>
    sprite_palette_fade_to_action(SpritePalettePtr&& palette, int duration_frames, fixed final_intensity) :
        to_template_action(forward<SpritePalettePtr>(palette), duration_frames, final_intensity)
    {
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class sprite_palette_fade_loop_action :
        public loop_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    template<class SpritePalettePtr>
    sprite_palette_fade_loop_action(SpritePalettePtr&& palette, int duration_frames, fixed final_intensity) :
        loop_template_action(forward<SpritePalettePtr>(palette), duration_frames, final_intensity)
    {
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


// rotate

class sprite_palette_rotate_manager
{

public:
    [[nodiscard]] static int get(const sprite_palette_ptr& palette)
    {
        return palette.rotate_count();
    }

    static void set(int count, sprite_palette_ptr& palette)
    {
        palette.set_rotate_count(count);
    }
};


class sprite_palette_rotate_by_action :
        public cyclic_duration_by_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    template<class SpritePalettePtr>
    sprite_palette_rotate_by_action(SpritePalettePtr&& palette, int duration_frames, int delta_count) :
        cyclic_duration_by_template_action(forward<SpritePalettePtr>(palette), duration_frames, delta_count, 0, 0)
    {
        set_after_max_property(this->palette().colors_count() - 1);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int delta_count() const
    {
        return delta_property();
    }
};


class sprite_palette_rotate_to_action :
        public to_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    template<class SpritePalettePtr>
    sprite_palette_rotate_to_action(SpritePalettePtr&& palette, int duration_frames, int final_count) :
        to_template_action(forward<SpritePalettePtr>(palette), duration_frames, final_count)
    {
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


class sprite_palette_rotate_loop_action :
        public loop_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    template<class SpritePalettePtr>
    sprite_palette_rotate_loop_action(SpritePalettePtr&& palette, int duration_frames, int final_count) :
        loop_template_action(forward<SpritePalettePtr>(palette), duration_frames, final_count)
    {
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};

}

#endif
