#include "bf_game_hero_bomb.h"

#include "btn_keypad.h"
#include "btn_blending.h"
#include "btn_sound_items.h"
#include "btn_regular_bg_builder.h"
#include "btn_regular_bg_items_hero_bomb.h"
#include "bf_constants.h"
#include "bf_game_hero.h"
#include "bf_game_intro.h"
#include "bf_game_enemies.h"
#include "bf_wave_generator.h"
#include "bf_game_boss_intro.h"
#include "bf_game_background.h"
#include "bf_game_enemy_bullets.h"

namespace bf::game
{

namespace
{
    constexpr const int open_frames = 50;
    constexpr const int close_frames = 130;

    [[nodiscard]] constexpr btn::array<btn::fixed, btn::display::height()> _create_wave_hblank_effect_deltas()
    {
        btn::array<btn::fixed, btn::display::height()> result;
        wave_generator().generate(result);
        return result;
    }

    constexpr const btn::array<btn::fixed, btn::display::height()> wave_hblank_effect_deltas =
            _create_wave_hblank_effect_deltas();
}

void hero_bomb::update(const intro& intro, const boss_intro& boss_intro, const btn::camera_ptr& camera, hero& hero,
                       enemies& enemies, enemy_bullets& enemy_bullets, background& background)
{
    switch(_status)
    {

    case status_type::INACTIVE:
        if(hero.alive() && ! intro.active() && ! boss_intro.active() && ! enemies.boss_dying() &&
                btn::keypad::a_pressed())
        {
            if(hero.throw_bomb())
            {
                const btn::fixed_point& hero_position = hero.weapon_position();
                _center = btn::point(hero_position.x().right_shift_integer(), hero_position.y().right_shift_integer());

                btn::regular_bg_builder builder(btn::regular_bg_items::hero_bomb);
                builder.set_priority(1);
                builder.set_blending_enabled(true);

                btn::regular_bg_ptr bg = builder.release_build();
                btn::window::outside().set_show_bg(bg, false);
                _bg_move_action.emplace(bg, -0.5, 4);

                btn::rect_window internal_window = btn::rect_window::internal();
                internal_window.set_boundaries(hero_position, hero_position);
                internal_window.set_show_blending(false);
                internal_window.set_camera(camera);
                _move_window_top_action.emplace(internal_window, -4);
                _move_window_bottom_action.emplace(internal_window, 4);

                _circle_generator.set_origin_y(hero_position.y());
                _circle_generator.set_radius(0);
                _circle_generator.generate(_circle_hblank_effect_deltas);
                _circle_hblank_effect = btn::rect_window_boundaries_hblank_effect_ptr::create_horizontal(
                            internal_window, _circle_hblank_effect_deltas);

                _wave_hblank_effect = btn::regular_bg_position_hblank_effect_ptr::create_horizontal(
                            move(bg), wave_hblank_effect_deltas);

                background.show_bomb_open(open_frames);
                btn::sound_items::explosion_2.play();
                _status = status_type::OPEN;
                _counter = open_frames;
                _flame_sound_counter = 0;
            }
            else
            {
                btn::sound_items::no_ammo.play();
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

            btn::fixed fixed_radius = _circle_generator.radius() + 4;
            int integer_radius = fixed_radius.right_shift_integer();
            enemies.check_hero_bomb(_center, integer_radius * integer_radius, camera);
            _circle_generator.set_radius(fixed_radius);
            _circle_generator.generate(_circle_hblank_effect_deltas);
            _circle_hblank_effect->reload_deltas_ref();

            _play_flame_sound();
        }
        else
        {
            _move_window_top_action.reset();
            _move_window_bottom_action.reset();

            btn::rect_window internal_window = btn::rect_window::internal();
            internal_window.set_boundaries(-1000, -1000, 1000, 1000);
            _circle_hblank_effect.reset();
            enemy_bullets.clear();

            internal_window.set_show_blending(true);
            btn::blending::set_transparency_alpha(1);
            _intensity_blending_action.emplace(30, 0.5);
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
            else if(_counter == 30)
            {
                btn::rect_window internal_window = btn::rect_window::internal();
                internal_window.set_boundaries(0, 0, 0, 0);
                internal_window.remove_camera();
                background.show_top(30);
                _bg_move_action.reset();
                _wave_hblank_effect.reset();
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
        BTN_ERROR("Invalid status: ", int(_status));
        break;
    }
}

void hero_bomb::_play_flame_sound()
{
    ++_flame_sound_counter;

    if(_flame_sound_counter > 16 && _flame_sound_counter % 16 == 0)
    {
        btn::sound_items::flame_thrower.play();
    }
}

}
