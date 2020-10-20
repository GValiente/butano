/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_ending.h"

#include "btn_keypad.h"
#include "btn_music_items.h"
#include "btn_sprite_text_generator.h"
#include "btn_regular_bg_items_butano_huge.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    constexpr const btn::fixed bg_y_inc_start = 30;
    constexpr const btn::fixed bg_y_inc_end = 1;
    constexpr const btn::fixed bg_y_inc_dec = 0.125;
    constexpr const btn::fixed transparency_alpha_inc = 1 / ((bg_y_inc_start - bg_y_inc_end) / bg_y_inc_dec);
}

ending::ending(btn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _bg(btn::regular_bg_items::butano_huge.create_bg(0, 0)),
    _bg_y_inc(bg_y_inc_start)
{
    _bg.set_priority(2);
    _bg.set_blending_enabled(true);

    btn::horizontal_alignment_type old_alignment = text_generator.alignment();
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);
    text_generator.generate(0, -32, "YOU DID IT!", _text_sprites);
    text_generator.generate(0, 0, "Now make your own", _text_sprites);
    text_generator.generate(0, 16, "GBA games with BUTANO", _text_sprites);
    text_generator.generate(0, 32, "github.com/GValiente/butano", _text_sprites);
    text_generator.set_alignment(old_alignment);

    for(btn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(false);
        text_sprite.set_blending_enabled(true);
    }

    btn::blending::set_transparency_alpha(0);
    butano_background.put_under_all();
    btn::music_items::fruit.play(0.4);
}

btn::optional<scene_type> ending::update()
{
    btn::optional<scene_type> result;
    _bg.set_y(_bg.y() - _bg_y_inc);

    if(_bg_y_inc > bg_y_inc_end)
    {
        _bg_y_inc = btn::max(_bg_y_inc - bg_y_inc_dec, bg_y_inc_end);
        btn::fixed transparency_alpha = btn::min(btn::blending::transparency_alpha() + transparency_alpha_inc, btn::fixed(1));
        btn::blending::set_transparency_alpha(transparency_alpha);
    }
    else
    {
        if(wait_for_key)
        {
            if(_blending_action)
            {
                _blending_action->update();

                if(_blending_action->done())
                {
                    if(_text_sprites.empty())
                    {
                        _bg.set_visible(false);
                        btn::blending::set_transparency_alpha(1);
                        result = scene_type::CREDITS;
                    }
                    else
                    {
                        _text_sprites.clear();
                        _bg.set_blending_enabled(true);
                        btn::blending::set_transparency_alpha(1);
                        _blending_action.emplace(60, 0);
                    }
                }
            }
            else
            {
                if(btn::keypad::a_pressed())
                {
                    _blending_action.emplace(60, 0);
                }
            }
        }
        else
        {
            if(_blending_action)
            {
                _blending_action->update();

                if(_blending_action->done())
                {
                    _blending_action.reset();
                    wait_for_key = true;
                }
            }
            else
            {
                _bg.set_blending_enabled(false);

                for(btn::sprite_ptr& text_sprite : _text_sprites)
                {
                    text_sprite.set_visible(true);
                }

                btn::blending::set_transparency_alpha(0);
                _blending_action.emplace(60, 1);
            }
        }
    }

    return result;
}

}
