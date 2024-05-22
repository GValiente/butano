/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_hero_bomb.h"

#include "bn_keypad.h"
#include "bn_blending.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_items_hero_bomb.h"
#include "bf_constants.h"
#include "bf_game_hero.h"
#include "bf_game_intro.h"
#include "bf_game_enemies.h"
#include "bf_wave_generator.h"
#include "bf_game_boss_intro.h"
#include "bf_game_background.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_rumble_manager.h"

namespace bf::game
{

namespace
{
    constexpr int open_frames = 50;
    constexpr int close_frames = 130;

    constexpr bn::array<bn::fixed, bn::display::height()> wave_hbe_deltas = []{
        bn::array<bn::fixed, bn::display::height()> result;
        wave_generator().generate(result);
        return result;
    }();
}

void hero_bomb::update(const intro& intro, const boss_intro& boss_intro, const bn::camera_ptr& camera, hero& hero,
                       enemies& enemies, enemy_bullets& enemy_bullets, background& background,
                       rumble_manager& rumble_manager)
{
    switch(_status)
    {

    case status_type::INACTIVE:
        if(hero.alive() && ! intro.active() && ! boss_intro.active() && ! enemies.boss_dying() &&
                bn::keypad::a_pressed())
        {
            if(hero.throw_bomb())
            {
                const bn::fixed_point& hero_position = hero.weapon_position();
                _center = bn::point(hero_position.x().right_shift_integer(), hero_position.y().right_shift_integer());

                bn::regular_bg_builder builder(bn::regular_bg_items::hero_bomb);
                builder.set_priority(1);
                builder.set_blending_enabled(true);

                bn::regular_bg_ptr bg = builder.release_build();
                bn::window::outside().set_show_bg(bg, false);
                _bg_move_action.emplace(bg, bn::fixed(-0.5), 4);

                bn::rect_window internal_window = bn::rect_window::internal();
                internal_window.set_boundaries(hero_position, hero_position);
                internal_window.set_show_blending(false);
                internal_window.set_camera(camera);
                _move_window_top_action.emplace(internal_window, -4);
                _move_window_bottom_action.emplace(internal_window, 4);

                _circle_generator.set_origin_y(hero_position.y());
                _circle_generator.set_radius(0);
                _circle_generator.generate(_circle_hbe_deltas);
                _circle_hbe = bn::rect_window_boundaries_hbe_ptr::create_horizontal(
                            internal_window, _circle_hbe_deltas);

                _wave_hbe = bn::regular_bg_position_hbe_ptr::create_horizontal(move(bg), wave_hbe_deltas);

                background.show_bomb_open(open_frames);
                bn::sound_items::explosion_2.play();
                rumble_manager.set_enabled(true);
                _status = status_type::OPEN;
                _counter = open_frames;
                _flame_sound_counter = 0;
            }
            else
            {
                bn::sound_items::no_ammo.play();
            }
        }
        break;

    case status_type::OPEN:
        _bg_move_action->update();

        if(_counter)
        {
            --_counter;

            _move_window_top_action->update();
            _move_window_bottom_action->update();

            bn::fixed fixed_radius = _circle_generator.radius() + 4;
            int integer_radius = fixed_radius.right_shift_integer();
            enemies.check_hero_bomb(_center, integer_radius * integer_radius, camera);
            _circle_generator.set_radius(fixed_radius);
            _circle_generator.generate(_circle_hbe_deltas);
            _circle_hbe->reload_deltas_ref();

            _play_flame_sound();
        }
        else
        {
            _move_window_top_action.reset();
            _move_window_bottom_action.reset();

            bn::rect_window internal_window = bn::rect_window::internal();
            internal_window.set_boundaries(-1000, -1000, 1000, 1000);
            _circle_hbe.reset();
            enemy_bullets.clear();

            internal_window.set_show_blending(true);
            bn::blending::set_transparency_alpha(1);
            _intensity_blending_action.emplace(30, bn::fixed(0.5));
            background.show_bomb_close(close_frames - 30);

            _status = status_type::CLOSE;
            _counter = close_frames;
        }
        break;

    case status_type::CLOSE:
        if(_bg_move_action)
        {
            _bg_move_action->update();
        }

        if(_transparency_blending_action)
        {
            _transparency_blending_action->update();

            if(_transparency_blending_action->done())
            {
                _transparency_blending_action.reset();
            }
        }

        if(_intensity_blending_action)
        {
            _intensity_blending_action->update();

            if(_intensity_blending_action->done())
            {
                _intensity_blending_action.reset();
            }
        }

        if(_counter)
        {
            --_counter;

            if(_counter == close_frames - 30)
            {
                _transparency_blending_action.emplace(close_frames - 60, 0);
                _intensity_blending_action.emplace(30, 0);
            }
            else if(_counter == close_frames - 60)
            {
                background.hide_bomb_close(close_frames - 90);
            }
            else if(_counter == 60)
            {
                rumble_manager.set_enabled(false);
            }
            else if(_counter == 30)
            {
                bn::rect_window internal_window = bn::rect_window::internal();
                internal_window.set_boundaries(0, 0, 0, 0);
                internal_window.remove_camera();
                background.show_top(30);
                _bg_move_action.reset();
                _wave_hbe.reset();
            }

            if(_counter > 40)
            {
                _play_flame_sound();
            }
        }
        else
        {
            _status = status_type::INACTIVE;
        }
        break;

    default:
        BN_ERROR("Invalid status: ", int(_status));
        break;
    }
}

void hero_bomb::_play_flame_sound()
{
    ++_flame_sound_counter;

    if(_flame_sound_counter > 16 && _flame_sound_counter % 16 == 0)
    {
        bn::sound_items::flame_thrower.play();
    }
}

}
