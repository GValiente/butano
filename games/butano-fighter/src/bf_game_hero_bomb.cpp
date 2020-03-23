#include "bf_game_hero_bomb.h"

#include "btn_sound.h"
#include "btn_keypad.h"
#include "btn_window.h"
#include "btn_blending.h"
#include "btn_regular_bg_builder.h"
#include "btn_hero_bomb_bg_item.h"
#include "btn_sound_items.h"
#include "bf_constants.h"
#include "bf_game_hero.h"
#include "bf_wave_generator.h"
#include "bf_game_background.h"

namespace bf
{

namespace
{
    constexpr const int open_frames = 30;
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
    _bg_move_action(_bg, -0.5, 4),
    _hblank_effect(btn::regular_bg_position_hblank_effect_ptr::create_horizontal(_bg, _hblank_effect_deltas))
{
    btn::window::outside().set_show_bg(_bg, false);

    wave_generator().generate(_hblank_effect_deltas);
    _hblank_effect.reload_deltas_ref();
    _hblank_effect.set_visible(false);
}

void game_hero_bomb::update(game_hero& hero, game_background& background)
{
    switch(_status)
    {

    case status_type::INACTIVE:
        if(btn::keypad::pressed(btn::keypad::button_type::A) && hero.throw_bomb())
        {
            btn::rect_window window = btn::window::internal();
            btn::fixed window_y = hero.weapon_position().y();
            window.set_boundaries(window_y, -constants::view_width, window_y, constants::view_width);
            _move_window_top_action.emplace(window, -4);
            _move_window_bottom_action.emplace(window, 4);

            background.hide_blending();
            btn::blending::set_transparency_alpha(0.9);
            _blending_action.emplace(open_frames, 1);

            background.show_mosaic();
            _hblank_effect.set_visible(true);
            btn::sound::play(btn::sound_items::explosion_2);
            _status = status_type::OPEN;
            _counter = open_frames;
        }
        break;

    case status_type::OPEN:
        _bg_move_action.update();
        _move_window_top_action->update();
        _move_window_bottom_action->update();

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
            _move_window_top_action.reset();
            _move_window_bottom_action.reset();

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
                background.show_hblank_effect(close_frames - 50);
            }
            else if(_counter == 20)
            {
                btn::window::internal().set_boundaries(0, 0, 0, 0);
                _blending_action.reset();
                background.show_blending();
                _hblank_effect.set_visible(false);
            }

            if(_counter > 40 && _counter % 16 == (close_frames - 1) % 16)
            {
                btn::sound::play(btn::sound_items::flame_thrower);
            }
        }
        else
        {
            _status = status_type::INACTIVE;
        }
        break;
    }
}

}
