/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_intro.h"

#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_version.h"
#include "bn_sprite_text_generator.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_action()
    {
        bn::bg_palettes::set_fade(bn::colors::maroon, 1);
        return bn::bg_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprite_fade_action()
    {
        bn::sprite_palettes::set_fade(bn::colors::maroon, 1);
        return bn::sprite_palettes_fade_to_action(60, 0);
    }
}

intro::intro(bn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _bg_fade_action(_create_bg_fade_action()),
    _sprite_fade_action(_create_sprite_fade_action()),
    _blending_action(60, 0)
{
    bn::string<32> middle_text("BUTANO ");
    middle_text.append(bn::to_string<4>(bn::version::major()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::minor()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::patch()));

    text_generator.set_center_alignment();
    text_generator.generate(0, -16, "Made with", _text_sprites);
    text_generator.generate(0, 0, middle_text, _text_sprites);
    text_generator.generate(0, 16, "github.com/GValiente/butano", _text_sprites);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_blending_enabled(true);
    }

    butano_background.put_under_all();
}

bn::optional<scene_type> intro::update()
{
    bn::optional<scene_type> result;

    if(_bg_fade_action.done())
    {
        if(_counter)
        {
            if(bn::keypad::a_pressed())
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
                bn::blending::set_transparency_alpha(1);
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
