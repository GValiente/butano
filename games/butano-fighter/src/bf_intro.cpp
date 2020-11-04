/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_intro.h"

#include "btn_string.h"
#include "btn_keypad.h"
#include "btn_colors.h"
#include "btn_version.h"
#include "btn_optional.h"
#include "btn_sprite_text_generator.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

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
}

intro::intro(btn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _bg_fade_action(_create_bg_fade_action()),
    _sprite_fade_action(_create_sprite_fade_action()),
    _blending_action(60, 0)
{
    btn::string<32> middle_text("BUTANO ");
    middle_text.append(btn::to_string<4>(btn::version::major()));
    middle_text.append('.');
    middle_text.append(btn::to_string<4>(btn::version::minor()));
    middle_text.append('.');
    middle_text.append(btn::to_string<4>(btn::version::patch()));

    text_generator.set_center_alignment();
    text_generator.generate(0, -16, "Made with", _text_sprites);
    text_generator.generate(0, 0, middle_text, _text_sprites);
    text_generator.generate(0, 16, "github.com/GValiente/butano", _text_sprites);

    for(btn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_blending_enabled(true);
    }

    butano_background.put_under_all();
}

btn::optional<scene_type> intro::update()
{
    btn::optional<scene_type> result;

    if(_bg_fade_action.done())
    {
        if(_counter)
        {
            if(btn::keypad::a_pressed())
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
            if(_blending_action.done())
            {
                _text_sprites.clear();
                btn::blending::set_transparency_alpha(1);
                result = scene_type::TITLE;
            }
            else
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

    return result;
}

}
