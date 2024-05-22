/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_rival_cars.h"

#include "fr_stage.h"
#include "fr_models_3d.h"
#include "fr_announcer.h"
#include "fr_player_car.h"
#include "fr_race_state.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_small_car.h"

namespace fr
{

namespace
{
    constexpr bn::fixed car_y = 4;
    constexpr int bump_distance_squared = bn::fixed(17.5 * 17.5).right_shift_integer();

    constexpr bn::fixed pi_2(3.14159265358979 + 3.14159265358979);

    [[nodiscard]] int _squared_distance(int x1, int y1, int x2, int y2)
    {
        int distance_x = x2 - x1;
        int distance_y = y2 - y1;
        return (distance_x * distance_x) + (distance_y * distance_y);
    }
}

rival_cars::rival_cars(const stage& stage, models_3d& models) :
    _models(models),
    _car_sprite_3d_item(bn::sprite_items::small_car, 0)
{
    const bn::span<const rival_car_info>& rival_car_infos = stage.rival_car_infos();

    if(int num_rival_cars = rival_car_infos.size())
    {
        const bn::sprite_item& sprite_item = rival_car_infos.data()[num_rival_cars - 1].sprite_item();
        _car_sprite_3d_item.tiles().set_tiles_ref(sprite_item.tiles_item());
        _car_sprite_3d_item.palette().set_colors(sprite_item.palette_item());
        _car_sprite = &models.create_sprite(_car_sprite_3d_item);
        _active_index = num_rival_cars - 1;

        int num_checkpoints = stage.rival_checkpoints().size();
        int16_t* checkpoint_indexes_ptr = _checkpoint_indexes;

        for(const rival_car_info& car_info : rival_car_infos)
        {
            int checkpoint_index = car_info.start_checkpoint_index();

            while(checkpoint_index >= num_checkpoints)
            {
                checkpoint_index -= num_checkpoints;
            }

            *checkpoint_indexes_ptr = checkpoint_index;
            ++checkpoint_indexes_ptr;
        }

        _update_active_car(stage);
    }
}

rival_cars::~rival_cars()
{
    if(_car_sprite)
    {
        _models.destroy_sprite(*_car_sprite);
        _car_sprite = nullptr;
    }
}

void rival_cars::update(const stage& stage, const race_state& race_state, player_car& player_car,
                        announcer& announcer)
{
    if(_car_sprite)
    {
        _go_to_the_next_checkpoint(stage);
        _select_active_car(stage, race_state, player_car, announcer);
        _update_active_car(stage);
    }
}

void rival_cars::_go_to_the_next_checkpoint(const stage& stage)
{
    const bn::span<const checkpoint>& rival_checkpoints = stage.rival_checkpoints();
    int num_checkpoints = rival_checkpoints.size();
    int num_rival_cars = stage.rival_car_infos().size();
    int16_t* checkpoint_indexes_ptr = _checkpoint_indexes;

    for(int index = 0; index < num_rival_cars; ++index)
    {
        int next_checkpoint_index = *checkpoint_indexes_ptr + 1;

        if(next_checkpoint_index == num_checkpoints)
        {
            next_checkpoint_index = 0;
        }

        *checkpoint_indexes_ptr = next_checkpoint_index;
        ++checkpoint_indexes_ptr;
    }
}

void rival_cars::_select_active_car(const stage& stage, const race_state& race_state, player_car& player_car,
                                    announcer& announcer)
{
    int closest_index = race_state.closest_rival_index();

    if(closest_index != _active_index)
    {
        const bn::sprite_item& sprite_item = stage.rival_car_infos().data()[closest_index].sprite_item();
        _car_sprite_3d_item.tiles().set_tiles_ref(sprite_item.tiles_item());
        _car_sprite_3d_item.palette().set_colors(sprite_item.palette_item());
        _active_index = closest_index;
    }

    if(player_car.can_bump())
    {
        const point_3d& player_position = player_car.position();
        int player_x = player_position.x().right_shift_integer();
        int player_y = player_position.z().right_shift_integer();

        int16_t closest_checkpoint_index = _checkpoint_indexes[closest_index];
        const checkpoint& closest_checkpoint = stage.rival_checkpoints().data()[closest_checkpoint_index];
        int closest_distance = _squared_distance(player_x, player_y, closest_checkpoint.x(), closest_checkpoint.y());

        if(closest_distance <= bump_distance_squared)
        {
            player_car.bump();
            bn::sound_items::crash.play();
            announcer.play_rival_crash_voice(61);
        }
    }
}

void rival_cars::_update_active_car(const stage& stage)
{
    const bn::span<const checkpoint>& rival_checkpoints = stage.rival_checkpoints();
    const checkpoint& current_checkpoint = rival_checkpoints.data()[_checkpoint_indexes[_active_index]];
    _car_sprite->set_position(point_3d(current_checkpoint.x(), car_y, current_checkpoint.y()));

    constexpr bn::rule_of_three_approximation angle_16_rule_of_three(pi_2.data(), 65536);
    int angle_16_data = angle_16_rule_of_three.calculate(current_checkpoint.angle().data());
    _car_sprite->set_theta(-angle_16_data);
}

}
