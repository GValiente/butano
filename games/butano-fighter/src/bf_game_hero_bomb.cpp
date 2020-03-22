#include "bf_game_hero_bomb.h"

#include "btn_sound.h"
#include "btn_keypad.h"
#include "btn_window.h"
#include "btn_blending.h"
#include "btn_regular_bg_builder.h"
#include "btn_hero_bomb_bg_item.h"
#include "btn_sound_items.h"
#include "bf_game_hero.h"
#include "bf_game_background.h"

namespace bf
{

namespace
{
    constexpr const int open_frames = 35;
    constexpr const int close_frames = 150;

    btn::regular_bg_ptr _create_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::hero_bomb);
        builder.set_priority(1);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }
}

game_hero_bomb::game_hero_bomb() :
    _bg(_create_bg()),
    _bg_move_action(_bg, -0.5, 4)
{
    btn::window::outside().set_show_bg(_bg, false);
}

void game_hero_bomb::update(game_hero& hero, game_background& background)
{
    switch(_status)
    {

    case status_type::INACTIVE:
        if(btn::keypad::pressed(btn::keypad::button_type::A) && hero.throw_bomb())
        {
            btn::rect_window window = btn::window::internal();
            int display_width = btn::display::width();
            int display_height = btn::display::height();
            int window_y = (display_height / 2) + hero.weapon_position().y().integer();
            window.set_boundaries(window_y, 0, window_y, display_width);
            _move_top_left_window_action.emplace(window, window_y / 4, 0, 0);
            _move_bottom_right_window_action.emplace(window, (display_height - window_y) / 4,
                                                     display_height, display_width);

            btn::blending::set_transparency_alpha(0.9);
            _blending_action.emplace(open_frames, 1);

            background.show_mosaic();
            btn::sound::play(btn::sound_items::explosion_2);
            _status = status_type::OPEN;
            _counter = open_frames;
        }
        break;

    case status_type::OPEN:
        _bg_move_action.update();

        if(_move_top_left_window_action)
        {
            _move_top_left_window_action->update();

            if(_move_top_left_window_action->done())
            {
                _move_top_left_window_action.reset();
            }
        }

        if(_move_bottom_right_window_action)
        {
            _move_bottom_right_window_action->update();

            if(_move_bottom_right_window_action->done())
            {
                _move_bottom_right_window_action.reset();
            }
        }

        if(_blending_action)
        {
            _blending_action->update();

            if(_blending_action->done())
            {
                _blending_action.reset();
            }
        }

        if(_counter)
        {
            --_counter;
        }
        else
        {
            if(_move_top_left_window_action)
            {
                _move_top_left_window_action.reset();
            }

            if(_move_bottom_right_window_action)
            {
                _move_bottom_right_window_action.reset();
            }

            btn::rect_window window = btn::window::internal();
            window.set_top_left(0, 0);
            window.set_bottom_right(btn::display::height(), btn::display::width());

            background.hide_mosaic();
            _status = status_type::CLOSE;
            _counter = close_frames;
        }
        break;

    case status_type::CLOSE:
        _bg_move_action.update();

        if(_blending_action)
        {
            _blending_action->update();

            if(_blending_action->done())
            {
                _blending_action.reset();
            }
        }

        if(_counter)
        {
            --_counter;

            if(_counter == close_frames - 30)
            {
                _blending_action.emplace(close_frames - 30, 0);
            }

            if(_counter > 20 && _counter % 16 == (close_frames - 1) % 16)
            {
                btn::sound::play(btn::sound_items::flame_thrower);
            }
        }
        else
        {
            btn::window::internal().set_bottom_right(0, 0);
            background.reset_blending();
            _status = status_type::INACTIVE;
        }
        break;
    }
}

}
