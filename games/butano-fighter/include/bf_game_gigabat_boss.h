#ifndef BF_GAME_GIGABAT_BOSS_H
#define BF_GAME_GIGABAT_BOSS_H

#include "btn_deque.h"
#include "btn_random.h"
#include "btn_fixed_rect.h"
#include "btn_sprite_animate_actions.h"
#include "btn_sprite_palette_actions.h"
#include "bf_game_boss.h"
#include "bf_game_explosion.h"

namespace bf::game
{

enum class enemy_bullet_type : int8_t;

class gigabat_boss : public boss
{

public:
    gigabat_boss(const btn::fixed_point& hero_position, const btn::sprite_palette_ptr& damage_palette,
                 const btn::camera_ptr& camera);

protected:
    [[nodiscard]] btn::fixed_point _position() const final
    {
        return _gigabat_position;
    }

    void _update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                       const btn::camera_ptr& camera, enemy_bullets& enemy_bullets) final;

    [[nodiscard]] bool _update_dead(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                    background& background) final;

    void _show_damage_palette(const btn::sprite_palette_ptr& damage_palette, const btn::camera_ptr& camera) final;

    void _hide_damage_palette() final;

    [[nodiscard]] bool _hero_should_look_down_impl(const btn::fixed_point& hero_position,
                                                   bool hero_is_looking_down) const final;

private:
    btn::vector<btn::fixed_rect, 1> _gigabat_rects;
    btn::vector<btn::sprite_ptr, 4> _sprites;
    btn::optional<btn::sprite_ptr> _shadow_sprite;
    btn::vector<btn::sprite_animate_action<5>, 2> _animate_actions;
    btn::fixed_point _gigabat_position;
    btn::sprite_palette_ptr _palette;
    btn::optional<btn::sprite_palette_fade_loop_action> _palette_action;
    btn::deque<btn::sprite_animate_action<5>, 8> _mini_explosions;
    btn::optional<btn::fixed> _target_x;
    btn::optional<explosion> _explosion;
    btn::fixed_point _delta_position;
    btn::random _random;
    int _state_index = 0;
    int _movement_index = 0;
    int _movement_counter = 0;
    int _bullets_index = 0;
    int _bullets_counter = 240;
    bool _vibrate = false;

    void _shoot_bullet(enemy_bullet_type bullet_type, const btn::fixed_point& delta_position,
                       const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                       enemy_bullets& enemy_bullets) const;

    void _shoot_target_random_bullet(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                     enemy_bullets& enemy_bullets);

    void _shoot_free_random_bullet(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                   enemy_bullets& enemy_bullets);

    void _update_sprites(const btn::fixed_point& hero_position, bool hide_shadow);

    void _update_rects();

    void _update_bullets(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                         enemy_bullets& enemy_bullets);

    void _update_explosions();
};

}

#endif
