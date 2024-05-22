/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_title.h"

#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_rumble.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_affine_mat_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_hero_head.h"
#include "bn_sprite_items_butano_font.h"
#include "bn_sprite_items_fighter_font.h"
#include "bn_sprite_items_butano_big_sprite.h"
#include "bf_status.h"
#include "bf_scene_type.h"
#include "bf_wave_generator.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    constexpr bn::fixed _text_x = -28;

    [[nodiscard]] bn::sprite_ptr _create_butano_up_sprite()
    {
        bn::sprite_builder builder(bn::sprite_items::butano_big_sprite, 0);
        builder.set_y((bn::display::height() / 2) + 32);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }

    [[nodiscard]] bn::sprite_ptr _create_butano_down_sprite()
    {
        bn::sprite_builder builder(bn::sprite_items::butano_big_sprite, 1);
        builder.set_y((bn::display::height() / 2) + 32 + 64);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }

    [[nodiscard]] bn::vector<bn::sprite_ptr, 6> _create_butano_characters()
    {
        bn::vector<bn::sprite_ptr, 6> result;

        for(int index = 0; index < 6; ++index)
        {
            result.push_back(bn::sprite_items::butano_font.create_sprite((index * 20) - 50, -28, index));
            result.back().set_visible(false);
        }

        return result;
    }

    [[nodiscard]] bn::vector<bn::sprite_ptr, 7> _create_fighter_characters()
    {
        bn::vector<bn::sprite_ptr, 7> result;
        bn::fixed fighter_x = 78;

        for(int index = 6; index >= 0; --index)
        {
            result.push_back(bn::sprite_items::fighter_font.create_sprite(fighter_x, 4, index));
            result.back().set_visible(false);
            fighter_x -= index <= 2 ? 21 : 29;
        }

        return result;
    }

    constexpr bn::array<bn::fixed, bn::display::height()> butano_character_hbe_deltas = []{
        bn::array<bn::fixed, bn::display::height()> result;
        int start = 64;

        for(int index = start; index < bn::display::height(); ++index)
        {
            result[index] = index - start;
        }

        return result;
    }();

    constexpr bn::array<bn::affine_mat_attributes, bn::display::height()> fighter_character_hbe_attributes = []{
        bn::array<bn::affine_mat_attributes, bn::display::height()> result;
        int start = 90;

        for(int index = 0; index < start; ++index)
        {
            result[index] = bn::affine_mat_attributes();
        }

        for(int index = start; index < bn::display::height(); ++index)
        {
            result[index].set_scale(1 + (bn::fixed(index - start) / 112));
            result[index].set_rotation_angle(index - start);
        }

        return result;
    }();
}

title::title(status& status, bn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _status(status),
    _text_generator(text_generator),
    _butano_up_sprite(_create_butano_up_sprite()),
    _butano_down_sprite(_create_butano_down_sprite()),
    _butano_characters(_create_butano_characters()),
    _fighter_characters(_create_fighter_characters()),
    _cursor_sprite(bn::sprite_items::hero_head.create_sprite(0, 0)),
    _butano_up_x_hbe(bn::sprite_position_hbe_ptr::create_horizontal(_butano_up_sprite, _butano_x_hbe_deltas)),
    _butano_down_x_hbe(bn::sprite_position_hbe_ptr::create_horizontal(_butano_down_sprite, _butano_x_hbe_deltas))
{
    bn::string<20> high_experience_text("HIGH EXP: ");
    high_experience_text.append(bn::to_string<8>(status.high_experience()));

    text_generator.set_center_alignment();
    text_generator.generate(0, 12 - (bn::display::height() / 2), high_experience_text, _high_experience_text_sprites);
    text_generator.set_left_alignment();
    text_generator.generate(_text_x, 34, "START", _start_text_sprites);
    text_generator.generate(_text_x, 34 + 12, "HOW TO PLAY", _how_to_play_sprites);
    text_generator.generate(_text_x, 34 + 12 + 12, "CREDITS", _credits_text_sprites);
    text_generator.generate(_text_x, 34 + 12 + 12 + 12, status.rumble_enabled() ? "RUMBLE: ON" : "RUMBLE: OFF",
                            _rumble_text_sprites);
    _cursor_sprite.set_position(_how_to_play_sprites[0].x() - 28, _start_text_sprites[0].y());
    _cursor_sprite.set_visible(false);

    for(bn::sprite_ptr& sprite : _high_experience_text_sprites)
    {
        sprite.set_visible(false);
    }

    for(bn::sprite_ptr& sprite : _start_text_sprites)
    {
        sprite.set_visible(false);
    }

    for(bn::sprite_ptr& sprite : _how_to_play_sprites)
    {
        sprite.set_visible(false);
    }

    for(bn::sprite_ptr& sprite : _credits_text_sprites)
    {
        sprite.set_visible(false);
    }

    for(bn::sprite_ptr& sprite : _rumble_text_sprites)
    {
        sprite.set_visible(false);
    }

    butano_background.put_under_all();

    if(! bn::music::playing())
    {
        bn::music_items::battle_clean.play(0.6);
    }
}

bn::optional<scene_type> title::update()
{
    _animate_butano_x();
    _animate_butano_y();
    _animate_butano_characters();
    _animate_fighter_characters();
    return _menu();
}

void title::_animate_butano_x()
{
    if(_butano_x_hbe_speed)
    {
        wave_generator generator;
        generator.set_speed(_butano_x_hbe_speed);

        if(_butano_x_hbe_speed > 32)
        {
            generator.set_amplitude(2);
            _butano_x_hbe_speed -= 32;
        }
        else
        {
            generator.set_amplitude(1);
            _butano_x_hbe_speed /= 2;
        }

        generator.generate(_butano_x_hbe_deltas);
        _butano_up_x_hbe->reload_deltas_ref();
        _butano_down_x_hbe->reload_deltas_ref();
    }
    else
    {
        if(_butano_up_x_hbe)
        {
            _butano_up_x_hbe.reset();
            _butano_down_x_hbe.reset();
        }
    }
}

void title::_animate_butano_y()
{
    if(_butano_y_up)
    {
        _butano_y_inc += 0.05;

        if(_butano_y_inc >= 1)
        {
            _butano_y_inc = 1;
            _butano_y_up = false;
        }
        else
        {
            if(_butano_y_inc >= 0 && _state == state::START)
            {
                _state = state::BUTANO_CHARACTERS_SHOW;
            }
        }
    }
    else
    {
        _butano_y_inc -= 0.05;

        if(_butano_y_inc <= -1)
        {
            _butano_y_inc = -1;
            _butano_y_up = true;
        }
    }

    bn::fixed butano_up_sprite_y = _butano_up_sprite.y() + _butano_y_inc;
    _butano_up_sprite.set_y(butano_up_sprite_y);
    _butano_down_sprite.set_y(butano_up_sprite_y + 64);

    if(int(_state) <= int(state::MENU))
    {
        if(butano_up_sprite_y <= -26)
        {
            bn::fixed blending_intensity = -butano_up_sprite_y - 26;
            bn::blending::set_intensity_alpha(blending_intensity / 40);
        }
        else
        {
            bn::blending::set_intensity_alpha(0);
        }
    }
}

void title::_animate_butano_characters()
{
    if(_state != state::BUTANO_CHARACTERS_SHOW)
    {
        return;
    }

    if(_butano_character_move_action)
    {
        _butano_character_move_action->update();

        if(_butano_character_move_action->done())
        {
            _butano_character_move_action.reset();
            _butano_character_hbe.reset();
        }
    }
    else
    {
        for(bn::sprite_ptr& butano_character : _butano_characters)
        {
            if(! butano_character.visible())
            {
                bn::fixed_point position = butano_character.position();
                butano_character.set_y(0);
                butano_character.set_visible(true);
                _butano_character_move_action.emplace(butano_character, 12, position);
                _butano_character_hbe = bn::sprite_position_hbe_ptr::create_vertical(
                            butano_character, butano_character_hbe_deltas);
                return;
            }
        }

        _state = state::FIGHTER_CHARACTERS_SHOW;
    }
}

void title::_animate_fighter_characters()
{
    if(_state != state::FIGHTER_CHARACTERS_SHOW)
    {
        return;
    }

    if(_fighter_character_move_action)
    {
        _fighter_character_move_action->update();

        if(_fighter_character_move_action->done())
        {
            _fighter_character_move_action.reset();
            _fighter_character_hbe.reset();

            for(bn::sprite_ptr& fighter_character : _fighter_characters)
            {
                if(fighter_character.affine_mat())
                {
                    fighter_character.remove_affine_mat();
                }
            }
        }
    }
    else
    {
        for(bn::sprite_ptr& fighter_character : _fighter_characters)
        {
            if(! fighter_character.visible())
            {
                bn::fixed_point position = fighter_character.position();
                fighter_character.set_y((bn::display::height() + 32) / 2);
                fighter_character.set_visible(true);
                _fighter_character_move_action.emplace(fighter_character, 16, position);

                bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
                fighter_character.set_affine_mat(affine_mat);
                fighter_character.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);
                _fighter_character_hbe = bn::sprite_affine_mat_attributes_hbe_ptr::create(
                            affine_mat, fighter_character_hbe_attributes);
                return;
            }
        }

        _state = state::MENU;
    }
}

bn::optional<scene_type> title::_menu()
{
    const int cursor_scale_frames = 20;
    const int sprites_hide_frames = 60;
    bn::optional<scene_type> result;

    if(_cursor_move_action)
    {
        _cursor_move_action->update();

        if(_cursor_move_action->done())
        {
            _cursor_move_action.reset();
        }
    }
    else if(_state == state::MENU)
    {
        if(! _cursor_sprite.visible())
        {
            _cursor_sprite.set_visible(true);
            _cursor_sprite.set_scale(0.01);
            _cursor_scale_action.emplace(_cursor_sprite, cursor_scale_frames, 1);

            for(bn::sprite_ptr& sprite : _high_experience_text_sprites)
            {
                sprite.set_visible(true);
            }

            for(bn::sprite_ptr& sprite : _start_text_sprites)
            {
                sprite.set_visible(true);
            }

            for(bn::sprite_ptr& sprite : _how_to_play_sprites)
            {
                sprite.set_visible(true);
            }

            for(bn::sprite_ptr& sprite : _credits_text_sprites)
            {
                sprite.set_visible(true);
            }

            for(bn::sprite_ptr& sprite : _rumble_text_sprites)
            {
                sprite.set_visible(true);
            }
        }

        if(bn::keypad::a_pressed())
        {
            if(_menu_index == 0)
            {
                bn::sound_items::start.play();

                if(_status.rumble_enabled())
                {
                    bn::rumble::set_enabled(true);
                }

                if(_status.how_to_play_viewed())
                {
                    _music_volume_action.emplace(cursor_scale_frames + sprites_hide_frames, 0);
                }
            }
            else
            {
                bn::sound_items::cure.play();

                if(_menu_index == 2)
                {
                    _music_volume_action.emplace(cursor_scale_frames + sprites_hide_frames, 0);
                }
                else if(_menu_index == 3)
                {
                    _status.set_rumble_enabled(! _status.rumble_enabled());

                    bn::fixed text_y = _rumble_text_sprites[0].y();
                    _rumble_text_sprites.clear();
                    _text_generator.set_left_alignment();
                    _text_generator.generate(_text_x, text_y, _status.rumble_enabled() ? "RUMBLE: ON" : "RUMBLE: OFF",
                                             _rumble_text_sprites);
                }
            }

            if(_menu_index != 3)
            {
                if(bn::blending::intensity_alpha() > 0)
                {
                    _blending_intensity_action.emplace(cursor_scale_frames, 0);
                }

                _cursor_scale_action.emplace(_cursor_sprite, cursor_scale_frames, bn::fixed(0.01));
                _state = state::HIDE_CURSOR;
            }
        }
        else if(bn::keypad::up_pressed() || bn::keypad::down_pressed())
        {
            if(bn::keypad::up_pressed())
            {
                if(_menu_index == 0)
                {
                    _menu_index = 3;
                }
                else
                {
                    _menu_index -= 1;
                }
            }
            else
            {
                if(_menu_index == 3)
                {
                    _menu_index = 0;
                }
                else
                {
                    _menu_index += 1;
                }
            }

            switch(_menu_index)
            {

            case 0:
                _cursor_move_action.emplace(_cursor_sprite, 3, _cursor_sprite.x(), _start_text_sprites[0].y());
                break;

            case 1:
                _cursor_move_action.emplace(_cursor_sprite, 3, _cursor_sprite.x(), _how_to_play_sprites[0].y());
                break;

            case 2:
                _cursor_move_action.emplace(_cursor_sprite, 3, _cursor_sprite.x(), _credits_text_sprites[0].y());
                break;

            case 3:
                _cursor_move_action.emplace(_cursor_sprite, 3, _cursor_sprite.x(), _rumble_text_sprites[0].y());
                break;

            default:
                BN_ERROR("Invalid menu index: ", _menu_index);
                break;
            }

            bn::sound_items::alert.play();
        }
    }

    if(_cursor_scale_action)
    {
        _cursor_scale_action->update();

        if(_cursor_scale_action->done())
        {
            _cursor_scale_action.reset();

            if(_state == state::HIDE_CURSOR)
            {
                for(bn::sprite_ptr& sprite : _butano_characters)
                {
                    sprite.set_blending_enabled(true);
                }

                for(bn::sprite_ptr& sprite : _fighter_characters)
                {
                    sprite.set_blending_enabled(true);
                }

                for(bn::sprite_ptr& sprite : _high_experience_text_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                for(bn::sprite_ptr& sprite : _start_text_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                for(bn::sprite_ptr& sprite : _how_to_play_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                for(bn::sprite_ptr& sprite : _credits_text_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                for(bn::sprite_ptr& sprite : _rumble_text_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                _cursor_sprite.set_visible(false);
                _blending_transparency_action.emplace(sprites_hide_frames, 0);
                bn::rumble::set_enabled(false);
                _state = state::HIDE_SPRITES;
            }
        }
    }

    if(_music_volume_action)
    {
        _music_volume_action->update();

        if(_music_volume_action->done())
        {
            _music_volume_action.reset();
        }
    }

    if(_blending_intensity_action)
    {
        _blending_intensity_action->update();

        if(_blending_intensity_action->done())
        {
            _blending_intensity_action.reset();
        }
    }

    if(_blending_transparency_action)
    {
        _blending_transparency_action->update();

        if(_blending_transparency_action->done())
        {
            _blending_transparency_action.reset();
            _butano_up_sprite.set_visible(false);
            _butano_down_sprite.set_visible(false);
            _butano_characters.clear();
            _fighter_characters.clear();
            _high_experience_text_sprites.clear();
            _start_text_sprites.clear();
            _how_to_play_sprites.clear();
            _credits_text_sprites.clear();
            _rumble_text_sprites.clear();
            bn::blending::set_transparency_alpha(1);

            if(_menu_index == 0)
            {
                if(_status.how_to_play_viewed())
                {
                    result = scene_type::GAME;
                }
                else
                {
                    result = scene_type::HOW_TO_PLAY_AND_GAME;
                }
            }
            else if(_menu_index == 1)
            {
                result = scene_type::HOW_TO_PLAY_AND_TITLE;
            }
            else
            {
                result = scene_type::CREDITS;
                bn::music::stop();
            }
        }
    }

    return result;
}

}
