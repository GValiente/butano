/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_TANK_BOSS_H
#define BF_GAME_TANK_BOSS_H

#include "bn_deque.h"
#include "bn_random.h"
#include "bn_fixed_rect.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_actions.h"
#include "bf_game_boss.h"
#include "bf_game_explosion.h"

namespace bf::game
{

class hero;

class tank_boss : public boss
{

public:
    tank_boss(const bn::fixed_point& hero_position, const bn::sprite_palette_ptr& damage_palette,
              const bn::camera_ptr& camera);

protected:
    [[nodiscard]] bn::fixed_point _position() const final
    {
        return bn::fixed_point(0, _y);
    }

    void _update_alive(const bn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                       const bn::camera_ptr& camera, enemy_bullets& enemy_bullets) final;

    [[nodiscard]] bool _update_dead(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                    background& background, rumble_manager& rumble_manager) final;

    void _show_damage_palette(const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera) final;

    void _hide_damage_palette() final;

    [[nodiscard]] bool _hero_should_look_down_impl(const bn::fixed_point& hero_position,
                                                   bool hero_is_looking_down) const final;

private:
    bn::vector<bn::fixed_rect, 2> _tank_rects;
    bn::vector<bn::sprite_ptr, 10> _footprint_sprites;
    bn::vector<bn::sprite_ptr, 9> _base_sprites;
    bn::optional<bn::sprite_ptr> _jelly_sprite;
    bn::optional<bn::sprite_ptr> _cannon_sprite;
    bn::optional<bn::sprite_ptr> _arrow_sprite;
    bn::optional<bn::sprite_cached_animate_action<2>> _jelly_animate_action;
    bn::sprite_palette_ptr _base_palette;
    bn::optional<bn::sprite_palette_fade_loop_action> _base_palette_action;
    bn::sprite_palette_ptr _jelly_palette;
    bn::sprite_palette_ptr _cannon_palette;
    bn::optional<bn::sprite_palette_fade_loop_action> _cannon_palette_action;
    bn::deque<bn::sprite_animate_action<7>, 8> _mini_explosions;
    bn::optional<explosion> _explosion;
    bn::random _random;
    bn::fixed _y;
    bn::fixed _y_inc;
    int _state_index = 0;
    int _bullets_index = 0;
    int _bullets_counter = 240;
    int _vibration_counter = 1;
    int _jelly_damage_counter = 1;
    int _footprint_graphics_index = 0;
    int _footprint_counter = 1;
    bool _jelly_crying = false;

    [[nodiscard]] bn::fixed _calculate_y_alive();

    [[nodiscard]] bn::fixed _calculate_y_dead();

    [[nodiscard]] bn::fixed _cannon_rotation_angle_inc() const;

    void _update_footprint_sprites(bn::fixed y);

    void _update_base_sprites(bn::fixed y);

    void _update_jelly_sprite(bn::fixed y, const bn::fixed_point& hero_position);

    void _update_cannon_sprite(bn::fixed y, const bn::fixed_point& hero_position);

    void _update_rects(bn::fixed y);

    void _update_bullets(const bn::fixed_point& hero_position, bn::fixed y, const bn::camera_ptr& camera,
                         enemy_bullets& enemy_bullets);

    void _update_explosions();
};

}

#endif
