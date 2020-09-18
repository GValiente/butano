#ifndef BF_GAME_BUTANO_BOSS_H
#define BF_GAME_BUTANO_BOSS_H

#include "btn_deque.h"
#include "btn_random.h"
#include "btn_fixed_rect.h"
#include "btn_sprite_actions.h"
#include "bf_game_boss.h"
#include "bf_game_explosion.h"

namespace bf::game
{

enum class enemy_bullet_type : int8_t;

class butano_boss : public boss
{

public:
    explicit butano_boss(const btn::sprite_palette_ptr& damage_palette);

    void play_music() const override;

protected:
    [[nodiscard]] btn::fixed_point _position() const final
    {
        return _butano_position;
    }

    void _update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                       enemy_bullets& enemy_bullets) final;

    [[nodiscard]] bool _update_dead(const btn::fixed_point& hero_position) final;

    void _show_damage_palette(const btn::sprite_palette_ptr& damage_palette) final;

    void _hide_damage_palette() final;

    [[nodiscard]] bool _hero_should_look_down_impl(const btn::fixed_point& hero_position,
                                                   bool hero_is_looking_down) const final;

private:
    btn::vector<btn::fixed_rect, 5> _butano_rects;
    btn::vector<btn::sprite_ptr, 3> _sprites;
    btn::deque<btn::sprite_animate_action<7>, 8> _mini_explosions;
    btn::optional<explosion> _explosion;
    btn::fixed_point _butano_position;
    btn::fixed_point _delta_position;
    btn::random _random;
    btn::fixed _rotation_angle = 180;
    btn::fixed _scale = 0.2;
    btn::fixed _fire_scale_mult = 1;
    int _state_index = 0;
    int _movement_index = 0;
    int _movement_counter = 0;
    int _bullets_index = 0;
    int _bullets_counter = 1;
    bool _vibrate = false;
    bool _increase_fire_scale_mult = false;

    [[nodiscard]] btn::fixed_point _top_position() const;

    void _shoot_bullet(enemy_bullet_type bullet_type, btn::fixed delta_speed,
                       const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) const;

    void _shoot_bullet(enemy_bullet_type bullet_type, const btn::fixed_point& delta_position,
                       const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) const;

    void _shoot_random_bullet(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets);

    void _update_sprites();

    void _update_rects();

    void _update_bullets(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets);

    void _update_explosions();
};

}

#endif
