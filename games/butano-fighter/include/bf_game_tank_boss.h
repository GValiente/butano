#ifndef BF_GAME_TANK_BOSS_H
#define BF_GAME_TANK_BOSS_H

#include "btn_optional.h"
#include "btn_fixed_rect.h"
#include "btn_sprite_actions.h"
#include "bf_game_boss.h"

namespace bf::game
{

class tank_boss : public boss
{

public:
    tank_boss(const btn::fixed_point& hero_position, const btn::sprite_palette_ptr& damage_palette);

protected:
    void _update_alive(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) final;

    [[nodiscard]] bool _update_dead(const btn::fixed_point& hero_position) final;

    void _show_damage_palette(const btn::sprite_palette_ptr& damage_palette) final;

    void _hide_damage_palette() final;

private:
    btn::vector<btn::fixed_rect, 2> _tank_rects;
    btn::vector<btn::sprite_ptr, 10> _footprint_sprites;
    btn::vector<btn::sprite_ptr, 9> _base_sprites;
    btn::optional<btn::sprite_ptr> _jelly_sprite;
    btn::optional<btn::sprite_ptr> _cannon_sprite;
    btn::optional<btn::sprite_cached_animate_action<2>> _jelly_animate_action;
    btn::sprite_palette_ptr _base_palette;
    btn::sprite_palette_ptr _jelly_palette;
    btn::sprite_palette_ptr _cannon_palette;
    btn::fixed _y;
    int _vibration_counter = 1;
    int _jelly_damage_counter = 1;
    int _footprint_graphics_index = 0;
    int _footprint_counter = 1;
    bool _jelly_crying = false;

    [[nodiscard]] btn::fixed _calculate_y();

    void _update_footprint_sprites(btn::fixed y);

    void _update_base_sprites(btn::fixed y);

    void _update_jelly_sprite(btn::fixed y, const btn::fixed_point& hero_position);

    void _update_cannon_sprite(btn::fixed y, const btn::fixed_point& hero_position);

    void _update_rects(btn::fixed y);
};

}

#endif
