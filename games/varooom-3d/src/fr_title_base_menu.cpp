/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_title_base_menu.h"

#include "bn_keypad.h"

#include "fr_common_stuff.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_small_variable_font.h"
#include "bn_sprite_palette_items_small_font_yellow.h"

namespace fr
{

namespace
{
    constexpr bn::fixed text_x = 18;
    constexpr bn::fixed scale_inc = bn::fixed(1) / 8;

    constexpr int y_inc = 11;
    constexpr bn::fixed race_text_y = -8;
    constexpr bn::fixed viewer_text_y = race_text_y + y_inc;
    constexpr bn::fixed options_text_y = viewer_text_y + y_inc;
    constexpr bn::fixed how_to_play_text_y = options_text_y + y_inc;
    constexpr bn::fixed credits_text_y = how_to_play_text_y + y_inc;

    [[nodiscard]] bn::sprite_ptr _create_cursor_sprite(common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(text_x - 10, race_text_y, ">", output_sprites);
        text_generator.set_one_sprite_per_character(false);
        return bn::move(output_sprites[0]);
    }
}

title_base_menu::title_base_menu(common_stuff& common_stuff) :
    _affine_mat(bn::sprite_affine_mat_ptr::create()),
    _cursor_sprite(_create_cursor_sprite(common_stuff)),
    _all_models_viewed(common_stuff.storage.model_viewer_items().all_items_viewed())
{
    const stages_status& stages_status = common_stuff.storage.stages();

    if(stages_status.unlocked_stage(difficulty_level::HARD, false) ||
            stages_status.unlocked_stage(difficulty_level::HARD, true))
    {
        _show_credits = true;
    }
    else
    {
        _y_inc = y_inc - 6;
        _update_cursor();
    }

    bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
    text_generator.generate(text_x, race_text_y + _y_inc, "RACE", _text_sprites);

    if(! _all_models_viewed)
    {
        text_generator.set_palette_item(bn::sprite_palette_items::small_font_yellow);
    }

    text_generator.generate(text_x, viewer_text_y + _y_inc, "3D VIEWER", _text_sprites);
    text_generator.set_palette_item(bn::sprite_items::small_variable_font.palette_item());
    text_generator.generate(text_x, options_text_y + _y_inc, "OPTIONS", _text_sprites);
    text_generator.generate(text_x, how_to_play_text_y + _y_inc, "HOW TO PLAY", _text_sprites);

    if(_show_credits)
    {
        text_generator.generate(text_x, credits_text_y + _y_inc, "CREDITS", _text_sprites);
    }

    _cursor_sprite.set_affine_mat(_affine_mat);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_affine_mat(_affine_mat);
    }

    _set_sprites_visible(false);
}

void title_base_menu::set_visible(bool visible, bool animate)
{
    if(animate)
    {
        if(visible)
        {
            _intro_done = false;
        }
        else
        {
            _outro_done = false;
        }
    }
    else
    {
        _set_sprites_visible(visible);
    }
}

bn::optional<title_base_menu::option> title_base_menu::update()
{
    bn::optional<option> result;

    if(! _intro_done)
    {
        _update_intro();
    }
    else if(! _outro_done)
    {
        _update_outro();
    }
    else
    {
        _menu_keypad.update();

        if(bn::keypad::a_pressed())
        {
            switch(_cursor_index)
            {

            case 0:
                bn::sound_items::blip09.play();
                break;

            case 1:
                bn::sound_items::plingpling4.play();
                break;

            case 2:
                bn::sound_items::blip09.play();
                break;

            case 3:
                bn::sound_items::plingpling4.play();
                break;

            case 4:
                bn::sound_items::plingpling4.play();
                break;

            default:
                BN_ERROR("Invalid cursor index: ", _cursor_index);
                break;
            }

            result = option(_cursor_index);
        }
        else if(_menu_keypad.up())
        {
            if(_cursor_index > 0)
            {
                --_cursor_index;
                _update_cursor();
                bn::sound_items::blip02.play();
            }
        }
        else if(_menu_keypad.down())
        {
            int max_index = 2;

            if(_show_credits)
            {
                ++max_index;
            }

            if(_cursor_index <= max_index)
            {
                ++_cursor_index;
                _update_cursor();
                bn::sound_items::blip02.play();
            }
        }
    }

    return result;
}

void title_base_menu::_set_sprites_visible(bool visible)
{
    _cursor_sprite.set_visible(visible);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(visible);
    }
}

void title_base_menu::_update_intro()
{
    bn::fixed vertical_scale;

    if(_cursor_sprite.visible())
    {
        vertical_scale = _cursor_sprite.vertical_scale();

        if(vertical_scale < 1)
        {
            vertical_scale += scale_inc;

            if(vertical_scale >= 1)
            {
                vertical_scale = 1;
                _menu_keypad.reset();
                _intro_done = true;
            }
        }
    }
    else
    {
        _set_sprites_visible(true);
        vertical_scale = scale_inc;
    }

    _affine_mat.set_vertical_scale(vertical_scale);
}

void title_base_menu::_update_outro()
{
    bn::fixed vertical_scale = _cursor_sprite.vertical_scale() - scale_inc;

    if(vertical_scale <= scale_inc)
    {
        _set_sprites_visible(false);
        vertical_scale = scale_inc;
        _outro_done = true;
    }

    _affine_mat.set_vertical_scale(vertical_scale);
}

void title_base_menu::_update_cursor()
{
    bool yellow_palette = false;

    switch(_cursor_index)
    {

    case 0:
        _cursor_sprite.set_y(race_text_y + _y_inc);
        break;

    case 1:
        yellow_palette = ! _all_models_viewed;
        _cursor_sprite.set_y(viewer_text_y + _y_inc);
        break;

    case 2:
        _cursor_sprite.set_y(options_text_y + _y_inc);
        break;

    case 3:
        _cursor_sprite.set_y(how_to_play_text_y + _y_inc);
        break;

    case 4:
        _cursor_sprite.set_y(credits_text_y + _y_inc);
        break;

    default:
        BN_ERROR("Invalid cursor index: ", _cursor_index);
        break;
    }

    if(yellow_palette)
    {
        _cursor_sprite.set_palette(bn::sprite_palette_items::small_font_yellow);
    }
    else
    {
        _cursor_sprite.set_palette(bn::sprite_items::small_variable_font.palette_item());
    }
}

}
