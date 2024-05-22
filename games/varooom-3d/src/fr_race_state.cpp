/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_race_state.h"

#include "fr_stage.h"
#include "fr_announcer.h"
#include "fr_player_car.h"
#include "fr_rival_cars.h"

#include "bn_sound_items.h"

namespace fr
{

namespace
{
    [[nodiscard]] bool _car_above_checkpoint(int car_x, int car_y, const checkpoint& checkpoint,
                                             const bn::point& checkpoint_vector)
    {
        int car_vector_x = car_x - checkpoint.x();
        int car_vector_y = car_y - checkpoint.y();
        int cross_product = (car_vector_x * checkpoint_vector.y()) - (car_vector_y * checkpoint_vector.x());
        return cross_product < 0;
    }

    [[nodiscard]] bool _car_above_checkpoint(const point_3d& car_position, const checkpoint& checkpoint,
                                             const bn::point& checkpoint_vector)
    {
        return _car_above_checkpoint(car_position.x().right_shift_integer(), car_position.z().right_shift_integer(),
                                     checkpoint, checkpoint_vector);
    }

    [[nodiscard]] int _squared_distance(int car_x, int car_y, int checkpoint_x, int checkpoint_y)
    {
        int distance_x = checkpoint_x - car_x;
        int distance_y = checkpoint_y - car_y;
        return (distance_x * distance_x) + (distance_y * distance_y);
    }
}

race_state::race_state(const stage& stage) :
    _current_position(stage.rival_car_infos().size()),
    _pending_time(stage.total_time()),
    _previous_checkpoint_index(stage.player_checkpoints().size() - 3),
    _next_checkpoint_index(1),
    _closest_rival_index(stage.rival_car_infos().size() - 1),
    _previous_checkpoint_above(
        _car_above_checkpoint(stage.start_position(), stage.player_checkpoints()[_previous_checkpoint_index],
                              stage.player_checkpoint_vectors()[_previous_checkpoint_index])),
    _next_checkpoint_above(
        _car_above_checkpoint(stage.start_position(), stage.player_checkpoints()[_next_checkpoint_index],
                              stage.player_checkpoint_vectors()[_next_checkpoint_index]))
{
    const bn::span<const checkpoint>& player_checkpoints = stage.player_checkpoints();
    const checkpoint* player_checkpoints_data = player_checkpoints.data();
    const bn::point* player_checkpoint_vectors_data = stage.player_checkpoint_vectors().data();
    const checkpoint* rival_checkpoints_data = stage.rival_checkpoints().data();
    int num_player_checkpoints = player_checkpoints.size();
    int num_rival_checkpoints = stage.rival_checkpoints().size();
    int rival_index = 0;

    for(const rival_car_info& rival_car : stage.rival_car_infos())
    {
        int rival_checkpoint_index = 0;
        int current_lap = 0;
        int next_checkpoint_index = 1;
        bool next_checkpoint_above = _next_checkpoint_above;

        for(int index = 0, limit = rival_car.start_checkpoint_index(); index < limit; ++index)
        {
            const checkpoint& rival_checkpoint = rival_checkpoints_data[rival_checkpoint_index];
            int rival_car_x = rival_checkpoint.x();
            int rival_car_y = rival_checkpoint.y();
            ++rival_checkpoint_index;

            if(rival_checkpoint_index == num_rival_checkpoints)
            {
                rival_checkpoint_index = 0;
            }

            bool above = _car_above_checkpoint(rival_car_x, rival_car_y,
                                               player_checkpoints_data[next_checkpoint_index],
                                               player_checkpoint_vectors_data[next_checkpoint_index]);

            if(above != next_checkpoint_above)
            {
                if(next_checkpoint_index == 0)
                {
                    next_checkpoint_index = 1;
                    ++current_lap;
                }
                else
                {
                    bool last_checkpoint = next_checkpoint_index == num_player_checkpoints - 1;
                    next_checkpoint_index = last_checkpoint ? 0 : next_checkpoint_index + 1;
                }

                next_checkpoint_above = _car_above_checkpoint(rival_car_x, rival_car_y,
                                                              player_checkpoints_data[next_checkpoint_index],
                                                              player_checkpoint_vectors_data[next_checkpoint_index]);
            }
        }

        rival_position& rival = _rival_positions[rival_index];
        rival.current_lap = current_lap;
        rival.next_checkpoint_index = next_checkpoint_index;
        rival.next_checkpoint_above = next_checkpoint_above;
        ++rival_index;
    }
}

void race_state::update(const stage& stage, const player_car& player_car, const rival_cars& rival_cars,
                        announcer& announcer)
{
    _update_player_checkpoint(stage, player_car, announcer);
    _update_rival_checkpoints(stage, rival_cars);
    _update_player_position(stage, player_car, rival_cars, announcer);
    _update_pending_time(announcer);
    _current_time += 2;
}

void race_state::_update_player_checkpoint(const stage& stage, const player_car& player_car, announcer& announcer)
{
    const point_3d& car_position = player_car.position();
    const bn::span<const checkpoint>& checkpoints = stage.player_checkpoints();
    const checkpoint* checkpoints_data = checkpoints.data();
    const bn::point* checkpoint_vectors_data = stage.player_checkpoint_vectors().data();
    int car_x = car_position.x().right_shift_integer();
    int car_y = car_position.z().right_shift_integer();
    int next_checkpoint_index = _next_checkpoint_index;
    bool above = _car_above_checkpoint(car_x, car_y, checkpoints_data[next_checkpoint_index],
                                       checkpoint_vectors_data[next_checkpoint_index]);

    if(above != _next_checkpoint_above)
    {
        _previous_checkpoint_index = next_checkpoint_index;
        _previous_checkpoint_above = above;

        if(next_checkpoint_index == 0)
        {
            next_checkpoint_index = 1;

            int current_lap = _current_lap + 1;
            _current_lap = current_lap;

            if(int pending_laps = stage.total_laps() - current_lap)
            {
                _pending_time += stage.time_increase();
                bn::sound_items::checkpoint02.play();
                announcer.play_new_lap(pending_laps, 43);
            }
        }
        else
        {
            if(next_checkpoint_index == stage.middle_player_checkpoint_index())
            {
                _pending_time += stage.time_increase();
                bn::sound_items::lap04.play();
                announcer.play_new_middle_checkpoint(41);
            }

            bool last_checkpoint = next_checkpoint_index == checkpoints.size() - 1;
            next_checkpoint_index = last_checkpoint ? 0 : next_checkpoint_index + 1;
        }

        _next_checkpoint_index = next_checkpoint_index;
        _next_checkpoint_above = _car_above_checkpoint(car_x, car_y, checkpoints_data[next_checkpoint_index],
                                                       checkpoint_vectors_data[next_checkpoint_index]);
    }
    else
    {
        int previous_checkpoint_index = _previous_checkpoint_index;
        above = _car_above_checkpoint(car_x, car_y, checkpoints_data[previous_checkpoint_index],
                                      checkpoint_vectors_data[previous_checkpoint_index]);

        if(above != _previous_checkpoint_above)
        {
            ++_inverse_frames;
        }
        else
        {
            _inverse_frames = 0;
        }
    }
}

void race_state::_update_rival_checkpoints(const stage& stage, const rival_cars& rival_cars)
{
    const bn::span<const checkpoint>& player_checkpoints = stage.player_checkpoints();
    const checkpoint* player_checkpoints_data = player_checkpoints.data();
    const bn::point* player_checkpoint_vectors_data = stage.player_checkpoint_vectors().data();
    int num_player_checkpoints = player_checkpoints.size();

    const checkpoint* rival_checkpoints_data = stage.rival_checkpoints().data();
    const int16_t* rival_checkpoint_indexes = rival_cars.checkpoint_indexes();
    int num_rivals = stage.rival_car_infos().size();

    for(int rival_index = 0; rival_index != num_rivals; ++rival_index)
    {
        rival_position& rival = _rival_positions[rival_index];
        int next_checkpoint_index = rival.next_checkpoint_index;

        const checkpoint& rival_checkpoint = rival_checkpoints_data[rival_checkpoint_indexes[rival_index]];
        int rival_car_x = rival_checkpoint.x();
        int rival_car_y = rival_checkpoint.y();

        bool above = _car_above_checkpoint(rival_car_x, rival_car_y,
                                           player_checkpoints_data[next_checkpoint_index],
                                           player_checkpoint_vectors_data[next_checkpoint_index]);

        if(above != rival.next_checkpoint_above)
        {
            if(next_checkpoint_index == 0)
            {
                next_checkpoint_index = 1;
                ++rival.current_lap;
            }
            else
            {
                bool last_checkpoint = next_checkpoint_index == num_player_checkpoints - 1;
                next_checkpoint_index = last_checkpoint ? 0 : next_checkpoint_index + 1;
            }

            rival.next_checkpoint_index = next_checkpoint_index;
            rival.next_checkpoint_above = _car_above_checkpoint(
                        rival_car_x, rival_car_y,
                        player_checkpoints_data[next_checkpoint_index],
                        player_checkpoint_vectors_data[next_checkpoint_index]);
        }
    }
}

void race_state::_update_player_position(const stage& stage, const player_car& player_car,
                                         const rival_cars& rival_cars, announcer& announcer)
{
    const checkpoint* player_checkpoints_data = stage.player_checkpoints().data();
    int num_player_checkpoints = stage.player_checkpoints().size();
    int player_position = _current_position;
    int player_lap = _current_lap;
    int player_next_checkpoint_index = _next_checkpoint_index;
    int player_current_checkpoint_index = player_next_checkpoint_index - 1;

    if(player_current_checkpoint_index < 0)
    {
        player_current_checkpoint_index += num_player_checkpoints;
    }

    const checkpoint* rival_checkpoints_data = stage.rival_checkpoints().data();
    const int16_t* rival_checkpoint_indexes = rival_cars.checkpoint_indexes();
    int player_checkpoint_position = (player_lap * num_player_checkpoints) + player_current_checkpoint_index;
    int num_rivals = stage.rival_car_infos().size();
    int previous_rival_index = player_position;
    int previous_rival_distance = bn::numeric_limits<int>::max();
    int next_rival_index = player_position - 1;
    int next_rival_distance = bn::numeric_limits<int>::max();

    if(player_position < num_rivals)
    {
        const rival_position& previous_rival = _rival_positions[previous_rival_index];
        int previous_rival_current_checkpoint_index = previous_rival.next_checkpoint_index - 1;

        if(previous_rival_current_checkpoint_index < 0)
        {
            previous_rival_current_checkpoint_index += num_player_checkpoints;
        }

        int previous_rival_current_lap = previous_rival.current_lap;
        int previous_rival_checkpoint_position = (previous_rival_current_lap * num_player_checkpoints) +
                previous_rival_current_checkpoint_index;
        previous_rival_distance = bn::abs(previous_rival_checkpoint_position - player_checkpoint_position);

        if(previous_rival_current_lap > player_lap)
        {
            ++_current_position;
        }
        else if(previous_rival_current_lap == player_lap)
        {
            if(previous_rival_current_checkpoint_index > player_current_checkpoint_index)
            {
                ++_current_position;
            }
            else if(previous_rival_current_checkpoint_index == player_current_checkpoint_index)
            {
                const checkpoint& next_checkpoint = player_checkpoints_data[player_next_checkpoint_index];
                int next_x = next_checkpoint.x();
                int next_y = next_checkpoint.y();

                const point_3d& player_car_position = player_car.position();
                int player_x = player_car_position.x().right_shift_integer();
                int player_y = player_car_position.z().right_shift_integer();
                int player_distance = _squared_distance(player_x, player_y, next_x, next_y);

                const checkpoint& rival_checkpoint = rival_checkpoints_data[rival_checkpoint_indexes[previous_rival_index]];
                int rival_x = rival_checkpoint.x();
                int rival_y = rival_checkpoint.y();
                int rival_distance = _squared_distance(rival_x, rival_y, next_x, next_y);

                if(player_distance > rival_distance)
                {
                    ++_current_position;
                }
            }
        }
    }

    if(player_position != _current_position)
    {
        _player_overtake_counter = 0;
        _rival_overtake_counter = 15;
    }
    else if(player_position)
    {
        const rival_position& next_rival = _rival_positions[next_rival_index];
        int next_rival_current_checkpoint_index = next_rival.next_checkpoint_index - 1;

        if(next_rival_current_checkpoint_index < 0)
        {
            next_rival_current_checkpoint_index += num_player_checkpoints;
        }

        int next_rival_current_lap = next_rival.current_lap;
        int next_rival_checkpoint_position = (next_rival_current_lap * num_player_checkpoints) +
                next_rival_current_checkpoint_index;
        next_rival_distance = bn::abs(next_rival_checkpoint_position - player_checkpoint_position);

        if(next_rival_current_lap < player_lap)
        {
            --_current_position;
        }
        else if(next_rival_current_lap == player_lap)
        {
            if(next_rival_current_checkpoint_index < player_current_checkpoint_index)
            {
                --_current_position;
            }
            else if(next_rival_current_checkpoint_index == player_current_checkpoint_index)
            {
                const checkpoint& next_checkpoint = player_checkpoints_data[player_next_checkpoint_index];
                int next_x = next_checkpoint.x();
                int next_y = next_checkpoint.y();

                const point_3d& player_car_position = player_car.position();
                int player_x = player_car_position.x().right_shift_integer();
                int player_y = player_car_position.z().right_shift_integer();
                int player_distance = _squared_distance(player_x, player_y, next_x, next_y);

                const checkpoint& rival_checkpoint = rival_checkpoints_data[rival_checkpoint_indexes[next_rival_index]];
                int rival_x = rival_checkpoint.x();
                int rival_y = rival_checkpoint.y();
                int rival_distance = _squared_distance(rival_x, rival_y, next_x, next_y);

                if(player_distance < rival_distance)
                {
                    --_current_position;
                }
            }
        }

        if(player_position != _current_position)
        {
            _player_overtake_counter = 15;
            _rival_overtake_counter = 0;
        }
    }

    if(previous_rival_distance < next_rival_distance)
    {
        _closest_rival_index = previous_rival_index;
    }
    else if(next_rival_distance < previous_rival_distance)
    {
        _closest_rival_index = next_rival_index;
    }

    if(_player_overtake_counter)
    {
        --_player_overtake_counter;

        if(! _player_overtake_counter)
        {
            announcer.play_player_overtake_voice(_current_position);
        }
    }

    if(_rival_overtake_counter)
    {
        --_rival_overtake_counter;

        if(! _rival_overtake_counter)
        {
            announcer.play_rival_overtake_voice();
        }
    }
}

void race_state::_update_pending_time(announcer& announcer)
{
    --_pending_updates;

    if(! _pending_updates)
    {
        int pending_time = _pending_time - 1;
        _pending_updates = 30;

        if(pending_time >= 0)
        {
            _pending_time = pending_time;

            if(pending_time == 9)
            {
                bn::sound_items::something01.play();
                announcer.play_pending_time_warning(24);
            }
            else if(pending_time < 9)
            {
                bn::sound_items::something01.play();
            }
        }
    }
}

}
