/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_race_outro.h"

#include "bn_colors.h"

#include "fr_stage.h"
#include "fr_announcer.h"
#include "fr_player_car.h"
#include "fr_race_state.h"
#include "fr_scene_type.h"
#include "fr_common_stuff.h"

#include "bn_music_items.h"

namespace fr
{

bn::optional<scene_type> race_outro::update(const stage& stage, const race_state& state, player_car& player_car,
                                            common_stuff& common_stuff, announcer& announcer)
{
    bn::optional<scene_type> result;

    if(_win_active)
    {
        result = _update_win(player_car, announcer);
    }
    else if(_lose_active)
    {
        result = _update_lose();
    }
    else if(player_car.can_crash())
    {
        if(state.current_lap() >= stage.total_laps())
        {
            common_stuff.set_finish_position_and_time(state.current_position(), state.current_time());
            _win_active = true;

            bn::bg_palettes::set_fade(bn::colors::white, 1);
            bn::sprite_palettes::set_fade(bn::colors::white, 1);
            bn::music_items::win.play(0.35);
            announcer.clear_queue();
        }
        else if(state.pending_time() == 0)
        {
            const point_3d& car_position = player_car.position();
            bn::fixed car_x = car_position.x();
            bn::fixed car_y = car_position.z();

            if(car_x == _car_x && car_y == _car_y)
            {
                ++_stopped_car_frames;

                if(_stopped_car_frames == 10)
                {
                    _lose_active = true;

                    bn::bg_palettes::set_fade(bn::colors::black, 0);
                    bn::sprite_palettes::set_fade(bn::colors::black, 0);
                    _bg_fade_action.emplace(60, 1);
                    _sprite_fade_action.emplace(60, 1);

                    if(bn::music::playing())
                    {
                        _volume_action.emplace(60, 0);
                    }
                }
            }
            else
            {
                _car_x = car_x;
                _car_y = car_y;
                _stopped_car_frames = 0;
            }
        }
    }

    return result;
}

void race_outro::_set_win_state(int win_state, player_car& player_car)
{
    bn::bg_palettes::set_fade(bn::colors::white, 1);
    bn::sprite_palettes::set_fade(bn::colors::white, 1);
    _bg_fade_action.emplace(24, 0);
    _sprite_fade_action.emplace(24, 0);
    player_car.set_win_state(win_state);
}

bn::optional<scene_type> race_outro::_update_win(player_car& player_car, announcer& announcer)
{
    bn::optional<scene_type> result;

    if(_bg_fade_action)
    {
        _bg_fade_action->update();
        _sprite_fade_action->update();

        if(_bg_fade_action->done())
        {
            _bg_fade_action.reset();
            _sprite_fade_action.reset();
            _win_counter = 6;
        }
    }
    else
    {
        if(_win_counter)
        {
            --_win_counter;
        }
        else
        {
            int win_state = player_car.win_state();

            if(win_state < 4)
            {
                if(win_state == 1)
                {
                    announcer.play_finish_voice();
                }

                _set_win_state(win_state + 1, player_car);
            }
            else
            {
                bn::bg_palettes::set_transparent_color(bn::colors::white);
                result = scene_type::WIN;
            }
        }
    }

    return result;
}

bn::optional<scene_type> race_outro::_update_lose()
{
    bn::optional<scene_type> result;

    if(_bg_fade_action)
    {
        _bg_fade_action->update();
        _sprite_fade_action->update();

        if(_volume_action)
        {
            _volume_action->update();
        }

        if(_bg_fade_action->done())
        {
            _bg_fade_action.reset();
            _sprite_fade_action.reset();

            if(_volume_action)
            {
                bn::music::stop();
                _volume_action.reset();
            }

            result = scene_type::LOSE;
        }
    }

    return result;
}

}
