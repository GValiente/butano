#include "bf_intro.h"

#include "btn_keypad.h"
#include "btn_colors.h"
#include "btn_sprite_text_generator.h"

namespace bf
{

namespace
{
    [[nodiscard]] btn::bg_palettes_fade_to_action _create_bg_fade_action()
    {
        btn::bg_palettes::set_fade(btn::colors::maroon, 1);
        return btn::bg_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] btn::sprite_palettes_fade_to_action _create_sprite_fade_action()
    {
        btn::sprite_palettes::set_fade(btn::colors::maroon, 1);
        return btn::sprite_palettes_fade_to_action(60, 0);
    }

    void _enable_blending(btn::ivector<btn::sprite_ptr>& text_sprites)
    {
        for(btn::sprite_ptr& text_sprite : text_sprites)
        {
            text_sprite.set_blending_enabled(true);
        }
    }
}

intro::intro(btn::sprite_text_generator& text_generator) :
    _bg_fade_action(_create_bg_fade_action()),
    _sprite_fade_action(_create_sprite_fade_action()),
    _blending_action(60, 0)
{
    btn::horizontal_alignment_type old_alignment = text_generator.alignment();
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);
    text_generator.generate(0, -20, "Made with", _up_text_sprites);
    text_generator.generate(0, 0, "BUTANO ENGINE", _middle_text_sprites);
    text_generator.generate(0, 20, "https://github.com/GValiente/butano", _down_text_sprites);
    text_generator.set_alignment(old_alignment);

    _enable_blending(_up_text_sprites);
    _enable_blending(_middle_text_sprites);
    _enable_blending(_down_text_sprites);
}

void intro::update()
{
    if(_bg_fade_action.done())
    {
        if(_counter)
        {
            if(btn::keypad::pressed(btn::keypad::button_type::A))
            {
                _counter = 0;
            }
            else
            {
                --_counter;
            }
        }
        else
        {
            if(! _blending_action.done())
            {
                _blending_action.update();
            }
        }
    }
    else
    {
        _bg_fade_action.update();
        _sprite_fade_action.update();
    }
}

}
