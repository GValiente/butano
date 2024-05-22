/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_ending.h"

#include "bn_keypad.h"
#include "bn_music_items.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_items_butano_huge.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    constexpr bn::fixed bg_y_inc_start = 30;
    constexpr bn::fixed bg_y_inc_end = 1;
    constexpr bn::fixed bg_y_inc_dec = 0.125;
    constexpr bn::fixed transparency_alpha_inc = 1 / ((bg_y_inc_start - bg_y_inc_end) / bg_y_inc_dec);
}

ending::ending(bn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _bg(bn::regular_bg_items::butano_huge.create_bg(0, 0)),
    _bg_y_inc(bg_y_inc_start)
{
    _bg.set_priority(2);
    _bg.set_blending_enabled(true);

    text_generator.set_center_alignment();
    text_generator.generate(0, -40, "YOU DID IT!", _text_sprites);
    text_generator.generate(0, -8, "Now make your own", _text_sprites);
    text_generator.generate(0, 8, "GBA games with BUTANO", _text_sprites);
    text_generator.generate(0, 40, "github.com/GValiente/butano", _text_sprites);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(false);
        text_sprite.set_blending_enabled(true);
    }

    bn::blending::set_transparency_alpha(0);
    butano_background.put_under_all();
    bn::music_items::fruit.play(0.4);
}

bn::optional<scene_type> ending::update()
{
    bn::optional<scene_type> result;
    _bg.set_y(_bg.y() - _bg_y_inc);

    if(_bg_y_inc > bg_y_inc_end)
    {
        _bg_y_inc = bn::max(_bg_y_inc - bg_y_inc_dec, bg_y_inc_end);
        bn::fixed transparency_alpha = bn::min(bn::blending::transparency_alpha() + transparency_alpha_inc, bn::fixed(1));
        bn::blending::set_transparency_alpha(transparency_alpha);
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
                        bn::blending::set_transparency_alpha(1);
                        result = scene_type::CREDITS;
                    }
                    else
                    {
                        _text_sprites.clear();
                        _bg.set_blending_enabled(true);
                        bn::blending::set_transparency_alpha(1);
                        _blending_action.emplace(60, 0);
                    }
                }
            }
            else
            {
                if(bn::keypad::a_pressed())
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

                for(bn::sprite_ptr& text_sprite : _text_sprites)
                {
                    text_sprite.set_visible(true);
                }

                bn::blending::set_transparency_alpha(0);
                _blending_action.emplace(60, 1);
            }
        }
    }

    return result;
}

}
