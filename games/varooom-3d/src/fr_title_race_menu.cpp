/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_title_race_menu.h"

#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_regular_bg_ptr.h"

#include "fr_common_stuff.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_race_menu_cursor.h"
#include "bn_sprite_items_race_menu_locked.h"
#include "bn_sprite_items_small_variable_font.h"
#include "bn_sprite_palette_items_small_font_red.h"
#include "bn_sprite_palette_items_small_font_orange.h"
#include "bn_sprite_palette_items_small_font_yellow.h"

namespace fr
{

namespace
{
    constexpr int animation_desp_total = 256;
    constexpr int animation_desp_inc = 8;
    constexpr int animation_frames = animation_desp_total / animation_desp_inc;

    constexpr bn::fixed easy_x = -72;
    constexpr bn::fixed normal_x = 0;
    constexpr bn::fixed hard_x = 72;

    constexpr bn::fixed forwards_y = -40;
    constexpr bn::fixed backwards_y = 32;
    constexpr bn::fixed text_y_inc = 33;

    bn::sprite_palette_item _text_palette_item(
            const stages_status& stages_status, difficulty_level difficulty, bool reverse)
    {
        if(stages_status.unlocked_stage(difficulty, reverse))
        {
            if(bn::optional<int> best_position = stages_status.best_position(difficulty, reverse))
            {
                switch(*best_position)
                {

                case 0:
                    return bn::sprite_palette_items::small_font_yellow;

                case 1:
                    return bn::sprite_palette_items::small_font_orange;

                default:
                    return bn::sprite_palette_items::small_font_red;
                }
            }
        }

        return bn::sprite_items::small_variable_font.palette_item();
    }
}

title_race_menu::title_race_menu(common_stuff& common_stuff)
{
    const stages_status& stages_status = common_stuff.storage.stages();
    bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
    text_generator.set_center_alignment();
    text_generator.set_palette_item(_text_palette_item(stages_status, difficulty_level::EASY, false));

    if(stages_status.unlocked_stage(difficulty_level::EASY, false))
    {
        text_generator.generate(easy_x, forwards_y + text_y_inc, "EASY", _label_sprites);
    }
    else
    {
        text_generator.generate(easy_x, forwards_y + text_y_inc, "---", _label_sprites);
        _locked_sprites.push_back(bn::sprite_items::race_menu_locked.create_sprite(easy_x, forwards_y));
    }

    text_generator.set_palette_item(_text_palette_item(stages_status, difficulty_level::NORMAL, false));

    if(stages_status.unlocked_stage(difficulty_level::NORMAL, false))
    {
        text_generator.generate(normal_x, forwards_y + text_y_inc, "NORMAL", _label_sprites);
    }
    else
    {
        text_generator.generate(normal_x, forwards_y + text_y_inc, "---", _label_sprites);
        _locked_sprites.push_back(bn::sprite_items::race_menu_locked.create_sprite(normal_x, forwards_y));
    }

    text_generator.set_palette_item(_text_palette_item(stages_status, difficulty_level::HARD, false));

    if(stages_status.unlocked_stage(difficulty_level::HARD, false))
    {
        text_generator.generate(hard_x, forwards_y + text_y_inc, "HARD", _label_sprites);
    }
    else
    {
        text_generator.generate(hard_x, forwards_y + text_y_inc, "---", _label_sprites);
        _locked_sprites.push_back(bn::sprite_items::race_menu_locked.create_sprite(hard_x, forwards_y));
    }

    text_generator.set_palette_item(_text_palette_item(stages_status, difficulty_level::EASY, true));

    if(stages_status.unlocked_stage(difficulty_level::EASY, true))
    {
        text_generator.generate(easy_x, backwards_y + text_y_inc, "EASY R", _label_sprites);
    }
    else
    {
        text_generator.generate(easy_x, backwards_y + text_y_inc, "---", _label_sprites);
        _locked_sprites.push_back(bn::sprite_items::race_menu_locked.create_sprite(easy_x, backwards_y));
    }

    text_generator.set_palette_item(_text_palette_item(stages_status, difficulty_level::NORMAL, true));

    if(stages_status.unlocked_stage(difficulty_level::NORMAL, true))
    {
        text_generator.generate(normal_x, backwards_y + text_y_inc, "NORMAL R", _label_sprites);
    }
    else
    {
        text_generator.generate(normal_x, backwards_y + text_y_inc, "---", _label_sprites);
        _locked_sprites.push_back(bn::sprite_items::race_menu_locked.create_sprite(normal_x, backwards_y));
    }

    text_generator.set_palette_item(_text_palette_item(stages_status, difficulty_level::HARD, true));

    if(stages_status.unlocked_stage(difficulty_level::HARD, true))
    {
        text_generator.generate(hard_x, backwards_y + text_y_inc, "HARD R", _label_sprites);
    }
    else
    {
        text_generator.generate(hard_x, backwards_y + text_y_inc, "---", _label_sprites);
        _locked_sprites.push_back(bn::sprite_items::race_menu_locked.create_sprite(hard_x, backwards_y));
    }

    text_generator.set_left_alignment();
    text_generator.set_palette_item(bn::sprite_items::small_variable_font.palette_item());

    for(bn::sprite_ptr& label_sprite : _label_sprites)
    {
        label_sprite.set_x(label_sprite.x() + animation_desp_total);
    }

    for(bn::sprite_ptr& locked_sprite : _locked_sprites)
    {
        locked_sprite.set_x(locked_sprite.x() + animation_desp_total);
        locked_sprite.set_bg_priority(2);
    }

    _cursor_sprites.push_back(bn::sprite_items::race_menu_cursor.create_sprite(0, 0));
    _cursor_sprites.push_back(bn::sprite_items::race_menu_cursor.create_sprite(0, 0));
    _cursor_sprites.back().set_horizontal_flip(true);
    _update_cursor(common_stuff);

    for(bn::sprite_ptr& cursor_sprite : _cursor_sprites)
    {
        cursor_sprite.set_x(cursor_sprite.x() + animation_desp_total);
    }

    for(bn::sprite_ptr& position_time_sprite : _position_time_sprites)
    {
        position_time_sprite.set_x(position_time_sprite.x() + animation_desp_total);
    }

    set_visible(false);
}

void title_race_menu::set_visible(bool visible)
{
    for(bn::sprite_ptr& cursor_sprite : _cursor_sprites)
    {
        cursor_sprite.set_visible(visible);
    }

    for(bn::sprite_ptr& label_sprite : _label_sprites)
    {
        label_sprite.set_visible(visible);
    }

    for(bn::sprite_ptr& position_time_sprite : _position_time_sprites)
    {
        position_time_sprite.set_visible(visible);
    }

    for(bn::sprite_ptr& locked_sprite : _locked_sprites)
    {
        locked_sprite.set_visible(visible);
    }

    if(visible)
    {
        _show_frames = animation_frames;
    }
}

bn::optional<bool> title_race_menu::update(common_stuff& common_stuff, bn::regular_bg_ptr& backdrop)
{
    bn::optional<bool> result;

    if(_show_frames)
    {
        _show(backdrop);
    }
    else if(_hide_frames)
    {
        _hide(backdrop);
    }
    else if(_return)
    {
        _return = false;
        result = false;
    }
    else if(visible())
    {
        sram_data& sram_data = common_stuff.storage;
        _menu_keypad.update();

        if(bn::keypad::a_pressed())
        {
            if(sram_data.stages().unlocked_stage(sram_data.difficulty(), sram_data.reverse_race()))
            {
                result = true;
                bn::sound_items::plingpling4.play();
            }
            else
            {
                bn::sound_items::blip05.play();
            }
        }
        else if(bn::keypad::b_pressed())
        {
            _hide_frames = animation_frames;
            _return = true;
            bn::sound_items::blip05.play();
        }
        else
        {
            if(_menu_keypad.up())
            {
                if(sram_data.reverse_race())
                {
                    sram_data.set_reverse_race(false);
                    _update_cursor(common_stuff);
                    bn::sound_items::blip02.play();
                }
            }
            else if(_menu_keypad.down())
            {
                if(! sram_data.reverse_race())
                {
                    sram_data.set_reverse_race(true);
                    _update_cursor(common_stuff);
                    bn::sound_items::blip02.play();
                }
            }
            else if(_menu_keypad.left())
            {
                if(sram_data.difficulty() == difficulty_level::HARD)
                {
                    sram_data.set_difficulty(difficulty_level::NORMAL);
                    _update_cursor(common_stuff);
                    bn::sound_items::blip02.play();
                }
                else if(sram_data.difficulty() == difficulty_level::NORMAL)
                {
                    sram_data.set_difficulty(difficulty_level::EASY);
                    _update_cursor(common_stuff);
                    bn::sound_items::blip02.play();
                }
            }
            else if(_menu_keypad.right())
            {
                if(sram_data.difficulty() == difficulty_level::EASY)
                {
                    sram_data.set_difficulty(difficulty_level::NORMAL);
                    _update_cursor(common_stuff);
                    bn::sound_items::blip02.play();
                }
                else if(sram_data.difficulty() == difficulty_level::NORMAL)
                {
                    sram_data.set_difficulty(difficulty_level::HARD);
                    _update_cursor(common_stuff);
                    bn::sound_items::blip02.play();
                }
            }
        }
    }

    return result;
}

void title_race_menu::blink_cursor()
{
    if(_blink_wait_frames)
    {
        --_blink_wait_frames;
    }
    else
    {
        _blink_wait_frames = 2;

        for(bn::sprite_ptr& cursor_sprite : _cursor_sprites)
        {
            cursor_sprite.set_visible(! cursor_sprite.visible());
        }
    }
}

void title_race_menu::_update_cursor(common_stuff& common_stuff)
{
    const sram_data& sram_data = common_stuff.storage;
    difficulty_level difficulty = sram_data.difficulty();
    bool reverse = sram_data.reverse_race();
    bn::fixed x;

    switch(difficulty)
    {

    case difficulty_level::EASY:
        x = easy_x;
        break;

    case difficulty_level::NORMAL:
        x = normal_x;
        break;

    case difficulty_level::HARD:
        x = hard_x;
        break;

    default:
        BN_ERROR("Invalid difficulty: ", int(difficulty));
        break;
    }

    const stages_status& stages_status = sram_data.stages();
    bn::fixed y = reverse ? backwards_y : forwards_y;
    _cursor_sprites[0].set_position(x - 17, y);
    _cursor_sprites[1].set_position(x + 17, y);
    _position_time_sprites.clear();

    if(stages_status.unlocked_stage(difficulty, reverse))
    {
        if(bn::optional<int> best_position = stages_status.best_position(difficulty, reverse))
        {
            int best_time = *stages_status.best_time(difficulty, reverse);
            int frames = best_time;
            int minutes = frames / 3600;
            frames -= minutes * 3600;

            int seconds = frames / 60;
            frames -= seconds * 60;

            int hundredths = (frames * 100) / 60;

            bn::string<16> string("BEST: ");
            bn::ostringstream string_stream(string);
            string_stream.append(*best_position + 1);

            if(stages_status::dollar_time(difficulty, reverse, best_time))
            {
                string_stream.append('$');
            }

            bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
            text_generator.set_center_alignment();
            text_generator.set_palette_item(_text_palette_item(stages_status, difficulty, reverse));
            text_generator.generate(x, y - 6, string, _position_time_sprites);

            string.clear();
            string_stream.append(minutes);
            string_stream.append(':');

            if(seconds < 10)
            {
                string_stream.append('0');
            }

            string_stream.append(seconds);
            string_stream.append('.');

            if(hundredths < 10)
            {
                string_stream.append('0');
            }

            string_stream.append(hundredths);

            text_generator.generate(x, y + 6, string, _position_time_sprites);

            text_generator.set_left_alignment();
            text_generator.set_palette_item(bn::sprite_items::small_variable_font.palette_item());
        }
    }
}

void title_race_menu::_show(bn::regular_bg_ptr& backdrop)
{
    --_show_frames;

    if(! _show_frames)
    {
        _menu_keypad.reset();
    }

    backdrop.set_x(backdrop.x() - animation_desp_inc);

    for(bn::sprite_ptr& cursor_sprite : _cursor_sprites)
    {
        cursor_sprite.set_x(cursor_sprite.x() - animation_desp_inc);
    }

    for(bn::sprite_ptr& label_sprite : _label_sprites)
    {
        label_sprite.set_x(label_sprite.x() - animation_desp_inc);
    }

    for(bn::sprite_ptr& position_time_sprite : _position_time_sprites)
    {
        position_time_sprite.set_x(position_time_sprite.x() - animation_desp_inc);
    }

    for(bn::sprite_ptr& locked_sprite : _locked_sprites)
    {
        locked_sprite.set_x(locked_sprite.x() - animation_desp_inc);
    }
}

void title_race_menu::_hide(bn::regular_bg_ptr& backdrop)
{
    --_hide_frames;
    backdrop.set_x(backdrop.x() + animation_desp_inc);

    for(bn::sprite_ptr& cursor_sprite : _cursor_sprites)
    {
        cursor_sprite.set_x(cursor_sprite.x() + animation_desp_inc);
    }

    for(bn::sprite_ptr& label_sprite : _label_sprites)
    {
        label_sprite.set_x(label_sprite.x() + animation_desp_inc);
    }

    for(bn::sprite_ptr& position_time_sprite : _position_time_sprites)
    {
        position_time_sprite.set_x(position_time_sprite.x() + animation_desp_inc);
    }

    for(bn::sprite_ptr& locked_sprite : _locked_sprites)
    {
        locked_sprite.set_x(locked_sprite.x() + animation_desp_inc);
    }
}


}
