#include "bf_game_hero_bomb.h"

#include "btn_sound.h"
#include "btn_keypad.h"
#include "btn_window.h"
#include "btn_blending.h"
#include "btn_regular_bg_builder.h"
#include "btn_hero_bomb_bg_item.h"
#include "btn_sound_items.h"
#include "bf_game_hero.h"

namespace bf
{

namespace
{
    btn::regular_bg_ptr _create_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::hero_bomb);
        builder.set_priority(1);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }

    btn::bgs_mosaic_stretch_loop_action _create_bgs_mosaic_action()
    {
        return btn::bgs_mosaic_stretch_loop_action(4, 0.5);
    }
}

game_hero_bomb::game_hero_bomb() :
    _bg(_create_bg()),
    _bg_move_action(_bg, -0.5, 4),
    _bgs_mosaic_action(_create_bgs_mosaic_action())
{
    btn::window::outside().set_show_bg(_bg, false);
}

void game_hero_bomb::update(game_hero& hero)
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

            _old_blending_transparency_alpha = btn::blending::transparency_alpha();
            btn::blending::set_transparency_alpha(0.9);

            btn::sound::play(btn::sound_items::explosion_2);
            _status = status_type::OPEN;
            _counter = 35;
        }
        break;

    case status_type::OPEN:
        _bg_move_action.update();
        _bgs_mosaic_action.update();

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
            btn::sound::play(btn::sound_items::flame_thrower);
            _status = status_type::CLOSE;
            _counter = 175;
        }
        break;

    case status_type::CLOSE:
        _bg_move_action.update();
        _bgs_mosaic_action.update();

        if(_counter)
        {
            --_counter;

            if(_counter && _counter % 16 == 0)
            {
                btn::sound::play(btn::sound_items::flame_thrower);
            }
        }
        else
        {
            btn::window::internal().set_bottom_right(0, 0);
            btn::blending::set_transparency_alpha(_old_blending_transparency_alpha);
            btn::bgs_mosaic::set_stretch(0);
            _bgs_mosaic_action = _create_bgs_mosaic_action();
            _status = status_type::INACTIVE;
        }
        break;
    }
}

}
