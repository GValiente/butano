/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_RIVAL_CARS_H
#define FR_RIVAL_CARS_H

#include "fr_constants_3d.h"
#include "fr_sprite_3d_item.h"

namespace fr
{

class stage;
class sprite_3d;
class models_3d;
class announcer;
class player_car;
class race_state;

class rival_cars
{

public:
    rival_cars(const stage& stage, models_3d& models);

    ~rival_cars();

    [[nodiscard]] const int16_t* checkpoint_indexes() const
    {
        return _checkpoint_indexes;
    }

    void update(const stage& stage, const race_state& race_state, player_car& player_car, announcer& announcer);

private:
    models_3d& _models;
    int16_t _checkpoint_indexes[constants_3d::max_rival_cars];
    sprite_3d_item _car_sprite_3d_item;
    sprite_3d* _car_sprite = nullptr;
    int _active_index;

    void _go_to_the_next_checkpoint(const stage& stage);

    void _select_active_car(const stage& stage, const race_state& race_state, player_car& player_car,
                            announcer& announcer);

    void _update_active_car(const stage& stage);
};

}

#endif
