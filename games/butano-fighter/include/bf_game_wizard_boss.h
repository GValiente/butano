/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_WIZARD_BOSS_H
#define BF_GAME_WIZARD_BOSS_H

#include "bn_deque.h"
#include "bn_random.h"
#include "bn_fixed_rect.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_actions.h"
#include "bf_game_boss.h"
#include "bf_game_explosion.h"

namespace bf::game
{

enum class enemy_bullet_type : int8_t;

class wizard_boss : public boss
{

public:
    wizard_boss(const bn::fixed_point& hero_position, const bn::sprite_palette_ptr& damage_palette,
                const bn::camera_ptr& camera);

protected:
    [[nodiscard]] bn::fixed_point _position() const final
    {
        return _wizard_position;
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
    bn::vector<bn::fixed_rect, 1> _wizard_rects;
    bn::vector<bn::sprite_ptr, 2> _sprites;
    bn::vector<bn::sprite_animate_action<6>, 2> _animate_actions;
    bn::vector<bn::sprite_ptr, 4> _aura_sprites;
    bn::vector<bn::sprite_animate_action<5>, 4> _aura_sprite_animate_actions;
    bn::fixed_point _wizard_position;
    bn::sprite_palette_ptr _palette;
    bn::optional<bn::sprite_palette_fade_loop_action> _palette_action;
    bn::deque<bn::sprite_animate_action<5>, 8> _mini_explosions;
    bn::optional<bn::fixed> _target_x;
    bn::optional<explosion> _explosion;
    bn::fixed_point _delta_position;
    bn::random _random;
    int _state_index = 0;
    int _movement_index = 0;
    int _movement_counter = 0;
    int _bullets_index = 0;
    int _bullets_counter = 240;
    bool _vibrate = false;

    void _shoot_bullet(enemy_bullet_type bullet_type, const bn::fixed_point& delta_position,
                       const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                       enemy_bullets& enemy_bullets) const;

    void _shoot_random_bullet(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                              enemy_bullets& enemy_bullets);

    void _update_sprites(const bn::fixed_point& hero_position);

    void _update_rects();

    void _update_bullets(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                         enemy_bullets& enemy_bullets);

    void _update_explosions();
};

}

#endif
