/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SCOREBOARD_H
#define FR_SCOREBOARD_H

#include "bn_sprite_actions.h"
#include "bn_sprite_palette_actions.h"

#include "fr_scoreboard_number.h"

namespace fr
{

class stage;
class race_state;
class common_stuff;

class scoreboard
{

public:
    scoreboard(const stage& stage, common_stuff& common_stuff);

    void update(const stage& stage, const race_state& state, bn::fixed turbo_energy, bool intro_done);

private:
    bn::sprite_palette_ptr _pending_time_palette;
    bn::sprite_palette_ptr _numbers_palette;
    scoreboard_number<2> _pending_time_number;
    scoreboard_number<2> _position_number;
    scoreboard_number<1> _current_lap_number;
    scoreboard_number<1> _total_laps_number;
    scoreboard_number<3> _cpu_number;
    bn::sprite_ptr _turbo_bar_top_sprite;
    bn::sprite_ptr _turbo_bar_bottom_sprite;
    bn::sprite_ptr _inverse_sprite;
    bn::optional<bn::sprite_horizontal_scale_to_action> _turbo_bar_top_scale_action;
    bn::optional<bn::sprite_horizontal_scale_to_action> _turbo_bar_bottom_scale_action;
    bn::optional<bn::sprite_palette_fade_to_action> _turbo_bar_palette_action;
    bn::vector<bn::sprite_ptr, 6> _left_label_sprites;
    bn::vector<bn::sprite_ptr, 2> _right_label_sprites;
    bn::fixed _cpu_usage;
    int _previous_pending_time;
    int _pending_time_effect_counter = 0;
    int _turbo_graphics_index = 0;
    int _intro_counter = 0;
    int _inverse_counter = 0;
    int _cpu_counter = 0;
    bool _even_update = false;
    bool _pending_time_red_palette = false;

    void _update_pending_time(const race_state& state);

    void _update_turbo_bar(bn::fixed turbo_energy);

    void _update_turbo_bar_actions();

    void _update_intro();

    void _update_inverse(const race_state& state);
};

}

#endif
