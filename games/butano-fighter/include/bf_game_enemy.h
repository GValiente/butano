/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_H
#define BF_GAME_ENEMY_H

#include "bn_fixed_rect_fwd.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bf_game_explosion.h"

namespace bf::game
{

class hero;
class enemy_event;
class enemy_bullets;
class check_hero_bullet_data;

class enemy
{

public:
    enemy(const enemy_event& event, const bn::sprite_palette_ptr& damage_palette, int8_t tag,
          const bn::camera_ptr& camera);

    [[nodiscard]] bn::fixed_point top_left() const;

    [[nodiscard]] int grid_columns() const
    {
        return _grid_columns;
    }

    [[nodiscard]] int grid_rows() const
    {
        return _grid_rows;
    }

    [[nodiscard]] int last_grid_column() const
    {
        return _last_grid_column;
    }

    void set_last_grid_column(int last_grid_column)
    {
        _last_grid_column = int8_t(last_grid_column);
    }

    [[nodiscard]] int last_grid_row() const
    {
        return _last_grid_row;
    }

    void set_last_grid_row(int last_grid_row)
    {
        _last_grid_row = int8_t(last_grid_row);
    }

    [[nodiscard]] int tag() const
    {
        return _tag;
    }

    [[nodiscard]] bool check_hero(const bn::fixed_rect& hero_rect) const;

    [[nodiscard]] bool check_hero_bullet(const check_hero_bullet_data& data);

    void check_hero_bomb(const bn::point& bomb_center, int bomb_squared_radius, const bn::camera_ptr& camera);

    [[nodiscard]] bool done() const;

    void update(const bn::fixed_point& hero_position, const bn::camera_ptr& camera, enemy_bullets& enemy_bullets);

private:
    const enemy_event* _event;
    bn::sprite_ptr _sprite;
    bn::sprite_move_by_action _move_action;
    bn::sprite_animate_action<4> _animate_action;
    bn::optional<bn::sprite_rotate_by_action> _rotate_action;
    bn::optional<bn::sprite_horizontal_scale_to_action> _horizontal_scale_action;
    bn::optional<bn::sprite_vertical_scale_to_action> _vertical_scale_action;
    bn::optional<bn::sprite_animate_action<7>> _mini_explosion;
    bn::optional<explosion> _explosion;
    bn::sprite_palette_ptr _sprite_palette;
    bn::sprite_palette_ptr _damage_palette;
    int16_t _life;
    int16_t _move_event_counter;
    int16_t _bullet_event_counter;
    int8_t _move_event_index;
    int8_t _bullet_event_index;
    int8_t _grid_columns;
    int8_t _grid_rows;
    int8_t _last_grid_column;
    int8_t _last_grid_row;
    int8_t _damage_palette_counter;
    int8_t _ignore_hero_bullet_counter;
    int8_t _tag;

    [[nodiscard]] bool _is_outside() const;

    void _add_damage(const bn::fixed_point& enemy_position, bn::fixed attack_x, int damage,
                     const bn::camera_ptr& camera);

    void _show_rotate_death(const bn::fixed_point& enemy_position, bn::fixed attack_x);
};

}

#endif
