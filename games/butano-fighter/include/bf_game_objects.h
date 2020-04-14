#ifndef BF_GAME_OBJECTS_H
#define BF_GAME_OBJECTS_H

#include "btn_deque.h"
#include "bf_game_gem.h"
#include "bf_constants.h"
#include "bf_game_object.h"
#include "bf_game_object_message.h"

namespace bf::game
{

class objects
{

public:
    explicit objects(const btn::sprite_palette_ptr& flash_palette);

    [[nodiscard]] bool check_hero_weapon(const btn::fixed_rect& hero_rect);

    [[nodiscard]] bool check_hero_bomb(const btn::fixed_rect& hero_rect, bool max_hero_bombs);

    [[nodiscard]] int check_gem(const btn::fixed_rect& hero_rect, int hero_level);

    void spawn_hero_weapon(const btn::fixed_point& position, int hero_level);

    void spawn_hero_bomb(const btn::fixed_point& position);

    void spawn_gem(const btn::fixed_point& position);

    void update();

private:
    btn::sprite_palette_ptr _flash_palette;
    btn::sprite_palette_ptr _gem_palette;
    btn::optional<object> _hero_weapon;
    btn::optional<object> _hero_bomb;
    btn::array<btn::sprite_tiles_ptr, 4> _gem_tiles;
    btn::vector<gem, constants::max_gems> _gems;
    btn::deque<object_message, constants::max_object_messages> _messages;
    bool _gem_check_odds = false;
};

}

#endif
