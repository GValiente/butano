/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_PLAYER_CAR_H
#define FR_PLAYER_CAR_H

#include "bn_sprite_palette_actions.h"

#include "fr_sprite_3d_item.h"

namespace fr
{

class stage;
class point_3d;
class sprite_3d;
class models_3d;
class camera_3d;
class announcer;
class race_state;
class background_3d;

class player_car
{

public:
    player_car(const stage& stage, const race_state& state, models_3d& models, camera_3d& camera,
               announcer& announcer, background_3d& background);

    ~player_car();

    [[nodiscard]] const point_3d& position() const;

    [[nodiscard]] bn::fixed turbo_energy() const
    {
        return bn::max(_turbo_energy, bn::fixed(0));
    }

    [[nodiscard]] bn::array<point_3d, 5> collision_points() const;

    [[nodiscard]] bool can_bump() const
    {
        return _bump_frames == 0;
    }

    void bump();

    [[nodiscard]] bool can_crash() const
    {
        return _crash_frames == 0;
    }

    void crash();

    [[nodiscard]] int win_state() const
    {
        return _win_state;
    }

    void set_win_state(int win_state);

    void update(const stage& stage, const race_state& state, bool read_keypad, bool rumble_allowed,
                announcer& announcer, background_3d& background);

    void update_camera(const stage& stage, bool add_delay, camera_3d& camera);

private:
    models_3d& _models;
    sprite_3d_item _car_sprite_3d_item;
    sprite_3d_item _turbo_explosion_sprite_3d_item;
    bn::optional<bn::sprite_palette_fade_to_action> _palette_action;
    sprite_3d* _car_sprite = nullptr;
    sprite_3d* _turbo_sprite = nullptr;
    sprite_3d* _explosion_sprite = nullptr;
    bn::fixed _x_velocity;
    bn::fixed _y_velocity;
    bn::fixed _power;
    bn::fixed _reverse;
    bn::fixed _angle;
    bn::fixed _angle_sin;
    bn::fixed _angle_cos;
    bn::fixed _angular_velocity;
    bn::fixed _turbo_energy;
    bn::fixed _turbo_explosion_angle;
    int _rumble_frames = 0;
    int _bump_frames = 0;
    int _crash_frames = 0;
    int _win_state = 0;

    void _update_turbo_gfx(const point_3d& car_position, bool turbo_enabled);

    void _stop_turbo();

    void _update_crash(const stage& stage, const race_state& state, background_3d& background);

    void _start_crash(const stage& stage, background_3d& background);

    void _continue_crash(const stage& stage, background_3d& background);

    void _end_crash(const stage& stage, const race_state& state);
};

}

#endif
