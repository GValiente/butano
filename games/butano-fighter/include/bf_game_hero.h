/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_HERO_H
#define BF_GAME_HERO_H

#include "bn_deque.h"
#include "bn_music_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_actions.h"
#include "bn_sprite_affine_mat_actions.h"
#include "bf_status.h"
#include "bf_game_explosion.h"

namespace bf
{
    enum class scene_type;
    class butano_background;
}

namespace bf::game
{

class objects;
class enemies;
class hero_bomb;
class background;
class enemy_bullets;
class rumble_manager;

class hero
{

public:
    hero(const bn::camera_ptr& camera, status& status);

    [[nodiscard]] bool alive() const
    {
        return ! _death_counter;
    }

    [[nodiscard]] int level() const
    {
        return _status.level();
    }

    [[nodiscard]] int experience() const
    {
        return _status.experience();
    }

    [[nodiscard]] bn::fixed next_level_experience_ratio() const
    {
        return _status.next_level_experience_ratio();
    }

    [[nodiscard]] bool add_experience(int experience)
    {
        return _status.add_experience(experience);
    }

    [[nodiscard]] int bombs_count() const
    {
        return _status.bombs_count();
    }

    [[nodiscard]] const bn::fixed_point& body_position() const
    {
        return _body_position;
    }

    [[nodiscard]] const bn::fixed_point& weapon_position() const
    {
        return _weapon_position;
    }

    [[nodiscard]] bool shooting() const
    {
        return _shooting;
    }

    void set_shooting(bool shooting)
    {
        _shooting = shooting;
    }

    [[nodiscard]] bool looking_down() const
    {
        return _looking_down;
    }

    void show_shoot(bn::color fade_color);

    [[nodiscard]] bool throw_bomb()
    {
        return _status.throw_bomb();
    }

    bn::optional<scene_type> update(const hero_bomb& hero_bomb, const enemies& enemies, enemy_bullets& enemy_bullets,
                                    objects& objects, background& background, butano_background& butano_background,
                                    bn::camera_ptr& camera, rumble_manager& rumble_manager);

private:
    struct body_snapshot_type
    {
        bn::fixed_point position;
        int16_t graphics_index = 0;
        bool looking_down = false;
    };

    constexpr static int body_snapshots_count = 16;

    status& _status;
    const bn::sprite_item& _body_sprite_item;
    bn::vector<bn::sprite_ptr, 3> _body_shadows;
    bn::sprite_cached_animate_action<2> _body_sprite_animate_action;
    bn::deque<body_snapshot_type, body_snapshots_count> _body_snapshots;
    bn::fixed_point _body_position;
    bn::fixed_point _weapon_position;
    bn::sprite_ptr _weapon_sprite;
    bn::sprite_ptr _shield_sprite;
    bn::sprite_affine_mat_ptr _bomb_sprites_affine_mat;
    bn::optional<bn::sprite_rotate_by_action> _body_rotate_action;
    bn::optional<bn::sprite_palette_fade_to_action> _body_palette_fade_action;
    bn::optional<bn::sprite_move_to_action> _weapon_move_action;
    bn::optional<bn::sprite_rotate_by_action> _weapon_rotate_action;
    bn::optional<bn::sprite_palette_fade_to_action> _weapon_palette_fade_action;
    bn::optional<bn::sprite_visible_toggle_action> _shield_toggle_action;
    bn::optional<bn::sprite_rotate_by_action> _shield_rotate_action;
    bn::vector<bn::sprite_move_by_action, constants::max_hero_bombs> _bomb_sprite_move_actions;
    bn::optional<bn::sprite_affine_mat_rotate_by_action> _bomb_sprites_rotate_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    bn::optional<explosion> _death_explosion;
    int _show_shoot_counter = 0;
    int _scale_weapon_counter = 0;
    int _body_shadows_counter = 0;
    int _shield_counter = 0;
    int _death_counter = 0;
    bool _shooting = false;
    bool _looking_down = false;
    bool _stage_done = false;

    void _move(bn::camera_ptr& camera);

    void _animate_alive(const bn::fixed_point& old_body_position);

    void _show_shield(int old_bombs_count, const bn::camera_ptr& camera, background& background);

    void _animate_shield(background& background, rumble_manager& rumble_manager);

    bn::optional<scene_type> _animate_dead(const bn::camera_ptr& camera, background& background,
                                           butano_background& butano_background, rumble_manager& rumble_manager);
};

}

#endif
