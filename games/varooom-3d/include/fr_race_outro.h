/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_RACE_OUTRO_H
#define FR_RACE_OUTRO_H

#include "bn_music_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "bn_sprite_palettes_actions.h"

namespace fr
{

class stage;
class announcer;
class player_car;
class race_state;
class common_stuff;
enum class scene_type;

class race_outro
{

public:
    [[nodiscard]] bool active() const
    {
        return _win_active || _lose_active;
    }

    [[nodiscard]] bn::optional<scene_type> update(
            const stage& stage, const race_state& state, player_car& player_car, common_stuff& common_stuff,
            announcer& announcer);

private:
    bn::optional<bn::bg_palettes_fade_to_action> _bg_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprite_fade_action;
    bn::optional<bn::music_volume_to_action> _volume_action;
    bn::fixed _car_x;
    bn::fixed _car_y;
    int _win_counter = 0;
    int _stopped_car_frames = 0;
    bool _win_active = false;
    bool _lose_active = false;

    void _set_win_state(int win_state, player_car& player_car);

    [[nodiscard]] bn::optional<scene_type> _update_win(player_car& player_car, announcer& announcer);

    [[nodiscard]] bn::optional<scene_type> _update_lose();
};

}

#endif
