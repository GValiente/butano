/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_OBJECTS_H
#define BF_GAME_OBJECTS_H

#include "bn_deque.h"
#include "bn_forward_list.h"
#include "bf_game_gem.h"
#include "bf_constants.h"
#include "bf_game_object.h"
#include "bf_game_object_message.h"

namespace bf::game
{

class objects
{

public:
    struct bomb_check_result
    {
        int experience_to_add = 0;
        bool add_bomb = false;
    };

    explicit objects(const bn::sprite_palette_ptr& flash_palette);

    [[nodiscard]] bool check_hero_weapon(const bn::fixed_rect& hero_rect, const bn::camera_ptr& camera);

    [[nodiscard]] bomb_check_result check_hero_bomb(const bn::fixed_rect& hero_rect, bool max_hero_bombs,
                                                    int hero_level, const bn::camera_ptr& camera);

    [[nodiscard]] int check_gem(const bn::fixed_rect& hero_rect, int hero_level, const bn::camera_ptr& camera);

    void spawn_hero_weapon_with_sound(const bn::fixed_point& position, int hero_level,
                                      const bn::camera_ptr& camera);

    void spawn_hero_weapon_without_sound(const bn::fixed_point& position, int hero_level,
                                         const bn::camera_ptr& camera);

    void spawn_hero_bomb_with_sound(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void spawn_hero_bomb_without_sound(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void spawn_gem(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update();

private:
    bn::sprite_palette_ptr _flash_palette;
    bn::sprite_palette_ptr _gem_palette;
    bn::optional<object> _hero_weapon;
    bn::optional<object> _hero_bomb;
    bn::array<bn::sprite_tiles_ptr, 4> _gem_tiles;
    bn::forward_list<gem, constants::max_gems> _gems;
    bn::deque<object_message, constants::max_object_messages> _messages;
};

}

#endif
