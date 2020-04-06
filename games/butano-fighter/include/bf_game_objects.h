#ifndef BF_GAME_OBJECTS_H
#define BF_GAME_OBJECTS_H

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

    void spawn_hero_weapon(const btn::fixed_point& position, int hero_level);

    void spawn_hero_bomb(const btn::fixed_point& position);

    void update();

private:
    btn::sprite_palette_ptr _flash_palette;
    btn::optional<object> _hero_weapon;
    btn::optional<object> _hero_bomb;
    btn::vector<object_message, constants::max_object_messages> _messages;
};

}

#endif
