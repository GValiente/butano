#ifndef BF_GAME_TANK_BOSS_H
#define BF_GAME_TANK_BOSS_H

#include "btn_deque.h"
#include "btn_random.h"
#include "btn_fixed_rect.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_palette_actions.h"
#include "bf_game_boss.h"
#include "bf_game_explosion.h"

namespace bf::game
{

class hero;

class tank_boss : public boss
{

public:
    tank_boss(const btn::fixed_point& hero_position, const btn::sprite_palette_ptr& damage_palette);

protected:
    [[nodiscard]] btn::fixed_point _position() const final
    {
        return btn::fixed_point(0, _y);
    }

    void _update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                       enemy_bullets& enemy_bullets) final;

    [[nodiscard]] bool _update_dead(const btn::fixed_point& hero_position, background& background) final;

    void _show_damage_palette(const btn::sprite_palette_ptr& damage_palette) final;

    void _hide_damage_palette() final;

    [[nodiscard]] bool _hero_should_look_down_impl(const btn::fixed_point& hero_position,
                                                   bool hero_is_looking_down) const final;

private:
    btn::vector<btn::fixed_rect, 2> _tank_rects;
    btn::vector<btn::sprite_ptr, 10> _footprint_sprites;
    btn::vector<btn::sprite_ptr, 9> _base_sprites;
    btn::optional<btn::sprite_ptr> _jelly_sprite;
    btn::optional<btn::sprite_ptr> _cannon_sprite;
    btn::optional<btn::sprite_ptr> _arrow_sprite;
    btn::optional<btn::sprite_cached_animate_action<2>> _jelly_animate_action;
    btn::sprite_palette_ptr _base_palette;
    btn::optional<btn::sprite_palette_fade_loop_action> _base_palette_action;
    btn::sprite_palette_ptr _jelly_palette;
    btn::sprite_palette_ptr _cannon_palette;
    btn::optional<btn::sprite_palette_fade_loop_action> _cannon_palette_action;
    btn::deque<btn::sprite_animate_action<7>, 8> _mini_explosions;
    btn::optional<explosion> _explosion;
    btn::random _random;
    btn::fixed _y;
    btn::fixed _y_inc;
    int _state_index = 0;
    int _bullets_index = 0;
    int _bullets_counter = 240;
    int _vibration_counter = 1;
    int _jelly_damage_counter = 1;
    int _footprint_graphics_index = 0;
    int _footprint_counter = 1;
    bool _jelly_crying = false;

    [[nodiscard]] btn::fixed _calculate_y_alive();

    [[nodiscard]] btn::fixed _calculate_y_dead();

    [[nodiscard]] btn::fixed _cannon_rotation_angle_inc() const;

    void _update_footprint_sprites(btn::fixed y);

    void _update_base_sprites(btn::fixed y);

    void _update_jelly_sprite(btn::fixed y, const btn::fixed_point& hero_position);

    void _update_cannon_sprite(btn::fixed y, const btn::fixed_point& hero_position);

    void _update_rects(btn::fixed y);

    void _update_bullets(const btn::fixed_point& hero_position, btn::fixed y, enemy_bullets& enemy_bullets);

    void _update_explosions();
};

}

#endif
