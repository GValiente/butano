#ifndef BF_GAME_ENEMY_BULLETS_H
#define BF_GAME_ENEMY_BULLETS_H

#include "btn_forward_list.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_palette_actions.h"
#include "bf_constants.h"
#include "bf_game_enemy_bullet_type.h"

namespace btn
{
    class fixed_rect;
}

namespace bf::game
{

class enemy_bullet_event;

class enemy_bullets
{

public:
    enemy_bullets();

    [[nodiscard]] bool check_hero(const btn::fixed_rect& hero_rect);

    void add_bullet(const btn::fixed_point& hero_position, const btn::fixed_point& enemy_position,
                    const enemy_bullet_event& event, const btn::camera_ptr& camera);

    void clear();

    void update();

private:
    struct bullet_type
    {
        btn::sprite_move_by_action sprite_move_action;
        btn::optional<btn::sprite_rotate_by_action> sprite_rotate_action;
        enemy_bullet_type type;
    };

    btn::sprite_palette_fade_loop_action _palette_fade_action;
    btn::array<btn::sprite_tiles_ptr, 2> _tiles_list;
    btn::forward_list<bullet_type, constants::max_enemy_bullets / 2> _even_bullets;
    btn::forward_list<bullet_type, constants::max_enemy_bullets / 2> _odd_bullets;
    bool _check_odds = false;
};

}

#endif
