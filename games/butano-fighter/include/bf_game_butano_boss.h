/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_BUTANO_BOSS_H
#define BF_GAME_BUTANO_BOSS_H

#include "bn_deque.h"
#include "bn_random.h"
#include "bn_display.h"
#include "bn_fixed_rect.h"
#include "bn_rect_window_actions.h"
#include "bn_sprite_animate_actions.h"
#include "bn_rect_window_boundaries_hbe_ptr.h"
#include "bf_game_boss.h"
#include "bf_circle_generator.h"

namespace bf::game
{

enum class enemy_bullet_type : int8_t;

class butano_boss : public boss
{

public:
    butano_boss(const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera);

    void play_music() const override;

protected:
    [[nodiscard]] bn::fixed_point _position() const final
    {
        return _butano_position;
    }

    void _update_alive(const bn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                       const bn::camera_ptr& camera, enemy_bullets& enemy_bullets) final;

    [[nodiscard]] bool _update_dead(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                    background& background, rumble_manager& rumble_manager) final;

    void _show_damage_palette(const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera) final;

    void _hide_damage_palette() final;

    [[nodiscard]] bool _hero_should_look_down_impl(const bn::fixed_point& hero_position,
                                                   bool hero_is_looking_down) const final;

    [[nodiscard]] bool _throw_bomb() const final;

private:
    bn::vector<bn::fixed_rect, 5> _butano_rects;
    bn::vector<bn::sprite_ptr, 3> _sprites;
    bn::deque<bn::sprite_animate_action<7>, 8> _mini_explosions;
    bn::fixed_point _butano_position;
    bn::fixed_point _delta_position;
    bn::random _random;
    bn::fixed _rotation_angle = 180;
    bn::fixed _scale = 0.2;
    bn::fixed _delta_scale;
    bn::fixed _fire_scale_mult = 1;
    int _state_index = 0;
    int _movement_index = 0;
    int _movement_counter = 0;
    int _bullets_index = 0;
    int _bullets_counter = 1;
    bool _vibrate = false;
    bool _increase_fire_scale_mult = false;

    bn::optional<bn::rect_window_move_top_by_action> _move_window_top_action;
    bn::optional<bn::rect_window_move_bottom_by_action> _move_window_bottom_action;
    bn::pair<bn::fixed, bn::fixed> _circle_hbe_deltas[bn::display::height()];
    circle_generator _circle_generator;
    bn::optional<bn::rect_window_boundaries_hbe_ptr> _circle_hbe;
    int _explosion_counter = 0;
    int _flame_sound_counter = 0;

    [[nodiscard]] bn::fixed_point _top_position() const;

    void _shoot_bullet(enemy_bullet_type bullet_type, bn::fixed delta_speed,
                       const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                       enemy_bullets& enemy_bullets) const;

    void _shoot_bullet(enemy_bullet_type bullet_type, const bn::fixed_point& delta_position,
                       const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                       enemy_bullets& enemy_bullets) const;

    void _shoot_random_bullet(bool down_only, const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                              enemy_bullets& enemy_bullets);

    void _update_sprites();

    void _update_rects();

    void _update_bullets(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                         enemy_bullets& enemy_bullets);

    void _update_explosions();
};

}

#endif
