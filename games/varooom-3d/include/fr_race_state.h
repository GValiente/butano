/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_RACE_STATE_H
#define FR_RACE_STATE_H

#include "fr_constants_3d.h"

namespace fr
{

class stage;
class announcer;
class player_car;
class rival_cars;

class race_state
{

public:
    explicit race_state(const stage& stage);

    [[nodiscard]] int pending_time() const
    {
        return _pending_time;
    }

    [[nodiscard]] int current_lap() const
    {
        return _current_lap;
    }

    [[nodiscard]] int current_position() const
    {
        return _current_position;
    }

    [[nodiscard]] int current_time() const
    {
        return _current_time;
    }

    [[nodiscard]] int inverse_frames() const
    {
        return _inverse_frames;
    }

    [[nodiscard]] int next_checkpoint_index() const
    {
        return _next_checkpoint_index;
    }

    [[nodiscard]] int closest_rival_index() const
    {
        return _closest_rival_index;
    }

    void update(const stage& stage, const player_car& player_car, const rival_cars& rival_cars, announcer& announcer);

private:
    struct rival_position
    {
        uint8_t current_lap;
        uint8_t next_checkpoint_index;
        bool next_checkpoint_above;
    };

    rival_position _rival_positions[constants_3d::max_rival_cars];
    int _current_position;
    int _current_time = 0;
    int _pending_time;
    int _pending_updates = 30;
    int _current_lap = 0;
    int _inverse_frames = 0;
    int _previous_checkpoint_index;
    int _next_checkpoint_index;
    int _closest_rival_index;
    int _player_overtake_counter = 0;
    int _rival_overtake_counter = 0;
    bool _previous_checkpoint_above;
    bool _next_checkpoint_above;

    void _update_player_checkpoint(const stage& stage, const player_car& player_car, announcer& announcer);

    void _update_rival_checkpoints(const stage& stage, const rival_cars& rival_cars);

    void _update_player_position(const stage& stage, const player_car& player_car, const rival_cars& rival_cars,
                                 announcer& announcer);

    void _update_pending_time(announcer& announcer);
};

}

#endif
