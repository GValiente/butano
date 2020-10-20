/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_BUTANO_BOSS_H
#define BF_GAME_BUTANO_BOSS_H

#include "btn_deque.h"
#include "btn_random.h"
#include "btn_display.h"
#include "btn_fixed_rect.h"
#include "btn_rect_window_actions.h"
#include "btn_sprite_animate_actions.h"
#include "btn_rect_window_boundaries_hblank_effect_ptr.h"
#include "bf_game_boss.h"
#include "bf_circle_generator.h"

namespace bf::game
{

enum class enemy_bullet_type : int8_t;

class butano_boss : public boss
{

public:
    butano_boss(const btn::sprite_palette_ptr& damage_palette, const btn::camera_ptr& camera);

    void play_music() const override;

protected:
    [[nodiscard]] btn::fixed_point _position() const final
    {
        return _butano_position;
    }

    void _update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                       const btn::camera_ptr& camera, enemy_bullets& enemy_bullets) final;

    [[nodiscard]] bool _update_dead(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                    background& background) final;

    void _show_damage_palette(const btn::sprite_palette_ptr& damage_palette, const btn::camera_ptr& camera) final;

    void _hide_damage_palette() final;

    [[nodiscard]] bool _hero_should_look_down_impl(const btn::fixed_point& hero_position,
                                                   bool hero_is_looking_down) const final;

    [[nodiscard]] bool _throw_bomb() const final;

private:
    btn::vector<btn::fixed_rect, 5> _butano_rects;
    btn::vector<btn::sprite_ptr, 3> _sprites;
    btn::deque<btn::sprite_animate_action<7>, 8> _mini_explosions;
    btn::fixed_point _butano_position;
    btn::fixed_point _delta_position;
    btn::random _random;
    btn::fixed _rotation_angle = 180;
    btn::fixed _scale = 0.2;
    btn::fixed _delta_scale;
    btn::fixed _fire_scale_mult = 1;
    int _state_index = 0;
    int _movement_index = 0;
    int _movement_counter = 0;
    int _bullets_index = 0;
    int _bullets_counter = 1;
    bool _vibrate = false;
    bool _increase_fire_scale_mult = false;

    btn::optional<btn::rect_window_move_top_by_action> _move_window_top_action;
    btn::optional<btn::rect_window_move_bottom_by_action> _move_window_bottom_action;
    btn::pair<btn::fixed, btn::fixed> _circle_hblank_effect_deltas[btn::display::height()];
    circle_generator _circle_generator;
    btn::optional<btn::rect_window_boundaries_hblank_effect_ptr> _circle_hblank_effect;
    int _explosion_counter = 0;
    int _flame_sound_counter = 0;

    [[nodiscard]] btn::fixed_point _top_position() const;

    void _shoot_bullet(enemy_bullet_type bullet_type, btn::fixed delta_speed,
                       const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                       enemy_bullets& enemy_bullets) const;

    void _shoot_bullet(enemy_bullet_type bullet_type, const btn::fixed_point& delta_position,
                       const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                       enemy_bullets& enemy_bullets) const;

    void _shoot_random_bullet(bool down_only, const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                              enemy_bullets& enemy_bullets);

    void _update_sprites();

    void _update_rects();

    void _update_bullets(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                         enemy_bullets& enemy_bullets);

    void _update_explosions();
};

}

#endif
