#ifndef BF_GAME_HERO_WEAPON_OBJECT_H
#define BF_GAME_HERO_WEAPON_OBJECT_H

#include "btn_sprite_ptr.h"
#include "btn_fixed_point.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{
    class fixed_rect;
}

namespace bf::game
{

class hero_weapon_object
{

public:
    hero_weapon_object(const btn::fixed_point& position, int hero_level, const btn::sprite_palette_ptr& flash_palette);

    [[nodiscard]] bool intersects_hero(const btn::fixed_rect& hero_rect) const;

    void update();

private:
    btn::fixed_point _position;
    btn::fixed_point _delta_position;
    btn::sprite_ptr _sprite;
    btn::sprite_palette_ptr _sprite_palette;
    btn::sprite_palette_ptr _flash_palette;
    int _flash_palette_counter;
};

}

#endif
